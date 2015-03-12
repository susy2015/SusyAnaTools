#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#include "SusyAnaTools/TopTagger/interface/combination.h"
#include "SusyAnaTools/TopTagger/interface/indexSort.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TRandom3.h"
#include "TLorentzVector.h"

typedef unsigned int size;
static const int sgnfnDof = 2;

using namespace edm;
using namespace std;

//For sorting by pt
struct GreaterByPtCandPtr
{
  bool operator()( const edm::Ptr<reco::Candidate> & t1, const edm::Ptr<reco::Candidate> & t2 ) const
  {return t1->pt() > t2->pt();}
};

class histAndTree : public edm::EDFilter{

  public:

    explicit histAndTree(const edm::ParameterSet & iConfig);
    ~histAndTree();
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void beginJob();
    virtual void endJob();
    virtual bool beginRun(edm::Run&, const edm::EventSetup&);
    virtual bool endRun(edm::Run&, const edm::EventSetup&);

    size run, event, ls; bool isdata;
    edm::InputTag vtxSrc_;
    edm::Handle<edm::View<reco::Vertex> > vertices;
    size nVtxPUcut_, vtxSize;
    edm::InputTag evtWeightInput_;
    edm::Handle<double> evtWeight_;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

    edm::InputTag jetSrc_;
    edm::Handle<edm::View<reco::Jet> > jets;
    edm::Handle<std::vector<pat::Jet> > patjets;
    edm::InputTag groomedJetSrc_, groomedJetIdxSrc_;
    edm::Handle<std::vector<TLorentzVector> > groomedJetsLVec_;
    edm::Handle<std::vector<std::vector<int> > > groomedJetsIdx_;
    std::vector<std::string> bTagKeyString_;
    size nbTagKeys;
    size nJets;
    reco::Jet jet1, jet2, jet3, jetother;
    edm::InputTag hepTaggerJetSrc_;
    edm::Handle<reco::BasicJetCollection> HEPTopTaggedCandidateHandle;
    bool isPatJet;
    virtual void loadRecoJets(const edm::Event& iEvent);

    edm::InputTag muonSrc_;
    edm::Handle<edm::View<reco::Candidate> > muons;
    edm::InputTag eleSrc_;
//    edm::Handle<edm::View<pat::Electron > > electrons;
    edm::Handle<edm::View<reco::Candidate> > electrons;
    edm::InputTag tauSrc_;
    edm::Handle<edm::View<pat::Tau > > taus;
    size nMuons, nElectrons, nTaus;
    edm::InputTag forVetoMuonSrc_;
    edm::Handle<edm::View<reco::Candidate> > forVetoMuons;
    edm::InputTag forVetoElectronSrc_;
//    edm::Handle<edm::View<pat::Electron > > forVetoElectrons;
    edm::Handle<edm::View<reco::Candidate> > forVetoElectrons;
    size nMuonsForVeto, nElectronsForVeto;
    reco::Muon muon1, muon2; int muon1Charge, muon2Charge;
    pat::Electron ele1, ele2; int ele1Charge, ele2Charge;
    virtual void loadLeptons(const edm::Event& iEvent);

    edm::InputTag photonSrc_;
    edm::Handle<edm::View<pat::Photon> > photons;
    size nPhotons;
    virtual void loadPhotons(const edm::Event& iEvent);

    edm::InputTag metSrc_;
    edm::Handle<edm::View<reco::MET> > met;
    edm::InputTag type1metSrc_;
    edm::Handle<edm::View<reco::MET> > type1met;
    edm::InputTag mhtSrc_, mht_forSgnfSrc_;
    edm::Handle<edm::View<reco::MET> > mht, mht_forSgnf;
    virtual void loadMETMHT(const edm::Event& iEvent);

    edm::InputTag htSrc_;
    edm::Handle<double> ht;
    virtual void loadHT(const edm::Event& iEvent);

    double pthat, scalePDF;
    edm::InputTag genJetSrc_;
    edm::Handle<edm::View<reco::GenJet > > genJets;
    size nGenJets;
    reco::GenJet genJet1, genJet2, genJet3, genJetother;
    edm::InputTag genParticleSrc_;
    edm::Handle<edm::View<reco::GenParticle > > genParticles;
    edm::InputTag genMETSrc_;
    edm::Handle<edm::View<reco::GenMET > > genMET;
    Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    int npv; double avg_npv; int nm1, n0, np1; double tru_npv;
    virtual void loadGenInfo(const edm::Event& iEvent);

    edm::InputTag dPhis_CUT_vec_Src_;
    edm::Handle<std::vector<double> > dPhis_CUT_vec_;
    edm::InputTag nJets_CUT_Src_;
    edm::Handle<int> nJets_CUT_;
    edm::InputTag externalBitToTree_Src_;
    edm::Handle<int> externalBitToTree_;
    virtual void loadAUX(const edm::Event& iEvent);

// Indices ==> 0 : for muon  1 : for electron 
// Combination is the following:
// 00 : veto all muons and electrons ==> standard RA2
// 10 : muon control sample ==> for lost-lepton, hadronic tau, W+Jets
// 20 : muon control sample ==> for Z+Jets
// others : currently NOT used
    std::vector<int> nLeptonsSels_;

    bool debug_;

    bool doFillTree_;

    bool isData;

    bool printOnce_;
    vector<std::string> availJECLevels_;

    TTree *outTree;
    double evtWeight_TR;
    double mht_TR, ht_TR, met_TR, mt_TR, type1mt_TR, type1met_TR;
    double mhtphi_TR, metphi_TR, type1metphi_TR;
    double metSgnf_TR, metSgnfProb_TR, type1metSgnf_TR, type1metSgnfProb_TR;
    double mht_forSgnf_TR, mhtphi_forSgnf_TR, mhtSgnf_TR, mhtSgnfProb_TR;
    double jet1pt_TR, jet1eta_TR, jet1phi_TR, jet1energy_TR;
    double jet2pt_TR, jet2eta_TR, jet2phi_TR, jet2energy_TR;
    double jet3pt_TR, jet3eta_TR, jet3phi_TR, jet3energy_TR;
    std::vector<double> *otherJetspt_TR, *otherJetseta_TR, *otherJetsphi_TR, *otherJetsenergy_TR;
    std::vector<TLorentzVector> *jetsLVec_TR;
    std::vector<TLorentzVector> *groomedJetsLVec_TR;

    double mu1pt_TR, mu1eta_TR, mu1phi_TR;
    double mu2pt_TR, mu2eta_TR, mu2phi_TR;
    std::vector<double> *otherMuspt_TR, *otherMuseta_TR, *otherMusphi_TR;
    std::vector<TLorentzVector> *muonsLVec_TR;
    std::vector<double> *muonsAux_TR;
    double ele1pt_TR, ele1eta_TR, ele1phi_TR;
    double ele2pt_TR, ele2eta_TR, ele2phi_TR;
    std::vector<double> *otherElespt_TR, *otherEleseta_TR, *otherElesphi_TR;
    std::vector<TLorentzVector> *elesLVec_TR;
    std::vector<double> *elesAux_TR;
    double dPhi0_CUT, dPhi1_CUT, dPhi2_CUT;
    int nJets_CUT;
    double genJet1pt_TR, genJet1eta_TR, genJet1phi_TR, genJet1energy_TR;
    double genJet2pt_TR, genJet2eta_TR, genJet2phi_TR, genJet2energy_TR;
    double genJet3pt_TR, genJet3eta_TR, genJet3phi_TR, genJet3energy_TR;
    std::vector<double> *otherGenJetspt_TR, *otherGenJetseta_TR, *otherGenJetsphi_TR, *otherGenJetsenergy_TR;
    double genMET_TR;
    double jet1Res_TR, jet2Res_TR, jet3Res_TR;
    std::vector<double> *otherJetsRes_TR;
    int externalBitToTree_TR;

    std::vector<int> *recoJetsFlavor_TR, *genJetsFlavor_TR;
    std::vector<double> *recoJetschargedHadronEnergyFraction_TR, *recoJetschargedEmEnergyFraction_TR;
    std::vector<std::vector<double> > recoJetsBtag_TR;


    void setTreeDefaultVars();

    double recoGenJetsDR_;

    bool doSgnf_;

    bool doFillGenTopInfo_;

    edm::InputTag gentopIdxVecSrc_, gentopDausIdxVecSrc_, genWDausIdxVecSrc_, gentauDausIdxVecSrc_, decayTypeVecSrc_;
    edm::Handle<std::vector<int> > gentopIdxVec_;
    edm::Handle<std::vector<std::vector<int> > > gentopDausIdxVec_, genWDausIdxVec_, gentauDausIdxVec_;
    edm::Handle<std::vector<std::string> > decayTypeVec_;

    std::vector<TLorentzVector> *gentopLVec_TR;
    std::vector<int> *gentoppdgIdVec_TR;
    std::vector<std::vector<TLorentzVector> > *gentopDausLVec_TR, *genWDausLVec_TR, *gentauDausLVec_TR;
    std::vector<std::vector<int> > *gentopDauspdgIdVec_TR, *genWDauspdgIdVec_TR, *gentauDauspdgIdVec_TR;
    std::vector<std::string> *decayTypeVec_TR;
    void fillGenDecayInfo(const edm::Event& iEvent);

    bool fillGenInfo_;

// For HepTopTagger
    std::vector<TLorentzVector> *hepTaggerJets_TR;
    std::vector<std::vector<TLorentzVector> > *hepTaggerSubJets_TR;

// To store top tagger information
    bool doTopTagger_;
    double maxJetEtaCutForTopTagger_, minJetPtCutForTopTagger_;
    topTagger::type3TopTagger *topTaggerPtr;

    int bestTopJetIdx_TR, pickedRemainingCombfatJetIdx_TR;
    bool remainPassCSVS_TR;
    double bestTopJetMass_TR, mTbestTopJet_TR, mTbJet_TR, MT2_TR, mTbestWJet_TR, mTbestbJet_TR, mTremainingTopJet_TR;

    int getConsMatchedJetIdx(const std::vector<pat::Jet> & patjets, const TLorentzVector tomatchLVec, const double tomatchCharge, const double minDRcut);
    double minDRcutForConsMatch_;

// To store gen part info related to the decays
//    edm::ESHandle<ParticleDataTable> pdt_;
    bool storeGenDecayInfo_;
    edm::InputTag genDecayStrVecSrc_, genDecayChainPartIdxVecSrc_;
    edm::Handle<std::vector<std::string> > genDecayStrVec_;
    edm::Handle<std::vector<int> > genDecayChainPartIdxVec_;
    std::vector<std::string> * genDecayStrVec_TR;
    std::vector<int> * genDecayIdxVec_TR, * genDecayPdgIdVec_TR, * genDecayMomIdxVec_TR;
    std::vector<TLorentzVector> * genDecayLVec_TR;
    int find_idx(const reco::Candidate & target);
    bool find_mother(int momPdgId, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec);
    bool find_mother(int momIdx, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec);
    void find_mother(std::vector<int> & momIdxVec, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec);
    int find_idx(int genIdx, const vector<int> &genDecayIdxVec);
    void find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec);
    std::vector<int> *W_emuVec_TR, *W_tau_emuVec_TR, *W_tau_prongsVec_TR;

