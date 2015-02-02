#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "TColor.h"

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

// Default to be -1
const double jet6ptcut_ = 40; //30, 35, 40

unsigned int njetsforptcuts = 6;
//                              0    1    2    3   4   5
const double jetptcuts01[] = { 80,  80,  30,  30, 30, 30};
const double jetptcuts02[] = { 80,  80,  40,  40, 40, 40};
const double jetptcuts03[] = { 80,  80,  50,  50, 50, 50};
const double jetptcuts04[] = {100, 100, 100, 100, 40, 40};
const double metcut_ = 170;

vector<string> keyStringCachedVec;
vector<TH1D*> h1_jet1ptVec, h1_jet2ptVec, h1_jet3ptVec, h1_jet4ptVec, h1_jet5ptVec, h1_jet6ptVec;
vector<TH1D*> h1_jet1etaVec, h1_jet2etaVec, h1_jet3etaVec, h1_jet4etaVec, h1_jet5etaVec, h1_jet6etaVec;
vector<TH1D*> h1_htVec;

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
  double template_jet1pt, template_jet1eta, template_jet1phi, template_jet1energy;
  double template_jet2pt, template_jet2eta, template_jet2phi, template_jet2energy;
  double template_jet3pt, template_jet3eta, template_jet3phi, template_jet3energy;
  vector<double> *template_otherJetspt =0, *template_otherJetseta =0, *template_otherJetsphi =0, *template_otherJetsenergy =0;
  double template_met;

  template_AUX->SetBranchAddress("run", &template_run);
  template_AUX->SetBranchAddress("event", &template_event);
  template_AUX->SetBranchAddress("lumi", &template_lumi);
  template_AUX->SetBranchAddress("avg_npv", &template_avg_npv);
  template_AUX->SetBranchAddress("nm1", &template_nm1);
  template_AUX->SetBranchAddress("n0", &template_n0);
  template_AUX->SetBranchAddress("np1", &template_np1);
