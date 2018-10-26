// CleanedJets.h
//
// Caleb J. Smith
// October 22, 2018

// Input vector of TLorentzVector: objects (photons, leptons, etc)
// Input vector of TLorentzVector: jets (before they are clean)
// Return vector of TLorentzVector: cleaned jets (dR cleaning)


#ifndef CLEANEDJETS_H
#define CLEANEDJETS_H

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"
#include "Math/VectorUtil.h"

#include <vector>
#include <iostream>
#include <string>

#endif

class CleanedJets {

private:

    void generateCleanedJets(NTupleReader& tr) 
    {
        const auto& gammaLVec               = tr.getVec<TLorentzVector>("gammaLVecPassLooseID");    // selected reco photon
        const auto& jetsLVec                = tr.getVec<TLorentzVector>("jetsLVec");                // jets
        const auto& prodJetsNoLep_jetsLVec  = tr.getVec<TLorentzVector>("prodJetsNoLep_jetsLVec");  // lepton cleaned jets

        const float dRMax = 0.15; // dR between photon and jet

        // loop over photons
        // determine which jets to keep
        std::vector<bool> keepJet1(jetsLVec.size(), true);
        std::vector<bool> keepJet2(prodJetsNoLep_jetsLVec.size(), true);
        for (int i = 0; i < gammaLVec.size(); ++i)
        {
            //jetObjectdRMatch(const TLorentzVector& object, const std::vector<TLorentzVector>& jetsLVec, const float jetObjectdRMax)
            int match1 = AnaFunctions::jetObjectdRMatch(gammaLVec[i], jetsLVec, dRMax);
            int match2 = AnaFunctions::jetObjectdRMatch(gammaLVec[i], prodJetsNoLep_jetsLVec, dRMax);
            if (match1 >= 0) keepJet1[match1] = false;
            if (match2 >= 0) keepJet2[match2] = false;
        }
        
        
        std::vector<std::string> jetVectors = {
                                                "jetsLVec",              
                                                "recoJetsCSVv2",
                                                "qgLikelihood",
                                                "qgPtD",                               
                                                "qgAxis1",                             
                                                "qgAxis2",                             
                                                "qgMult",                              
                                                "recoJetschargedHadronEnergyFraction", 
                                                "recoJetschargedEmEnergyFraction",     
                                                "recoJetsneutralEmEnergyFraction",     
                                                "recoJetsmuonEnergyFraction",          
                                                "recoJetsHFHadronEnergyFraction",     
                                                "recoJetsHFEMEnergyFraction",          
                                                "recoJetsneutralEnergyFraction",       
                                                "PhotonEnergyFraction",                
                                                "ElectronEnergyFraction",             
                                                "ChargedHadronMultiplicity",          
                                                "NeutralHadronMultiplicity",          
                                                "PhotonMultiplicity",                
                                                "ElectronMultiplicity",               
                                                "MuonMultiplicity",                    
                                                "DeepCSVb",                            
                                                "DeepCSVc",                            
                                                "DeepCSVl",                            
                                                "DeepCSVbb",                           
                                                "DeepCSVcc"
                                              };
        
        std::string noLeptonTag = "prodJetsNoLep_";
        for (const auto& jetVector : jetVectors)
        {
            // jetsLVec is stored as a TLorentz vector
            if (jetVector.compare("jetsLVec") == 0)
            {
                cleanVector<TLorentzVector>(tr, jetVector, keepJet1);
                cleanVector<TLorentzVector>(tr, noLeptonTag + jetVector, keepJet2);
            }
            // qgMult is stored as an integer
            else if (jetVector.compare("qgMult") == 0)
            {
                cleanVector<int>(tr, jetVector, keepJet1);
                cleanVector<int>(tr, noLeptonTag + jetVector, keepJet2);
            }
            // all other variables are floats
            else
            {
                cleanVector<float>(tr, jetVector, keepJet1);
                cleanVector<float>(tr, noLeptonTag + jetVector, keepJet2);
            }
        }
    }

    template <class type> void cleanVector(NTupleReader& tr, std::string vectorName, std::vector<bool> keepJet)
    {
        const auto& vec = tr.getVec<type>(vectorName); 
        std::vector<type>* cleanedVec = new std::vector<type>();
        if (keepJet.size() != vec.size())
        {
            std::cout << "ERROR in cleanVector(): vector sizes do not match for " << vectorName << std::endl;
            std::cout << "In cleanVector(): keepJet.size() = " << keepJet.size() << " and vec.size() = " << vec.size() << std::endl;
        }
        for (int i = 0; i < vec.size(); ++i)
        {
            if (keepJet[i]) cleanedVec->push_back(vec[i]);
        }
        tr.registerDerivedVec(vectorName+"_NoPhoton", cleanedVec);
    }

public:

    CleanedJets(){}

    ~CleanedJets(){}

    void operator()(NTupleReader& tr)
    {
        generateCleanedJets(tr);
    }
};

