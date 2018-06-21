#include "NTupleReader.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib> 

#include "samples.h"
#include "customize.h"

#include "TStopwatch.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"

#include "TGraphAsymmErrors.h"

#include "tdrstyle.h"
#include "TPrincipal.h"

#include "TH2Poly.h"

#include "searchBins.h"

#include "baselineDef.h"

#include "basicHists.h"

#include "PDFUncertainty.h"

#include "xSec.h"

#include "BTagCorrector.h"

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "../TopTagger/interface/indexSort.h"

//#include "../SkimsAUX/plugins/MT2CalcCore.h"

#include "ISRCorrector.h"

SearchBins * sb =0;

std::shared_ptr<topTagger::type3TopTagger>type3Ptr(nullptr);
std::shared_ptr<TopTagger>ttPtr(nullptr);

int nTotBins;
std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

const bool doSingleMuonCS = false;
const bool doInvDphi = false;
const bool usegenmet = false;

const bool prep_btag_ISR = false;

//MT2CalcCore * mt2Calc;

BaselineVessel * SRblv =0;
std::string spec = "MY";

const std::string spec_jecUp = "jecUp";
BaselineVessel * SRblv_jecUp =0;

const std::string spec_jecDn = "jecDn";
BaselineVessel * SRblv_jecDn =0;

const std::string spec_metMagUp = "metMagUp";
BaselineVessel * SRblv_metMagUp =0;

const std::string spec_metMagDn = "metMagDn";
BaselineVessel * SRblv_metMagDn =0;

const std::string spec_metPhiUp = "metPhiUp";
BaselineVessel * SRblv_metPhiUp =0;

const std::string spec_metPhiDn = "metPhiDn";
BaselineVessel * SRblv_metPhiDn =0;

PDFUncertainty * pdfScale =0;

TFile * bTagEffFile =0;

// 0 is used to set to process all events
int entryToProcess = -1;

const bool debug = false;

std::vector<std::string> keyStringCachedVec;
std::vector<double> scaleMCCachedVec;
std::vector<int> colorCachedVec;

TStopwatch timer;

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH1D*> &h1_inputVec, const int nRebin =1, const TString optDrawStr ="", const TString optComStr ="");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2D*> &h2_inputVec, const TString optDrawStr = "", const TString optComStr = "");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2Poly*> &h2_inputVec, const TString optDrawStr = "", const TString optComStr = "");

bool find_mother(int momIdx, int dauIdx, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec);
int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);
double calcMT(const TLorentzVector &objLVec, const TLorentzVector &metLVec);

const AnaConsts::IsoAccRec hadtau_muonsMiniIsoArr = {   -1,       2.4,      20,     -1,       0.2,     -1  };

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

TH2D * mu_mediumID_SF = 0, * mu_miniISO_SF = 0;
TH2D * sf_mu_mediumID = 0, * sf_mu_mediumID_mini02 = 0;
TGraphAsymmErrors * mu_trkptGT10_SF = 0, * mu_trkptLT10_SF = 0;

TH2D * ele_VetoID_SF = 0, * ele_miniISO_SF = 0;
TH2D * sf_el_vetoCB = 0, * sf_el_mini01 = 0;
TH2D * ele_trkpt_SF = 0;

double GetTriggerEffWeight (const double met) {
   if (met<25) return 0.002031716;
   else if (met<50) return 0.00386317;
   else if (met<75) return 0.01100592;
   else if (met<100) return 0.04153341;
   else if (met<125) return 0.1467812;
   else if (met<150) return 0.3698591;
   else if (met<175) return 0.6441574;
   else if (met<200) return 0.8272891;
   else if (met<275) return 0.9388552;
   else if (met<400) return 0.9876937;
   else if (met<600) return 0.9931271;
   else if (met<1000) return 1.0;
   else return 1.0;
}

double GetTriggerEffStatUncHi (const double met) {
   if (met<25) return 0.0001223576;
   else if (met<50) return 0.0001237282;
   else if (met<75) return 0.0002381682;
   else if (met<100) return 0.0005877047;
   else if (met<125) return 0.001379334;
   else if (met<150) return 0.002475498;
   else if (met<175) return 0.003194178;
   else if (met<200) return 0.003284595;
   else if (met<275) return 0.001936995;
   else if (met<400) return 0.001661775;
   else if (met<600) return 0.00328642;
   else if (met<1000) return 0.00687;
   else return 0.00687;
}

double GetTriggerEffStatUncLo (const double met) {
   if (met<25) return 0.0001156004;
   else if (met<50) return 0.0001199594;
   else if (met<75) return 0.00023323;
   else if (met<100) return 0.000579911;
   else if (met<125) return 0.001368742;
   else if (met<150) return 0.002468708;
   else if (met<175) return 0.003206974;
   else if (met<200) return 0.003334201;
   else if (met<275) return 0.001995109;
   else if (met<400) return 0.00189928;
   else if (met<600) return 0.005400859;
   else if (met<1000) return 0.02576996; // would be from HT>1000 ...
   else return 0.02576996;
}

// Not available for Moriond2017 ...
double GetTriggerEffSystUncHi (const double met) {
   return 0.000;
}

double GetTriggerEffSystUncLo (const double met) {
   return 0.000;
}

/*
double GetTopPtFastToFullSF(const double genTopPt){
   if( genTopPt < 50 ) return 1.0000;
   else if (genTopPt < 100 ) return 0.986824;
   else if (genTopPt < 150 ) return 0.98483;
   else if (genTopPt < 200 ) return 0.982835;
   else if (genTopPt < 250 ) return 0.980841;
   else if (genTopPt < 300 ) return 0.978846;
   else if (genTopPt < 350 ) return 0.976851;
   else if (genTopPt < 400 ) return 0.974857;
   else if (genTopPt < 450 ) return 0.972862;
   else if (genTopPt < 500 ) return 0.970868;
   else if (genTopPt < 550 ) return 0.968873;
   else if (genTopPt < 600 ) return 0.966879;
   else if (genTopPt < 800 ) return 0.961892;
   else if (genTopPt < 1000 ) return 0.953914;
   else return 0.953914;
}

double GetTopPtFastToFullSF_relErr(const double genTopPt){
   if( genTopPt < 50 ) return 0.00;
   return 0.05;
}
*/

double GetTopPtFastToFullSF(const double genTopPt){
   if( genTopPt < 100 ) return 0.988;
   else if (genTopPt < 200 ) return 0.984;
   else if (genTopPt < 300 ) return 0.980;
   else if (genTopPt < 400 ) return 0.976;
   else if (genTopPt < 600 ) return 0.970;
   else if (genTopPt < 800 ) return 0.962;
   else if (genTopPt < 1000 ) return 0.954;
   else return 0.954;
}

// side 1 : up   -1 : dn
double GetTopPtFastToFullSF_relErr(const double genTopPt, const int side=1){
   if( side ==1 ){
      if( genTopPt < 100 ) return 0.061;
      else if( genTopPt < 200 ) return 0.089;
      else if( genTopPt < 300 ) return 0.044;
      else if( genTopPt < 400 ) return 0.013;
      else if( genTopPt < 600 ) return 0.011;
      else if( genTopPt < 800 ) return 0.005;
      else if( genTopPt < 1000 ) return 0.121;
      else return 0.121;
   }
   if( side ==-1 ){
      if( genTopPt < 100 ) return 0.136;
      else if( genTopPt < 200 ) return 0.039;
      else if( genTopPt < 300 ) return 0.011;
      else if( genTopPt < 400 ) return 0.028;
      else if( genTopPt < 600 ) return 0.074;
      else if( genTopPt < 800 ) return 0.012;
      else if( genTopPt < 1000 ) return 0.009;
      else return 0.009;
   }
   return 0.00;
}

// type is the following
// 1 : mono-jet
// 2 : di-jet
// 3 : tri-jet
double GetTopPtDataToMCSF(const double recoTopPt, const int type){
   if(type ==3 ){
      if(recoTopPt < 50) return 1.000;
      else if(recoTopPt < 100) return 1.014;
      else if(recoTopPt < 150) return 1.039;
      else if(recoTopPt < 200) return 1.063;
      else if(recoTopPt < 250) return 1.038;
      else if(recoTopPt < 300) return 1.052;
      else if(recoTopPt < 350) return 1.049;
      else if(recoTopPt < 400) return 0.998;
      else if(recoTopPt < 450) return 0.914;
      else if(recoTopPt < 1000 ) return 1.003;
      else return 1.003;
   }else if(type ==2){
      if(recoTopPt < 230) return 1.000;
      else if(recoTopPt < 300) return 0.898;
      else if(recoTopPt < 350) return 0.963;
      else if(recoTopPt < 400) return 0.999;
      else if(recoTopPt < 450) return 0.981;
      else if(recoTopPt < 500) return 0.992;
      else if(recoTopPt < 1000) return 0.944;
      else return 0.944;
   }else if(type ==1){
      if(recoTopPt < 400) return 1.000;
      else if(recoTopPt < 450) return 0.970;
      else if(recoTopPt < 500) return 0.964;
      else if(recoTopPt < 550) return 0.955;
      else if(recoTopPt < 600) return 1.046;
      else if(recoTopPt < 1000) return 1.055;
      else return 1.055;
   }
   return 1.0;
}

double GetTopPtDataToMCSF_absErr(const double recoTopPt, const int type){
   if(type ==3 ){
      if(recoTopPt < 50) return 0.00;
      else if(recoTopPt < 100) return 0.016;
      else if(recoTopPt < 150) return 0.008;
      else if(recoTopPt < 200) return 0.006;
      else if(recoTopPt < 250) return 0.008;
      else if(recoTopPt < 300) return 0.011;
      else if(recoTopPt < 350) return 0.018;
      else if(recoTopPt < 400) return 0.019;
      else if(recoTopPt < 450) return 0.045;
      else if(recoTopPt < 1000 ) return 0.063;
      else return 0.063;
   }else if(type ==2){
      if(recoTopPt < 230) return 0.000;
      else if(recoTopPt < 300) return 0.022;
      else if(recoTopPt < 350) return 0.020;
      else if(recoTopPt < 400) return 0.022;
      else if(recoTopPt < 450) return 0.047;
      else if(recoTopPt < 500) return 0.065;
      else if(recoTopPt < 1000) return 0.089;
      else return 0.089;
   }else if(type ==1){
      if(recoTopPt < 400) return 0.000;
      else if(recoTopPt < 450) return 0.031;
      else if(recoTopPt < 500) return 0.030;
      else if(recoTopPt < 550) return 0.044;
      else if(recoTopPt < 600) return 0.050;
      else if(recoTopPt < 1000) return 0.055;
      else return 0.055;
   }
   return 0.0;
}


class HistContainer
{
private:
    TH1* met_;
    TH1* mt2_;
    TH1* nt_;
    TH1* nb_;
    TH1* baseline_met_;
    TH1* baseline_mt2_;
    TH1* baseline_nt_;
    TH1* baseline_nb_;
    TH1* baseline_nSearchBin_;
    TH1* xSec_;
    TH1* totEntries_;

    TH1* baseline_met_Nm1_;
    TH1* baseline_mt2_Nm1_;
    TH1* baseline_nt_Nm1_;
    TH1* baseline_nb_Nm1_;

    TH1* cutFlow_;

    TH1* baseline_nTopEQ1_nSearchBin_;
    TH1* baseline_nTopLE2_nSearchBin_;

    TH1* baseline_genInfo_allHad_;
    TH1* baseline_genInfo_lostLep_;
    TH1* baseline_genInfo_hadTau_;

    TH1* baseline_nTopEQ1_genInfo_allHad_;
    TH1* baseline_nTopEQ1_genInfo_lostLep_;
    TH1* baseline_nTopEQ1_genInfo_hadTau_;

    TH1* baseline_nTopLE2_genInfo_allHad_;
    TH1* baseline_nTopLE2_genInfo_lostLep_;
    TH1* baseline_nTopLE2_genInfo_hadTau_;

    TH1* baseline_no_fastsimFilter_nSearchBin_;

    //Any events with any jet pT > 2 TeV
    TH1* baseline_weirdEvt_;

    //TH1* baseline_noLepVeto_;
    TH1* baseline_muVeto_;
    TH1* baseline_muVeto_SF_;
    TH1* baseline_muVeto_Up_;
    TH1* baseline_muVeto_Dn_;
    TH1* baseline_eleVeto_;
    TH1* baseline_eleVeto_SF_;
    TH1* baseline_eleVeto_Up_;
    TH1* baseline_eleVeto_Dn_;
    TH1* baseline_isoLepTrkVeto_;
    TH1* baseline_isoPionTrkVeto_;

    // For signal contamination
    TH1* baseline_muCS_;
    TH1* baseline_eleCS_;

    TH1* baseline_genTopSFUp_;
    TH1* baseline_genTopSFCen_;
    TH1* baseline_genTopSFDn_;

    TH1* baseline_recoTopSFUp_;
    TH1* baseline_recoTopSFCen_;
    TH1* baseline_recoTopSFDn_;

    TH1* baseline_mistaggenTopSFUp_;
    TH1* baseline_mistaggenTopSFDn_;

    TH1* baseline_nb_ngenbLE3_;

    TH1* genTopPt_;
    TH1* baseline_genTopPt_;
 
    TH1* baseline_misTag_recoTopPt_;

    TH2* sumStopPt_vs_met_;

    TH2* baseline_MT2_vs_met_;

    TH2* loose_MT2_vs_met_;
    TH2* loose_alt_MT2_vs_met_;
    TH2* loose_MT2_vs_alt_MT2_;

    TH1* baseline_trigUncUp_;
    TH1* baseline_trigUncCen_;
    TH1* baseline_trigUncDn_;

    TH1* baseline_scaleUncUp_;
    TH1* baseline_scaleUncDn_;

    TH1* baseline_pdfUncUp_;
    TH1* baseline_pdfUncCen_;
    TH1* baseline_pdfUncDn_;

    TH1* baseline_isrUncUp_;
    TH1* baseline_isrUncCen_;
    TH1* baseline_isrUncDn_;

    TH1* baseline_metMagUp_;
    TH1* baseline_metMagDn_;

    TH1* baseline_metPhiUp_;
    TH1* baseline_metPhiDn_;

    TH1* baseline_jetJECUp_;
    TH1* baseline_jetJECDn_;

    TH1* baseline_bTagSFUp_;
    TH1* baseline_bTagSFCen_;
    TH1* baseline_bTagSFDn_;
    TH1* baseline_mistagSFUp_;
    TH1* baseline_mistagSFDn_;

    int mMass_;
    int dMass_;
    std::string sampleKeyStr_;

    TH2* n_eff_b_;
    TH2* n_eff_c_;
    TH2* n_eff_udsg_;
    TH2* d_eff_b_;
    TH2* d_eff_c_;
    TH2* d_eff_udsg_;

    TH1D* NJets_ISR_;

    double maxEffpt_, secMaxEffpt_;
    double secMaxNJets_ISR_for_Hist_ = 6, maxNJets_ISR_for_hist_ = 7;

