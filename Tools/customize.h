#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <string>

topTagger::type3TopTagger * type3Ptr;

namespace AnaConsts{
   const int nMuonsSel = 0, nElectronsSel = 0, nTausSel = 0, nIsoTrksSel = 0;

   const int nJetsSel = 4, nJetsSelPt30Eta24 = 4, nJetsSelPt50Eta24 = 2, nJetsSelPt70Eta24 = 2;
// [low_nJetsSelBtagged, high_nJetsSelBtagged)
   const int low_nJetsSelBtagged = 1, high_nJetsSelBtagged = -1;
// [low_nTopCandSortedSel, high_nTopCandSortedSel)
   const int low_nTopCandSortedSel = 1, high_nTopCandSortedSel = -1;
   const double minJetPt = 30;
//                                 minAbsEta, maxAbsEta, minPt, maxPt
   const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
   const double pt30Eta24Arr[] = {   -1,       2.4,      30,    -1  };
   const double pt50Eta24Arr[] = {   -1,       2.4,      50,    -1  };
   const double      dphiArr[] = {   -1,       4.7,      30,    -1  };
   const double      bTagArr[] = {   -1,       2.4,      30,    -1  };

   const double cutCSVS = 0.814, cutCSVSold = 0.679; // for T5tttt signals, currently old b-tagging was used
   const double defaultMETcut = 200;

//                                 minAbsEta, maxAbsEta, minPt, maxPt, maxrelIso, maxMtw
   const double     muonsArr[] = {   -1,       2.4,      5,     -1,       0.2,     -1  };
   const double      elesArr[] = {   -1,       2.5,      5,     -1,      0.15,     -1  };
   const double   isoTrksArr[] = {   -1,        -1,     10,     -1,       0.1,    100  };

   const double  dPhi0_CUT = 0.5, dPhi1_CUT = 0.5, dPhi2_CUT = 0.3;

   const double lowTopCut_ = 80, highTopCut_ = 270, lowWCut_ = 50, highWCut_ = 120;

   const double MT2cut_ = 300, mTcombcut_ = 500;

   std::set<std::string> activatedBranchNames = { "run", "lumi", "event", "evtWeight", 
                                                  "genDecayIdxVec", "genDecayLVec", "genDecayMomIdxVec", "genDecayPdgIdVec", "genDecayStrVec", 
                                                  "W_emuVec", "W_tau_emuVec", "W_tau_prongsVec", 
                                                  "jetsLVec", "recoJetsBtag_0", 
//                                                  "muonsCharge", "muonsLVec", "muonsMtw", "muonsRelIso", 
                                                  "muonsLVec", "muonsMtw", "muonsRelIso", 
//                                                  "elesCharge", "elesLVec", "elesMtw", "elesRelIso", 
                                                  "elesLVec", "elesMtw", "elesRelIso", 
//                                                  "loose_isoTrksLVec", "loose_isoTrks_charge", "loose_isoTrks_dz", "loose_isoTrks_iso", "loose_isoTrks_mtw", 
                                                  "loose_isoTrksLVec", "loose_isoTrks_iso", "loose_isoTrks_mtw", 
                                                  "met", "metphi", 
                                                  "tru_npv", "vtxSize" 
                                                };
}

namespace AnaFunctions{

   int countJets(const std::vector<TLorentzVector> &inputJets, const double *jetCutsArr){
      const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];
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

