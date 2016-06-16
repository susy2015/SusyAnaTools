
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"


class JetMHTDPhiFilter : public edm::EDFilter {

  public:

    explicit JetMHTDPhiFilter(const edm::ParameterSet & iConfig);
    ~JetMHTDPhiFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag mhtSrc_, jetSrc_;
    edm::EDGetTokenT<edm::View<reco::MET> > MhtTok_;
    edm::EDGetTokenT<edm::View<reco::Jet> > JetRTok_;
};


JetMHTDPhiFilter::JetMHTDPhiFilter(const edm::ParameterSet & iConfig) {
  mhtSrc_ = iConfig.getParameter<edm::InputTag>("MHTSource");
  jetSrc_ = iConfig.getParameter<edm::InputTag>("JetSource");
  MhtTok_ = consumes<edm::View<reco::MET> >(mhtSrc_);
  JetRTok_ = consumes<edm::View<reco::Jet> >(jetSrc_);
}


bool JetMHTDPhiFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::MET> > mht;
  iEvent.getByToken(MhtTok_, mht);
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(JetRTok_, jets);

  bool passed = true;
  if (jets->size() >= 1) passed = passed && (std::abs(reco::deltaPhi((*jets)[0].phi(), (*mht)[0].phi())) > 0.5);
  if (jets->size() >= 2) passed = passed && (std::abs(reco::deltaPhi((*jets)[1].phi(), (*mht)[0].phi())) > 0.5);
  if (jets->size() >= 3) passed = passed && (std::abs(reco::deltaPhi((*jets)[2].phi(), (*mht)[0].phi())) > 0.3);

  return passed;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(JetMHTDPhiFilter);