    void bookHists()
    {
       char hname[128];
       sprintf(hname, "%s_%d_%d", "met", mMass_, dMass_);
       met_ = new TH1D(hname, hname, 100, 0, 1000); met_->Sumw2();
       sprintf(hname, "%s_%d_%d", "mt2", mMass_, dMass_);
       mt2_ = new TH1D(hname, hname, 100, 0, 1000); mt2_->Sumw2();
       sprintf(hname, "%s_%d_%d", "nt", mMass_, dMass_);
       nt_ = new TH1D(hname, hname,  5, 0, 5); nt_->Sumw2();
       sprintf(hname, "%s_%d_%d", "nb", mMass_, dMass_);
       nb_ = new TH1D(hname, hname,  5, 0, 5); nb_->Sumw2();

       sprintf(hname, "%s_%d_%d", "cutFlow", mMass_, dMass_);
       cutFlow_ = new TH1D(hname, hname, 20, 0, 20); cutFlow_->Sumw2(); cutFlow_->SetCanExtend(TH1::kAllAxes);

       sprintf(hname, "%s_%d_%d", "sumStopPt_vs_met", mMass_, dMass_);
       sumStopPt_vs_met_ = new TH2D(hname, hname, 100, 0, 800, 100, 0, 800); sumStopPt_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_met", mMass_, dMass_);
       baseline_met_ = new TH1D(hname, hname, 100, 0, 1000); baseline_met_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mt2", mMass_, dMass_);
       baseline_mt2_ = new TH1D(hname, hname, 100, 0, 1000); baseline_mt2_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nt", mMass_, dMass_);
       baseline_nt_ = new TH1D(hname, hname,  5, 0, 5); baseline_nt_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nb", mMass_, dMass_);
       baseline_nb_ = new TH1D(hname, hname,  5, 0, 5); baseline_nb_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_met_Nm1", mMass_, dMass_);
       baseline_met_Nm1_ = new TH1D(hname, hname, 100, 0, 1000); baseline_met_Nm1_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mt2_Nm1", mMass_, dMass_);
       baseline_mt2_Nm1_ = new TH1D(hname, hname, 100, 0, 1000); baseline_mt2_Nm1_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nt_Nm1", mMass_, dMass_);
       baseline_nt_Nm1_ = new TH1D(hname, hname,  5, 0, 5); baseline_nt_Nm1_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nb_Nm1", mMass_, dMass_);
       baseline_nb_Nm1_ = new TH1D(hname, hname,  5, 0, 5); baseline_nb_Nm1_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nb_ngenbLE3", mMass_, dMass_);
       baseline_nb_ngenbLE3_ = new TH1D(hname, hname,  5, 0, 5); baseline_nb_ngenbLE3_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nSearchBin", mMass_, dMass_);
       baseline_nSearchBin_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nSearchBin_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopEQ1_nSearchBin", mMass_, dMass_);
       baseline_nTopEQ1_nSearchBin_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopEQ1_nSearchBin_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopLE2_nSearchBin", mMass_, dMass_);
       baseline_nTopLE2_nSearchBin_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopLE2_nSearchBin_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genInfo_allHad", mMass_, dMass_);
       baseline_genInfo_allHad_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_genInfo_allHad_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genInfo_lostLep", mMass_, dMass_);
       baseline_genInfo_lostLep_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_genInfo_lostLep_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genInfo_hadTau", mMass_, dMass_);
       baseline_genInfo_hadTau_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_genInfo_hadTau_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopEQ1_genInfo_allHad", mMass_, dMass_);
       baseline_nTopEQ1_genInfo_allHad_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopEQ1_genInfo_allHad_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopEQ1_genInfo_lostLep", mMass_, dMass_);
       baseline_nTopEQ1_genInfo_lostLep_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopEQ1_genInfo_lostLep_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopEQ1_genInfo_hadTau", mMass_, dMass_);
       baseline_nTopEQ1_genInfo_hadTau_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopEQ1_genInfo_hadTau_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopLE2_genInfo_allHad", mMass_, dMass_);
       baseline_nTopLE2_genInfo_allHad_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopLE2_genInfo_allHad_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopLE2_genInfo_lostLep", mMass_, dMass_);
       baseline_nTopLE2_genInfo_lostLep_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopLE2_genInfo_lostLep_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nTopLE2_genInfo_hadTau", mMass_, dMass_);
       baseline_nTopLE2_genInfo_hadTau_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_nTopLE2_genInfo_hadTau_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_no_fastsimFilter_nSearchBin", mMass_, dMass_);
       baseline_no_fastsimFilter_nSearchBin_ = new TH1D(hname, hname, nTotBins, 0, nTotBins);

       sprintf(hname, "%s_%d_%d", "baseline_weirdEvt", mMass_, dMass_);
       baseline_weirdEvt_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_weirdEvt_->Sumw2();

       //sprintf(hname, "%s_%d_%d", "baseline_noLepVeto", mMass_, dMass_);
       //baseline_noLepVeto_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_noLepVeto_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_muVeto", mMass_, dMass_);
       baseline_muVeto_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_muVeto_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_muVeto_SF", mMass_, dMass_);
       baseline_muVeto_SF_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_muVeto_SF_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_muVeto_Up", mMass_, dMass_);
       baseline_muVeto_Up_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_muVeto_Up_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_muVeto_Dn", mMass_, dMass_);
       baseline_muVeto_Dn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_muVeto_Dn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_eleVeto", mMass_, dMass_);
       baseline_eleVeto_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_eleVeto_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_eleVeto_SF", mMass_, dMass_);
       baseline_eleVeto_SF_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_eleVeto_SF_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_eleVeto_Up", mMass_, dMass_);
       baseline_eleVeto_Up_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_eleVeto_Up_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_eleVeto_Dn", mMass_, dMass_);
       baseline_eleVeto_Dn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_eleVeto_Dn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_isoLepTrkVeto", mMass_, dMass_);
       baseline_isoLepTrkVeto_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_isoLepTrkVeto_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_isoPionTrkVeto", mMass_, dMass_);
       baseline_isoPionTrkVeto_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_isoPionTrkVeto_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_muCS", mMass_, dMass_);
       baseline_muCS_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_muCS_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_eleCS", mMass_, dMass_);
       baseline_eleCS_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_eleCS_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_trigUncUp", mMass_, dMass_);
       baseline_trigUncUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_trigUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_trigUncCen", mMass_, dMass_);
       baseline_trigUncCen_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_trigUncCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_trigUncDn", mMass_, dMass_);
       baseline_trigUncDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_trigUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_scaleUncUp", mMass_, dMass_);
       baseline_scaleUncUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_scaleUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_scaleUncDn", mMass_, dMass_);
       baseline_scaleUncDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_scaleUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_pdfUncUp", mMass_, dMass_);
       baseline_pdfUncUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_pdfUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_pdfUncCen", mMass_, dMass_);
       baseline_pdfUncCen_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_pdfUncCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_pdfUncDn", mMass_, dMass_);
       baseline_pdfUncDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_pdfUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_isrUncUp", mMass_, dMass_);
       baseline_isrUncUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_isrUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_isrUncCen", mMass_, dMass_);
       baseline_isrUncCen_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_isrUncCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_isrUncDn", mMass_, dMass_);
       baseline_isrUncDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_isrUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_metMagUp", mMass_, dMass_);
       baseline_metMagUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_metMagUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_metMagDn", mMass_, dMass_);
       baseline_metMagDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_metMagDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_metPhiUp", mMass_, dMass_);
       baseline_metPhiUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_metPhiUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_metPhiDn", mMass_, dMass_);
       baseline_metPhiDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_metPhiDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_jetJECUp", mMass_, dMass_);
       baseline_jetJECUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_jetJECUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_jetJECDn", mMass_, dMass_);
       baseline_jetJECDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_jetJECDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_bTagSFUp", mMass_, dMass_);
       baseline_bTagSFUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_bTagSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_bTagSFCen", mMass_, dMass_);
       baseline_bTagSFCen_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_bTagSFCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_bTagSFDn", mMass_, dMass_);
       baseline_bTagSFDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_bTagSFDn_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mistagSFUp", mMass_, dMass_);
       baseline_mistagSFUp_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_mistagSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mistagSFDn", mMass_, dMass_);
       baseline_mistagSFDn_ = new TH1D(hname, hname,  nTotBins, 0, nTotBins); baseline_mistagSFDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genTopSFUp", mMass_, dMass_);
       baseline_genTopSFUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_genTopSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_genTopSFCen", mMass_, dMass_);
       baseline_genTopSFCen_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_genTopSFCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_genTopSFDn", mMass_, dMass_);
       baseline_genTopSFDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_genTopSFDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_recoTopSFUp", mMass_, dMass_);
       baseline_recoTopSFUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_recoTopSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_recoTopSFCen", mMass_, dMass_);
       baseline_recoTopSFCen_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_recoTopSFCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_recoTopSFDn", mMass_, dMass_);
       baseline_recoTopSFDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_recoTopSFDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_mistaggenTopSFUp", mMass_, dMass_);
       baseline_mistaggenTopSFUp_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_mistaggenTopSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mistaggenTopSFDn", mMass_, dMass_);
       baseline_mistaggenTopSFDn_ = new TH1D(hname, hname, nTotBins, 0, nTotBins); baseline_mistaggenTopSFDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "genTopPt", mMass_, dMass_);
       genTopPt_ = new TH1D(hname, hname, 120, 0, 1200); genTopPt_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genTopPt", mMass_, dMass_);
       baseline_genTopPt_ = new TH1D(hname, hname, 120, 0, 1200); baseline_genTopPt_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_misTag_recoTopPt", mMass_, dMass_);
       baseline_misTag_recoTopPt_ = new TH1D(hname, hname, 120, 0, 1200); baseline_misTag_recoTopPt_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_MT2_vs_met", mMass_, dMass_);
       baseline_MT2_vs_met_ = new TH2D(hname, hname, 100, 200, 700, 100, 200, 700); baseline_MT2_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "loose_MT2_vs_met", mMass_, dMass_);
       loose_MT2_vs_met_ = new TH2D(hname, hname, 100, 0, 1000, 100, 0, 1000); loose_MT2_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "loose_alt_MT2_vs_met", mMass_, dMass_);
       loose_alt_MT2_vs_met_ = new TH2D(hname, hname, 100, 0, 1000, 100, 0, 1000); loose_alt_MT2_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "loose_MT2_vs_alt_MT2", mMass_, dMass_);
       loose_MT2_vs_alt_MT2_ = new TH2D(hname, hname, 100, 0, 1000, 100, 0, 1000); loose_MT2_vs_alt_MT2_->Sumw2();

       sprintf(hname, "%s_%d_%d", "xSec", mMass_, dMass_);
       xSec_ = new TH1D(hname, hname, 10, 0, 10); xSec_->Sumw2();
       sprintf(hname, "%s_%d_%d", "totEntries", mMass_, dMass_);
       totEntries_ = new TH1D(hname, hname, 10, 0, 10); totEntries_->Sumw2();

       const int nPtBins = 17;
       const int nEtaBins = 3;

       maxEffpt_ = 900; secMaxEffpt_ = 800;

       const double ptBins[]   =  {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,secMaxEffpt_,maxEffpt_};
       const double etaBins[]  =  {0.0,0.8,1.6,2.4};

       sprintf(hname, "%s_%d_%d", "n_eff_b", mMass_, dMass_);
       n_eff_b_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); n_eff_b_->Sumw2();
       sprintf(hname, "%s_%d_%d", "n_eff_c", mMass_, dMass_);
       n_eff_c_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); n_eff_c_->Sumw2();
       sprintf(hname, "%s_%d_%d", "n_eff_udsg", mMass_, dMass_);
       n_eff_udsg_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); n_eff_udsg_->Sumw2();

       sprintf(hname, "%s_%d_%d", "d_eff_b", mMass_, dMass_);
       d_eff_b_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); d_eff_b_->Sumw2();
       sprintf(hname, "%s_%d_%d", "d_eff_c", mMass_, dMass_);
       d_eff_c_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); d_eff_c_->Sumw2();
       sprintf(hname, "%s_%d_%d", "d_eff_udsg", mMass_, dMass_);
       d_eff_udsg_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); d_eff_udsg_->Sumw2();

//       const int nisrJetBins = 9;
//       const double isrJetBins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 14};
       const int nisrJetBins = 7;
       const double isrJetBins[] = {0, 1, 2, 3, 4, 5, secMaxNJets_ISR_for_Hist_, maxNJets_ISR_for_hist_};
      
       sprintf(hname, "%s_%d_%d", "NJetsISR", mMass_, dMass_);
       NJets_ISR_ = new TH1D(hname, hname, nisrJetBins, isrJetBins); NJets_ISR_->Sumw2();
    }

    ISRCorrector * isrCorr;
    BTagCorrector * btagCorr;

public:
    HistContainer(int mMass, int dMass, std::string sampleKeyStr="T2tt") : mMass_(mMass), dMass_(dMass), sampleKeyStr_(sampleKeyStr)
    {
       bookHists();

       TString massPointStrT;
       massPointStrT.Form("%d_%d", mMass_, dMass_);

       if( !prep_btag_ISR ){
          if( sampleKeyStr_.find("T2tt") != std::string::npos ){
             isrCorr = new ISRCorrector("signalScan_fastsim_T2tt_bTagEff_ISR.root", "", massPointStrT);
             btagCorr = new BTagCorrector("signalScan_fastsim_T2tt_bTagEff_ISR.root", "CSVv2_Moriond17_B_H.csv", true, massPointStrT.Data());
          } else if( sampleKeyStr_.find("T1tttt") != std::string::npos ){
             isrCorr = new ISRCorrector("signalScan_fastsim_T1tttt_bTagEff_ISR.root", "", massPointStrT);
             btagCorr = new BTagCorrector("signalScan_fastsim_T1tttt_bTagEff_ISR.root", "CSVv2_Moriond17_B_H.csv", true, massPointStrT.Data());
          } else if( sampleKeyStr_.find("T5ttcc") != std::string::npos ){
             isrCorr = new ISRCorrector("signalScan_fastsim_T5ttcc_bTagEff_ISR.root", "", massPointStrT);
             btagCorr = new BTagCorrector("signalScan_fastsim_T5ttcc_bTagEff_ISR.root", "CSVv2_Moriond17_B_H.csv", true, massPointStrT.Data());
          } else if( sampleKeyStr_.find("T1ttbb") != std::string::npos ){
             isrCorr = new ISRCorrector("signalScan_fastsim_T1ttbb_bTagEff_ISR.root", "", massPointStrT);
             btagCorr = new BTagCorrector("signalScan_fastsim_T1ttbb_bTagEff_ISR.root", "CSVv2_Moriond17_B_H.csv", true, massPointStrT.Data());
          } else if( sampleKeyStr_.find("T5tttt_dM175") != std::string::npos ){
             isrCorr = new ISRCorrector("signalScan_fastsim_T5tttt_dM175_bTagEff_ISR.root", "", massPointStrT);
             btagCorr = new BTagCorrector("signalScan_fastsim_T5tttt_dM175_bTagEff_ISR.root", "CSVv2_Moriond17_B_H.csv", true, massPointStrT.Data());
          } else{
             std::cout<<"Signal points do NOT support right now. Please provide the NJetsISR distribution root file first!"<<std::endl;
          }
       }else{
          isrCorr = nullptr;
          btagCorr = nullptr;
       }
    }

    void fill(const NTupleReader& tr, const double weight)
    {

       const double genmet = tr.hasVar("genmet") ? tr.getVar<double>("genmet") : 0;
       const double genmetphi = tr.hasVar("genmetphi") ? tr.getVar<double>("genmetphi") : -999;
       TLorentzVector genmetLVec;
       if( tr.hasVar("genmet") ){
          genmetLVec.SetPtEtaPhiM(tr.getVar<double>("genmet"), 0, tr.getVar<double>("genmetphi"), 0);
       }

       const double met = ( usegenmet && tr.hasVar("genmet") )? genmet : tr.getVar<double>("met");
       const double metphi = ( usegenmet && tr.hasVar("genmetphi") )? genmetphi : tr.getVar<double>("metphi");
       const double& best_had_brJet_MT2 = tr.getVar<double>("best_had_brJet_MT2" + spec);
       const int& cntCSVS               = tr.getVar<int>("cntCSVS" + spec);
       const int& nTopCandSortedCnt     = tr.getVar<int>("nTopCandSortedCnt" + spec);
       const bool& passBaseline         = tr.getVar<bool>("passBaseline" + spec);
       const double HT = tr.getVar<double>("HT" + spec);

       TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);

       const int nElectrons = tr.getVar<int>("nElectrons_CUT"+spec);
       const int nMuons = tr.getVar<int>("nMuons_CUT"+spec);

       const bool& passBaselineNoLepVeto = tr.getVar<bool>("passBaselineNoLepVeto" + spec);
       const bool& passMuonVeto = tr.getVar<bool>("passMuonVeto" + spec);
       const bool& passEleVeto = tr.getVar<bool>("passEleVeto" + spec);
       const bool& passIsoTrkVeto = tr.getVar<bool>("passIsoTrkVeto" + spec);
       const bool& passIsoLepTrkVeto = tr.getVar<bool>("passIsoLepTrkVeto" + spec);
       const bool& passIsoPionTrkVeto = tr.getVar<bool>("passIsoPionTrkVeto" + spec);

       const bool passnJets = tr.getVar<bool>("passnJets" + spec);
       const bool passdPhis = tr.getVar<bool>("passdPhis" + spec);
       const bool passBJets = tr.getVar<bool>("passBJets" + spec);
       const bool passMET = tr.getVar<bool>("passMET" + spec);
       const bool passMT2 = tr.getVar<bool>("passMT2" + spec);
       const bool passHT = tr.getVar<bool>("passHT" + spec);
       const bool passTagger = tr.getVar<bool>("passTagger" + spec);
       const bool passNewTaggerReq = tr.hasVar("passNewTaggerReq" + spec)? tr.getVar<bool>("passNewTaggerReq" + spec) : tr.getVar<bool>("passTagger" + spec);
       const bool passNoiseEventFilter = tr.getVar<bool>("passNoiseEventFilter" + spec);
       const bool passFastsimEventFilter = tr.getVar<bool>("passFastsimEventFilter" + spec);

       const std::vector<int> & genDecayIdxVec = tr.getVec<int>("genDecayIdxVec");
       const std::vector<int> & genDecayMomIdxVec = tr.getVec<int>("genDecayMomIdxVec");
       const std::vector<int> & genDecayPdgIdVec = tr.getVec<int>("genDecayPdgIdVec");
       const std::vector<TLorentzVector> & genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
       const std::vector<std::string> & genDecayStrVec = tr.getVec<std::string>("genDecayStrVec");

       const double & Scaled_Variations_Up = tr.getVar<double>("Scaled_Variations_Up");
       const double & Scaled_Variations_Down = tr.getVar<double>("Scaled_Variations_Down");

       const double & NNPDF_From_Median_Central = tr.getVar<double>("NNPDF_From_Median_Central");
       //const double NNPDF_From_Median_Up = tr.getVar<double>("NNPDF_From_Median_Up") * NNPDF_From_Median_Central;
       //const double NNPDF_From_Median_Down = tr.getVar<double>("NNPDF_From_Median_Down") >0 ? tr.getVar<double>("NNPDF_From_Median_Down") * NNPDF_From_Median_Central : 0;
       const double NNPDF_From_Median_Up = tr.getVar<double>("NNPDF_From_Median_Up");
       const double NNPDF_From_Median_Down = tr.getVar<double>("NNPDF_From_Median_Down");

       const std::vector<double> & recoJetsBtag = tr.getVec<double>("recoJetsBtag_0");
       const std::vector<int> & recoJetsFlavor = tr.getVec<int>("recoJetsFlavor");
       const std::vector<TLorentzVector> & jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");

