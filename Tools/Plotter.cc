#include "Plotter.h"

#include "TROOT.h"
#include "TLegend.h"
#include "TCanvas.h"

Plotter::Plotter(std::vector<HistSummary> h, std::vector<std::vector<FileSummary>> t)
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
}

Plotter::Cuttable::Cuttable(std::string c)
{
    setCuts(c);
}

Plotter::HistSummary::HistSummary(std::string l, std::string n, std::string cuts, int nb, double ll, double ul, bool log, std::string xal, std::string yal) : Cuttable(cuts) 
{
    label = l;
    name = n;
    nBins = nb;
    low = ll;
    high = ul;
    isLog = log;
    xAxisLabel = xal;
    yAxisLabel = yal;
}

Plotter::FileSummary::FileSummary(std::string lab, std::string nam, std::string tree, std::string cuts, double xs, double l, double k, double n) : Cuttable(cuts)
{
    label = lab;
    name = nam;
    treePath = tree;
    xsec = xs;
    lumi = l;
    kfactor = k;
    nEvts = n;
}


void Plotter::createHistsFromTuple()
{
    TH1::AddDirectory(false);

    for(auto tree : trees_)
    {    
        for(HistSummary& hist : hists_)
        {
            hist.histVec.push_back(std::make_pair(tree.front().label, new TH1D(hist.name.c_str(), hist.name.c_str(), hist.nBins, hist.low, hist.high)));
            if(hist.hist())
            {
                hist.hist()->GetXaxis()->SetTitle(hist.xAxisLabel.c_str());
                hist.hist()->GetYaxis()->SetTitle(hist.yAxisLabel.c_str());
            }
            else
            {
                hist.histVec.pop_back();
            }
        }

        for(FileSummary& file : tree)
        {
            TFile *f = new TFile(file.name.c_str());
            TTree *t = (TTree*)f->Get(file.treePath.c_str());

            NTupleReader tr(t);

            while(tr.getNextEvent())
            {
                // tree level dynamical cuts are applied here
                if(!file.passCuts(tr)) continue;
                
                for(HistSummary& hist : hists_)
                {
                    //parse hist level cuts here
                    if(!hist.passCuts(tr)) continue;

                    //fill histograms here 
                    double weight = tr.getVar<double>("evtWeight") * file.xsec * file.lumi * file.kfactor / file.nEvts;
                    hist.hist()->Fill(tr.getVar<double>(hist.name));
                }
            }
        }
    }
}

void Plotter::Cuttable::setCuts(std::string c)
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
        if((sepPos = tmp.find('>')) != size_t(-1)) cutType = '>';
        else if((sepPos = tmp.find('<')) != size_t(-1)) cutType = '<';
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
    if (type == '<') return tr.getVar<double>(name) < val;
    else if(type == '>') return tr.getVar<double>(name) > val;
    else if(type == '-') return tr.getVar<double>(name) > val && tr.getVar<double>(name) < val2;
    else printf("Unrecognized cut type, %c\n", type);
    return false;
}

bool Plotter::Cuttable::passCuts(const NTupleReader& tr) const
{
    bool passCut = true;
    for(const Cut& cut : cutVec_)
    {
        passCut = passCut && cut.passCut(tr);
    }
    return passCut;
}

void Plotter::plot()
{
    gROOT->SetStyle("Plain");
    
    for(HistSummary& hist : hists_)
    {
        TCanvas *c = new TCanvas("c1", "c1", 800, 800);
        c->cd();
        c->SetMargin(0.15, 0.1, 0.1, 0.1);

        TH1 *dummy = new TH1F("dummy", "dummy", 1000, hist.hist()->GetBinLowEdge(1), hist.hist()->GetBinLowEdge(hist.hist()->GetNbinsX()) + hist.hist()->GetBinWidth(hist.hist()->GetNbinsX()));
        dummy->SetStats(0);
        dummy->GetYaxis()->SetTitle(hist.yAxisLabel.c_str());
        dummy->GetYaxis()->SetTitleOffset(1.1);
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
        for(std::pair<std::string, TH1*>& h : hist.histVec)
        {
            leg->AddEntry(h.second, h.first.c_str());
            max = std::max(max, h.second->GetMaximum());
            min = std::min(min, h.second->GetMaximum());
        }
        
        c->SetLogy(hist.isLog);
        if(hist.isLog)
        {
            dummy->GetYaxis()->SetRangeUser(std::max(0.0001, 0.2 * min), 3*max);
        }
        else
        {
            dummy->GetYaxis()->SetRangeUser(0.0, max*1.2);
        }
        dummy->Draw();

        for(std::pair<std::string, TH1*>& h : hist.histVec)
        {
            h.second->Draw("same");
        }
        leg->Draw();

        c->Print((hist.label+".png").c_str());

        leg->~TLegend();
        dummy->~TH1();
        c->~TCanvas();
    }    
}
