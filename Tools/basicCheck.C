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

BaselineVessel * SRblv =0;
const std::string spec = "MY";

void mypassBaselineFunc(NTupleReader &tr){
   (*SRblv)(tr);
}

AnaSamples::SampleSet        allSamples;
AnaSamples::SampleCollection allCollections(allSamples);

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

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

void anaFunc(NTupleReader *tr, std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar", int verbose=0){
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
     if( isData ) tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames_DataOnly);
     else tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames);
     tr->registerFunction(&mypassBaselineFunc);

     int entries = tr->getNEntries();
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     while(tr->getNextEvent()){

        if( entryToProcess >=0 && tr->getEvtNum() > entryToProcess ) break;

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (tr->getEvtNum()-1)%(entries/10) == 0 ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;

        // Internal evtWeight in the sample: default is 1.0 execept for MC samples with intrinsic weight, e.g., QCD flat sample.
        double iniWeight = tr->getVar<double>("evtWeight");
        double puWeight = 1.0; // currently set to be 1.0

        double stored_weight = keyStringT.Contains("Data") ? 1 : tr->getVar<double>("stored_weight");
        int sign_of_stored_weight = (stored_weight > 0) ? 1 : ((stored_weight < 0) ? -1 : 0);
        if( sign_of_stored_weight == 1 ) h1_cutFlow_auxVec.back()->Fill("posSign", 1);
        if( sign_of_stored_weight == -1 ) h1_cutFlow_auxVec.back()->Fill("negSign", 1);
        if( sign_of_stored_weight == 0 ) h1_cutFlow_auxVec.back()->Fill("zero", 1);
        
        double evtWeight = iniWeight * puWeight * sign_of_stored_weight;

// Get branches out directly from what stored in the tree
        const double met = tr->getVar<double>("met");
        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
        const std::vector<std::string> & TriggerNames = tr->getVec<std::string>("TriggerNames");
        const std::vector<int> & PassTrigger = tr->getVec<int>("PassTrigger");

        const std::vector<TLorentzVector> & muonsLVec = tr->getVec<TLorentzVector>("muonsLVec");
        const std::vector<double> & muonsRelIso = tr->getVec<double>("muonsRelIso");
        const std::vector<double> & muonsMiniIso = tr->getVec<double>("muonsMiniIso");
        const std::vector<double> & muonsMtw = tr->getVec<double>("muonsMtw");

        const std::vector<TLorentzVector> & elesLVec = tr->getVec<TLorentzVector>("elesLVec");
        const std::vector<double> & elesRelIso = tr->getVec<double>("elesRelIso");
        const std::vector<double> & elesMiniIso = tr->getVec<double>("elesMiniIso");
        const std::vector<double> & elesMtw = tr->getVec<double>("elesMtw");

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

// Can directly use passBaseline to get to baseline distribution, but can also configure this
        h1_cutFlowVec.back()->Fill("original", evtWeight * scaleMC);

        if( !passNoiseEventFilter ) continue; h1_cutFlowVec.back()->Fill("passNoiseEventFilter", evtWeight * scaleMC);

        if( !passnJets ) continue; h1_cutFlowVec.back()->Fill("passnJets", evtWeight * scaleMC);

        if( !passMuonVeto ) continue; h1_cutFlowVec.back()->Fill("passMuonVeto", evtWeight * scaleMC);
        if( !passEleVeto ) continue; h1_cutFlowVec.back()->Fill("passEleVeto", evtWeight * scaleMC);
        if( !passIsoTrkVeto ) continue; h1_cutFlowVec.back()->Fill("passIsoTrkVeto", evtWeight * scaleMC);

        if( !passdPhis ) continue; h1_cutFlowVec.back()->Fill("passdPhis", evtWeight * scaleMC);

        if( !passBJets ) continue; h1_cutFlowVec.back()->Fill("passBJets", evtWeight * scaleMC);

        if( !passMET ) continue; h1_cutFlowVec.back()->Fill("passMET", evtWeight * scaleMC);

        if( !passTagger ) continue; h1_cutFlowVec.back()->Fill("passTagger", evtWeight * scaleMC);
         
        if( !(nTops>=1) ) continue; h1_cutFlowVec.back()->Fill("passnTopsLE1", evtWeight * scaleMC);

// Fill histograms with looser requirement -> trigger req. for data...
        if( passHT ){
           h1_nJets_looseVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_looseVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_looseVec.back()->Fill(nbJets, evtWeight * scaleMC);
   
           h1_met_looseVec.back()->Fill(met, evtWeight * scaleMC);
           h1_metphi_looseVec.back()->Fill(metLVec.Phi(), evtWeight * scaleMC);
   
           h1_MT2_looseVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_HT_looseVec.back()->Fill(HT, evtWeight * scaleMC);
   
           h1_dphi1_looseVec.back()->Fill(dPhiVec[1], evtWeight * scaleMC);
           h1_dphi2_looseVec.back()->Fill(dPhiVec[2], evtWeight * scaleMC);
           h1_dphi3_looseVec.back()->Fill(dPhiVec[3], evtWeight * scaleMC);
   
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
   
           for(unsigned int im=0; im<muonsLVec.size(); im++){
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

        if( !passMT2 ) continue; h1_cutFlowVec.back()->Fill("passMT2", evtWeight * scaleMC);

        if( !passHT ) continue; h1_cutFlowVec.back()->Fill("passHT", evtWeight * scaleMC);

// No need, but store this for a cross-check
        h1_cutFlowVec.back()->Fill("passBaseline", passBaseline * evtWeight * scaleMC);

        h1_nJets_baselineVec.back()->Fill(nJets, evtWeight * scaleMC);
        h1_nTops_baselineVec.back()->Fill(nTops, evtWeight * scaleMC);
        h1_nbJets_baselineVec.back()->Fill(nbJets, evtWeight * scaleMC);

        h1_met_baselineVec.back()->Fill(met, evtWeight * scaleMC);
        h1_metphi_baselineVec.back()->Fill(metLVec.Phi(), evtWeight * scaleMC);

        h1_MT2_baselineVec.back()->Fill(MT2, evtWeight * scaleMC);
        h1_HT_baselineVec.back()->Fill(HT, evtWeight * scaleMC);

        h1_dphi1_baselineVec.back()->Fill(dPhiVec[1], evtWeight * scaleMC);
        h1_dphi2_baselineVec.back()->Fill(dPhiVec[2], evtWeight * scaleMC);
        h1_dphi3_baselineVec.back()->Fill(dPhiVec[3], evtWeight * scaleMC);

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

        for(unsigned int im=0; im<muonsLVec.size(); im++){
           h1_muPt_baselineVec.back()->Fill(muonsLVec.at(im).Pt(), evtWeight * scaleMC);
           h1_muEta_baselineVec.back()->Fill(muonsLVec.at(im).Eta(), evtWeight * scaleMC);
           h1_muPhi_baselineVec.back()->Fill(muonsLVec.at(im).Phi(), evtWeight * scaleMC);
        }

        for(unsigned int ie=0; ie<elesLVec.size(); ie++){
           h1_elePt_baselineVec.back()->Fill(elesLVec.at(ie).Pt(), evtWeight * scaleMC);
           h1_eleEta_baselineVec.back()->Fill(elesLVec.at(ie).Eta(), evtWeight * scaleMC);
           h1_elePhi_baselineVec.back()->Fill(elesLVec.at(ie).Phi(), evtWeight * scaleMC);
        }

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

void basicCheck(int argc, char *argv[]){

   AnaFunctions::prepareForNtupleReader();
   AnaFunctions::prepareTopTagger();

   type3Ptr->setdebug(true);

   build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   print_searchBins();

   declHistGlobal();

   NTupleReader *tr = 0;

   SRblv = new BaselineVessel(spec);

   std::string selKeyStr;
   if( argc >=2 ){
      selKeyStr = argv[1];
      std::cout<<"selKeyStr : "<<selKeyStr<<std::endl;
   }
   if( argc >=3 ){
      entryToProcess = atoi(argv[2]);
      std::cout<<"entryToProcess : "<<entryToProcess<<std::endl;
   }

   std::stringstream ssSelKey(selKeyStr);

   std::string buf;
   std::vector<std::string> keyStrVec;
   std::string nameStr;

   while(ssSelKey >> buf){ keyStrVec.push_back(buf); nameStr += "_"+buf; }

   std::vector<TTree*> treeVec;
   std::vector<std::string> subSampleKeysVec;
 
   for(const auto & filelist : allCollections){

      if( !keyStrVec.empty() ){
         bool found = false;
         for(auto& str : keyStrVec ){ if( filelist.first == str ) found = true; }
         if( !found ) continue;
      }

      std::cout<<"\nProcessing "<<filelist.first.c_str()<<" ..."<<std::endl;   
      treeVec.clear(); subSampleKeysVec.clear();
      for(auto & file : filelist.second){ 
         std::string perSubStr;
         for(const auto & perST : allSamples ){ if(perST.second == file ) perSubStr = perST.first; }

// Throw away low HT bin samples since we have HT>500 GeV cut...
         if(filelist.first == "WJetsToLNu" && (perSubStr == "WJetsToLNu_HT_100to200" || perSubStr == "WJetsToLNu_HT_200to400") ) continue;
         if(filelist.first == "ZJetsToNuNu" && (perSubStr == "ZJetsToNuNu_HT_100to200" || perSubStr == "ZJetsToNuNu_HT_200to400") ) continue;
         if(filelist.first == "QCD" && (perSubStr == "QCD_HT100to200" || perSubStr == "QCD_HT200to300" || perSubStr == "QCD_HT300to500") ) continue;

         TChain *aux = new TChain(file.treePath.c_str());  
         for(const auto& fn : file.getFilelist())
         {
             aux->Add(fn.c_str());
         }
         treeVec.push_back(aux);

         subSampleKeysVec.push_back(perSubStr);
      }
      anaFunc(tr, treeVec, subSampleKeysVec, filelist.first.c_str());

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

   TCanvas *cs = new TCanvas("cs", "cs", 1200, 800);
   int divW=3, divH=2;
   cs->Divide(divW, divH);

   TString pdfNameStrT = "basicCheck_allINone"+nameStr+".pdf";

   cs->Print(pdfNameStrT+"[");
   for(unsigned int ic=0; ic<h1_cutFlowVec.size(); ic++){ h1_cutFlowVec[ic]->LabelsDeflate(); h1_cutFlowVec[ic]->LabelsOption("v"); h1_cutFlowVec[ic]->SetMarkerSize(h1_cutFlowVec[ic]->GetMarkerSize()*1.5); }
   for(unsigned int ic=0; ic<h1_cutFlow_auxVec.size(); ic++){ h1_cutFlow_auxVec[ic]->LabelsDeflate(); h1_cutFlow_auxVec[ic]->LabelsOption("v"); h1_cutFlow_auxVec[ic]->SetMarkerSize(h1_cutFlow_auxVec[ic]->GetMarkerSize()*1.5); }
   draw1DallINone(cs, divW*divH, h1_cutFlowVec, 1, "text e"); cs->Print(pdfNameStrT);
   draw1DallINone(cs, divW*divH, h1_cutFlow_auxVec, 1, "text e"); cs->Print(pdfNameStrT);

   for(unsigned int ic=0; ic<h2_evtCnt_nbJets_vs_nTopsVec.size(); ic++){ h2_evtCnt_nbJets_vs_nTopsVec[ic]->SetMarkerSize(h2_evtCnt_nbJets_vs_nTopsVec[ic]->GetMarkerSize()*2.0); }
   draw2DallINone(cs, divW*divH, h2_evtCnt_nbJets_vs_nTopsVec, "colz text e"); cs->Print(pdfNameStrT);

   for(int iSR=0; iSR<nSR; iSR++){
      for(unsigned int ic=0; ic<h2_poly_MT2_vs_metVec[iSR].size(); ic++){ h2_poly_MT2_vs_metVec[iSR][ic]->SetMarkerSize(h2_poly_MT2_vs_metVec[iSR][ic]->GetMarkerSize()*2.0); h2_poly_MT2_vs_metVec[iSR][ic]->GetZaxis()->SetRangeUser(0, h2_poly_MT2_vs_metVec[iSR][ic]->GetMaximum()); }
      draw2DallINone(cs, divW*divH, h2_poly_MT2_vs_metVec[iSR], "colz text e"); cs->Print(pdfNameStrT);
   }
   cs->Print(pdfNameStrT+"]");

   TString rootNameStrT = "basicCheck"+nameStr+".root";

   TFile *basicCheckFile = new TFile(rootNameStrT, "RECREATE");

   h2_evtCnt_sumSM_nbJets_vs_nTops->Write();
   h1_keyString->Write();
   for(unsigned int ih=0; ih<h1_cutFlowVec.size(); ih++){
      h1_cutFlowVec[ih]->Write();
      h2_evtCnt_nbJets_vs_nTopsVec[ih]->Write();

      for(int iSR = 0; iSR < nSR; iSR++){
         h2_poly_MT2_vs_metVec[iSR][ih]->Write();
      }

      h1_nJets_looseVec[ih]->Write(); h1_nTops_looseVec[ih]->Write(); h1_nbJets_looseVec[ih]->Write();
      h1_met_looseVec[ih]->Write(); h1_MT2_looseVec[ih]->Write(); h1_HT_looseVec[ih]->Write(); h1_metphi_looseVec[ih]->Write();
      h1_dphi1_looseVec[ih]->Write(); h1_dphi2_looseVec[ih]->Write(); h1_dphi3_looseVec[ih]->Write();
      h1_vtxSize_looseVec[ih]->Write();
      h1_allJetPt_looseVec[ih]->Write(); h1_allJetEta_looseVec[ih]->Write(); h1_allJetPhi_looseVec[ih]->Write(); h1_allJetM_looseVec[ih]->Write();
      h1_leadJetPt_looseVec[ih]->Write(); h1_leadJetEta_looseVec[ih]->Write(); h1_leadJetPhi_looseVec[ih]->Write(); h1_leadJetM_looseVec[ih]->Write();
      h1_muPt_looseVec[ih]->Write(); h1_muEta_looseVec[ih]->Write(); h1_muPhi_looseVec[ih]->Write();
      h1_elePt_looseVec[ih]->Write(); h1_eleEta_looseVec[ih]->Write(); h1_elePhi_looseVec[ih]->Write();

      h1_nJets_baselineVec[ih]->Write(); h1_nTops_baselineVec[ih]->Write(); h1_nbJets_baselineVec[ih]->Write();
      h1_met_baselineVec[ih]->Write(); h1_MT2_baselineVec[ih]->Write(); h1_HT_baselineVec[ih]->Write(); h1_metphi_baselineVec[ih]->Write();
      h1_dphi1_baselineVec[ih]->Write(); h1_dphi2_baselineVec[ih]->Write(); h1_dphi3_baselineVec[ih]->Write();
      h1_vtxSize_baselineVec[ih]->Write();
      h1_allJetPt_baselineVec[ih]->Write(); h1_allJetEta_baselineVec[ih]->Write(); h1_allJetPhi_baselineVec[ih]->Write(); h1_allJetM_baselineVec[ih]->Write();
      h1_leadJetPt_baselineVec[ih]->Write(); h1_leadJetEta_baselineVec[ih]->Write(); h1_leadJetPhi_baselineVec[ih]->Write(); h1_leadJetM_baselineVec[ih]->Write();
      h1_muPt_baselineVec[ih]->Write(); h1_muEta_baselineVec[ih]->Write(); h1_muPhi_baselineVec[ih]->Write();
      h1_elePt_baselineVec[ih]->Write(); h1_eleEta_baselineVec[ih]->Write(); h1_elePhi_baselineVec[ih]->Write();
   }
   basicCheckFile->Write(); basicCheckFile->Close();

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
   basicCheck(argc, argv);
}