// To store SMS model parameters
    bool storeSMSmodelInfo_;
    edm::InputTag smsModelFileNameStrSrc_, smsModelStrSrc_, smsModelMotherMassSrc_, smsModelDaughterMassSrc_;
    edm::Handle<std::vector<std::string> > smsModelFileNameStrVec_, smsModelStrVec_;
    edm::Handle<double> smsModelMotherMass_, smsModelDaughterMass_;
    std::vector<std::string> * smsModelFileNameStr_TR, * smsModelStr_TR;
    double smsModelMotherMass_TR, smsModelDaughterMass_TR;

// To store genjet collections
    std::vector<edm::InputTag> genJetsInputTags_;
    std::vector<std::vector<TLorentzVector> > genJetsLVec_TR;

// To store isotrack information
// The isolated tracks used for direct veto - for convenience 
    edm::InputTag forVetoIsoTrkSrc_;
    edm::Handle<pat::PackedCandidateCollection> forVetoIsoTrks_;
    std::vector<int> * forVetoIsoTrkIdxVec_TR;
    size loose_nIsoTrks, nIsoTrksForVeto;

    double isotrk_dR_, isotrk_dz_;
    edm::InputTag pfCandSrc_, loose_isoTrkSrc_, loose_isotrk_isoVecSrc_, loose_isotrk_dzpvVecSrc_;
    edm::Handle<pat::PackedCandidateCollection> pfCandHandle_, loose_isoTrksHandle_;
    std::vector<TLorentzVector> * trksForIsoVetoLVec_TR;
    std::vector<std::vector<double> > * trksForIsoVetoAux_TR; // store charge, pdgId, dz, ...
    std::vector<TLorentzVector> * loose_isoTrksLVec_TR;
    std::vector<std::vector<double> > * loose_isoTrksAuxVec_TR;
    std::vector<int> * loose_isoTrksConsMatchedJetIdxVec_TR;
//    std::vector<TLorentzVector> * passed_trksForIsoVetoLVec_TR;
//    std::vector<double> * passed_trksForIsoVetoAux_TR; // store charge, pdgId, dz, iso value, ...
};

