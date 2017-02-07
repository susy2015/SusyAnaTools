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

#include "TGraphAsymmErrors.h"

#include "searchBins.h"

#include "xSec.h"

bool do37Bins = false;

SearchBins * sb =0;
int nTotBins;
std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

bool noobs = false;
bool addStatUnc = true;
bool addSigPts = false;
// 0: T2tt; 1: T1tttt
int addSigOpt = 0;

int nTotBins_loc = 45;

std::vector<TH2Poly*> h2_sumSM_poly_MT2_vs_metVec, h2_sumSM_poly_muonCSforLostLept_MT2_vs_metVec;
std::vector<TH2Poly*> h2_sumSM_poly_0bZnunu_MT2_vs_metVec;

std::vector<TH1D*> h1_cutFlowVec;

char names[200], dispt[200];

TStopwatch timer;

/*
const double ymax_Yields = 20000.;
const double ymin_Yields = 0.05;
const double ymax_Ratio=12.0;
const double ymin_Ratio=0.2;
*/
/* For 37 bins
const double ymax_Yields = 200000.;
const double ymin_Yields = 0.03;
const double ymax_Ratio= 4.6;
const double ymin_Ratio= 0.0;
*/
// For 45 bins
const double ymax_Yields = noobs? 1000000.: 4000000.;
const double ymin_Yields = noobs? 0.005: 0.003;
const double ymax_Ratio=  4.6;
const double ymin_Ratio= 0.0;

const double adjHalfBin = 0.5;
const double deltaY = ymax_Yields - ymin_Yields;

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

std::vector<std::string> todraw_h1_keyStrVec = {"nJets", "nbJets", "nTops", "met", "metphi", "HT", "MT2", "vtxSize", "allJetPt", "allJetM", "leadJetPt", "leadJetM", "topMass", "muPt", "dphi1", "dphi2", "dphi3", "cutFlow", "searchBinYields"};
std::vector<int>         todraw_h1_rebinVec  = {      1,        1,       1,    5,       10,     5,     5,         1,          5,         5,           5,          5,         5,      5,      10,      10,      10,        1,                  1};
std::vector<std::string> todraw_h1_xLabelVec = {"N_{jets}", "N_{b}", "N_{t}", "#slash{E}_{T} (GeV)", "#phi_{E_{T}^{miss}}", "H_{T} (GeV)", "M_{T2} (GeV)", "N_{vtx}", "P_{T}(all jets) (GeV)", "M(all jets) (GeV)", "P_{T}(lead jet) (GeV)", "M(lead jet) (GeV)", "M(top) (GeV)", "P_{T}(#mu)", "#Delta#phi(j_{1}, E_{T}^{miss})", "#Delta#phi(j_{2}, E_{T}^{miss})", "#Delta#phi(j_{3}, E_{T}^{miss})", "cutFlow", "idx(searchBin)"};

std::vector<std::vector<TH1D*> > cached_h1Vec(todraw_h1_keyStrVec.size());
std::vector<std::string> cached_sampleStrVec;
std::vector<int> cached_sampleColorVec;

std::vector<std::string> sel_fastsim_sampleKeyStrVec = {"300_225", "350_150", "350_175", "500_325", "600_200", "650_250", "700_0", "800_300", "850_50"};
std::vector<std::string> sel_fastsim_dispt_sampleKeyStrVec = {"T2tt(300,225)", "T2tt(350,150)", "T2tt(350,175)", "T2tt(500,325)", "T2tt(600,200)", "T2tt(650,250)", "T2tt(700,1)", "T2tt(800,300)", "T2tt(850,50)"};
std::vector<int>         sel_fastsim_momMassVec = {300, 350, 350, 500, 600, 650, 700, 800, 850};
std::vector<int>         sel_fastsim_dauMassVec = {225, 150, 175, 325, 200, 250,   0, 300,  50};
std::vector<int>         sel_fastsim_color_sampleKeyVec = {kRed, kBlue, kGreen+4, kMagenta, kTeal+4, kYellow-7};
std::vector<int>         todraw_h1_fastsim_rebinVec  = {         2,                   2,             1,             1,              5,                  5,                          1};
std::vector<std::string> todraw_h1_fastsim_xLabelVec = {"P_{T}^{gen} (GeV)", "P_{T}^{gen} (GeV)", "N_{b}", "N_{t}", "#slash{E}_{T} (GeV)", "M_{T2} (GeV)", "Search region bin number"};
std::vector<std::string> todraw_h1_fastsim_keyStrVec = {"genTopPt", "baseline_genTopPt", "baseline_nb", "baseline_nt", "baseline_met", "baseline_mt2", "baseline_nSearchBin"};
std::vector<std::vector<TH1D*> > sel_fastsim_h1Vec(todraw_h1_fastsim_keyStrVec.size());

std::vector<std::string> sel_fastsim_T2tb_sampleKeyStrVec = {"350_150", "350_175", "500_325", "600_200", "650_250", "700_0", "700_100", "750_50"};
std::vector<std::string> sel_fastsim_T2tb_dispt_sampleKeyStrVec = {"T2tb(350,150)", "T2tb(350,175)", "T2tb(500,325)", "T2tb(600,200)", "T2tb(650,250)", "T2tb(700,1)", "T2tb(700,100)", "T2tb(750,50)"};
std::vector<int>         sel_fastsim_T2tb_momMassVec = {350, 350, 500, 600, 650, 700, 700, 750};
std::vector<int>         sel_fastsim_T2tb_dauMassVec = {150, 175, 325, 200, 250,   0, 100,  50};
std::vector<std::vector<TH1D*> > sel_fastsim_T2tb_h1Vec(todraw_h1_fastsim_keyStrVec.size());

std::vector<std::string> sel_fastsim_T1tttt_sampleKeyStrVec = {"1200_800", "1400_900", "1500_900", "1650_100"};
std::vector<std::string> sel_fastsim_T1tttt_dispt_sampleKeyStrVec = {"T1tttt(1200,800)", "T1tttt(1400,900)", "T1tttt(1500,900)", "T1tttt(1650,100)"};
std::vector<int>         sel_fastsim_T1tttt_momMassVec = {1200, 1400, 1500, 1650};
std::vector<int>         sel_fastsim_T1tttt_dauMassVec = { 800,  900,  900,  100};
std::vector<std::vector<TH1D*> > sel_fastsim_T1tttt_h1Vec(todraw_h1_fastsim_keyStrVec.size());

std::vector<std::string> sel_fastsim_T5ttcc_sampleKeyStrVec = {"1200_800", "1400_900", "1500_100", "1500_200"};
std::vector<std::string> sel_fastsim_T5ttcc_dispt_sampleKeyStrVec = {"T5ttcc(1200,800)", "T5ttcc(1400,900)", "T5ttcc(1500,100)", "T5ttcc(1500,200)"};
std::vector<int>         sel_fastsim_T5ttcc_momMassVec = {1200, 1400, 1500, 1500};
std::vector<int>         sel_fastsim_T5ttcc_dauMassVec = { 800,  900,  100,  200};
std::vector<std::vector<TH1D*> > sel_fastsim_T5ttcc_h1Vec(todraw_h1_fastsim_keyStrVec.size());

