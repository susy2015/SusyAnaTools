#include "NTupleReader.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "samples.h"
#include "customize.h"

#include "TStopwatch.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"

#include "tdrstyle.h"

const bool debug = false;

std::vector<std::string> keyStringCachedVec;
std::vector<double> scaleMCCachedVec;
std::vector<int> colorCachedVec;

double cnt_passLeptVeto_WeightedScaledMC = 0, cnt_passLeptVeto_WeightedErrorScaledMC = 0;
double cnt_passnJets_WeightedScaledMC = 0, cnt_passnJets_WeightedErrorScaledMC = 0;
double cnt_passdPhis_WeightedScaledMC = 0, cnt_passdPhis_WeightedErrorScaledMC = 0;
double cnt_passBJets_WeightedScaledMC = 0, cnt_passBJets_WeightedErrorScaledMC = 0;
double cnt_passMET_WeightedScaledMC = 0, cnt_passMET_WeightedErrorScaledMC = 0;
double cnt_passTagger_WeightedScaledMC = 0, cnt_passTagger_WeightedErrorScaledMC = 0;
double cnt_passBaseline_WeightedScaledMC = 0, cnt_passBaseline_WeightedErrorScaledMC = 0;

std::vector<TH2D*> h2_evtCnt_nbJets_vs_nTopsVec;
TH2D * h2_evtCnt_sumSM_nbJets_vs_nTops = 0;

TStopwatch timer;

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH1D*> &h1_inputVec, const int nRebin =1, const TString optDrawStr ="");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2D*> &h2_inputVec, const TString optDrawStr = "");

void passBaselineFunc(NTupleReader &tr){
   bool passBaseline = true;

// Form TLorentzVector of MET
   TLorentzVector metLVec; metLVec.SetPtEtaPhiM(tr.getVar<double>("met"), 0, tr.getVar<double>("metphi"), 0);

// Calculate number of leptons
   int nMuons = AnaFunctions::countMuons(tr.getVec<TLorentzVector>("muonsLVec"), tr.getVec<double>("muonsRelIso"), tr.getVec<double>("muonsMtw"), AnaConsts::muonsArr);
   int nElectrons = AnaFunctions::countElectrons(tr.getVec<TLorentzVector>("elesLVec"), tr.getVec<double>("elesRelIso"), tr.getVec<double>("elesMtw"), AnaConsts::elesArr);
   int nIsoTrks = AnaFunctions::countIsoTrks(tr.getVec<TLorentzVector>("loose_isoTrksLVec"), tr.getVec<double>("loose_isoTrks_iso"), tr.getVec<double>("loose_isoTrks_mtw"), AnaConsts::isoTrksArr);

// Calculate number of jets and b-tagged jets
   int cntCSVS = AnaFunctions::countCSVS(tr.getVec<TLorentzVector>("jetsLVec"), tr.getVec<double>("recoJetsBtag_0"), AnaConsts::cutCSVS, AnaConsts::bTagArr);
   int cntNJetsPt50Eta24 = AnaFunctions::countJets(tr.getVec<TLorentzVector>("jetsLVec"), AnaConsts::pt50Eta24Arr);
   int cntNJetsPt30Eta24 = AnaFunctions::countJets(tr.getVec<TLorentzVector>("jetsLVec"), AnaConsts::pt30Eta24Arr);
   int cntNJetsPt30      = AnaFunctions::countJets(tr.getVec<TLorentzVector>("jetsLVec"), AnaConsts::pt30Arr);

// Calculate deltaPhi
   std::vector<double> * dPhiVec = new std::vector<double>();
   (*dPhiVec) = AnaFunctions::calcDPhi(tr.getVec<TLorentzVector>("jetsLVec"), tr.getVar<double>("metphi"), 3, AnaConsts::dphiArr);

// Prepare jets and b-tag working points for top tagger
   std::vector<TLorentzVector> *jetsLVec_forTagger = new std::vector<TLorentzVector>(); std::vector<double> *recoJetsBtag_forTagger = new std::vector<double>();
   AnaFunctions::prepareJetsForTagger(tr.getVec<TLorentzVector>("jetsLVec"), tr.getVec<double>("recoJetsBtag_0"), (*jetsLVec_forTagger), (*recoJetsBtag_forTagger));
   if( debug ) std::cout<<"\njetsLVec_forTagger->size : "<<jetsLVec_forTagger->size()<<"  recoJetsBtag_forTagger->size : "<<recoJetsBtag_forTagger->size()<<"  passBaseline : "<<passBaseline<<std::endl;

// Pass lepton veto?
   bool passLeptVeto = true;
   if( nMuons != AnaConsts::nMuonsSel ){ passBaseline = false; passLeptVeto = false; }
   if( nElectrons != AnaConsts::nElectronsSel ){ passBaseline = false; passLeptVeto = false; }
   if( nIsoTrks != AnaConsts::nIsoTrksSel ){ passBaseline = false; passLeptVeto = false; }
   if( debug ) std::cout<<"nMuons : "<<nMuons<<"  nElectrons : "<<nElectrons<<"  nIsoTrks : "<<nIsoTrks<<"  passBaseline : "<<passBaseline<<std::endl;

// Pass number of jets?
   bool passnJets = true;
   if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ){ passBaseline = false; passnJets = false; }
   if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ){ passBaseline = false; passnJets = false; }
   if( debug ) std::cout<<"cntNJetsPt50Eta24 : "<<cntNJetsPt50Eta24<<"  cntNJetsPt30Eta24 : "<<cntNJetsPt30Eta24<<"  cntNJetsPt30 : "<<cntNJetsPt30<<"  passBaseline : "<<passBaseline<<std::endl;

