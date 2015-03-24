#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"

#include <vector>

namespace plotterFunctions
{
    void exampleFunction(NTupleReader& tr)
    {
        const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");
        
        double lazyHt = 0.0;
        std::vector<const TLorentzVector*>* fewerJets = new std::vector<const TLorentzVector*>();

        for(auto& jet : jetsLVec) 
        {
            lazyHt += jet.Pt();
            if(jet.Pt() > 25) fewerJets->push_back(&jet);
        }

        tr.registerDerivedVar("lazyHt", lazyHt);
        tr.registerDerivedVec("fewerJets", fewerJets);
    }

    void exampleUseofDerived(NTupleReader& tr)
    {
        const std::vector<const TLorentzVector*>& jetsLVec = tr.getVec<TLorentzVector>("fewerJets");

        double lessLazyHt = 0.0;
        for(auto& jetPtr : jetsLec) lessLazyHt += jetPtr->Pt();

        tr.registerDerivedVar("lessLazyHt", lessLazyHt);
    }
    
    void registerFunctions(NTupleReader& tr)
    {
        //register functions with NTupleReader
        tr.registerFunction(&exampleFunction);
        tr.registerFunction(&exampleUseofDerived);
    }

    void activateBranches(std::set<std::string>& activeBranches)
    {
        activeBranches.insert("jetsLVec");
    }
}
