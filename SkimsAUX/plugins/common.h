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
    double ea = 0.;
    if      (fabs(eta)<=0.800) ea = 0.0913;
    else if (fabs(eta)<=1.300) ea = 0.0765;
    else if (fabs(eta)<=2.000) ea = 0.0546;
    else if (fabs(eta)<=2.200) ea = 0.0728;
    else if (fabs(eta)<=2.500) ea = 0.1177;
    return ea;
  }

  inline double GetElectronEA(double eta) {
    double ea = 0.;
    if      (fabs(eta)<=0.800) ea = 0.1013;
    else if (fabs(eta)<=1.300) ea = 0.0988;
    else if (fabs(eta)<=2.000) ea = 0.0572;
    else if (fabs(eta)<=2.200) ea = 0.0842;
    else if (fabs(eta)<=2.500) ea = 0.1530;
    return ea;
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
