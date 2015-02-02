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

//#include "recipeAUX/Muon/interface/RadialIsoCalculator.h"
//#include "recipeAUX/Muon/interface/RadialMVAIsoCalculator.h"
#include "recipeAUX/Muon/interface/MuonEffectiveArea.h"

#include "TTree.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

typedef unsigned int size;
//class RadialIsoCalculator;

class StudyMuonIsolation : public edm::EDFilter {

  public:

    explicit StudyMuonIsolation(const edm::ParameterSet & iConfig);
    ~StudyMuonIsolation();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    void beginJob();

    edm::InputTag muonSrc_;
    bool doMuonVeto_, doMuonID_, doMuonVtx_, doMuonIso_;
    double minMuPt_, maxMuEta_, maxMuD0_, maxMuDz_, maxMuRelIso_, minMuNumHit_;

    double matchDR_;
   
//    RadialIsoCalculator*    RadialCutBasedCalculator;
    edm::ParameterSet radialCalPSet_;
//    RadialMVAIsoCalculator* RadialMVACalculator;

// 0 : Traditional relative iso as last year --> sumChargedHadronPt depends on if PFnoPU is used or not
// 1 : Official recommended DeltaBeta PU correction
// 2 : Effective Area PU correction
// 3 : Radial PU correction
    int muonIsoSel_;
    std::vector<double> *isoVal0, *isoVal1, *isoVal2, *isoVal3;
    std::vector<int> *muonMomStatus;

    size run, event, lumi; bool isData;
    edm::InputTag vtxSrc_;
    edm::Handle<edm::View<reco::Vertex> > vertices;
    size nVtxPUcut_, vtxSize;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

    edm::InputTag rhoSrc_;
    edm::Handle<double> rho_;

    bool doFillTree_;
    void setTreeDefaultVars();
    TTree *auxTree;
    bool studyMode_;

    bool find_mother(const reco::Candidate* genp, const int motherID);
    bool find_mother(const reco::Candidate* genp, const reco::Candidate &target);

    edm::InputTag dPhis_CUT_vec_Src_;
    edm::Handle<std::vector<double> > dPhis_CUT_vec_;
    edm::InputTag nJets_CUT_Src_;
    edm::Handle<int> nJets_CUT_;
    edm::InputTag externalBitToTree_Src_;
    edm::Handle<int> externalBitToTree_;
    virtual void loadAUX(const edm::Event& iEvent);

    double dPhi0_CUT, dPhi1_CUT, dPhi2_CUT;
    int nJets_CUT;
    int externalBitToTree_TR;

    edm::InputTag metSrc_;
    edm::Handle<edm::View<reco::MET> > met;
    edm::InputTag mhtSrc_, mht_forSgnfSrc_;
    edm::Handle<edm::View<reco::MET> > mht, mht_forSgnf;
    virtual void loadMETMHT(const edm::Event& iEvent);

    edm::InputTag htSrc_;
    edm::Handle<double> ht;
    virtual void loadHT(const edm::Event& iEvent);

    double mht_TR, ht_TR, met_TR;

    edm::InputTag jetSrc_;
    edm::Handle<edm::View<reco::Jet> > jets;
    size nJets;
    virtual void loadRecoJets(const edm::Event& iEvent);

};


