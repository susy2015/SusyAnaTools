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

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

typedef unsigned int size;

class leptonDecaysFilter : public edm::EDFilter {

  public:

    explicit leptonDecaysFilter(const edm::ParameterSet & iConfig);
    ~leptonDecaysFilter();

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
// WEleMuTauFilter_: W->e, W->mu & W->tau
// WEleMuTauLepFilter_: W->e, W->mu, W->tau->e & W->tau->mu
    bool WEleMuTauFilter_, WEleMuTaulepFilter_;

    bool WMuFilter_, WEleFilter_, OtherMuFilter_, OtherEleFilter_, MuAndEleFilter_, allOthersFilter_;

// In this case, we can consider the WTau as BosonTau which includes either W or Z
    bool doZfilter_;

    bool OtherWLepRejection_;

};


leptonDecaysFilter::leptonDecaysFilter(const edm::ParameterSet & iConfig) {

  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");

  WTauHadFilter_ = iConfig.getParameter<bool>("WTauHadFilter");
  WTauMuFilter_ = iConfig.getParameter<bool>("WTauMuFilter");
  WTauEleFilter_ = iConfig.getParameter<bool>("WTauEleFilter");

  OtherTauHadFilter_ = iConfig.getParameter<bool>("OtherTauHadFilter");
  OtherTauMuFilter_ = iConfig.getParameter<bool>("OtherTauMuFilter");
  OtherTauEleFilter_ = iConfig.getParameter<bool>("OtherTauEleFilter");

  WEleMuTauFilter_ = iConfig.getParameter<bool>("WEleMuTauFilter");
  WEleMuTaulepFilter_ = iConfig.getParameter<bool>("WEleMuTaulepFilter");

  WMuFilter_ = iConfig.getParameter<bool>("WMuFilter");
  WEleFilter_ = iConfig.getParameter<bool>("WEleFilter");

  OtherMuFilter_ = iConfig.getParameter<bool>("OtherMuFilter");
  OtherEleFilter_ = iConfig.getParameter<bool>("OtherEleFilter");

  MuAndEleFilter_ = iConfig.getParameter<bool>("MuAndEleFilter");

  allOthersFilter_ = iConfig.getParameter<bool>("allOthersFilter");

  doZfilter_ = iConfig.getParameter<bool>("doZfilter");

  OtherWLepRejection_ = iConfig.getParameter<bool>("OtherWLepRejection");

}


leptonDecaysFilter::~leptonDecaysFilter() {
}


bool leptonDecaysFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;

  iEvent.getByLabel(genParticleSrc_, genParticles);
  iEvent.getByLabel(genJetSrc_, genJets);

  std::vector<std::vector<int> > evtTauDecayStatusVec;
  std::vector<int> tauGenIdxVec = tauDecays(evtTauDecayStatusVec);

  std::vector< std::vector<int> > evtLepDecayStatusVec;
  std::vector<int> lepGenIdxVec = lepDecays(evtLepDecayStatusVec);

  bool pass = false;

