
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class HTFilter : public edm::EDFilter {

  public:

    explicit HTFilter(const edm::ParameterSet & iConfig);
    ~HTFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag htSrc_;
    double minHT_;

};


HTFilter::HTFilter(const edm::ParameterSet & iConfig) {
  htSrc_ = iConfig.getParameter<edm::InputTag>("HTSource");
  minHT_  = iConfig.getParameter<double>("MinHT");
}


bool HTFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  edm::Handle<double> ht;
  iEvent.getByLabel(htSrc_, ht);
  return (*ht > minHT_);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(HTFilter);
