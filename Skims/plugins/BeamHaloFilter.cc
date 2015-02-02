
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"


class BeamHaloFilter : public edm::EDFilter {

  public:

    explicit BeamHaloFilter(const edm::ParameterSet & iConfig);
    ~BeamHaloFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    bool taggingMode_;    
};

BeamHaloFilter::BeamHaloFilter(const edm::ParameterSet & iConfig) {

  taggingMode_ = iConfig.getParameter<bool> ("taggingMode");

  produces<bool>();
}


bool BeamHaloFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<reco::BeamHaloSummary> beamHaloSummary;
  iEvent.getByLabel("BeamHaloSummary" , beamHaloSummary);

  bool result = !beamHaloSummary->CSCTightHaloId();

  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut); 

  if(taggingMode_) return true;
  else             return result; // return false if it is a beamhalo event
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BeamHaloFilter);