//  template_AUX->SetBranchAddress("tru_npv", &template_tru_npv);
  template_AUX->SetBranchAddress("vtxSize", &template_vtxSize);
  template_AUX->SetBranchAddress("evtWeight", &template_evtWeight);
  template_AUX->SetBranchAddress("jet1pt", &template_jet1pt);
  template_AUX->SetBranchAddress("jet2pt", &template_jet2pt);
  template_AUX->SetBranchAddress("jet3pt", &template_jet3pt);
  template_AUX->SetBranchAddress("otherJetspt", &template_otherJetspt);
  template_AUX->SetBranchAddress("jet1eta", &template_jet1eta);
  template_AUX->SetBranchAddress("jet2eta", &template_jet2eta);
  template_AUX->SetBranchAddress("jet3eta", &template_jet3eta);
  template_AUX->SetBranchAddress("otherJetseta", &template_otherJetseta);
  template_AUX->SetBranchAddress("jet1phi", &template_jet1phi);
  template_AUX->SetBranchAddress("jet2phi", &template_jet2phi);
  template_AUX->SetBranchAddress("jet3phi", &template_jet3phi);
  template_AUX->SetBranchAddress("otherJetsphi", &template_otherJetsphi);
  template_AUX->SetBranchAddress("jet1energy", &template_jet1energy);
  template_AUX->SetBranchAddress("jet2energy", &template_jet2energy);
  template_AUX->SetBranchAddress("jet3energy", &template_jet3energy);
  template_AUX->SetBranchAddress("otherJetsenergy", &template_otherJetsenergy);
  template_AUX->SetBranchAddress("met", &template_met);

  int template_Entries = template_AUX->GetEntries();
  std::cout<<"\n\n"<<keyString.c_str()<<"_Entries : "<<template_Entries<<"  scaleMC : "<<scaleMC<<std::endl;

  TH1D *template_h1_jet1pt = new TH1D(keyStringT+"_jet1pt", keyStringT+" : pt of jet1; P_{T} [GeV]", 100, 0, 800);
  TH1D *template_h1_jet2pt = new TH1D(keyStringT+"_jet2pt", keyStringT+" : pt of jet2; P_{T} [GeV]", 100, 0, 600);
  TH1D *template_h1_jet3pt = new TH1D(keyStringT+"_jet3pt", keyStringT+" : pt of jet3; P_{T} [GeV]", 100, 0, 500);
  TH1D *template_h1_jet4pt = new TH1D(keyStringT+"_jet4pt", keyStringT+" : pt of jet4; P_{T} [GeV]", 100, 0, 400);
  TH1D *template_h1_jet5pt = new TH1D(keyStringT+"_jet5pt", keyStringT+" : pt of jet5; P_{T} [GeV]", 100, 0, 300);
  TH1D *template_h1_jet6pt = new TH1D(keyStringT+"_jet6pt", keyStringT+" : pt of jet6; P_{T} [GeV]", 100, 0, 200);

  TH1D *template_h1_jet1eta = new TH1D(keyStringT+"_jet1eta", keyStringT+" : eta of jet1; #eta", 100, -5, 5);
  TH1D *template_h1_jet2eta = new TH1D(keyStringT+"_jet2eta", keyStringT+" : eta of jet2; #eta", 100, -5, 5);
  TH1D *template_h1_jet3eta = new TH1D(keyStringT+"_jet3eta", keyStringT+" : eta of jet3; #eta", 100, -5, 5);
  TH1D *template_h1_jet4eta = new TH1D(keyStringT+"_jet4eta", keyStringT+" : eta of jet4; #eta", 100, -5, 5);
  TH1D *template_h1_jet5eta = new TH1D(keyStringT+"_jet5eta", keyStringT+" : eta of jet5; #eta", 100, -5, 5);
  TH1D *template_h1_jet6eta = new TH1D(keyStringT+"_jet6eta", keyStringT+" : eta of jet6; #eta", 100, -5, 5);

  TH1D *template_h1_ht = new TH1D(keyStringT+"_ht", keyStringT+" : HT; H_{T} [GeV]", 100, 0, 1500);

  double cntjetptcuts01 =0, cntjetptcuts02 =0, cntjetptcuts03 =0, cntjetptcuts04 =0;
  double cntjetptcuts01PLUSmet =0, cntjetptcuts02PLUSmet =0, cntjetptcuts03PLUSmet =0, cntjetptcuts04PLUSmet = 0;
  double cntmetcut =0;

  for(int ie=0; ie<template_Entries; ie++){

     template_AUX->GetEntry(ie);

     double puWeight = 1.0;
// Because signal is fastsim and does not have PU correctly done!
/*
     if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
        puWeight = weightTruNPV(template_tru_npv);
     }
*/

     double ht =0;

     double jet6pt = -1;

     if( template_otherJetspt->size() >=3 ) jet6pt = (*template_otherJetspt)[2];

     vector<double> cachedjetptVec;

     if( template_jet1pt >=0 ){
        if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
           template_h1_jet1pt->Fill(template_jet1pt, template_evtWeight*puWeight);
           template_h1_jet1eta->Fill(template_jet1eta, template_evtWeight*puWeight);
        }
        if( template_jet1pt > 50 && fabs(template_jet1eta) < 2.5 ) ht += template_jet1pt;
        cachedjetptVec.push_back(template_jet1pt);
     }
     if( template_jet2pt >=0 ){
        if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
           template_h1_jet2pt->Fill(template_jet2pt, template_evtWeight*puWeight);
           template_h1_jet2eta->Fill(template_jet2eta, template_evtWeight*puWeight);
        }
        if( template_jet2pt > 50 && fabs(template_jet2eta) < 2.5 ) ht += template_jet2pt;
        cachedjetptVec.push_back(template_jet2pt);
     }
     if( template_jet3pt >=0 ){
        if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
           template_h1_jet3pt->Fill(template_jet3pt, template_evtWeight*puWeight);
           template_h1_jet3eta->Fill(template_jet3eta, template_evtWeight*puWeight);
        }
        if( template_jet3pt > 50 && fabs(template_jet3eta) < 2.5 ) ht += template_jet3pt;
        cachedjetptVec.push_back(template_jet3pt);
     }

     for(unsigned int io=0; io<template_otherJetspt->size(); io++){
        if( (*template_otherJetspt)[io] >0 ){

           if( (*template_otherJetspt)[io] > 50 && fabs( (*template_otherJetseta)[io] ) < 2.5 ) ht += (*template_otherJetspt)[io];

           if( io == 0 ){
              if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
                 template_h1_jet4pt->Fill((*template_otherJetspt)[io], template_evtWeight*puWeight);
                 template_h1_jet4eta->Fill((*template_otherJetseta)[io], template_evtWeight*puWeight);
              }
              cachedjetptVec.push_back((*template_otherJetspt)[io]);
           }
           if( io == 1 ){
              if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
                 template_h1_jet5pt->Fill((*template_otherJetspt)[io], template_evtWeight*puWeight);
                 template_h1_jet5eta->Fill((*template_otherJetseta)[io], template_evtWeight*puWeight);
              }
              cachedjetptVec.push_back((*template_otherJetspt)[io]);
           }
           if( io == 2 ){
              if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
                 template_h1_jet6pt->Fill((*template_otherJetspt)[io], template_evtWeight*puWeight);
                 template_h1_jet6eta->Fill((*template_otherJetseta)[io], template_evtWeight*puWeight);
              }
              cachedjetptVec.push_back((*template_otherJetspt)[io]);
           }
        }
     }

     if( jet6ptcut_ == -1 || jet6pt > jet6ptcut_ ){
        template_h1_ht->Fill(ht, template_evtWeight*puWeight);
     }

     if( template_met > metcut_ ) cntmetcut += template_evtWeight*puWeight;
     unsigned int ncachedjetpt = cachedjetptVec.size();
     bool passAllJetCuts = true;
     if( ncachedjetpt == njetsforptcuts ){
        passAllJetCuts = true; for(unsigned ij=0; ij<njetsforptcuts; ij++){ if( cachedjetptVec[ij] < jetptcuts01[ij] ) passAllJetCuts = false; } if( passAllJetCuts ){ cntjetptcuts01 += template_evtWeight*puWeight; if( template_met > metcut_ ) cntjetptcuts01PLUSmet += template_evtWeight*puWeight; }
        passAllJetCuts = true; for(unsigned ij=0; ij<njetsforptcuts; ij++){ if( cachedjetptVec[ij] < jetptcuts02[ij] ) passAllJetCuts = false; } if( passAllJetCuts ){ cntjetptcuts02 += template_evtWeight*puWeight; if( template_met > metcut_ ) cntjetptcuts02PLUSmet += template_evtWeight*puWeight; }
        passAllJetCuts = true; for(unsigned ij=0; ij<njetsforptcuts; ij++){ if( cachedjetptVec[ij] < jetptcuts03[ij] ) passAllJetCuts = false; } if( passAllJetCuts ){ cntjetptcuts03 += template_evtWeight*puWeight; if( template_met > metcut_ ) cntjetptcuts03PLUSmet += template_evtWeight*puWeight; }
        passAllJetCuts = true; for(unsigned ij=0; ij<njetsforptcuts; ij++){ if( cachedjetptVec[ij] < jetptcuts04[ij] ) passAllJetCuts = false; } if( passAllJetCuts ){ cntjetptcuts04 += template_evtWeight*puWeight; if( template_met > metcut_ ) cntjetptcuts04PLUSmet += template_evtWeight*puWeight; }
     }
  }

  printf("%20s : %7.0f  eff : %8.6f\n", "cntmetcut", cntmetcut, cntmetcut/template_Entries);
  printf("%20s : %7.0f  eff : %8.6f  -->  %20s : %7.0f  eff : %8.6f\n", "cntjetptcuts01", cntjetptcuts01, cntjetptcuts01/template_Entries, "cntjetptcuts01PLUSmet", cntjetptcuts01PLUSmet, cntjetptcuts01PLUSmet/template_Entries);
  printf("%20s : %7.0f  eff : %8.6f  -->  %20s : %7.0f  eff : %8.6f\n", "cntjetptcuts02", cntjetptcuts02, cntjetptcuts02/template_Entries, "cntjetptcuts02PLUSmet", cntjetptcuts02PLUSmet, cntjetptcuts02PLUSmet/template_Entries);
  printf("%20s : %7.0f  eff : %8.6f  -->  %20s : %7.0f  eff : %8.6f\n", "cntjetptcuts03", cntjetptcuts03, cntjetptcuts03/template_Entries, "cntjetptcuts03PLUSmet", cntjetptcuts03PLUSmet, cntjetptcuts03PLUSmet/template_Entries);
  printf("%20s : %7.0f  eff : %8.6f  -->  %20s : %7.0f  eff : %8.6f\n", "cntjetptcuts04", cntjetptcuts04, cntjetptcuts04/template_Entries, "cntjetptcuts04PLUSmet", cntjetptcuts04PLUSmet, cntjetptcuts04PLUSmet/template_Entries);
  std::cout<<std::endl;

  h1_jet1ptVec.push_back((TH1D*) template_h1_jet1pt->Clone());
  h1_jet2ptVec.push_back((TH1D*) template_h1_jet2pt->Clone());
  h1_jet3ptVec.push_back((TH1D*) template_h1_jet3pt->Clone());
  h1_jet4ptVec.push_back((TH1D*) template_h1_jet4pt->Clone());
  h1_jet5ptVec.push_back((TH1D*) template_h1_jet5pt->Clone());
  h1_jet6ptVec.push_back((TH1D*) template_h1_jet6pt->Clone());

  h1_jet1etaVec.push_back((TH1D*) template_h1_jet1eta->Clone());
  h1_jet2etaVec.push_back((TH1D*) template_h1_jet2eta->Clone());
  h1_jet3etaVec.push_back((TH1D*) template_h1_jet3eta->Clone());
  h1_jet4etaVec.push_back((TH1D*) template_h1_jet4eta->Clone());
  h1_jet5etaVec.push_back((TH1D*) template_h1_jet5eta->Clone());
  h1_jet6etaVec.push_back((TH1D*) template_h1_jet6eta->Clone());

  h1_htVec.push_back((TH1D*) template_h1_ht->Clone());
}

