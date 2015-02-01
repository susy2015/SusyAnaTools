#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/METReco/interface/MET.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#include "SusyAnaTools/TopTagger/interface/combination.h"
#include "SusyAnaTools/TopTagger/interface/indexSort.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"
#include "TLorentzVector.h"

class cmpPtLorentz {
  public:
    bool operator() (const math::XYZTLorentzVector &r1, const math::XYZTLorentzVector &r2) {
      return r1.pt()>r2.pt();
    }
};

static const int nEtaBins = 4;
static const std::string etaBinStrs[] = { "barrelA",   "barrelB",  "transition",   "endcap"  };
static const double etaBinDivs[]      = {0,         0.5,        1.1,           1.7,        -1}; // -1 means inf
static const int nPtBins = 4;
static const std::string ptBinStrs[]  = { "20to30",  "30to50",  "50to100",   "100toInf"  };
static const double ptBinDivs[]       = {20,       30,        50,         100,         -1}; // -1 means inf
static const int nVISBins = 300;
static const double lowVISfracRange = -0.5, highVISfracRange = 2.5;

static const UInt_t fSeed = 782365;

typedef unsigned int size;

using namespace edm;
using namespace std;

class HadronicTauSignalContamination : public edm::EDProducer {

  public:

    explicit HadronicTauSignalContamination(const edm::ParameterSet & iConfig);
    ~HadronicTauSignalContamination();

  private:

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    virtual void loadGenParticles(const edm::Event& iEvent);
    virtual void loadGenJets(const edm::Event& iEvent);

    edm::InputTag jetSrc_, oriJetSrc_;
    edm::Handle<std::vector<pat::Jet> > jets, orijets;
    edm::InputTag bigPtDiffSrc_;
    edm::InputTag muonSrc_;
    edm::InputTag muonForJetOverlapSrc_;
    edm::InputTag eleSrc_;

    edm::InputTag forVetoIsoTrkSrc_;
    edm::Handle<edm::View<reco::PFCandidate> > forVetoIsoTrks;
    size nIsoTrksForVeto;

    std::string jecLevel_;

    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;

    edm::InputTag vtxSrc_;
    edm::Handle<std::vector<reco::Vertex> > vertices;

    edm::InputTag evtWeightInput_;
    edm::Handle<double> evtWeight_;

    unsigned int nVtxPUcut_;
    unsigned int vtxSize;

    bool debug_;

    bool isData;
// For better debugging
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    unsigned int run, event, ls; double pthat, scalePDF;
    Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    int npv; double avg_npv; int nm1, n0, np1; double tru_npv;

    TRandom3 *dicer;

    bool doFillTree_;

    std::string templateRootName_;
    TFile *templateFile;

// Always pick the first one for each muon, since I expect
// for either iso and reco the parameterization is the same and
// different parts are combined, i.e., the global, tracker and ID efficiency 
// are combined.
// Also we always pick one muon, since this is the single muon control sample!

// Store all the jets satisfying pfJetCutForJetStoring_.
// Only need to be stored once before smearing since smearing has nothing
// to do with the existing jets --> This way saves big space!
    std::vector<TLorentzVector> *jetsLVec_TR;
    std::string bTagKeyString_;
    std::vector<double> *jetsCSVS_TR;
    StringCutObjectSelector<reco::Jet,true> pfJetCutForJetStoring_; // lazy parsing, to allow cutting on variables not in reco::Candidate class

    double tauJetReplacedbJetCSVS_TR;

    void setTreeDefaultVars();

// This is in eta bins
    std::vector<TH1D*> visEnergyTemplateInEtaBinsVec, rawVisEnergyTemplateInEtaBinsVec;
    std::vector<TH1D*> visEnergyTemplateInPtBinsVec, rawVisEnergyTemplateInPtBinsVec;
    TH1D * h1_visEnergyTemplateTot, *h1_rawVisEnergyTemplateTot;

    int initTauTemplate();
// 0 : use template for different pt bins
// 1 : use template for different eta bins
// 2 : use one single template
// 3 : special evaluation using the last pt bin
    unsigned int applyMethod_;

//    vector<double> calculateCorrFactor(const JetCorrector * corrector, const std::vector<pat::Muon> &myIsoMuon);
    vector<double> calculateCorrFactor(const JetCorrector * corrector, const std::vector<math::XYZTLorentzVector> &myIsoMuon, edm::Event& iEvent, const edm::EventSetup& iSetup);
// My jet input would be jet collections cleaned overlapping with muon
//    std::vector<math::XYZTLorentzVector> predictTAUJets( const std::vector<pat::Jet> &myjet, const std::vector<pat::Muon> &myIsoMuon , double corr_factor, double muonDeposit, math::XYZTLorentzVector &smearedMuon, int addSmearedMuonIn );
    std::vector<math::XYZTLorentzVector> predictTAUJets( const std::vector<math::XYZTLorentzVector> &myjet, const std::vector<math::XYZTLorentzVector> &myIsoMuon , double corr_factor, math::XYZTLorentzVector &smearedMuon, int addSmearedMuonIn );
    std::vector<math::XYZTLorentzVector> predictTAUJets( const std::vector<math::XYZTLorentzVector> &myjet, const std::vector<math::XYZTLorentzVector> &myIsoMuon , const vector<double> &corrFactorVec, math::XYZTLorentzVector &smearedMuon, edm::Event& iEvent, const edm::EventSetup& iSetup, int addSmearedMuonIn );

    bool displaceMuon_, useDeposit_;

// Two values each vector ==> 0 : pt   1 : eta
    std::vector<double> jetMHTcuts_, jetHTcuts_;
 
    std::vector<double> calcHT(const std::vector<math::XYZTLorentzVector> &newJet);
    math::XYZTLorentzVector calcMHT(const std::vector<math::XYZTLorentzVector> &newJet);
    std::vector<double> calcJetMHTDPhi(const std::vector<math::XYZTLorentzVector> &newJet, const math::XYZTLorentzVector &newMHT);
    std::vector<double> calcJetMHTDPhi( const std::vector<pat::Jet> &myjet, const edm::View<reco::MET> &mymht);

// Standard RA2 cut values
    edm::InputTag mhtInputTag_, htInputTag_;

    edm::Handle<edm::View<reco::MET> > mht;
    edm::Handle<double> ht;

    edm::InputTag metInputTag_;
    edm::Handle<edm::View<reco::MET> > met;

    edm::InputTag effWeightsInput_, effWeightErr2sInput_;
    edm::Handle<std::vector<double> > effWeights_, effWeightErr2s_;

// Acceptance... Have to be in this module, since we need a tau template smearing of the generate level muon!
    bool doACC_;
// Two values each vector ==> 0 : pt   1 : eta
    std::vector<double> muonKincuts_;
    bool doZfilter_;
    std::vector<int> lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec);
    bool find_mother(const reco::Candidate* genp, const int motherID);

// Two values each vector ==> 0 : lower cut  1 : higher cut
// Default value of -1 for both means MT paramter limits for lower and higher ranges, i.e., 0 for lower and infinity for higher.
    std::vector<double> cutMTvals_;

// ==> Sys. Errors
// Scale the tau visible energy template  0 : no scale  +X : scale (1+X)  -X : scale (1-X)
    double scaleVIS_;

    int Ntry_;

// ==> Sys. Errors
// Change MET by JEC scale by plus or minus 1 uncertainty
    std::string jecUncTypeInput_;
    double jecUncJetMinPtCut_;
    JetCorrectionUncertainty *jecUnc;
    void loadJecUnc(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    math::XYZTLorentzVector calcJecUncCorrMET(const std::vector<pat::Jet> &myjet);

    int subtractMuonPt;

// To store top tagger information
    bool doTopTagger_;
    double maxJetEtaCutForTopTagger_, minJetPtCutForTopTagger_;

    topTagger::type3TopTagger *topTaggerPtr;

// Jet counting, dphi and others for Stop analysis
    int countJets(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &jetCutsVec);
    int countJets(const std::vector<TLorentzVector> &inputJets, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt);
    std::vector<double> calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const std::vector<double> &jetCutsVec);
    std::vector<double> calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt);
    int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double CSVS, const std::vector<double> &jetCutsVec);
    int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt);

    std::vector<double> reqPt30Vec_, reqPt30Eta24Vec_, reqPt50Eta24Vec_, reqPt70Eta24Vec_, reqCSVSVec_, reqdPhiVec_;
    double cutCSVS_;
    int reqndPhi_;

    double cutMET_; // baseline MET cut

    double nJetsSel_, nJetsSelPt30Eta24_, nJetsSelPt50Eta24_, nJetsSelPt70Eta24_, nJetsSelBtagged_;
// Three values ==> 0 : dphi for leading jet  1 : dphi for second leading jet  2 : dphi for third leading jet
    std::vector<double> cutJetMETDPhivals_;

// Values used for MET cuts for all sets of searching regions
// If any of the MET values is -1, it will not be applied
// Lo:  -1                   100          200             -1    ...
// Hi:  -1                   -1           300            400    ...
//   (no cuts at all)     (>=100)   (>=200 && <300)  (>=-1 && <400)
    std::vector<double> cutLoMETvals_, cutHiMETvals_;
    size nSearchRegions;
    std::vector<string> searchRegionKeywords_;

    std::vector<double> scaleToPred_;

// String names for all the search regions
    std::vector<std::string> fullStageStrVec;

    double lowTopCut_, highTopCut_;

    bool passOriKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut);
    bool passNewKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut);

    edm::Service<TFileService> fs;

    TH2D *h2_NTotal;
// nSearchRegions of TH2D where nSearchRegions will be calculated after
// getting the cutXXMHTvals_ and cutXXHTvals_ inputs
    std::vector<TH2D*> h2_NPassedVec;
    std::vector<double> xAxisDivs_, yAxisDivs_;
    std::vector<int> xAxisBins_, yAxisBins_;

    std::vector<double> makeDivBins(const std::vector<double> &divPts, const std::vector<int> &nDivs);
    void bookHistograms();

