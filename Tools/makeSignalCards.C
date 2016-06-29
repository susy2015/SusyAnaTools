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

int nXbins = 35, nYbins = 22;
double loX =  87.5, hiX = 962.5;
double loY = -12.5, hiY = 537.5;

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

void makeSignalCards(const std::string inputRootName, const std::string inputRootName_hadtau_cont, const std::string inputRootName_lostle_cont){

   double dataLumi = 4004.345;

   TFile * sig_cont_hadtau_file = 0, * sig_cont_lostle_file = 0;
   if( !inputRootName_hadtau_cont.empty() ) sig_cont_hadtau_file = new TFile(inputRootName_hadtau_cont.c_str());
   if( !inputRootName_lostle_cont.empty() ) sig_cont_lostle_file = new TFile(inputRootName_lostle_cont.c_str());

   std::string sampleKeyStr = "";
   if( inputRootName.find("T2tt") != std::string::npos ) sampleKeyStr = "T2tt";
   if( inputRootName.find("T2tb") != std::string::npos ) sampleKeyStr = "T2tb";
   if( inputRootName.find("T2bb") != std::string::npos ) sampleKeyStr = "T2bb";
   if( inputRootName.find("T1tttt") != std::string::npos ) sampleKeyStr = "T1tttt";
   if( inputRootName.find("T1ttbb") != std::string::npos ) sampleKeyStr = "T1ttbb";
   if( inputRootName.find("T5ttcc") != std::string::npos ) sampleKeyStr = "T5ttcc";
   if( inputRootName.find("T5tttt_degen") != std::string::npos ) sampleKeyStr = "T5tttt_degen";
   if( inputRootName.find("T5ttttDM175") != std::string::npos ) sampleKeyStr = "T5ttttDM175";
   if( inputRootName.find("T6ttWW") != std::string::npos ) sampleKeyStr = "T6ttWW";

   std::cout<<"\nsampleKeyStr : "<<sampleKeyStr.c_str()<<std::endl<<std::endl;

   if( sampleKeyStr == "T1tttt" || sampleKeyStr == "T5ttcc" || sampleKeyStr == "T5tttt_degen" || sampleKeyStr == "T5ttttDM175" || sampleKeyStr == "T1ttbb"){
      nXbins = 52; nYbins = 49;
      loX = 612.5; hiX = 1912.5;
      loY = -12.5; hiY = 1212.5;
   }

   TFile * sig_file = new TFile(inputRootName.c_str());
   sig_file->cd();
   TDirectory *current_sourcedir = gDirectory;
   TIter nextkey( current_sourcedir->GetListOfKeys() );
   TKey *key, *oldkey=0;

   const int nEvtToRange = 100;
   double minstatRelUnc = 1.00, maxstatRelUnc = 0.00;
   double minscaleRelUnc = 1.00, maxscaleRelUnc = 0.00;
   double minpdfRelUnc = 1.00, maxpdfRelUnc = 0.00;
   double minisrRelUnc = 1.00, maxisrRelUnc = 0.00;
   double minmetMagRelUnc = 1.00, maxmetMagRelUnc = 0.00;
   double minmetPhiRelUnc = 1.00, maxmetPhiRelUnc = 0.00;
   double minjetJECRelUnc = 1.00, maxjetJECRelUnc = 0.00;
   double minbtagSFRelUnc = 1.00, maxbtagSFRelUnc = 0.00;
   double minmistagSFRelUnc = 1.00, maxmistagSFRelUnc = 0.00;
   double mintrigRelUnc = 1.00, maxtrigRelUnc = 0.00;
   double minlepVetoUnc = 1.00, maxlepVetoUnc = 0.00;

   double mingenTopSFUnc = 1.00, maxgenTopSFUnc = 0.00;
   double minmistaggenTopSFUnc = 1.00, maxmistaggenTopSFUnc = 0.00;

   double minhadtauCont = 1.00, maxhadtauCont = 0.00;
   double minlostleCont = 1.00, maxlostleCont = 0.00;
   double mintotCont = 1.00, maxtotCont = 0.00;

   TFile * study_signal_file = new TFile("study_signal.root", "RECREATE");

   TH2D * h2_acc_baseline = new TH2D("acc_baseline", "acceptance after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_stat_baseline = new TH2D("stat_baseline", "stat after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_syst_baseline = new TH2D("syst_baseline", "syst after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_btagSF_baseline = new TH2D("btagSF_baseline", "btagSF after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_mistagSF_baseline = new TH2D("mistagSF_baseline", "mistagSF after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_pdfUnc_baseline = new TH2D("pdfUnc_baseline", "pdfUnc after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_scaleUnc_baseline = new TH2D("scaleUnc_baseline", "scaleUnc after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_isrUnc_baseline = new TH2D("isrUnc_baseline", "isrUnc after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_metMag_baseline = new TH2D("metMag_baseline", "metMag after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_jetJEC_baseline = new TH2D("jetJEC_baseline", "jetJEC after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_lepVetoUnc_baseline = new TH2D("lepVetoUnc_baseline", "lepVetoUnc after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_genTopSF_baseline = new TH2D("genTopSF_baseline", "genTopSF after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);
   TH2D * h2_mistaggenTopSF_baseline = new TH2D("mistaggenTopSF_baseline", "mistaggenTopSF after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);

   TH2D * h2_contam_baseline = new TH2D("contam_baseline", "contam after baseline cuts", nXbins, loX, hiX, nYbins, loY, hiY);

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

      TString trigUncUp_str = "baseline_trigUncUp_" + v_mStop + "_" + v_mLSP;
      TString trigUncCen_str = "baseline_trigUncCen_" + v_mStop + "_" + v_mLSP;
      TString trigUncDn_str = "baseline_trigUncDn_" + v_mStop + "_" + v_mLSP;

      TString hadtauCont_str = "hPredYields_" + v_mStop + "_" + v_mLSP;
      TString lostleCont_str = "h_signal_pred_" + v_mStop + "_" + v_mLSP;

      TString muVetoed_str = "baseline_muVeto_" + v_mStop + "_" + v_mLSP;
      TString eleVetoed_str = "baseline_eleVeto_" + v_mStop + "_" + v_mLSP;
      TString isoLepTrkVetoed_str = "baseline_isoLepTrkVeto_" + v_mStop + "_" + v_mLSP;
      TString isoPionTrkVetoed_str = "baseline_isoPionTrkVeto_" + v_mStop + "_" + v_mLSP;

      TString genTopSFUp_str = "baseline_genTopSFUp_" + v_mStop + "_" + v_mLSP;
      TString genTopSFCen_str = "baseline_genTopSFCen_" + v_mStop + "_" + v_mLSP;
      TString genTopSFDn_str = "baseline_genTopSFDn_" + v_mStop + "_" + v_mLSP;

      TString mistaggenTopSFUp_str = "baseline_mistaggenTopSFUp_" + v_mStop + "_" + v_mLSP;
      TString mistaggenTopSFDn_str = "baseline_mistaggenTopSFDn_" + v_mStop + "_" + v_mLSP;

      TH1D * h1_scaleUncUp = (TH1D*) sig_file->Get(scaleUncUp_str);
      TH1D * h1_scaleUncDn = (TH1D*) sig_file->Get(scaleUncDn_str);

      TH1D * h1_pdfUncUp = (TH1D*) sig_file->Get(pdfUncUp_str);
      TH1D * h1_pdfUncCen = (TH1D*) sig_file->Get(pdfUncCen_str);
      TH1D * h1_pdfUncDn = (TH1D*) sig_file->Get(pdfUncDn_str);

      TH1D * h1_isrUncUp = (TH1D*) sig_file->Get(isrUncUp_str);
      TH1D * h1_isrUncDn = (TH1D*) sig_file->Get(isrUncDn_str);

      TH1D * h1_metMagUp = (TH1D*) sig_file->Get(metMagUp_str);
      TH1D * h1_metMagDn = (TH1D*) sig_file->Get(metMagDn_str);

      TH1D * h1_metPhiUp = (TH1D*) sig_file->Get(metPhiUp_str);
      TH1D * h1_metPhiDn = (TH1D*) sig_file->Get(metPhiDn_str);

      TH1D * h1_jetJECUp = (TH1D*) sig_file->Get(jetJECUp_str);
      TH1D * h1_jetJECDn = (TH1D*) sig_file->Get(jetJECDn_str);

      TH1D * h1_btagSFUp = (TH1D*) sig_file->Get(btagSFUp_str);
      TH1D * h1_btagSFCen = (TH1D*) sig_file->Get(btagSFCen_str);
      TH1D * h1_btagSFDn = (TH1D*) sig_file->Get(btagSFDn_str);

      TH1D * h1_mistagSFUp = (TH1D*) sig_file->Get(mistagSFUp_str);
      TH1D * h1_mistagSFDn = (TH1D*) sig_file->Get(mistagSFDn_str);

      TH1D * h1_trigUncUp = (TH1D*) sig_file->Get(trigUncUp_str);
      TH1D * h1_trigUncCen = (TH1D*) sig_file->Get(trigUncCen_str);
      TH1D * h1_trigUncDn = (TH1D*) sig_file->Get(trigUncDn_str);

      TH1D * h1_muVetoed = (TH1D*) sig_file->Get(muVetoed_str);
      TH1D * h1_eleVetoed = (TH1D*) sig_file->Get(eleVetoed_str);
      TH1D * h1_isoLepTrkVetoed = (TH1D*) sig_file->Get(isoLepTrkVetoed_str);
      TH1D * h1_isoPionTrkVetoed = (TH1D*) sig_file->Get(isoPionTrkVetoed_str);

      TH1D * h1_genTopSFUp = (TH1D*) sig_file->Get(genTopSFUp_str);
      TH1D * h1_genTopSFCen = (TH1D*) sig_file->Get(genTopSFCen_str);
      TH1D * h1_genTopSFDn = (TH1D*) sig_file->Get(genTopSFDn_str);

      TH1D * h1_mistaggenTopSFUp = (TH1D*) sig_file->Get(mistaggenTopSFUp_str);
      TH1D * h1_mistaggenTopSFDn = (TH1D*) sig_file->Get(mistaggenTopSFDn_str);

      TH1D * h1_hadtauCont =0, * h1_lostleCont =0;
      if( sig_cont_hadtau_file ) h1_hadtauCont = (TH1D*) sig_cont_hadtau_file->Get(hadtauCont_str);
      if( sig_cont_lostle_file ) h1_lostleCont = (TH1D*) sig_cont_lostle_file->Get(lostleCont_str);

      double xSec = 0, xSecErr = 0;
      if( sampleKeyStr == "T2tt" || sampleKeyStr == "T2tb" || sampleKeyStr == "T2bb" || sampleKeyStr == "T6ttWW" ){
         if( xSecMap.find(mStop) != xSecMap.end() ){
            xSec = xSecMap.find(mStop)->second;
            xSecErr = xSecErrMap.find(mStop)->second * xSec;   
         }
      }
      if( sampleKeyStr == "T1tttt" || sampleKeyStr == "T5ttcc" || sampleKeyStr == "T5tttt_degen" || sampleKeyStr == "T5ttttDM175" || sampleKeyStr == "T1ttbb"){
         if( xSecMap_glgl.find(mStop) != xSecMap_glgl.end() ){
            xSec = xSecMap_glgl.find(mStop)->second;
            xSecErr = xSecErrMap_glgl.find(mStop)->second * xSec;   
         }
      }

      char tmpStr[200], tmpStr2[200];
      sprintf(tmpStr, "totEntries_%d_%d", mStop, mLSP);
      TH1D * h1_totEntries = (TH1D*) sig_file->Get(tmpStr);

      double totEntries = h1_totEntries->GetBinContent(1);
      double totEntriesErr = sqrt(totEntries);

      double lumi_scale = dataLumi*xSec/totEntries;

      int nBins = h1_nSearchBin->GetXaxis()->GetNbins();
      if( nBins != nTotBins ){
         std::cout<<"\nWARNING ... nBins from nSearchBin hist : "<<nBins<<"  NOT EQUAL  nTotBins : "<<nTotBins<<"! forcing nBins = nTotBins ..."<<std::endl<<std::endl;
         nBins = nTotBins;
      }
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
      std::vector<double> trigUncUpVec(nBins), trigUncDnVec(nBins);
      std::vector<double> lepVetoUncUpVec(nBins), lepVetoUncDnVec(nBins);

      std::vector<double> genTopSFUpVec(nBins), genTopSFDnVec(nBins);
      std::vector<double> mistaggenTopSFUpVec(nBins), mistaggenTopSFDnVec(nBins);

      std::vector<double> hadtauContVec(nBins), lostleContVec(nBins), totContVec(nBins);

      double sum_scaleUncUp = 0, sum_scaleUncDn = 0;
      double sum_pdfUncUp = 0, sum_pdfUncCen = 0, sum_pdfUncDn = 0;
      double sum_cent = 0;
      for(int ib=0; ib<nBins; ib++){
         double cent = h1_nSearchBin->GetBinContent(ib+1); sum_cent += cent;

         double pdfUncUp_cent = h1_pdfUncUp->GetBinContent(ib+1); sum_pdfUncUp += pdfUncUp_cent;
         double pdfUncCen_cent = h1_pdfUncCen->GetBinContent(ib+1); sum_pdfUncCen += pdfUncCen_cent;
         double pdfUncDn_cent = h1_pdfUncDn->GetBinContent(ib+1); sum_pdfUncDn += pdfUncDn_cent;

         double scaleUncUp_cent = h1_scaleUncUp->GetBinContent(ib+1); sum_scaleUncUp += scaleUncUp_cent;
         double scaleUncDn_cent = h1_scaleUncDn->GetBinContent(ib+1); sum_scaleUncDn += scaleUncDn_cent;
      }

      double sum_cent_wt = 0, sum_err_wt =0;
      double stat_ACCwt = 0, bTagSF_ACCwt = 0, mistagSF_ACCwt =0, pdfUnc_ACCwt = 0, scaleUnc_ACCwt = 0, isrUnc_ACCwt = 0, metMag_ACCwt = 0, jetJEC_ACCwt = 0, lepVetoUnc_ACCwt = 0, genTopSF_ACCwt = 0, mistaggenTopSF_ACCwt = 0;
      double syst_ACCwt = 0;
      double contam_ACCwt = 0;
      for(int ib=0; ib<nBins; ib++){
         double cent = h1_nSearchBin->GetBinContent(ib+1);
         double err = h1_nSearchBin->GetBinError(ib+1);
         double rel_err = 0; if( cent!=0 ) rel_err = err/cent;
         rateVec[ib] = cent; statUncVec[ib] = rel_err; systUncVec[ib] = 0.2;

         double trigUncCen_cent = h1_trigUncCen->GetBinContent(ib+1);
         double trigUncCen_scale = cent !=0 ? trigUncCen_cent/cent : 1.0;

         double trigUncUp_cent = h1_trigUncUp->GetBinContent(ib+1);
         double trigUncDn_cent = h1_trigUncDn->GetBinContent(ib+1);
         double trigUncUp_relUnc = trigUncCen_cent !=0 ? trigUncUp_cent/trigUncCen_cent -1 : 0;
         double trigUncDn_relUnc = trigUncCen_cent !=0 ? 1 - trigUncDn_cent/trigUncCen_cent : 0;
         trigUncUpVec[ib] = trigUncUp_relUnc; trigUncDnVec[ib] = trigUncDn_relUnc;

         double genTopSFUp_cent = h1_genTopSFUp->GetBinContent(ib+1);
         double genTopSFCen_cent = h1_genTopSFCen->GetBinContent(ib+1);
         double genTopSFDn_cent = h1_genTopSFDn->GetBinContent(ib+1);
         double genTopSFCen_scale = cent !=0 ? genTopSFCen_cent/cent : 1.0;
         double genTopSFUp_relUnc = genTopSFCen_cent !=0 ? genTopSFUp_cent/genTopSFCen_cent : 0;
         double genTopSFDn_relUnc = genTopSFCen_cent !=0 ? genTopSFDn_cent/genTopSFCen_cent : 0;
         genTopSFUpVec[ib] = genTopSFUp_relUnc; genTopSFDnVec[ib] = genTopSFDn_relUnc;

         double mistaggenTopSFUp_cent = h1_mistaggenTopSFUp->GetBinContent(ib+1);
         double mistaggenTopSFDn_cent = h1_mistaggenTopSFDn->GetBinContent(ib+1);
         double mistaggenTopSFUp_relUnc = cent != 0 ? mistaggenTopSFUp_cent/cent : 0;
         double mistaggenTopSFDn_relUnc = cent != 0 ? mistaggenTopSFDn_cent/cent : 0;
         mistaggenTopSFUpVec[ib] = mistaggenTopSFUp_relUnc; mistaggenTopSFDnVec[ib] = mistaggenTopSFDn_relUnc;

         double bTagSFCen_cent = h1_btagSFCen->GetBinContent(ib+1);
         double bTagSFCen_scale = cent !=0 ? bTagSFCen_cent/cent : 1.0;
//         rate_scaleVec[ib] = lumi_scale * bTagSFCen_scale * trigUncCen_scale;
         rate_scaleVec[ib] = lumi_scale * bTagSFCen_scale * trigUncCen_scale * genTopSFCen_scale;
         sum_cent_wt += cent * bTagSFCen_scale * trigUncCen_scale * genTopSFCen_scale;
         sum_err_wt += err*err*pow(bTagSFCen_scale * trigUncCen_scale * genTopSFCen_scale, 2.0); 

         double bTagSFUp_cent = h1_btagSFUp->GetBinContent(ib+1);
         double bTagSFDn_cent = h1_btagSFDn->GetBinContent(ib+1);
         double bTagSFUp_relUnc = bTagSFCen_cent !=0 ? bTagSFUp_cent/bTagSFCen_cent -1 : 0;
         double bTagSFDn_relUnc = bTagSFCen_cent !=0 ? 1 - bTagSFDn_cent/bTagSFCen_cent : 0;
         bTagSFUpVec[ib] = bTagSFUp_relUnc; bTagSFDnVec[ib] = bTagSFDn_relUnc;

         double mistagSFUp_cent = h1_mistagSFUp->GetBinContent(ib+1);
         double mistagSFDn_cent = h1_mistagSFDn->GetBinContent(ib+1);
         double mistagSFUp_relUnc = bTagSFCen_cent !=0 ? mistagSFUp_cent/bTagSFCen_cent -1 : 0;
         double mistagSFDn_relUnc = bTagSFCen_cent !=0 ? 1 - mistagSFDn_cent/bTagSFCen_cent : 0;
         mistagSFUpVec[ib] = mistagSFUp_relUnc; mistagSFDnVec[ib] = mistagSFDn_relUnc;

         double pdfUncUp_cent = h1_pdfUncUp->GetBinContent(ib+1);
         double pdfUncCen_cent = h1_pdfUncCen->GetBinContent(ib+1);
         double pdfUncDn_cent = h1_pdfUncDn->GetBinContent(ib+1);
         double pdfUncUp_relUnc = pdfUncCen_cent !=0 ? pdfUncUp_cent/pdfUncCen_cent -1 : 0;
         double pdfUncDn_relUnc = pdfUncCen_cent !=0 ? 1 - pdfUncDn_cent/pdfUncCen_cent : 0;
         pdfUncUpVec[ib] = pdfUncUp_relUnc; pdfUncDnVec[ib] = pdfUncDn_relUnc;
         if( std::abs(pdfUncUpVec[ib]) > 1 || std::abs(pdfUncDnVec[ib]) > 1 ){
//            std::cout<<"\nv_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  sum_pdfUncCen : "<<sum_pdfUncCen<<"  sum_pdfUncUp : "<<sum_pdfUncUp<<"  sum_pdfUncDn : "<<sum_pdfUncDn<<"  ==>  pdfUncUpVec[ib] : "<<pdfUncUpVec[ib]<<"  pdfUncDnVec[ib] : "<<pdfUncDnVec[ib]<<std::endl;
         }
// Shape uncertainty only for PDF
         pdfUncUpVec[ib] = (pdfUncUpVec[ib] + 1) * (sum_pdfUncCen/sum_pdfUncUp) -1;
         pdfUncDnVec[ib] = 1 - (1 - pdfUncDnVec[ib]) * (sum_pdfUncCen/sum_pdfUncDn);

         double scaleUncUp_cent = h1_scaleUncUp->GetBinContent(ib+1);
         double scaleUncDn_cent = h1_scaleUncDn->GetBinContent(ib+1);
         double scaleUncUp_relUnc = cent !=0 ? scaleUncUp_cent/cent -1 : 0;
         double scaleUncDn_relUnc = cent !=0 ? 1 - scaleUncDn_cent/cent : 0;
         scaleUncUpVec[ib] = scaleUncUp_relUnc; scaleUncDnVec[ib] = scaleUncDn_relUnc;
// Shape uncertainty only for scale
         scaleUncUpVec[ib] = (scaleUncUpVec[ib] + 1) * (sum_cent/sum_scaleUncUp) -1;
         scaleUncDnVec[ib] = 1 - (1 - scaleUncDnVec[ib]) * (sum_cent/sum_scaleUncDn);

         double isrUncUp_cent = h1_isrUncUp->GetBinContent(ib+1);
         double isrUncDn_cent = h1_isrUncDn->GetBinContent(ib+1);
         double isrUncUp_relUnc = cent !=0 ? isrUncUp_cent/cent -1 : 0;
         double isrUncDn_relUnc = cent !=0 ? 1 - isrUncDn_cent/cent : 0;
         isrUncUpVec[ib] = isrUncUp_relUnc; isrUncDnVec[ib] = isrUncDn_relUnc;

         double metMagUp_cent = h1_metMagUp->GetBinContent(ib+1);
         double metMagDn_cent = h1_metMagDn->GetBinContent(ib+1);
         double metMagUp_relUnc = cent !=0 ? metMagUp_cent/cent -1 : 0;
         double metMagDn_relUnc = cent !=0 ? 1 - metMagDn_cent/cent : 0;
         metMagUpVec[ib] = metMagUp_relUnc; metMagDnVec[ib] = metMagDn_relUnc;

         double metPhiUp_cent = h1_metPhiUp->GetBinContent(ib+1);
         double metPhiDn_cent = h1_metPhiDn->GetBinContent(ib+1);
         double metPhiUp_relUnc = cent !=0 ? metPhiUp_cent/cent -1 : 0;
         double metPhiDn_relUnc = cent !=0 ? 1 - metPhiDn_cent/cent : 0;
         metPhiUpVec[ib] = metPhiUp_relUnc; metPhiDnVec[ib] = metPhiDn_relUnc;

         double jetJECUp_cent = h1_jetJECUp->GetBinContent(ib+1);
         double jetJECDn_cent = h1_jetJECDn->GetBinContent(ib+1);
         double jetJECUp_relUnc = cent !=0 ? jetJECUp_cent/cent -1 : 0;
         double jetJECDn_relUnc = cent !=0 ? 1 - jetJECDn_cent/cent : 0;
         jetJECUpVec[ib] = jetJECUp_relUnc; jetJECDnVec[ib] = jetJECDn_relUnc;

         double muVetoed_cent = h1_muVetoed->GetBinContent(ib+1);
         double eleVetoed_cent = h1_eleVetoed->GetBinContent(ib+1);
         double isoLepTrkVetoed_cent = h1_isoLepTrkVetoed->GetBinContent(ib+1);
         double isoPionTrkVetoed_cent = h1_isoPionTrkVetoed->GetBinContent(ib+1);

// ] mu and electron efficiency syst. are assumed to be 1% (https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSLeptonSF)
// ] mu and electron track syst. are assumed to be 10% (AN2015_003_v15 table 14): average mu and electron assuming equal amount
//   , i.e., 0.5*(8% + 15%) ~ 11.5% -> approx. 10%
// ] pion track syst. are: sqrt( 2%*2% + 0.2%*0.2% + 3%*3% + 3.9%*3.9% ) = 5.3% -> approx. 5%
         double muVetoed_var = 0.01 * muVetoed_cent;
         double eleVetoed_var = 0.01 * eleVetoed_cent;
         double isoLepTrkVetoed_var = 0.1 * isoLepTrkVetoed_cent; 
         double isoPionTrkVetoed_var = 0.05 * isoPionTrkVetoed_cent; 
         double sumVetoed_var = sqrt( muVetoed_var*muVetoed_var + eleVetoed_var*eleVetoed_var + isoLepTrkVetoed_var*isoLepTrkVetoed_var + isoPionTrkVetoed_var*isoPionTrkVetoed_var );
//         double tot_noLepVetoed_cent = cent + muVetoed_cent + eleVetoed_cent + isoLepTrkVetoed_cent + isoPionTrkVetoed_cent;
         lepVetoUncUpVec[ib] = cent !=0 ? sumVetoed_var/cent : 0;
         lepVetoUncDnVec[ib] = cent !=0 ? sumVetoed_var/cent : 0;
//         lepVetoUncUpVec[ib] = 0.02; lepVetoUncDnVec[ib] = 0.02;
         if( sampleKeyStr == "T1tttt" || sampleKeyStr == "T5ttcc" || sampleKeyStr == "T5tttt_degen" || sampleKeyStr == "T5ttttDM175" || sampleKeyStr == "T1ttbb"){
            if( lepVetoUncUpVec[ib] > 0.039 || lepVetoUncDnVec[ib] > 0.039 ){
               lepVetoUncUpVec[ib] = 0.039; lepVetoUncDnVec[ib] = 0.039;
            }
         }else{
            if( lepVetoUncUpVec[ib] > 0.033 || lepVetoUncDnVec[ib] > 0.033 ){
//               std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  rate : "<<cent<<"  muVetoed_cent : "<<muVetoed_cent<<"  eleVetoed_cent : "<<eleVetoed_cent<<"  isoLepTrkVetoed_cent : "<<isoLepTrkVetoed_cent<<"  isoPionTrkVetoed_cent : "<<isoPionTrkVetoed_cent<<"  lepVetoUncUpVec : "<<lepVetoUncUpVec[ib]<<std::endl;
               lepVetoUncUpVec[ib] = 0.033; lepVetoUncDnVec[ib] = 0.033;
            }
         }

         double hadtauCont_cent = 0.0, lostleCont_cent =0.0, totCont_cent =0.0;
         if( h1_hadtauCont ) hadtauCont_cent = h1_hadtauCont->GetBinContent(ib+1);
         if( h1_lostleCont ) lostleCont_cent = h1_lostleCont->GetBinContent(ib+1);

//         if( h1_hadtauCont && !h1_lostleCont ){ lostleCont_cent = hadtauCont_cent; if( ib==0 ) std::cout<<"Missing lostle signal contamination for v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<std::endl; }
//         if( !h1_hadtauCont && h1_lostleCont ){ hadtauCont_cent = lostleCont_cent; if( ib==0 ) std::cout<<"Missing hadtau signal contamination for v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<std::endl; }

         totCont_cent = hadtauCont_cent + lostleCont_cent;
         double hadtauCont_rel = cent!=0 ? hadtauCont_cent/cent : 0;
         double lostleCont_rel = cent!=0 ? lostleCont_cent/cent : 0;
         double totCont_rel = cent!=0 ? totCont_cent/cent : 0;
         if( totCont_rel >= 1.0 || cent == 0 ){
            std::cout<<"v_mStop : "<<v_mStop<<"  v_mLSP : "<<v_mLSP<<"  ib : "<<ib<<"  rate : "<<cent<<"  hadtauCont_cent : "<<hadtauCont_cent<<"  lostleCont_cent : "<<lostleCont_cent<<"  totCont_cent : "<<totCont_cent<<std::endl;
            if( hadtauCont_rel >= 1.0 ) hadtauCont_rel = 1.0;
            if( lostleCont_rel >= 1.0 ) lostleCont_rel = 1.0;
            totCont_rel = 1.0;
            totCont_cent = cent;
//            totCont_cent = round(cent); // this is for when cent is not integer
         }
         hadtauContVec[ib] = hadtauCont_cent; lostleContVec[ib] = lostleCont_cent; totContVec[ib] = totCont_cent;

         double perBin_acc_wt = cent * bTagSFCen_scale * trigUncCen_scale * genTopSFCen_scale / totEntries;

         stat_ACCwt += rel_err * perBin_acc_wt;
         double avg_abs_bTagSF_relUnc = (std::abs(bTagSFUp_relUnc) + std::abs(bTagSFDn_relUnc))*0.5;
         bTagSF_ACCwt += avg_abs_bTagSF_relUnc * perBin_acc_wt;
         double avg_abs_mistagSF_relUnc = (std::abs(mistagSFUp_relUnc) + std::abs(mistagSFDn_relUnc))*0.5;
         mistagSF_ACCwt += avg_abs_mistagSF_relUnc * perBin_acc_wt;
         double avg_abs_pdfUnc_relUnc = (std::abs(pdfUncUp_relUnc) + std::abs(pdfUncDn_relUnc))*0.5;
         pdfUnc_ACCwt += avg_abs_pdfUnc_relUnc * perBin_acc_wt;
         double avg_scaleUnc_relUnc = (std::abs(scaleUncUpVec[ib]) + std::abs(scaleUncDnVec[ib]))*0.5;
         scaleUnc_ACCwt += avg_scaleUnc_relUnc * perBin_acc_wt;
         double avg_isrUnc_relUnc = (std::abs(isrUncUp_relUnc) + std::abs(isrUncDn_relUnc))*0.5;
         isrUnc_ACCwt += avg_isrUnc_relUnc * perBin_acc_wt;
         double avg_metMag_relUnc = (std::abs(metMagUp_relUnc) + std::abs(metMagDn_relUnc))*0.5;
         metMag_ACCwt += avg_metMag_relUnc * perBin_acc_wt;
         double avg_jetJEC_relUnc = (std::abs(jetJECUp_relUnc) + std::abs(jetJECDn_relUnc))*0.5;
         jetJEC_ACCwt += avg_jetJEC_relUnc * perBin_acc_wt;
         double avg_lepVetoUnc_relUnc = (std::abs(lepVetoUncUpVec[ib]) + std::abs(lepVetoUncDnVec[ib]))*0.5;
         lepVetoUnc_ACCwt += avg_lepVetoUnc_relUnc * perBin_acc_wt;
         double avg_genTopSF_relUnc = (genTopSFUp_relUnc + genTopSFDn_relUnc)*0.5;
         genTopSF_ACCwt += avg_genTopSF_relUnc * perBin_acc_wt;
         double avg_mistaggenTopSFUp_relUnc = (mistaggenTopSFUp_relUnc + mistaggenTopSFDn_relUnc)*0.5;
         mistaggenTopSF_ACCwt += avg_mistaggenTopSFUp_relUnc * perBin_acc_wt;

// Added 0.05 for data/MC difference for top tagger; 0.027 is for lumi unc.
         syst_ACCwt += sqrt(avg_abs_bTagSF_relUnc*avg_abs_bTagSF_relUnc + avg_abs_mistagSF_relUnc*avg_abs_mistagSF_relUnc + avg_scaleUnc_relUnc*avg_scaleUnc_relUnc + avg_isrUnc_relUnc*avg_isrUnc_relUnc + avg_metMag_relUnc*avg_metMag_relUnc + avg_jetJEC_relUnc*avg_jetJEC_relUnc + avg_lepVetoUnc_relUnc*avg_lepVetoUnc_relUnc + avg_genTopSF_relUnc*avg_genTopSF_relUnc + avg_mistaggenTopSFUp_relUnc*avg_mistaggenTopSFUp_relUnc
 + 0.05 * 0.05 + 0.027 * 0.027 ) * perBin_acc_wt;

         contam_ACCwt += totCont_rel * perBin_acc_wt;

// Summarizing all the uncertainty ranges
         if( minhadtauCont > hadtauCont_rel ) minhadtauCont = hadtauCont_rel; if( maxhadtauCont < hadtauCont_rel ) maxhadtauCont = hadtauCont_rel;
         if( minlostleCont > lostleCont_rel ) minlostleCont = lostleCont_rel; if( maxlostleCont < lostleCont_rel ) maxlostleCont = lostleCont_rel;
         if( mintotCont > totCont_rel ) mintotCont = totCont_rel; if( maxtotCont < totCont_rel ) maxtotCont = totCont_rel;

         if( minstatRelUnc > rel_err ) minstatRelUnc = rel_err; if( maxstatRelUnc < rel_err ) maxstatRelUnc = rel_err;

         if( mingenTopSFUnc > genTopSFUp_relUnc ) mingenTopSFUnc = genTopSFUp_relUnc; if( maxgenTopSFUnc < genTopSFUp_relUnc ) maxgenTopSFUnc = genTopSFUp_relUnc;
         if( mingenTopSFUnc > genTopSFDn_relUnc ) mingenTopSFUnc = genTopSFDn_relUnc; if( maxgenTopSFUnc < genTopSFDn_relUnc ) maxgenTopSFUnc = genTopSFDn_relUnc;

         if( minmistaggenTopSFUnc > mistaggenTopSFUp_relUnc ) minmistaggenTopSFUnc = mistaggenTopSFUp_relUnc; if( maxmistaggenTopSFUnc < mistaggenTopSFUp_relUnc ) maxmistaggenTopSFUnc = mistaggenTopSFUp_relUnc;
         if( minmistaggenTopSFUnc > mistaggenTopSFDn_relUnc ) minmistaggenTopSFUnc = mistaggenTopSFDn_relUnc; if( maxmistaggenTopSFUnc < mistaggenTopSFDn_relUnc ) maxmistaggenTopSFUnc = mistaggenTopSFDn_relUnc;

         if( rateVec[ib] > nEvtToRange ){
            if( minlepVetoUnc > lepVetoUncUpVec[ib] ) minlepVetoUnc = lepVetoUncUpVec[ib]; if( maxlepVetoUnc < lepVetoUncUpVec[ib] ) maxlepVetoUnc = lepVetoUncUpVec[ib]; 
         }

// btag, mistag and isr seem well behaviored even with number of signal events less than 5.
         if( minbtagSFRelUnc > std::abs(bTagSFUp_relUnc) ) minbtagSFRelUnc = std::abs(bTagSFUp_relUnc); if( maxbtagSFRelUnc < std::abs(bTagSFUp_relUnc) ) maxbtagSFRelUnc = std::abs(bTagSFUp_relUnc);
         if( minbtagSFRelUnc > std::abs(bTagSFDn_relUnc) ) minbtagSFRelUnc = std::abs(bTagSFDn_relUnc); if( maxbtagSFRelUnc < std::abs(bTagSFDn_relUnc) ) maxbtagSFRelUnc = std::abs(bTagSFDn_relUnc);

         if( minmistagSFRelUnc > std::abs(mistagSFUp_relUnc) ) minmistagSFRelUnc = std::abs(mistagSFUp_relUnc); if( maxmistagSFRelUnc < std::abs(mistagSFUp_relUnc) ) maxmistagSFRelUnc = std::abs(mistagSFUp_relUnc);
         if( minmistagSFRelUnc > std::abs(mistagSFDn_relUnc) ) minmistagSFRelUnc = std::abs(mistagSFDn_relUnc); if( maxmistagSFRelUnc < std::abs(mistagSFDn_relUnc) ) maxmistagSFRelUnc = std::abs(mistagSFDn_relUnc);

         if( minisrRelUnc > std::abs(isrUncUp_relUnc) ) minisrRelUnc = std::abs(isrUncUp_relUnc); if( maxisrRelUnc < std::abs(isrUncUp_relUnc) ) maxisrRelUnc = std::abs(isrUncUp_relUnc);
         if( minisrRelUnc > std::abs(isrUncDn_relUnc) ) minisrRelUnc = std::abs(isrUncDn_relUnc); if( maxisrRelUnc < std::abs(isrUncDn_relUnc) ) maxisrRelUnc = std::abs(isrUncDn_relUnc);

         if( mintrigRelUnc > std::abs(trigUncUp_relUnc) ) mintrigRelUnc = std::abs(trigUncUp_relUnc); if( maxtrigRelUnc < std::abs(trigUncUp_relUnc) ) maxtrigRelUnc = std::abs(trigUncUp_relUnc);
         if( mintrigRelUnc > std::abs(trigUncDn_relUnc) ) mintrigRelUnc = std::abs(trigUncDn_relUnc); if( maxtrigRelUnc < std::abs(trigUncDn_relUnc) ) maxtrigRelUnc = std::abs(trigUncDn_relUnc);

         if( rateVec[ib] >= nEvtToRange ){
//         if( true ){
            if( minmetMagRelUnc > std::abs(metMagUp_relUnc) ) minmetMagRelUnc = std::abs(metMagUp_relUnc); if( maxmetMagRelUnc < std::abs(metMagUp_relUnc) ) maxmetMagRelUnc = std::abs(metMagUp_relUnc);
            if( minmetMagRelUnc > std::abs(metMagDn_relUnc) ) minmetMagRelUnc = std::abs(metMagDn_relUnc); if( maxmetMagRelUnc < std::abs(metMagDn_relUnc) ) maxmetMagRelUnc = std::abs(metMagDn_relUnc);

            if( minmetPhiRelUnc > std::abs(metPhiUp_relUnc) ) minmetPhiRelUnc = std::abs(metPhiUp_relUnc); if( maxmetPhiRelUnc < std::abs(metPhiUp_relUnc) ) maxmetPhiRelUnc = std::abs(metPhiUp_relUnc);
            if( minmetPhiRelUnc > std::abs(metPhiDn_relUnc) ) minmetPhiRelUnc = std::abs(metPhiDn_relUnc); if( maxmetPhiRelUnc < std::abs(metPhiDn_relUnc) ) maxmetPhiRelUnc = std::abs(metPhiDn_relUnc);

            if( minjetJECRelUnc > std::abs(jetJECUp_relUnc) ) minjetJECRelUnc = std::abs(jetJECUp_relUnc); if( maxjetJECRelUnc < std::abs(jetJECUp_relUnc) ) maxjetJECRelUnc = std::abs(jetJECUp_relUnc);
            if( minjetJECRelUnc > std::abs(jetJECDn_relUnc) ) minjetJECRelUnc = std::abs(jetJECDn_relUnc); if( maxjetJECRelUnc < std::abs(jetJECDn_relUnc) ) maxjetJECRelUnc = std::abs(jetJECDn_relUnc);

            if( minscaleRelUnc > std::abs(scaleUncUpVec[ib]) ) minscaleRelUnc = std::abs(scaleUncUpVec[ib]); if( maxscaleRelUnc < std::abs(scaleUncUpVec[ib]) ) maxscaleRelUnc = std::abs(scaleUncUpVec[ib]);
            if( minscaleRelUnc > std::abs(scaleUncDnVec[ib]) ) minscaleRelUnc = std::abs(scaleUncDnVec[ib]); if( maxscaleRelUnc < std::abs(scaleUncDnVec[ib]) ) maxscaleRelUnc = std::abs(scaleUncDnVec[ib]);

            if( minpdfRelUnc > std::abs(pdfUncUpVec[ib]) ) minpdfRelUnc = std::abs(pdfUncUpVec[ib]); if( maxpdfRelUnc < std::abs(pdfUncUpVec[ib]) ) maxpdfRelUnc = std::abs(pdfUncUpVec[ib]);
            if( minpdfRelUnc > std::abs(pdfUncDnVec[ib]) ) minpdfRelUnc = std::abs(pdfUncDnVec[ib]); if( maxpdfRelUnc < std::abs(pdfUncDnVec[ib]) ) maxpdfRelUnc = std::abs(pdfUncDnVec[ib]);
         }
      }
      sum_err_wt = sqrt(sum_err_wt);

      double perSig_acc_wt = sum_cent_wt/totEntries;
      stat_ACCwt /= perSig_acc_wt;
      bTagSF_ACCwt /= perSig_acc_wt;
      mistagSF_ACCwt /= perSig_acc_wt;
      pdfUnc_ACCwt /= perSig_acc_wt;
      scaleUnc_ACCwt /= perSig_acc_wt;
      isrUnc_ACCwt /= perSig_acc_wt;
      metMag_ACCwt /= perSig_acc_wt;
      jetJEC_ACCwt /= perSig_acc_wt;
      lepVetoUnc_ACCwt /= perSig_acc_wt;
      genTopSF_ACCwt /= perSig_acc_wt;
      mistaggenTopSF_ACCwt /= perSig_acc_wt;

      syst_ACCwt /= perSig_acc_wt;

      contam_ACCwt /= perSig_acc_wt;

      printf("signal_%d_%d (xSec : %5.3e +- %4.1f%) :   acc_wt : %4.2e  stat : %5.3f  bTagSF : %5.3f  mistagSF : %5.3f  pdfUnc : %5.3f  scaleUnc : %5.3f  isrUnc : %5.3f  metMag : %5.3f  jetJEC : %5.3f  lepVetoUnc : %5.3f  genTopSF : %5.3f  mistaggenTopSF : %5.3f  ->  syst : %5.3f  ->  contam : %5.3f\n", mStop, mLSP, xSec, xSecErr/xSec*100, perSig_acc_wt, stat_ACCwt, bTagSF_ACCwt, mistagSF_ACCwt, pdfUnc_ACCwt, scaleUnc_ACCwt, isrUnc_ACCwt, metMag_ACCwt, jetJEC_ACCwt, lepVetoUnc_ACCwt, genTopSF_ACCwt, mistaggenTopSF_ACCwt, syst_ACCwt, contam_ACCwt);

      int binXidx = h2_acc_baseline->GetXaxis()->FindBin(double(mStop));
      int binYidx = h2_acc_baseline->GetYaxis()->FindBin(double(mLSP));

// When filling, scale them by 100 to save effective digis
      h2_acc_baseline->SetBinContent(binXidx, binYidx, perSig_acc_wt*100);
      h2_stat_baseline->SetBinContent(binXidx, binYidx, stat_ACCwt*100);
      h2_syst_baseline->SetBinContent(binXidx, binYidx, syst_ACCwt*100);
      h2_btagSF_baseline->SetBinContent(binXidx, binYidx, bTagSF_ACCwt*100);
      h2_mistagSF_baseline->SetBinContent(binXidx, binYidx, mistagSF_ACCwt*100);
      h2_pdfUnc_baseline->SetBinContent(binXidx, binYidx, pdfUnc_ACCwt*100);
      h2_scaleUnc_baseline->SetBinContent(binXidx, binYidx, scaleUnc_ACCwt*100);
      h2_isrUnc_baseline->SetBinContent(binXidx, binYidx, isrUnc_ACCwt*100);
      h2_metMag_baseline->SetBinContent(binXidx, binYidx, metMag_ACCwt*100);
      h2_jetJEC_baseline->SetBinContent(binXidx, binYidx, jetJEC_ACCwt*100);
      h2_lepVetoUnc_baseline->SetBinContent(binXidx, binYidx, lepVetoUnc_ACCwt*100);
      h2_genTopSF_baseline->SetBinContent(binXidx, binYidx, genTopSF_ACCwt*100);
      h2_mistaggenTopSF_baseline->SetBinContent(binXidx, binYidx, mistaggenTopSF_ACCwt*100);

      h2_contam_baseline->SetBinContent(binXidx, binYidx, contam_ACCwt*100);

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
      
// Only total contamination makes sense since if either hadtau or lostle contamination larger than 
// signal yield (possible due to stat. flucturation), only can force total to be the signal yield.
      ofs<<"contam      = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.5f", totContVec[ib]);
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
//         sprintf(tmpStr, "%10.3f", 0.046);
         sprintf(tmpStr, "%10.3f", 0.027);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"lumi_unc_dn = ";
      for(int ib=0; ib<nBins; ib++){
//         sprintf(tmpStr, "%10.3f", 0.046);
         sprintf(tmpStr, "%10.3f", 0.027);
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

      ofs<<"trigUnc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", trigUncUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"trigUnc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", trigUncDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"lepVetoUnc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", lepVetoUncUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"lepVetoUnc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", lepVetoUncDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"genTopSF_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", genTopSFUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"genTopSF_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", genTopSFDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"mistaggenTopSF_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", mistaggenTopSFUpVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"mistaggenTopSF_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "% 10.3f", mistaggenTopSFDnVec[ib]);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"data_vs_MC_recoTop_unc_up = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", 0.05);
         ofs<<tmpStr;
      }
      ofs<<std::endl;

      ofs<<"data_vs_MC_recoTop_unc_dn = ";
      for(int ib=0; ib<nBins; ib++){
         sprintf(tmpStr, "%10.3f", 0.05);
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

   study_signal_file->Write(); study_signal_file->Close();

   std::cout<<std::endl;
   std::cout<<"minstatRelUnc : "<<minstatRelUnc<<"  maxstatRelUnc : "<<maxstatRelUnc<<std::endl;
   std::cout<<"minbtagSFRelUnc : "<<minbtagSFRelUnc<<"  maxbtagSFRelUnc : "<<maxbtagSFRelUnc<<std::endl;
   std::cout<<"minmistagSFRelUnc : "<<minmistagSFRelUnc<<"  maxmistagSFRelUnc : "<<maxmistagSFRelUnc<<std::endl;
   std::cout<<"minisrRelUnc : "<<minisrRelUnc<<"  maxisrRelUnc : "<<maxisrRelUnc<<std::endl;
   std::cout<<std::endl;
   std::cout<<"minscaleRelUnc : "<<minscaleRelUnc<<"  maxscaleRelUnc : "<<maxscaleRelUnc<<std::endl;
//   std::cout<<"minpdfRelUnc : "<<minpdfRelUnc<<"  maxpdfRelUnc : "<<maxpdfRelUnc<<std::endl;
   std::cout<<"minmetMagRelUnc : "<<minmetMagRelUnc<<"  maxmetMagRelUnc : "<<maxmetMagRelUnc<<std::endl;
//   std::cout<<"minmetPhiRelUnc : "<<minmetPhiRelUnc<<"  maxmetPhiRelUnc : "<<maxmetPhiRelUnc<<std::endl;
   std::cout<<"minjetJECRelUnc : "<<minjetJECRelUnc<<"  maxjetJECRelUnc : "<<maxjetJECRelUnc<<std::endl;
   std::cout<<"mintrigRelUnc : "<<mintrigRelUnc<<"  maxtrigRelUnc : "<<maxtrigRelUnc<<std::endl;
   std::cout<<"minlepVetoUnc : "<<minlepVetoUnc<<"  maxlepVetoUnc : "<<maxlepVetoUnc<<std::endl;
   std::cout<<std::endl;

   std::cout<<"mingenTopSFUnc : "<<mingenTopSFUnc<<"  maxgenTopSFUnc : "<<maxgenTopSFUnc<<std::endl;
   std::cout<<"minmistaggenTopSFUnc : "<<minmistaggenTopSFUnc<<"  maxmistaggenTopSFUnc : "<<maxmistaggenTopSFUnc<<std::endl;

   std::cout<<std::endl;
   std::cout<<"minhadtauCont : "<<minhadtauCont<<"  maxhadtauCont : "<<maxhadtauCont<<std::endl;
   std::cout<<"minlostleCont : "<<minlostleCont<<"  maxlostleCont : "<<maxlostleCont<<std::endl;
   std::cout<<"mintotCont : "<<mintotCont<<"  maxtotCont : "<<maxtotCont<<std::endl;

}

int main(int argc, char *argv[]){

   sb = new SearchBins();

   nTotBins = sb->nSearchBins();

   sb->build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   std::string inputRootName = "signalScan_SMS-T2tt.root";
   std::string inputRootName_hadtau_cont = "";
   std::string inputRootName_lostle_cont = "";
   if( argc >= 2 ) inputRootName = argv[1];
   if( argc >= 3 ) inputRootName_hadtau_cont = argv[2];
   if( argc >= 4 ) inputRootName_lostle_cont = argv[3];
   std::cout<<"\ninputRootName : "<<inputRootName.c_str()<<"  inputRootName_hadtau_cont : "<<inputRootName_hadtau_cont.c_str()<<"  inputRootName_lostle_cont : "<<inputRootName_lostle_cont.c_str()<<std::endl;
   makeSignalCards(inputRootName, inputRootName_hadtau_cont, inputRootName_lostle_cont);
}