histAndTree::histAndTree(const edm::ParameterSet & iConfig) {

   isData = true;
 
   jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
   hepTaggerJetSrc_ = iConfig.getParameter<edm::InputTag>("hepTaggerJetSrc");
   muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSrc");
   eleSrc_ = iConfig.getParameter<edm::InputTag>("eleSrc");
//   tauSrc_ = iConfig.getParameter<edm::InputTag>("tauSrc");

   groomedJetSrc_ = iConfig.getParameter<edm::InputTag>("groomedJetSrc");
   groomedJetIdxSrc_ = iConfig.getParameter<edm::InputTag>("groomedJetIdxSrc");

//   std::string defaultbTagKeyString[] = {"jetBProbabilityBJetTags","jetProbabilityBJetTags","trackCountingHighPurBJetTags","trackCountingHighEffBJetTags","simpleSecondaryVertexHighEffBJetTags","simpleSecondaryVertexHighPurBJetTags","combinedSecondaryVertexBJetTags","combinedSecondaryVertexMVABJetTags"};
   std::string defaultbTagKeyString[] = {"combinedInclusiveSecondaryVertexV2BJetTags"};
   std::vector<std::string> defaultbTagKeyStringStr(defaultbTagKeyString, defaultbTagKeyString + sizeof(defaultbTagKeyString)/sizeof(defaultbTagKeyString[0]));
   bTagKeyString_ = iConfig.getUntrackedParameter<std::vector<std::string> >("bTagKeyString", defaultbTagKeyStringStr);
   nbTagKeys = bTagKeyString_.size();
   std::cout<<"nbTagKeys : "<<nbTagKeys<<std::endl;

   forVetoMuonSrc_ = iConfig.getParameter<edm::InputTag>("forVetoMuonSrc");
   forVetoElectronSrc_ = iConfig.getParameter<edm::InputTag>("forVetoElectronSrc");
   forVetoIsoTrkSrc_ = iConfig.getParameter<edm::InputTag>("forVetoIsoTrkSrc");
 
   genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
   genMETSrc_ = iConfig.getParameter<edm::InputTag>("genMETSrc");
   genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");

   recoGenJetsDR_ = iConfig.getParameter<double>("recoGenJetsDR");
 
   vtxSrc_ = iConfig.getParameter<edm::InputTag>("vtxSrc");
   evtWeightInput_ = iConfig.getParameter<edm::InputTag>("evtWeightInput");
   nVtxPUcut_ = iConfig.getParameter<unsigned int>("nVtxPUcut");
 
   metSrc_ = iConfig.getParameter<edm::InputTag>("metSrc");
   type1metSrc_ = iConfig.getParameter<edm::InputTag>("type1metSrc");
   mhtSrc_ = iConfig.getParameter<edm::InputTag>("mhtSrc");
   mht_forSgnfSrc_ = iConfig.getParameter<edm::InputTag>("mht_forSgnfSrc");
   htSrc_ = iConfig.getParameter<edm::InputTag>("htSrc");

   dPhis_CUT_vec_Src_ = iConfig.getParameter<edm::InputTag>("dPhis_CUT_vec_Src");
   nJets_CUT_Src_ = iConfig.getParameter<edm::InputTag>("nJets_CUT_Src");

   nLeptonsSels_ = iConfig.getParameter<std::vector<int> >("nLeptonsSels");
 
   debug_ = iConfig.getParameter<bool>("debug");

   doFillTree_ = iConfig.getParameter<bool>("doFillTree");

   externalBitToTree_Src_ = iConfig.getParameter<edm::InputTag>("externalBitToTree_Src");

   doSgnf_ = iConfig.getParameter<bool>("doSgnf");

   doFillGenTopInfo_ = iConfig.getParameter<bool>("doFillGenTopInfo");
   gentopIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("gentopIdxVecSrc");
   gentopDausIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("gentopDausIdxVecSrc");
   genWDausIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("genWDausIdxVecSrc");
   gentauDausIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("gentauDausIdxVecSrc");
   decayTypeVecSrc_ = iConfig.getParameter<edm::InputTag>("decayTypeVecSrc");

   fillGenInfo_ = iConfig.getParameter<bool>("fillGenInfo");

   doTopTagger_ = iConfig.getUntrackedParameter<bool>("doTopTagger", false);
   maxJetEtaCutForTopTagger_ = iConfig.getUntrackedParameter<double>("maxJetEtaCutForTopTagger", -1);
   minJetPtCutForTopTagger_ = iConfig.getUntrackedParameter<double>("minJetPtCutForTopTagger", 30);

   minDRcutForConsMatch_ = iConfig.getUntrackedParameter<double>("minDRcutForConsMatch", 0.01);

   storeGenDecayInfo_ = iConfig.getUntrackedParameter<bool>("storeGenDecayInfo", false);
   genDecayStrVecSrc_ = iConfig.getParameter<edm::InputTag>("genDecayStrVecSrc");
   genDecayChainPartIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("genDecayChainPartIdxVecSrc");

   storeSMSmodelInfo_ = iConfig.getUntrackedParameter<bool>("storeSMSmodelInfo", false);
   smsModelFileNameStrSrc_ = iConfig.getParameter<edm::InputTag>("smsModelFileNameStrSrc");
   smsModelStrSrc_ = iConfig.getParameter<edm::InputTag>("smsModelStrSrc");
   smsModelMotherMassSrc_ = iConfig.getParameter<edm::InputTag>("smsModelMotherMassSrc");
   smsModelDaughterMassSrc_ = iConfig.getParameter<edm::InputTag>("smsModelDaughterMassSrc");

   genJetsInputTags_ = iConfig.getParameter<std::vector<edm::InputTag> >("genJetsInputTags");
   std::cout<<"genJetsInputTags_.size : "<<genJetsInputTags_.size()<<std::endl;

   pfCandSrc_        = iConfig.getParameter<edm::InputTag>("pfCandSrc");
   isotrk_dR_        = iConfig.getParameter<double>("isotrk_dR");
   isotrk_dz_        = iConfig.getParameter<double>("isotrk_dz");
   loose_isoTrkSrc_  = iConfig.getParameter<edm::InputTag>("loose_isoTrkSrc");
   loose_isotrk_isoVecSrc_ = iConfig.getParameter<edm::InputTag>("loose_isotrk_isoVecSrc");
   loose_isotrk_dzpvVecSrc_ = iConfig.getParameter<edm::InputTag>("loose_isotrk_dzpvVecSrc");

   if( doTopTagger_ ) topTaggerPtr = new topTagger::type3TopTagger();
 
   otherJetspt_TR = new std::vector<double>; otherJetseta_TR = new std::vector<double>; otherJetsphi_TR = new std::vector<double>; otherJetsenergy_TR = new std::vector<double>;
   otherMuspt_TR = new std::vector<double>; otherMuseta_TR = new std::vector<double>; otherMusphi_TR = new std::vector<double>;
   otherElespt_TR = new std::vector<double>; otherEleseta_TR = new std::vector<double>; otherElesphi_TR = new std::vector<double>;
   otherGenJetspt_TR = new std::vector<double>; otherGenJetseta_TR = new std::vector<double>; otherGenJetsphi_TR = new std::vector<double>; otherGenJetsenergy_TR = new std::vector<double>;
   otherJetsRes_TR = new std::vector<double>;

   recoJetsFlavor_TR = new std::vector<int>; genJetsFlavor_TR = new std::vector<int>;
   recoJetschargedHadronEnergyFraction_TR = new std::vector<double>;
   recoJetschargedEmEnergyFraction_TR = new std::vector<double>;

   recoJetsBtag_TR.clear();
  
   for(size ib=0; ib<nbTagKeys; ib++){
      std::vector<double> dummyVec;
      recoJetsBtag_TR.push_back(dummyVec);
   }

   npv = -1; avg_npv = -1; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;

// TLorentzVectors of generator level particles of the top decays
   gentopLVec_TR = new std::vector<TLorentzVector>; gentoppdgIdVec_TR = new std::vector<int>;
   gentopDausLVec_TR = new std::vector<std::vector<TLorentzVector> >; gentopDauspdgIdVec_TR = new std::vector<std::vector<int> >;
   genWDausLVec_TR = new std::vector<std::vector<TLorentzVector> >; genWDauspdgIdVec_TR = new std::vector<std::vector<int> >;
   gentauDausLVec_TR = new std::vector<std::vector<TLorentzVector> >; gentauDauspdgIdVec_TR = new std::vector<std::vector<int> >;
   decayTypeVec_TR = new std::vector<std::string>;

   jetsLVec_TR = new std::vector<TLorentzVector>;
   groomedJetsLVec_TR = new std::vector<TLorentzVector>;

   loose_isoTrksLVec_TR = new std::vector<TLorentzVector>(); loose_isoTrksAuxVec_TR = new std::vector<std::vector<double> >();
   loose_isoTrksConsMatchedJetIdxVec_TR = new std::vector<int>();

   muonsLVec_TR = new std::vector<TLorentzVector>; muonsAux_TR = new std::vector<double>;
   elesLVec_TR = new std::vector<TLorentzVector>; elesAux_TR = new std::vector<double>;

// For HepTopTagger
   hepTaggerJets_TR = new std::vector<TLorentzVector>;
   hepTaggerSubJets_TR = new std::vector<std::vector<TLorentzVector> >;

// For gen decay info
   genDecayStrVec_TR = new std::vector<std::string>;
   genDecayIdxVec_TR = new std::vector<int>; genDecayPdgIdVec_TR = new std::vector<int>; genDecayMomIdxVec_TR = new std::vector<int>;
   genDecayLVec_TR = new std::vector<TLorentzVector>;
   W_emuVec_TR = new std::vector<int>; W_tau_emuVec_TR = new std::vector<int>; W_tau_prongsVec_TR = new std::vector<int>;

// For SMS model info
   smsModelFileNameStr_TR = new std::vector<std::string>;
   smsModelStr_TR = new std::vector<std::string>;
   smsModelMotherMass_TR = -1; smsModelDaughterMass_TR = -1;

// For genjet collections
   genJetsLVec_TR.clear();
   for( unsigned int ig =0; ig<genJetsInputTags_.size(); ig++ ){
      std::vector<TLorentzVector> dummyLVec;
      genJetsLVec_TR.push_back(dummyLVec);
   }

// For isotrk veto
   forVetoIsoTrkIdxVec_TR = new std::vector<int>;

   trksForIsoVetoLVec_TR = new std::vector<TLorentzVector>;
   trksForIsoVetoAux_TR  = new std::vector<std::vector<double> >;

   setTreeDefaultVars();

   edm::Service<TFileService> fs;

   if( doFillTree_ ){

      outTree = fs->make<TTree>("AUX", "aux info");
      outTree->Branch("run", &run, "run/I");
      outTree->Branch("event", &event, "event/I");
      outTree->Branch("lumi", &ls, "lumi/I");
      outTree->Branch("npv", &npv, "npv/I");
      outTree->Branch("avg_npv", &avg_npv, "avg_npv/D");
      outTree->Branch("nm1", &nm1, "nm1/I");
      outTree->Branch("n0", &n0, "n0/I");
      outTree->Branch("np1", &np1, "np1/I");
      outTree->Branch("tru_npv", &tru_npv, "tru_npv/D");
      outTree->Branch("vtxSize", &vtxSize, "vtxSize/I");
      outTree->Branch("nJets", &nJets, "nJets/I");
      outTree->Branch("nMuons", &nMuons, "nMuons/I");
      outTree->Branch("nMuons_CUT", &nMuonsForVeto, "nMuonsForVeto/I");
      outTree->Branch("nElectrons", &nElectrons, "nElectrons/I");
      outTree->Branch("nElectrons_CUT", &nElectronsForVeto, "nElectronsForVeto/I");
      outTree->Branch("nIsoTrks", &loose_nIsoTrks, "loose_nIsoTrks/I");
      outTree->Branch("nIsoTrks_CUT", &nIsoTrksForVeto, "nIsoTrksForVeto/I");
      outTree->Branch("evtWeight", &evtWeight_TR, "evtWeight/D");
      outTree->Branch("mht", &mht_TR, "mht/D");
      outTree->Branch("mht_forSgnf", &mht_forSgnf_TR, "mht_forSgnf/D");
      outTree->Branch("ht", &ht_TR, "ht/D");
      outTree->Branch("met", &met_TR, "met/D");
      outTree->Branch("type1met", &type1met_TR, "type1met/D");
      outTree->Branch("mt", &mt_TR, "mt/D");
      outTree->Branch("type1mt", &type1mt_TR, "type1mt/D");
      outTree->Branch("mhtphi", &mhtphi_TR, "mhtphi/D");
      outTree->Branch("mhtphi_forSgnf", &mhtphi_forSgnf_TR, "mhtphi_forSgnf/D");
      outTree->Branch("metphi", &metphi_TR, "metphi/D");
      outTree->Branch("metSgnf", &metSgnf_TR, "metSgnf/D");
      outTree->Branch("metSgnfProb", &metSgnfProb_TR, "metSgnfProb/D");
      outTree->Branch("type1metphi", &type1metphi_TR, "type1metphi/D");
      outTree->Branch("type1metSgnf", &type1metSgnf_TR, "type1metSgnf/D");
      outTree->Branch("type1metSgnfProb", &type1metSgnfProb_TR, "type1metSgnfProb/D");
      outTree->Branch("mhtSgnf", &mhtSgnf_TR, "mhtSgnf/D");
      outTree->Branch("mhtSgnfProb", &mhtSgnfProb_TR, "mhtSgnfProb/D");
      outTree->Branch("jetsLVec", "std::vector<TLorentzVector>", &jetsLVec_TR, 32000, 0);
      outTree->Branch("groomedJetsLVec", "std::vector<TLorentzVector>", &groomedJetsLVec_TR, 32000, 0);
      outTree->Branch("muonsLVec", "std::vector<TLorentzVector>", &muonsLVec_TR, 32000, 0);
      outTree->Branch("muonsAux", "std::vector<double>", &muonsAux_TR, 32000, 0);
      outTree->Branch("elesLVec", "std::vector<TLorentzVector>", &elesLVec_TR, 32000, 0);
      outTree->Branch("elesAux", "std::vector<double>", &elesAux_TR, 32000, 0);
      outTree->Branch("dPhi0_CUT", &dPhi0_CUT, "dPhi0_CUT/D"); 
      outTree->Branch("dPhi1_CUT", &dPhi1_CUT, "dPhi1_CUT/D"); 
      outTree->Branch("dPhi2_CUT", &dPhi2_CUT, "dPhi2_CUT/D");
      outTree->Branch("nJets_CUT", &nJets_CUT, "nJets_CUT/I");

      outTree->Branch("loose_isoTrksLVec", "std::vector<TLorentzVector>", &loose_isoTrksLVec_TR, 32000, 0);
      outTree->Branch("loose_isoTrksAuxVec", "std::vector<std::vector<double> >", &loose_isoTrksAuxVec_TR, 32000, 0);
      outTree->Branch("loose_isoTrksConsMatchedJetIdxVec", "std::vector<int>", &loose_isoTrksConsMatchedJetIdxVec_TR, 32000, 0);
      outTree->Branch("forVetoIsoTrkIdxVec", "std::vector<int>", &forVetoIsoTrkIdxVec_TR, 32000, 0);
      outTree->Branch("trksForIsoVetoLVec", "std::vector<TLorentzVector>", &trksForIsoVetoLVec_TR, 32000, 0);
      outTree->Branch("trksForIsoVetoAux", "std::vector<std::vector<double> >", &trksForIsoVetoAux_TR, 32000, 0);
    
      if( fillGenInfo_ ){
         outTree->Branch("genJet1pt", &genJet1pt_TR, "genJet1pt/D");
         outTree->Branch("genJet1eta", &genJet1eta_TR, "genJet1eta/D");
         outTree->Branch("genJet1phi", &genJet1phi_TR, "genJet1phi/D");
         outTree->Branch("genJet1energy", &genJet1energy_TR, "genJet1energy/D");
         outTree->Branch("genJet2pt", &genJet2pt_TR, "genJet2pt/D");
         outTree->Branch("genJet2eta", &genJet2eta_TR, "genJet2eta/D");
         outTree->Branch("genJet2phi", &genJet2phi_TR, "genJet2phi/D");
         outTree->Branch("genJet2energy", &genJet2energy_TR, "genJet2energy/D");
         outTree->Branch("genJet3pt", &genJet3pt_TR, "genJet3pt/D");
         outTree->Branch("genJet3eta", &genJet3eta_TR, "genJet3eta/D");
         outTree->Branch("genJet3phi", &genJet3phi_TR, "genJet3phi/D");
         outTree->Branch("genJet3energy", &genJet3energy_TR, "genJet3energy/D");
         outTree->Branch("otherGenJetspt", "std::vector<double>", &otherGenJetspt_TR, 32000, 0);
         outTree->Branch("otherGenJetseta", "std::vector<double>", &otherGenJetseta_TR, 32000, 0);
         outTree->Branch("otherGenJetsphi", "std::vector<double>", &otherGenJetsphi_TR, 32000, 0);
         outTree->Branch("otherGenJetsenergy", "std::vector<double>", &otherGenJetsenergy_TR, 32000, 0);
         outTree->Branch("genMET", &genMET_TR, "genMET/D");
         outTree->Branch("jet1Res", &jet1Res_TR, "jet1Res/D");
         outTree->Branch("jet2Res", &jet2Res_TR, "jet2Res/D");
         outTree->Branch("jet3Res", &jet3Res_TR, "jet3Res/D");
         outTree->Branch("otherJetsRes", "std::vector<double>", &otherJetsRes_TR, 32000, 0);
         outTree->Branch("genJetsFlavor", "std::vector<int>", &genJetsFlavor_TR, 32000, 0);
      }
      
      outTree->Branch("externalBit", &externalBitToTree_TR, "externalBitToTree_TR/I");

      outTree->Branch("recoJetsFlavor", "std::vector<int>", &recoJetsFlavor_TR, 32000, 0);
      outTree->Branch("recoJetschargedHadronEnergyFraction", "std::vector<double>", &recoJetschargedHadronEnergyFraction_TR, 32000, 0);
      outTree->Branch("recoJetschargedEmEnergyFraction", "std::vector<double>", &recoJetschargedEmEnergyFraction_TR, 32000, 0);

      outTree->Branch("bTagKeyString", "std::vector<std::string>", &bTagKeyString_, 32000, 0);
      char treeBranchNameStr[200];
      for(size ib=0; ib<nbTagKeys; ib++){
         sprintf(treeBranchNameStr, "recoJetsBtag_%d", ib);
         outTree->Branch(treeBranchNameStr, "std::vector<double>", &recoJetsBtag_TR[ib], 32000, 0);
      }

      

      if( doFillGenTopInfo_ ){
         outTree->Branch("gentopLVec", "std::vector<TLorentzVector>", &gentopLVec_TR, 32000, 0);
         outTree->Branch("gentoppdgIdVec", "std::vector<int>", &gentoppdgIdVec_TR, 32000, 0);
         outTree->Branch("gentopDausLVec", "std::vector<std::vector<TLorentzVector> >", &gentopDausLVec_TR, 32000, 0);
         outTree->Branch("gentopDauspdgIdVec", "std::vector<std::vector<int> >", &gentopDauspdgIdVec_TR, 32000, 0);
         outTree->Branch("genWDausLVec", "std::vector<std::vector<TLorentzVector> >", &genWDausLVec_TR, 32000, 0);
         outTree->Branch("genWDauspdgIdVec", "std::vector<std::vector<int> >", &genWDauspdgIdVec_TR, 32000, 0);
         outTree->Branch("gentauDausLVec", "std::vector<std::vector<TLorentzVector> >", &gentauDausLVec_TR, 32000, 0);
         outTree->Branch("gentauDauspdgIdVec", "std::vector<std::vector<int> >", &gentauDauspdgIdVec_TR, 32000, 0);
         outTree->Branch("decayTypeVec", "std::vector<std::string>", &decayTypeVec_TR, 32000, 0);
      }
      outTree->Branch("hepTaggerJets", "std::vector<TLorentzVector>", &hepTaggerJets_TR, 32000, 0);
      outTree->Branch("hepTaggerSubJets", "std::vector<std::vector<TLorentzVector> >", &hepTaggerSubJets_TR, 32000, 0);

      if( doTopTagger_ ){
        outTree->Branch("bestTopJetIdx", &bestTopJetIdx_TR, "bestTopJetIdx/I");
        outTree->Branch("pickedRemainingCombfatJetIdx", &pickedRemainingCombfatJetIdx_TR, "pickedRemainingCombfatJetIdx/I");
        outTree->Branch("remainPassCSVS", &remainPassCSVS_TR, "remainPassCSVS/O");
        outTree->Branch("bestTopJetMass", &bestTopJetMass_TR, "bestTopJetMass/D");
        outTree->Branch("mTbestTopJet", &mTbestTopJet_TR, "mTbestTopJet/D");
        outTree->Branch("mTbJet", &mTbJet_TR, "mTbJet/D");
        outTree->Branch("MT2", &MT2_TR, "MT2/D");
        outTree->Branch("mTbestWJet", &mTbestWJet_TR, "mTbestWJet/D");
        outTree->Branch("mTbestbJet", &mTbestbJet_TR, "mTbestbJet/D");
        outTree->Branch("mTremainingTopJet", &mTremainingTopJet_TR, "mTremainingTopJet/D");
     }
     
     if( storeGenDecayInfo_ ){
        outTree->Branch("genDecayStrVec", "std::vector<std::string>", &genDecayStrVec_TR, 32000, 0);
        outTree->Branch("genDecayIdxVec", "std::vector<int>", &genDecayIdxVec_TR, 32000, 0);
        outTree->Branch("genDecayMomIdxVec", "std::vector<int>", &genDecayMomIdxVec_TR, 32000, 0);
        outTree->Branch("genDecayPdgIdVec", "std::vector<int>", &genDecayPdgIdVec_TR, 32000, 0);
        outTree->Branch("genDecayLVec", "std::vector<TLorentzVector>", &genDecayLVec_TR, 32000, 0);
        outTree->Branch("W_emuVec", "std::vector<int>", &W_emuVec_TR, 32000, 0);
        outTree->Branch("W_tau_emuVec", "std::vector<int>", &W_tau_emuVec_TR, 32000, 0);
        outTree->Branch("W_tau_prongsVec", "std::vector<int>", &W_tau_prongsVec_TR, 32000, 0);

        for(unsigned int ig=0; ig<genJetsInputTags_.size(); ig++){
           sprintf(treeBranchNameStr, "genJetsLVec_%s", genJetsInputTags_[ig].label().c_str());
           std::cout<<"Declaring a tree branch with name : "<<treeBranchNameStr<<std::endl;
           outTree->Branch(treeBranchNameStr, "std::vector<TLorentzVector>", &genJetsLVec_TR[ig], 32000, 0);
        }
     }

     if( storeSMSmodelInfo_ ){
        outTree->Branch("smsModelFileNameStrVec", "std::vector<std::string>", &smsModelFileNameStr_TR, 32000, 0);
        outTree->Branch("smsModelStrVec", "std::vector<std::string>", &smsModelStr_TR, 32000, 0);
        outTree->Branch("smsModelMotherMass", &smsModelMotherMass_TR, "smsModelMotherMass/D");
        outTree->Branch("smsModelDaughterMass", &smsModelDaughterMass_TR, "smsModelDaughterMass/D");
     }
   }
}