// "mSUGRA" : for mSUGRA scan
// "SMS" : for Simplified Model
    std::string  susyScanType_;

// If "SMS" type, then use this to specify the topology
// "T1", "T2" and so on
    std::string   susyScanTopology_;

    std::vector<double> getLHEmSUGRApars(const LHEEventProduct& lhep);
    std::vector<double> getLHESMSpars(const LHEEventProduct& lhep);

    bool applyMTcut_;

    std::string puInputFileName_, puInputHistName_;

    std::vector<double> puWeights_;
    double weightTruNPV(int trunpv);
    int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);
};

HadronicTauSignalContamination::HadronicTauSignalContamination(const edm::ParameterSet & iConfig):
   pfJetCutForJetStoring_(iConfig.existsAs<std::string>("pfJetCutForJetStoring") ? iConfig.getParameter<std::string>("pfJetCutForJetStoring") : "", true)
{

  isData = true;

  susyScanType_     = iConfig.getParameter<std::string> ("susyScanType");
  susyScanTopology_ = iConfig.getParameter<std::string> ("susyScanTopology");

  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSource");
  oriJetSrc_ = iConfig.getParameter<edm::InputTag>("oriJetSource");
  bigPtDiffSrc_ = iConfig.getParameter<edm::InputTag>("bigPtDiffSource");
  muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSource");
  muonForJetOverlapSrc_ = iConfig.getParameter<edm::InputTag>("muonForJetOverlapSource");
  eleSrc_ = iConfig.getParameter<edm::InputTag>("eleSource");

  forVetoIsoTrkSrc_ = iConfig.getParameter<edm::InputTag>("forVetoIsoTrkSrc");

  jecLevel_ = iConfig.getUntrackedParameter<std::string>("jecLevel", "ak5PFchsL2L3");

  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");

  vtxSrc_ = iConfig.getParameter<edm::InputTag>("VertexSource");
  nVtxPUcut_ = iConfig.getParameter<unsigned int>("nVtxPUcut");
 
  evtWeightInput_ = iConfig.getParameter<edm::InputTag>("evtWeightInput");

  debug_ = iConfig.getParameter<bool>("debug");

  jetMHTcuts_ = iConfig.getParameter<std::vector<double> >("jetMHTcuts");
  jetHTcuts_ = iConfig.getParameter<std::vector<double> >("jetHTcuts");

  cutMTvals_ = iConfig.getParameter<std::vector<double> >("cutMTvals");

  mhtInputTag_ = iConfig.getParameter<edm::InputTag>("mhtInputTag");
  htInputTag_ = iConfig.getParameter<edm::InputTag>("htInputTag");
  metInputTag_ = iConfig.getParameter<edm::InputTag>("metInputTag");

  templateRootName_ = iConfig.getParameter<std::string>("templateRootName");

  doFillTree_ = iConfig.getParameter<bool>("doFillTree");

  displaceMuon_ = iConfig.getParameter<bool>("displaceMuon");
  useDeposit_ = iConfig.getParameter<bool>("useDepost");

  Ntry_ = iConfig.getParameter<int>("Ntry");

  scaleVIS_ = iConfig.getUntrackedParameter<double>("scaleVIS", 0.0);

  jecUncTypeInput_ = iConfig.getUntrackedParameter<std::string>("jecUncTypeInput", "AK5PF");
  jecUncJetMinPtCut_ = iConfig.getUntrackedParameter<double>("jecUncJetMinPtCut", 10.0); // ref: MET JINST sec 4

  doACC_ = iConfig.getUntrackedParameter<bool>("doACC", false);
  muonKincuts_ = iConfig.getParameter<std::vector<double> >("muonKincuts");
  doZfilter_ = iConfig.getParameter<bool>("doZfilter");

  applyMethod_ = iConfig.getParameter<unsigned int>("applyMethod");

  std::cout<<"applyMethod_ : "<<applyMethod_<<std::endl;
 
  effWeightsInput_ = iConfig.getParameter<edm::InputTag>("effWeightsInput");
  effWeightErr2sInput_ = iConfig.getParameter<edm::InputTag>("effWeightErr2sInput");

  bTagKeyString_ = iConfig.getParameter<std::string>("bTagKeyString");

  doTopTagger_ = iConfig.getUntrackedParameter<bool>("doTopTagger", true);
  maxJetEtaCutForTopTagger_ = iConfig.getUntrackedParameter<double>("maxJetEtaCutForTopTagger", -1);
  minJetPtCutForTopTagger_ = iConfig.getUntrackedParameter<double>("minJetPtCutForTopTagger", 30);

  reqPt30Vec_      = iConfig.getParameter<std::vector<double> >("reqPt30Vec");
  reqPt30Eta24Vec_ = iConfig.getParameter<std::vector<double> >("reqPt30Eta24Vec");
  reqPt50Eta24Vec_ = iConfig.getParameter<std::vector<double> >("reqPt50Eta24Vec");
  reqPt70Eta24Vec_ = iConfig.getParameter<std::vector<double> >("reqPt70Eta24Vec");
  reqCSVSVec_ = iConfig.getParameter<std::vector<double> >("reqCSVSVec");
  reqdPhiVec_ = iConfig.getParameter<std::vector<double> >("reqdPhiVec");
  cutCSVS_    = iConfig.getUntrackedParameter<double>("cutCSVS", 0.679);
  reqndPhi_   = iConfig.getUntrackedParameter<int>("reqndPhi", 3);

  cutMET_ = iConfig.getUntrackedParameter<double>("cutMET", 175);

  nJetsSel_   = iConfig.getUntrackedParameter<double>("nJetsSel", 5);
  nJetsSelPt30Eta24_ = iConfig.getUntrackedParameter<double>("nJetsSelPt30Eta24", 5);
  nJetsSelPt50Eta24_ = iConfig.getUntrackedParameter<double>("nJetsSelPt50Eta24", 4);
  nJetsSelPt70Eta24_ = iConfig.getUntrackedParameter<double>("nJetsSelPt70Eta24", 2);
  nJetsSelBtagged_   = iConfig.getUntrackedParameter<double>("nJetsSelBtagged", 1);

  cutJetMETDPhivals_ = iConfig.getParameter<std::vector<double> >("cutJetMETDPhivals");

  cutLoMETvals_ = iConfig.getParameter<std::vector<double> > ("cutLoMETvals");
  cutHiMETvals_ = iConfig.getParameter<std::vector<double> > ("cutHiMETvals");
  scaleToPred_  = iConfig.getParameter<std::vector<double> > ("scaleToPred");
  searchRegionKeywords_ = iConfig.getParameter<std::vector<string> > ("searchRegionKeywords");

  lowTopCut_ = iConfig.getUntrackedParameter<double>("lowTopCut", 80);
  highTopCut_ = iConfig.getUntrackedParameter<double>("highTopCut", 270);

  if( cutLoMETvals_.size() != cutHiMETvals_.size() || cutLoMETvals_.size() != scaleToPred_.size() ){
     std::cout<<"ERROR ... mismatching between  cutLoMETvals_.size : "<<cutLoMETvals_.size()<<"  cutHiMETvals_.size : "<<cutHiMETvals_.size()<<std::endl;
     std::cout<<"                           OR  scaleToPred_.size  : "<<scaleToPred_.size()<<std::endl;
     return;
  }
  nSearchRegions = cutLoMETvals_.size();
  std::cout<<"INFO ... nSearchRegions : "<<nSearchRegions<<std::endl;

  xAxisDivs_ = iConfig.getParameter<std::vector<double> > ("xAxisDivs");
  yAxisDivs_ = iConfig.getParameter<std::vector<double> > ("yAxisDivs");
  xAxisBins_ = iConfig.getParameter<std::vector<int> > ("xAxisBins");
  yAxisBins_ = iConfig.getParameter<std::vector<int> > ("yAxisBins");

  applyMTcut_ = iConfig.getUntrackedParameter<bool>("applyMTcut");

  puInputFileName_ = iConfig.getUntrackedParameter<std::string> ("puInputFileName", "");
  puInputHistName_ = iConfig.getUntrackedParameter<std::string> ("puInputHistName", "");

  if( puInputFileName_ != "" && puInputHistName_ != "" ){
     initPUinput(puInputFileName_, puInputHistName_);
  }

  if( xAxisDivs_.size() -1 != xAxisBins_.size() || yAxisDivs_.size() -1 != yAxisBins_.size() ){
     std::cout<<"ERROR ... mismatching between  xAxisDivs_.size -1 : "<<xAxisDivs_.size()-1<<"  xAxisBins_.size : "<<xAxisBins_.size()<<std::endl;
     std::cout<<"                           OR  yAxisDivs_.size -1 : "<<yAxisDivs_.size()-1<<"  yAxisBins_.size : "<<yAxisBins_.size()<<std::endl;
     return;
  }

  double lowestMETcut = 1e6;
  for(size is=0; is<nSearchRegions; is++){
     if( lowestMETcut > cutLoMETvals_[is] ) lowestMETcut = cutLoMETvals_[is];
  }
  if( lowestMETcut < cutMET_ ){
     std::cout<<"ERROR ... lowestMETcut for search regions : "<<lowestMETcut<<"  LOWER than baseline MET cut : "<<cutMET_<<std::endl;
     return;
  }

  fullStageStrVec.clear();
  char namesStage[200];
  for(size is=0; is<nSearchRegions; is++){
     sprintf(namesStage, "%s_%3.0fMET%3.0f", searchRegionKeywords_[is].c_str(), cutLoMETvals_[is], cutHiMETvals_[is]);
     TString strTmp(namesStage); strTmp.ReplaceAll(" -1", "inf"); strcpy(namesStage, strTmp.Data());
     fullStageStrVec.push_back(namesStage);
  }

  produces<std::vector<std::string> >("stageNames");
  produces<std::vector<double> >("predictions");

  if( doTopTagger_ ) topTaggerPtr = new topTagger::type3TopTagger();

  dicer = new TRandom3(fSeed);

  if( doACC_ && applyMethod_ != 2 ){ std::cout<<"WARNING ... for acceptance, adviced to choose applyMethod 2! Smearing gen muons in all pt and eta ranges! But we should try to be consistent with the main templates used!"<<std::endl; }

  std::cout<<"opening template root file : "<<templateRootName_.c_str()<<std::endl;
  if( !initTauTemplate() ) std::cout<<"ERROR ... initTauTemplate()!"<<std::endl;

  jetsLVec_TR = new std::vector<TLorentzVector>(); jetsCSVS_TR = new std::vector<double>();

  npv = -1; avg_npv = -1; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;

  setTreeDefaultVars();

  bookHistograms();
}