// Pass deltaPhi?
   bool passdPhis = true;
   if( dPhiVec->at(0) < AnaConsts::dPhi0_CUT || dPhiVec->at(1) < AnaConsts::dPhi1_CUT || dPhiVec->at(2) < AnaConsts::dPhi2_CUT ){ passBaseline = false; passdPhis = false; }
   if( debug ) std::cout<<"dPhi0 : "<<dPhiVec->at(0)<<"  dPhi1 : "<<dPhiVec->at(1)<<"  dPhi2 : "<<dPhiVec->at(2)<<"  passBaseline : "<<passBaseline<<std::endl;

// Pass number of b-tagged jets?
   bool passBJets = true;
   if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) ){ passBaseline = false; passBJets = false; }
   if( debug ) std::cout<<"cntCSVS : "<<cntCSVS<<"  passBaseline : "<<passBaseline<<std::endl;

// Pass the baseline MET requirement?
   bool passMET = true;
   if( tr.getVar<double>("met") < AnaConsts::defaultMETcut ){ passBaseline = false; passMET = false; }
   if( debug ) std::cout<<"met : "<<tr.getVar<double>("met")<<"  defaultMETcut : "<<AnaConsts::defaultMETcut<<"  passBaseline : "<<passBaseline<<std::endl;

// Calculate top tagger related variables. 
// Note that to save speed, only do the calculation after previous base line requirements.
   int bestTopJetIdx = -1;
   bool remainPassCSVS = false;
   int pickedRemainingCombfatJetIdx = -1;
   double bestTopJetMass = -1;
   int nTopCandSortedCnt = 0;
   double MT2 = -1;
   double mTcomb = -1;

//   if( cntNJetsPt30 >= AnaConsts::nJetsSel ){
   if( passBaseline && cntNJetsPt30 >= AnaConsts::nJetsSel ){
      type3Ptr->processEvent((*jetsLVec_forTagger), (*recoJetsBtag_forTagger), metLVec);
      bestTopJetIdx = type3Ptr->bestTopJetIdx;
      remainPassCSVS = type3Ptr->remainPassCSVS;
      pickedRemainingCombfatJetIdx = type3Ptr->pickedRemainingCombfatJetIdx;
      if( bestTopJetIdx != -1 ) bestTopJetMass = type3Ptr->bestTopJetLVec.M();

      nTopCandSortedCnt = type3Ptr->nTopCandSortedCnt;
      MT2 = type3Ptr->MT2;
      mTcomb = type3Ptr->mTbJet + 0.5*type3Ptr->mTbestTopJet;
   }

// Pass top tagger requirement?
   bool passTagger = true;
// bestTopJetIdx != -1 means at least 1 top candidate!
   if( bestTopJetIdx == -1 ){ passBaseline = false; passTagger = false; }
   if( ! remainPassCSVS ){ passBaseline = false; passTagger = false; }
   if( pickedRemainingCombfatJetIdx == -1 && jetsLVec_forTagger->size()>=6 ){ passBaseline = false; passTagger = false; }
   if( ! (bestTopJetMass > AnaConsts::lowTopCut_ && bestTopJetMass < AnaConsts::highTopCut_ ) ){ passBaseline = false; passTagger = false; }
   if( debug ) std::cout<<"bestTopJetidx : "<<bestTopJetIdx<<"  remainPassCSVS : "<<remainPassCSVS<<"  pickedRemainingCombfatJetIdx : "<<pickedRemainingCombfatJetIdx<<"  bestTopJetMass : "<<bestTopJetMass<<"  passBaseline : "<<passBaseline<<std::endl;