histAndTree::~histAndTree() {

}

void histAndTree::setTreeDefaultVars(){

   evtWeight_TR = 1.0;
   mht_TR= -99, ht_TR= -99, met_TR= -99, mt_TR= -99, type1mt_TR = -99, type1met_TR = -99;
   mhtphi_TR= -99, metphi_TR= -99, type1metphi_TR = -99;
   metSgnf_TR= -99, metSgnfProb_TR= -99, type1metSgnf_TR= -99, type1metSgnfProb_TR= -99;
   mht_forSgnf_TR = -99, mhtphi_forSgnf_TR = -99;
   mhtSgnf_TR= -99, mhtSgnfProb_TR= -99;
   jet1pt_TR= -99, jet1eta_TR= -99, jet1phi_TR= -99, jet1energy_TR= -99;
   jet2pt_TR= -99, jet2eta_TR= -99, jet2phi_TR= -99, jet2energy_TR= -99;
   jet3pt_TR= -99, jet3eta_TR= -99, jet3phi_TR= -99, jet3energy_TR= -99;
   otherJetspt_TR->clear(); otherJetseta_TR->clear(); otherJetsphi_TR->clear(); otherJetsenergy_TR->clear();
   jetsLVec_TR->clear();
   groomedJetsLVec_TR->clear();
   loose_isoTrksLVec_TR->clear(); loose_isoTrksAuxVec_TR->clear(); loose_isoTrksConsMatchedJetIdxVec_TR->clear();

   bestTopJetIdx_TR = -99; pickedRemainingCombfatJetIdx_TR = -99;
   remainPassCSVS_TR = false;
   bestTopJetMass_TR =-99; mTbestTopJet_TR =-99; mTbJet_TR =-99; MT2_TR =-99; mTbestWJet_TR =-99; mTbestbJet_TR =-99; mTremainingTopJet_TR = -99;
 
   mu1pt_TR= -99, mu1eta_TR= -99, mu1phi_TR= -99;
   mu2pt_TR= -99, mu2eta_TR= -99, mu2phi_TR= -99;
   otherMuspt_TR->clear(); otherMuseta_TR->clear(); otherMusphi_TR->clear();
   ele1pt_TR= -99, ele1eta_TR= -99, ele1phi_TR= -99;
   ele2pt_TR= -99, ele2eta_TR= -99, ele2phi_TR= -99;
   otherElespt_TR->clear(); otherEleseta_TR->clear(); otherElesphi_TR->clear();

   muonsLVec_TR->clear(); elesLVec_TR->clear();
   muonsAux_TR->clear(); elesAux_TR->clear();
 
   forVetoIsoTrkIdxVec_TR->clear();

   trksForIsoVetoLVec_TR->clear();
   trksForIsoVetoAux_TR->clear();

   dPhi0_CUT = -99, dPhi1_CUT = -99, dPhi2_CUT = -99;
   nJets_CUT = -99;

   genJet1pt_TR= -99, genJet1eta_TR= -99, genJet1phi_TR= -99, genJet1energy_TR= -99;
   genJet2pt_TR= -99, genJet2eta_TR= -99, genJet2phi_TR= -99, genJet2energy_TR= -99;
   genJet3pt_TR= -99, genJet3eta_TR= -99, genJet3phi_TR= -99, genJet3energy_TR= -99;
   otherGenJetspt_TR->clear(); otherGenJetseta_TR->clear(); otherGenJetsphi_TR->clear(); otherGenJetsenergy_TR->clear();
   genMET_TR = -99;
   jet1Res_TR = -99, jet2Res_TR = -99, jet3Res_TR = -99;
   otherJetsRes_TR->clear();
   externalBitToTree_TR = -99;

   npv = -1; avg_npv = -1; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;

   recoJetsFlavor_TR->clear(); genJetsFlavor_TR->clear();
   recoJetschargedHadronEnergyFraction_TR->clear();
   recoJetschargedEmEnergyFraction_TR->clear();

   for(size ib=0; ib<nbTagKeys; ib++){
      recoJetsBtag_TR[ib].clear();
   }

   gentopLVec_TR->clear(); gentoppdgIdVec_TR->clear();
   gentopDausLVec_TR->clear(); gentopDauspdgIdVec_TR->clear();
   genWDausLVec_TR->clear(); genWDauspdgIdVec_TR->clear();
   gentauDausLVec_TR->clear(); gentauDauspdgIdVec_TR->clear();
   decayTypeVec_TR->clear();

   hepTaggerJets_TR->clear(); hepTaggerSubJets_TR->clear();

   genDecayStrVec_TR->clear();
   genDecayIdxVec_TR->clear(); genDecayPdgIdVec_TR->clear(); genDecayMomIdxVec_TR->clear();
   genDecayLVec_TR->clear();

   W_emuVec_TR->clear(); W_tau_emuVec_TR->clear(); W_tau_prongsVec_TR->clear();

   smsModelFileNameStr_TR->clear();
   smsModelStr_TR->clear();
   smsModelMotherMass_TR = -1; smsModelDaughterMass_TR = -1;

   for( unsigned int ig =0; ig<genJetsInputTags_.size(); ig++ ){
      genJetsLVec_TR[ig].clear();
   }
}

// ------------ method called on each new Event  ------------
bool histAndTree::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

   setTreeDefaultVars();

//   iSetup.getData( pdt_ );

// Event setup
   loadEventInfo(iEvent, iSetup);

   loadGenInfo(iEvent);
   loadMETMHT(iEvent);
   loadRecoJets(iEvent);
   loadLeptons(iEvent);
   loadHT(iEvent);
   loadAUX(iEvent); if( externalBitToTree_.isValid() ) externalBitToTree_TR = (*externalBitToTree_);

   if( doFillGenTopInfo_ ) fillGenDecayInfo(iEvent);

   if( nLeptonsSels_.size() <2 ){ std::cout<<"\nERROR ... input nLeptonsSels_.size : "<<nLeptonsSels_.size()<<"  less than 2?!"<<std::endl; } 