HadronicTauSignalContamination::~HadronicTauSignalContamination() {

  if( dicer ) delete dicer;

  if( templateFile ) delete templateFile;
}

void HadronicTauSignalContamination::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

   if( !iEvent.isRealData() ) isData = false;

   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();

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

   iEvent.getByLabel(evtWeightInput_, evtWeight_);
}

void HadronicTauSignalContamination::loadJecUnc(const edm::Event& iEvent, const edm::EventSetup& iSetup){

   edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
   iSetup.get<JetCorrectionsRecord>().get(jecUncTypeInput_, JetCorParColl);
   JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
   jecUnc = new JetCorrectionUncertainty(JetCorPar);
}

math::XYZTLorentzVector HadronicTauSignalContamination::calcJecUncCorrMET(const std::vector<pat::Jet> &myjet){
  math::XYZTLorentzVector newJecUncCorrMET;  
  for( size ij=0; ij<myjet.size(); ij++){
     if( myjet[ij].pt() <= jecUncJetMinPtCut_ ) continue;
     jecUnc->setJetEta( myjet[ij].eta() );
     jecUnc->setJetPt( myjet[ij].pt() );
     double unc = jecUnc->getUncertainty(true);
     math::XYZTLorentzVector p4( myjet[ij].px()*unc, myjet[ij].py()*unc, myjet[ij].pz()*unc, myjet[ij].energy()*unc );
     newJecUncCorrMET -= p4;
  }
  return newJecUncCorrMET;
}

std::vector<double> HadronicTauSignalContamination::calcHT(const std::vector<math::XYZTLorentzVector> &newJet){

   std::vector<double> outHTvec;

   double newht = 0, nHTJet =0;
   for( size ij=0; ij<newJet.size(); ij++){
      if( newJet[ij].pt() <= jetHTcuts_[0] ) continue;
      if( fabs(newJet[ij].eta()) >= jetHTcuts_[1] ) continue;

      newht += newJet[ij].pt();

      nHTJet += 1.0;
   }

   outHTvec.push_back(newht); outHTvec.push_back(nHTJet);

   return outHTvec;
}

math::XYZTLorentzVector HadronicTauSignalContamination::calcMHT(const std::vector<math::XYZTLorentzVector> &newJet){
   math::XYZTLorentzVector newMHT;
   for( size ij=0; ij<newJet.size(); ij++){
      if( newJet[ij].pt() <= jetMHTcuts_[0] ) continue;
      if( fabs(newJet[ij].eta()) >= jetMHTcuts_[1] ) continue;

      math::XYZTLorentzVector p4(newJet[ij].px(), newJet[ij].py(), newJet[ij].pz(), newJet[ij].energy());
      newMHT -= p4;
   }
   return newMHT;
}

std::vector<double> HadronicTauSignalContamination::calcJetMHTDPhi(const std::vector<math::XYZTLorentzVector> &newJet, const math::XYZTLorentzVector &newMHT){
   std::vector<double> jetMHTDPhiVec;
// Make their default values to be larger than any possible phi cut, i.e., default to be true
   double dPhi0 = 999., dPhi1 = 999., dPhi2 = 999.;

   if( newJet.size() >=1 ) dPhi0 = fabs(deltaPhi(newJet[0].phi(), newMHT.phi()));
   if( newJet.size() >=2 ) dPhi1 = fabs(deltaPhi(newJet[1].phi(), newMHT.phi()));
   if( newJet.size() >=3 ) dPhi2 = fabs(deltaPhi(newJet[2].phi(), newMHT.phi()));

   jetMHTDPhiVec.push_back(dPhi0); jetMHTDPhiVec.push_back(dPhi1); jetMHTDPhiVec.push_back(dPhi2); 

   return jetMHTDPhiVec;
}

std::vector<double> HadronicTauSignalContamination::calcJetMHTDPhi( const std::vector<pat::Jet> &myjet, const edm::View<reco::MET> &mymht){

   std::vector<double> jetMHTDPhiVec;
// Make their default values to be larger than any possible phi cut, i.e., default to be true
   double dPhi0 = 999., dPhi1 = 999., dPhi2 = 999.;

   if( myjet.size() >=1 ) dPhi0 = fabs(deltaPhi(myjet[0].phi(), mymht[0].phi()));
   if( myjet.size() >=2 ) dPhi1 = fabs(deltaPhi(myjet[1].phi(), mymht[0].phi()));
   if( myjet.size() >=3 ) dPhi2 = fabs(deltaPhi(myjet[2].phi(), mymht[0].phi()));

   jetMHTDPhiVec.push_back(dPhi0); jetMHTDPhiVec.push_back(dPhi1); jetMHTDPhiVec.push_back(dPhi2);

   return jetMHTDPhiVec;
}

void HadronicTauSignalContamination::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  setTreeDefaultVars();

  loadEventInfo(iEvent, iSetup);
  if( doACC_ && isData ){
     std::cout<<"ERROR ... cannot do acceptance on data !"<<std::endl;
     return;
  }

  // read in the jets
  iEvent.getByLabel(jetSrc_, jets);
  iEvent.getByLabel(oriJetSrc_, orijets);

  edm::Handle<std::vector<int> > bigPtDiff;
  iEvent.getByLabel(bigPtDiffSrc_, bigPtDiff);
  
  edm::Handle<std::vector<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  edm::Handle<std::vector<pat::Muon> > muonsForJetOverlap;
  iEvent.getByLabel(muonForJetOverlapSrc_, muonsForJetOverlap);

  edm::Handle<std::vector<pat::Electron> > eles;
  iEvent.getByLabel(eleSrc_, eles);

  iEvent.getByLabel(forVetoIsoTrkSrc_, forVetoIsoTrks); nIsoTrksForVeto = forVetoIsoTrks->size();

  iEvent.getByLabel(effWeightsInput_, effWeights_);
  iEvent.getByLabel(effWeightErr2sInput_, effWeightErr2s_);

  edm::Handle<LHEEventProduct> lheprod;
  iEvent.getByLabel("source", lheprod);

  std::vector<double> LHEpars;
  if( susyScanType_ == "mSUGRA" ){
     LHEpars = getLHEmSUGRApars( *lheprod );
  }else if( susyScanType_ == "SMS" ){
     LHEpars = getLHESMSpars( *lheprod );
  }else{
//     std::cout<<"ERROR ... cannot recoganize susyScanType : "<<susyScanType_.c_str()<<std::endl;
//     return;
     LHEpars.push_back(h2_NTotal->GetXaxis()->GetBinLowEdge(1)); LHEpars.push_back(h2_NTotal->GetYaxis()->GetBinLowEdge(h2_NTotal->GetYaxis()->GetNbins()/2+1));
  }
   
  double xpar = LHEpars[0], ypar = LHEpars[1];

// Assume only one muon picked!
// In case of multiple muons picked, always use the first one 
  subtractMuonPt = -1;
  if( bigPtDiff->size() != muonsForJetOverlap->size() ){
     std::cout<<"ERROR ... bigPtDiff->size() : "<<bigPtDiff->size()<<"  NOT equal to muonsForJetOverlap->size() : "<<muonsForJetOverlap->size()<<std::endl;
     return;
  }
  int pickedMuonIdxForSubtract = -1;
  for(int im1=0; im1<(int)muonsForJetOverlap->size() && !muons->empty(); im1++){
//     if( (*muons)[0].p4() == (*muonsForJetOverlap)[im1].p4() ) pickedMuonIdxForSubtract = im1;
     if( 
            fabs( (*muons)[0].px() - (*muonsForJetOverlap)[im1].px() ) < 1e-6 
         && fabs( (*muons)[0].py() - (*muonsForJetOverlap)[im1].py() ) < 1e-6 
         && fabs( (*muons)[0].pz() - (*muonsForJetOverlap)[im1].pz() ) < 1e-6 
       ){
          pickedMuonIdxForSubtract = im1;
       }
  }
  if( !bigPtDiff->empty() && !muons->empty() ) subtractMuonPt = (*bigPtDiff)[pickedMuonIdxForSubtract];

  if( subtractMuonPt !=-1 ){
     double checkMuonEta = (*muons)[0].eta(), checkMuonPhi = (*muons)[0].phi();
     double checkJetEta = (*jets)[subtractMuonPt].eta(), checkJetPhi = (*jets)[subtractMuonPt].phi();
     if( reco::deltaR(checkJetEta, checkJetPhi, checkMuonEta, checkMuonPhi) > 0.5 ){
        std::cout<<"WARNING ... possible mis-matching between the matched jet and muon?!"<<std::endl;
     }
  }

  if( effWeights_->size() != effWeightErr2s_->size() || effWeights_->size() != muons->size() ){
     std::cout<<"ERROR ... effWeights_->size : "<<effWeights_->size()<<"  effWeightErr2s_->size : "<<effWeightErr2s_->size()<<"  muons->size : "<<muons->size()<<"  NOT equal?!"<<std::endl;
     return;
  }

  if( !isData ){
     loadGenParticles(iEvent);
     loadGenJets(iEvent);
  }

