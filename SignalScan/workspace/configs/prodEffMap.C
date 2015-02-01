#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "TGraphErrors.h"
#include "TPaletteAxis.h"
#include "TColor.h"

//const int nXbins = 45, nYbins = 47;
//const double loX = 100, hiX = 1225;
//const double loY = 50, hiY = 1225;
// Same as the configuration in the initial scan
const int nXbins = 60, nYbins = 60;
const double loX = -12.5, hiX = 1487.5;
const double loY = -12.5, hiY = 1487.5;
const double widthXbin = 25, widthYbin = 25, widthBin = 25;

static const int nsels = 14;
//                                             sel0                 sel1                sel2                  sel3                   sel4                   sel5                  sel6                   sel7                     sel8                  sel9                  sel10                    sel11                   sel12                 sel13
static const string exclRegions[] = {"500HT800_200MHT350", "500HT800_350MHT500", "500HT800_500MHT600", "500HT800_600MHTinf", "800HT1000_200MHT350", "800HT1000_350MHT500", "800HT1000_500MHT600", "800HT1000_600MHTinf", "1000HT1200_200MHT350", "1000HT1200_350MHT500", "1000HT1200_500MHTinf", "1200HT1400_200MHT350", "1200HT1400_350MHTinf", "1400HTinf_200MHTinf"};

static const string disptRegionsHT[] = {"500<H_{T}<800 GeV", "500<H_{T}<800 GeV", "500<H_{T}<800 GeV", "500<H_{T}<800 GeV", "800<H_{T}<1000 GeV", "800<H_{T}<1000 GeV", "800<H_{T}<1000 GeV", "800<H_{T}<1000 GeV", "1000<H_{T}<1200 GeV", "1000<H_{T}<1200 GeV", "1000<H_{T}<1200 GeV", "1200<H_{T}<1400 GeV", "1200<H_{T}<1400 GeV", "1400<H_{T} GeV"};
static const string disptRegionsMHT[] = {"200<#slash{H}_{T}<350 GeV", "350<#slash{H}_{T}<500 GeV", "500<#slash{H}_{T}<600 GeV", "600<#slash{H}_{T} GeV", "200<#slash{H}_{T}<350 GeV", "350<#slash{H}_{T}<500 GeV", "500<#slash{H}_{T}<600 GeV", "600<#slash{H}_{T} GeV", "200<#slash{H}_{T}<350 GeV", "350<#slash{H}_{T}<500 GeV", "500<#slash{H}_{T} GeV", "200<#slash{H}_{T}<350 GeV", "350<#slash{H}_{T} GeV", "200<#slash{H}_{T} GeV"};

static const string histTitlesHT[] = {"500<HT<800 GeV", "500<HT<800 GeV", "500<HT<800 GeV", "500<HT<800 GeV", "800<HT<1000 GeV", "800<HT<1000 GeV", "800<HT<1000 GeV", "800<HT<1000 GeV", "1000<HT<1200 GeV", "1000<HT<1200 GeV", "1000<HT<1200 GeV", "1200<HT<1400 GeV", "1200<HT<1400 GeV", "1400<HT GeV"};
static const string histTitlesMHT[] = {"200<MHT<350 GeV", "350<MHT<500 GeV", "500<MHT<600 GeV", "600<MHT GeV", "200<MHT<350 GeV", "350<MHT<500 GeV", "500<MHT<600 GeV", "600<MHT GeV", "200<MHT<350 GeV", "350<MHT<500 GeV", "500<MHT GeV", "200<MHT<350 GeV", "350<MHT GeV", "200<MHT GeV"};

void fillEmptyPoints(const TH2D *inHist, TH2D *outHist, int xBinLargery, int pickedMethod =1);

bool wrapFinder( const std::vector<int> &indexVec, int index){

   if( find( indexVec.begin(), indexVec.end(), index) != indexVec.end() ) return true;
   
   return false;
}

