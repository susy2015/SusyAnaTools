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
using namespace std;

class topDecaysFilter : public edm::EDFilter {

  public:

    explicit topDecaysFilter(const edm::ParameterSet & iConfig);
    ~topDecaysFilter();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag genJetSrc_;
    edm::InputTag genParticleSrc_;
    edm::Handle<std::vector<reco::GenParticle > > genParticles;
    edm::Handle<std::vector<reco::GenJet > > genJets;

    bool find_mother(const reco::Candidate* genp, const int motherID);
    bool find_mother(const reco::Candidate* genp, const reco::Candidate &target);

    void topDecays(std::vector<int> &topIdxVec, std::vector<std::vector<int> > &topDausIdxVec, std::vector<std::vector<int> > &WDausIdxVec, std::vector<std::vector<int> > &tauDausIdxVec, std::vector<string> &decayTypeVec);

    int find_idx(const reco::Candidate* target);

    vector<string> toPickTypeStrVec_;

    bool debug_;

    bool isData;
};


topDecaysFilter::topDecaysFilter(const edm::ParameterSet & iConfig) {

  genJetSrc_ = iConfig.getParameter<edm::InputTag>("genJetSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");
  toPickTypeStrVec_ = iConfig.getParameter<vector<string> >("toPickTypeStrVec");
  debug_ = iConfig.getParameter<bool>("debug");

  produces<std::vector<int> >("topIdxVec");
  produces<std::vector<std::vector<int> > >("topDausIdxVec");
  produces<std::vector<std::vector<int> > >("WDausIdxVec");
  produces<std::vector<std::vector<int> > >("tauDausIdxVec");
  produces<std::vector<std::string> >("decayTypeVec");

  isData = false;

}


topDecaysFilter::~topDecaysFilter() {
}


bool topDecaysFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;

  if( iEvent.isRealData() ) isData = true; if( isData ) return true;

  iEvent.getByLabel(genParticleSrc_, genParticles);
  iEvent.getByLabel(genJetSrc_, genJets);

  std::vector<int> topIdxVec; 
  std::vector<std::vector<int> > topDausIdxVec, WDausIdxVec, tauDausIdxVec; 
  std::vector<std::string> decayTypeVec;

  std::auto_ptr<std::vector<int> > topIdxVecPtr (new std::vector<int> );
  std::auto_ptr<std::vector<std::vector<int> > > topDausIdxVecPtr (new std::vector<std::vector<int> >);
  std::auto_ptr<std::vector<std::vector<int> > > WDausIdxVecPtr (new std::vector<std::vector<int> >);
  std::auto_ptr<std::vector<std::vector<int> > > tauDausIdxVecPtr (new std::vector<std::vector<int> >);
  std::auto_ptr<std::vector<std::string> > decayTypeVecPtr (new std::vector<std::string> );

  topDecays(topIdxVec, topDausIdxVec, WDausIdxVec, tauDausIdxVec, decayTypeVec);

  for(size it=0; it<topIdxVec.size(); it++){
     topIdxVecPtr->push_back(topIdxVec[it]);
     topDausIdxVecPtr->push_back(topDausIdxVec[it]);
     WDausIdxVecPtr->push_back(WDausIdxVec[it]);
     tauDausIdxVecPtr->push_back(tauDausIdxVec[it]);
     decayTypeVecPtr->push_back(decayTypeVec[it]);
  }

  if( debug_ ){
     std::cout<<"\nFound decay types : ";
     for(size id=0; id<decayTypeVec.size(); id++){
        std::cout<<"  "<<decayTypeVec[id].c_str();
     }
     std::cout<<std::endl;
  }

  bool pass = false;

  if( toPickTypeStrVec_.empty() ) pass = false;
  else{
     for(size ip=0; ip<toPickTypeStrVec_.size(); ip++){
        if( toPickTypeStrVec_[ip] == "all" ){ pass = true; break; }
        vector<string>::iterator vsi = find(decayTypeVec.begin(), decayTypeVec.end(), toPickTypeStrVec_[ip]);
        if( vsi != decayTypeVec.end() ) pass = true;
     }
  }

  iEvent.put(topIdxVecPtr, "topIdxVec");
  iEvent.put(topDausIdxVecPtr, "topDausIdxVec");
  iEvent.put(WDausIdxVecPtr, "WDausIdxVec");
  iEvent.put(tauDausIdxVecPtr, "tauDausIdxVec");
  iEvent.put(decayTypeVecPtr, "decayTypeVec");

  return pass;
}

