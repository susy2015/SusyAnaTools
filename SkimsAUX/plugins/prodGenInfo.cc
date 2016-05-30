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

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"

#include "TLorentzVector.h"

class prodGenInfo : public edm::EDFilter {

  public:

    explicit prodGenInfo(const edm::ParameterSet & iConfig);
    ~prodGenInfo();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag genParticleSrc_;
    edm::Handle<edm::View<reco::GenParticle > > genParticles;

    edm::InputTag genDecayStrVecSrc_, genDecayChainPartIdxVecSrc_;
    edm::Handle<std::vector<std::string> > genDecayStrVec_;
    edm::Handle<std::vector<int> > genDecayChainPartIdxVec_;

    edm::InputTag pfCandsSrc_;
    edm::Handle<pat::PackedCandidateCollection> pfcands;

    edm::InputTag rhoSrc_;
    edm::EDGetTokenT<edm::View<reco::GenParticle > > GenParticleTok_;
    edm::EDGetTokenT<std::vector<std::string> > GenDecayStrVecTok_;
    edm::EDGetTokenT<std::vector<int> > GenDecayChainParIdxVecTok_;   
    edm::EDGetTokenT<pat::PackedCandidateCollection> PfCandsTok_;
    edm::EDGetTokenT<double> RhoTok_;

    bool debug_;

    int find_idx(const reco::Candidate & target);
    int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);

    void find_mother(std::vector<int> & momIdxVec, int dauIdx, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec);
    void find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tauVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, std::vector<int> &W_tau_nuVec, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec, const std::vector<int> &genDecayPdgIdVec);

    double calc_pfActivity (const int idx, const std::vector<int> &genDecayIdxVec, const double rho);
};


prodGenInfo::prodGenInfo(const edm::ParameterSet & iConfig) {

  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");

  genDecayStrVecSrc_ = iConfig.getParameter<edm::InputTag>("genDecayStrVecSrc");
  genDecayChainPartIdxVecSrc_ = iConfig.getParameter<edm::InputTag>("genDecayChainPartIdxVecSrc");

  pfCandsSrc_   = iConfig.getParameter<edm::InputTag>("PFCandSource");
  
  debug_       = iConfig.getParameter<bool>("debug");

  rhoSrc_       = iConfig.getParameter<edm::InputTag>("RhoSource");
  GenParticleTok_ =consumes<edm::View<reco::GenParticle > >(genParticleSrc_);
  GenDecayStrVecTok_ =consumes<std::vector<std::string> >(genDecayStrVecSrc_);
  GenDecayChainParIdxVecTok_ =consumes<std::vector<int> >(genDecayChainPartIdxVecSrc_);
  PfCandsTok_= consumes<pat::PackedCandidateCollection>(pfCandsSrc_);
  RhoTok_ = consumes<double>(rhoSrc_);

  produces<std::vector<std::string> >("genDecayStrVec");
  produces<std::vector<int> >("genDecayIdxVec");
  produces<std::vector<int> >("genDecayPdgIdVec");
  produces<std::vector<int> >("genDecayMomIdxVec");
  produces<std::vector<TLorentzVector> >("genDecayLVec");
  produces<std::vector<int> >("genDecayMomRefVec");

  produces<std::vector<int> >("WemuVec");
  produces<std::vector<int> >("WtauVec");
  produces<std::vector<int> >("WtauemuVec");
  produces<std::vector<int> >("WtauprongsVec");
  produces<std::vector<int> >("WtaunuVec");

  produces<std::vector<double> >("WemupfActivityVec");
  produces<std::vector<double> >("WtauemupfActivityVec");
  produces<std::vector<double> >("WtauprongspfActivityVec");
}


prodGenInfo::~prodGenInfo() {
}


