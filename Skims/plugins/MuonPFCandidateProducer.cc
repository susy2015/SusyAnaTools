
#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


class MuonPFCandidateProducer : public edm::EDProducer {

  public:

    explicit MuonPFCandidateProducer(const edm::ParameterSet & iConfig);
    ~MuonPFCandidateProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    edm::InputTag theMuonLabel_;

};



MuonPFCandidateProducer::MuonPFCandidateProducer(const edm::ParameterSet & iConfig) {
  theMuonLabel_ = iConfig.getParameter<edm::InputTag>("PATMuonCollection");
  produces<reco::PFCandidateCollection>("");
}


MuonPFCandidateProducer::~MuonPFCandidateProducer() {
}


void MuonPFCandidateProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<std::vector<pat::Muon> > muons;
  iEvent.getByLabel(theMuonLabel_, muons);

  std::auto_ptr<reco::PFCandidateCollection> mp(new reco::PFCandidateCollection());
  for (std::vector<pat::Muon>::const_iterator it = muons->begin(); it != muons->end(); ++it) {
    mp->push_back(*(it->pfCandidateRef()));
  }
  iEvent.put(mp);

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuonPFCandidateProducer);
