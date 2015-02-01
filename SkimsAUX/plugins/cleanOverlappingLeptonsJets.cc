
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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "TLorentzVector.h"

class cleanOverlappingLeptonsJets : public edm::EDProducer {

  public:

    explicit cleanOverlappingLeptonsJets(const edm::ParameterSet & iConfig);
    ~cleanOverlappingLeptonsJets();

  private:

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    virtual void loadGenParticles(const edm::Event& iEvent);
    virtual void loadGenJets(const edm::Event& iEvent);

    const std::vector<TLorentzVector> & searchGenSigChnForAcc(int *signWptr=0, std::vector<TLorentzVector> *bosonVecPtr = 0);
    const std::vector<TLorentzVector> & searchGenEleSigChnForAcc(int *signWptr=0, std::vector<TLorentzVector> *bosonVecPtr = 0);

    edm::InputTag jetSrc_;
    edm::InputTag muonSrc_;
    edm::InputTag muonCSSrc_;
    edm::InputTag eleSrc_;

    typedef unsigned long key_type;
    edm::InputTag muonProductIDSrc_, muonKeySrc_;
    edm::InputTag eleProductIDSrc_, eleKeySrc_;

    std::vector<edm::ProductID> muonPFbaseProductIDVec_; std::vector<key_type> muonPFbaseKeyVec_;
    std::vector<edm::ProductID> elePFbaseProductIDVec_; std::vector<key_type> elePFbaseKeyVec_;

    bool doGenLevel_;
    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;
    double genJetMinPt_, genJetMaxEta_;
    double deltaR_, deltaRcon_, deltaRtighterForEle_;

    bool debug_;

    bool useEle_;

    bool verbose_;

    bool isData;

    bool useDeltaR_;

    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

    int _totCleanedMuonJetCnt, _totCleanedEleJetCnt;
    int _totInputMuonSizeCnt, _totInputEleSizeCnt;

    double recoJetMinPt_, recoJetMaxEta_;

    bool noRemovalOfBigJetMuonPtDiff_;
    double diffJetMuonPt_;

    std::string bTagKeyString_;
};


cleanOverlappingLeptonsJets::cleanOverlappingLeptonsJets(const edm::ParameterSet & iConfig) {

  isData = true;

  jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSource");
  muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSource");
  eleSrc_ = iConfig.getParameter<edm::InputTag>("eleSource");
  muonCSSrc_ = iConfig.getParameter<edm::InputTag>("muonCSSource");

  muonProductIDSrc_ = iConfig.getParameter<edm::InputTag>("muonProductIDSrc");
  muonKeySrc_ = iConfig.getParameter<edm::InputTag>("muonKeySrc");
  eleProductIDSrc_ = iConfig.getParameter<edm::InputTag>("eleProductIDSrc");
  eleKeySrc_ = iConfig.getParameter<edm::InputTag>("eleKeySrc");

  doGenLevel_ = iConfig.getParameter<bool>("doGenLevel");
  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");
  genJetMinPt_ = iConfig.getParameter<double>("genJetMinPt");
  genJetMaxEta_ = iConfig.getParameter<double>("genJetMaxEta");
  deltaR_ = iConfig.getParameter<double>("deltaR");
// deltaRcon_ can be very small, since the (eta, phi) between
// the matched constituent and the leptons (ele or muon) are
// very close (almost the same). For a p4 direct comparison,
// the only problem is that somehow ele has a bit large difference
// between the constituent's pt. While for muon its pt is 
// almost the same as the constituent (matched).
// In general, requiring a very tight deltRcon (~0.01) is safe enough!
  deltaRcon_ = iConfig.getParameter<double>("deltaRcon");
  deltaRtighterForEle_ = iConfig.getParameter<double>("deltaRtighterForEle");

  recoJetMinPt_ = iConfig.getParameter<double>("recoJetMinPt");
  recoJetMaxEta_ = iConfig.getParameter<double>("recoJetMaxEta");

  debug_ = iConfig.getParameter<bool>("debug");

  verbose_ = iConfig.getParameter<bool>("verbose");

  useEle_ = iConfig.getParameter<bool>("useEle");

  useDeltaR_ = iConfig.getParameter<bool>("useDeltaR");
 
  noRemovalOfBigJetMuonPtDiff_ = iConfig.getUntrackedParameter<bool>("noRemovalOfBigJetMuonPtDiff", false);
  diffJetMuonPt_ = iConfig.getUntrackedParameter<double>("diffJetMuonPt", 30);

  produces<std::vector<pat::Jet> >("");
// size same as the input muons
// content is the index of the jet
  produces<std::vector<int> >("bigPtDiff");

  bTagKeyString_ = iConfig.getUntrackedParameter<std::string>("bTagKeyString", "combinedSecondaryVertexBJetTags");

  if( doGenLevel_ ){
     produces<std::vector<reco::GenJet> >("cleanGenJets");
  }

  _totCleanedMuonJetCnt =0; _totCleanedEleJetCnt =0;
  _totInputMuonSizeCnt =0; _totInputEleSizeCnt=0;

}


