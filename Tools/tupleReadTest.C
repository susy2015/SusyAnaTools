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
    char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_10Feb2019_v1//Data_SingleMuon_2016/Data_SingleMuon_2016_0.root";

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

        while(tr.getNextEvent())
        {
            if(tr.getEvtNum() == 1)
            {
                tr.printTupleMembers();
                FILE * fout = fopen("NTupleTypes.txt", "w");
                tr.printTupleMembers(fout);
                fclose(fout);
            }
      
            int njets = tr.getVar<unsigned int>("nJet");

            printf("MET_pt: %10f MET_phi: %10f nJet: %10d Jet_pt: %10d Stop0l_nHOT: %-10d Stop0l_HOTtype: %10d\n",
                    tr.getVar<float>("MET_pt"),
                    tr.getVar<float>("MET_phi"),
                    njets,
                    tr.getVec<float>("Jet_pt").size(),
                    tr.getVar<int>("Stop0l_nHOT"),
                    tr.getVec<int>("Stop0l_HOTtype").size()
                    );
            
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

