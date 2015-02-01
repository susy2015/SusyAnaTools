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

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

typedef unsigned int size;
//class RadialIsoCalculator;

class GoldenMuonSelector : public edm::EDFilter {

  public:

    explicit GoldenMuonSelector(const edm::ParameterSet & iConfig);
    ~GoldenMuonSelector();

    typedef unsigned long key_type;

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    void beginJob();

    edm::InputTag muonSrc_;
    edm::InputTag vtxSrc_;
    bool doMuonVeto_, doMuonID_, doMuonVtx_, doMuonIso_;
    double minMuPt_, maxMuEta_, maxMuD0_, maxMuDz_, maxMuRelIso_, minMuNumHit_;

//    RadialIsoCalculator*    RadialCutBasedCalculator;
    edm::ParameterSet radialCalPSet_;
//    RadialMVAIsoCalculator* RadialMVACalculator;

// 0 : Traditional relative iso as last year --> sumChargedHadronPt depends on if PFnoPU is used or not
// 1 : Official recommended DeltaBeta PU correction
// 2 : Effective Area PU correction
// 3 : Radial PU correction
    int muonIsoSel_;
    std::vector<double> *isoVal0, *isoVal1, *isoVal2, *isoVal3;
    size run, event, lumi;
   
    edm::InputTag rhoSrc_;
    edm::Handle<double> rho_;

    bool doFillTree_;
    void setTreeDefaultVars();
    TTree *auxTree;
    bool studyMode_;

    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);
  
    bool doMuonProvInfo_;

    edm::InputTag inputProductIDSrc_, inputKeySrc_;
    edm::Handle<std::vector<edm::ProductID> > inputProductID_;
    edm::Handle<std::vector<key_type> > inputKey_;

    bool doRemoveMuonsFrombJets_;
// 0 : dR or dRcon to bJet (CSVM?) method
// 1 : dxy and dz method: For muons close to a bJet using method 0, there are additional requirement that
//     only (|dxy| > 0.005 || |dz| > 0.018) muons are considered b muons -- default
// 2 : dxy, dz + relIso method: Based on method 1, with additional requirement that relIso < 0.15
    int methodRemoveMuonsFrombJets_;
    edm::InputTag jetSrc_;
    edm::Handle<edm::View<pat::Jet> > patjets;
    std::string bTagKeyString_;
    double CSVS_;
    double deltaRcon_;

    double dxyCutForMuonsFrombJets_, dzCutForMuonsFrombJets_, relIsoCutForMuonsFrombJets_;

    bool doGenLevbMuonRejection_;
    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;
    double deltaRGenMuRecoMu_, deltaRGenbRecoMu_;
    std::vector<int> lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec);
    bool find_mother(const reco::Candidate* genp, const reco::Candidate &target);
    bool find_mother(const reco::Candidate* genp, const int motherID);
    bool doZfilter_;
};