cleanOverlappingLeptonsJets::~cleanOverlappingLeptonsJets() {

   std::cout<<"\n"<<jetSrc_.label().c_str()<<"_pt"<<recoJetMinPt_<<"_eta"<<recoJetMaxEta_<<"  _totCleanedMuonJetCnt : "<<_totCleanedMuonJetCnt<<"  _totInputMuonSizeCnt : "<<_totInputMuonSizeCnt<<std::endl; 
   std::cout<<"  _totCleanedEleJetCnt : "<<_totCleanedEleJetCnt<<"  _totInputEleSizeCnt : "<<_totInputEleSizeCnt<<std::endl<<std::endl;

}


void cleanOverlappingLeptonsJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData = false;

  // read in the jets
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_, jets);
  
  edm::Handle<edm::View<reco::Candidate> > muons;
  iEvent.getByLabel(muonSrc_, muons);

  edm::Handle<edm::View<reco::Candidate> > muonsCS;
  iEvent.getByLabel(muonCSSrc_, muonsCS);

  edm::Handle<edm::View<reco::Candidate> > eles;
  iEvent.getByLabel(eleSrc_, eles);

  edm::Handle<std::vector<edm::ProductID> > muonsProductID;
  iEvent.getByLabel(muonProductIDSrc_, muonsProductID);

  edm::Handle<std::vector<key_type> > muonsKey;
  iEvent.getByLabel(muonKeySrc_, muonsKey);

  edm::Handle<std::vector<edm::ProductID> > elesProductID;
  iEvent.getByLabel(eleProductIDSrc_, elesProductID);

  edm::Handle<std::vector<key_type> > elesKey;
  iEvent.getByLabel(eleKeySrc_, elesKey);

  _totInputMuonSizeCnt += (int)muons->size();
  if( eles.isValid() ) _totInputEleSizeCnt += (int)eles->size();

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());

  std::auto_ptr<std::vector<int> > bigPtDiffPtr(new std::vector<int>());
  int muonSize = (int)muons->size();
// initializing the default to be -1
  for(int im=0; im<muonSize; im++) bigPtDiffPtr->push_back(-1);