std::vector<std::string> sel_fastsim_T5ttttDM175_sampleKeyStrVec = {"1200_800", "1400_900", "1500_200"};
std::vector<std::string> sel_fastsim_T5ttttDM175_dispt_sampleKeyStrVec = {"T5ttttDM175(1200,800)", "T5ttttDM175(1400,900)", "T5ttttDM175(1500,200)"};
std::vector<int>         sel_fastsim_T5ttttDM175_momMassVec = {1200, 1400, 1500};
std::vector<int>         sel_fastsim_T5ttttDM175_dauMassVec = { 800,  900,  200};
std::vector<std::vector<TH1D*> > sel_fastsim_T5ttttDM175_h1Vec(todraw_h1_fastsim_keyStrVec.size());

TH1D * h1_searchBinYields_topW = 0, * h1_searchBinYields_HadTau_topW = 0, * h1_searchBinYields_LostLep_topW = 0, * h1_searchBinYields_Overlap_topW = 0; 

void makeUnblindPlots(const std::string cutLev="baseline", const std::string datatype="HTMHT"){

//   const double dataLumi = 816.27;
//   const double dataLumi = 5189.90;
   const double dataLumi = 12891.528090802;
//   const double dataLumi = 36352.970569733;
//   const double dataLumi = 5189.90*1.30;
   const double bkgLumi = 36352.970569733;
//   const double dataLumi = 4004.345;
//   const double bkgLumi = 4004.345; 
   const double norm_bkg_to_data = noobs? 1.0 : dataLumi/bkgLumi;

   std::ifstream infile("prt_table.txt");
   int prt_chn;
   double prt_data, prt_data_stat, prt_pred, prt_pred_stat_up, prt_pred_stat_dn, prt_pred_syst_up, prt_pred_syst_dn;
   double prt_lostle_rate, prt_lostle_stat_up, prt_lostle_stat_dn, prt_lostle_syst_up, prt_lostle_syst_dn;
   double prt_hadtau_rate, prt_hadtau_stat_up, prt_hadtau_stat_dn, prt_hadtau_syst_up, prt_hadtau_syst_dn;
   double prt_zinv_rate, prt_zinv_stat_up, prt_zinv_stat_dn, prt_zinv_syst_up, prt_zinv_syst_dn;
   double prt_qcd_rate, prt_qcd_stat_up, prt_qcd_stat_dn, prt_qcd_syst_up, prt_qcd_syst_dn;
   double prt_ttz_rate, prt_ttz_stat_up, prt_ttz_stat_dn, prt_ttz_syst_up, prt_ttz_syst_dn;
   double prt_rare_rate, prt_rare_stat_up, prt_rare_stat_dn, prt_rare_syst_up, prt_rare_syst_dn;
   std::cout<<"\nPrinting out the data vs. prediction table..."<<std::endl;   
   sb->print_searchBins_headerstr(" & Obs. & Sum. Pred. & Lost. Lep. & Had. Tau & Z($\\nu\\nu$)+Jets & QCD & ttZ & rare \\\\");
   std::vector<double> xVec, yVec, xSysErrUpVec, xSysErrDnVec, ySysErrUpVec, ySysErrDnVec;
   std::vector<double> yStatErrUpVec, yStatErrDnVec;
   std::vector<double> ySumErrUpVec, ySumErrDnVec;
   std::vector<double> yRatioVec, yRatioSysErrUpVec, yRatioSysErrDnVec;
   std::vector<double> yRatioSumErrUpVec, yRatioSumErrDnVec;

   double sumObs =0, sumPred =0;

   while(infile >> prt_chn >> prt_data >> prt_data_stat >> prt_pred >> prt_pred_stat_up >> prt_pred_stat_dn >> prt_pred_syst_up >> prt_pred_syst_dn >> prt_lostle_rate >> prt_lostle_stat_up >> prt_lostle_stat_dn >> prt_lostle_syst_up >> prt_lostle_syst_dn >> prt_hadtau_rate >> prt_hadtau_stat_up >> prt_hadtau_stat_dn >> prt_hadtau_syst_up >> prt_hadtau_syst_dn >> prt_zinv_rate >> prt_zinv_stat_up >> prt_zinv_stat_dn >> prt_zinv_syst_up >> prt_zinv_syst_dn >> prt_qcd_rate >> prt_qcd_stat_up >> prt_qcd_stat_dn >> prt_qcd_syst_up >> prt_qcd_syst_dn >> prt_ttz_rate >> prt_ttz_stat_up >> prt_ttz_stat_dn >> prt_ttz_syst_up >> prt_ttz_syst_dn >> prt_rare_rate >> prt_rare_stat_up >> prt_rare_stat_dn >> prt_rare_syst_up >> prt_rare_syst_dn){
      char tmpstr[500];
// Complete table in AN
      sprintf(tmpstr, " & %6.0f & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ \\\\", prt_data, 
                        prt_pred*norm_bkg_to_data, prt_pred_stat_up*norm_bkg_to_data, prt_pred_stat_dn*norm_bkg_to_data, prt_pred_syst_up*norm_bkg_to_data, prt_pred_syst_dn*norm_bkg_to_data, prt_lostle_rate*norm_bkg_to_data, prt_lostle_stat_up*norm_bkg_to_data, prt_lostle_stat_dn*norm_bkg_to_data, prt_lostle_syst_up*norm_bkg_to_data, prt_lostle_syst_dn*norm_bkg_to_data, prt_hadtau_rate*norm_bkg_to_data, prt_hadtau_stat_up*norm_bkg_to_data, prt_hadtau_stat_dn*norm_bkg_to_data, prt_hadtau_syst_up*norm_bkg_to_data, prt_hadtau_syst_dn*norm_bkg_to_data, prt_zinv_rate*norm_bkg_to_data, prt_zinv_stat_up*norm_bkg_to_data, prt_zinv_stat_dn*norm_bkg_to_data, prt_zinv_syst_up*norm_bkg_to_data, prt_zinv_syst_dn*norm_bkg_to_data, prt_qcd_rate*norm_bkg_to_data, prt_qcd_stat_up*norm_bkg_to_data, prt_qcd_stat_dn*norm_bkg_to_data, prt_qcd_syst_up*norm_bkg_to_data, prt_qcd_syst_dn*norm_bkg_to_data, prt_ttz_rate*norm_bkg_to_data, prt_ttz_stat_up*norm_bkg_to_data, prt_ttz_stat_dn*norm_bkg_to_data, prt_ttz_syst_up*norm_bkg_to_data, prt_ttz_syst_dn*norm_bkg_to_data, prt_rare_rate*norm_bkg_to_data, prt_rare_stat_up*norm_bkg_to_data, prt_rare_stat_dn*norm_bkg_to_data, prt_rare_syst_up*norm_bkg_to_data, prt_rare_syst_dn*norm_bkg_to_data);
// for QCD only
//      sprintf(tmpstr, " & %6.3f $^{+%5.3f}_{-%5.3f}$ $^{+%5.3f}_{-%5.3f}$  \\\\", prt_qcd_rate, prt_qcd_stat_up, prt_qcd_stat_dn, prt_qcd_syst_up, prt_qcd_syst_dn);
// for ttZ and rare
//      sprintf(tmpstr, " & %6.3f $^{+%5.3f}_{-%5.3f}$ & %6.3f $^{+%5.3f}_{-%5.3f}$  \\\\", prt_ttz_rate, sqrt(prt_ttz_stat_up*prt_ttz_stat_up + prt_ttz_syst_up*prt_ttz_syst_up), sqrt(prt_ttz_stat_dn*prt_ttz_stat_dn + prt_ttz_syst_dn*prt_ttz_syst_dn), prt_rare_rate, sqrt(prt_rare_stat_up*prt_rare_stat_up + prt_rare_syst_up*prt_rare_syst_up), sqrt(prt_rare_stat_dn*prt_rare_stat_dn + prt_rare_syst_dn*prt_rare_syst_dn) );
// No break down
//      sprintf(tmpstr, " & %6.0f & %6.2f $^{+%4.2f}_{-%4.2f}$ $^{+%4.2f}_{-%4.2f}$ \\\\", prt_data, prt_pred, prt_pred_stat_up, prt_pred_stat_dn, prt_pred_syst_up, prt_pred_syst_dn);
      
      std::string outstr = sb->get_searchBins_defstr(prt_chn, tmpstr);
      printf("%s", outstr.c_str());

      sumObs += prt_data;
      sumPred += prt_pred*norm_bkg_to_data;

      xVec.push_back(prt_chn+0.5);
      yVec.push_back(prt_pred*norm_bkg_to_data);
      xSysErrUpVec.push_back(0.5); xSysErrDnVec.push_back(0.5); 
      ySysErrUpVec.push_back(prt_pred_syst_up*norm_bkg_to_data); ySysErrDnVec.push_back(prt_pred_syst_dn*norm_bkg_to_data);
      yStatErrUpVec.push_back(prt_pred_stat_up*norm_bkg_to_data); yStatErrDnVec.push_back(prt_pred_stat_dn*norm_bkg_to_data);

      const double sumErrUp = sqrt(prt_pred_syst_up*prt_pred_syst_up + prt_pred_stat_up*prt_pred_stat_up);
      const double sumErrDn = sqrt(prt_pred_syst_dn*prt_pred_syst_dn + prt_pred_stat_dn*prt_pred_stat_dn);

      ySumErrUpVec.push_back(sumErrUp*norm_bkg_to_data);
      ySumErrDnVec.push_back(sumErrDn*norm_bkg_to_data);

      yRatioVec.push_back(1.0);
//      yRatioSysErrUpVec.push_back(prt_pred_syst_up/prt_pred*prt_data/prt_pred); yRatioSysErrDnVec.push_back(prt_pred_syst_dn/prt_pred*prt_data/prt_pred);
      yRatioSysErrUpVec.push_back(prt_pred_syst_up/prt_pred); yRatioSysErrDnVec.push_back(prt_pred_syst_dn/prt_pred);
      yRatioSumErrUpVec.push_back(sumErrUp/prt_pred); yRatioSumErrDnVec.push_back(sumErrDn/prt_pred);
   }
   std::cout<<std::endl;
   infile.close();
   std::cout<<"\nsumObs : "<<sumObs<<"  sumPred : "<<sumPred<<std::endl;
   if( addSigPts ){
      TFile * signal_fastsim_file = new TFile("signalScan_SMS-T2tt.root");
      for(unsigned int ik=0; ik<todraw_h1_fastsim_keyStrVec.size(); ik++){
         for(unsigned int is=0; is<sel_fastsim_sampleKeyStrVec.size(); is++){
            std::string hfullname = todraw_h1_fastsim_keyStrVec[ik]+"_"+sel_fastsim_sampleKeyStrVec[is];
   
            double xSec = 0, xSecErr = 0;
            if( xSecMap.find(sel_fastsim_momMassVec[is]) != xSecMap.end() ){
               xSec = xSecMap.find(sel_fastsim_momMassVec[is])->second;
               xSecErr = xSecErrMap.find(sel_fastsim_momMassVec[is])->second * xSec;
            }
   
            char tmpStr[200];
            sprintf(tmpStr, "totEntries_%d_%d", sel_fastsim_momMassVec[is], sel_fastsim_dauMassVec[is]);
            TH1D * h1_totEntries = (TH1D*) signal_fastsim_file->Get(tmpStr);
   
            double totEntries = h1_totEntries->GetBinContent(1);
            double totEntriesErr = sqrt(totEntries);
   
            double lumi_scale = dataLumi*xSec/totEntries;
   
            TH1D * tmpHist = (TH1D*) signal_fastsim_file->Get(hfullname.c_str());
            tmpHist->Scale(lumi_scale);
            sel_fastsim_h1Vec[ik].push_back((TH1D*)tmpHist);
         }
      }
   
   // T1tttt
      TFile * signal_fastsim_T1tttt_file = new TFile("signalScan_SMS-T1tttt.root");
      for(unsigned int ik=0; ik<todraw_h1_fastsim_keyStrVec.size(); ik++){
         for(unsigned int is=0; is<sel_fastsim_T1tttt_sampleKeyStrVec.size(); is++){
            std::string hfullname = todraw_h1_fastsim_keyStrVec[ik]+"_"+sel_fastsim_T1tttt_sampleKeyStrVec[is];
   
            double xSec = 0, xSecErr = 0;
            if( xSecMap_glgl.find(sel_fastsim_T1tttt_momMassVec[is]) != xSecMap_glgl.end() ){
               xSec = xSecMap_glgl.find(sel_fastsim_T1tttt_momMassVec[is])->second;
               xSecErr = xSecErrMap_glgl.find(sel_fastsim_T1tttt_momMassVec[is])->second * xSec;
            }
   
            char tmpStr[200];
            sprintf(tmpStr, "totEntries_%d_%d", sel_fastsim_T1tttt_momMassVec[is], sel_fastsim_T1tttt_dauMassVec[is]);
            TH1D * h1_totEntries = (TH1D*) signal_fastsim_T1tttt_file->Get(tmpStr);
   
            double totEntries = h1_totEntries->GetBinContent(1);
            double totEntriesErr = sqrt(totEntries);
   
            double lumi_scale = dataLumi*xSec/totEntries;
   
            TH1D * tmpHist = (TH1D*) signal_fastsim_T1tttt_file->Get(hfullname.c_str());
            tmpHist->Scale(lumi_scale);
            sel_fastsim_T1tttt_h1Vec[ik].push_back((TH1D*)tmpHist);
         }
      }
   }

   TCanvas *cs = new TCanvas("cs", "cs", 1200, 900);
   int divW=3, divH=3;
   cs->Divide(divW, divH);

   TCanvas *ct = new TCanvas("ct", "ct", 900, 600);

   Float_t legendX1 = .57;
   Float_t legendX2 = .72;
   Float_t legendY1 = .60;
   Float_t legendY2 = .85;
   TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg1); catLeg1 = new TLegend(legendX1+0.10,legendY1+0.15,legendX2,legendY2); }
   catLeg1->SetTextSize(0.060);

   legendX1 = .76;
   legendX2 = .91;
   legendY1 = .60;
   legendY2 = .85;
   TLegend* catLeg2 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg2); catLeg2 = new TLegend(legendX1,legendY1+0.15,legendX2,legendY2); }
   catLeg2->SetTextSize(0.060);

   legendX1 = .36;
   legendX2 = .52;
   legendY1 = .65;
   legendY2 = .73;
   TLegend* catLeg_unc = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg_unc); catLeg_unc = new TLegend(legendX1+0.10,legendY1+0.10,legendX2+0.10,legendY2+0.10); }
   catLeg_unc->SetTextSize(0.050);

   legendX1 = .36;
   legendX2 = .52;
   legendY1 = .52;
   legendY2 = .60;
   TLegend* catLeg_sig1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg_sig1); catLeg_sig1 = new TLegend(legendX1+0.10,legendY1+0.10,legendX2+0.10,legendY2+0.10); }
   catLeg_sig1->SetTextSize(0.045);

   legendX1 = .555;
   legendX2 = .715;
   legendY1 = .52;
   legendY2 = .60;
   TLegend* catLeg_sig2 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg_sig2); catLeg_sig2 = new TLegend(legendX1+0.10,legendY1+0.10,legendX2+0.10,legendY2+0.10); }
   catLeg_sig2->SetTextSize(0.045);

   legendX1 = .74;
   legendX2 = .89;
   legendY1 = .52;
   legendY2 = .60;
   TLegend* catLeg_sig3 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   if( addSigPts ){ delete(catLeg_sig3); catLeg_sig3 = new TLegend(legendX1+0.10,legendY1+0.10,legendX2+0.10,legendY2+0.10); }
   catLeg_sig3->SetTextSize(0.045);

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
 
   tdrStyle->SetPadTopMargin(0.12); tdrStyle->SetPadBottomMargin(0.30);
   tdrStyle->SetPadLeftMargin(0.10); tdrStyle->SetPadRightMargin(0.05);
 
