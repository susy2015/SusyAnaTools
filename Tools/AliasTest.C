#include "NTupleReader.h"
#include "StopleAlias.h"
#include "baselineDef.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>


int main()
{
  char nBase[] = "root://cmseos.fnal.gov//store/user/mkilpatr/13TeV/120917/merged/met-2017b-23jun17_1_ntuple_postproc.root";

  TChain *ch = new TChain("Events");
  //TChain *ch = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }
    NTupleReader tr(ch);
    StopleAlias alias;
    tr.setReThrow(false);
    tr.registerFunction(alias);
    //std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
      if(tr.getEvtNum() == 1)
      {
        tr.printTupleMembers();
        FILE * fout = fopen("NewUCSBNTupleTypes.txt", "w");
        tr.printTupleMembers(fout);
        fclose(fout);
      }
      if(tr.getEvtNum() > 10)
        break;

      std::cout << "MET " << tr.getVar<float>("met")  <<" met_pt " << tr.getVar<float>("met_pt") << std::endl;
    }
    ch->Reset();
}


