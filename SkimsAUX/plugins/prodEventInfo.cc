#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/METReco/interface/MET.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TLorentzVector.h"

class prodEventInfo : public edm::EDFilter {

  public:

    explicit prodEventInfo(const edm::ParameterSet & iConfig);
    ~prodEventInfo();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag vtxSrc_;
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    bool debug_;
    bool isData_;
};


prodEventInfo::prodEventInfo(const edm::ParameterSet & iConfig) {
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");

  debug_       = iConfig.getParameter<bool>("debug");

  isData_      = true;

  produces<int>("vtxSize");
  produces<double>("trunpv");
  produces<double>("avgnpv");
  produces<int>("npv");
  produces<int>("nm1");
  produces<int>("n0");
  produces<int>("np1");
  produces<double>("storedWeight");
}


prodEventInfo::~prodEventInfo() {
}


bool prodEventInfo::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData_ = false;

  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
//  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  std::auto_ptr<int> vtxSize(new int);
  *vtxSize = vertices->size();

  std::auto_ptr<double> tru_npv(new double);
  *tru_npv = -1;

  std::auto_ptr<double> avg_npv(new double);
  *avg_npv = 0;

  std::auto_ptr<int> nm1(new int), n0(new int), np1(new int), npv(new int);
  *nm1 = -1; *n0 = -1; *np1 = -1; *npv = -1;

  std::auto_ptr<double> storedWeight(new double);
  *storedWeight = -1.0;

  if( !isData_ ){
     iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
     std::vector<PileupSummaryInfo>::const_iterator PVI;

     for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

        int BX = PVI->getBunchCrossing();

        *avg_npv += double(PVI->getPU_NumInteractions());

        if(BX == -1) {
           *nm1 = PVI->getPU_NumInteractions();
        }
        if(BX == 0) {
           *n0 = PVI->getPU_NumInteractions();
        }
        if(BX == 1) {
           *np1 = PVI->getPU_NumInteractions();
        }

        if(BX == 0) {
           *npv = PVI->getPU_NumInteractions();
           *tru_npv = PVI->getTrueNumInteractions();
           continue; // No touching of this "continue", since I'm not sure why it's here originally
        }
     }
     *avg_npv /= 3.0;

     edm::Handle<GenEventInfoProduct> genEvtInfoHandle;
     iEvent.getByLabel("generator", genEvtInfoHandle);
     if (genEvtInfoHandle.isValid()) {
        *storedWeight = genEvtInfoHandle->weight();
     }
  }

  iEvent.put(vtxSize, "vtxSize");
  iEvent.put(tru_npv, "trunpv");
  iEvent.put(avg_npv, "avgnpv");
  iEvent.put(npv, "npv");
  iEvent.put(nm1, "nm1");
  iEvent.put(n0, "n0");
  iEvent.put(np1, "np1");
  iEvent.put(storedWeight, "storedWeight");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodEventInfo);