// Register all the calculated variables
   tr.registerDerivedVar("nMuons_CUT", nMuons);
   tr.registerDerivedVar("nElectrons_CUT", nElectrons);
   tr.registerDerivedVar("nIsoTrks_CUT", nIsoTrks);

   tr.registerDerivedVar("cntNJetsPt50Eta24", cntNJetsPt50Eta24);
   tr.registerDerivedVar("cntNJetsPt30Eta24", cntNJetsPt30Eta24);

   tr.registerDerivedVec("dPhiVec", dPhiVec);

   tr.registerDerivedVar("cntCSVS", cntCSVS);

   tr.registerDerivedVec("jetsLVec_forTagger", jetsLVec_forTagger);
   tr.registerDerivedVec("recoJetsBtag_forTagger", recoJetsBtag_forTagger);

   tr.registerDerivedVar("cntNJetsPt30", cntNJetsPt30);

   tr.registerDerivedVar("bestTopJetIdx", bestTopJetIdx);
   tr.registerDerivedVar("remainPassCSVS", remainPassCSVS);
   tr.registerDerivedVar("pickedRemainingCombfatJetIdx", pickedRemainingCombfatJetIdx);
   tr.registerDerivedVar("bestTopJetMass", bestTopJetMass);

   tr.registerDerivedVar("passLeptVeto", passLeptVeto);
   tr.registerDerivedVar("passnJets", passnJets);
   tr.registerDerivedVar("passdPhis", passdPhis);
   tr.registerDerivedVar("passBJets", passBJets);
   tr.registerDerivedVar("passMET", passMET);
   tr.registerDerivedVar("passTagger", passTagger);
   tr.registerDerivedVar("passBaseline", passBaseline);

   if( debug ) std::cout<<"nTopCandSortedCnt : "<<nTopCandSortedCnt<<"  MT2 : "<<MT2<<"  mTcomb : "<<mTcomb<<"  passBaseline : "<<passBaseline<<std::endl;

   tr.registerDerivedVar("nTopCandSortedCnt", nTopCandSortedCnt);
   tr.registerDerivedVar("MT2", MT2);
   tr.registerDerivedVar("mTcomb", mTcomb);

   if( debug ) std::cout<<"passBaseline : "<<passBaseline<<"  passBaseline : "<<passBaseline<<std::endl;
}

