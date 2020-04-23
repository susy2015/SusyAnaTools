#include "NTupleReader.h"
#include "TopWeightCalculator.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>


int main()
{
    char nBase[] = "root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_11Apr2019_fastsimv5_v6p1/SMS_T2tt_mStop_400to1200_fastsim_2016/SMS_T2tt_mStop1000_mLSP0_fastsim_2016_Skim_070417_0.root";

    TChain *ch = new TChain("Events");

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }

    try
    {
        NTupleReader tr(ch, {"nJet"});

        TopWeightCalculator twc("TopTaggerCfg-DeepResolved_DeepCSV_GR_nanoAOD_2016_v1.0.6/tTagEff_2016.root", "SMS_T2tt_mStop1000_mLSP0_fastsim_2016", "2016");
        tr.registerFunction(twc);

        while(tr.getNextEvent())
        {
            //auto& FJ_ltv      = tr.getVec_LVFromNano<float>("FatJet");
            //auto& FJ_genMatch = tr.getVec<int>("FatJet_GenMatch");
            //auto& FJ_Stop0l   = tr.getVec<int>("FatJet_Stop0l");
            //
            //auto weightMap = twc.getTopWWeight(FJ_ltv, FJ_genMatch, FJ_Stop0l);
            //
            //for(const auto& p : weightMap) std::cout << p.first << ": " << p.second << std::endl;
            std::cout << "weight: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc") << std::endl;
            std::cout << "weight_up: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_up") << std::endl;
            std::cout << "weight_dn: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_dn") << std::endl;
            std::cout << std::endl;

            
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

