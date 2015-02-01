#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TTree.h"
#include "TLorentzVector.h"

typedef unsigned int size;

class studyHadTauDecayInGenLevelFilter : public edm::EDFilter {

  public:

    explicit studyHadTauDecayInGenLevelFilter(const edm::ParameterSet & iConfig);
    ~studyHadTauDecayInGenLevelFilter();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;

    std::vector<int> tauDecays(std::vector<std::vector<int> > &evtTauDecayStatusVec);
    std::vector<int> lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec);

    bool find_mother(const reco::Candidate* genp, const int motherID);
    bool find_mother(const reco::Candidate* genp, const reco::Candidate &target);

    bool WTauHadFilter_, WTauMuFilter_, WTauEleFilter_, OtherTauHadFilter_, OtherTauMuFilter_, OtherTauEleFilter_;

// In this case, we can consider the WTau as BosonTau which includes either W or Z
    bool doZfilter_;

// 1 : reject both electron and muon
// -1 : reject only muon
    int OtherWLepRejection_;

    bool doFillTree_;
    void setTreeDefaultVars();
    TTree *auxTree;

    edm::InputTag evtWeightInput_;
    double evtWeight_TR;
    edm::Handle<double> evtWeight_;

    unsigned int run, event, lumi; bool isData;
    edm::InputTag vtxSrc_;
    unsigned int vtxSize;
    edm::Handle<edm::View<reco::Vertex> > vertices;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    int npv; double avg_npv; int nm1, n0, np1; double tru_npv;
    virtual void loadGenInfo(const edm::Event& iEvent);

    std::vector<TLorentzVector> *hadTauLVec_TR;
    std::vector<std::vector<TLorentzVector> > *hadTauDausLVec_TR;
    std::vector<std::vector<double> > *hadTauDausChargeVec_TR;
    std::vector<std::vector<int> > *hadTauDausPDGIdVec_TR;

};


studyHadTauDecayInGenLevelFilter::studyHadTauDecayInGenLevelFilter(const edm::ParameterSet & iConfig) {

  isData = true;

  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");

  WTauHadFilter_ = iConfig.getParameter<bool>("WTauHadFilter");
  WTauMuFilter_ = iConfig.getParameter<bool>("WTauMuFilter");
  WTauEleFilter_ = iConfig.getParameter<bool>("WTauEleFilter");

  OtherTauHadFilter_ = iConfig.getParameter<bool>("OtherTauHadFilter");
  OtherTauMuFilter_ = iConfig.getParameter<bool>("OtherTauMuFilter");
  OtherTauEleFilter_ = iConfig.getParameter<bool>("OtherTauEleFilter");

  doZfilter_ = iConfig.getParameter<bool>("doZfilter");

  OtherWLepRejection_ = iConfig.getParameter<int>("OtherWLepRejection");

  doFillTree_ = iConfig.getParameter<bool>("doFillTree");

  evtWeightInput_ = iConfig.getParameter<edm::InputTag>("evtWeightInput");

  vtxSrc_ = iConfig.getParameter<edm::InputTag>("vtxSrc");

  produces<std::vector<int> >("pickedHadTauGenIdx");
  produces<std::vector<math::XYZTLorentzVector> >("pickedHadTauGenP4");

  evtWeight_TR = 1.0;

  hadTauLVec_TR = new std::vector<TLorentzVector>; hadTauDausLVec_TR = new std::vector<std::vector<TLorentzVector> >; 
  hadTauDausChargeVec_TR = new std::vector<std::vector<double> >; hadTauDausPDGIdVec_TR = new std::vector<std::vector<int> >;

  edm::Service<TFileService> fs;
  if( doFillTree_ ){
     auxTree = fs->make<TTree>("AUX", "aux info");
     auxTree->Branch("run", &run, "run/I");
     auxTree->Branch("event", &event, "event/I");
     auxTree->Branch("lumi", &lumi, "lumi/I");
     auxTree->Branch("avg_npv", &avg_npv, "avg_npv/D");
     auxTree->Branch("nm1", &nm1, "nm1/I");
     auxTree->Branch("n0", &n0, "n0/I");
     auxTree->Branch("np1", &np1, "np1/I");
     auxTree->Branch("tru_npv", &tru_npv, "tru_npv/D");
     auxTree->Branch("vtxSize", &vtxSize, "vtxSize/I");
     auxTree->Branch("evtWeight", &evtWeight_TR, "evtWeight/D");
     auxTree->Branch("hadTauLVec", "std::vector<TLorentzVector>", &hadTauLVec_TR, 32000, 0);
     auxTree->Branch("hadTauDausLVec", "std::vector<std::vector<TLorentzVector> >", &hadTauDausLVec_TR, 32000, 0);
     auxTree->Branch("hadTauDausChargeVec", "std::vector<std::vector<double> >", &hadTauDausChargeVec_TR, 32000, 0);
     auxTree->Branch("hadTauDausPDGIdVec", "std::vector<std::vector<int> >", &hadTauDausPDGIdVec_TR, 32000, 0);
  }

}