void prodEffMap(){

   gROOT->ProcessLine(".L tdrstyle.C");
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

/*
   tdrStyle->SetHistLineWidth(2);
   tdrStyle->SetFrameLineWidth(2);
*/

      //  For the axis
   tdrStyle->SetAxisColor(1,"XYZ");
   tdrStyle->SetTickLength(0.03,"XYZ");
   tdrStyle->SetNdivisions(505,"XYZ");
   tdrStyle->SetPadTickX(1);
   tdrStyle->SetPadTickY(1);
   tdrStyle->SetStripDecimals(kFALSE);

//   tdrStyle->SetPalette(1);
   int nColors = 999;
   int MyPalette[nColors];
/*
   Double_t r[]    = {1.0, 0.0, 0.0, 0.0, 1.0, 1.0};
   Double_t g[]    = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0};
   Double_t b[]    = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0};
   Double_t stop[] = {0.000, 0.166, 0.332, 0.500, 0.666, 0.832, 1.000};
   int FI = TColor::CreateGradientColorTable(6, stop, r, g, b, nColors);
*/
   Double_t r[]    = {0.0, 0.0, 0.0, 1.0, 1.0};
   Double_t g[]    = {0.0, 1.0, 1.0, 1.0, 0.0};
   Double_t b[]    = {1.0, 1.0, 0.0, 0.0, 0.0};
   Double_t stop[] = {0.000, 0.200, 0.400, 0.600, 0.800, 1.000};
   int FI = TColor::CreateGradientColorTable(5, stop, r, g, b, nColors);

   for(int i=0; i<nColors; i++) MyPalette[i] = FI+i;

   tdrStyle->SetNumberContours(nColors);

//   tdrStyle->SetTitleSize(0.065,"XYZ");
//   tdrStyle->SetTitleXOffset(1.15); tdrStyle->SetTitleYOffset(1.75); tdrStyle->SetTitleOffset(1.40, "Z");
//   tdrStyle->SetPadLeftMargin(0.150); tdrStyle->SetPadRightMargin(0.130);
//   tdrStyle->SetPadTopMargin(0.08); tdrStyle->SetPadBottomMargin(0.115);

   tdrStyle->SetLabelSize(0.050, "XYZ");

//   tdrStyle->SetTitleFontSize(0.9);

   char name[200];

   int xBinLargery = 2; const int moreISRremoval = 5;
   xBinLargery += moreISRremoval;

   std::vector<TH2D*> effMapT1Vec(nsels);
   TFile *effT1file = new TFile("EffUnc_SMS_T1.root");
   for(int is=0; is<nsels; is++){
      TString keyStr = exclRegions[is].c_str();
      sprintf(name, "h_RA2Eff_%s", keyStr.Data());
      TH2D * effT1oriTMP = (TH2D*) effT1file->Get(name);

      sprintf(name, "h_RA2Eff_%s_SHIFT", keyStr.Data());
      TH2D * effT1ori = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);
      sprintf(name, "h_RA2Eff_%s_cloned", keyStr.Data());
      TH2D * effT1cloned = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);

      for(int ix=0; ix<=effT1oriTMP->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT1oriTMP->GetYaxis()->GetNbins(); iy++){
            effT1ori->SetBinContent(ix, iy, effT1oriTMP->GetBinContent(ix, iy));
            if( ix-iy < xBinLargery ) continue;
            effT1cloned->SetBinContent(ix, iy, effT1oriTMP->GetBinContent(ix, iy));
         }
      }

      TH2D * effT1cloned2 = (TH2D*) effT1cloned->Clone();
      fillEmptyPoints(effT1ori, effT1cloned2, xBinLargery);
      if( is == 3 || is == 7 || is == 11 ) fillEmptyPoints(effT1ori, effT1cloned2, xBinLargery, 2);
      for(int ix=0; ix<=effT1cloned2->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT1cloned2->GetYaxis()->GetNbins(); iy++){
            if( ix-iy < xBinLargery ){
               effT1cloned2->SetBinContent(ix, iy, 0);
            }else{
               continue;
            }
         }
      }
      effMapT1Vec[is] = (TH2D*) effT1cloned2->Clone();

      effMapT1Vec[is]->GetXaxis()->SetLabelSize( effT1oriTMP->GetXaxis()->GetLabelSize() );
      effMapT1Vec[is]->GetXaxis()->SetLabelOffset( effT1oriTMP->GetXaxis()->GetLabelOffset() );
      effMapT1Vec[is]->GetXaxis()->SetTitleSize( effT1oriTMP->GetXaxis()->GetTitleSize() );
      effMapT1Vec[is]->GetXaxis()->SetTitleOffset( effT1oriTMP->GetXaxis()->GetTitleOffset() );

      effMapT1Vec[is]->GetYaxis()->SetLabelSize( effT1oriTMP->GetYaxis()->GetLabelSize() );
      effMapT1Vec[is]->GetYaxis()->SetLabelOffset( effT1oriTMP->GetYaxis()->GetLabelOffset() );
      effMapT1Vec[is]->GetYaxis()->SetTitleSize( effT1oriTMP->GetYaxis()->GetTitleSize() );
      effMapT1Vec[is]->GetYaxis()->SetTitleOffset( effT1oriTMP->GetYaxis()->GetTitleOffset() );

      effMapT1Vec[is]->GetZaxis()->SetLabelSize( effT1oriTMP->GetZaxis()->GetLabelSize() );
      effMapT1Vec[is]->GetZaxis()->SetLabelOffset( effT1oriTMP->GetZaxis()->GetLabelOffset() );
      effMapT1Vec[is]->GetZaxis()->SetTitleSize( effT1oriTMP->GetZaxis()->GetTitleSize() );
      effMapT1Vec[is]->GetZaxis()->SetTitleOffset( effT1oriTMP->GetZaxis()->GetTitleOffset() );
   }

   std::vector<TH2D*> effMapT2Vec(nsels);
   TFile *effT2file = new TFile("EffUnc_SMS_T2.root");
   for(int is=0; is<nsels; is++){
      TString keyStr = exclRegions[is].c_str();
      keyStr.ReplaceAll("inf", "Inf");
      sprintf(name, "h_RA2Eff_%s", keyStr.Data());
      TH2D * effT2oriTMP = (TH2D*) effT2file->Get(name);

      sprintf(name, "h_RA2Eff_%s_SHIFT", keyStr.Data());
      TH2D * effT2ori = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);
      sprintf(name, "h_RA2Eff_%s_cloned", keyStr.Data());
      TH2D * effT2cloned = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);

      for(int ix=0; ix<=effT2oriTMP->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT2oriTMP->GetYaxis()->GetNbins(); iy++){
            effT2ori->SetBinContent(ix, iy, effT2oriTMP->GetBinContent(ix, iy));
            if( ix-iy < xBinLargery ) continue;
            effT2cloned->SetBinContent(ix, iy, effT2oriTMP->GetBinContent(ix, iy));
         }
      }
      TH2D * effT2cloned2 = (TH2D*) effT2cloned->Clone();
      fillEmptyPoints(effT2ori, effT2cloned2, xBinLargery); 
      for(int ix=0; ix<=effT2cloned2->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT2cloned2->GetYaxis()->GetNbins(); iy++){
            if( ix-iy < xBinLargery ){
               effT2cloned2->SetBinContent(ix, iy, 0);
            }else{
               continue;
            }
         }
      }
      effMapT2Vec[is] = (TH2D*) effT2cloned2->Clone();

      effMapT2Vec[is]->GetXaxis()->SetLabelSize( effT2oriTMP->GetXaxis()->GetLabelSize() );
      effMapT2Vec[is]->GetXaxis()->SetLabelOffset( effT2oriTMP->GetXaxis()->GetLabelOffset() );
      effMapT2Vec[is]->GetXaxis()->SetTitleSize( effT2oriTMP->GetXaxis()->GetTitleSize() );
      effMapT2Vec[is]->GetXaxis()->SetTitleOffset( effT2oriTMP->GetXaxis()->GetTitleOffset() );

      effMapT2Vec[is]->GetYaxis()->SetLabelSize( effT2oriTMP->GetYaxis()->GetLabelSize() );
      effMapT2Vec[is]->GetYaxis()->SetLabelOffset( effT2oriTMP->GetYaxis()->GetLabelOffset() );
      effMapT2Vec[is]->GetYaxis()->SetTitleSize( effT2oriTMP->GetYaxis()->GetTitleSize() );
      effMapT2Vec[is]->GetYaxis()->SetTitleOffset( effT2oriTMP->GetYaxis()->GetTitleOffset() );

      effMapT2Vec[is]->GetZaxis()->SetLabelSize( effT2oriTMP->GetZaxis()->GetLabelSize() );
      effMapT2Vec[is]->GetZaxis()->SetLabelOffset( effT2oriTMP->GetZaxis()->GetLabelOffset() );
      effMapT2Vec[is]->GetZaxis()->SetTitleSize( effT2oriTMP->GetZaxis()->GetTitleSize() );
      effMapT2Vec[is]->GetZaxis()->SetTitleOffset( effT2oriTMP->GetZaxis()->GetTitleOffset() );
   }

   std::vector<TH2D*> effMapT5ZZIncVec(nsels);
   TFile *effT5ZZIncfile = new TFile("EffUnc_SMS_T5ZZInc.root");
   for(int is=0; is<nsels; is++){
      TString keyStr = exclRegions[is].c_str();
      keyStr.ReplaceAll("inf", "Inf");
      sprintf(name, "h_RA2Eff_%s", keyStr.Data());
      TH2D * effT5ZZIncoriTMP = (TH2D*) effT5ZZIncfile->Get(name);

      sprintf(name, "h_RA2Eff_%s_SHIFT", keyStr.Data());
      TH2D * effT5ZZIncori = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);
      sprintf(name, "h_RA2Eff_%s_cloned", keyStr.Data());
      TH2D * effT5ZZInccloned = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);

      for(int ix=0; ix<=effT5ZZIncoriTMP->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT5ZZIncoriTMP->GetYaxis()->GetNbins(); iy++){
            effT5ZZIncori->SetBinContent(ix, iy, effT5ZZIncoriTMP->GetBinContent(ix, iy));
            if( ix-iy < xBinLargery+2 ) continue;
            effT5ZZInccloned->SetBinContent(ix, iy, effT5ZZIncoriTMP->GetBinContent(ix, iy));
         }
      }
      TH2D * effT5ZZInccloned2 = (TH2D*) effT5ZZInccloned->Clone();
      fillEmptyPoints(effT5ZZIncori, effT5ZZInccloned2, xBinLargery+2); 
      for(int ix=0; ix<=effT5ZZInccloned2->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT5ZZInccloned2->GetYaxis()->GetNbins(); iy++){
            if( ix-iy < xBinLargery+2 ){
               effT5ZZInccloned2->SetBinContent(ix, iy, 0);
            }else{
               continue;
            }
         }
      }
      effMapT5ZZIncVec[is] = (TH2D*) effT5ZZInccloned2->Clone();

      effMapT5ZZIncVec[is]->GetXaxis()->SetLabelSize( effT5ZZIncoriTMP->GetXaxis()->GetLabelSize() );
      effMapT5ZZIncVec[is]->GetXaxis()->SetLabelOffset( effT5ZZIncoriTMP->GetXaxis()->GetLabelOffset() );
      effMapT5ZZIncVec[is]->GetXaxis()->SetTitleSize( effT5ZZIncoriTMP->GetXaxis()->GetTitleSize() );
      effMapT5ZZIncVec[is]->GetXaxis()->SetTitleOffset( effT5ZZIncoriTMP->GetXaxis()->GetTitleOffset() );

      effMapT5ZZIncVec[is]->GetYaxis()->SetLabelSize( effT5ZZIncoriTMP->GetYaxis()->GetLabelSize() );
      effMapT5ZZIncVec[is]->GetYaxis()->SetLabelOffset( effT5ZZIncoriTMP->GetYaxis()->GetLabelOffset() );
      effMapT5ZZIncVec[is]->GetYaxis()->SetTitleSize( effT5ZZIncoriTMP->GetYaxis()->GetTitleSize() );
      effMapT5ZZIncVec[is]->GetYaxis()->SetTitleOffset( effT5ZZIncoriTMP->GetYaxis()->GetTitleOffset() );

      effMapT5ZZIncVec[is]->GetZaxis()->SetLabelSize( effT5ZZIncoriTMP->GetZaxis()->GetLabelSize() );
      effMapT5ZZIncVec[is]->GetZaxis()->SetLabelOffset( effT5ZZIncoriTMP->GetZaxis()->GetLabelOffset() );
      effMapT5ZZIncVec[is]->GetZaxis()->SetTitleSize( effT5ZZIncoriTMP->GetZaxis()->GetTitleSize() );
      effMapT5ZZIncVec[is]->GetZaxis()->SetTitleOffset( effT5ZZIncoriTMP->GetZaxis()->GetTitleOffset() );
   }

   std::vector<TH2D*> effMapT1bbbbVec(nsels);
   TFile *effT1bbbbfile = new TFile("EffUnc_SMS_T1bbbb.root");
   for(int is=0; is<nsels; is++){
      TString keyStr = exclRegions[is].c_str();
      keyStr.ReplaceAll("inf", "Inf");
      sprintf(name, "h_RA2Eff_%s", keyStr.Data());
      TH2D * effT1bbbboriTMP = (TH2D*) effT1bbbbfile->Get(name);

      sprintf(name, "h_RA2Eff_%s_SHIFT", keyStr.Data());
      TH2D * effT1bbbbori = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);
      sprintf(name, "h_RA2Eff_%s_cloned", keyStr.Data());
      TH2D * effT1bbbbcloned = new TH2D(name, name, nXbins, loX, hiX, nYbins, loY, hiY);

      for(int ix=0; ix<=effT1bbbboriTMP->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT1bbbboriTMP->GetYaxis()->GetNbins(); iy++){
            effT1bbbbori->SetBinContent(ix, iy, effT1bbbboriTMP->GetBinContent(ix, iy));
            if( ix-iy < xBinLargery ) continue;
            effT1bbbbcloned->SetBinContent(ix, iy, effT1bbbboriTMP->GetBinContent(ix, iy));
         }
      }
      TH2D * effT1bbbbcloned2 = (TH2D*) effT1bbbbcloned->Clone();
      fillEmptyPoints(effT1bbbbori, effT1bbbbcloned2, xBinLargery); 
      if( is == 3 || is == 7 || is == 11 ) fillEmptyPoints(effT1bbbbori, effT1bbbbcloned2, xBinLargery, 2);
      for(int ix=0; ix<=effT1bbbbcloned2->GetXaxis()->GetNbins(); ix++){
         for(int iy=0; iy<=effT1bbbbcloned2->GetYaxis()->GetNbins(); iy++){
            if( ix-iy < xBinLargery ){
               effT1bbbbcloned2->SetBinContent(ix, iy, 0);
            }else{
               continue;
            }
         }
      }
      effMapT1bbbbVec[is] = (TH2D*) effT1bbbbcloned2->Clone();

      effMapT1bbbbVec[is]->GetXaxis()->SetLabelSize( effT1bbbboriTMP->GetXaxis()->GetLabelSize() );
      effMapT1bbbbVec[is]->GetXaxis()->SetLabelOffset( effT1bbbboriTMP->GetXaxis()->GetLabelOffset() );
      effMapT1bbbbVec[is]->GetXaxis()->SetTitleSize( effT1bbbboriTMP->GetXaxis()->GetTitleSize() );
      effMapT1bbbbVec[is]->GetXaxis()->SetTitleOffset( effT1bbbboriTMP->GetXaxis()->GetTitleOffset() );

      effMapT1bbbbVec[is]->GetYaxis()->SetLabelSize( effT1bbbboriTMP->GetYaxis()->GetLabelSize() );
      effMapT1bbbbVec[is]->GetYaxis()->SetLabelOffset( effT1bbbboriTMP->GetYaxis()->GetLabelOffset() );
      effMapT1bbbbVec[is]->GetYaxis()->SetTitleSize( effT1bbbboriTMP->GetYaxis()->GetTitleSize() );
      effMapT1bbbbVec[is]->GetYaxis()->SetTitleOffset( effT1bbbboriTMP->GetYaxis()->GetTitleOffset() );

      effMapT1bbbbVec[is]->GetZaxis()->SetLabelSize( effT1bbbboriTMP->GetZaxis()->GetLabelSize() );
      effMapT1bbbbVec[is]->GetZaxis()->SetLabelOffset( effT1bbbboriTMP->GetZaxis()->GetLabelOffset() );
      effMapT1bbbbVec[is]->GetZaxis()->SetTitleSize( effT1bbbboriTMP->GetZaxis()->GetTitleSize() );
      effMapT1bbbbVec[is]->GetZaxis()->SetTitleOffset( effT1bbbboriTMP->GetZaxis()->GetTitleOffset() );
   }

// Make a diagonal graph
   double hiDiagRange = -1, loDiagRange = 0;
   if( hiDiagRange < hiX ) hiDiagRange = hiX;
   if( hiDiagRange < hiY ) hiDiagRange = hiY;
   int nDiagBins = (hiDiagRange - loDiagRange)/widthBin +1;
   double xPtsDiag[nDiagBins], xErrsDiag[nDiagBins], yPtsDiag[nDiagBins], yErrsDiag[nDiagBins];
   for(int ib=0; ib<nDiagBins; ib++){
      xPtsDiag[ib] = loDiagRange + ib*widthBin; xErrsDiag[ib] = 0.0;
      yPtsDiag[ib] = loDiagRange + ib*widthBin; yErrsDiag[ib] = 0.0;
   }
   TGraphErrors * diagonalGraph = new TGraphErrors(nDiagBins, xPtsDiag, yPtsDiag, xErrsDiag, yErrsDiag);
   diagonalGraph->SetLineWidth(2); diagonalGraph->SetLineStyle(7);
   diagonalGraph->SetName("diagonalGraph");

   const double cavWidth = 800, cavHeight = 800;
   TCanvas *cs = new TCanvas("cs", "cs", cavWidth, cavHeight);
   cs->SetLogz();
   cs->cd();

/*
   const double t_ = 0.042;
   const double b_ = 0.042;
   const double l_ = 0.042;
   const double r_ = 0.042;
*/
   const double t_ = 0.057;
   const double b_ = 0.057;
   const double l_ = 0.057;
   const double r_ = 0.057;
/*
   const double y_ = (1.-t_-b_);
   const double x_ = (1.-l_-r_)/2.;
   const double fracHeightTopPad = 0.8;
   const double ratioMin = 0.1;
   const double ratioMax = 2.4;
*/
   const int nX=4, nY=4;
   double xDivPadRatio = (1-l_-r_)/nX, yDivPadRatio = (1-t_-b_)/nY;
 
   tdrStyle->SetPadTopMargin(0); tdrStyle->SetPadBottomMargin(0);
   tdrStyle->SetPadLeftMargin(0); tdrStyle->SetPadRightMargin(0);
