#include "Plotter.h"
#include "derivedTupleVariables.h"
#include "baselineDef.h"
#include "tdrstyle.h"

#include "TROOT.h"
#include "TCanvas.h"
#include "TTree.h"
//#include "TChain.h"
#include "TF1.h"
#include "THStack.h"
#include "TLatex.h"

const int colors[] = {
    kRed,
    kBlue,
    kGreen-2,
    kBlack,
    kOrange,
    kCyan,
    kMagenta,
    kYellow,
    kGreen
};
const int NCOLORS = sizeof(colors)/sizeof(int);

const int stackColors[] = {
    kAzure,
    kOrange + 7,
    kGreen + 2,
    kYellow + 4,
    kMagenta - 1,
    kRed,
    kBlue,
    kGreen
};
const int NSTACKCOLORS = sizeof(stackColors) / sizeof(int);

Plotter::Plotter(std::vector<HistSummary>& h, std::set<AnaSamples::FileSummary>& t, const bool readFromTuple, std::string ofname, const int nFile, const int startFile, const int nEvts) : nFile_(nFile), startFile_(startFile), maxEvts_(nEvts)
{
    TH1::AddDirectory(false);

    AnaFunctions::prepareTopTagger();

    hists_ = h;
    trees_ = t;
    readFromTuple_ = readFromTuple;
    if(ofname.size() == 0) ofname = "histoutput.root";
    if(readFromTuple) 
    {
        fout_ = new TFile(ofname.c_str(), "RECREATE");
        createHistsFromTuple();
    }
    else
    {
        fout_ = new TFile(ofname.c_str());
        createHistsFromFile();
    }
}

Plotter::~Plotter()
{
    if(fout_)
    {
        fout_->Close();
        delete fout_;
    }
}

Plotter::Cut::Cut(std::string s, char t, bool inv, double v, double v2)
{
    name = s;
    type = t;
    inverted = inv;
    val = v;
    val2 = v2;
    parseName();
}

void Plotter::Cut::parseName()
{
    size_t b1 = 0, b2 = 0;
    if((b1 = name.find("(")) != std::string::npos && (b2 = name.find(")")) != std::string::npos)
    {
        vecVar = name.substr(b1+1, b2 - b1 - 1);
        name = name.substr(0, b1);
    }
    else vecVar = "";
}

Plotter::Cuttable::Cuttable(const std::string& c)
{
    setCuts(c);
}

void Plotter::Cuttable::extractCuts(std::set<std::string>& ab) const
{
    for(auto& cut : cutVec_) ab.insert(cut.name);
}

Plotter::HistSummary::HistSummary(std::string l, std::vector<Plotter::DataCollection> ns, std::pair<int, int> ratio, std::string cuts, int nb, double ll, double ul, bool log, bool norm, std::string xal, std::string yal) : Cuttable(cuts), name(l), nBins(nb), low(ll), high(ul), isLog(log), isNorm(norm), xAxisLabel(xal), yAxisLabel(yal), ratio(ratio)
{
    parseName(ns);
}

void Plotter::HistSummary::parseName(std::vector<Plotter::DataCollection>& ns)
{
    for(auto& n : ns)
    {
        std::vector<std::shared_ptr<HistCutSummary>> tmphtp;
        for(auto& dataset : n.datasets)
        {
            size_t b1 = 0, b2 = 0;
            std::string hname, hvecvar;
            if((b1 = dataset.first.find("(")) != std::string::npos && (b2 = dataset.first.find(")")) != std::string::npos)
            {
                hvecvar = dataset.first.substr(b1+1, b2 - b1 - 1);
                hname   = dataset.first.substr(0, b1);
            }
            else 
            {
                hname = dataset.first;
                hvecvar = "";
            }

            std::string histname = name + hname + hvecvar + dataset.first + dataset.second.label + n.type;
            
            tmphtp.push_back(std::shared_ptr<HistCutSummary>(new HistCutSummary(dataset.second.label, histname, std::make_pair(hname, hvecvar), nullptr, dataset.second)));
        }
        hists.push_back(HistVecAndType(tmphtp, n.type));
    }
}