//   tdrStyle->SetOptStat(1111);
 
   tdrStyle->SetHistLineWidth(1);
 
   tdrStyle->SetPaintTextFormat("4.2f");

   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

   TFile * sr_file = new TFile("searchBins.root");

   TH1D * h1_data = (TH1D*) sr_file->Get("data");
   TH1D * h1_lostle = (TH1D*) sr_file->Get("lostle");
   TH1D * h1_hadtau = (TH1D*) sr_file->Get("hadtau");
   TH1D * h1_zinv = (TH1D*) sr_file->Get("zinv");
   TH1D * h1_qcd = (TH1D*) sr_file->Get("qcd");
   TH1D * h1_ttz = (TH1D*) sr_file->Get("ttz");
   TH1D * h1_rare = (TH1D*) sr_file->Get("rare");

   TH1D * h1_lostle_syst = (TH1D*) sr_file->Get("lostle_syst");
   TH1D * h1_hadtau_syst = (TH1D*) sr_file->Get("hadtau_syst");
   TH1D * h1_zinv_syst = (TH1D*) sr_file->Get("zinv_syst");
   TH1D * h1_qcd_syst = (TH1D*) sr_file->Get("qcd_syst");
   TH1D * h1_ttz_syst = (TH1D*) sr_file->Get("ttz_syst");
   TH1D * h1_rare_syst = (TH1D*) sr_file->Get("rare_syst");

   h1_lostle->Scale(norm_bkg_to_data);
   h1_hadtau->Scale(norm_bkg_to_data);
   h1_zinv->Scale(norm_bkg_to_data);
   h1_qcd->Scale(norm_bkg_to_data);
   h1_ttz->Scale(norm_bkg_to_data);
   h1_rare->Scale(norm_bkg_to_data);

   h1_lostle_syst->Scale(norm_bkg_to_data);
   h1_hadtau_syst->Scale(norm_bkg_to_data);
   h1_zinv_syst->Scale(norm_bkg_to_data);
   h1_qcd_syst->Scale(norm_bkg_to_data);
   h1_ttz_syst->Scale(norm_bkg_to_data);
   h1_rare_syst->Scale(norm_bkg_to_data);

   h1_data->SetBinErrorOption(TH1::kPoisson);

   if( do37Bins ) nTotBins_loc = 37;

   h1_data->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_lostle->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_hadtau->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_zinv->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_qcd->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_ttz->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_rare->GetXaxis()->SetRangeUser(0, nTotBins_loc);

   h1_lostle_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_hadtau_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_zinv_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_qcd_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_ttz_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);
   h1_rare_syst->GetXaxis()->SetRangeUser(0, nTotBins_loc);

   h1_ttz->Add(h1_rare);

   ct->cd(); catLeg1->Clear();
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.30, 1, 1.0);
   pad1->SetBottomMargin(0);
   pad1->Draw();             // Draw the upper pad: pad1                                                        
   pad1->cd();

   pad1->SetLogy();

   THStack * hs_sum_SM = new THStack("hs", "");
   THStack * hs2_sum_SM = new THStack("hs2", "");

   // TLegend - starts
   catLeg1->AddEntry(h1_data, "Data");

   h1_ttz->SetFillColor(kTeal-7); h1_ttz->SetLineColor(kTeal-7); h1_ttz->SetMarkerColor(kTeal-7);
   hs_sum_SM->Add(h1_ttz);
   hs2_sum_SM->Add(h1_ttz_syst);

   h1_qcd->SetFillColor(kAzure-4); h1_qcd->SetLineColor(kAzure-4); h1_qcd->SetMarkerColor(kAzure-4);
   hs_sum_SM->Add(h1_qcd);
   hs2_sum_SM->Add(h1_qcd_syst);

   h1_zinv->SetFillColor(kMagenta-2); h1_zinv->SetLineColor(kMagenta-2); h1_zinv->SetMarkerColor(kMagenta-2);
   hs_sum_SM->Add(h1_zinv);
   hs2_sum_SM->Add(h1_zinv_syst);

   h1_hadtau->SetFillColor(kOrange+2); h1_hadtau->SetLineColor(kOrange+2); h1_hadtau->SetMarkerColor(kOrange+2);
   hs_sum_SM->Add(h1_hadtau);
   hs2_sum_SM->Add(h1_hadtau_syst);

   h1_lostle->SetFillColor(kAzure+2); h1_lostle->SetLineColor(kAzure+2); h1_lostle->SetMarkerColor(kAzure+2);
   hs_sum_SM->Add(h1_lostle);
   hs2_sum_SM->Add(h1_lostle_syst);

   if( !addSigPts ){
      catLeg1->AddEntry(h1_lostle, "t#bar{t}/W+jets(e,#mu)");
      catLeg1->AddEntry(h1_hadtau, "t#bar{t}/W+jets(#tau_{had})");
      catLeg2->AddEntry(h1_zinv, "Z(#nu#bar{#nu})+jets");
      catLeg2->AddEntry(h1_qcd, "QCD");
      catLeg2->AddEntry(h1_ttz, "t#bar{t}Z(#nu#bar{#nu})");
   }

   //TLegend - ends
   
   h1_data->SetLineWidth(2); h1_data->SetTitle(""); 
   h1_data->SetMarkerStyle(20); h1_data->SetMarkerSize(0.7); 
   h1_data->SetLineColor(kBlack);

   h1_data->GetXaxis()->SetTitle("Search region bin number");
   h1_data->GetYaxis()->SetTitle("Events");
   h1_data->GetYaxis()->SetTitleSize(0.07);
   h1_data->GetYaxis()->SetLabelSize(0.04);
   h1_data->GetYaxis()->SetTitleOffset(0.6);
   if( !noobs || addSigPts ){
      h1_data->Draw("");
      h1_data->SetMaximum(ymax_Yields); h1_data->SetMinimum(ymin_Yields);

      if( !addSigPts ){
         hs_sum_SM->Draw("hist same");
         hs_sum_SM->SetMaximum(hs_sum_SM->GetMaximum()*2.5);
      }else{
         TH1D * tmp_sum_SM = (TH1D*) hs_sum_SM->GetStack()->Last();
         tmp_sum_SM->SetFillColor(kAzure+2); tmp_sum_SM->SetLineColor(kAzure+2); tmp_sum_SM->SetMarkerColor(kAzure+2);
         tmp_sum_SM->Draw("hist same");
         tmp_sum_SM->SetMaximum(tmp_sum_SM->GetMaximum()*2.5);

         catLeg2->AddEntry(tmp_sum_SM, "Sum Bkg.");
      }

      h1_data->Draw("same");
   }else{
      TH1D * tmp_data = (TH1D*) h1_data->Clone(); tmp_data->Reset();
      tmp_data->Draw("");
      tmp_data->SetMaximum(ymax_Yields); tmp_data->SetMinimum(ymin_Yields);

      hs_sum_SM->Draw("hist same");
   }

   TH1D * tmp_sum_SM = (TH1D*) hs_sum_SM->GetStack()->Last();
   TH1D * tmp2_sum_SM = (TH1D*) hs2_sum_SM->GetStack()->Last();

   for(int ib=0; ib<nTotBins_loc; ib++){
      double pred_cent = tmp_sum_SM->GetBinContent(ib+1);
//      std::cout<<" drt root --> ib : "<<ib<<"  pred_cent : "<<pred_cent<<std::endl;
//      yRatioSysErrUpVec[ib] = ySysErrUpVec[ib]/pred_cent;
//      yRatioSysErrDnVec[ib] = ySysErrDnVec[ib]/pred_cent;
//      yRatioSumErrUpVec[ib] = ySumErrUpVec[ib]/pred_cent;
//      yRatioSumErrDnVec[ib] = ySumErrDnVec[ib]/pred_cent;
   }

   if( addStatUnc ){
      TGraphAsymmErrors * gr_Sum_AsymErr = new TGraphAsymmErrors(xVec.size(), &xVec[0], &yVec[0], &xSysErrDnVec[0], &xSysErrUpVec[0], &ySumErrDnVec[0], &ySumErrUpVec[0]);
      gr_Sum_AsymErr->SetFillColor(kGray+2); gr_Sum_AsymErr->SetFillStyle(3244); gr_Sum_AsymErr->SetLineWidth(1); gr_Sum_AsymErr->SetLineColor(0); gr_Sum_AsymErr->SetMarkerSize(0); gr_Sum_AsymErr->SetMarkerColor(0);
      gr_Sum_AsymErr->Draw("2");

      catLeg_unc->AddEntry(gr_Sum_AsymErr, "Bkg. Stat. Unc.", "F");
   }

   TGraphAsymmErrors * gr_AsymErr = new TGraphAsymmErrors(xVec.size(), &xVec[0], &yVec[0], &xSysErrDnVec[0], &xSysErrUpVec[0], &ySysErrDnVec[0], &ySysErrUpVec[0]);
   gr_AsymErr->SetFillColor(kGray+4); gr_AsymErr->SetFillStyle(3244); gr_AsymErr->SetLineWidth(1); gr_AsymErr->SetLineColor(0); //gr_AsymErr->SetMarkerSize(0); gr_AsymErr->SetMarkerColor(0);
   gr_AsymErr->Draw("2");

   catLeg_unc->AddEntry(gr_AsymErr, "Bkg. Syst. Unc.", "F");

   if( !noobs || addSigPts ){
      h1_data->Draw("same");
   }

