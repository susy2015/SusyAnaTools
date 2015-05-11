#ifndef ANACONST_CUSTOMIZE_H
#define ANACONST_CUSTOMIZE_H

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <string>
/*
   This file is used to customize the NTupleReader and the flat ntuples for our baseline and search region definitions.
   Currently these include jet pt, eta requirements for counting, functions to calculate nJets, nbJets, deltaphi, and so on.
   The functions here have nothing to do with NTupleReader so that can be used in other cases independent of the reader.
   An example baseline cut flow is defined in a separate code, i.e., baselineDef.C.
*/

topTagger::type3TopTagger * type3Ptr;

namespace AnaConsts{
    
    struct AccRec
    {
        double minAbsEta, maxAbsEta, minPt, maxPt;
    };

    struct IsoAccRec
    {
        double minAbsEta, maxAbsEta, minPt, maxPt, maxIso, maxMtw;
    };

    struct ElecIsoAccRec
    {
        double minAbsEta, maxAbsEta, minPt, maxPt, maxIsoEB, maxIsoEE, maxMtw;
    };

    struct ActRec
    {
        double minAbsEta, maxAbsEta, minPt, maxPt, mindR, maxdR;
    };

    
   const int nMuonsSel = 0, nElectronsSel = 0, nTausSel = 0, nIsoTrksSel = 0;

   const int nJetsSel = 4, nJetsSelPt30Eta24 = 4, nJetsSelPt50Eta24 = 2, nJetsSelPt70Eta24 = 2;
// [low_nJetsSelBtagged, high_nJetsSelBtagged)
   const int low_nJetsSelBtagged = 1, high_nJetsSelBtagged = -1;
// [low_nTopCandSortedSel, high_nTopCandSortedSel)
   const int low_nTopCandSortedSel = 1, high_nTopCandSortedSel = -1;
   const double minJetPt = 30;
//                               minAbsEta, maxAbsEta, minPt, maxPt
   const AccRec      pt30Arr = {   -1,        -1,      30,    -1  };
   const AccRec pt30Eta24Arr = {   -1,       2.4,      30,    -1  };
   const AccRec pt50Eta24Arr = {   -1,       2.4,      50,    -1  };
   const AccRec      dphiArr = {   -1,       4.7,      30,    -1  };
   const AccRec      bTagArr = {   -1,       2.4,      30,    -1  };

   const double cutCSVS = 0.814, cutCSVSold = 0.679; // for T5tttt signals, currently old b-tagging was used
   const double defaultMETcut = 200;

//                                    minAbsEta, maxAbsEta, minPt, maxPt,   maxIso,  maxMtw
   const IsoAccRec     muonsArr =    {   -1,       2.4,      5,     -1,       0.2,     -1  };
   const IsoAccRec muonsMiniIsoArr = {   -1,       2.4,      5,     -1,       0.1,     -1  };
   const IsoAccRec muonsTrigArr =    {   -1,       2.4,      5,     -1,       0.4,     -1  };
   const IsoAccRec   isoTrksArr =    {   -1,        -1,      10,     -1,       0.1,    100 };

//                                    minAbsEta, maxAbsEta, minPt, maxPt, maxIsoEB, maxIsoEE,  maxMtw
   const ElecIsoAccRec  elesArr =    {   -1,       2.5,      5,     -1,  0.164369, 0.1212604,    -1  };


//                                  minAbsEta, maxAbsEta, minPt, maxPt,   mindR,   maxdR
   const ActRec     muonsAct =    {   -1,       -1,        10,    -1,       -1,     1.0  };
   const ActRec      elesAct =    {   -1,       -1,        10,    -1,       -1,     1.0  };

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

   bool jetPassCuts(const TLorentzVector& jet, const AnaConsts::AccRec& jetCutsArr)
   {
       const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
       double perjetpt = jet.Pt(), perjeteta = jet.Eta();
       return  ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
           && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
           && (     minPt == -1 || perjetpt >= minPt )
           && (     maxPt == -1 || perjetpt < maxPt );
   }       

