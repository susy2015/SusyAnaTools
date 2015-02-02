#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TTree.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

typedef unsigned int size;

class provInfoMuons : public edm::EDFilter {

  public:

    explicit provInfoMuons(const edm::ParameterSet & iConfig);
    ~provInfoMuons();

    typedef unsigned long key_type;

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    void beginJob();

    edm::InputTag muonSrc_;

    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

};


provInfoMuons::provInfoMuons(const edm::ParameterSet & iConfig) {
  muonSrc_     = iConfig.getParameter<edm::InputTag>("MuonSource");

  produces<std::vector<edm::ProductID> >("muonPFbaseProductID");
  produces<std::vector<key_type> >("muonPFbaseKey");
}


provInfoMuons::~provInfoMuons() {
}


void provInfoMuons::beginJob(){
}


bool provInfoMuons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  reco::CandidatePtr leptonOutPtr;
  unsigned long dummyKey = 999999999;
  edm::ProductID dummyProductID;

  std::auto_ptr<std::vector<edm::ProductID> > muonPFbaseProductIDptr(new std::vector<edm::ProductID>() );
  std::auto_ptr<std::vector<key_type> > muonPFbaseKeyptr(new std::vector<key_type>() );

  edm::View<pat::Muon>::const_iterator m;
  unsigned int imu;
  for (m = muons->begin(), imu=0; m != muons->end(); ++m, imu++) {

     edm::ProductID tofillProductID = dummyProductID;
     key_type tofillKey = dummyKey;

     const reco::CandidatePtr& muonRef =m->pfCandidateRef()->sourceCandidatePtr(0);

     if( !m->pfCandidateRef().isNull() ){
        if( !searchToPFbase(iEvent, muonRef, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for muon?!"<<std::endl<<std::endl; }

        if( leptonOutPtr.isNonnull() ){
           tofillProductID = leptonOutPtr.id();
           tofillKey = leptonOutPtr.key();
        }
     }
     muonPFbaseProductIDptr->push_back(tofillProductID); muonPFbaseKeyptr->push_back(tofillKey);
  }

  iEvent.put( muonPFbaseProductIDptr, "muonPFbaseProductID" );
  iEvent.put( muonPFbaseKeyptr, "muonPFbaseKey" );

  return true;
}

int provInfoMuons::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
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

DEFINE_FWK_MODULE(provInfoMuons);
