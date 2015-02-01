
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class FilterBoolean : public edm::EDFilter {

  public:

    explicit FilterBoolean(const edm::ParameterSet & iConfig);
    ~FilterBoolean() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag boolSrc_;

};


FilterBoolean::FilterBoolean(const edm::ParameterSet & iConfig) {
  boolSrc_ = iConfig.getParameter<edm::InputTag>("ResultSource");
}


bool FilterBoolean::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<bool> result;
  iEvent.getByLabel(boolSrc_, result);

  return (*result);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FilterBoolean);