Plotter::HistSummary::~HistSummary()
{
}

Plotter::HistCutSummary::~HistCutSummary()
{
    if(h) delete h;
}

Plotter::DatasetSummary::DatasetSummary(std::string lab, std::vector<AnaSamples::FileSummary>& f, std::string cuts, std::string weights, double k) : Cuttable(cuts), label(lab), files(f), kfactor(k), weightStr(weights)
{
    parseWeights();
}

void Plotter::DatasetSummary::parseWeights()
{
    //parse which weights to use here.  
    for(size_t pos = 0, npos = 0; npos < weightStr.size(); pos = npos + 1)
    {
        npos = weightStr.find(';', pos + 1);
        if(npos == size_t(-1)) npos = weightStr.size();
        std::string tmp = weightStr.substr(pos, npos - pos);
     
        weightVec_.push_back(tmp);
    }
}

double Plotter::DatasetSummary::getWeight(const NTupleReader& tr) const
{
    double retval = 1.0;
    for(auto& weight : weightVec_)
    {
        if(tr.getVar<double>(weight) > 25) std::cout << weight << "\t" << tr.getVar<double>(weight) << std::endl;
        retval *= tr.getVar<double>(weight);
    }
    return retval;
}

double Plotter::DatasetSummary::extractWeightNames(std::set<std::string>& ab) const
{
    for(auto& w : weightVec_) ab.insert(w);
}

Plotter::DataCollection::DataCollection(std::string type, std::string var, std::vector<DatasetSummary> ds) : type(type)
{
    for(auto& dataset : ds)
    {
        datasets.push_back(std::make_pair(var, dataset));
    }
}

void Plotter::createHistsFromTuple()
{
    for(const AnaSamples::FileSummary& file : trees_)
    {
        std::set<std::string> activeBranches;
        
        //make vector of hists to fill
        std::vector<std::shared_ptr<HistCutSummary>> histsToFill;
        for(HistSummary& hs : hists_)
        {
            for(HistVecAndType& histvec : hs.hists)
            {
                for(std::shared_ptr<HistCutSummary>& hist : histvec.hcsVec)
                {
                    //annoying hack to fix vector pointer issue
                    if(hist->hs == nullptr) hist->hs = &hs;

                    // Make histogram if it is blank
                    if(hist->h == nullptr)
                    {
                        hist->h = new TH1D(hist->name.c_str(), hist->variable.first.c_str(), hs.nBins, hs.low, hs.high);
                        hist->h->Sumw2();
                        hist->h->GetXaxis()->SetTitle(hs.xAxisLabel.c_str());
                        hist->h->GetYaxis()->SetTitle(hs.yAxisLabel.c_str());
                    }

                    //hist->dss.extractCuts(activeBranches);
                    //hist->dss.extractWeightNames(activeBranches);
                    //hist->hs->extractCuts(activeBranches);

                    for(AnaSamples::FileSummary& fileToComp : hist->dss.files)
                    {
                        if(file == fileToComp)
                        {
                            histsToFill.push_back(hist);
                        }
                    }
                }
            } 
        }

        //TChain *t = new TChain(file.treePath.c_str());
        //file.addFilesToChain(t);
        std::cout << "Processing file(s): " << file.filePath << std::endl;

        int fileCount = 0, startCount = 0;
        for(const std::string& fname : file.filelist_)
        {
            if(startCount++ < startFile_) continue;
            if(nFile_ > 0 && fileCount++ >= nFile_) break;
            TFile *f = TFile::Open(fname.c_str());

            if(!f)
            {
                std::cout << "File \"" << fname << "\" not found!!!!!!" << std::endl;
                continue;
            }

            TTree *t = (TTree*)f->Get(file.treePath.c_str());

            if(!t)
            {
                std::cout << "Tree \"" << file.treePath << "\" not found in file \"" << fname << "\"!!!!!!" << std::endl;
                continue;
            }

            std::cout << "\t" << fname << std::endl;

            plotterFunctions::activateBranches(activeBranches);

            NTupleReader tr(t, activeBranches);
            tr.registerFunction(&passBaselineFunc);
            plotterFunctions::registerFunctions(tr);

            while(tr.getNextEvent())
            {
                if(maxEvts_ > 0 && tr.getEvtNum() > maxEvts_) break;
                if(tr.getEvtNum() %1000 == 0) std::cout << "Event #: " << tr.getEvtNum() << std::endl;
                for(auto& hist : histsToFill)
                {
                    // tree level dynamical cuts are applied here
                    if(!hist->dss.passCuts(tr)) continue;

                    // parse hist level cuts here
                    if(!hist->hs->passCuts(tr)) continue;

                    //fill histograms here 
                    double weight = file.getWeight() * hist->dss.getWeight(tr) * hist->dss.kfactor;

                    fillHist(hist->h, hist->variable, tr, weight);
                }
            }
            f->Close();
        }
    }
}

