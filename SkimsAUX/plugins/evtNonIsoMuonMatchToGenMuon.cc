
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "TLorentzVector.h"

class evtNonIsoMuonMatchToGenMuon : public edm::EDFilter {

  public:

    explicit evtNonIsoMuonMatchToGenMuon(const edm::ParameterSet & iConfig);
    ~evtNonIsoMuonMatchToGenMuon();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    virtual void loadGenParticles(const edm::Event& iEvent);
    virtual void loadGenJets(const edm::Event& iEvent);

    const std::vector<TLorentzVector> & searchGenSigChnForAcc(int *signWptr=0, std::vector<TLorentzVector> *bosonVecPtr = 0);
    const std::vector<TLorentzVector> & searchGenEleSigChnForAcc(int *signWptr=0, std::vector<TLorentzVector> *bosonVecPtr = 0);

    edm::InputTag jetSrc_;
    edm::InputTag muonSrc_;

    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;
    double deltaR_;

    double maxMuRelIso_;

    bool debug_;

    bool doFilter_;

};


evtNonIsoMuonMatchToGenMuon::evtNonIsoMuonMatchToGenMuon(const edm::ParameterSet & iConfig) {
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSource");
  muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSource");

  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");
  deltaR_ = iConfig.getParameter<double>("deltaR");

  maxMuRelIso_ = iConfig.getParameter<double>("maxMuRelIso");

  debug_ = iConfig.getParameter<bool>("debug");

  doFilter_ = iConfig.getParameter<bool>("doFilter");

  produces<std::vector<pat::Muon> >("");
}


evtNonIsoMuonMatchToGenMuon::~evtNonIsoMuonMatchToGenMuon() {
}


