#ifndef ANACONST_CUSTOMIZE_H
#define ANACONST_CUSTOMIZE_H

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

// Top Tagger
#include "TopTagger/TopTagger/interface/TopTagger.h"
#include "TopTagger/TopTagger/interface/TopTaggerResults.h"
#include "TopTagger/TopTagger/interface/TopTaggerUtilities.h"

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

namespace AnaConsts{
    
    struct AccRec
    {
        float minAbsEta, maxAbsEta, minPt, maxPt;
    };

    struct IsoAccRec
    {
        float minAbsEta, maxAbsEta, minPt, maxPt, maxIso, maxMtw;
    };

    struct ActRec
    {
        float minAbsEta, maxAbsEta, minPt, maxPt, mindR, maxdR;
    };

    
   const int nMuonsSel = 0, nElectronsSel = 0, nTausSel = 0, nIsoTracksSel = 0;

   const int nJetsSel = 4, nJetsSelPt30Eta24 = 4, nJetsSelPt50Eta24 = 2, nJetsSelPt70Eta24 = 2, nJetsSelPt20Eta24 = 2;
// [low_nJetsSelBtagged, high_nJetsSelBtagged)
   const int low_nJetsSelBtagged = 1, high_nJetsSelBtagged = -1;
// [low_nTopCandSortedSel, high_nTopCandSortedSel)
   const int low_nTopCandSortedSel = 1, high_nTopCandSortedSel = -1;
   const float minJetPt = 30;
//                               minAbsEta, maxAbsEta, minPt, maxPt
   const AccRec pt30Arr       = {   -1,        -1,      30,    -1  };
   const AccRec pt20Eta24Arr  = {   -1,       2.4,      20,    -1  };
   const AccRec pt30Eta24Arr  = {   -1,       2.4,      30,    -1  };
   const AccRec pt40Eta24Arr  = {   -1,       2.4,      40,    -1  };
   const AccRec pt50Eta24Arr  = {   -1,       2.4,      50,    -1  };
   const AccRec pt200Eta24Arr = {   -1,       2.4,     200,    -1  };
   const AccRec pt20Eta47Arr  = {   -1,       4.7,      20,    -1  };
   const AccRec pt30Eta47Arr  = {   -1,       4.7,      30,    -1  };
   const AccRec pt40Eta47Arr  = {   -1,       4.7,      40,    -1  };
   const AccRec pt50Eta47Arr  = {   -1,       4.7,      50,    -1  };
   const AccRec dphiNArr      = {   -1,       2.4,      30,    -1  };
   const AccRec bTagArr       = {   -1,       2.4,      20,    -1  };
   const AccRec pt20Eta25Arr  = {   -1,       2.5,      20,    -1  };

//   const float cutCSVS = 0.814, cutCSVSold = 0.679; // for T5tttt signals, currently old b-tagging was used
// Note the new working points are for Spring15 samples & data: cutCSVS is the medium working point
// According to https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80X, the CSVv2M cut is 0.800
   const float cutCSVS = 0.8484, cutCSVL = 0.5426, cutCSVT = 0.9535, cutCSVSold = 0.800; // old is for ICHEP working point
   
   // Used For Post-Production v2.7:
   //2016 Data and  8_0_X MC: https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#Data_MC_Scale_Factors_period_dep
   //2017 Data and  9_4_X MC: https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation94X
   //2018 Data and 10_2_X MC: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X

   // Updated 2016 for next production:
   //2016 Legacy: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy

