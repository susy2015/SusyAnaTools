
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "SusyAnaTools/Skims/plugins/SAKLooseLepton.h"


class SAKLooseElectronSelector : public edm::EDFilter {

  public:

    explicit SAKLooseElectronSelector(const edm::ParameterSet & iConfig);
    ~SAKLooseElectronSelector();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag electronSrc_, vtxSrc_, pfSrc_;
    bool doElectronVeto_;
    bool doElectronIso_, doElectronID_;
};


SAKLooseElectronSelector::SAKLooseElectronSelector(const edm::ParameterSet & iConfig) {
  electronSrc_    = iConfig.getParameter<edm::InputTag>("ElectronSource");
  vtxSrc_         = iConfig.getParameter<edm::InputTag>("VertexSource");
  pfSrc_          = iConfig.getParameter<edm::InputTag>("PFSource");
  doElectronVeto_ = iConfig.getParameter<bool>("DoElectronVeto");
  doElectronIso_  = iConfig.getParameter<bool>("DoElectronIso");
  doElectronID_   = iConfig.getParameter<bool>("DoElectronID");
  produces<std::vector<reco::GsfElectron> >("");
}


SAKLooseElectronSelector::~SAKLooseElectronSelector() {
}


bool SAKLooseElectronSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::GsfElectron> > electrons;
  iEvent.getByLabel(electronSrc_, electrons);
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  edm::Handle<std::vector<reco::PFCandidate> > pfcands;
  iEvent.getByLabel(pfSrc_, pfcands);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  // check which ones to keep
  std::auto_ptr<std::vector<reco::GsfElectron> > prod(new std::vector<reco::GsfElectron>());
  for (edm::View<reco::GsfElectron>::const_iterator e = electrons->begin(); e != electrons->end(); ++e) {
    if ( (!doElectronIso_ || (doElectronIso_ && isLooseElectronIso(*e, *pfcands))) && (!doElectronID_ || (doElectronID_ && isLooseElectronID(*e, vtxpos))) ){
      prod->push_back(reco::GsfElectron(*e));
    }
  }

  // determine result before losing ownership of the pointer
  bool result = (doElectronVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);
  return result;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SAKLooseElectronSelector);
