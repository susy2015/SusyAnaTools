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

const bool doSingleMuonCS = false;
const bool doInvDphi = false;

BaselineVessel * SRblv =0;
const std::string spec = "MY";

void mypassBaselineFunc(NTupleReader &tr){
   (*SRblv)(tr);
}

const std::string spec_jecUp = "jecUp";
BaselineVessel * SRblv_jecUp =0;
void jecUpBaselineFunc(NTupleReader &tr){
   (*SRblv_jecUp)(tr);
}

const std::string spec_jecDn = "jecDn";
BaselineVessel * SRblv_jecDn =0;
void jecDnBaselineFunc(NTupleReader &tr){
   (*SRblv_jecDn)(tr);
}

const std::string spec_metMagUp = "metMagUp";
BaselineVessel * SRblv_metMagUp =0;
void metMagUpBaselineFunc(NTupleReader &tr){
   (*SRblv_metMagUp)(tr);
}

const std::string spec_metMagDn = "metMagDn";
BaselineVessel * SRblv_metMagDn =0;
void metMagDnBaselineFunc(NTupleReader &tr){
   (*SRblv_metMagDn)(tr);
}

const std::string spec_metPhiUp = "metPhiUp";
BaselineVessel * SRblv_metPhiUp =0;
void metPhiUpBaselineFunc(NTupleReader &tr){
   (*SRblv_metPhiUp)(tr);
}

const std::string spec_metPhiDn = "metPhiDn";
BaselineVessel * SRblv_metPhiDn =0;
void metPhiDnBaselineFunc(NTupleReader &tr){
   (*SRblv_metPhiDn)(tr);
}

