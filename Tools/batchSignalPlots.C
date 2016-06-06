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

std::vector<std::string> todraw_h2_fastsim_keyStrVec = {"num_eff_b", "den_eff_b", "num_eff_c", "den_eff_c", "num_eff_udsg", "den_eff_udsg"};
std::vector<int>         todraw_h2_fastsim_rebinVec  = {          1,           1,           1,           1,              1,              1};
std::vector<std::string> todraw_h2_fastsim_xLabelVec = {"P_{T} (GeV)", "P_{T} (GeV)", "P_{T} (GeV)", "P_{T} (GeV)", "P_{T} (GeV)", "P_{T} (GeV)"};
std::vector<std::string> todraw_h2_fastsim_yLabelVec = {       "#eta",        "#eta",        "#eta",        "#eta",        "#eta",        "#eta"};
std::vector<std::vector<TH2D*> > sel_fastsim_h2Vec(todraw_h2_fastsim_keyStrVec.size());
std::vector<TH2D*> sel_fastsim_sum_h2Vec(todraw_h2_fastsim_keyStrVec.size());

std::vector<std::vector<TH1D*> > cached_h1Vec(todraw_h1_keyStrVec.size());
std::vector<std::string> cached_sampleStrVec;
std::vector<int> cached_sampleColorVec;

void batchSignalPlots(const std::string inputRootName){

   double dataLumi = 2153.74;

   TFile * sig_file = new TFile(inputRootName.c_str());
   sig_file->cd();

   TDirectory *current_sourcedir = gDirectory;
   TIter nextkey( current_sourcedir->GetListOfKeys() );
   TKey *key, *oldkey=0;
   while ( (key = (TKey*)nextkey())) {

      TString nameT = key->GetName();
      bool foundh2Key = false, foundh1Key = false;
      int pickedh2Idx = -1, pickedh1Idx = -1;
      for(unsigned int ik=0; ik<todraw_h2_fastsim_keyStrVec.size(); ik++){
         if( nameT.Contains(todraw_h2_fastsim_keyStrVec[ik].c_str()) ){ foundh2Key = true; pickedh2Idx = ik; }
      }
      if( !foundh2Key && !foundh1Key ) continue;
/*
      TObjArray * vlist =0;
      vlist = nameT.Tokenize("_");

      TObjString* obj_mStop = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-2));
      TString v_mStop=obj_mStop->GetString();
      int mStop = v_mStop.Atoi();

      TObjString* obj_mLSP = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-1));
      TString v_mLSP=obj_mLSP->GetString();
      int mLSP = v_mLSP.Atoi();
*/
      TObject *obj = key->ReadObj();
      TH1D * h1_tmp =0;
      TH2D * h2_tmp =0;
      if( foundh1Key) h1_tmp = (TH1D*) obj;
      if( foundh2Key) h2_tmp = (TH2D*) obj;

      if( foundh2Key && todraw_h2_fastsim_keyStrVec[pickedh2Idx].find("num_eff") != std::string::npos ){
         TString eff_nameT = nameT, alt_nameT = nameT;
         alt_nameT.ReplaceAll("num", "den");
         eff_nameT.ReplaceAll("num_", "");
         TH2D * h2_alt_tmp = (TH2D*) sig_file->Get(alt_nameT);
         TH2D * h2_eff_tmp = (TH2D*) h2_tmp->Clone();
//         h2_eff_tmp->Divide(h2_alt_tmp);
         h2_eff_tmp->Divide(h2_tmp, h2_alt_tmp, 1, 1, "B");
         h2_eff_tmp->SetName(eff_nameT);
         sel_fastsim_h2Vec[pickedh2Idx].push_back(h2_eff_tmp);
      }else if( foundh2Key ){
         sel_fastsim_h2Vec[pickedh2Idx].push_back(h2_tmp); 
      }
      if( foundh2Key ){
         if( sel_fastsim_sum_h2Vec[pickedh2Idx] ) sel_fastsim_sum_h2Vec[pickedh2Idx]->Add(h2_tmp);
         else sel_fastsim_sum_h2Vec[pickedh2Idx] = (TH2D*) h2_tmp->Clone();
      }
   }

   for(unsigned int ik=0; ik<sel_fastsim_sum_h2Vec.size(); ik++){
      if( todraw_h2_fastsim_keyStrVec[ik].find("num_eff") != std::string::npos ){
         sel_fastsim_sum_h2Vec[ik]->Divide(sel_fastsim_sum_h2Vec[ik], sel_fastsim_sum_h2Vec[ik+1], 1, 1, "B");
//         sel_fastsim_sum_h2Vec[ik]->Divide(sel_fastsim_sum_h2Vec[ik+1]);
         TString eff_nameT(todraw_h2_fastsim_keyStrVec[ik]);
         eff_nameT.ReplaceAll("num_", "");
         sel_fastsim_sum_h2Vec[ik]->SetName(eff_nameT);
      }
   }

   TFile * batch_file = new TFile("batchSignalPlots.root", "RECREATE");
   batch_file->cd();
   for(unsigned int ik=0; ik<sel_fastsim_h2Vec.size(); ik++){
      for(unsigned int is=0; is<sel_fastsim_h2Vec[ik].size(); is++){
         sel_fastsim_h2Vec[ik][is]->Write();
      }
      sel_fastsim_sum_h2Vec[ik]->Write();
   }
   batch_file->Close();
}

int main(int argc, char *argv[]){
   std::string inputRootName = "signalScan_SMS-T2tt.root";
   if( argc >= 2 ) inputRootName = argv[1];
   std::cout<<"\ninputRootName : "<<inputRootName.c_str()<<std::endl;
   batchSignalPlots(inputRootName);
}
