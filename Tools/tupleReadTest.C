#include "NTupleReader.h"
#include "baselineDef.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>


int main()
{
    //char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Spring16_80X_Nov_2016_Ntp_v11X_new_IDs/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/Spring16_80X_Nov_2016_Ntp_v11p0_new_IDs_TTJets_SingleLeptFromT/161111_230825/0000/stopFlatNtuples_%i.root";
    char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_10Feb2019_v1//Data_SingleMuon_2016/Data_SingleMuon_2016_0.root";
    // "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_10Feb2019_v1//Data_SingleMuon_2016/Data_SingleMuon_2016_0.root"
    // "root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_80X_v2_NanAOD_MC/PostProcess_v1//GJets_HT-100To200/GJets_HT-100To200_0.root"

    //TChain *ch = new TChain("stopTreeMaker/AUX");
    TChain *ch = new TChain("Events");

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }

    try
    {
        NTupleReader tr(ch);
        //tr.addAlias("met", "aliasedMET");
        //tr.addAlias("jetsLVec", "aliasedJetsLVec");
        //BaselineVessel blv(tr);
        //tr.registerFunction(blv);

        while(tr.getNextEvent())
        {
            // if(tr.getEvtNum() == 1)
            // {
            //     //tr.printTupleMembers();
            //     //FILE * fout = fopen("NTupleTypes.txt", "w");
            //     //tr.printTupleMembers(fout);
            //     //fclose(fout);
            // }
            // break;
      
            // //std::cout << "MET " << tr.getVar<double>("met")  << " nTop" << tr.getVar<int>("nTopCandSortedCnt") << std::endl;
            // //std::cout << "MET " << tr.getVar<double>("met")  << " aliasedMET " << tr.getVar<double>("aliasedMET");
            // //std::cout << "Nj " << tr.getVec<TLorentzVector>("jetsLVec").size() << " aliasedNj " << tr.getVec<TLorentzVector>("aliasedJetsLVec").size() << std::endl;
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

