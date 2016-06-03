#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SusyAnaTools/SkimsAUX/plugins/common.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "TLorentzVector.h"

class nJetsForSkimsRA2 : public edm::EDFilter {

  public:

    explicit nJetsForSkimsRA2(const edm::ParameterSet & iConfig);
    ~nJetsForSkimsRA2() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag jetSrc_;
    edm::EDGetTokenT<edm::View<reco::Jet> > JetTok_;
    double minJetPt_;
    double maxJetEta_;

};


nJetsForSkimsRA2::nJetsForSkimsRA2(const edm::ParameterSet & iConfig) {
  jetSrc_ = iConfig.getParameter<edm::InputTag>("JetSource");
  minJetPt_    = iConfig.getParameter<double>("MinJetPt");
  maxJetEta_   = iConfig.getParameter<double>("MaxJetEta");
  JetTok_  = consumes<edm::View<reco::Jet> > (jetSrc_);
  produces<int>("nJets");
}


bool nJetsForSkimsRA2::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(JetTok_, jets);

  int nJets =0;
  for (edm::View<reco::Jet>::const_iterator it = jets->begin(); it != jets->end(); ++it) {
    if (it->pt() > minJetPt_ && fabs(it->eta()) < maxJetEta_) {
       nJets ++;
    }
  }

  std::auto_ptr<int> nJetsPtr(new int(nJets));
  iEvent.put(nJetsPtr, "nJets");

  return true;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(nJetsForSkimsRA2);
