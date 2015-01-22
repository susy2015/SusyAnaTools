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
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    char nBase[] = "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_TTJets_MSDecaysCKM_madgraph-tauola/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141224_052628/0000/stopFlatNtuples_%d.root";
    char fName[512];

    TChain *f = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();

    for(int i = 1; i <= 1; ++i)
    {
        sprintf(fName, nBase, i);
        f->Add(fName);
    }
    
    //TFile *f = new TFile(fName);
    //TTree *t = (TTree*)f->Get("stopTreeMaker/AUX");

    NTupleReader tr(f);
    tr.registerDerivedVar<double>("joe", &joeFunctions::calcJoe);

    std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
//        std::cout << tr.getNEntries() << "\t" << tr.run << "\t" << tr.lumi << "\t" << tr.event << "\t" << tr.genDecayLVec->size() << "\t" <<  tr.mht << std::endl;
        if(tr.getEvtNum()%100000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
        std::cout << tr("met") << "\t" << tr("mht") << "\t" << tr("joe") << "\t" << tr.getVec<double>("muonsMtw").size() << std::endl;
    }
}
