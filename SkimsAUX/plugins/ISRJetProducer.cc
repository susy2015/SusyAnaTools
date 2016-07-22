// system include files                                                                               
#include <memory>
#include <algorithm>
#include <iostream>
#include <set>

// user include files                                                                                 
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

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"

#include "TLorentzVector.h"


class ISRJetProducer : public edm::EDFilter {

public:

  explicit ISRJetProducer(const edm::ParameterSet & iConfig);
  ~ISRJetProducer();

private:

  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag genParticleSrc_;
  edm::Handle<edm::View<reco::GenParticle > > genParticles;
  edm::EDGetTokenT<edm::View<reco::GenParticle > > GenParticleTok_;

  edm::InputTag cleanJetSrc_;
  edm::Handle<std::vector<pat::Jet> > cleanJet;
  edm::EDGetTokenT<std::vector<pat::Jet> >cleanJetTok_;

  bool debug_;

  int nisrMatch(edm::Handle<edm::View<reco::GenParticle > > genParticles, edm::Handle<std::vector<pat::Jet> > clean_jets);
};


ISRJetProducer::ISRJetProducer(const edm::ParameterSet & iConfig) {

  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");
  cleanJetSrc_ = iConfig.getParameter<edm::InputTag>("cleanJetSrc");
  debug_       = iConfig.getParameter<bool>("debug");

  GenParticleTok_ =consumes<edm::View<reco::GenParticle > >(genParticleSrc_);
  cleanJetTok_ = consumes<std::vector<pat::Jet> >(cleanJetSrc_);


  produces<int>("NJetsISR");


}


ISRJetProducer::~ISRJetProducer() {
}

bool ISRJetProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( iEvent.isRealData() ) return true;


  iEvent.getByToken(GenParticleTok_, genParticles);
  iEvent.getByToken(cleanJetTok_, cleanJet);

  std::auto_ptr<int> NJetsISR (new int);
  *NJetsISR = nisrMatch(genParticles, cleanJet);

  iEvent.put(NJetsISR, "NJetsISR");

  return true;
}

int ISRJetProducer::nisrMatch(edm::Handle<edm::View<reco::GenParticle > > genParticles, edm::Handle<std::vector<pat::Jet> > clean_jets)
{
  bool verbose = false;
  int nisr(0);
  for (size_t ijet(0); ijet<clean_jets->size(); ijet++){


    bool matched=false;
    for (size_t imc(0); imc < genParticles->size(); imc++) {
      if (matched) break;
      const reco::GenParticle &mc = (*genParticles)[imc];
      if (mc.status()!=23 || abs(mc.pdgId())>5) continue;
      int momid = abs(mc.mother()->pdgId());
      if(!(momid==6 || momid==23 || momid==24 || momid==25 || momid>1e6)) continue;
      //check against daughter in case of hard initial splitting                                   
      for (size_t idau(0); idau < mc.numberOfDaughters(); idau++) {
	float dR = deltaR(clean_jets->at(ijet), mc.daughter(idau)->p4());
	if(dR<0.3){
	  if (verbose) {
	    std::cout<<"Jet: ("<<clean_jets->at(ijet).pt()<<", "<<clean_jets->at(ijet).eta()<<", "\
		     <<clean_jets->at(ijet).phi() <<"), MC: ("<<mc.daughter(idau)->pt()<<", "<<mc.daughter(idau)->eta()<<", "<<mc.daughter(idau)->phi()<<"), ID "<<mc.daughter(idau)->pdgId()<<"-> dR "<<dR <<std::endl;
	  }
	  matched = true;
	  break;
	}
      }
    } // Loop over MC particles                                                                    
    if(!matched) {

      if(clean_jets->at(ijet).pt() > 30.  &&  std::abs(clean_jets->at(ijet).eta()) < 2.4) nisr++;
    }

  }
  return nisr;
}




#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ISRJetProducer);





