// draw signal point here!
   int lineStyleIdx =2, redCntIdx =1;

   if( addSigPts ){
      if( addSigOpt == 0 ){
         for(unsigned int ik=0; ik<sel_fastsim_h1Vec.size(); ik++){
            if( todraw_h1_fastsim_keyStrVec[ik] != "baseline_nSearchBin" ) continue;
            int cntDraw = -1;
            for(unsigned int is=0; is<sel_fastsim_h1Vec[ik].size(); is++){
               if( sel_fastsim_sampleKeyStrVec[is] != "850_50" && sel_fastsim_sampleKeyStrVec[is] != "800_300" ) continue;
      
               drawOverFlowBin(sel_fastsim_h1Vec[ik][is]);
   
               cntDraw++;
      
               sel_fastsim_h1Vec[ik][is]->Rebin(todraw_h1_fastsim_rebinVec[ik]);
               if( cntDraw ==0 ){ sel_fastsim_h1Vec[ik][is]->SetLineColor(kRed+redCntIdx); sel_fastsim_h1Vec[ik][is]->SetMarkerColor(kRed+redCntIdx); }
               if( cntDraw ==1 ){ sel_fastsim_h1Vec[ik][is]->SetLineColor(kGreen+redCntIdx); sel_fastsim_h1Vec[ik][is]->SetMarkerColor(kGreen+redCntIdx); }
               sel_fastsim_h1Vec[ik][is]->SetLineStyle(lineStyleIdx); sel_fastsim_h1Vec[ik][is]->SetLineWidth(2);
               if( lineStyleIdx == 3) sel_fastsim_h1Vec[ik][is]->SetLineStyle(9);
      
               if( sel_fastsim_sampleKeyStrVec[is] == "800_300" ) catLeg_sig1->AddEntry(sel_fastsim_h1Vec[ik][is], "T2tt(800,300)");
               if( sel_fastsim_sampleKeyStrVec[is] == "850_50" ) catLeg_sig2->AddEntry(sel_fastsim_h1Vec[ik][is], "T2tt(850,50)");
      
               sel_fastsim_h1Vec[ik][is]->Draw("hist same");
               lineStyleIdx ++; //redCntIdx ++;
            }
         }
      }
      if( addSigOpt == 1 ){
         for(unsigned int ik=0; ik<sel_fastsim_T1tttt_h1Vec.size(); ik++){
            if( todraw_h1_fastsim_keyStrVec[ik] != "baseline_nSearchBin" ) continue;
            int cntDraw = -1;
            for(unsigned int is=0; is<sel_fastsim_T1tttt_h1Vec[ik].size(); is++){
               if( sel_fastsim_T1tttt_sampleKeyStrVec[is] != "1650_100" && sel_fastsim_T1tttt_sampleKeyStrVec[is] != "1500_900" ) continue;
      
               drawOverFlowBin(sel_fastsim_T1tttt_h1Vec[ik][is]);
      
               cntDraw++;
      
               sel_fastsim_T1tttt_h1Vec[ik][is]->Rebin(todraw_h1_fastsim_rebinVec[ik]);
               if( cntDraw ==0 ){ sel_fastsim_T1tttt_h1Vec[ik][is]->SetLineColor(kRed+redCntIdx); sel_fastsim_T1tttt_h1Vec[ik][is]->SetMarkerColor(kRed+redCntIdx); }
               if( cntDraw ==1 ){ sel_fastsim_T1tttt_h1Vec[ik][is]->SetLineColor(kGreen+redCntIdx); sel_fastsim_T1tttt_h1Vec[ik][is]->SetMarkerColor(kGreen+redCntIdx); }
               sel_fastsim_T1tttt_h1Vec[ik][is]->SetLineStyle(lineStyleIdx); sel_fastsim_T1tttt_h1Vec[ik][is]->SetLineWidth(2);
               if( lineStyleIdx == 3) sel_fastsim_T1tttt_h1Vec[ik][is]->SetLineStyle(9);
      
               if( sel_fastsim_T1tttt_sampleKeyStrVec[is] == "1500_900" ) catLeg_sig1->AddEntry(sel_fastsim_T1tttt_h1Vec[ik][is], "T1tttt(1500,900)");
               if( sel_fastsim_T1tttt_sampleKeyStrVec[is] == "1650_100" ) catLeg_sig2->AddEntry(sel_fastsim_T1tttt_h1Vec[ik][is], "T1tttt(1650,100)");
      
               sel_fastsim_T1tttt_h1Vec[ik][is]->Draw("hist same");
               lineStyleIdx ++; //redCntIdx ++;
            }
         }
      }
   }

   pad1->RedrawAxis();

   sb->drawSBregionDef(ymin_Yields, ymax_Yields, true);
