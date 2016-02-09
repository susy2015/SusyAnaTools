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

#include "TObjString.h"
#include "TObjArray.h"
#include "TKey.h"

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

#include "xSec.h"

AnaSamples::SampleSet        allSamples;
AnaSamples::SampleCollection allCollections(allSamples);

std::vector<TH2Poly*> h2_sumSM_poly_MT2_vs_metVec, h2_sumSM_poly_muonCSforLostLept_MT2_vs_metVec;
std::vector<TH2Poly*> h2_sumSM_poly_0bZnunu_MT2_vs_metVec;

std::vector<TH1D*> h1_cutFlowVec;

char names[200], dispt[200];

TStopwatch timer;

const double ymax_Yields = 500.;
const double ymin_Yields = 0.05;

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

void makeSignalCards(const std::string inputRootName){

   double dataLumi = 2153.74;

   TFile * sig_file = new TFile(inputRootName.c_str());
   sig_file->cd();

   TDirectory *current_sourcedir = gDirectory;
   TIter nextkey( current_sourcedir->GetListOfKeys() );
   TKey *key, *oldkey=0;
   while ( (key = (TKey*)nextkey())) {

      TString nameT = key->GetName();
      if( !nameT.Contains("baseline_nSearchBin") ) continue;

      TObjArray * vlist =0;
      vlist = nameT.Tokenize("_");

      TObjString* obj_mStop = dynamic_cast<TObjString*>(vlist->At(2));
      TString v_mStop=obj_mStop->GetString();
      int mStop = v_mStop.Atoi();

      TObjString* obj_mLSP = dynamic_cast<TObjString*>(vlist->At(3));
      TString v_mLSP=obj_mLSP->GetString();
      int mLSP = v_mLSP.Atoi();

    // read object from first source file
      TObject *obj = key->ReadObj();
      TH1D * h1_nSearchBin = (TH1D*) obj;

      TString scaleUncUp_str = "baseline_scaleUncUp_" + v_mStop + "_" + v_mLSP;
      TString scaleUncDn_str = "baseline_scaleUncDn_" + v_mStop + "_" + v_mLSP;

      TString pdfUncUp_str = "baseline_pdfUncUp_" + v_mStop + "_" + v_mLSP;
      TString pdfUncCen_str = "baseline_pdfUncCen_" + v_mStop + "_" + v_mLSP;
      TString pdfUncDn_str = "baseline_pdfUncDn_" + v_mStop + "_" + v_mLSP;

      TString isrUncUp_str = "baseline_isrUncUp_" + v_mStop + "_" + v_mLSP;
      TString isrUncDn_str = "baseline_isrUncDn_" + v_mStop + "_" + v_mLSP;

      TString metMagUp_str = "baseline_metMagUp_" + v_mStop + "_" + v_mLSP;
      TString metMagDn_str = "baseline_metMagDn_" + v_mStop + "_" + v_mLSP;

      TString metPhiUp_str = "baseline_metPhiUp_" + v_mStop + "_" + v_mLSP;
      TString metPhiDn_str = "baseline_metPhiDn_" + v_mStop + "_" + v_mLSP;

      TString jetJECUp_str = "baseline_jetJECUp_" + v_mStop + "_" + v_mLSP;
      TString jetJECDn_str = "baseline_jetJECDn_" + v_mStop + "_" + v_mLSP;

      TString btagSFUp_str = "baseline_bTagSFUp_" + v_mStop + "_" + v_mLSP;
      TString btagSFCen_str = "baseline_bTagSFCen_" + v_mStop + "_" + v_mLSP;
      TString btagSFDn_str = "baseline_bTagSFDn_" + v_mStop + "_" + v_mLSP;

      TString mistagSFUp_str = "baseline_mistagSFUp_" + v_mStop + "_" + v_mLSP;
      TString mistagSFDn_str = "baseline_mistagSFDn_" + v_mStop + "_" + v_mLSP;
//      std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<std::endl;

      TH1D * h1_scaleUncUp = (TH1D*) sig_file->Get(scaleUncUp_str);
      TH1D * h1_scaleUncDn = (TH1D*) sig_file->Get(scaleUncDn_str);
//      std::cout<<"  scaleUncUp_str : "<<scaleUncUp_str<<"  scaleUncDn_str : "<<scaleUncDn_str<<std::endl;

      TH1D * h1_pdfUncUp = (TH1D*) sig_file->Get(pdfUncUp_str);
      TH1D * h1_pdfUncCen = (TH1D*) sig_file->Get(pdfUncCen_str);
      TH1D * h1_pdfUncDn = (TH1D*) sig_file->Get(pdfUncDn_str);
//      std::cout<<"  pdfUncUp_str : "<<pdfUncUp_str<<"  pdfUncDn_str : "<<pdfUncDn_str<<std::endl;

      TH1D * h1_isrUncUp = (TH1D*) sig_file->Get(isrUncUp_str);
      TH1D * h1_isrUncDn = (TH1D*) sig_file->Get(isrUncDn_str);
//      std::cout<<"  isrUncUp_str : "<<isrUncUp_str<<"  isrUncDn_str : "<<isrUncDn_str<<std::endl;

      TH1D * h1_metMagUp = (TH1D*) sig_file->Get(metMagUp_str);
      TH1D * h1_metMagDn = (TH1D*) sig_file->Get(metMagDn_str);
//      std::cout<<"  metMagUp_str : "<<metMagUp_str<<"  metMagDn_str : "<<metMagDn_str<<std::endl;

      TH1D * h1_metPhiUp = (TH1D*) sig_file->Get(metPhiUp_str);
      TH1D * h1_metPhiDn = (TH1D*) sig_file->Get(metPhiDn_str);
//      std::cout<<"  metPhiUp_str : "<<metPhiUp_str<<"  metPhiDn_str : "<<metPhiDn_str<<std::endl;

      TH1D * h1_jetJECUp = (TH1D*) sig_file->Get(jetJECUp_str);
      TH1D * h1_jetJECDn = (TH1D*) sig_file->Get(jetJECDn_str);
//      std::cout<<"  jetJECUp_str : "<<jetJECUp_str<<"  jetJECDn_str : "<<jetJECDn_str<<std::endl;

      TH1D * h1_btagSFUp = (TH1D*) sig_file->Get(btagSFUp_str);
      TH1D * h1_btagSFCen = (TH1D*) sig_file->Get(btagSFCen_str);
      TH1D * h1_btagSFDn = (TH1D*) sig_file->Get(btagSFDn_str);
//      std::cout<<"  btagSFUp_str : "<<btagSFUp_str<<"  btagSFDn_str : "<<btagSFDn_str<<"  btagSFCen_str : "<<btagSFCen_str<<std::endl;

      TH1D * h1_mistagSFUp = (TH1D*) sig_file->Get(mistagSFUp_str);
      TH1D * h1_mistagSFDn = (TH1D*) sig_file->Get(mistagSFDn_str);
//      std::cout<<"  mistagSFUp_str : "<<mistagSFUp_str<<"  mistagSFDn_str : "<<mistagSFDn_str<<std::endl;

      double xSec = 0, xSecErr = 0;
      if( xSecMap.find(mStop) != xSecMap.end() ){
         xSec = xSecMap.find(mStop)->second;
         xSecErr = xSecErrMap.find(mStop)->second * xSec;   
      }

      char tmpStr[200], tmpStr2[200];
      sprintf(tmpStr, "totEntries_%d_%d", mStop, mLSP);
      TH1D * h1_totEntries = (TH1D*) sig_file->Get(tmpStr);

      double totEntries = h1_totEntries->GetBinContent(1);
      double totEntriesErr = sqrt(totEntries);

      double lumi_scale = dataLumi*xSec/totEntries;

      int nBins = h1_nSearchBin->GetXaxis()->GetNbins();
      std::vector<double> rateVec(nBins), statUncVec(nBins), systUncVec(nBins);
      std::vector<double> rate_scaleVec(nBins);
      std::vector<double> bTagSFUpVec(nBins), bTagSFDnVec(nBins);
      std::vector<double> mistagSFUpVec(nBins), mistagSFDnVec(nBins);
      std::vector<double> pdfUncUpVec(nBins), pdfUncDnVec(nBins);
      std::vector<double> scaleUncUpVec(nBins), scaleUncDnVec(nBins);
      std::vector<double> isrUncUpVec(nBins), isrUncDnVec(nBins);
      std::vector<double> metMagUpVec(nBins), metMagDnVec(nBins);
      std::vector<double> metPhiUpVec(nBins), metPhiDnVec(nBins);
      std::vector<double> jetJECUpVec(nBins), jetJECDnVec(nBins);
//      std::cout<<"xSec : "<<xSec<<"  totEntries : "<<totEntries<<"  lumi_scale : "<<lumi_scale<<std::endl;

      double sum_scaleUncUp = 0, sum_scaleUncDn = 0;
      double sum_pdfUncUp = 0, sum_pdfUncCen = 0, sum_pdfUncDn = 0;
      double sum_cent = 0;
      for(int ib=0; ib<nBins; ib++){
         double cent = h1_nSearchBin->GetBinContent(ib+1); sum_cent += cent;
         double err = h1_nSearchBin->GetBinError(ib+1);
         double rel_err = 0; if( cent!=0 ) rel_err = err/cent;
         rateVec[ib] = cent; statUncVec[ib] = rel_err; systUncVec[ib] = 0.2;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  rate : "<<cent<<std::endl;

         double bTagSFCen_cent = h1_btagSFCen->GetBinContent(ib+1);
         double bTagSFCen_scale = cent !=0 ? bTagSFCen_cent/cent : 1.0;
         rate_scaleVec[ib] = lumi_scale * bTagSFCen_scale;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  rate : "<<cent<<"  rate_scale : "<<lumi_scale*bTagSFCen_scale<<std::endl;

         double bTagSFUp_cent = h1_btagSFUp->GetBinContent(ib+1);
         double bTagSFDn_cent = h1_btagSFDn->GetBinContent(ib+1);
         double bTagSFUp_relUnc = bTagSFCen_cent !=0 ? bTagSFUp_cent/bTagSFCen_cent -1 : 0;
         double bTagSFDn_relUnc = bTagSFCen_cent !=0 ? 1 - bTagSFDn_cent/bTagSFCen_cent : 0;
         bTagSFUpVec[ib] = bTagSFUp_relUnc; bTagSFDnVec[ib] = bTagSFDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  bTagSFUp_cent : "<<bTagSFUp_cent<<"  bTagSFDn_cent : "<<bTagSFDn_cent<<std::endl;

         double mistagSFUp_cent = h1_mistagSFUp->GetBinContent(ib+1);
         double mistagSFDn_cent = h1_mistagSFDn->GetBinContent(ib+1);
         double mistagSFUp_relUnc = bTagSFCen_cent !=0 ? mistagSFUp_cent/bTagSFCen_cent -1 : 0;
         double mistagSFDn_relUnc = bTagSFCen_cent !=0 ? 1 - mistagSFDn_cent/bTagSFCen_cent : 0;
         mistagSFUpVec[ib] = mistagSFUp_relUnc; mistagSFDnVec[ib] = mistagSFDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  mistagSFUp_cent : "<<mistagSFUp_cent<<"  mistagSFDn_cent : "<<mistagSFDn_cent<<std::endl;

         double pdfUncUp_cent = h1_pdfUncUp->GetBinContent(ib+1); sum_pdfUncUp += pdfUncUp_cent;
         double pdfUncCen_cent = h1_pdfUncCen->GetBinContent(ib+1); sum_pdfUncCen += pdfUncCen_cent;
         double pdfUncDn_cent = h1_pdfUncDn->GetBinContent(ib+1); sum_pdfUncDn += pdfUncDn_cent;
         double pdfUncUp_relUnc = pdfUncCen_cent !=0 ? pdfUncUp_cent/pdfUncCen_cent -1 : 0;
         double pdfUncDn_relUnc = pdfUncCen_cent !=0 ? 1 - pdfUncDn_cent/pdfUncCen_cent : 0;
         pdfUncUpVec[ib] = pdfUncUp_relUnc; pdfUncDnVec[ib] = pdfUncDn_relUnc;

         if( std::abs(pdfUncUp_relUnc) > 1 || std::abs(pdfUncDn_relUnc) > 1 ){
//            std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  pdfUncCen_cent : "<<pdfUncCen_cent<<"  pdfUncUp_cent : "<<pdfUncUp_cent<<"  pdfUncDn_cent : "<<pdfUncDn_cent<<std::endl;
         }

         double scaleUncUp_cent = h1_scaleUncUp->GetBinContent(ib+1); sum_scaleUncUp += scaleUncUp_cent;
         double scaleUncDn_cent = h1_scaleUncDn->GetBinContent(ib+1); sum_scaleUncDn += scaleUncDn_cent;
         double scaleUncUp_relUnc = cent !=0 ? scaleUncUp_cent/cent -1 : 0;
         double scaleUncDn_relUnc = cent !=0 ? 1 - scaleUncDn_cent/cent : 0;
         scaleUncUpVec[ib] = scaleUncUp_relUnc; scaleUncDnVec[ib] = scaleUncDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  scaleUncUp_cent : "<<scaleUncUp_cent<<"  scaleUncDn_cent : "<<scaleUncDn_cent<<std::endl;

         double isrUncUp_cent = h1_isrUncUp->GetBinContent(ib+1);
         double isrUncDn_cent = h1_isrUncDn->GetBinContent(ib+1);
         double isrUncUp_relUnc = cent !=0 ? isrUncUp_cent/cent -1 : 0;
         double isrUncDn_relUnc = cent !=0 ? 1 - isrUncDn_cent/cent : 0;
         isrUncUpVec[ib] = isrUncUp_relUnc; isrUncDnVec[ib] = isrUncDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  isrUncUp_cent : "<<isrUncUp_cent<<"  isrUncDn_cent : "<<isrUncDn_cent<<std::endl;

         double metMagUp_cent = h1_metMagUp->GetBinContent(ib+1);
         double metMagDn_cent = h1_metMagDn->GetBinContent(ib+1);
         double metMagUp_relUnc = cent !=0 ? metMagUp_cent/cent -1 : 0;
         double metMagDn_relUnc = cent !=0 ? 1 - metMagDn_cent/cent : 0;
         metMagUpVec[ib] = metMagUp_relUnc; metMagDnVec[ib] = metMagDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  metMagUp_cent : "<<metMagUp_cent<<"  metMagDn_cent : "<<metMagDn_cent<<std::endl;

         double metPhiUp_cent = h1_metPhiUp->GetBinContent(ib+1);
         double metPhiDn_cent = h1_metPhiDn->GetBinContent(ib+1);
         double metPhiUp_relUnc = cent !=0 ? metPhiUp_cent/cent -1 : 0;
         double metPhiDn_relUnc = cent !=0 ? 1 - metPhiDn_cent/cent : 0;
         metPhiUpVec[ib] = metPhiUp_relUnc; metPhiDnVec[ib] = metPhiDn_relUnc;
//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  metPhiUp_cent : "<<metPhiUp_cent<<"  metPhiDn_cent : "<<metPhiDn_cent<<std::endl;

         double jetJECUp_cent = h1_jetJECUp->GetBinContent(ib+1);
         double jetJECDn_cent = h1_jetJECDn->GetBinContent(ib+1);
         double jetJECUp_relUnc = cent !=0 ? jetJECUp_cent/cent -1 : 0;
         double jetJECDn_relUnc = cent !=0 ? 1 - jetJECDn_cent/cent : 0;
         jetJECUpVec[ib] = jetJECUp_relUnc; jetJECDnVec[ib] = jetJECDn_relUnc;

//         std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  rate : "<<cent<<"  bTagSFUp_cent : "<<bTagSFUp_cent<<"  bTagSFDn_cent : "<<bTagSFDn_cent<<"  mistagSFUp_cent : "<<mistagSFUp_cent<<"  mistagSFDn_cent : "<<mistagSFDn_cent<<"  scaleUncUp_cent : "<<scaleUncUp_cent<<"  scaleUncDn_cent : "<<scaleUncDn_cent<<"  isrUncUp_cent : "<<isrUncUp_cent<<"  isrUncDn_cent : "<<isrUncDn_cent<<"  metMagUp_cent : "<<metMagUp_cent<<"  metMagDn_cent : "<<metMagDn_cent<<"  metPhiUp_cent : "<<metPhiUp_cent<<"  metPhiDn_cent : "<<metPhiDn_cent<<"  jetJECUp_cent : "<<jetJECUp_cent<<"  jetJECDn_cent : "<<jetJECDn_cent<<std::endl;
      }

      for(unsigned int ib=0; ib<nBins; ib++){
         if( std::abs(pdfUncUpVec[ib]) > 1 || std::abs(pdfUncDnVec[ib]) > 1 ){
//            std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  sum_pdfUncCen : "<<sum_pdfUncCen<<"  sum_pdfUncUp : "<<sum_pdfUncUp<<"  sum_pdfUncDn : "<<sum_pdfUncDn<<std::endl;
         }
         scaleUncUpVec[ib] = (scaleUncUpVec[ib] + 1) * (sum_cent/sum_scaleUncUp) -1;
         scaleUncDnVec[ib] = 1 - (1 - scaleUncDnVec[ib]) * (sum_cent/sum_scaleUncDn);

         pdfUncUpVec[ib] = (pdfUncUpVec[ib] + 1) * (sum_pdfUncCen/sum_pdfUncUp) -1;
         pdfUncDnVec[ib] = 1 - (1 - pdfUncDnVec[ib]) * (sum_pdfUncCen/sum_pdfUncDn);
      }

//      std::cout<<"mStop : "<<mStop<<"  mLSP : "<<mLSP<<"  totEntries : "<<totEntries<<"  nBins : "<<nBins<<"  xSec : "<<xSec<<std::endl;

      sprintf(tmpStr, "signal_%d_%d.txt", mStop, mLSP);
      std::ofstream ofs;
      ofs.open(tmpStr, std::ofstream::out);
      ofs<<"luminosity = "<<dataLumi<<std::endl;
      ofs<<"channels = 45"<<std::endl;
      ofs<<"sample = signal"<<std::endl;
      
      ofs<<"channel     = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr2, "bin%d", ib+1);
         sprintf(tmpStr, "%10s", tmpStr2);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"rate        = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.5f", rateVec[ib]*rate_scaleVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"cs_event    = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.0f", rateVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;
      
      ofs<<"avg_weight  = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.7f", rate_scaleVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"stat_unc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", statUncVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"stat_unc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", statUncVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"lumi_unc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", 0.046);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"lumi_unc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", 0.046);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"bTagSF_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", bTagSFUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"bTagSF_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", bTagSFDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"mistagSF_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", mistagSFUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"mistagSF_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", mistagSFDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"pdfUnc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", pdfUncUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"pdfUnc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", pdfUncDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"scaleUnc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", scaleUncUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"scaleUnc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", scaleUncDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"isrUnc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", isrUncUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"isrUnc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", isrUncDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"metMag_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", metMagUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"metMag_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", metMagDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"metPhi_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", metPhiUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"metPhi_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", metPhiDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"jetJEC_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", jetJECUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"jetJEC_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", jetJECDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

/*
      ofs<<"syst_unc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", systUncVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"syst_unc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", systUncVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;
*/

      ofs.close();
   }
}

int main(int argc, char *argv[]){
   std::string inputRootName = "signalScan_SMS-T2tt.root";
   if( argc >= 2 ) inputRootName = argv[1];
   std::cout<<"\ninputRootName : "<<inputRootName.c_str()<<std::endl;
   makeSignalCards(inputRootName);
}
