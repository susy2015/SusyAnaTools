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
    edm::InputTag metSrcMuCleanOnly_;
    edm::InputTag metSrcEGCleanOnly_;
    edm::InputTag metSrcNoClean_;

    bool debug_;

    bool isData_;

    edm::EDGetTokenT<edm::View<pat::MET> >MetTok_;
    edm::EDGetTokenT<edm::View<pat::MET> >MetTokMuCleanOnly_;
    edm::EDGetTokenT<edm::View<pat::MET> >MetTokEGCleanOnly_;
    edm::EDGetTokenT<edm::View<pat::MET> >MetTokNoClean_;

    std::vector<pat::MET::METUncertainty> uncUpList, uncDownList;
};


prodMET::prodMET(const edm::ParameterSet & iConfig) {

  isData_ = true;

  metSrc_            = iConfig.getParameter<edm::InputTag>("metSrc");
  metSrcMuCleanOnly_ = iConfig.getParameter<edm::InputTag>("metSrcMuCleanOnly");
  metSrcEGCleanOnly_ = iConfig.getParameter<edm::InputTag>("metSrcEGCleanOnly");
  metSrcNoClean_     = iConfig.getParameter<edm::InputTag>("metSrcNoClean");

  debug_       = iConfig.getParameter<bool>("debug");

  MetTok_             = consumes<edm::View<pat::MET> >(metSrc_);
  MetTokMuCleanOnly_  = consumes<edm::View<pat::MET> >(metSrcMuCleanOnly_);
  MetTokEGCleanOnly_  = consumes<edm::View<pat::MET> >(metSrcEGCleanOnly_);
  MetTokNoClean_      = consumes<edm::View<pat::MET> >(metSrcNoClean_);

  uncUpList = {pat::MET::JetResUp, pat::MET::JetEnUp, pat::MET::MuonEnUp, pat::MET::ElectronEnUp, pat::MET::TauEnUp, pat::MET::UnclusteredEnUp, pat::MET::PhotonEnUp};
  uncDownList = {pat::MET::JetResDown, pat::MET::JetEnDown, pat::MET::MuonEnDown, pat::MET::ElectronEnDown, pat::MET::TauEnDown, pat::MET::UnclusteredEnDown, pat::MET::PhotonEnDown};

  produces<double>("met");
  produces<double>("metphi");
  produces<double>("metMuCleanOnly");
  produces<double>("metphiMuCleanOnly");
  produces<double>("metEGCleanOnly");
  produces<double>("metphiEGCleanOnly");
  produces<double>("metNoClean");
  produces<double>("metphiNoClean");

  produces<double>("genmet");
  produces<double>("genmetphi");
  produces<double>("calomet");
  produces<double>("calometphi");
  produces<std::vector<double> >("metMagUp");
  produces<std::vector<double> >("metMagDown");
  produces<std::vector<double> >("metPhiUp");
  produces<std::vector<double> >("metPhiDown");
}


prodMET::~prodMET() {
}


