#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "TColor.h"
#include "TObject.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<double>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<double> >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#pragma link C++ class vector<vector<TFile*> >;
#pragma link C++ class vector<vector<TH1D*> >;
#pragma link C++ class vector< map<int, int> >;
#endif

using namespace std;

const int nMC = 9;
const string   mcStr[] = {   "TTbar", "WJetsToLNu",   "Zinv",   "DYJets",    "QCDFlat",    "T_t",    "T_tW",    "Signal_x500_y100",  "Signal_x350_y50" };
const double xSecArr[] = {  225.1967,     25.22,       5.274,  3532.8149,  2.99913994E10,   47.0,  11.1773*2.0,    0.0855847,           0.807323       };
const int    nEvtArr[] = {   6722660,   1647807,     1006925,   1000000,     9998151,      23777,   473554,          98720,              99992         };

const double dataLumi = 698.873/1000. + 4.389; // in fb-1
const double scaledToDataLumi = 10; // in fb-1

double dataScale = 1.0;

vector<double> scalesVec;

vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

const double mTbcut_ = 500, mTtcut_ = 365, MT2cut_ = 300, mTWcut_ = 600;
const double lowTopCut_ = 80, highTopCut_ = 270, lowWCut_ = 50, highWCut_ = 120;

const double dzcut_ = 0.05;
const double isoCut_ = 0.1;

double template_cntEventsWeighted =0, template_cntEventsWeightedSquared =0;
double template_sumpuWeight =0;
double template_cntNumEvents =0;
double template_cntAftPreCutsWeighted =0, template_cntAftPreCutsWeightedSquared =0;
double template_cntGoodFatJetPairsWeighted =0, template_cntGoodFatJetPairsWeightedSquared =0;
double template_cntMT2Weighted =0, template_cntMT2WeightedSquared =0;
double template_cntmTbWeighted =0, template_cntmTbWeightedSquared =0;
double template_cntmTtWeighted =0, template_cntmTtWeightedSquared =0;
double template_cntmTWWeighted =0, template_cntmTWWeightedSquared =0;

double template_cntEventsWeightedScaledMC = 0;
double template_cntEventsWeightedErrorScaledMC = 0; 

double template_cntAftPreCutsWeightedScaledMC = 0, template_cntAftPreCutsWeightedErrorScaledMC = 0;
double template_cntGoodFatJetPairsWeightedScaledMC = 0, template_cntGoodFatJetPairsWeightedErrorScaledMC = 0;
double template_cntMT2WeightedScaledMC = 0, template_cntMT2WeightedErrorScaledMC = 0;
double template_cntmTbWeightedScaledMC = 0, template_cntmTbWeightedErrorScaledMC = 0;
double template_cntmTtWeightedScaledMC = 0, template_cntmTtWeightedErrorScaledMC = 0;
double template_cntmTWWeightedScaledMC = 0, template_cntmTWWeightedErrorScaledMC = 0;

vector<string> keyStringCachedVec;
vector<TH1D*> h1_nChgTrksInNoHadTauVec, h1_totChgInNoHadTauVec, h1_chgTrkPtInNoHadTauVec, h1_chgTrkEtaInNoHadTauVec, h1_totPtChgTrksInNoHadTauVec, h1_totPtNeuPtclsInNoHadTauVec;
vector<TH1D*> h1_nChgTrksInHadTauVec, h1_totChgInHadTauVec, h1_chgTrkPtInHadTauVec, h1_chgTrkEtaInHadTauVec, h1_totPtChgTrksInHadTauVec, h1_totPtNeuPtclsInHadTauVec;

vector<TH1D*> h1_trkIsoValInNoHadTauVec, h1_trkIsoValInHadTauVec;

