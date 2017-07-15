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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CommonTools/Utils/interface/PtComparator.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "TLorentzVector.h"

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/BTauReco/interface/ShallowTagInfo.h"
#include "DeepNTuples/JetAnalysis/interface/JetHelper.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_bTagVars.h"
#include "DataFormats/BTauReco/interface/CandIPTagInfo.h"
#include "DataFormats/BTauReco/interface/CandSoftLeptonTagInfo.h"
#include "DataFormats/BTauReco/interface/BoostedDoubleSVTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/IPTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_bTagVars.h"

#include "DeepNTuples/DeepNtuplizer/interface/ntuple_content.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_SV.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_JetInfo.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_pfCands.h"
#include "DeepNTuples/DeepNtuplizer/interface/ntuple_bTagVars.h"

class prodJets : public edm::EDFilter 
{
 public:

  explicit prodJets(const edm::ParameterSet & iConfig);
  ~prodJets();


 private:

  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag jetSrc_, jetOtherSrc_;
  // All have to be pat::Jet, otherwise cannot get b-tagging information!
  edm::Handle<std::vector<pat::Jet> > jets, otherjets; 
  std::string bTagKeyString_;

  std::string jetPBJetTags_;
  std::string jetPNegBJetTags_;
  std::string jetPPosBJetTags_;

  std::string jetBPBJetTags_;
  std::string jetBPNegBJetTags_;
  std::string jetBPPosBJetTags_;

  std::string deepCSVBJetTags_;
  std::string deepCSVNegBJetTags_;
  std::string deepCSVPosBJetTags_;

  std::string combinedSVBJetTags_;
  std::string combinedSVNegBJetTags_;
  std::string combinedSVPosBJetTags_;

  std::string combinedIVFSVBJetTags_;
  std::string combinedIVFSVPosBJetTags_;
  std::string combinedIVFSVNegBJetTags_;

  std::string simpleSVHighEffBJetTags_;
  std::string simpleSVNegHighEffBJetTags_;
  std::string simpleSVHighPurBJetTags_;
  std::string simpleSVNegHighPurBJetTags_;

  std::string softPFMuonBJetTags_;
  std::string softPFMuonNegBJetTags_;
  std::string softPFMuonPosBJetTags_;

  std::string softPFElectronBJetTags_;
  std::string softPFElectronNegBJetTags_;
  std::string softPFElectronPosBJetTags_;

  std::string doubleSVBJetTags_;

  std::string cMVABJetTags_;
  std::string cMVAv2BJetTags_;
  std::string cMVAv2NegBJetTags_;
  std::string cMVAv2PosBJetTags_;

  std::string   CvsBCJetTags_;
  std::string   CvsBNegCJetTags_;
  std::string   CvsBPosCJetTags_;
  std::string   CvsLCJetTags_;
  std::string   CvsLNegCJetTags_;
  std::string   CvsLPosCJetTags_;

  edm::InputTag vtxSrc_;
  edm::InputTag metSrc_;
  bool isPatJet;
  bool debug_;

  bool isData_;

  double jetPtCut_miniAOD_, genMatch_dR_;
  double relPt_for_xCheck_, dR_for_xCheck_;

  edm::EDGetTokenT<std::vector<pat::Jet> >JetTok_;
  edm::EDGetTokenT<std::vector<pat::Jet> >OtherJetsTok_;
  edm::EDGetTokenT<std::vector<int> > W_EmuVec_Tok_;
  edm::EDGetTokenT<std::vector<int> >W_TauVec_Tok_;
  edm::EDGetTokenT<std::vector<int> >W_Tau_EmuVec_Tok_;
  edm::EDGetTokenT<std::vector<int> >W_Tau_ProngsVec_Tok_;
  edm::EDGetTokenT<std::vector<int> >W_Tau_NuVec_Tok_;
  edm::EDGetTokenT<std::vector<TLorentzVector> >GenDecayLVec_Tok_;
  edm::EDGetTokenT<std::vector<int> >GenDecayMomRefVec_Tok_;
  edm::EDGetTokenT<std::vector<TLorentzVector> >EleLVec_Tok_;
  edm::EDGetTokenT<std::vector<TLorentzVector> >MuLVec_Tok_;
  edm::EDGetTokenT<std::vector<TLorentzVector> >TrksForIsoVetolVec_Tok_;
  edm::EDGetTokenT<std::vector<TLorentzVector> >LooseIsoTrksVec_Tok_;
  edm::EDGetTokenT< std::vector<reco::Vertex> >VtxTok_;
  edm::EDGetTokenT<std::vector<pat::Jet>> PuppiJetsSrc_Tok_;
  edm::EDGetTokenT<std::vector<pat::Jet>> PuppiSubJetsSrc_Tok_;
  edm::EDGetTokenT<std::vector<pat::Jet>> Ak8Jets_Tok_;
  edm::EDGetTokenT<std::vector<pat::Jet>> Ak8SubJets_Tok_;

  edm::InputTag W_emuVec_Src_, W_tauVec_Src_, W_tau_emuVec_Src_, W_tau_prongsVec_Src_, W_tau_nuVec_Src_;
  edm::Handle<std::vector<int> > W_emuVec_, W_tauVec_, W_tau_emuVec_, W_tau_prongsVec_, W_tau_nuVec_;

  edm::InputTag genDecayLVec_Src_;
  edm::Handle<std::vector<TLorentzVector> > genDecayLVec_;

  edm::InputTag genDecayMomRefVec_Src_;
  edm::Handle<std::vector<int> > genDecayMomRefVec_;

  edm::InputTag eleLVec_Src_, muLVec_Src_;
  edm::Handle<std::vector<TLorentzVector> > eleLVec_, muLVec_;

  edm::InputTag trksForIsoVetoLVec_Src_, looseisoTrksLVec_Src_;
  edm::Handle<std::vector<TLorentzVector> > trksForIsoVetoLVec_, looseisoTrksLVec_;
  double deltaRcon_;

  //PUPPI sources
  edm::InputTag puppiJetsSrc_, puppiSubJetsSrc_;
  edm::Handle<std::vector<pat::Jet> > puppiJets;
  edm::Handle<std::vector<pat::Jet> > puppiSubJets; 
 
  //AK8 Jets
  edm::InputTag ak8JetsSrc_, ak8SubJetsSrc_;
  edm::Handle<std::vector<pat::Jet> > ak8Jets;
  edm::Handle<std::vector<pat::Jet> > ak8SubJets;

  std::string jetType_;
  std::string qgTaggerKey_;
  std::string NjettinessAK8Puppi_label_;
  std::string ak8PFJetsPuppi_label_;

  std::string tagInfoName_;
  std::string ipTagInfos_;
  std::string svTagInfos_;

  float jetNTracks_;              // tracks associated to jet
  float trackJetPt_;
};