bool prodGenInfo::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  iEvent.getByToken(GenParticleTok_, genParticles);
  iEvent.getByToken(GenDecayStrVecTok_, genDecayStrVec_);
  iEvent.getByToken(GenDecayChainParIdxVecTok_, genDecayChainPartIdxVec_);

  iEvent.getByToken(PfCandsTok_, pfcands);

  edm::Handle< double > rho_;
  //iEvent.getByLabel("fixedGridRhoFastjetCentralNeutral", rho_); // Central rho recommended for SUSY
  iEvent.getByToken(RhoTok_,rho_);
  double rho = *rho_;

  std::auto_ptr<std::vector<TLorentzVector> > genDecayLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<int> > genDecayIdxVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > genDecayPdgIdVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > genDecayMomIdxVec(new std::vector<int>());
  std::auto_ptr<std::vector<std::string> > genDecayStrVec(new std::vector<std::string>());
  std::auto_ptr<std::vector<int> > genDecayMomRefVec(new std::vector<int>());

  std::auto_ptr<std::vector<int> > W_emuVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > W_tauVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > W_tau_emuVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > W_tau_prongsVec(new std::vector<int>());
  std::auto_ptr<std::vector<int> > W_tau_nuVec(new std::vector<int>());

  std::auto_ptr<std::vector<double> > W_emu_pfActivityVec(new std::vector<double>());
  std::auto_ptr<std::vector<double> > W_tau_emu_pfActivityVec(new std::vector<double>());
  std::auto_ptr<std::vector<double> > W_tauprongs_pfActivityVec(new std::vector<double>());

  for(unsigned int id=0; id<genDecayStrVec_->size(); id++){
     genDecayStrVec->push_back( (*genDecayStrVec_)[id] );
  }

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

     genDecayIdxVec->push_back(idxGen);
     genDecayMomIdxVec->push_back(momIdx);
     genDecayPdgIdVec->push_back(pdgId);
     genDecayLVec->push_back(genPartLVec);
  }
  for(unsigned int id=0; id<genDecayMomIdxVec->size(); id++){
     int momIdx = genDecayMomIdxVec->at(id);
     genDecayMomRefVec->push_back(find_idx(momIdx, (*genDecayIdxVec)));
  }

  if( debug_ ){
     std::cout<<"\nord/idxGen/pdgId/momIdx/momRef : ";
     for(unsigned int ig=0; ig<genDecayIdxVec->size(); ig++){
        std::cout<<"  "<<ig<<"/"<<genDecayIdxVec->at(ig)<<"/"<<genDecayPdgIdVec->at(ig)<<"/"<<genDecayMomIdxVec->at(ig)<<"/"<<genDecayMomRefVec->at(ig);
     }
     std::cout<<std::endl;
  }

  find_W_emu_tauprongs((*W_emuVec), (*W_tauVec), (*W_tau_emuVec), (*W_tau_prongsVec), (*W_tau_nuVec), (*genDecayIdxVec), (*genDecayMomIdxVec), (*genDecayPdgIdVec));
  if( debug_ ){
     std::cout<<"\nW_emu (idx: idxGen/pdgId/momRef) : ";
     for(unsigned int id=0; id<W_emuVec->size(); id++){
        std::cout<<"  "<<"("<<W_emuVec->at(id)<<": "<<genDecayIdxVec->at(W_emuVec->at(id))<<"/"<<genDecayPdgIdVec->at(W_emuVec->at(id))<<"/"<<genDecayMomRefVec->at(W_emuVec->at(id))<<")";
     }
     std::cout<<std::endl;
     std::cout<<"\nW_tau (idx: idxGen/pdgId/momRef) : ";
     for(unsigned int id=0; id<W_tauVec->size(); id++){
        std::cout<<"  "<<"("<<W_tauVec->at(id)<<": "<<genDecayIdxVec->at(W_tauVec->at(id))<<"/"<<genDecayPdgIdVec->at(W_tauVec->at(id))<<"/"<<genDecayMomRefVec->at(W_tauVec->at(id))<<")";
     }
     std::cout<<std::endl;
     std::cout<<"\nW_tau_emu (idx: idxGen/pdgId/momRef) : ";
     for(unsigned int id=0; id<W_tau_emuVec->size(); id++){
        std::cout<<"  "<<"("<<W_tau_emuVec->at(id)<<": "<<genDecayIdxVec->at(W_tau_emuVec->at(id))<<"/"<<genDecayPdgIdVec->at(W_tau_emuVec->at(id))<<"/"<<genDecayMomRefVec->at(W_tau_emuVec->at(id))<<")";
     }
     std::cout<<std::endl;
     std::cout<<"\nW_tau_prongs (idx: idxGen/pdgId/momRef) : ";
     for(unsigned int id=0; id<W_tau_prongsVec->size(); id++){
        std::cout<<"  "<<"("<<W_tau_prongsVec->at(id)<<": "<<genDecayIdxVec->at(W_tau_prongsVec->at(id))<<"/"<<genDecayPdgIdVec->at(W_tau_prongsVec->at(id))<<"/"<<genDecayMomRefVec->at(W_tau_prongsVec->at(id))<<")";
     }
     std::cout<<std::endl;
     std::cout<<"\nW_tau_nu (idx: idxGen/pdgId/momRef) : ";
     for(unsigned int id=0; id<W_tau_nuVec->size(); id++){
        std::cout<<"  "<<"("<<W_tau_nuVec->at(id)<<": "<<genDecayIdxVec->at(W_tau_nuVec->at(id))<<"/"<<genDecayPdgIdVec->at(W_tau_nuVec->at(id))<<"/"<<genDecayMomRefVec->at(W_tau_nuVec->at(id))<<")";
     }
     std::cout<<std::endl<<std::endl;
  }