   // CSVv2 btag working points
   const std::map<std::string, std::map<std::string, float > > CSVv2 = {
     { "2016", {
                   {"cutL", 0.5426},
                   {"cutM", 0.8484},
                   {"cutT", 0.9535}
 
     }},
     { "2017", {
                   {"cutL", 0.5803},
                   {"cutM", 0.8838},
                   {"cutT", 0.9693}
 
     }},
     // not from recommendation; use 2017 for now
     { "2018", {
                   {"cutL", 0.5803},
                   {"cutM", 0.8838},
                   {"cutT", 0.9693}
 
     }},
   };
   // DeepCSV btag working points
   const std::map<std::string, std::map<std::string, float > > DeepCSV = {
     { "2016", {
                   {"cutL", 0.2219},
                   {"cutM", 0.6324},
                   {"cutT", 0.8958}
 
     }},
     { "2017", {
                   {"cutL", 0.1522},
                   {"cutM", 0.4941},
                   {"cutT", 0.8001}
 
     }},
     { "2018", {
                   {"cutL", 0.1241},
                   {"cutM", 0.4184},
                   {"cutT", 0.7527}
 
     }},
   };
   const float defaultMETcut = 250;
   const float defaultMT2cut = 200;
   const float defaultHTcut  = 300;

//                                    minAbsEta, maxAbsEta, minPt, maxPt,   maxIso,  maxMtw
   const IsoAccRec muonsArr        = {   -1,       2.4,      10,     -1,       0.2,     -1  };
   const IsoAccRec muonsMiniIsoArr = {   -1,       2.4,       5,     -1,       0.2,     -1  };
   const IsoAccRec muonsTrigArr    = {   -1,       2.4,       5,     -1,       0.4,     -1  };
//   const IsoAccRec isoTrksArr      = {   -1,        -1,      10,     -1,       0.1,    100  };
   const IsoAccRec isoLepTrksArr   = {   -1,       2.5,       5,     -1,       0.2,    100  };
   const IsoAccRec isoHadTrksArr   = {   -1,       2.5,      10,     -1,       0.1,    100  };
//   const IsoAccRec isoTrksArr      = {   -1,        -1,      10,     -1,       0.1,    120  };

//                                       minAbsEta, maxAbsEta, minPt, maxPt, maxIsoEB, maxIsoEE,  maxMtw
   //const ElecIsoAccRec elesArr        = {   -1,       2.5,      10,     -1,  0.164369, 0.212604,    -1  };
   const IsoAccRec elesArr        = {   -1,       2.5,      10,     -1,     0.10,    -1  };
   const IsoAccRec elesMiniIsoArr = {   -1,       2.5,       5,     -1,     0.10,    -1  };
   const IsoAccRec oldelesArr     = {   -1,       2.5,       5,     -1,     0.15,    -1  };

//                                       minAbsEta, maxAbsEta, minPt, maxPt,   mindR,   maxdR
   const ActRec muonsAct              = {   -1,       -1,        10,    -1,     -1,      1.0  };
   const ActRec elesAct               = {   -1,       -1,        10,    -1,     -1,      1.0  };

   const float  dPhi0_CUT = 0.5, dPhi1_CUT = 0.5, dPhi2_CUT = 0.3;
   const float  minDPhiN_CUT = 6;

   const float MT2cut_ = 300, mTcombcut_ = 500;

   static std::set<std::string> activatedBranchNames_DataOnly = { "run", "lumi", "event", "evtWeight", 
                                                  "jetsLVec", "recoJetsCSVv2", "muMatchedJetIdx", "eleMatchedJetIdx", "looseisoTrksMatchedJetIdx", "trksForIsoVetoMatchedJetIdx",
                                                  "recoJetsJecUnc", "recoJetsJecScaleRawToFull",
                                                  "muonsLVec", "muonsMtw", "muonsRelIso", "muonsMiniIso", "muonsFlagMedium", "muonsFlagTight", "muonspfActivity",
                                                  "elesLVec", "elesMtw", "elesRelIso", "elesMiniIso", "elesisEB", "elesFlagVeto", "elesFlagMedium", "elespfActivity",
                                                  "loose_isoTrksLVec", "loose_isoTrks_iso", "loose_isoTrks_mtw", "loose_isoTrks_pdgId",
                                                  "met", "metphi", 
                                                  "tru_npv", "vtxSize",
                                                  "prodJetsNoLep_recoJetsCSVv2", "prodJetsNoLep_jetsLVec",
                          "recoJetschargedEmEnergyFraction", "recoJetsneutralEmEnergyFraction", "recoJetschargedHadronEnergyFraction", "recoJetsmuonEnergyFraction",
//                                                  "AK4looseJetID", "tightJetID", "AK4NoLeplooseJetID", "tightJetID_NoLep", "CSCTightHaloFilter", "EcalDeadCellTriggerPrimitiveFilter", "HBHENoiseFilter", "HBHEIsoNoiseFilter",
                                                  "AK4looseJetID", "tightJetID", "AK4NoLeplooseJetID", "tightJetID_NoLep", "EcalDeadCellTriggerPrimitiveFilter", "HBHENoiseFilter", "HBHEIsoNoiseFilter", "goodVerticesFilter", "eeBadScFilter", "eeBadScListFilter", "CSCTightHaloListFilter", "badResolutionTrackListFilter", "muonBadTrackListFilter",
                                                  "TriggerNames", "PassTrigger", "TriggerPrescales", 
                                                  "stored_weight",
                                                  "globalTightHalo2016Filter", "BadChargedCandidateFilter", "BadPFMuonFilter",
                                                  "calomet"
                                                };

   static std::set<std::string> activatedBranchNames = {
                                                  // Only exist in MC samples
                                                  "genDecayIdxVec", "genDecayLVec", "genDecayMomIdxVec", "genDecayPdgIdVec", "genDecayStrVec", "genDecayMomRefVec",
                                                  "W_emuVec", "W_tau_emuVec", "W_tau_prongsVec", "W_tau_nuVec",
                                                  "genHT", "PDFweights", "PDFids",
                                                  "id1", "id2", "x1", "x2", "q",
                                                  "W_emu_pfActivityVec", "W_tau_emu_pfActivityVec", "W_tau_prongs_pfActivityVec", "ScaleWeightsMiniAOD",
                                                  "prodJetsNoLep_recoJetsJecUnc", "metMagUp", "metMagDown", "metPhiUp", "metPhiDown",
                                                  "SusyMotherMass", "SusyLSPMass", "recoJetsFlavor",
                                                  "genjetsLVec", "genmet", "genmetphi",
                                                  "NJetsISR",
                                                  "specialFixtype", "specialFixMuonsCharge", "specialFixMuonsLVec"
                                                };

}

