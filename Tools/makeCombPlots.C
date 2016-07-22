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
#include "TLine.h"

#include "TLatex.h"

#include "TF1.h"

#include "TFile.h"

#include "searchBins.h"

#include "samples.h"

SearchBins * sb =0;
int nTotBins;
std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

AnaSamples::SampleSet        allSamples;
AnaSamples::SampleCollection allCollections(allSamples);

std::vector<TH2Poly*> h2_sumSM_poly_MT2_vs_metVec, h2_sumSM_poly_muonCSforLostLept_MT2_vs_metVec;
std::vector<TH2Poly*> h2_sumSM_poly_0bZnunu_MT2_vs_metVec;

std::vector<TH1D*> h1_cutFlowVec;

char names[200], dispt[200];

TStopwatch timer;

const double ymax_Yields = 500.;
const double ymin_Yields = 0.05;

bool displaybins = false, display45bins = false;

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

std::vector<std::string> todraw_h1_keyStrVec = {"nJets", "nbJets", "nTops", "met", "metphi", "HT", "MT2", "vtxSize", "allJetPt", "allJetM", "leadJetPt", "leadJetM", "topMass", "muPt", "dphi1", "dphi2", "dphi3", "cutFlow", "searchBinYields"};
std::vector<int>         todraw_h1_rebinVec  = {      1,        1,       1,    5,       10,     5,     5,         1,          5,         5,           5,          5,         5,      5,      10,      10,      10,        1,                  1};
std::vector<std::string> todraw_h1_xLabelVec = {"N_{jets}", "N_{b}", "N_{t}", "E_{T}^{miss} (GeV)", "#phi_{E_{T}^{miss}}", "H_{T} (GeV)", "M_{T2} (GeV)", "N_{vtx}", "P_{T}(all jets) (GeV)", "M(all jets) (GeV)", "P_{T}(lead jet) (GeV)", "M(lead jet) (GeV)", "M(top) (GeV)", "P_{T}(#mu)", "#Delta#phi(j_{1}, E_{T}^{miss})", "#Delta#phi(j_{2}, E_{T}^{miss})", "#Delta#phi(j_{3}, E_{T}^{miss})", "cutFlow", "idx(searchBin)"};

std::vector<std::vector<TH1D*> > cached_h1Vec(todraw_h1_keyStrVec.size());
std::vector<std::string> cached_sampleStrVec;
std::vector<int> cached_sampleColorVec;

TH1D * h1_searchBinYields_topW = 0, * h1_searchBinYields_HadTau_topW = 0, * h1_searchBinYields_LostLep_topW = 0, * h1_searchBinYields_Overlap_topW = 0; 

void makeCombPlots(const std::string cutLev="baseline", const std::string datatype="HTMHT"){

   double dataLumi = 0;

   std::string colName = "Data_"+datatype;
   for(const auto & file : allCollections[colName]) dataLumi += file.lumi;
//   dataLumi = 2153.736;
//   dataLumi = 10000.0;
   dataLumi = 5871.102343734 + 1760.842972044;
   double scaleMCtoData = dataLumi/AnaSamples::luminosity;
   std::cout<<"\ndataLumi : "<<dataLumi<<"  mc assumed lumi : "<<AnaSamples::luminosity<<"  scaleMCtoData : "<<scaleMCtoData<<std::endl<<std::endl;

   TFile * TTbar_file = new TFile("basicCheck_TTbarNoHad.root");
   TH1D * TTbar_h1_keyString = (TH1D*) TTbar_file->Get("h1_keyString"); TTbar_h1_keyString->LabelsDeflate();
   std::string TTbar_sampleKey = "TTbar"; int TTbar_color = allCollections[TTbar_sampleKey].front().color;
   cached_sampleStrVec.push_back(TTbar_sampleKey); cached_sampleColorVec.push_back(TTbar_color);

   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      TH1D * tmpHist = 0;
      for(unsigned int ib=0; ib<TTbar_h1_keyString->GetXaxis()->GetNbins(); ib++){
         std::string per_sampleKey = TTbar_h1_keyString->GetXaxis()->GetBinLabel(ib+1);
         std::string hfullname = per_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
         if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
            hfullname = per_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
         }
         if( ib== 0 ) tmpHist = (TH1D*)TTbar_file->Get(hfullname.c_str());
         else tmpHist->Add( (TH1D*)TTbar_file->Get(hfullname.c_str()) );
      }
      cached_h1Vec[is].push_back((TH1D*)tmpHist);
   }

   h1_searchBinYields_topW = (TH1D*) TTbar_file->Get("TTbarNoHad_h1_searchBinYields")->Clone("searchBinYields_topW"); h1_searchBinYields_topW->SetTitle("search bin yields for ttbar+W+single top");
   h1_searchBinYields_HadTau_topW = (TH1D*) TTbar_file->Get("TTbarNoHad_h1_searchBinYields_HadTau")->Clone("searchBinYields_HadTau_topW"); h1_searchBinYields_HadTau_topW->SetTitle("HadTau: search bin yields for ttbar+W+single top");
   h1_searchBinYields_LostLep_topW = (TH1D*) TTbar_file->Get("TTbarNoHad_h1_searchBinYields_LostLep")->Clone("searchBinYields_LostLep_topW"); h1_searchBinYields_LostLep_topW->SetTitle("LostLep: search bin yields for ttbar+W+single top");
   h1_searchBinYields_Overlap_topW = (TH1D*) TTbar_file->Get("TTbarNoHad_h1_searchBinYields_Overlap")->Clone("searchBinYields_Overlap_topW"); h1_searchBinYields_Overlap_topW->SetTitle("Overlap: search bin yields for ttbar+W+single top");
