#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
//#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "SusyAnaTools/Skims/plugins/ElectronEffectiveArea.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "DataFormats/METReco/interface/MET.h"

#include "TLorentzVector.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"

typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;

class prodElectrons : public edm::EDFilter 
{
  enum elesIDLevel {VETO, LOOSE, MEDIUM, TIGHT};
 public:
   explicit prodElectrons(const edm::ParameterSet & iConfig);
   ~prodElectrons();
 private:
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag electronSrc_;
  edm::InputTag conversionsSrc_;
  edm::InputTag vtxSrc_;
  edm::InputTag metSrc_;
  edm::InputTag beamSpotSrc_;
  edm::InputTag pfCandsSrc_;
  edm::InputTag rhoSrc_;
  edm::EDGetTokenT< edm::View<pat::Electron> > ElecTok_;
  edm::EDGetTokenT< std::vector<reco::Vertex> > VtxTok_;
  edm::EDGetTokenT<edm::View<reco::MET> > MetTok_;
  edm::EDGetTokenT< std::vector<reco::Conversion> > ConversionsTok_;
  edm::EDGetTokenT<reco::BeamSpot> BeamSpotTok_;
  edm::EDGetTokenT<pat::PackedCandidateCollection>  PfcandTok_;
  edm::EDGetTokenT<double> RhoTok_;

  bool doEleVeto_, dod0dz_;
  int doEleIso_; // 0: don't do any isolation; 1: relIso;  2: miniIso
  double minElePt_, maxEleEta_;
  bool debug_;
  double minElePtForElectron2Clean_, maxEleMiniIso_;

  bool passElectronID(const pat::Electron & ele, const edm::Handle< std::vector<reco::Vertex> > & vertices, const elesIDLevel level);
  bool passElectronISO(const pat::Electron & ele, const double relIso, const elesIDLevel level);
};


typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;


prodElectrons::prodElectrons(const edm::ParameterSet & iConfig)
{
  electronSrc_   = iConfig.getParameter<edm::InputTag>("ElectronSource");
  conversionsSrc_= iConfig.getParameter<edm::InputTag>("ConversionsSource");
  vtxSrc_        = iConfig.getParameter<edm::InputTag>("VertexSource");
  metSrc_        = iConfig.getParameter<edm::InputTag>("metSource");
  beamSpotSrc_   = iConfig.getParameter<edm::InputTag>("BeamSpotSource");
  pfCandsSrc_    = iConfig.getParameter<edm::InputTag>("PFCandSource");
  minElePt_      = iConfig.getParameter<double>("MinElePt");
  maxEleEta_     = iConfig.getParameter<double>("MaxEleEta");
  doEleVeto_     = iConfig.getParameter<bool>("DoElectronVeto");
  dod0dz_        = iConfig.getParameter<bool>("Dod0dz");
  doEleIso_      = iConfig.getParameter<int>("DoElectronIsolation");
  maxEleMiniIso_ = iConfig.getParameter<double>("MaxEleMiniIso");
  debug_         = iConfig.getParameter<bool>("Debug");
  rhoSrc_       = iConfig.getParameter<edm::InputTag>("RhoSource");

  minElePtForElectron2Clean_ = iConfig.getUntrackedParameter<double>("minElePtForElectron2Clean", 10);
  
  ConversionsTok_ = consumes< std::vector<reco::Conversion> >(conversionsSrc_);
  BeamSpotTok_ = consumes<reco::BeamSpot>(beamSpotSrc_);
  ElecTok_ = consumes< edm::View<pat::Electron> >(electronSrc_);
  MetTok_ = consumes<edm::View<reco::MET>>(metSrc_);
  VtxTok_ = consumes<std::vector<reco::Vertex>>(vtxSrc_);
  PfcandTok_ = consumes<pat::PackedCandidateCollection>(pfCandsSrc_);
  RhoTok_ = consumes<double>(rhoSrc_);

  produces<std::vector<pat::Electron> >("");
  produces<std::vector<pat::Electron> >("ele2Clean");  

  produces<std::vector<int> >("elesFlagVeto");
  produces<std::vector<int> >("elesFlagMedium");
  produces<std::vector<TLorentzVector> >("elesLVec");
  produces<std::vector<double> >("elesCharge");
  produces<std::vector<double> >("elesMtw");
  produces<std::vector<double> >("elesRelIso");
  produces<std::vector<bool> >("elesisEB");
  produces<std::vector<double> >("elesMiniIso");
  produces<std::vector<double> >("elespfActivity");
  produces<int>("nElectrons");
}


