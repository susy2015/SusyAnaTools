#ifndef BASIC_HISTS_H
#define BASIC_HISTS_H

#include <sstream>
#include <iostream>
#include <fstream>

#include "TH1D.h"
#include "TH2D.h"
#include "TH2Poly.h"
#include "TString.h"

#include "searchBins.h"

const double nTops_SR_lo[]   = { 1,  1,  1,  2,  2,  2,  3,  3,  3 };
const double nTops_SR_hi[]   = { 2,  2,  2,  3,  3,  3, -1, -1, -1 };

const double nbJets_SR_lo[]  = { 1,  2,  3,  1,  2,  3,  1,  2,  3 };
const double nbJets_SR_hi[]  = { 2,  3, -1,  2,  3, -1,  2,  3, -1 };

const std::string keyStr_nTops_SR[]  = { "EQ1", "EQ1", "EQ1", "EQ2", "EQ2", "EQ2", "LE3", "LE3", "LE3" };
const std::string disStr_nTops_SR[]  = {  "=1",  "=1",  "=1",  "=2",  "=2",  "=2", "#geq3", "#geq3", "#geq3" };

const std::string keyStr_nbJets_SR[] = { "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3" };
const std::string disStr_nbJets_SR[] = {  "=1",  "=2", "#geq3",  "=1",  "=2", "#geq3",  "=1",  "=2", "#geq3" };

const int nSR = sizeof(nTops_SR_lo)/sizeof(nTops_SR_lo[0]);

const double max_MT2_for_binEdge = 450, max_met_for_binEdge = 800;
const double max_MT2_for_binCent = max_MT2_for_binEdge + 50, max_met_for_binCent = max_met_for_binEdge + 25;
const double pseudoMax_MT2_for_hist = max_MT2_for_binEdge + 100, pseudoMax_met_for_hist = max_met_for_binEdge + 50;

char names[200], dispt[200];

TH2D * h2_evtCnt_sumSM_nbJets_vs_nTops = 0;
std::vector<TString> keyWordVec;

std::vector<TH1D*> h1_cutFlowVec, h1_cutFlow_auxVec, h1_cutFlow_miscVec;
TH1D * h1_keyString =0;

std::vector<TH2D*> h2_evtCnt_nbJets_vs_nTopsVec;

std::vector<std::string> declaredSampleStrVec;
std::vector<std::vector<TH2Poly*> > h2_poly_MT2_vs_metVec;

std::vector<std::vector<TH1D*> > h1_nJetsVec, h1_metVec, h1_MT2Vec, h1_mTcombVec, h1_HTVec, h1_nJetsRsysVec;

std::vector<std::vector<TH1D*> > h1_minDphi_topmetVec;

std::vector<TH1D*> h1_nJets_looseVec, h1_nTops_looseVec, h1_nbJets_looseVec;
std::vector<TH1D*> h1_met_looseVec, h1_MT2_looseVec, h1_HT_looseVec, h1_metphi_looseVec, h1_mht_looseVec, h1_mhtphi_looseVec;
std::vector<TH1D*> h1_genmet_looseVec, h1_genmetphi_looseVec;
std::vector<TH1D*> h1_met_absres_looseVec, h1_met_relres_looseVec;
std::vector<TH1D*> h1_dphi1_looseVec, h1_dphi2_looseVec, h1_dphi3_looseVec;
std::vector<TH1D*> h1_topMass_looseVec;
std::vector<TH1D*> h1_vtxSize_looseVec;
std::vector<TH1D*> h1_allJetPt_looseVec, h1_allJetEta_looseVec, h1_allJetPhi_looseVec, h1_allJetM_looseVec;
std::vector<TH1D*> h1_leadJetPt_looseVec, h1_leadJetEta_looseVec, h1_leadJetPhi_looseVec, h1_leadJetM_looseVec;
std::vector<TH1D*> h1_muPt_looseVec, h1_muEta_looseVec, h1_muPhi_looseVec;
std::vector<TH1D*> h1_elePt_looseVec, h1_eleEta_looseVec, h1_elePhi_looseVec;

std::vector<TH1D*> h1_met_loose_allHadVec, h1_nJets_loose_allHadVec, h1_nTops_loose_allHadVec, h1_nbJets_loose_allHadVec, h1_MT2_loose_allHadVec;
std::vector<TH1D*> h1_met_loose_leptonicVec, h1_nJets_loose_leptonicVec, h1_nTops_loose_leptonicVec, h1_nbJets_loose_leptonicVec, h1_MT2_loose_leptonicVec;
std::vector<TH1D*> h1_genmet_loose_allHadVec, h1_genmet_loose_leptonicVec;
std::vector<TH1D*> h1_met_absres_loose_allHadVec, h1_met_relres_loose_allHadVec;
std::vector<TH1D*> h1_met_absres_loose_leptonicVec, h1_met_relres_loose_leptonicVec;

std::vector<TH1D*> h1_met_baseline_allHadVec, h1_nJets_baseline_allHadVec, h1_nTops_baseline_allHadVec, h1_nbJets_baseline_allHadVec, h1_MT2_baseline_allHadVec;
std::vector<TH1D*> h1_met_baseline_leptonicVec, h1_nJets_baseline_leptonicVec, h1_nTops_baseline_leptonicVec, h1_nbJets_baseline_leptonicVec, h1_MT2_baseline_leptonicVec;
std::vector<TH1D*> h1_genmet_baseline_allHadVec, h1_genmet_baseline_leptonicVec;
std::vector<TH1D*> h1_met_absres_baseline_allHadVec, h1_met_relres_baseline_allHadVec;
std::vector<TH1D*> h1_met_absres_baseline_leptonicVec, h1_met_relres_baseline_leptonicVec;

std::vector<TH1D*> h1_nJets_baselineVec, h1_nTops_baselineVec, h1_nbJets_baselineVec;
std::vector<TH1D*> h1_met_baselineVec, h1_MT2_baselineVec, h1_HT_baselineVec, h1_metphi_baselineVec, h1_mht_baselineVec, h1_mhtphi_baselineVec;
std::vector<TH1D*> h1_genmet_baselineVec, h1_genmetphi_baselineVec;
std::vector<TH1D*> h1_met_absres_baselineVec, h1_met_relres_baselineVec;
std::vector<TH1D*> h1_dphi1_baselineVec, h1_dphi2_baselineVec, h1_dphi3_baselineVec;
std::vector<TH1D*> h1_topMass_baselineVec;
std::vector<TH1D*> h1_vtxSize_baselineVec;
std::vector<TH1D*> h1_allJetPt_baselineVec, h1_allJetEta_baselineVec, h1_allJetPhi_baselineVec, h1_allJetM_baselineVec;
std::vector<TH1D*> h1_leadJetPt_baselineVec, h1_leadJetEta_baselineVec, h1_leadJetPhi_baselineVec, h1_leadJetM_baselineVec;
std::vector<TH1D*> h1_muPt_baselineVec, h1_muEta_baselineVec, h1_muPhi_baselineVec;
std::vector<TH1D*> h1_elePt_baselineVec, h1_eleEta_baselineVec, h1_elePhi_baselineVec;

std::vector<TH1D*> h1_nTops_Nm1_baselineVec, h1_nbJets_Nm1_baselineVec, h1_met_Nm1_baselineVec, h1_MT2_Nm1_baselineVec;