/*
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTbar_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = TTbar_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)TTbar_file->Get(hfullname.c_str()));
   }
*/    

   TFile * WJetsToLNu_file = new TFile("basicCheck_WJetsToLNu.root");
   TH1D * WJetsToLNu_h1_keyString = (TH1D*) WJetsToLNu_file->Get("h1_keyString"); WJetsToLNu_h1_keyString->LabelsDeflate();
   std::string WJetsToLNu_sampleKey = WJetsToLNu_h1_keyString->GetXaxis()->GetBinLabel(1); int WJetsToLNu_color = allCollections[WJetsToLNu_sampleKey].front().color;
   cached_sampleStrVec.push_back(WJetsToLNu_sampleKey); cached_sampleColorVec.push_back(WJetsToLNu_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = WJetsToLNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = WJetsToLNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)WJetsToLNu_file->Get(hfullname.c_str()));
   }

   h1_searchBinYields_topW->Add((TH1D*)WJetsToLNu_file->Get("WJetsToLNu_h1_searchBinYields"));
   h1_searchBinYields_HadTau_topW->Add((TH1D*)WJetsToLNu_file->Get("WJetsToLNu_h1_searchBinYields_HadTau"));
   h1_searchBinYields_LostLep_topW->Add((TH1D*)WJetsToLNu_file->Get("WJetsToLNu_h1_searchBinYields_LostLep"));
   h1_searchBinYields_Overlap_topW->Add((TH1D*)WJetsToLNu_file->Get("WJetsToLNu_h1_searchBinYields_Overlap"));

   TFile * tW_file = new TFile("basicCheck_tW.root");
   TH1D * tW_h1_keyString = (TH1D*) tW_file->Get("h1_keyString"); tW_h1_keyString->LabelsDeflate();
   std::string tW_sampleKey = tW_h1_keyString->GetXaxis()->GetBinLabel(1); int tW_color = allCollections[tW_sampleKey].front().color;
   cached_sampleStrVec.push_back(tW_sampleKey); cached_sampleColorVec.push_back(tW_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = tW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = tW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)tW_file->Get(hfullname.c_str()));
   }

   h1_searchBinYields_topW->Add((TH1D*)tW_file->Get("tW_h1_searchBinYields"));
   h1_searchBinYields_HadTau_topW->Add((TH1D*)tW_file->Get("tW_h1_searchBinYields_HadTau"));
   h1_searchBinYields_LostLep_topW->Add((TH1D*)tW_file->Get("tW_h1_searchBinYields_LostLep"));
   h1_searchBinYields_Overlap_topW->Add((TH1D*)tW_file->Get("tW_h1_searchBinYields_Overlap"));

   TFile * ZJetsToNuNu_file = new TFile("basicCheck_ZJetsToNuNu.root");
   TH1D * ZJetsToNuNu_h1_keyString = (TH1D*) ZJetsToNuNu_file->Get("h1_keyString"); ZJetsToNuNu_h1_keyString->LabelsDeflate();
   std::string ZJetsToNuNu_sampleKey = ZJetsToNuNu_h1_keyString->GetXaxis()->GetBinLabel(1); int ZJetsToNuNu_color = allCollections[ZJetsToNuNu_sampleKey].front().color;
   cached_sampleStrVec.push_back(ZJetsToNuNu_sampleKey); cached_sampleColorVec.push_back(ZJetsToNuNu_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = ZJetsToNuNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = ZJetsToNuNu_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)ZJetsToNuNu_file->Get(hfullname.c_str()));
   }
    
   TFile * QCD_file = new TFile("basicCheck_QCD.root");
   TH1D * QCD_h1_keyString = (TH1D*) QCD_file->Get("h1_keyString"); QCD_h1_keyString->LabelsDeflate();
   std::string QCD_sampleKey = QCD_h1_keyString->GetXaxis()->GetBinLabel(1); int QCD_color = allCollections[QCD_sampleKey].front().color;
   cached_sampleStrVec.push_back(QCD_sampleKey); cached_sampleColorVec.push_back(QCD_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = QCD_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = QCD_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)QCD_file->Get(hfullname.c_str()));
   }
    
   TFile * TTZ_file = new TFile("basicCheck_TTZ.root");
   TH1D * TTZ_h1_keyString = (TH1D*) TTZ_file->Get("h1_keyString"); TTZ_h1_keyString->LabelsDeflate();
   std::string TTZ_sampleKey = TTZ_h1_keyString->GetXaxis()->GetBinLabel(1); int TTZ_color = allCollections[TTZ_sampleKey].front().color;
   cached_sampleStrVec.push_back(TTZ_sampleKey); cached_sampleColorVec.push_back(TTZ_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTZ_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = TTZ_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)TTZ_file->Get(hfullname.c_str()));
   }
    
   TFile * TTW_file = new TFile("basicCheck_TTW.root");
   TH1D * TTW_h1_keyString = (TH1D*) TTW_file->Get("h1_keyString"); TTW_h1_keyString->LabelsDeflate();
   std::string TTW_sampleKey = TTW_h1_keyString->GetXaxis()->GetBinLabel(1); int TTW_color = allCollections[TTW_sampleKey].front().color;
