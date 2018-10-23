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
        std::cout << "Running CleanedJets" << std::endl;
        const auto& gammaLVec               = tr.getVec<TLorentzVector>("gammaLVec");               // reco photon
        const auto& jetsLVec                = tr.getVec<TLorentzVector>("jetsLVec");                // jets
        const auto& prodJetsNoLep_jetsLVec  = tr.getVec<TLorentzVector>("prodJetsNoLep_jetsLVec");  // lepton cleaned jets
        // jet related variables
        const auto& qgPtD                                   = tr.getVec<float>("qgPtD");                               
        const auto& qgAxis1                                 = tr.getVec<float>("qgAxis1");                             
        const auto& qgAxis2                                 = tr.getVec<float>("qgAxis2");                             
        const auto& qgMult                                  = tr.getVec<float>("qgMult");                              
        const auto& recoJetschargedHadronEnergyFraction     = tr.getVec<float>("recoJetschargedHadronEnergyFraction"); 
        const auto& recoJetschargedEmEnergyFraction         = tr.getVec<float>("recoJetschargedEmEnergyFraction");     
        const auto& recoJetsneutralEmEnergyFraction         = tr.getVec<float>("recoJetsneutralEmEnergyFraction");     
        const auto& recoJetsmuonEnergyFraction              = tr.getVec<float>("recoJetsmuonEnergyFraction");          
        const auto& recoJetsHFHadronEnergyFraction          = tr.getVec<float>("recoJetsHFHadronEnergyFraction");     
        const auto& recoJetsHFEMEnergyFraction              = tr.getVec<float>("recoJetsHFEMEnergyFraction");          
        const auto& recoJetsneutralEnergyFraction           = tr.getVec<float>("recoJetsneutralEnergyFraction");       
        const auto& PhotonEnergyFraction                    = tr.getVec<float>("PhotonEnergyFraction");                
        const auto& ElectronEnergyFraction                  = tr.getVec<float>("ElectronEnergyFraction");             
        const auto& ChargedHadronMultiplicity               = tr.getVec<float>("ChargedHadronMultiplicity");          
        const auto& NeutralHadronMultiplicity               = tr.getVec<float>("NeutralHadronMultiplicity");          
        const auto& PhotonMultiplicity                      = tr.getVec<float>("PhotonMultiplicity");                
        const auto& ElectronMultiplicity                    = tr.getVec<float>("ElectronMultiplicity");               
        const auto& MuonMultiplicity                        = tr.getVec<float>("MuonMultiplicity");                    
        const auto& DeepCSVb                                = tr.getVec<float>("DeepCSVb");                            
        const auto& DeepCSVc                                = tr.getVec<float>("DeepCSVc");                            
        const auto& DeepCSVl                                = tr.getVec<float>("DeepCSVl");                            
        const auto& DeepCSVbb                               = tr.getVec<float>("DeepCSVbb");                           
        const auto& DeepCSVcc                               = tr.getVec<float>("DeepCSVcc");                           
            
        auto* jetsNoPhotonLVec          = new std::vector<TLorentzVector>();
        auto* jetsNoPhotonNoLeptonLVec  = new std::vector<TLorentzVector>();

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
        for (int i = 0; i < jetsLVec.size(); ++i)
        {
            if (keepJet1[i]) jetsNoPhotonLVec->push_back(jetsLVec[i]);
        }
        for (int i = 0; i < prodJetsNoLep_jetsLVec.size(); ++i)
        {
            if (keepJet1[i]) jetsNoPhotonNoLeptonLVec->push_back(prodJetsNoLep_jetsLVec[i]);
        }

        tr.registerDerivedVec("jetsNoPhotonLVec",           jetsNoPhotonLVec);
        tr.registerDerivedVec("jetsNoPhotonNoLeptonLVec",   jetsNoPhotonNoLeptonLVec);
    }
    template <class type> void cleanVariable(NTupleReader& tr, std::string variable, std::vector<bool> mask)
    {
        const auto& var = tr.getVec<type>(variable); 
        tr.registerDerivedVec(variable+"_cleaned", var);
    }
public:

    CleanedJets(){}

    ~CleanedJets(){}

    void operator()(NTupleReader& tr)
    {
        generateCleanedJets(tr);
    }
};

