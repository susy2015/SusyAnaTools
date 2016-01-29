#include "NTupleReader.h"
#include "samples.h"
#include "baselineDef.h"
#include "searchBins.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TChain.h"

class HistContainer
{
private:
    TH1* met_;
    TH1* mt2_;
    TH1* nt_;
    TH1* nb_;
    TH1* baseline_met_;
    TH1* baseline_mt2_;
    TH1* baseline_nt_;
    TH1* baseline_nb_;
    TH1* baseline_nSearchBin_;
    
    int mMass_;
    int dMass_;
    
    void bookHists()
        {
            char hname[128];
            sprintf(hname, "%s_%d_%d", "met", mMass_, dMass_);
            met_ = new TH1D(hname, hname, 150, 0, 3000);
            sprintf(hname, "%s_%d_%d", "mt2", mMass_, dMass_);
            mt2_ = new TH1D(hname, hname, 150, 0, 3000);
            sprintf(hname, "%s_%d_%d", "nt", mMass_, dMass_);
            nt_  = new TH1D(hname, hname,  10, 0, 10);
            sprintf(hname, "%s_%d_%d", "nb", mMass_, dMass_);
            nb_  = new TH1D(hname, hname,  10, 0, 10);

            sprintf(hname, "%s_%d_%d", "baseline_met", mMass_, dMass_);
            baseline_met_ = new TH1D(hname, hname, 150, 0, 3000);
            sprintf(hname, "%s_%d_%d", "baseline_mt2", mMass_, dMass_);
            baseline_mt2_ = new TH1D(hname, hname, 150, 0, 3000);
            sprintf(hname, "%s_%d_%d", "baseline_nt", mMass_, dMass_);
            baseline_nt_  = new TH1D(hname, hname,  10, 0, 10);
            sprintf(hname, "%s_%d_%d", "baseline_nb", mMass_, dMass_);
            baseline_nb_  = new TH1D(hname, hname,  10, 0, 10);

            sprintf(hname, "%s_%d_%d", "baseline_nSearchBin", mMass_, dMass_);
            baseline_nSearchBin_  = new TH1D(hname, hname,  45, 0, 45);
        }
    
public:
    HistContainer(int mMass, int dMass) : mMass_(mMass), dMass_(dMass) 
        {
            bookHists();
        }

    void fill(const NTupleReader& tr, const double weight)
        {
            const int& nSearchBin            = tr.getVar<int>("nSearchBin");
            const double& met                = tr.getVar<double>("met");
            const double& best_had_brJet_MT2 = tr.getVar<double>("best_had_brJet_MT2");
            const int& cntCSVS               = tr.getVar<int>("cntCSVS");
            const int& nTopCandSortedCnt     = tr.getVar<int>("nTopCandSortedCnt");
            const bool& passBaseline         = tr.getVar<bool>("passBaseline");

            met_->Fill(met, weight);
            mt2_->Fill(best_had_brJet_MT2, weight);
            nt_->Fill(nTopCandSortedCnt, weight);
            nb_->Fill(cntCSVS, weight);

            if(passBaseline)
            {
                baseline_met_->Fill(met, weight);
                baseline_mt2_->Fill(best_had_brJet_MT2, weight);
                baseline_nt_->Fill(nTopCandSortedCnt, weight);
                baseline_nb_->Fill(cntCSVS, weight);

                baseline_nSearchBin_->Fill(nSearchBin, weight);
            }
        }

    void write()
        {
            met_->Write();
            mt2_->Write();
            nt_->Write();
            nb_->Write();
            baseline_met_->Write();
            baseline_mt2_->Write();
            baseline_nt_->Write();
            baseline_nb_->Write();
            baseline_nSearchBin_->Write();
        }
};

void calcSearchBin(NTupleReader& tr)
{
    const double& met                = tr.getVar<double>("met");
    const double& best_had_brJet_MT2 = tr.getVar<double>("best_had_brJet_MT2");
    const int& cntCSVS               = tr.getVar<int>("cntCSVS");
    const int& nTopCandSortedCnt     = tr.getVar<int>("nTopCandSortedCnt");

    int nSearchBin = find_Binning_Index(cntCSVS, nTopCandSortedCnt, best_had_brJet_MT2, met);

    tr.registerDerivedVar("nSearchBin", nSearchBin);
}

int main()
{
    TH1::AddDirectory(false);

    try
    {

        AnaSamples::SampleSet        ss(AnaSamples::fileDir, 2153.74);
        AnaSamples::SampleCollection sc(ss);

        std::set<std::string> activatedBranch;
        for(auto& branch : AnaConsts::activatedBranchNames_DataOnly) activatedBranch.insert(branch);
        for(auto& branch : AnaConsts::activatedBranchNames) activatedBranch.insert(branch);

        std::map<std::pair<int, int>, HistContainer> histVec;

        for(auto& fs : sc["Signal_fastsim_T2tt_scan"])
        {
            TChain * t = new TChain(fs.treePath.c_str());
            fs.addFilesToChain(t);

            BaselineVessel blv;

            NTupleReader tr(t, activatedBranch);
            tr.registerFunction(blv);
            tr.registerFunction(&calcSearchBin);

            while(tr.getNextEvent())
            {
                if(tr.getEvtNum() % 10000 == 0) std::cout << "Event #: " << tr.getEvtNum() << std::endl;

                const double& SusyMotherMass  = tr.getVar<double>("SusyMotherMass");
                const double& SusyLSPMass     = tr.getVar<double>("SusyLSPMass");

                std::pair<int, int> iMP((int)SusyMotherMass, (int)SusyLSPMass);

                auto iter = histVec.find(iMP);
                if(iter == histVec.end()) iter = histVec.emplace(iMP, HistContainer(iMP.first, iMP.second)).first;

                iter->second.fill(tr, fs.getWeight());
            }
        }

        TFile *f = TFile::Open("signalHists.root");
        f->cd();
        for(auto& p : histVec) p.second.write();
        f->Close();
    }
    catch(const std::string e)
    {
        std::cout << e << std::endl;
    }

}