PDFUncertainty * pdfScale =0;
void myPDFUncertaintyFunc(NTupleReader &tr){
   (*pdfScale)(tr);
}

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

    TH1* baseline_nb_ngenbLE3_;

    TH1* genTopPt_;
    TH1* baseline_genTopPt_;

    TH2* sumStopPt_vs_met_;

    TH2* baseline_MT2_vs_met_;

    TH1* baseline_scaleUncUp_;
    TH1* baseline_scaleUncDn_;

    TH1* baseline_pdfUncUp_;
    TH1* baseline_pdfUncCen_;
    TH1* baseline_pdfUncDn_;

    TH1* baseline_isrUncUp_;
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

    TH2* num_eff_b_;
    TH2* num_eff_c_;
    TH2* num_eff_udsg_;
    TH2* den_eff_b_;
    TH2* den_eff_c_;
    TH2* den_eff_udsg_;

    double maxEffpt_, secMaxEffpt_;

    void bookHists()
    {
       char hname[128];
       sprintf(hname, "%s_%d_%d", "met", mMass_, dMass_);
       met_ = new TH1D(hname, hname, 100, 0, 1000); met_->Sumw2();
       sprintf(hname, "%s_%d_%d", "mt2", mMass_, dMass_);
       mt2_ = new TH1D(hname, hname, 100, 0, 1000); mt2_->Sumw2();
       sprintf(hname, "%s_%d_%d", "nt", mMass_, dMass_);
       nt_ = new TH1D(hname, hname,  4, 0, 4); nt_->Sumw2();
       sprintf(hname, "%s_%d_%d", "nb", mMass_, dMass_);
       nb_ = new TH1D(hname, hname,  5, 0, 5); nb_->Sumw2();

       sprintf(hname, "%s_%d_%d", "sumStopPt_vs_met", mMass_, dMass_);
       sumStopPt_vs_met_ = new TH2D(hname, hname, 100, 0, 800, 100, 0, 800); sumStopPt_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_met", mMass_, dMass_);
       baseline_met_ = new TH1D(hname, hname, 100, 0, 1000); baseline_met_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mt2", mMass_, dMass_);
       baseline_mt2_ = new TH1D(hname, hname, 100, 0, 1000); baseline_mt2_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nt", mMass_, dMass_);
       baseline_nt_ = new TH1D(hname, hname,  4, 0, 4); baseline_nt_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_nb", mMass_, dMass_);
       baseline_nb_ = new TH1D(hname, hname,  5, 0, 5); baseline_nb_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nb_ngenbLE3", mMass_, dMass_);
       baseline_nb_ngenbLE3_ = new TH1D(hname, hname,  5, 0, 5); baseline_nb_ngenbLE3_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_nSearchBin", mMass_, dMass_);
       baseline_nSearchBin_ = new TH1D(hname, hname,  45, 0, 45); baseline_nSearchBin_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_scaleUncUp", mMass_, dMass_);
       baseline_scaleUncUp_ = new TH1D(hname, hname,  45, 0, 45); baseline_scaleUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_scaleUncDn", mMass_, dMass_);
       baseline_scaleUncDn_ = new TH1D(hname, hname,  45, 0, 45); baseline_scaleUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_pdfUncUp", mMass_, dMass_);
       baseline_pdfUncUp_ = new TH1D(hname, hname,  45, 0, 45); baseline_pdfUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_pdfUncCen", mMass_, dMass_);
       baseline_pdfUncCen_ = new TH1D(hname, hname,  45, 0, 45); baseline_pdfUncCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_pdfUncDn", mMass_, dMass_);
       baseline_pdfUncDn_ = new TH1D(hname, hname,  45, 0, 45); baseline_pdfUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_isrUncUp", mMass_, dMass_);
       baseline_isrUncUp_ = new TH1D(hname, hname,  45, 0, 45); baseline_isrUncUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_isrUncDn", mMass_, dMass_);
       baseline_isrUncDn_ = new TH1D(hname, hname,  45, 0, 45); baseline_isrUncDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_metMagUp", mMass_, dMass_);
       baseline_metMagUp_ = new TH1D(hname, hname, 45, 0, 45); baseline_metMagUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_metMagDn", mMass_, dMass_);
       baseline_metMagDn_ = new TH1D(hname, hname, 45, 0, 45); baseline_metMagDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_metPhiUp", mMass_, dMass_);
       baseline_metPhiUp_ = new TH1D(hname, hname, 45, 0, 45); baseline_metPhiUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_metPhiDn", mMass_, dMass_);
       baseline_metPhiDn_ = new TH1D(hname, hname, 45, 0, 45); baseline_metPhiDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_jetJECUp", mMass_, dMass_);
       baseline_jetJECUp_ = new TH1D(hname, hname, 45, 0, 45); baseline_jetJECUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_jetJECDn", mMass_, dMass_);
       baseline_jetJECDn_ = new TH1D(hname, hname, 45, 0, 45); baseline_jetJECDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_bTagSFUp", mMass_, dMass_);
       baseline_bTagSFUp_ = new TH1D(hname, hname,  45, 0, 45); baseline_bTagSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_bTagSFCen", mMass_, dMass_);
       baseline_bTagSFCen_ = new TH1D(hname, hname,  45, 0, 45); baseline_bTagSFCen_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_bTagSFDn", mMass_, dMass_);
       baseline_bTagSFDn_ = new TH1D(hname, hname,  45, 0, 45); baseline_bTagSFDn_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mistagSFUp", mMass_, dMass_);
       baseline_mistagSFUp_ = new TH1D(hname, hname,  45, 0, 45); baseline_mistagSFUp_->Sumw2();
       sprintf(hname, "%s_%d_%d", "baseline_mistagSFDn", mMass_, dMass_);
       baseline_mistagSFDn_ = new TH1D(hname, hname,  45, 0, 45); baseline_mistagSFDn_->Sumw2();

       sprintf(hname, "%s_%d_%d", "genTopPt", mMass_, dMass_);
       genTopPt_ = new TH1D(hname, hname, 100, 0, 1000); genTopPt_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_genTopPt", mMass_, dMass_);
       baseline_genTopPt_ = new TH1D(hname, hname, 100, 0, 1000); baseline_genTopPt_->Sumw2();

       sprintf(hname, "%s_%d_%d", "baseline_MT2_vs_met", mMass_, dMass_);
       baseline_MT2_vs_met_ = new TH2D(hname, hname, 100, 200, 700, 100, 200, 700); baseline_MT2_vs_met_->Sumw2();

       sprintf(hname, "%s_%d_%d", "xSec", mMass_, dMass_);
       xSec_ = new TH1D(hname, hname, 10, 0, 10); xSec_->Sumw2();
       sprintf(hname, "%s_%d_%d", "totEntries", mMass_, dMass_);
       totEntries_ = new TH1D(hname, hname, 10, 0, 10); totEntries_->Sumw2();

       const int nPtBins = 17;
       const int nEtaBins = 3;

       maxEffpt_ = 900; secMaxEffpt_ = 800;

       const double ptBins[]   =  {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,secMaxEffpt_,maxEffpt_};
       const double etaBins[]  =  {0.0,0.8,1.6,2.4};

       sprintf(hname, "%s_%d_%d", "num_eff_b", mMass_, dMass_);
       num_eff_b_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); num_eff_b_->Sumw2();
       sprintf(hname, "%s_%d_%d", "num_eff_c", mMass_, dMass_);
       num_eff_c_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); num_eff_c_->Sumw2();
       sprintf(hname, "%s_%d_%d", "num_eff_udsg", mMass_, dMass_);
       num_eff_udsg_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); num_eff_udsg_->Sumw2();

       sprintf(hname, "%s_%d_%d", "den_eff_b", mMass_, dMass_);
       den_eff_b_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); den_eff_b_->Sumw2();
       sprintf(hname, "%s_%d_%d", "den_eff_c", mMass_, dMass_);
       den_eff_c_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); den_eff_c_->Sumw2();
       sprintf(hname, "%s_%d_%d", "den_eff_udsg", mMass_, dMass_);
       den_eff_udsg_ = new TH2D(hname, hname, nPtBins, ptBins, nEtaBins, etaBins); den_eff_udsg_->Sumw2();
    }