void anaFunc(NTupleReader *tr, std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar", int verbose=0){
  TString sampleKeyStringT(sampleKeyString);

  keyStringCachedVec.push_back(sampleKeyString);
  double sampleScaleMC = 1.0; int sampleColor = 1;
  for(int ib=0; ib<nMC; ib++){
     TString permcStrT(mcStr[ib]);
     if( permcStrT.Contains(sampleKeyString) ) { sampleColor = colors[ib]; }
  }
  scaleMCCachedVec.push_back( sampleScaleMC );
  colorCachedVec.push_back( sampleColor );

  cnt_passLeptVeto_WeightedScaledMC = 0; cnt_passLeptVeto_WeightedErrorScaledMC = 0;
  cnt_passnJets_WeightedScaledMC = 0; cnt_passnJets_WeightedErrorScaledMC = 0;
  cnt_passdPhis_WeightedScaledMC = 0; cnt_passdPhis_WeightedErrorScaledMC = 0;
  cnt_passBJets_WeightedScaledMC = 0; cnt_passBJets_WeightedErrorScaledMC = 0;
  cnt_passMET_WeightedScaledMC = 0; cnt_passMET_WeightedErrorScaledMC = 0;
  cnt_passTagger_WeightedScaledMC = 0; cnt_passTagger_WeightedErrorScaledMC = 0;
  cnt_passBaseline_WeightedScaledMC = 0; cnt_passBaseline_WeightedErrorScaledMC = 0;

  TH2D * h2_evtCnt_nbJets_vs_nTops = new TH2D(sampleKeyStringT+"_h2_evtCnt_nbJets_vs_nTops", sampleKeyStringT+": event counts nbJets versus nTops; nTops; nbJets", 5, 0, 5, 5, 0, 5);
  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     bool isData = false;

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = 1.0;
     for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ){ scaleMC = scalesVec[ib]; } }
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ){ scaleMC = dataScale; isData = true; }

     if( tr ) delete tr;
     tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames);
     tr->registerFunction(&passBaselineFunc);

     int entries = tr->getNEntries();
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     double cnt_passBaseline_Weighted = 0, cnt_passBaseline_WeightedSquared =0;

     while(tr->getNextEvent()){

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (tr->getEvtNum()-1)%(entries/10) == 0 ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;
/*
        if(tr->getEvtNum() == 1){
           tr->printTupleMembers();
           FILE * fout = fopen("NTupleTypes.txt", "w");
           tr->printTupleMembers(fout);
           fclose(fout);
        }
*/
        // Internal evtWeight in the sample: default is 1.0 execept for MC samples with intrinsic weight, e.g., QCD flat sample.
        double iniWeight = tr->getVar<double>("evtWeight");
        double puWeight = 1.0; // currently set to be 1.0
        
        double evtWeight = iniWeight * puWeight;

        if( !tr->getVar<bool>("passLeptVeto") ) continue;
        cnt_passLeptVeto_WeightedScaledMC += evtWeight * scaleMC; cnt_passLeptVeto_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passnJets") ) continue;
        cnt_passnJets_WeightedScaledMC += evtWeight * scaleMC; cnt_passnJets_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passdPhis") ) continue;
        cnt_passdPhis_WeightedScaledMC += evtWeight * scaleMC; cnt_passdPhis_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passBJets") ) continue;
        cnt_passBJets_WeightedScaledMC += evtWeight * scaleMC; cnt_passBJets_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passMET") ) continue;
        cnt_passMET_WeightedScaledMC += evtWeight * scaleMC; cnt_passMET_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passTagger") ) continue;
        cnt_passTagger_WeightedScaledMC += evtWeight * scaleMC; cnt_passTagger_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        if( !tr->getVar<bool>("passBaseline") ) continue;
        cnt_passBaseline_WeightedScaledMC += evtWeight * scaleMC; cnt_passBaseline_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

        int nbJets = tr->getVar<int>("cntCSVS"), nTops = tr->getVar<int>("nTopCandSortedCnt");
        double MT2 = tr->getVar<double>("MT2"), mTcomb = tr->getVar<double>("mTcomb");
// Search region in bins of
// nbJets: 1, 2, >=3
// nTops : 1, 2, >=3
        if( nbJets >=3 ) nbJets = 3; if( nTops >=3 ) nTops = 3;

        if( nTops != 1 ){
           h2_evtCnt_nbJets_vs_nTops->Fill(nTops, nbJets, evtWeight*scaleMC);
           if( !keyStringT.Contains("Signal") ){
              h2_evtCnt_sumSM_nbJets_vs_nTops->Fill(nTops, nbJets, evtWeight*scaleMC);
           }
        }else/* if( MT2 > AnaConsts::MT2cut_ && mTcomb > AnaConsts::mTcombcut_ )*/{
           h2_evtCnt_nbJets_vs_nTops->Fill(nTops, nbJets, evtWeight*scaleMC);
           if( !keyStringT.Contains("Signal") ){
              h2_evtCnt_sumSM_nbJets_vs_nTops->Fill(nTops, nbJets, evtWeight*scaleMC);
           }
        }
     }
  }
  h2_evtCnt_nbJets_vs_nTopsVec.push_back((TH2D*) h2_evtCnt_nbJets_vs_nTops->Clone()); 

  cnt_passLeptVeto_WeightedErrorScaledMC = sqrt(cnt_passLeptVeto_WeightedErrorScaledMC);
  cnt_passnJets_WeightedErrorScaledMC = sqrt(cnt_passnJets_WeightedErrorScaledMC);
  cnt_passdPhis_WeightedErrorScaledMC = sqrt(cnt_passdPhis_WeightedErrorScaledMC);
  cnt_passBJets_WeightedErrorScaledMC = sqrt(cnt_passBJets_WeightedErrorScaledMC);
  cnt_passMET_WeightedErrorScaledMC = sqrt(cnt_passMET_WeightedErrorScaledMC);
  cnt_passTagger_WeightedErrorScaledMC = sqrt(cnt_passTagger_WeightedErrorScaledMC);
  cnt_passBaseline_WeightedErrorScaledMC = sqrt(cnt_passBaseline_WeightedErrorScaledMC);

  std::cout<<"\n\n"<<sampleKeyString.c_str()<<"_cnt_passLeptVeto : "<<cnt_passLeptVeto_WeightedScaledMC<<" +- "<<cnt_passLeptVeto_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passnJets : "<<cnt_passnJets_WeightedScaledMC<<" +- "<<cnt_passnJets_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passdPhis : "<<cnt_passdPhis_WeightedScaledMC<<" +- "<<cnt_passdPhis_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passBJets : "<<cnt_passBJets_WeightedScaledMC<<" +- "<<cnt_passBJets_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passMET : "<<cnt_passMET_WeightedScaledMC<<" +- "<<cnt_passMET_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passTagger : "<<cnt_passTagger_WeightedScaledMC<<" +- "<<cnt_passTagger_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passBaseline : "<<cnt_passBaseline_WeightedScaledMC<<" +- "<<cnt_passBaseline_WeightedErrorScaledMC<<std::endl;
}

