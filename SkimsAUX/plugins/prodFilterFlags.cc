// -*- C++ -*-
//
// Package:    prodFilterFlags
// Class:      prodFilterFlags
// 
/**\class prodFilterFlags prodFilterFlags.cc RA2Classic/prodFilterFlags/src/prodFilterFlags.cc
 * 
 * Description: [one line class summary]
 * 
 * Implementation:
 *     [Notes on implementation]
 */
//
// Original Author:  Arne-Rasmus Draeger,68/111,4719,
//         Created:  Fri Apr 11 16:35:33 CEST 2014
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include <DataFormats/Math/interface/deltaR.h>
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

//
// class declaration
//

class prodFilterFlags : public edm::EDProducer {
public:
  explicit prodFilterFlags(const edm::ParameterSet&);
  ~prodFilterFlags();
	
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
	
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  edm::InputTag trigTagSrc_;
  std::string filterName_;
  edm::EDGetTokenT<edm::TriggerResults> TrigTagTok_;
  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
prodFilterFlags::prodFilterFlags(const edm::ParameterSet& iConfig)
{
  filterName_ = iConfig.getParameter<std::string>("filterName");
  trigTagSrc_ = iConfig.getParameter<edm::InputTag>("trigTagSrc");
  TrigTagTok_ = consumes<edm::TriggerResults>(trigTagSrc_);
  produces<int>("");
}


prodFilterFlags::~prodFilterFlags()
{
	
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
	
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
prodFilterFlags::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  int passesTrigger = -1;
  edm::Handle<edm::TriggerResults> trigResults; //our trigger result object
 // iEvent.getByLabel(trigTagSrc_, trigResults);
   iEvent.getByToken(TrigTagTok_, trigResults);
  const edm::TriggerNames& trigNames = iEvent.triggerNames(*trigResults); 
  if(trigNames.triggerIndex(filterName_) < trigResults->size())
    passesTrigger=trigResults->accept(trigNames.triggerIndex(filterName_));

  std::unique_ptr<int> htp(new int(passesTrigger));
  iEvent.put(std::move(htp));
}

// ------------ method called once each job just before starting event loop  ------------
void 
prodFilterFlags::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
prodFilterFlags::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
prodFilterFlags::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
prodFilterFlags::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
prodFilterFlags::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
prodFilterFlags::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
prodFilterFlags::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(prodFilterFlags);
