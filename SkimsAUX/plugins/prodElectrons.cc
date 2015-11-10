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

class prodElectrons : public edm::EDFilter {

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
    bool   doEleVeto_;
    int doEleIso_; // 0: don't do any isolation; 1: relIso;  2: miniIso
    double minElePt_, maxEleEta_;
    bool debug_;
    double minElePtForElectron2Clean_, maxEleMiniIso_;

    bool passElectronID(const pat::Electron & ele, const edm::Handle< std::vector<reco::Vertex> > & vertices, const elesIDLevel level);
    bool passElectronISO(const pat::Electron & ele, const double relIso, const elesIDLevel level);
};


typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;


prodElectrons::prodElectrons(const edm::ParameterSet & iConfig) {
  electronSrc_   = iConfig.getParameter<edm::InputTag>("ElectronSource");
  conversionsSrc_= iConfig.getParameter<edm::InputTag>("ConversionsSource");
  vtxSrc_        = iConfig.getParameter<edm::InputTag>("VertexSource");
  metSrc_        = iConfig.getParameter<edm::InputTag>("metSource");
  beamSpotSrc_   = iConfig.getParameter<edm::InputTag>("BeamSpotSource");
  pfCandsSrc_    = iConfig.getParameter<edm::InputTag>("PFCandSource");
  minElePt_      = iConfig.getParameter<double>("MinElePt");
  maxEleEta_     = iConfig.getParameter<double>("MaxEleEta");
  doEleVeto_     = iConfig.getParameter<bool>("DoElectronVeto");
  doEleIso_      = iConfig.getParameter<int>("DoElectronIsolation");
  maxEleMiniIso_ = iConfig.getParameter<double>("MaxEleMiniIso");
  debug_         = iConfig.getParameter<bool>("Debug");

  minElePtForElectron2Clean_ = iConfig.getUntrackedParameter<double>("minElePtForElectron2Clean", 10);

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


prodElectrons::~prodElectrons() {
}


bool prodElectrons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // electrons
  edm::Handle< edm::View<pat::Electron> > electrons;   
  iEvent.getByLabel(electronSrc_, electrons);

  // conversions
  edm::Handle< std::vector<reco::Conversion> > conversions;
  iEvent.getByLabel(conversionsSrc_, conversions);

  // beam spot
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByLabel(beamSpotSrc_, beamspot);
//  const reco::BeamSpot &beamSpot = *(beamspot.product());
  
  // vertices
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByLabel(pfCandsSrc_, pfcands);

  edm::Handle< double > rho_;
  iEvent.getByLabel("fixedGridRhoFastjetCentralNeutral", rho_); // Central rho recommended for SUSY
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
  for( edm::View<pat::Electron>::const_iterator ele = electrons->begin(); ele != electrons->end(); ele++ ){

    double pt = ele->pt();
    if (ele->pt() < minElePt_) continue;

    // get the ID variables from the electron object
    // kinematic variables
    bool isEB = ele->isEB() ? true : false;

    bool isVetoID = passElectronID((*ele), vertices, VETO);
//    bool isLooseID = passElectronID((*ele), vertices, LOOSE);
    bool isMediumID = passElectronID((*ele), vertices, MEDIUM);
//    bool isTightID = passElectronID((*ele), vertices, TIGHT);

    if( ! (isVetoID || isMediumID) ) continue;

    // isolation cuts                                                                                                                                        
    reco::GsfElectron::PflowIsolationVariables pfIso = ele->pfIsolationVariables();
    double absiso = pfIso.sumChargedHadronPt + std::max(0.0 , pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - 0.5 * pfIso.sumPUPt );

    // compute final isolation
    double iso = absiso/pt;
//    double miniIso = commonFunctions::getPFIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), 0.05, 0.2, 10., false, false);
    double miniIso = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), "electron", rho);
    double pfActivity = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), "electron", rho, true);

    if (doEleIso_ == 1 ) {
       if( !passElectronISO((*ele), iso, VETO) ) continue;
    } else if (doEleIso_ == 2 ){
       if( miniIso >= maxEleMiniIso_ ) continue;
    }

    // electron is ID'd and isolated! - only accept if vertex present
    if (vertices->size()>0){
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

bool prodElectrons::passElectronID(const pat::Electron & ele, const edm::Handle< std::vector<reco::Vertex> > & vertices, const elesIDLevel level) {

  // electron ID cuts, updated for Spring15 25ns MC and Run2015C-D data 
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2

  // barrel electrons
  double eb_ieta_cut[4] = {0.0114, 0.0103, 0.0101, 0.0101};
  double eb_deta_cut[4] = {0.0152, 0.0105, 0.0103, 0.00926};
  double eb_dphi_cut[4] = {0.216, 0.115, 0.0336, 0.0336};
  double eb_hovere_cut[4] = {0.181, 0.104, 0.0876, 0.0597};
  double eb_ooeminusoop_cut[4] = {0.207, 0.102, 0.0174, 0.012};
  double eb_d0_cut[4] = {0.0564, 0.0261, 0.0118, 0.0111};
  double eb_dz_cut[4] = {0.472, 0.41, 0.373, 0.0466};
  int eb_misshits_cut[4] = {2, 2, 2, 2};

  // endcap electrons
  double ee_ieta_cut[4] = {0.0352, 0.0301, 0.0283, 0.0279};
  double ee_deta_cut[4] = {0.0113, 0.00814, 0.00733, 0.00724};
  double ee_dphi_cut[4] = {0.237, 0.182, 0.114, 0.0918};
  double ee_hovere_cut[4] = {0.116, 0.0897, 0.0678, 0.0615};
  double ee_ooeminusoop_cut[4] = {0.174, 0.126, 0.0898, 0.00999};
  double ee_d0_cut[4] = {0.222, 0.118, 0.0739, 0.0351};
  double ee_dz_cut[4] = {0.921, 0.822, 0.602, 0.417};
  int ee_misshits_cut[4] = {3, 1, 1, 1};

  // common
  bool reqConvVeto[4] = {true, true, true, true};

  // id variables
  double sigmaIEtaIEta = ele.full5x5_sigmaIetaIeta();
  double dEtaIn        = ele.deltaEtaSuperClusterTrackAtVtx();
  double dPhiIn        = ele.deltaPhiSuperClusterTrackAtVtx();
  double hoe           = ele.hadronicOverEm();
  double ooemoop       = 1e30;
  if( ele.ecalEnergy() !=0 && std::isfinite(ele.ecalEnergy()) ){
     ooemoop = std::abs(1.0/ele.ecalEnergy() - ele.eSuperClusterOverP()/ele.ecalEnergy());
  }

  // impact parameter variables
  double d0vtx         = 0.0;
  double dzvtx         = 0.0;
  if (vertices->size() > 0) {
      reco::VertexRef vtx(vertices, 0);    
      d0vtx = ele.gsfTrack()->dxy(vtx->position());
      dzvtx = ele.gsfTrack()->dz(vtx->position());
  } else {
      d0vtx = ele.gsfTrack()->dxy();
      dzvtx = ele.gsfTrack()->dz();
  }

  // conversion rejection variables
  bool convVeto = ele.passConversionVeto();
  double mHits = ele.gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);
  
  if (ele.isEB()) {
    return 
         eb_ieta_cut[level] > sigmaIEtaIEta
      && eb_deta_cut[level] > fabs(dEtaIn)
      && eb_dphi_cut[level] > fabs(dPhiIn)
      && eb_hovere_cut[level] > hoe
      && eb_ooeminusoop_cut[level] > fabs(ooemoop)
      && eb_d0_cut[level] > fabs(d0vtx)
      && eb_dz_cut[level] > fabs(dzvtx)
      && (reqConvVeto[level] == convVeto)
      && (eb_misshits_cut[level] >= mHits);
  } else if (ele.isEE()) {
    return 
         ee_ieta_cut[level] > sigmaIEtaIEta
      && ee_deta_cut[level] > fabs(dEtaIn)
      && ee_dphi_cut[level] > fabs(dPhiIn)
      && ee_hovere_cut[level] > hoe
      && ee_ooeminusoop_cut[level] > fabs(ooemoop)
      && ee_d0_cut[level] > fabs(d0vtx)
      && ee_dz_cut[level] > fabs(dzvtx)
      && (reqConvVeto[level] == convVeto)
      && (ee_misshits_cut[level] >= mHits);
  } else return false;

}

bool prodElectrons::passElectronISO(const pat::Electron & ele, const double relIso, const elesIDLevel level){
   double eb_relIsoWithEA_cut[4] = {0.126, 0.0893, 0.0766, 0.0354};
   double ee_relIsoWithEA_cut[4] = {0.144, 0.121, 0.0678, 0.0646}; 
   
   if( ele.isEB() ){
      return relIso < eb_relIsoWithEA_cut[level];
   }else if (ele.isEE() ){
      return relIso < ee_relIsoWithEA_cut[level];
   } else return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodElectrons);