//  if( tauGenIdxVec.empty() && lepGenIdxVec.empty() ) return pass;

  size nGenTau = tauGenIdxVec.size();

  bool isWTauHadDecay = false, isWTauMuDecay = false, isWTauEleDecay = false, isOtherTauHadDecay = false, isOtherTauMuDecay = false, isOtherTauEleDecay = false;
  int isWTauDecayCnt =0;

  std::vector<int> pickedHadTauGenIdx, pickedTauMuGenIdx, pickedTauEleGenIdx;

  for(size ig=0; ig<nGenTau; ig++){

     bool isWTauDecay = false, isTauMuDecay = false, isTauEleDecay = false, isTauLeptonicDecay = false;

     if( evtTauDecayStatusVec[ig][0] ) isWTauDecay = true;
     if( evtTauDecayStatusVec[ig][1] ) isTauMuDecay = true;
     if( evtTauDecayStatusVec[ig][2] ) isTauEleDecay = true;
     if( evtTauDecayStatusVec[ig][3] ) isTauLeptonicDecay = true;

     if( isWTauDecay ){
        isWTauDecayCnt ++;
        if( !isTauLeptonicDecay ){ isWTauHadDecay = true; pickedHadTauGenIdx.push_back(tauGenIdxVec[ig]); }
        if( isTauLeptonicDecay && isTauMuDecay ){ isWTauMuDecay = true; pickedTauMuGenIdx.push_back(tauGenIdxVec[ig]); }
        if( isTauLeptonicDecay && isTauEleDecay ){ isWTauEleDecay = true; pickedTauEleGenIdx.push_back(tauGenIdxVec[ig]); }
     }else{
        if( !isTauLeptonicDecay ) isOtherTauHadDecay = true;
        if( isTauLeptonicDecay && isTauMuDecay ) isOtherTauMuDecay = true;
        if( isTauLeptonicDecay && isTauEleDecay ) isOtherTauEleDecay = true;
     }

  }

  size nleps = lepGenIdxVec.size();

  std::vector<int> pickedWLepGenIdx;

  bool isLepDecay = false, isWLepDecay = false, isWMuDecay = false, isWEleDecay = false, isOtherLepDecay = false, isWTauLepDecay = false;
  bool isOtherMuDecay = false, isOtherEleDecay = false, isMuAndEleDecay = false, isMuDecay = false, isEleDecay = false;

  for(size il=0; il<nleps; il++){

     bool isMu = false, isEle = false; // only muon and electron (tau is treated specially)

     const reco::GenParticle& gen = genParticles->at(lepGenIdxVec[il]);
     if( abs(gen.pdgId()) == 13 ) isMu = true;
     if( abs(gen.pdgId()) == 11 ) isEle = true;

     if( evtLepDecayStatusVec[il][0] ){
        isLepDecay = true;
        if( isMu ) isMuDecay = true;
        if( isEle ) isEleDecay = true;
     }
     if( evtLepDecayStatusVec[il][1] ) { 
        isWLepDecay = true;
        if( isMu ){ isWMuDecay = true; pickedWLepGenIdx.push_back( lepGenIdxVec[il] ); }
        if( isEle ){ isWEleDecay = true; pickedWLepGenIdx.push_back( lepGenIdxVec[il] ); }
     }
     if( evtLepDecayStatusVec[il][2] ){
        isWTauLepDecay = true;
        if( isMu && !isWTauMuDecay ){ std::cout<<"ERROR ... leptonDecaysFilter  mis-matching between lepDecays and tauDecays for isWTauMuDecay : "<<isWTauMuDecay<<std::endl; }
        if( isEle && !isWTauEleDecay ){ std::cout<<"ERROR ... leptonDecaysFilter  mis-matching between lepDecays and tauDecays for isWTauEleDecay : "<<isWTauEleDecay<<std::endl; }
     }
     if( !isWLepDecay ){
        isOtherLepDecay = true;
        if( isMu ) isOtherMuDecay = true;
        if( isEle ) isOtherEleDecay = true;
     }
  }

  if( isLepDecay ){/*empty to avoid an error*/}
  if( isOtherLepDecay ){/*empty to avoid an error*/}
  if( isWTauLepDecay ){/*empty to avoid an error*/}

  if( isMuDecay && isEleDecay ) isMuAndEleDecay = true;

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

        for(size il=0; il<pickedWLepGenIdx.size(); il++){
           int ilep = pickedWLepGenIdx[il];
           const reco::GenParticle& genLep = genParticles->at(ilep);
           bool isFromThisBoson = find_mother( &genLep, gen);
           if( isFromThisBoson ) isWLepFromThisBoson ++;
        }

        if( isHadTauFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... leptonDecaysFilter ==> both W->tau(had) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauHadFilter_ ) foundMotherOverlap ++;
        }

        if( isTauMuFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... leptonDecaysFilter ==> both W->tau(->mu) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauMuFilter_ ) foundMotherOverlap ++;
        }

        if( isTauEleFromThisBoson && isWLepFromThisBoson ){
           std::cout<<"ERROR ... leptonDecaysFilter ==> both W->tau(->e) and W->mu/e from the same boson with ig/pdgId : "<<ig<<"/"<<gen.pdgId()<<" ?!"<<std::endl;
           if( WTauEleFilter_ ) foundMotherOverlap ++;
        }

     }
     if( !foundMotherOverlap && !pickedHadTauGenIdx.empty() && !pickedWLepGenIdx.empty() && WTauHadFilter_ ){
        isWTauHadDecay = false;
     }
     if( !foundMotherOverlap && !pickedTauMuGenIdx.empty() && !pickedWLepGenIdx.empty() && WTauMuFilter_ ){
        isWTauMuDecay = false;
     }
     if( !foundMotherOverlap && !pickedTauEleGenIdx.empty() && !pickedWLepGenIdx.empty() && WTauEleFilter_ ){
        isWTauEleDecay = false;
     }
  }

  bool isAllOthersDecay = false;
  if( tauGenIdxVec.empty() && lepGenIdxVec.empty() && pickedHadTauGenIdx.empty() ) isAllOthersDecay = true;

  if( WTauHadFilter_ && isWTauHadDecay ) pass = true;
  if( WTauMuFilter_ && isWTauMuDecay ) pass = true;
  if( WTauEleFilter_ && isWTauEleDecay ) pass = true;

  if( OtherTauHadFilter_ && isOtherTauHadDecay ) pass = true;
  if( OtherTauMuFilter_ && isOtherTauMuDecay ) pass = true;
  if( OtherTauEleFilter_ && isOtherTauEleDecay ) pass = true;

  if( WEleMuTauFilter_ && ( isWLepDecay || isWTauDecayCnt ) ) pass = true;
  if( WEleMuTaulepFilter_ && ( isWLepDecay || isWTauMuDecay || isWTauEleDecay ) ) pass = true;

  if( WMuFilter_ && isWMuDecay ) pass = true;
  if( WEleFilter_ && isWEleDecay ) pass = true;
  if( OtherMuFilter_ && isOtherMuDecay ) pass = true;
  if( OtherEleFilter_ && isOtherEleDecay ) pass = true;
  if( MuAndEleFilter_ && isMuAndEleDecay ) pass = true;
  if( allOthersFilter_ && isAllOthersDecay ) pass = true;

  return pass;

}

// Concentrate to tau with status 2
// Two cases
// W(3) -> tau(3) ->  tau (2) -> hadronic and/or leptonic ( directly target leptonic (all status 1)-- easier )
// B(2) -> tau(2) -> xxxx (same) 
std::vector<int> leptonDecaysFilter::tauDecays(std::vector<std::vector<int> > &evtTauDecayStatusVec){

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

bool leptonDecaysFilter::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}

std::vector<int> leptonDecaysFilter::lepDecays(std::vector<std::vector<int> > &evtLepDecayStatusVec){

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


bool leptonDecaysFilter::find_mother(const reco::Candidate* genp, const reco::Candidate &target){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->p4() == target.p4() && (genp->mother())->vertex() == target.vertex() && (genp->mother())->charge() == target.charge()) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), target);
    if (result) return true;
  }
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(leptonDecaysFilter);
