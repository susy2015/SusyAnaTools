#include "NTupleReader.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "samples.h"
#include "customize.h"

#include "TStopwatch.h"
#include "TString.h"

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

TStopwatch timer;

/*
   The main baseline definition function. Features of the NTupleReader class are used.
 ] The function return type has to be void in order to do the tr->registerFunction(&passBaselineFunc).
   The registeration of the function would enable that for each event the function will be run automatically.
 ] Inside the passBaselineFunc (close to the end), one can register the further calculated variables or vectors
   using registerDerivedVar or registerDerivedVec. The nice feature of this is that these registered variables
   will be updated for each event and one can access them using getVar or getVec very similar to whatever already
   stored in the flat ntuples.
 ] If used independent of the NTupleReader class, please follow the calculation of the number of leptons, jets, deltaphi
   and so on so that we are sync'ed. Then one can turn the return to be a boolean (with return of the passBaseline).
*/
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
// The jets stored in flat ntuples might have looser pt or eta requirement (here it's pt>10 GeV in flat ntuple),
// while for the top tagger we need higher pt requirement as defined in AnaConsts::pt30Arr array.
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

// All the pass booleans are stored/registered into the NTupleReader and can be used later
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

/*
   This is an extra function which is not related to baseline definition, but hopefully it has some nice
   feature that others can re-use.
*/
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

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     bool isData = false;

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = 1.0;
     for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ){ scaleMC = scalesVec[ib]; } }
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ){ scaleMC = dataScale; isData = true; }

     if( tr ) delete tr;
// The NTupleReader class can take a tree and std::list as inputs.
// The AnaConsts::activatedBranchNames list custmoizes the NTupleReader to only read in branches
// defined in the list. By default, it reads in all the variables stored in the flat ntuple.
     tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames);
// The passBaselineFunc is registered here
     tr->registerFunction(&passBaselineFunc);

     int entries = tr->getNEntries();
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     double cnt_passBaseline_Weighted = 0, cnt_passBaseline_WeightedSquared =0;

     while(tr->getNextEvent()){

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (tr->getEvtNum()-1)%(entries/10) == 0 ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;
/*
// If uncommented, a file NTupleTypes.txt will be produced and it contains all the variables defined/held in the NTupleReader class.
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

// Now count the cut flow
// Note that the orders of the cuts can be flexibly changed.
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

// If only interested in pass the baseline, then this is it.
        if( !tr->getVar<bool>("passBaseline") ) continue;
        cnt_passBaseline_WeightedScaledMC += evtWeight * scaleMC; cnt_passBaseline_WeightedErrorScaledMC += evtWeight * evtWeight * scaleMC * scaleMC;

// Examples of getting variables calculted in passBaselineFunc out
        int nbJets = tr->getVar<int>("cntCSVS"), nTops = tr->getVar<int>("nTopCandSortedCnt");
        double MT2 = tr->getVar<double>("MT2"), mTcomb = tr->getVar<double>("mTcomb");
     }
  }

  cnt_passLeptVeto_WeightedErrorScaledMC = sqrt(cnt_passLeptVeto_WeightedErrorScaledMC);
  cnt_passnJets_WeightedErrorScaledMC = sqrt(cnt_passnJets_WeightedErrorScaledMC);
  cnt_passdPhis_WeightedErrorScaledMC = sqrt(cnt_passdPhis_WeightedErrorScaledMC);
  cnt_passBJets_WeightedErrorScaledMC = sqrt(cnt_passBJets_WeightedErrorScaledMC);
  cnt_passMET_WeightedErrorScaledMC = sqrt(cnt_passMET_WeightedErrorScaledMC);
  cnt_passTagger_WeightedErrorScaledMC = sqrt(cnt_passTagger_WeightedErrorScaledMC);
  cnt_passBaseline_WeightedErrorScaledMC = sqrt(cnt_passBaseline_WeightedErrorScaledMC);

// Print out the cut flows
  std::cout<<"\n\n"<<sampleKeyString.c_str()<<"_cnt_passLeptVeto : "<<cnt_passLeptVeto_WeightedScaledMC<<" +- "<<cnt_passLeptVeto_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passnJets : "<<cnt_passnJets_WeightedScaledMC<<" +- "<<cnt_passnJets_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passdPhis : "<<cnt_passdPhis_WeightedScaledMC<<" +- "<<cnt_passdPhis_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passBJets : "<<cnt_passBJets_WeightedScaledMC<<" +- "<<cnt_passBJets_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passMET : "<<cnt_passMET_WeightedScaledMC<<" +- "<<cnt_passMET_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passTagger : "<<cnt_passTagger_WeightedScaledMC<<" +- "<<cnt_passTagger_WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cnt_passBaseline : "<<cnt_passBaseline_WeightedScaledMC<<" +- "<<cnt_passBaseline_WeightedErrorScaledMC<<std::endl;
}

void baselineDef(){

// initialize the MC scale factors defined in samples.h
   initMCscales();

// initialize the type3Ptr defined in the customize.h
   AnaFunctions::prepareTopTagger();

   NTupleReader *tr = 0;

// treeVec can handle trees from different HT-binned samples
   std::vector<TTree*> treeVec;
// subSampleKeysVec contains strings from the mcStr[] defined in samples.h.
// It is used to get the MC scale factor to data lumi.
   std::vector<std::string> subSampleKeysVec;

// An example of working on the T1tttt sample
   char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_T1tttt_mGl-1500_mLSP-100-madgraph-tauola/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141225_024211/0000/stopFlatNtuples_%d.root";

   TChain *chn = new TChain(treeStrTtype+"/AUX");

   char fname[512];
   for(int ir = 1; ir < 2; ++ir){
      sprintf(fname, nBase, ir);
      chn->Add(fname);
   }
   treeVec.push_back(chn); subSampleKeysVec.push_back("Signal_T1tttt_mGluino1500_mLSP100");
// The "T1tttt_mGluino1500_mLSP100" is a keyword string for printout and other usage to distinguish between MC processes
   anaFunc(tr, treeVec, subSampleKeysVec, "T1tttt_mGluino1500_mLSP100");
 
}

int main(){
   baselineDef();
}
