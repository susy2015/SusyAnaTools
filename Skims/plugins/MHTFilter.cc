
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"


class MHTFilter : public edm::EDFilter {

  public:

    explicit MHTFilter(const edm::ParameterSet & iConfig);
    ~MHTFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag mhtSrc_;
    double minMHT_;
    edm::EDGetTokenT<edm::View<reco::MET> > MhtTok_;
};


MHTFilter::MHTFilter(const edm::ParameterSet & iConfig) {
  mhtSrc_ = iConfig.getParameter<edm::InputTag>("MHTSource");
  minMHT_  = iConfig.getParameter<double>("MinMHT");
  MhtTok_ = consumes<edm::View<reco::MET> >(mhtSrc_);
}


bool MHTFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the MHT
  edm::Handle<edm::View<reco::MET> > mht;
  iEvent.getByToken(MhtTok_, mht);

  return ((*mht)[0].pt() > minMHT_);

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MHTFilter);
