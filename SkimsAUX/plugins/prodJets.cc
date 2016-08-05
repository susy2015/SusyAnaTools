
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

  std::string jetType_;
};


prodJets::prodJets(const edm::ParameterSet & iConfig) 
{
  isData_ = true;

  jetSrc_      = iConfig.getParameter<edm::InputTag>("jetSrc");
  std::cout << "Input Tag Label: " << jetSrc_.label() << std::endl;
  jetOtherSrc_ = iConfig.getParameter<edm::InputTag>("jetOtherSrc");
  std::cout << "Input Tag Label: " << jetOtherSrc_.label() << std::endl;
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");
  //metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");
  bTagKeyString_ = iConfig.getParameter<std::string>("bTagKeyString");

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

  //produces<std::vector<pat::Jet> >("");
  produces<std::vector<TLorentzVector> >("jetsLVec");
  produces<std::vector<int> >("recoJetsFlavor");
  produces<std::vector<double> >("recoJetsBtag");
  produces<std::vector<double> >("recoJetsJecUnc");
  produces<std::vector<double> >("recoJetsJecScaleRawToFull");
  produces<int>("nJets");
  produces<std::vector<double> >("qgLikelihood");
  produces<std::vector<double> >("qgPtD");
  produces<std::vector<double> >("qgAxis2");
  produces<std::vector<int> >("qgMult");

  //produce variables needed for Lost Lepton study, added by hua.wei@cern.ch
  produces<std::vector<double> >("recoJetschargedHadronEnergyFraction");
  produces<std::vector<double> >("recoJetschargedEmEnergyFraction");
  produces<std::vector<double> >("recoJetsneutralEmEnergyFraction");

  produces<std::vector<double> >("recoJetsmuonEnergyFraction");

  produces<std::vector<int> >("muMatchedJetIdx");
  produces<std::vector<int> >("eleMatchedJetIdx");

  produces<std::vector<int> >("trksForIsoVetoMatchedJetIdx");
  produces<std::vector<int> >("looseisoTrksMatchedJetIdx");
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

  //check which ones to keep
  //std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());
  std::auto_ptr<std::vector<TLorentzVector> > jetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<int> > recoJetsFlavor(new std::vector<int>());
  std::auto_ptr<std::vector<double> > recoJetsBtag(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsJecUnc(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsJecScaleRawToFull(new std::vector<double>());

  std::auto_ptr<std::vector<double> > qgLikelihood(new std::vector<double>());
  std::auto_ptr<std::vector<double> > qgPtD(new std::vector<double>());
  std::auto_ptr<std::vector<double> > qgAxis2(new std::vector<double>());
  std::auto_ptr<std::vector<int> > qgMult(new std::vector<int>());

  std::auto_ptr<std::vector<double> > recoJetschargedHadronEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetschargedEmEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsneutralEmEnergyFraction(new std::vector<double>());
  std::auto_ptr<std::vector<double> > recoJetsmuonEnergyFraction(new std::vector<double>());

  std::auto_ptr<std::vector<int> > muMatchedJetIdx(new std::vector<int>(muLVec_->size(), -1));
  std::auto_ptr<std::vector<int> > eleMatchedJetIdx(new std::vector<int>(eleLVec_->size(), -1));

  std::auto_ptr<std::vector<int> > trksForIsoVetoMatchedJetIdx(new std::vector<int>(trksForIsoVetoLVec_->size(), -1));
  std::auto_ptr<std::vector<int> > looseisoTrksMatchedJetIdx(new std::vector<int>(looseisoTrksLVec_->size(), -1));

  if( !isData_ )
  {
    int cntJetPassPtCut = 0;
    for(unsigned int io=0; io < otherjets->size(); io++)
    {
      const double otjet_pt = otherjets->at(io).pt(), otjet_eta = otherjets->at(io).eta(), otjet_phi = otherjets->at(io).phi();
      TLorentzVector perLVec; perLVec.SetPtEtaPhiE(otjet_pt, otjet_eta, otjet_phi, otherjets->at(io).energy());
      int cntFound = 0, matchedIdx = -1;
      double minDR = 999.0;
      for(unsigned int ij=0; ij< jets->size(); ij++)
      {
        const double jet_eta = jets->at(ij).eta(), jet_phi = jets->at(ij).phi();
        const double dR = reco::deltaR(otjet_eta, otjet_phi, jet_eta, jet_phi);
        if( minDR > dR )
        {
          minDR = dR; matchedIdx = ij;
        }
      }
      if( matchedIdx != -1 )
      {
        if( minDR < dR_for_xCheck_ && std::abs(otjet_pt - jets->at(matchedIdx).pt())/jets->at(matchedIdx).pt() < relPt_for_xCheck_ )
        {
          cntFound ++;
        }
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
          if( cntgenMatch )
          {
            extJets.push_back(otherjets->at(io));

            double thisqgLikelihood = (jets->at(matchedIdx)).userFloat("QGTagger:qgLikelihood");
            qgLikelihood->push_back(thisqgLikelihood);

            double thisqgPtD = (jets->at(matchedIdx)).userFloat("QGTagger:ptD");
            qgPtD->push_back(thisqgPtD);

            double thisqgAxis2 = (jets->at(matchedIdx)).userFloat("QGTagger:axis2");
            qgAxis2->push_back(thisqgAxis2);

            int thisqgMult = (jets->at(matchedIdx)).userInt("QGTagger:mult");
            qgMult->push_back(thisqgMult);
          }
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

  int cntJetLowPt = 0;
  //for(unsigned int ij=0; ij < jets->size(); ij++)  
  for(unsigned int ij=0; ij < extJets.size(); ij++)
  {
    //const pat::Jet& jet = jets->at(ij);
    const pat::Jet& jet = extJets[ij];

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
    /*
    double thisqgLikelihood = jet.userFloat("QGTagger:qgLikelihood");
    qgLikelihood->push_back(thisqgLikelihood);

    double thisqgPtD = jet.userFloat("QGTagger:ptD");
    qgPtD->push_back(thisqgPtD);
    
    double thisqgAxis2 = jet.userFloat("QGTagger:axis2");
    qgAxis2->push_back(thisqgAxis2);
    
    int thisqgMult = jet.userInt("QGTagger:mult");
    qgMult->push_back(thisqgMult);
    */
    double btag = jet.bDiscriminator(bTagKeyString_.c_str());
    recoJetsBtag->push_back(btag);

    double chargedHadronEnergyFraction = jet.chargedHadronEnergyFraction();
    recoJetschargedHadronEnergyFraction->push_back( chargedHadronEnergyFraction );

    double chargedEmEnergyFraction = jet.chargedEmEnergyFraction();
    recoJetschargedEmEnergyFraction->push_back( chargedEmEnergyFraction );

    double neutralEmEnergyFraction = jet.neutralEmEnergyFraction();
    recoJetsneutralEmEnergyFraction->push_back( neutralEmEnergyFraction );

    double muonEnergyFraction = jet.muonEnergyFraction();
    recoJetsmuonEnergyFraction->push_back( muonEnergyFraction );

    //std::cout << chargedEmEnergyFraction << std::endl;

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
        if( mindRmuonCon > dRmuonCon )
        {
          mindRmuonCon = dRmuonCon;
        }
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
        if( mindReleCon > dReleCon )
        {
          mindReleCon = dReleCon;
        }
      }
      if( mindReleCon < deltaRcon_ ) (*eleMatchedJetIdx)[ie] = ij;
    }

    for(unsigned int it=0; it < trksForIsoVetoLVec_->size(); it++)
    {
      double trkEta = trksForIsoVetoLVec_->at(it).Eta(), trkPhi = trksForIsoVetoLVec_->at(it).Phi();
      double mindRtrkCon = 999.;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
	      //const reco::PFCandidatePtr& constituent = constituents[iCon];
	      const reco::Candidate * constituent = jet.daughter(iCon);
	      const double dRtrkCon = reco::deltaR(constituent->eta(), constituent->phi(), trkEta, trkPhi);
	      if( mindRtrkCon > dRtrkCon )
        {
	        mindRtrkCon = dRtrkCon;
	      }
      }
      if( mindRtrkCon < deltaRcon_ ) (*trksForIsoVetoMatchedJetIdx)[it] = ij;
    }

    for(unsigned int ist=0; ist < looseisoTrksLVec_->size(); ist++)
    {
      double isotrkEta = looseisoTrksLVec_->at(ist).Eta(), isotrkPhi = looseisoTrksLVec_->at(ist).Phi();
      double mindRisotrkCon = 999.;
      for (unsigned int iCon = 0; iCon < numConstituents; ++iCon)
      {
        //const reco::PFCandidatePtr& constituent = constituents[iCon];
        const reco::Candidate * constituent = jet.daughter(iCon);
        const double dRisotrkCon = reco::deltaR(constituent->eta(), constituent->phi(), isotrkEta, isotrkPhi);
        if( mindRisotrkCon > dRisotrkCon )
        {
          mindRisotrkCon = dRisotrkCon;
        }
      }
      if( mindRisotrkCon < deltaRcon_ ) (*looseisoTrksMatchedJetIdx)[ist] = ij;
    }
  }

  if( cntJetLowPt ) std::cout<<"WARNING ... NON ZERO ("<<cntJetLowPt<<") number of jets with pt < "<<jetPtCut_miniAOD_<<std::endl;

  std::auto_ptr<int> nJets (new int);

  *nJets = jetsLVec->size();

  //store in the event
  //iEvent.put(prod);
  iEvent.put(jetsLVec, "jetsLVec");
  iEvent.put(recoJetsFlavor, "recoJetsFlavor");
  iEvent.put(recoJetsBtag, "recoJetsBtag");
  iEvent.put(recoJetsJecUnc, "recoJetsJecUnc");
  iEvent.put(recoJetsJecScaleRawToFull, "recoJetsJecScaleRawToFull");
  iEvent.put(nJets, "nJets");

  iEvent.put(qgLikelihood, "qgLikelihood");
  iEvent.put(qgPtD, "qgPtD");
  iEvent.put(qgAxis2, "qgAxis2");
  iEvent.put(qgMult, "qgMult");

  iEvent.put(recoJetschargedHadronEnergyFraction, "recoJetschargedHadronEnergyFraction");
  iEvent.put(recoJetschargedEmEnergyFraction, "recoJetschargedEmEnergyFraction");
  iEvent.put(recoJetsneutralEmEnergyFraction, "recoJetsneutralEmEnergyFraction");

  iEvent.put(recoJetsmuonEnergyFraction, "recoJetsmuonEnergyFraction");

  iEvent.put(muMatchedJetIdx, "muMatchedJetIdx");
  iEvent.put(eleMatchedJetIdx, "eleMatchedJetIdx");

  iEvent.put(trksForIsoVetoMatchedJetIdx, "trksForIsoVetoMatchedJetIdx");
  iEvent.put(looseisoTrksMatchedJetIdx, "looseisoTrksMatchedJetIdx");

  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodJets);