// If multiple WMu decays in an event, always pick the first one since it's random.
// Others will bias, e.g., pick the largest pt and so on.
  std::vector<int> pickedGenMuonIdxVec; std::vector<reco::GenParticle> pickedGenMuonVec; 
  int isFoundWMu =0, isPassMuonKins =0;
  if( doACC_ ){

     std::vector< std::vector<int> > evtLepDecayStatusVec;
     std::vector<int> lepGenIdxVec = lepDecays(evtLepDecayStatusVec);
     size nleps = lepGenIdxVec.size();
  
     for(size il=0; il<nleps; il++){
        const reco::GenParticle& gen = genParticles->at(lepGenIdxVec[il]);
        if( evtLepDecayStatusVec[il][1] ==1 && abs(gen.pdgId()) == 13 ){
           isFoundWMu ++; pickedGenMuonIdxVec.push_back( lepGenIdxVec[il] ); pickedGenMuonVec.push_back(gen);
        }
     }
     if( !isFoundWMu ) return;
     
     if( pickedGenMuonVec.front().pt() > muonKincuts_[0] && fabs(pickedGenMuonVec.front().eta() ) < muonKincuts_[1] ) isPassMuonKins =1;
  }
  if( isPassMuonKins ){/*empty to avoid a compling error*/}

  std::auto_ptr<std::vector<double> > predictionsPtr(new std::vector<double> );
  std::auto_ptr<std::vector<std::string> > stageNamesPtr(new std::vector<std::string> );

  iEvent.getByLabel(vtxSrc_, vertices);

  vtxSize = vertices->size();

  iEvent.getByLabel(mhtInputTag_, mht);
  iEvent.getByLabel(htInputTag_, ht);
  iEvent.getByLabel(metInputTag_, met);

// For jec unc
  loadJecUnc(iEvent, iSetup);
  math::XYZTLorentzVector newJecUncCorrMET = calcJecUncCorrMET( (*jets) );
  math::XYZTLorentzVector oriMET( (*met)[0].px(), (*met)[0].py(), 0, (*met)[0].pt() );
  math::XYZTLorentzVector jecUncCorrPlusMET = oriMET + newJecUncCorrMET;
  math::XYZTLorentzVector jecUncCorrMinusMET = oriMET - newJecUncCorrMET;

  if( debug_ ) cout<<"oriMET : "<<oriMET.pt()<<"  met : "<<(*met)[0].pt()<<"  +jecUnc : "<<jecUncCorrPlusMET.pt()<<"  -jecUnc : "<<jecUncCorrMinusMET.pt()<<endl;

  double mtw = -1, mtwJecUncPlus = -1, mtwJecUncMinus = -1;
//  if( muons->size() == 1 ){
  if( muons->size() >= 1 ){
     mtw = sqrt( 2*( (*met)[0].pt()*(*muons)[0].pt() -( (*met)[0].px()*(*muons)[0].px() + (*met)[0].py()*(*muons)[0].py() ) ) );
     mtwJecUncPlus = sqrt( 2*( jecUncCorrPlusMET.pt()*(*muons)[0].pt() -( jecUncCorrPlusMET.px()*(*muons)[0].px() + jecUncCorrPlusMET.py()*(*muons)[0].py() ) ) );
     mtwJecUncMinus = sqrt( 2*( jecUncCorrMinusMET.pt()*(*muons)[0].pt() -( jecUncCorrMinusMET.px()*(*muons)[0].px() + jecUncCorrMinusMET.py()*(*muons)[0].py() ) ) );
  }
  if( debug_ ) cout<<"orimtw : "<<mtw<<"  +jecUnc : "<<mtwJecUncPlus<<"  -jecUnc : "<<mtwJecUncMinus<<endl<<endl;

  int isPassMTcut = 0, isPassMTcut_jecPlus = 0, isPassMTcut_jecMinus = 0;
  if( ( cutMTvals_[0] == -1 || ( cutMTvals_[0] != -1 && mtw >= cutMTvals_[0] ) ) && ( cutMTvals_[1] == -1 || ( cutMTvals_[1] != -1 && mtw <cutMTvals_[1] ) ) ){
     isPassMTcut = 1;
  }
  if( ( cutMTvals_[0] == -1 || ( cutMTvals_[0] != -1 && mtwJecUncPlus >= cutMTvals_[0] ) ) && ( cutMTvals_[1] == -1 || ( cutMTvals_[1] != -1 && mtwJecUncPlus <cutMTvals_[1] ) ) ){
     isPassMTcut_jecPlus = 1;
  }
  if( ( cutMTvals_[0] == -1 || ( cutMTvals_[0] != -1 && mtwJecUncMinus >= cutMTvals_[0] ) ) && ( cutMTvals_[1] == -1 || ( cutMTvals_[1] != -1 && mtwJecUncMinus <cutMTvals_[1] ) ) ){
     isPassMTcut_jecMinus = 1;
  }

  if( isPassMTcut_jecPlus ){/*empty to avoid a compiling error*/}
  if( isPassMTcut_jecMinus ){/*empty to avoid a compiling error*/}

  vector<double> jetMHTDPhiVec = calcJetMHTDPhi( (*jets), (*mht) );

  double evtTotWeight = (*evtWeight_);

  double effWeight = 1.0, effWeightErr2 = 0.0;
  if( !effWeights_->empty() ){ effWeight = (*effWeights_)[0]; effWeightErr2 = (*effWeightErr2s_)[0]; }

  if( effWeightErr2 ){/*empty to avoid a compiling error*/}

  double evtPUweight = weightTruNPV(tru_npv);
  evtTotWeight *= evtPUweight;

  h2_NTotal->Fill(xpar, ypar, evtTotWeight);

  std::vector<math::XYZTLorentzVector> myIsoMuon, myjet;
  for( size ij=0; ij<jets->size(); ij++){ myjet.push_back( (*jets)[ij].p4() ); }
  for( size im=0; im<muons->size(); im++){ myIsoMuon.push_back( (*muons)[im].p4() ); }

  if( doACC_ ){
// Change the to-be-smeared muon to the generate level one if no reconstructed one found ?!
//     if( muons->size() != 1 ){
     if( muons->empty() ){
        myIsoMuon.clear(); 
        for(size im=0; im<pickedGenMuonVec.size(); im++){ myIsoMuon.push_back( pickedGenMuonVec[im].p4() ); } 
     }
  }

