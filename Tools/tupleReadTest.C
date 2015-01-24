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
    void calcJoe(NTupleReader& tr)
    {
        double retval = tr.getVar<double>("met") + tr.getVar<double>("mht");

        int five = 5;

        std::vector<double> *vec = new std::vector<double>(4, tr.getVar<double>("mht"));

        tr.registerDerivedVar("joe", retval);
        tr.registerDerivedVar("five", five);
        tr.registerDerivedVec("threeNum", vec);
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
    tr.registerFunction(&joeFunctions::calcJoe);

    std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
        if(tr.getEvtNum() == 1)
        {
            tr.printTupleMembers();
            FILE * fout = fopen("NTupleTypes.txt", "w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }
        if(tr.getEvtNum()%100000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
        //std::cout << tr.getVar<double>("met") << "\t" << tr.getVar<double>("mht") << "\t" << tr.getVar<double>("joe") << "\t" << tr.getVar<int>("five") << "\t" << tr.getVec<double>("muonsMtw").size() << "\t" << tr.getVec<double>("threeNum")[2] << std::endl;
    }

    const unsigned int& run = tr.getVar<unsigned int>("run");

}