GoldenMuonSelector::GoldenMuonSelector(const edm::ParameterSet & iConfig) {
  muonSrc_     = iConfig.getParameter<edm::InputTag>("MuonSource");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("VertexSource");
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

  muonIsoSel_  = iConfig.getParameter<int>("muonIsoSel");

  radialCalPSet_ = iConfig.getParameter<edm::ParameterSet>("RadialIsoCalculator");
//  std::cout<<"  radialCalPSet_ : "<<radialCalPSet_<<std::endl;
//  RadialCutBasedCalculator = new RadialIsoCalculator (radialCalPSet_);
//  std::cout<<"muonIsoSel_ : "<<muonIsoSel_<<std::endl;

  doFillTree_  = iConfig.getParameter<bool>("doFillTree");
  studyMode_   = iConfig.getParameter<bool>("studyMode");

  doMuonProvInfo_ = iConfig.getParameter<bool>("DoMuonProvInfo");

  inputProductIDSrc_ = iConfig.getParameter<edm::InputTag>("inputProductIDSrc");
  inputKeySrc_ = iConfig.getParameter<edm::InputTag>("inputKeySrc");

  doRemoveMuonsFrombJets_ = iConfig.getUntrackedParameter<bool>("doRemoveMuonsFrombJets", false);
  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
  bTagKeyString_ = iConfig.getUntrackedParameter<std::string>("bTagKeyString", "combinedSecondaryVertexBJetTags");
  CSVS_ = iConfig.getUntrackedParameter<double>("CSVS", 0.679);
  deltaRcon_ = iConfig.getUntrackedParameter<double>("deltaRcon", 0.01);

  methodRemoveMuonsFrombJets_ = iConfig.getUntrackedParameter<int>("methodRemoveMuonsFrombJets", 1);
  dxyCutForMuonsFrombJets_ = iConfig.getUntrackedParameter<double>("dxyCutForMuonsFrombJets", 0.005);
  dzCutForMuonsFrombJets_ = iConfig.getUntrackedParameter<double>("dzCutForMuonsFrombJets", 0.018);
  relIsoCutForMuonsFrombJets_ = iConfig.getUntrackedParameter<double>("relIsoCutForMuonsFrombJets", 0.15);

  doGenLevbMuonRejection_ = iConfig.getUntrackedParameter<bool>("doGenLevbMuonRejection", false);
  genJetSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genJetSrc", edm::InputTag("ak5GenJets"));
  genParticleSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genParticleSrc", edm::InputTag("genParticles"));
  deltaRGenMuRecoMu_ = iConfig.getUntrackedParameter<double>("deltaRGenMuRecoMu", 0.1);
  deltaRGenbRecoMu_ = iConfig.getUntrackedParameter<double>("deltaRGenbRecoMu", 0.5);

  doZfilter_ = iConfig.getUntrackedParameter<bool>("doZfilter", false);

  isoVal0 = new std::vector<double>; isoVal1 = new std::vector<double>; isoVal2 = new std::vector<double>; isoVal3 = new std::vector<double>;

  setTreeDefaultVars();

  if( doFillTree_ ){
     edm::Service<TFileService> fs;
     
     auxTree = fs->make<TTree>("AUX", "aux info");
     auxTree->Branch("run", &run, "run/I");
     auxTree->Branch("event", &event, "event/I");
     auxTree->Branch("lumi", &lumi, "lumi/I");
     auxTree->Branch("isoVal0", "std::vector<double>", &isoVal0, 32000, 0);
     auxTree->Branch("isoVal1", "std::vector<double>", &isoVal1, 32000, 0);
     auxTree->Branch("isoVal2", "std::vector<double>", &isoVal2, 32000, 0);
     auxTree->Branch("isoVal3", "std::vector<double>", &isoVal3, 32000, 0);
  }

//  eaCalcPtr    = new MuonEffectiveArea();
  produces<std::vector<pat::Muon> >("");
  if( doMuonProvInfo_ ){
     produces<std::vector<edm::ProductID> >("muonPFbaseProductID");
     produces<std::vector<key_type> >("muonPFbaseKey");
  }
}


GoldenMuonSelector::~GoldenMuonSelector() {

//   if( RadialCutBasedCalculator ) delete RadialCutBasedCalculator;
//   if( auxTree ) delete auxTree;

}


void GoldenMuonSelector::beginJob(){
//   RadialCutBasedCalculator->beginJob(radialCalPSet_);
}