//   cached_sampleStrVec.push_back(TTW_sampleKey); cached_sampleColorVec.push_back(TTW_color);
   cached_sampleStrVec.push_back("rare"); cached_sampleColorVec.push_back(TTW_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = TTW_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)TTW_file->Get(hfullname.c_str()));
   }
    
// Summed to TTW
   TFile * Diboson_file = new TFile("basicCheck_Diboson.root");
   TH1D * Diboson_h1_keyString = (TH1D*) Diboson_file->Get("h1_keyString"); Diboson_h1_keyString->LabelsDeflate();
   std::string Diboson_sampleKey = Diboson_h1_keyString->GetXaxis()->GetBinLabel(1); int Diboson_color = allCollections[Diboson_sampleKey].front().color;
//   cached_sampleStrVec.push_back(Diboson_sampleKey); cached_sampleColorVec.push_back(Diboson_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = Diboson_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = Diboson_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
//      cached_h1Vec[is].push_back((TH1D*)Diboson_file->Get(hfullname.c_str()));
      cached_h1Vec[is].back()->Add((TH1D*)Diboson_file->Get(hfullname.c_str()));
   }

// Summed to TTW    
   TFile * Triboson_file = new TFile("basicCheck_Triboson.root");
   TH1D * Triboson_h1_keyString = (TH1D*) Triboson_file->Get("h1_keyString"); Triboson_h1_keyString->LabelsDeflate();
   std::string Triboson_sampleKey = Triboson_h1_keyString->GetXaxis()->GetBinLabel(1); int Triboson_color = allCollections[Triboson_sampleKey].front().color;
//   cached_sampleStrVec.push_back(Triboson_sampleKey); cached_sampleColorVec.push_back(Triboson_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = Triboson_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = Triboson_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
//      cached_h1Vec[is].push_back((TH1D*)Triboson_file->Get(hfullname.c_str()));
      cached_h1Vec[is].back()->Add((TH1D*)Triboson_file->Get(hfullname.c_str()));
   }

/*
   TFile * TTX_file = new TFile("basicCheck_TTX.root");
   TH1D * TTX_h1_keyString = (TH1D*) TTX_file->Get("h1_keyString"); TTX_h1_keyString->LabelsDeflate();
   std::string TTX_sampleKey = TTX_h1_keyString->GetXaxis()->GetBinLabel(1); int TTX_color = (int)TTX_h1_keyString->GetBinContent(1);
   cached_sampleStrVec.push_back(TTX_sampleKey); cached_sampleColorVec.push_back(TTX_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = TTX_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      cached_h1Vec[is].push_back((TH1D*)TTX_file->Get(hfullname.c_str()));
   }
*/
   std::string dataFileName = "basicCheck_Data_"+datatype+".root";
   TFile * Data_HTMHT25ns_file = new TFile(dataFileName.c_str());
   TH1D * Data_HTMHT25ns_h1_keyString = (TH1D*) Data_HTMHT25ns_file->Get("h1_keyString"); Data_HTMHT25ns_h1_keyString->LabelsDeflate();
   std::string Data_HTMHT25ns_sampleKey = Data_HTMHT25ns_h1_keyString->GetXaxis()->GetBinLabel(1); int Data_HTMHT25ns_color = (int)Data_HTMHT25ns_h1_keyString->GetBinContent(1);
   cached_sampleStrVec.push_back(Data_HTMHT25ns_sampleKey); cached_sampleColorVec.push_back(Data_HTMHT25ns_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = Data_HTMHT25ns_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = Data_HTMHT25ns_sampleKey+"_h1_"+todraw_h1_keyStrVec[is];
      }
      cached_h1Vec[is].push_back((TH1D*)Data_HTMHT25ns_file->Get(hfullname.c_str()));
   }