vector<TH2D*> h2_nChgTrksVStotChgInNoHadTauVec, h2_nChgTrksVStotChgInHadTauVec;
vector<TH2D*> h2_totPtChgTrksVSchgTrkPtInNoHadTauVec, h2_totPtChgTrksVSchgTrkPtInHadTauVec;
vector<TH2D*> h2_trkIsoValVSnChgTrksInNoHadTauVec, h2_trkIsoValVSnChgTrksInHadTauVec;
vector<TH2D*> h2_trkIsoValVSchgTrkPtInNoHadTauVec, h2_trkIsoValVSchgTrkPtInHadTauVec;
vector<TH2D*> h2_trkIsoValVSavgChgTrksPtInNoHadTauVec, h2_trkIsoValVSavgChgTrksPtInHadTauVec;
vector<TH2D*> h2_avgPtChgTrksVSchgTrkPtInNoHadTauVec, h2_avgPtChgTrksVSchgTrkPtInHadTauVec;

void initMCscales(){

   dataScale = scaledToDataLumi/dataLumi;
   cout<<"\nscaledToDataLumi : "<<scaledToDataLumi<<"  dataLumi : "<<dataLumi<<"  dataScale : "<<dataScale<<endl;
   for(int ib=0; ib<nMC; ib++){
      double perScale = scaledToDataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
      scalesVec.push_back(perScale);
      printf("%20s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
   }
   cout<<endl;
}

void templatePlotsFunc(TTree *template_AUX, const std::string keyString="ttbar"){

  keyStringCachedVec.push_back(keyString);

  double scaleMC = 1.0;
  for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ) scaleMC = scalesVec[ib]; }
  TString keyStringT(keyString);
  if( keyStringT.Contains("Data") ) scaleMC = dataScale;

  int template_run, template_event, template_lumi, template_nm1, template_n0, template_np1, template_vtxSize;
  double template_avg_npv, template_tru_npv;
  double template_evtWeight;
  vector<TLorentzVector> *template_chgTrksLVec=0;
  vector<int> *template_chgOfTrks=0;
  vector<double> *template_dzOfTrks=0;
  vector<vector<TLorentzVector> > *template_pfCandsInAConeLVec=0;
  vector<vector<int> > *template_chgOfpfCandsInACone=0;
  vector<vector<double> > *template_dzOfpfCandsInACone=0;

  template_AUX->SetBranchAddress("run", &template_run);
  template_AUX->SetBranchAddress("event", &template_event);
  template_AUX->SetBranchAddress("lumi", &template_lumi);
  template_AUX->SetBranchAddress("avg_npv", &template_avg_npv);
  template_AUX->SetBranchAddress("nm1", &template_nm1);
  template_AUX->SetBranchAddress("n0", &template_n0);
  template_AUX->SetBranchAddress("np1", &template_np1);
  template_AUX->SetBranchAddress("tru_npv", &template_tru_npv);
  template_AUX->SetBranchAddress("vtxSize", &template_vtxSize);
  template_AUX->SetBranchAddress("evtWeight", &template_evtWeight);
  template_AUX->SetBranchAddress("chgTrksLVec", &template_chgTrksLVec);
  template_AUX->SetBranchAddress("chgOfTrks", &template_chgOfTrks);
  template_AUX->SetBranchAddress("dzOfTrks", &template_dzOfTrks);
  template_AUX->SetBranchAddress("pfCandsInAConeLVec", &template_pfCandsInAConeLVec);
  template_AUX->SetBranchAddress("chgOfpfCandsInACone", &template_chgOfpfCandsInACone);
  template_AUX->SetBranchAddress("dzOfpfCandsInACone", &template_dzOfpfCandsInACone);  

  int template_Entries = template_AUX->GetEntries();
  std::cout<<"\n\n"<<keyString.c_str()<<"_Entries : "<<template_Entries<<"  scaleMC : "<<scaleMC<<std::endl;

  template_cntEventsWeighted =0; template_cntEventsWeightedSquared =0;
  template_sumpuWeight =0;
  template_cntNumEvents =0;
  template_cntAftPreCutsWeighted =0; template_cntAftPreCutsWeightedSquared =0;
  template_cntGoodFatJetPairsWeighted =0; template_cntGoodFatJetPairsWeightedSquared =0;
  template_cntMT2Weighted =0; template_cntMT2WeightedSquared =0;
  template_cntmTbWeighted =0; template_cntmTbWeightedSquared =0;
  template_cntmTtWeighted =0; template_cntmTtWeightedSquared =0;
  template_cntmTWWeighted =0; template_cntmTWWeightedSquared =0;

  TH1D *template_h1_nChgTrks = new TH1D(keyStringT+"_h1_nChgTrks", keyStringT+" : number of charged tracks; N", 25, 0, 25);
  TH1D *template_h1_totChg = new TH1D(keyStringT+"_h1_totChg", keyStringT+" : total charge; C", 12, -6, 6);
  TH1D *template_h1_chgTrkPt = new TH1D(keyStringT+"_h1_chgTrkPt", keyStringT+" : P_{T} of the charged track; P_{T} [GeV]", 50, 0, 300);
  TH1D *template_h1_chgTrkEta = new TH1D(keyStringT+"_h1_chgTrkEta", keyStringT+" : #eta of the charged track; #eta", 50, -3.2, 3.2);
  TH1D *template_h1_totPtChgTrks = new TH1D(keyStringT+"_h1_totPtChgTrks", keyStringT+" : sum P_{T} of the charged tracks; P_{T} [GeV]", 50, 0, 300);
  TH1D *template_h1_totPtNeuPtcls = new TH1D(keyStringT+"_h1_totPtNeuPtcls", keyStringT+" : sum P_{T} of the neutral particles; P_{T} [GeV]", 50, 0, 10);
  TH1D *template_h1_trkIsoVal = new TH1D(keyStringT+"_h1_trkIsoVal", keyStringT+" : value of tracking isolation; trkIsoVal", 50, 0, 3.0);

  TH2D *template_h2_nChgTrksVStotChg = new TH2D(keyStringT+"_h2_nChgTrksVStotChg", keyStringT+" : number of charged tracks versus total charge; C; N", 12, -6, 6, 25, 0, 25);
  TH2D *template_h2_totPtChgTrksVSchgTrkPt = new TH2D(keyStringT+"_h2_totPtChgTrksVSchgTrkPt", keyStringT+" : sum P_{T} of the charged trackes versus charged track P_{T}; P_{T} [GeV]; #SUM P_{T} [GeV]", 50, 0, 300, 50, 0, 300);
  TH2D *template_h2_avgPtChgTrksVSchgTrkPt = new TH2D(keyStringT+"_h2_avgPtChgTrksVSchgTrkPt", keyStringT+" : avg P_{T} of the charged trackes versus charged track P_{T}; P_{T} [GeV]; avg P_{T} [GeV]", 50, 0, 300, 50, 0, 50);

  TH2D *template_h2_trkIsoValVSnChgTrks = new TH2D(keyStringT+"_h2_trkIsoValVSnChgTrks", keyStringT+" : value of tracking isolation versus number of charged tracks; N; trkIsoVal", 25, 0, 25, 50, 0, 3.0);
  TH2D *template_h2_trkIsoValVSchgTrkPt = new TH2D(keyStringT+"_h2_trkIsoValVSchgTrkPt", keyStringT+" : value of tracking isolation versus charged track P_{T}; P_{T} [GeV]; trkIsoVal", 50, 0, 300, 50, 0, 3.0);
  TH2D *template_h2_trkIsoValVSavgChgTrksPt = new TH2D(keyStringT+"_h2_trkIsoValVSavgChgTrksPt", keyStringT+" : value of tracking isolation versus averaged charged tracks P_{T}; avg P_{T} [GeV]; trkIsoVal", 50, 0, 50, 50, 0, 3.0);

  int cntIsoVetoEvents = 0;
  for(int ie=0; ie<template_Entries; ie++){

     template_AUX->GetEntry(ie);

     double puWeight = 1.0;
// Because signal is fastsim and does not have PU correctly done!
     if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
        puWeight = weightTruNPV(template_tru_npv);
     }

     unsigned int nTrks = template_chgTrksLVec->size();

     unsigned int pickedMinIsoValIdx = -1;
     double minIsoVal = 1e09;
     for(unsigned int it=0; it<nTrks; it++){

        int trkCharge = (*template_chgOfTrks)[it];

        if( trkCharge != 0 ){

           double trkIso = 0.0;
           TLorentzVector trkLVec = (*template_chgTrksLVec)[it];
           vector<TLorentzVector> allpfCandsInAConeLVec = (*template_pfCandsInAConeLVec)[it];
           vector<int> allchgOfpfCandsInACone = (*template_chgOfpfCandsInACone)[it];
           vector<double> alldzOfpfCandsInACone = (*template_dzOfpfCandsInACone)[it];

           for(unsigned int jt=0; jt<allpfCandsInAConeLVec.size(); jt++){

              if( fabs(alldzOfpfCandsInACone[jt]) > dzcut_ ) continue;

              if( allchgOfpfCandsInACone[jt] == 0 ) continue;

              TLorentzVector trkInAConeLVec = allpfCandsInAConeLVec[jt];

              trkIso += trkInAConeLVec.Pt();
           }

           if( fabs((*template_dzOfTrks)[it]) > dzcut_ ) continue;

           if( pickedMinIsoValIdx == -1 ){
              pickedMinIsoValIdx = it;
              minIsoVal = trkIso/trkLVec.Pt();
           }else if( minIsoVal > trkIso/trkLVec.Pt() ){
              pickedMinIsoValIdx = it;
              minIsoVal = trkIso/trkLVec.Pt();
           }
//           if( trkIso/trkLVec.Pt() > isoCut_ ) continue;
        }
     }

     int cntIsoTrk = 0;
     for(unsigned int it=0; it<nTrks; it++){

//        if( pickedMinIsoValIdx == -1 || it != pickedMinIsoValIdx ) continue;

        int trkCharge = (*template_chgOfTrks)[it];

        if( trkCharge != 0 ){

           double trkIso = 0.0;
           TLorentzVector trkLVec = (*template_chgTrksLVec)[it];
           vector<TLorentzVector> allpfCandsInAConeLVec = (*template_pfCandsInAConeLVec)[it];
           vector<int> allchgOfpfCandsInACone = (*template_chgOfpfCandsInACone)[it];
           vector<double> alldzOfpfCandsInACone = (*template_dzOfpfCandsInACone)[it];

           int cntChgTrks =0, totChg =0;
           double totPtChgTrks =0, totPtNeuPtcls =0;
           double sameChgtrkIso = 0.0;
           for(unsigned int jt=0; jt<allpfCandsInAConeLVec.size(); jt++){

              if( fabs(alldzOfpfCandsInACone[jt]) > dzcut_ ) continue;

              if( allchgOfpfCandsInACone[jt] !=0 ){
                 cntChgTrks ++;
                 totPtChgTrks += allpfCandsInAConeLVec[jt].Pt();
              }else{
                 totPtNeuPtcls += allpfCandsInAConeLVec[jt].Pt();
              }
              totChg += allchgOfpfCandsInACone[jt];
              
              if( allchgOfpfCandsInACone[jt] == 0 ) continue;

              TLorentzVector trkInAConeLVec = allpfCandsInAConeLVec[jt];

              trkIso += trkInAConeLVec.Pt();
              if( trkCharge == allchgOfpfCandsInACone[jt] ) sameChgtrkIso += trkInAConeLVec.Pt();
           }

           if( fabs((*template_dzOfTrks)[it]) > dzcut_ ) continue;

//           if( cntChgTrks != 2 ) continue;
/*
           if( cntChgTrks == 2 && totChg !=0 ) continue;
           if( cntChgTrks == 2 && totChg ==0 ) trkIso = sameChgtrkIso;
*/
           template_h1_nChgTrks->Fill(cntChgTrks, template_evtWeight);
           template_h1_totChg->Fill(totChg, template_evtWeight);
           template_h1_chgTrkPt->Fill(trkLVec.Pt(), template_evtWeight);
           template_h1_chgTrkEta->Fill(trkLVec.Eta(), template_evtWeight);
           template_h1_totPtChgTrks->Fill(totPtChgTrks, template_evtWeight); 
           template_h1_totPtNeuPtcls->Fill(totPtNeuPtcls, template_evtWeight); 

           template_h2_nChgTrksVStotChg->Fill(totChg, cntChgTrks, template_evtWeight);
           template_h2_totPtChgTrksVSchgTrkPt->Fill(trkLVec.Pt(), totPtChgTrks, template_evtWeight);

           template_h2_avgPtChgTrksVSchgTrkPt->Fill(trkLVec.Pt(), totPtChgTrks/cntChgTrks, template_evtWeight);

           template_h1_trkIsoVal->Fill(trkIso/trkLVec.Pt(), template_evtWeight);

           template_h2_trkIsoValVSnChgTrks->Fill(cntChgTrks, trkIso/trkLVec.Pt(), template_evtWeight);
           template_h2_trkIsoValVSchgTrkPt->Fill(trkLVec.Pt(), trkIso/trkLVec.Pt(), template_evtWeight);
           template_h2_trkIsoValVSavgChgTrksPt->Fill(totPtChgTrks/cntChgTrks, trkIso/trkLVec.Pt(), template_evtWeight);

           if( trkIso/trkLVec.Pt() > isoCut_ ) continue;
//           if( (cntChgTrks == 2 && totChg ==0 && trkIso/trkLVec.Pt() > 0.2) || ( !(cntChgTrks == 2 && totChg ==0) && trkIso/trkLVec.Pt() > isoCut_ ) ) continue;

           cntIsoTrk ++;
        }
     }
     if( cntIsoTrk !=0 ) cntIsoVetoEvents ++; 
  }

  if( keyStringT.Contains("InNoHadTauEvents") ){
     h1_nChgTrksInNoHadTauVec.push_back((TH1D*)template_h1_nChgTrks->Clone());
     h1_totChgInNoHadTauVec.push_back((TH1D*)template_h1_totChg->Clone());
     h1_chgTrkPtInNoHadTauVec.push_back((TH1D*)template_h1_chgTrkPt->Clone());
     h1_chgTrkEtaInNoHadTauVec.push_back((TH1D*)template_h1_chgTrkEta->Clone());
     h1_totPtChgTrksInNoHadTauVec.push_back((TH1D*)template_h1_totPtChgTrks->Clone());
     h1_totPtNeuPtclsInNoHadTauVec.push_back((TH1D*)template_h1_totPtNeuPtcls->Clone());

     h2_nChgTrksVStotChgInNoHadTauVec.push_back((TH2D*)template_h2_nChgTrksVStotChg->Clone());

     h1_trkIsoValInNoHadTauVec.push_back((TH1D*)template_h1_trkIsoVal->Clone());
     h2_totPtChgTrksVSchgTrkPtInNoHadTauVec.push_back( (TH2D*) template_h2_totPtChgTrksVSchgTrkPt->Clone() );

     h2_trkIsoValVSnChgTrksInNoHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSnChgTrks->Clone() );

     h2_trkIsoValVSchgTrkPtInNoHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSchgTrkPt->Clone() );
     h2_trkIsoValVSavgChgTrksPtInNoHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSavgChgTrksPt->Clone() );

     h2_avgPtChgTrksVSchgTrkPtInNoHadTauVec.push_back( (TH2D*) template_h2_avgPtChgTrksVSchgTrkPt->Clone() );
  }else if( keyStringT.Contains("InHadTauEvents") ){
     h1_nChgTrksInHadTauVec.push_back((TH1D*)template_h1_nChgTrks->Clone());
     h1_totChgInHadTauVec.push_back((TH1D*)template_h1_totChg->Clone());
     h1_chgTrkPtInHadTauVec.push_back((TH1D*)template_h1_chgTrkPt->Clone());
     h1_chgTrkEtaInHadTauVec.push_back((TH1D*)template_h1_chgTrkEta->Clone());
     h1_totPtChgTrksInHadTauVec.push_back((TH1D*)template_h1_totPtChgTrks->Clone());
     h1_totPtNeuPtclsInHadTauVec.push_back((TH1D*)template_h1_totPtNeuPtcls->Clone());

     h2_nChgTrksVStotChgInHadTauVec.push_back((TH2D*)template_h2_nChgTrksVStotChg->Clone());

     h1_trkIsoValInHadTauVec.push_back((TH1D*)template_h1_trkIsoVal->Clone());
     h2_totPtChgTrksVSchgTrkPtInHadTauVec.push_back( (TH2D*) template_h2_totPtChgTrksVSchgTrkPt->Clone() );

     h2_trkIsoValVSnChgTrksInHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSnChgTrks->Clone() );

     h2_trkIsoValVSchgTrkPtInHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSchgTrkPt->Clone() );
     h2_trkIsoValVSavgChgTrksPtInHadTauVec.push_back((TH2D*) template_h2_trkIsoValVSavgChgTrksPt->Clone() );

     h2_avgPtChgTrksVSchgTrkPtInHadTauVec.push_back( (TH2D*) template_h2_avgPtChgTrksVSchgTrkPt->Clone() );
  }

  std::cout<<keyString.c_str()<<"  -->   cntIsoVetoEvents : "<<cntIsoVetoEvents<<std::endl;
  std::cout<<std::endl;
}

