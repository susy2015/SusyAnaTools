#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/METReco/interface/MET.h"

#include "TLorentzVector.h"

class prodMET : public edm::EDFilter {

  public:

    explicit prodMET(const edm::ParameterSet & iConfig);
    ~prodMET();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag metSrc_;
    bool debug_;
};


prodMET::prodMET(const edm::ParameterSet & iConfig) {
  metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");

  debug_       = iConfig.getParameter<bool>("debug");

  produces<double>("met");
  produces<double>("metphi");
}


prodMET::~prodMET() {
}


bool prodMET::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  std::auto_ptr<double> metPtr(new double);
  std::auto_ptr<double> metphiPtr(new double);

  *metPtr = (*met)[0].pt();
  *metphiPtr = (*met)[0].phi();

  iEvent.put(metPtr, "met");
  iEvent.put(metphiPtr, "metphi");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodMET);