void Plotter::createHistsFromFile()
{
    for(auto& hs : hists_)
    {
        for(auto& histvec : hs.hists)
        {
            for(auto& hist : histvec.hcsVec)
            {
                if(fout_) hist->h = static_cast<TH1*>(fout_->Get(hist->name.c_str()));
                else std::cout << "Input file \"" << fout_ << "\" not found!!!!" << std::endl;
                if(!hist->h) std::cout << "Histogram not found: \"" << hist->name << "\"!!!!!!" << std::endl;
            }
        } 
    }
}

void Plotter::Cuttable::setCuts(const std::string& c)
{
    cuts_ = c;
    cutVec_.clear();
    parseCutString();
}

void Plotter::Cuttable::parseCutString()
{
    for(size_t pos = 0, npos = 0; npos < cuts_.size(); pos = npos + 1)
    {
        npos = cuts_.find(';', pos + 1);
        if(npos == size_t(-1)) npos = cuts_.size();
        std::string tmp = cuts_.substr(pos, npos - pos);
        size_t sepPos = 0;
        char cutType = ' ';
        bool inverted = false;
        if((sepPos = tmp.find("!")) != size_t(-1))
        {
            inverted = true;
            tmp = tmp.substr(sepPos + 1, size_t(-1));
        }
        sepPos = 0;
        if(     (sepPos = tmp.find('>')) != size_t(-1)) cutType = '>';
        else if((sepPos = tmp.find('<')) != size_t(-1)) cutType = '<';
        else if((sepPos = tmp.find('=')) != size_t(-1)) cutType = '=';
        else 
        {
            cutType = 'B';
            cutVec_.push_back(Cut(tmp, cutType, inverted, 0.0));
            continue;
        }
        std::string t1 = tmp.substr(0, sepPos), t2 = tmp.substr(sepPos + 1, (size_t(-1)));
        t1.erase(remove(t1.begin(),t1.end(),' '),t1.end());
        t2.erase(remove(t2.begin(),t2.end(),' '),t2.end());
        char vname[32];
        sscanf(t1.c_str(), "%s", vname);
        tmp = vname;
        double cutvalue;
        sscanf(t2.c_str(), "%lf", &cutvalue);
        Cut tmpCut(tmp, cutType, inverted, cutvalue);
        cutVec_.push_back(tmpCut);
    }
}

bool Plotter::Cut::passCut(const NTupleReader& tr) const
{
    if     (type == '<') return translateVar(tr) < val;
    else if(type == '>') return translateVar(tr) > val;
    else if(type == '=') return translateVar(tr) == val;
    else if(type == '-') return translateVar(tr) > val && translateVar(tr) < val2;
    else if(type == 'B') return boolReturn(tr);
    else printf("Unrecognized cut type, %c\n", type);
    return false;
}

