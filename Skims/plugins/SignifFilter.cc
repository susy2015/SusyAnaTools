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

class SignifFilter : public edm::EDFilter {

  public:

    explicit SignifFilter(const edm::ParameterSet & iConfig);
    ~SignifFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag metSrc_, jetSrc_;
    StringCutObjectSelector<reco::Jet,true> jetCutForMHT_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    StringCutObjectSelector<reco::Jet,true> jetCutForHT_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    const double signifCut_;
// 0 : MET/sqrt(HT)
// 1 : MHT/sqrt(HT)
// 2 : (met)[0].significance()
// 3 : (mht)[0].significance()
    const int signifMethodSel_;
};


SignifFilter::SignifFilter(const edm::ParameterSet & iConfig) :
   jetCutForMHT_(iConfig.existsAs<std::string>("jetCutForMHT") ? iConfig.getParameter<std::string>("jetCutForMHT") : "", true),
   jetCutForHT_(iConfig.existsAs<std::string>("jetCutForHT") ? iConfig.getParameter<std::string>("jetCutForHT") : "", true),
   signifCut_(iConfig.getUntrackedParameter<double>("signifCut", 5)),
   signifMethodSel_(iConfig.getUntrackedParameter<int>("signifMethodSel", 0))
{
  metSrc_ = iConfig.getParameter<edm::InputTag>("metSrc");
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
}


bool SignifFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);

  double metVal = (*met)[0].pt();

  bool passed = false;

  double newht =0;
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     if( !jetCutForHT_(jet) ) continue;
     newht += jet.pt();
  }

  math::XYZTLorentzVector mhtLVec(0, 0, 0, 0);
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     if( !jetCutForMHT_(jet) ) continue;
     mhtLVec -= jet.p4();
  }
  double newmht = mhtLVec.pt();
 
// The default -1 is always less than the cut value
// since the physical cut should NOT be negative.
  double signif = -1;
  if( signifMethodSel_ == 0 ){
     if( newht != 0 ) signif = metVal/sqrt(newht);
  }else if( signifMethodSel_ == 1 ){
     if( newht !=0 ) signif = newmht/sqrt(newht);
// Option 2 & 3 are the same, only difference should be the input of "met" or "mht"
  }else if( signifMethodSel_ == 2 || signifMethodSel_ ==3 ){
      signif = (*met)[0].significance();
  }
 
  if( signif >= signifCut_ ) passed = true;

  return passed;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SignifFilter);
