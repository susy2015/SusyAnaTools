#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Provenance/interface/ProductID.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

namespace edm { using ::std::advance; }

class provInfo : public edm::EDProducer {

  public:

    explicit provInfo(const edm::ParameterSet & iConfig);
    ~provInfo();

    typedef unsigned long key_type;

  private:

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag muonSrc_;
    edm::InputTag eleSrc_;

    bool debug_;

    bool isData;

    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

};


provInfo::provInfo(const edm::ParameterSet & iConfig) {

  isData = true;

  muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSource");
  eleSrc_ = iConfig.getParameter<edm::InputTag>("eleSource");

  debug_ = iConfig.getParameter<bool>("debug");
/*
  produces<std::vector<edm::ProductID> >("muonProvProductID");
  produces<std::vector<key_type> >("muonProvKey");
  produces<std::vector<edm::ProductID> >("eleProvProductID");
  produces<std::vector<key_type> >("eleProvKey");
*/
  produces<edm::ValueMap<edm::ProductID> >("muonPFbaseProductID");
  produces<edm::ValueMap<key_type> >("muonPFbaseKey");
  produces<edm::ValueMap<edm::ProductID> >("elePFbaseProductID");
  produces<edm::ValueMap<key_type> >("elePFbaseKey");

  produces<edm::OwnVector<pat::UserData> >("muonPFbaseProductIDOV");
  produces<edm::ValueMap<edm::Ptr<pat::UserData> > >("muonPFbaseProductIDVM");

  produces<edm::OwnVector<pat::UserData> >("elePFbaseProductIDOV");
  produces<edm::ValueMap<edm::Ptr<pat::UserData> > >("elePFbaseProductIDVM");

}


provInfo::~provInfo() {
}


void provInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData = false;

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  edm::Handle<edm::View<pat::Electron> > eles;
  iEvent.getByLabel(eleSrc_, eles);

  std::unique_ptr<std::vector<edm::ProductID> > muonProvProductIDptr(new std::vector<edm::ProductID>() );
  std::unique_ptr<std::vector<key_type> > muonProvKeyptr(new std::vector<key_type>() ); 

  std::unique_ptr<std::vector<edm::ProductID> > eleProvProductIDptr(new std::vector<edm::ProductID>() );
  std::unique_ptr<std::vector<key_type> > eleProvKeyptr(new std::vector<key_type>() ); 

  std::unique_ptr<edm::ValueMap<edm::ProductID> > muonPFbaseProductIDptr(new edm::ValueMap<edm::ProductID>() );
  std::unique_ptr<edm::ValueMap<key_type> > muonPFbaseKeyptr(new edm::ValueMap<key_type>() );

  edm::ValueMap<edm::ProductID>::Filler muonProductIDfiller(*muonPFbaseProductIDptr);
  edm::ValueMap<key_type>::Filler muonKeyfiller(*muonPFbaseKeyptr);

  std::unique_ptr<edm::ValueMap<edm::ProductID> > elePFbaseProductIDptr(new edm::ValueMap<edm::ProductID>() );
  std::unique_ptr<edm::ValueMap<key_type> > elePFbaseKeyptr(new edm::ValueMap<key_type>() );

  edm::ValueMap<edm::ProductID>::Filler eleProductIDfiller(*elePFbaseProductIDptr);
  edm::ValueMap<key_type>::Filler eleKeyfiller(*elePFbaseKeyptr);

  reco::CandidatePtr leptonOutPtr;

  key_type dummyKey = 999999999;
  edm::ProductID dummyProductID;

  std::unique_ptr<edm::OwnVector<pat::UserData> > muonPFbaseProductIDOVptr(new edm::OwnVector<pat::UserData>());
  for (edm::View<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {

     const reco::CandidatePtr& muonRef =m->pfCandidateRef()->sourceCandidatePtr(0);
     if( !searchToPFbase(iEvent, muonRef, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for muon?!"<<std::endl<<std::endl; }

     if( leptonOutPtr.isNonnull() ){
        muonProvProductIDptr->push_back(leptonOutPtr.id() );
        muonProvKeyptr->push_back(leptonOutPtr.key() );
        muonPFbaseProductIDOVptr->push_back( pat::UserData::make(leptonOutPtr.id()) );
     }else{
        muonProvProductIDptr->push_back(dummyProductID);
        muonProvKeyptr->push_back(dummyKey);
        muonPFbaseProductIDOVptr->push_back( pat::UserData::make(dummyProductID) );
     }
  }

  std::unique_ptr<edm::OwnVector<pat::UserData> > elePFbaseProductIDOVptr(new edm::OwnVector<pat::UserData>());
  for (edm::View<pat::Electron>::const_iterator e = eles->begin(); e != eles->end(); ++e) {

     const reco::CandidatePtr& eleRef = e->pfCandidateRef()->sourceCandidatePtr(0);
     if( !searchToPFbase(iEvent, eleRef, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for ele?!"<<std::endl<<std::endl; }

     if( leptonOutPtr.isNonnull() ){
        eleProvProductIDptr->push_back(leptonOutPtr.id() );
        eleProvKeyptr->push_back(leptonOutPtr.key() );
        elePFbaseProductIDOVptr->push_back( pat::UserData::make(leptonOutPtr.id()) );
     }else{
        eleProvProductIDptr->push_back(dummyProductID);
        eleProvKeyptr->push_back(dummyKey);
        elePFbaseProductIDOVptr->push_back( pat::UserData::make(dummyProductID) );
     }
  }

  muonProductIDfiller.insert(muons, muonProvProductIDptr->begin(), muonProvProductIDptr->end()); muonProductIDfiller.fill();
  muonKeyfiller.insert(muons, muonProvKeyptr->begin(), muonProvKeyptr->end()); muonKeyfiller.fill();
  eleProductIDfiller.insert(eles, eleProvProductIDptr->begin(), eleProvProductIDptr->end()); eleProductIDfiller.fill();
  eleKeyfiller.insert(eles, eleProvKeyptr->begin(), eleProvKeyptr->end()); eleKeyfiller.fill();

  edm::OrphanHandle<edm::OwnVector<pat::UserData> > muonPFbaseProductIDOVhandle = iEvent.put(std::move(muonPFbaseProductIDOVptr), "muonPFbaseProductIDOV");
  std::vector<edm::Ptr<pat::UserData> > muonPFbaseProductIDtmpPtr;
  for (unsigned int im = 0; im < muons->size(); ++im) {
  // It is crucial to use the OrphanHandle here and not a RefProd from GetRefBeforePut
     muonPFbaseProductIDtmpPtr.push_back(edm::Ptr<pat::UserData>(muonPFbaseProductIDOVhandle, im));
  }

  std::unique_ptr<edm::ValueMap<edm::Ptr<pat::UserData> > > muonPFbaseProductIDVMptr (new edm::ValueMap<edm::Ptr<pat::UserData> >());

  edm::ValueMap<edm::Ptr<pat::UserData> >::Filler muonPFbaseProductIDVMfiller(*muonPFbaseProductIDVMptr);
  muonPFbaseProductIDVMfiller.insert(muons, muonPFbaseProductIDtmpPtr.begin(), muonPFbaseProductIDtmpPtr.end());
  muonPFbaseProductIDVMfiller.fill();

  edm::OrphanHandle<edm::OwnVector<pat::UserData> > elePFbaseProductIDOVhandle = iEvent.put(std::move(elePFbaseProductIDOVptr), "elePFbaseProductIDOV");
  std::vector<edm::Ptr<pat::UserData> > elePFbaseProductIDtmpPtr;
  for (unsigned int im = 0; im < eles->size(); ++im) {
  // It is crucial to use the OrphanHandle here and not a RefProd from GetRefBeforePut
     elePFbaseProductIDtmpPtr.push_back(edm::Ptr<pat::UserData>(elePFbaseProductIDOVhandle, im));
  }

  std::unique_ptr<edm::ValueMap<edm::Ptr<pat::UserData> > > elePFbaseProductIDVMptr (new edm::ValueMap<edm::Ptr<pat::UserData> >());

  edm::ValueMap<edm::Ptr<pat::UserData> >::Filler elePFbaseProductIDVMfiller(*elePFbaseProductIDVMptr);
  elePFbaseProductIDVMfiller.insert(eles, elePFbaseProductIDtmpPtr.begin(), elePFbaseProductIDtmpPtr.end());
  elePFbaseProductIDVMfiller.fill();

  // store in the event
  iEvent.put(std::move(muonPFbaseProductIDptr), "muonPFbaseProductID");
  iEvent.put(std::move(muonPFbaseKeyptr), "muonPFbaseKey");
  iEvent.put(std::move(elePFbaseProductIDptr), "elePFbaseProductID");
  iEvent.put(std::move(elePFbaseKeyptr), "elePFbaseKey");

  iEvent.put(std::move(muonPFbaseProductIDVMptr), "muonPFbaseProductIDVM");
  iEvent.put(std::move(elePFbaseProductIDVMptr), "elePFbaseProductIDVM");
}

int provInfo::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
  outPtr = inPtr;
  if( inPtr.isNull() ){ return 0; }
  std::string transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
  while ( transModuleLabel != "particleFlow" ){
     outPtr = outPtr->sourceCandidatePtr(0);
     if( outPtr.isNonnull() ){
        transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
     }else{
        break;
     }
  }
  if( outPtr.isNull() ){ return 0; }
  return 1;
}

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE(provInfo);