/*
       const std::vector<double> & prob_Up = tr.getVec<double>("bTagSF_EventWeightProb_Up");
       const std::vector<double> & prob_Cen = tr.getVec<double>("bTagSF_EventWeightProb_Central");
       const std::vector<double> & prob_Dn = tr.getVec<double>("bTagSF_EventWeightProb_Down");
       const std::vector<double> & prob_mistag_Up = tr.getVec<double>("mistagSF_EventWeightProb_Up");
       const std::vector<double> & prob_mistag_Dn = tr.getVec<double>("mistagSF_EventWeightProb_Down");
*/
       double method1a_Up = 1.0, method1a_Cen = 1.0, method1a_Dn = 1.0, method1a_mistag_Up = 1.0, method1a_mistag_Dn = 1.0;
       if( btagCorr ){
          int switch_Unc = 0, switch_udsg_Unc = 0;
          btagCorr->SetBtagSFunc(switch_Unc); btagCorr->SetBtagCFunc(switch_Unc);
          btagCorr->SetCtagSFunc(switch_Unc); btagCorr->SetCtagCFunc(switch_Unc);
          btagCorr->SetMistagSFunc(switch_udsg_Unc); btagCorr->SetMistagCFunc(switch_udsg_Unc);
          method1a_Cen = btagCorr->GetSimpleCorrection(&jetsLVec, &recoJetsFlavor, &recoJetsBtag);
          if( std::isnan( method1a_Cen) || std::isinf(method1a_Cen) ){ method1a_Cen = 1.0; }

          switch_Unc = 1, switch_udsg_Unc = 0;
          btagCorr->SetBtagSFunc(switch_Unc); btagCorr->SetBtagCFunc(switch_Unc);
          btagCorr->SetCtagSFunc(switch_Unc); btagCorr->SetCtagCFunc(switch_Unc);
          btagCorr->SetMistagSFunc(switch_udsg_Unc); btagCorr->SetMistagCFunc(switch_udsg_Unc);
          method1a_Up = btagCorr->GetSimpleCorrection(&jetsLVec, &recoJetsFlavor, &recoJetsBtag);
          if( std::isnan( method1a_Up) || std::isinf(method1a_Up) ){ method1a_Up = 1.0; }

          switch_Unc = -1, switch_udsg_Unc = 0;
          btagCorr->SetBtagSFunc(switch_Unc); btagCorr->SetBtagCFunc(switch_Unc);
          btagCorr->SetCtagSFunc(switch_Unc); btagCorr->SetCtagCFunc(switch_Unc);
          btagCorr->SetMistagSFunc(switch_udsg_Unc); btagCorr->SetMistagCFunc(switch_udsg_Unc);
          method1a_Dn = btagCorr->GetSimpleCorrection(&jetsLVec, &recoJetsFlavor, &recoJetsBtag);
          if( std::isnan( method1a_Dn) || std::isinf(method1a_Dn) ){ method1a_Dn = 1.0; }

          switch_Unc = 0, switch_udsg_Unc = 1;
          btagCorr->SetBtagSFunc(switch_Unc); btagCorr->SetBtagCFunc(switch_Unc);
          btagCorr->SetCtagSFunc(switch_Unc); btagCorr->SetCtagCFunc(switch_Unc);
          btagCorr->SetMistagSFunc(switch_udsg_Unc); btagCorr->SetMistagCFunc(switch_udsg_Unc);
          method1a_mistag_Up = btagCorr->GetSimpleCorrection(&jetsLVec, &recoJetsFlavor, &recoJetsBtag);
          if( std::isnan( method1a_mistag_Up) || std::isinf(method1a_mistag_Up) ){ method1a_mistag_Up = 1.0; }

          switch_Unc = 0, switch_udsg_Unc = -1;
          btagCorr->SetBtagSFunc(switch_Unc); btagCorr->SetBtagCFunc(switch_Unc);
          btagCorr->SetCtagSFunc(switch_Unc); btagCorr->SetCtagCFunc(switch_Unc);
          btagCorr->SetMistagSFunc(switch_udsg_Unc); btagCorr->SetMistagCFunc(switch_udsg_Unc);
          method1a_mistag_Dn = btagCorr->GetSimpleCorrection(&jetsLVec, &recoJetsFlavor, &recoJetsBtag);
          if( std::isnan( method1a_mistag_Dn) || std::isinf(method1a_mistag_Dn) ){ method1a_mistag_Dn = 1.0; }
       }

       const std::vector<TLorentzVector> & selGenParticle = tr.getVec<TLorentzVector>("selGenParticle");
       const std::vector<int> & selPDGid = tr.getVec<int>("selPDGid");

       const double & met_metMagUp = tr.getVar<double>("met_metMagUp");
       const double & best_had_brJet_MT2_metMagUp = tr.getVar<double>("best_had_brJet_MT2" + spec_metMagUp);
       const int & cntCSVS_metMagUp = tr.getVar<int>("cntCSVS" + spec_metMagUp);
       const int & nTopCandSortedCnt_metMagUp = tr.getVar<int>("nTopCandSortedCnt" + spec_metMagUp);
       const bool & passBaseline_metMagUp = tr.getVar<bool>("passBaseline" + spec_metMagUp);

       const double & met_metMagDn = tr.getVar<double>("met_metMagDn");
       const double & best_had_brJet_MT2_metMagDn = tr.getVar<double>("best_had_brJet_MT2" + spec_metMagDn);
       const int & cntCSVS_metMagDn = tr.getVar<int>("cntCSVS" + spec_metMagDn);
       const int & nTopCandSortedCnt_metMagDn = tr.getVar<int>("nTopCandSortedCnt" + spec_metMagDn);
       const bool & passBaseline_metMagDn = tr.getVar<bool>("passBaseline" + spec_metMagDn);

       const double & met_metPhiUp = tr.getVar<double>("met");
       const double & best_had_brJet_MT2_metPhiUp = tr.getVar<double>("best_had_brJet_MT2" + spec_metPhiUp);
       const int & cntCSVS_metPhiUp = tr.getVar<int>("cntCSVS" + spec_metPhiUp);
       const int & nTopCandSortedCnt_metPhiUp = tr.getVar<int>("nTopCandSortedCnt" + spec_metPhiUp);
       const bool & passBaseline_metPhiUp = tr.getVar<bool>("passBaseline" + spec_metPhiUp);

       const double & met_metPhiDn = tr.getVar<double>("met");
       const double & best_had_brJet_MT2_metPhiDn = tr.getVar<double>("best_had_brJet_MT2" + spec_metPhiDn);
       const int & cntCSVS_metPhiDn = tr.getVar<int>("cntCSVS" + spec_metPhiDn);
       const int & nTopCandSortedCnt_metPhiDn = tr.getVar<int>("nTopCandSortedCnt" + spec_metPhiDn);
       const bool & passBaseline_metPhiDn = tr.getVar<bool>("passBaseline" + spec_metPhiDn);

       const double & met_jecUp = tr.getVar<double>("met");
       const double & best_had_brJet_MT2_jecUp = tr.getVar<double>("best_had_brJet_MT2" + spec_jecUp);
       const int & cntCSVS_jecUp = tr.getVar<int>("cntCSVS" + spec_jecUp);
       const int & nTopCandSortedCnt_jecUp = tr.getVar<int>("nTopCandSortedCnt" + spec_jecUp);
       const bool & passBaseline_jecUp = tr.getVar<bool>("passBaseline" + spec_jecUp);

       const double & met_jecDn = tr.getVar<double>("met");
       const double & best_had_brJet_MT2_jecDn = tr.getVar<double>("best_had_brJet_MT2" + spec_jecDn);
       const int & cntCSVS_jecDn = tr.getVar<int>("cntCSVS" + spec_jecDn);
       const int & nTopCandSortedCnt_jecDn = tr.getVar<int>("nTopCandSortedCnt" + spec_jecDn);
       const bool & passBaseline_jecDn = tr.getVar<bool>("passBaseline" + spec_jecDn);

       const std::vector<TLorentzVector> & vTops = tr.getVec<TLorentzVector>("vTops" + spec);

       const int & NJetsISR = tr.getVar<int>("NJetsISR");

// No event weights
       for(unsigned int ij=0; ij<jetsLVec.size(); ij++){
          if( !AnaFunctions::jetPassCuts(jetsLVec[ij], AnaConsts::pt30Eta24Arr) ) continue;
          double pt = jetsLVec[ij].Pt(), eta = std::abs(jetsLVec[ij].Eta());
          if( pt > maxEffpt_ ) pt = (maxEffpt_ + secMaxEffpt_) * 0.5;
          double csvs = recoJetsBtag[ij];
          int flav = std::abs(recoJetsFlavor[ij]);
          if(flav == 5){ //b jets
             d_eff_b_->Fill(pt, eta);
             if( csvs > AnaConsts::cutCSVS) n_eff_b_->Fill(pt, eta);
          }else if(flav == 4){ //c jets
             d_eff_c_->Fill(pt, eta);
             if( csvs > AnaConsts::cutCSVS) n_eff_c_->Fill(pt, eta);
          }else if(flav < 4 || flav == 21){ //light quark jets & gluon
             d_eff_udsg_->Fill(pt, eta);
             if( csvs > AnaConsts::cutCSVS) n_eff_udsg_->Fill(pt, eta);
          }
       }

       if(NJetsISR >= secMaxNJets_ISR_for_Hist_) NJets_ISR_->Fill(secMaxNJets_ISR_for_Hist_);
       else NJets_ISR_->Fill(NJetsISR);

       double genTopSF_evt = 1.0, genTopSF_relErr_evt_up = 0.0, genTopSF_relErr_evt_dn = 0.0; int cntgenTop_misMatched = 0;
       std::vector<int> pickedRecoTopIdxVec;

       const TopTaggerResults& ttr = ttPtr->getResults();
       std::vector<TopObject*> Ntop = ttr.getTops();

       int cnt_eleTop =0, cnt_muTop =0, cnt_taumuTop =0, cnt_taueleTop =0, cnt_tauhadTop =0, cnt_allhadTop =0;
       int cnt_genTops =0;
       for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){

          if( std::abs(genDecayPdgIdVec[ig]) != 6 ) continue;

          // sometimes a gen top 'decay's to another gen top (probably just changing energy...)
          bool dup_gentop = false;

          bool islepTop = false;
          int per_cnt_ele =0, per_cnt_mu =0, per_cnt_tau =0;
          for(unsigned int jg=0; jg<genDecayPdgIdVec.size(); jg++){
             int pdgId = genDecayPdgIdVec.at(jg);
             // if there is another different gen top (abs(pdgId)==6) which has mother of this gentop (ig), this means
             // the "duplication" happens.
             if( jg != ig && abs(pdgId) == 6 && find_mother(ig, jg, genDecayIdxVec, genDecayMomIdxVec) ){
                dup_gentop = true;
                break;
             }
             if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15 ){
                if( find_mother(ig, jg, genDecayIdxVec, genDecayMomIdxVec) ){
                   islepTop = true;
                   if( abs(pdgId) == 11 ) per_cnt_ele ++;
                   if( abs(pdgId) == 13 ) per_cnt_mu ++;
                   if( abs(pdgId) == 15 ) per_cnt_tau ++;
                }
             }
          }
          if( dup_gentop ) continue;

          cnt_genTops ++;

          double genTopPt = genDecayLVec[ig].Pt();

          genTopPt_->Fill(genTopPt, weight);

          if(passBaseline) baseline_genTopPt_->Fill(genTopPt, weight);

          if( per_cnt_tau !=0 && per_cnt_mu != 0 ) cnt_taumuTop ++;
          if( per_cnt_tau !=0 && per_cnt_ele !=0 ) cnt_taueleTop ++;
          if( per_cnt_tau !=0 && per_cnt_mu ==0 && per_cnt_ele ==0 ) cnt_tauhadTop ++;

          if( per_cnt_tau ==0 && per_cnt_mu != 0 ) cnt_muTop ++;
          if( per_cnt_tau ==0 && per_cnt_ele != 0 ) cnt_eleTop ++;

          if( per_cnt_tau ==0 && per_cnt_mu ==0 && per_cnt_ele ==0 ) cnt_allhadTop ++;

          bool foundMatch = false;
          // for each gen top, try to find the reco top that matches to it
          // if found, apply (accumulate) the weight and error and break (don't apply multiple times (could match to more than 1 reco top...)
          double min_matchDR = 999.0;
          int matchedIdx = -1;
          int cnt_recoTop_idx = -1;
          for(auto Top : Ntop){
             cnt_recoTop_idx ++;
             TLorentzVector topLVec = Top->P();
             if( min_matchDR > genDecayLVec[ig].DeltaR(topLVec) ){
                min_matchDR = genDecayLVec[ig].DeltaR(topLVec);
                matchedIdx = cnt_recoTop_idx;
             }
          }
          if( min_matchDR < 0.4 ){
             foundMatch = true;
             if( !islepTop ){
                genTopSF_evt *= GetTopPtFastToFullSF(genTopPt);
                genTopSF_relErr_evt_up += GetTopPtFastToFullSF_relErr(genTopPt, 1) * GetTopPtFastToFullSF_relErr(genTopPt, 1);
                genTopSF_relErr_evt_dn += GetTopPtFastToFullSF_relErr(genTopPt, -1) * GetTopPtFastToFullSF_relErr(genTopPt, -1);
             }
          }
          if( !foundMatch && !islepTop ) cntgenTop_misMatched ++;
       }
       genTopSF_relErr_evt_up = genTopSF_relErr_evt_up == 0? 0 : sqrt(genTopSF_relErr_evt_up);
       genTopSF_relErr_evt_dn = genTopSF_relErr_evt_dn == 0? 0 : sqrt(genTopSF_relErr_evt_dn);

       // apply data/MC SF for tops
       double recoTopSF_evt = 1.0, recoTopSF_relErr_evt = 0.0;
       for(auto Top : Ntop){
          int type = Top->getNConstituents();
          TLorentzVector topLVec = Top->P();
          recoTopSF_evt *= GetTopPtDataToMCSF(topLVec.Pt(), type);
          double relErr = GetTopPtDataToMCSF_absErr(topLVec.Pt(), type)/GetTopPtDataToMCSF(topLVec.Pt(), type);
          recoTopSF_relErr_evt += relErr * relErr;
       }
       recoTopSF_relErr_evt = recoTopSF_relErr_evt == 0? 0 : sqrt(recoTopSF_relErr_evt);

       // if cnt_genTops ==0, all the cnt_eleTop, cnt_muTop, cnt_taumuTop, cnt_taueleTop, cnt_tauhadTop and cnt_allhadTop are 0!
       // This can happen when stop -> b W neutralino
       // In this case, reuse the counters!
       if( cnt_genTops == 0 ){
          for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
             if( std::abs(genDecayPdgIdVec[ig]) != 1000006 ) continue;

             int per_cnt_ele =0, per_cnt_mu = 0, per_cnt_tau = 0;
             for(unsigned int jg=0; jg<genDecayPdgIdVec.size(); jg++){
                int pdgId = genDecayPdgIdVec.at(jg);
                if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15 ){
                   if( find_mother(ig, jg, genDecayIdxVec, genDecayMomIdxVec) ){
                      if( abs(pdgId) == 11 ) per_cnt_ele ++;
                      if( abs(pdgId) == 13 ) per_cnt_mu ++;
                      if( abs(pdgId) == 15 ) per_cnt_tau ++;
                   }
                }
             }

             if( per_cnt_tau !=0 && per_cnt_mu != 0 ) cnt_taumuTop ++;
             if( per_cnt_tau !=0 && per_cnt_ele !=0 ) cnt_taueleTop ++;
             if( per_cnt_tau !=0 && per_cnt_mu ==0 && per_cnt_ele ==0 ) cnt_tauhadTop ++;

             if( per_cnt_tau ==0 && per_cnt_mu != 0 ) cnt_muTop ++;
             if( per_cnt_tau ==0 && per_cnt_ele != 0 ) cnt_eleTop ++;

             if( per_cnt_tau ==0 && per_cnt_mu ==0 && per_cnt_ele ==0 ) cnt_allhadTop ++;
          }
       }

       bool is_gen_allHad = (cnt_eleTop ==0 && cnt_muTop ==0 && cnt_taumuTop ==0 && cnt_taueleTop ==0 && cnt_tauhadTop ==0 && cnt_allhadTop !=0);
       bool is_gen_lostLep = (cnt_eleTop !=0 || cnt_muTop !=0 || cnt_taumuTop !=0 || cnt_taueleTop !=0 );
       bool is_gen_hadTau = (!is_gen_lostLep && cnt_tauhadTop !=0 );

       for(unsigned int iv=0; iv<vTops.size(); iv++){
          bool isMatched = false;
          for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
             if( std::abs(genDecayPdgIdVec[ig]) != 6 ) continue;
             if( genDecayLVec[ig].DeltaR(vTops[iv]) < 0.4 ) isMatched = true;
          }
          if( !isMatched ) pickedRecoTopIdxVec.push_back(iv);
       }

       TLorentzVector sumStopLVec;
       int cntgenb =0;
       for(unsigned int is=0; is<selGenParticle.size(); is++){
          if( sampleKeyStr_.find("T2tt") != std::string::npos && std::abs(selPDGid[is]) == 1000006 ) sumStopLVec += selGenParticle[is];
          if( sampleKeyStr_.find("T6tt") != std::string::npos && std::abs(selPDGid[is]) == 1000005 ) sumStopLVec += selGenParticle[is];
          if( (sampleKeyStr_.find("T1tt") != std::string::npos || sampleKeyStr_.find("T5tt") != std::string::npos) && std::abs(selPDGid[is]) == 1000021 ) sumStopLVec += selGenParticle[is];
          if( std::abs(selPDGid[is]) == 5 ){
//             if( AnaFunctions::jetPassCuts(selGenParticle[is], AnaConsts::pt30Eta24Arr) ) cntgenb ++;
          }
       }

       int cntbflavJets = 0;
       for(unsigned int ij=0; ij<jetsLVec.size(); ij++){
          if( !AnaFunctions::jetPassCuts(jetsLVec[ij], AnaConsts::pt30Eta24Arr) ) continue;
          int flav = std::abs(recoJetsFlavor[ij]);
          if( flav == 5 ) cntbflavJets++;
       }

