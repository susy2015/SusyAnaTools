
// execute in batch mode to avoid too many histograms

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "TH2D.h"
#include "TCanvas.h"


static const int nJetBins = 3;
static const int cutLonJetsArr[] = { 3, 6,  8 };
static const int cutHinJetsArr[] = { 6, 8, -1 };

static const double cutLoHTArr[]  = { 500, 500, 500, 500,  800,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, /**/  500,  500,  500,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, /**/  500,  800,  800, 1000, 1000, 1250, 1250, 1500, 1500 };
static const double cutHiHTArr[]  = { 800, 800, 800, 800, 1000, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1250, 1500, 1500, 1500, 1500,   -1,   -1,   -1, /**/  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, 1500,   -1,   -1,   -1, /**/  800, 1000, 1000, 1250, 1250, 1500, 1500,   -1,   -1 };
static const double cutLoMHTArr[] = { 200, 300, 450, 600,  100,  200,  300,  450,  600,  100,  200,  300,  450,  600,  100,  200,  300,  450,  100,  200,  300, /**/  200,  300,  450,  100,  200,  300,  450,  100,  200,  300,  450,  100,  200,  300,  450,  100,  200,  300, /**/  200,  100,  200,  100,  200,  100,  200,  100,  200 };
static const double cutHiMHTArr[] = { 300, 450, 600,  -1,  200,  300,  450,  600,   -1,  200,  300,  450,  600,   -1,  200,  300,  450,   -1,  200,  300,   -1, /**/  300,  450,   -1,  200,  300,  450,   -1,  200,  300,  450,   -1,  200,  300,  450,   -1,  200,  300,   -1, /**/   -1,  200,   -1,  200,   -1,  200,   -1,  200,   -1 };

static const int nStages = sizeof(cutLoHTArr)/sizeof(cutLoHTArr[0]);

void plotAllT1tttt(){

  std::cout << sizeof(cutLoHTArr) << " " << sizeof(cutLoHTArr[0]) << " " << nStages << std::endl;

  char names[200];
  int cntJetBin = 0;
  char njet1[100], njet2[100], ht1[100], ht2[100], mht1[100], mht2[100];
  for(int is=0; is<nStages; is++){
    sprintf(njet1, "%i",    cutLonJetsArr[cntJetBin]);
    sprintf(njet2, "%i",    cutHinJetsArr[cntJetBin]);
    if(cutHinJetsArr[cntJetBin]==-1) sprintf(njet2, "inf");
    sprintf(ht1,   "%3.0f", cutLoHTArr[is]);
    sprintf(ht2,   "%3.0f", cutHiHTArr[is]);
    if(cutHiHTArr[is]==-1) sprintf(ht2,   "inf");
    sprintf(mht1,  "%3.0f", cutLoMHTArr[is]);
    sprintf(mht2,  "%3.0f", cutHiMHTArr[is]);
    if(cutHiMHTArr[is]==-1) sprintf(mht2,   "inf");

    if(cutHiHTArr[is] ==-1 && cutHiMHTArr[is] == -1 ) cntJetBin++;
    
    std::cout << " nJets " << njet1 << "-" << njet2
	      << " ht "    << ht1   << "-" << ht2
	      << " mht "   << mht1  << "-" << mht2
	      << std::endl;

    plotHistos(ht1, ht2, mht1, mht2, njet1, njet2, "T1tttt", true);
  }

}