/*
   if( do37Bins ){
//For 37 bins
   //KH drawSBregionDef(ymin_Yields*10, ymax_Yields, true);

      //-----------------------------------------------------------
      // Putting lines and labels explaining search region definitions
      //-----------------------------------------------------------
      // Ntop separation lines
      TLine *tl_ntop = new TLine();
      tl_ntop->SetLineStyle(2);
      tl_ntop->DrawLine(21,ymin_Yields,21,ymax_Yields); 

      // Ntop labels
      TLatex * ttext_ntop = new TLatex();
      ttext_ntop->SetTextFont(42);
      ttext_ntop->SetTextSize(0.06);
      ttext_ntop->SetTextAlign(22);
      ttext_ntop->DrawLatex(10.5, ymax_Yields/2.5 ,"N_{t} = 1");
      ttext_ntop->DrawLatex(29.0, ymax_Yields/2000. ,"N_{t} #geq 2");
            
      // Nb separation lines
      TLine *tl_nb = new TLine();
      tl_nb->SetLineStyle(3);
      // tl_nb->SetLineColor(32);
      tl_nb->DrawLine(11,ymin_Yields,11,ymax_Yields/10.); 
      tl_nb->DrawLine(29,ymin_Yields,29,ymax_Yields/1000.); 

      // Nb labels
      TLatex * ttext2 = new TLatex();
      ttext2->SetTextFont(42);
      //ttext2->SetTextColor(32);
      ttext2->SetTextSize(0.06);
      ttext2->SetTextAlign(22);
      ttext2->Draw();

      ttext2->DrawLatex( 5.5, ymax_Yields/6., "N_{b} = 1");
      ttext2->DrawLatex(16.0, ymax_Yields/6., "N_{b} #geq 2");
      //ttext2->DrawLatex(25.0, ymax_Yields/320., "N_{b} = 1");
      //ttext2->DrawLatex(33.0, ymax_Yields/320., "N_{b} #geq 2");

      // MT2 separation lines
      TLine *tl_mt2 = new TLine();
      tl_mt2->SetLineStyle(4);
      //tl_mt2->SetLineColor(49);
      tl_mt2->DrawLine(4,ymin_Yields,4,ymax_Yields/200.); 
      tl_mt2->DrawLine(8,ymin_Yields,8,ymax_Yields/200.); 
      tl_mt2->DrawLine(15,ymin_Yields,15,ymax_Yields/200.); 
      tl_mt2->DrawLine(19,ymin_Yields,19,ymax_Yields/200.);
      
      tl_mt2->DrawLine(24,ymin_Yields,24,ymax_Yields/2500.); 
      tl_mt2->DrawLine(27,ymin_Yields,27,ymax_Yields/2500.); 
      tl_mt2->DrawLine(32,ymin_Yields,32,ymax_Yields/2500.);
      tl_mt2->DrawLine(35,ymin_Yields,35,ymax_Yields/2500.);

      // MT2 labels
      TLatex * ttextmt2 = new TLatex();
      ttextmt2->SetTextFont(42);
      //ttextmt2->SetTextColor(49);
      ttextmt2->SetTextSize(0.050);
      ttextmt2->SetTextAlign(12);
      ttextmt2->SetTextAngle(90);
      ttextmt2->DrawLatex( 2.5, ymax_Yields/10000. , "M_{T2}=[200,300]");
      ttextmt2->DrawLatex( 6.0, ymax_Yields/10000. , "M_{T2}=[300,400]");
      ttextmt2->DrawLatex( 9.5, ymax_Yields/10000. , "M_{T2}#geq400 GeV");
      //-----------------------------------------------------------
   }else{
      TLine *tl_ntop = new TLine();
      tl_ntop->SetLineStyle(2);
      tl_ntop->DrawLine(27.5 + adjHalfBin,ymin_Yields,27.5 + adjHalfBin,ymax_Yields);
      tl_ntop->DrawLine(52.5 + adjHalfBin,ymin_Yields,52.5 + adjHalfBin,ymax_Yields);

    // Ntop labels
      TLatex * ttext_ntop = new TLatex();
      ttext_ntop->SetTextFont(42);
      ttext_ntop->SetTextSize(0.050);
      ttext_ntop->SetTextAlign(22);
      ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields/3 ,"N_{t} = 1");
      ttext_ntop->DrawLatex(39.5 + adjHalfBin, ymax_Yields/1500. ,"N_{t} = 2");
      ttext_ntop->SetTextAngle(90.); 
      ttext_ntop->DrawLatex(55.5 + adjHalfBin, ymax_Yields/2500. ,"N_{t} #geq 3");

      // Nb separation lines
      TLine *tl_nb = new TLine();
      tl_nb->SetLineStyle(3);
      tl_nb->SetLineColor(1);
      tl_nb->DrawLine(11.5 + adjHalfBin,ymin_Yields,11.5 + adjHalfBin,ymax_Yields/5);
      tl_nb->DrawLine(22.5 + adjHalfBin,ymin_Yields,22.5 + adjHalfBin,ymax_Yields/5);
      tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields/2000.);
      tl_nb->DrawLine(48.5 + adjHalfBin,ymin_Yields,48.5 + adjHalfBin,ymax_Yields/2000.);
      tl_nb->DrawLine(54.5 + adjHalfBin,ymin_Yields,54.5 + adjHalfBin,ymax_Yields/5000.);
      tl_nb->DrawLine(56.5 + adjHalfBin,ymin_Yields,56.5 + adjHalfBin,ymax_Yields/5000.);

     // Nb labels
      TLatex * ttext2 = new TLatex();
      ttext2->SetTextFont(42);
      //ttext2->SetTextColor(32);
      ttext2->SetTextColor(kBlack);
      ttext2->SetTextSize(0.050);
      ttext2->SetTextAlign(22);
      ttext2->Draw();

      ttext2->DrawLatex(5.5 + adjHalfBin, ymax_Yields/10., "N_{b} = 1");
      ttext2->DrawLatex(17.5 + adjHalfBin, ymax_Yields/10., "N_{b} = 2");
      ttext2->SetTextAngle(90.);
      ttext2->DrawLatex(24.0  + adjHalfBin, ymax_Yields/2500 , "N_{b} #geq 3");

      // MT2 separation lines
      TLine *tl_mt2 = new TLine();
      tl_mt2->SetLineStyle(4);
      tl_mt2->SetLineColor(1);
      tl_mt2->DrawLine(3.5 + adjHalfBin,ymin_Yields,3.5 + adjHalfBin,ymax_Yields/100.);
      tl_mt2->DrawLine(7.5 + adjHalfBin,ymin_Yields,7.5 + adjHalfBin,ymax_Yields/100.);
      tl_mt2->DrawLine(15.5 + adjHalfBin,ymin_Yields,15.5 + adjHalfBin,ymax_Yields/1500.);
      tl_mt2->DrawLine(19.5 + adjHalfBin,ymin_Yields,19.5 + adjHalfBin,ymax_Yields/1500.);
      tl_mt2->DrawLine(25.5 + adjHalfBin,ymin_Yields,25.5 + adjHalfBin,ymax_Yields/1500.);
      tl_mt2->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields/5000.);
      tl_mt2->DrawLine(35.5 + adjHalfBin,ymin_Yields,35.5 + adjHalfBin,ymax_Yields/5000.);
      tl_mt2->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields/5000.);
      tl_mt2->DrawLine(45.5 + adjHalfBin,ymin_Yields,45.5 + adjHalfBin,ymax_Yields/5000.);
      tl_mt2->DrawLine(50.5 + adjHalfBin,ymin_Yields,50.5 + adjHalfBin,ymax_Yields/5000.);

      // MT2 labels
      TLatex * ttextmt2 = new TLatex();
      ttextmt2->SetTextFont(42);
      //ttextmt2->SetTextColor(49);
      ttextmt2->SetTextColor(kBlack);
      ttextmt2->SetTextSize(0.040);
      ttextmt2->SetTextAlign(12);
      ttextmt2->SetTextAngle(90);

      ttextmt2->DrawLatex( 3.0, ymax_Yields/8000. , "M_{T2}=[200,350]");
      ttextmt2->DrawLatex( 6.0, ymax_Yields/8000. , "M_{T2}=[350,450]");
      ttextmt2->DrawLatex( 10.5, ymax_Yields/8000. , "M_{T2}#geq450 GeV");
      ttextmt2->DrawLatex( 27.0, ymax_Yields/50000. , "M_{T2}#geq350 GeV");
   }
*/
   
   TLatex mark;
   mark.SetNDC(true);
   double fontScale = 1.6;
   char lumistamp[128];
   if( !noobs ) sprintf(lumistamp, "%.1f fb^{-1} (13 TeV)", dataLumi/1000.0);
   else sprintf(lumistamp, "%.1f fb^{-1} (13 TeV)", bkgLumi/1000.0);
   //Draw CMS mark

   mark.SetTextAlign(11);
   mark.SetTextSize(0.042 * fontScale * 1.25);
   mark.SetTextFont(61);
   mark.DrawLatex(gPad->GetLeftMargin(), 1 - (gPad->GetTopMargin() - 0.017), "CMS"); // #scale[0.8]{#it{Preliminary}}");        
   mark.SetTextSize(0.042 * fontScale);
   mark.SetTextFont(52);
   mark.DrawLatex(gPad->GetLeftMargin() + 0.09, 1 - (gPad->GetTopMargin() - 0.017), "Preliminary");
   //Draw lumistamp                                                                                                                         
   mark.SetTextFont(42);
   mark.SetTextAlign(31);
   mark.DrawLatex(1 - gPad->GetRightMargin(), 1 - (gPad->GetTopMargin() - 0.017), lumistamp);
   
   catLeg1->SetFillColor(kWhite);
   catLeg1->SetBorderSize(0);
   catLeg1->Draw();
   catLeg2->SetFillColor(kWhite);
   catLeg2->SetBorderSize(0);
   catLeg2->Draw();

   catLeg_sig1->SetFillColor(kWhite);
   catLeg_sig1->SetBorderSize(0);
   if( addSigPts ) catLeg_sig1->Draw();
   catLeg_sig2->SetFillColor(kWhite);
   catLeg_sig2->SetBorderSize(0);
   if( addSigPts ) catLeg_sig2->Draw();
   catLeg_sig3->SetFillColor(kWhite);
   catLeg_sig3->SetBorderSize(0);
   if( addSigPts ) catLeg_sig3->Draw();

   catLeg_unc->SetFillColor(kWhite);
   catLeg_unc->SetBorderSize(0);
   catLeg_unc->Draw();

   TH1D * h1_ratio = (TH1D*) h1_data->Clone();
   if( !noobs ){
      TH1D * tmp_sum_SM_cloned = (TH1D*) tmp_sum_SM->Clone();
      for(int ib=0; ib<tmp_sum_SM_cloned->GetXaxis()->GetNbins()+1; ib++){
         tmp_sum_SM_cloned->SetBinError(ib, 0);
      }
//      h1_ratio->Divide(tmp_sum_SM);
      h1_ratio->Divide(tmp_sum_SM_cloned);
   }else{
      h1_ratio = (TH1D*) tmp_sum_SM->Clone();
      h1_ratio->Divide(tmp_sum_SM);
      h1_ratio->Reset();
   }

   ct->cd();
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.30);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.30);
   //pad2->SetGridy();