// Signals
   TFile * signal_T2tt_mStop850_mLSP100_file = new TFile("basicCheck_Signal_T2tt_mStop850_mLSP100.root");
   TH1D * signal_T2tt_mStop850_mLSP100_h1_keyString = (TH1D*) signal_T2tt_mStop850_mLSP100_file->Get("h1_keyString"); signal_T2tt_mStop850_mLSP100_h1_keyString->LabelsDeflate();
   std::string signal_T2tt_mStop850_mLSP100_sampleKey = signal_T2tt_mStop850_mLSP100_h1_keyString->GetXaxis()->GetBinLabel(1); int signal_T2tt_mStop850_mLSP100_color = allCollections[signal_T2tt_mStop850_mLSP100_sampleKey].front().color;
   cached_sampleStrVec.push_back(signal_T2tt_mStop850_mLSP100_sampleKey); cached_sampleColorVec.push_back(signal_T2tt_mStop850_mLSP100_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = signal_T2tt_mStop850_mLSP100_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = signal_T2tt_mStop850_mLSP100_sampleKey + "_h1_" + todraw_h1_keyStrVec[is]; 
      }
      cached_h1Vec[is].push_back((TH1D*)signal_T2tt_mStop850_mLSP100_file->Get(hfullname.c_str()));
   }
   
   TFile * signal_T2tt_mStop500_mLSP325_file = new TFile("basicCheck_Signal_T2tt_mStop500_mLSP325.root");
   TH1D * signal_T2tt_mStop500_mLSP325_h1_keyString = (TH1D*) signal_T2tt_mStop500_mLSP325_file->Get("h1_keyString"); signal_T2tt_mStop500_mLSP325_h1_keyString->LabelsDeflate();
   std::string signal_T2tt_mStop500_mLSP325_sampleKey = signal_T2tt_mStop500_mLSP325_h1_keyString->GetXaxis()->GetBinLabel(1); int signal_T2tt_mStop500_mLSP325_color = allCollections[signal_T2tt_mStop500_mLSP325_sampleKey].front().color;
   cached_sampleStrVec.push_back(signal_T2tt_mStop500_mLSP325_sampleKey); cached_sampleColorVec.push_back(signal_T2tt_mStop500_mLSP325_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = signal_T2tt_mStop500_mLSP325_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = signal_T2tt_mStop500_mLSP325_sampleKey + "_h1_" + todraw_h1_keyStrVec[is]; 
      }
      cached_h1Vec[is].push_back((TH1D*)signal_T2tt_mStop500_mLSP325_file->Get(hfullname.c_str()));
   }
   
    
   TFile * signal_T1tttt_mGluino1500_mLSP100_file = new TFile("basicCheck_Signal_T1tttt_mGluino1500_mLSP100.root");
   TH1D * signal_T1tttt_mGluino1500_mLSP100_h1_keyString = (TH1D*) signal_T1tttt_mGluino1500_mLSP100_file->Get("h1_keyString"); signal_T1tttt_mGluino1500_mLSP100_h1_keyString->LabelsDeflate();
   std::string signal_T1tttt_mGluino1500_mLSP100_sampleKey = signal_T1tttt_mGluino1500_mLSP100_h1_keyString->GetXaxis()->GetBinLabel(1); int signal_T1tttt_mGluino1500_mLSP100_color = allCollections[signal_T1tttt_mGluino1500_mLSP100_sampleKey].front().color;
   cached_sampleStrVec.push_back(signal_T1tttt_mGluino1500_mLSP100_sampleKey); cached_sampleColorVec.push_back(signal_T1tttt_mGluino1500_mLSP100_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = signal_T1tttt_mGluino1500_mLSP100_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = signal_T1tttt_mGluino1500_mLSP100_sampleKey + "_h1_" + todraw_h1_keyStrVec[is]; 
      }
      cached_h1Vec[is].push_back((TH1D*)signal_T1tttt_mGluino1500_mLSP100_file->Get(hfullname.c_str()));
   }
   
    
   TFile * signal_T1tttt_mGluino1200_mLSP800_file = new TFile("basicCheck_Signal_T1tttt_mGluino1200_mLSP800.root");
   TH1D * signal_T1tttt_mGluino1200_mLSP800_h1_keyString = (TH1D*) signal_T1tttt_mGluino1200_mLSP800_file->Get("h1_keyString"); signal_T1tttt_mGluino1200_mLSP800_h1_keyString->LabelsDeflate();
   std::string signal_T1tttt_mGluino1200_mLSP800_sampleKey = signal_T1tttt_mGluino1200_mLSP800_h1_keyString->GetXaxis()->GetBinLabel(1); int signal_T1tttt_mGluino1200_mLSP800_color = allCollections[signal_T1tttt_mGluino1200_mLSP800_sampleKey].front().color;
   cached_sampleStrVec.push_back(signal_T1tttt_mGluino1200_mLSP800_sampleKey); cached_sampleColorVec.push_back(signal_T1tttt_mGluino1200_mLSP800_color);
   for(unsigned int is=0; is<todraw_h1_keyStrVec.size(); is++){
      std::string hfullname = signal_T1tttt_mGluino1200_mLSP800_sampleKey+"_h1_"+todraw_h1_keyStrVec[is]+"_"+cutLev;
      if( todraw_h1_keyStrVec[is]=="cutFlow" || todraw_h1_keyStrVec[is] == "searchBinYields" ){
         hfullname = signal_T1tttt_mGluino1200_mLSP800_sampleKey + "_h1_" + todraw_h1_keyStrVec[is]; 
      }
      cached_h1Vec[is].push_back((TH1D*)signal_T1tttt_mGluino1200_mLSP800_file->Get(hfullname.c_str()));
   }
    

   TCanvas *cs = new TCanvas("cs", "cs", 1200, 900);
   int divW=3, divH=3;
   cs->Divide(divW, divH);

   TCanvas *ct = new TCanvas("ct", "ct", 800, 600);

   Float_t legendX1 = .60;
   Float_t legendX2 = .80;
   Float_t legendY1 = .60;
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
   tdrStyle->SetTitleFontSize(0.3);
   tdrStyle->SetTitleAlign(13);
   tdrStyle->SetTitleX(0.6);
   tdrStyle->SetTitleH(0.05);
   tdrStyle->SetTitleBorderSize(0);
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
 
   tdrStyle->SetPadTopMargin(0.1); tdrStyle->SetPadBottomMargin(0.20);
   tdrStyle->SetPadLeftMargin(0.20); tdrStyle->SetPadRightMargin(0.05);
 