bool GoldenMuonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  setTreeDefaultVars();

  // read in the objects
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel("particleFlow",pfCandidates);

  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  iEvent.getByLabel(rhoSrc_, rho_);

  if( doRemoveMuonsFrombJets_ ){
     iEvent.getByLabel(jetSrc_, patjets);
  }

  std::vector<int> isGenLevbMuonVec;
  if( doGenLevbMuonRejection_ ){
     iEvent.getByLabel(genJetSrc_, genJets);
     iEvent.getByLabel(genParticleSrc_, genParticles);

     std::vector< std::vector<int> > evtLepDecayStatusVec;
     std::vector<int> lepGenIdxVec = lepDecays(evtLepDecayStatusVec);
     unsigned int nleps = lepGenIdxVec.size();

     std::vector<reco::GenParticle> allgenbVec;
     for(unsigned int ig=0; ig<genParticles->size(); ig++){
        const reco::GenParticle & gen = genParticles->at(ig);
        int genpdgId = gen.pdgId(), genstatus = gen.status();
        if( abs(genpdgId) == 5 && genstatus == 3 ){
           allgenbVec.push_back(gen);
        }
     }

     std::vector<double> mindRGenMuRecoMuVec;
     std::vector<double> mindRGenbRecoMuVec;
     for(unsigned int im=0; im<muons->size(); im++){
        double mindRRecomuGenmu = 999.0;
        for(unsigned int il=0; il<nleps; il++){
           bool isMu = false, isEle = false;
           const reco::GenParticle& gen = genParticles->at(lepGenIdxVec[il]);
           if( abs(gen.pdgId()) == 13 ) isMu = true;
           if( abs(gen.pdgId()) == 11 ) isEle = true;

           bool selMu = false, selEle = false;
           if( selEle ){/*empty to avoid a compiling error*/}

           if( evtLepDecayStatusVec[il][1] || evtLepDecayStatusVec[il][2] ) {
              if( isMu ) selMu = true;
              if( isEle ) selEle = true;
           }

           if( selMu ){
              double perdeltaR = reco::deltaR((*muons)[im].eta(), (*muons)[im].phi(), gen.eta(), gen.phi());
              if( mindRRecomuGenmu > perdeltaR ) mindRRecomuGenmu = perdeltaR;
           }
        }
        mindRGenMuRecoMuVec.push_back(mindRRecomuGenmu);

        double mindRRecomuGenb = 999.0;
        for(unsigned int ib=0; ib<allgenbVec.size(); ib++){
           double perdeltaR = reco::deltaR((*muons)[im].eta(), (*muons)[im].phi(), allgenbVec[ib].eta(), allgenbVec[ib].phi());
           if( mindRRecomuGenb > perdeltaR ) mindRRecomuGenb = perdeltaR;
        }
        mindRGenbRecoMuVec.push_back(mindRRecomuGenb);
     }

     for(unsigned int im=0; im<mindRGenMuRecoMuVec.size(); im++){
        if( mindRGenMuRecoMuVec[im] > deltaRGenMuRecoMu_ && mindRGenbRecoMuVec[im] < deltaRGenbRecoMu_ ) isGenLevbMuonVec.push_back(1);
        else isGenLevbMuonVec.push_back(0);
     }
  }

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());

  reco::CandidatePtr leptonOutPtr;
  unsigned long dummyKey = 999999999;
  edm::ProductID dummyProductID;

  iEvent.getByLabel(inputProductIDSrc_, inputProductID_);
  iEvent.getByLabel(inputKeySrc_, inputKey_);

  bool validInputProvInfo = false;
  if( inputProductID_.isValid() && inputKey_.isValid() ) validInputProvInfo = true;

  std::auto_ptr<std::vector<edm::ProductID> > muonPFbaseProductIDptr(new std::vector<edm::ProductID>() );
  std::auto_ptr<std::vector<key_type> > muonPFbaseKeyptr(new std::vector<key_type>() );

  if (vertices->size() > 0) {
     edm::View<pat::Muon>::const_iterator m;
     unsigned int imu;
     for (m = muons->begin(), imu=0; m != muons->end(); ++m, imu++) {

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
        double muRelIsoValForMuonsFrombJets = -1;
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
//              muRelIso = ( m->pfIsolationR03().sumChargedHadronPt + max(0., m->pfIsolationR03().sumNeutralHadronEt + m->pfIsolationR03().sumPhotonEt - 0.5*m->pfIsolationR03().sumPUPt) )/ m->pt();
              muRelIso = ( m->pfIsolationR04().sumChargedHadronPt + max(0., m->pfIsolationR04().sumNeutralHadronEt + m->pfIsolationR04().sumPhotonEt - 0.5*m->pfIsolationR04().sumPUPt) )/ m->pt();
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

           muRelIsoValForMuonsFrombJets = muRelIso;
           if (muRelIso >= maxMuRelIso_) continue;
        }

        edm::ProductID tofillProductID = dummyProductID;
        key_type tofillKey = dummyKey;

        const reco::CandidatePtr& muonRef =m->pfCandidateRef()->sourceCandidatePtr(0);
//        const reco::CandidatePtr& muonRef =m->sourceCandidatePtr(0);

        if( doMuonProvInfo_ && !m->pfCandidateRef().isNull() ){
           if( validInputProvInfo ){
              tofillProductID = (*inputProductID_)[imu];
              tofillKey = (*inputKey_)[imu];
           }else{
              if( !searchToPFbase(iEvent, muonRef, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for muon?!"<<std::endl<<std::endl; }

              if( doMuonProvInfo_ && leptonOutPtr.isNonnull() ){
                 tofillProductID = leptonOutPtr.id();
                 tofillKey = leptonOutPtr.key();
              }
           }
        }

 // muon is ID'd and isolated! - only accept if vertex present
        bool isMuonFrombJets = false;
        if( doRemoveMuonsFrombJets_ ){
           for (edm::View<pat::Jet>::const_iterator j = patjets->begin(); j != patjets->end(); ++j) {

              double btag = j->bDiscriminator(bTagKeyString_.c_str());
              if( btag < CSVS_ ) continue;

              const std::vector<reco::PFCandidatePtr> & constituents = j->getPFConstituents();
              const unsigned int numConstituents = constituents.size();

              double mindRmuonCon = 999.; //int selConIdx = -1;
              for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
                 const reco::PFCandidatePtr& constituent = constituents[iCon];
                 if( constituent->charge() != m->charge() ) continue;
                 const double dRmuonCon = reco::deltaR(constituent->eta(), constituent->phi(), m->eta(), m->phi());
                 if( mindRmuonCon > dRmuonCon ){
                    mindRmuonCon = dRmuonCon;
//                    selConIdx = (int)iCon;
                 }
              }
              if( mindRmuonCon < deltaRcon_ ){
                 if( methodRemoveMuonsFrombJets_ ==0 ){
                    isMuonFrombJets = true;
                 }else if( methodRemoveMuonsFrombJets_ ==1 ){
                    if( std::abs(m->muonBestTrack()->dxy(vtxpos)) >= dxyCutForMuonsFrombJets_ || std::abs(m->muonBestTrack()->dz(vtxpos)) >= dzCutForMuonsFrombJets_ ){
                       isMuonFrombJets = true;
                    }
                 }else if( methodRemoveMuonsFrombJets_ ==2 ){
                    if( std::abs(m->muonBestTrack()->dxy(vtxpos)) >= dxyCutForMuonsFrombJets_ || std::abs(m->muonBestTrack()->dz(vtxpos)) >= dzCutForMuonsFrombJets_ || muRelIsoValForMuonsFrombJets > relIsoCutForMuonsFrombJets_ ){
                       isMuonFrombJets = true;
                    }
                 }
              }
           }
        }

// Override previous method is doGenLevbMuonRejection_ is enabled
        if( doGenLevbMuonRejection_ ){
           int muonPos = std::distance(muons->begin(), m);
           if(isGenLevbMuonVec[muonPos]) isMuonFrombJets = true;
        }

        if ( vertices->size() > 0 && (!doRemoveMuonsFrombJets_ || !isMuonFrombJets) ){
           prod->push_back(pat::Muon(*m));
           if( doMuonProvInfo_ ){ muonPFbaseProductIDptr->push_back(tofillProductID); muonPFbaseKeyptr->push_back(tofillKey); }
        }
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
  bool result = (doMuonVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);
  if( doMuonProvInfo_ ){
     iEvent.put( muonPFbaseProductIDptr, "muonPFbaseProductID" );
     iEvent.put( muonPFbaseKeyptr, "muonPFbaseKey" );
  }

  return result;
}

void GoldenMuonSelector::setTreeDefaultVars(){

   isoVal0->clear(); isoVal1->clear(); isoVal2->clear(); isoVal3->clear();
}

int GoldenMuonSelector::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
  outPtr = inPtr;
  if( inPtr.isNull() ){ return 0; }
  std::string transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
  while ( transModuleLabel != "particleFlow" ){
     outPtr = outPtr->sourceCandidatePtr(0);
     if( outPtr.isNonnull() ){
        transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
     }else{
        break;
     }
  }
  if( outPtr.isNull() ){ return 0; }
  return 1;
}