studyHadTauDecayInGenLevelFilter::~studyHadTauDecayInGenLevelFilter() {
}


bool studyHadTauDecayInGenLevelFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;

  setTreeDefaultVars();

  loadEventInfo(iEvent, iSetup);

  loadGenInfo(iEvent);

  iEvent.getByLabel(genParticleSrc_, genParticles);
  iEvent.getByLabel(genJetSrc_, genJets);

  std::vector<std::vector<int> > evtTauDecayStatusVec;
  std::vector<int> tauGenIdxVec = tauDecays(evtTauDecayStatusVec);

  std::vector< std::vector<int> > evtLepDecayStatusVec;
  std::vector<int> lepGenIdxVec = lepDecays(evtLepDecayStatusVec);

  std::auto_ptr<std::vector<int> > pickedHadTauGenIdxPtr(new std::vector<int> );
  std::auto_ptr<std::vector<math::XYZTLorentzVector> > pickedHadTauGenP4Ptr(new std::vector<math::XYZTLorentzVector> );

  bool pass = false;

  if( tauGenIdxVec.empty() ) return pass;

  size nGenTau = tauGenIdxVec.size();

  bool isWTauHadDecay = false, isWTauMuDecay = false, isWTauEleDecay = false, isOtherTauHadDecay = false, isOtherTauMuDecay = false, isOtherTauEleDecay = false;

  std::vector<int> pickedHadTauGenIdx, pickedTauMuGenIdx, pickedTauEleGenIdx;

  for(size ig=0; ig<nGenTau; ig++){

     bool isWTauDecay = false, isTauMuDecay = false, isTauEleDecay = false, isTauLeptonicDecay = false;

     if( evtTauDecayStatusVec[ig][0] ) isWTauDecay = true;
     if( evtTauDecayStatusVec[ig][1] ) isTauMuDecay = true;
     if( evtTauDecayStatusVec[ig][2] ) isTauEleDecay = true;
     if( evtTauDecayStatusVec[ig][3] ) isTauLeptonicDecay = true;

     if( isWTauDecay ){
        if( !isTauLeptonicDecay ){ isWTauHadDecay = true; pickedHadTauGenIdx.push_back(tauGenIdxVec[ig]); }
        if( isTauLeptonicDecay && isTauMuDecay ){ isWTauMuDecay = true; pickedTauMuGenIdx.push_back(tauGenIdxVec[ig]); }
        if( isTauLeptonicDecay && isTauEleDecay ){ isWTauEleDecay = true; pickedTauEleGenIdx.push_back(tauGenIdxVec[ig]); }
     }else{
        if( !isTauLeptonicDecay ) isOtherTauHadDecay = true;
        if( isTauLeptonicDecay && isTauMuDecay ) isOtherTauMuDecay = true;
        if( isTauLeptonicDecay && isTauEleDecay ) isOtherTauEleDecay = true;
     }

  }