std::vector<TH1D*> h1_searchBinYieldsVec, h1_searchBinYields_HadTauVec, h1_searchBinYields_LostLepVec, h1_searchBinYields_OverlapVec;
std::vector<TH1D*> h1_no_fastsimFilter_searchBinYieldsVec;

std::vector<TH1D*> h1_scaleUncNominal_searchBinYieldsVec, h1_scaleUncUp_searchBinYieldsVec, h1_scaleUncDown_searchBinYieldsVec;
std::vector<TH1D*> h1_pdfUncCentral_searchBinYieldsVec, h1_pdfUncUp_searchBinYieldsVec, h1_pdfUncDown_searchBinYieldsVec;

std::vector<TH2D*> h2_nbJets_vs_nJetsVec, h2_nTops_vs_nJetsVec, h2_nTops_vs_nbJetsVec;
std::vector<TH2D*> h2_nbJets_vs_nJets_baselineVec, h2_nTops_vs_nJets_baselineVec, h2_nTops_vs_nbJets_baselineVec;

std::vector<TH2D*> h2_MT2_vs_met_baselineVec;

std::vector<TH2D*> h2_MT2_vs_met_looseVec, h2_alt_MT2_vs_met_looseVec, h2_MT2_vs_alt_MT2_looseVec;

std::vector<TH1D*> h1_mtw_looseVec, h1_mtw_baselineVec;