// All the muons & jets are defined for all cases including doACC_ == true
  TLorentzVector pickedMuonLVec;
  pickedMuonLVec.SetPtEtaPhiE( myIsoMuon.at(0).pt(), myIsoMuon.at(0).eta(), myIsoMuon.at(0).phi(), myIsoMuon.at(0).energy() );
 
  for(unsigned int ij=0; ij<jets->size(); ij++){
     const reco::Jet & jet = (*jets)[ij];
     const pat::Jet patjet = (*jets)[ij];

     TLorentzVector pickedJetLVec;
     pickedJetLVec.SetPtEtaPhiE( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
     double btag = patjet.bDiscriminator(bTagKeyString_.c_str());

     if( (int)ij == subtractMuonPt ){
        tauJetReplacedbJetCSVS_TR = patjet.bDiscriminator(bTagKeyString_.c_str());
        continue;
     }

     if( !pfJetCutForJetStoring_(jet) ) continue;
     jetsLVec_TR->push_back( pickedJetLVec );

     jetsCSVS_TR->push_back( btag);
  }

  double muonP = sqrt(myIsoMuon.at(0).px()*myIsoMuon.at(0).px() + myIsoMuon.at(0).py()*myIsoMuon.at(0).py() + myIsoMuon.at(0).pz()*myIsoMuon.at(0).pz());
  if( fabs(muonP - myIsoMuon.at(0).P()) > 1e-10 ) std::cout<<"ERROR ... wrong function used for math::XYZTLorentzVector::M() : "<<myIsoMuon.at(0).P()<<"  muonP : "<<muonP<<endl;

  std::vector<TLorentzVector> oriJetsVec;
  std::vector<double> recoJetsBtagCSVS;

  for(unsigned int ij=0; ij<jetsLVec_TR->size(); ij++){
     double jetEta = jetsLVec_TR->at(ij).Eta(), jetPt = jetsLVec_TR->at(ij).Pt();
     if( maxJetEtaCutForTopTagger_ != -1 && fabs(jetEta) >= maxJetEtaCutForTopTagger_ ) continue;
     if( minJetPtCutForTopTagger_ != -1 && jetPt <= minJetPtCutForTopTagger_ ) continue;
     oriJetsVec.push_back( jetsLVec_TR->at(ij) );
     recoJetsBtagCSVS.push_back( jetsCSVS_TR->at(ij) );
  }

  predictionsPtr->clear(); predictionsPtr->resize(nSearchRegions);
  stageNamesPtr->clear(); stageNamesPtr->resize(nSearchRegions);
  for(size is=0; is<nSearchRegions; is++){
     (*stageNamesPtr)[is] = fullStageStrVec[is];
  }

/* Not working as expected...
// Some pre-selection cuts to avoid sampling in vain
  int pre_cntNJetsPt30      = countJets(oriJetsVec, reqPt30Vec_);
  int pre_cntNJetsPt30Eta24 = countJets(oriJetsVec, reqPt30Eta24Vec_);
  int pre_cntNJetsPt50Eta24 = countJets(oriJetsVec, reqPt50Eta24Vec_);
  int pre_cntNJetsPt70Eta24 = countJets(oriJetsVec, reqPt70Eta24Vec_);
  int pre_cntCSVS           = countCSVS(oriJetsVec, recoJetsBtagCSVS, cutCSVS_, reqCSVSVec_);

  if( pre_cntNJetsPt30 < nJetsSel_ -1 ) return;
  if( pre_cntNJetsPt30Eta24 < nJetsSelPt30Eta24_ -1 ) return;
  if( pre_cntNJetsPt50Eta24 < nJetsSelPt50Eta24_ -1 ) return;
  if( pre_cntNJetsPt70Eta24 < nJetsSelPt70Eta24_ -1 ) return;
  if( tauJetReplacedbJetCSVS_TR <= cutCSVS_ && pre_cntCSVS < nJetsSelBtagged_ ) return;
*/

  if( (applyMTcut_ && isPassMTcut) || !applyMTcut_ ){
     for( int it=0; it<Ntry_; it++){
   
        const JetCorrector* corrector;
// Tau templates are obtained from MC. So no need of Residual correction!
        corrector = JetCorrector::getJetCorrector(jecLevel_, iSetup); 
   
        vector<double> corrFactorVec = calculateCorrFactor( corrector, myIsoMuon, iEvent, iSetup );
   
        math::XYZTLorentzVector smearedMuon;
   
        std::vector<math::XYZTLorentzVector> newJets = predictTAUJets(myjet, myIsoMuon, corrFactorVec, smearedMuon, iEvent, iSetup, 1);
   
        TLorentzVector pickedtauJetLVec;
        pickedtauJetLVec.SetPtEtaPhiE( smearedMuon.pt(), smearedMuon.eta(), smearedMuon.phi(), smearedMuon.energy() );
  
// Thinking this way, the met is the negative sum of pt of all particles.
// So we first calculate what are the pt change of all the particles, then take the negative.
// --> newmet = oldmet + -1*( newmuon - replaced jet )
//                          |<----  deltamet   ----> |
// The replaced jet is just the muon if the deltaPt is less than 30 GeV, otherwise the replaced
// jet can be extracted with index subtractMuonPt.
        TLorentzVector oldmetLVec;
        oldmetLVec.SetPtEtaPhiE( (*met)[0].pt(), (*met)[0].eta(), (*met)[0].phi(), (*met)[0].energy() );
        double deltametpx = 0, deltametpy = 0;
// Now MET does not have type1 correction related to muon
// For the jet containing the muon, it has softPart + muon. The softPart has its JEC propagated to
// MET (pt>10GeV) --> the one we should replace is (the ones included in the type1 MET calculation) :
//       if JEC corrected softPart pt > 10 GeV, softPart(afterJEC) + muon
//  else if .......................pt <= 10 GeV, softPart(afterL1) + muon
// Now we have a tau jet after muon smearing, it should be included in the MET. So after subtracting
// the softPart(afterCorr) + muon, we should add the smeared muon (tau jet) back.
//  ==> oldmet + -1*( smearedMuon - (softPart(afterCorr) + muon) )
        if( subtractMuonPt == -1 /*no softPart*/ ){
           deltametpx = smearedMuon.px()-myIsoMuon.at(0).px(); deltametpy = smearedMuon.py()-myIsoMuon.at(0).py();
        }else{
           int pickedJetIdx = -1;
           for(unsigned int ij=0; ij<orijets->size(); ij++){
               double matchDeltaR = reco::deltaR((*jets)[subtractMuonPt].eta(), (*jets)[subtractMuonPt].phi(), (*orijets)[ij].eta(), (*orijets)[ij].phi());
               if( matchDeltaR < 1e-6 ) pickedJetIdx = (int)ij;
           }
           if( pickedJetIdx == -1 ){
              std::cout<<"ERROR ...  applyTauTemplate::produce ... cannot find matched orijets!"<<std::endl;
              return;
           }
           std::vector<std::string> availableJECLevels = (*orijets)[pickedJetIdx].availableJECLevels();
           const JetCorrector* corrector = 0;
           if( !isData ){
              corrector = JetCorrector::getJetCorrector("ak5PFchsL1FastL2L3",iSetup);
           }else{
              corrector = JetCorrector::getJetCorrector("ak5PFchsL1FastL2L3Residual",iSetup);
           }
           const JetCorrector* correctorL1 = JetCorrector::getJetCorrector("ak5PFchsL1Fastjet",iSetup);
   
           pat::Jet pickedrawJet = (*orijets)[pickedJetIdx];
           double scaleBackToRaw = (*orijets)[pickedJetIdx].jecFactor("Uncorrected")/(*orijets)[pickedJetIdx].jecFactor(availableJECLevels.back());
           pickedrawJet.scaleEnergy(scaleBackToRaw);
           double corrFactor   = corrector->correction(pickedrawJet, iEvent,iSetup);
           double corrFactorL1 = correctorL1->correction(pickedrawJet, iEvent,iSetup);
   
           math::XYZTLorentzVector pickedrawJetP4 = pickedrawJet.p4();
           pickedrawJetP4 -= myIsoMuon.at(0);
   
           math::XYZTLorentzVector toSubtractP4 = myIsoMuon.at(0);
           if( pickedrawJetP4.pt()*corrFactor > 10.0 ){  toSubtractP4 += pickedrawJetP4*corrFactor; }
           else{ toSubtractP4 += pickedrawJetP4*corrFactorL1; }
   
           deltametpx = smearedMuon.px() - toSubtractP4.px(); deltametpy = smearedMuon.py() - toSubtractP4.py();
//        deltametpx = smearedMuon.px()-(*jets)[subtractMuonPt].px(); deltametpy = smearedMuon.py()-(*jets)[subtractMuonPt].py();
        }
        TLorentzVector deltametLVec;
        deltametLVec.SetPxPyPzE( deltametpx, deltametpy, 0, sqrt(deltametpx*deltametpx + deltametpy*deltametpy) );
        TLorentzVector tmpnewmetLVec;
        tmpnewmetLVec = oldmetLVec - deltametLVec;
// Zero the mass of the MET!
        TLorentzVector newmetLVec; newmetLVec.SetPtEtaPhiM( tmpnewmetLVec.Pt(), 0, tmpnewmetLVec.Phi(), 0 );
   
        std::vector<TLorentzVector> tmpLVec = (*jetsLVec_TR);
        std::vector<double> tmpCSVSVec = (*jetsCSVS_TR);
        tmpLVec.push_back(pickedtauJetLVec);
        tmpCSVSVec.push_back(tauJetReplacedbJetCSVS_TR);
   
        std::vector<size_t> allptindexes;
        std::vector<double> alljetsPtVec;
        for(unsigned int jj=0; jj<tmpLVec.size(); jj++){ alljetsPtVec.push_back(tmpLVec[jj].Pt()); }
        stdindexSort::argsort(alljetsPtVec.begin(), alljetsPtVec.end(), std::greater<double>(), allptindexes);
   
        std::vector<TLorentzVector> allJetsLVecSorted;
        std::vector<double> allJetsCSVSSorted;
        for(unsigned int ii=0; ii<allptindexes.size(); ii++){
           unsigned int indexMapped = allptindexes[ii];
           allJetsLVecSorted.push_back( tmpLVec[indexMapped] );
           allJetsCSVSSorted.push_back( tmpCSVSVec[indexMapped] );
        }
   
        int cntNJetsPt30      = countJets(allJetsLVecSorted, reqPt30Vec_);
        int cntNJetsPt30Eta24 = countJets(allJetsLVecSorted, reqPt30Eta24Vec_);
        int cntNJetsPt50Eta24 = countJets(allJetsLVecSorted, reqPt50Eta24Vec_);
        int cntNJetsPt70Eta24 = countJets(allJetsLVecSorted, reqPt70Eta24Vec_);
        int cntCSVS          = countCSVS(allJetsLVecSorted, allJetsCSVSSorted, cutCSVS_, reqCSVSVec_);
        vector<double> dPhiVec = calcDPhi(allJetsLVecSorted, newmetLVec.Phi(), reqndPhi_, reqdPhiVec_);

        bool passNJets = true, passCSVS = true, passdPhi = true;

        if( cntNJetsPt30 < nJetsSel_ ) passNJets = false;
        if( cntNJetsPt30Eta24 < nJetsSelPt30Eta24_ ) passNJets = false;
        if( cntNJetsPt50Eta24 < nJetsSelPt50Eta24_ ) passNJets = false;
        if( cntNJetsPt70Eta24 < nJetsSelPt70Eta24_ ) passNJets = false;
        if( cntCSVS < nJetsSelBtagged_ ) passCSVS = false;
        if( dPhiVec[0] < cutJetMETDPhivals_[0] || dPhiVec[1] < cutJetMETDPhivals_[1] || dPhiVec[2] < cutJetMETDPhivals_[2] ) passdPhi = false;
   
        if( !passNJets ) continue;
        if( !passCSVS ) continue;

        if( doTopTagger_ ){
           std::vector<TLorentzVector> oriJetsVecForTopTagger = oriJetsVec;
           std::vector<double> recoJetsBtagCSVSForTopTagger = recoJetsBtagCSVS;
   
           if(   (maxJetEtaCutForTopTagger_ ==-1 || fabs(pickedtauJetLVec.Eta()) < maxJetEtaCutForTopTagger_ )
              && (minJetPtCutForTopTagger_ ==-1 || pickedtauJetLVec.Pt() > minJetPtCutForTopTagger_ ) ){
              oriJetsVecForTopTagger.push_back(pickedtauJetLVec);
              recoJetsBtagCSVSForTopTagger.push_back(tauJetReplacedbJetCSVS_TR);
           }
   
           std::vector<size_t> ptindexes;
           std::vector<double> tmpJetsPtVec;
           for(unsigned int jj=0; jj<oriJetsVecForTopTagger.size(); jj++){ tmpJetsPtVec.push_back(oriJetsVecForTopTagger[jj].Pt()); }
           stdindexSort::argsort(tmpJetsPtVec.begin(), tmpJetsPtVec.end(), std::greater<double>(), ptindexes);
       
           std::vector<TLorentzVector> oriJetsVecForTopTaggerSorted;
           std::vector<double> recoJetsBtagCSVSForTopTaggerSorted;
           for(unsigned int ii=0; ii<ptindexes.size(); ii++){
              unsigned int indexMapped = ptindexes[ii];
              oriJetsVecForTopTaggerSorted.push_back( oriJetsVecForTopTagger[indexMapped] );
              recoJetsBtagCSVSForTopTaggerSorted.push_back( recoJetsBtagCSVSForTopTagger[indexMapped] );
           }
   
           bool passTopTagger = topTaggerPtr->processEvent(oriJetsVecForTopTaggerSorted, recoJetsBtagCSVSForTopTaggerSorted, newmetLVec);
           if( passTopTagger ){/*empty to avoid a compiling error*/}

           bool passTriplet = true;
           if( topTaggerPtr->bestTopJetIdx == -1 ) passTriplet = false;
           if( !(topTaggerPtr->bestTopJetMass > lowTopCut_ && topTaggerPtr->bestTopJetMass < highTopCut_) ) passTriplet = false;
           if( !topTaggerPtr->remainPassCSVS ) passTriplet = false;
           if( topTaggerPtr->pickedRemainingCombfatJetIdx == -1 && cntNJetsPt30 >=6 ) passTriplet = false;

           double MT2 = topTaggerPtr->MT2, mTb = topTaggerPtr->mTbJet, mTt = topTaggerPtr->mTbestTopJet;

           bool passBaselineOriKin = passOriKinCuts(newmetLVec.Pt(), MT2, mTt, mTb, cutMET_);
           bool passBaselineNewKin = passNewKinCuts(newmetLVec.Pt(), MT2, mTt, mTb, cutMET_);

           if( debug_ ){
              std::cout<<"iTry : "<<it<<"  isPassMTcut : "<<isPassMTcut<<"  passNJets : "<<passNJets<<"  passCSVS : "<<passCSVS<<"  passdPhi : "<<passdPhi<<"  passTriplet : "<<passTriplet<<"  passBaselineOriKin : "<<passBaselineOriKin<<"  passBaselineNewKin : "<<passBaselineNewKin<<std::endl;
              std::cout<<"  ==> MT2 : "<<MT2<<"  mTb : "<<mTb<<"  mTt : "<<mTt<<"  MET : "<<newmetLVec.Pt()<<"  mTb + 0.5*mTt : "<<mTb + 0.5*mTt<<std::endl;
           }

           if( !passTriplet ) continue;

           for(size is=0; is<nSearchRegions; is++){
              if( (cutLoMETvals_[is] == -1 || newmetLVec.Pt() >= cutLoMETvals_[is]) && (cutHiMETvals_[is] == -1 || newmetLVec.Pt()  < cutHiMETvals_[is]) ){
                 double pred = evtTotWeight*effWeight*scaleToPred_[is]/Ntry_;

                 TString stageT = fullStageStrVec[is];
                 if( stageT.Contains("NewKin") && passBaselineNewKin ){
                    if( stageT.Contains("flipDphi") ){
                       if( !passdPhi ){
                          (*predictionsPtr)[is] += pred;
                          h2_NPassedVec[is]->Fill(xpar, ypar, pred);
                       }
                    }else{
                       if( passdPhi ){
                          (*predictionsPtr)[is] += pred;
                          h2_NPassedVec[is]->Fill(xpar, ypar, pred);
                       }
                    }
                 }
                 if( stageT.Contains("OriKin") && passBaselineOriKin ){
                    if( stageT.Contains("flipDphi") ){
                       if( !passdPhi ){
                          (*predictionsPtr)[is] += pred;
                          h2_NPassedVec[is]->Fill(xpar, ypar, pred);
                       }
                    }else{
                       if( passdPhi ){
                          (*predictionsPtr)[is] += pred;
                          h2_NPassedVec[is]->Fill(xpar, ypar, pred);
                       }
                    }
                 }
              }
           }
        }
     }
  }

  if(jecUnc ) delete jecUnc;

  iEvent.put(predictionsPtr, "predictions");
  iEvent.put(stageNamesPtr, "stageNames");
}