/*
       if( cntgenb >=3 ){
          std::cout<<"\nSusyMotherMass : "<<tr.getVar<double>("SusyMotherMass")<<"  SusyLSPMass : "<<tr.getVar<double>("SusyLSPMass")<<"  cntbflavJets : "<<cntbflavJets<<std::endl;
          std::cout<<"--> genDecayStrVec : "<<genDecayStrVec.at(0).c_str()<<std::endl;
          for(unsigned int is=0; is<selGenParticle.size(); is++){
             std::cout<<"  selPDGid : "<<selPDGid[is]<<"  Pt : "<<selGenParticle[is].Pt()<<"  Eta : "<<selGenParticle[is].Eta()<<"  Phi : "<<selGenParticle[is].Phi()<<"  M : "<<selGenParticle[is].M()<<std::endl;
          }
       }
*/

       int cntWeirdEvt_perEvt = 0;
       for(unsigned int ij=0; ij<jetsLVec.size(); ij++){
          if( jetsLVec[ij].Pt() > 2000. ){ cntWeirdEvt_perEvt ++;}
       }

       sumStopPt_vs_met_->Fill(met, sumStopLVec.Pt(), weight);

       const int nSearchBin = sb->find_Binning_Index(cntCSVS, nTopCandSortedCnt, best_had_brJet_MT2, met, HT);

       met_->Fill(met, weight);
       mt2_->Fill(best_had_brJet_MT2, weight);
       nt_->Fill(nTopCandSortedCnt, weight);
       nb_->Fill(cntCSVS, weight);

       totEntries_->Fill(0.0, weight);

       cutFlow_->Fill("original",             weight);
       cutFlow_->Fill("passNoiseEventFilter", weight * passNoiseEventFilter);
       cutFlow_->Fill("passFastsimEventFilter", weight * passNoiseEventFilter * passFastsimEventFilter);
       cutFlow_->Fill("passMuonVeto",         weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto);
       cutFlow_->Fill("passEleVeto",          weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto);
       cutFlow_->Fill("passIsoTrkVeto",       weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto);
       cutFlow_->Fill("passnJets",            weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets);
       cutFlow_->Fill("passBJets",            weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets);
       cutFlow_->Fill("passHT",               weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT);
       cutFlow_->Fill("passMET",              weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET);
       cutFlow_->Fill("passdPhis",            weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis);
       cutFlow_->Fill("passTagger",           weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis * passTagger);
       cutFlow_->Fill("passMT2",              weight * passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis * passTagger * passMT2);
       cutFlow_->Fill("passBaseline",         weight * passBaseline);

       if(passBaselineNoLepVeto){
//          baseline_noLepVeto_->Fill(nSearchBin, weight);
          const std::vector<TLorentzVector> & muonsLVec = tr.getVec<TLorentzVector>("muonsLVec");
          const std::vector<double> & muonsMiniIso = tr.getVec<double>("muonsMiniIso");
          const std::vector<double> & muonsMtw = tr.getVec<double>("muonsMtw");
          const std::vector<int> & muonsFlagMedium = tr.getVec<int>("muonsFlagMedium");

          const std::vector<TLorentzVector> & elesLVec = tr.getVec<TLorentzVector>("elesLVec");
          const std::vector<double> & elesMiniIso = tr.getVec<double>("elesMiniIso");
          const std::vector<double> & elesMtw = tr.getVec<double>("elesMtw");
          const std::vector<unsigned int> & elesisEB = tr.getVec<unsigned int>("elesisEB");
          const std::vector<int> & elesFlagVeto = tr.getVec<int>("elesFlagVeto");

          if( !passMuonVeto ){
// If multiple muons selected, since any of them could trigger the veto we use the most probable muon which is in general lowest in pt.
// This is because the muon pt spectrum drops quickly for high values
             int pickedMuonIdx = -1;
             for(unsigned int im=0; im<muonsLVec.size(); im++){
                if( AnaFunctions::passMuon(muonsLVec[im], muonsMiniIso[im], muonsMtw[im], muonsFlagMedium[im], AnaConsts::muonsMiniIsoArr) ){
                   if( pickedMuonIdx == -1 ) pickedMuonIdx = (int)im;
                   else if( muonsLVec[pickedMuonIdx].Pt() > muonsLVec[im].Pt() ) pickedMuonIdx = (int)im;
                }
             }
             baseline_muVeto_->Fill(nSearchBin, weight);
             double mu_id_SF = 1.0, mu_iso_SF = 1.0, mu_trk_SF = 1.0;
             if( pickedMuonIdx != -1 ){
                const double eta = muonsLVec[pickedMuonIdx].Eta(), pt = muonsLVec[pickedMuonIdx].Pt();
                const double abseta = std::abs(eta);
                if( mu_mediumID_SF ){
                   mu_id_SF = mu_mediumID_SF->GetBinContent(mu_mediumID_SF->FindBin(pt, abseta));
                   mu_id_SF *= sf_mu_mediumID->GetBinContent(sf_mu_mediumID->FindBin(pt, abseta));
                   if( mu_id_SF == 0 ) mu_id_SF = 1.0; // very simple way dealing with out of range issue of the TH2D
                }
                if( mu_miniISO_SF ){
                   mu_iso_SF = mu_miniISO_SF->GetBinContent(mu_miniISO_SF->FindBin(pt, abseta));
                   mu_iso_SF *= sf_mu_mediumID_mini02->GetBinContent(sf_mu_mediumID_mini02->FindBin(pt, abseta));
                   if( mu_iso_SF == 0 ) mu_iso_SF = 1.0;
                }
                if( pt < 10 && mu_trkptLT10_SF ){
                   mu_trk_SF = mu_trkptLT10_SF->Eval(eta);
                   if( mu_trk_SF == 0 ) mu_trk_SF = 1.0;
                }
                if( pt >= 10 && mu_trkptGT10_SF ){
                   mu_trk_SF = mu_trkptGT10_SF->Eval(eta);
                   if( mu_trk_SF == 0 ) mu_trk_SF = 1.0;
                }
             }
             baseline_muVeto_SF_->Fill(nSearchBin, weight*mu_id_SF*mu_iso_SF*mu_trk_SF);
// It is documented on https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Scale_Factors_for_Moriond2017
// that for data/MC, a 3% is recommended; for full/fast, a 2% is recommended. Therefore combined is sqrt(3*3 + 2*2) ~ 3.6%
             baseline_muVeto_Up_->Fill(nSearchBin, weight*(1+0.036));
             baseline_muVeto_Dn_->Fill(nSearchBin, weight*(1-0.036));
             if( nMuons ==1 && nElectrons == AnaConsts::nElectronsSel ){
                const TLorentzVector muLVec = muonsLVec[pickedMuonIdx];
                const double mtw = calcMT(muLVec, metLVec);
                const bool pass_mtw = mtw<100 ? true : false;
                if( pass_mtw ) baseline_muCS_->Fill(nSearchBin, weight);
             }
          }
          if( !passEleVeto ){
             int pickedEleIdx = -1;
             for(unsigned int ie=0; ie<elesLVec.size(); ie++){
                if( AnaFunctions::passElectron(elesLVec[ie], elesMiniIso[ie], elesMtw[ie], elesisEB[ie], elesFlagVeto[ie], AnaConsts::elesMiniIsoArr) ){
                   if( pickedEleIdx == -1 ) pickedEleIdx = (int)ie;
                   else if( elesLVec[pickedEleIdx].Pt() > elesLVec[ie].Pt() ) pickedEleIdx = (int)ie;
                }
             }
             baseline_eleVeto_->Fill(nSearchBin, weight);
             double ele_id_SF = 1.0, ele_iso_SF = 1.0, ele_trk_SF = 1.0;
             double ele_id_SF_err = 0.0, ele_iso_SF_err = 0.0, ele_trk_SF_err = 0.0;
             if( pickedEleIdx != -1 ){
                const double eta = elesLVec[pickedEleIdx].Eta(), pt = elesLVec[pickedEleIdx].Pt();
                const double abseta = std::abs(eta);
                if( ele_VetoID_SF ){
                   ele_id_SF = ele_VetoID_SF->GetBinContent(ele_VetoID_SF->FindBin(pt, abseta));
                   ele_id_SF *= sf_el_vetoCB->GetBinContent(sf_el_vetoCB->FindBin(pt, abseta));
                   ele_id_SF_err = ele_VetoID_SF->GetBinError(ele_VetoID_SF->FindBin(pt, abseta));
                   if( ele_id_SF == 0 ){ ele_id_SF = 1.0; ele_id_SF_err = 0.0; }
                }
                if( ele_miniISO_SF ){
                   ele_iso_SF = ele_miniISO_SF->GetBinContent(ele_miniISO_SF->FindBin(pt, abseta));
                   ele_iso_SF *= sf_el_mini01->GetBinContent(sf_el_mini01->FindBin(pt, abseta));
                   ele_iso_SF_err = ele_miniISO_SF->GetBinError(ele_miniISO_SF->FindBin(pt, abseta));
                   if( ele_iso_SF == 0 ){ ele_iso_SF = 1.0; ele_iso_SF_err = 0.0; }
                }
                if( ele_trkpt_SF ){
                   ele_trk_SF = ele_trkpt_SF->GetBinContent(ele_trkpt_SF->FindBin(eta, pt));
                   ele_trk_SF_err = pt<20 || pt>80? 0.01: 0.00;
                   if( ele_trk_SF == 0 ){ ele_trk_SF = 1.0; ele_trk_SF_err = 0.0; }
                }
             }
             double ele_tot_SF_err = sqrt(ele_id_SF_err*ele_id_SF_err + ele_iso_SF_err*ele_iso_SF_err + ele_trk_SF_err*ele_trk_SF_err);
             baseline_eleVeto_SF_->Fill(nSearchBin, weight*ele_id_SF*ele_iso_SF*ele_trk_SF);
             baseline_eleVeto_Up_->Fill(nSearchBin, weight*(1+ele_tot_SF_err));
             baseline_eleVeto_Dn_->Fill(nSearchBin, weight*(1-ele_tot_SF_err));
             if( nElectrons ==1 & nMuons == AnaConsts::nMuonsSel ){
                const TLorentzVector eleLVec = elesLVec[pickedEleIdx];
                const double mtw = calcMT(eleLVec, metLVec);
                const bool pass_mtw = mtw<100 ? true : false;
                if( pass_mtw ) baseline_eleCS_->Fill(nSearchBin, weight);
             }
          }
          if( passMuonVeto && passEleVeto && !passIsoLepTrkVeto ) baseline_isoLepTrkVeto_->Fill(nSearchBin, weight);
          if( passMuonVeto && passEleVeto && passIsoLepTrkVeto && !passIsoPionTrkVeto ) baseline_isoPionTrkVeto_->Fill(nSearchBin, weight);
       }

       if( passNoiseEventFilter && passFastsimEventFilter && passMuonVeto && passEleVeto && passIsoTrkVeto && passnJets && passBJets && passHT && passMET && passdPhis && passTagger ){
          const double alt_MT2 = -99909;//mt2Calc->getMT2Hemi(tr.getVec<TLorentzVector>("jetsLVec_forTagger" + spec), metLVec);

          loose_MT2_vs_met_->Fill(met, best_had_brJet_MT2, weight);
          loose_alt_MT2_vs_met_->Fill(met, alt_MT2, weight);
          loose_MT2_vs_alt_MT2_->Fill(alt_MT2, best_had_brJet_MT2, weight);
       }

       if( passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * /*passMET */ passdPhis * passTagger * passMT2 ) baseline_met_Nm1_->Fill(met, weight);
       if( passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis * passTagger /* passMT2*/ ) baseline_mt2_Nm1_->Fill(best_had_brJet_MT2, weight);
// MT2 has to reply on ntop >=1 ...
       if( passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis * passNewTaggerReq /* passMT2*/ ){
          if( nTopCandSortedCnt == 0 || (nTopCandSortedCnt>=1 && passMT2) ) baseline_nt_Nm1_->Fill(nTopCandSortedCnt, weight);
       }
       if( passNoiseEventFilter * passFastsimEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * /*passBJets */ passHT * passMET * passdPhis * (nTopCandSortedCnt>=1) ){
          if( cntCSVS==0 || (passNewTaggerReq && passMT2) ) baseline_nb_Nm1_->Fill(cntCSVS, weight);
       }

       if( passNoiseEventFilter * passMuonVeto * passEleVeto * passIsoTrkVeto * passnJets * passBJets * passHT * passMET * passdPhis * passTagger * passMT2 ){
          baseline_no_fastsimFilter_nSearchBin_->Fill(nSearchBin, weight);
       }

