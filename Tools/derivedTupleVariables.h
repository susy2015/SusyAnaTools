#include "NTupleReader.h"

#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

#include <vector>

namespace plotterFunctions
{
    void generateWeight(NTupleReader& tr)
    {
        // Calculate PU weight

        // Calculate Z-eff weight
        
        
    }

    void muEffInfo(NTupleReader& tr)
    {
        const std::vector<int>& genDecayPdgIdVec        = tr.getVec<int>("genDecayPdgIdVec");
        const std::vector<TLorentzVector>& genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
        const std::vector<TLorentzVector>& muonsLVec    = tr.getVec<TLorentzVector>("muonsLVec");

        std::vector<TLorentzVector>* genMatchMu = new std::vector<TLorentzVector>();

        for(int j = 0; j < muonsLVec.size(); ++j)
        {
            int jMindR = 0;
            double dRMin = 999.9;

            for(int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
            {
                double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], muonsLVec[j]);
                if(abs(genDecayPdgIdVec[i]) == 13)
                {
                    if(dR < dRMin)
                    {
                        dRMin = dR;
                        jMindR = j;
                    }
                }
         
            }
            if(dRMin < 0.02)
            {
                genMatchMu->push_back(muonsLVec[j]);
            }
        }

        int nMuGenMatch = genMatchMu->size();

        tr.registerDerivedVec("genMatchMu", genMatchMu);
        tr.registerDerivedVar("nGenMatchedMu", nMuGenMatch);
    }
    
    void registerFunctions(NTupleReader& tr)
    {
        tr.registerFunction(&generateWeight);
        tr.registerFunction(&muEffInfo);        
    }
}
