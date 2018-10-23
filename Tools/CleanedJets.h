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
#include "Math/VectorUtil.h"

#include <vector>
#include <iostream>
#include <string>

#endif

class CleanedJets {

private:

  void generateCleanedJets(NTupleReader& tr) {
    const auto& gammaLVec = tr.getVec<TLorentzVector>("gammaLVec"); // reco photon
    double htWithoutPhoton = 0.0;
    int match = -1;
    match = AnaFunctions::jetObjectdRMatch(photon, jetsLVec, dRMax);

    tr.registerDerivedVar("htWithoutPhoton", htWithoutPhoton);
  }
public:

  CleanedJets(){}

  ~CleanedJets(){}

  void operator()(NTupleReader& tr)
  {
    generateCleanedJets(tr);
  }
};