// If muons or electrons are not the size we require, don't go further
   if( (nLeptonsSels_[0] !=-1 && (int)nMuonsForVeto != nLeptonsSels_[0]) || (nLeptonsSels_[1] !=-1 && (int)nElectronsForVeto != nLeptonsSels_[1]) ){
      if( debug_ ){
         std::cout<<"nMuonsForVeto : "<<nMuonsForVeto<<"  nElectronsForVeto : "<<nElectronsForVeto<<std::endl;
         std::cout<<"nLeptonsSels_[0] : "<<nLeptonsSels_[0]<<"  nLeptonsSels_[1] : "<<nLeptonsSels_[1]<<std::endl;
      }
      return true;
   }

   if( storeGenDecayInfo_ ){
      iEvent.getByLabel(genDecayStrVecSrc_, genDecayStrVec_);
      for(unsigned int id=0; id<genDecayStrVec_->size(); id++){
         genDecayStrVec_TR->push_back( (*genDecayStrVec_)[id] );
      }

      iEvent.getByLabel(genDecayChainPartIdxVecSrc_, genDecayChainPartIdxVec_);
      for(unsigned int id=0; id<genDecayChainPartIdxVec_->size(); id++){
         int idxGen = (*genDecayChainPartIdxVec_)[id];
         const reco::GenParticle& genPart = genParticles->at(idxGen);
         TLorentzVector genPartLVec;
         genPartLVec.SetPtEtaPhiE(genPart.pt(), genPart.eta(), genPart.phi(), genPart.energy());
         int pdgId = genPart.pdgId();

         int momIdx = -1;
         if( genPart.numberOfMothers() >=1 ){
            momIdx = find_idx( * (genPart.mother(0)) );

            bool foundMom = false;
            const reco::Candidate * ptrMomPart = genPart.mother(0);
            int tmpmomIdx = momIdx;
            while(!foundMom && tmpmomIdx >= genDecayChainPartIdxVec_->front() ){
               for(unsigned int jd=0; jd<genDecayChainPartIdxVec_->size(); jd++){
                  if(tmpmomIdx == (*genDecayChainPartIdxVec_)[jd] ){ foundMom = true; break; }
               }
               if( !foundMom ){
                  if( ptrMomPart->numberOfMothers() >= 1 ){
                     ptrMomPart = ptrMomPart->mother(0);
                     tmpmomIdx = find_idx( * ptrMomPart );
                  }else{
                     tmpmomIdx = -1; break;
                  }
               }
            }
            if( foundMom || tmpmomIdx == -1 ) momIdx = tmpmomIdx;
         }
         if( momIdx == -1 ) std::cout<<"WARNING ... idxGen : "<<idxGen<<"  pdgId : "<<pdgId<<"  momIdx : "<<momIdx<<std::endl;

         genDecayIdxVec_TR->push_back(idxGen);
         genDecayMomIdxVec_TR->push_back(momIdx);
         genDecayPdgIdVec_TR->push_back(pdgId);
         genDecayLVec_TR->push_back(genPartLVec);
      }
      if( debug_ ){
         std::cout<<"\nidxGen/pdgId/momIdx : ";
         for(unsigned int ig=0; ig<genDecayIdxVec_TR->size(); ig++){
            std::cout<<"  "<<genDecayIdxVec_TR->at(ig)<<"/"<<genDecayPdgIdVec_TR->at(ig)<<"/"<<genDecayMomIdxVec_TR->at(ig);
         }
         std::cout<<std::endl;
      }

      for(unsigned int ig=0; ig<genJetsInputTags_.size(); ig++){
         edm::Handle<edm::View<reco::GenJet > > perGenJets;
         iEvent.getByLabel(genJetsInputTags_[ig], perGenJets);
         for(unsigned int ip=0; ip<perGenJets->size(); ip++){
            TLorentzVector perGenJetLVec;
            perGenJetLVec.SetPtEtaPhiE((*perGenJets)[ip].pt(), (*perGenJets)[ip].eta(), (*perGenJets)[ip].phi(), (*perGenJets)[ip].energy());
            genJetsLVec_TR[ig].push_back(perGenJetLVec);
         }
      }
      find_W_emu_tauprongs((*W_emuVec_TR), (*W_tau_emuVec_TR), (*W_tau_prongsVec_TR), (*genDecayIdxVec_TR), (*genDecayMomIdxVec_TR), (*genDecayPdgIdVec_TR));
      if( debug_ ){
         std::cout<<"\nW_emu (idx: idxGen/pdgId/momIdx) : ";
         for(unsigned int id=0; id<W_emuVec_TR->size(); id++){
            std::cout<<"  "<<"("<<W_emuVec_TR->at(id)<<": "<<genDecayIdxVec_TR->at(W_emuVec_TR->at(id))<<"/"<<genDecayPdgIdVec_TR->at(W_emuVec_TR->at(id))<<"/"<<genDecayMomIdxVec_TR->at(W_emuVec_TR->at(id))<<")";
         }
         std::cout<<std::endl;
         std::cout<<"\nW_tau_emu (idx: idxGen/pdgId/momIdx) : ";
         for(unsigned int id=0; id<W_tau_emuVec_TR->size(); id++){
            std::cout<<"  "<<"("<<W_tau_emuVec_TR->at(id)<<": "<<genDecayIdxVec_TR->at(W_tau_emuVec_TR->at(id))<<"/"<<genDecayPdgIdVec_TR->at(W_tau_emuVec_TR->at(id))<<"/"<<genDecayMomIdxVec_TR->at(W_tau_emuVec_TR->at(id))<<")";
         }
         std::cout<<std::endl;
         std::cout<<"\nW_tau_prongs (idx: idxGen/pdgId/momIdx) : ";
         for(unsigned int id=0; id<W_tau_prongsVec_TR->size(); id++){
            std::cout<<"  "<<"("<<W_tau_prongsVec_TR->at(id)<<": "<<genDecayIdxVec_TR->at(W_tau_prongsVec_TR->at(id))<<"/"<<genDecayPdgIdVec_TR->at(W_tau_prongsVec_TR->at(id))<<"/"<<genDecayMomIdxVec_TR->at(W_tau_prongsVec_TR->at(id))<<")";
         }
         std::cout<<std::endl;
      }
   }
  
// Selection requirement of jets can be decided by the input jet collection.
   edm::Handle<std::vector<double> >  loose_isotrk_isoVecHandle, loose_isotrk_dzpvVecHandle;
   iEvent.getByLabel(loose_isotrk_isoVecSrc_, loose_isotrk_isoVecHandle);
   iEvent.getByLabel(loose_isotrk_dzpvVecSrc_, loose_isotrk_dzpvVecHandle);
   if( loose_isoTrksHandle_.isValid() && loose_isotrk_isoVecHandle.isValid() && loose_isotrk_dzpvVecHandle.isValid() ){
      if( loose_nIsoTrks != loose_isotrk_isoVecHandle->size() || loose_nIsoTrks != loose_isotrk_dzpvVecHandle->size() ){
         std::cout<<"ERROR ... mis-matching between loose_nIsoTrks : "<<loose_nIsoTrks<<"  loose_isotrk_isoVecHandle->size : "<<loose_isotrk_isoVecHandle->size()<<"  loose_isotrk_dzpvVecHandle->size : "<<loose_isotrk_dzpvVecHandle->size()<<std::endl;
      }
   }

   if( debug_ ) std::cout<<"\nloose_nIsoTrks : "<<loose_nIsoTrks<<"  nIsoTrksForVeto : "<<nIsoTrksForVeto<<std::endl;
   for(unsigned int is=0; is<loose_nIsoTrks; is++){
      const pat::PackedCandidate isoTrk = (*loose_isoTrksHandle_)[is];
      double isoTrkpt = isoTrk.pt(), isoTrketa = isoTrk.eta(), isoTrkphi = isoTrk.phi(), isoTrkenergy = isoTrk.energy();
      double isoTrkcharge = isoTrk.charge();

//      if( isoTrkpt != isoTrkpt ){ std::cout<<"\nSkipping an isolated track where isoTrkpt : "<<isoTrkpt<<" is BAD!"<<std::endl<<std::endl; continue; }

      TLorentzVector perIsoTrkLVec;
      perIsoTrkLVec.SetPtEtaPhiE(isoTrkpt, isoTrketa, isoTrkphi, isoTrkenergy);
      loose_isoTrksLVec_TR->push_back(perIsoTrkLVec);

      double mtw = sqrt( 2*( (*met)[0].pt()*(*loose_isoTrksHandle_)[is].pt() -( (*met)[0].px()*(*loose_isoTrksHandle_)[is].px() + (*met)[0].py()*(*loose_isoTrksHandle_)[is].py() ) ) );
   
      std::vector<double> perAuxVec;
      perAuxVec.push_back(isoTrkcharge); perAuxVec.push_back((*loose_isoTrksHandle_)[is].pdgId()); perAuxVec.push_back((*loose_isotrk_dzpvVecHandle)[is]); perAuxVec.push_back((*loose_isotrk_isoVecHandle)[is]); perAuxVec.push_back(mtw);
      loose_isoTrksAuxVec_TR->push_back(perAuxVec);

// TODO: fix this getCansMatchedJetIdx in miniAOD ...
//      int perIsoTrksConsMatchedJetIdx = getConsMatchedJetIdx((*patjets), perIsoTrkLVec, isoTrkcharge, minDRcutForConsMatch_);
//      loose_isoTrksConsMatchedJetIdxVec_TR->push_back(perIsoTrksConsMatchedJetIdx);
      if( debug_ ){
         std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrkpt<<"/"<<isoTrketa<<"/"<<isoTrkphi<<"/"<<isoTrkcharge<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : "<<(*loose_isotrk_dzpvVecHandle)[is]<<"  iso/pt : "<<(*loose_isotrk_isoVecHandle)[is]/isoTrkpt<<std::endl;
      }
   }
   if( debug_ ) std::cout<<std::endl;

   reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());
   iEvent.getByLabel(pfCandSrc_, pfCandHandle_);
   if( pfCandHandle_.isValid() ){
      for(unsigned int ip=0; ip<pfCandHandle_->size(); ip++){
         TLorentzVector perLVec;
         perLVec.SetPtEtaPhiE( (*pfCandHandle_)[ip].pt(), (*pfCandHandle_)[ip].eta(), (*pfCandHandle_)[ip].phi(), (*pfCandHandle_)[ip].energy() );

         for(unsigned int is=0; is<forVetoIsoTrks_->size(); is++){
            if( (*forVetoIsoTrks_)[is].pt() == (*pfCandHandle_)[ip].pt() && (*forVetoIsoTrks_)[is].eta() == (*pfCandHandle_)[ip].eta() && (*forVetoIsoTrks_)[is].phi() == (*pfCandHandle_)[ip].phi() && (*forVetoIsoTrks_)[is].energy() == (*pfCandHandle_)[ip].energy() && (*forVetoIsoTrks_)[is].pdgId() == (*pfCandHandle_)[ip].pdgId() ){
               forVetoIsoTrkIdxVec_TR->push_back(ip);
            }
         }

         int perCharge = pfCandHandle_->at(ip).charge();
         if( perCharge ==0 ) continue;

//         double dz = (*pfCandHandle_)[ip].pseudoTrack().dz(vtxpos);
         double dz = 100.;
         if( (*pfCandHandle_)[ip].bestTrack() ){
            dz = (*pfCandHandle_)[ip].bestTrack()->dz(vtxpos);
         }
         if( dz > isotrk_dz_ ) continue;

         int matched = 0;
         for(unsigned int is=0; is<loose_nIsoTrks; is++){
            double perdeltaR = perLVec.DeltaR(loose_isoTrksLVec_TR->at(is));
            if( perdeltaR < isotrk_dR_ ) matched ++;
            if( (*loose_isoTrksHandle_)[is].pt() == (*pfCandHandle_)[ip].pt() && (*loose_isoTrksHandle_)[is].eta() == (*pfCandHandle_)[ip].eta() && (*loose_isoTrksHandle_)[is].phi() == (*pfCandHandle_)[ip].phi() && (*loose_isoTrksHandle_)[is].energy() == (*pfCandHandle_)[ip].energy() && (*loose_isoTrksHandle_)[is].pdgId() == (*pfCandHandle_)[ip].pdgId() ){
               (*loose_isoTrksAuxVec_TR)[is].push_back(ip);
            }
         }
         for(unsigned int ig=0; ig<W_emuVec_TR->size(); ig++){
            int perIdx = W_emuVec_TR->at(ig);
            TLorentzVector genLVec = genDecayLVec_TR->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < isotrk_dR_ ) matched ++;
         }

         for(unsigned int ig=0; ig<W_tau_emuVec_TR->size(); ig++){
            int perIdx = W_tau_emuVec_TR->at(ig);
            TLorentzVector genLVec = genDecayLVec_TR->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < isotrk_dR_ ) matched ++;
         }

         for(unsigned int ig=0; ig<W_tau_prongsVec_TR->size(); ig++){
            int perIdx = W_tau_prongsVec_TR->at(ig);
            TLorentzVector genLVec = genDecayLVec_TR->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < isotrk_dR_ ) matched ++;
         }

         if( !matched ) continue;

         trksForIsoVetoLVec_TR->push_back(perLVec);
         vector<double> perAux;
         perAux.push_back(perCharge); perAux.push_back(pfCandHandle_->at(ip).pdgId()); perAux.push_back(dz); perAux.push_back(ip);
         trksForIsoVetoAux_TR->push_back(perAux);
      }
   }
   if( debug_ ){
      std::cout<<"\nntrksForIsoVeto : "<<trksForIsoVetoLVec_TR->size()<<std::endl;
      std::cout<<"idx of forVetoIsoTrks : ";
      for(unsigned int is=0; is<nIsoTrksForVeto; is++){
         std::cout<<"  "<<forVetoIsoTrkIdxVec_TR->at(is);
      }
      std::cout<<std::endl;
      std::cout<<"idx of loose_isoTrks : ";
      for(unsigned int is=0; is<loose_nIsoTrks; is++){
         std::cout<<"  "<<loose_isoTrksAuxVec_TR->at(is).back();
      }
      std::cout<<std::endl;
   }