prodElectrons::~prodElectrons()
{
}


bool prodElectrons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  // electrons
  edm::Handle< edm::View<pat::Electron> > electrons;   
  iEvent.getByToken(ElecTok_, electrons);

  // conversions
  edm::Handle< std::vector<reco::Conversion> > conversions;
  iEvent.getByToken(ConversionsTok_, conversions);

  // beam spot
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByToken(BeamSpotTok_, beamspot);
  //const reco::BeamSpot &beamSpot = *(beamspot.product());
  
  // vertices
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByToken(MetTok_, met);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByToken(PfcandTok_, pfcands);

  edm::Handle< double > rho_;
  //iEvent.getByLabel("fixedGridRhoFastjetCentralNeutral", rho_); // Central rho recommended for SUSY
  iEvent.getByToken(RhoTok_,rho_);
  double rho = *rho_;

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Electron> > prod(new std::vector<pat::Electron>());
  std::auto_ptr<std::vector<pat::Electron> > ele2Clean(new std::vector<pat::Electron>());

  std::auto_ptr<std::vector<TLorentzVector> > elesLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<double> > elesCharge(new std::vector<double>());
  std::auto_ptr<std::vector<double> > elesMtw(new std::vector<double>());
  std::auto_ptr<std::vector<double> > elesRelIso(new std::vector<double>());
  std::auto_ptr<std::vector<bool> > elesisEB(new std::vector<bool>());
  std::auto_ptr<std::vector<double> > elesMiniIso(new std::vector<double>());
  std::auto_ptr<std::vector<double> > elespfActivity(new std::vector<double>());

  std::auto_ptr<std::vector<int> > elesFlagVeto(new std::vector<int>());
  std::auto_ptr<std::vector<int> > elesFlagMedium(new std::vector<int>());

  // loop on electrons
  for( edm::View<pat::Electron>::const_iterator ele = electrons->begin(); ele != electrons->end(); ele++ )
  {
    double pt = ele->pt();
    if (ele->pt() < minElePt_) continue;

    // get the ID variables from the electron object
    // kinematic variables
    bool isEB = ele->isEB() ? true : false;

    bool isVetoID = passElectronID((*ele), vertices, VETO);
    //bool isLooseID = passElectronID((*ele), vertices, LOOSE);
    bool isMediumID = passElectronID((*ele), vertices, MEDIUM);
    //bool isTightID = passElectronID((*ele), vertices, TIGHT);

    if( ! (isVetoID || isMediumID) ) continue;

    // isolation cuts                                                                                                                                        
    reco::GsfElectron::PflowIsolationVariables pfIso = ele->pfIsolationVariables();
    double absiso = pfIso.sumChargedHadronPt + std::max(0.0 , pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - 0.5 * pfIso.sumPUPt );

    // compute final isolation
    double iso = absiso/pt;
    //double miniIso = commonFunctions::getPFIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), 0.05, 0.2, 10., false, false);
    double miniIso = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), "electron", rho);
    double pfActivity = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), "electron", rho, true);

    if(doEleIso_ == 1 ) 
    {
      if( !passElectronISO((*ele), iso, VETO) ) continue;
    } 
    else if(doEleIso_ == 2 )
    {
      if( miniIso >= maxEleMiniIso_ ) continue;
    }

    // electron is ID'd and isolated! - only accept if vertex present
    if (vertices->size()>0)
    {
      prod->push_back(pat::Electron(*ele));
      TLorentzVector perLVec; perLVec.SetPtEtaPhiE(ele->pt(), ele->eta(), ele->phi(), ele->energy());
      elesLVec->push_back(perLVec);

      double mtw = sqrt( 2*( (*met)[0].pt()*ele->pt() -( (*met)[0].px()*ele->px() + (*met)[0].py()*ele->py() ) ) );

      elesCharge->push_back(ele->charge());
      elesMtw->push_back(mtw);
      elesRelIso->push_back(iso);
      elesisEB->push_back(isEB);
      elesMiniIso->push_back(miniIso);

      if( isVetoID ) elesFlagVeto->push_back(1); else elesFlagVeto->push_back(0); 
      if( isMediumID ) elesFlagMedium->push_back(1); else elesFlagMedium->push_back(0); 

      elespfActivity->push_back(pfActivity);
    }
    // add eles to clean from jets
    if( isVetoID && miniIso < maxEleMiniIso_ && ele->pt() > minElePtForElectron2Clean_ ) ele2Clean->push_back(*ele);
  }

  // determine result before losing ownership of the pointer
  bool result = (doEleVeto_ ? (prod->size() == 0) : true);

  std::auto_ptr<int> nElectrons (new int);

  *nElectrons = prod->size();

  // store in the event
  iEvent.put(prod);
  iEvent.put(ele2Clean, "ele2Clean");
  iEvent.put(elesFlagVeto, "elesFlagVeto");
  iEvent.put(elesFlagMedium, "elesFlagMedium");
  iEvent.put(elesLVec, "elesLVec");
  iEvent.put(elesCharge, "elesCharge");
  iEvent.put(elesMtw, "elesMtw");
  iEvent.put(elesRelIso, "elesRelIso");
  iEvent.put(elesisEB, "elesisEB");
  iEvent.put(elesMiniIso, "elesMiniIso");
  iEvent.put(elespfActivity, "elespfActivity");
  iEvent.put(nElectrons, "nElectrons");

  return result;
}