   int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double cutCSVS, const double *jetCutsArr){
      const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];
      int cntNJets =0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
         if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
            && (     minPt == -1 || perjetpt >= minPt )
            && (     maxPt == -1 || perjetpt < maxPt ) ){

            if( inputCSVS[ij] > cutCSVS ) cntNJets ++;
         }
      }
      return cntNJets;
   }

   std::vector<double> calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr){
      const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];
      int cntNJets =0;
      std::vector<double> outDPhiVec(nDPhi, 999);
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

   int countMuons(const std::vector<TLorentzVector> &muonsLVec, const std::vector<double> &muonsRelIso, const std::vector<double> &muonsMtw, const double *muonsArr){
      const double minAbsEta = muonsArr[0], maxAbsEta = muonsArr[1], minPt = muonsArr[2], maxPt = muonsArr[3], maxRelIso = muonsArr[4], maxMtw = muonsArr[5];
      int cntNMuons = 0;
      for(unsigned int im=0; im<muonsLVec.size(); im++){
         double permuonpt = muonsLVec[im].Pt(), permuoneta = muonsLVec[im].Eta();
         if(   ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
            && (     minPt == -1 || permuonpt >= minPt )
            && (     maxPt == -1 || permuonpt < maxPt ) ){

            if( maxRelIso != -1 && muonsRelIso[im] >= maxRelIso ) continue;
            if( maxMtw != -1 && muonsMtw[im] >= maxMtw ) continue;

            cntNMuons ++;
         }
      }
      return cntNMuons;
   }

   int countElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<double> &electronsRelIso, const std::vector<double> &electronsMtw, const double *elesArr){
      const double minAbsEta = elesArr[0], maxAbsEta = elesArr[1], minPt = elesArr[2], maxPt = elesArr[3], maxRelIso = elesArr[4], maxMtw = elesArr[5];
      int cntNElectrons = 0;
      for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
         double perelectronpt = electronsLVec[ie].Pt(), perelectroneta = electronsLVec[ie].Eta();
         if(   ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
            && (     minPt == -1 || perelectronpt >= minPt )
            && (     maxPt == -1 || perelectronpt < maxPt ) ){

            if( maxRelIso != -1 && electronsRelIso[ie] >= maxRelIso ) continue;
            if( maxMtw != -1 && electronsMtw[ie] >= maxMtw ) continue;

            cntNElectrons ++;
         }
      }
      return cntNElectrons;
   }

   int countIsoTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<double> &isoTrksIso, const std::vector<double> &isoTrksMtw, const double *isoTrksArr){
      const double minAbsEta = isoTrksArr[0], maxAbsEta = isoTrksArr[1], minPt = isoTrksArr[2], maxPt = isoTrksArr[3], maxRelIso = isoTrksArr[4], maxMtw = isoTrksArr[5];
      int cntNIsoTrks = 0;
      for(unsigned int is=0; is<isoTrksLVec.size(); is++){
         double perisotrkpt = isoTrksLVec[is].Pt(), perisotrketa = isoTrksLVec[is].Eta();
         if(   ( minAbsEta == -1 || fabs(perisotrketa) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perisotrketa) < maxAbsEta )
            && (     minPt == -1 || perisotrkpt >= minPt )
            && (     maxPt == -1 || perisotrkpt < maxPt ) ){

            if( maxRelIso != -1 && isoTrksIso[is]/perisotrkpt >= maxRelIso ) continue;
            if( maxMtw != -1 && isoTrksMtw[is] >= maxMtw ) continue;

            cntNIsoTrks ++;
         }
      }
      return cntNIsoTrks;
   }

   void prepareJetsForTagger(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<double> &inirecoJetsBtag, std::vector<TLorentzVector> &jetsLVec_forTagger, std::vector<double> &recoJetsBtag_forTagger){

      jetsLVec_forTagger.clear(); recoJetsBtag_forTagger.clear();

      for(unsigned int ij=0; ij<inijetsLVec.size(); ij++){
         std::vector<TLorentzVector> dummyJetsVec;
         dummyJetsVec.push_back(inijetsLVec.at(ij));
         int dummycntNJetsPt30 = countJets(dummyJetsVec, AnaConsts::pt30Arr);
         if( !dummycntNJetsPt30 ) continue;
         jetsLVec_forTagger.push_back(inijetsLVec.at(ij));
         recoJetsBtag_forTagger.push_back(inirecoJetsBtag.at(ij));
      }
   }

// cntNJetsVec stores number of jets counters from tightest requirement to loosest requirement.
// cutCSVS is the CSV cut value
   void preparecntNJets(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<double> &inirecoJetsBtag, const double cutCSVS, std::vector<int> &cntNJetsVec, std::vector<int> &cntNbJetsVec){
      cntNJetsVec.clear(); cntNbJetsVec.clear();
      int cntNJetsPt50Eta24 = countJets(inijetsLVec, AnaConsts::pt50Eta24Arr);
      int cntNJetsPt30Eta24 = countJets(inijetsLVec, AnaConsts::pt30Eta24Arr);
      int cntNJetsPt30      = countJets(inijetsLVec, AnaConsts::pt30Arr);
      cntNJetsVec.push_back(cntNJetsPt50Eta24);
      cntNJetsVec.push_back(cntNJetsPt30Eta24);
      cntNJetsVec.push_back(cntNJetsPt30);

      int cntCSVS = countCSVS(inijetsLVec, inirecoJetsBtag, cutCSVS, AnaConsts::bTagArr);
      cntNbJetsVec.push_back(cntCSVS);
   }

   void preparecalcDPhi(const std::vector<TLorentzVector> &inijetsLVec, const double metphi, std::vector<double> &outDPhiVec){
      outDPhiVec.clear();
      outDPhiVec = calcDPhi(inijetsLVec, metphi, 3, AnaConsts::dphiArr);
   }

   void prepareTopTagger(){
      if( !type3Ptr ) type3Ptr = new topTagger::type3TopTagger();
      type3Ptr->setnJetsSel(AnaConsts::nJetsSel);
   }

   bool passBaseline(){

   }

}
