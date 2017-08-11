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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CommonTools/Utils/interface/PtComparator.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "TLorentzVector.h"

class prodGenJets : public edm::EDFilter 
{
 public:
  explicit prodGenJets(const edm::ParameterSet & iConfig);
  ~prodGenJets();

 private:
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag genjetSrc_;
  // All have to be pat::Jet, otherwise cannot get b-tagging information!
  edm::Handle<std::vector<reco::GenJet> > genjets;
  edm::EDGetTokenT<std::vector<reco::GenJet> > GenJetTok_;
};


prodGenJets::prodGenJets(const edm::ParameterSet & iConfig) 
{
	genjetSrc_ = iConfig.getParameter<edm::InputTag>("genjetSrc");
	produces<std::vector<TLorentzVector> >("genjetsLVec");
        GenJetTok_ = consumes<std::vector<reco::GenJet> >(genjetSrc_);
	//produces<std::vector<int> >("genJetsFlavor");
}

prodGenJets::~prodGenJets() 
{
}

bool prodGenJets::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  iEvent.getByToken(GenJetTok_, genjets);

  // check which ones to keep
	std::unique_ptr<std::vector<TLorentzVector> > genjetsLVec(new std::vector<TLorentzVector>());
  //std::unique_ptr<std::vector<int> > genJetsFlavor(new std::vector<int>());

  for(unsigned int io=0; io < genjets->size(); io++)
	{
		TLorentzVector perJetLVec;
		perJetLVec.SetPtEtaPhiE( genjets->at(io).pt(), genjets->at(io).eta(), genjets->at(io).phi(), genjets->at(io).energy() );
    genjetsLVec->push_back(perJetLVec);

		//int flavor = genjets->at(io).partonFlavour();
	  //genJetsFlavor->push_back(flavor);
  }

  //store in the event
	iEvent.put(std::move(genjetsLVec), "genjetsLVec");
  //iEvent.put(genJetsFlavor, "genJetsFlavor");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodGenJets);