void baselineDef(){

   initMCscales();

   AnaFunctions::prepareTopTagger();

   NTupleReader *tr = 0;

   std::vector<TTree*> treeVec;
   std::vector<std::string> subSampleKeysVec;
 
   char filenames[500], names[500];
   std::vector<std::string> filesDataVec;
   std::vector<std::string> filesTTJetsVec,
                            filesWJetsToLNu_HT_600toInfVec, filesWJetsToLNu_HT_400to600Vec, filesWJetsToLNu_HT_200to400Vec, filesWJetsToLNu_HT_100to200Vec,
                            filesZJetsToNuNu_HT_600toInfVec, filesZJetsToNuNu_HT_400to600Vec, filesZJetsToNuNu_HT_200to400Vec, filesZJetsToNuNu_HT_100to200Vec,
                            filesDYJetsToLL_HT_600toInfVec, filesDYJetsToLL_HT_400to600Vec, filesDYJetsToLL_HT_200to400Vec, filesDYJetsToLL_HT_100to200Vec,
                            filesQCD_HT_1000toInfVec, filesQCD_HT_500to1000Vec, filesQCD_HT_250to500Vec,
                            filesT_tWVec, filesTbar_tWVec, filesTTZVec;
   std::vector<std::string> filesSignal_T1tttt_mGluino1200_mLSP800Vec,
                            filesSignal_T1tttt_mGluino1500_mLSP100Vec,
                            filesSignal_T5tttt_mGluino1300_mStop300_mChi280Vec, filesSignal_T5tttt_mGluino1300_mStop300_mCh285Vec,
                            filesSignal_T1bbbb_mGluino1000_mLSP900Vec,
                            filesSignal_T1bbbb_mGluino1500_mLSP100Vec,
                            filesSignal_T2tt_mStop425_mLSP325Vec,
                            filesSignal_T2tt_mStop500_mLSP325Vec,
                            filesSignal_T2tt_mStop650_mLSP325Vec,
                            filesSignal_T2tt_mStop850_mLSP100Vec,
                            filesSignal_T2bb_mSbottom600_mLSP580Vec,
                            filesSignal_T2bb_mSbottom900_mLSP100Vec;
 
   ifstream finData("rootlist_Data_METPD.txt"); while( finData.getline(filenames, 500) ){ filesDataVec.push_back(filenames); }
 
   ifstream finTTJets("rootlist_TTJets.txt"); while( finTTJets.getline(filenames, 500) ){ filesTTJetsVec.push_back(filenames); }
 
   ifstream finWJetsToLNu_HT_600toInf("rootlist_WJetsToLNu_HT_600toInf.txt"); while( finWJetsToLNu_HT_600toInf.getline(filenames, 500) ){ filesWJetsToLNu_HT_600toInfVec.push_back(filenames); }
   ifstream finWJetsToLNu_HT_400to600("rootlist_WJetsToLNu_HT_400to600.txt"); while( finWJetsToLNu_HT_400to600.getline(filenames, 500) ){ filesWJetsToLNu_HT_400to600Vec.push_back(filenames); }
   ifstream finWJetsToLNu_HT_200to400("rootlist_WJetsToLNu_HT_200to400.txt"); while( finWJetsToLNu_HT_200to400.getline(filenames, 500) ){ filesWJetsToLNu_HT_200to400Vec.push_back(filenames); }
   ifstream finWJetsToLNu_HT_100to200("rootlist_WJetsToLNu_HT_100to200.txt"); while( finWJetsToLNu_HT_100to200.getline(filenames, 500) ){ filesWJetsToLNu_HT_100to200Vec.push_back(filenames); }
 
   ifstream finZJetsToNuNu_HT_600toInf("rootlist_ZJetsToNuNu_HT_600toInf.txt"); while( finZJetsToNuNu_HT_600toInf.getline(filenames, 500) ){ filesZJetsToNuNu_HT_600toInfVec.push_back(filenames); }
   ifstream finZJetsToNuNu_HT_400to600("rootlist_ZJetsToNuNu_HT_400to600.txt"); while( finZJetsToNuNu_HT_400to600.getline(filenames, 500) ){ filesZJetsToNuNu_HT_400to600Vec.push_back(filenames); }
   ifstream finZJetsToNuNu_HT_200to400("rootlist_ZJetsToNuNu_HT_200to400.txt"); while( finZJetsToNuNu_HT_200to400.getline(filenames, 500) ){ filesZJetsToNuNu_HT_200to400Vec.push_back(filenames); }
   ifstream finZJetsToNuNu_HT_100to200("rootlist_ZJetsToNuNu_HT_100to200.txt"); while( finZJetsToNuNu_HT_100to200.getline(filenames, 500) ){ filesZJetsToNuNu_HT_100to200Vec.push_back(filenames); }
 
   ifstream finDYJetsToLL_HT_600toInf("rootlist_DYJetsToLL_HT_600toInf.txt"); while( finDYJetsToLL_HT_600toInf.getline(filenames, 500) ){ filesDYJetsToLL_HT_600toInfVec.push_back(filenames); }
   ifstream finDYJetsToLL_HT_400to600("rootlist_DYJetsToLL_HT_400to600.txt"); while( finDYJetsToLL_HT_400to600.getline(filenames, 500) ){ filesDYJetsToLL_HT_400to600Vec.push_back(filenames); }
   ifstream finDYJetsToLL_HT_200to400("rootlist_DYJetsToLL_HT_200to400.txt"); while( finDYJetsToLL_HT_200to400.getline(filenames, 500) ){ filesDYJetsToLL_HT_200to400Vec.push_back(filenames); }
   ifstream finDYJetsToLL_HT_100to200("rootlist_DYJetsToLL_HT_100to200.txt"); while( finDYJetsToLL_HT_100to200.getline(filenames, 500) ){ filesDYJetsToLL_HT_100to200Vec.push_back(filenames); }
 
   ifstream finQCD_HT_1000toInf("rootlist_QCD_HT_1000toInf.txt"); while( finQCD_HT_1000toInf.getline(filenames, 500) ){ filesQCD_HT_1000toInfVec.push_back(filenames); }
   ifstream finQCD_HT_500to1000("rootlist_QCD_HT_500to1000.txt"); while( finQCD_HT_500to1000.getline(filenames, 500) ){ filesQCD_HT_500to1000Vec.push_back(filenames); }
   ifstream finQCD_HT_250to500("rootlist_QCD_HT_250to500.txt"); while( finQCD_HT_250to500.getline(filenames, 500) ){ filesQCD_HT_250to500Vec.push_back(filenames); }
 
   ifstream finT_tW("rootlist_T_tW.txt"); while( finT_tW.getline(filenames, 500) ){ filesT_tWVec.push_back(filenames); }
   ifstream finTbar_tW("rootlist_Tbar_tW.txt"); while( finTbar_tW.getline(filenames, 500) ){ filesTbar_tWVec.push_back(filenames); }
   ifstream finTTZ("rootlist_TTZ.txt"); while( finTTZ.getline(filenames, 500) ){ filesTTZVec.push_back(filenames); }
 
   ifstream finSignal_T1tttt_mGluino1200_mLSP800("rootlist_T1tttt_mGluino1200_mLSP800.txt"); while(finSignal_T1tttt_mGluino1200_mLSP800.getline(filenames, 500) ){ filesSignal_T1tttt_mGluino1200_mLSP800Vec.push_back(filenames); }
   ifstream finSignal_T1tttt_mGluino1500_mLSP100("rootlist_T1tttt_mGluino1500_mLSP100.txt"); while(finSignal_T1tttt_mGluino1500_mLSP100.getline(filenames, 500) ){ filesSignal_T1tttt_mGluino1500_mLSP100Vec.push_back(filenames); }
   ifstream finSignal_T5tttt_mGluino1300_mStop300_mChi280("rootlist_T5tttt_mGluino1300_mStop300_mChi280.txt"); while(finSignal_T5tttt_mGluino1300_mStop300_mChi280.getline(filenames, 500) ){filesSignal_T5tttt_mGluino1300_mStop300_mChi280Vec.push_back(filenames); }
   ifstream finSignal_T5tttt_mGluino1300_mStop300_mCh285("rootlist_T5tttt_mGluino1300_mStop300_mCh285.txt"); while(finSignal_T5tttt_mGluino1300_mStop300_mCh285.getline(filenames, 500) ){ filesSignal_T5tttt_mGluino1300_mStop300_mCh285Vec.push_back(filenames); }
   ifstream finSignal_T1bbbb_mGluino1000_mLSP900("rootlist_T1bbbb_mGluino1000_mLSP900.txt"); while(finSignal_T1bbbb_mGluino1000_mLSP900.getline(filenames, 500) ){ filesSignal_T1bbbb_mGluino1000_mLSP900Vec.push_back(filenames); }
   ifstream finSignal_T1bbbb_mGluino1500_mLSP100("rootlist_T1bbbb_mGluino1500_mLSP100.txt"); while(finSignal_T1bbbb_mGluino1500_mLSP100.getline(filenames, 500) ){ filesSignal_T1bbbb_mGluino1500_mLSP100Vec.push_back(filenames); }
   ifstream finSignal_T2tt_mStop425_mLSP325("rootlist_T2tt_mStop425_mLSP325.txt"); while(finSignal_T2tt_mStop425_mLSP325.getline(filenames, 500) ){ filesSignal_T2tt_mStop425_mLSP325Vec.push_back(filenames); }
   ifstream finSignal_T2tt_mStop500_mLSP325("rootlist_T2tt_mStop500_mLSP325.txt"); while(finSignal_T2tt_mStop500_mLSP325.getline(filenames, 500) ){ filesSignal_T2tt_mStop500_mLSP325Vec.push_back(filenames); }
   ifstream finSignal_T2tt_mStop650_mLSP325("rootlist_T2tt_mStop650_mLSP325.txt"); while(finSignal_T2tt_mStop650_mLSP325.getline(filenames, 500) ){ filesSignal_T2tt_mStop650_mLSP325Vec.push_back(filenames); }
   ifstream finSignal_T2tt_mStop850_mLSP100("rootlist_T2tt_mStop850_mLSP100.txt"); while(finSignal_T2tt_mStop850_mLSP100.getline(filenames, 500) ){ filesSignal_T2tt_mStop850_mLSP100Vec.push_back(filenames); }
   ifstream finSignal_T2bb_mSbottom600_mLSP580("rootlist_T2bb_mSbottom600_mLSP580.txt"); while(finSignal_T2bb_mSbottom600_mLSP580.getline(filenames, 500) ){ filesSignal_T2bb_mSbottom600_mLSP580Vec.push_back(filenames); }
   ifstream finSignal_T2bb_mSbottom900_mLSP100("rootlist_T2bb_mSbottom900_mLSP100.txt"); while(finSignal_T2bb_mSbottom900_mLSP100.getline(filenames, 500) ){ filesSignal_T2bb_mSbottom900_mLSP100Vec.push_back(filenames); }

   std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

   h2_evtCnt_sumSM_nbJets_vs_nTops = new TH2D("h2_evtCnt_sumSM_nbJets_vs_nTops", "SumSM: event counts nbJets versus nTops; nTops; nbJets", 5, 0, 5, 5, 0, 5);

#include "bkgSamples.h"
#include "sigSamples.h"

// Plotting
   setTDRStyle();

     //  For the frame
   tdrStyle->SetFrameBorderMode(0);
   tdrStyle->SetFrameBorderSize(1);
   tdrStyle->SetFrameFillColor(kBlack);
   tdrStyle->SetFrameFillStyle(0);
   tdrStyle->SetFrameLineColor(kBlack);
   tdrStyle->SetFrameLineStyle(0);
   tdrStyle->SetFrameLineWidth(1);
 
      //  For the Pad
   tdrStyle->SetPadBorderMode(0);
   tdrStyle->SetPadColor(kWhite);
   tdrStyle->SetPadGridX(false);
   tdrStyle->SetPadGridY(false);
   tdrStyle->SetGridColor(0);
   tdrStyle->SetGridStyle(3);
   tdrStyle->SetGridWidth(1);
 
      //  For the axis
   tdrStyle->SetAxisColor(1,"XYZ");
   tdrStyle->SetTickLength(0.03,"XYZ");
   tdrStyle->SetNdivisions(505,"XYZ");
   tdrStyle->SetPadTickX(1);
   tdrStyle->SetPadTickY(1);
   tdrStyle->SetStripDecimals(kFALSE);
 
   tdrStyle->SetLabelSize(0.050, "XYZ");
 
   tdrStyle->SetPadTopMargin(0.1); tdrStyle->SetPadBottomMargin(0.15);
   tdrStyle->SetPadLeftMargin(0.15); tdrStyle->SetPadRightMargin(0.15);
 
   tdrStyle->SetOptStat(1111);
 
   tdrStyle->SetHistLineWidth(1);
 
   tdrStyle->SetPaintTextFormat("4.2f");

   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

   TCanvas *cs = new TCanvas("cs", "cs", 1200, 900);
   int divW=4, divH=3;
   cs->Divide(divW, divH);

   cs->Print("allINone_"+treeStrTtype+".pdf[");
   draw2DallINone(cs, divW*divH, h2_evtCnt_nbJets_vs_nTopsVec, "text"); cs->Print("allINone_"+treeStrTtype+".pdf");
   cs->Print("allINone_"+treeStrTtype+".pdf]");

   TCanvas *ct = new TCanvas("ct", "ct", 800, 800);
   ct->cd();
   h2_evtCnt_sumSM_nbJets_vs_nTops->Draw("colz text");
   ct->Print("evtCnt_sumSM_nbJets_vs_nTops.pdf");

}