void studyIsoTrackFilter(){
 
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  initMCscales();

  initPUinput("PUData_dist.root", "pileup");

// TTJets
  TFile *T2tt_File = new TFile("T2tt/studyIsoTrackFilter.root");
  TTree *trkIsoInNoHadTauEvents_AUX = (TTree*) T2tt_File->Get("trackIsolationFilterInNoHadTauEvents/AUX");
  templatePlotsFunc(trkIsoInNoHadTauEvents_AUX, "T2tt_InNoHadTauEvents");
  TTree *trkIsoInHadTauEvents_AUX = (TTree*) T2tt_File->Get("trackIsolationFilterInHadTauEvents/AUX");
  templatePlotsFunc(trkIsoInHadTauEvents_AUX, "T2tt_InHadTauEvents");

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

  tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

//  tdrStyle->SetOptStat(1111);

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
  c1->Divide(3, 2);

  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){
     
     TString keyStringT(keyStringCachedVec[it]);
     TObjArray * vlist = keyStringT.Tokenize("_");
     TString sharedKeyT = dynamic_cast<TObjString*>(vlist->At(0))->GetString();

     if( it%2 == 0 ){
        c1->cd(1); TPad *pad1 = (TPad*) c1->GetPad(1); pad1->Clear();
        pad1->SetLogy(); h1_nChgTrksInNoHadTauVec[it/2]->SetLineColor(kRed); h1_nChgTrksInNoHadTauVec[it/2]->Draw();

        c1->cd(2); TPad *pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
        pad2->SetLogy(); h1_totChgInNoHadTauVec[it/2]->SetLineColor(kRed); h1_totChgInNoHadTauVec[it/2]->Draw();

        c1->cd(3); TPad *pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
        pad3->SetLogy(); h1_chgTrkPtInNoHadTauVec[it/2]->SetLineColor(kRed); h1_chgTrkPtInNoHadTauVec[it/2]->Draw();

        c1->cd(4); TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
        pad4->SetLogy(); h1_chgTrkEtaInNoHadTauVec[it/2]->SetLineColor(kRed); h1_chgTrkEtaInNoHadTauVec[it/2]->Draw();

        c1->cd(5); TPad *pad5 = (TPad*) c1->GetPad(5); pad5->Clear();
        pad5->SetLogy(); h1_totPtChgTrksInNoHadTauVec[it/2]->SetLineColor(kRed); h1_totPtChgTrksInNoHadTauVec[it/2]->Draw();

        c1->cd(6); TPad *pad6 = (TPad*) c1->GetPad(6); pad6->Clear();
        pad6->SetLogy(); h1_totPtNeuPtclsInNoHadTauVec[it/2]->SetLineColor(kRed); h1_totPtNeuPtclsInNoHadTauVec[it/2]->Draw();
     }
     if( it%2 == 1 ){

        c1->cd(1); 
        h1_nChgTrksInHadTauVec[it/2]->SetLineColor(kBlue); h1_nChgTrksInHadTauVec[it/2]->Draw("same");

        c1->cd(2);
        h1_totChgInHadTauVec[it/2]->SetLineColor(kBlue); h1_totChgInHadTauVec[it/2]->Draw("same");

        c1->cd(3);
        h1_chgTrkPtInHadTauVec[it/2]->SetLineColor(kBlue); h1_chgTrkPtInHadTauVec[it/2]->Draw("same");

        c1->cd(4);
        h1_chgTrkEtaInHadTauVec[it/2]->SetLineColor(kBlue); h1_chgTrkEtaInHadTauVec[it/2]->Draw("same");

        c1->cd(5);
        h1_totPtChgTrksInHadTauVec[it/2]->SetLineColor(kBlue); h1_totPtChgTrksInHadTauVec[it/2]->Draw("same");

        c1->cd(6);
        h1_totPtNeuPtclsInHadTauVec[it/2]->SetLineColor(kBlue); h1_totPtNeuPtclsInHadTauVec[it/2]->Draw("same");

        c1->Print("studyIsoTrackFilter_"+sharedKeyT+".pdf");

        c1->Print("studyIsoTrackFilter_"+sharedKeyT+"_2D.pdf[");

        c1->cd(1); TPad *pad1 = (TPad*) c1->GetPad(1); pad1->Clear(); pad1->SetLogy(0);
        h2_nChgTrksVStotChgInNoHadTauVec[it/2]->Draw("colz");

        c1->cd(2); TPad *pad2 = (TPad*) c1->GetPad(2); pad2->Clear(); pad2->SetLogy(0);
        h2_nChgTrksVStotChgInHadTauVec[it/2]->Draw("colz");

        c1->cd(3); TPad *pad3 = (TPad*) c1->GetPad(3); pad3->Clear(); pad3->SetLogy(1);
        h1_trkIsoValInNoHadTauVec[it/2]->SetLineColor(kRed); h1_trkIsoValInNoHadTauVec[it/2]->Draw();
        h1_trkIsoValInHadTauVec[it/2]->SetLineColor(kBlue); h1_trkIsoValInHadTauVec[it/2]->Draw("same");
        c1->cd(4); TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear(); pad4->SetLogy(0);
        h2_totPtChgTrksVSchgTrkPtInNoHadTauVec[it/2]->Draw("colz");        
        c1->cd(5); TPad *pad5 = (TPad*) c1->GetPad(5); pad5->Clear(); pad5->SetLogy(0);
        h2_totPtChgTrksVSchgTrkPtInHadTauVec[it/2]->Draw("colz");
        c1->cd(6); TPad *pad6 = (TPad*) c1->GetPad(6); pad6->Clear(); pad6->SetLogy(0);

        c1->Print("studyIsoTrackFilter_"+sharedKeyT+"_2D.pdf");

        c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->Clear(); pad1->SetLogy(0);
        h2_trkIsoValVSnChgTrksInNoHadTauVec[it/2]->Draw("colz");

        c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear(); pad2->SetLogy(0);
        h2_trkIsoValVSnChgTrksInHadTauVec[it/2]->Draw("colz");

        c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear(); pad3->SetLogy(0);
        h2_trkIsoValVSchgTrkPtInNoHadTauVec[it/2]->Draw("colz");
        c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear(); pad4->SetLogy(0);
        h2_trkIsoValVSchgTrkPtInHadTauVec[it/2]->Draw("colz");
        c1->cd(5); pad5 = (TPad*) c1->GetPad(5); pad5->Clear(); pad5->SetLogy(0);
        h2_trkIsoValVSavgChgTrksPtInNoHadTauVec[it/2]->Draw("colz");
        c1->cd(6); pad6 = (TPad*) c1->GetPad(6); pad6->Clear(); pad6->SetLogy(0);
        h2_trkIsoValVSavgChgTrksPtInHadTauVec[it/2]->Draw("colz");

        c1->Print("studyIsoTrackFilter_"+sharedKeyT+"_2D.pdf");

        c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->Clear(); pad1->SetLogy(0);
        h2_avgPtChgTrksVSchgTrkPtInNoHadTauVec[it/2]->Draw("colz");

        c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear(); pad2->SetLogy(0);
        h2_avgPtChgTrksVSchgTrkPtInHadTauVec[it/2]->Draw("colz");

        c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear(); pad3->SetLogy(0);
        c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear(); pad4->SetLogy(0);
        c1->cd(5); pad5 = (TPad*) c1->GetPad(5); pad5->Clear(); pad5->SetLogy(0);
        c1->cd(6); pad6 = (TPad*) c1->GetPad(6); pad6->Clear(); pad6->SetLogy(0);

        c1->Print("studyIsoTrackFilter_"+sharedKeyT+"_2D.pdf");
        c1->Print("studyIsoTrackFilter_"+sharedKeyT+"_2D.pdf]");
        
     }
  }
}


