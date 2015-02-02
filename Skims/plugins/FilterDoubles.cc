
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class FilterDoubles : public edm::EDFilter {

  public:

    explicit FilterDoubles(const edm::ParameterSet & iConfig);
    ~FilterDoubles() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag doubleSrc_;
//              lowCut_                 highCut_
// > b:            b        (dohighCut_ == false) 
// < a:         (dolowCut_ == false)      a
// >b && <a :      b                      a
    double lowCut_, highCut_;
    bool  dolowCut_, dohighCut_;

};


FilterDoubles::FilterDoubles(const edm::ParameterSet & iConfig) {
// Default values are -9999. If lowCut(higCut) is specified, its value will change anyway.
// Default values are not used because of the switch of dolowCut and dohighCut.
   lowCut_ = -9999; highCut_ = -9999;
   iConfig.existsAs<double>("lowCut") ? dolowCut_ = true, lowCut_ = iConfig.getParameter<double>("lowCut") : dolowCut_ = false;
   iConfig.existsAs<double>("highCut") ? dohighCut_ = true, highCut_ = iConfig.getParameter<double>("highCut") : dohighCut_ = false;
   doubleSrc_ = iConfig.getParameter<edm::InputTag>("ResultSource");
}


bool FilterDoubles::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<double> result;
  iEvent.getByLabel(doubleSrc_, result);

  bool passCut = true;
 
  if( ( dolowCut_ && (*result) <= lowCut_ ) || ( dohighCut_ && (*result) >= highCut_ ) ) passCut = false;

  return passCut;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(FilterDoubles);
