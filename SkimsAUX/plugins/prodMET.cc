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

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
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

    std::vector<pat::MET::METUncertainty> uncUpList, uncDownList;
};


prodMET::prodMET(const edm::ParameterSet & iConfig) {
  metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");

  debug_       = iConfig.getParameter<bool>("debug");

  uncUpList = {pat::MET::JetResUp, pat::MET::JetEnUp, pat::MET::MuonEnUp, pat::MET::ElectronEnUp, pat::MET::TauEnUp, pat::MET::UnclusteredEnUp, pat::MET::PhotonEnUp};
  uncDownList = {pat::MET::JetResDown, pat::MET::JetEnDown, pat::MET::MuonEnDown, pat::MET::ElectronEnDown, pat::MET::TauEnDown, pat::MET::UnclusteredEnDown, pat::MET::PhotonEnDown};

  produces<double>("met");
  produces<double>("metphi");
  produces<std::vector<double> >("metMagUp");
  produces<std::vector<double> >("metMagDown");
  produces<std::vector<double> >("metPhiUp");
  produces<std::vector<double> >("metPhiDown");
}


prodMET::~prodMET() {
}


bool prodMET::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<pat::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  std::auto_ptr<double> metPtr(new double);
  std::auto_ptr<double> metphiPtr(new double);

  std::auto_ptr<std::vector<double> > metMagUp_ (new std::vector<double>(uncUpList.size(), 0.));
  std::auto_ptr<std::vector<double> > metPhiUp_ (new std::vector<double>(uncUpList.size(), 0.));
  std::auto_ptr<std::vector<double> > metMagDown_ (new std::vector<double>(uncDownList.size(), 0.));
  std::auto_ptr<std::vector<double> > metPhiDown_ (new std::vector<double>(uncDownList.size(), 0.));

  *metPtr = (*met)[0].pt();
  *metphiPtr = (*met)[0].phi();

  for(unsigned int iu=0; iu<uncUpList.size(); ++iu){
     (*metMagUp_)[iu] = met->at(0).shiftedPt(uncUpList[iu], pat::MET::Type1);
     (*metPhiUp_)[iu] = met->at(0).shiftedPhi(uncUpList[iu], pat::MET::Type1);
  }
  for(unsigned int iu=0; iu<uncDownList.size(); ++iu){
     (*metMagDown_)[iu] = met->at(0).shiftedPt(uncDownList[iu], pat::MET::Type1);
     (*metPhiDown_)[iu] = met->at(0).shiftedPhi(uncDownList[iu], pat::MET::Type1);
  }

  iEvent.put(metMagUp_, "metMagUp");
  iEvent.put(metPhiUp_, "metPhiUp");
  iEvent.put(metMagDown_, "metMagDown");
  iEvent.put(metPhiDown_, "metPhiDown");
  iEvent.put(metPtr, "met");
  iEvent.put(metphiPtr, "metphi");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodMET);
