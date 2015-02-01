// -*- C++ -*-
//
// Package:    applyBEfilter
// Class:      applyBEfilter
// 
/**\class applyBEfilter applyBEfilter.cc
*/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Ulla's BE filter
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include "DataFormats/METReco/interface/AnomalousECALVariables.h"
#include "DataFormats/METReco/interface/BoundaryInformation.h"
#include "RecoMET/METFilters/interface/EcalBoundaryInfoCalculator.h"

using namespace edm;
using namespace std;

class applyBEfilter : public edm::EDFilter {
public:
  explicit applyBEfilter(const edm::ParameterSet&);
  ~applyBEfilter();

private:
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void beginJob();
  virtual void endJob();
//  virtual bool beginRun(const edm::Run&, const edm::EventSetup&);
//  virtual bool endRun(const edm::Run&, const edm::EventSetup&);

  // ----------member data ---------------------------
  bool debug_;

// Ulla's Boundary Energy filter
  edm::InputTag ecalAnomalousFilterTag_;
  Handle<AnomalousECALVariables> anomalousECALvarsHandle;
  AnomalousECALVariables anomalousECALvars;
  void loadBEfilterObj(edm::Event& iEvent, const edm::EventSetup& iSetup);

  double minDeadClusterSize_, maxBoundaryEnergy_;
  vector<int> limitDeadCellToChannelStatusEB_, limitDeadCellToChannelStatusEE_;

  void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  unsigned int run, event, ls; bool isdata; double pthat, scalePDF;

};


void applyBEfilter::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){
   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();
   isdata = iEvent.isRealData();
}


void applyBEfilter::loadBEfilterObj(edm::Event& iEvent, const edm::EventSetup& iSetup){

  iEvent.getByLabel(ecalAnomalousFilterTag_, anomalousECALvarsHandle);
  
  if (anomalousECALvarsHandle.isValid()) {
     anomalousECALvars = *anomalousECALvarsHandle;
  } else {
     cout << "anomalous ECAL Vars not valid/found" << endl;
  }
}

//
// static data member definitions
//

//
// constructors and destructor
//
applyBEfilter::applyBEfilter(const edm::ParameterSet& iConfig){

  debug_= iConfig.getUntrackedParameter<bool>("debug",false);

  ecalAnomalousFilterTag_ = iConfig.getParameter<edm::InputTag>("ecalAnomalousFilterTag");

  minDeadClusterSize_ = iConfig.getParameter<double>("minDeadClusterSize");
  maxBoundaryEnergy_ = iConfig.getParameter<double>("maxBoundaryEnergy");
  limitDeadCellToChannelStatusEB_ = iConfig.getParameter<std::vector<int> >("limitDeadCellToChannelStatusEB");
  limitDeadCellToChannelStatusEE_ = iConfig.getParameter<std::vector<int> >("limitDeadCellToChannelStatusEE");

}

applyBEfilter::~applyBEfilter() {

}

// ------------ method called on each new Event  ------------
bool applyBEfilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

// Event setup
//  loadEventInfo(iEvent, iSetup);

  loadBEfilterObj(iEvent, iSetup);

  bool pass = true;

  bool isBEfiltered = false;

  isBEfiltered = anomalousECALvars.isDeadEcalCluster(maxBoundaryEnergy_, limitDeadCellToChannelStatusEB_, limitDeadCellToChannelStatusEE_);

  if( isBEfiltered ){ pass = false; }

  return pass;

}

// ------------ method called once each job just before starting event loop  ------------
void applyBEfilter::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void applyBEfilter::endJob() {
}

// ------------ method called once each run just before starting event loop  ------------
//bool applyBEfilter::beginRun(const edm::Run &run, const edm::EventSetup& iSetup) {
//  return true;
//}

// ------------ method called once each run just after starting event loop  ------------
//bool applyBEfilter::endRun(const edm::Run &run, const edm::EventSetup& iSetup) {
//  return true;
//}

//define this as a plug-in
DEFINE_FWK_MODULE(applyBEfilter);
