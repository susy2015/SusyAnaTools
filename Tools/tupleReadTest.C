#include "StopTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <cstdio>
#include <string>

int main()
{
    char nBase[] = "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";

    char fName[256];

    TChain *f = new TChain("stopTreeMaker/AUX");

    for(int i = 0; i < 10; ++i)
    {
        sprintf(fName, nBase, i);
        f->Add(fName);
    }
    
    //TTree *t = (TTree*)f->Get("stopTreeMaker/AUX");

    NTupleReader tr(f);

    while(tr.getNextEvent())
    {
//        std::cout << tr.getNEntries() << "\t" << tr.run << "\t" << tr.lumi << "\t" << tr.event << "\t" << tr.genDecayLVec->size() << "\t" <<  tr.mht << std::endl;
    }
}