   int countJets(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){
      int cntNJets =0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
          if(jetPassCuts(inputJets[ij], jetCutsArr)) cntNJets ++;
      }
      return cntNJets;
   }

   int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double cutCSVS, const AnaConsts::AccRec& jetCutsArr){
      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
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

   std::vector<double> calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const AnaConsts::AccRec& jetCutsArr){
      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
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
   
   bool passMuon(const TLorentzVector& muon, const double& muonIso, const double& muonMtw, const AnaConsts::IsoAccRec& muonsArr)
   {
      const double minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt, maxIso = muonsArr.maxIso, maxMtw = muonsArr.maxMtw; 
      double permuonpt = muon.Pt(), permuoneta = muon.Eta();
      return ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
          && (     minPt == -1 || permuonpt >= minPt )
          && (     maxPt == -1 || permuonpt < maxPt )
          && (    maxIso == -1 || muonIso < maxIso)
          &&     (maxMtw == -1 || muonMtw < maxMtw);
   }

   bool passMuonAccOnly(const TLorentzVector& muon, const AnaConsts::IsoAccRec& muonsArr)
   {
      const double minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt;
      double permuonpt = muon.Pt(), permuoneta = muon.Eta();
      return ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
          && (     minPt == -1 || permuonpt >= minPt )
          && (     maxPt == -1 || permuonpt < maxPt );
   }
   
   int countMuons(const std::vector<TLorentzVector> &muonsLVec, const std::vector<double> &muonsRelIso, const std::vector<double> &muonsMtw, const AnaConsts::IsoAccRec& muonsArr){
      
      int cntNMuons = 0;
      for(unsigned int im=0; im<muonsLVec.size(); im++){
          if(passMuon(muonsLVec[im], muonsRelIso[im], muonsMtw[im], muonsArr)) cntNMuons ++;
      }
      return cntNMuons;
   }

   bool passElectron(const TLorentzVector& elec, const double electronIso, const double electronMtw, bool isEB, const AnaConsts::ElecIsoAccRec& elesArr)
   {
       const double minAbsEta = elesArr.minAbsEta, maxAbsEta = elesArr.maxAbsEta, minPt = elesArr.minPt, maxPt = elesArr.maxPt, maxIso = (isEB)?(elesArr.maxIsoEB):(elesArr.maxIsoEE), maxMtw = elesArr.maxMtw;
       double perelectronpt = elec.Pt(), perelectroneta = elec.Eta();
       return ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
           && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
           && (     minPt == -1 || perelectronpt >= minPt )
           && (     maxPt == -1 || perelectronpt < maxPt ) 
           && (    maxIso == -1 || electronIso < maxIso )
           && (    maxMtw == -1 || electronMtw < maxMtw );       
   }

   int countElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<double> &electronsRelIso, const std::vector<double> &electronsMtw, const std::vector<unsigned int>& isEBVec, const AnaConsts::ElecIsoAccRec& elesArr){

      int cntNElectrons = 0;
      for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
          if(passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], isEBVec[ie], elesArr)) cntNElectrons ++;
      }
      return cntNElectrons;
   }

   double getElectronActivity(const TLorentzVector& elec, const std::vector<TLorentzVector>& jetLVec, const std::vector<double>& recoJetschargedHadronEnergyFraction, const AnaConsts::ActRec& elesAct)
   {
       double activity = 0;
       for( unsigned int i = 0 ; i < jetLVec.size() ; i++ )
       {
           if( ROOT::Math::VectorUtil::DeltaR(elec , jetLVec.at(i)) < elesAct.maxdR && jetLVec.at(i).Pt() > elesAct.minPt )
//for now, we study the activity integral over jetpt > 10, we will lower the pt cut later
           {
               activity += jetLVec.at(i).Pt() * recoJetschargedHadronEnergyFraction.at(i);
           }
           else
               continue;
       }
       return activity;
   }

   double getMuonActivity(const TLorentzVector& muon, const std::vector<TLorentzVector>& jetLVec, const std::vector<double>& recoJetschargedHadronEnergyFraction, const std::vector<double>& recoJetschargedEmEnergyFraction, const AnaConsts::ActRec& muonsAct)
   {
       double activity = 0;
       for( unsigned int i = 0 ; i < jetLVec.size() ; i++ )
       {
           if( ROOT::Math::VectorUtil::DeltaR(muon , jetLVec.at(i)) < muonsAct.maxdR && jetLVec.at(i).Pt() > muonsAct.minPt )
//for now, we study the activity integral over jetpt > 10, we will lower the pt cut later
           {
               activity += jetLVec.at(i).Pt() * (recoJetschargedEmEnergyFraction.at(i) + recoJetschargedHadronEnergyFraction.at(i));
           }
           else
               continue;
       }
       return activity;
   }


   int countIsoTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<double> &isoTrksIso, const std::vector<double> &isoTrksMtw, const AnaConsts::IsoAccRec& isoTrksArr){
      const double minAbsEta = isoTrksArr.minAbsEta, maxAbsEta = isoTrksArr.maxAbsEta, minPt = isoTrksArr.minPt, maxPt = isoTrksArr.maxPt, maxIso = isoTrksArr.maxIso, maxMtw = isoTrksArr.maxMtw;
      int cntNIsoTrks = 0;
      for(unsigned int is=0; is<isoTrksLVec.size(); is++){
         double perisotrkpt = isoTrksLVec[is].Pt(), perisotrketa = isoTrksLVec[is].Eta();
         if(   ( minAbsEta == -1 || fabs(perisotrketa) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perisotrketa) < maxAbsEta )
            && (     minPt == -1 || perisotrkpt >= minPt )
            && (     maxPt == -1 || perisotrkpt < maxPt ) ){

            if( maxIso != -1 && isoTrksIso[is]/perisotrkpt >= maxIso ) continue;
            if( maxMtw != -1 && isoTrksMtw[is] >= maxMtw ) continue;

            cntNIsoTrks ++;
         }
      }
      //WARNING  HERE IS A HACK TO DEACTIVATE THIS CUT FOR NOW!!!
      return 0;//cntNIsoTrks;
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

   int jetLepdRMatch(const TLorentzVector& lep, const std::vector<TLorentzVector>& jetsLVec, const double jldRMax)
   {
       double dRmin = 999.0;
       int minJMatch = -1;
                
       for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
       {
           double dR = ROOT::Math::VectorUtil::DeltaR(jetsLVec[iJet], lep);
           if(dR < dRmin)
           {
               dRmin = dR;
               minJMatch = iJet;
           }
       }
       if(dRmin < jldRMax) return minJMatch;
       else                return -1;
   }

}

#endif