StudyMuonIsolation::StudyMuonIsolation(const edm::ParameterSet & iConfig) {

  isData = true;
  
  muonSrc_     = iConfig.getParameter<edm::InputTag>("MuonSource");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");
  minMuPt_     = iConfig.getParameter<double>("MinMuPt");
  maxMuEta_    = iConfig.getParameter<double>("MaxMuEta");
  maxMuD0_     = iConfig.getParameter<double>("MaxMuD0");
  maxMuDz_     = iConfig.getParameter<double>("MaxMuDz");
  maxMuRelIso_ = iConfig.getParameter<double>("MaxMuRelIso");
  minMuNumHit_ = iConfig.getParameter<double>("MinMuNumHit");
  doMuonVeto_  = iConfig.getParameter<bool>("DoMuonVeto");
  doMuonID_    = iConfig.getParameter<bool>("DoMuonID");
  doMuonVtx_   = iConfig.getParameter<bool>("DoMuonVtxAssociation");
  doMuonIso_   = iConfig.getParameter<bool>("DoMuonIsolation");
  rhoSrc_      = iConfig.getParameter<edm::InputTag>("rhoSrc");

  matchDR_     = iConfig.getParameter<double>("matchDR");

  muonIsoSel_  = iConfig.getParameter<int>("muonIsoSel");

  radialCalPSet_ = iConfig.getParameter<edm::ParameterSet>("RadialIsoCalculator");
//  std::cout<<"  radialCalPSet_ : "<<radialCalPSet_<<std::endl;
//  RadialCutBasedCalculator = new RadialIsoCalculator (radialCalPSet_);
//  std::cout<<"muonIsoSel_ : "<<muonIsoSel_<<std::endl;

  doFillTree_  = iConfig.getParameter<bool>("doFillTree");
  studyMode_   = iConfig.getParameter<bool>("studyMode");

  nVtxPUcut_ = iConfig.getParameter<unsigned int>("nVtxPUcut");

  dPhis_CUT_vec_Src_ = iConfig.getParameter<edm::InputTag>("dPhis_CUT_vec_Src");
  nJets_CUT_Src_ = iConfig.getParameter<edm::InputTag>("nJets_CUT_Src");

  externalBitToTree_Src_ = iConfig.getParameter<edm::InputTag>("externalBitToTree_Src");

  metSrc_ = iConfig.getParameter<edm::InputTag>("metSrc");
  mhtSrc_ = iConfig.getParameter<edm::InputTag>("mhtSrc");
  htSrc_ = iConfig.getParameter<edm::InputTag>("htSrc");

  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc"); 

  isoVal0 = new std::vector<double>; isoVal1 = new std::vector<double>; isoVal2 = new std::vector<double>; isoVal3 = new std::vector<double>;
  muonMomStatus = new std::vector<int>;

  setTreeDefaultVars();

  if( doFillTree_ ){
     edm::Service<TFileService> fs;
     
     auxTree = fs->make<TTree>("AUX", "aux info");
     auxTree->Branch("run", &run, "run/I");
     auxTree->Branch("event", &event, "event/I");
     auxTree->Branch("lumi", &lumi, "lumi/I");
     auxTree->Branch("vtxSize", &vtxSize, "vtxSize/I");
     auxTree->Branch("mht", &mht_TR, "mht/D");
     auxTree->Branch("ht", &ht_TR, "ht/D");
     auxTree->Branch("met", &met_TR, "met/D");
     auxTree->Branch("nJets", &nJets, "nJets/I");
     auxTree->Branch("isoVal0", "std::vector<double>", &isoVal0, 32000, 0);
     auxTree->Branch("isoVal1", "std::vector<double>", &isoVal1, 32000, 0);
     auxTree->Branch("isoVal2", "std::vector<double>", &isoVal2, 32000, 0);
     auxTree->Branch("isoVal3", "std::vector<double>", &isoVal3, 32000, 0);
     auxTree->Branch("muonMomStatus", "std::vector<int>", &muonMomStatus, 32000, 0);
     auxTree->Branch("dPhi0_CUT", &dPhi0_CUT, "dPhi0_CUT/D");
     auxTree->Branch("dPhi1_CUT", &dPhi1_CUT, "dPhi1_CUT/D");
     auxTree->Branch("dPhi2_CUT", &dPhi2_CUT, "dPhi2_CUT/D");
     auxTree->Branch("nJets_CUT", &nJets_CUT, "nJets_CUT/I");
     auxTree->Branch("externalBit", &externalBitToTree_TR, "externalBitToTree_TR/I");
  }

//  eaCalcPtr    = new MuonEffectiveArea();
  produces<std::vector<pat::Muon> >("");
  produces<std::vector<double> >("ptVec");
  produces<std::vector<double> >("etaVec");
  produces<std::vector<double> >("phiVec");
}


StudyMuonIsolation::~StudyMuonIsolation() {

//   if( RadialCutBasedCalculator ) delete RadialCutBasedCalculator;
//   if( auxTree ) delete auxTree;

}


void StudyMuonIsolation::beginJob(){
//   RadialCutBasedCalculator->beginJob(radialCalPSet_);
}



