#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TLorentzVector.h"

typedef unsigned int size;

class BJetsSelector : public edm::EDFilter {

 public:
  
  explicit BJetsSelector(const edm::ParameterSet & iConfig);
  ~BJetsSelector();
  
 private:
  
  virtual bool   filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag jetSrc_;
  double        jetPtMin_;
  double        jetEtaMax_;
  std::string   btagname_;
  double        btagvalue_;
  bool          selecOnBJets_;
  bool          debug_;
  edm::EDGetTokenT<std::vector<pat::Jet> > JetTok_;
};


BJetsSelector::BJetsSelector(const edm::ParameterSet & iConfig) 
{
  jetSrc_      = iConfig.getParameter<edm::InputTag>("JetSrc");
  jetPtMin_    = iConfig.getParameter<double>       ("JetPtMin");
  jetEtaMax_   = iConfig.getParameter<double>       ("JetEtaMax");
  btagname_    = iConfig.getParameter<std::string>  ("bTagName");
  btagvalue_   = iConfig.getParameter<double>       ("bTagValue");
  selecOnBJets_= iConfig.getParameter<bool>         ("SelecOnBJets");
  debug_       = iConfig.getUntrackedParameter<bool>("debug", false);
  JetTok_  = consumes<std::vector<pat::Jet> > (jetSrc_);
  produces<std::vector<pat::Jet> >("");
}


BJetsSelector::~BJetsSelector() {
}

bool BJetsSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByToken(JetTok_, jets);
  
  // collection of b-tagged jets
  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
 
  int jetIdx =-1; 
  edm::View<pat::Jet>::const_iterator jetItr;
  for(jetItr = jets->begin(); jetItr != jets->end(); jetItr++){

    jetIdx++;

    const pat::Jet jet = (*jetItr);

    if( debug_ ){
       printf("DEBUG ... idx : %3d  pt : %8.3f  eta : % 6.3f  phi : % 6.3f  bdiscr : % 6.3f\n", jetIdx, jetItr->pt(), jetItr->eta(), jetItr->phi(), jetItr->bDiscriminator(btagname_.c_str()));
    }

    // kinematic cuts on jets
    const double jetPt = jetItr->pt(), jetEta = jetItr->eta();
    if(jetPt<jetPtMin_ || std::abs(jetEta)>jetEtaMax_) continue;
    
    // check if a b-jet
    double bdiscr = jetItr->bDiscriminator(btagname_.c_str()) ;
    if(bdiscr > btagvalue_) {
      prod->push_back((*jetItr));
    }
  }

  // store in the event
  iEvent.put(prod);
  
  //filter should return false if no b-jets found
  bool result = (selecOnBJets_ ? !(prod->size() == 0) : true);

  return result;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BJetsSelector);