// end of isotrks

   evtWeight_TR = (*evtWeight_);

   double sgnfMET =0, probMET =0;
   double sgnfType1MET =0, probType1MET =0;
   double sgnfMHT =0, probMHT =0;
   if( doSgnf_ ){ 
      sgnfMET = (*met)[0].significance(); probMET = TMath::Prob(sgnfMET, sgnfnDof); 
      sgnfType1MET = (*type1met)[0].significance(); probType1MET = TMath::Prob(sgnfType1MET, sgnfnDof); 
      sgnfMHT = (*mht_forSgnf)[0].significance(); probMHT = TMath::Prob(sgnfMHT, sgnfnDof); 
   }

   double mtw = -1, type1mtw = -1;
   if( nMuons == 1 ){
      mtw = sqrt( 2*( (*met)[0].pt()*(*muons)[0].pt() -( (*met)[0].px()*(*muons)[0].px() + (*met)[0].py()*(*muons)[0].py() ) ) ); 
      type1mtw = sqrt( 2*( (*type1met)[0].pt()*(*muons)[0].pt() -( (*type1met)[0].px()*(*muons)[0].px() + (*type1met)[0].py()*(*muons)[0].py() ) ) ); 
   }
   if( nMuons ==2 && muon1Charge*muon2Charge == -1 ){
      mtw = sqrt( 2*( (*muons)[1].pt()*(*muons)[0].pt() -( (*muons)[1].px()*(*muons)[0].px() + (*muons)[1].py()*(*muons)[0].py() ) ) );
      type1mtw = sqrt( 2*( (*muons)[1].pt()*(*muons)[0].pt() -( (*muons)[1].px()*(*muons)[0].px() + (*muons)[1].py()*(*muons)[0].py() ) ) );
   }

   mht_TR = (*mht)[0].pt(); ht_TR = (*ht); met_TR = (*met)[0].pt(); type1met_TR = (*type1met)[0].pt(); mt_TR = mtw; type1mt_TR = type1mtw;
   mhtphi_TR = (*mht)[0].phi(); metphi_TR = (*met)[0].phi(); type1metphi_TR = (*type1met)[0].phi();
   metSgnf_TR = sgnfMET; metSgnfProb_TR = probMET;
   type1metSgnf_TR = sgnfType1MET; type1metSgnfProb_TR = probType1MET;
   mhtSgnf_TR = sgnfMHT; mhtSgnfProb_TR = probMHT;
   mht_forSgnf_TR = (*mht_forSgnf)[0].pt(); mhtphi_forSgnf_TR = (*mht_forSgnf)[0].phi();

   if( !isData && fillGenInfo_ ){
      genMET_TR = (*genMET)[0].pt();
      for(size ig=0; ig<nGenJets; ig++){
         if( ig ==0 ){
            genJet1 = (*genJets)[ig];
            genJet1pt_TR = genJet1.pt(); genJet1eta_TR = genJet1.eta(); genJet1phi_TR = genJet1.phi(); genJet1energy_TR = genJet1.energy();
         }
         if( ig ==1 ){
            genJet2 = (*genJets)[ig];
            genJet2pt_TR = genJet2.pt(); genJet2eta_TR = genJet2.eta(); genJet2phi_TR = genJet2.phi(); genJet2energy_TR = genJet2.energy();
         }
         if( ig ==2 ){
            genJet3 = (*genJets)[ig];
            genJet3pt_TR = genJet3.pt(); genJet3eta_TR = genJet3.eta(); genJet3phi_TR = genJet3.phi(); genJet3energy_TR = genJet3.energy();
         }
         if( ig >2 ){
            genJetother = (*genJets)[ig];
            otherGenJetspt_TR->push_back(genJetother.pt()); otherGenJetseta_TR->push_back(genJetother.eta()); otherGenJetsphi_TR->push_back(genJetother.phi()); otherGenJetsenergy_TR->push_back(genJetother.energy());
         }
      }   
   }

   if( groomedJetsLVec_.isValid() && groomedJetsIdx_.isValid() ){
      if( groomedJetsLVec_->size() != groomedJetsIdx_->size() || groomedJetsLVec_->size() != nJets ){
         std::cout<<"ERROR ... mismatch between  groomedJetsLVec_->size : "<<groomedJetsLVec_->size()<<"  groomedJetsIdx_->size : "<<groomedJetsIdx_->size()<<"  nJets : "<<nJets<<std::endl;
         return false;
      }
   }

   for(size ij=0; ij<nJets; ij++){
      if( !isData && fillGenInfo_ ){
         double jeteta = (*jets)[ij].eta(), jetphi = (*jets)[ij].phi(), jetpt = (*jets)[ij].pt();
         double minDR = 999.0, jetRes = -999.0;
         for(size ig=0; ig<nGenJets; ig++){
            double genjeteta = (*genJets)[ig].eta(), genjetphi = (*genJets)[ig].phi(), genjetpt = (*genJets)[ig].pt();
            const double dR = reco::deltaR(jeteta, jetphi, genjeteta, genjetphi);
            if( minDR > dR ){ minDR = dR; jetRes = jetpt/genjetpt; }
         }
         if( minDR < recoGenJetsDR_ ){
            if( ij == 0 ){
               jet1Res_TR = jetRes;
            }
            if( ij == 1 ){
               jet2Res_TR = jetRes;
            }
            if( ij == 2 ){
               jet3Res_TR = jetRes;
            }
            if( ij > 2 ){
               otherJetsRes_TR->push_back(jetRes);
            }
         }
      }
      TLorentzVector perJetLVec;
      perJetLVec.SetPtEtaPhiE( (*jets)[ij].pt(), (*jets)[ij].eta(), (*jets)[ij].phi(), (*jets)[ij].energy() );
      jetsLVec_TR->push_back(perJetLVec);
      if( groomedJetsLVec_.isValid() && groomedJetsIdx_.isValid() ){
         if( (*groomedJetsIdx_)[ij].size() != 1 ){
            std::cout<<"ERROR ... per groomedJetsIdx_[].size : "<<(*groomedJetsIdx_)[ij].size()<<" NOT 1?"<<std::endl;
            return false;
         }
         if( (*groomedJetsIdx_)[ij][0] != (int)ij ){
            std::cout<<"ERROR ... mismatch between (*groomedJetsIdx_)[ij][0] : "<<(*groomedJetsIdx_)[ij][0]<<" and ij : "<<ij<<std::endl;
            return false;
         }
         groomedJetsLVec_TR->push_back((*groomedJetsLVec_)[ij]);
      }

      if( ij ==0 ){
         jet1 = (*jets)[ij];
         jet1pt_TR = jet1.pt(); jet1eta_TR = jet1.eta(); jet1phi_TR = jet1.phi(); jet1energy_TR = jet1.energy();
      }
      if( ij ==1 ){
         jet2 = (*jets)[ij];
         jet2pt_TR = jet2.pt(); jet2eta_TR = jet2.eta(); jet2phi_TR = jet2.phi(); jet2energy_TR = jet2.energy();
      }
      if( ij ==2 ){
         jet3 = (*jets)[ij]; 
         jet3pt_TR = jet3.pt(); jet3eta_TR = jet3.eta(); jet3phi_TR = jet3.phi(); jet3energy_TR = jet3.energy();
      }
      if( ij > 2 ){
         jetother = (*jets)[ij];
         otherJetspt_TR->push_back(jetother.pt()); otherJetseta_TR->push_back(jetother.eta()); otherJetsphi_TR->push_back(jetother.phi()); otherJetsenergy_TR->push_back(jetother.energy());
      }
   }

   if( isPatJet )
   {
      for(size ij=0; ij<nJets; ij++)
      {
         const pat::Jet& jet = (*patjets)[ij];
         int flavor = jet.partonFlavour();
         recoJetsFlavor_TR->push_back(flavor);
      
         double chargedHadronEnergyFraction = jet.chargedHadronEnergyFraction();
         recoJetschargedHadronEnergyFraction_TR->push_back( chargedHadronEnergyFraction );

         double chargedEmEnergyFraction = jet.chargedEmEnergyFraction();
         recoJetschargedEmEnergyFraction_TR->push_back( chargedEmEnergyFraction );
  
         for(size ib=0; ib<nbTagKeys; ib++)
         {
            double btag = jet.bDiscriminator(bTagKeyString_[ib].c_str());
            recoJetsBtag_TR[ib].push_back(btag);
         }
      }
   }

   if( doTopTagger_ ){

      std::vector<TLorentzVector> oriJetsVec;
      std::vector<double> recoJetsBtagCSVS;
      for(unsigned int ij=0; ij<jetsLVec_TR->size(); ij++){
         double jetEta = jetsLVec_TR->at(ij).Eta(), jetPt = jetsLVec_TR->at(ij).Pt();
         if( maxJetEtaCutForTopTagger_ != -1 && fabs(jetEta) >= maxJetEtaCutForTopTagger_ ) continue;
         if( minJetPtCutForTopTagger_ != -1 && jetPt <= minJetPtCutForTopTagger_ ) continue;
         oriJetsVec.push_back( jetsLVec_TR->at(ij) );
         recoJetsBtagCSVS.push_back( recoJetsBtag_TR.at(0)[ij] );
      }

      TLorentzVector metLVec; metLVec.SetPtEtaPhiM( met_TR, 0, metphi_TR, 0);

      bool passTopTagger = topTaggerPtr->processEvent(oriJetsVec, recoJetsBtagCSVS, metLVec);
      if( passTopTagger ){/*empty to avoid a compiling error*/}

      bestTopJetIdx_TR = topTaggerPtr->bestTopJetIdx; pickedRemainingCombfatJetIdx_TR = topTaggerPtr->pickedRemainingCombfatJetIdx;
      remainPassCSVS_TR = topTaggerPtr->remainPassCSVS;
      bestTopJetMass_TR = topTaggerPtr->bestTopJetMass;
      mTbestTopJet_TR = topTaggerPtr->mTbestTopJet;
      mTbJet_TR = topTaggerPtr->mTbJet;
      MT2_TR = topTaggerPtr->MT2;
      mTbestWJet_TR = topTaggerPtr->mTbestWJet;
      mTbestbJet_TR = topTaggerPtr->mTbestbJet;
      mTremainingTopJet_TR = topTaggerPtr->mTremainingTopJet;

   }

   if( storeSMSmodelInfo_ ){
      iEvent.getByLabel(smsModelFileNameStrSrc_, smsModelFileNameStrVec_);
      iEvent.getByLabel(smsModelStrSrc_, smsModelStrVec_);
      iEvent.getByLabel(smsModelMotherMassSrc_, smsModelMotherMass_);
      iEvent.getByLabel(smsModelDaughterMassSrc_, smsModelDaughterMass_);
      for(unsigned int is=0; is<smsModelFileNameStrVec_->size(); is++){
         smsModelFileNameStr_TR->push_back((*smsModelFileNameStrVec_)[is]);
      }
      for(unsigned int is=0; is<smsModelStrVec_->size(); is++){
         smsModelStr_TR->push_back((*smsModelStrVec_)[is]);
      }
      smsModelMotherMass_TR = (*smsModelMotherMass_);
      smsModelDaughterMass_TR = (*smsModelDaughterMass_);
   }

   if( HEPTopTaggedCandidateHandle.isValid() ){

      for(size_t i = 0; i < HEPTopTaggedCandidateHandle->size(); ++i){
         reco::BasicJetRef itJet (HEPTopTaggedCandidateHandle, i);

         TLorentzVector perJetLVec;
         perJetLVec.SetPtEtaPhiE(itJet->pt(), itJet->eta(), itJet->phi(), itJet->energy());
         hepTaggerJets_TR->push_back(perJetLVec);

         reco::Jet::Constituents subjets = itJet->getJetConstituents();

         sort(subjets.begin(), subjets.end(), GreaterByPtCandPtr()); //Sort them by pt
         
         std::vector<TLorentzVector> subJetsVec;

         for(size_t is=0; is<subjets.size(); is++){
            TLorentzVector persubJetLVec;
            persubJetLVec.SetPtEtaPhiE(subjets[is]->p4().pt(), subjets[is]->p4().eta(), subjets[is]->p4().phi(), subjets[is]->p4().energy());
            subJetsVec.push_back(persubJetLVec);
         }
         hepTaggerSubJets_TR->push_back(subJetsVec);
      }
   }

   for(size im=0; im<nMuons; im++){
      TLorentzVector perMuonLVec;
      perMuonLVec.SetPtEtaPhiE( (*muons)[im].pt(), (*muons)[im].eta(), (*muons)[im].phi(), (*muons)[im].energy() );

      double mtw = sqrt( 2*( (*met)[0].pt()*(*muons)[im].pt() -( (*met)[0].px()*(*muons)[im].px() + (*met)[0].py()*(*muons)[im].py() ) ) );

      muonsLVec_TR->push_back(perMuonLVec);
      muonsAux_TR->push_back( (*muons)[im].charge() ); muonsAux_TR->push_back( mtw );

      if( im == 0){
         mu1pt_TR = (*muons)[0].pt(); mu1eta_TR = (*muons)[0].eta(); mu1phi_TR = (*muons)[0].phi();
      }
      if( im == 1){
         mu2pt_TR = (*muons)[1].pt(); mu2eta_TR = (*muons)[1].eta(); mu2phi_TR = (*muons)[1].phi();
      }
      if( im >1 ){
         otherMuspt_TR->push_back((*muons)[im].pt()); otherMuseta_TR->push_back((*muons)[im].eta()); otherMusphi_TR->push_back((*muons)[im].phi());
      }
   }

   for(size ie=0; ie<nElectrons; ie++){
      TLorentzVector perEleLVec;
      perEleLVec.SetPtEtaPhiE( (*electrons)[ie].pt(), (*electrons)[ie].eta(), (*electrons)[ie].phi(), (*electrons)[ie].energy() );

      double mtw = sqrt( 2*( (*met)[0].pt()*(*electrons)[ie].pt() -( (*met)[0].px()*(*electrons)[ie].px() + (*met)[0].py()*(*electrons)[ie].py() ) ) );

      elesLVec_TR->push_back(perEleLVec);
      elesAux_TR->push_back( (*electrons)[ie].charge() ); elesAux_TR->push_back( mtw );

      if( ie == 0){
         ele1pt_TR = (*electrons)[ie].pt(); ele1eta_TR = (*electrons)[ie].eta(); ele1phi_TR = (*electrons)[ie].phi();
      }
      if( ie == 1){
         ele2pt_TR = (*electrons)[ie].pt(); ele2eta_TR = (*electrons)[ie].eta(); ele2phi_TR = (*electrons)[ie].phi();
      }
      if( ie >1 ){
         otherElespt_TR->push_back((*electrons)[ie].pt()); otherEleseta_TR->push_back((*electrons)[ie].eta()); otherElesphi_TR->push_back((*electrons)[ie].phi());
      }
   }

   if( doFillTree_ ){
      dPhi0_CUT = (*dPhis_CUT_vec_)[0]; dPhi1_CUT = (*dPhis_CUT_vec_)[1]; dPhi2_CUT = (*dPhis_CUT_vec_)[2];
      nJets_CUT = (*nJets_CUT_);
      outTree->Fill(); 
   }

   return true;

}