bool StudyMuonIsolation::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  setTreeDefaultVars();

  loadEventInfo(iEvent, iSetup);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  loadMETMHT(iEvent);
  loadHT(iEvent);
  loadRecoJets(iEvent);
  loadAUX(iEvent); if( externalBitToTree_.isValid() ) externalBitToTree_TR = (*externalBitToTree_);

  // read in the objects
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel("particleFlow",pfCandidates);

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  std::vector<reco::GenParticle> genMuonVec;
// 1 : from b jet    -1 : from bbar jet
// 2 : from W+       -2 : from W-
// 0 : undifined??
  std::vector<int> genMuonMomStatus;

  if( !isData ){
     edm::Handle<std::vector<reco::GenParticle > > genParticles;
     iEvent.getByLabel("genParticles", genParticles);

     for(unsigned int ig=0; ig<genParticles->size(); ig++){

        const reco::GenParticle& gen = genParticles->at(ig);

        if( abs(gen.pdgId()) == 13 && gen.status() == 1 ){
           bool frombjet = find_mother( &gen, 5 );
           bool frombbarjet = find_mother( &gen, -5 );
           bool fromWplus = find_mother( &gen, 24);
           bool fromWminus = find_mother( &gen, -24);
   
           bool fromdjet = find_mother( &gen, 1 );
           bool fromdbarjet = find_mother( &gen, -1 );
           bool fromujet = find_mother( &gen, 2 );
           bool fromubarjet = find_mother( &gen, -2 );
           bool fromsjet = find_mother( &gen, 3 );
           bool fromsbarjet = find_mother( &gen, -3 );
           bool fromcjet = find_mother( &gen, 4 );
           bool fromcbarjet = find_mother( &gen, -4 );
           
   
           int status = 0;
           if( frombjet || frombbarjet ){
              if( frombjet && frombbarjet ) std::cout<<"muon belongs to both b and bbar?!"<<std::endl;
              if( frombjet ) status = 5;
              if( frombbarjet ) status = -5;
           }else if( fromWplus || fromWminus ){
              if( fromWplus && fromWminus ) std::cout<<"muon belongs to both W+ and W-?!"<<std::endl;
              if( fromWplus ) status = 24;
              if( fromWminus) status = -24;
           }else if( fromdjet || fromdbarjet || fromujet || fromubarjet || fromsjet || fromsjet || fromcjet || fromcbarjet ){
              if( fromdjet ) status = 1; if( fromdbarjet ) status = -1;
              if( fromujet ) status = 2; if( fromubarjet ) status = -2;
              if( fromsjet ) status = 3; if( fromsbarjet ) status = -3;
              if( fromcjet ) status = 4; if( fromcbarjet ) status = -4;
           }
           genMuonMomStatus.push_back(status);
           genMuonVec.push_back(gen);
        }
     }
  }

  iEvent.getByLabel(rhoSrc_, rho_);

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());
  std::auto_ptr<std::vector<double> > ptVecPtr(new std::vector<double>());
  std::auto_ptr<std::vector<double> > etaVecPtr(new std::vector<double>());
  std::auto_ptr<std::vector<double> > phiVecPtr(new std::vector<double>());
  if (vertices->size() > 0) {
     edm::View<pat::Muon>::const_iterator m;
     unsigned int imu;
     for (m = muons->begin(), imu=0; m != muons->end(); ++m, imu++) {

        int muonMomStatusTMP = -99;
        for(unsigned int ig=0; ig<genMuonVec.size(); ig++){
           const reco::GenParticle& gen = genMuonVec.at(ig);
           if( reco::deltaR(m->eta(), m->phi(), gen.eta(), gen.phi() ) < matchDR_ ){
              muonMomStatusTMP = genMuonMomStatus.at(ig);
           }
        } 

        ptVecPtr->push_back( m->pt() );
        etaVecPtr->push_back( m->eta() );
        phiVecPtr->push_back( m->phi() );

        // acceptance cuts
        if (m->pt() < minMuPt_) continue;
        if (std::abs(m->eta()) >= maxMuEta_) continue;
// ID cuts - https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Tight_Muon
        if( doMuonID_ ){
// The candidate is reconstructed as a Global Muon
//           if( !m->isGolbalMuon() ) continue;
           if( !m->muonID("AllGlobalMuons") ) continue; // pat::Muon type of isGlobalMuon
// At least one muon chamber hit included in the global-muon track fit (To suppress hadronic punch-through and muons from decays in flight.)
           if( m->globalTrack()->hitPattern().numberOfValidMuonHits() <=0 ) continue;
// Muon segments in at least two muon stations This implies that the muon is also an arbitrated tracker muon, see SWGuideTrackerMuons (To suppress punch-through and accidental track-to-segment matches. Also makes selection consistent with the logic of the muon trigger, which requires segments in at least two muon stations to obtain a meaningful estimate of the muon pT.)
           if( m->numberOfMatchedStations() <=1 ) continue;
// Number of pixel hits > 0 (To further suppress muons from decays in flight.)
           if( m->innerTrack()->hitPattern().numberOfValidPixelHits() <=0 ) continue;
// Particle-Flow muon id 
           if( !m->isPFMuon() ) continue;
// chi^2/ndof of the global-muon track fit < 10 (To suppress hadronic punch-through and muons from decays in flight (see CMS AN 2008/098). This cut might need to be re-tuned due to the change to fully segment based global fit in 50X releases and later.)
           if( m->globalTrack()->normalizedChi2() >= 10. ) continue;
// Cut on number of tracker layers with hits >5 (To guarantee a good pT measurement, for which some minimal number of measurement points in the tracker is needed. Also suppresses muons from decays in flight.)
           if( m->innerTrack()->hitPattern().trackerLayersWithMeasurement() <=5 ) continue;
        }
// Vertex association cuts - ignore if no vertex (see further)
        if (doMuonVtx_) {
// Its tracker track has transverse impact parameter dxy < 2 mm w.r.t. the primary vertex (To suppress cosmic muons and further suppress muons from decays in flight (see CMS AN 2008/098). The 2 mm cut preserves efficiency for muons from decays of b and c hadrons. It is a loose cut and can be tightened further with minimal loss of efficiency for prompt muons if background from cosmic muons is an issue. Another way to obtain a better cosmic-ray suppression is to complement the dxy cut with a cut on the opening angle Î± or use a dedicated cosmic-id algorithm (see Section 7.1 of MUO-10-004).
           if (std::abs(m->muonBestTrack()->dxy(vtxpos)) >= maxMuD0_ && vertices->size() > 0) continue;
//           if (std::abs(m->innerTrack()->dxy(vtxpos)) >= maxMuD0_ && vertices->size() > 0) continue;
// The longitudinal distance of the tracker track wrt. the primary vertex is dz < 5 mm (Loose cut to further suppress cosmic muons, muons from decays in flight and tracks from PU)
           if (std::abs(m->muonBestTrack()->dz(vtxpos)) >= maxMuDz_ && vertices->size() > 0) continue;
//           if (std::abs(m->vz() - vtxpos.z()) >= maxMuDz_ && vertices->size() > 0) continue;
        }
// Isolation cuts
        if (doMuonIso_) {
           double muRelIso = -1;

           if( muonIsoSel_ == 0 || studyMode_ ){
              muRelIso = ( m->pfIsolationR03().sumChargedHadronPt + m->pfIsolationR03().sumNeutralHadronEt + m->pfIsolationR03().sumPhotonEt )/ m->pt();
              isoVal0->push_back(muRelIso);
           }

// Proposed Configuration(DeltaBeta corrections)at cone of 0.3: I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
// Note that since sumPUPt is subtracted, no need to set PFnoPU globally!
// TODO: What is the working point for RA2 ?
           if( muonIsoSel_ == 1 || studyMode_ ){
              muRelIso = ( m->pfIsolationR03().sumChargedHadronPt + max(0., m->pfIsolationR03().sumNeutralHadronEt + m->pfIsolationR03().sumPhotonEt - 0.5*m->pfIsolationR03().sumPUPt) )/ m->pt();
              isoVal1->push_back(muRelIso);
           }

// Effective Areas are also provided using following prescription
// TODO: 1. Has to have PFnoPU set in global --> NOT necessarily!
//       2. rho has to be input from setting of kt6PFJetsCentralNeutral = kt6PFJets.clone( src = cms.InputTag("pfAllNeutralHadronsAndPhotons"), Ghost_EtaMax = cms.double(3.1), Rho_EtaMax = cms.double(2.5), inputEtMin = cms.double(0.5) )
           if( muonIsoSel_ == 2 || studyMode_ ){
              double EACombined = MuonEffectiveArea::GetMuonEffectiveArea(MuonEffectiveArea::kMuGammaAndNeutralHadronIso03, m->eta(), MuonEffectiveArea::kMuEAData2012);
              muRelIso = ( m->pfIsolationR03().sumChargedHadronPt + max(0., m->pfIsolationR03().sumNeutralHadronEt + m->pfIsolationR03().sumPhotonEt - (*rho_)*EACombined) )/m->pt();
              isoVal2->push_back(muRelIso);
           }

// Radial isolation
           if( muonIsoSel_ == 3 || studyMode_ ){

              reco::TrackRef muTrk = m->track();
              if (muTrk.isNull()) {
                 muTrk = m->standAloneMuon();
              }
              if (muTrk.isNull()) {
             //if muon is not standalone either, then return -9999
                 muRelIso = -1;
              }else{
//                 muRelIso = RadialCutBasedCalculator->GetIsolationValue(iEvent, iSetup, (*m));
              }
              isoVal3->push_back(muRelIso);
           }

           muonMomStatus->push_back(muonMomStatusTMP);

           if (muRelIso >= maxMuRelIso_) continue;
        }
 // muon is ID'd and isolated! - only accept if vertex present
        if (vertices->size() > 0) prod->push_back(pat::Muon(*m));
    }
  }

  if( doFillTree_ ){
     met_TR = (*met)[0].pt(); mht_TR = (*mht)[0].pt(); ht_TR = (*ht);  
     dPhi0_CUT = (*dPhis_CUT_vec_)[0]; dPhi1_CUT = (*dPhis_CUT_vec_)[1]; dPhi2_CUT = (*dPhis_CUT_vec_)[2];
     nJets_CUT = (*nJets_CUT_);
     auxTree->Fill();
  }

  // determine result before losing ownership of the pointer
  bool result = (doMuonVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);
  iEvent.put(ptVecPtr, "ptVec");
  iEvent.put(etaVecPtr, "etaVec");
  iEvent.put(phiVecPtr, "phiVec");

  return result;

}

