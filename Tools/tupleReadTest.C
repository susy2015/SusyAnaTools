#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>

namespace joeFunctions
{
    void calcJoe(const NTupleReader& tr, void* val)
    {
        double retval = tr("mht") + tr("met");
        NTupleReader::setDerived(retval, val);
    }
}

int main()
{
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_*.root";
    char nBase[] = "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_*.root";
    
    TChain *f = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();

    f->Add(nBase);
    
    NTupleReader tr(f);
    tr.registerDerivedVar<double>("joe", &joeFunctions::calcJoe);

    std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
        if(tr.getEvtNum()%100000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
        std::cout << tr("met") << "\t" << tr("mht") << "\t" << tr("joe") << "\t" << tr.getVec<double>("muonsMtw").size() << std::endl;
    }
}
