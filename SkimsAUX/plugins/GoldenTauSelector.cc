#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/METReco/interface/MET.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "TTree.h"
#include "TLorentzVector.h"

typedef unsigned int size;

class GoldenTauSelector : public edm::EDFilter {

  public:

    explicit GoldenTauSelector(const edm::ParameterSet & iConfig);
    ~GoldenTauSelector();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag jetSrc_;
    StringCutObjectSelector<reco::Jet,true> pfJetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class

    edm::InputTag metSrc_;

// MT is calculated using jet and met
// If MTcut_ is -1, no MT cut is applied
// XXX: event with tau-like jets usually has MT = sqrt(2*pt*MET*(1-cos(Dphi))) < 100 GeV.
    double MTCut_;
// dPhiJetMET is the deltaPhi between jet and met
// -1 means no deltaPhi cut
// A W->tau nu decay usually has a deltaPhi < pi/5 ~ 0.628 
    double dPhiJetMETCut_;

// nTrackesCut is the number of charged tracks requirement in the jet
// -1 means no cuts
// e.g.,   [4,  5]
    std::vector<int> nTracksCut_;
// ptDivFornTracksCut is the divide range of pt that different number
// of tracks cuts should be used. -1 means infinite.
// e.g., [0, 80, -1]
    std::vector<double> ptDivFornTracksCut_;

// Vertex information (in case we need it)
   edm::InputTag vtxSrc_;
   double maxD0_, maxDz_;

// Veto tau jet?
   bool doTauVeto_;

   double deltaRTrkCone_, deltaRleadingChgPartToJetCut_;

// TODO: Add in isolation requirement ??

   bool doFillTree_;
   void setTreeDefaultVars();
   TTree *auxTree;

   size run, event, lumi;
   std::vector<int> *ntracks_TR; 
   std::vector<double> *MT_TR, *dPhi_TR, *pt_TR;
};


GoldenTauSelector::GoldenTauSelector(const edm::ParameterSet & iConfig) :
   pfJetCut_(iConfig.existsAs<std::string>("pfJetSelection") ? iConfig.getParameter<std::string>("pfJetSelection") : "", true)
{
  jetSrc_             = iConfig.getParameter<edm::InputTag>("jetSrc");
  metSrc_             = iConfig.getParameter<edm::InputTag>("metSrc");
  vtxSrc_             = iConfig.getParameter<edm::InputTag>("vtxSrc");

  MTCut_              = iConfig.getParameter<double>("MTCut");
  dPhiJetMETCut_      = iConfig.getParameter<double>("dPhiJetMETCut");

  nTracksCut_         = iConfig.getParameter<std::vector<int> >("nTracksCut");
  ptDivFornTracksCut_ = iConfig.getParameter<std::vector<double> >("ptDivFornTracksCut");

  maxD0_              = iConfig.getParameter<double>("maxD0");
  maxDz_              = iConfig.getParameter<double>("maxDz");

  doFillTree_         = iConfig.getParameter<bool>("doFillTree");

  doTauVeto_          = iConfig.getParameter<bool>("doTauVeto");

  deltaRTrkCone_      = iConfig.getParameter<double>("deltaRTrkCone");
  deltaRleadingChgPartToJetCut_ = iConfig.getParameter<double>("deltaRleadingChgPartToJetCut");

  ntracks_TR = new std::vector<int>; 
  MT_TR = new std::vector<double>; dPhi_TR = new std::vector<double>; pt_TR = new std::vector<double>;

  setTreeDefaultVars();

  if( doFillTree_ ){
     edm::Service<TFileService> fs;
     
     auxTree = fs->make<TTree>("AUX", "aux info");
     auxTree->Branch("run", &run, "run/I");
     auxTree->Branch("event", &event, "event/I");
     auxTree->Branch("lumi", &lumi, "lumi/I");
     auxTree->Branch("ntracks", "std::vector<int>", &ntracks_TR, 32000, 0);
     auxTree->Branch("MT", "std::vector<double>", &MT_TR, 32000, 0);
     auxTree->Branch("dPhi", "std::vector<double>", &dPhi_TR, 32000, 0);
     auxTree->Branch("pt", "std::vector<double>", &pt_TR, 32000, 0);
  }

  produces<std::vector<pat::Jet> >("");
}


GoldenTauSelector::~GoldenTauSelector() {
}