void StudyMuonIsolation::setTreeDefaultVars(){

   isoVal0->clear(); isoVal1->clear(); isoVal2->clear(); isoVal3->clear();
   muonMomStatus->clear();

   mht_TR= -99, ht_TR= -99, met_TR= -99;

   dPhi0_CUT = -99, dPhi1_CUT = -99, dPhi2_CUT = -99;
   nJets_CUT = -99;

   externalBitToTree_TR = -99;
}

bool StudyMuonIsolation::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}

bool StudyMuonIsolation::find_mother(const reco::Candidate* genp, const reco::Candidate &target){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->p4() == target.p4() && (genp->mother())->vertex() == target.vertex() && (genp->mother())->charge() == target.charge()) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), target);
    if (result) return true;
  }
  return false;
}

void StudyMuonIsolation::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   lumi = iEvent.luminosityBlock();

// Get vertices
   iEvent.getByLabel(vtxSrc_, vertices); vtxSize = vertices->size();
}

void StudyMuonIsolation::loadMETMHT(const edm::Event& iEvent){
   iEvent.getByLabel(metSrc_, met);
   iEvent.getByLabel(mhtSrc_, mht);
}

void StudyMuonIsolation::loadHT(const edm::Event& iEvent){
   iEvent.getByLabel(htSrc_, ht);
}

void StudyMuonIsolation::loadAUX(const edm::Event& iEvent){
   iEvent.getByLabel(dPhis_CUT_vec_Src_, dPhis_CUT_vec_);
   iEvent.getByLabel(nJets_CUT_Src_, nJets_CUT_);

   iEvent.getByLabel(externalBitToTree_Src_, externalBitToTree_);
}

void StudyMuonIsolation::loadRecoJets(const edm::Event& iEvent){
   iEvent.getByLabel(jetSrc_, jets); nJets = jets->size();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(StudyMuonIsolation);
