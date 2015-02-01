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

class testRead : public edm::EDProducer {

  public:

    explicit testRead(const edm::ParameterSet & iConfig);
    ~testRead();

    typedef unsigned long key_type;

  private:

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag muonSrc_;
    edm::InputTag eleSrc_;

    bool debug_;

    bool isData;

    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

};


testRead::testRead(const edm::ParameterSet & iConfig) {

  isData = true;

  muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSource");
  eleSrc_ = iConfig.getParameter<edm::InputTag>("eleSource");

  debug_ = iConfig.getParameter<bool>("debug");
}


testRead::~testRead() {
}


void testRead::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData = false;

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  edm::Handle<edm::View<pat::Electron> > eles;
  iEvent.getByLabel(eleSrc_, eles);

  for (edm::View<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {

     unsigned int uDNsize = m->userDataNames().size(), uFNsize = m->userFloatNames().size();
     std::cout<<"muons # : "<< (m - muons->begin() ) <<"  uDNsize : "<<uDNsize<<"  uFNsize : "<<uFNsize<<std::endl;
     for(unsigned int iu=0; iu<uDNsize; iu++){
        std::cout<<"\tuDN["<<iu<<"] : "<<m->userDataNames()[iu]<<std::endl;
     }
     std::cout<<std::endl;
     if( m->hasUserData("provInfo:muonPFbaseProductIDVM") ){
        std::cout<<"\ttype : "<<m->userDataObjectType("provInfo")<<std::endl;
        std::cout<<"\tval  : "<<(*m->userData<edm::ProductID>("provInfo"))<<std::endl;
     }else{
        std::cout<<"\tcannot find userData provInfo"<<std::endl;
     }
  }

}

int testRead::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
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
DEFINE_FWK_MODULE(testRead);