double weightTruNPV(int trunpv){

   double w = 1.;
   if (trunpv < static_cast<int> (puWeights_.size())) {
      w = puWeights_.at(trunpv);
   } else {
      std::cerr << "WARNING ... number of PU vertices = " << trunpv
            << " out of histogram binning." << std::endl;
   }

   return w;
}


int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName){

   puWeights_.clear();

   TFile *puDatafile = new TFile(puDatafileName.c_str());
   TH1D *data_npu_estimated = (TH1D*) puDatafile->Get(puDatahistName.c_str());
 
// Distribution used for Summer2012 MC.
   Double_t Summer2012[60] = {
      2.344E-05,
      2.344E-05,
      2.344E-05,
      2.344E-05,
      4.687E-04,
      4.687E-04,
      7.032E-04,
      9.414E-04,
      1.234E-03,
      1.603E-03,
      2.464E-03,
      3.250E-03,
      5.021E-03,
      6.644E-03,
      8.502E-03,
      1.121E-02,
      1.518E-02,
      2.033E-02,
      2.608E-02,
      3.171E-02,
      3.667E-02,
      4.060E-02,
      4.338E-02,
      4.520E-02,
      4.641E-02,
      4.735E-02,
      4.816E-02,
      4.881E-02,
      4.917E-02,
      4.909E-02,
      4.842E-02,
      4.707E-02,
      4.501E-02,
      4.228E-02,
      3.896E-02,
      3.521E-02,
      3.118E-02,
      2.702E-02,
      2.287E-02,
      1.885E-02,
      1.508E-02,
      1.166E-02,
      8.673E-03,
      6.190E-03,
      4.222E-03,
      2.746E-03,
      1.698E-03,
      9.971E-04,
      5.549E-04,
      2.924E-04,
      1.457E-04,
      6.864E-05,
      3.054E-05,
      1.282E-05,
      5.081E-06,
      1.898E-06,
      6.688E-07,
      2.221E-07,
      6.947E-08,
      2.047E-08
   };

   puWeights_.resize(60);
   double s = 0.0;
   for (int npu = 0; npu < 60; ++npu) {
      double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));
      puWeights_[npu] = npu_estimated / Summer2012[npu];
      s += npu_estimated;
   }

   // normalize weights such that the total sum of weights over the whole sample is 1.0, i.e., sum_i  puWeights_[i] * Summer2012[i] should be 1.0 (!)
   for (int npu = 0; npu < 60; ++npu) {
      puWeights_[npu] /= s;
   }

   return 1;
}