void studyTrigger(){

  initMCscales();

  initPUinput("PUData_dist.root", "pileup");

// xpar500_ypar100
  TFile *xpar500_ypar100_File = new TFile("xpar500_ypar100/directStop_merged.root");
  TTree *xpar500_ypar100_histAndTree_noCuts_AUX = (TTree*) xpar500_ypar100_File->Get("histAndTree_recoLevelEleMuTauVeto_NoCuts/AUX");
  templatePlotsFunc(xpar500_ypar100_histAndTree_noCuts_AUX, "xpar500_ypar100");

// xpar350_ypar50
  TFile *xpar350_ypar50_File = new TFile("xpar350_ypar50/directStop_merged.root");
  TTree *xpar350_ypar50_histAndTree_noCuts_AUX = (TTree*) xpar350_ypar50_File->Get("histAndTree_recoLevelEleMuTauVeto_NoCuts/AUX");
  templatePlotsFunc(xpar350_ypar50_histAndTree_noCuts_AUX, "xpar350_ypar50");

// TTJets
  TFile *TTJets_File = new TFile("TTJets/directStop_merged.root");
  TTree *TTJets_histAndTree_noCuts_AUX = (TTree*) TTJets_File->Get("histAndTree_recoLevelEleMuTauVeto_NoCuts/AUX");
//  templatePlotsFunc(TTJets_histAndTree_noCuts_AUX, "TTJets");

  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(3, 2);

  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){

     TString keyStringT(keyStringCachedVec[it]);

     c1->Print("studyTrigger_"+keyStringT+".pdf[");
 
     c1->cd(1); TPad *pad1 = (TPad*) c1->GetPad(1); pad1->Clear();
     pad1->SetLogy(); h1_jet1ptVec[it]->Draw(); 

     c1->cd(2); TPad *pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
     pad2->SetLogy(); h1_jet2ptVec[it]->Draw(); 

     c1->cd(3); TPad *pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
     pad3->SetLogy(); h1_jet3ptVec[it]->Draw(); 

     c1->cd(4); TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
     pad4->SetLogy(); h1_jet4ptVec[it]->Draw(); 

     c1->cd(5); TPad *pad5 = (TPad*) c1->GetPad(5); pad5->Clear();
     pad5->SetLogy(); h1_jet5ptVec[it]->Draw(); 

     c1->cd(6); TPad *pad6 = (TPad*) c1->GetPad(6); pad6->Clear();
     pad6->SetLogy(); h1_jet6ptVec[it]->Draw(); 

     c1->Print("studyTrigger_"+keyStringT+".pdf");

     c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->Clear();
     pad1->SetLogy(); h1_jet1etaVec[it]->Draw(); 

     c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
     pad2->SetLogy(); h1_jet2etaVec[it]->Draw(); 

     c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
     pad3->SetLogy(); h1_jet3etaVec[it]->Draw(); 

     c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
     pad4->SetLogy(); h1_jet4etaVec[it]->Draw(); 

     c1->cd(5); pad5 = (TPad*) c1->GetPad(5); pad5->Clear();
     pad5->SetLogy(); h1_jet5etaVec[it]->Draw(); 

     c1->cd(6); pad6 = (TPad*) c1->GetPad(6); pad6->Clear();
     pad6->SetLogy(); h1_jet6etaVec[it]->Draw(); 

     c1->Print("studyTrigger_"+keyStringT+".pdf");

     c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->Clear();
     pad1->SetLogy(); h1_htVec[it]->Draw(); 

     c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();

     c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();

     c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();

     c1->cd(5); pad5 = (TPad*) c1->GetPad(5); pad5->Clear();

     c1->cd(6); pad6 = (TPad*) c1->GetPad(6); pad6->Clear();

     c1->Print("studyTrigger_"+keyStringT+".pdf");
     c1->Print("studyTrigger_"+keyStringT+".pdf]");
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
