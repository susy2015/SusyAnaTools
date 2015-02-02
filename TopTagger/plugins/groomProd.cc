/* class groomProd
 *
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include <iostream>
#include <sstream> 
#include <algorithm>

#include "fastjet/ClusterSequence.hh"
#include "fastjet/tools/Pruner.hh"
//#include "fastjet/tools/PseudoJet.hh"
#include "fastjet/PseudoJet.hh"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "SusyAnaTools/TopTagger/interface/combination.h"

#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace stdcomb;

class groomProd : public edm::EDFilter{
public:
  groomProd( const edm::ParameterSet & );
private:
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag jetSrc_;
  bool debug_;

  /// options
  int groomingOpt_; // 1: pruning (default); 2: filtering; 3: trimming
  double pruning_RcutFactor_, pruning_zCut_, pruning_rParam_, pruning_jetPtMin_;

  edm::Handle<edm::View<pat::Jet> > inputJets;

  void pruning(const std::vector<std::vector<int> > & combIdxJetsHolder, std::vector<TLorentzVector> & outputJets);
};

groomProd::groomProd( const ParameterSet & cfg ) :
  jetSrc_( cfg.getParameter<InputTag>( "jetSrc" ) ),
  debug_( cfg.getUntrackedParameter<bool>( "debug", false ) ),
  groomingOpt_( cfg.getUntrackedParameter<int>( "groomingOpt", 1 ) ),
  pruning_RcutFactor_( cfg.getUntrackedParameter<double>( "pruningRcutFactor", 0.5 ) ),
  pruning_zCut_( cfg.getUntrackedParameter<double>( "pruningzCut", 0.1 ) ),
  pruning_rParam_( cfg.getUntrackedParameter<double>( "pruningrParam", 1000 ) ),
  pruning_jetPtMin_( cfg.getUntrackedParameter<double>( "pruningjetPtMin", 5.0 ) ){
 
  produces<std::vector<TLorentzVector> >("groomedJets");
  produces<std::vector<std::vector<int> > >("groomedJetsIdx");
  produces<std::vector<TLorentzVector> >("groomedDiJets");
  produces<std::vector<std::vector<int> > >("groomedDiJetsIdx");
  produces<std::vector<TLorentzVector> >("groomedTriJets");
  produces<std::vector<std::vector<int> > >("groomedTriJetsIdx");
}

bool groomProd::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  iEvent.getByLabel(jetSrc_, inputJets);

  std::auto_ptr<std::vector<TLorentzVector> > groomedJetsPtr (new std::vector<TLorentzVector> );
  std::auto_ptr<std::vector<TLorentzVector> > groomedDiJetsPtr (new std::vector<TLorentzVector> );
  std::auto_ptr<std::vector<TLorentzVector> > groomedTriJetsPtr (new std::vector<TLorentzVector> );

  std::auto_ptr<std::vector<std::vector<int> > > groomedJetsIdxPtr (new std::vector<std::vector<int> > );
  std::auto_ptr<std::vector<std::vector<int> > > groomedDiJetsIdxPtr (new std::vector<std::vector<int> > );
  std::auto_ptr<std::vector<std::vector<int> > > groomedTriJetsIdxPtr (new std::vector<std::vector<int> > );

  std::vector<std::vector<int> > combJetsHolder, combDiJetsHolder, combTriJetsHolder;

  std::vector<int> idxVec, iniDiJetsComb, iniTriJetsComb;

  for(unsigned int ij=0; ij<inputJets->size(); ij++){
    idxVec.push_back(ij);

    std::vector<int> perIdx; perIdx.push_back(ij);
    combJetsHolder.push_back(perIdx);
    groomedJetsIdxPtr->push_back(perIdx);
  }

  if( inputJets->size() >= 2 ){
    iniDiJetsComb.push_back(0); iniDiJetsComb.push_back(1);
    do{
      combDiJetsHolder.push_back(iniDiJetsComb);
      groomedDiJetsIdxPtr->push_back(iniDiJetsComb);
    }while(next_combination(idxVec.begin(),idxVec.end(),iniDiJetsComb.begin(),iniDiJetsComb.end()) );
  }

  if( inputJets->size() >= 3 ){
    iniTriJetsComb.push_back(0); iniTriJetsComb.push_back(1); iniTriJetsComb.push_back(2);
    do{
      combTriJetsHolder.push_back(iniTriJetsComb);
      groomedTriJetsIdxPtr->push_back(iniTriJetsComb);
    }while(next_combination(idxVec.begin(),idxVec.end(),iniTriJetsComb.begin(),iniTriJetsComb.end()) );
  }

  pruning( combJetsHolder, (*groomedJetsPtr) );
  pruning( combDiJetsHolder, (*groomedDiJetsPtr) );
  pruning( combTriJetsHolder, (*groomedTriJetsPtr) );

  const JetCorrector* corrector = 0;
  if( iEvent.isRealData() ){
     corrector = JetCorrector::getJetCorrector("ak4PFchsL1FastL2L3Residual",iSetup);
  }else{
     corrector = JetCorrector::getJetCorrector("ak4PFchsL1FastL2L3",iSetup);
  } 

  if( debug_ ){
     std::cout<<"\n\nSingle jet ...  combJetsHolder.size : "<<combJetsHolder.size()<<"  groomedJetsPtr->size : "<<groomedJetsPtr->size()<<"  inputJets->size : "<<inputJets->size()<<std::endl;
     for(unsigned int ic=0; ic<combJetsHolder.size(); ic++){
        TLorentzVector perCombLVec;
        TLorentzVector perGroomedLVec = groomedJetsPtr->at(ic);
        std::cout<<"idx(";
        for(unsigned int id=0; id<combJetsHolder[ic].size(); id++){
           TLorentzVector perLVec;
           perLVec.SetPxPyPzE(inputJets->at(combJetsHolder[ic][id]).px(), inputJets->at(combJetsHolder[ic][id]).py(), inputJets->at(combJetsHolder[ic][id]).pz(), inputJets->at(combJetsHolder[ic][id]).energy());
           perCombLVec += perLVec;
           printf("%2d", combJetsHolder[ic][id]);
           if( id != combJetsHolder[ic].size() -1 ) std::cout<<",";
        }
        pat::Jet oripatJet = inputJets->at(combJetsHolder[ic][0]);
        std::vector<std::string> availableJECLevels = oripatJet.availableJECLevels();
        pat::Jet rawpatJet = oripatJet;
        double scaleBackToRaw = oripatJet.jecFactor("Uncorrected")/oripatJet.jecFactor(availableJECLevels.back());
        rawpatJet.scaleEnergy(scaleBackToRaw);

        math::XYZTLorentzVector groomedp4;
        groomedp4.SetCoordinates(perGroomedLVec.Px(), perGroomedLVec.Py(), perGroomedLVec.Pz(), perGroomedLVec.E());
        pat::Jet groomedpatJet = rawpatJet;
        groomedpatJet.setP4(groomedp4);

        double corrFactor = 1.0;
        if( corrector ) corrFactor = corrector->correction(groomedpatJet, iEvent, iSetup);

        perGroomedLVec.SetPxPyPzE(groomedp4.px()*corrFactor, groomedp4.py()*corrFactor, groomedp4.pz()*corrFactor, groomedp4.energy()*corrFactor);
        
        printf(") -->  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n", perCombLVec.Pt(), perCombLVec.Eta(), perCombLVec.Phi(), perCombLVec.M());
        printf("        raw  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n", rawpatJet.pt(), rawpatJet.eta(), rawpatJet.phi(), rawpatJet.mass());
        printf("raw groomed  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n", groomedJetsPtr->at(ic).Pt(), groomedJetsPtr->at(ic).Eta(), groomedJetsPtr->at(ic).Phi(), groomedJetsPtr->at(ic).M());
        printf("    groomed  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n\n", perGroomedLVec.Pt(), perGroomedLVec.Eta(), perGroomedLVec.Phi(), perGroomedLVec.M());

// Replace the groomed jet LorentzVector with the JEC corrected ones!
        (*groomedJetsPtr)[ic] = perGroomedLVec;
     }

     std::cout<<"\n\nDijets ...  combDiJetsHolder.size : "<<combDiJetsHolder.size()<<"  groomedDiJetsPtr->size : "<<groomedDiJetsPtr->size()<<std::endl;
     for(unsigned int ic=0; ic<combDiJetsHolder.size(); ic++){
        TLorentzVector perCombLVec;
        TLorentzVector perGroomedLVec = groomedDiJetsPtr->at(ic);
        std::cout<<"idx(";
        for(unsigned int id=0; id<combDiJetsHolder[ic].size(); id++){
           TLorentzVector perLVec;
           perLVec.SetPxPyPzE(inputJets->at(combDiJetsHolder[ic][id]).px(), inputJets->at(combDiJetsHolder[ic][id]).py(), inputJets->at(combDiJetsHolder[ic][id]).pz(), inputJets->at(combDiJetsHolder[ic][id]).energy());
           perCombLVec += perLVec;
           printf("%2d", combDiJetsHolder[ic][id]);
           if( id != combDiJetsHolder[ic].size() -1 ) std::cout<<",";
        }
        printf(") -->  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n", perCombLVec.Pt(), perCombLVec.Eta(), perCombLVec.Phi(), perCombLVec.M());
        printf("       groomed  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n\n", perGroomedLVec.Pt(), perGroomedLVec.Eta(), perGroomedLVec.Phi(), perGroomedLVec.M());
     }

     std::cout<<"\n\nTrijets ...  combTriJetsHolder.size : "<<combTriJetsHolder.size()<<"  groomedTriJetsPtr->size : "<<groomedTriJetsPtr->size()<<std::endl;
     for(unsigned int ic=0; ic<combTriJetsHolder.size(); ic++){
        TLorentzVector perCombLVec;
        TLorentzVector perGroomedLVec = groomedTriJetsPtr->at(ic);
        std::cout<<"idx(";
        for(unsigned int id=0; id<combTriJetsHolder[ic].size(); id++){
           TLorentzVector perLVec;
           perLVec.SetPxPyPzE(inputJets->at(combTriJetsHolder[ic][id]).px(), inputJets->at(combTriJetsHolder[ic][id]).py(), inputJets->at(combTriJetsHolder[ic][id]).pz(), inputJets->at(combTriJetsHolder[ic][id]).energy());
           perCombLVec += perLVec;
           printf("%2d", combTriJetsHolder[ic][id]);
           if( id != combTriJetsHolder[ic].size() -1 ) std::cout<<",";
        }
        printf(") -->  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n", perCombLVec.Pt(), perCombLVec.Eta(), perCombLVec.Phi(), perCombLVec.M());
        printf("          groomed  pt : %6.2f  eta : % 7.2f  phi : % 7.2f  m : % 7.2f\n\n", perGroomedLVec.Pt(), perGroomedLVec.Eta(), perGroomedLVec.Phi(), perGroomedLVec.M());
     }
  }

  iEvent.put(groomedJetsPtr, "groomedJets");
  iEvent.put(groomedDiJetsPtr, "groomedDiJets");
  iEvent.put(groomedTriJetsPtr, "groomedTriJets");
  
  iEvent.put(groomedJetsIdxPtr, "groomedJetsIdx");
  iEvent.put(groomedDiJetsIdxPtr, "groomedDiJetsIdx");
  iEvent.put(groomedTriJetsIdxPtr, "groomedTriJetsIdx");
  
  return true;
}

void groomProd::pruning(const std::vector<std::vector<int> > & combIdxJetsHolder, std::vector<TLorentzVector> & outputJets){

   outputJets.clear();

// turn your PF particles into fastjet pseudojets
   for(unsigned int ic=0; ic<combIdxJetsHolder.size(); ic++){
      TLorentzVector perCombLVec;

      std::vector<fastjet::PseudoJet> FJparticles;
      for(unsigned int ij=0; ij<combIdxJetsHolder[ic].size(); ij++){
         int idxJet = combIdxJetsHolder[ic][ij];

         TLorentzVector perLVec;
         perLVec.SetPxPyPzE(inputJets->at(idxJet).px(), inputJets->at(idxJet).py(), inputJets->at(idxJet).pz(), inputJets->at(idxJet).energy());
         perCombLVec += perLVec;

//         const std::vector<reco::PFCandidatePtr> & constituents = (*inputJets)[idxJet].getPFConstituents();
         const unsigned int numConstituents = (*inputJets)[idxJet].numberOfDaughters();
         for (unsigned int iCon = 0; iCon < numConstituents; ++iCon){
//            const reco::PFCandidatePtr& constituent = constituents[iCon];
            const reco::Candidate * constituent = (*inputJets)[idxJet].daughter(iCon);
            FJparticles.push_back( fastjet::PseudoJet( constituent->px(), constituent->py(), constituent->pz(), constituent->energy() ) );
         }
      }

// declare pruner
      fastjet::Pruner pruner(fastjet::cambridge_algorithm, pruning_zCut_, pruning_RcutFactor_);
//      fastjet::Pruner pruner(fastjet::antikt_algorithm, pruning_zCut_, pruning_RcutFactor_);

// recluster your jet (FJparticles)
//      fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, pruning_rParam_);
      fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, pruning_rParam_);
      fastjet::ClusterSequence thisClustering_basic(FJparticles, jetDef);
      std::vector<fastjet::PseudoJet> out_jets_basic = sorted_by_pt(thisClustering_basic.inclusive_jets(pruning_jetPtMin_));
      std::vector<fastjet::PseudoJet> out_jets_basic_noPtCut = sorted_by_pt(thisClustering_basic.inclusive_jets(0));
      if(out_jets_basic.size() != 1){
         if( debug_ ) std::cout<<"WARNING ... out_jets_basic.size : "<<out_jets_basic.size()<<"  NOT 1"<<std::endl;
//         continue;
      }
     
      if( out_jets_basic.size() ==1 && groomingOpt_ ==1 ){
         fastjet::PseudoJet prunedJet = pruner(out_jets_basic.at(0));

         TLorentzVector perJet;
         perJet.SetPxPyPzE( prunedJet.px(), prunedJet.py(), prunedJet.pz(), prunedJet.e() );
         outputJets.push_back(perJet);
      }else{
         TLorentzVector perJet;
         perJet.SetPxPyPzE( out_jets_basic_noPtCut.at(0).px(), out_jets_basic_noPtCut.at(0).py(), out_jets_basic_noPtCut.at(0).pz(), out_jets_basic_noPtCut.at(0).e() );
         outputJets.push_back(perJet);
      }
   }

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( groomProd );