//   tdrStyle->SetOptStat(1111);
 
   tdrStyle->SetHistLineWidth(1);
 
   tdrStyle->SetPaintTextFormat("4.2f");

   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

// tmp_hs_sum_nJets used for auto scale
   double scaleToSameYield = 1.0;
   for(unsigned int ip=0; ip<todraw_h1_keyStrVec.size(); ip++){
      if( todraw_h1_keyStrVec[ip] != "nJets" ) continue;

      THStack * tmp_hs_sum_nJets = new THStack("tmp_hs_sum_nJets", "");
      TH1D * tmp_data = 0;

      for(unsigned int is=0; is<cached_h1Vec[ip].size(); is++){
         TH1D * tmp_any = (TH1D*) cached_h1Vec[ip][is]->Clone();

         if( cached_sampleStrVec[is].find("Signal") != std::string::npos ) continue;

         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            tmp_data = (TH1D*)tmp_any->Clone();
            tmp_data->Rebin(todraw_h1_rebinVec[ip]);
            continue;
         }
         tmp_any->Rebin(todraw_h1_rebinVec[ip]);
         tmp_any->Scale(scaleMCtoData);
         tmp_hs_sum_nJets->Add(tmp_any);
      }

      TH1D * last = (TH1D*)tmp_hs_sum_nJets->GetStack()->Last();
//      scaleToSameYield = tmp_data->Integral()/last->Integral();
      std::cout<<"\ndata : "<<tmp_data->Integral()<<"  MC : "<<last->Integral()<<"  scaleToSameYield : "<<scaleToSameYield<<std::endl<<std::endl;
   }


