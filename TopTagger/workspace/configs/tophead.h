#ifndef tophead_h
#define tophead_h

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//#include <fstream.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Riostream.h"

#include <TLorentzVector.h>
#include <TVector3.h>

#include "TSystem.h"
#include <TStyle.h>
#include <TCanvas.h>
#include "TPostScript.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TClass.h"
#include "TClassTable.h"
#include "TCollection.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TObject.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TMath.h"
#include <string>

#include "Math/Interpolator.h"

#include "Math/ProbFuncMathCore.h"
#include "Math/PdfFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"

#include "TProfile.h"
#include "TProfile2D.h"

#include "TRandom.h"
#include "TRandom3.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TMatrixDSymEigen.h"
#include "TMatrixDBase.h"
#include "TTree.h"
#include "TKey.h"

#include "TList.h"
#include "THashList.h"

#include "TObjString.h"
#include "TTreeFormula.h"

#include "TMinuit.h"
#include "TVirtualFitter.h"

#include "TText.h"
#include "TLatex.h"

#include "TPaveStats.h"

#include "TComplex.h"

#include "TRegexp.h"
#include "TPRegexp.h"

#include <algorithm>
#include <functional>
#include <numeric>

#include <vector>
#include <list>
#include <map>
#include <iterator>

using namespace std;
typedef int Int_t;
typedef double Double_t;
typedef char Char_t;
typedef float Float_t;

#include <iomanip>
using std::setw;
using std::setiosflags;
using std::setprecision;

using namespace ROOT::Math;

const UInt_t glbfSeed = 782365;

const string BCwhite__f = "\033[1;37m";
const string BCred__f   = "\033[31m";
const string BCblue__f  = "\033[34m";
const string BCblue__b  = "\033[44m";
const string BCred__b   = "\033[1;41m";
const string EC__       = "\033[0m";
const string BClblue__b = "\033[1;44m";
const string BC_yellow  = "\033[1;33m";
const string BC_lgreen  = "\033[1;32m";
const string BC_green   = "\033[32m";