void HadronicTauSignalContamination::loadGenParticles(const edm::Event& iEvent){
   iEvent.getByLabel(genParticleSrc_, genParticles);
}

void HadronicTauSignalContamination::loadGenJets(const edm::Event& iEvent){
   iEvent.getByLabel(genJetSrc_, genJets);
}

int HadronicTauSignalContamination::initTauTemplate(){

  char names[200];
  visEnergyTemplateInEtaBinsVec.clear(); rawVisEnergyTemplateInEtaBinsVec.clear();
  visEnergyTemplateInPtBinsVec.clear(); rawVisEnergyTemplateInPtBinsVec.clear();

  templateFile = new TFile(templateRootName_.c_str());
  if( templateFile->GetKey("visible_energy") ){
     h1_visEnergyTemplateTot = (TH1D*) templateFile->Get("visible_energy")->Clone();
  }
  std::cout<<"\n\nWARNING ... direct use rawVisEnergyTemplate to handle L1FastJet corrected templates!"<<std::endl<<std::endl;
//  if( templateFile->GetKey("raw_visible_energy") ){
  if( templateFile->GetKey("fastJet_visible_energy") ){
     h1_rawVisEnergyTemplateTot = (TH1D*) templateFile->Get("fastJet_visible_energy")->Clone();
  }else if( applyMethod_ == 2 ){
//     std::cout<<"ERROR ... no raw_visible_energy found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     std::cout<<"ERROR ... no fastJet_visible_energy found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     return 0;
  }

  for(int ib=0; ib<nEtaBins; ib++){
     sprintf(names, "visible_energy_%s_anyTauPt", etaBinStrs[ib].c_str());
     if( templateFile->GetKey(names ) ){
        visEnergyTemplateInEtaBinsVec.push_back( (TH1D*) templateFile->Get(names)->Clone() );
     }

//     sprintf(names, "raw_visible_energy_%s_anyTauPt", etaBinStrs[ib].c_str());
     sprintf(names, "fastJet_visible_energy_%s_anyTauPt", etaBinStrs[ib].c_str());
     if( templateFile->GetKey(names ) ){
        rawVisEnergyTemplateInEtaBinsVec.push_back( (TH1D*) templateFile->Get(names)->Clone() );
     }
  }
  if( visEnergyTemplateInEtaBinsVec.size() != rawVisEnergyTemplateInEtaBinsVec.size() || ( !visEnergyTemplateInEtaBinsVec.empty() && (int) visEnergyTemplateInEtaBinsVec.size() != nEtaBins ) ){
     std::cout<<"ERROR ... nEtaBins : "<<nEtaBins<<"  visEnergyTemplateInEtaBinsVec.size : "<<visEnergyTemplateInEtaBinsVec.size()<<"  rawVisEnergyTemplateInEtaBinsVec.size : "<<rawVisEnergyTemplateInEtaBinsVec.size()<<std::endl;
     return 0;
  }
  if( rawVisEnergyTemplateInEtaBinsVec.empty() && applyMethod_ == 1 ){
//     std::cout<<"ERROR ... no raw_visible_energy in ETA bins found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     std::cout<<"ERROR ... no fastJet_visible_energy in ETA bins found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     return 0;
  }

  for(int ib=0; ib<nPtBins; ib++){
     sprintf(names, "visible_energy_%s_anyTauEta", ptBinStrs[ib].c_str());
     if( templateFile->GetKey(names ) ){
        visEnergyTemplateInPtBinsVec.push_back( (TH1D*) templateFile->Get(names)->Clone() );
     }

//     sprintf(names, "raw_visible_energy_%s_anyTauEta", ptBinStrs[ib].c_str());
     sprintf(names, "fastJet_visible_energy_%s_anyTauEta", ptBinStrs[ib].c_str());
     if( templateFile->GetKey(names ) ){
        rawVisEnergyTemplateInPtBinsVec.push_back( (TH1D*) templateFile->Get(names)->Clone() );
     }
  }
  if( visEnergyTemplateInPtBinsVec.size() != rawVisEnergyTemplateInPtBinsVec.size() || ( !visEnergyTemplateInPtBinsVec.empty() && (int) visEnergyTemplateInPtBinsVec.size() != nPtBins ) ){
     std::cout<<"ERROR ... nPtBins : "<<nPtBins<<"  visEnergyTemplateInPtBinsVec.size : "<<visEnergyTemplateInPtBinsVec.size()<<"  rawVisEnergyTemplateInPtBinsVec.size : "<<rawVisEnergyTemplateInPtBinsVec.size()<<std::endl;
     return 0;
  }
  if( rawVisEnergyTemplateInPtBinsVec.empty() && applyMethod_ == 0 ){
//     std::cout<<"ERROR ... no raw_visible_energy in PT bins found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     std::cout<<"ERROR ... no fastJet_visible_energy in PT bins found when applyMethod_ : "<<applyMethod_<<" is set!"<<std::endl;
     return 0;
  }
 
  return 1;
}