//       if(passBaseline && !cntWeirdEvt_perEvt)
       if(passBaseline)
       {
          cutFlow_->Fill("nbEQ1_ntEQ1", weight * (cntCSVS ==1 && nTopCandSortedCnt ==1));
          cutFlow_->Fill("nbEQ1_ntLE2", weight * (cntCSVS ==1 && nTopCandSortedCnt >=2));
          cutFlow_->Fill("nbLE2_ntEQ1", weight * (cntCSVS >=2 && nTopCandSortedCnt ==1));
          cutFlow_->Fill("nbLE2_ntLE2", weight * (cntCSVS >=2 && nTopCandSortedCnt >=2));

          cutFlow_->Fill("MT2_200-300", weight * (best_had_brJet_MT2 >= 200 && best_had_brJet_MT2 < 300));
          cutFlow_->Fill("MT2_300-400", weight * (best_had_brJet_MT2 >= 300 && best_had_brJet_MT2 < 400));
          cutFlow_->Fill("MT2_400-inf", weight * (best_had_brJet_MT2 >= 400));

          cutFlow_->Fill("MT2_200-300_MET_200_275", weight * (best_had_brJet_MT2 >= 200 && best_had_brJet_MT2 < 300 && met >= 200 && met < 275));
          cutFlow_->Fill("MT2_200-300_MET_275_350", weight * (best_had_brJet_MT2 >= 200 && best_had_brJet_MT2 < 300 && met >= 275 && met < 350));
          cutFlow_->Fill("MT2_200-300_MET_350_450", weight * (best_had_brJet_MT2 >= 200 && best_had_brJet_MT2 < 300 && met >= 350 && met < 450));
          cutFlow_->Fill("MT2_200-300_MET_450_inf", weight * (best_had_brJet_MT2 >= 200 && best_had_brJet_MT2 < 300 && met >= 450));

          cutFlow_->Fill("MT2_300-400_MET_200_275", weight * (best_had_brJet_MT2 >= 300 && best_had_brJet_MT2 < 400 && met >= 200 && met < 275));
          cutFlow_->Fill("MT2_300-400_MET_275_350", weight * (best_had_brJet_MT2 >= 300 && best_had_brJet_MT2 < 400 && met >= 275 && met < 350));
          cutFlow_->Fill("MT2_300-400_MET_350_450", weight * (best_had_brJet_MT2 >= 300 && best_had_brJet_MT2 < 400 && met >= 350 && met < 450));
          cutFlow_->Fill("MT2_300-400_MET_450_inf", weight * (best_had_brJet_MT2 >= 300 && best_had_brJet_MT2 < 400 && met >= 450));

          cutFlow_->Fill("MT2_400-inf_MET_200_350", weight * (best_had_brJet_MT2 >= 400 && met >= 200 && met < 350));
          cutFlow_->Fill("MT2_400-inf_MET_350_450", weight * (best_had_brJet_MT2 >= 400 && met >= 350 && met < 450));
          cutFlow_->Fill("MT2_400-inf_MET_450_inf", weight * (best_had_brJet_MT2 >= 400 && met >= 450));

          baseline_met_->Fill(met, weight);
          baseline_mt2_->Fill(best_had_brJet_MT2, weight);
          baseline_nt_->Fill(nTopCandSortedCnt, weight);
          baseline_nb_->Fill(cntCSVS, weight);

          if( cntgenb >=3 ) baseline_nb_ngenbLE3_->Fill(cntCSVS, weight);

          baseline_nSearchBin_->Fill(nSearchBin, weight);
          if( nTopCandSortedCnt ==1 ) baseline_nTopEQ1_nSearchBin_->Fill(nSearchBin, weight);
          if( nTopCandSortedCnt >=2 ) baseline_nTopLE2_nSearchBin_->Fill(nSearchBin, weight);

          if( is_gen_allHad ) baseline_genInfo_allHad_->Fill(nSearchBin, weight);
          if( is_gen_lostLep ) baseline_genInfo_lostLep_->Fill(nSearchBin, weight);
          if( is_gen_hadTau ) baseline_genInfo_hadTau_->Fill(nSearchBin, weight);

          if( is_gen_allHad && nTopCandSortedCnt ==1 ) baseline_nTopEQ1_genInfo_allHad_->Fill(nSearchBin, weight);
          if( is_gen_lostLep && nTopCandSortedCnt ==1 ) baseline_nTopEQ1_genInfo_lostLep_->Fill(nSearchBin, weight);
          if( is_gen_hadTau && nTopCandSortedCnt ==1 ) baseline_nTopEQ1_genInfo_hadTau_->Fill(nSearchBin, weight);

          if( is_gen_allHad && nTopCandSortedCnt >=2 ) baseline_nTopLE2_genInfo_allHad_->Fill(nSearchBin, weight);
          if( is_gen_lostLep && nTopCandSortedCnt >=2 ) baseline_nTopLE2_genInfo_lostLep_->Fill(nSearchBin, weight);
          if( is_gen_hadTau && nTopCandSortedCnt >=2 ) baseline_nTopLE2_genInfo_hadTau_->Fill(nSearchBin, weight);

          if( cntWeirdEvt_perEvt ) baseline_weirdEvt_->Fill(nSearchBin, weight);

          const double trigEff = GetTriggerEffWeight(met);
          const double trigEff_stat_up = GetTriggerEffStatUncHi(met);
          const double trigEff_stat_dn = GetTriggerEffStatUncLo(met);
          const double trigEff_syst_up = GetTriggerEffSystUncHi(met);
          const double trigEff_syst_dn = GetTriggerEffSystUncLo(met);

          const double trigEff_comb_up = sqrt( trigEff_stat_up*trigEff_stat_up + trigEff_syst_up*trigEff_syst_up );
          const double trigEff_comb_dn = sqrt( trigEff_stat_dn*trigEff_stat_dn + trigEff_syst_dn*trigEff_syst_dn );
          const double trigEff_comb_avg = (trigEff_comb_up + trigEff_comb_dn)*0.5;

          baseline_trigUncUp_->Fill(nSearchBin, trigEff * (1 + trigEff_comb_up) * weight);
          baseline_trigUncCen_->Fill(nSearchBin, trigEff * weight);
          baseline_trigUncDn_->Fill(nSearchBin, trigEff * (1 - trigEff_comb_dn) * weight);

          baseline_scaleUncUp_->Fill(nSearchBin, Scaled_Variations_Up * weight);
          baseline_scaleUncDn_->Fill(nSearchBin, Scaled_Variations_Down * weight);

          baseline_pdfUncUp_->Fill(nSearchBin, NNPDF_From_Median_Up * weight);
          baseline_pdfUncCen_->Fill(nSearchBin, NNPDF_From_Median_Central * weight);
          baseline_pdfUncDn_->Fill(nSearchBin, NNPDF_From_Median_Down * weight);

// Method 1a
          baseline_bTagSFUp_->Fill(nSearchBin, method1a_Up * weight);
          baseline_bTagSFCen_->Fill(nSearchBin, method1a_Cen * weight);
          baseline_bTagSFDn_->Fill(nSearchBin, method1a_Dn * weight);

          baseline_mistagSFUp_->Fill(nSearchBin, method1a_mistag_Up * weight);
          baseline_mistagSFDn_->Fill(nSearchBin, method1a_mistag_Dn * weight);

          baseline_genTopSFUp_->Fill(nSearchBin, (genTopSF_relErr_evt_up + 1) * genTopSF_evt * weight);
          baseline_genTopSFCen_->Fill(nSearchBin, genTopSF_evt * weight);
          baseline_genTopSFDn_->Fill(nSearchBin, (1 - genTopSF_relErr_evt_dn) * genTopSF_evt * weight);

          baseline_recoTopSFUp_->Fill(nSearchBin, (recoTopSF_relErr_evt + 1) * recoTopSF_evt * weight);
          baseline_recoTopSFCen_->Fill(nSearchBin, recoTopSF_evt * weight);
          baseline_recoTopSFDn_->Fill(nSearchBin, (1 - recoTopSF_relErr_evt) * recoTopSF_evt * weight);

// mistag rate now is flat (ref to zinv measurement of ntop data/MC distribution) at 0.0 and with very small error
// therefore it should be ignored here! Or just a flat small error later when signal card is made...
          if( cntgenTop_misMatched ) baseline_mistaggenTopSFUp_->Fill(nSearchBin, 0.00*weight);
          if( cntgenTop_misMatched ) baseline_mistaggenTopSFDn_->Fill(nSearchBin, 0.00*weight);

          for(unsigned int ip=0; ip<pickedRecoTopIdxVec.size(); ip++){
             baseline_misTag_recoTopPt_->Fill(vTops[pickedRecoTopIdxVec[ip]].Pt(), weight);
          }

          baseline_MT2_vs_met_->Fill(met, best_had_brJet_MT2, weight);

          double isr_cen_wt = 1.0, isr_up_wt = 1.0, isr_dn_wt = 1.0;

          if( isrCorr ){
             isr_up_wt = isrCorr->GetCorrection_Up(NJetsISR);
             isr_cen_wt = isrCorr->GetCorrection_Cent(NJetsISR);
             isr_dn_wt = isrCorr->GetCorrection_Down(NJetsISR);
          }

          baseline_isrUncUp_->Fill(nSearchBin, weight*isr_up_wt);
          baseline_isrUncCen_->Fill(nSearchBin, weight*isr_cen_wt);
          baseline_isrUncDn_->Fill(nSearchBin, weight*isr_dn_wt);

/*
          if( sumStopLVec.Pt() < 400 ){
             baseline_isrUncUp_->Fill(nSearchBin, weight);
             baseline_isrUncDn_->Fill(nSearchBin, weight);
          }else if( sumStopLVec.Pt() < 600 ){
             baseline_isrUncUp_->Fill(nSearchBin, (1+0.15) * weight);
             baseline_isrUncDn_->Fill(nSearchBin, (1-0.15) * weight);
          }else{
             baseline_isrUncUp_->Fill(nSearchBin, (1+0.30) * weight);
             baseline_isrUncDn_->Fill(nSearchBin, (1-0.30) * weight);
          }
*/
       }

       if( passBaseline_metMagUp ){
          const int nSearchBin_metMagUp = sb->find_Binning_Index(cntCSVS_metMagUp, nTopCandSortedCnt_metMagUp, best_had_brJet_MT2_metMagUp, met_metMagUp, HT);
          baseline_metMagUp_->Fill(nSearchBin_metMagUp, weight);
       }
       if( passBaseline_metMagDn ){
          const int nSearchBin_metMagDn = sb->find_Binning_Index(cntCSVS_metMagDn, nTopCandSortedCnt_metMagDn, best_had_brJet_MT2_metMagDn, met_metMagDn, HT);
          baseline_metMagDn_->Fill(nSearchBin_metMagDn, weight);
       }
       if( passBaseline_metPhiUp ){
          const int nSearchBin_metPhiUp = sb->find_Binning_Index(cntCSVS_metPhiUp, nTopCandSortedCnt_metPhiUp, best_had_brJet_MT2_metPhiUp, met_metPhiUp, HT);
          baseline_metPhiUp_->Fill(nSearchBin_metPhiUp, weight);
       }
       if( passBaseline_metPhiDn ){
          const int nSearchBin_metPhiDn = sb->find_Binning_Index(cntCSVS_metPhiDn, nTopCandSortedCnt_metPhiDn, best_had_brJet_MT2_metPhiDn, met_metPhiDn, HT);
          baseline_metPhiDn_->Fill(nSearchBin_metPhiDn, weight);
       }
       if( passBaseline_jecUp ){
          const int nSearchBin_jetUp = sb->find_Binning_Index(cntCSVS_jecUp, nTopCandSortedCnt_jecUp, best_had_brJet_MT2_jecUp, met_jecUp, HT);
          baseline_jetJECUp_->Fill(nSearchBin_jetUp, weight);
       }
       if( passBaseline_jecDn ){
          const int nSearchBin_jetDn = sb->find_Binning_Index(cntCSVS_jecDn, nTopCandSortedCnt_jecDn, best_had_brJet_MT2_jecDn, met_jecDn, HT);
          baseline_jetJECDn_->Fill(nSearchBin_jetDn, weight);
       }
    }

    void fillxSec(const double xSec, const double xSecErr)
    {
       xSec_->Fill(0.0, xSec);
       xSec_->Fill(1.0, xSecErr);
    }

    void write()
    {
       met_->Write();
       mt2_->Write();
       nt_->Write();
       nb_->Write();

       cutFlow_->Write();

       baseline_met_->Write();
       baseline_mt2_->Write();
       baseline_nt_->Write();
       baseline_nb_->Write();
       baseline_nb_ngenbLE3_->Write();

       baseline_met_Nm1_->Write();
       baseline_mt2_Nm1_->Write();
       baseline_nt_Nm1_->Write();
       baseline_nb_Nm1_->Write();

       baseline_nSearchBin_->Write();
       baseline_nTopEQ1_nSearchBin_->Write();
       baseline_nTopLE2_nSearchBin_->Write();

       baseline_genInfo_allHad_->Write();
       baseline_genInfo_lostLep_->Write();
       baseline_genInfo_hadTau_->Write();

       baseline_nTopEQ1_genInfo_allHad_->Write();
       baseline_nTopEQ1_genInfo_lostLep_->Write();
       baseline_nTopEQ1_genInfo_hadTau_->Write();

       baseline_nTopLE2_genInfo_allHad_->Write();
       baseline_nTopLE2_genInfo_lostLep_->Write();
       baseline_nTopLE2_genInfo_hadTau_->Write();
 
       baseline_no_fastsimFilter_nSearchBin_->Write();

       baseline_weirdEvt_->Write();

//       baseline_noLepVeto_->Write();
       baseline_muVeto_->Write();
       baseline_muVeto_SF_->Write();
       baseline_muVeto_Up_->Write();
       baseline_muVeto_Dn_->Write();
       baseline_eleVeto_->Write();
       baseline_eleVeto_SF_->Write();
       baseline_eleVeto_Up_->Write();
       baseline_eleVeto_Dn_->Write();
       baseline_isoLepTrkVeto_->Write();
       baseline_isoPionTrkVeto_->Write();

       baseline_muCS_->Write();
       baseline_eleCS_->Write();

       baseline_MT2_vs_met_->Write();

       loose_MT2_vs_met_->Write();
       loose_alt_MT2_vs_met_->Write();
       loose_MT2_vs_alt_MT2_->Write();

       totEntries_->Write();

       baseline_trigUncUp_->Write();
       baseline_trigUncCen_->Write();
       baseline_trigUncDn_->Write();

       baseline_scaleUncUp_->Write();
       baseline_scaleUncDn_->Write();

       baseline_pdfUncUp_->Write();
       baseline_pdfUncCen_->Write();
       baseline_pdfUncDn_->Write();

       baseline_isrUncUp_->Write();
       baseline_isrUncCen_->Write();
       baseline_isrUncDn_->Write();

       baseline_metMagUp_->Write();
       baseline_metMagDn_->Write();

       baseline_metPhiUp_->Write();
       baseline_metPhiDn_->Write();

       baseline_jetJECUp_->Write();
       baseline_jetJECDn_->Write();

       baseline_bTagSFUp_->Write();
       baseline_bTagSFCen_->Write();
       baseline_bTagSFDn_->Write();

       baseline_mistagSFUp_->Write();
       baseline_mistagSFDn_->Write();

       baseline_genTopSFUp_->Write();
       baseline_genTopSFCen_->Write();
       baseline_genTopSFDn_->Write();

       baseline_recoTopSFUp_->Write();
       baseline_recoTopSFCen_->Write();
       baseline_recoTopSFDn_->Write();

       baseline_mistaggenTopSFUp_->Write();
       baseline_mistaggenTopSFDn_->Write();

       genTopPt_->Write();
       baseline_genTopPt_->Write();

//       baseline_misTag_recoTopPt_->Write();

       sumStopPt_vs_met_->Write();

       if( prep_btag_ISR ){
          n_eff_b_->Write();
          n_eff_c_->Write();
          n_eff_udsg_->Write();
          d_eff_b_->Write();
          d_eff_c_->Write();
          d_eff_udsg_->Write();

          NJets_ISR_->Write();
       }
//       xSec_->Write();
    }
};

class SystematicPrep{
   private:

   void systematicPrep(NTupleReader& tr){
      const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");
      const std::vector<double> & recoJetsBtag = tr.getVec<double>("recoJetsBtag_0");
      const std::vector<double>& recoJetsJecUnc = tr.getVec<double>("recoJetsJecUnc");

      const std::vector<double>& metMagUpVec = tr.getVec<double>("metMagUp");
      const std::vector<double>& metMagDnVec = tr.getVec<double>("metMagDown");
      const std::vector<double>& metPhiUpVec = tr.getVec<double>("metPhiUp");
      const std::vector<double>& metPhiDnVec = tr.getVec<double>("metPhiDown");

      const double genmet = tr.hasVar("genmet") ? tr.getVar<double>("genmet") : 0;
      const double genmetphi = tr.hasVar("genmetphi") ? tr.getVar<double>("genmetphi") : -999;

      const double met = ( usegenmet && tr.hasVar("genmet") )? genmet : tr.getVar<double>("met");
      const double metphi = ( usegenmet && tr.hasVar("genmetphi") )? genmetphi : tr.getVar<double>("metphi");

      std::vector<TLorentzVector> *jetLVecUp = new std::vector<TLorentzVector>;
      std::vector<TLorentzVector> *jetLVecDn = new std::vector<TLorentzVector>;

      std::vector<double> *recoJetsBtagUp = new std::vector<double>;
      std::vector<double> *recoJetsBtagDn = new std::vector<double>;

//      std::vector<double> *dmetMag = new std::vector<double>;
//      std::vector<double> *dmetPhi = new std::vector<double>;

      double metUp = 0.0, metDn = 99999.0;
      double metPhiUp = metphi, metPhiDn = metphi;

      for(int iMet = 0; iMet < metMagUpVec.size(); ++iMet){
         metUp = std::max(metUp, metMagUpVec[iMet]);
         metDn = std::min(metDn, metMagDnVec[iMet]);

         if( TVector2::Phi_mpi_pi(metPhiUpVec[iMet] - metphi) > 0 && metPhiUp < metPhiUpVec[iMet] ) metPhiUp = metPhiUpVec[iMet];
         if( TVector2::Phi_mpi_pi(metPhiUpVec[iMet] - metphi) < 0 && metPhiDn > metPhiUpVec[iMet] ) metPhiDn = metPhiUpVec[iMet];
          
         if( TVector2::Phi_mpi_pi(metPhiDnVec[iMet] - metphi) > 0 && metPhiUp < metPhiDnVec[iMet] ) metPhiUp = metPhiDnVec[iMet];
         if( TVector2::Phi_mpi_pi(metPhiDnVec[iMet] - metphi) < 0 && metPhiDn > metPhiDnVec[iMet] ) metPhiDn = metPhiDnVec[iMet];
          
//         dmetMag->push_back((metMagUp[iMet] - met)/met);
//         dmetMag->push_back((metMagDown[iMet] - met)/met);

//         dmetPhi->push_back(TVector2::Phi_mpi_pi(metPhiUp[iMet] - metphi));
//         dmetPhi->push_back(TVector2::Phi_mpi_pi(metPhiDown[iMet] - metphi));
      }

      std::vector<double> tmpjetPtUp, tmpjetPtDn;

      for(int iJet = 0; iJet < jetsLVec.size(); ++iJet){
         tmpjetPtUp.push_back( jetsLVec[iJet].Pt() * (1 + recoJetsJecUnc[iJet]) );
         tmpjetPtDn.push_back( jetsLVec[iJet].Pt() * (1 - recoJetsJecUnc[iJet]) );
      }

      std::vector<size_t> ptIdxUp, ptIdxDn;
      stdindexSort::argsort(tmpjetPtUp.begin(), tmpjetPtUp.end(), std::greater<double>(), ptIdxUp);
      stdindexSort::argsort(tmpjetPtDn.begin(), tmpjetPtDn.end(), std::greater<double>(), ptIdxDn);
      for(unsigned int ip=0; ip<ptIdxUp.size(); ip++){
         unsigned int idxMapped = ptIdxUp[ip];
         jetLVecUp->push_back( jetsLVec[idxMapped] * (1 + recoJetsJecUnc[idxMapped]) );
         recoJetsBtagUp->push_back( recoJetsBtag[idxMapped] );
      }

      for(unsigned int ip=0; ip<ptIdxDn.size(); ip++){
         unsigned int idxMapped = ptIdxDn[ip];
         jetLVecDn->push_back( jetsLVec[idxMapped] * (1 - recoJetsJecUnc[idxMapped]) );
         recoJetsBtagDn->push_back( recoJetsBtag[idxMapped] );
      }

      tr.registerDerivedVar("met_metMagUp", metUp);
      tr.registerDerivedVar("met_metMagDn", metDn);

      tr.registerDerivedVar("metphi_metPhiUp", metPhiUp);
      tr.registerDerivedVar("metphi_metPhiDn", metPhiDn);

      tr.registerDerivedVec("jetLVec_jecUp", jetLVecUp);
      tr.registerDerivedVec("jetLVec_jecDn", jetLVecDn);

      tr.registerDerivedVec("recoJetsBtag_jecUp", recoJetsBtagUp);
      tr.registerDerivedVec("recoJetsBtag_jecDn", recoJetsBtagDn);
   }