void plotAllT1qqqq(){

  std::cout << sizeof(cutLoHTArr) << " " << sizeof(cutLoHTArr[0]) << " " << nStages << std::endl;

  char names[200];
  int cntJetBin = 0;
  char njet1[100], njet2[100], ht1[100], ht2[100], mht1[100], mht2[100];
  for(int is=0; is<nStages; is++){
    sprintf(njet1, "%i",    cutLonJetsArr[cntJetBin]);
    sprintf(njet2, "%i",    cutHinJetsArr[cntJetBin]);
    if(cutHinJetsArr[cntJetBin]==-1) sprintf(njet2, "inf");
    sprintf(ht1,   "%3.0f", cutLoHTArr[is]);
    sprintf(ht2,   "%3.0f", cutHiHTArr[is]);
    if(cutHiHTArr[is]==-1) sprintf(ht2,   "inf");
    sprintf(mht1,  "%3.0f", cutLoMHTArr[is]);
    sprintf(mht2,  "%3.0f", cutHiMHTArr[is]);
    if(cutHiMHTArr[is]==-1) sprintf(mht2,   "inf");

    if(cutHiHTArr[is] ==-1 && cutHiMHTArr[is] == -1 ) cntJetBin++;
    
    std::cout << " nJets " << njet1 << "-" << njet2
	      << " ht "    << ht1   << "-" << ht2
	      << " mht "   << mht1  << "-" << mht2
	      << std::endl;

    plotHistos(ht1, ht2, mht1, mht2, njet1, njet2, "T1qqqq", true);
  }

}

