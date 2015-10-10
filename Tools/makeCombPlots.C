#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt */

#include <sstream>
#include <iostream>
#include <fstream>

#include "TStopwatch.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"

#include "tdrstyle.h"
#include "TPrincipal.h"

#include "TH2Poly.h"

#include "THStack.h"

#include "TFile.h"

#include "searchBins.h"

#include "samples.h"

AnaSamples::SampleSet        allSamples;
AnaSamples::SampleCollection allCollections(allSamples);

std::vector<TH2Poly*> h2_sumSM_poly_MT2_vs_metVec, h2_sumSM_poly_muonCSforLostLept_MT2_vs_metVec;
std::vector<TH2Poly*> h2_sumSM_poly_0bZnunu_MT2_vs_metVec;

std::vector<TH1D*> h1_cutFlowVec;

char names[200], dispt[200];

TStopwatch timer;

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

std::vector<std::string> todraw_h1_keyStrVec = {"nJets", "nbJets", "nTops", "met", "metphi", "HT", "MT2", "vtxSize", "allJetPt", "allJetM", "leadJetPt", "leadJetM"};
std::vector<int>         todraw_h1_rebinVec  = {      1,        1,       1,    5,        5,     5,     5,         1,          5,         5,           5,          5};

std::vector<std::vector<TH1D*> > cached_h1Vec(todraw_h1_keyStrVec.size());
std::vector<std::string> cached_sampleStrVec;
std::vector<int> cached_sampleColorVec;

const double scaleToSameYield = 1./1.5;

void makeCombPlots(const std::string cutLev="baseline"){

   double dataLumi = 0;

   for(const auto & file : allCollections["Data_HTMHT25ns"]) dataLumi += file.lumi;
   double scaleMCtoData = dataLumi/AnaSamples::luminosity * scaleToSameYield;
   std::cout<<"\ndataLumi : "<<dataLumi<<"  mc assumed lumi : "<<AnaSamples::luminosity<<"  scaleMCtoData : "<<scaleMCtoData<<std::endl<<std::endl;

   TFile * TTbar_file = new TFile("basicCheck_TTbar.root");
   TH1D * TTbar_h1_keyString = (TH1D*) TTbar_file->Get("h1_keyString"); TTbar_h1_keyString->LabelsDeflate();
   std::string TTbar_sampleKey = TTbar_h1_keyString->GetXaxis()->GetBinLabel(1); int TTbar_color = allCollections[TTbar_sampleKey].front().color;
   cached_sampleStrVec.push_back(TTbar_sampleKey); cached_sampleColorVec.push_back(TTbar_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTbar_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)TTbar_file->Get(hfullname.c_str()));
   }
    
   TFile * WJetsToLNu_file = new TFile("basicCheck_WJetsToLNu.root");
   TH1D * WJetsToLNu_h1_keyString = (TH1D*) WJetsToLNu_file->Get("h1_keyString"); WJetsToLNu_h1_keyString->LabelsDeflate();
   std::string WJetsToLNu_sampleKey = WJetsToLNu_h1_keyString->GetXaxis()->GetBinLabel(1); int WJetsToLNu_color = allCollections[WJetsToLNu_sampleKey].front().color;
   cached_sampleStrVec.push_back(WJetsToLNu_sampleKey); cached_sampleColorVec.push_back(WJetsToLNu_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = WJetsToLNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)WJetsToLNu_file->Get(hfullname.c_str()));
   }

   TFile * ZJetsToNuNu_file = new TFile("basicCheck_ZJetsToNuNu.root");
   TH1D * ZJetsToNuNu_h1_keyString = (TH1D*) ZJetsToNuNu_file->Get("h1_keyString"); ZJetsToNuNu_h1_keyString->LabelsDeflate();
   std::string ZJetsToNuNu_sampleKey = ZJetsToNuNu_h1_keyString->GetXaxis()->GetBinLabel(1); int ZJetsToNuNu_color = allCollections[ZJetsToNuNu_sampleKey].front().color;
   cached_sampleStrVec.push_back(ZJetsToNuNu_sampleKey); cached_sampleColorVec.push_back(ZJetsToNuNu_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = ZJetsToNuNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)ZJetsToNuNu_file->Get(hfullname.c_str()));
   }
    
   TFile * QCD_file = new TFile("basicCheck_QCD.root");
   TH1D * QCD_h1_keyString = (TH1D*) QCD_file->Get("h1_keyString"); QCD_h1_keyString->LabelsDeflate();
   std::string QCD_sampleKey = QCD_h1_keyString->GetXaxis()->GetBinLabel(1); int QCD_color = allCollections[QCD_sampleKey].front().color;
   cached_sampleStrVec.push_back(QCD_sampleKey); cached_sampleColorVec.push_back(QCD_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = QCD_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)QCD_file->Get(hfullname.c_str()));
   }
    
   TFile * TTZ_file = new TFile("basicCheck_TTZ.root");
   TH1D * TTZ_h1_keyString = (TH1D*) TTZ_file->Get("h1_keyString"); TTZ_h1_keyString->LabelsDeflate();
   std::string TTZ_sampleKey = TTZ_h1_keyString->GetXaxis()->GetBinLabel(1); int TTZ_color = allCollections[TTZ_sampleKey].front().color;
   cached_sampleStrVec.push_back(TTZ_sampleKey); cached_sampleColorVec.push_back(TTZ_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTZ_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)TTZ_file->Get(hfullname.c_str()));
   }
    
   TFile * TTW_file = new TFile("basicCheck_TTW.root");
   TH1D * TTW_h1_keyString = (TH1D*) TTW_file->Get("h1_keyString"); TTW_h1_keyString->LabelsDeflate();
   std::string TTW_sampleKey = TTW_h1_keyString->GetXaxis()->GetBinLabel(1); int TTW_color = allCollections[TTW_sampleKey].front().color;
   cached_sampleStrVec.push_back(TTW_sampleKey); cached_sampleColorVec.push_back(TTW_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)TTW_file->Get(hfullname.c_str()));
   }
    
   TFile * tW_file = new TFile("basicCheck_tW.root");
   TH1D * tW_h1_keyString = (TH1D*) tW_file->Get("h1_keyString"); tW_h1_keyString->LabelsDeflate();
   std::string tW_sampleKey = tW_h1_keyString->GetXaxis()->GetBinLabel(1); int tW_color = (int)tW_h1_keyString->GetBinContent(1);
   cached_sampleStrVec.push_back(tW_sampleKey); cached_sampleColorVec.push_back(tW_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = tW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)tW_file->Get(hfullname.c_str()));
   }
    
   TFile * Data_HTMHT25ns_file = new TFile("basicCheck_Data_HTMHT25ns.root");
   TH1D * Data_HTMHT25ns_h1_keyString = (TH1D*) Data_HTMHT25ns_file->Get("h1_keyString"); Data_HTMHT25ns_h1_keyString->LabelsDeflate();
   std::string Data_HTMHT25ns_sampleKey = Data_HTMHT25ns_h1_keyString->GetXaxis()->GetBinLabel(1); int Data_HTMHT25ns_color = (int)Data_HTMHT25ns_h1_keyString->GetBinContent(1);
   cached_sampleStrVec.push_back(Data_HTMHT25ns_sampleKey); cached_sampleColorVec.push_back(Data_HTMHT25ns_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = Data_HTMHT25ns_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)Data_HTMHT25ns_file->Get(hfullname.c_str()));
   }
    

   TCanvas *cs = new TCanvas("cs", "cs", 1200, 900);
   int divW=3, divH=3;
   cs->Divide(divW, divH);

   TCanvas *ct = new TCanvas("ct", "ct", 1200, 900);

   Float_t legendX1 = .60;
   Float_t legendX2 = .85;
   Float_t legendY1 = .65;
   Float_t legendY2 = .85;
   TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   catLeg1->SetTextSize(0.025);