public:
    HistContainer(int mMass, int dMass) : mMass_(mMass), dMass_(dMass)
    {
       bookHists();
    }

    void fill(const NTupleReader& tr, const double weight)
    {
       const double& met                = tr.getVar<double>("met");
       const double& best_had_brJet_MT2 = tr.getVar<double>("best_had_brJet_MT2" + spec);
       const int& cntCSVS               = tr.getVar<int>("cntCSVS" + spec);
       const int& nTopCandSortedCnt     = tr.getVar<int>("nTopCandSortedCnt" + spec);
       const bool& passBaseline         = tr.getVar<bool>("passBaseline" + spec);

       const std::vector<int> & genDecayIdxVec = tr.getVec<int>("genDecayIdxVec");
       const std::vector<int> & genDecayMomIdxVec = tr.getVec<int>("genDecayMomIdxVec");
       const std::vector<int> & genDecayPdgIdVec = tr.getVec<int>("genDecayPdgIdVec");
       const std::vector<TLorentzVector> & genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
       const std::vector<std::string> & genDecayStrVec = tr.getVec<std::string>("genDecayStrVec");

       const double & Scaled_Variations_Up = tr.getVar<double>("Scaled_Variations_Up");
       const double & Scaled_Variations_Down = tr.getVar<double>("Scaled_Variations_Down");

       const double & NNPDF_From_Median_Central = tr.getVar<double>("NNPDF_From_Median_Central");
//       const double NNPDF_From_Median_Up = tr.getVar<double>("NNPDF_From_Median_Up") * NNPDF_From_Median_Central;
//       const double NNPDF_From_Median_Down = tr.getVar<double>("NNPDF_From_Median_Down") >0 ? tr.getVar<double>("NNPDF_From_Median_Down") * NNPDF_From_Median_Central : 0;
       const double NNPDF_From_Median_Up = tr.getVar<double>("NNPDF_From_Median_Up");
       const double NNPDF_From_Median_Down = tr.getVar<double>("NNPDF_From_Median_Down");

       const std::vector<double> & prob_Up = tr.getVec<double>("bTagSF_EventWeightProb_Up");
       const std::vector<double> & prob_Cen = tr.getVec<double>("bTagSF_EventWeightProb_Central");
       const std::vector<double> & prob_Dn = tr.getVec<double>("bTagSF_EventWeightProb_Down");

       const double & method1a_Up = tr.getVar<double>("bTagSF_EventWeightSimple_Up");
       const double & method1a_Cen = tr.getVar<double>("bTagSF_EventWeightSimple_Central");
       const double & method1a_Dn = tr.getVar<double>("bTagSF_EventWeightSimple_Down");
       const double & method1a_mistag_Up = tr.getVar<double>("mistagSF_EventWeightSimple_Up");
       const double & method1a_mistag_Dn = tr.getVar<double>("mistagSF_EventWeightSimple_Down");

       const std::vector<TLorentzVector> & selGenParticle = tr.getVec<TLorentzVector>("selGenParticle");
       const std::vector<int> & selPDGid = tr.getVec<int>("selPDGid");

       const std::vector<double> & recoJetsBtag = tr.getVec<double>("recoJetsBtag_0");
       const std::vector<int> & recoJetsFlavor = tr.getVec<int>("recoJetsFlavor");
       const std::vector<TLorentzVector> & jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");

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

       for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){

          if( std::abs(genDecayPdgIdVec[ig]) != 6 ) continue;

          double genTopPt = genDecayLVec[ig].Pt();

          genTopPt_->Fill(genTopPt, weight);

          if(passBaseline) baseline_genTopPt_->Fill(genTopPt, weight);
       }

       TLorentzVector sumStopLVec;
       int cntgenb =0;
       for(unsigned int is=0; is<selGenParticle.size(); is++){
          if( std::abs(selPDGid[is]) == 1000006 ) sumStopLVec += selGenParticle[is];
          if( std::abs(selPDGid[is]) == 5 > 0 ){
             if( AnaFunctions::jetPassCuts(selGenParticle[is], AnaConsts::pt30Eta24Arr) ) cntgenb ++;
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

       sumStopPt_vs_met_->Fill(met, sumStopLVec.Pt(), weight);

       const int nSearchBin = find_Binning_Index(cntCSVS, nTopCandSortedCnt, best_had_brJet_MT2, met);

       met_->Fill(met, weight);
       mt2_->Fill(best_had_brJet_MT2, weight);
       nt_->Fill(nTopCandSortedCnt, weight);
       nb_->Fill(cntCSVS, weight);

       totEntries_->Fill(0.0, weight);

       if(passBaseline)
       {
          baseline_met_->Fill(met, weight);
          baseline_mt2_->Fill(best_had_brJet_MT2, weight);
          baseline_nt_->Fill(nTopCandSortedCnt, weight);
          baseline_nb_->Fill(cntCSVS, weight);

          if( cntgenb >=3 ) baseline_nb_ngenbLE3_->Fill(cntCSVS, weight);

          baseline_nSearchBin_->Fill(nSearchBin, weight);

          baseline_scaleUncUp_->Fill(nSearchBin, Scaled_Variations_Up * weight);
          baseline_scaleUncDn_->Fill(nSearchBin, Scaled_Variations_Down * weight);

          baseline_pdfUncUp_->Fill(nSearchBin, NNPDF_From_Median_Up * weight);
          baseline_pdfUncCen_->Fill(nSearchBin, NNPDF_From_Median_Central * weight);
          baseline_pdfUncDn_->Fill(nSearchBin, NNPDF_From_Median_Down * weight);

          for(unsigned int ij=0; ij<jetsLVec.size(); ij++){
             if( !AnaFunctions::jetPassCuts(jetsLVec[ij], AnaConsts::pt30Eta24Arr) ) continue;
             double pt = jetsLVec[ij].Pt(), eta = std::abs(jetsLVec[ij].Eta());
             if( pt > maxEffpt_ ) pt = (maxEffpt_ + secMaxEffpt_) * 0.5;
             double csvs = recoJetsBtag[ij];
             int flav = std::abs(recoJetsFlavor[ij]);
             if(flav == 5){ //b jets
                den_eff_b_->Fill(pt, eta, weight);
                if( csvs > AnaConsts::cutCSVS) num_eff_b_->Fill(pt, eta, weight);
             }else if(flav == 4){ //c jets
                den_eff_c_->Fill(pt, eta, weight);
                if( csvs > AnaConsts::cutCSVS) num_eff_c_->Fill(pt, eta, weight);
             }else if(flav < 4 || flav == 21){ //light quark jets & gluon
                den_eff_udsg_->Fill(pt, eta, weight);
                if( csvs > AnaConsts::cutCSVS) num_eff_udsg_->Fill(pt, eta, weight);
             }
          }

          baseline_bTagSFUp_->Fill(nSearchBin, method1a_Up * weight);
          baseline_bTagSFCen_->Fill(nSearchBin, method1a_Cen * weight);
          baseline_bTagSFDn_->Fill(nSearchBin, method1a_Dn * weight);

          baseline_mistagSFUp_->Fill(nSearchBin, method1a_mistag_Up * weight);
          baseline_mistagSFDn_->Fill(nSearchBin, method1a_mistag_Dn * weight);

/*
          for(unsigned int ib=0; ib<4; ib++){
             const int iSB = find_Binning_Index(ib, nTopCandSortedCnt, best_had_brJet_MT2, met);
             if( iSB == -1 ) continue;
             baseline_bTagSFUp_->Fill(iSB, prob_Up[ib] * weight);
             baseline_bTagSFCen_->Fill(iSB, prob_Cen[ib] * weight);
             baseline_bTagSFDn_->Fill(iSB, prob_Dn[ib] * weight);
          }
*/
/*
          for(unsigned int ib=0; ib<nTotBins; ib++){
             struct searchBinDef sbDef;
             find_BinBoundaries(ib, sbDef);
             int tmp_cntCSVS = cntCSVS <=3 ? cntCSVS:3;
             if( sbDef.bJet_lo == tmp_cntCSVS ){
                baseline_bTagSFUp_->Fill(ib, prob_Up[tmp_cntCSVS] * weight);
                baseline_bTagSFCen_->Fill(ib, prob_Cen[tmp_cntCSVS] * weight);
                baseline_bTagSFDn_->Fill(ib, prob_Dn[tmp_cntCSVS] * weight);
             }
          }
*/
/*
          if(cntCSVS == 1){
             baseline_bTagSFUp_->Fill(nSearchBin, prob_Up[1] * weight);
             baseline_bTagSFCen_->Fill(nSearchBin, prob_Cen[1] * weight);
             baseline_bTagSFDn_->Fill(nSearchBin, prob_Dn[1] * weight);
          }else if(cntCSVS ==2 ){
             baseline_bTagSFUp_->Fill(nSearchBin, prob_Up[2] * weight);
             baseline_bTagSFCen_->Fill(nSearchBin, prob_Cen[2] * weight);
             baseline_bTagSFDn_->Fill(nSearchBin, prob_Dn[2] * weight);
          }else if(cntCSVS >=3 ){
             baseline_bTagSFUp_->Fill(nSearchBin, prob_Up[3] * weight);
             baseline_bTagSFCen_->Fill(nSearchBin, prob_Cen[3] * weight);
             baseline_bTagSFDn_->Fill(nSearchBin, prob_Dn[3] * weight);
          }
*/
          baseline_MT2_vs_met_->Fill(met, best_had_brJet_MT2, weight);

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
       }

       if( passBaseline_metMagUp ){
          const int nSearchBin_metMagUp = find_Binning_Index(cntCSVS_metMagUp, nTopCandSortedCnt_metMagUp, best_had_brJet_MT2_metMagUp, met_metMagUp);
          baseline_metMagUp_->Fill(nSearchBin_metMagUp, weight);
       }
       if( passBaseline_metMagDn ){
          const int nSearchBin_metMagDn = find_Binning_Index(cntCSVS_metMagDn, nTopCandSortedCnt_metMagDn, best_had_brJet_MT2_metMagDn, met_metMagDn);
          baseline_metMagDn_->Fill(nSearchBin_metMagDn, weight);
       }
       if( passBaseline_metPhiUp ){
          const int nSearchBin_metPhiUp = find_Binning_Index(cntCSVS_metPhiUp, nTopCandSortedCnt_metPhiUp, best_had_brJet_MT2_metPhiUp, met_metPhiUp);
          baseline_metPhiUp_->Fill(nSearchBin_metPhiUp, weight);
       }
       if( passBaseline_metPhiDn ){
          const int nSearchBin_metPhiDn = find_Binning_Index(cntCSVS_metPhiDn, nTopCandSortedCnt_metPhiDn, best_had_brJet_MT2_metPhiDn, met_metPhiDn);
          baseline_metPhiDn_->Fill(nSearchBin_metPhiDn, weight);
       }
       if( passBaseline_jecUp ){
          const int nSearchBin_jetUp = find_Binning_Index(cntCSVS_jecUp, nTopCandSortedCnt_jecUp, best_had_brJet_MT2_jecUp, met_jecUp);
          baseline_jetJECUp_->Fill(nSearchBin_jetUp, weight);
       }
       if( passBaseline_jecDn ){
          const int nSearchBin_jetDn = find_Binning_Index(cntCSVS_jecDn, nTopCandSortedCnt_jecDn, best_had_brJet_MT2_jecDn, met_jecDn);
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
       baseline_met_->Write();
       baseline_mt2_->Write();
       baseline_nt_->Write();
       baseline_nb_->Write();
       baseline_nb_ngenbLE3_->Write();

       baseline_nSearchBin_->Write();
       baseline_MT2_vs_met_->Write();
       totEntries_->Write();

       baseline_scaleUncUp_->Write();
       baseline_scaleUncDn_->Write();

       baseline_pdfUncUp_->Write();
       baseline_pdfUncCen_->Write();
       baseline_pdfUncDn_->Write();

       baseline_isrUncUp_->Write();
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

       genTopPt_->Write();
       baseline_genTopPt_->Write();

       sumStopPt_vs_met_->Write();

       num_eff_b_->Write();
       num_eff_c_->Write();
       num_eff_udsg_->Write();
       den_eff_b_->Write();
       den_eff_c_->Write();
       den_eff_udsg_->Write();
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

      const double& met    = tr.getVar<double>("met");
      const double& metphi = tr.getVar<double>("metphi");

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

  declHistPerSample(sampleKeyString);

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     bool isData = false;

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = allSamples[keyString].getWeight();
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ){ scaleMC = 1.0; isData = true; }

     if( tr ) delete tr;
     std::set<std::string> activatedBranch;
     for(auto& branch : AnaConsts::activatedBranchNames_DataOnly) activatedBranch.insert(branch);
     for(auto& branch : AnaConsts::activatedBranchNames) activatedBranch.insert(branch);
     activatedBranch.insert("selGenParticle"); activatedBranch.insert("selPDGid");
     activatedBranch.insert("recoJetsFlavor");

     tr = new NTupleReader(treeVec[ist], activatedBranch);
     tr->registerFunction(&mypassBaselineFunc);
     if(!isData) tr->registerFunction(&myPDFUncertaintyFunc);

//     tr->registerFunction(&sysPrepFunc);
     tr->registerFunction(sysPrep);

     tr->registerFunction(&jecUpBaselineFunc);
     tr->registerFunction(&jecDnBaselineFunc);
     tr->registerFunction(&metMagUpBaselineFunc);
     tr->registerFunction(&metMagDnBaselineFunc);
     tr->registerFunction(&metPhiUpBaselineFunc);
     tr->registerFunction(&metPhiDnBaselineFunc);

     BTagCorrector btagcorr;
     btagcorr.SetFastSim(true); btagcorr.SetCalibFastSim("CSV_13TEV_Combined_20_11_2015.csv");
     tr->registerFunction(btagcorr);

     int entries = tr->getNEntries();
     if( entryToProcess >0 ) entries = entryToProcess;
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     while(tr->getNextEvent()){

        if( entryToProcess >0 && tr->getEvtNum() > entryToProcess ) break;

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (entries>=10 && (tr->getEvtNum()-1)%(entries/10) == 0) ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;

        const double& SusyMotherMass  = tr->getVar<double>("SusyMotherMass");
        const double& SusyLSPMass     = tr->getVar<double>("SusyLSPMass");

        std::pair<int, int> iMP((int)SusyMotherMass, (int)SusyLSPMass);

        auto iter = histVec.find(iMP);
        if(iter == histVec.end()) iter = histVec.emplace(iMP, HistContainer(iMP.first, iMP.second)).first;

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
        const double met = tr->getVar<double>("met");
        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
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

        const int searchBinIdx = find_Binning_Index(nbJets, nTops, MT2, met);

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
   
           h1_met_looseVec.back()->Fill(tr->getVar<double>("met"), evtWeight * scaleMC);
           h1_metphi_looseVec.back()->Fill(tr->getVar<double>("metphi"), evtWeight * scaleMC);

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

        h1_met_baselineVec.back()->Fill(tr->getVar<double>("met"), evtWeight * scaleMC);
        h1_metphi_baselineVec.back()->Fill(tr->getVar<double>("metphi"), evtWeight * scaleMC);

        h1_mht_baselineVec.back()->Fill(mhtLVec.Pt(), evtWeight * scaleMC);
        h1_mhtphi_baselineVec.back()->Fill(mhtLVec.Phi(), evtWeight * scaleMC);
   
        h1_MT2_baselineVec.back()->Fill(MT2, evtWeight * scaleMC);
        h1_HT_baselineVec.back()->Fill(HT, evtWeight * scaleMC);

        h2_MT2_vs_met_baselineVec.back()->Fill(tr->getVar<double>("met"), MT2, evtWeight * scaleMC);

        for(unsigned int it=0; it<nTops; it++){
           TLorentzVector topLVec = type3Ptr->buildLVec(jetsLVec_forTagger, type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
           h1_topMass_baselineVec.back()->Fill(topLVec.M(), evtWeight * scaleMC);
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

   AnaFunctions::prepareForNtupleReader();
   AnaFunctions::prepareTopTagger();

   type3Ptr->setdebug(true);

   build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   print_searchBins();
   print_searchBins_latex();

   declHistGlobal();

   NTupleReader *tr = 0;
   SRblv = new BaselineVessel(spec, "fastsim");
   pdfScale = new PDFUncertainty();

   SRblv_jecUp = new BaselineVessel(spec_jecUp, "fastsim");
   SRblv_jecDn = new BaselineVessel(spec_jecDn, "fastsim");
   SRblv_metMagUp = new BaselineVessel(spec_metMagUp, "fastsim");
   SRblv_metMagDn = new BaselineVessel(spec_metMagDn, "fastsim");
   SRblv_metPhiUp = new BaselineVessel(spec_metPhiUp, "fastsim");
   SRblv_metPhiDn = new BaselineVessel(spec_metPhiDn, "fastsim");

   int startfile = 0, filerun = -1;

   std::string selKeyStr;
   if( argc >=2 ){
      selKeyStr = argv[1];
      std::cout<<"selKeyStr : "<<selKeyStr<<std::endl;
   }

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
         }
      }
   }

   std::string condorSpec;
   if( argc >=6 ){
      condorSpec = argv[5];
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
 
   for(const auto & filelist : allCollections){

      if( !keyStrVec.empty() ){
         bool found = false;
         for(auto& str : keyStrVec ){ if( filelist.first == str ) found = true; }
         if( !found && entryToProcess !=0 ) continue;
      }

      if( entryToProcess == 0 ){
         bool found = false;
         for(auto & file : filelist.second){
            std::string perSubStr;
            for(const auto & perST : allSamples ){ if(perST.second == file ) perSubStr = perST.first; }
            for(auto& str : keyStrVec ){ if( perSubStr == str ) found = true; }
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

// Throw away low HT bin samples since we have HT>500 GeV cut...
/*
         if(filelist.first == "WJetsToLNu" && (perSubStr == "WJetsToLNu_HT_100to200" || perSubStr == "WJetsToLNu_HT_200to400") ) continue;
         if(filelist.first == "ZJetsToNuNu" && (perSubStr == "ZJetsToNuNu_HT_100to200" || perSubStr == "ZJetsToNuNu_HT_200to400") ) continue;
         if(filelist.first == "QCD" && (perSubStr == "QCD_HT100to200" || perSubStr == "QCD_HT200to300" || perSubStr == "QCD_HT300to500") ) continue;
*/
         std::cout<<"  "<<perSubStr.c_str();

         TChain *aux = new TChain(file.treePath.c_str());  
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

