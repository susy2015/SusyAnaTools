#ifndef SKIMSAUX_COMMON_H
#define SKIMSAUX_COMMON_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

namespace commonFunctions
{

  inline double GetMuonEA(double eta) {
    double abseta = fabs(eta);
    if (abseta < 0.8) return 0.0735;
    else if (abseta < 1.3) return 0.0619;
    else if (abseta < 2.0) return 0.0465;
    else if (abseta < 2.2) return 0.0433;
    else if (abseta < 2.5) return 0.0577;
    else return 0;
  }

  inline double GetElectronEA(double eta) {
    double abseta = fabs(eta);
    if (abseta < 1) return 0.1752;
    else if (abseta < 1.479) return 0.1862;
    else if (abseta < 2.0) return 0.1411;
    else if (abseta < 2.2) return 0.1534;
    else if (abseta < 2.3) return 0.1903;
    else if (abseta < 2.4) return 0.2243;
    else if (abseta < 2.5) return 0.2687;
    else return 0;
  }

  double GetMiniIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands,
				 const reco::Candidate* ptcl, std::string type, 
				 double rho, bool computeMT2Activity=false,
				 double r_iso_min=0.05, double r_iso_max=0.2, double kt_scale=10.,
				 bool useEAcorr=true, bool charged_only=false);

  double GetRA2Activity(edm::Handle<pat::JetCollection> jets, const reco::Candidate* ptcl, const bool useEME=true);

  double GetTrackActivity(edm::Handle<pat::PackedCandidateCollection> other_pfcands, const pat::PackedCandidate* track);

// The following is the old function definition
//        double getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands,
//                          const reco::Candidate* ptcl,
//                          double r_iso_min, double r_iso_max, double kt_scale,
//                              bool use_pfweight, bool charged_only);

}


#endif