//   pad2->SetLogy();
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad                                                           
   TF1 * fline = new TF1("line", "pol0", h1_ratio->GetBinLowEdge(1), h1_ratio->GetBinLowEdge(h1_ratio->GetNbinsX()) + h1_ratio->GetBinWidth(h1_ratio->GetNbinsX()));
   fline->SetParameter(0, 1);
   h1_ratio->SetMarkerColor(kBlack);
   h1_ratio->SetTitle(0);
   h1_ratio->SetLineColor(kBlack);

   h1_ratio->SetMinimum(ymin_Ratio);  // Define Y ..                                                                      
   h1_ratio->SetMaximum(ymax_Ratio); // .. range                                                                          
   h1_ratio->SetStats(0);

   // Y axis ratio plot settings                                                                               
   h1_ratio->GetYaxis()->SetTitle("#frac{Data}{Prediction}");
   h1_ratio->GetYaxis()->SetNdivisions(505);
   h1_ratio->GetYaxis()->SetTitleSize(26);
   h1_ratio->GetYaxis()->SetTitleFont(43);
   h1_ratio->GetYaxis()->SetTitleOffset(1.0);
   h1_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)                          
   h1_ratio->GetYaxis()->SetLabelSize(17);

   // X axis ratio plot settings                                                                               
   h1_ratio->GetXaxis()->SetTitleSize(26);
   h1_ratio->GetXaxis()->SetTitleFont(43);
   h1_ratio->GetXaxis()->SetTitleOffset(3.0);
   h1_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)                          
   h1_ratio->GetXaxis()->SetLabelSize(17);
   h1_ratio->Draw("E1");
   fline->SetLineColor(1);
   fline->SetLineStyle(2);   
   fline->Draw("same");

   if( addStatUnc ){
      TGraphAsymmErrors * gr_Ratio_Sum_AsymErr = new TGraphAsymmErrors(xVec.size(), &xVec[0], &yRatioVec[0], &xSysErrDnVec[0], &xSysErrUpVec[0], &yRatioSumErrUpVec[0], &yRatioSumErrDnVec[0]);
      gr_Ratio_Sum_AsymErr->SetFillColor(kGray+1); gr_Ratio_Sum_AsymErr->SetFillStyle(3244); gr_Ratio_Sum_AsymErr->SetLineWidth(1); gr_Ratio_Sum_AsymErr->SetLineColor(0); gr_Ratio_Sum_AsymErr->SetMarkerSize(0); gr_Ratio_Sum_AsymErr->SetMarkerColor(0);
      gr_Ratio_Sum_AsymErr->Draw("2");
   }

   TGraphAsymmErrors * gr_Ratio_AsymErr = new TGraphAsymmErrors(xVec.size(), &xVec[0], &yRatioVec[0], &xSysErrDnVec[0], &xSysErrUpVec[0], &yRatioSysErrDnVec[0], &yRatioSysErrUpVec[0]);
   gr_Ratio_AsymErr->SetFillColor(kGray+4); gr_Ratio_AsymErr->SetFillStyle(3244);
   gr_Ratio_AsymErr->Draw("2");
   fline->Draw("same");
   h1_ratio->Draw("E1same");

   sb->drawSBregionDef(0, ymin_Yields, false, true);
      //-----------------------------------------------------------
      // Putting lines and labels explaining search region definitions
      //-----------------------------------------------------------