prodJets::prodJets(const edm::ParameterSet & iConfig) 
{
  isData_ = true;

  jetSrc_      = iConfig.getParameter<edm::InputTag>("jetSrc");
  jetOtherSrc_ = iConfig.getParameter<edm::InputTag>("jetOtherSrc");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");
  //metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");
  bTagKeyString_ = iConfig.getParameter<std::string>("bTagKeyString");

  jetPBJetTags_        = iConfig.getParameter<std::string>("jetPBJetTags");
  jetPNegBJetTags_     = iConfig.getParameter<std::string>("jetPNegBJetTags");
  jetPPosBJetTags_     = iConfig.getParameter<std::string>("jetPPosBJetTags");

  jetBPBJetTags_        = iConfig.getParameter<std::string>("jetBPBJetTags");
  jetBPNegBJetTags_     = iConfig.getParameter<std::string>("jetBPNegBJetTags");
  jetBPPosBJetTags_     = iConfig.getParameter<std::string>("jetBPPosBJetTags");

  deepCSVBJetTags_    = iConfig.getParameter<std::string>("deepCSVBJetTags");
  deepCSVNegBJetTags_ = iConfig.getParameter<std::string>("deepCSVNegBJetTags");
  deepCSVPosBJetTags_ = iConfig.getParameter<std::string>("deepCSVPosBJetTags");

  combinedSVBJetTags_     = iConfig.getParameter<std::string>("combinedSVBJetTags");
  combinedSVNegBJetTags_  = iConfig.getParameter<std::string>("combinedSVNegBJetTags");
  combinedSVPosBJetTags_  = iConfig.getParameter<std::string>("combinedSVPosBJetTags");

  combinedIVFSVBJetTags_      = iConfig.getParameter<std::string>("combinedIVFSVBJetTags");
  combinedIVFSVPosBJetTags_   = iConfig.getParameter<std::string>("combinedIVFSVPosBJetTags");
  combinedIVFSVNegBJetTags_   = iConfig.getParameter<std::string>("combinedIVFSVNegBJetTags");

  simpleSVHighEffBJetTags_      = iConfig.getParameter<std::string>("simpleSVHighEffBJetTags");
  simpleSVNegHighEffBJetTags_   = iConfig.getParameter<std::string>("simpleSVNegHighEffBJetTags");
  simpleSVHighPurBJetTags_      = iConfig.getParameter<std::string>("simpleSVHighPurBJetTags");
  simpleSVNegHighPurBJetTags_   = iConfig.getParameter<std::string>("simpleSVNegHighPurBJetTags");

	deepCSVBJetTags_    = iConfig.getParameter<std::string>("deepCSVBJetTags");
	deepCSVNegBJetTags_ = iConfig.getParameter<std::string>("deepCSVNegBJetTags");
	deepCSVPosBJetTags_ = iConfig.getParameter<std::string>("deepCSVPosBJetTags");

  combinedIVFSVBJetTags_      = iConfig.getParameter<std::string>("combinedIVFSVBJetTags");
  combinedIVFSVPosBJetTags_   = iConfig.getParameter<std::string>("combinedIVFSVPosBJetTags");
  combinedIVFSVNegBJetTags_   = iConfig.getParameter<std::string>("combinedIVFSVNegBJetTags");

  softPFMuonBJetTags_       = iConfig.getParameter<std::string>("softPFMuonBJetTags");
  softPFMuonNegBJetTags_    = iConfig.getParameter<std::string>("softPFMuonNegBJetTags");
  softPFMuonPosBJetTags_    = iConfig.getParameter<std::string>("softPFMuonPosBJetTags");

  softPFElectronBJetTags_       = iConfig.getParameter<std::string>("softPFElectronBJetTags");
  softPFElectronNegBJetTags_    = iConfig.getParameter<std::string>("softPFElectronNegBJetTags");
  softPFElectronPosBJetTags_    = iConfig.getParameter<std::string>("softPFElectronPosBJetTags");

  doubleSVBJetTags_ = iConfig.getParameter<std::string>("doubleSVBJetTags");

  cMVABJetTags_ = iConfig.getParameter<std::string>("cMVABJetTags");
  cMVAv2BJetTags_ = iConfig.getParameter<std::string>("cMVAv2BJetTags");
  cMVAv2NegBJetTags_ = iConfig.getParameter<std::string>("cMVAv2NegBJetTags");
  cMVAv2PosBJetTags_ = iConfig.getParameter<std::string>("cMVAv2PosBJetTags");

  CvsBCJetTags_             = iConfig.getParameter<std::string>("CvsBCJetTags");
  CvsBNegCJetTags_             = iConfig.getParameter<std::string>("CvsBNegCJetTags");
  CvsBPosCJetTags_             = iConfig.getParameter<std::string>("CvsBPosCJetTags");
  CvsLCJetTags_             = iConfig.getParameter<std::string>("CvsLCJetTags");
  CvsLNegCJetTags_             = iConfig.getParameter<std::string>("CvsLNegCJetTags");
  CvsLPosCJetTags_             = iConfig.getParameter<std::string>("CvsLPosCJetTags");

  debug_       = iConfig.getParameter<bool>("debug");

  jetPtCut_miniAOD_ = iConfig.getUntrackedParameter<double>("jetPtCut_miniAOD", 10);
  genMatch_dR_ = iConfig.getUntrackedParameter<double>("genMatch_dR", 1.0);
  dR_for_xCheck_ = iConfig.getUntrackedParameter<double>("dR_for_xCheck", 0.2);
  relPt_for_xCheck_ = iConfig.getUntrackedParameter<double>("relPt_for_xCheck", 1e-2);

  W_emuVec_Src_ = iConfig.getParameter<edm::InputTag>("W_emuVec");
  W_tauVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tauVec");
  W_tau_emuVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tau_emuVec");
  W_tau_prongsVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tau_prongsVec");
  W_tau_nuVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tau_nuVec");

  genDecayLVec_Src_ = iConfig.getParameter<edm::InputTag>("genDecayLVec");

  genDecayMomRefVec_Src_ = iConfig.getParameter<edm::InputTag>("genDecayMomRefVec");

  eleLVec_Src_ = iConfig.getParameter<edm::InputTag>("eleLVec");
  muLVec_Src_ = iConfig.getParameter<edm::InputTag>("muLVec");
  
  trksForIsoVetoLVec_Src_ = iConfig.getParameter<edm::InputTag>("trksForIsoVetoLVec");
  looseisoTrksLVec_Src_ = iConfig.getParameter<edm::InputTag>("looseisoTrksLVec");

  deltaRcon_ = iConfig.getUntrackedParameter<double>("deltaRcon", 0.01);

  jetType_ = iConfig.getParameter<std::string>("jetType");

  qgTaggerKey_ = iConfig.getParameter<std::string>("qgTaggerKey");
  
  //Puppi source
  puppiJetsSrc_ = iConfig.getParameter<edm::InputTag>("puppiJetsSrc");
  puppiSubJetsSrc_ = iConfig.getParameter<edm::InputTag>("puppiSubJetsSrc");
  //ak8 jets
  ak8JetsSrc_ = iConfig.getParameter<edm::InputTag>("ak8JetsSrc");
  ak8SubJetsSrc_ = iConfig.getParameter<edm::InputTag>("ak8SubJetsSrc");
  
  NjettinessAK8Puppi_label_ = iConfig.getParameter<std::string>("NjettinessAK8Puppi_label");
  ak8PFJetsPuppi_label_ = iConfig.getParameter<std::string>("ak8PFJetsPuppi_label");

  tagInfoName_= iConfig.getParameter<std::string>("tagInfoName");

  ipTagInfos_              = iConfig.getParameter<std::string>("ipTagInfos");
  svTagInfos_              = iConfig.getParameter<std::string>("svTagInfos");


  JetTok_ = consumes<std::vector<pat::Jet> >(jetSrc_);
  OtherJetsTok_ = consumes<std::vector<pat::Jet> >(jetOtherSrc_);
  W_EmuVec_Tok_=consumes<std::vector<int> >(W_emuVec_Src_);
  W_TauVec_Tok_=consumes<std::vector<int> >(W_tauVec_Src_);
  W_Tau_EmuVec_Tok_=consumes<std::vector<int> >(W_tau_emuVec_Src_);
  W_Tau_ProngsVec_Tok_ = consumes<std::vector<int> >(W_tau_prongsVec_Src_);
  W_Tau_NuVec_Tok_ = consumes<std::vector<int> >(W_tau_nuVec_Src_);
  GenDecayLVec_Tok_=consumes<std::vector<TLorentzVector> >(genDecayLVec_Src_);
  GenDecayMomRefVec_Tok_=consumes<std::vector<int> >(genDecayMomRefVec_Src_);
  EleLVec_Tok_=consumes<std::vector<TLorentzVector> >(eleLVec_Src_);
  MuLVec_Tok_=consumes<std::vector<TLorentzVector> >(muLVec_Src_);
  TrksForIsoVetolVec_Tok_=consumes<std::vector<TLorentzVector> >(trksForIsoVetoLVec_Src_);
  LooseIsoTrksVec_Tok_=consumes<std::vector<TLorentzVector> >(looseisoTrksLVec_Src_);
  VtxTok_=consumes< std::vector<reco::Vertex> >(vtxSrc_);
  PuppiJetsSrc_Tok_ = consumes<std::vector<pat::Jet>>(puppiJetsSrc_);
  PuppiSubJetsSrc_Tok_ = consumes<std::vector<pat::Jet>>(puppiSubJetsSrc_);
  Ak8Jets_Tok_ = consumes<std::vector<pat::Jet>>(ak8JetsSrc_);
  Ak8SubJets_Tok_ = consumes<std::vector<pat::Jet>>(ak8SubJetsSrc_);

  //produces<std::vector<pat::Jet> >("");
  produces<std::vector<TLorentzVector> >("jetsLVec");
  produces<std::vector<int> >("recoJetsFlavor");
  produces<std::vector<double> >("recoJetsBtag");

  produces<std::vector<double> >("JetProba");
  produces<std::vector<double> >("JetProbaN");
  produces<std::vector<double> >("JetProbaP");
  produces<std::vector<double> >("JetBprob");
  produces<std::vector<double> >("JetBprobP");
  produces<std::vector<double> >("JetBprobN");

  produces<std::vector<double> >("CombinedSvtx");
  produces<std::vector<double> >("CombinedSvtxN");
  produces<std::vector<double> >("CombinedSvtxP");

  produces<std::vector<double> >("recoJetsCharge");
  produces<std::vector<double> >("recoJetsJecUnc");
  produces<std::vector<double> >("recoJetsJecScaleRawToFull");
  produces<int>("nJets");
  produces<std::vector<double> >("qgLikelihood");
  produces<std::vector<double> >("qgPtD");
  produces<std::vector<double> >("qgAxis2");
  produces<std::vector<int> >("qgMult");
/*
  produces<std::vector<double> >("qgPtDrLog");
  produces<std::vector<double> >("qgAxis1");
  produces<std::vector<int> >("qgcMult");
  produces<std::vector<int> >("qgnMult");
*/
  produces<std::vector<double> >("DeepCSVb");
  produces<std::vector<double> >("DeepCSVc");
  produces<std::vector<double> >("DeepCSVl");
  produces<std::vector<double> >("DeepCSVbb");
  produces<std::vector<double> >("DeepCSVcc");

  produces<std::vector<double> >("DeepCSVbN");
  produces<std::vector<double> >("DeepCSVcN");
  produces<std::vector<double> >("DeepCSVlN");
  produces<std::vector<double> >("DeepCSVbbN");
  produces<std::vector<double> >("DeepCSVccN");

  produces<std::vector<double> >("DeepCSVbP");
  produces<std::vector<double> >("DeepCSVcP");
  produces<std::vector<double> >("DeepCSVlP");
  produces<std::vector<double> >("DeepCSVbbP");
  produces<std::vector<double> >("DeepCSVccP");

  produces<std::vector<float> >("CombinedIVF");
  produces<std::vector<float> >("CombinedIVFP");
  produces<std::vector<float> >("CombinedIVFN");

  produces<std::vector<double> >("Svtx");
  produces<std::vector<double> >("SvtxN");
  produces<std::vector<double> >("SvtxHP");
  produces<std::vector<double> >("SvtxNHP");

  produces<std::vector<double> >("SoftM");
  produces<std::vector<double> >("SoftMN");
  produces<std::vector<double> >("SoftMP");

  produces<std::vector<double> >("SoftE");
  produces<std::vector<double> >("SoftEN");
  produces<std::vector<double> >("SoftEP");

  produces<std::vector<double> >("DoubleSV");

  produces<std::vector<double> >("cMVA");
  produces<std::vector<double> >("cMVAv2");
  produces<std::vector<double> >("cMVAv2Neg");
  produces<std::vector<double> >("cMVAv2Pos");

  produces<std::vector<double> >("CvsB");
  produces<std::vector<double> >("CvsBNeg");
  produces<std::vector<double> >("CvsBPos");
  produces<std::vector<double> >("CvsL");
  produces<std::vector<double> >("CvsLNeg");
  produces<std::vector<double> >("CvsLPos");

 //produces<std::vector<int>>("nTracks");
 //produces<std::vector<int>>("nSVs");

  //produce variables needed for Lost Lepton study, added by hua.wei@cern.ch
  produces<std::vector<double> >("recoJetschargedHadronEnergyFraction");
  produces<std::vector<double> >("recoJetsneutralEnergyFraction");
  produces<std::vector<double> >("recoJetschargedEmEnergyFraction");
  produces<std::vector<double> >("recoJetsneutralEmEnergyFraction");
  produces<std::vector<double> >("PhotonEnergyFraction");
  produces<std::vector<double> >("ElectronEnergyFraction");

  produces<std::vector<double> >("recoJetsmuonEnergyFraction");

 produces<std::vector<double> >("ChargedHadronMultiplicity");
 produces<std::vector<double> >("NeutralHadronMultiplicity");
 produces<std::vector<double> >("PhotonMultiplicity");
 produces<std::vector<double> >("ElectronMultiplicity");
 produces<std::vector<double> >("MuonMultiplicity");

  produces<std::vector<double> >("TagVarCSVtrackSumJetEtRatio");

  produces<std::vector<int> >("muMatchedJetIdx");
  produces<std::vector<int> >("eleMatchedJetIdx");

  produces<std::vector<int> >("trksForIsoVetoMatchedJetIdx");
  produces<std::vector<int> >("looseisoTrksMatchedJetIdx");

  //PUPPI
  produces<std::vector<TLorentzVector> >("puppiJetsLVec");
  produces<std::vector<TLorentzVector> >("puppiSubJetsLVec");
  produces<std::vector<double> >("puppisoftDropMass");
  produces<std::vector<double> >("puppitau1");
  produces<std::vector<double> >("puppitau2");
  produces<std::vector<double> >("puppitau3");
  produces<std::vector<double> >("puppiSubJetsBdisc");

  //ak8
  produces<std::vector<TLorentzVector> >("ak8JetsLVec");
  produces<std::vector<TLorentzVector> >("ak8SubJetsLVec");
  produces<std::vector<double> >("softDropMass");
  produces<std::vector<double> >("tau1");
  produces<std::vector<double> >("tau2");
  produces<std::vector<double> >("tau3");
  produces<std::vector<double> >("ak8SubJetsBdisc");


}