// ------------ method called once each job just before starting event loop  ------------
void histAndTree::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void histAndTree::endJob() {
}

// ------------ method called once each run just before starting event loop  ------------
bool histAndTree::beginRun(edm::Run &run, const edm::EventSetup& iSetup) {
  return true;
}

// ------------ method called once each run just after starting event loop  ------------
bool histAndTree::endRun(edm::Run &run, const edm::EventSetup& iSetup) {
  return true;
}


void histAndTree::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();

// Get vertices
   iEvent.getByLabel(vtxSrc_, vertices); vtxSize = vertices->size();
   
// Get event weight
   iEvent.getByLabel(evtWeightInput_, evtWeight_);

}

void histAndTree::loadGenInfo(const edm::Event& iEvent){

// MC generate level related info
   scalePDF = -1; pthat = -1;
   if (!isData) {
      edm::Handle<edm::HepMCProduct> evt;
      edm::Handle< GenEventInfoProduct > GenInfoHandle;

      iEvent.getByLabel("generator", evt);
      if (evt.isValid()) {
         HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(evt->GetEvent()));
         scalePDF = myGenEvent->event_scale();
         if( myGenEvent ) delete myGenEvent;
      }

      iEvent.getByLabel( "generator", GenInfoHandle );
      if (GenInfoHandle.isValid()) { pthat = ( GenInfoHandle->hasBinningValues() ? (GenInfoHandle->binningValues())[0] : 0.0); }

      iEvent.getByLabel(genParticleSrc_, genParticles);
      iEvent.getByLabel(genJetSrc_, genJets); nGenJets = genJets->size();
      iEvent.getByLabel(genMETSrc_, genMET); 

      iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
      std::vector<PileupSummaryInfo>::const_iterator PVI;

      npv = -1; avg_npv = 0; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;
      for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

         int BX = PVI->getBunchCrossing();

         avg_npv += double(PVI->getPU_NumInteractions());

         if(BX == -1) { 
            nm1 = PVI->getPU_NumInteractions();
         }
         if(BX == 0) { 
            n0 = PVI->getPU_NumInteractions();
         }
         if(BX == 1) { 
            np1 = PVI->getPU_NumInteractions();
         }

         if(BX == 0) {
            npv = PVI->getPU_NumInteractions();
            tru_npv = PVI->getTrueNumInteractions();
            continue; // No touching of this "continue", since I'm not sure why it's here originally
         }
      }
      avg_npv /= 3.0;
   }
}

void histAndTree::loadRecoJets(const edm::Event& iEvent){
   iEvent.getByLabel(jetSrc_, jets); nJets = jets->size();
   iEvent.getByLabel(groomedJetSrc_, groomedJetsLVec_);
   iEvent.getByLabel(groomedJetIdxSrc_, groomedJetsIdx_);

   isPatJet = false;
   edm::ProductID jetProdID = jets.id();
   const edm::Provenance & jetProv = iEvent.getProvenance(jetProdID);
   const std::string jetclassName = jetProv.className();
   TString jetclassNameT(jetclassName);
   if( jetclassNameT.Contains("pat::Jet") ) isPatJet = true;

   if( isPatJet ) iEvent.getByLabel(jetSrc_, patjets);
   
   iEvent.getByLabel(hepTaggerJetSrc_, HEPTopTaggedCandidateHandle);
}