double Plotter::Cut::translateVar(const NTupleReader& tr) const 
{
    std::string varType;
    tr.getType(name, varType);
    
    if     (varType.find("double")       != std::string::npos) return tr.getVar<double>(name);
    else if(varType.find("unsigned int") != std::string::npos) return static_cast<double>(tr.getVar<unsigned int>(name));
    else if(varType.find("int")          != std::string::npos) return static_cast<double>(tr.getVar<int>(name));
}

bool Plotter::Cut::boolReturn(const NTupleReader& tr) const
{
    // Functions here

    // Booleans here
    return tr.getVar<bool>(name);
}

bool Plotter::Cuttable::passCuts(const NTupleReader& tr) const
{
    bool passCut = true;
    for(const Cut& cut : cutVec_)
    {
        if(cut.vecVar.size() == 0)
        {
            passCut = passCut && ((cut.inverted)?(!cut.passCut(tr)):(cut.passCut(tr)));
        }
    }
    return passCut;
}

void Plotter::saveHists()
{
    fout_->cd();

    for(HistSummary& hist : hists_)
    {
        for(auto& hvec : hist.hists)
        {
            if(readFromTuple_ && fout_)
            {         
                for(auto& h : hvec.hcsVec)
                {
                    h->h->Write();
                }
            }
        }
    }
}

void Plotter::setPlotDir(std::string plotDir)
{
    plotDir_ = plotDir + "/";
}

