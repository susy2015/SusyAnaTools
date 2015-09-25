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
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Mar14_2014_v2/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150328_003540/0000/stopFlatNtuples_%d.root";
    char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";

    TChain *f = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();

    char fname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(fname, nBase, i);
        f->Add(fname);
    }
    
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

        const std::vector<double>& threeNum = tr.getVec<double>("threeNum");
        const std::vector<TLorentzVector>& trksForIsoVetoLVec = tr.getVec<TLorentzVector>("trksForIsoVetoLVec");
        const std::vector<double>& trksForIsoVeto_charge = tr.getVec<double>("trksForIsoVeto_charge");
        const std::vector<double>& trksForIsoVeto_dz = tr.getVec<double>("trksForIsoVeto_dz");
        const std::vector<int>& trksForIsoVeto_idx = tr.getVec<int>("trksForIsoVeto_idx");
        const std::vector<int>& trksForIsoVeto_pdgId = tr.getVec<int>("trksForIsoVeto_pdgId");

        const std::vector<int>& fish = tr.getVec<int>("fish");

        if(&fish != nullptr) std::cout << fish.size() << std::endl;

        if(tr.getEvtNum()%100000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
        //std::cout << tr.getVar<double>("met") << "\t" << tr.getVar<double>("mht") << "\t" << tr.getVar<double>("joe") << "\t" << tr.getVar<int>("five") << "\t" << tr.getVec<double>("muonsMtw").size() << "\t" << tr.getVec<double>("threeNum")[2] << std::endl;
    }

    //const unsigned int& run = tr.getVar<unsigned int>("run");

}