// decay type definition : 
// "allHad"     : all hadronic decay of the top,                      top->b+W; W->jj
// "allHadrad"  : all hadronic decay of the top with top radiation,   top->b+W+j(s); W->jj
// "eleW"       : leptonic W decay of the top,                        top->b+W; W->ele nu_ele
// "eleWrad"    : leptonic W decay of the top with top radiation,     top->b+W+j(s); W->ele nu_ele
// "muW"        : leptonic W decay of the top,                        top->b+W; W->mu nu_mu
// "muWrad"     : leptonic W decay of the top with top radiation,     top->b+W+j(s); W->mu nu_mu
// "taumuW"     : leptonic W decay of the top,                        top->b+W; W->tau nu_tau; tau->mu nu_mu
// "taumuWrad"  : leptonic W decay of the top with top radiation,     top->b+W+j(s); W->tau nu_tau; tau->mu nu_mu
// "taueleW"    : leptonic W decay of the top,                        top->b+W; W->tau nu_tau; tau->ele nu_ele
// "taueleWrad" : leptonic W decay of the top with top radiation,     top->b+W+j(s); W->tau nu_tau; tau->ele nu_ele
// "tauhadW"    : leptonic W decay of the top,                        top->b+W; W->tau nu_tau; tau->had nu_had
// "tauhadWrad" : leptonic W decay of the top with top radiation,     top->b+W+j(s); W->tau nu_tau; tau->had nu_had
void topDecaysFilter::topDecays(std::vector<int> &topIdxVec, std::vector<std::vector<int> > &topDausIdxVec, std::vector<std::vector<int> > &WDausIdxVec, std::vector<std::vector<int> > &tauDausIdxVec, std::vector<string> &decayTypeVec){

   using namespace edm;
   using namespace std;
              
   topIdxVec.clear(); topDausIdxVec.clear(); 
   WDausIdxVec.clear(); tauDausIdxVec.clear(); 
   decayTypeVec.clear(); 
               
   for(size ig=0; ig<genParticles->size(); ig++){
         
      const reco::GenParticle& gen = genParticles->at(ig);
         
      if( abs(gen.pdgId()) == 6 && gen.status() ==3 ){
          
         topIdxVec.push_back(ig);

         vector<int> pertopDausIdxVec, perWDausIdxVec, pertauDausIdxVec;

         size numTopDaus = gen.numberOfDaughters();

         int pickedWIdx = -1, pickedbIdx = -1;
         int cntW = 0, cntb = 0, cntTopDaus =0;
         for(size itd =0; itd<numTopDaus; itd++){
            const reco::Candidate * topDau = gen.daughter(itd);
            if( abs(topDau->pdgId() ) ==6 ) continue;
 
            cntTopDaus ++;

            int topDauIdx = find_idx(topDau);
            pertopDausIdxVec.push_back(topDauIdx);

            if( abs(topDau->pdgId()) == 24 && topDau->status() == 3 ){
               pickedWIdx = topDauIdx; cntW ++;
            }
            if( abs(topDau->pdgId()) == 5 && topDau->status() == 3 ){
               pickedbIdx = topDauIdx; cntb ++;
            }
         }
         if( pickedbIdx ){/*empty to avoid a compiling error*/}
         if( cntW != 1 ){
            std::cout<<"WARNING ... for the top with index : "<<ig<<"   number of W daughters : "<<cntW<<"  NOT 1!"<<std::endl;
         }
         if( cntb != 1 ){
            std::cout<<"WARNING ... for the top with index : "<<ig<<"   number of b daughters : "<<cntb<<"  NOT 1!"<<std::endl;
         }

         const reco::GenParticle& genW = genParticles->at(pickedWIdx);
         size numWDaus = genW.numberOfDaughters();
         int pickedTauIdx = -1, cntTau = 0;

         int isWEleDecay = 0, isWMuDecay =0, isWTauDecay = 0;
         int cntWDaus =0;
         for(size iwd=0; iwd<numWDaus; iwd++){
            const reco::Candidate * wDau = genW.daughter(iwd);
            if( abs(wDau->pdgId()) == 24 ) continue;

            cntWDaus ++;

            int wDauIdx = find_idx(wDau);
            perWDausIdxVec.push_back(wDauIdx);
            if( abs(wDau->pdgId()) == 15 && wDau->status() == 3 ){
               pickedTauIdx = wDauIdx; cntTau++;
               isWTauDecay ++;
            }
            if( abs(wDau->pdgId()) == 11 && wDau->status() == 3 ){
               isWEleDecay ++;
            }
            if( abs(wDau->pdgId()) == 13 && wDau->status() == 3 ){
               isWMuDecay ++;
            }
         } 
         if( cntTau >=2 ){
            std::cout<<"WARNING ... more than 2 taus from the W?!"<<std::endl;
         }
         if( isWEleDecay + isWMuDecay + isWTauDecay >=2 ){
            std::cout<<"WARNING ... more than 2 leptons, i.e., ele, mu or tau, directly from the W decay?!"<<std::endl;
         }
           
         int isTauEleDecay = 0, isTauMuDecay = 0, isTauHadDecay = 0;
         int cntTauDaus = 0;
         if( pickedTauIdx != -1 ){
            const reco::GenParticle& genTau = genParticles->at(pickedTauIdx);
            for(size tg=0; tg<genParticles->size(); tg++){
               const reco::GenParticle& genTauDau = genParticles->at(tg);
               if( genTauDau.status() != 1 ) continue;
               if( abs(genTauDau.pdgId()) == 15 ) continue;
               bool isFromTau = find_mother(&genTauDau, genTau);
               if( !isFromTau ) continue;

               cntTauDaus ++;

               pertauDausIdxVec.push_back(tg);

               if( abs(genTauDau.pdgId()) == 11 ){
                  isTauEleDecay++;
               }
               if( abs(genTauDau.pdgId()) == 13 ){
                  isTauMuDecay++;
               }
            }
         }
         if( isTauEleDecay + isTauMuDecay >=2 ){
//            std::cout<<"WARNING ... more than 2 leptons, i.e., ele or mu, directly from the tau decay?! likely bramlung eles"<<std::endl;
            isTauEleDecay =0; isTauMuDecay =0;
         }
         if( !isTauEleDecay && !isTauMuDecay ) isTauHadDecay = 1;

         if( isTauHadDecay ){/*empty to avoid a compiling error*/}

         topDausIdxVec.push_back(pertopDausIdxVec);
         WDausIdxVec.push_back(perWDausIdxVec);
         tauDausIdxVec.push_back(pertauDausIdxVec);
         string typeStr;
         if( isWEleDecay ){typeStr = "eleW"; if(cntTopDaus >2) typeStr = "eleWrad"; }
         else if( isWMuDecay ){ typeStr = "muW"; if(cntTopDaus >2) typeStr = "muWrad"; }
         else if( isWTauDecay ){
            if( isTauEleDecay ){ typeStr = "taueleW"; if(cntTopDaus >2) typeStr = "taueleWrad"; }
            else if( isTauMuDecay ){ typeStr = "taumuW"; if(cntTopDaus >2) typeStr = "taumuWrad"; }
            else{ typeStr = "tauhadW"; if(cntTopDaus >2) typeStr = "tauhadWrad"; }
         }else{
            typeStr = "allHad"; if(cntTopDaus >2) typeStr = "allHadrad";
         }
         decayTypeVec.push_back(typeStr);
      }
   }

   return;
}

int topDecaysFilter::find_idx(const reco::Candidate* target){
   int pickedIdx = -1;
   for(size ig=0; ig<genParticles->size(); ig++){
      const reco::GenParticle& gen = genParticles->at(ig);
      if( target->p4() == gen.p4() && target->vertex() == gen.vertex() && target->charge() == gen.charge() ){
         pickedIdx = (int)ig;
         break;
      }
   }
   return pickedIdx; 
}

bool topDecaysFilter::find_mother(const reco::Candidate* genp, const reco::Candidate &target){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->p4() == target.p4() && (genp->mother())->vertex() == target.vertex() && (genp->mother())->charge() == target.charge()) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), target);
    if (result) return true;
  }
  return false;
}

bool topDecaysFilter::find_mother(const reco::Candidate* genp, const int motherID){
  if (genp->numberOfMothers()>0) {
    if ((genp->mother())->pdgId()==motherID) return true;
  }
  if ((genp->mother())->numberOfMothers()>0) {
    const bool result=find_mother(genp->mother(), motherID);
    if (result) return true;
  }
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(topDecaysFilter);