void Plotter::plot()
{
    //gROOT->SetStyle("Plain");
    setTDRStyle();

    for(HistSummary& hist : hists_)
    {
        bool skip = false;
        for(auto& hvec : hist.hists)  for(auto& h : hvec.hcsVec) if(!h->h) skip = true;
        if(skip) continue;

        bool showRatio = true;
        if(hist.ratio.first == hist.ratio.second || hist.ratio.first < 1 || hist.ratio.second < 1)
        {
            showRatio = false;
        }

        TCanvas *c;
        double fontScale;
        if(showRatio)
        {
            c = new TCanvas("c1", "c1", 800, 900);
            c->Divide(1, 2);
            c->cd(1);
            gPad->SetPad("p1", "p1", 0, 2.5 / 9.0, 1, 1, kWhite, 0, 0);
            gPad->SetBottomMargin(0.01);
            fontScale = 1.0;
        }
        else
        {
            c = new TCanvas("c1", "c1", 800, 800);
            c->Divide(1, 1);
            c->cd(1);
            gPad->SetPad("p1", "p1", 0, 0, 1, 1, kWhite, 0, 0);
            gPad->SetBottomMargin(0.12);
            fontScale = 6.5 / 8;
        }
        gPad->SetLeftMargin(0.15);
        gPad->SetRightMargin(0.06);
        gPad->SetTopMargin(0.06 * (8.0 / 6.5) * fontScale);

        TH1 *dummy = new TH1F("dummy", "dummy", 1000, hist.fhist()->GetBinLowEdge(1), hist.fhist()->GetBinLowEdge(hist.fhist()->GetNbinsX()) + hist.fhist()->GetBinWidth(hist.fhist()->GetNbinsX()));
        dummy->SetStats(0);
        dummy->SetTitle(0);
        dummy->GetYaxis()->SetTitle(hist.yAxisLabel.c_str());
        dummy->GetYaxis()->SetTitleOffset(1.1*1.05 / (fontScale));
        dummy->GetXaxis()->SetTitleOffset(1.05);
        if(showRatio) dummy->GetXaxis()->SetTitle("");
        else          dummy->GetXaxis()->SetTitle(hist.xAxisLabel.c_str());
        dummy->GetXaxis()->SetTitleSize(0.20 * 2 / 6.5 * fontScale);
        dummy->GetXaxis()->SetLabelSize(0.20 * 2 / 6.5 * fontScale);
        dummy->GetYaxis()->SetTitleSize(0.20 * 2 / 6.5 * fontScale);
        dummy->GetYaxis()->SetLabelSize(0.20 * 2 / 6.5 * fontScale);
        if(dummy->GetNdivisions() % 100 > 5) dummy->GetXaxis()->SetNdivisions(6, 5, 0);

        TLegend *leg = new TLegend(0.50, 0.70, 0.89, 0.88);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetLineWidth(1);
        leg->SetNColumns(1);
        leg->SetTextFont(42);

        double max = 0.0, lmax = 0.0, min = 1.0e300, minAvgWgt = 1.0e300;
        int iSingle = 0, iStack = 0, iRatio = 0;
        char legEntry[128];
        for(auto& hvec : hist.hists)
        {
            if(hvec.type.compare("data") == 0)
            {
                if(hvec.hcsVec.size())
                {
                    hvec.hcsVec.front()->h->SetLineColor(kBlack);
                    hvec.hcsVec.front()->h->SetLineWidth(3);
                    hvec.hcsVec.front()->h->SetMarkerColor(kBlack);
                    hvec.hcsVec.front()->h->SetMarkerStyle(20);
                    double integral = hvec.hcsVec.front()->h->Integral(0, hvec.hcsVec.front()->h->GetNbinsX() + 1);
                    if(     integral < 3.0)   sprintf(legEntry, "%s (%0.2lf)", hvec.hcsVec.front()->label.c_str(), integral);
                    else if(integral < 1.0e5) sprintf(legEntry, "%s (%0.0lf)", hvec.hcsVec.front()->label.c_str(), integral);
                    else                      sprintf(legEntry, "%s (%0.2e)",  hvec.hcsVec.front()->label.c_str(), integral);
                    leg->AddEntry(hvec.hcsVec.front()->h, legEntry, "PE");
                    if(hist.isNorm) hvec.hcsVec.front()->h->Scale(hist.fhist()->Integral()/hvec.hcsVec.front()->h->Integral());
                    smartMax(hvec.hcsVec.front()->h, leg, static_cast<TPad*>(gPad), min, max, lmax);

                    hvec.h = static_cast<TNamed*>(hvec.hcsVec.front()->h->Clone());
                }
            }
            else if(hvec.type.compare("single") == 0)
            {
                for(auto& h : hvec.hcsVec)
                {
                    h->h->SetLineColor(colors[iSingle%NCOLORS]);
                    h->h->SetLineWidth(3);
                    iSingle++;
                    double integral = h->h->Integral(0, h->h->GetNbinsX() + 1);
                    if(     integral < 3.0)   sprintf(legEntry, "%s (%0.2lf)", h->label.c_str(), integral);
                    else if(integral < 1.0e5) sprintf(legEntry, "%s (%0.0lf)", h->label.c_str(), integral);
                    else                      sprintf(legEntry, "%s (%0.2e)",  h->label.c_str(), integral);
                    leg->AddEntry(h->h, legEntry);
                    if(hist.isNorm) h->h->Scale(hist.fhist()->Integral()/h->h->Integral());
                    smartMax(h->h, leg, static_cast<TPad*>(gPad), min, max, lmax);
                    minAvgWgt = std::min(minAvgWgt, h->h->GetSumOfWeights()/h->h->GetEntries());
                }
            }
            else if(hvec.type.compare("ratio") == 0 && hvec.hcsVec.size() >= 2)
            {
                auto hIter = hvec.hcsVec.begin();
                TH1* hratio = static_cast<TH1*>((*hIter)->h->Clone());
                hratio->SetLineColor(colors[iRatio%NCOLORS]);
                hratio->SetLineWidth(3);
                ++iRatio;
                hvec.h = static_cast<TNamed*>(hratio);
                ++hIter;
                if(hist.isNorm) hratio->Scale((*hIter)->h->Integral()/hratio->Integral());
                hratio->Divide((*hIter)->h);
                leg->AddEntry(hratio, hvec.flabel().c_str());
                smartMax(hratio, leg, static_cast<TPad*>(gPad), min, max, lmax);
                minAvgWgt = std::min(minAvgWgt, 1.0);
            }
            else if(hvec.type.compare("stack") == 0)
            {
                THStack *stack = new THStack((hist.name + hvec.hcsVec.front()->label).c_str(), "stack");
                hvec.h = static_cast<TNamed*>(stack);

                double sow = 0, te = 0;
                bool firstHIS = true;
                TH1* thstacksucks;
                for(auto& h : hvec.hcsVec)
                {
                    if(firstHIS)
                    {
                        firstHIS = false;
                        thstacksucks = static_cast<TH1*>(h->h->Clone());
                    }
                    else
                    {
                        thstacksucks->Add(h->h);
                    }
                    h->h->SetLineColor(stackColors[iStack%NSTACKCOLORS]);
                    h->h->SetFillColor(stackColors[iStack%NSTACKCOLORS]);
                    iStack++;
                    stack->Add(h->h);
                    double integral = h->h->Integral(0, h->h->GetNbinsX() + 1);
                    if(     integral < 3.0)   sprintf(legEntry, "%s (%0.2lf)", h->label.c_str(), integral);
                    else if(integral < 1.0e5) sprintf(legEntry, "%s (%0.0lf)", h->label.c_str(), integral);
                    else                      sprintf(legEntry, "%s (%0.2e)",  h->label.c_str(), integral);
                    leg->AddEntry(h->h, legEntry);
                    sow += h->h->GetSumOfWeights();
                    te +=  h->h->GetEntries();
                }
                smartMax(thstacksucks, leg, static_cast<TPad*>(gPad), min, max, lmax);
                minAvgWgt = std::min(minAvgWgt, sow/te);
                if(thstacksucks) delete thstacksucks;
            }
        }
        
        gPad->SetLogy(hist.isLog);
        if(hist.isLog)
        {
            double locMin = std::min(0.2*minAvgWgt, std::max(0.0001, 0.05 * min));
            double legSpan = (log10(3*max) - log10(locMin)) * (leg->GetY1() - gPad->GetBottomMargin()) / ((1 - gPad->GetTopMargin()) - gPad->GetBottomMargin());
            double legMin = legSpan + log10(locMin);
            if(log10(lmax) > legMin) 
            {
                double scale = (log10(lmax) - log10(locMin)) / (legMin - log10(locMin));
                max = pow(max/locMin, scale)*locMin;
            }
            dummy->GetYaxis()->SetRangeUser(locMin, 3*max);
        }
        else
        {
            double locMin = 0.0;
            double legMin = (1.2*max - locMin) * (leg->GetY1() - gPad->GetBottomMargin()) / ((1 - gPad->GetTopMargin()) - gPad->GetBottomMargin());
            if(lmax > legMin) max *= (lmax - locMin)/(legMin - locMin);
            dummy->GetYaxis()->SetRangeUser(0.0, max*1.2);
            if(hist.hists.front().type.compare("ratio") == 0 && max > 5) dummy->GetYaxis()->SetRangeUser(0.0, 5*1.2);
        }
        dummy->Draw();

        for(auto& hvec : hist.hists)
        {
            if(!hvec.h)
            {
                for(auto& h : hvec.hcsVec)
                {
                    h->h->Draw("hist same");
                }
            }
            else
            {
                if(     hvec.type.compare("ratio") == 0) hvec.h->Draw("hist same E1");
                else if(hvec.type.compare("data") == 0)  hvec.h->Draw("same");
                else                                     hvec.h->Draw("hist same");
            }
        }
        leg->Draw();

        fixOverlay();

        TH1 *dummy2 = nullptr, *h1 = nullptr, *h2 = nullptr;
        if(showRatio)
        {
            c->cd(2);
            gPad->SetPad("p2", "p2", 0, 0, 1, 2.5 / 9.0, kWhite, 0, 0);
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.06);
            gPad->SetTopMargin(0.01);
            gPad->SetBottomMargin(0.37);

            dummy2 = new TH1F("dummy2", "dummy2", 1000, hist.fhist()->GetBinLowEdge(1), hist.fhist()->GetBinLowEdge(hist.fhist()->GetNbinsX()) + hist.fhist()->GetBinWidth(hist.fhist()->GetNbinsX()));
            dummy2->GetXaxis()->SetTitle(hist.xAxisLabel.c_str());
            dummy2->GetXaxis()->SetTitleOffset(1.05);
            dummy2->GetYaxis()->SetTitle("Ratio");
            dummy2->GetYaxis()->SetTitleOffset(0.42);
            dummy2->GetYaxis()->SetNdivisions(3, 5, 0, true);

            dummy2->GetYaxis()->SetTitleSize(0.16 * 2 / 2.5);
            dummy2->GetYaxis()->SetLabelSize(0.20 * 2 / 2.5);
            dummy2->GetXaxis()->SetTitleSize(0.20 * 2 / 2.5);
            dummy2->GetXaxis()->SetLabelSize(0.20 * 2 / 2.5);

            dummy2->SetStats(0);
            dummy2->SetTitle(0);
            if(dummy2->GetNdivisions() % 100 > 5) dummy2->GetXaxis()->SetNdivisions(6, 5, 0);

            int iHist = 1;
            if(hist.hists.size() == 1 && hist.hists.front().type.compare("single") == 0)
            {
                for(auto& h : hist.hists.front().hcsVec)
                {
                    if(     iHist == hist.ratio.first)  h1 = static_cast<TH1*>(h->h->Clone());
                    else if(iHist == hist.ratio.second) h2 = static_cast<TH1*>(h->h->Clone());
                    ++iHist;
                }
            }
            else
            {
                for(auto& hvec : hist.hists)
                {
                    if(hvec.type.compare("single") == 0 || hvec.type.compare("data") == 0)
                    {
                        if(iHist == hist.ratio.first)  h1 = static_cast<TH1*>(hvec.hcsVec.front()->h->Clone());
                        if(iHist == hist.ratio.second) h2 = static_cast<TH1*>(hvec.hcsVec.front()->h->Clone());
                    }
                    else if(hvec.type.compare("stack") == 0)
                    {
                        bool firstHIS = true;
                        TH1* thstacksucks = 0;
                        if(iHist == hist.ratio.first || iHist == hist.ratio.second)
                        {
                            for(auto& h : hvec.hcsVec)
                            {
                                if(firstHIS)
                                {
                                    firstHIS = false;
                                    thstacksucks = static_cast<TH1*>(h->h->Clone());
                                }
                                else
                                {
                                    thstacksucks->Add(h->h);
                                }
                            }
                            if(iHist == hist.ratio.first)  h1 = static_cast<TH1*>(thstacksucks->Clone());
                            if(iHist == hist.ratio.second) h2 = static_cast<TH1*>(thstacksucks->Clone());
                        }
                        if(thstacksucks) delete thstacksucks;
                    }
                    ++iHist;
                }
            }

            TF1 * fline = new TF1("line", "pol0", hist.fhist()->GetBinLowEdge(1), hist.fhist()->GetBinLowEdge(hist.fhist()->GetNbinsX()) + hist.fhist()->GetBinWidth(hist.fhist()->GetNbinsX()));
            fline->SetParameter(0, 1);

            if(h1 && h2)
            {
                fline->SetLineColor(h2->GetLineColor());

                h1->Divide(h2);
                //h1->SetLineColor(kBlack);
                double d2ymin = 0.0;
                double d2ymax = 1.5;
                for(int iBin = 1; iBin <= h1->GetNbinsX(); ++iBin)
                {
                    if(h1->GetBinContent(iBin) < 5.0)
                    {
                        d2ymax = std::max(d2ymax, h1->GetBinContent(iBin));
                    }
                }
                dummy2->GetYaxis()->SetRangeUser(d2ymin, 1.5*d2ymax);

                dummy2->Draw();
                fline->Draw("same");
                
                h1->Draw("same E1");

                fixOverlay();
            }
        }

        c->cd(1);
        char lumistamp[128];
        sprintf(lumistamp, "%.1f fb^{-1} at 13 TeV", AnaSamples::lumi / 1000.0);
        TLatex mark;
        mark.SetTextSize(0.042 * fontScale);
        mark.SetTextFont(42);
        mark.SetNDC(true);
        mark.DrawLatex(gPad->GetLeftMargin(), 0.95, "CMS Preliminary");
        mark.SetTextAlign(31);
        mark.DrawLatex(1 - gPad->GetRightMargin(), 0.95, lumistamp);

        fixOverlay();
        c->Print((plotDir_ + hist.name+".png").c_str());
        c->Print((plotDir_ + hist.name+".pdf").c_str());

        delete leg;
        delete dummy;
        if(dummy2) delete dummy2;
        delete c;
        if(h1) delete h1;
        if(h2) delete h2;
    }    
}

