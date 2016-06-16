
#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/METReco/interface/MET.h"


class MHTProducer : public edm::EDProducer {

  public:

    explicit MHTProducer(const edm::ParameterSet & iConfig);
    ~MHTProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    edm::InputTag theJetLabel_;
    double minJetPt_;
    double maxJetEta_;
    edm::EDGetTokenT<edm::View<reco::Jet> >TheJetLabelTok_;
};



MHTProducer::MHTProducer(const edm::ParameterSet & iConfig) {
  theJetLabel_ = iConfig.getParameter<edm::InputTag>("JetCollection");
  minJetPt_    = iConfig.getParameter<double>("MinJetPt");
  maxJetEta_   = iConfig.getParameter<double>("MaxJetEta");
  TheJetLabelTok_ = consumes<edm::View<reco::Jet> >(theJetLabel_);
  produces<std::vector<reco::MET> >("");
  produces<double>("mht");
  produces<double>("mhtphi");
}


MHTProducer::~MHTProducer() {
}


void MHTProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(TheJetLabelTok_, jets);

  // calculate MHT
  std::auto_ptr<std::vector<reco::MET> > mhtp(new std::vector<reco::MET>());
  reco::MET::LorentzVector mht(0,0,0,0);
  for (edm::View<reco::Jet>::const_iterator it = jets->begin(); it != jets->end(); ++it) {
    if (it->pt() > minJetPt_ && fabs(it->eta()) < maxJetEta_) {
      mht -= it->p4();
    }
  }
  mhtp->push_back(reco::MET(mht, reco::MET::Point()));

  std::auto_ptr<double> mhtVal(new double);
  *mhtVal = mht.pt();

  std::auto_ptr<double> mhtphiVal(new double);
  *mhtphiVal = mht.phi();

  iEvent.put(mhtp);
  iEvent.put(mhtVal, "mht");
  iEvent.put(mhtphiVal, "mhtphi");

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MHTProducer);