// Plotting
   setTDRStyle();

  //  For the Global title:
   tdrStyle->SetOptTitle(1);
   tdrStyle->SetTitleFont(42,"");
   tdrStyle->SetTitleColor(1);
   tdrStyle->SetTitleTextColor(1);
   tdrStyle->SetTitleFillColor(0);
   tdrStyle->SetTitleFontSize(0.1);
   tdrStyle->SetTitleAlign(13);
   tdrStyle->SetTitleX(0.6);
   tdrStyle->SetTitleH(0.05);
   tdrStyle->SetTitleBorderSize(0);
   tdrStyle->SetTitleAlign(13);
   tdrStyle->SetTitleX(0.19);
   tdrStyle->SetTitleH(0.038);
                                                                         
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
 
//   tdrStyle->SetOptStat(1111);
 
   tdrStyle->SetHistLineWidth(1);
 
   tdrStyle->SetPaintTextFormat("4.2f");

   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

   for(unsigned int ip=0; ip<todraw_h1_keyStrVec.size(); ip++){
      ct->cd(); catLeg1->Clear();

      THStack * hs_sum_SM = new THStack("hs", "");
      TH1D * tmp_data = 0;
      for(unsigned int is=0; is<cached_h1Vec[ip].size(); is++){

         TH1D * tmp_any = (TH1D*) cached_h1Vec[ip][is]->Clone();

         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            tmp_data = (TH1D*)tmp_any->Clone();
            tmp_data->Rebin(todraw_h1_rebinVec[ip]);
            catLeg1->AddEntry(tmp_data, cached_sampleStrVec[is].c_str());
            continue;
         }

         tmp_any->Rebin(todraw_h1_rebinVec[ip]);
         tmp_any->Scale(scaleMCtoData); 
         tmp_any->SetFillColor(cached_sampleColorVec[is]); tmp_any->SetLineColor(cached_sampleColorVec[is]); tmp_any->SetMarkerColor(cached_sampleColorVec[is]);
         hs_sum_SM->Add(tmp_any);
         catLeg1->AddEntry(tmp_any, cached_sampleStrVec[is].c_str());
      }

      hs_sum_SM->Draw("hist");
      hs_sum_SM->SetMaximum(hs_sum_SM->GetMaximum()*1.5);
      if( todraw_h1_keyStrVec[ip] == "HT" ) hs_sum_SM->GetXaxis()->SetRangeUser(400, hs_sum_SM->GetXaxis()->GetXmax());
      tmp_data->Draw("same");

      if( todraw_h1_keyStrVec[ip] == "nJets" ){
         TH1D * last = (TH1D*)hs_sum_SM->GetStack()->Last();
         std::cout<<"\ndata : "<<tmp_data->Integral()<<"  MC : "<<last->Integral()<<std::endl<<std::endl;
      }

      catLeg1->SetFillColor(kWhite);
      catLeg1->SetBorderSize(0);
      catLeg1->Draw();
      ct->Print("comb_"+TString(todraw_h1_keyStrVec[ip])+"_"+TString(cutLev)+".pdf");
   }
}

int main(int argc, char *argv[]){
   std::string cutLev = "baseline";
   if( argc >= 2 ) cutLev = argv[1];
   std::cout<<"\ncutLev : "<<cutLev.c_str()<<std::endl;
   makeCombPlots(cutLev);
}