void Plotter::fillHist(TH1 * const h, const std::pair<std::string, std::string>& name, const NTupleReader& tr, const double weight)
{
    std::string type;
    tr.getType(name.first, type);

    if(type.find("vector") != std::string::npos)
    {
        if(type.find("*") != std::string::npos)
        {
            if     (type.find("TLorentzVector") != std::string::npos) fillHistFromVec<TLorentzVector*>(h, name, tr, weight);
        }
        else
        {
            if     (type.find("pair")           != std::string::npos) fillHistFromVec<std::pair<double, double> >(h, name, tr, weight);
            else if(type.find("double")         != std::string::npos) fillHistFromPrimVec<double>(h, name, tr, weight);
            else if(type.find("unsigned int")   != std::string::npos) fillHistFromPrimVec<unsigned int>(h, name, tr, weight);
            else if(type.find("int")            != std::string::npos) fillHistFromPrimVec<int>(h, name, tr, weight);
            else if(type.find("TLorentzVector") != std::string::npos) fillHistFromVec<TLorentzVector>(h, name, tr, weight);         
        }
    }
    else
    {
        if     (type.find("double")         != std::string::npos) h->Fill(tr.getVar<double>(name.first), weight);
        else if(type.find("unsigned int")   != std::string::npos) h->Fill(tr.getVar<unsigned int>(name.first), weight);
        else if(type.find("int")            != std::string::npos) h->Fill(tr.getVar<int>(name.first), weight);
//        else if(type.find("TLorentzVector") != std::string::npos) h->Fill(tlvGetValue(name.second, tr.getVar<TLorentzVector>(name.first)));
    }
}

