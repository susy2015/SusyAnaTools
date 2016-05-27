#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

class simpleJetSelector : public edm::EDFilter {

  public:

    explicit simpleJetSelector(const edm::ParameterSet & iConfig);
    ~simpleJetSelector() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag jetSrc_;
    edm::EDGetTokenT<std::vector<pat::Jet> > JetTok_;
    StringCutObjectSelector<pat::Jet,true> pfJetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
};


simpleJetSelector::simpleJetSelector(const edm::ParameterSet & iConfig) :
   pfJetCut_(iConfig.existsAs<std::string>("pfJetCut") ? iConfig.getParameter<std::string>("pfJetCut") : "", true)
{
  produces<std::vector<pat::Jet> >("");
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
  JetTok_  = consumes<std::vector<pat::Jet> > (jetSrc_);
}


bool simpleJetSelector::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByToken(JetTok_, jets);

  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());

  for(unsigned int ij=0; ij<jets->size(); ij++){
     const pat::Jet & jet = (*jets)[ij];
     if( !pfJetCut_(jet) ) continue;
     prod->push_back(jet);
  }

  iEvent.put(prod);

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(simpleJetSelector);