// Calculate pfActivity for gen ele, mu, pi and ? (enough?)
  for(unsigned int il=0; il < W_emuVec->size(); il++){
    int idx = W_emuVec->at(il);
    double pfActivity = calc_pfActivity (idx, (*genDecayIdxVec), rho);
    W_emu_pfActivityVec->push_back(pfActivity);
  }

  for(unsigned int il=0; il < W_tau_emuVec->size(); il++){
    int idx = W_tau_emuVec->at(il);
    double pfActivity = calc_pfActivity (idx, (*genDecayIdxVec), rho);
    W_tau_emu_pfActivityVec->push_back(pfActivity);
  }

  for(unsigned int ih=0; ih < W_tau_prongsVec->size(); ih++){
    int idx = W_tau_prongsVec->at(ih);
    double pfActivity = calc_pfActivity (idx, (*genDecayIdxVec), rho);
    W_tauprongs_pfActivityVec->push_back(pfActivity);
  }

  // store in the event
  iEvent.put(genDecayLVec, "genDecayLVec");
  iEvent.put(genDecayStrVec, "genDecayStrVec");

  iEvent.put(genDecayIdxVec, "genDecayIdxVec");
  iEvent.put(genDecayPdgIdVec, "genDecayPdgIdVec");
  iEvent.put(genDecayMomIdxVec, "genDecayMomIdxVec");
  iEvent.put(genDecayMomRefVec, "genDecayMomRefVec");

  iEvent.put(W_tauVec, "WtauVec");
  iEvent.put(W_emuVec, "WemuVec");
  iEvent.put(W_tau_emuVec, "WtauemuVec");
  iEvent.put(W_tau_prongsVec, "WtauprongsVec");
  iEvent.put(W_tau_nuVec, "WtaunuVec");

  iEvent.put(W_emu_pfActivityVec, "WemupfActivityVec");
  iEvent.put(W_tau_emu_pfActivityVec, "WtauemupfActivityVec");
  iEvent.put(W_tauprongs_pfActivityVec, "WtauprongspfActivityVec");

  return true;
}

int prodGenInfo::find_idx(const reco::Candidate & target){
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

int prodGenInfo::find_idx(int genIdx, const std::vector<int> &genDecayIdxVec){
   for(int ig=0; ig<(int)genDecayIdxVec.size(); ig++){
      if( genDecayIdxVec[ig] == genIdx ) return ig;
   }
   return -1;
}

void prodGenInfo::find_mother(std::vector<int> & momIdxVec, int dauIdx, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec){
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

void prodGenInfo::find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tauVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, std::vector<int> &W_tau_nuVec, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec, const std::vector<int> &genDecayPdgIdVec){
   W_emuVec.clear(); W_tauVec.clear(), W_tau_emuVec.clear(); W_tau_prongsVec.clear(); W_tau_nuVec.clear();
   if( !genDecayPdgIdVec.empty() ){
      for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
         int pdgId = genDecayPdgIdVec.at(ig);
         if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15 || abs(pdgId) == 211 || abs(pdgId) == 321 || abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16 ){
            std::vector<int> momIdxVec;
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
               if( abs(pdgId) == 15 ) W_tauVec.push_back(ig);
               if( cntMomTau ){
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_tau_emuVec.push_back(ig);
                  else if( abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16 ) W_tau_nuVec.push_back(ig);
                  else if( abs(pdgId) != 15 ) W_tau_prongsVec.push_back(ig);
               }else{
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_emuVec.push_back(ig);
               }
            }
         }
      }
   }
   return;
}

double prodGenInfo::calc_pfActivity (const int idx, const std::vector<int> &genDecayIdxVec, const double rho){
   int idxGen = genDecayIdxVec.at(idx);

   const reco::GenParticle& genPart = genParticles->at(idxGen);
   int pdgId = genPart.pdgId();

   const reco::Candidate* lep = dynamic_cast<const reco::Candidate *>(&genPart);

   std::string gen_type = "";
   if( std::abs(pdgId) ==11) gen_type = "electron";
   else if ( std::abs(pdgId) == 13 ) gen_type = "muon"; 

   double pfActivity = commonFunctions::GetMiniIsolation(pfcands, lep, gen_type, rho, true);
   return pfActivity;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodGenInfo);
