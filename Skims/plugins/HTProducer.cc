
#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"


class HTProducer : public edm::EDProducer {

  public:

    explicit HTProducer(const edm::ParameterSet & iConfig);
    ~HTProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    edm::InputTag theJetLabel_;
    double minJetPt_;
    double maxJetEta_;
    edm::EDGetTokenT<edm::View<reco::Jet> >TheJetLabelTok_;
};



HTProducer::HTProducer(const edm::ParameterSet & iConfig) {
  theJetLabel_ = iConfig.getParameter<edm::InputTag>("JetCollection");
  minJetPt_    = iConfig.getParameter<double>("MinJetPt");
  maxJetEta_   = iConfig.getParameter<double>("MaxJetEta");
  TheJetLabelTok_ = consumes<edm::View<reco::Jet> >(theJetLabel_);
  produces<double>("");
}


HTProducer::~HTProducer() {
}


void HTProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(TheJetLabelTok_, jets);

  // calculate MHT
  double ht = 0;
  for (edm::View<reco::Jet>::const_iterator it = jets->begin(); it != jets->end(); ++it) {
    if (it->pt() > minJetPt_ && fabs(it->eta()) < maxJetEta_) {
      ht += it->pt();
    }
  }

  std::unique_ptr<double> htp(new double(ht));
  iEvent.put(std::move(htp));

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(HTProducer);