std::vector<int> GoldenMuonSelector::lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec){

   using namespace edm;
   using namespace std;

   std::vector<int> lepGenIdxVec;
   evtLepDecayStatusVec.clear();

   for(size ig=0; ig<genParticles->size(); ig++){

      int isLepDecay = 0, isWLepDecay =0, isWTauLepDecay =0;

      const reco::GenParticle& gen = genParticles->at(ig);

      if( (abs(gen.pdgId()) == 13 || abs(gen.pdgId()) == 11 ) && gen.status() == 1 ){

         isLepDecay = 1;

         int signLep = gen.pdgId()/abs(gen.pdgId());
         int idLep = gen.pdgId();

         bool frombjet = find_mother( &gen, 5 );
         bool frombbarjet = find_mother( &gen, -5 );

         const reco::Candidate *momLep = &gen;
// If from b or bbar fragmentation, should be put to "other" category, i.e., not the WLep one which we really need
         if( gen.numberOfMothers() && !frombjet && !frombbarjet ){

            while( momLep->mother(0)->pdgId() == idLep && momLep->numberOfMothers() ){
               momLep = momLep->mother(0);
            }

            if( momLep->numberOfMothers() && momLep->pdgId() == idLep && momLep->status() == 3 ){
               if( momLep->mother(0)->pdgId() == -1*signLep*24 ) isWLepDecay = 1;
               if( doZfilter_ && momLep->mother(0)->pdgId() == 23 ) isWLepDecay = 1;
            }

            if( momLep->numberOfMothers() && momLep->mother(0)->pdgId() == signLep*15 && momLep->mother(0)->status() == 2 ){
               const reco::Candidate *momTau = momLep->mother(0);

               while( momTau->mother(0)->pdgId() == signLep*15 && momTau->numberOfMothers() ){
                  momTau = momTau->mother(0);
               }

               if( momTau->numberOfMothers() && momTau->pdgId() == signLep*15 && momTau->status() == 3 ){
                  if( momTau->mother(0)->pdgId() == -1*signLep*24 ) isWTauLepDecay =1;
                  if( doZfilter_ && momTau->mother(0)->pdgId() == 23 ) isWTauLepDecay =1;
               }
            }
         }

         lepGenIdxVec.push_back(ig);

         vector<int> perDecayStatusVec;
         perDecayStatusVec.push_back( isLepDecay ); perDecayStatusVec.push_back( isWLepDecay ); perDecayStatusVec.push_back( isWTauLepDecay );
         evtLepDecayStatusVec.push_back(perDecayStatusVec);
      }
   }

   return lepGenIdxVec;
}

bool GoldenMuonSelector::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}


bool GoldenMuonSelector::find_mother(const reco::Candidate* genp, const reco::Candidate &target){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->p4() == target.p4() && (genp->mother())->vertex() == target.vertex() && (genp->mother())->charge()
== target.charge()) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), target);
    if (result) return true;
  }
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GoldenMuonSelector);