bool prodMET::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData_ = false;

  edm::Handle<edm::View<pat::MET> > met;
  iEvent.getByToken(MetTok_, met);

  std::auto_ptr<double> metPtr(new double);
  std::auto_ptr<double> metphiPtr(new double);

  std::auto_ptr<double> genmetPtr(new double);
  std::auto_ptr<double> genmetphiPtr(new double);

  std::auto_ptr<double> calometPtr(new double);
  std::auto_ptr<double> calometphiPtr(new double);

  std::auto_ptr<std::vector<double> > metMagUp_ (new std::vector<double>(uncUpList.size(), 0.));
  std::auto_ptr<std::vector<double> > metPhiUp_ (new std::vector<double>(uncUpList.size(), 0.));
  std::auto_ptr<std::vector<double> > metMagDown_ (new std::vector<double>(uncDownList.size(), 0.));
  std::auto_ptr<std::vector<double> > metPhiDown_ (new std::vector<double>(uncDownList.size(), 0.));

  *metPtr = (*met)[0].pt();
  *metphiPtr = (*met)[0].phi();

  std::auto_ptr<double> metPtrMuCleanOnly(new double);
  std::auto_ptr<double> metphiPtrMuCleanOnly(new double);

  std::auto_ptr<double> metPtrEGCleanOnly(new double);
  std::auto_ptr<double> metphiPtrEGCleanOnly(new double);

  std::auto_ptr<double> metPtrNoClean(new double);
  std::auto_ptr<double> metphiPtrNoClean(new double);

  if(isData_){
    edm::Handle<edm::View<pat::MET> > metMuCleanOnly;
    iEvent.getByToken(MetTokMuCleanOnly_, metMuCleanOnly);

    edm::Handle<edm::View<pat::MET> > metEGCleanOnly;
    iEvent.getByToken(MetTokEGCleanOnly_, metEGCleanOnly);

    edm::Handle<edm::View<pat::MET> > metNoClean;
    iEvent.getByToken(MetTokNoClean_, metNoClean);

    *metPtrMuCleanOnly = (*metMuCleanOnly)[0].pt();
    *metphiPtrMuCleanOnly = (*metMuCleanOnly)[0].phi();

    *metPtrEGCleanOnly = (*metEGCleanOnly)[0].pt();
    *metphiPtrEGCleanOnly = (*metEGCleanOnly)[0].phi();

    *metPtrNoClean = (*metNoClean)[0].pt();
    *metphiPtrNoClean = (*metNoClean)[0].phi();

    if( metMuCleanOnly.isValid() ){
      *calometPtr = metMuCleanOnly->at(0).caloMETPt();
      *calometphiPtr = metMuCleanOnly->at(0).caloMETPhi();
    }
  }
  else{
    if( met.isValid() ){
      *calometPtr = met->at(0).caloMETPt();
      *calometphiPtr = met->at(0).caloMETPhi();
    }
  }

  for(unsigned int iu=0; iu<uncUpList.size(); ++iu){
    (*metMagUp_)[iu] = met->at(0).shiftedPt(uncUpList[iu], pat::MET::Type1);
    (*metPhiUp_)[iu] = met->at(0).shiftedPhi(uncUpList[iu], pat::MET::Type1);
  }
  for(unsigned int iu=0; iu<uncDownList.size(); ++iu){
    (*metMagDown_)[iu] = met->at(0).shiftedPt(uncDownList[iu], pat::MET::Type1);
    (*metPhiDown_)[iu] = met->at(0).shiftedPhi(uncDownList[iu], pat::MET::Type1);
  }

  if( met.isValid() && !isData_ ){
    if( met->at(0).genMET() ){
      const reco::GenMET * theGenMET(met->at(0).genMET());
      *genmetPtr = theGenMET->pt();
      *genmetphiPtr = theGenMET->phi();
    }
  }

  iEvent.put(metMagUp_, "metMagUp");
  iEvent.put(metPhiUp_, "metPhiUp");
  iEvent.put(metMagDown_, "metMagDown");
  iEvent.put(metPhiDown_, "metPhiDown");
  iEvent.put(metPtr, "met");
  iEvent.put(metphiPtr, "metphi");
  iEvent.put(metPtrMuCleanOnly, "metMuCleanOnly");
  iEvent.put(metphiPtrMuCleanOnly, "metphiMuCleanOnly");
  iEvent.put(metPtrEGCleanOnly, "metEGCleanOnly");
  iEvent.put(metphiPtrEGCleanOnly, "metphiEGCleanOnly");
  iEvent.put(metPtrNoClean, "metNoClean");
  iEvent.put(metphiPtrNoClean, "metphiNoClean");
  iEvent.put(genmetPtr, "genmet");
  iEvent.put(genmetphiPtr, "genmetphi");
  iEvent.put(calometPtr, "calomet");
  iEvent.put(calometphiPtr, "calometphi");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodMET);