void histAndTree::loadLeptons(const edm::Event& iEvent){
   iEvent.getByLabel(muonSrc_, muons); nMuons = muons->size();
   iEvent.getByLabel(eleSrc_, electrons); nElectrons = electrons->size();
//   iEvent.getByLabel(tauSrc_, taus); nTaus = taus->size();
   iEvent.getByLabel(forVetoMuonSrc_, forVetoMuons); nMuonsForVeto = forVetoMuons->size();
   iEvent.getByLabel(forVetoElectronSrc_, forVetoElectrons); nElectronsForVeto = forVetoElectrons->size();

   for( size im=0; im<nMuons; im++){
//      if( im ==0 ){ muon1 = (*muons)[im]; muon2 = (*muons)[im]; }
//      if( im ==1 ) muon2 = (*muons)[im];
      if( im ==0 ){ muon1Charge = (*muons)[im].charge(); muon2Charge = (*muons)[im].charge(); }
      if( im ==1 ) muon2Charge = (*muons)[im].charge();
   }

//   muon1Charge = muon1.charge(); muon2Charge = muon2.charge();

   for( size ie=0; ie<nElectrons; ie++){
//      if( ie ==0 ){ ele1 = (*electrons)[ie]; ele2 = (*electrons)[ie]; }
//      if( ie ==1 ) ele2 = (*electrons)[ie];
      if( ie ==0 ){ ele1Charge = (*electrons)[ie].charge(); ele2Charge = (*electrons)[ie].charge(); }
      if( ie ==1 ) ele2Charge = (*electrons)[ie].charge();
   }

//   ele1Charge = ele1.charge(); ele2Charge = ele2.charge();

// For isotrack vetos
   iEvent.getByLabel(loose_isoTrkSrc_, loose_isoTrksHandle_); if( loose_isoTrksHandle_.isValid() ) loose_nIsoTrks = loose_isoTrksHandle_->size(); else loose_nIsoTrks =0;
   iEvent.getByLabel(forVetoIsoTrkSrc_, forVetoIsoTrks_); if( forVetoIsoTrks_.isValid() ) nIsoTrksForVeto = forVetoIsoTrks_->size(); else nIsoTrksForVeto =0;
}

void histAndTree::loadPhotons(const edm::Event& iEvent){
   iEvent.getByLabel(photonSrc_, photons); nPhotons = photons->size();
}

void histAndTree::loadMETMHT(const edm::Event& iEvent){
   iEvent.getByLabel(metSrc_, met);
   iEvent.getByLabel(type1metSrc_, type1met);
   iEvent.getByLabel(mhtSrc_, mht);
   iEvent.getByLabel(mht_forSgnfSrc_, mht_forSgnf);
}

void histAndTree::loadHT(const edm::Event& iEvent){
   iEvent.getByLabel(htSrc_, ht);
}

void histAndTree::loadAUX(const edm::Event& iEvent){
   iEvent.getByLabel(dPhis_CUT_vec_Src_, dPhis_CUT_vec_);
   iEvent.getByLabel(nJets_CUT_Src_, nJets_CUT_);

   iEvent.getByLabel(externalBitToTree_Src_, externalBitToTree_);
}

void histAndTree::fillGenDecayInfo(const edm::Event& iEvent){
   if( !isData ){

      iEvent.getByLabel(gentopIdxVecSrc_, gentopIdxVec_);
      iEvent.getByLabel(gentopDausIdxVecSrc_, gentopDausIdxVec_);
      iEvent.getByLabel(genWDausIdxVecSrc_, genWDausIdxVec_);
      iEvent.getByLabel(gentauDausIdxVecSrc_, gentauDausIdxVec_);
      iEvent.getByLabel(decayTypeVecSrc_, decayTypeVec_);

      unsigned int nGentops = gentopIdxVec_->size();
      for(unsigned int it=0; it<nGentops; it++){
         int gentopIdx = (*gentopIdxVec_)[it];
         const reco::GenParticle& gentop = genParticles->at(gentopIdx);
         TLorentzVector gentopLVec;
         gentopLVec.SetPtEtaPhiE(gentop.pt(), gentop.eta(), gentop.phi(), gentop.energy());

         vector<TLorentzVector> gentopDausLVec, genWDausLVec, gentauDausLVec;
         vector<int> gentopDauspdgIdVec, genWDauspdgIdVec, gentauDauspdgIdVec;
         for(unsigned int itd=0; itd<(*gentopDausIdxVec_)[it].size(); itd++){
            int gentopDausIdx = (*gentopDausIdxVec_)[it][itd];
            const reco::GenParticle &gentopDau = genParticles->at(gentopDausIdx);
            TLorentzVector perLVec;
            perLVec.SetPtEtaPhiE(gentopDau.pt(), gentopDau.eta(), gentopDau.phi(), gentopDau.energy());

            gentopDausLVec.push_back(perLVec);
            gentopDauspdgIdVec.push_back(gentopDau.pdgId());
         }
         for(unsigned int iwd=0; iwd<(*genWDausIdxVec_)[it].size(); iwd++){
            int genWDausIdx = (*genWDausIdxVec_)[it][iwd];
            const reco::GenParticle &genWDau = genParticles->at(genWDausIdx);
            TLorentzVector perLVec;
            perLVec.SetPtEtaPhiE(genWDau.pt(), genWDau.eta(), genWDau.phi(), genWDau.energy());

            genWDausLVec.push_back(perLVec);
            genWDauspdgIdVec.push_back(genWDau.pdgId());
         }
         for(unsigned int itd=0; itd<(*gentauDausIdxVec_)[it].size(); itd++){
            int gentauDausIdx = (*gentauDausIdxVec_)[it][itd];
            const reco::GenParticle &gentauDau = genParticles->at(gentauDausIdx);
            TLorentzVector perLVec;
            perLVec.SetPtEtaPhiE(gentauDau.pt(), gentauDau.eta(), gentauDau.phi(), gentauDau.energy());

            gentauDausLVec.push_back(perLVec);
            gentauDauspdgIdVec.push_back(gentauDau.pdgId());
         }
         gentopLVec_TR->push_back(gentopLVec); gentoppdgIdVec_TR->push_back(gentop.pdgId());
         gentopDausLVec_TR->push_back(gentopDausLVec); gentopDauspdgIdVec_TR->push_back(gentopDauspdgIdVec);
         genWDausLVec_TR->push_back(genWDausLVec); genWDauspdgIdVec_TR->push_back(genWDauspdgIdVec);
         gentauDausLVec_TR->push_back(gentauDausLVec); gentauDauspdgIdVec_TR->push_back(gentauDauspdgIdVec);
         decayTypeVec_TR->push_back((*decayTypeVec_)[it]);
      }
   }
}

int histAndTree::getConsMatchedJetIdx(const std::vector<pat::Jet> & patjets, const TLorentzVector tomatchLVec, const double tomatchCharge, const double minDRcut){

   int consMatchedJetsIdx = -1;
   double tomatchEta = tomatchLVec.Eta(), tomatchPhi = tomatchLVec.Phi();
   for(int ij=0; ij<(int)patjets.size(); ij++){

      const std::vector<reco::PFCandidatePtr> & constituents = patjets[ij].getPFConstituents();
      const unsigned int numConstituents = constituents.size();

      double minDRCon = 999.; int selConIdx = -1;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
         const reco::PFCandidatePtr& constituent = constituents[iCon];
         if( constituent->charge() != tomatchCharge ) continue;
         const double tomatchDR = reco::deltaR(constituent->eta(), constituent->phi(), tomatchEta, tomatchPhi);
         if( minDRCon > tomatchDR ){
            minDRCon = tomatchDR;
            selConIdx = (int)iCon;
         }
      }
      if( selConIdx ){/*empty to avoid a compiling error*/}

      if( minDRCon < minDRcut ){
         consMatchedJetsIdx = ij;
         break;
      }
   }

   return consMatchedJetsIdx;
}

int histAndTree::find_idx(const reco::Candidate & target){
   int pickedIdx = -1;
   for(size_t ig=0; ig<genParticles->size(); ig++){
      const reco::GenParticle& gen = genParticles->at(ig);
      if( target.p4() == gen.p4() && target.vertex() == gen.vertex() && target.charge() == gen.charge() ){
         pickedIdx = (int)ig;
         break;
      }
   }
   return pickedIdx;
}

bool histAndTree::find_mother(int momPdgId, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec){
   if( dauIdx == -1 ) return false;

   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx != -1 ){
         if( abs(genDecayPdgIdVec[thisIdx]) == momPdgId ) return true;
      }
   }
   return false;
}

bool histAndTree::find_mother(int momIdx, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec){
   if( momIdx == -1 || dauIdx == -1 ) return false;

   if( dauIdx == momIdx ) return true;

   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx == momIdx ) return true;
   }
   return false;
}

void histAndTree::find_mother(std::vector<int> & momIdxVec, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec){
   momIdxVec.clear();
   if( dauIdx == -1 ) return;
   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx != -1 ) momIdxVec.push_back(thisIdx);
   }
   return;
}

int histAndTree::find_idx(int genIdx, const vector<int> &genDecayIdxVec){
   for(int ig=0; ig<(int)genDecayIdxVec.size(); ig++){
      if( genDecayIdxVec[ig] == genIdx ) return ig;
   }
   return -1;
}

void histAndTree::find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec){
   W_emuVec.clear(); W_tau_emuVec.clear(); W_tau_prongsVec.clear();
   if( !genDecayPdgIdVec.empty() ){
      for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
         int pdgId = genDecayPdgIdVec.at(ig);
         if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 211 || abs(pdgId) == 321 ){
            vector<int> momIdxVec;
            find_mother(momIdxVec, ig, genDecayIdxVec, genDecayMomIdxVec);
            int cntMomW = 0, cntMomTau = 0;
            for(unsigned int im=0; im<momIdxVec.size(); im++){
// Make this one general for both W and Z decays (need to be careful when both Z and W appear in the decay chain, 
// but this is unlikely given currently how the MC are generated and how the genDecayXXX vectors are produced)
//               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 24 ) cntMomW ++;
               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 24 || abs(genDecayPdgIdVec[momIdxVec[im]]) == 23 ) cntMomW ++;
               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 15 ) cntMomTau ++;
            }
            if( cntMomW ){
               if( cntMomTau ){
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_tau_emuVec.push_back(ig);
                  else W_tau_prongsVec.push_back(ig);
               }else{
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_emuVec.push_back(ig);
               }
            }
         }
      }
   }
   return;
}

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE(histAndTree);