//  reco::CandidatePtr leptonOutPtr;
  reco::CandidatePtr jetOutPtr;

  for (edm::View<pat::Jet>::const_iterator j = jets->begin(); j != jets->end(); ++j) {

    double jetEta = j->eta(), jetPhi = j->phi(), jetPt = j->pt();
 
    if( recoJetMinPt_ != -1 && jetPt <= recoJetMinPt_ ) continue;
    if( recoJetMaxEta_ != -1 && fabs(jetEta) >= recoJetMaxEta_ ) continue;

    const std::vector<reco::PFCandidatePtr> & constituents = j->getPFConstituents();
    const unsigned int numConstituents = constituents.size();

    int associatedMuon = -1;
    int isDRoverlappedMuonIdx =-1;

    TVector3 jetVec; jetVec.SetPtEtaPhi(jetPt, jetEta, jetPhi);
    TVector3 muonVec; 

    double minMuonDR = 999;
    for (edm::View<reco::Candidate>::const_iterator m = muons->begin(); m != muons->end(); ++m) {

       double muonEta = m->eta(), muonPhi = m->phi(), muonPt = m->pt();

       int muonPos = std::distance(muons->begin(), m);

       const double dR = reco::deltaR(jetEta, jetPhi, muonEta, muonPhi);
       if( dR < deltaR_ ){ isDRoverlappedMuonIdx = muonPos; }
       if( minMuonDR > dR ){ minMuonDR = dR; }

       edm::ProductID muonProductID;
       key_type muonKey = 0;
       if( !useDeltaR_ ){
          muonProductID = (*muonsProductID)[muonPos];
          muonKey = (*muonsKey)[muonPos];
       }

       double mindRmuonCon = 999.; int selConIdx = -1;
       for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
          const reco::PFCandidatePtr& constituent = constituents[iCon];
          if( !useDeltaR_ ){
             if( !searchToPFbase(iEvent, (reco::CandidatePtr)constituent, jetOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for jet?!"<<std::endl<<std::endl; }
             if( muonProductID == jetOutPtr.id() && muonKey == jetOutPtr.key() ){
                associatedMuon = muonPos;
                break;
             }
          }else{
             if( constituent->charge() != m->charge() ) continue;
             const double dRmuonCon = reco::deltaR(constituent->eta(), constituent->phi(), muonEta, muonPhi);
             if( mindRmuonCon > dRmuonCon ){
                mindRmuonCon = dRmuonCon;
                selConIdx = (int)iCon;
             }
          }
       }
       if( useDeltaR_ ){
          if( mindRmuonCon < deltaRcon_ ){
             associatedMuon = muonPos;
             if( debug_ ){
                const reco::PFCandidatePtr& constituent = constituents[selConIdx];
                math::XYZTLorentzVector infoMuonp4 = (*muons)[associatedMuon].p4();
                std::cout<<"muon -->  pt : "<<infoMuonp4.pt()<<"  eta : "<<infoMuonp4.eta()<<"  phi : "<<infoMuonp4.phi()<<"  charge : "<<(*muons)[associatedMuon].charge()<<std::endl;
                std::cout<<"cons -->  pt : "<<constituent->pt()<<"  eta : "<<constituent->eta()<<"  phi : "<<constituent->phi()<<"  charge : "<<constituent->charge()<<std::endl;
             }
          }
       }
       // already found overlapping
       if( associatedMuon != -1 ){
          muonVec.SetPtEtaPhi(muonPt, muonEta, muonPhi); 
          break;
       }
    }

    if( (associatedMuon == -1 && isDRoverlappedMuonIdx !=-1) || (associatedMuon != -1 && isDRoverlappedMuonIdx ==-1) || (associatedMuon !=-1 && isDRoverlappedMuonIdx !=-1 && associatedMuon != isDRoverlappedMuonIdx) ){
       if( verbose_ ){
          std::cout<<"\nWARNING ... "<<jetSrc_.label().c_str()<<"_pt"<<recoJetMinPt_<<"_eta"<<recoJetMaxEta_<<"  cleanOverlappingLeptonsJets  DR NOT EQUAL key()/id() method for muon!?"<<std::endl;
          std::cout<<"\tassociatedMuon : "<<associatedMuon<<"  isDRoverlappedMuonIdx : "<<isDRoverlappedMuonIdx<<"  minMuonDR : "<<minMuonDR<<std::endl;
          math::XYZTLorentzVector infoMuonp4;
          if( associatedMuon !=-1 ) infoMuonp4 = (*muons)[associatedMuon].p4();
          if( isDRoverlappedMuonIdx != -1 ) infoMuonp4 = (*muons)[isDRoverlappedMuonIdx].p4();
          std::cout<<"\tmuon -->  pt : "<<infoMuonp4.pt()<<"  eta : "<<infoMuonp4.eta()<<"  phi : "<<infoMuonp4.phi()<<std::endl;
          std::cout<<"\t jet -->  pt : "<<jetPt<<"  eta : "<<jetEta<<"  phi : "<<jetPhi<<"  CSV : "<<j->bDiscriminator(bTagKeyString_.c_str())<<std::endl;
       }
    }

    if( associatedMuon != -1 ){
       TVector3 diffVec = jetVec-muonVec;
// Set the printout to be diffVec.Pt() at least 30 GeV to reduce printout messages!
       if( diffVec.Pt() >=diffJetMuonPt_ && diffVec.Pt() > 30 && verbose_ ){
          std::cout<<"\nINFO ... CSV : "<<j->bDiscriminator(bTagKeyString_.c_str())<<"  jetPt-muonPt : "<<diffVec.Pt()<<"  deltaPt/muonPt : "<<diffVec.Pt()/muonVec.Pt()<<"  jet -->  pt : "<<jetVec.Pt()<<"  eta : "<<jetVec.Eta()<<"  phi : "<<jetVec.Phi()<<"  muon -->  pt : "<<muonVec.Pt()<<"  eta : "<<muonVec.Eta()<<"  phi : "<<muonVec.Phi()<<std::endl;
       }
       _totCleanedMuonJetCnt++;
    }
    
    int associatedEle = -1;
    int isDRoverlappedEleIdx =-1;

    double minEleDR = 999;
    if( eles.isValid() ){
       for (edm::View<reco::Candidate>::const_iterator e = eles->begin(); e != eles->end(); ++e) {
   
          double eleEta = e->eta(), elePhi = e->phi();
       
          int elePos = std::distance(eles->begin(), e);
   
          const double dR = reco::deltaR(jetEta, jetPhi, eleEta, elePhi);
//          if( dR < deltaR_ ){ isDRoverlappedEleIdx = elePos; }
          if( minEleDR > dR ){
             minEleDR = dR; 
             if( minEleDR < deltaR_ ){ isDRoverlappedEleIdx = elePos; }
          }
  
          edm::ProductID eleProductID;
          key_type eleKey = 0;
          if( !useDeltaR_ ){
             eleProductID = (*elesProductID)[elePos];
             eleKey = (*elesKey)[elePos];
          } 

          double mindReleCon = 999.; int selConIdx = -1;
          for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
             const reco::PFCandidatePtr& constituent = constituents[iCon];
             if( !useDeltaR_ ){
                if( !searchToPFbase(iEvent, (reco::CandidatePtr)constituent, jetOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for jet?!"<<std::endl<<std::endl; }
                if( eleProductID == jetOutPtr.id() && eleKey == jetOutPtr.key() ){
                   associatedEle = elePos;
                   break;
                }
             }else{
                if( constituent->charge() != e->charge() ) continue;
                const double dReleCon = reco::deltaR(constituent->eta(), constituent->phi(), eleEta, elePhi);
                if( mindReleCon > dReleCon ){
                   mindReleCon = dReleCon;
                   selConIdx = (int)iCon;
                }
             }
          }
          if( useDeltaR_ ){
             if( mindReleCon < deltaRcon_ ){
                associatedEle = elePos;
                if( debug_ ){
                   const reco::PFCandidatePtr& constituent = constituents[selConIdx];
                   math::XYZTLorentzVector infoElep4 = (*eles)[associatedEle].p4();
                   std::cout<<" ele -->  pt : "<<infoElep4.pt()<<"  eta : "<<infoElep4.eta()<<"  phi : "<<infoElep4.phi()<<"  charge : "<<(*eles)[associatedEle].charge()<<std::endl;
                   std::cout<<"cons -->  pt : "<<constituent->pt()<<"  eta : "<<constituent->eta()<<"  phi : "<<constituent->phi()<<"  charge : "<<constituent->charge()<<std::endl;
                }
             }
          }
// This is somehow related to reco-PF mis-matching for the electron
// The deltaR matching between the reco electron and PF jet is good,
// but reco electron does NOT match to any constituent of the PF jet.
          if( minEleDR < deltaRtighterForEle_ && associatedEle == -1 ){
             associatedEle = isDRoverlappedEleIdx;
          }
          // already found overlapping
          if( associatedEle != -1 ) break;
       }
    }

    if( (associatedEle == -1 && isDRoverlappedEleIdx !=-1) || (associatedEle != -1 && isDRoverlappedEleIdx ==-1) || (associatedEle !=-1 && isDRoverlappedEleIdx !=-1 && associatedEle != isDRoverlappedEleIdx) ){
       if( verbose_ ){
          std::cout<<"\nWARNING ... "<<jetSrc_.label().c_str()<<"_pt"<<recoJetMinPt_<<"_eta"<<recoJetMaxEta_<<"  cleanOverlappingLeptonsJets  DR NOT EQUAL key()/id() method for ele!?"<<std::endl;
          std::cout<<"\tassociatedEle : "<<associatedEle<<"  isDRoverlappedEleIdx : "<<isDRoverlappedEleIdx<<"  minEleDR : "<<minEleDR<<std::endl;
          math::XYZTLorentzVector infoElep4;
          if( associatedEle !=-1 ) infoElep4 = (*eles)[associatedEle].p4();
          if( isDRoverlappedEleIdx != -1 ) infoElep4 = (*eles)[isDRoverlappedEleIdx].p4();
          std::cout<<"\telectron -->  pt : "<<infoElep4.pt()<<"  eta : "<<infoElep4.eta()<<"  phi : "<<infoElep4.phi()<<std::endl;
          std::cout<<"\t jet -->  pt : "<<jetPt<<"  eta : "<<jetEta<<"  phi : "<<jetPhi<<"  CSV : "<<j->bDiscriminator(bTagKeyString_.c_str())<<std::endl;
          if( isDRoverlappedEleIdx != -1 ){
             std::cout<<"\t   conts : ";
             for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
                const reco::PFCandidatePtr& constituent = constituents[iCon];
                std::cout<<"  "<<constituent->particleId();
             }
             std::cout<<std::endl;
          }
       }
    }
    
    if( associatedEle != -1 ) _totCleanedEleJetCnt++;
    
// Only use key()/id() method. Keep WARNING of DR versus key()/id() in order to find problems of code migration which causes
// the searchToPFbase method NOT working well, i.e., large amount of WARNING message.
    if( associatedMuon == -1 && associatedEle == -1 ) prod->push_back(pat::Jet(*j));

    TVector3 diffVec = jetVec-muonVec;
    int isFirstCSmuon = 0;
    if( !muonsCS->empty() ){
       if(
            fabs( (*muonsCS)[0].px() - muonVec.Px() ) < 1e-6
         && fabs( (*muonsCS)[0].py() - muonVec.Py() ) < 1e-6
         && fabs( (*muonsCS)[0].pz() - muonVec.Pz() ) < 1e-6
       ){
          isFirstCSmuon = 1;
       }
    }

    if( associatedMuon != -1 && diffVec.Pt() >=diffJetMuonPt_ && noRemovalOfBigJetMuonPtDiff_ == true && isFirstCSmuon ){
       if( (*bigPtDiffPtr)[associatedMuon] == -1 ){
          (*bigPtDiffPtr)[associatedMuon] = (int)prod->size();
       }else{
          std::cout<<"\nWARNING ... multiple jets matching to the same muon?!"<<std::endl;
       }
       prod->push_back(pat::Jet(*j));
    }
  }

  if( doGenLevel_ ){
     std::auto_ptr<std::vector<reco::GenJet> > genColl(new std::vector<reco::GenJet>());

     loadGenParticles(iEvent);
     loadGenJets(iEvent);

     int maxLepNeedCleaned = 0;

     int signW = 0; std::vector<TLorentzVector> Wvec;
     const std::vector<TLorentzVector> WJetsAccVec = searchGenSigChnForAcc(&signW, &Wvec);

     int signWele = 0; std::vector<TLorentzVector> WeleVec;
     const std::vector<TLorentzVector> WJetsEleAccVec = searchGenEleSigChnForAcc(&signWele, &WeleVec);

     if( !WJetsAccVec.empty() ) maxLepNeedCleaned++;
     if( !WJetsEleAccVec.empty() ) maxLepNeedCleaned++;
    
     int numCleanedJets =0;
     for(unsigned int ig=0; ig<genJets->size(); ig++){
        const reco::GenJet genJet = (*genJets)[ig];
        double jetPt = genJet.pt(), jetEta = genJet.eta(), jetPhi = genJet.phi();
        if( jetPt < genJetMinPt_ || abs(jetEta) > genJetMaxEta_ ) continue;
        if( !WJetsAccVec.empty() ){
           double muEta = WJetsAccVec.front().Eta(), muPhi = WJetsAccVec.front().Phi();
           const double dR = reco::deltaR(jetEta, jetPhi, muEta, muPhi);
           if( debug_ ){
              std::cout<<"DEBUG doGenLevel ... jetEta : "<<jetEta<<"  jetPhi : "<<jetPhi<<"  muEta : "<<muEta<<"  muPhi : "<<muPhi<<"  dR : "<<dR<<std::endl;
           }
           if( dR < deltaR_ ){ numCleanedJets++; continue; }
        }
        if( useEle_ && !WJetsEleAccVec.empty() ){
           double eleEta = WJetsEleAccVec.front().Eta(), elePhi = WJetsEleAccVec.front().Phi();
           const double dR = reco::deltaR(jetEta, jetPhi, eleEta, elePhi);
           if( debug_ ){
              std::cout<<"DEBUG doGenLevel ... jetEta : "<<jetEta<<"  jetPhi : "<<jetPhi<<"  eleEta : "<<eleEta<<"  elePhi : "<<elePhi<<"  dR : "<<dR<<std::endl;
           }
           if( dR < deltaR_ ){ numCleanedJets++; continue; }
        }
        genColl->push_back(genJet);
     }
     if( numCleanedJets >maxLepNeedCleaned ){ std::cout<<"WARNING doGenLevel ... cleanOverlappingLeptonsJets  numCleanedJets : "<<numCleanedJets<<"  &&  maxLepNeedCleaned : "<<maxLepNeedCleaned<<" ?!"<<std::endl; }
     if( debug_ ){
        std::cout<<"DEBUG doGenLevel ... WJets size : "<<WJetsAccVec.size()<<"  genJets size : "<<genJets->size()<<"  numCleanedJets : "<<numCleanedJets<<std::endl;
     }
     iEvent.put(genColl, "cleanGenJets");
  }

  // store in the event
  iEvent.put(prod);
  iEvent.put(bigPtDiffPtr, "bigPtDiff");
}


 const std::vector<TLorentzVector> & cleanOverlappingLeptonsJets::searchGenEleSigChnForAcc(int *signWptr, std::vector<TLorentzVector> *bosonVecPtr){
          
    static std::vector<TLorentzVector> eleCached; eleCached.clear();
    if( bosonVecPtr ) bosonVecPtr->clear();
    
    if( signWptr ) *signWptr = 0; // default value
          
    for(unsigned int ig=0; ig<genParticles->size(); ig++){
             
       const reco::GenParticle& gen = genParticles->at(ig);
             
       if( abs(gen.pdgId()) == 24 ){ // W+ : 24   W- : -24  (W+->mu+ nu_mu)
          int signW = gen.pdgId()/abs(gen.pdgId()); if( signWptr ) *signWptr = signW;
          unsigned int numDaug = gen.numberOfDaughters();
          int numEleMatched =0, numNeutrinoMatched =0;
          for(unsigned int id=0; id<numDaug; id++){
             int daupdgId = gen.daughter(id)->pdgId();
             int status = gen.daughter(id)->status(); // status : 3 (stable particle?)
             if( status ==3 && (daupdgId == -signW*11 /*e+ : -11  e- : 11*/ || daupdgId == signW*12 /*nu_e : 12  anti-nu_e : -12*/) ){
                if( daupdgId == -signW*11 ){
                   TLorentzVector daug; daug.SetPxPyPzE(gen.daughter(id)->px(), gen.daughter(id)->py(), gen.daughter(id)->pz(), gen.daughter(id)->energy());
                   eleCached.push_back( daug );
                   numEleMatched ++;
                }
                if( daupdgId == signW*12 ) numNeutrinoMatched ++;
             }
          }
          if( numEleMatched ==1 && numNeutrinoMatched == 1 ){
             TLorentzVector boson; boson.SetPxPyPzE(gen.px(), gen.py(), gen.pz(), gen.energy());
             if( bosonVecPtr ) bosonVecPtr->push_back(boson);
             return eleCached;
          }
       }
    }

    eleCached.clear();
    return eleCached;

 }


 const std::vector<TLorentzVector> & cleanOverlappingLeptonsJets::searchGenSigChnForAcc(int *signWptr, std::vector<TLorentzVector> *bosonVecPtr){

    static std::vector<TLorentzVector> muonCached; muonCached.clear();
    if( bosonVecPtr ) bosonVecPtr->clear();

    if( signWptr ) *signWptr = 0; // default value

    for(unsigned int ig=0; ig<genParticles->size(); ig++){

       const reco::GenParticle& gen = genParticles->at(ig);

       if( abs(gen.pdgId()) == 24 ){ // W+ : 24   W- : -24  (W+->mu+ nu_mu)
          int signW = gen.pdgId()/abs(gen.pdgId()); if( signWptr ) *signWptr = signW;
          unsigned int numDaug = gen.numberOfDaughters();
          int numMuonMatched =0, numNeutrinoMatched =0;
          for(unsigned int id=0; id<numDaug; id++){
             int daupdgId = gen.daughter(id)->pdgId();
             int status = gen.daughter(id)->status(); // status : 3 (stable particle?)
             if( status ==3 && (daupdgId == -signW*13 /*mu+ : -13  mu- : 13*/ || daupdgId == signW*14 /*nu_mu : 14  anti-nu_mu : -14*/) ){
                if( daupdgId == -signW*13 ){
                   TLorentzVector daug; daug.SetPxPyPzE(gen.daughter(id)->px(), gen.daughter(id)->py(), gen.daughter(id)->pz(), gen.daughter(id)->energy());
                   muonCached.push_back( daug );
                   numMuonMatched ++;
                }
                if( daupdgId == signW*14 ) numNeutrinoMatched ++;
             }
          }
          if( numMuonMatched ==1 && numNeutrinoMatched == 1 ){
             TLorentzVector boson; boson.SetPxPyPzE(gen.px(), gen.py(), gen.pz(), gen.energy());
             if( bosonVecPtr ) bosonVecPtr->push_back(boson);
             return muonCached;
          }
       }
    }

    muonCached.clear();
    return muonCached;

 }

 void cleanOverlappingLeptonsJets::loadGenParticles(const edm::Event& iEvent){
    iEvent.getByLabel(genParticleSrc_, genParticles);
 }
 void cleanOverlappingLeptonsJets::loadGenJets(const edm::Event& iEvent){
    iEvent.getByLabel(genJetSrc_, genJets);
 }

 int cleanOverlappingLeptonsJets::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
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

#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(cleanOverlappingLeptonsJets);
