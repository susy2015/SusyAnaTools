#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/View.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

class JESVariation : public edm::EDProducer {

public:
  explicit JESVariation(const edm::ParameterSet & iConfig);
  ~JESVariation();
  
  virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  static bool sort_jets(pat::Jet j1, pat::Jet j2);
  
  typedef math::XYZTLorentzVector LorentzVector;
  
private:
  edm::InputTag jetSrc_;
  std::string mPayloadName;
  std::string mUncertaintyTag_;
  std::string flagUpDn_;

  double jetMaxEtaCut_, jetMinPtCut_;

};


JESVariation::JESVariation(const edm::ParameterSet & iConfig) {
  jetSrc_          = iConfig.getParameter<edm::InputTag>("JetSource");
  mPayloadName     = iConfig.getParameter<std::string>("PayloadName");
  mUncertaintyTag_ = iConfig.getParameter<std::string>("UncertaintyTag");
  flagUpDn_        = iConfig.getParameter<std::string>("JECUpDown");
  jetMaxEtaCut_     = iConfig.getParameter<double>("jetMaxEtaCut");
  jetMinPtCut_     = iConfig.getParameter<double>("jetMinPtCut");

  produces<std::vector<pat::Jet> >("");
}


JESVariation::~JESVariation() {
}


void JESVariation::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  
  using namespace reco;
  
  /////////////////////////////
  // corrections and uncertainties
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get(mPayloadName,JetCorParColl); 
  JetCorrectorParameters const & JetCorPar = (*JetCorParColl)[mUncertaintyTag_];
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);
  
  //////////////////////

  double uncType = 0.0;
  if( flagUpDn_=="UP"   || flagUpDn_=="up"   || flagUpDn_=="Up"  ) uncType =  1.0;
  if( flagUpDn_=="Down" || flagUpDn_=="down" || flagUpDn_=="DOWN") uncType = -1.0;

  double uncertainty = 0.0;
  
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);
 
  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
  
  int ijet=0;
  for (edm::View<pat::Jet>::const_iterator j = jets->begin(); j != jets->end(); ++j, ijet++) {

    pat::Jet newJet = *j;
    
    jecUnc->setJetEta(j->eta());
    jecUnc->setJetPt(j->pt());
    uncertainty = jecUnc->getUncertainty(true);
    
    newJet.scaleEnergy(1.0 + uncType*uncertainty);

    if( (jetMaxEtaCut_ != -1 && fabs(newJet.eta()) > jetMaxEtaCut_) || (jetMinPtCut_ != -1 && newJet.pt() < jetMinPtCut_) ) continue;
    
    prod->push_back( newJet );
    
  }

  std::sort( (*prod).begin(), (*prod).end(),sort_jets );

  iEvent.put(prod);

  if( jecUnc ) delete jecUnc;
  
}

bool JESVariation::sort_jets(pat::Jet j1, pat::Jet j2) {
  return j1.pt() > j2.pt();
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(JESVariation);
