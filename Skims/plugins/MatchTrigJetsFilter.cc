#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

const double     jetMinPtCutsArr[] = { 70, 70 };
const double jetMaxAbsEtaCutsArr[] = { -1, -1 };

class MatchTrigJetsFilter : public edm::EDFilter {

  public:

    explicit MatchTrigJetsFilter(const edm::ParameterSet & iConfig);
    ~MatchTrigJetsFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag jetSrc_;
    const std::vector<double> defaultJetMinPtCuts, jetMinPtCuts_;
    const std::vector<double> defaultJetMaxAbsEtaCuts, jetMaxAbsEtaCuts_;
    StringCutObjectSelector<reco::Jet,true> pfJetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class

    const unsigned int nJetCuts_;
};


MatchTrigJetsFilter::MatchTrigJetsFilter(const edm::ParameterSet & iConfig) :
   defaultJetMinPtCuts(jetMinPtCutsArr, jetMinPtCutsArr+sizeof(jetMinPtCutsArr)/sizeof(jetMinPtCutsArr[0])),
   jetMinPtCuts_(iConfig.getUntrackedParameter<std::vector<double> >("jetMinPtCuts", defaultJetMinPtCuts)),
   defaultJetMaxAbsEtaCuts(jetMaxAbsEtaCutsArr, jetMaxAbsEtaCutsArr+sizeof(jetMaxAbsEtaCutsArr)/sizeof(jetMaxAbsEtaCutsArr[0])),
   jetMaxAbsEtaCuts_(iConfig.getUntrackedParameter<std::vector<double> >("jetMaxAbsEtaCuts", defaultJetMaxAbsEtaCuts)),
   pfJetCut_(iConfig.existsAs<std::string>("pfJetCut") ? iConfig.getParameter<std::string>("pfJetCut") : "", true),
   nJetCuts_(jetMinPtCuts_.size())
{
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
}


bool MatchTrigJetsFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);

  bool passed = true;

  unsigned int cntSelJet =0;
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     if( !pfJetCut_(jet) ) continue; 
     cntSelJet ++; 
     if( cntSelJet <= nJetCuts_ ){
        if( jet.pt() < jetMinPtCuts_[cntSelJet-1] || (jetMaxAbsEtaCuts_[cntSelJet-1] != -1 && std::abs( jet.eta() ) > jetMaxAbsEtaCuts_[cntSelJet-1]) ) passed = false;
     }
  }

// This implies number of jets cut, because it has to be a match to the trigger requirement!
// For instance, we might use DiJet trigger where we require >=2 offline jets
  if( cntSelJet < nJetCuts_ ) passed = false;

  return passed;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MatchTrigJetsFilter);