namespace AnaFunctions{
  bool jetPassCuts(const TLorentzVector& jet, const AnaConsts::AccRec& jetCutsArr);
  int countJets(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr);
  int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<float> &inputCSVS, const float cutCSVS, const AnaConsts::AccRec& jetCutsArr, std::vector<unsigned int> *outputIdxs=NULL);
  std::vector<float> calcDPhi(const std::vector<TLorentzVector> &inputJets, const TLorentzVector &metLVec, const int nDPhi, const AnaConsts::AccRec& jetCutsArr);
  std::vector<float> calcDPhiN(const std::vector<TLorentzVector> &inputJets, const TLorentzVector &metLVec, const int nDPhi, const AnaConsts::AccRec& jetCutsArr);
  float calcDeltaT(unsigned int pickedJetIdx, const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr);
  bool passMuon(const TLorentzVector& muon, const float& muonIso, const float& muonMtw, unsigned char flagID, const AnaConsts::IsoAccRec& muonsArr);
  bool passMuonAccOnly(const TLorentzVector& muon, const AnaConsts::IsoAccRec& muonsArr);
  int countMuons(const std::vector<TLorentzVector> &muonsLVec, const std::vector<float> &muonsRelIso, const std::vector<float> &muonsMtw, const std::vector<unsigned char> &muonsFlagID, const AnaConsts::IsoAccRec& muonsArr);
  bool passElectron(const TLorentzVector& elec, const float electronIso, const float electronMtw, unsigned char flagID, const AnaConsts::IsoAccRec& elesArr);
  int countOldElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<float> &electronsRelIso, const std::vector<float> &electronsMtw, const std::vector<int> &electronsFlagID, const AnaConsts::IsoAccRec& elesArr);
  int countElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<float> &electronsRelIso, const std::vector<float> &electronsMtw, const std::vector<unsigned int>& isEBVec, const std::vector<unsigned char> &electronsFlagID, const AnaConsts::IsoAccRec& elesArr);
  float getElectronActivity(const TLorentzVector& elec, const std::vector<TLorentzVector>& jetLVec, const std::vector<float>& recoJetschargedHadronEnergyFraction, const AnaConsts::ActRec& elesAct);
  float getMuonActivity(const TLorentzVector& muon, const std::vector<TLorentzVector>& jetLVec, const std::vector<float>& recoJetschargedHadronEnergyFraction, const std::vector<float>& recoJetschargedEmEnergyFraction, const AnaConsts::ActRec& muonsAct);
  bool passIsoTrk(const TLorentzVector& isoTrk, const float isoTrkIso, const float isoTrkMtw, const AnaConsts::IsoAccRec& isoTrksArr);
  int countIsoTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId);
  void prepareJetsForTagger(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<float> &inirecoJetsBtag, std::vector<TLorentzVector> &jetsLVec_forTagger, std::vector<float> &recoJetsBtag_forTagger, const std::vector<float>& qgLikelihood = *static_cast<std::vector<float>*>(nullptr), std::vector<float>& qgLikelihood_forTagger = *static_cast<std::vector<float>*>(nullptr));
  void preparecntNJets(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<float> &inirecoJetsBtag, const float cutCSVS, std::vector<int> &cntNJetsVec, std::vector<int> &cntNbJetsVec);
  void preparecalcDPhi(const std::vector<TLorentzVector> &inijetsLVec, const TLorentzVector &metLVec, std::vector<float> &outDPhiVec);
  void prepareForNtupleReader();
  float calcHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr);
  bool passBaseline();
  // match dR between jet and object (photon, lepton, etc)
  int jetObjectdRMatch(const TLorentzVector& object, const std::vector<TLorentzVector>& jetsLVec, const float jetObjectdRMax, std::vector<float>* dRvec);
  // true if jet matches object, false otherwise
  std::vector<bool> getJetMatchesObjectVec(const std::vector<TLorentzVector>& Jet_TLV, const std::vector<TLorentzVector>& Object_TLV, const std::vector<int>& Object_JetIndex, const float& DRMAX);
  std::vector<bool> getJetMatchesObjectVec(const std::vector<TLorentzVector>& Jet_TLV, const std::vector<TLorentzVector>& Object_TLV, const float& DRMAX);
  bool passElectronAccOnly(const TLorentzVector& elec, const AnaConsts::IsoAccRec& elesArr);
  TLorentzVector calcMHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr);
  int countIsoLepTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId);
  int countIsoPionTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId);

}

#endif