vector<double> HadronicTauSignalContamination::calculateCorrFactor(const JetCorrector* corrector, const std::vector<math::XYZTLorentzVector> &myIsoMuon, edm::Event& iEvent, const edm::EventSetup& iSetup) {

  vector<double> corrFactorVec;

  double uniNumber[1];
  uniNumber[0]=dicer->Rndm();

  double uniNumberBis[1];

  double muonEta = myIsoMuon.at(0).eta(), muonPt = myIsoMuon.at(0).pt();
  if( applyMethod_ == 0 ){
     int pickedPtBinIdx = -1;
     for(int ib=0; ib<nPtBins; ib++){
        if( muonPt >= ptBinDivs[ib] && ( ptBinDivs[ib+1] == -1 || muonPt < ptBinDivs[ib+1] ) ){
           rawVisEnergyTemplateInPtBinsVec[ib]->GetQuantiles(1,uniNumberBis,uniNumber);
           pickedPtBinIdx = ib;
        }
     }
// If muon pt is out of the range of pt in binned template measurement,
// revert back to the one single template case --> This could happen when
// applyMethod_ == 0 is used for doACC_ == true cases where muon is out of
// acceptance.
     if( pickedPtBinIdx == -1 ){
        h1_rawVisEnergyTemplateTot->GetQuantiles(1,uniNumberBis,uniNumber);
     }
  }
  if( applyMethod_ == 1 ){
     for(int ib=0; ib<nEtaBins; ib++){
        if( fabs(muonEta) >= etaBinDivs[ib] && ( etaBinDivs[ib+1] == -1 || fabs(muonEta) < etaBinDivs[ib+1] ) ){
           rawVisEnergyTemplateInEtaBinsVec[ib]->GetQuantiles(1,uniNumberBis,uniNumber);
        }
     }
  }
  if( applyMethod_ == 2 ){
     h1_rawVisEnergyTemplateTot->GetQuantiles(1,uniNumberBis,uniNumber);
  }
  if( applyMethod_ == 3 ){
     rawVisEnergyTemplateInPtBinsVec.back()->GetQuantiles(1,uniNumberBis,uniNumber);
  }

  double visib_frac=uniNumberBis[0]; visib_frac *= (1.0+scaleVIS_);

  math::XYZTLorentzVector p4;

  p4.SetCoordinates(0,0,0,0);

  if( subtractMuonPt == -1 ){
     p4.SetCoordinates(myIsoMuon.at(0).px()*visib_frac, myIsoMuon.at(0).py()*visib_frac, myIsoMuon.at(0).pz()*visib_frac, myIsoMuon.at(0).P()*visib_frac);
  }else{
//     const pat::Jet fastJetOnlyJet = (*jets)[subtractMuonPt].correctedJet("L1FastJet");
//     math::XYZTLorentzVector overlapJetSubtractedMuonp4 = fastJetOnlyJet.p4();
     int pickedJetIdx = -1;
     for(unsigned int ij=0; ij<orijets->size(); ij++){
         double matchDeltaR = reco::deltaR((*jets)[subtractMuonPt].eta(), (*jets)[subtractMuonPt].phi(), (*orijets)[ij].eta(), (*orijets)[ij].phi());
         if( matchDeltaR < 1e-6 ) pickedJetIdx = (int)ij;
     }
     if( pickedJetIdx == -1 ){
        std::cout<<"ERROR ...  applyTauTemplate::calculateCorrFactor ... cannot find matched orijets!"<<std::endl;
        return corrFactorVec;
     }
     std::vector<std::string> availableJECLevels = (*orijets)[pickedJetIdx].availableJECLevels();
     const JetCorrector* correctorL1 = JetCorrector::getJetCorrector("ak5PFchsL1Fastjet",iSetup);
     pat::Jet pickedJetL1Fast = (*orijets)[pickedJetIdx];
     double scaleBackToRaw = (*orijets)[pickedJetIdx].jecFactor("Uncorrected")/(*orijets)[pickedJetIdx].jecFactor(availableJECLevels.back());
     pickedJetL1Fast.scaleEnergy(scaleBackToRaw);
     double scaleToL1Fast = correctorL1->correction(pickedJetL1Fast, iEvent, iSetup);
     pickedJetL1Fast.scaleEnergy(scaleToL1Fast);

     math::XYZTLorentzVector overlapJetSubtractedMuonp4 = pickedJetL1Fast.p4();
     overlapJetSubtractedMuonp4 -= myIsoMuon.at(0);

     p4.SetCoordinates(overlapJetSubtractedMuonp4.px()+myIsoMuon.at(0).px()*visib_frac, overlapJetSubtractedMuonp4.py()+myIsoMuon.at(0).py()*visib_frac, overlapJetSubtractedMuonp4.pz()+myIsoMuon.at(0).pz()*visib_frac, overlapJetSubtractedMuonp4.P()+myIsoMuon.at(0).P()*visib_frac);
  }

  double scale = corrector->correction(p4);

  double corr_factor= visib_frac*scale;

  corrFactorVec.push_back(visib_frac); corrFactorVec.push_back(scale);

  if( debug_ ){
     std::cout<<"\n\nInfo  run : "<<run<<"  event : "<<event<<"  lumi : "<<ls<<std::endl;
     std::cout<<"Ori  muonPt : "<<myIsoMuon.at(0).pt()<<"  muonEta : "<<myIsoMuon.at(0).eta()<<"  muonPhi : "<<myIsoMuon.at(0).phi()<<std::endl;
     std::cout<<"rndm : "<<uniNumber[0]<<"  visib_frac : "<<visib_frac<<"  scale : "<<scale<<"  corr_factor : "<<corr_factor<<std::endl;
  }

  return corrFactorVec;

}

std::vector<math::XYZTLorentzVector> HadronicTauSignalContamination::predictTAUJets(const std::vector<math::XYZTLorentzVector> &myjet, const std::vector<math::XYZTLorentzVector> &myIsoMuon, const vector<double> &corrFactorVec, math::XYZTLorentzVector &smearedMuon, edm::Event& iEvent, const edm::EventSetup& iSetup, int addSmearedMuonIn =1){

  ////////////////////
  ///// CALCULATE newJet
  ///////////////////

  std::vector<math::XYZTLorentzVector> newJet = myjet;

  math::XYZTLorentzVector mySmearedMuon;
  mySmearedMuon.SetCoordinates(0,0,0,0);

/*
  /// All these systematics are for CALO and not for PF
  if(displaceMuon_) {

    double phi = myIsoMuon.at(0).phi();

    if(myIsoMuon.at(0).charge()>0) phi = myIsoMuon.at(0).phi()-0.03;
    if(myIsoMuon.at(0).charge()<0) phi = myIsoMuon.at(0).phi()+0.03;

    double PxMu = myIsoMuon.at(0).pt()*cos(phi)*corr_factor;
    double PyMu = myIsoMuon.at(0).pt()*sin(phi)*corr_factor;

    mySmearedMuon.SetCoordinates(PxMu,PyMu,myIsoMuon.at(0).pz()*corr_factor,myIsoMuon.at(0).P()*corr_factor);

  }else if(useDeposit_) {

    double theta = myIsoMuon.at(0).theta();
    double phi = myIsoMuon.at(0).phi();

    double PxMu = (myIsoMuon.at(0).px()-(muonDeposit*sin(theta)*cos(phi)))*corr_factor;
    double PyMu = (myIsoMuon.at(0).py()-(muonDeposit*sin(theta)*sin(phi)))*corr_factor;

    mySmearedMuon.SetCoordinates(PxMu,PyMu,myIsoMuon.at(0).pz()*corr_factor,myIsoMuon.at(0).P()*corr_factor);

  }else{

    mySmearedMuon.SetCoordinates(myIsoMuon.at(0).px()*corr_factor, myIsoMuon.at(0).py()*corr_factor, myIsoMuon.at(0).pz()*corr_factor, myIsoMuon.at(0).P()*corr_factor);

  }
*/
  if( subtractMuonPt == -1 ){
     double corr_factor = corrFactorVec[0]*corrFactorVec[1];
     mySmearedMuon.SetCoordinates(myIsoMuon.at(0).px()*corr_factor, myIsoMuon.at(0).py()*corr_factor, myIsoMuon.at(0).pz()*corr_factor, myIsoMuon.at(0).P()*corr_factor);
  }else{
//     const pat::Jet fastJetOnlyJet = (*jets)[subtractMuonPt].correctedJet("L1FastJet");
//     math::XYZTLorentzVector overlapJetSubtractedMuonp4 = fastJetOnlyJet.p4();
     int pickedJetIdx = -1;
     for(unsigned int ij=0; ij<orijets->size(); ij++){
         double matchDeltaR = reco::deltaR((*jets)[subtractMuonPt].eta(), (*jets)[subtractMuonPt].phi(), (*orijets)[ij].eta(), (*orijets)[ij].phi());
         if( matchDeltaR < 1e-6 ) pickedJetIdx = (int)ij;
     }
     if( pickedJetIdx == -1 ){
        std::cout<<"ERROR ...  applyTauTemplate::predictTAUJets ... cannot find matched orijets!"<<std::endl;
        return newJet;
     }
     std::vector<std::string> availableJECLevels = (*orijets)[pickedJetIdx].availableJECLevels();
     const JetCorrector* correctorL1 = JetCorrector::getJetCorrector("ak5PFchsL1Fastjet",iSetup);
     pat::Jet pickedJetL1Fast = (*orijets)[pickedJetIdx];
     double scaleBackToRaw = (*orijets)[pickedJetIdx].jecFactor("Uncorrected")/(*orijets)[pickedJetIdx].jecFactor(availableJECLevels.back());
     pickedJetL1Fast.scaleEnergy(scaleBackToRaw);
     double scaleToL1Fast = correctorL1->correction(pickedJetL1Fast, iEvent, iSetup);
     pickedJetL1Fast.scaleEnergy(scaleToL1Fast);

     math::XYZTLorentzVector overlapJetSubtractedMuonp4 = pickedJetL1Fast.p4();
     overlapJetSubtractedMuonp4 -= myIsoMuon.at(0);

     mySmearedMuon.SetCoordinates( (overlapJetSubtractedMuonp4.px() + myIsoMuon.at(0).px()*corrFactorVec[0])*corrFactorVec[1],
                                   (overlapJetSubtractedMuonp4.py() + myIsoMuon.at(0).py()*corrFactorVec[0])*corrFactorVec[1],
                                   (overlapJetSubtractedMuonp4.pz() + myIsoMuon.at(0).pz()*corrFactorVec[0])*corrFactorVec[1],
                                   (overlapJetSubtractedMuonp4.P()  + myIsoMuon.at(0).P()*corrFactorVec[0] )*corrFactorVec[1]
                                 );
  }

  if( debug_ ){
     std::cout<<"Smr  muonPt : "<<mySmearedMuon.pt()<<"  muonEta : "<<mySmearedMuon.eta()<<"  muonPhi : "<<mySmearedMuon.phi()<<std::endl;
  }

  if( addSmearedMuonIn ){
     if( subtractMuonPt == -1 ){
        newJet.push_back(mySmearedMuon);
     }else{
        newJet[subtractMuonPt] = mySmearedMuon;
     }
  }

  sort(newJet.begin(),newJet.end(),cmpPtLorentz());

  smearedMuon = mySmearedMuon;

  return newJet;

}

std::vector<int> HadronicTauSignalContamination::lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec){

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

bool HadronicTauSignalContamination::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}

int HadronicTauSignalContamination::countJets(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &jetCutsVec){
   return countJets(inputJets, jetCutsVec[0], jetCutsVec[1], jetCutsVec[2], jetCutsVec[3]);
}

int HadronicTauSignalContamination::countJets(const std::vector<TLorentzVector> &inputJets, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){
         cntNJets ++;
      }
   }
   return cntNJets;
}