// Search bin yields print out
   int idxSB = -1;
   for(unsigned int ik=0; ik<todraw_h1_keyStrVec.size(); ik++){
      if( todraw_h1_keyStrVec[ik] == "searchBinYields" ) idxSB = (int) ik;
   }

   std::cout<<"\nSearch bin yields : "<<std::endl;
   std::cout<<"  "<<std::endl;
   for(unsigned int is=0; is<cached_sampleStrVec.size(); is++){
      printf("  %20s", cached_sampleStrVec[is].c_str());
   }
   printf("  %20s\n", "sumSM");
   for(unsigned int ib=0; ib<cached_h1Vec[idxSB][0]->GetXaxis()->GetNbins(); ib++){
      printf("%2d", ib);
      double sumSM = 0, sumSMerr = 0;
      for(unsigned int is=0; is<cached_h1Vec[idxSB].size(); is++){
         if( cached_sampleStrVec[is].find("Data") == std::string::npos && cached_sampleStrVec[is].find("Signal") == std::string::npos ){
            sumSM += cached_h1Vec[idxSB][is]->GetBinContent(ib+1)*scaleMCtoData;
            sumSMerr += (cached_h1Vec[idxSB][is]->GetBinError(ib+1)*scaleMCtoData * cached_h1Vec[idxSB][is]->GetBinError(ib+1)*scaleMCtoData);
         }
      }
      sumSMerr = sqrt(sumSMerr);

      for(unsigned int is=0; is<cached_h1Vec[idxSB].size(); is++){
         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            printf("  %6.3f +- %5.3f", cached_h1Vec[idxSB][is]->GetBinContent(ib+1), cached_h1Vec[idxSB][is]->GetBinError(ib+1));
         }else{
            printf("  %6.3f +- %5.3f (%2.0f\%)", cached_h1Vec[idxSB][is]->GetBinContent(ib+1)*scaleMCtoData, cached_h1Vec[idxSB][is]->GetBinError(ib+1)*scaleMCtoData, cached_h1Vec[idxSB][is]->GetBinContent(ib+1)*scaleMCtoData/sumSM*100);
         }
      }
      printf("  %7.3f  +- %7.3f", sumSM, sumSMerr);
      std::cout<<std::endl;
   }
   std::cout<<std::endl;

   std::ofstream ofs;
   ofs.open("data.txt", std::ofstream::out);
   ofs<<"luminosity = "<<dataLumi<<std::endl;
   ofs<<"channels = "<<nTotBins<<std::endl;
   ofs<<"sample = signal"<<std::endl;

   char tmpStr[200], tmpStr2[200];

   ofs<<"\nchannel     = ";
   for(unsigned int ib=0; ib<nTotBins; ib++){
      sprintf(tmpStr2, "bin%d", ib+1);
      sprintf(tmpStr, "%10s", tmpStr2);
      ofs<<tmpStr;
   }
   ofs<<std::endl;

   ofs<<"\nrate        = ";
   for(unsigned int ib=0; ib<nTotBins; ib++){
      for(unsigned int is=0; is<cached_h1Vec[idxSB].size(); is++){
         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            sprintf(tmpStr, "%10.0f", cached_h1Vec[idxSB][is]->GetBinContent(ib+1));
            ofs<<tmpStr;
         }
      }
   }
   ofs<<std::endl;

   ofs.close();

// Break down ttbar+W+single top to had. tau, lost lept and overlap
   std::cout<<"\nSearch bin yields break down for ttbar+W+single top : "<<std::endl;
   std::cout<<"  "<<std::endl;
   printf("  %17s", "total");
   printf("  %17s", "hadronic tau");
   printf("  %17s", "lost lepton");
   printf("  %17s\n", "overlap");
   for(unsigned int ib=0; ib<h1_searchBinYields_topW->GetXaxis()->GetNbins(); ib++){
      printf("%2d", ib);
      printf("  %6.3f +- %5.3f", h1_searchBinYields_topW->GetBinContent(ib+1)*scaleMCtoData, h1_searchBinYields_topW->GetBinError(ib+1)*scaleMCtoData);
      printf("  %6.3f +- %5.3f (%2.0f\%)", h1_searchBinYields_HadTau_topW->GetBinContent(ib+1)*scaleMCtoData, h1_searchBinYields_HadTau_topW->GetBinError(ib+1)*scaleMCtoData, h1_searchBinYields_HadTau_topW->GetBinContent(ib+1)/h1_searchBinYields_topW->GetBinContent(ib+1)*100);
      printf("  %6.3f +- %5.3f (%2.0f\%)", h1_searchBinYields_LostLep_topW->GetBinContent(ib+1)*scaleMCtoData, h1_searchBinYields_LostLep_topW->GetBinError(ib+1)*scaleMCtoData, h1_searchBinYields_LostLep_topW->GetBinContent(ib+1)/h1_searchBinYields_topW->GetBinContent(ib+1)*100);
      printf("  %6.3f +- %5.3f (%2.0f\%)", h1_searchBinYields_Overlap_topW->GetBinContent(ib+1)*scaleMCtoData, h1_searchBinYields_Overlap_topW->GetBinError(ib+1)*scaleMCtoData, h1_searchBinYields_Overlap_topW->GetBinContent(ib+1)/h1_searchBinYields_topW->GetBinContent(ib+1)*100);
      std::cout<<std::endl;
   }
   std::cout<<std::endl;

