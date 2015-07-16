// -*- C++ -*-
//
// Package:   prodFilterOutScraping
// Class:     prodFilterOutScraping
//
// Original Author:  Luca Malgeri

#ifndef prodFilterOutScraping_H
#define prodFilterOutScraping_H

// system include files
#include <memory>
#include <vector>
#include <map>
#include <set>

// user include files
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//
// class declaration
//


class prodFilterOutScraping : public edm::EDFilter {
public:
  explicit prodFilterOutScraping( const edm::ParameterSet & );
  ~prodFilterOutScraping();
  
private:
  virtual bool filter ( edm::Event &, const edm::EventSetup&) override;
  
  bool applyfilter;
  bool debugOn;
  double thresh;
  unsigned int numtrack;
  edm::EDGetTokenT<reco::TrackCollection> tracks_;
  reco::TrackBase::TrackQuality _trackQuality;
};

#endif