// Filling the tree variables
  for(size ih=0; ih<pickedHadTauGenIdx.size(); ih++){
     const reco::GenParticle & genTau = genParticles->at(pickedHadTauGenIdx[ih]);
     std::vector<TLorentzVector> genTauDausLVec;
     std::vector<double> genTauDausChargeVec;
     std::vector<int> genTauDausPDGIdVec;

     for(unsigned int ig=0; ig<genParticles->size(); ig++){
        const reco::GenParticle & genPtcls = genParticles->at(ig);

        if( genPtcls.status() != 1 ) continue;

        if( find_mother( &genPtcls, genTau ) ){
           TLorentzVector perLVec;
           perLVec.SetPtEtaPhiE( genPtcls.pt(), genPtcls.eta(), genPtcls.phi(), genPtcls.energy() );

           genTauDausLVec.push_back(perLVec);
           genTauDausChargeVec.push_back( genPtcls.charge() ); genTauDausPDGIdVec.push_back( genPtcls.pdgId() );
        }
     }

     TLorentzVector genTauLVec;
     genTauLVec.SetPtEtaPhiE( genTau.pt(), genTau.eta(), genTau.phi(), genTau.energy() );

     hadTauLVec_TR->push_back(genTauLVec); hadTauDausLVec_TR->push_back(genTauDausLVec);
     hadTauDausChargeVec_TR->push_back(genTauDausChargeVec); hadTauDausPDGIdVec_TR->push_back(genTauDausPDGIdVec);
  }

  if( doFillTree_ ){
     evtWeight_TR = (*evtWeight_);
     auxTree->Fill();
  }

  size nleps = lepGenIdxVec.size();

  std::vector<int> pickedWLepGenIdx, pickedWMuGenIdx, pickedWEleGenIdx;

  bool isLepDecay = false, isWLepDecay = false, isWMuDecay = false, isWEleDecay = false, isOtherLepDecay = false, isWTauLepDecay = false;

  for(size il=0; il<nleps; il++){

     bool isMu = false, isEle = false; // only muon and electron (tau is treated specially)

     const reco::GenParticle& gen = genParticles->at(lepGenIdxVec[il]);
     if( abs(gen.pdgId()) == 13 ) isMu = true;
     if( abs(gen.pdgId()) == 11 ) isEle = true;

     if( evtLepDecayStatusVec[il][0] ) isLepDecay = true;
     if( evtLepDecayStatusVec[il][1] ) { 
        isWLepDecay = true;
        if( isMu ){ isWMuDecay = true; pickedWLepGenIdx.push_back( lepGenIdxVec[il] ); pickedWMuGenIdx.push_back( lepGenIdxVec[il] ); }
        if( isEle ){ isWEleDecay = true; pickedWLepGenIdx.push_back( lepGenIdxVec[il] ); pickedWEleGenIdx.push_back( lepGenIdxVec[il] ); }
     }
     if( evtLepDecayStatusVec[il][2] ){
        isWTauLepDecay = true;
        if( isMu && !isWTauMuDecay ){ std::cout<<"ERROR ... studyHadTauDecayInGenLevelFilter  mis-matching between lepDecays and tauDecays for isWTauMuDecay : "<<isWTauMuDecay<<std::endl; }
        if( isEle && !isWTauEleDecay ){ std::cout<<"ERROR ... studyHadTauDecayInGenLevelFilter  mis-matching between lepDecays and tauDecays for isWTauEleDecay : "<<isWTauEleDecay<<std::endl; }
     }
     if( !isWLepDecay ) isOtherLepDecay = true;
  }

  if( isLepDecay ){/*empty to avoid an error*/}
  if( isWMuDecay ){/*empty to avoid an error*/}
  if( isWEleDecay ){/*empty to avoid an error*/}
  if( isOtherLepDecay ){/*empty to avoid an error*/}
  if( isWTauLepDecay ){/*empty to avoid an error*/}

  std::vector<int> pickedCachedGenIdx;
  if( OtherWLepRejection_ ==1 ) pickedCachedGenIdx = pickedWLepGenIdx;
  if( OtherWLepRejection_ ==-1 ) pickedCachedGenIdx = pickedWMuGenIdx;

  if( OtherWLepRejection_ ){
     int foundMotherOverlap =0;

     for(unsigned int ig=0; ig<genParticles->size(); ig++){

        const reco::GenParticle& gen = genParticles->at(ig);
        if( ! ( abs( gen.pdgId() ) == 24 || ( doZfilter_ && abs( gen.pdgId() ) == 23 ) ) ) continue;

        int isHadTauFromThisBoson =0, isWLepFromThisBoson =0, isTauMuFromThisBoson =0, isTauEleFromThisBoson =0;

        for(size it=0; it<pickedHadTauGenIdx.size(); it++){
           int itau = pickedHadTauGenIdx[it];
           const reco::GenParticle& genTau = genParticles->at(itau);
           bool isFromThisBoson = find_mother( &genTau, gen);
           if( isFromThisBoson ) isHadTauFromThisBoson ++;
        }

        for(size it=0; it<pickedTauMuGenIdx.size(); it++){
           int itau = pickedTauMuGenIdx[it];
           const reco::GenParticle& genTau = genParticles->at(itau);
           bool isFromThisBoson = find_mother( &genTau, gen);
           if( isFromThisBoson ) isTauMuFromThisBoson ++;
        }

        for(size it=0; it<pickedTauEleGenIdx.size(); it++){
           int itau = pickedTauEleGenIdx[it];
           const reco::GenParticle& genTau = genParticles->at(itau);
           bool isFromThisBoson = find_mother( &genTau, gen);
           if( isFromThisBoson ) isTauEleFromThisBoson ++;
        }

        for(size il=0; il<pickedCachedGenIdx.size(); il++){
           int ilep = pickedCachedGenIdx[il];
           const reco::GenParticle& genLep = genParticles->at(ilep);
           bool isFromThisBoson = find_mother( &genLep, gen);
           if( isFromThisBoson ) isWLepFromThisBoson ++;
        }

        if( isHadTauFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... studyHadTauDecayInGenLevelFilter ==> both W->tau(had) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauHadFilter_ ) foundMotherOverlap ++;
        }

        if( isTauMuFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... studyHadTauDecayInGenLevelFilter ==> both W->tau(->mu) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauMuFilter_ ) foundMotherOverlap ++;
        }

        if( isTauEleFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... studyHadTauDecayInGenLevelFilter ==> both W->tau(->e) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauEleFilter_ ) foundMotherOverlap ++;
        }

     }
     if( !foundMotherOverlap && !pickedHadTauGenIdx.empty() && !pickedCachedGenIdx.empty() && WTauHadFilter_ ){
        isWTauHadDecay = false;
     }
     if( !foundMotherOverlap && !pickedTauMuGenIdx.empty() && !pickedCachedGenIdx.empty() && WTauMuFilter_ ){
        isWTauMuDecay = false;
     }
     if( !foundMotherOverlap && !pickedTauEleGenIdx.empty() && !pickedCachedGenIdx.empty() && WTauEleFilter_ ){
        isWTauEleDecay = false;
     }
  }