bool prodElectrons::passElectronID(const pat::Electron & ele, const edm::Handle< std::vector<reco::Vertex> > & vertices, const elesIDLevel level) 
{
  // electron ID cuts, updated for Spring15 25ns MC and Run2015C-D data 
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2

  // barrel electrons
  double eb_ieta_cut[4] = {0.0115, 0.011, 0.00998, 0.00998};
  double eb_deta_cut[4] = {0.00749, 0.00477, 0.00311, 0.00308};
  double eb_dphi_cut[4] = {0.228, 0.222, 0.103, 0.0816};
  double eb_hovere_cut[4] = {0.356, 0.298, 0.253, 0.0414};
  double eb_ooeminusoop_cut[4] = {0.299, 0.241, 0.134, 0.0129};
  int eb_misshits_cut[4] = {2, 1, 1, 1};

  // endcap electrons
  double ee_ieta_cut[4] = {0.037, 0.0314, 0.0298, 0.0292};
  double ee_deta_cut[4] = {0.00895, 0.00868, 0.00609, 0.00605};
  double ee_dphi_cut[4] = {0.213, 0.213, 0.045, 0.0394};
  double ee_hovere_cut[4] = {0.211, 0.101, 0.0878, 0.0641};
  double ee_ooeminusoop_cut[4] = {0.15, 0.14, 0.13, 0.0129};
  int ee_misshits_cut[4] = {3, 1, 1, 1};

  // common
  bool reqConvVeto[4] = {true, true, true, true};

  // id variables
  double sigmaIEtaIEta = ele.full5x5_sigmaIetaIeta();
  double dEtaIn        = ele.deltaEtaSuperClusterTrackAtVtx();
  double dPhiIn        = ele.deltaPhiSuperClusterTrackAtVtx();
  double hoe           = ele.hadronicOverEm();
  double ooemoop       = 1e30;
  
  if( ele.ecalEnergy() !=0 && std::isfinite(ele.ecalEnergy()) )
  {
    ooemoop = std::abs(1.0/ele.ecalEnergy() - ele.eSuperClusterOverP()/ele.ecalEnergy());
  }

  //Impact parameter cuts: differently from previous cut-based electron ID working points, this time the d0 and dz cuts are NOT part of the tuned ID. The reasons for this decision include: efficiency strongly dependent on the physics of the event, measurements wanting to have impact parameter handling different from per-electron d0/dz, relatively low discriminating power of the variables. Instead an average analysis is recommended to use safe highly efficient (when PV is properly found) baseline cuts given in the table below. The d0 and dz are NOT applied in the VID framework, and are left for regular users to cut on explicitly if desired.
  // impact parameter variables
  double eb_d0_cut[4] = {0.05, 0.05, 0.05, 0.05};
  double eb_dz_cut[4] = {0.1, 0.1, 0.1, 0.1};
  double ee_d0_cut[4] = {0.1, 0.1, 0.1, 0.1};
  double ee_dz_cut[4] = {0.2, 0.2, 0.2, 0.2};
  double d0vtx = 0.0;
  double dzvtx = 0.0;
  if (vertices->size() > 0)
  {
    reco::VertexRef vtx(vertices, 0);    
    d0vtx = ele.gsfTrack()->dxy(vtx->position());
    dzvtx = ele.gsfTrack()->dz(vtx->position());
  } 
  else 
  {
    d0vtx = ele.gsfTrack()->dxy();
    dzvtx = ele.gsfTrack()->dz();
  }
  bool dod0dz = dod0dz_, passd0dz_eb = true, passd0dz_ee = true;
  dod0dz ? passd0dz_eb = (eb_d0_cut[level] > fabs(d0vtx)) && (eb_dz_cut[level] > fabs(dzvtx)) : passd0dz_eb = true;
  dod0dz ? passd0dz_ee = (ee_d0_cut[level] > fabs(d0vtx)) && (ee_dz_cut[level] > fabs(dzvtx)) : passd0dz_ee = true;

  // conversion rejection variables
  bool convVeto = ele.passConversionVeto();
  double mHits = ele.gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);
  
  if (ele.isEB()) 
  {
    return 
         eb_ieta_cut[level] > sigmaIEtaIEta
      && eb_deta_cut[level] > fabs(dEtaIn)
      && eb_dphi_cut[level] > fabs(dPhiIn)
      && eb_hovere_cut[level] > hoe
      && eb_ooeminusoop_cut[level] > fabs(ooemoop)
      && passd0dz_eb
      //&& eb_d0_cut[level] > fabs(d0vtx)
      //&& eb_dz_cut[level] > fabs(dzvtx)
      && (reqConvVeto[level] == convVeto)
      && (eb_misshits_cut[level] >= mHits);
  } 
  else if (ele.isEE()) 
  {
    return 
         ee_ieta_cut[level] > sigmaIEtaIEta
      && ee_deta_cut[level] > fabs(dEtaIn)
      && ee_dphi_cut[level] > fabs(dPhiIn)
      && ee_hovere_cut[level] > hoe
      && ee_ooeminusoop_cut[level] > fabs(ooemoop)
      && passd0dz_ee
      //&& ee_d0_cut[level] > fabs(d0vtx)
      //&& ee_dz_cut[level] > fabs(dzvtx)
      && (reqConvVeto[level] == convVeto)
      && (ee_misshits_cut[level] >= mHits);
  } else return false;

}

bool prodElectrons::passElectronISO(const pat::Electron & ele, const double relIso, const elesIDLevel level)
{
  double eb_relIsoWithEA_cut[4] = {0.175, 0.0994, 0.0695, 0.0588};
  double ee_relIsoWithEA_cut[4] = {0.159, 0.1070, 0.0821, 0.0571}; 
   
  if( ele.isEB() )
  {
    return relIso < eb_relIsoWithEA_cut[level];
  }
  else if (ele.isEE() )
  {
    return relIso < ee_relIsoWithEA_cut[level];
  } 
  else return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodElectrons);
