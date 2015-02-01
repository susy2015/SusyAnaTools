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

class prodJets : public edm::EDFilter {

  public:

    explicit prodJets(const edm::ParameterSet & iConfig);
    ~prodJets();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag jetSrc_;
    edm::Handle<edm::View<reco::Jet> > jets;
    edm::Handle<std::vector<pat::Jet> > patjets;
    std::string bTagKeyString_;
    edm::InputTag vtxSrc_;
    edm::InputTag metSrc_;
    bool isPatJet;
    bool debug_;
};


prodJets::prodJets(const edm::ParameterSet & iConfig) {
  jetSrc_      = iConfig.getParameter<edm::InputTag>("jetSrc");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");
  metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");
  bTagKeyString_ = iConfig.getParameter<std::string>("bTagKeyString");

  debug_       = iConfig.getParameter<bool>("debug");

//  produces<std::vector<pat::Jet> >("");
  produces<std::vector<TLorentzVector> >("jetsLVec");
  produces<std::vector<int> >("recoJetsFlavor");
  produces<std::vector<double> >("recoJetsBtag");
  produces<int>("nJets");
}


prodJets::~prodJets() {
}


bool prodJets::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  iEvent.getByLabel(jetSrc_, jets);

  isPatJet = false;
  edm::ProductID jetProdID = jets.id();
  const edm::Provenance & jetProv = iEvent.getProvenance(jetProdID);
  const std::string jetclassName = jetProv.className();
  TString jetclassNameT(jetclassName);
  if( jetclassNameT.Contains("pat::Jet") ) isPatJet = true;

  if( isPatJet ) iEvent.getByLabel(jetSrc_, patjets);

  // read in the objects
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
//  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  // check which ones to keep
//  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
  std::auto_ptr<std::vector<TLorentzVector> > jetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<int> > recoJetsFlavor(new std::vector<int>());
  std::auto_ptr<std::vector<double> > recoJetsBtag(new std::vector<double>());

  for(unsigned int ij=0; ij < jets->size(); ij++){
     TLorentzVector perJetLVec;
     perJetLVec.SetPtEtaPhiE( (*jets)[ij].pt(), (*jets)[ij].eta(), (*jets)[ij].phi(), (*jets)[ij].energy() );
     jetsLVec->push_back(perJetLVec);
  }

  if( isPatJet ){
     for(unsigned int ij=0; ij < patjets->size(); ij++){
        const pat::Jet& jet = (*patjets)[ij];
        int flavor = jet.partonFlavour();
        recoJetsFlavor->push_back(flavor);

        double btag = jet.bDiscriminator(bTagKeyString_.c_str());
        recoJetsBtag->push_back(btag);
     }
  }

  std::auto_ptr<int> nJets (new int);

  *nJets = jetsLVec->size();

  // store in the event
//  iEvent.put(prod);
  iEvent.put(jetsLVec, "jetsLVec");
  iEvent.put(recoJetsFlavor, "recoJetsFlavor");
  iEvent.put(recoJetsBtag, "recoJetsBtag");
  iEvent.put(nJets, "nJets");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodJets);
