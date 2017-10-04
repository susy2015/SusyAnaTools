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
    char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Spring16_80X_Nov_2016_Ntp_v11X_new_IDs/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/Spring16_80X_Nov_2016_Ntp_v11p0_new_IDs_TTJets_SingleLeptFromT/161111_230825/0000/stopFlatNtuples_%i.root";


    TChain *ch = new TChain("stopTreeMaker/AUX");

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }

    NTupleReader tr(ch);
    BaselineVessel blv(tr);
    tr.registerFunction(blv);

    while(tr.getNextEvent())
    {
        if(tr.getEvtNum() == 1)
        {
            tr.printTupleMembers();
            FILE * fout = fopen("NTupleTypes.txt", "w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }
      
      std::cout << "MET " << tr.getVar<double>("met")  << " nTop" << tr.getVar<int>("nTopCandSortedCnt") << std::endl;
    }

    return 0;
}

