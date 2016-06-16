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

#include "DataFormats/Common/interface/Ptr.h"

class subtractMuons : public edm::EDProducer {

  public:

    explicit subtractMuons(const edm::ParameterSet & iConfig);
    ~subtractMuons();

  private:

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag oriMuonSrc_;
    edm::InputTag subSetMuonSrc_;

    edm::EDGetTokenT<std::vector<pat::Muon>> OriMuonTok_;
    edm::EDGetTokenT<std::vector<pat::Muon>> SubSetMuonTok_;

    double deltaR_;

    bool debug_;

    bool verbose_;
};


subtractMuons::subtractMuons(const edm::ParameterSet & iConfig) {

  oriMuonSrc_ = iConfig.getParameter<edm::InputTag>("oriMuonSrc");
  subSetMuonSrc_ = iConfig.getParameter<edm::InputTag>("subSetMuonSrc");

  OriMuonTok_ = consumes<std::vector<pat::Muon>>(oriMuonSrc_);
  SubSetMuonTok_ = consumes<std::vector<pat::Muon>>(subSetMuonSrc_);

  deltaR_ = iConfig.getParameter<double>("deltaR");

  produces<std::vector<pat::Muon> >("");

}


subtractMuons::~subtractMuons() {

}


void subtractMuons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<pat::Muon> > oriMuons;
  iEvent.getByToken(OriMuonTok_, oriMuons);

  edm::Handle<edm::View<pat::Muon> > subSetMuons;
  iEvent.getByToken(SubSetMuonTok_, subSetMuons);

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());

  int oriMuonSize = (int)oriMuons->size();
  int subSetMuonSize = (int)subSetMuons->size();

  for(int iom=0; iom<oriMuonSize; iom++){
     double oriMuonEta = oriMuons->at(iom).eta(), oriMuonPhi = oriMuons->at(iom).phi();
     double minDR = 9999.0; int minsubSetIdx = -1;
     for(int ism=0; ism<subSetMuonSize; ism++){
        double subSetMuonEta = subSetMuons->at(ism).eta(), subSetMuonPhi = subSetMuons->at(ism).phi();
        double deltaR = reco::deltaR(oriMuonEta, oriMuonPhi, subSetMuonEta, subSetMuonPhi);
        if( minDR > deltaR ){
           minDR = deltaR; minsubSetIdx = ism;
        }
     }
     if( minsubSetIdx ){/*empty to avoid a compiling error*/}
     if( minDR < deltaR_ ) continue;
     prod->push_back(pat::Muon(oriMuons->at(iom)));
  }

  // store in the event
  iEvent.put(prod);
}


#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(subtractMuons);