// Cut flow print out
   int idxCF = -1;
   for(unsigned int ik=0; ik<todraw_h1_keyStrVec.size(); ik++){
      if( todraw_h1_keyStrVec[ik] == "cutFlow" ) idxCF = (int) ik;
   }

   std::cout<<"\n\nCut flow yields : "<<std::endl;
   printf("%20s", " ");
   for(unsigned int is=0; is<cached_sampleStrVec.size(); is++){
      printf("  %28s", cached_sampleStrVec[is].c_str());
   }
   cached_h1Vec[idxCF][0]->LabelsDeflate();
   printf("  %20s\n", "sumSM");
   double pre_sumSM = 0;
   for(unsigned int ib=0; ib<cached_h1Vec[idxCF][0]->GetXaxis()->GetNbins(); ib++){
      printf("%20s", cached_h1Vec[idxCF][0]->GetXaxis()->GetBinLabel(ib+1));
      double sumSM = 0, sumSMerr = 0;
      for(unsigned int is=0; is<cached_h1Vec[idxCF].size(); is++){
         if( cached_sampleStrVec[is].find("Data") == std::string::npos && cached_sampleStrVec[is].find("Signal") == std::string::npos ){
            sumSM += cached_h1Vec[idxCF][is]->GetBinContent(ib+1)*scaleMCtoData;
            sumSMerr += (cached_h1Vec[idxCF][is]->GetBinError(ib+1)*scaleMCtoData * cached_h1Vec[idxCF][is]->GetBinError(ib+1)*scaleMCtoData);
         }
      }
      sumSMerr = sqrt(sumSMerr);
      if( ib == 0 ) pre_sumSM = sumSM;

      for(unsigned int is=0; is<cached_h1Vec[idxCF].size(); is++){
         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            double preVal = ib ==0 ? cached_h1Vec[idxCF][is]->GetBinError(ib+1) : cached_h1Vec[idxCF][is]->GetBinError(ib);
            printf("  %6.3e +- %5.3e (%3.0f\%)", cached_h1Vec[idxCF][is]->GetBinContent(ib+1), cached_h1Vec[idxCF][is]->GetBinError(ib+1), cached_h1Vec[idxCF][is]->GetBinError(ib+1)/preVal*100);
         }else{
            double preVal = ib ==0 ? cached_h1Vec[idxCF][is]->GetBinContent(ib+1) : cached_h1Vec[idxCF][is]->GetBinContent(ib);
            printf("  %6.3e +- %5.3e (%3.0f\%)", cached_h1Vec[idxCF][is]->GetBinContent(ib+1)*scaleMCtoData, cached_h1Vec[idxCF][is]->GetBinError(ib+1)*scaleMCtoData, cached_h1Vec[idxCF][is]->GetBinContent(ib+1)/preVal*100);
         }
      }
      printf("  %7.3e  +- %7.3e (%3.0f\%)", sumSM, sumSMerr, sumSM/pre_sumSM*100);
      std::cout<<std::endl;
      
      pre_sumSM = sumSM;
   }
   std::cout<<std::endl;

   for(unsigned int ip=0; ip<todraw_h1_keyStrVec.size(); ip++){
      ct->cd(); catLeg1->Clear();
      TPad *pad1 = new TPad("pad1", "pad1", 0, 0.30, 1, 1.0);
      pad1->SetBottomMargin(0);
      pad1->Draw();             // Draw the upper pad: pad1                                                        
      pad1->cd();

      if( todraw_h1_keyStrVec[ip] == "met" || todraw_h1_keyStrVec[ip] == "HT" || todraw_h1_keyStrVec[ip] == "allJetPt" || todraw_h1_keyStrVec[ip] == "leadJetPt" || todraw_h1_keyStrVec[ip] == "muPt" || todraw_h1_keyStrVec[ip] == "searchBinYields" ) 
      pad1->SetLogy();
      else pad1->SetLogy(0);

      THStack * hs_sum_SM = new THStack("hs", "");
      TH1D * tmp_data = 0;
      for(unsigned int js=0; js<cached_h1Vec[ip].size(); js++){

         unsigned int is = cached_h1Vec[ip].size() - 1 - js;

         if( cached_sampleStrVec[is].find("Signal") != std::string::npos ) continue;

         TH1D * tmp_any = (TH1D*) cached_h1Vec[ip][is]->Clone();

         if( cached_sampleStrVec[is].find("Data") != std::string::npos ){
            tmp_data = (TH1D*)tmp_any->Clone();
            tmp_data->Rebin(todraw_h1_rebinVec[ip]);
            catLeg1->AddEntry(tmp_data, cached_sampleStrVec[is].c_str());
            continue;
         }

         tmp_any->Rebin(todraw_h1_rebinVec[ip]);
         if( todraw_h1_keyStrVec[ip] == "searchBinYields" ) tmp_any->Scale(scaleMCtoData);
         else tmp_any->Scale(scaleMCtoData*scaleToSameYield);
         tmp_any->SetFillColor(cached_sampleColorVec[is]); tmp_any->SetLineColor(cached_sampleColorVec[is]); tmp_any->SetMarkerColor(cached_sampleColorVec[is]);
         hs_sum_SM->Add(tmp_any);
         catLeg1->AddEntry(tmp_any, cached_sampleStrVec[is].c_str());
      }
      tmp_data->GetXaxis()->SetTitle(todraw_h1_xLabelVec[ip].c_str());
      if( todraw_h1_keyStrVec[ip] == "HT" ) tmp_data->GetXaxis()->SetRangeUser(400, tmp_data->GetXaxis()->GetXmax());
      if( todraw_h1_keyStrVec[ip] == "topMass" ) tmp_data->GetXaxis()->SetRangeUser(50, tmp_data->GetXaxis()->GetXmax());
      tmp_data->Draw("");
//      if( todraw_h1_keyStrVec[ip] == "searchBinYields" ){ tmp_data->SetMaximum(tmp_data->GetMaximum()*10.0); tmp_data->SetMinimum(5e-2); }
      if( todraw_h1_keyStrVec[ip] == "searchBinYields" ){ tmp_data->SetMaximum(ymax_Yields); tmp_data->SetMinimum(ymin_Yields); }
      else tmp_data->SetMaximum(tmp_data->GetMaximum()*2.5);

      hs_sum_SM->Draw("hist same");
      hs_sum_SM->SetMaximum(hs_sum_SM->GetMaximum()*2.5);
      if( todraw_h1_keyStrVec[ip] == "HT" ) hs_sum_SM->GetXaxis()->SetRangeUser(400, hs_sum_SM->GetXaxis()->GetXmax());
      if( todraw_h1_keyStrVec[ip] == "topMass" ) hs_sum_SM->GetXaxis()->SetRangeUser(50, hs_sum_SM->GetXaxis()->GetXmax());

      tmp_data->Draw("same");

      pad1->RedrawAxis();

      if( todraw_h1_keyStrVec[ip] == "searchBinYields" ) sb->drawSBregionDef(ymin_Yields, ymax_Yields);

      catLeg1->SetFillColor(kWhite);
      catLeg1->SetBorderSize(0);
      catLeg1->Draw();

      TH1D * h1_ratio = (TH1D*) tmp_data->Clone();
      TH1D * tmp_sum_SM = (TH1D*) hs_sum_SM->GetStack()->Last();
      h1_ratio->Divide(tmp_sum_SM);

      ct->cd();
      TPad *pad2 = new TPad("pad2", "pad2", 0, 0.03, 1, 0.30);
      pad2->SetTopMargin(0);
      pad2->SetBottomMargin(0.30);
      pad2->SetGridy();
      pad2->Draw();
      pad2->cd();       // pad2 becomes the current pad                                                           
      TF1 * fline = new TF1("line", "pol0", h1_ratio->GetBinLowEdge(1), h1_ratio->GetBinLowEdge(h1_ratio->GetNbinsX()) + h1_ratio->GetBinWidth(h1_ratio->GetNbinsX()));
      fline->SetParameter(0, 1);
      h1_ratio->SetTitle(0);
      h1_ratio->SetLineColor(kBlue);
      h1_ratio->SetMinimum(0);  // Define Y ..                                                                      
      h1_ratio->SetMaximum(2); // .. range                                                                          
      h1_ratio->SetStats(0);

      // Y axis ratio plot settings                                                                               
      h1_ratio->GetYaxis()->SetTitle("Data/MC");
      h1_ratio->GetYaxis()->SetNdivisions(505);
      h1_ratio->GetYaxis()->SetTitleSize(18);
      h1_ratio->GetYaxis()->SetTitleFont(43);
      h1_ratio->GetYaxis()->SetTitleOffset(1.25);
      h1_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)                          
      h1_ratio->GetYaxis()->SetLabelSize(15);

      // X axis ratio plot settings                                                                               
      h1_ratio->GetXaxis()->SetTitleSize(18);
      h1_ratio->GetXaxis()->SetTitleFont(43);
      h1_ratio->GetXaxis()->SetTitleOffset(3.5);
      h1_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)                          
      h1_ratio->GetXaxis()->SetLabelSize(15);
      h1_ratio->Draw("E1");
      fline->Draw("same");

      ct->Print("comb_"+TString(todraw_h1_keyStrVec[ip])+"_"+TString(cutLev)+".pdf");
   }
}

int main(int argc, char *argv[]){

   sb = new SearchBins();
   nTotBins = sb->nSearchBins();
   sb->build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);
   sb->print_searchBins();

   std::string cutLev = "baseline";
   std::string datatype = "HTMHT";
   if( argc >= 2 ) cutLev = argv[1];
   if( argc >= 3 ) datatype = argv[2];
   std::cout<<"\ncutLev : "<<cutLev.c_str()<<"  datatype : "<<datatype.c_str()<<std::endl;
   makeCombPlots(cutLev, datatype);
}