prodJets::~prodJets() 
{
}


bool prodJets::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

 

  if( !iEvent.isRealData() ) isData_ = false;

  iEvent.getByToken(JetTok_, jets);

  //get the JEC uncertainties
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get(jetType_, JetCorParColl);
  JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
  std::auto_ptr<JetCorrectionUncertainty> jecUnc( new JetCorrectionUncertainty(JetCorPar) );

  if( !isData_ )
  {
    iEvent.getByToken(OtherJetsTok_, otherjets);
    iEvent.getByToken(W_EmuVec_Tok_, W_emuVec_);
    iEvent.getByToken(W_TauVec_Tok_, W_tauVec_);
    iEvent.getByToken(W_Tau_EmuVec_Tok_, W_tau_emuVec_);
    iEvent.getByToken(W_Tau_ProngsVec_Tok_, W_tau_prongsVec_);
    iEvent.getByToken(W_Tau_NuVec_Tok_, W_tau_nuVec_);

    iEvent.getByToken(GenDecayLVec_Tok_, genDecayLVec_);
    iEvent.getByToken(GenDecayMomRefVec_Tok_, genDecayMomRefVec_);
  }

  iEvent.getByToken(EleLVec_Tok_, eleLVec_);
  iEvent.getByToken(MuLVec_Tok_, muLVec_);

  iEvent.getByToken(TrksForIsoVetolVec_Tok_, trksForIsoVetoLVec_);
  iEvent.getByToken(LooseIsoTrksVec_Tok_,looseisoTrksLVec_);

  //read in the objects
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);
  //reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());
  //edm::Handle<edm::View<reco::MET> > met;
  //iEvent.getByLabel(metSrc_, met);

  std::vector<pat::Jet> extJets = (*jets);
  //PUPPI
  iEvent.getByToken(PuppiJetsSrc_Tok_, puppiJets);
  iEvent.getByToken(PuppiSubJetsSrc_Tok_, puppiSubJets);
  //ak8
  iEvent.getByToken(Ak8Jets_Tok_, ak8Jets);
  iEvent.getByToken(Ak8SubJets_Tok_, ak8SubJets);

  //check which ones to keep
  //std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
  std::auto_ptr<std::vector<TLorentzVector> > jetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<int> > recoJetsFlavor(new std::vector<int>());
  std::auto_ptr<std::vector<double> > recoJetsBtag(new std::vector<double>());

  std::auto_ptr<std::vector<double> > JetProba(new std::vector<double>());
  std::auto_ptr<std::vector<double> > JetProbaN(new std::vector<double>());
  std::auto_ptr<std::vector<double> > JetProbaP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > JetBprob(new std::vector<double>());
  std::auto_ptr<std::vector<double> > JetBprobP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > JetBprobN(new std::vector<double>());

  std::auto_ptr<std::vector<double> >CombinedSvtx(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CombinedSvtxN(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CombinedSvtxP(new std::vector<double>());

  std::auto_ptr<std::vector<double> > recoJetsCharge(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsJecUnc(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsJecScaleRawToFull(new std::vector<double>());

  std::auto_ptr<std::vector<double> > qgLikelihood(new std::vector<double>());
  std::auto_ptr<std::vector<double> > qgPtD(new std::vector<double>());
  std::auto_ptr<std::vector<double> > qgAxis2(new std::vector<double>());
  std::auto_ptr<std::vector<int> > qgMult(new std::vector<int>());
/*
  std::auto_ptr<std::vector<double> > qgPtDrLog(new std::vector<double>());
  std::auto_ptr<std::vector<double> > qgAxis1(new std::vector<double>());
  std::auto_ptr<std::vector<int> > qgnMult(new std::vector<int>());
  std::auto_ptr<std::vector<int> > qgcMult(new std::vector<int>());
*/
  std::auto_ptr<std::vector<double> > DeepCSVb(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVc(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVl(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVbb(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVcc(new std::vector<double>());

  std::auto_ptr<std::vector<double> > DeepCSVbN(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVcN(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVlN(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVbbN(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVccN(new std::vector<double>());

  std::auto_ptr<std::vector<double> > DeepCSVbP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVcP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVlP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVbbP(new std::vector<double>());
  std::auto_ptr<std::vector<double> > DeepCSVccP(new std::vector<double>());

  std::auto_ptr<std::vector<float> >CombinedIVF(new std::vector<float>());
  std::auto_ptr<std::vector<float> >CombinedIVFN(new std::vector<float>());
  std::auto_ptr<std::vector<float> >CombinedIVFP(new std::vector<float>());

  std::auto_ptr<std::vector<double> >Svtx(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SvtxN(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SvtxHP(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SvtxNHP(new std::vector<double>());

  std::auto_ptr<std::vector<double> >SoftM(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SoftMN(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SoftMP(new std::vector<double>());

  std::auto_ptr<std::vector<double> >SoftE(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SoftEN(new std::vector<double>());
  std::auto_ptr<std::vector<double> >SoftEP(new std::vector<double>());

  std::auto_ptr<std::vector<double> >DoubleSV(new std::vector<double>());

  std::auto_ptr<std::vector<double> >cMVA(new std::vector<double>());
  std::auto_ptr<std::vector<double> >cMVAv2(new std::vector<double>());
  std::auto_ptr<std::vector<double> >cMVAv2Neg(new std::vector<double>());
  std::auto_ptr<std::vector<double> >cMVAv2Pos(new std::vector<double>());

  std::auto_ptr<std::vector<double> >CvsB(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CvsBNeg(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CvsBPos(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CvsL(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CvsLNeg(new std::vector<double>());
  std::auto_ptr<std::vector<double> >CvsLPos(new std::vector<double>());

 //std::auto_ptr<std::vector<int>>nTracks(new std::vector<int>(ipTagInfo->selectedTracks().size(),-1));
 //std::auto_ptr<std::vector<int>>nSVs(new std::vector<int>(svTagInfo.size(),-1));

  std::auto_ptr<std::vector<double> > recoJetschargedHadronEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsneutralEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetschargedEmEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsneutralEmEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsmuonEnergyFraction(new std::vector<double>());

  std::auto_ptr<std::vector<double> > PhotonEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > ElectronEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > ChargedHadronMultiplicity(new std::vector<double>());
  std::auto_ptr<std::vector<double> > NeutralHadronMultiplicity(new std::vector<double>());
  std::auto_ptr<std::vector<double> > PhotonMultiplicity(new std::vector<double>());
  std::auto_ptr<std::vector<double> > ElectronMultiplicity(new std::vector<double>());
  std::auto_ptr<std::vector<double> > MuonMultiplicity(new std::vector<double>());

  std::auto_ptr<std::vector<double> > TagVarCSVtrackSumJetEtRatio(new std::vector<double>());

  std::auto_ptr<std::vector<int> > muMatchedJetIdx(new std::vector<int>(muLVec_->size(), -1));
  std::auto_ptr<std::vector<int> > eleMatchedJetIdx(new std::vector<int>(eleLVec_->size(), -1));

  std::auto_ptr<std::vector<int> > trksForIsoVetoMatchedJetIdx(new std::vector<int>(trksForIsoVetoLVec_->size(), -1));
  std::auto_ptr<std::vector<int> > looseisoTrksMatchedJetIdx(new std::vector<int>(looseisoTrksLVec_->size(), -1));

  //PUPPI
  std::auto_ptr<std::vector<TLorentzVector> > puppiJetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<TLorentzVector> > puppiSubJetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<double> > puppiSubJetsBdisc(new std::vector<double>());
  std::auto_ptr<std::vector<double> > puppisoftDropMass(new std::vector<double>());
  std::auto_ptr<std::vector<double> > puppitau1(new std::vector<double>());
  std::auto_ptr<std::vector<double> > puppitau2(new std::vector<double>());
  std::auto_ptr<std::vector<double> > puppitau3(new std::vector<double>());

  //ak8
  std::auto_ptr<std::vector<TLorentzVector> > ak8JetsLVec(new std::vector<TLorentzVector>());  
  std::auto_ptr<std::vector<TLorentzVector> > ak8SubJetsLVec(new std::vector<TLorentzVector>());  
  std::auto_ptr<std::vector<double> > ak8SubJetsBdisc(new std::vector<double>());
  std::auto_ptr<std::vector<double> > softDropMass(new std::vector<double>());
  std::auto_ptr<std::vector<double> > tau1(new std::vector<double>());
  std::auto_ptr<std::vector<double> > tau2(new std::vector<double>());
  std::auto_ptr<std::vector<double> > tau3(new std::vector<double>());


  if( !isData_ ){
     int cntJetPassPtCut = 0;
     for(unsigned int io=0; io < otherjets->size(); io++){
        const double otjet_pt = otherjets->at(io).pt(), otjet_eta = otherjets->at(io).eta(), otjet_phi = otherjets->at(io).phi();
        TLorentzVector perLVec; perLVec.SetPtEtaPhiE(otjet_pt, otjet_eta, otjet_phi, otherjets->at(io).energy());
        int cntFound = 0, matchedIdx = -1;
        double minDR = 999.0;
        for(unsigned int ij=0; ij< jets->size(); ij++){
           const double jet_eta = jets->at(ij).eta(), jet_phi = jets->at(ij).phi();
           const double dR = reco::deltaR(otjet_eta, otjet_phi, jet_eta, jet_phi);
           if( minDR > dR ){
              minDR = dR; matchedIdx = ij;
           }
        }
      if( matchedIdx != -1 )
      {
        if( minDR < dR_for_xCheck_ && std::abs(otjet_pt - jets->at(matchedIdx).pt())/jets->at(matchedIdx).pt() < relPt_for_xCheck_ ){ cntFound ++; }
      }
      if( otjet_pt >= jetPtCut_miniAOD_ )
      {
        cntJetPassPtCut ++;
        if( cntFound != 1 && debug_ )
        {
          std::cout<<"WARNING ... jet mis-matching between otherjets and jets for pt > "<<jetPtCut_miniAOD_<<"  matchedIdx : "<<matchedIdx<<"  cntFound : "<<cntFound<<"  minDR : "<<minDR<<std::endl;
          std::cout<<"otjet_pt : "<<otjet_pt<<"  otjet_eta : "<<otjet_eta<<"  otjet_phi : "<<otjet_phi<<std::endl;
          if( matchedIdx != -1 ) std::cout<<"  jet_pt : "<<jets->at(matchedIdx).pt()<<"    jet_eta : "<<jets->at(matchedIdx).eta()<<"    jet_phi : "<<jets->at(matchedIdx).phi()<<std::endl;
        }
      }
      else
      {
        if( cntFound && debug_ )
        {
          std::cout<<"WARNING ... otjet with pt : "<<otjet_pt<<"  matching to one of the jets for pt > "<<jetPtCut_miniAOD_<<" ?!"<<std::endl;
          std::cout<<"otjet_pt : "<<otjet_pt<<"  otjet_eta : "<<otjet_eta<<"  otjet_phi : "<<otjet_phi<<std::endl;
          std::cout<<"  jet_pt : "<<jets->at(matchedIdx).pt()<<"    jet_eta : "<<jets->at(matchedIdx).eta()<<"    jet_phi : "<<jets->at(matchedIdx).phi()<<std::endl;
        }
        else
        {
          int cntgenMatch = 0;
          for(unsigned int ig=0; ig<W_emuVec_->size(); ig++)
          {
            int perIdx = W_emuVec_->at(ig);
            TLorentzVector genLVec = genDecayLVec_->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int ig=0; ig<W_tauVec_->size(); ig++)
          {
            int perIdx = W_tauVec_->at(ig);
            TLorentzVector genLVec = genDecayLVec_->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int ig=0; ig<W_tau_emuVec_->size(); ig++)
          {
            int perIdx = W_tau_emuVec_->at(ig);
            TLorentzVector genLVec = genDecayLVec_->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int ig=0; ig<W_tau_prongsVec_->size(); ig++)
          {
            int perIdx = W_tau_prongsVec_->at(ig);
            TLorentzVector genLVec = genDecayLVec_->at(perIdx);
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int ig=0; ig<W_tauVec_->size(); ig++)
          {
            int perIdx = W_tauVec_->at(ig);
            TLorentzVector genLVec = genDecayLVec_->at(perIdx);
            for(unsigned int in=0; in<W_tau_nuVec_->size(); in++)
            {
              int perJdx = W_tau_nuVec_->at(in);
              TLorentzVector gennuLVec = genDecayLVec_->at(perJdx);
              int momIdx = perJdx;
              bool isFound = false;
              while( momIdx != -1 )
              {
                momIdx = genDecayMomRefVec_->at(momIdx);
                if( momIdx == perIdx ){ isFound = true; break; }
              }
              if( isFound ) genLVec -= gennuLVec;
            }
            double perdeltaR = perLVec.DeltaR(genLVec);
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int im=0; im<muLVec_->size(); im++)
          {
            double perdeltaR = perLVec.DeltaR(muLVec_->at(im));
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          for(unsigned int ie=0; ie<eleLVec_->size(); ie++)
          {
            double perdeltaR = perLVec.DeltaR(eleLVec_->at(ie));
            if( perdeltaR < genMatch_dR_ ) cntgenMatch ++;
          }
          if( cntgenMatch ){ extJets.push_back(otherjets->at(io)); }
        }
      }
    } 
   
    if( cntJetPassPtCut != (int)jets->size() && debug_ ) std::cout<<"WARNING ... cntJetPassPtCut : "<<cntJetPassPtCut<<"  NOT EQUAL jets->size : "<<jets->size()<<std::endl;
    if( (int)jets->size() >= 4 && std::abs(1.0*cntJetPassPtCut - 1.0*jets->size())/(1.0*jets->size()) > 0.1 && debug_ )
    {
      std::cout<<"\nWARNING ... cntJetPassPtCut : "<<cntJetPassPtCut<<"  slimmedJets.size : "<<jets->size()<<std::endl;
      std::cout<<"Please checking if global tag used for re-clustering the jets is the same as used to produce the miniAOD!"<<std::endl<<std::endl;
    }
  }
  //PUPPI

  for(unsigned int ip = 0; ip < puppiJets->size(); ip++){
     TLorentzVector perPuppiJetLVec;
     perPuppiJetLVec.SetPtEtaPhiE( puppiJets->at(ip).pt(), puppiJets->at(ip).eta(), puppiJets->at(ip).phi(), puppiJets->at(ip).energy() );
     puppiJetsLVec->push_back(perPuppiJetLVec);

     double puppi_tau1_uf         = puppiJets->at(ip).userFloat(NjettinessAK8Puppi_label_+":tau1");
     puppitau1->push_back(puppi_tau1_uf);
     double puppi_tau2_uf         = puppiJets->at(ip).userFloat(NjettinessAK8Puppi_label_+":tau2");
     puppitau2->push_back(puppi_tau2_uf);
     double puppi_tau3_uf         = puppiJets->at(ip).userFloat(NjettinessAK8Puppi_label_+":tau3");
     puppitau3->push_back(puppi_tau3_uf);

     double puppisoftDropMass_uf = puppiJets->at(ip).userFloat(ak8PFJetsPuppi_label_+"SoftDropMass");
     puppisoftDropMass->push_back(puppisoftDropMass_uf);
  }

  for(unsigned int ip = 0; ip < puppiSubJets->size(); ip++){
     // The subjet collection is a collection of softdropped jets, and you have to access the subjets from there
     // Most of the time there are two daughters, sometimes there is only one
     // Not all of the puppiJets have a matching puppiSubJets jet

     TLorentzVector perPuppiSubJetLVec;
     for(unsigned int id=0; id<puppiSubJets->at(ip).numberOfDaughters(); ++id){
       perPuppiSubJetLVec.SetPtEtaPhiE( puppiSubJets->at(ip).daughter(id)->pt(), 
					puppiSubJets->at(ip).daughter(id)->eta(), 
					puppiSubJets->at(ip).daughter(id)->phi(), 
					puppiSubJets->at(ip).daughter(id)->energy() );
       puppiSubJetsLVec->push_back(perPuppiSubJetLVec);
       // btag info is not available for reco::Candidates, so must cast it to a pat::Jet
       puppiSubJetsBdisc->push_back(dynamic_cast<const pat::Jet *>(puppiSubJets->at(ip).daughter(id))->bDiscriminator(bTagKeyString_.c_str()));
     }
  }
  //Puppi End ************
  //
  for(unsigned int ak = 0; ak < ak8Jets->size(); ak++){

     TLorentzVector perak8JetLVec;
     perak8JetLVec.SetPtEtaPhiE( ak8Jets->at(ak).pt(), ak8Jets->at(ak).eta(), ak8Jets->at(ak).phi(), ak8Jets->at(ak).energy() );
     ak8JetsLVec->push_back(perak8JetLVec);
 
     double softDropMass_uf = ak8Jets->at(ak).userFloat("ak8PFJetsCHSSoftDropMass");
     softDropMass->push_back(softDropMass_uf);
     double tau1_uf = ak8Jets->at(ak).userFloat("NjettinessAK8:tau1");
     tau1->push_back(tau1_uf);
     double tau2_uf         = ak8Jets->at(ak).userFloat("NjettinessAK8:tau2");
     tau2->push_back(tau2_uf);
     double tau3_uf         = ak8Jets->at(ak).userFloat("NjettinessAK8:tau3");
     tau3->push_back(tau3_uf);  
  }

  for(unsigned int ik = 0; ik < ak8SubJets->size(); ik++){
    // This subjet collection seems to have double the number of jets compared to the ak8Jets collection. 
    // So probably this is just a list of the subjets, which we can deltaR match to the ak8Jets.

    TLorentzVector perSubJetLVec;
    perSubJetLVec.SetPtEtaPhiE( ak8SubJets->at(ik).pt(), 
				ak8SubJets->at(ik).eta(), 
				ak8SubJets->at(ik).phi(), 
				ak8SubJets->at(ik).energy() );
    ak8SubJetsLVec->push_back(perSubJetLVec);
    ak8SubJetsBdisc->push_back(ak8SubJets->at(ik).bDiscriminator(bTagKeyString_.c_str()));
  }


  int cntJetLowPt = 0;
  for(unsigned int ij=0; ij < extJets.size(); ij++)
  {

 
    const pat::Jet& jet = extJets[ij];

    const reco::TrackIPTagInfo *ipTagInfo = jet.tagInfoTrackIP(ipTagInfos_.c_str());
    const auto    *svTagInfo = jet.tagInfoSecondaryVertex(svTagInfos_.c_str());

    TLorentzVector perJetLVec;
    perJetLVec.SetPtEtaPhiE( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
    jetsLVec->push_back(perJetLVec);

    //Additional jec qualities
    std::vector<std::string> availableJECSets   = jet.availableJECSets();
    std::vector<std::string> availableJECLevels = jet.availableJECLevels(jet.currentJECSet());
    double scaleRawToFull = jet.jecFactor(jet.currentJECLevel(), "none", jet.currentJECSet())/jet.jecFactor("Uncorrected", "none", jet.currentJECSet());
    //double scaleRawToFull = jet.jecFactor(availableJECLevels.back())/jet.jecFactor("Uncorrected");
    recoJetsJecScaleRawToFull->push_back(scaleRawToFull);
    if( debug_ && ij==0 )
    {
      std::cout<<"\nAvailable JEC sets:"<<"   current : "<<jet.currentJECSet().c_str()<<std::endl;
      for(unsigned int ia=0; ia<availableJECSets.size(); ia++)
      {
         std::cout<<"ia : "<<ia<<"  --> "<<availableJECSets[ia].c_str()<<std::endl;
      }
      std::cout<<"\nAvailable JEC levels:"<<"   current : "<<jet.currentJECLevel().c_str()<<std::endl;
      for(unsigned int ia=0; ia<availableJECLevels.size(); ia++)
      {
        std::cout<<"ia : "<<ia<<"  --> "<<availableJECLevels[ia].c_str()<<std::endl;
      }
      std::cout<<"scaleRawToFull : "<<scaleRawToFull<<"  current : "<<jet.jecFactor(availableJECLevels.back())<<"  uncor : "<<jet.jecFactor("Uncorrected")<<std::endl;
    }

    //get JEC unc for this jet, using corrected pT
    jecUnc->setJetEta(jet.eta());
    jecUnc->setJetPt(jet.pt());

    double uncertainty = jecUnc->getUncertainty(true);
    //safety check if uncertainty is not available for a jet
    if( uncertainty==-999. ) uncertainty = 0;
    recoJetsJecUnc->push_back(uncertainty);

    if( perJetLVec.Pt() < jetPtCut_miniAOD_ && ij < jets->size() ) cntJetLowPt ++;

    int flavor = jet.partonFlavour();
    recoJetsFlavor->push_back(flavor);

    std::string toGetName = qgTaggerKey_+":qgLikelihood";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:qgLikelihood";
    double thisqgLikelihood = jet.userFloat(toGetName.c_str());
    qgLikelihood->push_back(thisqgLikelihood);
 
    toGetName = qgTaggerKey_+":ptD";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:ptD";
    double thisqgPtD = jet.userFloat(toGetName.c_str());
    qgPtD->push_back(thisqgPtD);
   
    toGetName = qgTaggerKey_+":axis2"; 
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:axis2";
    double thisqgAxis2 = jet.userFloat(toGetName.c_str());
    qgAxis2->push_back(thisqgAxis2);
   
    toGetName = qgTaggerKey_+":mult"; 
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:mult";
    int thisqgMult = jet.userInt(toGetName.c_str());
    qgMult->push_back(thisqgMult);
/*
    toGetName = qgTaggerKey_+":ptDrLog";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:ptDrLog";
    double thisqgPtDrLog = jet.userFloat(toGetName.c_str());
    qgPtDrLog->push_back(thisqgPtDrLog);

    toGetName = qgTaggerKey_+":axis1";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:axis1";
    double thisqgAxis1 = jet.userFloat(toGetName.c_str());
    qgAxis1->push_back(thisqgAxis1);

    toGetName = qgTaggerKey_+":cmult";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:cmult";
    int thisqgcMult = jet.userInt(toGetName.c_str());
    qgcMult->push_back(thisqgcMult);

    toGetName = qgTaggerKey_+":nmult";
    if( ij >= jets->size() && qgTaggerKey_ == "QGTagger" ) toGetName = qgTaggerKey_+"Other:nmult";
    int thisqgnMult = jet.userInt(toGetName.c_str());
    qgnMult->push_back(thisqgnMult);
*/
    float trialDeepCSVb = jet.bDiscriminator((deepCSVBJetTags_+":probb").c_str());
    DeepCSVb->push_back(trialDeepCSVb);

    float trialDeepCSVc = jet.bDiscriminator((deepCSVBJetTags_+":probc").c_str());
    DeepCSVc->push_back(trialDeepCSVc);

    float trialDeepCSVl = jet.bDiscriminator((deepCSVBJetTags_+":probudsg").c_str());
    DeepCSVl->push_back(trialDeepCSVl);

    float trialDeepCSVbb = jet.bDiscriminator((deepCSVBJetTags_+":probbb").c_str());
    DeepCSVbb->push_back(trialDeepCSVbb);

    float trialDeepCSVcc = jet.bDiscriminator((deepCSVBJetTags_+":probcc").c_str());
    DeepCSVcc->push_back(trialDeepCSVcc);

    float trialDeepCSVbN = jet.bDiscriminator((deepCSVNegBJetTags_+":probb").c_str());
    DeepCSVbN->push_back(trialDeepCSVbN);

    float trialDeepCSVcN = jet.bDiscriminator((deepCSVNegBJetTags_+":probc").c_str());
    DeepCSVcN->push_back(trialDeepCSVcN);

    float trialDeepCSVlN = jet.bDiscriminator((deepCSVNegBJetTags_+":probudsg").c_str());
    DeepCSVlN->push_back(trialDeepCSVlN);

    float trialDeepCSVbbN = jet.bDiscriminator((deepCSVNegBJetTags_+":probbb").c_str());
    DeepCSVbbN->push_back(trialDeepCSVbbN);

    double trialDeepCSVccN = jet.bDiscriminator((deepCSVNegBJetTags_+":probcc").c_str());
    DeepCSVccN->push_back(trialDeepCSVccN);

    double trialDeepCSVbP = jet.bDiscriminator((deepCSVPosBJetTags_+":probb").c_str());
    DeepCSVbP->push_back(trialDeepCSVbP);

    double trialDeepCSVcP = jet.bDiscriminator((deepCSVPosBJetTags_+":probc").c_str());
    DeepCSVcP->push_back(trialDeepCSVcP);

    double trialDeepCSVlP = jet.bDiscriminator((deepCSVPosBJetTags_+":probudsg").c_str());
    DeepCSVlP->push_back(trialDeepCSVlP);

    double trialDeepCSVbbP = jet.bDiscriminator((deepCSVPosBJetTags_+":probbb").c_str());
    DeepCSVbbP->push_back(trialDeepCSVbbP);

    double trialDeepCSVccP = jet.bDiscriminator((deepCSVPosBJetTags_+":probcc").c_str());
    DeepCSVccP->push_back(trialDeepCSVccP);

    float tri_CombinedIVF =jet.bDiscriminator(combinedIVFSVBJetTags_.c_str());
    CombinedIVF->push_back(tri_CombinedIVF);

    float tri_CombinedIVF_P =jet.bDiscriminator(combinedIVFSVPosBJetTags_.c_str());
    CombinedIVFP ->push_back(tri_CombinedIVF_P);

    float tri_CombinedIVF_N =jet.bDiscriminator(combinedIVFSVNegBJetTags_.c_str());
    CombinedIVFN ->push_back(tri_CombinedIVF_N);

    double btag = jet.bDiscriminator(bTagKeyString_.c_str());
    recoJetsBtag->push_back(btag);

    double Proba = jet.bDiscriminator(jetPBJetTags_.c_str());
    JetProba->push_back(Proba);

    double ProbaN = jet.bDiscriminator(jetPNegBJetTags_.c_str());
    JetProbaN->push_back(ProbaN);

    double ProbaP = jet.bDiscriminator(jetPPosBJetTags_.c_str());
    JetProbaP->push_back(ProbaP);

    double Bprob = jet.bDiscriminator(jetBPBJetTags_.c_str());
    JetBprob->push_back(Bprob);

    double BprobN = jet.bDiscriminator(jetBPNegBJetTags_.c_str());
    JetBprobN->push_back(BprobN);

    double BprobP = jet.bDiscriminator(jetBPPosBJetTags_.c_str());
    JetBprobP->push_back(BprobP);

    double Tri_CombinedSvtx = jet.bDiscriminator(combinedSVBJetTags_.c_str());
    CombinedSvtx->push_back(Tri_CombinedSvtx);

    double Tri_CombinedSvtxN = jet.bDiscriminator(combinedSVNegBJetTags_.c_str());
    CombinedSvtxN->push_back(Tri_CombinedSvtxN);
    
    double Tri_CombinedSvtxP = jet.bDiscriminator(combinedSVPosBJetTags_.c_str());
    CombinedSvtxP-> push_back(Tri_CombinedSvtxP);

    float tri_Svtx = jet.bDiscriminator(simpleSVHighEffBJetTags_.c_str());
    Svtx  ->push_back(tri_Svtx);
    float tri_SvtxN = jet.bDiscriminator(simpleSVNegHighEffBJetTags_.c_str());
    SvtxN ->push_back(tri_SvtxN);
    float tri_SvtxHP  = jet.bDiscriminator(simpleSVHighPurBJetTags_.c_str());
    SvtxHP->push_back(tri_SvtxHP);
    float tri_SvtxNHP = jet.bDiscriminator(simpleSVNegHighPurBJetTags_.c_str());
    SvtxNHP->push_back(tri_SvtxNHP);

    float tri_SoftM  = jet.bDiscriminator(softPFMuonBJetTags_.c_str());
    SoftM->push_back(tri_SoftM);
    float tri_SoftMN = jet.bDiscriminator(softPFMuonNegBJetTags_.c_str());
    SoftMN->push_back(tri_SoftMN);
    float tri_SoftMP = jet.bDiscriminator(softPFMuonPosBJetTags_.c_str());
    SoftMP->push_back(tri_SoftMP);

    float tri_SoftE  = jet.bDiscriminator(softPFElectronBJetTags_.c_str());
    SoftE->push_back(tri_SoftE);
    float tri_SoftEN = jet.bDiscriminator(softPFElectronNegBJetTags_.c_str());
    SoftEN->push_back(tri_SoftEN);
    float tri_SoftEP = jet.bDiscriminator(softPFElectronPosBJetTags_.c_str());
    SoftEP->push_back(tri_SoftEP);

    float tri_DoubleSV = jet.bDiscriminator(doubleSVBJetTags_.c_str());
    DoubleSV->push_back(tri_DoubleSV);

    float tri_cMVA = jet.bDiscriminator(cMVABJetTags_.c_str());
    cMVA->push_back(tri_cMVA);
    float tri_cMVAv2 = jet.bDiscriminator(cMVAv2BJetTags_.c_str());
    cMVAv2->push_back(tri_cMVAv2);
    float tri_cMVAv2Neg = jet.bDiscriminator(cMVAv2NegBJetTags_.c_str());
    cMVAv2Neg->push_back(tri_cMVAv2Neg);
    float tri_cMVAv2Pos = jet.bDiscriminator(cMVAv2PosBJetTags_.c_str());
    cMVAv2Pos->push_back(tri_cMVAv2Pos);

    float tri_CvsB = jet.bDiscriminator(CvsBCJetTags_.c_str());
    CvsB->push_back(tri_CvsB);
    float tri_CvsBNeg = jet.bDiscriminator(CvsBNegCJetTags_.c_str());
    CvsBNeg->push_back(tri_CvsBNeg);
    float tri_CvsBPos = jet.bDiscriminator(CvsBPosCJetTags_.c_str());
    CvsBPos->push_back(tri_CvsBPos);
    float tri_CvsL = jet.bDiscriminator(CvsLCJetTags_.c_str());
    CvsL->push_back(tri_CvsL);
    float tri_CvsLNeg = jet.bDiscriminator(CvsLNegCJetTags_.c_str());
    CvsLNeg->push_back(tri_CvsLNeg);
    float tri_CvsLPos = jet.bDiscriminator(CvsLPosCJetTags_.c_str());
    CvsLPos->push_back(tri_CvsLPos);

    //reco::TaggingVariableList ipVars = ipTagInfo->taggingVariables();
    //reco::TaggingVariableList svVars = svTagInfo->taggingVariables();

// std::auto_ptr<std::vector<int>>nTracks(new std::vector<int>(ipTagInfo->selectedTracks().size(),-1));
// std::auto_ptr<std::vector<int>>nSVs(new std::vector<int>(ipTagInfo->selectedTracks().size(),-1));

    //int nTracks_tri = ipTagInfo->selectedTracks().size();
    //int nSVs_tri = svTagInfo->nVertices();

    // per jet
    //nTracks->push_back(nTracks_tri);
    //nSVs->push_back(nSVs_tri);

    double charge = jet.jetCharge();
    recoJetsCharge->push_back(charge);

    double chargedHadronEnergyFraction = jet.chargedHadronEnergyFraction();
    recoJetschargedHadronEnergyFraction->push_back( chargedHadronEnergyFraction );

    double neutralHadronEnergyFraction = jet.neutralHadronEnergyFraction();
    recoJetsneutralEnergyFraction->push_back( neutralHadronEnergyFraction );

    double chargedEmEnergyFraction = jet.chargedEmEnergyFraction();
    recoJetschargedEmEnergyFraction->push_back( chargedEmEnergyFraction );

    double neutralEmEnergyFraction = jet.neutralEmEnergyFraction();
    recoJetsneutralEmEnergyFraction->push_back( neutralEmEnergyFraction );

    double muonEnergyFraction = jet.muonEnergyFraction();
    recoJetsmuonEnergyFraction->push_back( muonEnergyFraction );

    double photonEnergyFraction = jet.photonEnergyFraction();
    PhotonEnergyFraction->push_back( photonEnergyFraction );

    double electronEnergyFraction = jet.electronEnergyFraction();
    ElectronEnergyFraction->push_back( electronEnergyFraction );

    double chargedHadronMultiplicity = jet.chargedHadronMultiplicity();
    ChargedHadronMultiplicity->push_back( chargedHadronMultiplicity );

    double neutralHadronMultiplicity = jet.neutralHadronMultiplicity();
    NeutralHadronMultiplicity->push_back( neutralHadronMultiplicity );

    double photonMultiplicity = jet.photonMultiplicity();
    PhotonMultiplicity->push_back( photonMultiplicity );

    double electronMultiplicity = jet.electronMultiplicity();
    ElectronMultiplicity->push_back( electronMultiplicity );

    double muonMultiplicity1 = jet.muonMultiplicity();
    MuonMultiplicity->push_back( muonMultiplicity1 );

    //WRONGconst reco::ShallowTagInfo* tagInfo = dynamic_cast<const reco::ShallowTagInfo*>(jet.tagInfo(tagInfoName_));
    //WORNGreco::TaggingVariableList vars = tagInfo->taggingVariables();
    //reco::TaggingVariableList ipVars = ipTagInfo->taggingVariables();

/*
       const reco::BoostedDoubleSVTagInfo *bdsvTagInfo = fj->tagInfoBoostedDoubleSV(tagInfoFName_);  
     // const reco::ShallowTagInfo* bdsvTagInfo =  dynamic_cast<const reco::ShallowTagInfo*>(jet.tagInfoBoostedDoubleSV(tagInfoFName_));
        reco::TaggingVariableList vars = bdsvTagInfo->taggingVariables();
*/ 
 
    //trackJetPt_                 = vars.get(reco::btau::trackJetPt, -999);

    //double TagVarCSVtrackSumJetEtRatio_TR = jet.TagVar_jetNSecondaryVertices;
    //TagVarCSVtrackSumJetEtRatio->push_back( TagVarCSVtrackSumJetEtRatio_TR );

    //std::cout << trackJetPt_ << std::endl;

    //const std::vector<reco::PFCandidatePtr> & constituents = jet.getPFConstituents();
    //const unsigned int numConstituents = constituents.size();
    const unsigned int numConstituents = jet.numberOfDaughters();

    for(unsigned int im=0; im < muLVec_->size(); im++)
    {
      double muEta = muLVec_->at(im).Eta(), muPhi = muLVec_->at(im).Phi();
      double mindRmuonCon = 999.;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
        //const reco::PFCandidatePtr& constituent = constituents[iCon];
        const reco::Candidate * constituent = jet.daughter(iCon);
        const double dRmuonCon = reco::deltaR(constituent->eta(), constituent->phi(), muEta, muPhi);
        if( mindRmuonCon > dRmuonCon ){ mindRmuonCon = dRmuonCon; }
      }
      if( mindRmuonCon < deltaRcon_ ) (*muMatchedJetIdx)[im] = ij;
    }

    for(unsigned int ie=0; ie < eleLVec_->size(); ie++)
    {
      double eleEta = eleLVec_->at(ie).Eta(), elePhi = eleLVec_->at(ie).Phi();
      double mindReleCon = 999.;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
        //const reco::PFCandidatePtr& constituent = constituents[iCon];
        const reco::Candidate * constituent = jet.daughter(iCon);
        const double dReleCon = reco::deltaR(constituent->eta(), constituent->phi(), eleEta, elePhi);
        if( mindReleCon > dReleCon ){ mindReleCon = dReleCon; }
      }
      if( mindReleCon < deltaRcon_ ) (*eleMatchedJetIdx)[ie] = ij;
    }

    for(unsigned int it=0; it < trksForIsoVetoLVec_->size(); it++)
    {
      double trkEta = trksForIsoVetoLVec_->at(it).Eta(), trkPhi = trksForIsoVetoLVec_->at(it).Phi();
      double mindRtrkCon = 999.;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
        //          const reco::PFCandidatePtr& constituent = constituents[iCon];
        const reco::Candidate * constituent = jet.daughter(iCon);
        const double dRtrkCon = reco::deltaR(constituent->eta(), constituent->phi(), trkEta, trkPhi);
        if( mindRtrkCon > dRtrkCon ){ mindRtrkCon = dRtrkCon; }
      }
      if( mindRtrkCon < deltaRcon_ ) (*trksForIsoVetoMatchedJetIdx)[it] = ij;
    }

    for(unsigned int ist=0; ist < looseisoTrksLVec_->size(); ist++)
    {
      double isotrkEta = looseisoTrksLVec_->at(ist).Eta(), isotrkPhi = looseisoTrksLVec_->at(ist).Phi();
      double mindRisotrkCon = 999.;
      for(unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
        //const reco::PFCandidatePtr& constituent = constituents[iCon];
        const reco::Candidate * constituent = jet.daughter(iCon);
        const double dRisotrkCon = reco::deltaR(constituent->eta(), constituent->phi(), isotrkEta, isotrkPhi);
        if( mindRisotrkCon > dRisotrkCon ){ mindRisotrkCon = dRisotrkCon; }
      }
      if( mindRisotrkCon < deltaRcon_ ) (*looseisoTrksMatchedJetIdx)[ist] = ij;
    }
  }

  if( cntJetLowPt ) std::cout<<"WARNING ... NON ZERO ("<<cntJetLowPt<<") number of jets with pt < "<<jetPtCut_miniAOD_<<std::endl;

  std::auto_ptr<int> nJets (new int);

  *nJets = jetsLVec->size();

  // store in the event
  // iEvent.put(prod);
  iEvent.put(jetsLVec, "jetsLVec");
  iEvent.put(recoJetsFlavor, "recoJetsFlavor");
  iEvent.put(recoJetsBtag, "recoJetsBtag");

  iEvent.put(JetProba, "JetProba");
  iEvent.put(JetProbaN, "JetProbaN");
  iEvent.put(JetProbaP, "JetProbaP");
  iEvent.put(JetBprob, "JetBprob");
  iEvent.put(JetBprobP, "JetBprobP");
  iEvent.put(JetBprobN, "JetBprobN");

  iEvent.put(recoJetsCharge, "recoJetsCharge");
  iEvent.put(recoJetsJecUnc, "recoJetsJecUnc");
  iEvent.put(recoJetsJecScaleRawToFull, "recoJetsJecScaleRawToFull");
  iEvent.put(nJets, "nJets");

  iEvent.put(qgLikelihood, "qgLikelihood");
  iEvent.put(qgPtD, "qgPtD");
  iEvent.put(qgAxis2, "qgAxis2");
  iEvent.put(qgMult, "qgMult");
/*
  iEvent.put(qgPtDrLog, "qgPtDrLog");
  iEvent.put(qgAxis1, "qgAxis1");
  iEvent.put(qgcMult, "qgcMult");
  iEvent.put(qgnMult, "qgnMult");
*/
  iEvent.put(DeepCSVb,"DeepCSVb");
  iEvent.put(DeepCSVc,"DeepCSVc");
  iEvent.put(DeepCSVl,"DeepCSVl");
  iEvent.put(DeepCSVbb,"DeepCSVbb");
  iEvent.put(DeepCSVcc,"DeepCSVcc");

  iEvent.put(DeepCSVbN,"DeepCSVbN");
  iEvent.put(DeepCSVcN,"DeepCSVcN");
  iEvent.put(DeepCSVlN,"DeepCSVlN");
  iEvent.put(DeepCSVbbN,"DeepCSVbbN");
  iEvent.put(DeepCSVccN,"DeepCSVccN");

  iEvent.put(DeepCSVbP,"DeepCSVbP");
  iEvent.put(DeepCSVcP,"DeepCSVcP");
  iEvent.put(DeepCSVlP,"DeepCSVlP");
  iEvent.put(DeepCSVbbP,"DeepCSVbbP");
  iEvent.put(DeepCSVccP,"DeepCSVccP");

  iEvent.put(CombinedSvtx,"CombinedSvtx");
  iEvent.put(CombinedSvtxN,"CombinedSvtxN");
  iEvent.put(CombinedSvtxP,"CombinedSvtxP");

  iEvent.put(CombinedIVF,"CombinedIVF");
  iEvent.put(CombinedIVFP,"CombinedIVFP");
  iEvent.put(CombinedIVFN,"CombinedIVFN");

  iEvent.put(Svtx,"Svtx");
  iEvent.put(SvtxN,"SvtxN");
  iEvent.put(SvtxHP,"SvtxHP");
  iEvent.put(SvtxNHP,"SvtxNHP");

  iEvent.put(SoftM,"SoftM");
  iEvent.put(SoftMN,"SoftMN");
  iEvent.put(SoftMP,"SoftMP");

  iEvent.put(SoftE,"SoftE");
  iEvent.put(SoftEN,"SoftEN");
  iEvent.put(SoftEP,"SoftEP");

  iEvent.put(DoubleSV,"DoubleSV");

  iEvent.put(cMVA,"cMVA");
  iEvent.put(cMVAv2,"cMVAv2");
  iEvent.put(cMVAv2Neg,"cMVAv2Neg");
  iEvent.put(cMVAv2Pos,"cMVAv2Pos");

  iEvent.put(CvsB,"CvsB");
  iEvent.put(CvsBNeg,"CvsBNeg");
  iEvent.put(CvsBPos,"CvsBPos");
  iEvent.put(CvsL,"CvsL");
  iEvent.put(CvsLNeg,"CvsLNeg");
  iEvent.put(CvsLPos,"CvsLPos");

  //iEvent.put(nTracks,"nTracks");
  //iEvent.put(nSVs,"nSVs");

  iEvent.put(recoJetschargedHadronEnergyFraction, "recoJetschargedHadronEnergyFraction");
  iEvent.put(recoJetsneutralEnergyFraction, "recoJetsneutralEnergyFraction");
  iEvent.put(recoJetschargedEmEnergyFraction, "recoJetschargedEmEnergyFraction");
  iEvent.put(recoJetsneutralEmEnergyFraction, "recoJetsneutralEmEnergyFraction");

  iEvent.put(recoJetsmuonEnergyFraction, "recoJetsmuonEnergyFraction");
  iEvent.put(PhotonEnergyFraction, "PhotonEnergyFraction");
  iEvent.put(ElectronEnergyFraction, "ElectronEnergyFraction");

  iEvent.put(ChargedHadronMultiplicity, "ChargedHadronMultiplicity");
  iEvent.put(NeutralHadronMultiplicity, "NeutralHadronMultiplicity");
  iEvent.put(PhotonMultiplicity, "PhotonMultiplicity");
  iEvent.put(ElectronMultiplicity, "ElectronMultiplicity");
  iEvent.put(MuonMultiplicity, "MuonMultiplicity");

  //iEvent.put(TagVarCSVtrackSumJetEtRatio,"TagVarCSVtrackSumJetEtRatio");

  iEvent.put(muMatchedJetIdx, "muMatchedJetIdx");
  iEvent.put(eleMatchedJetIdx, "eleMatchedJetIdx");

  iEvent.put(trksForIsoVetoMatchedJetIdx, "trksForIsoVetoMatchedJetIdx");
  iEvent.put(looseisoTrksMatchedJetIdx, "looseisoTrksMatchedJetIdx");
  //PUPPI
  iEvent.put(puppiJetsLVec, "puppiJetsLVec");
  iEvent.put(puppiSubJetsLVec, "puppiSubJetsLVec");
  iEvent.put(puppiSubJetsBdisc, "puppiSubJetsBdisc");
  iEvent.put(puppisoftDropMass,"puppisoftDropMass");
  iEvent.put(puppitau1,"puppitau1");
  iEvent.put(puppitau2,"puppitau2");
  iEvent.put(puppitau3,"puppitau3");

  //ak8
  iEvent.put(ak8JetsLVec, "ak8JetsLVec");
  iEvent.put(ak8SubJetsLVec, "ak8SubJetsLVec");
  iEvent.put(ak8SubJetsBdisc, "ak8SubJetsBdisc");
  iEvent.put(softDropMass,"softDropMass");
  iEvent.put(tau1, "tau1");
  iEvent.put(tau2, "tau2");
  iEvent.put(tau3, "tau3");

  //iEvent.put(trackJetPt_, "trackJetPt_");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodJets);