// If we find multiple tau's. Priority is for the WTau decay, then others.
// Then priority is for the leptonic decay, assuming it'll be removed by our lepton veto.
//  if( isWTauMuDecay || isWTauEleDecay ){ isOtherTauMuDecay = false; isOtherTauEleDecay = false; isWTauHadDecay = false; isOtherTauHadDecay = false; }
//  if( isWTauHadDecay ){ isOtherTauHadDecay = false; isOtherTauMuDecay = false; isOtherTauEleDecay = false; }

  if( WTauHadFilter_ && isWTauHadDecay ){ 
     pass = true;
     for(int it=0; it<(int)pickedHadTauGenIdx.size(); it++){
        int itau = pickedHadTauGenIdx[it];
        const reco::GenParticle& genTau = genParticles->at(itau);
        math::XYZTLorentzVector tauP4 = genTau.p4();

        pickedHadTauGenIdxPtr->push_back(itau);
        pickedHadTauGenP4Ptr->push_back(tauP4);
     }
  }
  if( WTauMuFilter_ && isWTauMuDecay ) pass = true;
  if( WTauEleFilter_ && isWTauEleDecay ) pass = true;

  if( OtherTauHadFilter_ && isOtherTauHadDecay ) pass = true;
  if( OtherTauMuFilter_ && isOtherTauMuDecay ) pass = true;
  if( OtherTauEleFilter_ && isOtherTauEleDecay ) pass = true;

  iEvent.put(pickedHadTauGenIdxPtr, "pickedHadTauGenIdx");
  iEvent.put(pickedHadTauGenP4Ptr, "pickedHadTauGenP4");

  return pass;

}