   public:
   SystematicPrep() { }

   void operator()(NTupleReader& tr) {
      systematicPrep(tr);
   }
} sysPrep;

/*
void sysPrepFunc(NTupleReader &tr){
   sysPrep(tr);
}
*/

std::map<std::pair<int, int>, HistContainer> histVec;

void anaFunc(NTupleReader *tr, std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString,
  const AnaSamples::SampleSet  & allSamples,
  const AnaSamples::SampleCollection & allCollections){

  TString sampleKeyStringT(sampleKeyString);

  std::vector<AnaSamples::FileSummary> fs = allCollections[sampleKeyString];

  double sampleScaleMC = 1.0; int sampleColor = fs.front().color; 

  keyStringCachedVec.push_back(sampleKeyString);
  h1_keyString->Fill(sampleKeyString.c_str(), sampleColor);

  scaleMCCachedVec.push_back( sampleScaleMC );
  colorCachedVec.push_back( sampleColor );

  keyWordVec.push_back(sampleKeyStringT);

  declHistPerSample(sampleKeyString, nTotBins, out_MT2_met_Binning_forTH2Poly);

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     bool isData = false;

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = allSamples[keyString].getWeight();
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ){ scaleMC = 1.0; isData = true; }
     if( keyStringT.Contains("fastsim") ){ scaleMC = 1.0; }
     scaleMC = 1.0;
     std::cout<<"\nWARNING ... forcing scaleMC to be always 1.0 in the signalScan code!\n"<<std::endl;

     if( tr ) delete tr;
     std::set<std::string> activatedBranch;
     for(auto& branch : AnaConsts::activatedBranchNames_DataOnly) activatedBranch.insert(branch);
     for(auto& branch : AnaConsts::activatedBranchNames) activatedBranch.insert(branch);
     activatedBranch.insert("selGenParticle"); activatedBranch.insert("selPDGid");
//     activatedBranch.insert("recoJetsFlavor");

     tr = new NTupleReader(treeVec[ist], activatedBranch);
     tr->registerFunction((*SRblv));
     if(!isData) tr->registerFunction((*pdfScale));

//     tr->registerFunction(&sysPrepFunc);
     tr->registerFunction(sysPrep);

     tr->registerFunction((*SRblv_jecUp));
     tr->registerFunction((*SRblv_jecDn));
     tr->registerFunction((*SRblv_metMagUp));
     tr->registerFunction((*SRblv_metMagDn));
     tr->registerFunction((*SRblv_metPhiUp));
     tr->registerFunction((*SRblv_metPhiDn));

     int entries = tr->getNEntries();
     if( entryToProcess >0 ) entries = entryToProcess;
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     bool cached_rethrow = tr->getReThrow();
     tr->setReThrow(false);

     while(tr->getNextEvent()){

        if( entryToProcess >0 && tr->getEvtNum() > entryToProcess ) break;

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (entries>=10 && (tr->getEvtNum()-1)%(entries/10) == 0) ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;

        const double& SusyMotherMass  = tr->getVar<double>("SusyMotherMass");
        const double& SusyLSPMass     = tr->getVar<double>("SusyLSPMass");

        std::pair<int, int> iMP((int)SusyMotherMass, (int)SusyLSPMass);

        auto iter = histVec.find(iMP);
        if(iter == histVec.end()) iter = histVec.emplace(iMP, HistContainer(iMP.first, iMP.second, sampleKeyString)).first;

        iter->second.fill(*tr, scaleMC);

        // Internal evtWeight in the sample: default is 1.0 execept for MC samples with intrinsic weight, e.g., QCD flat sample.
        double iniWeight = tr->getVar<double>("evtWeight");
        double puWeight = 1.0; // currently set to be 1.0

        double stored_weight = keyStringT.Contains("Data") ? 1 : tr->getVar<double>("stored_weight");
        int sign_of_stored_weight = (stored_weight > 0) ? 1 : ((stored_weight < 0) ? -1 : 0);
        if( sign_of_stored_weight == 1 ) h1_cutFlow_auxVec.back()->Fill("posSign", 1);
        if( sign_of_stored_weight == -1 ) h1_cutFlow_auxVec.back()->Fill("negSign", 1);
        if( sign_of_stored_weight == 0 ) h1_cutFlow_auxVec.back()->Fill("zero", 1);
        
        double evtWeight = iniWeight >=0 ? iniWeight * puWeight * sign_of_stored_weight : iniWeight * puWeight;

// Get branches out directly from what stored in the tree
        const unsigned int & run = tr->getVar<unsigned int>("run"); 
        const unsigned int & lumi = tr->getVar<unsigned int>("lumi"); 
        const unsigned int & event = tr->getVar<unsigned int>("event"); 

        const double genmet = tr->hasVar("genmet") ? tr->getVar<double>("genmet") : 0;
        const double genmetphi = tr->hasVar("genmetphi") ? tr->getVar<double>("genmetphi") : -999;
        TLorentzVector genmetLVec;
        if( tr->hasVar("genmet") ){
           genmetLVec.SetPtEtaPhiM(tr->getVar<double>("genmet"), 0, tr->getVar<double>("genmetphi"), 0);
        }

        const double met = ( usegenmet && tr->hasVar("genmet") )? genmet : tr->getVar<double>("met");
        const double metphi = ( usegenmet && tr->hasVar("genmetphi") )? genmetphi : tr->getVar<double>("metphi");
        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);
        const TLorentzVector mhtLVec = AnaFunctions::calcMHT(tr->getVec<TLorentzVector>("jetsLVec"), AnaConsts::pt30Arr);

        const std::vector<std::string> & TriggerNames = tr->getVec<std::string>("TriggerNames");
        const std::vector<int> & PassTrigger = tr->getVec<int>("PassTrigger");

        const std::vector<TLorentzVector> & muonsLVec = tr->getVec<TLorentzVector>("muonsLVec");
        const std::vector<double> & muonsRelIso = tr->getVec<double>("muonsRelIso");
        const std::vector<double> & muonsMiniIso = tr->getVec<double>("muonsMiniIso");
        const std::vector<double> & muonsMtw = tr->getVec<double>("muonsMtw");
        const std::vector<int> & muonsFlagMedium = tr->getVec<int>("muonsFlagMedium");

        const std::vector<TLorentzVector> & elesLVec = tr->getVec<TLorentzVector>("elesLVec");
        const std::vector<double> & elesRelIso = tr->getVec<double>("elesRelIso");
        const std::vector<double> & elesMiniIso = tr->getVec<double>("elesMiniIso");
        const std::vector<double> & elesMtw = tr->getVec<double>("elesMtw");

        const std::vector<int> W_tau_prongsVec = keyStringT.Contains("Data")? std::vector<int>() : tr->getVec<int>("W_tau_prongsVec");
        const std::vector<int> W_tau_emuVec = keyStringT.Contains("Data")? std::vector<int>() : tr->getVec<int>("W_tau_emuVec");
        const std::vector<int> W_emuVec = keyStringT.Contains("Data")? std::vector<int>() : tr->getVec<int>("W_emuVec");

        const std::vector<double>& recoJetsJecUnc = tr->getVec<double>("recoJetsJecUnc");
        const std::vector<double>& metMagUp   = tr->getVec<double>("metMagUp");
        const std::vector<double>& metMagDown = tr->getVec<double>("metMagDown");
        const std::vector<double>& metPhiUp   = tr->getVec<double>("metPhiUp");
        const std::vector<double>& metPhiDown = tr->getVec<double>("metPhiDown");         

        std::vector<int> emuVec_merge;
        emuVec_merge.reserve( W_emuVec.size() + W_tau_emuVec.size() );
        emuVec_merge.insert( emuVec_merge.end(), W_emuVec.begin(), W_emuVec.end() );
        emuVec_merge.insert( emuVec_merge.end(), W_tau_emuVec.begin(), W_tau_emuVec.end() );

// Get branches out from further computation in the baselineDef.h -> note that the spec string needs to be added
        const std::vector<TLorentzVector> & jetsLVec_forTagger = tr->getVec<TLorentzVector>("jetsLVec_forTagger" + spec);
        const std::vector<double> & recoJetsBtag_forTagger = tr->getVec<double>("recoJetsBtag_forTagger" + spec);

        const int nbJets = tr->getVar<int>("cntCSVS" + spec), nTops = tr->getVar<int>("nTopCandSortedCnt" + spec);
        const double MT2 = tr->getVar<double>("best_had_brJet_MT2" + spec);
        const double HT = tr->getVar<double>("HT" + spec);

        const int nJets = tr->getVar<int>("cntNJetsPt30Eta24" + spec);
        const int nJetsPt50Eta24 = tr->getVar<int>("cntNJetsPt50Eta24" + spec);

        const std::vector<double> & dPhiVec = tr->getVec<double>("dPhiVec" + spec);

        const bool passLeptVeto = tr->getVar<bool>("passLeptVeto" + spec);
        const bool passMuonVeto = tr->getVar<bool>("passMuonVeto" + spec);
        const bool passEleVeto = tr->getVar<bool>("passEleVeto" + spec);
        const bool passIsoTrkVeto = tr->getVar<bool>("passIsoTrkVeto" + spec);
        const bool passnJets = tr->getVar<bool>("passnJets" + spec);
        const bool passdPhis = tr->getVar<bool>("passdPhis" + spec);
        const bool passBJets = tr->getVar<bool>("passBJets" + spec);
        const bool passMET = tr->getVar<bool>("passMET" + spec);
        const bool passMT2 = tr->getVar<bool>("passMT2" + spec);
        const bool passHT = tr->getVar<bool>("passHT" + spec);
        const bool passTagger = tr->getVar<bool>("passTagger" + spec);
        const bool passNoiseEventFilter = tr->getVar<bool>("passNoiseEventFilter" + spec);

        const bool passBaseline = tr->getVar<bool>("passBaseline" + spec);
        const bool passBaselineNoTag = tr->getVar<bool>("passBaselineNoTag" + spec);

        const int searchBinIdx = sb->find_Binning_Index(nbJets, nTops, MT2, met, HT);

        h1_cutFlowVec.back()->Fill("all", evtWeight * scaleMC);

// Check trigger bit for data. Different PD can have different triggers.
// Can directly use passBaseline to get to baseline distribution, but can also configure this
        h1_cutFlowVec.back()->Fill("original", evtWeight * scaleMC);

        if( !passNoiseEventFilter ) continue; h1_cutFlowVec.back()->Fill("passNoiseEventFilter", evtWeight * scaleMC);

        if( !doSingleMuonCS && !passMuonVeto ) continue; h1_cutFlowVec.back()->Fill("passMuonVeto", evtWeight * scaleMC);
        if( !passEleVeto ) continue; h1_cutFlowVec.back()->Fill("passEleVeto", evtWeight * scaleMC);
        if( !doSingleMuonCS && !passIsoTrkVeto ) continue; h1_cutFlowVec.back()->Fill("passIsoTrkVeto", evtWeight * scaleMC);

        if( !passnJets ) continue; h1_cutFlowVec.back()->Fill("passnJets", evtWeight * scaleMC);

        int cntMuons_LostLepton = 0, cntMuons_HadTau = 0;
        std::vector<int> selMuonIdx_LostLeptonVec, selMuonIdx_HadTauVec;
        for(unsigned int im=0; im<muonsLVec.size(); im++){
           if( AnaFunctions::passMuon(muonsLVec.at(im), muonsMiniIso.at(im), muonsMtw.at(im), muonsFlagMedium.at(im), AnaConsts::muonsMiniIsoArr) ){
              cntMuons_LostLepton ++;
              selMuonIdx_LostLeptonVec.push_back(im);
           }
           if( AnaFunctions::passMuon(muonsLVec.at(im), muonsMiniIso.at(im), muonsMtw.at(im), muonsFlagMedium.at(im), hadtau_muonsMiniIsoArr) ){
              cntMuons_HadTau ++;
              selMuonIdx_HadTauVec.push_back(im);
           }
        }

// Fill histograms with looser requirement -> trigger req. for data...
        const bool looseCond = doSingleMuonCS ? (passHT && passMET && passdPhis && passBJets && cntMuons_HadTau ==1) : (passHT && passMET && passdPhis && passBJets);
        if( looseCond ){
           h1_nJets_looseVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_looseVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_looseVec.back()->Fill(nbJets, evtWeight * scaleMC);
   
           h1_met_looseVec.back()->Fill(met, evtWeight * scaleMC);
           h1_metphi_looseVec.back()->Fill(metphi, evtWeight * scaleMC);

           h1_mht_looseVec.back()->Fill(mhtLVec.Pt(), evtWeight * scaleMC);
           h1_mhtphi_looseVec.back()->Fill(mhtLVec.Phi(), evtWeight * scaleMC);
   
           h1_MT2_looseVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_HT_looseVec.back()->Fill(HT, evtWeight * scaleMC);

           h1_dphi1_looseVec.back()->Fill(dPhiVec[0], evtWeight * scaleMC);
           h1_dphi2_looseVec.back()->Fill(dPhiVec[1], evtWeight * scaleMC);
           h1_dphi3_looseVec.back()->Fill(dPhiVec[2], evtWeight * scaleMC);

           h1_vtxSize_looseVec.back()->Fill(tr->getVar<int>("vtxSize"), evtWeight * scaleMC);

           for(unsigned int ij=0; ij<jetsLVec_forTagger.size(); ij++){
              h1_allJetPt_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Pt(), evtWeight * scaleMC);
              h1_allJetEta_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Eta(), evtWeight * scaleMC);
              h1_allJetPhi_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Phi(), evtWeight * scaleMC);
              h1_allJetM_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).M(), evtWeight * scaleMC);
   
              if( ij == 0 ){
                 h1_leadJetPt_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Pt(), evtWeight * scaleMC);
                 h1_leadJetEta_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Eta(), evtWeight * scaleMC);
                 h1_leadJetPhi_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).Phi(), evtWeight * scaleMC);
                 h1_leadJetM_looseVec.back()->Fill(jetsLVec_forTagger.at(ij).M(), evtWeight * scaleMC);
              }
           }
   
           for(unsigned int is=0; is<selMuonIdx_HadTauVec.size(); is++){
              int im = selMuonIdx_HadTauVec[is];
              h1_muPt_looseVec.back()->Fill(muonsLVec.at(im).Pt(), evtWeight * scaleMC);
              h1_muEta_looseVec.back()->Fill(muonsLVec.at(im).Eta(), evtWeight * scaleMC);
              h1_muPhi_looseVec.back()->Fill(muonsLVec.at(im).Phi(), evtWeight * scaleMC);
           }
   
           for(unsigned int ie=0; ie<elesLVec.size(); ie++){
              h1_elePt_looseVec.back()->Fill(elesLVec.at(ie).Pt(), evtWeight * scaleMC);
              h1_eleEta_looseVec.back()->Fill(elesLVec.at(ie).Eta(), evtWeight * scaleMC);
              h1_elePhi_looseVec.back()->Fill(elesLVec.at(ie).Phi(), evtWeight * scaleMC);
           }
        }
