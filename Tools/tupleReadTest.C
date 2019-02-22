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
      
            printf("MET_pt: %10f MET_phi: %10f nJet: %10d Jet_pt: %10d Stop0l_nHOT: %-10d Stop0l_HOTtype: %10d\n",
                    tr.getVar<float>("MET_pt"),
                    tr.getVar<float>("MET_phi"),
                    tr.getVar<unsigned int>("nJet"),
                    tr.getVec<float>("Jet_pt").size(),
                    tr.getVar<int>("Stop0l_nHOT"),
                    tr.getVec<int>("Stop0l_HOTtype").size()
                    );
            
            //std::cout << "MET_pt: "         << tr.getVar<float>("MET_pt") << " ";
            //std::cout << "MET_phi: "        << tr.getVar<float>("MET_phi") << " ";
            //std::cout << "Jet_pt: "         << tr.getVec<float>("Jet_pt").size() << " ";
            //std::cout << "Stop0l_nHOT: "    << tr.getVar<int>("Stop0l_nHOT") << " ";
            //std::cout << "Stop0l_HOTtype: " << tr.getVec<int>("Stop0l_HOTtype").size() << " ";
            //std::cout << std::endl;
            
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