std::vector<double> HadronicTauSignalContamination::calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const std::vector<double> &jetCutsVec){
   return calcDPhi(inputJets, metphi, nDPhi, jetCutsVec[0], jetCutsVec[1], jetCutsVec[2], jetCutsVec[3]);
}

std::vector<double> HadronicTauSignalContamination::calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   vector<double> outDPhiVec(nDPhi, 999);
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( cntNJets < nDPhi ){
            double perDPhi = fabs(TVector2::Phi_mpi_pi( inputJets[ij].Phi() - metphi ));
            outDPhiVec[cntNJets] = perDPhi;
         }
         cntNJets ++;
      }
   }

   return outDPhiVec;
}

int HadronicTauSignalContamination::countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double CSVS, const std::vector<double> &jetCutsVec){
   return countCSVS(inputJets, inputCSVS, CSVS, jetCutsVec[0], jetCutsVec[1], jetCutsVec[2], jetCutsVec[3]);
}

int HadronicTauSignalContamination::countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( inputCSVS[ij] > CSVS ) cntNJets ++;
      }
   }
   return cntNJets;
}

bool HadronicTauSignalContamination::passOriKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut){
   bool pass = true;

   const double mTcomb = mTb + 0.5*mTt;
   if( mTcomb < 500 ) pass = false;
   if( MT2 < 300 ) pass = false;
   if( met < metCut ) pass = false;

   return pass;
}

bool HadronicTauSignalContamination::passNewKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut){
   bool pass = true;

// Cuts in (mTb, mTt) plane   
   const double mTcomb = mTb + mTt;
   if( mTb < 200 ) pass = false;
   if( mTt < 350 ) pass = false;
   if( mTcomb < 600 ) pass = false;

// Cuts in (met, MT2) plane
   if( MT2 < 200 ) pass = false;
   if( 0.6*met + 0.4*MT2 < 240 ) pass = false;
   if( -0.8*met + MT2 > 200 ) pass = false;

// Cuts in (met, mTb) plane
   if( met + mTb < 800 ) pass = false;

   if( met < metCut ) pass = false;

   return pass;
}

void HadronicTauSignalContamination::bookHistograms() {

   std::vector<double> xDivBinsVec = makeDivBins(xAxisDivs_, xAxisBins_);
   int totxBins = (int)xDivBinsVec.size() - 1;
   std::vector<double> yDivBinsVec = makeDivBins(yAxisDivs_, yAxisBins_);
   int totyBins = (int)yDivBinsVec.size() - 1;

   char hname[100], htit[100];

   sprintf(hname, "NTotal");
   sprintf(htit,  "Total no. of events processed");
   h2_NTotal = fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]);
   h2_NTotal->Sumw2();

   h2_NPassedVec.clear();
   for(size is=0; is<nSearchRegions; is++){
      sprintf(hname, "NPassed_%s", fullStageStrVec[is].c_str());
      sprintf(htit, "Passed : %s", fullStageStrVec[is].c_str());

      h2_NPassedVec.push_back( fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]) );
      h2_NPassedVec.back()->Sumw2();
   }
}

std::vector<double> HadronicTauSignalContamination::makeDivBins(const std::vector<double> &divPts, const std::vector<int> &nDivs){

   std::vector<double> divBins;

   const int nRangesPt = (int)nDivs.size();

   int totBinsPt = 0;
   for(int ib=0; ib<nRangesPt; ib++){ totBinsPt += nDivs[ib]; }
   divBins.clear(); divBins.resize(totBinsPt+1);

   int reBinIdxPt = -1;
   for(int ib=0; ib<nRangesPt; ib++){
      for(int id=0; id<nDivs[ib]; id++){
         double perDiv = (divPts[ib+1]-divPts[ib])/nDivs[ib];
         double divVal = divPts[ib] + perDiv*id;
         reBinIdxPt ++;
         divBins[reBinIdxPt] = divVal;
         if( ib==nRangesPt-1 && id == nDivs[ib] -1 ) divBins[totBinsPt] = divPts[ib+1];
      }
   }

   return divBins;

}

std::vector<double> HadronicTauSignalContamination::getLHEmSUGRApars(const LHEEventProduct& lhep){

   typedef std::vector<std::string>::const_iterator comments_const_iterator;

   comments_const_iterator c_begin = lhep.comments_begin();
   comments_const_iterator c_end = lhep.comments_end();

   double mzero, mhalf, tanb, azero, mu=1.0;
   double signMu;

   std::vector<double> resVec;

   for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
      size_t found = (*cit).find("model");

      if( found != std::string::npos)   {
         size_t foundLength = (*cit).size();
         found = (*cit).find("=");
         std::string smaller = (*cit).substr(found+1,foundLength);
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());

         std::istringstream iss(smaller);
         iss >> mzero;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> mhalf;
         iss.clear();


         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> tanb;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> azero;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> signMu;
         iss.clear();
         mu *= signMu;
      }
   }

   if(debug_ ) std::cout<<" ==> mzero : "<<mzero<<"  mhalf : "<<mhalf<<"  tanb : "<<tanb<<"  azero : "<<azero<<"  mu : "<<mu<<std::endl;
   resVec.push_back(mzero); resVec.push_back(mhalf); resVec.push_back(tanb); resVec.push_back(azero); resVec.push_back(mu);
   return resVec;
}

std::vector<double> HadronicTauSignalContamination::getLHESMSpars(const LHEEventProduct& lhep){

   typedef std::vector<std::string>::const_iterator comments_const_iterator;

   comments_const_iterator c_begin = lhep.comments_begin();
   comments_const_iterator c_end = lhep.comments_end();

   double mGL = -1.0, mLSP = -1.0, xCHI = -1.0;

   std::vector<double> resVec;

   TString susyScanTopologyT(susyScanTopology_);

   for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
      size_t found = (*cit).find("model");

      //model T1_0.0_1025.0_525.0
      if( found != std::string::npos)   {
         size_t foundLength = (*cit).size();

         found = (*cit).find(susyScanTopology_);

         std::string smaller = (*cit).substr(found+1,foundLength);
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());

         std::istringstream iss(smaller);

// See string identifier in https://twiki.cern.ch/twiki/bin/view/CMS/SUSYSimplifiedModelsPAS
         if( susyScanTopologyT.Contains("T1") || susyScanTopologyT.Contains("T3") || ( susyScanTopologyT.Contains("T5") && !susyScanTopologyT.Contains("T5zzlnu") ) ) {
            iss >> xCHI;
            iss.clear();
            found = smaller.find("_");
            smaller = smaller.substr(found+1,smaller.size());
            iss.str(smaller);
         }

         iss >> mGL;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> mLSP;
         iss.clear();
      }
   }

   if(debug_ ) std::cout<<" ==> gluino : "<<mGL<<"  LSP : "<<mLSP<<"  xFrac : "<<xCHI <<std::endl;
   resVec.push_back(mGL); resVec.push_back(mLSP); resVec.push_back(xCHI);
   return resVec;
}

void HadronicTauSignalContamination::setTreeDefaultVars(){
// In this tree case, don't need the vector for selSteps. We only need the vector for nTry
   
    jetsLVec_TR->clear(); jetsCSVS_TR->clear();
      
    tauJetReplacedbJetCSVS_TR = -99;
       
    npv = -1; avg_npv = -1; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;
}

// Handle my default value of trunpv==-1, this is the case for data.
double HadronicTauSignalContamination::weightTruNPV(int trunpv){

   double w = 1.;
   if (trunpv < static_cast<int> (puWeights_.size()) && trunpv >=0) {
      w = puWeights_.at(trunpv);
   } else if ( !puWeights_.empty() && trunpv >= static_cast<int> (puWeights_.size()) ) {
      std::cerr << "WARNING ... number of PU vertices = " << trunpv
            << " out of histogram binning." << std::endl;
   }

   return w;
}

int HadronicTauSignalContamination::initPUinput(const std::string &puDatafileName, const std::string &puDatahistName){

   puWeights_.clear();

   TFile *puDatafile = new TFile(puDatafileName.c_str());
   TH1D *data_npu_estimated = (TH1D*) puDatafile->Get(puDatahistName.c_str());

   double Summer12_S10[60] = {
      2.560E-06,
      5.239E-06,
      1.420E-05,
      5.005E-05,
      1.001E-04,
      2.705E-04,
      1.999E-03,
      6.097E-03,
      1.046E-02,
      1.383E-02,
      1.685E-02,
      2.055E-02,
      2.572E-02,
      3.262E-02,
      4.121E-02,
      4.977E-02,
      5.539E-02,
      5.725E-02,
      5.607E-02,
      5.312E-02,
      5.008E-02,
      4.763E-02,
      4.558E-02,
      4.363E-02,
      4.159E-02,
      3.933E-02,
      3.681E-02,
      3.406E-02,
      3.116E-02,
      2.818E-02,
      2.519E-02,
      2.226E-02,
      1.946E-02,
      1.682E-02,
      1.437E-02,
      1.215E-02,
      1.016E-02,
      8.400E-03,
      6.873E-03,
      5.564E-03,
      4.457E-03,
      3.533E-03,
      2.772E-03,
      2.154E-03,
      1.656E-03,
      1.261E-03,
      9.513E-04,
      7.107E-04,
      5.259E-04,
      3.856E-04,
      2.801E-04,
      2.017E-04,
      1.439E-04,
      1.017E-04,
      7.126E-05,
      4.948E-05,
      3.405E-05,
      2.322E-05,
      1.570E-05,
      5.005E-06
   };

   puWeights_.resize(60);
   double s = 0.0;
   for (int npu = 0; npu < 60; ++npu) {
      double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));
      puWeights_[npu] = npu_estimated / Summer12_S10[npu];
      s += npu_estimated;
   }

   // normalize weights such that the total sum of weights over the whole sample is 1.0, i.e., sum_i  puWeights_[i] * Summer2012[i] should be 1.0 (!)
   for (int npu = 0; npu < 60; ++npu) {
      puWeights_[npu] /= s;
   }

   return 1;
}

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE(HadronicTauSignalContamination);