void plotHistos(char *hht1, char *hht2, char *hmht1, char *hmht2, char *njet1, char *njet2, char *topo, bool exclusive) {

  bool ifPrint = false;
  bool writeUnc= true;
  bool doPDF   = true;
  bool doDraw = false;

  char someTxt[200];
  if(topo=="T1qqqq")
    sprintf(someTxt, "pp #rightArrow #tilde{G}#tilde{G}, #tilde{G}->Jet+Jet+LSP");
  else if(topo=="T2")
    sprintf(someTxt, "pp #rightArrow #tilde{Q}#tilde{Q}, #tilde{Q}->Jet+LSP");
  else if(topo=="T2tt")
    sprintf(someTxt, "pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
  else if(topo=="T5ZZInc")
    sprintf(someTxt, "pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow qqChi20 #rightarrow  Z+LSP");
  if(topo=="T1tttt")
    sprintf(someTxt, "pp #rightArrow #tilde{G}#tilde{G}, #tilde{G}->t+#bar{t}+LSP");

  gStyle->SetPadRightMargin(0.125);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  char fname[200];
  sprintf( fname, "RA2SMHists_Topology%s_EffJesJerPdfIsr.root", topo);
  TFile *_fin = new TFile(fname);
  
  TDirectory* STD = (TDirectory*) _fin->FindObjectAny("STD");
  TDirectory* JES = (TDirectory*) _fin->FindObjectAny("JES");
  TDirectory* JER = (TDirectory*) _fin->FindObjectAny("JER");
  TDirectory* PDF = (TDirectory*) _fin->FindObjectAny("PDF");
  TDirectory* ISR = (TDirectory*) _fin->FindObjectAny("ISR");
  
  TH2F* h_NTotalRA2 = (TH2F*) STD->FindObjectAny("NTotal");  
  TH2F* h_NTotalISR = (TH2F*) ISR->FindObjectAny("NTotal_ISRFSR");

  char htmht[200];
  char hname[200];
  char hnameJesUp [200];
  char hnameJesDn [200];
  char hnameJerNom[200];
  char hnameJerUp [200];
  char hnameJerDn [200];
  char hnameIsrFsr[200];
  if( exclusive ) {
    sprintf(hname, "NPassed_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameJesUp, "NPassed_JESUP_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameJesDn, "NPassed_JESDN_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameJerNom,"NPassed_JER_%snJets%s_%sHT%s_%sMHT%s",   njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameJerUp, "NPassed_JERUP_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameJerDn, "NPassed_JERDN_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(hnameIsrFsr,"NPassed_ISRFSR_%snJets%s_%sHT%s_%sMHT%s", njet1, njet2, hht1, hht2, hmht1, hmht2);
    sprintf(htmht, "%sNJet%s_%sHT%s_%sMHT%s",        njet1, njet2, hht1, hht2, hmht1, hmht2);
  } else if {
    // in case test inclusive hists
    double ht  = ht1; 
    double mht = mht1;
    sprintf(hname, "h_NPassed_HT%s_MHT%s", hht, hmht);
    sprintf(htmht, "HT%s_MHT%s", hht, hmht);
  }


  TH2F* h_RA2Analysis    = (TH2F*) STD->FindObjectAny(hname);
  TH2F* h_RA2JesUP       = (TH2F*) JES->FindObjectAny(hnameJesUp);
  TH2F* h_RA2JesDN       = (TH2F*) JES->FindObjectAny(hnameJesDn);
  TH2F* h_RA2JerNominal  = (TH2F*) JER->FindObjectAny(hnameJerNom);
  TH2F* h_RA2JerUP       = (TH2F*) JER->FindObjectAny(hnameJerUp);
  TH2F* h_RA2JerDN       = (TH2F*) JER->FindObjectAny(hnameJerDn);
  TH2F* h_RA2AnalysisISR = (TH2F*) ISR->FindObjectAny(hnameIsrFsr);
 
  char cname[300], text[200], cprint[200];
  
  // Acceptance
  TH2F* h_RA2Acceptance  = h_RA2Analysis->Clone("h_RA2Acceptance");
  h_RA2Acceptance->Divide(h_NTotalRA2);
  h_RA2Acceptance->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2Acceptance->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2Acceptance->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2Acceptance->GetYaxis()->SetRangeUser(0, 1500);
  h_RA2Acceptance->GetYaxis()->SetTitleOffset(1.5);
  if( doDraw ){
     sprintf(cname, "c_Accptance_%s_%s", topo, htmht);
     TCanvas *c_h_Acceptance = new TCanvas(cname, cname, 500,500);
     h_RA2Acceptance->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.63, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     else if(topo=="T2tt")
       t_eff_1->AddText("pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Acceptance");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     t_eff_1->Draw("same");
   
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_Acceptance->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_Acceptance->Print(cprint);
  }


  //=================================================================================

  // jes up/down uncertainty
  TH2F* h_RA2Unc_JesUpDn  = h_RA2JesUP->Clone("h_RA2Unc_JesUpDn");
  h_RA2Unc_JesUpDn->Add(h_RA2JesDN, -1.0);
  h_RA2Unc_JesUpDn->Divide(h_RA2Analysis);
  h_RA2Unc_JesUpDn->Scale(0.5);
  h_RA2Unc_JesUpDn->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2Unc_JesUpDn->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2Unc_JesUpDn->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_JesUpDn->GetYaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_JesUpDn->GetYaxis()->SetTitleOffset(1.5);
  for(int ix=1; ix<=h_RA2Acceptance->GetNbinsX(); ix++){
    for(int iy=1; iy<=h_RA2Acceptance->GetNbinsY(); iy++){
      if( std::abs(h_RA2Acceptance->GetBinContent(ix,iy))==0 ) h_RA2Unc_JesUpDn->SetBinContent(ix, iy, -1.0);
    }
  }
  h_RA2Unc_JesUpDn->GetZaxis()->SetRangeUser(-0.2, 0.5);

  if( doDraw ){
     sprintf(cname, "c_RA2UncJesUpDn_%s_%s", topo, htmht);
     TCanvas *c_h_JesUpDn = new TCanvas(cname, cname, 500,500);
     h_RA2Unc_JesUpDn->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.63, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     else if(topo=="T2tt")
       t_eff_1->AddText("pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Uncertainty - JES up/down");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     t_eff_1->Draw("same");
   
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_JesUpDn->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_JesUpDn->Print(cprint);
  }
  //=================================================================================

  // jer up/down uncertainty
  TH2F* h_RA2Unc_JerUpDn  = h_RA2JerUP->Clone("h_RA2Unc_JerUpDn");
  h_RA2Unc_JerUpDn->Add(h_RA2JerDN, -1.0);
  h_RA2Unc_JerUpDn->Divide(h_RA2JerNominal);
  h_RA2Unc_JerUpDn->Scale(0.5);
  h_RA2Unc_JerUpDn->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2Unc_JerUpDn->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2Unc_JerUpDn->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_JerUpDn->GetYaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_JerUpDn->GetYaxis()->SetTitleOffset(1.5);
  for(int ix=1; ix<=h_RA2Acceptance->GetNbinsX(); ix++){
    for(int iy=1; iy<=h_RA2Acceptance->GetNbinsY(); iy++){
      if( std::abs(h_RA2Acceptance->GetBinContent(ix,iy))==0 ) h_RA2Unc_JerUpDn->SetBinContent(ix, iy, -1.0);
    }
  }
  h_RA2Unc_JerUpDn->GetZaxis()->SetRangeUser(-0.5, 0.5);

  if( doDraw ){
     sprintf(cname, "c_RA2UncJerUpDn_%s_%s", topo, htmht);
     TCanvas *c_h_JerUpDn = new TCanvas(cname, cname, 500,500);
     h_RA2Unc_JerUpDn->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.63, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     else if(topo=="T2tt")
       t_eff_1->AddText("pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Uncertainty - JER up/down");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     //t_eff_1->Draw("same");
   
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_JerUpDn->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_JerUpDn->Print(cprint);
  }
  //=================================================================================

  TH2F *h_RA2Unc_PDF = new TH2F("h_RA2Unc_PDF", "h_RA2Unc_PDF", 60,0,1500, 60,0,1500);
  if( doPDF ) {
  // pdf uncertainty
  TDirectory* PDF = (TDirectory*) _fin->FindObjectAny("PDF");
  
  //---CTEQ6
  TH2F* h_PDFWgt1_NTotal[45];
  TH2F* h_PDFWgt1_NPassed[45];
  for(int ii=0; ii<45; ii++) {
    sprintf(hname, "PDFWgt1_NTotal_%i", ii);
    h_PDFWgt1_NTotal[ii] = (TH2F*)PDF->FindObjectAny(hname);
    sprintf(hname, "PDFWgt1_NPassed_%i_%snJets%s_%sHT%s_%sMHT%s", ii, njet1, njet2, hht1, hht2, hmht1, hmht2);
    h_PDFWgt1_NPassed[ii] = (TH2F*)PDF->FindObjectAny(hname);
    //std::cout << h_PDFWgt1_NPassed[ii]->GetName() << std::endl;
  }
  //---MSTW
  TH2F* h_PDFWgt2_NTotal[41];
  TH2F* h_PDFWgt2_NPassed[41];
  for(int ii=0; ii<41; ii++) {
    sprintf(hname, "PDFWgt2_NTotal_%i", ii);
    h_PDFWgt2_NTotal[ii] = (TH2F*)PDF->FindObjectAny(hname);
    //sprintf(hname, "h_PDFWgt2_NPassed_HT%s_MHT%s_%i", ht, mht, ii);
    sprintf(hname, "PDFWgt2_NPassed_%i_%snJets%s_%sHT%s_%sMHT%s", ii, njet1, njet2, hht1, hht2, hmht1, hmht2);
    h_PDFWgt2_NPassed[ii] = (TH2F*)PDF->FindObjectAny(hname);
    //std::cout << h_PDFWgt2_NPassed[ii]->GetName() << std::endl;
  }
  //---NNPDF
  TH2F* h_PDFWgt3_NTotal[101];
  TH2F* h_PDFWgt3_NPassed[101];
  for(int ii=0; ii<101; ii++) {
    sprintf(hname, "PDFWgt3_NTotal_%i", ii);
    h_PDFWgt3_NTotal[ii] = (TH2F*)PDF->FindObjectAny(hname);
    //sprintf(hname, "h_PDFWgt3_NPassed_HT%s_MHT%s_%i", ht, mht, ii);
    sprintf(hname, "PDFWgt3_NPassed_%i_%snJets%s_%sHT%s_%sMHT%s", ii, njet1, njet2, hht1, hht2, hmht1, hmht2);
    h_PDFWgt3_NPassed[ii] = (TH2F*)PDF->FindObjectAny(hname);
    //std::cout << h_PDFWgt3_NPassed[ii]->GetName() << std::endl;
  }

  //TH2F *h_RA2Unc_PDF = new TH2F("h_RA2Unc_PDF", "h_RA2Unc_PDF", 60,0,1500, 60,0,1500);
  h_RA2Unc_PDF->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2Unc_PDF->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2Unc_PDF->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_PDF->GetYaxis()->SetRangeUser(0, 1500);
  //h_RA2Unc_PDF->GetZaxis()->SetRangeUser(-0.5, 0.5);
  h_RA2Unc_PDF->GetYaxis()->SetTitleOffset(1.5);

  TH2F *h_RA2CenValue_PDF = new TH2F("h_RA2CenValue_PDF", "h_RA2CenValue_PDF", 60,0,1500, 60,0,1500);
  h_RA2CenValue_PDF->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2CenValue_PDF->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2CenValue_PDF->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2CenValue_PDF->GetYaxis()->SetRangeUser(0, 1500);
  //h_RA2CenValue_PDF->GetZaxis()->SetRangeUser(-0.5, 0.5);
  h_RA2CenValue_PDF->GetYaxis()->SetTitleOffset(1.5);

  /*
  // debug output
  std::cout << "h_RA2Unc_PDF->GetNbinsX() " << h_RA2Unc_PDF->GetNbinsX()<<" " 
	    << "h_RA2Unc_PDF->GetNbinsY() " << h_RA2Unc_PDF->GetNbinsY()
	    << std::endl;
  */
  for(int ix=1; ix<=h_RA2Unc_PDF->GetNbinsX(); ix++){
    for(int iy=1; iy<=h_RA2Unc_PDF->GetNbinsY(); iy++){
      
      double mmo  = h_RA2Unc_PDF->GetXaxis()->GetBinCenter(ix);
      double mdau = h_RA2Unc_PDF->GetYaxis()->GetBinCenter(iy);

      if( h_NTotalRA2->GetBinContent(ix,iy)<= 0) continue;

      double var_eff[3];
      for (int i=0; i<3; i++ ) var_eff[i]=0.0;

      double eff_pdf1[45];
      for(int ii=0; ii<45; ii++) {
	eff_pdf1[ii]=0.0;
	if(h_PDFWgt1_NTotal[ii]->GetBinContent(ix,iy)>0) 
	  eff_pdf1[ii] = h_PDFWgt1_NPassed[ii]->GetBinContent(ix,iy)/h_PDFWgt1_NTotal[ii]->GetBinContent(ix,iy);     
	//if(mmo>500 && mmo<525 && mdau>250 && mdau<275)  std::cout << ix <<"  " << iy << " " << eff_pdf1[ii] << " " << std::endl;
      }
      for (int k=1;k<=22;k++) 
	var_eff[0] += 0.25*(eff_pdf1[2*k]-eff_pdf1[2*k-1])*(eff_pdf1[2*k]-eff_pdf1[2*k-1]);//---CTEQ6 
      //if(mmo>500 && mmo<525 && mdau>250 && mdau<275) std::cout << "ix " << ix << " iy " << iy << " " << eff_pdf1[0] << " " << var_eff[0] << std::endl;
      
      double eff_pdf2[41];
      for(int ii=0; ii<41; ii++) {
	eff_pdf2[ii] = -1.0;
	if(h_PDFWgt2_NTotal[ii]->GetBinContent(ix,iy)>0)
	  eff_pdf2[ii] = h_PDFWgt2_NPassed[ii]->GetBinContent(ix,iy)/h_PDFWgt2_NTotal[ii]->GetBinContent(ix,iy);
	//std::cout << ix <<"  " << iy << " " << eff_pdf2[ii] << " " << h_PDFWgt2_NTotal[ii]->GetBinContent(ix,iy) <<" " <<h_PDFWgt2_NPassed[ii]->GetBinContent(ix,iy) << std::endl;	
      }
      for (int k=1;k<=20;k++) {
	if( eff_pdf2[2*k] >= 0 &&  eff_pdf2[2*k-1] >= 0 ) 
	var_eff[1] += 0.25*(eff_pdf2[2*k]-eff_pdf2[2*k-1])*(eff_pdf2[2*k]-eff_pdf2[2*k-1]);
      }      
      //std::cout << var_eff[1] << std::endl;

      double eff_pdf3[101];
      for(int ii=0; ii<101; ii++) {
	eff_pdf3[ii] = -1.0;
	if(h_PDFWgt3_NTotal[ii]->GetBinContent(ix,iy)>0)
	  eff_pdf3[ii] = h_PDFWgt3_NPassed[ii]->GetBinContent(ix,iy)/h_PDFWgt3_NTotal[ii]->GetBinContent(ix,iy);	      }
      for (int k=1;k<=50;k++) 
	var_eff[2] += 0.25*(eff_pdf3[2*k]-eff_pdf3[2*k-1])*(eff_pdf3[2*k]-eff_pdf3[2*k-1]);
          
      for (int k=0;k<3;k++) var_eff[k]=std::sqrt(var_eff[k]);//----get the std error
      var_eff[0] /= 1.645;//----CTEQ6 variations correspond to 68% CL, so this factor extrapolates to 90%CL
      var_eff[2] /= std::sqrt(50.0);//----NNPDF variations are sampled from a Gaussian, so divide by sqrt(N)

      //---get central value and the error of the efficiency from the envelope of the 3 PDF set
      double eff_pdf_cen = 0.5*(TMath::Max(eff_pdf1[0]+var_eff[0],TMath::Max(eff_pdf2[0]+var_eff[1],eff_pdf3[0]+var_eff[2])) + TMath::Min(eff_pdf1[0]-var_eff[0],TMath::Min(eff_pdf2[0]-var_eff[1],eff_pdf3[0]-var_eff[2])));
      double eeff_pdf_sys = 0.5*(TMath::Max(eff_pdf1[0]+var_eff[0],TMath::Max(eff_pdf2[0]+var_eff[1],eff_pdf3[0]+var_eff[2])) - TMath::Min(eff_pdf1[0]-var_eff[0],TMath::Min(eff_pdf2[0]-var_eff[1],eff_pdf3[0]-var_eff[2])));

      h_RA2CenValue_PDF->SetBinContent(ix, iy, eff_pdf_cen);
      
      if( eff_pdf_cen>0 ) eeff_pdf_sys = eeff_pdf_sys/eff_pdf_cen;
      h_RA2Unc_PDF     ->SetBinContent(ix, iy, eeff_pdf_sys);
    }
  }

  if( doDraw ){
     sprintf(cname, "c_h_RA2CenValue_PDF_%s_%s", topo, htmht);
     TCanvas *c_h_RA2CenValue_PDF = new TCanvas(cname, cname, 500,500);
     h_RA2CenValue_PDF->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.63, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     else if(topo=="T2tt")
       t_eff_1->AddText("pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Mean value - PDF Variation");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     //t_eff_1->Draw("same");
   
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_RA2CenValue_PDF->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_RA2CenValue_PDF->Print(cprint);
   
     sprintf(cname, "c_h_RA2UncPDF_%s_%s", topo, htmht);
     TCanvas *c_h_PDF = new TCanvas(cname, cname, 500,500);
     if( doDraw ) h_RA2Unc_PDF->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.63, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Uncertainty - PDF Variation");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     //t_eff_1->Draw("same");
   
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_PDF->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_PDF->Print(cprint);
  }
  } // end if(doPDF)
  //=================================================================================

  // ISR/FSR uncertainrt
  TH2F* h_RA2Unc_IsrFsr = h_RA2Analysis->Clone("h_RA2Unc_IsrFsr");
  h_RA2Unc_IsrFsr->Add(h_RA2AnalysisISR, -1.0);
  h_RA2Unc_IsrFsr->Divide(h_RA2Analysis);  
  h_RA2Unc_IsrFsr->GetXaxis()->SetTitle("M_{Mother} (GeV)");
  h_RA2Unc_IsrFsr->GetYaxis()->SetTitle("M_{LSP} (GeV)");
  h_RA2Unc_IsrFsr->GetXaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_IsrFsr->GetYaxis()->SetRangeUser(0, 1500);
  h_RA2Unc_IsrFsr->GetYaxis()->SetTitleOffset(1.5);

  for(int ix=1; ix<=h_RA2Acceptance->GetNbinsX(); ix++){
    for(int iy=1; iy<=h_RA2Acceptance->GetNbinsY(); iy++){
      if( std::abs(h_RA2Acceptance->GetBinContent(ix,iy))==0 ) h_RA2Unc_IsrFsr->SetBinContent(ix, iy, -1.0);
    }
  }
  h_RA2Unc_IsrFsr->GetZaxis()->SetRangeUser(-0.1, 1.2);

  if( doDraw ){
     sprintf(cname, "c_IsrFsr_%s_%s", topo, htmht);
     TCanvas *c_h_IsrFsr = new TCanvas(cname, cname, 500,500);
     h_RA2Unc_IsrFsr->Draw("colz");
     TPaveText *t_eff_1 = new TPaveText(0.15, 0.67, 0.6, 0.88, "NDC");
     t_eff_1->SetTextAlign(12);
     t_eff_1->SetFillColor(10);
     t_eff_1->SetBorderSize(0);
     t_eff_1->SetTextSize(0.038);
     if(topo=="T1qqqq")
       t_eff_1->AddText("pp #rightarrow #tilde{G}#tilde{G},    #tilde{G}#rightarrow 2Jets+LSP");
     else if(topo=="T2")
       t_eff_1->AddText("pp #rightarrow #tilde{Q}#tilde{Q},    #tilde{Q}#rightarrow Jet+LSP");
     else if(topo=="T2tt")
       t_eff_1->AddText("pp #rightarrow #tilde{t_{1}}#tilde{t_{1}},    #tilde{t_{1}}#rightarrow t+LSP");
     t_eff_1->AddText(" ");
     t_eff_1->AddText("Uncertainty - ISR");
     //sprintf(text, "(HT>%i, MHT>%i GeV)", ht, mht);
     //t_eff_1->AddText(text);
     //t_eff_1->Draw("same");
     
     sprintf(cprint, "%s.eps",cname);
     if(ifPrint) c_h_IsrFsr->Print(cprint);
     sprintf(cprint, "%s.png",cname);
     if(ifPrint) c_h_IsrFsr->Print(cprint);
  }

 //=================================================================================

  // Open a root file & write the histograms in Jim style

  if( !writeUnc ) return;

  char hhname[200], hhtit[200];

  sprintf(hhname, "h_RA2Unc_JerUpDn_%s", htmht);
  h_RA2Unc_JerUpDn->SetName(hhname);
  h_RA2Unc_JerUpDn->SetTitle(hhname);

  sprintf(hhname, "h_RA2Unc_JesUpDn_%s", htmht);
  h_RA2Unc_JesUpDn->SetName(hhname);
  h_RA2Unc_JesUpDn->SetTitle(hhname);
  
  sprintf(hhname, "h_RA2Eff_%s", htmht);
  TH2F *hmzmhEff = (TH2F*) h_RA2Acceptance->Clone(hhname);
  
  sprintf(hhname, "h_RA2EffUnc_%s", htmht);
  sprintf(hhtit,  "h_RA2EffUnc_%s", htmht);
  TH2F *hmzmhEffUnc = new TH2F(hhname, hhtit, 60,0,1500, 60,0,1500);
  
  TH2F *h_RA2Unc_AccStat = new TH2F("h_RA2Unc_AccStat", "h_RA2Unc_AccStat", 60,0,1500, 60,0,1500);
  sprintf(hhname, "h_RA2Unc_StatAccep_%s", htmht);
  h_RA2Unc_AccStat->SetName(hhname);
  h_RA2Unc_AccStat->SetTitle(hhname);

  sprintf(hhname, "h_RA2Unc_IsrFsr_%s", htmht);
  h_RA2Unc_IsrFsr->SetName(hhname);
  h_RA2Unc_IsrFsr->SetTitle(hhname);

  sprintf(hhname, "h_RA2Unc_PDF_%s", htmht);
  h_RA2Unc_PDF->SetName(hhname);
  h_RA2Unc_PDF->SetTitle(hhname);
  
  for(int ix=0; ix<=hmzmhEffUnc->GetNbinsX(); ix++) {
    for(int iy=0; iy<=hmzmhEffUnc->GetNbinsY(); iy++) {
      int ibin    = hmzmhEffUnc->GetBin(ix,iy);
      double jesE = h_RA2Unc_JesUpDn->GetBinContent(ibin);
      double jerE = h_RA2Unc_JerUpDn->GetBinContent(ibin);
      double isrE = h_RA2Unc_IsrFsr ->GetBinContent(ibin);
      double pdfE = 0.0;
      if(doPDF) pdfE = h_RA2Unc_PDF    ->GetBinContent(ibin);
      
      double statE = 0.0;
      if( h_RA2Acceptance->GetBinContent(ibin) ) statE = h_RA2Acceptance->GetBinError(ibin)/h_RA2Acceptance->GetBinContent(ibin);
      else statE = 0.0;
      h_RA2Unc_AccStat->SetBinContent(ibin, statE);
      
      //double totE = std::sqrt(statE*statE + jesE*jesE + jerE*jerE + isrE*isrE + pdfE*pdfE);
      double totE = std::sqrt(statE*statE + jesE*jesE + jerE*jerE + pdfE*pdfE);
      hmzmhEffUnc->SetBinContent(ibin, totE);      

      //if( hmzmhEff->GetBinContent(ibin)>0) {
      //	std::cout << hmzmhEff->GetXaxis()->GetBinCenter(ix)<<" "<<hmzmhEff->GetYaxis()->GetBinCenter(iy)
      //		  << " jesE "<< jesE << " jerE " << jerE << " isrE " << isrE << " pdfE " << pdfE
      //		  << " statE " << statE << " totalE " << totE
      //		  << std::endl;
      //}
      //if( hmzmhEff->GetBinContent(ibin)<=0) hmzmhEffUnc->SetBinContent(ibin, -1.0);
    }
  }
  
  char fout[200];
  //sprintf(fout, "EffUnc_HT%i_MHT%i_%s.root", ht, mht, topo);
  sprintf(fout, "Unc_%s/EffUnc_%s_%s.root", topo, htmht, topo);
  TFile *f_fout = new TFile(fout, "RECREATE");  
  f_fout->cd();
  h_RA2Unc_JerUpDn->Write();
  h_RA2Unc_JesUpDn->Write();
  h_RA2Unc_AccStat->Write();
  if(doPDF) h_RA2Unc_PDF    ->Write();
  h_RA2Unc_IsrFsr ->Write();
  hmzmhEff->Write();
  hmzmhEffUnc->Write();
  f_fout->Close();   
  

}
