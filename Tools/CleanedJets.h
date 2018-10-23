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

  void generateCleanedJets(NTupleReader& tr) {
    const auto& gammaLVec               = tr.getVec<TLorentzVector>("gammaLVec");               // reco photon
    const auto& jetsLVec                = tr.getVec<TLorentzVector>("jetsLVec");                // jets
    const auto& prodJetsNoLep_jetsLVec  = tr.getVec<TLorentzVector>("prodJetsNoLep_jetsLVec");  // lepton cleaned jets
        
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
public:

  CleanedJets(){}

  ~CleanedJets(){}

  void operator()(NTupleReader& tr)
  {
    generateCleanedJets(tr);
  }
};