inline double glbdrawPull(TPad *pC, int numC, const vector<TH1D*> &hists, TString title){

   int nHist = (int)hists.size();
   if( nHist <=1 ){
      cout<<"ERROR   glbdrawPull  nHist : "<<nHist<<endl;
      return 0;
   }

   TObjArray * vlist =0;
   vlist = title.Tokenize(";");
   TString captionT, axisXT, axisYT;
   for(int i=0; i<vlist->GetEntries(); i++){
      TObjString* obj = dynamic_cast<TObjString*>(vlist->At(i));
      TString v=obj->GetString();
      if( i==0 ) captionT = v;
      if( i==1 ) axisXT = v;
      if( i==2 ) axisYT = v;
   }
   TString axisXfull("; ");
   axisXfull.Append(axisXT);

   char tmpName[200];
   TH1F *dataHist = (TH1F*) hists[0]->Clone();
   sprintf(tmpName, "%s_glbdrawPull", dataHist->GetName()); dataHist->SetName(tmpName);
   TH1F *fitHist = (TH1F*) hists[1]->Clone();
   sprintf(tmpName, "%s_glbdrawPull", fitHist->GetName()); fitHist->SetName(tmpName);

   double heightScale = 1.2;
   double divRatio = 0.25;
   double labelRatio = (1-divRatio)/divRatio;
   double small = 0;

   TAxis *xd = dataHist->GetXaxis();
   TAxis *xf = fitHist->GetXaxis();

   Int_t nd = xd->GetNbins();
   Int_t nf = xf->GetNbins();
   if( nd != nf ){
      cout<<"ERROR   glbdrawPull  dataHist incompatible with fitHist!"<<endl;
      return 0;
   }

   double dDivBins[nd+1];// fDivBins[nf+1];
   int isCompatible =1;
   for(int id =1; id<nd+1; id++){

      double dLowEdge = xd->GetBinLowEdge(id);
      double fLowEdge = xf->GetBinLowEdge(id);
      if( dLowEdge != fLowEdge ){ isCompatible =0; break; }

      dDivBins[id-1] = dLowEdge;// fDivBins[id-1] = fLowEdge;

      if( id == nd ){
         double dUpEdge = xd->GetBinUpEdge(id);
         double fUpEdge = xf->GetBinUpEdge(id);
         if( dUpEdge != fUpEdge ){ isCompatible =0; break; }

         dDivBins[nd] = dUpEdge;// fDivBins[nd] = fUpEdge;
      }

   }

   if( !isCompatible ){
      cout<<"ERROR   glbdrawPull  dataHist incompatible with fitHist!"<<endl;
      return 0;
   }

   TH1F *pullHist = new TH1F("pull", """", nd, dDivBins);
   pullHist->SetTitle("pull");


   TH1F *oneSigmaPlusHist = new TH1F("oneSigmaPlus", """", nd, dDivBins);
   TH1F *oneSigmaMinusHist = new TH1F("oneSigmaMinus", """", nd, dDivBins);

   double totchisq =0;

   for(int ibin=0; ibin<nd; ibin++){

      Double_t nPdf = fitHist->GetBinContent(ibin+1);
      Double_t nData = dataHist->GetBinContent(ibin+1);
      Double_t chi2 = 2*(nPdf - nData);
      if (nData>0 && nPdf>0) chi2 +=  2*nData*log(nData/nPdf);
      Double_t chi = (nData >= nPdf ? sqrt(chi2) : -sqrt(chi2));

      pullHist->SetBinContent(ibin+1, chi);
      pullHist->SetBinError(ibin+1, 1.0);

      oneSigmaPlusHist->SetBinContent(ibin+1, 2.0);
      oneSigmaMinusHist->SetBinContent(ibin+1, -2.0);

      totchisq += chi2;
   }

   pC->cd(numC);

   TPad *pad1 = (TPad*) pC->GetPad(numC); pad1->Clear();
   pad1->Divide(1, 2);

   pad1->cd(1);
   TPad *pad11 = (TPad*) pad1->GetPad(1);
   pad11->SetPad("", "", 0, divRatio, 1.0, 1.0, kWhite);
   pad11->SetBottomMargin(small);

   if( !captionT.IsWhitespace() ) dataHist->SetTitle(captionT.Data());
   dataHist->SetAxisRange(0, dataHist->GetMaximum()*heightScale, "Y");
   dataHist->SetMarkerStyle(kFullDotLarge);
   dataHist->SetStats(kFALSE);
   dataHist->DrawCopy();
   fitHist->SetLineColor(kRed);
   fitHist->DrawCopy("same");
   for(int ie =2; ie<nHist; ie++){
      hists[ie]->DrawCopy("same");
   }

   pad1->cd(2);
   TPad *pad12 = (TPad*) pad1->GetPad(2);
   pad12->SetPad("pull", "", 0, 0, 1.0, divRatio, kWhite);
   pad12->SetBottomMargin(0.40);
   pad12->SetTopMargin(small);

   TAxis* x = pullHist->GetXaxis();

   x->SetTickLength(x->GetTickLength()*labelRatio);
   x->SetLabelSize(x->GetLabelSize()*labelRatio);
   x->SetLabelOffset(x->GetLabelOffset()*labelRatio);
   pullHist->SetMinimum(-8);
   pullHist->SetMaximum(+8);

   TAxis* y = pullHist->GetYaxis();

   y->SetNdivisions(505);
   y->SetLabelSize(y->GetLabelSize()*labelRatio);

   pullHist->SetTitleSize(0.15);
   if( !axisXfull.IsWhitespace() ) pullHist->SetTitle(axisXfull.Data());
   pullHist->SetStats(kFALSE);
   pullHist->SetMarkerStyle(kFullDotLarge);
   pullHist->DrawCopy();

   oneSigmaPlusHist->SetLineColor(kRed);
   oneSigmaPlusHist->SetLineStyle(2);
   oneSigmaPlusHist->DrawCopy("same");
   oneSigmaMinusHist->SetLineColor(kRed);
   oneSigmaMinusHist->SetLineStyle(2);
   oneSigmaMinusHist->DrawCopy("same");

   if( pullHist ) delete pullHist;
   if( oneSigmaPlusHist ) delete oneSigmaPlusHist;
   if( oneSigmaMinusHist ) delete oneSigmaMinusHist;

   if( totchisq ==0 ){
      cout<<"\nWARNING   glbdrawPull  totchisq is ZERO!"<<endl<<endl; 
      totchisq = -1;
   }      
   return totchisq;

}


inline int glbdrawRatio(TPad *pC, int numC, const vector<TH1D*> &hists, TString title){

   int nHist = (int)hists.size();
   if( nHist <=1 ){
      cout<<"ERROR   glbdrawRatio  nHist : "<<nHist<<endl;
      return 0;
   }

   TObjArray * vlist =0;
   vlist = title.Tokenize(";");
   TString captionT, axisXT, axisYT;
   for(int i=0; i<vlist->GetEntries(); i++){
      TObjString* obj = dynamic_cast<TObjString*>(vlist->At(i));
      TString v=obj->GetString();
      if( i==0 ) captionT = v;
      if( i==1 ) axisXT = v;
      if( i==2 ) axisYT = v;
   }
   TString axisXfull("; ");
   axisXfull.Append(axisXT);

   char tmpName[200];
   TH1F *dataHist = (TH1F*) hists[0]->Clone();
   sprintf(tmpName, "%s_glbdrawRatio", dataHist->GetName()); dataHist->SetName(tmpName);
   TH1F *fitHist = (TH1F*) hists[1]->Clone();
   sprintf(tmpName, "%s_glbdrawRatio", fitHist->GetName()); fitHist->SetName(tmpName);

   double heightScale = 1.2;
   double divRatio = 0.25;
   double labelRatio = (1-divRatio)/divRatio;
   double small = 0;

   TAxis *xd = dataHist->GetXaxis();
   TAxis *xf = fitHist->GetXaxis();

   Int_t nd = xd->GetNbins();
   Int_t nf = xf->GetNbins();
   if( nd != nf ){
      cout<<"ERROR   glbdrawRatio  dataHist incompatible with fitHist!"<<endl;
      return 0;
   }

   double dDivBins[nd+1];// fDivBins[nf+1];
   int isCompatible =1;
   for(int id =1; id<nd+1; id++){

      double dLowEdge = xd->GetBinLowEdge(id);
      double fLowEdge = xf->GetBinLowEdge(id);
      if( dLowEdge != fLowEdge ){ isCompatible =0; break; }

      dDivBins[id-1] = dLowEdge;// fDivBins[id-1] = fLowEdge;

      if( id == nd ){
         double dUpEdge = xd->GetBinUpEdge(id);
         double fUpEdge = xf->GetBinUpEdge(id);
         if( dUpEdge != fUpEdge ){ isCompatible =0; break; }

         dDivBins[nd] = dUpEdge;// fDivBins[nd] = fUpEdge;
      }

   }

   if( !isCompatible ){
      cout<<"ERROR   glbdrawRatio  dataHist incompatible with fitHist!"<<endl;
      return 0;
   }

   TH1F *ratioHist = (TH1F*) dataHist->Clone();
   ratioHist->SetTitle("ratio");
   ratioHist->Divide(fitHist);

   TH1F *oneSigmaPlusHist = new TH1F("oneSigmaPlus", """", nd, dDivBins);
   TH1F *oneSigmaMinusHist = new TH1F("oneSigmaMinus", """", nd, dDivBins);

   for(int ibin=0; ibin<nd; ibin++){
      oneSigmaPlusHist->SetBinContent(ibin+1, 1.1);
      oneSigmaMinusHist->SetBinContent(ibin+1, 0.9);
   }

   pC->cd(numC);

   TPad *pad1 = (TPad*) pC->GetPad(numC); pad1->Clear();
   pad1->Divide(1, 2);

   pad1->cd(1);
   TPad *pad11 = (TPad*) pad1->GetPad(1);
   pad11->SetPad("", "", 0, divRatio, 1.0, 1.0, kWhite);
   pad11->SetBottomMargin(small);

   if( !captionT.IsWhitespace() ) dataHist->SetTitle(captionT.Data());
   dataHist->SetAxisRange(0, dataHist->GetMaximum()*heightScale, "Y");
   dataHist->SetStats(kFALSE);
   dataHist->SetLineColor(kRed);
   dataHist->DrawCopy();
   fitHist->DrawCopy("same");
   for(int ie =2; ie<nHist; ie++){
      hists[ie]->DrawCopy("same");
   }

   pad1->cd(2);
   TPad *pad12 = (TPad*) pad1->GetPad(2);
   pad12->SetPad("pull", "", 0, 0, 1.0, divRatio, kWhite);
   pad12->SetBottomMargin(0.20);
   pad12->SetTopMargin(small);

   TAxis* x = ratioHist->GetXaxis();

   x->SetTickLength(x->GetTickLength()*labelRatio);
   x->SetLabelSize(x->GetLabelSize()*labelRatio);
   x->SetLabelOffset(x->GetLabelOffset()*labelRatio);
   ratioHist->SetMinimum(0.60);
   ratioHist->SetMaximum(1.40);

   TAxis* y = ratioHist->GetYaxis();

   y->SetNdivisions(010);
   y->SetLabelSize(y->GetLabelSize()*labelRatio);

   ratioHist->SetTitleSize(0.10);
   if( !axisXfull.IsWhitespace() ) ratioHist->SetTitle(axisXfull.Data());
   ratioHist->SetStats(kFALSE);
   ratioHist->DrawCopy();

   oneSigmaPlusHist->SetLineColor(kRed);
   oneSigmaPlusHist->SetLineStyle(2);
   oneSigmaPlusHist->DrawCopy("same");
   oneSigmaMinusHist->SetLineColor(kRed);
   oneSigmaMinusHist->SetLineStyle(2);
   oneSigmaMinusHist->DrawCopy("same");

   if( ratioHist ) delete ratioHist;
   if( oneSigmaPlusHist ) delete oneSigmaPlusHist;
   if( oneSigmaMinusHist ) delete oneSigmaMinusHist;

   return 1;

}

#endif