template<> inline void Plotter::vectorFill(TH1 * const h, const std::pair<std::string, std::string>& name, const TLorentzVector& obj, const double weight)
{
    h->Fill(tlvGetValue(name.second, obj), weight);
}

template<> inline void Plotter::vectorFill(TH1 * const h, const std::pair<std::string, std::string>& name, const std::pair<double, double>& obj, const double weight)
{
    h->Fill(obj.first, obj.second * weight);
}

void Plotter::smartMax(const TH1* const h, const TLegend* const l, const TPad* const p, double& gmin, double& gmax, double& gpThreshMax) const
{
    const bool isLog = p->GetLogy();
    double min = 9e99;
    double max = -9e99;
    double pThreshMax = -9e99;
    int threshold = static_cast<int>(h->GetNbinsX()*(l->GetX1() - p->GetLeftMargin())/((1 - p->GetRightMargin()) - p->GetLeftMargin()));
    
    for(int i = 1; i <= h->GetNbinsX(); ++i)
    {
        double bin = h->GetBinContent(i);
        if(bin > max) max = bin;
        else if(bin > 1e-10 && bin < min) min = bin;
        if(i >= threshold && bin > pThreshMax) pThreshMax = bin;
    }

    gpThreshMax = std::max(gpThreshMax, pThreshMax);
    gmax = std::max(gmax, max);
    gmin = std::min(gmin, min);
}
