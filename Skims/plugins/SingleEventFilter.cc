
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class SingleEventFilter : public edm::EDFilter {

  public:

    explicit SingleEventFilter(const edm::ParameterSet & iConfig);
    ~SingleEventFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    unsigned int run_, lumi_, event_;

};


SingleEventFilter::SingleEventFilter(const edm::ParameterSet & iConfig) {
  run_   = iConfig.getParameter<unsigned int>("Run");
  lumi_  = iConfig.getParameter<unsigned int>("Lumi");
  event_ = iConfig.getParameter<unsigned int>("Event");
}


bool SingleEventFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  return (iEvent.id().run() == run_ &&
          iEvent.id().luminosityBlock() == lumi_ &&
          iEvent.id().event() == event_);

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SingleEventFilter);
