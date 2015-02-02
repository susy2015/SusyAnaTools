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

class NewHTFilter : public edm::EDFilter {

  public:

    explicit NewHTFilter(const edm::ParameterSet & iConfig);
    ~NewHTFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag jetSrc_;
    StringCutObjectSelector<reco::Jet,true> jetCutForHT_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    const double htCut_;
};


NewHTFilter::NewHTFilter(const edm::ParameterSet & iConfig) :
   jetCutForHT_(iConfig.existsAs<std::string>("jetCutForHT") ? iConfig.getParameter<std::string>("jetCutForHT") : "", true),
   htCut_(iConfig.getUntrackedParameter<double>("htCut", 400))
{
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
}


bool NewHTFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);

  bool passed = true;

  double newht =0;
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     if( !jetCutForHT_(jet) ) continue;
     newht += jet.pt();
  }

  if( newht < htCut_ ) passed = false;

  return passed;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NewHTFilter);