void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2D*> &h2_inputVec, const TString optDrawStr){

  int cntPadIdx = 0;
  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){
     if( it == 0 ) cntPadIdx = 0;
     TString keyStringT(keyStringCachedVec[it]);
     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" || keyStringT.Contains("Data") ) continue;
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(0);
     h2_inputVec[it]->Draw("colz "+optDrawStr);
  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); }
}

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH1D*> &h1_inputVec, const int nRebin, const TString optDrawStr){

  int statusLogy = 0;
  if( optDrawStr.Contains("SetLogy") ) statusLogy =1;
  int doNormalization = 0;
  if( optDrawStr.Contains("normalization") ) doNormalization =1;

  int cntPadIdx = 0;
  std::vector<TH1D*> h1_stackedVec; TH1D * h1_dataCached =0, * h1_signal1Cached =0, *h1_signal2Cached =0;

  std::vector<TString> stackedKeyStringTVec;

  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){

     TString keyStringT(keyStringCachedVec[it]);

     if( it == 0 ){ cntPadIdx = 0; h1_stackedVec.clear(); }

//     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" ) continue;
     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" || keyStringT.Contains("Data") ) continue;

     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(statusLogy);

     TH1D *tmpHist = (TH1D*) h1_inputVec[it]->Clone();

     tmpHist->Rebin(nRebin); tmpHist->Scale(scaleMCCachedVec[it]);

     tmpHist->SetLineColor(colorCachedVec[it]); tmpHist->SetMarkerColor(colorCachedVec[it]); tmpHist->SetMarkerStyle(6); tmpHist->SetMarkerSize(0.25);

     if( keyStringT.Contains("Data") ){ tmpHist->SetLineColor(kBlack); tmpHist->SetMarkerColor(kBlack); }

     if( !statusLogy ){
        tmpHist->SetAxisRange(0, tmpHist->GetMaximum()*1.5, "Y");
     }else{
        tmpHist->SetAxisRange(1e-1, tmpHist->GetMaximum()*32*5, "Y");
     }

     if( keyStringT.Contains("Data") || keyStringT.Contains("Signal") ){
        tmpHist->Draw("e");
     }else{
        tmpHist->Draw("hist");
     }

     if( !keyStringT.Contains("Data") && !keyStringT.Contains("Signal") ){
        stackedKeyStringTVec.push_back(keyStringT);
        if( h1_stackedVec.empty() ){
           h1_stackedVec.push_back( (TH1D*) tmpHist->Clone() );
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] );
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] );
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] );
        }else{
           TH1D *tmpSumHist = (TH1D*) tmpHist->Clone();
           tmpSumHist->Add(h1_stackedVec.back(), 1);
           h1_stackedVec.push_back( (TH1D*) tmpSumHist->Clone() );
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] );
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] );
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] );
        }
     }
     if( keyStringT.Contains("Data") ){ h1_dataCached = (TH1D*) tmpHist->Clone(); }
     if( keyStringT.Contains("Signal") ){
        if( !h1_signal1Cached ) h1_signal1Cached = (TH1D*) tmpHist->Clone();
        else h1_signal2Cached = (TH1D*) tmpHist->Clone();
     }

  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); pad->SetLogy(statusLogy); }

  Float_t legendX1 = .60;
  Float_t legendX2 = .85;
  Float_t legendY1 = .55;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.030);

  cs->cd(lastPadIdx);

  if( h1_dataCached ){
     double dataIntegral = h1_dataCached->Integral();
     double sumTotIntegral = h1_stackedVec.back()->Integral();
     double normScale = dataIntegral/sumTotIntegral;

     h1_dataCached->Draw("e");
     catLeg1->AddEntry(h1_dataCached, "Data");
     for(unsigned int is=0; is<h1_stackedVec.size(); is++){
        unsigned int ivsIdx = h1_stackedVec.size()-is-1;
        TH1D * tmpStacked = (TH1D*) h1_stackedVec[ivsIdx]->Clone();
        if( doNormalization ) tmpStacked->Scale(normScale);
        tmpStacked->Draw("hist same");
        catLeg1->AddEntry(tmpStacked, stackedKeyStringTVec[ivsIdx]);
     }
     h1_signal1Cached->SetLineColor(kRed); h1_signal1Cached->SetMarkerColor(kRed); h1_signal1Cached->SetLineStyle(3);
     h1_signal1Cached->Draw("same");
     h1_dataCached->Draw("same e");

     catLeg1->AddEntry(h1_signal1Cached, "Signal");
     catLeg1->SetFillColor(kWhite);
     catLeg1->SetBorderSize(0);
     catLeg1->Draw();

     TPad *lastPad = (TPad*) cs->GetPad(lastPadIdx);
     lastPad->RedrawAxis();
  }

}

int main(){
   baselineDef();
}

