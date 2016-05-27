
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/Provenance/interface/ProductID.h"

class provInfoElectrons : public edm::EDFilter {

  public:

    explicit provInfoElectrons(const edm::ParameterSet & iConfig);
    ~provInfoElectrons();

    typedef unsigned long key_type;

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

    edm::InputTag electronSrc_;

};


provInfoElectrons::provInfoElectrons(const edm::ParameterSet & iConfig) {
  electronSrc_  = iConfig.getParameter<edm::InputTag>("ElectronSource");

  produces<std::vector<edm::ProductID> >("elePFbaseProductID");
  produces<std::vector<key_type> >("elePFbaseKey");
}


provInfoElectrons::~provInfoElectrons() {
}


bool provInfoElectrons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(electronSrc_, electrons);

  reco::CandidatePtr leptonOutPtr;
  unsigned long dummyKey = 999999999;
  edm::ProductID dummyProductID;

  std::auto_ptr<std::vector<edm::ProductID> > elePFbaseProductIDptr(new std::vector<edm::ProductID>() );
  std::auto_ptr<std::vector<key_type> > elePFbaseKeyptr(new std::vector<key_type>() );

  unsigned int iele = 0;
  edm::View<pat::Electron>::const_iterator eleItr;
  for (eleItr = electrons->begin(), iele =0; eleItr != electrons->end(); ++eleItr, iele++) {

     edm::ProductID tofillProductID = dummyProductID;
     key_type tofillKey = dummyKey;

     const reco::CandidatePtr& elePtr =eleItr->pfCandidateRef()->sourceCandidatePtr(0);

     if( !eleItr->pfCandidateRef().isNull() ){
        if( !searchToPFbase(iEvent, elePtr, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for ele?!"<<std::endl<<std::endl; }

        if( leptonOutPtr.isNonnull() ){
           tofillProductID = leptonOutPtr.id();
           tofillKey = leptonOutPtr.key();
        }
     }
     elePFbaseProductIDptr->push_back(tofillProductID); elePFbaseKeyptr->push_back(tofillKey);
  }

  iEvent.put( elePFbaseProductIDptr, "elePFbaseProductID" );
  iEvent.put( elePFbaseKeyptr, "elePFbaseKey" );

  return true;
}

int provInfoElectrons::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
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

DEFINE_FWK_MODULE(provInfoElectrons);
