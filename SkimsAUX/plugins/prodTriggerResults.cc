// -*- C++ -*-
//
// Package:    prodTriggerResults
// Class:      prodTriggerResults
// 
/**\class prodTriggerResults prodTriggerResults.cc RA2Classic/prodTriggerResults/src/prodTriggerResults.cc
 */
//
// Original Author:  Sam Bein,
//         Created:  Wednesday June 24 2015
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
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//
// class declaration
//

class prodTriggerResults : public edm::EDProducer {
public:
  explicit prodTriggerResults(const edm::ParameterSet&);
  ~prodTriggerResults();
	
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
  edm::InputTag trigPrescalesTagSrc_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>TrigPrescalesTagTok_;
  edm::EDGetTokenT<edm::TriggerResults>TrigTagTok_;
  std::vector<std::string> parsedTrigNamesVec_;

  bool debug_;
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
prodTriggerResults::prodTriggerResults(const edm::ParameterSet& iConfig)
{
  parsedTrigNamesVec_ = iConfig.getParameter <std::vector<std::string> > ("triggerNameList");
  trigTagSrc_ = iConfig.getParameter<edm::InputTag> ("trigTagSrc");
  trigPrescalesTagSrc_ = iConfig.getParameter<edm::InputTag> ("trigPrescalesTagSrc"); 
  TrigPrescalesTagTok_=consumes<pat::PackedTriggerPrescales>(trigPrescalesTagSrc_);
  TrigTagTok_=consumes<edm::TriggerResults>(trigTagSrc_);
  debug_ = iConfig.getParameter<bool>("debug");

  if( debug_ ){
     std::cout << "will store trigger information for..." << std::endl;
  }

  produces<std::vector<std::string> >("TriggerNames");
  produces<std::vector<int> >("TriggerPrescales");
  produces<std::vector<int> >("PassTrigger");
}


prodTriggerResults::~prodTriggerResults()
{}

//
// member functions
//

// ------------ method called to produce the data  ------------
void
prodTriggerResults::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::unique_ptr<std::vector<int> > passTrigVec(new std::vector<int>());
  std::unique_ptr<std::vector<std::string> > trigNamesVec(new std::vector<std::string>());
  std::unique_ptr<std::vector<int> > trigPrescaleVec(new std::vector<int>());

  int passesTrigger;
  edm::Handle<edm::TriggerResults> trigResults; //our trigger result object
  iEvent.getByToken(TrigTagTok_,trigResults);
  edm::Handle<pat::PackedTriggerPrescales> trigPrescales;
  iEvent.getByToken(TrigPrescalesTagTok_,trigPrescales);
  const edm::TriggerNames& trigNames = iEvent.triggerNames(*trigResults);

  const std::vector<std::string> & allNames = trigNames.triggerNames();

  for(unsigned int ip = 0; ip < parsedTrigNamesVec_.size(); ip++){
     for(unsigned int ia=0; ia<allNames.size(); ia++){
        if(allNames.at(ia).find(parsedTrigNamesVec_.at(ip)) != std::string::npos) trigNamesVec->push_back(allNames.at(ia));
     }
  }

  std::sort(trigNamesVec->begin(), trigNamesVec->end());
  trigNamesVec->erase( std::unique( trigNamesVec->begin(), trigNamesVec->end() ), trigNamesVec->end() );

  std::vector<unsigned int> selTrigIdxVec(trigNamesVec->size());
  for(unsigned int it=0; it<trigNamesVec->size(); it++){
     for(unsigned int ia=0; ia<allNames.size(); ia++){
        if( allNames.at(ia) == trigNamesVec->at(it) ) selTrigIdxVec[it] = ia; 
     }
  }

  for(unsigned int it=0; it<trigNamesVec->size(); it++){
     passesTrigger = trigResults->accept(trigNames.triggerIndex(trigNamesVec->at(it)));
     passTrigVec->push_back(passesTrigger);

     trigPrescaleVec->push_back(trigPrescales->getPrescaleForIndex(selTrigIdxVec[it]));
  }

  // for (int iHLT = 0 ; iHLT<static_cast<int>(trigResults->size()); ++iHLT) 
  // {	
  //   //std::cout << "tname="<< trigNames.triggerName(iHLT)<< std::endl;
  //   //std::cout << "tndex="<< trigNames.triggerIndex(trigNames.triggerName(iHLT)) << std::endl;
  //   passesTrigger=trigResults->accept(iHLT);
  //   passTrigVec->push_back(passesTrigger);
  // }
  iEvent.put(std::move(passTrigVec),"PassTrigger");
  iEvent.put(std::move(trigPrescaleVec),"TriggerPrescales");
  iEvent.put(std::move(trigNamesVec),"TriggerNames");	
}


// ------------ method called once each job just before starting event loop  ------------
void 
prodTriggerResults::beginJob()
{
/*
  std::cout << "will store trigger information for..." << std::endl;
  std::stringstream ss(triggerNameList_);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> vstrings(begin, end);
  std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
  for(unsigned int i = 0; i< vstrings.size(); i++)
    parsedTrigNamesVec.push_back(vstrings.at(i));
*/
}

// ------------ method called once each job just after ending the event loop  ------------
void 
prodTriggerResults::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
prodTriggerResults::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
prodTriggerResults::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
prodTriggerResults::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
prodTriggerResults::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
prodTriggerResults::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(prodTriggerResults);