bool GoldenTauSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  setTreeDefaultVars();

  // read in the objects
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);
  const double MET = (*met)[0].pt(), METphi = (*met)[0].phi();
   
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());

  edm::View<pat::Jet>::const_iterator jetItr;
  unsigned ijet;
  if( vertices->size() > 0) {
     for(jetItr = jets->begin(), ijet=0; jetItr != jets->end(); jetItr++, ijet++){
        const reco::Jet jet = (*jetItr);
        if( !pfJetCut_(jet) ) continue;
   
        const double jetPt = jetItr->pt(), jetPhi = jetItr->phi(), jetEta = jetItr->eta();
    
        const std::vector<reco::PFCandidatePtr> & constituents = jetItr->getPFConstituents();
        const size numConstituents = constituents.size();
   
        int leadingChgParticleIdx = -1;
        double leadingPt = -1;
        std::vector<TLorentzVector> cachedTracks;
        for(size icon=0; icon<numConstituents; icon++){
           const reco::PFCandidatePtr& constituent = constituents[icon];
           if( constituent->charge() == 0 ) continue;
           if( constituent->pt() < 1 ) continue;
           if( !constituent->trackRef().isNonnull() ) continue;
           if( std::abs(constituent->trackRef()->dxy(vtxpos)) >= maxD0_ ) continue;
           if( std::abs(constituent->trackRef()->dz(vtxpos))  >= maxDz_ ) continue;

           TLorentzVector tracksLVec;
           tracksLVec.SetPtEtaPhiE( constituent->pt(), constituent->eta(), constituent->phi(), constituent->energy() );
           cachedTracks.push_back(tracksLVec);

           if( leadingChgParticleIdx == -1 ){
              leadingChgParticleIdx = (int)cachedTracks.size()-1;
              leadingPt = constituent->pt();
           }else if( leadingPt < constituent->pt() ){
              leadingChgParticleIdx = (int)cachedTracks.size()-1;
              leadingPt = constituent->pt();
           }
        }
        if( !cachedTracks.empty() && leadingChgParticleIdx == -1 ){ std::cout<<"WARNING ... no leadingChgParticle found?!"<<std::endl; }

        int ntracksCnt = 0; 
        for(size ip=0; ip<cachedTracks.size(); ip++){
           if( (int)ip == leadingChgParticleIdx ){ ntracksCnt++; continue; }
           const double deltaR = reco::deltaR(cachedTracks[ip].Eta(), cachedTracks[ip].Phi(), cachedTracks[leadingChgParticleIdx].Eta(), cachedTracks[leadingChgParticleIdx].Phi());
           if( deltaR > deltaRTrkCone_ ) continue;
           ntracksCnt ++;
        }
 
        double deltaRleadingChgPartToJet = 99.0;
        if( leadingChgParticleIdx != -1 ) deltaRleadingChgPartToJet = reco::deltaR(jetEta, jetPhi, cachedTracks[leadingChgParticleIdx].Eta(), cachedTracks[leadingChgParticleIdx].Phi());
        if( deltaRleadingChgPartToJetCut_ != -1 && deltaRleadingChgPartToJet > deltaRleadingChgPartToJetCut_ ) continue;   

        int pickedCutIdx = -1;
        for(size ip=0; ip<ptDivFornTracksCut_.size()-1; ip++){
           if( jetPt > ptDivFornTracksCut_[ip] && ( ptDivFornTracksCut_[ip+1] == -1 || jetPt < ptDivFornTracksCut_[ip+1] ) ){
              pickedCutIdx = ip; break;
           }
        }
        if( nTracksCut_[pickedCutIdx] != -1 && (ntracksCnt > nTracksCut_[pickedCutIdx] || ntracksCnt ==0) ) continue;
        
        const double dPhiJetMET = std::abs(reco::deltaPhi(jetPhi, METphi));
        if( dPhiJetMETCut_ != -1 && dPhiJetMET > dPhiJetMETCut_ ) continue;
   
        const double MT = sqrt( 2*(MET*jetPt - (MET*cos(METphi)*jetItr->px() + MET*sin(METphi)*jetItr->py()) ) );
        if( MTCut_ != -1 && MT > MTCut_ ) continue;
   
        ntracks_TR->push_back(ntracksCnt);
        MT_TR->push_back(MT);
        dPhi_TR->push_back(dPhiJetMET);
        pt_TR->push_back(jetPt);
   
        prod->push_back((*jetItr));
     } 
  }

  if( doFillTree_ ){
// Get run, event, lumi info
     run = iEvent.id().run();
     event = iEvent.id().event();
     lumi = iEvent.luminosityBlock();

     auxTree->Fill();
  }

  // determine result before losing ownership of the pointer
  bool result = (doTauVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);

  return result;
}

void GoldenTauSelector::setTreeDefaultVars(){
   ntracks_TR->clear(); MT_TR->clear(); dPhi_TR->clear(); pt_TR->clear();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GoldenTauSelector);