void declHistPerSample(const std::string &sampleKeyString, const int nTotBins, const std::vector<std::vector<std::vector<double> > > & out_MT2_met_Binning_forTH2Poly){

  const TString sampleKeyStringT = sampleKeyString;

  TH1D * h1_searchBinYields = new TH1D(sampleKeyStringT+"_h1_searchBinYields", sampleKeyStringT+": search bin yields", nTotBins, 0, nTotBins); h1_searchBinYields->Sumw2(); h1_searchBinYieldsVec.push_back((TH1D*)h1_searchBinYields->Clone());
  TH1D * h1_searchBinYields_HadTau = new TH1D(sampleKeyStringT+"_h1_searchBinYields_HadTau", sampleKeyStringT+": search bin yields for HadTau", nTotBins, 0, nTotBins); h1_searchBinYields_HadTau->Sumw2(); h1_searchBinYields_HadTauVec.push_back((TH1D*)h1_searchBinYields_HadTau->Clone());
  TH1D * h1_searchBinYields_LostLep = new TH1D(sampleKeyStringT+"_h1_searchBinYields_LostLep", sampleKeyStringT+": search bin yields for LostLep", nTotBins, 0, nTotBins); h1_searchBinYields_LostLep->Sumw2(); h1_searchBinYields_LostLepVec.push_back((TH1D*)h1_searchBinYields_LostLep->Clone());
  TH1D * h1_searchBinYields_Overlap = new TH1D(sampleKeyStringT+"_h1_searchBinYields_Overlap", sampleKeyStringT+": search bin yields for Overlap", nTotBins, 0, nTotBins); h1_searchBinYields_Overlap->Sumw2(); h1_searchBinYields_OverlapVec.push_back((TH1D*)h1_searchBinYields_Overlap->Clone());

  TH1D * h1_no_fastsimFilter_searchBinYields = new TH1D(sampleKeyStringT+"_h1_no_fastsimFilter_searchBinYields", sampleKeyStringT+": search bin yields", nTotBins, 0, nTotBins); h1_no_fastsimFilter_searchBinYields->Sumw2(); h1_no_fastsimFilter_searchBinYieldsVec.push_back((TH1D*)h1_no_fastsimFilter_searchBinYields->Clone());

  TH1D * h1_scaleUncNominal_searchBinYields = new TH1D(sampleKeyStringT+"_h1_scaleUncNominal_searchBinYields", sampleKeyStringT+": scale unc norminal", nTotBins, 0, nTotBins); h1_scaleUncNominal_searchBinYields->Sumw2(); h1_scaleUncNominal_searchBinYieldsVec.push_back((TH1D*)h1_scaleUncNominal_searchBinYields->Clone());
  TH1D * h1_scaleUncUp_searchBinYields = new TH1D(sampleKeyStringT+"_h1_scaleUncUp_searchBinYields", sampleKeyStringT+": scale unc up", nTotBins, 0, nTotBins); h1_scaleUncUp_searchBinYields->Sumw2(); h1_scaleUncUp_searchBinYieldsVec.push_back((TH1D*)h1_scaleUncUp_searchBinYields->Clone());
  TH1D * h1_scaleUncDown_searchBinYields = new TH1D(sampleKeyStringT+"_h1_scaleUncDown_searchBinYields", sampleKeyStringT+": scale unc down", nTotBins, 0, nTotBins); h1_scaleUncDown_searchBinYields->Sumw2(); h1_scaleUncDown_searchBinYieldsVec.push_back((TH1D*)h1_scaleUncDown_searchBinYields->Clone());

  TH1D * h1_pdfUncCentral_searchBinYields = new TH1D(sampleKeyStringT+"_h1_pdfUncCentral_searchBinYields", sampleKeyStringT+": pdf unc central", nTotBins, 0, nTotBins); h1_pdfUncCentral_searchBinYields->Sumw2(); h1_pdfUncCentral_searchBinYieldsVec.push_back((TH1D*)h1_pdfUncCentral_searchBinYields->Clone());
  TH1D * h1_pdfUncUp_searchBinYields = new TH1D(sampleKeyStringT+"_h1_pdfUncUp_searchBinYields", sampleKeyStringT+": pdf unc up", nTotBins, 0, nTotBins); h1_pdfUncUp_searchBinYields->Sumw2(); h1_pdfUncUp_searchBinYieldsVec.push_back((TH1D*)h1_pdfUncUp_searchBinYields->Clone());
  TH1D * h1_pdfUncDown_searchBinYields = new TH1D(sampleKeyStringT+"_h1_pdfUncDown_searchBinYields", sampleKeyStringT+": pdf unc down", nTotBins, 0, nTotBins); h1_pdfUncDown_searchBinYields->Sumw2(); h1_pdfUncDown_searchBinYieldsVec.push_back((TH1D*)h1_pdfUncDown_searchBinYields->Clone());

  TH1D * h1_cutFlow = new TH1D(sampleKeyStringT+"_h1_cutFlow", sampleKeyStringT+": cut flow table", 20, 0, 20); h1_cutFlow->SetBit(TH1::kCanRebin); h1_cutFlow->Sumw2(); h1_cutFlowVec.push_back((TH1D*)h1_cutFlow->Clone());
  TH1D * h1_cutFlow_aux = new TH1D(sampleKeyStringT+"_h1_cutFlow_aux", sampleKeyStringT+": more cut flow table", 20, 0, 20); h1_cutFlow_aux->SetBit(TH1::kCanRebin); h1_cutFlow_aux->Sumw2(); h1_cutFlow_auxVec.push_back((TH1D*)h1_cutFlow_aux->Clone());
  TH1D * h1_cutFlow_misc = new TH1D(sampleKeyStringT+"_h1_cutFlow_misc", sampleKeyStringT+": more cut flow table", 20, 0, 20); h1_cutFlow_misc->SetBit(TH1::kCanRebin); h1_cutFlow_misc->Sumw2(); h1_cutFlow_miscVec.push_back((TH1D*)h1_cutFlow_misc->Clone());

  TH1D * h1_nJets_loose = new TH1D(sampleKeyStringT+"_h1_nJets_loose", sampleKeyStringT+": nJets after loose; nJets", 14, 0, 14); h1_nJets_loose->Sumw2(); h1_nJets_looseVec.push_back((TH1D*)h1_nJets_loose->Clone());
  TH1D * h1_nbJets_loose = new TH1D(sampleKeyStringT+"_h1_nbJets_loose", sampleKeyStringT+": nbJets after loose; nbJets", 5, 0, 5); h1_nbJets_loose->Sumw2(); h1_nbJets_looseVec.push_back((TH1D*)h1_nbJets_loose->Clone());
  TH1D * h1_nTops_loose = new TH1D(sampleKeyStringT+"_h1_nTops_loose", sampleKeyStringT+": nTops after loose; nTops", 5, 0, 5); h1_nTops_loose->Sumw2(); h1_nTops_looseVec.push_back((TH1D*)h1_nTops_loose->Clone());

  TH1D * h1_met_loose = new TH1D(sampleKeyStringT+"_h1_met_loose", sampleKeyStringT+": met after loose; met [GeV]", 100, 0, 1000); h1_met_loose->Sumw2(); h1_met_looseVec.push_back((TH1D*)h1_met_loose->Clone());
  TH1D * h1_MT2_loose = new TH1D(sampleKeyStringT+"_h1_MT2_loose", sampleKeyStringT+": MT2 after loose; MT2 [GeV]", 100, 0, 1000); h1_MT2_loose->Sumw2(); h1_MT2_looseVec.push_back((TH1D*)h1_MT2_loose->Clone());
  TH1D * h1_HT_loose = new TH1D(sampleKeyStringT+"_h1_HT_loose", sampleKeyStringT+": HT after loose; HT [GeV]", 100, 0, 1000); h1_HT_loose->Sumw2(); h1_HT_looseVec.push_back((TH1D*)h1_HT_loose->Clone());
  TH1D * h1_metphi_loose = new TH1D(sampleKeyStringT+"_h1_metphi_loose", sampleKeyStringT+": metphi after loose; metphi", 100, -3.2, 3.2); h1_metphi_loose->Sumw2(); h1_metphi_looseVec.push_back((TH1D*)h1_metphi_loose->Clone());
  TH1D * h1_genmet_loose = new TH1D(sampleKeyStringT+"_h1_genmet_loose", sampleKeyStringT+": genmet after loose; genmet [GeV]", 100, 0, 1000); h1_genmet_loose->Sumw2(); h1_genmet_looseVec.push_back((TH1D*)h1_genmet_loose->Clone());
  TH1D * h1_genmetphi_loose = new TH1D(sampleKeyStringT+"_h1_genmetphi_loose", sampleKeyStringT+": genmetphi after loose; genmetphi", 100, -3.2, 3.2); h1_genmetphi_loose->Sumw2(); h1_genmetphi_looseVec.push_back((TH1D*)h1_genmetphi_loose->Clone());
  TH1D * h1_met_absres_loose = new TH1D(sampleKeyStringT+"_h1_met_absres_loose", sampleKeyStringT+": met absres after loose; met-genmet [GeV]", 100, -200, 200); h1_met_absres_loose->Sumw2(); h1_met_absres_looseVec.push_back((TH1D*)h1_met_absres_loose->Clone());
  TH1D * h1_met_relres_loose = new TH1D(sampleKeyStringT+"_h1_met_relres_loose", sampleKeyStringT+": met relres after loose; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_loose->Sumw2(); h1_met_relres_looseVec.push_back((TH1D*)h1_met_relres_loose->Clone());

  TH1D * h1_nJets_loose_allHad = new TH1D(sampleKeyStringT+"_h1_nJets_loose_allHad", sampleKeyStringT+": nJets after loose_allHad; nJets", 14, 0, 14); h1_nJets_loose_allHad->Sumw2(); h1_nJets_loose_allHadVec.push_back((TH1D*)h1_nJets_loose_allHad->Clone());
  TH1D * h1_nbJets_loose_allHad = new TH1D(sampleKeyStringT+"_h1_nbJets_loose_allHad", sampleKeyStringT+": nbJets after loose_allHad; nbJets", 5, 0, 5); h1_nbJets_loose_allHad->Sumw2(); h1_nbJets_loose_allHadVec.push_back((TH1D*)h1_nbJets_loose_allHad->Clone());
  TH1D * h1_nTops_loose_allHad = new TH1D(sampleKeyStringT+"_h1_nTops_loose_allHad", sampleKeyStringT+": nTops after loose_allHad; nTops", 5, 0, 5); h1_nTops_loose_allHad->Sumw2(); h1_nTops_loose_allHadVec.push_back((TH1D*)h1_nTops_loose_allHad->Clone());
  TH1D * h1_met_loose_allHad = new TH1D(sampleKeyStringT+"_h1_met_loose_allHad", sampleKeyStringT+": met after loose_allHad; met [GeV]", 100, 0, 1000); h1_met_loose_allHad->Sumw2(); h1_met_loose_allHadVec.push_back((TH1D*)h1_met_loose_allHad->Clone());
  TH1D * h1_MT2_loose_allHad = new TH1D(sampleKeyStringT+"_h1_MT2_loose_allHad", sampleKeyStringT+": MT2 after loose_allHad; MT2 [GeV]", 100, 0, 1000); h1_MT2_loose_allHad->Sumw2(); h1_MT2_loose_allHadVec.push_back((TH1D*)h1_MT2_loose_allHad->Clone());
  TH1D * h1_genmet_loose_allHad = new TH1D(sampleKeyStringT+"_h1_genmet_loose_allHad", sampleKeyStringT+": genmet after loose_allHad; genmet [GeV]", 100, 0, 1000); h1_genmet_loose_allHad->Sumw2(); h1_genmet_loose_allHadVec.push_back((TH1D*)h1_genmet_loose_allHad->Clone());
  TH1D * h1_met_absres_loose_allHad = new TH1D(sampleKeyStringT+"_h1_met_absres_loose_allHad", sampleKeyStringT+": met absres after loose_allHad; met-genmet [GeV]", 100, -200, 200); h1_met_absres_loose_allHad->Sumw2(); h1_met_absres_loose_allHadVec.push_back((TH1D*)h1_met_absres_loose_allHad->Clone());
  TH1D * h1_met_relres_loose_allHad = new TH1D(sampleKeyStringT+"_h1_met_relres_loose_allHad", sampleKeyStringT+": met relres after loose_allHad; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_loose_allHad->Sumw2(); h1_met_relres_loose_allHadVec.push_back((TH1D*)h1_met_relres_loose_allHad->Clone());

  TH1D * h1_nJets_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_nJets_loose_leptonic", sampleKeyStringT+": nJets after loose_leptonic; nJets", 14, 0, 14); h1_nJets_loose_leptonic->Sumw2(); h1_nJets_loose_leptonicVec.push_back((TH1D*)h1_nJets_loose_leptonic->Clone());
  TH1D * h1_nbJets_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_nbJets_loose_leptonic", sampleKeyStringT+": nbJets after loose_leptonic; nbJets", 5, 0, 5); h1_nbJets_loose_leptonic->Sumw2(); h1_nbJets_loose_leptonicVec.push_back((TH1D*)h1_nbJets_loose_leptonic->Clone());
  TH1D * h1_nTops_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_nTops_loose_leptonic", sampleKeyStringT+": nTops after loose_leptonic; nTops", 5, 0, 5); h1_nTops_loose_leptonic->Sumw2(); h1_nTops_loose_leptonicVec.push_back((TH1D*)h1_nTops_loose_leptonic->Clone());
  TH1D * h1_met_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_met_loose_leptonic", sampleKeyStringT+": met after loose_leptonic; met [GeV]", 100, 0, 1000); h1_met_loose_leptonic->Sumw2(); h1_met_loose_leptonicVec.push_back((TH1D*)h1_met_loose_leptonic->Clone());
  TH1D * h1_MT2_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_MT2_loose_leptonic", sampleKeyStringT+": MT2 after loose_leptonic; MT2 [GeV]", 100, 0, 1000); h1_MT2_loose_leptonic->Sumw2(); h1_MT2_loose_leptonicVec.push_back((TH1D*)h1_MT2_loose_leptonic->Clone());
  TH1D * h1_genmet_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_genmet_loose_leptonic", sampleKeyStringT+": genmet after loose_leptonic; genmet [GeV]", 100, 0, 1000); h1_genmet_loose_leptonic->Sumw2(); h1_genmet_loose_leptonicVec.push_back((TH1D*)h1_genmet_loose_leptonic->Clone());
  TH1D * h1_met_absres_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_met_absres_loose_leptonic", sampleKeyStringT+": met absres after loose_leptonic; met-genmet [GeV]", 100, -200, 200); h1_met_absres_loose_leptonic->Sumw2(); h1_met_absres_loose_leptonicVec.push_back((TH1D*)h1_met_absres_loose_leptonic->Clone());
  TH1D * h1_met_relres_loose_leptonic = new TH1D(sampleKeyStringT+"_h1_met_relres_loose_leptonic", sampleKeyStringT+": met relres after loose_leptonic; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_loose_leptonic->Sumw2(); h1_met_relres_loose_leptonicVec.push_back((TH1D*)h1_met_relres_loose_leptonic->Clone());

  TH1D * h1_nJets_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_nJets_baseline_allHad", sampleKeyStringT+": nJets after baseline_allHad; nJets", 14, 0, 14); h1_nJets_baseline_allHad->Sumw2(); h1_nJets_baseline_allHadVec.push_back((TH1D*)h1_nJets_baseline_allHad->Clone());
  TH1D * h1_nbJets_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_nbJets_baseline_allHad", sampleKeyStringT+": nbJets after baseline_allHad; nbJets", 5, 0, 5); h1_nbJets_baseline_allHad->Sumw2(); h1_nbJets_baseline_allHadVec.push_back((TH1D*)h1_nbJets_baseline_allHad->Clone());
  TH1D * h1_nTops_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_nTops_baseline_allHad", sampleKeyStringT+": nTops after baseline_allHad; nTops", 5, 0, 5); h1_nTops_baseline_allHad->Sumw2(); h1_nTops_baseline_allHadVec.push_back((TH1D*)h1_nTops_baseline_allHad->Clone());
  TH1D * h1_met_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_met_baseline_allHad", sampleKeyStringT+": met after baseline_allHad; met [GeV]", 100, 0, 1000); h1_met_baseline_allHad->Sumw2(); h1_met_baseline_allHadVec.push_back((TH1D*)h1_met_baseline_allHad->Clone());
  TH1D * h1_MT2_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_MT2_baseline_allHad", sampleKeyStringT+": MT2 after baseline_allHad; MT2 [GeV]", 100, 0, 1000); h1_MT2_baseline_allHad->Sumw2(); h1_MT2_baseline_allHadVec.push_back((TH1D*)h1_MT2_baseline_allHad->Clone());
  TH1D * h1_genmet_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_genmet_baseline_allHad", sampleKeyStringT+": genmet after baseline_allHad; genmet [GeV]", 100, 0, 1000); h1_genmet_baseline_allHad->Sumw2(); h1_genmet_baseline_allHadVec.push_back((TH1D*)h1_genmet_baseline_allHad->Clone());
  TH1D * h1_met_absres_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_met_absres_baseline_allHad", sampleKeyStringT+": met absres after baseline_allHad; met-genmet [GeV]", 100, -200, 200); h1_met_absres_baseline_allHad->Sumw2(); h1_met_absres_baseline_allHadVec.push_back((TH1D*)h1_met_absres_baseline_allHad->Clone());
  TH1D * h1_met_relres_baseline_allHad = new TH1D(sampleKeyStringT+"_h1_met_relres_baseline_allHad", sampleKeyStringT+": met relres after baseline_allHad; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_baseline_allHad->Sumw2(); h1_met_relres_baseline_allHadVec.push_back((TH1D*)h1_met_relres_baseline_allHad->Clone());

  TH1D * h1_nJets_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_nJets_baseline_leptonic", sampleKeyStringT+": nJets after baseline_leptonic; nJets", 14, 0, 14); h1_nJets_baseline_leptonic->Sumw2(); h1_nJets_baseline_leptonicVec.push_back((TH1D*)h1_nJets_baseline_leptonic->Clone());
  TH1D * h1_nbJets_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_nbJets_baseline_leptonic", sampleKeyStringT+": nbJets after baseline_leptonic; nbJets", 5, 0, 5); h1_nbJets_baseline_leptonic->Sumw2(); h1_nbJets_baseline_leptonicVec.push_back((TH1D*)h1_nbJets_baseline_leptonic->Clone());
  TH1D * h1_nTops_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_nTops_baseline_leptonic", sampleKeyStringT+": nTops after baseline_leptonic; nTops", 5, 0, 5); h1_nTops_baseline_leptonic->Sumw2(); h1_nTops_baseline_leptonicVec.push_back((TH1D*)h1_nTops_baseline_leptonic->Clone());
  TH1D * h1_met_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_met_baseline_leptonic", sampleKeyStringT+": met after baseline_leptonic; met [GeV]", 100, 0, 1000); h1_met_baseline_leptonic->Sumw2(); h1_met_baseline_leptonicVec.push_back((TH1D*)h1_met_baseline_leptonic->Clone());
  TH1D * h1_MT2_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_MT2_baseline_leptonic", sampleKeyStringT+": MT2 after baseline_leptonic; MT2 [GeV]", 100, 0, 1000); h1_MT2_baseline_leptonic->Sumw2(); h1_MT2_baseline_leptonicVec.push_back((TH1D*)h1_MT2_baseline_leptonic->Clone());
  TH1D * h1_genmet_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_genmet_baseline_leptonic", sampleKeyStringT+": genmet after baseline_leptonic; genmet [GeV]", 100, 0, 1000); h1_genmet_baseline_leptonic->Sumw2(); h1_genmet_baseline_leptonicVec.push_back((TH1D*)h1_genmet_baseline_leptonic->Clone());
  TH1D * h1_met_absres_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_met_absres_baseline_leptonic", sampleKeyStringT+": met absres after baseline_leptonic; met-genmet [GeV]", 100, -200, 200); h1_met_absres_baseline_leptonic->Sumw2(); h1_met_absres_baseline_leptonicVec.push_back((TH1D*)h1_met_absres_baseline_leptonic->Clone());
  TH1D * h1_met_relres_baseline_leptonic = new TH1D(sampleKeyStringT+"_h1_met_relres_baseline_leptonic", sampleKeyStringT+": met relres after baseline_leptonic; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_baseline_leptonic->Sumw2(); h1_met_relres_baseline_leptonicVec.push_back((TH1D*)h1_met_relres_baseline_leptonic->Clone());

  TH1D * h1_mht_loose = new TH1D(sampleKeyStringT+"_h1_mht_loose", sampleKeyStringT+": mht after loose; mht [GeV]", 100, 0, 1000); h1_mht_loose->Sumw2(); h1_mht_looseVec.push_back((TH1D*)h1_mht_loose->Clone());
  TH1D * h1_mhtphi_loose = new TH1D(sampleKeyStringT+"_h1_mhtphi_loose", sampleKeyStringT+": mhtphi after loose; mhtphi", 100, -3.2, 3.2); h1_mhtphi_loose->Sumw2(); h1_mhtphi_looseVec.push_back((TH1D*)h1_mhtphi_loose->Clone());

  TH1D * h1_dphi1_loose = new TH1D(sampleKeyStringT+"_h1_dphi1_loose", sampleKeyStringT+": dphi1 after loose; dphi1", 100, 0.0, 3.2); h1_dphi1_loose->Sumw2(); h1_dphi1_looseVec.push_back((TH1D*)h1_dphi1_loose->Clone());
  TH1D * h1_dphi2_loose = new TH1D(sampleKeyStringT+"_h1_dphi2_loose", sampleKeyStringT+": dphi2 after loose; dphi2", 100, 0.0, 3.2); h1_dphi2_loose->Sumw2(); h1_dphi2_looseVec.push_back((TH1D*)h1_dphi2_loose->Clone());
  TH1D * h1_dphi3_loose = new TH1D(sampleKeyStringT+"_h1_dphi3_loose", sampleKeyStringT+": dphi3 after loose; dphi3", 100, 0.0, 3.2); h1_dphi3_loose->Sumw2(); h1_dphi3_looseVec.push_back((TH1D*)h1_dphi3_loose->Clone());

  TH1D * h1_topMass_loose = new TH1D(sampleKeyStringT+"_h1_topMass_loose", sampleKeyStringT+": top mass after loose; topM [GeV]", 100, 0, 300); h1_topMass_loose->Sumw2(); h1_topMass_looseVec.push_back((TH1D*)h1_topMass_loose->Clone()); 

  TH1D * h1_vtxSize_loose = new TH1D(sampleKeyStringT+"_h1_vtxSize_loose", sampleKeyStringT+": vtxSize after loose; vtxSize", 50, 0, 50); h1_vtxSize_loose->Sumw2(); h1_vtxSize_looseVec.push_back((TH1D*)h1_vtxSize_loose->Clone());

  TH1D * h1_allJetPt_loose = new TH1D(sampleKeyStringT+"_h1_allJetPt_loose", sampleKeyStringT+": allJetPt after loose; allJetPt [GeV]", 100, 0, 1000); h1_allJetPt_loose->Sumw2(); h1_allJetPt_looseVec.push_back((TH1D*)h1_allJetPt_loose->Clone());
  TH1D * h1_allJetEta_loose = new TH1D(sampleKeyStringT+"_h1_allJetEta_loose", sampleKeyStringT+": allJetEta after loose; allJetEta", 100, -5, 5); h1_allJetEta_loose->Sumw2(); h1_allJetEta_looseVec.push_back((TH1D*)h1_allJetEta_loose->Clone());
  TH1D * h1_allJetPhi_loose = new TH1D(sampleKeyStringT+"_h1_allJetPhi_loose", sampleKeyStringT+": allJetPhi after loose; allJetPhi", 100, -3.2, 3.2); h1_allJetPhi_loose->Sumw2(); h1_allJetPhi_looseVec.push_back((TH1D*)h1_allJetPhi_loose->Clone());
  TH1D * h1_allJetM_loose = new TH1D(sampleKeyStringT+"_h1_allJetM_loose", sampleKeyStringT+": allJetM after loose; allJetM [GeV]", 100, 0, 250); h1_allJetM_loose->Sumw2(); h1_allJetM_looseVec.push_back((TH1D*)h1_allJetM_loose->Clone());

  TH1D * h1_leadJetPt_loose = new TH1D(sampleKeyStringT+"_h1_leadJetPt_loose", sampleKeyStringT+": leadJetPt after loose; leadJetPt [GeV]", 100, 0, 1000); h1_leadJetPt_loose->Sumw2(); h1_leadJetPt_looseVec.push_back((TH1D*)h1_leadJetPt_loose->Clone());
  TH1D * h1_leadJetEta_loose = new TH1D(sampleKeyStringT+"_h1_leadJetEta_loose", sampleKeyStringT+": leadJetEta after loose; leadJetEta", 100, -5, 5); h1_leadJetEta_loose->Sumw2(); h1_leadJetEta_looseVec.push_back((TH1D*)h1_leadJetEta_loose->Clone());
  TH1D * h1_leadJetPhi_loose = new TH1D(sampleKeyStringT+"_h1_leadJetPhi_loose", sampleKeyStringT+": leadJetPhi after loose; leadJetPhi", 100, -3.2, 3.2); h1_leadJetPhi_loose->Sumw2(); h1_leadJetPhi_looseVec.push_back((TH1D*)h1_leadJetPhi_loose->Clone());
  TH1D * h1_leadJetM_loose = new TH1D(sampleKeyStringT+"_h1_leadJetM_loose", sampleKeyStringT+": leadJetM after loose; leadJetM [GeV]", 100, 0, 250); h1_leadJetM_loose->Sumw2(); h1_leadJetM_looseVec.push_back((TH1D*)h1_leadJetM_loose->Clone());

  TH1D * h1_muPt_loose = new TH1D(sampleKeyStringT+"_h1_muPt_loose", sampleKeyStringT+": muPt after loose; muPt [GeV]", 100, 0, 500); h1_muPt_loose->Sumw2(); h1_muPt_looseVec.push_back((TH1D*)h1_muPt_loose->Clone());
  TH1D * h1_muEta_loose = new TH1D(sampleKeyStringT+"_h1_muEta_loose", sampleKeyStringT+": muEta after loose; muEta", 100, -5, 5); h1_muEta_loose->Sumw2(); h1_muEta_looseVec.push_back((TH1D*)h1_muEta_loose->Clone());
  TH1D * h1_muPhi_loose = new TH1D(sampleKeyStringT+"_h1_muPhi_loose", sampleKeyStringT+": muPhi after loose; muPhi", 100, -3.2, 3.2); h1_muPhi_loose->Sumw2(); h1_muPhi_looseVec.push_back((TH1D*)h1_muPhi_loose->Clone());

  TH1D * h1_elePt_loose = new TH1D(sampleKeyStringT+"_h1_elePt_loose", sampleKeyStringT+": elePt after loose; elePt [GeV]", 100, 0, 500); h1_elePt_loose->Sumw2(); h1_elePt_looseVec.push_back((TH1D*)h1_elePt_loose->Clone());
  TH1D * h1_eleEta_loose = new TH1D(sampleKeyStringT+"_h1_eleEta_loose", sampleKeyStringT+": eleEta after loose; eleEta", 100, -5, 5); h1_eleEta_loose->Sumw2(); h1_eleEta_looseVec.push_back((TH1D*)h1_eleEta_loose->Clone());
  TH1D * h1_elePhi_loose = new TH1D(sampleKeyStringT+"_h1_elePhi_loose", sampleKeyStringT+": elePhi after loose; elePhi", 100, -3.2, 3.2); h1_elePhi_loose->Sumw2(); h1_elePhi_looseVec.push_back((TH1D*)h1_elePhi_loose->Clone());

  TH1D * h1_mtw_loose = new TH1D(sampleKeyStringT+"_h1_mtw_loose", sampleKeyStringT+": mtw after loose; mtw [GeV]", 500, 0, 500); h1_mtw_loose->Sumw2(); h1_mtw_looseVec.push_back((TH1D*)h1_mtw_loose->Clone());

  TH1D * h1_nJets_baseline = new TH1D(sampleKeyStringT+"_h1_nJets_baseline", sampleKeyStringT+": nJets after baseline; nJets", 14, 0, 14); h1_nJets_baseline->Sumw2(); h1_nJets_baselineVec.push_back((TH1D*)h1_nJets_baseline->Clone());
  TH1D * h1_nbJets_baseline = new TH1D(sampleKeyStringT+"_h1_nbJets_baseline", sampleKeyStringT+": nbJets after baseline; nbJets", 5, 0, 5); h1_nbJets_baseline->Sumw2(); h1_nbJets_baselineVec.push_back((TH1D*)h1_nbJets_baseline->Clone());
  TH1D * h1_nTops_baseline = new TH1D(sampleKeyStringT+"_h1_nTops_baseline", sampleKeyStringT+": nTops after baseline; nTops", 5, 0, 5); h1_nTops_baseline->Sumw2(); h1_nTops_baselineVec.push_back((TH1D*)h1_nTops_baseline->Clone());

  TH1D * h1_met_baseline = new TH1D(sampleKeyStringT+"_h1_met_baseline", sampleKeyStringT+": met after baseline; met [GeV]", 100, 0, 1000); h1_met_baseline->Sumw2(); h1_met_baselineVec.push_back((TH1D*)h1_met_baseline->Clone());
  TH1D * h1_MT2_baseline = new TH1D(sampleKeyStringT+"_h1_MT2_baseline", sampleKeyStringT+": MT2 after baseline; MT2 [GeV]", 100, 0, 1000); h1_MT2_baseline->Sumw2(); h1_MT2_baselineVec.push_back((TH1D*)h1_MT2_baseline->Clone());
  TH1D * h1_HT_baseline = new TH1D(sampleKeyStringT+"_h1_HT_baseline", sampleKeyStringT+": HT after baseline; HT [GeV]", 100, 0, 1000); h1_HT_baseline->Sumw2(); h1_HT_baselineVec.push_back((TH1D*)h1_HT_baseline->Clone());
  TH1D * h1_metphi_baseline = new TH1D(sampleKeyStringT+"_h1_metphi_baseline", sampleKeyStringT+": metphi after baseline; metphi", 100, -3.2, 3.2); h1_metphi_baseline->Sumw2(); h1_metphi_baselineVec.push_back((TH1D*)h1_metphi_baseline->Clone());
  TH1D * h1_genmet_baseline = new TH1D(sampleKeyStringT+"_h1_genmet_baseline", sampleKeyStringT+": genmet after baseline; genmet [GeV]", 100, 0, 1000); h1_genmet_baseline->Sumw2(); h1_genmet_baselineVec.push_back((TH1D*)h1_genmet_baseline->Clone());
  TH1D * h1_genmetphi_baseline = new TH1D(sampleKeyStringT+"_h1_genmetphi_baseline", sampleKeyStringT+": genmetphi after baseline; genmetphi", 100, -3.2, 3.2); h1_genmetphi_baseline->Sumw2(); h1_genmetphi_baselineVec.push_back((TH1D*)h1_genmetphi_baseline->Clone());
  TH1D * h1_met_absres_baseline = new TH1D(sampleKeyStringT+"_h1_met_absres_baseline", sampleKeyStringT+": met absres after baseline; met-genmet [GeV]", 100, -200, 200); h1_met_absres_baseline->Sumw2(); h1_met_absres_baselineVec.push_back((TH1D*)h1_met_absres_baseline->Clone());
  TH1D * h1_met_relres_baseline = new TH1D(sampleKeyStringT+"_h1_met_relres_baseline", sampleKeyStringT+": met relres after baseline; (met-genmet)/genmet", 100, -2, 10); h1_met_relres_baseline->Sumw2(); h1_met_relres_baselineVec.push_back((TH1D*)h1_met_relres_baseline->Clone());

  TH1D * h1_nbJets_Nm1_baseline = new TH1D(sampleKeyStringT+"_h1_nbJets_Nm1_baseline", sampleKeyStringT+": nbJets after Nm1_baseline; nbJets", 5, 0, 5); h1_nbJets_Nm1_baseline->Sumw2(); h1_nbJets_Nm1_baselineVec.push_back((TH1D*)h1_nbJets_Nm1_baseline->Clone());
  TH1D * h1_nTops_Nm1_baseline = new TH1D(sampleKeyStringT+"_h1_nTops_Nm1_baseline", sampleKeyStringT+": nTops after Nm1_baseline; nTops", 5, 0, 5); h1_nTops_Nm1_baseline->Sumw2(); h1_nTops_Nm1_baselineVec.push_back((TH1D*)h1_nTops_Nm1_baseline->Clone());
  TH1D * h1_met_Nm1_baseline = new TH1D(sampleKeyStringT+"_h1_met_Nm1_baseline", sampleKeyStringT+": met after Nm1_baseline; met [GeV]", 100, 0, 1000); h1_met_Nm1_baseline->Sumw2(); h1_met_Nm1_baselineVec.push_back((TH1D*)h1_met_Nm1_baseline->Clone());
  TH1D * h1_MT2_Nm1_baseline = new TH1D(sampleKeyStringT+"_h1_MT2_Nm1_baseline", sampleKeyStringT+": MT2 after Nm1_baseline; MT2 [GeV]", 100, 0, 1000); h1_MT2_Nm1_baseline->Sumw2(); h1_MT2_Nm1_baselineVec.push_back((TH1D*)h1_MT2_Nm1_baseline->Clone());

  TH1D * h1_mht_baseline = new TH1D(sampleKeyStringT+"_h1_mht_baseline", sampleKeyStringT+": mht after baseline; mht [GeV]", 100, 0, 1000); h1_mht_baseline->Sumw2(); h1_mht_baselineVec.push_back((TH1D*)h1_mht_baseline->Clone());
  TH1D * h1_mhtphi_baseline = new TH1D(sampleKeyStringT+"_h1_mhtphi_baseline", sampleKeyStringT+": mhtphi after baseline; mhtphi", 100, -3.2, 3.2); h1_mhtphi_baseline->Sumw2(); h1_mhtphi_baselineVec.push_back((TH1D*)h1_mhtphi_baseline->Clone());

  TH1D * h1_dphi1_baseline = new TH1D(sampleKeyStringT+"_h1_dphi1_baseline", sampleKeyStringT+": dphi1 after baseline; dphi1", 100, 0.0, 3.2); h1_dphi1_baseline->Sumw2(); h1_dphi1_baselineVec.push_back((TH1D*)h1_dphi1_baseline->Clone());
  TH1D * h1_dphi2_baseline = new TH1D(sampleKeyStringT+"_h1_dphi2_baseline", sampleKeyStringT+": dphi2 after baseline; dphi2", 100, 0.0, 3.2); h1_dphi2_baseline->Sumw2(); h1_dphi2_baselineVec.push_back((TH1D*)h1_dphi2_baseline->Clone());
  TH1D * h1_dphi3_baseline = new TH1D(sampleKeyStringT+"_h1_dphi3_baseline", sampleKeyStringT+": dphi3 after baseline; dphi3", 100, 0.0, 3.2); h1_dphi3_baseline->Sumw2(); h1_dphi3_baselineVec.push_back((TH1D*)h1_dphi3_baseline->Clone());

  TH1D * h1_topMass_baseline = new TH1D(sampleKeyStringT+"_h1_topMass_baseline", sampleKeyStringT+": top mass after baseline; topM [GeV]", 100, 0, 300); h1_topMass_baseline->Sumw2(); h1_topMass_baselineVec.push_back((TH1D*)h1_topMass_baseline->Clone()); 

  TH1D * h1_vtxSize_baseline = new TH1D(sampleKeyStringT+"_h1_vtxSize_baseline", sampleKeyStringT+": vtxSize after baseline; vtxSize", 50, 0, 50); h1_vtxSize_baseline->Sumw2(); h1_vtxSize_baselineVec.push_back((TH1D*)h1_vtxSize_baseline->Clone());

  TH1D * h1_allJetPt_baseline = new TH1D(sampleKeyStringT+"_h1_allJetPt_baseline", sampleKeyStringT+": allJetPt after baseline; allJetPt [GeV]", 100, 0, 1000); h1_allJetPt_baseline->Sumw2(); h1_allJetPt_baselineVec.push_back((TH1D*)h1_allJetPt_baseline->Clone());
  TH1D * h1_allJetEta_baseline = new TH1D(sampleKeyStringT+"_h1_allJetEta_baseline", sampleKeyStringT+": allJetEta after baseline; allJetEta", 100, -5, 5); h1_allJetEta_baseline->Sumw2(); h1_allJetEta_baselineVec.push_back((TH1D*)h1_allJetEta_baseline->Clone());
  TH1D * h1_allJetPhi_baseline = new TH1D(sampleKeyStringT+"_h1_allJetPhi_baseline", sampleKeyStringT+": allJetPhi after baseline; allJetPhi", 100, -3.2, 3.2); h1_allJetPhi_baseline->Sumw2(); h1_allJetPhi_baselineVec.push_back((TH1D*)h1_allJetPhi_baseline->Clone());
  TH1D * h1_allJetM_baseline = new TH1D(sampleKeyStringT+"_h1_allJetM_baseline", sampleKeyStringT+": allJetM after baseline; allJetM [GeV]", 100, 0, 250); h1_allJetM_baseline->Sumw2(); h1_allJetM_baselineVec.push_back((TH1D*)h1_allJetM_baseline->Clone());

  TH1D * h1_leadJetPt_baseline = new TH1D(sampleKeyStringT+"_h1_leadJetPt_baseline", sampleKeyStringT+": leadJetPt after baseline; leadJetPt [GeV]", 100, 0, 1000); h1_leadJetPt_baseline->Sumw2(); h1_leadJetPt_baselineVec.push_back((TH1D*)h1_leadJetPt_baseline->Clone());
  TH1D * h1_leadJetEta_baseline = new TH1D(sampleKeyStringT+"_h1_leadJetEta_baseline", sampleKeyStringT+": leadJetEta after baseline; leadJetEta", 100, -5, 5); h1_leadJetEta_baseline->Sumw2(); h1_leadJetEta_baselineVec.push_back((TH1D*)h1_leadJetEta_baseline->Clone());
  TH1D * h1_leadJetPhi_baseline = new TH1D(sampleKeyStringT+"_h1_leadJetPhi_baseline", sampleKeyStringT+": leadJetPhi after baseline; leadJetPhi", 100, -3.2, 3.2); h1_leadJetPhi_baseline->Sumw2(); h1_leadJetPhi_baselineVec.push_back((TH1D*)h1_leadJetPhi_baseline->Clone());
  TH1D * h1_leadJetM_baseline = new TH1D(sampleKeyStringT+"_h1_leadJetM_baseline", sampleKeyStringT+": leadJetM after baseline; leadJetM [GeV]", 100, 0, 250); h1_leadJetM_baseline->Sumw2(); h1_leadJetM_baselineVec.push_back((TH1D*)h1_leadJetM_baseline->Clone());

  TH1D * h1_muPt_baseline = new TH1D(sampleKeyStringT+"_h1_muPt_baseline", sampleKeyStringT+": muPt after baseline; muPt [GeV]", 100, 0, 500); h1_muPt_baseline->Sumw2(); h1_muPt_baselineVec.push_back((TH1D*)h1_muPt_baseline->Clone());
  TH1D * h1_muEta_baseline = new TH1D(sampleKeyStringT+"_h1_muEta_baseline", sampleKeyStringT+": muEta after baseline; muEta", 100, -5, 5); h1_muEta_baseline->Sumw2(); h1_muEta_baselineVec.push_back((TH1D*)h1_muEta_baseline->Clone());
  TH1D * h1_muPhi_baseline = new TH1D(sampleKeyStringT+"_h1_muPhi_baseline", sampleKeyStringT+": muPhi after baseline; muPhi", 100, -3.2, 3.2); h1_muPhi_baseline->Sumw2(); h1_muPhi_baselineVec.push_back((TH1D*)h1_muPhi_baseline->Clone());

  TH1D * h1_elePt_baseline = new TH1D(sampleKeyStringT+"_h1_elePt_baseline", sampleKeyStringT+": elePt after baseline; elePt [GeV]", 100, 0, 500); h1_elePt_baseline->Sumw2(); h1_elePt_baselineVec.push_back((TH1D*)h1_elePt_baseline->Clone());
  TH1D * h1_eleEta_baseline = new TH1D(sampleKeyStringT+"_h1_eleEta_baseline", sampleKeyStringT+": eleEta after baseline; eleEta", 100, -5, 5); h1_eleEta_baseline->Sumw2(); h1_eleEta_baselineVec.push_back((TH1D*)h1_eleEta_baseline->Clone());
  TH1D * h1_elePhi_baseline = new TH1D(sampleKeyStringT+"_h1_elePhi_baseline", sampleKeyStringT+": elePhi after baseline; elePhi", 100, -3.2, 3.2); h1_elePhi_baseline->Sumw2(); h1_elePhi_baselineVec.push_back((TH1D*)h1_elePhi_baseline->Clone());

  TH1D * h1_mtw_baseline = new TH1D(sampleKeyStringT+"_h1_mtw_baseline", sampleKeyStringT+": mtw after baseline; mtw [GeV]", 500, 0, 500); h1_mtw_baseline->Sumw2(); h1_mtw_baselineVec.push_back((TH1D*)h1_mtw_baseline->Clone());

  TH2D * h2_MT2_vs_met_baseline = new TH2D(sampleKeyStringT+"_h2_MT2_vs_met_baseline", sampleKeyStringT+": MT2 versus met after baseline; met [GeV]; MT2 [GeV]", 100, 200, 700, 100, 200, 700); h2_MT2_vs_met_baseline->Sumw2(); h2_MT2_vs_met_baselineVec.push_back((TH2D*)h2_MT2_vs_met_baseline->Clone());

  TH2D * h2_MT2_vs_met_loose = new TH2D(sampleKeyStringT+"_h2_MT2_vs_met_loose", sampleKeyStringT+": MT2 versus met after loose; met [GeV]; MT2 [GeV]", 100,  0, 1000, 100,  0, 1000); h2_MT2_vs_met_loose->Sumw2(); h2_MT2_vs_met_looseVec.push_back((TH2D*)h2_MT2_vs_met_loose->Clone());

  TH2D * h2_alt_MT2_vs_met_loose = new TH2D(sampleKeyStringT+"_h2_alt_MT2_vs_met_loose", sampleKeyStringT+": alt_MT2 versus met after loose; met [GeV]; alt MT2 [GeV]", 100,  0, 1000, 100,  0, 1000); h2_alt_MT2_vs_met_loose->Sumw2(); h2_alt_MT2_vs_met_looseVec.push_back((TH2D*)h2_alt_MT2_vs_met_loose->Clone());

  TH2D * h2_MT2_vs_alt_MT2_loose = new TH2D(sampleKeyStringT+"_h2_MT2_vs_alt_MT2_loose", sampleKeyStringT+": MT2 versus alternative MT2 after loose; alt MT2 [GeV]; MT2 [GeV]", 100,   0, 1000, 100,   0, 1000); h2_MT2_vs_alt_MT2_loose->Sumw2(); h2_MT2_vs_alt_MT2_looseVec.push_back((TH2D*)h2_MT2_vs_alt_MT2_loose->Clone());

  TH2D * h2_evtCnt_nbJets_vs_nTops = new TH2D(sampleKeyStringT+"_h2_evtCnt_nbJets_vs_nTops", sampleKeyStringT+": event counts nbJets versus nTops; nTops; nbJets", 4, 0, 4, 3, 1, 4); h2_evtCnt_nbJets_vs_nTops->Sumw2(); h2_evtCnt_nbJets_vs_nTopsVec.push_back((TH2D*) h2_evtCnt_nbJets_vs_nTops->Clone());

  TH2D * h2_nbJets_vs_nJets = new TH2D(sampleKeyStringT+"_h2_nbJets_vs_nJets", sampleKeyStringT+": nbJets versus nJets; nJets; nbJets", 10, 4, 14, 4, 1, 5); h2_nbJets_vs_nJets->Sumw2(); h2_nbJets_vs_nJetsVec.push_back((TH2D*) h2_nbJets_vs_nJets->Clone());
  TH2D * h2_nTops_vs_nJets = new TH2D(sampleKeyStringT+"_h2_nTops_vs_nJets", sampleKeyStringT+": nTops versus nJets; nJets; nTops", 10, 4, 14, 5, 0, 5); h2_nTops_vs_nJets->Sumw2(); h2_nTops_vs_nJetsVec.push_back((TH2D*) h2_nTops_vs_nJets->Clone());
  TH2D * h2_nTops_vs_nbJets = new TH2D(sampleKeyStringT+"_h2_nTops_vs_nbJets", sampleKeyStringT+": nTops versus nbJets; nbJets; nTops", 4, 1, 5, 5, 0, 5); h2_nTops_vs_nbJets->Sumw2(); h2_nTops_vs_nbJetsVec.push_back((TH2D*) h2_nTops_vs_nbJets->Clone());

  TH2D * h2_nbJets_vs_nJets_baseline = new TH2D(sampleKeyStringT+"_h2_nbJets_vs_nJets_baseline", sampleKeyStringT+": nbJets versus nJets after baseline; nJets; nbJets", 10, 4, 14, 4, 1, 5); h2_nbJets_vs_nJets_baseline->Sumw2(); h2_nbJets_vs_nJets_baselineVec.push_back((TH2D*) h2_nbJets_vs_nJets_baseline->Clone());
  TH2D * h2_nTops_vs_nJets_baseline = new TH2D(sampleKeyStringT+"_h2_nTops_vs_nJets_baseline", sampleKeyStringT+": nTops versus nJets after baseline; nJets; nTops", 10, 4, 14, 5, 0, 5); h2_nTops_vs_nJets_baseline->Sumw2(); h2_nTops_vs_nJets_baselineVec.push_back((TH2D*) h2_nTops_vs_nJets_baseline->Clone());
  TH2D * h2_nTops_vs_nbJets_baseline = new TH2D(sampleKeyStringT+"_h2_nTops_vs_nbJets_baseline", sampleKeyStringT+": nTops versus nbJets after baseline; nbJets; nTops", 4, 1, 5, 5, 0, 5); h2_nTops_vs_nbJets_baseline->Sumw2(); h2_nTops_vs_nbJets_baselineVec.push_back((TH2D*) h2_nTops_vs_nbJets_baseline->Clone());

  bool isDeclared = false;
  if( std::find(declaredSampleStrVec.begin(), declaredSampleStrVec.end(), sampleKeyStringT.Data()) != declaredSampleStrVec.end() ) isDeclared = true;
  else{ declaredSampleStrVec.push_back(sampleKeyString); }

  for(unsigned int iSR=0; iSR<nSR && !isDeclared; iSR++){
     std::vector<TH2Poly*> h2_PS_poly_MT2_vs_metVec;
     sprintf(names, "%s_h2_poly_MT2_vs_met_nbJets%s_nTops%s", sampleKeyStringT.Data(), keyStr_nbJets_SR[iSR].c_str(), keyStr_nTops_SR[iSR].c_str());
     sprintf(dispt, "%s: MT2 versus met for nbJets%s and nTops%s; met [GeV]; MT2 [GeV]", sampleKeyStringT.Data(), disStr_nbJets_SR[iSR].c_str(), disStr_nTops_SR[iSR].c_str());
     TH2Poly * h2_poly_MT2_vs_met = new TH2Poly(names, dispt, 200, pseudoMax_met_for_hist, 200, pseudoMax_MT2_for_hist);
     h2_poly_MT2_vs_met->SetName(names); h2_poly_MT2_vs_met->SetTitle(dispt);
     for(unsigned int ib=0; ib<out_MT2_met_Binning_forTH2Poly[iSR].size(); ib++){ h2_poly_MT2_vs_met->AddBin(out_MT2_met_Binning_forTH2Poly[iSR][ib][0], out_MT2_met_Binning_forTH2Poly[iSR][ib][1], out_MT2_met_Binning_forTH2Poly[iSR][ib][2], out_MT2_met_Binning_forTH2Poly[iSR][ib][3]); }
     h2_poly_MT2_vs_met->Sumw2();
     if( h2_poly_MT2_vs_metVec.size() < iSR+1 ) { h2_PS_poly_MT2_vs_metVec.push_back((TH2Poly*)h2_poly_MT2_vs_met->Clone()); h2_poly_MT2_vs_metVec.push_back(h2_PS_poly_MT2_vs_metVec); }
     else{ h2_poly_MT2_vs_metVec[iSR].push_back((TH2Poly*)h2_poly_MT2_vs_met->Clone());}
  }

}

void declHistGlobal(){

   h2_evtCnt_sumSM_nbJets_vs_nTops = new TH2D("h2_evtCnt_sumSM_nbJets_vs_nTops", "SumSM: event counts nbJets versus nTops; nTops; nbJets", 4, 0, 4, 3, 1, 4); h2_evtCnt_sumSM_nbJets_vs_nTops->Sumw2();
   h1_keyString = new TH1D("h1_keyString", "key string", 5, 0, 5); h1_keyString->SetBit(TH1::kCanRebin);

}

#endif
