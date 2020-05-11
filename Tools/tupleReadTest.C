#include "NTupleReader.h"
#include "TopWeightCalculator.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>

class GetScaleWeights
{
private:
    NTupleReader trSupp_;

    void getScaleWeights(NTupleReader& tr)
    {
        if(trSupp_.getNextEvent() &&
           tr.getVar<unsigned long long>("event") == trSupp_.getVar<int>("event") &&
           tr.getVar<float>("GenMET_pt")          == static_cast<float>(trSupp_.getVar<double>("GenMET_pt")) )
        {
            tr.registerDerivedVec("LHEScaleWeight", new std::vector(trSupp_.getVec<double>("LHEScaleWeight")));
            std::cout << tr.getVar<unsigned long long>("event") << "\t" << trSupp_.getVar<int>("event") << "\t" << tr.getVar<float>("GenMET_pt") << "\t" << trSupp_.getVar<double>("GenMET_pt") << std::endl;
        }
        else
        {
            THROW_SATEXCEPTION("ERROR: Event mismatch between master and supplamental file!!!!");
        }
    }

public:
    GetScaleWeights(TChain *ch) : trSupp_(ch, {"event"}) {}
    GetScaleWeights(GetScaleWeights&) = delete;
    GetScaleWeights(GetScaleWeights&& gsw) : trSupp_(std::move(gsw.trSupp_)) {}

    void operator()(NTupleReader& tr) { getScaleWeights(tr); }
};

int main()
{
    char baseFile[] = "root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_11Apr2019_fastsimv5_v6p1_v6p5/SMS_T2bW_fastsim_2016//SMS_T2bW_mStop1000_mLSP0_fastsim_2016_Skim_070602_0_100332.root";
    char supplamantalFile[] = "root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/ScaleProcessed_11Apr2019_fastsimv5_v6p1_v6p5/SMS_T2bW_fastsim_2016//SMS_T2bW_mStop1000_mLSP0_fastsim_2016_Skim_070602_0_100332.root";

    TChain *chBase = new TChain("Events");
    chBase->Add(baseFile);

    TChain *chSupp = new TChain("Events");
    chSupp->Add(supplamantalFile);

    try
    {
        NTupleReader tr(chBase, {"nJet"});

        //For NTupleReader users you simply need to register the class with NTupleReader 
        tr.emplaceModule<GetScaleWeights>(chSupp);

        while(tr.getNextEvent())
        {
            const auto& LHEScaleWeight = tr.getVec<double>("LHEScaleWeight");

            std::cout << LHEScaleWeight.size() << std::endl;
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

