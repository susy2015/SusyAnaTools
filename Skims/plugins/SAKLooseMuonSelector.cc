
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "SusyAnaTools/Skims/plugins/SAKLooseLepton.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


class SAKLooseMuonSelector : public edm::EDFilter {

  public:

    explicit SAKLooseMuonSelector(const edm::ParameterSet & iConfig);
    ~SAKLooseMuonSelector();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag muonSrc_, vtxSrc_, pfSrc_;
    bool doMuonVeto_;
    bool doMuonIso_, doMuonID_;
};


SAKLooseMuonSelector::SAKLooseMuonSelector(const edm::ParameterSet & iConfig) {
  muonSrc_     = iConfig.getParameter<edm::InputTag>("MuonSource");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("VertexSource");
  pfSrc_       = iConfig.getParameter<edm::InputTag>("PFSource");
  doMuonVeto_  = iConfig.getParameter<bool>("DoMuonVeto");
  doMuonIso_   = iConfig.getParameter<bool>("DoMuonIso");
  doMuonID_    = iConfig.getParameter<bool>("DoMuonID");

  produces<std::vector<pat::Muon> >("");
}


SAKLooseMuonSelector::~SAKLooseMuonSelector() {
}


bool SAKLooseMuonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  edm::Handle<std::vector<reco::PFCandidate> > pfcands;
  iEvent.getByLabel(pfSrc_, pfcands);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());
  for (edm::View<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {
    if( (!doMuonIso_ || (doMuonIso_ && isLooseMuonIso(*m, *pfcands))) && (!doMuonID_ || (doMuonID_ && isLooseMuonID(*m, vtxpos))) ){
      prod->push_back(pat::Muon(*m));
    }
  }

  // determine result before losing ownership of the pointer
  bool result = (doMuonVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);
  return result;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SAKLooseMuonSelector);
