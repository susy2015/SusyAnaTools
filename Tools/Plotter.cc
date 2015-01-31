#include "Plotter.h"
#include "derivedTupleVariables.h"
#include "tdrstyle.h"

#include "TROOT.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TChain.h"

const int colors[] = {
    kRed,
    kBlue,
    kGreen-2,
    kBlack,
    kOrange,
    kYellow
};
const int NCOLORS = sizeof(colors)/sizeof(int);

Plotter::Plotter(std::vector<HistSummary>& h, std::vector<std::vector<FileSummary>>& t)
{
    hists_ = h;
    trees_ = t;
    createHistsFromTuple();
}

Plotter::Cut::Cut(std::string s, char t, double v, double v2)
{
    name = s;
    type = t;
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

void Plotter::Cuttable::extractCuts(std::set<std::string>& ab)
{
    for(auto& cut : cutVec_) ab.insert(cut.name);
}

Plotter::HistSummary::HistSummary(std::string l, std::vector<std::pair<Plotter::DataCollection, std::string>> ns, std::string cuts, int nb, double ll, double ul, bool log, bool norm, std::string xal, std::string yal) : Cuttable(cuts) 
{
    name = l;
    nBins = nb;
    low = ll;
    high = ul;
    isLog = log;
    isNorm = norm;
    xAxisLabel = xal;
    yAxisLabel = yal;

    parseName(ns);
}

void Plotter::HistSummary::parseName(std::vector<std::pair<Plotter::DataCollection, std::string>>& ns)
{

    for(auto& n : ns)
    {
        size_t b1 = 0, b2 = 0;
        //std::string hname = n.first.datasets.front().label + "_" + n.second;
        std::string hname, hvecvar;
        if((b1 = n.second.find("(")) != std::string::npos && (b2 = n.second.find(")")) != std::string::npos)
        {
            hvecvar  = n.second.substr(b1+1, b2 - b1 - 1);
            hname    = n.second.substr(0, b1);
        }
        else 
        {
            hname = n.second;
            hvecvar = "";
        }

        TH1 *h = new TH1D((name + hname + hvecvar + n.first.datasets.front().label).c_str(), hname.c_str(), nBins, low, high);
        h->GetXaxis()->SetTitle(xAxisLabel.c_str());
        h->GetYaxis()->SetTitle(yAxisLabel.c_str());
        hists.push_back(HistCutSummary(n.first.datasets.front().label, h, std::make_pair(hname, hvecvar), nullptr, n.first.datasets.front()));
    }
}

Plotter::FileSummary::FileSummary(std::string nam, std::string tp, double x, double l, double k, double n)
{
    name = nam;
    treePath = tp;
    xsec = x;
    lumi = l;
    kfactor = k;
    nEvts = n;
}

Plotter::DatasetSummary::DatasetSummary(std::string lab, std::vector<FileSummary>& f, std::string cuts, double k) : Cuttable(cuts)
{
    label = lab;
    files = f;
    kfactor = k;
}

void Plotter::createHistsFromTuple()
{
    TH1::AddDirectory(false);

    for(auto& fileList : trees_)
    {    
        //make vector of hists to fill
        std::vector<HistCutSummary*> histsToFill;
        for(auto& hs : hists_)
        {
            for(auto& hist : hs.hists)
            {
                if(hist.dss.files == fileList)
                {
                    //annoying hack to fix vector pointer issue
                    hist.hs = &hs;

                    histsToFill.push_back(&hist);
                }
            } 
        }

        for(FileSummary& file : fileList)
        {
            //TFile *f = new TFile(file.name.c_str());
            //TTree *t = (TTree*)f->Get(file.treePath.c_str());
            TChain *t = new TChain(file.treePath.c_str());
            t->Add(file.name.c_str());
            std::cout << "Processing file(s): " << file.name << std::endl;

            //file.extractCuts(activeBranches_);
            //plotterFunctions::activateBranches(activeBranches_);
            //activeBranches_.insert("evtWeight");

            NTupleReader tr(t, activeBranches_);
            plotterFunctions::registerFunctions(tr);

            while(tr.getNextEvent())
            {
                for(auto& hist : histsToFill)
                {
                    // tree level dynamical cuts are applied here
                    if(!hist->dss.passCuts(tr)) continue;

                    // parse hist level cuts here
                    if(!hist->hs->passCuts(tr)) continue;

                    //fill histograms here 
                    double weight = tr.getVar<double>("evtWeight") * file.xsec * file.lumi * file.kfactor * hist->dss.kfactor / file.nEvts;

                    fillHist(hist->h, hist->variable, tr, weight);
                }
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
        if(     (sepPos = tmp.find('>')) != size_t(-1)) cutType = '>';
        else if((sepPos = tmp.find('<')) != size_t(-1)) cutType = '<';
        else if((sepPos = tmp.find('=')) != size_t(-1)) cutType = '=';
        else continue;
        std::string t1 = tmp.substr(0, sepPos), t2 = tmp.substr(sepPos + 1, (size_t(-1)));
        t1.erase(remove(t1.begin(),t1.end(),' '),t1.end());
        t2.erase(remove(t2.begin(),t2.end(),' '),t2.end());
        char vname[32];
        sscanf(t1.c_str(), "%s", vname);
        tmp = vname;
        double cutvalue;
        sscanf(t2.c_str(), "%lf", &cutvalue);
        Cut tmpCut(tmp, cutType, cutvalue);
        cutVec_.push_back(tmpCut);
    }
}

bool Plotter::Cut::passCut(const NTupleReader& tr) const
{
    if     (type == '<') return translateVar(tr) < val;
    else if(type == '>') return translateVar(tr) > val;
    else if(type == '=') return translateVar(tr) == val;
    else if(type == '-') return translateVar(tr) > val && translateVar(tr) < val2;
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

bool Plotter::Cuttable::passCuts(const NTupleReader& tr) const
{
    bool passCut = true;
    for(const Cut& cut : cutVec_)
    {
        if(cut.vecVar.size() == 0)
        {
            passCut = passCut && cut.passCut(tr);
        }
    }
    return passCut;
}

void Plotter::plot()
{
    //gROOT->SetStyle("Plain");
    setTDRStyle();
    
    for(HistSummary& hist : hists_)
    {
        TCanvas *c = new TCanvas("c1", "c1", 800, 800);
        c->cd();
        c->SetMargin(0.15, 0.1, 0.1, 0.1);

        TH1 *dummy = new TH1F("dummy", "dummy", 1000, hist.fhist()->GetBinLowEdge(1), hist.fhist()->GetBinLowEdge(hist.fhist()->GetNbinsX()) + hist.fhist()->GetBinWidth(hist.fhist()->GetNbinsX()));
        dummy->SetStats(0);
        dummy->SetTitle(0);
        dummy->GetYaxis()->SetTitle(hist.yAxisLabel.c_str());
        dummy->GetYaxis()->SetTitleOffset(1.20);
        dummy->GetXaxis()->SetTitle(hist.xAxisLabel.c_str());
        dummy->GetXaxis()->SetTitleOffset(1.05);
        dummy->GetXaxis()->SetTitleSize(0.04);
        dummy->GetXaxis()->SetLabelSize(0.04);
        dummy->GetYaxis()->SetTitleSize(0.04);
        dummy->GetYaxis()->SetLabelSize(0.04);
        if(dummy->GetNdivisions() % 100 > 5) dummy->GetXaxis()->SetNdivisions(6, 5, 0);

        TLegend *leg = new TLegend(0.45, 0.61, 0.89, 0.91);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetLineWidth(1);
        leg->SetNColumns(1);
        leg->SetTextFont(42);

        double max = 0.0, min = 1.0e300;
        int i = 0;
        for(auto& h : hist.hists)
        {
            h.h->SetLineColor(colors[i%NCOLORS]);
            i++;
            leg->AddEntry(h.h, h.label.c_str());
            max = std::max(max, h.h->GetMaximum());
            min = std::min(min, h.h->GetMaximum());
        }
        
        c->SetLogy(hist.isLog);
        if(hist.isLog)
        {
            dummy->GetYaxis()->SetRangeUser(std::min(1.0, std::max(0.0001, 0.2 * min)), 3*max);
        }
        else
        {
            dummy->GetYaxis()->SetRangeUser(0.0, max*1.2);
        }
        dummy->Draw();

        for(auto& h : hist.hists)
        {
            if(hist.isNorm) h.h->Scale(hist.fhist()->Integral()/h.h->Integral());
            h.h->Draw("same");
        }
        leg->Draw();

        c->Print((hist.name+".png").c_str());

        leg->~TLegend();
        dummy->~TH1();
        c->~TCanvas();
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
            if     (type.find("double")         != std::string::npos) fillHistFromVec<double>(h, name, tr, weight);
            else if(type.find("unsigned int")   != std::string::npos) fillHistFromVec<unsigned int>(h, name, tr, weight);
            else if(type.find("int")            != std::string::npos) fillHistFromVec<int>(h, name, tr, weight);
            else if(type.find("TLorentzVector") != std::string::npos) fillHistFromVec<TLorentzVector>(h, name, tr, weight);
        }
    }
    else
    {
        if     (type.find("double")       != std::string::npos) h->Fill(tr.getVar<double>(name.first), weight);
        else if(type.find("unsigned int") != std::string::npos) h->Fill(tr.getVar<unsigned int>(name.first), weight);
        else if(type.find("int")          != std::string::npos) h->Fill(tr.getVar<int>(name.first), weight);
    }
}

template<> inline void Plotter::vectorFill(TH1 * const h, const std::pair<std::string, std::string>& name, const TLorentzVector& obj, const double weight)
{
    h->Fill(tlvGetValue(name.second, obj), weight);;
}