/*
   if( do37Bins ){
      //Ntop separation lines
      TLine *tl_ntop = new TLine();
      tl_ntop->SetLineStyle(2);
      tl_ntop->DrawLine(21,ymin_Ratio,21,ymax_Ratio);

      // Nb separation lines
      TLine *tl_nb = new TLine();
      tl_nb->SetLineStyle(3);
      tl_nb->SetLineColor(1);
      tl_nb->DrawLine(11,ymin_Ratio,11,ymax_Ratio);
      tl_nb->DrawLine(29,ymin_Ratio,29,ymax_Ratio);

      // MT2 separation lines
      TLine *tl_mt2 = new TLine();
      tl_mt2->SetLineStyle(4);
      tl_mt2->SetLineColor(1);

      tl_mt2->DrawLine(4,ymin_Ratio,4,ymax_Ratio);
      tl_mt2->DrawLine(8,ymin_Ratio,8,ymax_Ratio);
      tl_mt2->DrawLine(15,ymin_Ratio,15,ymax_Ratio);
      tl_mt2->DrawLine(19,ymin_Ratio,19,ymax_Ratio);

      tl_mt2->DrawLine(24,ymin_Ratio,24,ymax_Ratio);
      tl_mt2->DrawLine(27,ymin_Ratio,27,ymax_Ratio);
      tl_mt2->DrawLine(32,ymin_Ratio,32,ymax_Ratio);
      tl_mt2->DrawLine(35,ymin_Ratio,35,ymax_Ratio);
   }else{

      //Ntop separation lines
      TLine *tl_ntop = new TLine();
      tl_ntop->SetLineStyle(2);
      tl_ntop->DrawLine(27.5 + adjHalfBin,ymin_Ratio,27.5 + adjHalfBin,ymax_Yields);
      tl_ntop->DrawLine(52.5 + adjHalfBin,ymin_Ratio,52.5 + adjHalfBin,ymax_Yields);

      // Nb separation lines
      TLine *tl_nb = new TLine();
      tl_nb->SetLineStyle(3);
      tl_nb->SetLineColor(1);
      tl_nb->DrawLine(11.5 + adjHalfBin,ymin_Ratio,11.5 + adjHalfBin,ymax_Yields/6);
      tl_nb->DrawLine(22.5 + adjHalfBin,ymin_Ratio,22.5 + adjHalfBin,ymax_Yields/6.);
      tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Ratio,39.5 + adjHalfBin,ymax_Yields/160.);
      tl_nb->DrawLine(48.5 + adjHalfBin,ymin_Ratio,48.5 + adjHalfBin,ymax_Yields/160.);
      tl_nb->DrawLine(54.5 + adjHalfBin,ymin_Ratio,54.5 + adjHalfBin,ymax_Yields/160.);
      tl_nb->DrawLine(56.5 + adjHalfBin,ymin_Ratio,56.5 + adjHalfBin,ymax_Yields/160.);

      // MT2 separation lines
      TLine *tl_mt2 = new TLine();
      tl_mt2->SetLineStyle(4);
      tl_mt2->SetLineColor(1);
      tl_mt2->DrawLine(3.5 + adjHalfBin,ymin_Ratio,3.5 + adjHalfBin,ymax_Yields/40.);
      tl_mt2->DrawLine(7.5 + adjHalfBin,ymin_Ratio,7.5 + adjHalfBin,ymax_Yields/40.);
      tl_mt2->DrawLine(15.5 + adjHalfBin,ymin_Ratio,15.5 + adjHalfBin,ymax_Yields/40.);
      tl_mt2->DrawLine(19.5 + adjHalfBin,ymin_Ratio,19.5 + adjHalfBin,ymax_Yields/40.);
      tl_mt2->DrawLine(25.5 + adjHalfBin,ymin_Ratio,25.5 + adjHalfBin,ymax_Yields/100.);
      tl_mt2->DrawLine(31.5 + adjHalfBin,ymin_Ratio,31.5 + adjHalfBin,ymax_Yields/100.);
      tl_mt2->DrawLine(35.5 + adjHalfBin,ymin_Ratio,35.5 + adjHalfBin,ymax_Yields/320.);
      tl_mt2->DrawLine(42.5 + adjHalfBin,ymin_Ratio,42.5 + adjHalfBin,ymax_Yields/320.);
      tl_mt2->DrawLine(45.5 + adjHalfBin,ymin_Ratio,45.5 + adjHalfBin,ymax_Yields/320.);
      tl_mt2->DrawLine(50.5 + adjHalfBin,ymin_Ratio,50.5 + adjHalfBin,ymax_Yields/320.);

      //-----------------------------------------------------------
   }
*/   
   ct->Print("UnblindPlots.pdf");
   ct->Print("UnblindPlots.eps");