/*
   tdrStyle->SetTitleSize(0.550,"XYZ");
   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(5.50); tdrStyle->SetTitleOffset(1.40, "Z");
*/
   tdrStyle->SetTitleSize(0.550,"XYZ");
   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(5.50); tdrStyle->SetTitleOffset(1.40, "Z");

   tdrStyle->SetTitleBorderSize(0);
   tdrStyle->SetTitleAlign(22);
   tdrStyle->SetTitleX(0.50);
   tdrStyle->SetTitleY(0.93);
   tdrStyle->SetTitleH(0.135);

   std::string titleT1Str = "CMS Simulation, #sqrt{s} = 7 TeV, Acceptance #times Efficiency for pp#rightarrow#tilde{g}#tilde{g}, #tilde{g}#rightarrow qq#tilde{#chi}^{0}; m(#tilde{q})>>m(#tilde{g})";
   std::string titleT2Str = "CMS Simulation, #sqrt{s} = 7 TeV, Acceptance #times Efficiency for pp#rightarrow#tilde{q}#tilde{q}, #tilde{q}#rightarrow q#tilde{#chi}^{0}; m(#tilde{g})>>m(#tilde{q})";
   std::string titleT5ZZIncStr = "CMS Simulation, #sqrt{s} = 7 TeV, Acceptance #times Efficiency for pp#rightarrow#tilde{g}#tilde{g}, #tilde{g}#rightarrow qq#tilde{#chi}^{0}_{2}, #tilde{#chi}^{0}_{2}#rightarrow Z#tilde{#chi}^{0}";
   std::string titleT1bbbbStr = "CMS Simulation, #sqrt{s} = 7 TeV, Acceptance #times Efficiency for pp#rightarrow#tilde{g}#tilde{g}, #tilde{g}#rightarrow bb#tilde{#chi}^{0}; m(#tilde{b})>>m(#tilde{g})";

   std::string axisTitleT1Str = "m(#tilde{g}) [GeV]; m(#tilde{#chi}^{0}) [GeV]";
   std::string axisTitleT2Str = "m(#tilde{q}) [GeV]; m(#tilde{#chi}^{0}) [GeV]";
   std::string axisTitleT5ZZIncStr = "m(#tilde{g}) [GeV]; m(#tilde{#chi}^{0}) [GeV]";
   std::string axisTitleT1bbbbStr = "m(#tilde{g}) [GeV]; m(#tilde{#chi}^{0}) [GeV]";
/*
   std::string axisTitleT1Str = "m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}} [GeV]";
   std::string axisTitleT2Str = "m_{#tilde{q}} [GeV]; m_{#tilde{#chi}^{0}} [GeV]";
   std::string axisTitleT5ZZIncStr = "m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}} [GeV]";
*/
//   std::string cmsLabelStr = "CMS Preliminary";
//   std::string cmsLabelStr = "CMS";
   std::string cmsLabelStr = "";

   double axisTitleScale = 1.86;
   double axisLabelScale = 1.6;

   double disptFontSizeFix = 0.075;

   double disptPavexLowFix = 0.04, disptPavexHighFix = 0.40;
   double disptPaveyLowFix = 0.85, disptPaveyHighFix = 0.95;
 
   double disptLabelCMSxLowFix = disptPavexLowFix, disptLabelCMSxHighFix = disptPavexHighFix;
   double disptLabelCMSyLowFix = disptPaveyLowFix-0.15, disptLabelCMSyHighFix = disptPaveyLowFix-0.10;

   const double titleXoffsetFromCenter = 0.3, titleYoffsetFromCenter = 0.3;

   const int topPadIdxArr[]        ={0, 1, 2, 3};
   const int leftPadIdxArr[]       ={0, 4, 8, 12};
   const int rightPadIdxArr[]      ={3, 7, 10, 14};
   const int bottomPadIdxArr[]     ={7, 12, 13, 14};
   const int fourCornerPadIdxArr[] ={0, 3, 7, 12, 14};
   const int middlePadIdxArr[]     ={5, 6, 9};

// index is hist index
   const int mapHistToPadArr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14 };

   std::vector<int> topVec( topPadIdxArr, topPadIdxArr+sizeof(topPadIdxArr)/sizeof(int) );
   std::vector<int> leftVec( leftPadIdxArr, leftPadIdxArr+sizeof(leftPadIdxArr)/sizeof(int) );
   std::vector<int> rightVec( rightPadIdxArr, rightPadIdxArr+sizeof(rightPadIdxArr)/sizeof(int) );
   std::vector<int> bottomVec( bottomPadIdxArr, bottomPadIdxArr+sizeof(bottomPadIdxArr)/sizeof(int) );
   std::vector<int> fourCornerVec( fourCornerPadIdxArr, fourCornerPadIdxArr+sizeof(fourCornerPadIdxArr)/sizeof(int) );
   std::vector<int> middleVec( middlePadIdxArr, middlePadIdxArr+sizeof(middlePadIdxArr)/sizeof(int) );

   const double minZaxisT1 = 0.70e-4, maxZaxisT1 = 3.0e-1;
   const double minZaxisT2 = 0.70e-4, maxZaxisT2 = 1.8e-1;
   const double minZaxisT5ZZInc = 0.30e-4, maxZaxisT5ZZInc = 3.0e-1;
   const double minZaxisT1bbbb = 0.70e-4, maxZaxisT1bbbb = 3.0e-1;
/*
   const double minZaxisT1 = 0.60e-5, maxZaxisT1 = 3.0e-1;
   const double minZaxisT2 = 0.40e-4, maxZaxisT2 = 1.8e-1;
*/

   for(int ih=0; ih<nsels; ih++){
      int is = mapHistToPadArr[ih];

      char padNames[20];
      sprintf(padNames, "pad%2d", is+1);
      cs->cd();

      TPad *pad =0;

      int condCnt = 0; std::vector<int> condIdxVec(9); std::vector<std::string> condStrVec(9);

// Bottom row
    // bottom-left corner
      condStrVec[0] = "bl"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[0] = 1; 
      }
    // bottom middle
      condStrVec[1] = "bm";  
      if( wrapFinder(bottomVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[1] = 1;
      }
    // bottom-right corner
      condStrVec[2] = "br"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[2] = 1;
      }