bool evtNonIsoMuonMatchToGenMuon::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  bool pass = true;

  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());

  bool isData = true;

  if( !iEvent.isRealData() ) isData = false;

  std::vector<TLorentzVector> WJetsAccVec;

  if( !isData ){
     loadGenParticles(iEvent);
     loadGenJets(iEvent);

     int signW = 0; std::vector<TLorentzVector> Wvec;
     WJetsAccVec = searchGenSigChnForAcc(&signW, &Wvec);

     for (edm::View<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {
        // isolation cuts -- copied from SandBox/Skims/plugins/GoodMuonSelector.cc
        float muRelIso = -1;
        if (m->pfCandidateRef().isNull()) { // check wether this is a std or PF muon
          muRelIso = (m->isolationR03().sumPt+m->isolationR03().emEt+m->isolationR03().hadEt) / m->innerTrack()->pt();
        } else {
          muRelIso = (m->chargedHadronIso()+m->photonIso()+m->neutralHadronIso()) / m->pt();
        }
        double perEta = m->eta(), perPhi = m->phi();
        if( !WJetsAccVec.empty() ){
           double mu1eta = WJetsAccVec.front().Eta(), mu1phi = WJetsAccVec.front().Phi();
           const double dR = reco::deltaR(mu1eta, mu1phi, perEta, perPhi);
           // matching && not isolated!!
           if( dR< deltaR_ && muRelIso > maxMuRelIso_ ){ prod->push_back(pat::Muon(*m)); pass = false; }
        }
     }
  }

  if( WJetsAccVec.empty() && prod->size() != 0 ){ std::cout<<"Error NO WJets found but product size not ZERO?!"<<std::endl; }

  iEvent.put(prod);

// Well, do NOT do the filter...
  if( !doFilter_ ) return true;

// No Gen Muon to match for data, so empty product and return true
  if( isData ) return true;

  return pass; 

}


 const std::vector<TLorentzVector> & evtNonIsoMuonMatchToGenMuon::searchGenEleSigChnForAcc(int *signWptr, std::vector<TLorentzVector> *bosonVecPtr){
          
    static std::vector<TLorentzVector> eleCached; eleCached.clear();
    if( bosonVecPtr ) bosonVecPtr->clear();
    
    if( signWptr ) *signWptr = 0; // default value
          
    for(unsigned int ig=0; ig<genParticles->size(); ig++){
             
       const reco::GenParticle& gen = genParticles->at(ig);
             
       if( abs(gen.pdgId()) == 24 ){ // W+ : 24   W- : -24  (W+->mu+ nu_mu)
          int signW = gen.pdgId()/abs(gen.pdgId()); if( signWptr ) *signWptr = signW;
          unsigned int numDaug = gen.numberOfDaughters();
          int numEleMatched =0, numNeutrinoMatched =0;
          for(unsigned int id=0; id<numDaug; id++){
             int daupdgId = gen.daughter(id)->pdgId();
             int status = gen.daughter(id)->status(); // status : 3 (stable particle?)
             if( status ==3 && (daupdgId == -signW*11 /*e+ : -11  e- : 11*/ || daupdgId == signW*12 /*nu_e : 12  anti-nu_e : -12*/) ){
                if( daupdgId == -signW*11 ){
                   TLorentzVector daug; daug.SetPxPyPzE(gen.daughter(id)->px(), gen.daughter(id)->py(), gen.daughter(id)->pz(), gen.daughter(id)->energy());
                   eleCached.push_back( daug );
                   numEleMatched ++;
                }
                if( daupdgId == signW*12 ) numNeutrinoMatched ++;
             }
          }
          if( numEleMatched ==1 && numNeutrinoMatched == 1 ){
             TLorentzVector boson; boson.SetPxPyPzE(gen.px(), gen.py(), gen.pz(), gen.energy());
             if( bosonVecPtr ) bosonVecPtr->push_back(boson);
             return eleCached;
          }
       }
    }

    eleCached.clear();
    return eleCached;

 }


 const std::vector<TLorentzVector> & evtNonIsoMuonMatchToGenMuon::searchGenSigChnForAcc(int *signWptr, std::vector<TLorentzVector> *bosonVecPtr){

    static std::vector<TLorentzVector> muonCached; muonCached.clear();
    if( bosonVecPtr ) bosonVecPtr->clear();

    if( signWptr ) *signWptr = 0; // default value

    for(unsigned int ig=0; ig<genParticles->size(); ig++){

       const reco::GenParticle& gen = genParticles->at(ig);

       if( abs(gen.pdgId()) == 24 ){ // W+ : 24   W- : -24  (W+->mu+ nu_mu)
          int signW = gen.pdgId()/abs(gen.pdgId()); if( signWptr ) *signWptr = signW;
          unsigned int numDaug = gen.numberOfDaughters();
          int numMuonMatched =0, numNeutrinoMatched =0;
          for(unsigned int id=0; id<numDaug; id++){
             int daupdgId = gen.daughter(id)->pdgId();
             int status = gen.daughter(id)->status(); // status : 3 (stable particle?)
             if( status ==3 && (daupdgId == -signW*13 /*mu+ : -13  mu- : 13*/ || daupdgId == signW*14 /*nu_mu : 14  anti-nu_mu : -14*/) ){
                if( daupdgId == -signW*13 ){
                   TLorentzVector daug; daug.SetPxPyPzE(gen.daughter(id)->px(), gen.daughter(id)->py(), gen.daughter(id)->pz(), gen.daughter(id)->energy());
                   muonCached.push_back( daug );
                   numMuonMatched ++;
                }
                if( daupdgId == signW*14 ) numNeutrinoMatched ++;
             }
          }
          if( numMuonMatched ==1 && numNeutrinoMatched == 1 ){
             TLorentzVector boson; boson.SetPxPyPzE(gen.px(), gen.py(), gen.pz(), gen.energy());
             if( bosonVecPtr ) bosonVecPtr->push_back(boson);
             return muonCached;
          }
       }
    }

    muonCached.clear();
    return muonCached;

 }

 void evtNonIsoMuonMatchToGenMuon::loadGenParticles(const edm::Event& iEvent){
    iEvent.getByLabel(genParticleSrc_, genParticles);
 }
 void evtNonIsoMuonMatchToGenMuon::loadGenJets(const edm::Event& iEvent){
    iEvent.getByLabel(genJetSrc_, genJets);
 }


#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(evtNonIsoMuonMatchToGenMuon);