// End of filling histograms with loose requirement

        if( doSingleMuonCS && cntMuons_LostLepton != 1 ) continue; if( doSingleMuonCS ) h1_cutFlowVec.back()->Fill("sel1Muon", evtWeight * scaleMC);

        if( !passBJets ) continue; h1_cutFlowVec.back()->Fill("passBJets", evtWeight * scaleMC);

        if( !passHT ) continue; h1_cutFlowVec.back()->Fill("passHT", evtWeight * scaleMC);

        if( !passMET ) continue; h1_cutFlowVec.back()->Fill("passMET", evtWeight * scaleMC);

        if( (!doInvDphi && !passdPhis) || (doInvDphi && passdPhis) ) continue; h1_cutFlowVec.back()->Fill("passdPhis", evtWeight * scaleMC);

        if( !passTagger ) continue; h1_cutFlowVec.back()->Fill("passTagger", evtWeight * scaleMC);
         
        if( !(nTops>=1) ) continue; h1_cutFlowVec.back()->Fill("passnTopsLE1", evtWeight * scaleMC);

        if( !passMT2 ) continue; h1_cutFlowVec.back()->Fill("passMT2", evtWeight * scaleMC);

// No need, but store this for a cross-check
        h1_cutFlowVec.back()->Fill("passBaseline", passBaseline * evtWeight * scaleMC);

        h1_nJets_baselineVec.back()->Fill(nJets, evtWeight * scaleMC);
        h1_nTops_baselineVec.back()->Fill(nTops, evtWeight * scaleMC);
        h1_nbJets_baselineVec.back()->Fill(nbJets, evtWeight * scaleMC);

        h1_met_baselineVec.back()->Fill(met, evtWeight * scaleMC);
        h1_metphi_baselineVec.back()->Fill(metphi, evtWeight * scaleMC);

        h1_mht_baselineVec.back()->Fill(mhtLVec.Pt(), evtWeight * scaleMC);
        h1_mhtphi_baselineVec.back()->Fill(mhtLVec.Phi(), evtWeight * scaleMC);
   
        h1_MT2_baselineVec.back()->Fill(MT2, evtWeight * scaleMC);
        h1_HT_baselineVec.back()->Fill(HT, evtWeight * scaleMC);

        h2_MT2_vs_met_baselineVec.back()->Fill(met, MT2, evtWeight * scaleMC);

        for(unsigned int it=0; it<nTops; it++){
//           TLorentzVector topLVec = type3Ptr->buildLVec(jetsLVec_forTagger, type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
//           h1_topMass_baselineVec.back()->Fill(topLVec.M(), evtWeight * scaleMC);
        }
   
        h1_dphi1_baselineVec.back()->Fill(dPhiVec[0], evtWeight * scaleMC);
        h1_dphi2_baselineVec.back()->Fill(dPhiVec[1], evtWeight * scaleMC);
        h1_dphi3_baselineVec.back()->Fill(dPhiVec[2], evtWeight * scaleMC);

        h1_vtxSize_baselineVec.back()->Fill(tr->getVar<int>("vtxSize"), evtWeight * scaleMC);

        for(unsigned int ij=0; ij<jetsLVec_forTagger.size(); ij++){
           h1_allJetPt_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Pt(), evtWeight * scaleMC);
           h1_allJetEta_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Eta(), evtWeight * scaleMC);
           h1_allJetPhi_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Phi(), evtWeight * scaleMC);
           h1_allJetM_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).M(), evtWeight * scaleMC);

           if( ij == 0 ){
              h1_leadJetPt_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Pt(), evtWeight * scaleMC);
              h1_leadJetEta_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Eta(), evtWeight * scaleMC);
              h1_leadJetPhi_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).Phi(), evtWeight * scaleMC);
              h1_leadJetM_baselineVec.back()->Fill(jetsLVec_forTagger.at(ij).M(), evtWeight * scaleMC);
           }
        }

        for(unsigned int is=0; is<selMuonIdx_LostLeptonVec.size(); is++){
           int im = selMuonIdx_LostLeptonVec[is];
           h1_muPt_baselineVec.back()->Fill(muonsLVec.at(im).Pt(), evtWeight * scaleMC);
           h1_muEta_baselineVec.back()->Fill(muonsLVec.at(im).Eta(), evtWeight * scaleMC);
           h1_muPhi_baselineVec.back()->Fill(muonsLVec.at(im).Phi(), evtWeight * scaleMC);
        }

        for(unsigned int ie=0; ie<elesLVec.size(); ie++){
           h1_elePt_baselineVec.back()->Fill(elesLVec.at(ie).Pt(), evtWeight * scaleMC);
           h1_eleEta_baselineVec.back()->Fill(elesLVec.at(ie).Eta(), evtWeight * scaleMC);
           h1_elePhi_baselineVec.back()->Fill(elesLVec.at(ie).Phi(), evtWeight * scaleMC);
        }
 
        h1_searchBinYieldsVec.back()->Fill(searchBinIdx, evtWeight * scaleMC);

        if( !isData ){
//           h1_scaleUncNominal_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("Scaled_Variations_Nominal") * evtWeight * scaleMC);
           h1_scaleUncUp_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("Scaled_Variations_Up") * evtWeight * scaleMC);
           h1_scaleUncDown_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("Scaled_Variations_Down") * evtWeight * scaleMC);

           h1_pdfUncCentral_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("NNPDF_From_Median_Central") * evtWeight * scaleMC);
           h1_pdfUncUp_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("NNPDF_From_Median_Up") * evtWeight * scaleMC);
           h1_pdfUncDown_searchBinYieldsVec.back()->Fill(searchBinIdx, tr->getVar<double>("NNPDF_From_Median_Down") * evtWeight * scaleMC);
        }

        int cntGenemu = (int)emuVec_merge.size(), cnttauprongs = (int)W_tau_prongsVec.size();
        if( cntGenemu !=0 ) h1_searchBinYields_LostLepVec.back()->Fill(searchBinIdx, evtWeight * scaleMC);
        if( cnttauprongs != 0 ) h1_searchBinYields_HadTauVec.back()->Fill(searchBinIdx, evtWeight * scaleMC);
        if( cntGenemu !=0 && cnttauprongs != 0 ) h1_searchBinYields_OverlapVec.back()->Fill(searchBinIdx, evtWeight * scaleMC);

        double copymet = met, copyMT2 = MT2;

        for(int iSR=0; iSR<nSR; iSR++){
           if(    ( ((nbJets_SR_lo[iSR] == -1 || nbJets >= nbJets_SR_lo[iSR]) && (nbJets_SR_hi[iSR] == -1 || nbJets < nbJets_SR_hi[iSR])) || nbJets ==0 )
               && (nTops_SR_lo[iSR] == -1 || nTops >= nTops_SR_lo[iSR]) && (nTops_SR_hi[iSR] == -1 || nTops < nTops_SR_hi[iSR])
             ){
              copymet = met; copyMT2 = MT2;
              if( nTops ==0 ) copyMT2 = 300;
              if( copyMT2 > max_MT2_for_binCent ) copyMT2 = max_MT2_for_binCent;
              if( copymet > max_met_for_binCent ) copymet = max_met_for_binCent;
              h2_poly_MT2_vs_metVec[iSR].back()->Fill(copymet, copyMT2, evtWeight*scaleMC);
           }
        }
// Search region in bins of
// nbJets: 1, 2, >=3
// nTops : 1, 2, >=3
        int nbJetsCopy = nbJets, nTopsCopy = nTops;
        if( nbJetsCopy >=3 ) nbJetsCopy = 3; if( nTopsCopy >=3 ) nTopsCopy = 3;

        h2_evtCnt_nbJets_vs_nTopsVec.back()->Fill(nTopsCopy, nbJetsCopy, evtWeight*scaleMC);
        if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
           h2_evtCnt_sumSM_nbJets_vs_nTops->Fill(nTopsCopy, nbJetsCopy, evtWeight*scaleMC);
        }
     }
  }
}

void signalScan(int argc, char *argv[]){

    //mt2Calc = new MT2CalcCore();

   AnaFunctions::prepareForNtupleReader();

   sb = new SearchBins();

   nTotBins = sb->nSearchBins();

   sb->build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   sb->print_searchBins();
   sb->print_searchBins_latex();

   declHistGlobal();

   TFile * allINone_leptonSF_file = new TFile("allINone_leptonSF_Moriond17.root");
   if( !allINone_leptonSF_file->IsZombie() ){
// mu
// SF for data/full
      mu_mediumID_SF = (TH2D*) allINone_leptonSF_file->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0");
      mu_miniISO_SF = (TH2D*) allINone_leptonSF_file->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_Medium2016_pass");
// SF for full/fast
      sf_mu_mediumID = (TH2D*) allINone_leptonSF_file->Get("sf_mu_mediumID");
      sf_mu_mediumID_mini02 = (TH2D*) allINone_leptonSF_file->Get("sf_mu_mediumID_mini02");
// SF for muon tracks
      mu_trkptGT10_SF = (TGraphAsymmErrors*) allINone_leptonSF_file->Get("ratio_eff_eta3_dr030e030_corr");
      mu_trkptLT10_SF = (TGraphAsymmErrors*) allINone_leptonSF_file->Get("ratio_eff_eta3_tk0_dr030e030_corr");

// ele
// SF for data/full
      ele_VetoID_SF = (TH2D*) allINone_leptonSF_file->Get("GsfElectronToVeto");
      ele_miniISO_SF = (TH2D*) allINone_leptonSF_file->Get("MVAVLooseElectronToMini");
// SF for full/fast
      sf_el_vetoCB = (TH2D*) allINone_leptonSF_file->Get("sf_el_vetoCB");
      sf_el_mini01 = (TH2D*) allINone_leptonSF_file->Get("sf_el_mini01");
// SF for ele tracks
      ele_trkpt_SF = (TH2D*) allINone_leptonSF_file->Get("EGamma_SF2D");
   }

   NTupleReader *tr = 0;

   int startfile = 0, filerun = -1;

   std::string selKeyStr, specColKeyStr;
   if( argc >=2 ){
      selKeyStr = argv[1];
      std::cout<<"selKeyStr : "<<selKeyStr<<std::endl;
   }

   if( usegenmet ){
      spec = "usegenmet";
   }

   pdfScale = new PDFUncertainty();
// Assume by default signal MC are all from fastsim!
   if( selKeyStr.find("fullsim") != std::string::npos ){
      if( usegenmet ){
         SRblv = new BaselineVessel(*tr, spec);
   
         SRblv_jecUp = new BaselineVessel(*tr, spec_jecUp + " usegenmet");
         SRblv_jecDn = new BaselineVessel(*tr, spec_jecDn + " usegenmet");
         SRblv_metMagUp = new BaselineVessel(*tr, spec_metMagUp + " usegenmet");
         SRblv_metMagDn = new BaselineVessel(*tr, spec_metMagDn + " usegenmet");
         SRblv_metPhiUp = new BaselineVessel(*tr, spec_metPhiUp + " usegenmet");
         SRblv_metPhiDn = new BaselineVessel(*tr, spec_metPhiDn + " usegenmet");
      }else{
         SRblv = new BaselineVessel(*tr, spec);
   
         SRblv_jecUp = new BaselineVessel(*tr, spec_jecUp);
         SRblv_jecDn = new BaselineVessel(*tr, spec_jecDn);
         SRblv_metMagUp = new BaselineVessel(*tr, spec_metMagUp);
         SRblv_metMagDn = new BaselineVessel(*tr, spec_metMagDn);
         SRblv_metPhiUp = new BaselineVessel(*tr, spec_metPhiUp);
         SRblv_metPhiDn = new BaselineVessel(*tr, spec_metPhiDn);
      }
   }else{
      if( usegenmet ){
         SRblv = new BaselineVessel(*tr, spec, "fastsim");
   
         SRblv_jecUp = new BaselineVessel(*tr, spec_jecUp + " usegenmet", "fastsim");
         SRblv_jecDn = new BaselineVessel(*tr, spec_jecDn + " usegenmet", "fastsim");
         SRblv_metMagUp = new BaselineVessel(*tr, spec_metMagUp + " usegenmet", "fastsim");
         SRblv_metMagDn = new BaselineVessel(*tr, spec_metMagDn + " usegenmet", "fastsim");
         SRblv_metPhiUp = new BaselineVessel(*tr, spec_metPhiUp + " usegenmet", "fastsim");
         SRblv_metPhiDn = new BaselineVessel(*tr, spec_metPhiDn + " usegenmet", "fastsim");
      }else{
         SRblv = new BaselineVessel(*tr, spec, "fastsim");
   
         SRblv_jecUp = new BaselineVessel(*tr, spec_jecUp, "fastsim");
         SRblv_jecDn = new BaselineVessel(*tr, spec_jecDn, "fastsim");
         SRblv_metMagUp = new BaselineVessel(*tr, spec_metMagUp, "fastsim");
         SRblv_metMagDn = new BaselineVessel(*tr, spec_metMagDn, "fastsim");
         SRblv_metPhiUp = new BaselineVessel(*tr, spec_metPhiUp, "fastsim");
         SRblv_metPhiDn = new BaselineVessel(*tr, spec_metPhiDn, "fastsim");
      }
   }

   ttPtr = SRblv->GetTopTaggerPtr();
//   type3Ptr = SRblv->GetType3Ptr();
//   type3Ptr->setdebug(true);

   bool doSel = false; std::ostringstream convert;
   if( argc >=3 ){
      entryToProcess = atoi(argv[2]);
      std::cout<<"entryToProcess : "<<entryToProcess<<std::endl;
// Now entryToProcess is re-used as index to select which sub-sample to run: note that keep entryToProcess to be negative!
// entryToProcess starts from 1!
      if( entryToProcess < 0 ){ doSel = true; convert << abs(entryToProcess); }

      if( argc >=4 ){
         filerun = atoi(argv[3]);
         if( argc >=5 ){
            startfile = atoi(argv[4]);
            if( argc >=6 ){
               specColKeyStr = argv[5];
               std::cout<<"specColKeyStr : "<<specColKeyStr<<std::endl;
            }
         }
      }
   }

   std::string condorSpec;
   if( argc >=7 ){
      condorSpec = argv[6];
   }

   AnaSamples::SampleSet        allSamples = condorSpec.empty()? AnaSamples::SampleSet():AnaSamples::SampleSet(condorSpec);
   AnaSamples::SampleCollection allCollections(allSamples);

   std::stringstream ssSelKey(selKeyStr);

   std::string buf;
   std::vector<std::string> keyStrVec;
   std::string nameStr;

   while(ssSelKey >> buf){ keyStrVec.push_back(buf); nameStr += "_"+buf; }
   if( doSel ) nameStr += "_"+convert.str();

   if( startfile != 0 || filerun != -1 ){
      int idx = startfile/filerun;
      convert << idx; 
      nameStr += "_"+convert.str();
   }

   std::cout<<"nameKeyStr : "<<nameStr.c_str()<<std::endl;

   std::vector<TTree*> treeVec;
   std::vector<std::string> subSampleKeysVec;

   std::vector<int> toRunVec(keyStrVec.size(), 0);
 
   for(const auto & filelist : allCollections){

      if( !specColKeyStr.empty() && specColKeyStr != filelist.first ) continue;

      if( !keyStrVec.empty() ){
         bool found = false;
         for(unsigned int ik=0; ik<keyStrVec.size(); ik++){
            if( filelist.first == keyStrVec[ik] ){
               if( toRunVec[ik] ) found = false;
               else{
                  found = true;
                  if(  entryToProcess !=0 ) toRunVec[ik]++;
               }
            }
         }
         if( !found && entryToProcess !=0 ) continue;
      }

      if( entryToProcess == 0 ){
         bool found = false;
         for(auto & file : filelist.second){
            std::string perSubStr;
            for(const auto & perST : allSamples ){ if(perST.second == file ) perSubStr = perST.first; }
            for(unsigned int ik=0; ik<keyStrVec.size(); ik++){
               if( perSubStr == keyStrVec[ik] ){
                  if( toRunVec[ik] ) found = false;
                  else{
                     found = true;
                     toRunVec[ik]++;
                  }
               }
            }
         }
         if( !found ) continue;
      }

      treeVec.clear(); subSampleKeysVec.clear();

      std::cout<<"\nProcessing "<<filelist.first.c_str()<<" ... -->";
      int idxCnt = 0;
      for(auto & file : filelist.second){
         idxCnt ++;

         std::string perSubStr;
         for(const auto & perST : allSamples ){ if(perST.second == file ) perSubStr = perST.first; }

// Skip from the very beginning
         if( doSel && abs(entryToProcess) != idxCnt ) continue;

         if( entryToProcess == 0 ){
            bool found = false;
            for(auto& str : keyStrVec ){ if( perSubStr == str ) found = true; }
            if( !found ) continue;
         }

         std::cout<<"  "<<perSubStr.c_str();

         TChain *aux = new TChain(file.treePath.c_str());  
         (*const_cast<AnaSamples::FileSummary*>(&file)).readFileList();
         file.addFilesToChain(aux, startfile, filerun);
         treeVec.push_back(aux);

         subSampleKeysVec.push_back(perSubStr);
      }
      std::cout<<std::endl;
      
      if( !treeVec.empty() ) anaFunc(tr, treeVec, subSampleKeysVec, filelist.first.c_str(), allSamples, allCollections);

      std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
   }  

   std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

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
 
   tdrStyle->SetOptStat(1111);
 
   tdrStyle->SetHistLineWidth(1);
 
   tdrStyle->SetPaintTextFormat("4.2f");

   tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

   if( condorSpec != "condor" ){
      TCanvas *cs = new TCanvas("cs", "cs", 1200, 800);
      int divW=3, divH=2;
      cs->Divide(divW, divH);

      TString pdfNameStrT = "signalScan_allINone"+nameStr+".pdf";
   
      cs->Print(pdfNameStrT+"[");
      for(unsigned int ic=0; ic<h1_cutFlowVec.size(); ic++){ h1_cutFlowVec[ic]->LabelsDeflate(); h1_cutFlowVec[ic]->LabelsOption("v"); h1_cutFlowVec[ic]->SetMarkerSize(h1_cutFlowVec[ic]->GetMarkerSize()*1.5); }
      for(unsigned int ic=0; ic<h1_cutFlow_auxVec.size(); ic++){ h1_cutFlow_auxVec[ic]->LabelsDeflate(); h1_cutFlow_auxVec[ic]->LabelsOption("v"); h1_cutFlow_auxVec[ic]->SetMarkerSize(h1_cutFlow_auxVec[ic]->GetMarkerSize()*1.5); }
      draw1DallINone(cs, divW*divH, h1_cutFlowVec, 1, "text e"); cs->Print(pdfNameStrT);
      draw1DallINone(cs, divW*divH, h1_cutFlow_auxVec, 1, "text e"); cs->Print(pdfNameStrT);
   
      draw1DallINone(cs, divW*divH, h1_searchBinYieldsVec, 1, "hist text e"); cs->Print(pdfNameStrT);
   
      for(unsigned int ic=0; ic<h2_evtCnt_nbJets_vs_nTopsVec.size(); ic++){ h2_evtCnt_nbJets_vs_nTopsVec[ic]->SetMarkerSize(h2_evtCnt_nbJets_vs_nTopsVec[ic]->GetMarkerSize()*2.0); }
      draw2DallINone(cs, divW*divH, h2_evtCnt_nbJets_vs_nTopsVec, "colz text e"); cs->Print(pdfNameStrT);
   
      for(int iSR=0; iSR<nSR; iSR++){
         for(unsigned int ic=0; ic<h2_poly_MT2_vs_metVec[iSR].size(); ic++){ h2_poly_MT2_vs_metVec[iSR][ic]->SetMarkerSize(h2_poly_MT2_vs_metVec[iSR][ic]->GetMarkerSize()*2.0); h2_poly_MT2_vs_metVec[iSR][ic]->GetZaxis()->SetRangeUser(0, h2_poly_MT2_vs_metVec[iSR][ic]->GetMaximum()); }
         draw2DallINone(cs, divW*divH, h2_poly_MT2_vs_metVec[iSR], "colz text e"); cs->Print(pdfNameStrT);
      }
      cs->Print(pdfNameStrT+"]");
   }


   TString rootNameStrT = "signalScan"+nameStr+".root";

   TFile *signalScanFile = new TFile(rootNameStrT, "RECREATE");

   for(auto& p : histVec){
/*
      int mMass = p.first.first;
      double xSec = 0.0, xSecErr = 0.0;
      if( xSecMap.find(mMass) != xSecMap.end() ) xSec = xSecMap.find(mMass)->second;
      if( xSecErrMap.find(mMass) != xSecErrMap.end() ) xSecErr = xSecErrMap.find(mMass)->second;
      p.second.fillxSec(xSec, xSecErr);
*/
      p.second.write();
   }

   if( h2_evtCnt_sumSM_nbJets_vs_nTops ) h2_evtCnt_sumSM_nbJets_vs_nTops->Write();
   if( h1_keyString ) h1_keyString->Write();
   for(unsigned int ih=0; ih<h1_cutFlowVec.size(); ih++){
      h1_cutFlowVec[ih]->Write();
      h2_evtCnt_nbJets_vs_nTopsVec[ih]->Write();

      for(int iSR = 0; iSR < nSR; iSR++){
         h2_poly_MT2_vs_metVec[iSR][ih]->Write();
      }

      h1_nJets_looseVec[ih]->Write(); h1_nTops_looseVec[ih]->Write(); h1_nbJets_looseVec[ih]->Write();
      h1_met_looseVec[ih]->Write(); h1_MT2_looseVec[ih]->Write(); h1_HT_looseVec[ih]->Write(); h1_metphi_looseVec[ih]->Write();
      h1_mht_looseVec[ih]->Write(); h1_mhtphi_looseVec[ih]->Write();
      h1_dphi1_looseVec[ih]->Write(); h1_dphi2_looseVec[ih]->Write(); h1_dphi3_looseVec[ih]->Write();
      h1_topMass_looseVec[ih]->Write();
      h1_vtxSize_looseVec[ih]->Write();
      h1_allJetPt_looseVec[ih]->Write(); h1_allJetEta_looseVec[ih]->Write(); h1_allJetPhi_looseVec[ih]->Write(); h1_allJetM_looseVec[ih]->Write();
      h1_leadJetPt_looseVec[ih]->Write(); h1_leadJetEta_looseVec[ih]->Write(); h1_leadJetPhi_looseVec[ih]->Write(); h1_leadJetM_looseVec[ih]->Write();
      h1_muPt_looseVec[ih]->Write(); h1_muEta_looseVec[ih]->Write(); h1_muPhi_looseVec[ih]->Write();
      h1_elePt_looseVec[ih]->Write(); h1_eleEta_looseVec[ih]->Write(); h1_elePhi_looseVec[ih]->Write();

      h1_nJets_baselineVec[ih]->Write(); h1_nTops_baselineVec[ih]->Write(); h1_nbJets_baselineVec[ih]->Write();
      h1_met_baselineVec[ih]->Write(); h1_MT2_baselineVec[ih]->Write(); h1_HT_baselineVec[ih]->Write(); h1_metphi_baselineVec[ih]->Write();
      h1_mht_baselineVec[ih]->Write(); h1_mhtphi_baselineVec[ih]->Write();
      h1_topMass_baselineVec[ih]->Write();
      h1_dphi1_baselineVec[ih]->Write(); h1_dphi2_baselineVec[ih]->Write(); h1_dphi3_baselineVec[ih]->Write();
      h1_vtxSize_baselineVec[ih]->Write();
      h1_allJetPt_baselineVec[ih]->Write(); h1_allJetEta_baselineVec[ih]->Write(); h1_allJetPhi_baselineVec[ih]->Write(); h1_allJetM_baselineVec[ih]->Write();
      h1_leadJetPt_baselineVec[ih]->Write(); h1_leadJetEta_baselineVec[ih]->Write(); h1_leadJetPhi_baselineVec[ih]->Write(); h1_leadJetM_baselineVec[ih]->Write();
      h1_muPt_baselineVec[ih]->Write(); h1_muEta_baselineVec[ih]->Write(); h1_muPhi_baselineVec[ih]->Write();
      h1_elePt_baselineVec[ih]->Write(); h1_eleEta_baselineVec[ih]->Write(); h1_elePhi_baselineVec[ih]->Write();

      h1_searchBinYieldsVec[ih]->Write();
      h1_searchBinYields_HadTauVec[ih]->Write(); h1_searchBinYields_LostLepVec[ih]->Write(); h1_searchBinYields_OverlapVec[ih]->Write();

      h1_scaleUncNominal_searchBinYieldsVec[ih]->Write(); h1_scaleUncUp_searchBinYieldsVec[ih]->Write(); h1_scaleUncDown_searchBinYieldsVec[ih]->Write();

      h1_pdfUncCentral_searchBinYieldsVec[ih]->Write(); h1_pdfUncUp_searchBinYieldsVec[ih]->Write(); h1_pdfUncDown_searchBinYieldsVec[ih]->Write();

      h2_MT2_vs_met_baselineVec[ih]->Write();
   }
   signalScanFile->Write(); signalScanFile->Close();

}