/*
   std::cout<<"\nPrinting out the data vs. prediction table..."<<std::endl;
   print_searchBins_headerstr(" & Obs. & Sum. Pred. & Stat. Unc. & Syst. Unc. \\\\");
   for(unsigned int ib=0; ib<nTotBins; ib++){
//      printf("%2d   data : %6.3f +- %5.3f   sumPred : %6.3f +- %6.3f +- %6.3f\n", ib, h1_data->GetBinContent(ib+1), h1_data->GetBinError(ib+1), tmp_sum_SM->GetBinContent(ib+1), tmp_sum_SM->GetBinError(ib+1), tmp2_sum_SM->GetBinError(ib+1));
      char tmpstr[500];
      sprintf(tmpstr, " & %6.0f & %6.2f & $\\pm$ %5.2f & $\\pm$ %6.2f \\\\", h1_data->GetBinContent(ib+1), tmp_sum_SM->GetBinContent(ib+1), tmp_sum_SM->GetBinError(ib+1), tmp2_sum_SM->GetBinError(ib+1));
//      sprintf(tmpstr, " & %6.0f $\\pm$ %5.1f & %6.2f & $\\pm$ %5.2f & $\\pm$ %6.2f \\\\", h1_data->GetBinContent(ib+1), h1_data->GetBinError(ib+1), tmp_sum_SM->GetBinContent(ib+1), tmp_sum_SM->GetBinError(ib+1), tmp2_sum_SM->GetBinError(ib+1));
      std::string outstr = get_searchBins_defstr(ib, tmpstr);
      printf("%s", outstr.c_str());
   }
   std::cout<<std::endl;
*/
}

int main(int argc, char *argv[]){
   sb = new SearchBins();

   nTotBins = sb->nSearchBins();

   sb->build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   nTotBins_loc = nTotBins;

   std::string cutLev = "baseline";
   std::string datatype = "HTMHT";
   if( argc >= 2 ) cutLev = argv[1];
   if( argc >= 3 ) datatype = argv[2];
   std::cout<<"\ncutLev : "<<cutLev.c_str()<<"  datatype : "<<datatype.c_str()<<std::endl;
   makeUnblindPlots(cutLev, datatype);
}
