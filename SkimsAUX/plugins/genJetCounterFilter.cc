
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

class genJetCounterFilter : public edm::EDFilter {

  public:

    explicit genJetCounterFilter(const edm::ParameterSet & iConfig);
    ~genJetCounterFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag genJetSrc_;
    int minJetNum_, maxJetNum_;

};


genJetCounterFilter::genJetCounterFilter(const edm::ParameterSet & iConfig) {
  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  minJetNum_ = iConfig.getParameter<int>("minJetNum");
  maxJetNum_ = iConfig.getParameter<int>("maxJetNum");
}


bool genJetCounterFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  edm::Handle<std::vector<reco::GenJet> > genJets;
  iEvent.getByLabel(genJetSrc_, genJets);
  return ( (int)genJets->size()>=minJetNum_ && (int)genJets->size()<maxJetNum_);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(genJetCounterFilter);