// Middle rows
   // middle-left
      condStrVec[3] = "ml"; 
      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[3] = 1;
      }
   // middle-middle
      condStrVec[4] = "mm"; 
      if( wrapFinder(middleVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[4] = 1;
      }
   // middle-right
      condStrVec[5] = "mr"; 
      if( wrapFinder(rightVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[5] = 1;
      }

// Top row
   // top-left
      condStrVec[6] = "tl"; 
      if( wrapFinder(topVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_);   
         condCnt++; condIdxVec[6] = 1;
      }
   // top-middle
      condStrVec[7] = "tm"; 
      if( wrapFinder(topVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[7] = 1;
      }
   // top-right
      condStrVec[8] = "tr"; 
      if( wrapFinder(topVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[8] = 1;
      }
 
      if( ih == 0 ){
         printf("%35s", " ");
         for(int id=0; id<9; id++){
            printf("  %2s", condStrVec[id].c_str());
         }
         std::cout<<std::endl;
      }
      printf("ih : %2d  is : %2d  condCnt : %2d  ==>", ih, is, condCnt);
      for(int id=0; id<9; id++){
         printf("  %2d", condIdxVec[id]);
      }
      std::cout<<std::endl;

      pad->Draw(); pad->cd(); pad->SetLogz();

      if( wrapFinder(leftVec, is) ){ pad->SetLeftMargin(l_*nX); pad->SetRightMargin(0); }
      if( wrapFinder(rightVec, is) ){ pad->SetRightMargin(r_*nX-0.0230);  pad->SetLeftMargin(0); }
      if( wrapFinder(topVec, is) ){ pad->SetTopMargin(t_*nY);    pad->SetBottomMargin(0); }
      if( wrapFinder(bottomVec, is) ){ pad->SetBottomMargin(b_*nY); pad->SetTopMargin(0); }
      if( wrapFinder(middleVec, is) ){ pad->SetTopMargin(0); pad->SetBottomMargin(0); pad->SetLeftMargin(0); pad->SetRightMargin(0); }
      if( is == 7 ){ pad->SetBottomMargin(b_*nY-0.0230); pad->SetTopMargin(0); }

      sprintf(name, "%s; %s", "", axisTitleT1Str.c_str());
      if( is == 10 ) sprintf(name, "%s; ; ", "");

      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT1Vec[ih]->GetYaxis()->SetLabelSize( effMapT1Vec[ih]->GetYaxis()->GetLabelSize()*ratioY);
         effMapT1Vec[ih]->GetYaxis()->SetTitleSize( effMapT1Vec[ih]->GetYaxis()->GetTitleSize()*ratioY);
      }
      if( wrapFinder(leftVec, is) && wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT1Vec[ih]->GetYaxis()->SetTitleOffset( effMapT1Vec[ih]->GetYaxis()->GetTitleOffset()*ratioY);
      }
      if( wrapFinder(bottomVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetXaxis()->SetTitleOffset( effMapT1Vec[ih]->GetXaxis()->GetTitleOffset()*ratioX);
         }
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetXaxis()->SetTitleSize( effMapT1Vec[ih]->GetXaxis()->GetTitleSize()*ratioX);
            effMapT1Vec[ih]->GetXaxis()->SetLabelSize( effMapT1Vec[ih]->GetXaxis()->GetLabelSize()*ratioX);
         }
      }
      if( wrapFinder(rightVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetZaxis()->SetLabelSize( effMapT1Vec[ih]->GetZaxis()->GetLabelSize()*ratioX);
         }
      }

      effMapT1Vec[ih]->GetXaxis()->SetTitleSize( effMapT1Vec[ih]->GetXaxis()->GetTitleSize()*axisTitleScale*1.1);
      effMapT1Vec[ih]->GetYaxis()->SetTitleSize( effMapT1Vec[ih]->GetYaxis()->GetTitleSize()*axisTitleScale);

      effMapT1Vec[ih]->GetXaxis()->SetTitleOffset( effMapT1Vec[ih]->GetXaxis()->GetTitleOffset()/axisTitleScale*1.71);
      effMapT1Vec[ih]->GetYaxis()->SetTitleOffset( effMapT1Vec[ih]->GetYaxis()->GetTitleOffset()/axisTitleScale*1.48);

      double ratioX = (xDivPadRatio+l_)/xDivPadRatio; // ratioY = (xDivPadRatio+t_)/yDivPadRatio;
      if( wrapFinder(leftVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetYaxis()->SetLabelSize( effMapT1Vec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1Vec[ih]->GetYaxis()->SetLabelSize( effMapT1Vec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(rightVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetZaxis()->SetLabelSize( effMapT1Vec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1Vec[ih]->GetZaxis()->SetLabelSize( effMapT1Vec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(bottomVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1Vec[ih]->GetXaxis()->SetLabelSize( effMapT1Vec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1Vec[ih]->GetXaxis()->SetLabelSize( effMapT1Vec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale*1.1);
            effMapT1Vec[ih]->GetXaxis()->SetLabelOffset( effMapT1Vec[ih]->GetXaxis()->GetLabelOffset()*axisLabelScale*1.3 );
         }
      }

      effMapT1Vec[ih]->GetXaxis()->SetTitleFont(42); effMapT1Vec[ih]->GetYaxis()->SetTitleFont(42); effMapT1Vec[ih]->GetZaxis()->SetTitleFont(42);
      effMapT1Vec[ih]->GetXaxis()->SetLabelFont(42); effMapT1Vec[ih]->GetYaxis()->SetLabelFont(42); effMapT1Vec[ih]->GetZaxis()->SetLabelFont(42);

      effMapT1Vec[ih]->SetTitle(name);
      effMapT1Vec[ih]->GetXaxis()->SetRangeUser(300, 1200);
      effMapT1Vec[ih]->GetYaxis()->SetRangeUser(50, 1200);
      effMapT1Vec[ih]->GetZaxis()->SetRangeUser(minZaxisT1, maxZaxisT1);
      effMapT1Vec[ih]->GetXaxis()->SetNdivisions(505);
      effMapT1Vec[ih]->GetYaxis()->SetNdivisions(506);
      if( wrapFinder(rightVec, is) ){
         effMapT1Vec[ih]->Draw("colz");
      }else{
         effMapT1Vec[ih]->Draw("col");
      }

      double disptFontSize = disptFontSizeFix;

      double disptPavexLow = disptPavexLowFix, disptPavexHigh = disptPavexHighFix;
      double disptPaveyLow = disptPaveyLowFix, disptPaveyHigh = disptPaveyHighFix;

      double disptLabelCMSxLow = disptLabelCMSxLowFix, disptLabelCMSxHigh = disptLabelCMSxHighFix;
      double disptLabelCMSyLow = disptLabelCMSyLowFix, disptLabelCMSyHigh = disptLabelCMSyHighFix;

      if( !wrapFinder(fourCornerVec, is) ){
         disptFontSize *= ratioX;
         if( !wrapFinder(middleVec, is) ){
//            disptFontSize /= 1.1;
         }
      }
      if( wrapFinder(leftVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);
      }
      if( wrapFinder(rightVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio)/(xDivPadRatio+r_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio)/(xDivPadRatio+r_);
      }
      if( wrapFinder(topVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio)/(yDivPadRatio+t_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio)/(yDivPadRatio+t_);
      } 
      if( wrapFinder(bottomVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);
      } 

      TPaveText *disptPave = new TPaveText(disptPavexLow, disptPaveyLow, disptPavexHigh, disptPaveyHigh, "NDC");
      disptPave->SetTextSize(disptFontSize); disptPave->SetTextFont(42); disptPave->SetTextAlign(13);
      disptPave->SetBorderSize(0);
      disptPave->SetFillColor(0);
      disptPave->AddText(disptRegionsHT[ih].c_str());
      disptPave->AddText(disptRegionsMHT[ih].c_str());
      disptPave->Draw();

      TPaveText *disptLabelCMS = new TPaveText(disptLabelCMSxLow, disptLabelCMSyLow, disptLabelCMSxHigh, disptLabelCMSyHigh, "NDC");
      disptLabelCMS->SetTextSize(disptFontSize); disptLabelCMS->SetTextFont(42); disptLabelCMS->SetTextAlign(13);
      disptLabelCMS->SetBorderSize(0);
      disptLabelCMS->SetFillColor(0);
      disptLabelCMS->AddText(cmsLabelStr.c_str());
      disptLabelCMS->Draw();

      TAxis *axisXT1 = (TAxis*)effMapT1Vec[ih]->GetXaxis();
      TAxis *axisYT1 = (TAxis*)effMapT1Vec[ih]->GetYaxis();

      if( wrapFinder(bottomVec, is) && !wrapFinder(rightVec, is)){
         double axisXModifXlow = 0.888, axisXModifXhigh = 1.0, axisXModifYlow = pad->GetBottomMargin()-0.075, axisXModifYhigh = pad->GetBottomMargin()-0.005;
         TPaveText* axisXmodifT1 = new TPaveText(axisXModifXlow, axisXModifYlow, axisXModifXhigh, axisXModifYhigh, "NDC");
         axisXmodifT1->SetTextSize(axisXT1->GetLabelSize()); axisXmodifT1->SetTextColor(axisXT1->GetLabelColor()); axisXmodifT1->SetTextFont(axisXT1->GetLabelFont());
         axisXmodifT1->SetBorderSize(0);
         axisXmodifT1->SetFillColor(0);
         axisXmodifT1->Draw();
      }

      if( wrapFinder(leftVec, is)){
         double axisYModifXlow = pad->GetLeftMargin()-0.135, axisYModifXhigh = pad->GetLeftMargin()-0.005, axisYModifYlow = 0.90, axisYModifYhigh = 1.0;
         TPaveText* axisYmodifT1 = new TPaveText(axisYModifXlow, axisYModifYlow, axisYModifXhigh, axisYModifYhigh, "NDC");
         axisYmodifT1->SetTextSize(axisYT1->GetLabelSize()); axisYmodifT1->SetTextColor(axisYT1->GetLabelColor()); axisYmodifT1->SetTextFont(axisYT1->GetLabelFont());
         axisYmodifT1->SetBorderSize(0);
         axisYmodifT1->SetFillColor(0);
         axisYmodifT1->Draw();
      }

      if( is == 7 ){
         TPaveText* axisXmodif2T1 = new TPaveText(0., pad->GetBottomMargin()-0.155, 0.2, pad->GetBottomMargin()-0.005, "NDC");
         axisXmodif2T1->SetTextSize(axisXT1->GetLabelSize()); axisXmodif2T1->SetTextColor(axisXT1->GetLabelColor()); axisXmodif2T1->SetTextFont(axisXT1->GetLabelFont());
         axisXmodif2T1->SetBorderSize(0);
         axisXmodif2T1->SetFillColor(0);
         axisXmodif2T1->Draw();
      }

      diagonalGraph->Draw("l");
   }
   cs->cd();
   TPad *titleT1Pad = new TPad("titleT1", "", 0, 1-t_, 1, 1.0); titleT1Pad->Draw(); titleT1Pad->cd();
   TPaveText *titleT1Pave = new TPaveText( 0.5-titleXoffsetFromCenter, 0.5-titleYoffsetFromCenter, 0.5+titleXoffsetFromCenter, 0.5+titleYoffsetFromCenter, "NDC");
   titleT1Pave->SetTextSize(0.48);
   titleT1Pave->SetBorderSize(0);
   titleT1Pave->SetFillColor(0);
   titleT1Pave->SetTextFont(42);
   titleT1Pave->AddText(titleT1Str.c_str());
   titleT1Pave->Draw();
    
   cs->Print("effMapT1_4x4.pdf");
   cs->Print("effMapT1_4x4.png");


// For T2. They are copied from T1 only with keyword replacement!

   for(int ih=0; ih<nsels; ih++){
      int is = mapHistToPadArr[ih];

      char padNames[20];
      sprintf(padNames, "pad%2d", is+1);
      cs->cd();

      TPad *pad =0;

      int condCnt = 0; std::vector<int> condIdxVec(9); std::vector<std::string> condStrVec(9);

// Bottom row
    // bottom-left corner
      condStrVec[0] = "bl"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[0] = 1; 
      }
    // bottom middle
      condStrVec[1] = "bm";  
      if( wrapFinder(bottomVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[1] = 1;
      }
    // bottom-right corner
      condStrVec[2] = "br"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[2] = 1;
      }

// Middle rows
   // middle-left
      condStrVec[3] = "ml"; 
      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[3] = 1;
      }
   // middle-middle
      condStrVec[4] = "mm"; 
      if( wrapFinder(middleVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[4] = 1;
      }
   // middle-right
      condStrVec[5] = "mr"; 
      if( wrapFinder(rightVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[5] = 1;
      }

// Top row
   // top-left
      condStrVec[6] = "tl"; 
      if( wrapFinder(topVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_);   
         condCnt++; condIdxVec[6] = 1;
      }
   // top-middle
      condStrVec[7] = "tm"; 
      if( wrapFinder(topVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[7] = 1;
      }
   // top-right
      condStrVec[8] = "tr"; 
      if( wrapFinder(topVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[8] = 1;
      }
 
      if( ih == 0 ){
         printf("%35s", " ");
         for(int id=0; id<9; id++){
            printf("  %2s", condStrVec[id].c_str());
         }
         std::cout<<std::endl;
      }
      printf("ih : %2d  is : %2d  condCnt : %2d  ==>", ih, is, condCnt);
      for(int id=0; id<9; id++){
         printf("  %2d", condIdxVec[id]);
      }
      std::cout<<std::endl;

      pad->Draw(); pad->cd(); pad->SetLogz();

      if( wrapFinder(leftVec, is) ){ pad->SetLeftMargin(l_*nX); pad->SetRightMargin(0); }
      if( wrapFinder(rightVec, is) ){ pad->SetRightMargin(r_*nX-0.0230);  pad->SetLeftMargin(0); }
      if( wrapFinder(topVec, is) ){ pad->SetTopMargin(t_*nY);    pad->SetBottomMargin(0); }
      if( wrapFinder(bottomVec, is) ){ pad->SetBottomMargin(b_*nY); pad->SetTopMargin(0); }
      if( wrapFinder(middleVec, is) ){ pad->SetTopMargin(0); pad->SetBottomMargin(0); pad->SetLeftMargin(0); pad->SetRightMargin(0); }
      if( is == 7 ){ pad->SetBottomMargin(b_*nY-0.0230); pad->SetTopMargin(0); }

      sprintf(name, "%s; %s", "", axisTitleT2Str.c_str());
      if( is == 10 ) sprintf(name, "%s; ; ", "");

      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT2Vec[ih]->GetYaxis()->SetLabelSize( effMapT2Vec[ih]->GetYaxis()->GetLabelSize()*ratioY);
         effMapT2Vec[ih]->GetYaxis()->SetTitleSize( effMapT2Vec[ih]->GetYaxis()->GetTitleSize()*ratioY);
      }
      if( wrapFinder(leftVec, is) && wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT2Vec[ih]->GetYaxis()->SetTitleOffset( effMapT2Vec[ih]->GetYaxis()->GetTitleOffset()*ratioY);
      }
      if( wrapFinder(bottomVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetXaxis()->SetTitleOffset( effMapT2Vec[ih]->GetXaxis()->GetTitleOffset()*ratioX);
         }
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetXaxis()->SetTitleSize( effMapT2Vec[ih]->GetXaxis()->GetTitleSize()*ratioX);
            effMapT2Vec[ih]->GetXaxis()->SetLabelSize( effMapT2Vec[ih]->GetXaxis()->GetLabelSize()*ratioX);
         }
      }
      if( wrapFinder(rightVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetZaxis()->SetLabelSize( effMapT2Vec[ih]->GetZaxis()->GetLabelSize()*ratioX);
         }
      }

      effMapT2Vec[ih]->GetXaxis()->SetTitleSize( effMapT2Vec[ih]->GetXaxis()->GetTitleSize()*axisTitleScale*1.1);
      effMapT2Vec[ih]->GetYaxis()->SetTitleSize( effMapT2Vec[ih]->GetYaxis()->GetTitleSize()*axisTitleScale);

      effMapT2Vec[ih]->GetXaxis()->SetTitleOffset( effMapT2Vec[ih]->GetXaxis()->GetTitleOffset()/axisTitleScale*1.71);
      effMapT2Vec[ih]->GetYaxis()->SetTitleOffset( effMapT2Vec[ih]->GetYaxis()->GetTitleOffset()/axisTitleScale*1.48);

      double ratioX = (xDivPadRatio+l_)/xDivPadRatio; // ratioY = (xDivPadRatio+t_)/yDivPadRatio;
      if( wrapFinder(leftVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetYaxis()->SetLabelSize( effMapT2Vec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT2Vec[ih]->GetYaxis()->SetLabelSize( effMapT2Vec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(rightVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetZaxis()->SetLabelSize( effMapT2Vec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT2Vec[ih]->GetZaxis()->SetLabelSize( effMapT2Vec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(bottomVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT2Vec[ih]->GetXaxis()->SetLabelSize( effMapT2Vec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT2Vec[ih]->GetXaxis()->SetLabelSize( effMapT2Vec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale*1.1);
            effMapT2Vec[ih]->GetXaxis()->SetLabelOffset( effMapT2Vec[ih]->GetXaxis()->GetLabelOffset()*axisLabelScale*1.3 );
         }
      }

      effMapT2Vec[ih]->GetXaxis()->SetTitleFont(42); effMapT2Vec[ih]->GetYaxis()->SetTitleFont(42); effMapT2Vec[ih]->GetZaxis()->SetTitleFont(42);
      effMapT2Vec[ih]->GetXaxis()->SetLabelFont(42); effMapT2Vec[ih]->GetYaxis()->SetLabelFont(42); effMapT2Vec[ih]->GetZaxis()->SetLabelFont(42);

      effMapT2Vec[ih]->SetTitle(name);
      effMapT2Vec[ih]->GetXaxis()->SetRangeUser(300, 1200);
      effMapT2Vec[ih]->GetYaxis()->SetRangeUser(50, 1200);
      effMapT2Vec[ih]->GetZaxis()->SetRangeUser(minZaxisT2, maxZaxisT2);
      effMapT2Vec[ih]->GetXaxis()->SetNdivisions(505);
      effMapT2Vec[ih]->GetYaxis()->SetNdivisions(506);
      if( wrapFinder(rightVec, is) ){
         effMapT2Vec[ih]->Draw("colz");
      }else{
         effMapT2Vec[ih]->Draw("col");
      }

      double disptFontSize = disptFontSizeFix;

      double disptPavexLow = disptPavexLowFix, disptPavexHigh = disptPavexHighFix;
      double disptPaveyLow = disptPaveyLowFix, disptPaveyHigh = disptPaveyHighFix;

      double disptLabelCMSxLow = disptLabelCMSxLowFix, disptLabelCMSxHigh = disptLabelCMSxHighFix;
      double disptLabelCMSyLow = disptLabelCMSyLowFix, disptLabelCMSyHigh = disptLabelCMSyHighFix;

      if( !wrapFinder(fourCornerVec, is) ){
         disptFontSize *= ratioX;
         if( !wrapFinder(middleVec, is) ){
//            disptFontSize /= 1.1;
         }
      }
      if( wrapFinder(leftVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);
      }
      if( wrapFinder(rightVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio)/(xDivPadRatio+r_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio)/(xDivPadRatio+r_);
      }
      if( wrapFinder(topVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio)/(yDivPadRatio+t_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio)/(yDivPadRatio+t_);
      } 
      if( wrapFinder(bottomVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);
      } 

      TPaveText *disptPave = new TPaveText(disptPavexLow, disptPaveyLow, disptPavexHigh, disptPaveyHigh, "NDC");
      disptPave->SetTextSize(disptFontSize); disptPave->SetTextFont(42); disptPave->SetTextAlign(13);
      disptPave->SetBorderSize(0);
      disptPave->SetFillColor(0);
      disptPave->AddText(disptRegionsHT[ih].c_str());
      disptPave->AddText(disptRegionsMHT[ih].c_str());
      disptPave->Draw();

      TPaveText *disptLabelCMS = new TPaveText(disptLabelCMSxLow, disptLabelCMSyLow, disptLabelCMSxHigh, disptLabelCMSyHigh, "NDC");
      disptLabelCMS->SetTextSize(disptFontSize); disptLabelCMS->SetTextFont(42); disptLabelCMS->SetTextAlign(13);
      disptLabelCMS->SetBorderSize(0);
      disptLabelCMS->SetFillColor(0);
      disptLabelCMS->AddText(cmsLabelStr.c_str());
      disptLabelCMS->Draw();

      TAxis *axisXT2 = (TAxis*)effMapT2Vec[ih]->GetXaxis();
      TAxis *axisYT2 = (TAxis*)effMapT2Vec[ih]->GetYaxis();

      if( wrapFinder(bottomVec, is) && !wrapFinder(rightVec, is)){
         double axisXModifXlow = 0.888, axisXModifXhigh = 1.0, axisXModifYlow = pad->GetBottomMargin()-0.075, axisXModifYhigh = pad->GetBottomMargin()-0.005;
         TPaveText* axisXmodifT2 = new TPaveText(axisXModifXlow, axisXModifYlow, axisXModifXhigh, axisXModifYhigh, "NDC");
         axisXmodifT2->SetTextSize(axisXT2->GetLabelSize()); axisXmodifT2->SetTextColor(axisXT2->GetLabelColor()); axisXmodifT2->SetTextFont(axisXT2->GetLabelFont());
         axisXmodifT2->SetBorderSize(0);
         axisXmodifT2->SetFillColor(0);
         axisXmodifT2->Draw();
      }

      if( wrapFinder(leftVec, is)){
         double axisYModifXlow = pad->GetLeftMargin()-0.135, axisYModifXhigh = pad->GetLeftMargin()-0.005, axisYModifYlow = 0.90, axisYModifYhigh = 1.0;
         TPaveText* axisYmodifT2 = new TPaveText(axisYModifXlow, axisYModifYlow, axisYModifXhigh, axisYModifYhigh, "NDC");
         axisYmodifT2->SetTextSize(axisYT2->GetLabelSize()); axisYmodifT2->SetTextColor(axisYT2->GetLabelColor()); axisYmodifT2->SetTextFont(axisYT2->GetLabelFont());
         axisYmodifT2->SetBorderSize(0);
         axisYmodifT2->SetFillColor(0);
         axisYmodifT2->Draw();
      }

      if( is == 7 ){
         TPaveText* axisXmodif2T2 = new TPaveText(0., pad->GetBottomMargin()-0.155, 0.2, pad->GetBottomMargin()-0.005, "NDC");
         axisXmodif2T2->SetTextSize(axisXT2->GetLabelSize()); axisXmodif2T2->SetTextColor(axisXT2->GetLabelColor()); axisXmodif2T2->SetTextFont(axisXT2->GetLabelFont());
         axisXmodif2T2->SetBorderSize(0);
         axisXmodif2T2->SetFillColor(0);
         axisXmodif2T2->Draw();
      }

      diagonalGraph->Draw("l");
   }
   cs->cd();
   TPad *titleT2Pad = new TPad("titleT2", "", 0, 1-t_, 1, 1.0); titleT2Pad->Draw(); titleT2Pad->cd();
   TPaveText *titleT2Pave = new TPaveText( 0.5-titleXoffsetFromCenter, 0.5-titleYoffsetFromCenter, 0.5+titleXoffsetFromCenter, 0.5+titleYoffsetFromCenter, "NDC");
   titleT2Pave->SetTextSize(0.48);
   titleT2Pave->SetBorderSize(0);
   titleT2Pave->SetFillColor(0);
   titleT2Pave->SetTextFont(42);
   titleT2Pave->AddText(titleT2Str.c_str());
   titleT2Pave->Draw();
    
   cs->Print("effMapT2_4x4.pdf");
   cs->Print("effMapT2_4x4.png");


// For T5ZZInc. They are copied from T1 only with keyword replacement!

   for(int ih=0; ih<nsels; ih++){
      int is = mapHistToPadArr[ih];

      char padNames[20];
      sprintf(padNames, "pad%2d", is+1);
      cs->cd();

      TPad *pad =0;

      int condCnt = 0; std::vector<int> condIdxVec(9); std::vector<std::string> condStrVec(9);

// Bottom row
    // bottom-left corner
      condStrVec[0] = "bl"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[0] = 1; 
      }
    // bottom middle
      condStrVec[1] = "bm";  
      if( wrapFinder(bottomVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[1] = 1;
      }
    // bottom-right corner
      condStrVec[2] = "br"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[2] = 1;
      }

// Middle rows
   // middle-left
      condStrVec[3] = "ml"; 
      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[3] = 1;
      }
   // middle-middle
      condStrVec[4] = "mm"; 
      if( wrapFinder(middleVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[4] = 1;
      }
   // middle-right
      condStrVec[5] = "mr"; 
      if( wrapFinder(rightVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[5] = 1;
      }

// Top row
   // top-left
      condStrVec[6] = "tl"; 
      if( wrapFinder(topVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_);   
         condCnt++; condIdxVec[6] = 1;
      }
   // top-middle
      condStrVec[7] = "tm"; 
      if( wrapFinder(topVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[7] = 1;
      }
   // top-right
      condStrVec[8] = "tr"; 
      if( wrapFinder(topVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[8] = 1;
      }
 
      if( ih == 0 ){
         printf("%35s", " ");
         for(int id=0; id<9; id++){
            printf("  %2s", condStrVec[id].c_str());
         }
         std::cout<<std::endl;
      }
      printf("ih : %2d  is : %2d  condCnt : %2d  ==>", ih, is, condCnt);
      for(int id=0; id<9; id++){
         printf("  %2d", condIdxVec[id]);
      }
      std::cout<<std::endl;

      pad->Draw(); pad->cd(); pad->SetLogz();

      if( wrapFinder(leftVec, is) ){ pad->SetLeftMargin(l_*nX); pad->SetRightMargin(0); }
      if( wrapFinder(rightVec, is) ){ pad->SetRightMargin(r_*nX-0.0230);  pad->SetLeftMargin(0); }
      if( wrapFinder(topVec, is) ){ pad->SetTopMargin(t_*nY);    pad->SetBottomMargin(0); }
      if( wrapFinder(bottomVec, is) ){ pad->SetBottomMargin(b_*nY); pad->SetTopMargin(0); }
      if( wrapFinder(middleVec, is) ){ pad->SetTopMargin(0); pad->SetBottomMargin(0); pad->SetLeftMargin(0); pad->SetRightMargin(0); }
      if( is == 7 ){ pad->SetBottomMargin(b_*nY-0.0230); pad->SetTopMargin(0); }

      sprintf(name, "%s; %s", "", axisTitleT5ZZIncStr.c_str());
      if( is == 10 ) sprintf(name, "%s; ; ", "");

      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT5ZZIncVec[ih]->GetYaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetYaxis()->GetLabelSize()*ratioY);
         effMapT5ZZIncVec[ih]->GetYaxis()->SetTitleSize( effMapT5ZZIncVec[ih]->GetYaxis()->GetTitleSize()*ratioY);
      }
      if( wrapFinder(leftVec, is) && wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT5ZZIncVec[ih]->GetYaxis()->SetTitleOffset( effMapT5ZZIncVec[ih]->GetYaxis()->GetTitleOffset()*ratioY);
      }
      if( wrapFinder(bottomVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetXaxis()->SetTitleOffset( effMapT5ZZIncVec[ih]->GetXaxis()->GetTitleOffset()*ratioX);
         }
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetXaxis()->SetTitleSize( effMapT5ZZIncVec[ih]->GetXaxis()->GetTitleSize()*ratioX);
            effMapT5ZZIncVec[ih]->GetXaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetXaxis()->GetLabelSize()*ratioX);
         }
      }
      if( wrapFinder(rightVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetZaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetZaxis()->GetLabelSize()*ratioX);
         }
      }

      effMapT5ZZIncVec[ih]->GetXaxis()->SetTitleSize( effMapT5ZZIncVec[ih]->GetXaxis()->GetTitleSize()*axisTitleScale*1.1);
      effMapT5ZZIncVec[ih]->GetYaxis()->SetTitleSize( effMapT5ZZIncVec[ih]->GetYaxis()->GetTitleSize()*axisTitleScale);

      effMapT5ZZIncVec[ih]->GetXaxis()->SetTitleOffset( effMapT5ZZIncVec[ih]->GetXaxis()->GetTitleOffset()/axisTitleScale*1.71);
      effMapT5ZZIncVec[ih]->GetYaxis()->SetTitleOffset( effMapT5ZZIncVec[ih]->GetYaxis()->GetTitleOffset()/axisTitleScale*1.48);

      double ratioX = (xDivPadRatio+l_)/xDivPadRatio; // ratioY = (xDivPadRatio+t_)/yDivPadRatio;
      if( wrapFinder(leftVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetYaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT5ZZIncVec[ih]->GetYaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(rightVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetZaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT5ZZIncVec[ih]->GetZaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(bottomVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT5ZZIncVec[ih]->GetXaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT5ZZIncVec[ih]->GetXaxis()->SetLabelSize( effMapT5ZZIncVec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale*1.1);
            effMapT5ZZIncVec[ih]->GetXaxis()->SetLabelOffset( effMapT5ZZIncVec[ih]->GetXaxis()->GetLabelOffset()*axisLabelScale*1.3 );
         }
      }

      effMapT5ZZIncVec[ih]->GetXaxis()->SetTitleFont(42); effMapT5ZZIncVec[ih]->GetYaxis()->SetTitleFont(42); effMapT5ZZIncVec[ih]->GetZaxis()->SetTitleFont(42);
      effMapT5ZZIncVec[ih]->GetXaxis()->SetLabelFont(42); effMapT5ZZIncVec[ih]->GetYaxis()->SetLabelFont(42); effMapT5ZZIncVec[ih]->GetZaxis()->SetLabelFont(42);

      effMapT5ZZIncVec[ih]->SetTitle(name);
      effMapT5ZZIncVec[ih]->GetXaxis()->SetRangeUser(300, 1200);
      effMapT5ZZIncVec[ih]->GetYaxis()->SetRangeUser(50, 1200);
      effMapT5ZZIncVec[ih]->GetZaxis()->SetRangeUser(minZaxisT5ZZInc, maxZaxisT5ZZInc);
      effMapT5ZZIncVec[ih]->GetXaxis()->SetNdivisions(505);
      effMapT5ZZIncVec[ih]->GetYaxis()->SetNdivisions(506);
      if( wrapFinder(rightVec, is) ){
         effMapT5ZZIncVec[ih]->Draw("colz");
      }else{
         effMapT5ZZIncVec[ih]->Draw("col");
      }

      double disptFontSize = disptFontSizeFix;

      double disptPavexLow = disptPavexLowFix, disptPavexHigh = disptPavexHighFix;
      double disptPaveyLow = disptPaveyLowFix, disptPaveyHigh = disptPaveyHighFix;

      double disptLabelCMSxLow = disptLabelCMSxLowFix, disptLabelCMSxHigh = disptLabelCMSxHighFix;
      double disptLabelCMSyLow = disptLabelCMSyLowFix, disptLabelCMSyHigh = disptLabelCMSyHighFix;

      if( !wrapFinder(fourCornerVec, is) ){
         disptFontSize *= ratioX;
         if( !wrapFinder(middleVec, is) ){
//            disptFontSize /= 1.1;
         }
      }
      if( wrapFinder(leftVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);
      }
      if( wrapFinder(rightVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio)/(xDivPadRatio+r_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio)/(xDivPadRatio+r_);
      }
      if( wrapFinder(topVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio)/(yDivPadRatio+t_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio)/(yDivPadRatio+t_);
      } 
      if( wrapFinder(bottomVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);
      } 

      TPaveText *disptPave = new TPaveText(disptPavexLow, disptPaveyLow, disptPavexHigh, disptPaveyHigh, "NDC");
      disptPave->SetTextSize(disptFontSize); disptPave->SetTextFont(42); disptPave->SetTextAlign(13);
      disptPave->SetBorderSize(0);
      disptPave->SetFillColor(0);
      disptPave->AddText(disptRegionsHT[ih].c_str());
      disptPave->AddText(disptRegionsMHT[ih].c_str());
      disptPave->Draw();

      TPaveText *disptLabelCMS = new TPaveText(disptLabelCMSxLow, disptLabelCMSyLow, disptLabelCMSxHigh, disptLabelCMSyHigh, "NDC");
      disptLabelCMS->SetTextSize(disptFontSize); disptLabelCMS->SetTextFont(42); disptLabelCMS->SetTextAlign(13);
      disptLabelCMS->SetBorderSize(0);
      disptLabelCMS->SetFillColor(0);
      disptLabelCMS->AddText(cmsLabelStr.c_str());
      disptLabelCMS->Draw();

      TAxis *axisXT5ZZInc = (TAxis*)effMapT5ZZIncVec[ih]->GetXaxis();
      TAxis *axisYT5ZZInc = (TAxis*)effMapT5ZZIncVec[ih]->GetYaxis();

      if( wrapFinder(bottomVec, is) && !wrapFinder(rightVec, is)){
         double axisXModifXlow = 0.888, axisXModifXhigh = 1.0, axisXModifYlow = pad->GetBottomMargin()-0.075, axisXModifYhigh = pad->GetBottomMargin()-0.005;
         TPaveText* axisXmodifT5ZZInc = new TPaveText(axisXModifXlow, axisXModifYlow, axisXModifXhigh, axisXModifYhigh, "NDC");
         axisXmodifT5ZZInc->SetTextSize(axisXT5ZZInc->GetLabelSize()); axisXmodifT5ZZInc->SetTextColor(axisXT5ZZInc->GetLabelColor()); axisXmodifT5ZZInc->SetTextFont(axisXT5ZZInc->GetLabelFont());
         axisXmodifT5ZZInc->SetBorderSize(0);
         axisXmodifT5ZZInc->SetFillColor(0);
         axisXmodifT5ZZInc->Draw();
      }

      if( wrapFinder(leftVec, is)){
         double axisYModifXlow = pad->GetLeftMargin()-0.135, axisYModifXhigh = pad->GetLeftMargin()-0.005, axisYModifYlow = 0.90, axisYModifYhigh = 1.0;
         TPaveText* axisYmodifT5ZZInc = new TPaveText(axisYModifXlow, axisYModifYlow, axisYModifXhigh, axisYModifYhigh, "NDC");
         axisYmodifT5ZZInc->SetTextSize(axisYT5ZZInc->GetLabelSize()); axisYmodifT5ZZInc->SetTextColor(axisYT5ZZInc->GetLabelColor()); axisYmodifT5ZZInc->SetTextFont(axisYT5ZZInc->GetLabelFont());
         axisYmodifT5ZZInc->SetBorderSize(0);
         axisYmodifT5ZZInc->SetFillColor(0);
         axisYmodifT5ZZInc->Draw();
      }

      if( is == 7 ){
         TPaveText* axisXmodif2T5ZZInc = new TPaveText(0., pad->GetBottomMargin()-0.155, 0.2, pad->GetBottomMargin()-0.005, "NDC");
         axisXmodif2T5ZZInc->SetTextSize(axisXT5ZZInc->GetLabelSize()); axisXmodif2T5ZZInc->SetTextColor(axisXT5ZZInc->GetLabelColor()); axisXmodif2T5ZZInc->SetTextFont(axisXT5ZZInc->GetLabelFont());
         axisXmodif2T5ZZInc->SetBorderSize(0);
         axisXmodif2T5ZZInc->SetFillColor(0);
         axisXmodif2T5ZZInc->Draw();
      }

      diagonalGraph->Draw("l");
   }
   cs->cd();
   TPad *titleT5ZZIncPad = new TPad("titleT5ZZInc", "", 0, 1-t_, 1, 1.0); titleT5ZZIncPad->Draw(); titleT5ZZIncPad->cd();
   TPaveText *titleT5ZZIncPave = new TPaveText( 0.5-titleXoffsetFromCenter, 0.5-titleYoffsetFromCenter, 0.5+titleXoffsetFromCenter, 0.5+titleYoffsetFromCenter, "NDC");
   titleT5ZZIncPave->SetTextSize(0.48);
   titleT5ZZIncPave->SetBorderSize(0);
   titleT5ZZIncPave->SetFillColor(0);
   titleT5ZZIncPave->SetTextFont(42);
   titleT5ZZIncPave->AddText(titleT5ZZIncStr.c_str());
   titleT5ZZIncPave->Draw();
    
   cs->Print("effMapT5ZZInc_4x4.pdf");
   cs->Print("effMapT5ZZInc_4x4.png");

// For T1bbbb. They are copied from T1 only with keyword replacement!

   for(int ih=0; ih<nsels; ih++){
      int is = mapHistToPadArr[ih];

      char padNames[20];
      sprintf(padNames, "pad%2d", is+1);
      cs->cd();

      TPad *pad =0;

      int condCnt = 0; std::vector<int> condIdxVec(9); std::vector<std::string> condStrVec(9);

// Bottom row
    // bottom-left corner
      condStrVec[0] = "bl"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[0] = 1; 
      }
    // bottom middle
      condStrVec[1] = "bm";  
      if( wrapFinder(bottomVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[1] = 1;
      }
    // bottom-right corner
      condStrVec[2] = "br"; 
      if( wrapFinder(bottomVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, (nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio);
         condCnt++; condIdxVec[2] = 1;
      }

// Middle rows
   // middle-left
      condStrVec[3] = "ml"; 
      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[3] = 1;
      }
   // middle-middle
      condStrVec[4] = "mm"; 
      if( wrapFinder(middleVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[4] = 1;
      }
   // middle-right
      condStrVec[5] = "mr"; 
      if( wrapFinder(rightVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio); 
         condCnt++; condIdxVec[5] = 1;
      }

// Top row
   // top-left
      condStrVec[6] = "tl"; 
      if( wrapFinder(topVec, is) && wrapFinder(leftVec, is) ){
         pad = new TPad(padNames, "", (is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_);   
         condCnt++; condIdxVec[6] = 1;
      }
   // top-middle
      condStrVec[7] = "tm"; 
      if( wrapFinder(topVec, is) && !wrapFinder(fourCornerVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[7] = 1;
      }
   // top-right
      condStrVec[8] = "tr"; 
      if( wrapFinder(topVec, is) && wrapFinder(rightVec, is) ){
         pad = new TPad(padNames, "", l_+(is%nX)*xDivPadRatio, b_+(nY-1-is/nX)*yDivPadRatio, l_+(is%nX+1)*xDivPadRatio+r_, b_+(nY-is/nX)*yDivPadRatio+t_); 
         condCnt++; condIdxVec[8] = 1;
      }
 
      if( ih == 0 ){
         printf("%35s", " ");
         for(int id=0; id<9; id++){
            printf("  %2s", condStrVec[id].c_str());
         }
         std::cout<<std::endl;
      }
      printf("ih : %2d  is : %2d  condCnt : %2d  ==>", ih, is, condCnt);
      for(int id=0; id<9; id++){
         printf("  %2d", condIdxVec[id]);
      }
      std::cout<<std::endl;

      pad->Draw(); pad->cd(); pad->SetLogz();

      if( wrapFinder(leftVec, is) ){ pad->SetLeftMargin(l_*nX); pad->SetRightMargin(0); }
      if( wrapFinder(rightVec, is) ){ pad->SetRightMargin(r_*nX-0.0230);  pad->SetLeftMargin(0); }
      if( wrapFinder(topVec, is) ){ pad->SetTopMargin(t_*nY);    pad->SetBottomMargin(0); }
      if( wrapFinder(bottomVec, is) ){ pad->SetBottomMargin(b_*nY); pad->SetTopMargin(0); }
      if( wrapFinder(middleVec, is) ){ pad->SetTopMargin(0); pad->SetBottomMargin(0); pad->SetLeftMargin(0); pad->SetRightMargin(0); }
      if( is == 7 ){ pad->SetBottomMargin(b_*nY-0.0230); pad->SetTopMargin(0); }

      sprintf(name, "%s; %s", "", axisTitleT1bbbbStr.c_str());
      if( is == 10 ) sprintf(name, "%s; ; ", "");

      if( wrapFinder(leftVec, is) && !wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT1bbbbVec[ih]->GetYaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetYaxis()->GetLabelSize()*ratioY);
         effMapT1bbbbVec[ih]->GetYaxis()->SetTitleSize( effMapT1bbbbVec[ih]->GetYaxis()->GetTitleSize()*ratioY);
      }
      if( wrapFinder(leftVec, is) && wrapFinder(fourCornerVec, is) ){
         double ratioY = (yDivPadRatio+t_)/yDivPadRatio;
         effMapT1bbbbVec[ih]->GetYaxis()->SetTitleOffset( effMapT1bbbbVec[ih]->GetYaxis()->GetTitleOffset()*ratioY);
      }
      if( wrapFinder(bottomVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetXaxis()->SetTitleOffset( effMapT1bbbbVec[ih]->GetXaxis()->GetTitleOffset()*ratioX);
         }
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetXaxis()->SetTitleSize( effMapT1bbbbVec[ih]->GetXaxis()->GetTitleSize()*ratioX);
            effMapT1bbbbVec[ih]->GetXaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetXaxis()->GetLabelSize()*ratioX);
         }
      }
      if( wrapFinder(rightVec, is) ){
         double ratioX = (xDivPadRatio+l_)/xDivPadRatio;
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetZaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetZaxis()->GetLabelSize()*ratioX);
         }
      }

      effMapT1bbbbVec[ih]->GetXaxis()->SetTitleSize( effMapT1bbbbVec[ih]->GetXaxis()->GetTitleSize()*axisTitleScale*1.1);
      effMapT1bbbbVec[ih]->GetYaxis()->SetTitleSize( effMapT1bbbbVec[ih]->GetYaxis()->GetTitleSize()*axisTitleScale);

      effMapT1bbbbVec[ih]->GetXaxis()->SetTitleOffset( effMapT1bbbbVec[ih]->GetXaxis()->GetTitleOffset()/axisTitleScale*1.71);
      effMapT1bbbbVec[ih]->GetYaxis()->SetTitleOffset( effMapT1bbbbVec[ih]->GetYaxis()->GetTitleOffset()/axisTitleScale*1.48);

      double ratioX = (xDivPadRatio+l_)/xDivPadRatio; // ratioY = (xDivPadRatio+t_)/yDivPadRatio;
      if( wrapFinder(leftVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetYaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1bbbbVec[ih]->GetYaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetYaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(rightVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetZaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1bbbbVec[ih]->GetZaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetZaxis()->GetLabelSize()*axisLabelScale);
         }
      }
      if( wrapFinder(bottomVec, is) ){
         if( !wrapFinder(fourCornerVec, is) ){
            effMapT1bbbbVec[ih]->GetXaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale);
         }else{
            effMapT1bbbbVec[ih]->GetXaxis()->SetLabelSize( effMapT1bbbbVec[ih]->GetXaxis()->GetLabelSize()*axisLabelScale*1.1);
            effMapT1bbbbVec[ih]->GetXaxis()->SetLabelOffset( effMapT1bbbbVec[ih]->GetXaxis()->GetLabelOffset()*axisLabelScale*1.3 );
         }
      }

      effMapT1bbbbVec[ih]->GetXaxis()->SetTitleFont(42); effMapT1bbbbVec[ih]->GetYaxis()->SetTitleFont(42); effMapT1bbbbVec[ih]->GetZaxis()->SetTitleFont(42);
      effMapT1bbbbVec[ih]->GetXaxis()->SetLabelFont(42); effMapT1bbbbVec[ih]->GetYaxis()->SetLabelFont(42); effMapT1bbbbVec[ih]->GetZaxis()->SetLabelFont(42);

      effMapT1bbbbVec[ih]->SetTitle(name);
      effMapT1bbbbVec[ih]->GetXaxis()->SetRangeUser(300, 1200);
      effMapT1bbbbVec[ih]->GetYaxis()->SetRangeUser(50, 1200);
      effMapT1bbbbVec[ih]->GetZaxis()->SetRangeUser(minZaxisT1bbbb, maxZaxisT1bbbb);
      effMapT1bbbbVec[ih]->GetXaxis()->SetNdivisions(505);
      effMapT1bbbbVec[ih]->GetYaxis()->SetNdivisions(506);
      if( wrapFinder(rightVec, is) ){
         effMapT1bbbbVec[ih]->Draw("colz");
      }else{
         effMapT1bbbbVec[ih]->Draw("col");
      }

      double disptFontSize = disptFontSizeFix;

      double disptPavexLow = disptPavexLowFix, disptPavexHigh = disptPavexHighFix;
      double disptPaveyLow = disptPaveyLowFix, disptPaveyHigh = disptPaveyHighFix;

      double disptLabelCMSxLow = disptLabelCMSxLowFix, disptLabelCMSxHigh = disptLabelCMSxHighFix;
      double disptLabelCMSyLow = disptLabelCMSyLowFix, disptLabelCMSyHigh = disptLabelCMSyHighFix;

      if( !wrapFinder(fourCornerVec, is) ){
         disptFontSize *= ratioX;
         if( !wrapFinder(middleVec, is) ){
//            disptFontSize /= 1.1;
         }
      }
      if( wrapFinder(leftVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio + l_)/(xDivPadRatio+l_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio + l_)/(xDivPadRatio+l_);
      }
      if( wrapFinder(rightVec, is) ){
         disptPavexLow = (disptPavexLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptPavexHigh = (disptPavexHigh*xDivPadRatio)/(xDivPadRatio+r_);

         disptLabelCMSxLow = (disptLabelCMSxLow*xDivPadRatio)/(xDivPadRatio+r_);
         disptLabelCMSxHigh = (disptLabelCMSxHigh*xDivPadRatio)/(xDivPadRatio+r_);
      }
      if( wrapFinder(topVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio)/(yDivPadRatio+t_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio)/(yDivPadRatio+t_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio)/(yDivPadRatio+t_);
      } 
      if( wrapFinder(bottomVec, is) ){
         disptPaveyLow = (disptPaveyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptPaveyHigh = (disptPaveyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);

         disptLabelCMSyLow = (disptLabelCMSyLow*yDivPadRatio + b_)/(yDivPadRatio+b_);
         disptLabelCMSyHigh = (disptLabelCMSyHigh*yDivPadRatio + b_)/(yDivPadRatio+b_);
      } 

      TPaveText *disptPave = new TPaveText(disptPavexLow, disptPaveyLow, disptPavexHigh, disptPaveyHigh, "NDC");
      disptPave->SetTextSize(disptFontSize); disptPave->SetTextFont(42); disptPave->SetTextAlign(13);
      disptPave->SetBorderSize(0);
      disptPave->SetFillColor(0);
      disptPave->AddText(disptRegionsHT[ih].c_str());
      disptPave->AddText(disptRegionsMHT[ih].c_str());
      disptPave->Draw();

      TPaveText *disptLabelCMS = new TPaveText(disptLabelCMSxLow, disptLabelCMSyLow, disptLabelCMSxHigh, disptLabelCMSyHigh, "NDC");
      disptLabelCMS->SetTextSize(disptFontSize); disptLabelCMS->SetTextFont(42); disptLabelCMS->SetTextAlign(13);
      disptLabelCMS->SetBorderSize(0);
      disptLabelCMS->SetFillColor(0);
      disptLabelCMS->AddText(cmsLabelStr.c_str());
      disptLabelCMS->Draw();

      TAxis *axisXT1bbbb = (TAxis*)effMapT1bbbbVec[ih]->GetXaxis();
      TAxis *axisYT1bbbb = (TAxis*)effMapT1bbbbVec[ih]->GetYaxis();

      if( wrapFinder(bottomVec, is) && !wrapFinder(rightVec, is)){
         double axisXModifXlow = 0.888, axisXModifXhigh = 1.0, axisXModifYlow = pad->GetBottomMargin()-0.075, axisXModifYhigh = pad->GetBottomMargin()-0.005;
         TPaveText* axisXmodifT1bbbb = new TPaveText(axisXModifXlow, axisXModifYlow, axisXModifXhigh, axisXModifYhigh, "NDC");
         axisXmodifT1bbbb->SetTextSize(axisXT1bbbb->GetLabelSize()); axisXmodifT1bbbb->SetTextColor(axisXT1bbbb->GetLabelColor()); axisXmodifT1bbbb->SetTextFont(axisXT1bbbb->GetLabelFont());
         axisXmodifT1bbbb->SetBorderSize(0);
         axisXmodifT1bbbb->SetFillColor(0);
         axisXmodifT1bbbb->Draw();
      }

      if( wrapFinder(leftVec, is)){
         double axisYModifXlow = pad->GetLeftMargin()-0.135, axisYModifXhigh = pad->GetLeftMargin()-0.005, axisYModifYlow = 0.90, axisYModifYhigh = 1.0;
         TPaveText* axisYmodifT1bbbb = new TPaveText(axisYModifXlow, axisYModifYlow, axisYModifXhigh, axisYModifYhigh, "NDC");
         axisYmodifT1bbbb->SetTextSize(axisYT1bbbb->GetLabelSize()); axisYmodifT1bbbb->SetTextColor(axisYT1bbbb->GetLabelColor()); axisYmodifT1bbbb->SetTextFont(axisYT1bbbb->GetLabelFont());
         axisYmodifT1bbbb->SetBorderSize(0);
         axisYmodifT1bbbb->SetFillColor(0);
         axisYmodifT1bbbb->Draw();
      }

      if( is == 7 ){
         TPaveText* axisXmodif2T1bbbb = new TPaveText(0., pad->GetBottomMargin()-0.155, 0.2, pad->GetBottomMargin()-0.005, "NDC");
         axisXmodif2T1bbbb->SetTextSize(axisXT1bbbb->GetLabelSize()); axisXmodif2T1bbbb->SetTextColor(axisXT1bbbb->GetLabelColor()); axisXmodif2T1bbbb->SetTextFont(axisXT1bbbb->GetLabelFont());
         axisXmodif2T1bbbb->SetBorderSize(0);
         axisXmodif2T1bbbb->SetFillColor(0);
         axisXmodif2T1bbbb->Draw();
      }

      diagonalGraph->Draw("l");
   }
   cs->cd();
   TPad *titleT1bbbbPad = new TPad("titleT1bbbb", "", 0, 1-t_, 1, 1.0); titleT1bbbbPad->Draw(); titleT1bbbbPad->cd();
   TPaveText *titleT1bbbbPave = new TPaveText( 0.5-titleXoffsetFromCenter, 0.5-titleYoffsetFromCenter, 0.5+titleXoffsetFromCenter, 0.5+titleYoffsetFromCenter, "NDC");
   titleT1bbbbPave->SetTextSize(0.48);
   titleT1bbbbPave->SetBorderSize(0);
   titleT1bbbbPave->SetFillColor(0);
   titleT1bbbbPave->SetTextFont(42);
   titleT1bbbbPave->AddText(titleT1bbbbStr.c_str());
   titleT1bbbbPave->Draw();
    
   cs->Print("effMapT1bbbb_4x4.pdf");
   cs->Print("effMapT1bbbb_4x4.png");



   std::string disptT1Str = "pp->gluino gluino, gluino->qq neutralino; squark mass >> gluino mass";
   std::string disptT2Str = "pp->squark squark, squark-> q neutralino; gluino mass >> squark mass"; 
   std::string disptT5ZZIncStr = "pp->gluino gluino, gluino->qq neutralino_2; neutralino_2->Z neutralino";
   std::string disptT1bbbbStr = "pp->gluino gluino, gluino->bb neutralino; sbottom mass >> gluino mass";
   char histTitle[200];

   TFile *combEffMapFile = new TFile("combinedEffMap.root", "RECREATE");
   TDirectory *outT1dir = combEffMapFile->mkdir("T1", disptT1Str.c_str());
   outT1dir->cd();
   for(int is=0; is<nsels; is++){
      TString nameT = effMapT1Vec[is]->GetName();
      nameT.ReplaceAll("h_RA2Eff_", "eff_");
      nameT.ReplaceAll("_SHIFT", "");
      effMapT1Vec[is]->SetName(nameT);
      sprintf(histTitle, "Acceptance x Efficiency : %16s #; %16s", histTitlesHT[is].c_str(), histTitlesMHT[is].c_str());
      effMapT1Vec[is]->SetTitle(histTitle);
      effMapT1Vec[is]->Write(); 
   }
   TDirectory *outT2dir = combEffMapFile->mkdir("T2", disptT2Str.c_str());
   outT2dir->cd();
   for(int is=0; is<nsels; is++){
      TString nameT = effMapT2Vec[is]->GetName();
      nameT.ReplaceAll("h_RA2Eff_", "eff_");
      nameT.ReplaceAll("_SHIFT", "");
      effMapT2Vec[is]->SetName(nameT);
      sprintf(histTitle, "Acceptance x Efficiency : %16s #; %16s", histTitlesHT[is].c_str(), histTitlesMHT[is].c_str());
      effMapT2Vec[is]->SetTitle(histTitle);
      effMapT2Vec[is]->Write(); 
   }
   TDirectory *outT5ZZIncdir = combEffMapFile->mkdir("T5ZZInc", disptT5ZZIncStr.c_str());
   outT5ZZIncdir->cd();
   for(int is=0; is<nsels; is++){
      TString nameT = effMapT5ZZIncVec[is]->GetName();
      nameT.ReplaceAll("h_RA2Eff_", "eff_");
      nameT.ReplaceAll("_SHIFT", "");
      effMapT5ZZIncVec[is]->SetName(nameT);
      sprintf(histTitle, "Acceptance x Efficiency : %16s #; %16s", histTitlesHT[is].c_str(), histTitlesMHT[is].c_str());
      effMapT5ZZIncVec[is]->SetTitle(histTitle);
      effMapT5ZZIncVec[is]->Write(); 
   }
   TDirectory *outT1bbbbdir = combEffMapFile->mkdir("T1bbbb", disptT1bbbbStr.c_str());
   outT1bbbbdir->cd();
   for(int is=0; is<nsels; is++){
      TString nameT = effMapT1bbbbVec[is]->GetName();
      nameT.ReplaceAll("h_RA2Eff_", "eff_");
      nameT.ReplaceAll("_SHIFT", "");
      effMapT1bbbbVec[is]->SetName(nameT);
      sprintf(histTitle, "Acceptance x Efficiency : %16s #; %16s", histTitlesHT[is].c_str(), histTitlesMHT[is].c_str());
      effMapT1bbbbVec[is]->SetTitle(histTitle);
      effMapT1bbbbVec[is]->Write(); 
   }
   combEffMapFile->Write(); combEffMapFile->Close();
}


void fillEmptyPoints(const TH2D *inHist, TH2D *outHist, int xBinLargery, int pickedMethod){
   if( outHist ) delete outHist;
   outHist = (TH2D*) inHist->Clone();

   int nXbinsTmp = outHist->GetXaxis()->GetNbins();
   int nYbinsTmp = outHist->GetYaxis()->GetNbins();

   int foundEmpty = 1;
/*
   double biasLeftUp   = 3.0,    biasUp   = 2.0,     biasRightUp   = 1.0;

   double biasLeft     = 2.0,                        biasRight     = 2.0/3.0; 

   double biasLeftDown = 1.0,    biasDown = 2.0/3.0, biasRightDown = 1.0/3.0;
*/
//   const double scale = 1.0 + 1e-5;
   const double scale = 1.0;

   double biasLeftUp   = scale,          biasUp   = (1.0+scale)/2.,            biasRightUp   = 1.0;

   double biasLeft     = (1.0+scale)/2.,  /*center   = 1.0*/                   biasRight     = (1.0+scale)/2.0/scale;

   double biasLeftDown = 1.0,            biasDown = (1.0+scale)/2.0/scale,     biasRightDown = 1.0/scale;

   while( foundEmpty ){
      foundEmpty =0;
      for(int ix=nXbinsTmp; ix>=1; ix--){
         for(int iy=1; iy<=nYbinsTmp; iy++){
// Removal of diagonal region
            if( ix-iy < xBinLargery ) continue;
// Search along Y axis for only nYbinsTmp*ix + nXbinsTmp*iy >= nXbinsTmp*nYbinsTmp (the upper-right region)
//            if( nYbinsTmp*ix + nXbinsTmp*iy < nXbinsTmp*nYbinsTmp ) continue; 
            double cont = outHist->GetBinContent(ix, iy);
            if( cont == 0 ){
               int left_ix = ix-1, right_ix = ix+1;
               int down_iy = iy-1, up_iy = iy+1;
               double left_cont = 0, right_cont = 0, down_cont = 0, up_cont = 0;
               double left_down_cont = 0, left_up_cont = 0, right_down_cont = 0, right_up_cont = 0;
               double totalDist = 0, totalWeight =0;
               int nonZeroNeighborCnt =0;
               if( left_ix >=1 ){
                  if( pickedMethod == 1 ) left_cont = outHist->GetBinContent(left_ix, iy); 
                  if( pickedMethod == 2 ) left_cont = inHist->GetBinContent(left_ix, iy); 
                  if( left_cont !=0 ){ 
                     totalDist += biasLeft*1.0; totalWeight += left_cont*biasLeft; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( right_ix <=nXbinsTmp ){ 
                  if( pickedMethod == 1 ) right_cont = outHist->GetBinContent(right_ix, iy); 
                  if( pickedMethod == 2 ) right_cont = inHist->GetBinContent(right_ix, iy); 
                  if( right_cont !=0 ){ 
                     totalDist += biasRight*1.0; totalWeight += right_cont*biasRight; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( down_iy >=1 ){ 
                  if( pickedMethod == 1 ) down_cont = outHist->GetBinContent(ix, down_iy); 
                  if( pickedMethod == 2 ) down_cont = inHist->GetBinContent(ix, down_iy); 
                  if( down_cont !=0 ){ 
                     totalDist += biasDown *1.0; totalWeight += down_cont*biasDown; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) up_cont = outHist->GetBinContent(ix, up_iy); 
                  if( pickedMethod == 2 ) up_cont = inHist->GetBinContent(ix, up_iy); 
                  if( up_cont !=0 ){
                     totalDist += biasUp *1.0; totalWeight += up_cont*biasUp; 
                     nonZeroNeighborCnt++;
                  }
               }

               if( left_ix >=1 && down_iy >=1 ){ 
                  if( pickedMethod == 1 ) left_down_cont = outHist->GetBinContent(left_ix, down_iy); 
                  if( pickedMethod == 2 ) left_down_cont = inHist->GetBinContent(left_ix, down_iy); 
                  if( left_down_cont !=0 ){
                     totalDist += biasLeftDown*sqrt(2.0); totalWeight += left_down_cont*biasLeftDown*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( left_ix >=1 && up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) left_up_cont = outHist->GetBinContent(left_ix, up_iy); 
                  if( pickedMethod == 2 ) left_up_cont = inHist->GetBinContent(left_ix, up_iy); 
                  if( left_up_cont !=0 ){
                     totalDist += biasLeftUp*sqrt(2.0); totalWeight += left_up_cont*biasLeftUp*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( right_ix <=nXbinsTmp && down_iy >=1 ){ 
                  if( pickedMethod == 1 ) right_down_cont = outHist->GetBinContent(right_ix, down_iy); 
                  if( pickedMethod == 2 ) right_down_cont = inHist->GetBinContent(right_ix, down_iy); 
                  if( right_down_cont !=0 ){
                     totalDist += biasRightDown*sqrt(2.0); totalWeight += right_down_cont*biasRightDown*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( right_ix <=nXbinsTmp && up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) right_up_cont = outHist->GetBinContent(right_ix, up_iy); 
                  if( pickedMethod == 2 ) right_up_cont = inHist->GetBinContent(right_ix, up_iy); 
                  if( right_up_cont !=0 ){
                     totalDist += biasRightUp*sqrt(2.0); totalWeight += right_up_cont*biasRightUp*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }

               if(pickedMethod == 1 ) if( (iy !=3 && nonZeroNeighborCnt <5) || (iy==3 && nonZeroNeighborCnt<3) ) continue;
               if(pickedMethod == 2 ) if( (iy !=3 && nonZeroNeighborCnt <5) || (iy==3 && nonZeroNeighborCnt<3) ) continue;
               foundEmpty ++;

               double cont_center = -1.0;
               if( totalDist !=0 ) cont_center = totalWeight/totalDist;
               outHist->SetBinContent(ix, iy, cont_center);
            }
         }
      }

      for(int iy=1; iy<=nYbinsTmp; iy++){
         for(int ix=nXbinsTmp; ix>=1; ix--){
// Removal of diagonal region
            if( ix-iy < xBinLargery ) continue;
// Search along Y axis for only nYbinsTmp*ix + nXbinsTmp*iy <= nXbinsTmp*nYbinsTmp (the bottom-left region)
//            if( nYbinsTmp*ix + nXbinsTmp*iy > nXbinsTmp*nYbinsTmp ) continue; 
            double cont = outHist->GetBinContent(ix, iy);
            if( cont == 0 ){
               int left_ix = ix-1, right_ix = ix+1;
               int down_iy = iy-1, up_iy = iy+1;
               double left_cont = 0, right_cont = 0, down_cont = 0, up_cont = 0;
               double left_down_cont = 0, left_up_cont = 0, right_down_cont = 0, right_up_cont = 0;
               double totalDist = 0, totalWeight =0;
               int nonZeroNeighborCnt =0;
               if( left_ix >=1 ){
                  if( pickedMethod == 1 ) left_cont = outHist->GetBinContent(left_ix, iy); 
                  if( pickedMethod == 2 ) left_cont = inHist->GetBinContent(left_ix, iy); 
                  if( left_cont !=0 ){ 
                     totalDist += biasLeft*1.0; totalWeight += left_cont*biasLeft; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( right_ix <=nXbinsTmp ){ 
                  if( pickedMethod == 1 ) right_cont = outHist->GetBinContent(right_ix, iy); 
                  if( pickedMethod == 2 ) right_cont = inHist->GetBinContent(right_ix, iy); 
                  if( right_cont !=0 ){ 
                     totalDist += biasRight*1.0; totalWeight += right_cont*biasRight; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( down_iy >=1 ){ 
                  if( pickedMethod == 1 ) down_cont = outHist->GetBinContent(ix, down_iy); 
                  if( pickedMethod == 2 ) down_cont = inHist->GetBinContent(ix, down_iy); 
                  if( down_cont !=0 ){ 
                     totalDist += biasDown *1.0; totalWeight += down_cont*biasDown; 
                     nonZeroNeighborCnt++;
                  } 
               }
               if( up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) up_cont = outHist->GetBinContent(ix, up_iy); 
                  if( pickedMethod == 2 ) up_cont = inHist->GetBinContent(ix, up_iy); 
                  if( up_cont !=0 ){
                     totalDist += biasUp *1.0; totalWeight += up_cont*biasUp; 
                     nonZeroNeighborCnt++;
                  }
               }

               if( left_ix >=1 && down_iy >=1 ){ 
                  if( pickedMethod == 1 ) left_down_cont = outHist->GetBinContent(left_ix, down_iy); 
                  if( pickedMethod == 2 ) left_down_cont = inHist->GetBinContent(left_ix, down_iy); 
                  if( left_down_cont !=0 ){
                     totalDist += biasLeftDown*sqrt(2.0); totalWeight += left_down_cont*biasLeftDown*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( left_ix >=1 && up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) left_up_cont = outHist->GetBinContent(left_ix, up_iy); 
                  if( pickedMethod == 2 ) left_up_cont = inHist->GetBinContent(left_ix, up_iy); 
                  if( left_up_cont !=0 ){
                     totalDist += biasLeftUp*sqrt(2.0); totalWeight += left_up_cont*biasLeftUp*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( right_ix <=nXbinsTmp && down_iy >=1 ){ 
                  if( pickedMethod == 1 ) right_down_cont = outHist->GetBinContent(right_ix, down_iy); 
                  if( pickedMethod == 2 ) right_down_cont = inHist->GetBinContent(right_ix, down_iy); 
                  if( right_down_cont !=0 ){
                     totalDist += biasRightDown*sqrt(2.0); totalWeight += right_down_cont*biasRightDown*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }
               if( right_ix <=nXbinsTmp && up_iy <=nYbinsTmp ){ 
                  if( pickedMethod == 1 ) right_up_cont = outHist->GetBinContent(right_ix, up_iy); 
                  if( pickedMethod == 2 ) right_up_cont = inHist->GetBinContent(right_ix, up_iy); 
                  if( right_up_cont !=0 ){
                     totalDist += biasRightUp*sqrt(2.0); totalWeight += right_up_cont*biasRightUp*sqrt(2.0); 
                     nonZeroNeighborCnt++;
                  }
               }

               if( pickedMethod == 1 ) if( (iy !=3 && nonZeroNeighborCnt <5) || (iy==3 && nonZeroNeighborCnt<3) ) continue;
               if( pickedMethod == 2 ) if( (iy !=3 && nonZeroNeighborCnt <5) || (iy==3 && nonZeroNeighborCnt<3) ) continue;
               foundEmpty ++;

               double cont_center = -1.0;
               if( totalDist !=0 ) cont_center = totalWeight/totalDist;
               outHist->SetBinContent(ix, iy, cont_center);
            }
         }
      }
   }
}