// Concentrate to tau with status 2
// Two cases
// W(3) -> tau(3) ->  tau (2) -> hadronic and/or leptonic ( directly target leptonic (all status 1)-- easier )
// B(2) -> tau(2) -> xxxx (same) 
std::vector<int> studyHadTauDecayInGenLevelFilter::tauDecays(std::vector<std::vector<int> > &evtTauDecayStatusVec){

   using namespace edm;
   using namespace std;

   std::vector<int> tauGenIdxVec;
   evtTauDecayStatusVec.clear();

   for(unsigned int ig=0; ig<genParticles->size(); ig++){

      int isTauLeptonicDecay = 0, isTauMuDecay = 0, isTauEleDecay = 0;
      int isWTauDecay = 0;

      const reco::GenParticle& gen = genParticles->at(ig);

      if( abs(gen.pdgId()) == 15 && gen.status() == 2 ){

         size numDaug = gen.numberOfDaughters();

// If any of the tau daughter is tau itself, this means we have not reached the final tau yet!
         bool isFinalTau = true;
         for(size id=0; id<numDaug; id++){
            if( gen.daughter(id)->pdgId() == gen.pdgId() && gen.daughter(id)->status() ==2 ){
               isFinalTau = false;
            }
         }
         if( !isFinalTau ) continue;

         int signTau = gen.pdgId()/abs(gen.pdgId());
         int numEleMatched =0, numEleNeutMatched =0;
         int numMuonMatched =0, numMuonNeutMatched =0;
         for(size id=0; id<numDaug; id++){
            int daupdgId = gen.daughter(id)->pdgId();
            int status = gen.daughter(id)->status();
            if( status ==1 && (daupdgId == signTau*11 /*e+ : -11  e- : 11*/ || daupdgId == -signTau*12 /*nu_e : 12  anti-nu_tau : -12*/) ){
               if( daupdgId == signTau*11 ) numEleMatched++;
               if( daupdgId == -signTau*12 ) numEleNeutMatched ++;
            }
            if( status ==1 && (daupdgId == signTau*13 /*mu+ : -13  mu- : 13*/ || daupdgId == -signTau*14 /*nu_mu : 14  anti-nu_tau : -14*/) ){
               if( daupdgId == signTau*13 ) numMuonMatched++;
               if( daupdgId == -signTau*14 ) numMuonNeutMatched ++;
            }
         }
         if( (numEleMatched + numEleNeutMatched) !=0 && (numMuonMatched + numMuonNeutMatched ) !=0 ){
            cout<<"WARNING ... a tau decays to both ele and muon ?!"<<endl;
            return tauGenIdxVec;
         }
         if( (numEleMatched != numEleNeutMatched) || (numMuonMatched != numMuonNeutMatched ) ){
            cout<<"WARNING ... tau decays into products violating leptonic number conservation ?!"<<endl;
            return tauGenIdxVec;
         }
         if( numEleMatched > 1 || numMuonMatched > 1 ){
            cout<<"WARNING ... a tau decays to multiple ele or muon ?!"<<endl;
            return tauGenIdxVec;
         }
         if( numEleMatched == 1 || numMuonMatched == 1 ){
            isTauLeptonicDecay =1;
            if( numMuonMatched ) isTauMuDecay = 1;
            if( numEleMatched ) isTauEleDecay = 1;
         }

         bool frombjet = find_mother( &gen, 5 );
         bool frombbarjet = find_mother( &gen, -5 );

         const reco::Candidate *momTau = &gen;
// If from b or bbar fragmentation, should be put to "other" category, i.e., not the WTau one which we really need
         if( gen.numberOfMothers() && !frombjet && !frombbarjet ){
           
            while( momTau->pdgId() == signTau *15 && momTau->status() == 2 && momTau->numberOfMothers() ){
               momTau = momTau->mother(0);
            }

            if( momTau->numberOfMothers() && momTau->pdgId() == signTau *15 && momTau->status() == 3 ){
               if( momTau->mother(0)->pdgId() == -1*signTau*24 ) isWTauDecay = 1;
               if( doZfilter_ && momTau->mother(0)->pdgId() == 23 ) isWTauDecay = 1;
            }
         }

         tauGenIdxVec.push_back(ig);

         vector<int> perDecayStatusVec;
         perDecayStatusVec.push_back( isWTauDecay ); perDecayStatusVec.push_back( isTauMuDecay ); perDecayStatusVec.push_back( isTauEleDecay ); perDecayStatusVec.push_back( isTauLeptonicDecay );
         evtTauDecayStatusVec.push_back(perDecayStatusVec);
      }
   }

   return tauGenIdxVec;
}

bool studyHadTauDecayInGenLevelFilter::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}

std::vector<int> studyHadTauDecayInGenLevelFilter::lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec){

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


bool studyHadTauDecayInGenLevelFilter::find_mother(const reco::Candidate* genp, const reco::Candidate &target){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->p4() == target.p4() && (genp->mother())->vertex() == target.vertex() && (genp->mother())->charge() == target.charge()) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), target);
    if (result) return true;
  }
  return false;
}


void studyHadTauDecayInGenLevelFilter::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){
// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   lumi = iEvent.luminosityBlock();

// Get vertices
   iEvent.getByLabel(vtxSrc_, vertices); vtxSize = vertices->size();

// Get event weight
   iEvent.getByLabel(evtWeightInput_, evtWeight_);
}


void studyHadTauDecayInGenLevelFilter::loadGenInfo(const edm::Event& iEvent){

// MC generate level related info
   if (!isData) {

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


void studyHadTauDecayInGenLevelFilter::setTreeDefaultVars(){
   evtWeight_TR = 1;

   hadTauLVec_TR->clear(); hadTauDausLVec_TR->clear();
   hadTauDausChargeVec_TR->clear(); hadTauDausPDGIdVec_TR->clear();

   npv = -1; avg_npv = -1; nm1 = -1; n0 = -1; np1 = -1; tru_npv = -1;
}


#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(studyHadTauDecayInGenLevelFilter);
