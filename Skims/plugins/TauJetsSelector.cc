#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/METReco/interface/MET.h"
#include "TLorentzVector.h"

typedef unsigned int size;

class TauJetsSelector : public edm::EDFilter {

 public:
  
  explicit TauJetsSelector(const edm::ParameterSet & iConfig);
  ~TauJetsSelector();
  
 private:
  
  virtual bool   filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
  TLorentzVector leadingTrack(const std::vector<reco::PFCandidatePtr> & pfcands);
  int            numChargedCand(const pat::Jet& jet, const std::vector<reco::PFCandidatePtr> & pfcands);
  double         LRM(const pat::Jet& jet, const std::vector<reco::PFCandidatePtr> & pfcands);

  edm::InputTag metSrc_;
  edm::InputTag jetSrc_;
  double        jetPtMin_;
  double        jetEtaMax_;
  std::string   btagname_;
  double        btagvalue_;
  bool          doTauVeto_;

};


TauJetsSelector::TauJetsSelector(const edm::ParameterSet & iConfig) 
{
  metSrc_    = iConfig.getParameter<edm::InputTag>("MetSrc");
  jetSrc_    = iConfig.getParameter<edm::InputTag>("JetSrc");
  jetPtMin_  = iConfig.getParameter<double>       ("JetPtMin");
  jetEtaMax_ = iConfig.getParameter<double>       ("JetEtaMax");
  btagname_  = iConfig.getParameter<std::string>  ("bTagName");
  btagvalue_ = iConfig.getParameter<double>       ("bTagValue");
  doTauVeto_ = iConfig.getParameter<bool>         ("DoTauVeto");
  produces<std::vector<pat::Jet> >("");
}


TauJetsSelector::~TauJetsSelector() {
}

bool TauJetsSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read met collection
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  // read jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);
  
  // collection of tau-like jets
  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
  
  // filter should return true if no tau jet is found
  bool noTauJet = true;

  edm::View<pat::Jet>::const_iterator jetItr;
  for(jetItr = jets->begin(); jetItr != jets->end(); jetItr++){

    const pat::Jet jet = (*jetItr);

    const double jetPt = jetItr->pt(), /*jetPhi = jetItr->phi(),*/ jetEta = jetItr->eta();

    // kinematic cuts on jets
    if(jetPt<jetPtMin_ || std::abs(jetEta)>jetEtaMax_) continue;
    
    // check if a b-jet
    double bdiscr = jetItr->bDiscriminator(btagname_.c_str()) ;
    if(bdiscr < btagvalue_) continue;
    
    const std::vector<reco::PFCandidatePtr> & constituents = jetItr->getPFConstituents();

    //get number of charged constituents of this jet
    int nCharged = numChargedCand(jet, constituents);
    if     ( jetPt>80.0 && nCharged>5 ) continue; 
    else if( jetPt<80.0 && nCharged>4 ) continue;
    
    // reject jets with large LRM
    double maxLRM = 0.05;
    if(jetPt<40.0) maxLRM = -0.000875 * jetPt + 0.0875;
    double lrm = LRM(jet, constituents);
    if(lrm > maxLRM ) continue;
    
    // add these jets to the event
    prod->push_back((*jetItr));
    
    // apply MT cuts for tau veto mode
    double metVal = (*met)[0].pt();
    double metPhi = (*met)[0].phi();
    double dPhi   = reco::deltaPhi(metPhi,jetItr->phi());
    double mt     = std::sqrt( 2*metVal*jetPt*(1-cos(dPhi)) );
    if(mt < 90.0) noTauJet = false;
  }

  // store in the event
  iEvent.put(prod);
  
  bool result = (doTauVeto_ ? noTauJet : true);
  
  return result;
}


int TauJetsSelector::numChargedCand(const pat::Jet& jet, const std::vector<reco::PFCandidatePtr> & pfcands){
  
  TLorentzVector leadTrk = leadingTrack(pfcands);
  
  int    nch  = 9999;
  double eta1 = jet.eta();
  double phi1 = jet.phi();

  // return if leading track is too away from jet 
  if(reco::deltaR(eta1,phi1,leadTrk.Eta(),leadTrk.Phi())<0.15) 
    return nch;
  
  nch = 0;
  std::vector<reco::PFCandidatePtr>::const_iterator itr;

  for(itr = pfcands.begin(); itr != pfcands.end(); itr++) {
    if((*itr)->particleId()==reco::PFCandidate::e  || 
       (*itr)->particleId()==reco::PFCandidate::mu || 
       (*itr)->particleId()==reco::PFCandidate::h ) {

      if((*itr)->pt()>1.0) { // min Pt be configurable
	double eta2 = (*itr)->eta();
	double phi2 = (*itr)->phi();
	double dR = reco::deltaR(eta1,phi1,eta2,phi2);
	if(dR<0.3) nch++;  // max dR can be configurable
      }
    }
  }

  return nch;
}

double TauJetsSelector::LRM(const pat::Jet& jet, const std::vector<reco::PFCandidatePtr> & pfcands) {
  // LRM : Linear Radial Momentum
  
  TLorentzVector leadTrk = leadingTrack(pfcands);
  double eta1 = leadTrk.Eta();
  double phi1 = leadTrk.Phi();

  double lrm = 0;

  // return if leading track is too away from jet 
  if( reco::deltaR(eta1,phi1,jet.eta(),jet.phi())<0.15 ) {
    lrm = 999.0;
    return lrm;
  }

  std::vector<reco::PFCandidatePtr>::const_iterator itr;
  for(itr = pfcands.begin(); itr != pfcands.end(); itr++) {
    if((*itr)->particleId()==reco::PFCandidate::e  || 
       (*itr)->particleId()==reco::PFCandidate::mu || 
       (*itr)->particleId()==reco::PFCandidate::h) 
      {
	//LRM = sum[ (PFContituentPt/JetPT)* dR wrt leading track in Jet]
	double eta2 = (*itr)->eta();
	double phi2 = (*itr)->phi();
	double dR = reco::deltaR(eta1,phi1,eta2,phi2);
	lrm += (*itr)->pt() * dR;
      }  
  }
  lrm = lrm/jet.pt();
  return lrm;
}


TLorentzVector TauJetsSelector::leadingTrack(const std::vector<reco::PFCandidatePtr> & pfcands) {
  
  TLorentzVector leadTrk(0.0,0.0,0.0,0.0);

  std::vector<reco::PFCandidatePtr>::const_iterator itr;

  for(itr = pfcands.begin(); itr != pfcands.end(); itr++) {
    if((*itr)->particleId()==reco::PFCandidate::e  || 
       (*itr)->particleId()==reco::PFCandidate::mu || 
       (*itr)->particleId()==reco::PFCandidate::h) 
      {
	if((*itr)->pt()>leadTrk.Pt()) {
	  leadTrk.SetPtEtaPhiE((*itr)->pt(),(*itr)->eta(),(*itr)->phi(),(*itr)->energy());
	}
      }
  }

  return leadTrk;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TauJetsSelector);