bool find_mother(int momIdx, int dauIdx, const std::vector<int> &genDecayIdxVec, const std::vector<int> &genDecayMomIdxVec){
   if( momIdx == -1 || dauIdx == -1 ) return false;

   if( dauIdx == momIdx ) return true;

   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx == momIdx ) return true;
   }

   return false;
}

int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec){
   for(int ig=0; ig<(int)genDecayIdxVec.size(); ig++){
      if( genDecayIdxVec[ig] == genIdx ) return ig;
   }
   return -1;
}

double calcMT(const TLorentzVector &objLVec, const TLorentzVector &metLVec){

   const double objMass = objLVec.M(), objPt = objLVec.Pt(), objPx = objLVec.Px(), objPy = objLVec.Py();
   const double met = metLVec.Pt(), metphi = metLVec.Phi();

   double mt = sqrt( objMass*objMass + 2*( met*sqrt(objMass*objMass + objPt*objPt) -( met*cos(metphi)*objPx + met*sin(metphi)*objPy ) ) );

   return mt;
}

void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2D*> &h2_inputVec, const TString optDrawStr, const TString optComStr){

  if( h2_inputVec.empty() ) return;

  int cntPadIdx = 0;
  std::vector<std::string> copykeyStringVec = keyStringCachedVec;
  if( optComStr.Contains("sumSM") ) copykeyStringVec.push_back("sumSM");

  unsigned int ntype = copykeyStringVec.size();
  
  for(unsigned int it=0; it<ntype; it++){
     if( it == 0 ) cntPadIdx = 0;
     TString keyStringT(copykeyStringVec[it]);
//     if( keyStringT.Contains("DYJets") || copykeyStringVec[it] == "T_t" || keyStringT.Contains("Data") ) continue;
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(0);
     if( optDrawStr.Length() == 0 ){
        h2_inputVec[it]->Draw("colz");
     }else{
        h2_inputVec[it]->Draw(optDrawStr);
     }
  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); }
}

void draw2DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH2Poly*> &h2_inputVec, const TString optDrawStr, const TString optComStr){

  if( h2_inputVec.empty() ) return;

  int cntPadIdx = 0;
  std::vector<std::string> copykeyStringVec = keyStringCachedVec;
  if( optComStr.Contains("sumSM") ) copykeyStringVec.push_back("sumSM");

  unsigned int ntype = copykeyStringVec.size();
  
  for(unsigned int it=0; it<ntype; it++){
     if( it == 0 ) cntPadIdx = 0;
     TString keyStringT(copykeyStringVec[it]);
//     if( keyStringT.Contains("DYJets") || copykeyStringVec[it] == "T_t" || keyStringT.Contains("Data") ) continue;
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(0);
     if( optDrawStr.Length() == 0 ){
        h2_inputVec[it]->Draw("colz");
     }else{
        h2_inputVec[it]->Draw(optDrawStr);
     }
  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); }
}

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const std::vector<TH1D*> &h1_inputVec, const int nRebin, const TString optDrawStr, const TString optComStr){

   if( h1_inputVec.empty() ) return;

  int statusLogy = 0;
  if( optDrawStr.Contains("SetLogy") ) statusLogy =1;
  int doNormalization = 0;
  if( optDrawStr.Contains("normalization") ) doNormalization =1;
  int drawText = 0;
  if( optDrawStr.Contains("text") ) drawText =1;

  int cntPadIdx = 0;
  std::vector<TH1D*> h1_stackedVec; TH1D * h1_dataCached =0, * h1_signal1Cached =0, *h1_signal2Cached =0;

  std::vector<TString> stackedKeyStringTVec;

  std::vector<std::string> copykeyStringVec = keyStringCachedVec;
  std::vector<int> copycolorVec = colorCachedVec;
  std::vector<double> copyscaleMCVec = scaleMCCachedVec;
  if( optComStr.Contains("sumSM") ){ copyscaleMCVec.push_back(1.0); copycolorVec.push_back(kBlack); copykeyStringVec.push_back("sumSM"); }

  unsigned int ntype = copykeyStringVec.size();

  for(unsigned int it=0; it<ntype; it++){

     TString keyStringT(copykeyStringVec[it]);

     if( it == 0 ){ cntPadIdx = 0; h1_stackedVec.clear(); }

//     if( keyStringT.Contains("DYJets") || copykeyStringVec[it] == "T_t" ) continue;
//     if( keyStringT.Contains("DYJets") || copykeyStringVec[it] == "T_t" || keyStringT.Contains("Data") ) continue;

     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ /*std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl;*/ cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(statusLogy);

     TH1D *tmpHist = (TH1D*) h1_inputVec[it]->Clone();

     tmpHist->Rebin(nRebin); tmpHist->Scale(copyscaleMCVec[it]);

     tmpHist->SetLineColor(copycolorVec[it]); tmpHist->SetMarkerColor(copycolorVec[it]); tmpHist->SetMarkerStyle(6); if( !drawText) tmpHist->SetMarkerSize(0.25);

     drawOverFlowBin(tmpHist);

     if( keyStringT.Contains("Data") ){ tmpHist->SetLineColor(kBlack); tmpHist->SetMarkerColor(kBlack); }

     if( !statusLogy ){
        tmpHist->SetAxisRange(0, tmpHist->GetMaximum()*1.5, "Y");
     }else{
        tmpHist->SetAxisRange(tmpHist->GetMaximum()/1e03, tmpHist->GetMaximum()*32*5, "Y");
     }

     if( keyStringT.Contains("Data") || keyStringT.Contains("Signal") ){
        if( !drawText ) tmpHist->Draw("e");
        else tmpHist->Draw("e text");
     }else{
        if( !drawText) tmpHist->Draw("hist");
        else tmpHist->Draw("hist text e");
     }

     if( !keyStringT.Contains("Data") && !keyStringT.Contains("Signal") ){
        stackedKeyStringTVec.push_back(keyStringT);
        if( h1_stackedVec.empty() ){
           h1_stackedVec.push_back( (TH1D*) tmpHist->Clone() );
           h1_stackedVec.back()->SetFillColor( copycolorVec[it] );
           h1_stackedVec.back()->SetLineColor( copycolorVec[it] );
           h1_stackedVec.back()->SetMarkerColor( copycolorVec[it] );
        }else{
           TH1D *tmpSumHist = (TH1D*) tmpHist->Clone();
           tmpSumHist->Add(h1_stackedVec.back(), 1);
           h1_stackedVec.push_back( (TH1D*) tmpSumHist->Clone() );
           h1_stackedVec.back()->SetFillColor( copycolorVec[it] );
           h1_stackedVec.back()->SetLineColor( copycolorVec[it] );
           h1_stackedVec.back()->SetMarkerColor( copycolorVec[it] );
        }
     }
//     if( keyStringT.Contains("Data") ){ h1_dataCached = (TH1D*) tmpHist->Clone(); }
     if( keyStringT.Contains("Signal") ){
        if( !h1_signal1Cached ) h1_signal1Cached = (TH1D*) tmpHist->Clone();
        else h1_signal2Cached = (TH1D*) tmpHist->Clone();
     }

  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); pad->SetLogy(statusLogy); }

  Float_t legendX1 = .60;
  Float_t legendX2 = .85;
  Float_t legendY1 = .55;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.030);

  cs->cd(lastPadIdx);

  if( h1_dataCached ){
     double dataIntegral = h1_dataCached->Integral();
     double sumTotIntegral = h1_stackedVec.back()->Integral();
     double normScale = dataIntegral/sumTotIntegral;

     h1_dataCached->Draw("e");
     catLeg1->AddEntry(h1_dataCached, "Data");
     for(unsigned int is=0; is<h1_stackedVec.size(); is++){
        unsigned int ivsIdx = h1_stackedVec.size()-is-1;
        TH1D * tmpStacked = (TH1D*) h1_stackedVec[ivsIdx]->Clone();
        if( doNormalization ) tmpStacked->Scale(normScale);
        tmpStacked->Draw("hist same");
        catLeg1->AddEntry(tmpStacked, stackedKeyStringTVec[ivsIdx]);
     }
     h1_signal1Cached->SetLineColor(kRed); h1_signal1Cached->SetMarkerColor(kRed); h1_signal1Cached->SetLineStyle(3);
     h1_signal1Cached->Draw("same");
     h1_dataCached->Draw("same e");

     catLeg1->AddEntry(h1_signal1Cached, "Signal");
     catLeg1->SetFillColor(kWhite);
     catLeg1->SetBorderSize(0);
     catLeg1->Draw();

     TPad *lastPad = (TPad*) cs->GetPad(lastPadIdx);
     lastPad->RedrawAxis();
  }

}

int main(int argc, char *argv[]){
   signalScan(argc, argv);
}

