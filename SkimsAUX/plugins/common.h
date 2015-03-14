#ifndef SKIMSAUX_COMMON_H
#define SKIMSAUX_COMMON_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

namespace commonFunctions
{
        double getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands,
                          const reco::Candidate* ptcl,
                          double r_iso_min, double r_iso_max, double kt_scale,
                              bool use_pfweight, bool charged_only);
}


#endif
