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
    //char nBase[] = "root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_11Apr2019_fastsimv5_v6p1/SMS_T2tt_mStop_400to1200_fastsim_2016/SMS_T2tt_mStop1000_mLSP0_fastsim_2016_Skim_070417_0.root";
    char nBase[] = "../../PhysicsTools/NanoSUSYTools/python/processors/SMS_T2tt_mStop1000_mLSP0_fastsim_2016_Skim_241609.root";

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

        //For NTupleReader users you simply need to register the class with NTupleReader 
        // the weight calculater requires three inputs
        //  - the top tag efficienc input file - this is found in the nanoAOD tagger releases (last 3 lines here https://github.com/susy2015/NanoSUSY-tools/blob/master/README.md#set-up-nanosusytools-framework)
        //  - the sampleSet name of the smaple being run over
        //  - the year of MC being run over 
        TopWeightCalculator twc("TopTaggerCfg-DeepResolved_DeepCSV_GR_nanoAOD_2016_v1.0.6/tTagEff_2016.root", "SMS_T2tt_mStop1000_mLSP0_fastsim_2016", "2016");
        tr.registerFunction(twc);

        // For non NTupleReader users the class is instantiated in the same way 
        TopWeightCalculator twc_nontr("TopTaggerCfg-DeepResolved_DeepCSV_GR_nanoAOD_2016_v1.0.6/tTagEff_2016.root", "SMS_T2tt_mStop1000_mLSP0_fastsim_2016", "2016");


        while(tr.getNextEvent())
        {
            //If using NTupleReader you can get the variables like this 
            std::cout << "weight:    " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc") << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight") << std::endl;
            std::cout << "weight_up: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_up")  << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_total_up") << std::endl;
            std::cout << "weight_dn: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_dn")  << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_total_dn") << std::endl;
            std::cout << "weight_t_up: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_up")  << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_top_up") << std::endl;
            std::cout << "weight_t_dn: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_dn")  << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_top_dn") << std::endl;
            std::cout << "weight_w_up: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_up")    << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_w_up") << std::endl;
            std::cout << "weight_w_dn: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_dn")    << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_w_dn") << std::endl;
            std::cout << "weight_v_up: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_up") << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_veto_up") << std::endl;
            std::cout << "weight_v_dn: " << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_dn") << "\t" << tr.getVar<float>("Stop0l_DeepAK8_SFWeight_veto_dn") << std::endl;
            std::cout << std::endl;


//            //If you are not using NTupleReader you can access the weights from the class as shown below
//            auto& FJ_tlv      = tr.getVec_LVFromNano<float>("FatJet");
//            auto& FJ_genMatch = tr.getVec<int>("FatJet_GenMatch");
//            auto& FJ_Stop0l   = tr.getVec<int>("FatJet_Stop0l");
//            
//            //gets a map of all necessary weights 
//            auto weightMap = twc_nontr.getTopWWeight(FJ_tlv, FJ_genMatch, FJ_Stop0l);
//            
//            for(const auto& p : weightMap) std::cout << p.first << ": " << p.second << std::endl;
//            std::cout << std::endl;
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

