#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

const double defaultdPhiCutsArr[] = {0.4, 0.4, 0.4}; 

class DPhiFilter : public edm::EDFilter {

  public:

    explicit DPhiFilter(const edm::ParameterSet & iConfig);
    ~DPhiFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag metSrc_, jetSrc_;
    StringCutObjectSelector<reco::Jet,true> jetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    const std::vector<double> defaultdPhiCutsVec;
    const std::vector<double> dPhiCuts_;
    const unsigned int ndPhiCuts_;

    const bool debug_;

};


DPhiFilter::DPhiFilter(const edm::ParameterSet & iConfig) :
   jetCut_(iConfig.existsAs<std::string>("jetCut") ? iConfig.getParameter<std::string>("jetCut") : "", true),
   defaultdPhiCutsVec(defaultdPhiCutsArr, defaultdPhiCutsArr+sizeof(defaultdPhiCutsArr)/sizeof(defaultdPhiCutsArr[0])),
   dPhiCuts_(iConfig.getUntrackedParameter<std::vector<double> >("dPhiCuts", defaultdPhiCutsVec)),
   ndPhiCuts_(dPhiCuts_.size()),
   debug_(iConfig.getUntrackedParameter<bool>("debug", false))
{
  metSrc_ = iConfig.getParameter<edm::InputTag>("metSrc");
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
}


bool DPhiFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);

  if( debug_ ){
     printf("DEBUG ... %s :  met--> pt : %8.3f  eta : % 6.3f  phi : % 6.3f  mass : %8.3f\n", __FUNCTION__, (*met)[0].pt(), (*met)[0].eta(), (*met)[0].phi(), (*met)[0].mass());
  }

  bool passed = true;

  unsigned int cntSelJet =0;
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     if( debug_ ){
        printf("DEBUG ... %s :  jet  idx : %3d  pt : %8.3f  eta : % 6.3f  phi : % 6.3f  mass : %8.3f\n", __FUNCTION__, ij, jet.pt(), jet.eta(), jet.phi(), jet.mass());
     }
     if( !jetCut_(jet) ) continue;
     cntSelJet ++;
     if( cntSelJet <= ndPhiCuts_ ){
        passed = passed && (std::abs(reco::deltaPhi((*jets)[ij].phi(), (*met)[0].phi())) >= dPhiCuts_[cntSelJet-1]);
     }
  }

  return passed;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DPhiFilter);
