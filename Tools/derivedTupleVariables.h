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

        std::vector<int>* genMatchMu = new std::vector<int>();

        for(int j = 0; j < muonsLVec.size(); ++j)
        {
            double dRMin = 999.9;

            for(int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
            {
                double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], muonsLVec[j]);
                if(abs(genDecayPdgIdVec[i]) == 13)
                {
                    if(dR < dRMin)
                    {
                        dRMin = dR;
                    }
                }
         
            }
            if(dRMin < 0.02)
            {
                genMatchMu->push_back(j);
            }
        }

        tr.registerDerivedVec("genMatchMu", genMatchMu);
    }
    
    void registerFunctions(NTupleReader& tr)
    {
        tr.registerFunction(&generateWeight);
        tr.registerFunction(&muEffInfo);        
    }
}
