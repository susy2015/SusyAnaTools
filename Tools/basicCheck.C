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

#include "../SkimsAUX/plugins/MT2CalcCore.h"

SearchBins * sb =0;
int nTotBins;
std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

const bool isblind = false;
const bool doSingleMuonCS = false;
const bool doInvDphi = false;
const bool usegenmet = false;

const bool evtFilterFor_ttZ_rare = true;

MT2CalcCore * mt2Calc;

BaselineVessel * SRblv =0;
std::shared_ptr<topTagger::type3TopTagger>type3Ptr(nullptr);
std::string spec = "MY";

void mypassBaselineFunc(NTupleReader &tr){
   (*SRblv)(tr);
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

const AnaConsts::IsoAccRec hadtau_muonsMiniIsoArr = {   -1,       2.4,      20,     -1,       0.2,     100 };
const AnaConsts::IsoAccRec lostle_muonsMiniIsoArr = {   -1,       2.4,      10,     -1,       0.2,     100 };

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

std::vector<int> GetGenChilds(const std::vector<int> &genDecayPdgIdVec, const std::vector<int> &genDecayMomIdxVec, int parent, const std::vector<int> &pdgs){
  std::vector<int> outs;
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i) {
    if (abs(genDecayMomIdxVec[i]) == parent) {
      for(unsigned int j=0; j < pdgs.size(); ++j) {
        if (abs(genDecayPdgIdVec.at(i)) == pdgs.at(j)) {
          outs.push_back(i);
        }
      }
    }
  }
  return outs;
} // -----  end of function GetGenChilds  ----

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
//     if( keyStringT.Contains("fastsim") && keyStringT.Contains("Signal") ){ scaleMC = 1.0; }

     if( tr ) delete tr;
     if( isData ) tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames_DataOnly);
     else tr = new NTupleReader(treeVec[ist], AnaConsts::activatedBranchNames);
     tr->registerFunction(&mypassBaselineFunc);
     if(!isData) tr->registerFunction(&myPDFUncertaintyFunc);

     int entries = tr->getNEntries();
     if( entryToProcess >0 ) entries = entryToProcess;
     std::cout<<"\n\n"<<keyString.c_str()<<"_entries : "<<entries<<"  scaleMC : "<<scaleMC<<std::endl;

     bool cached_rethrow = tr->getReThrow();
     tr->setReThrow(false);

     while(tr->getNextEvent()){

        if( entryToProcess >0 && tr->getEvtNum() > entryToProcess ) break;

        if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (entries>=10 && (tr->getEvtNum()-1)%(entries/10) == 0) ) std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;

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

        if( isData && isblind && (!doSingleMuonCS || !doInvDphi) ){
           if( run > 274240 ) continue;
        }

        const double & genmet_tmp = tr->getVar<double>("genmet");
        const double & genmetphi_tmp = tr->getVar<double>("genmetphi");
        const double genmet = (&genmet_tmp) != nullptr ? tr->getVar<double>("genmet") : 0;
        const double genmetphi = (&genmetphi_tmp) != nullptr ? tr->getVar<double>("genmetphi") : -999;
        TLorentzVector genmetLVec;
        if( (&genmet_tmp) != nullptr ){
           genmetLVec.SetPtEtaPhiM(tr->getVar<double>("genmet"), 0, tr->getVar<double>("genmetphi"), 0);
        }

        const double met = ( usegenmet && (&genmet_tmp) != nullptr )? genmet : tr->getVar<double>("met");
        const double metphi = ( usegenmet && (&genmet_tmp) != nullptr )? genmetphi : tr->getVar<double>("metphi");
        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);
        const TLorentzVector mhtLVec = AnaFunctions::calcMHT(tr->getVec<TLorentzVector>("jetsLVec"), AnaConsts::pt30Arr);
//        metLVec.SetPtEtaPhiM(mhtLVec.Pt(), 0, mhtLVec.Phi(), 0); 
//        const double met = mhtLVec.Pt();

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
        const bool passNewTaggerReq = tr->getVar<bool>("passNewTaggerReq" + spec);
        const bool passNoiseEventFilter = tr->getVar<bool>("passNoiseEventFilter" + spec);
        const bool passFastsimEventFilter = tr->getVar<bool>("passFastsimEventFilter" + spec);

        const bool passBaseline = tr->getVar<bool>("passBaseline" + spec);
        const bool passBaselineNoTag = tr->getVar<bool>("passBaselineNoTag" + spec);

        std::vector<double> tighter_dPhiVec = AnaFunctions::calcDPhi(tr->getVec<TLorentzVector>("jetsLVec"), metLVec.Phi(), 4, AnaConsts::dphiArr);
        const bool pass_tighter_dPhis = passdPhis && tighter_dPhiVec.at(3) >= 0.3;

        int nbJetsCopy_TMP = nbJets, nTopsCopy_TMP = nTops, nJetsCopy_TMP = nJets;
        if( nbJetsCopy_TMP >=4 ) nbJetsCopy_TMP = 4; if( nTopsCopy_TMP >=4 ) nTopsCopy_TMP = 4; if( nJetsCopy_TMP >= 14 ) nJetsCopy_TMP = 14;

        const int searchBinIdx = sb->find_Binning_Index(nbJets, nTops, MT2, met);

        int cnt_eleTop =0, cnt_muTop =0, cnt_taumuTop =0, cnt_taueleTop =0, cnt_tauhadTop =0, cnt_allhadTop =0;
        int cnt_genTops =0;
        if( !isData ){
           const std::vector<int> & genDecayIdxVec = tr->getVec<int>("genDecayIdxVec");
           const std::vector<int> & genDecayMomIdxVec = tr->getVec<int>("genDecayMomIdxVec");
           const std::vector<int> & genDecayPdgIdVec = tr->getVec<int>("genDecayPdgIdVec");
           const std::vector<TLorentzVector> & genDecayLVec = tr->getVec<TLorentzVector>("genDecayLVec");
           const std::vector<std::string> & genDecayStrVec = tr->getVec<std::string>("genDecayStrVec");

           for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
              if( std::abs(genDecayPdgIdVec[ig]) != 6 ) continue;

              cnt_genTops ++;

              bool islepTop = false;
              int per_cnt_ele =0, per_cnt_mu =0, per_cnt_tau =0;
              for(unsigned int jg=0; jg<genDecayPdgIdVec.size(); jg++){
                 int pdgId = genDecayPdgIdVec.at(jg);
                 if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15 ){
                    if( find_mother(ig, jg, genDecayIdxVec, genDecayMomIdxVec) ){
                       islepTop = true;
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

           if( evtFilterFor_ttZ_rare && (keyStringT.Contains("TTZ") || keyStringT.Contains("TTW") || keyStringT.Contains("Diboson") || keyStringT.Contains("Triboson")) ){
// If there are any gen leptons of e, mu, tau, we assume they are covered by either lost lepton or hadronic tau.
// Things that are not covered are:
// TTZ: leptonic ttbar with whatever Z decays would be covered. Not covered is all-hadronic ttbar + Z->vv
// TTW: only all-hadronic of TTW are NOT covered (neglible)
// WW, WZ, ZZ: leptonic of W decay would be covered. If Z->ee,mumu,tautau, probably not covered but no real MET. If any of Z->vv with others decay hadronically or di-leptonic Z, then not covered.
// WWZ, WZZ, ZZZ: leptonic of W decay would be covered.
              int cntGenLep = 0, cntGenZinv = 0; 
              for (unsigned int ig = 0; ig < genDecayMomIdxVec.size(); ++ig){
                 if (abs(genDecayPdgIdVec[ig]) == 23){
          //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the leptons  ~~~~~
                    std::vector<int> out = GetGenChilds( genDecayPdgIdVec, genDecayMomIdxVec, genDecayIdxVec[ig], {12, 14, 16});
                    if (out.size() > 0){
                       assert(out.size() == 2);
                       cntGenZinv ++;
                    }
                 }
                 if (abs(genDecayPdgIdVec[ig]) == 24){
          //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the leptons  ~~~~~
                    std::vector<int> out = GetGenChilds( genDecayPdgIdVec, genDecayMomIdxVec, genDecayIdxVec[ig], {11, 13, 15});
                    if (out.size() > 0){
                       assert(out.size() == 1);
                       cntGenLep ++;
                    }
                 }
              }
              if( cntGenLep != 0 ) continue;
              if( cntGenZinv == 0 ) continue;
           }
        }

        bool is_gen_allHad = (cnt_eleTop ==0 && cnt_muTop ==0 && cnt_taumuTop ==0 && cnt_taueleTop ==0 && cnt_tauhadTop ==0 && cnt_allhadTop !=0);
        bool is_gen_lostLep = (cnt_eleTop !=0 || cnt_muTop !=0 || cnt_taumuTop !=0 || cnt_taueleTop !=0 );
        bool is_gen_hadTau = (!is_gen_lostLep && cnt_tauhadTop !=0 );

        bool is_gen_allHad_alt = (cnt_allhadTop !=0);

        h1_cutFlowVec.back()->Fill("all", evtWeight * scaleMC);

// Check trigger bit for data. Different PD can have different triggers.
        if( keyStringT.Contains("Data") ){
           bool foundTrigger = false;
           if( keyStringT.Contains("Placeholder") ) foundTrigger = true;
           for(unsigned it=0; it<TriggerNames.size(); it++){
              if( keyStringT.Contains("HTMHT") ){
/*
                 if(    TriggerNames[it].find("HLT_PFMET170_NoiseCleaned_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET170_JetIdCleaned_v") != std::string::npos 
                     || TriggerNames[it].find("HLT_PFMET100_PFMHT100_IDTight_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET110_PFMHT110_IDTight_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET120_PFMHT120_IDTight_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET130_PFMHT130_IDTight_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET140_PFMHT140_IDTight_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFMET150_PFMHT150_IDTight_v") != std::string::npos
                 ){
*/
/*                 if(    TriggerNames[it].find("HLT_PFHT350_PFMET100_JetIdCleaned_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFHT350_PFMET100_NoiseCleaned_v") != std::string::npos
                     || TriggerNames[it].find("HLT_PFHT350_PFMET100_v") != std::string::npos ){
*/
                 if( TriggerNames[it].find("HLT_PFHT300_PFMET100_v") != std::string::npos ){

                    if( PassTrigger[it] ) foundTrigger = true;
                 }
              }
              if( keyStringT.Contains("SingleMuon") ){
                 if( TriggerNames[it].find("HLT_Mu15_IsoVVVL_PFHT350_v") != std::string::npos ){
//                 if( TriggerNames[it].find("HLT_Mu15_IsoVVVL_PFHT350_v") ){
//                 if( TriggerNames[it].find("HLT_Mu45_eta2p1_v") ){
                    if( PassTrigger[it] ) foundTrigger = true;
                 }
              }
           }
           if( !foundTrigger ) continue;
        }

        int cntMuons_LostLepton = 0, cntMuons_HadTau = 0;
        std::vector<int> selMuonIdx_LostLeptonVec, selMuonIdx_HadTauVec;
        for(unsigned int im=0; im<muonsLVec.size(); im++){
//           if( AnaFunctions::passMuon(muonsLVec.at(im), muonsMiniIso.at(im), muonsMtw.at(im), muonsFlagMedium.at(im), AnaConsts::muonsMiniIsoArr) ){
           if( AnaFunctions::passMuon(muonsLVec.at(im), muonsMiniIso.at(im), muonsMtw.at(im), muonsFlagMedium.at(im), lostle_muonsMiniIsoArr) ){
              cntMuons_LostLepton ++;
              selMuonIdx_LostLeptonVec.push_back(im);
           }
           if( AnaFunctions::passMuon(muonsLVec.at(im), muonsMiniIso.at(im), muonsMtw.at(im), muonsFlagMedium.at(im), hadtau_muonsMiniIsoArr) ){
              cntMuons_HadTau ++;
              selMuonIdx_HadTauVec.push_back(im);
           }
        }

// Can directly use passBaseline to get to baseline distribution, but can also configure this
        h1_cutFlowVec.back()->Fill("original", evtWeight * scaleMC);
/*
        if( !tr->getVar<bool>("HBHENoiseFilter") ) continue; h1_cutFlowVec.back()->Fill("passHBHE", evtWeight * scaleMC);
        if( !tr->getVar<bool>("HBHEIsoNoiseFilter") ) continue; h1_cutFlowVec.back()->Fill("passHBHEIso", evtWeight * scaleMC);
        if( keyStringT.Contains("Data") ){
           if( ! (tr->getVar<int>("eeBadScFilter") && tr->getVar<int>("eeBadScListFilter") ) ) continue; h1_cutFlowVec.back()->Fill("passeeBadSc", evtWeight * scaleMC);
           if( ! tr->getVar<int>("CSCTightHaloListFilter") ) continue; h1_cutFlowVec.back()->Fill("passCSC", evtWeight * scaleMC);
        }else{
           h1_cutFlowVec.back()->Fill("passeeBadSc", evtWeight * scaleMC);
           h1_cutFlowVec.back()->Fill("passCSC", evtWeight * scaleMC);
        }
        if( !(tr->getVar<int>("vtxSize")>=1) ) continue; h1_cutFlowVec.back()->Fill("passVtx", evtWeight * scaleMC);
        if( !tr->getVar<int>("looseJetID_NoLep") ) continue; h1_cutFlowVec.back()->Fill("passJetId", evtWeight * scaleMC);
*/

        if( passNoiseEventFilter && passMuonVeto && passEleVeto && passIsoTrkVeto && passnJets && passBJets && passHT && passMET && passdPhis * passTagger * passMT2 ){
           h1_no_fastsimFilter_searchBinYieldsVec.back()->Fill(searchBinIdx, evtWeight * scaleMC);
        }

        if( !passNoiseEventFilter ) continue; h1_cutFlowVec.back()->Fill("passNoiseEventFilter", evtWeight * scaleMC);
        if( !passFastsimEventFilter ) continue; h1_cutFlowVec.back()->Fill("passFastsimEventFilter", evtWeight * scaleMC);

// The plots make sense only after fastsim bug fixes
        if( is_gen_allHad ){
           h1_met_loose_allHadVec.back()->Fill(met, evtWeight * scaleMC);
           h1_MT2_loose_allHadVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_nJets_loose_allHadVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_loose_allHadVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_loose_allHadVec.back()->Fill(nbJets, evtWeight * scaleMC);
           h1_genmet_loose_allHadVec.back()->Fill(genmet, evtWeight * scaleMC);
           h1_met_absres_loose_allHadVec.back()->Fill(met-genmet, evtWeight * scaleMC);
           if( genmet!=0 ) h1_met_relres_loose_allHadVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);
        }else{
           h1_met_loose_leptonicVec.back()->Fill(met, evtWeight * scaleMC);
           h1_MT2_loose_leptonicVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_nJets_loose_leptonicVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_loose_leptonicVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_loose_leptonicVec.back()->Fill(nbJets, evtWeight * scaleMC);
           h1_genmet_loose_leptonicVec.back()->Fill(genmet, evtWeight * scaleMC);
           h1_met_absres_loose_leptonicVec.back()->Fill(met-genmet, evtWeight * scaleMC);
           if( genmet!=0 ) h1_met_relres_loose_leptonicVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);
        }

        if( !doSingleMuonCS && !passMuonVeto ) continue; h1_cutFlowVec.back()->Fill("passMuonVeto", evtWeight * scaleMC);
        if( !passEleVeto ) continue; h1_cutFlowVec.back()->Fill("passEleVeto", evtWeight * scaleMC);
        if( !doSingleMuonCS && !passIsoTrkVeto ) continue; h1_cutFlowVec.back()->Fill("passIsoTrkVeto", evtWeight * scaleMC);

        if( !passnJets ) continue; h1_cutFlowVec.back()->Fill("passnJets", evtWeight * scaleMC);

// Fill histograms with looser requirement -> trigger req. for data...
//        const bool looseCond = doSingleMuonCS ? (passHT && nJets >=3 && nJetsPt50Eta24 >=1 && cntMuons_HadTau ==1) : (passHT && passMET && passdPhis && passBJets);
//        const bool looseCond = doSingleMuonCS ? (passHT && passMET && cntMuons_HadTau ==1) : (passHT && passMET && passdPhis && passBJets);
//        const bool looseCond = doSingleMuonCS ? (passHT && passMET && passdPhis && passBJets && cntMuons_LostLepton ==1) : (passHT && passMET && passdPhis && passBJets);
        const bool looseCond = doSingleMuonCS ? (passHT && passMET && passdPhis && passBJets && cntMuons_HadTau ==1) : (passHT && passMET && passdPhis && passBJets);
//        if( passHT && nJets >=3 && nJetsPt50Eta24 >=1 && cntMuons_HadTau ==1 ){
//        if( looseCond ){
        if( true ){
           h1_nJets_looseVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_looseVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_looseVec.back()->Fill(nbJets, evtWeight * scaleMC);
   
           h1_met_looseVec.back()->Fill(met, evtWeight * scaleMC);
           h1_metphi_looseVec.back()->Fill(metphi, evtWeight * scaleMC);

           h1_genmet_looseVec.back()->Fill(genmet, evtWeight * scaleMC);
           h1_genmetphi_looseVec.back()->Fill(genmetphi, evtWeight * scaleMC);

           h1_met_absres_looseVec.back()->Fill(met-genmet, evtWeight * scaleMC);
           if( genmet!=0 ) h1_met_relres_looseVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);

           h1_mht_looseVec.back()->Fill(mhtLVec.Pt(), evtWeight * scaleMC);
           h1_mhtphi_looseVec.back()->Fill(mhtLVec.Phi(), evtWeight * scaleMC);
   
           h1_MT2_looseVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_HT_looseVec.back()->Fill(HT, evtWeight * scaleMC);
/*
           for(unsigned int it=0; it<nTops; it++){
              TLorentzVector topLVec = type3Ptr->buildLVec(jetsLVec_forTagger, type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
              h1_topMass_looseVec.back()->Fill(topLVec.M(), evtWeight * scaleMC);
           }
*/
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
        if( doSingleMuonCS ){
           int is = selMuonIdx_LostLeptonVec[0];
           double mtw_muonCS = calcMT(muonsLVec.at(is), metLVec);
           if( mtw_muonCS < 100 ) continue;
        }

        if( passHT && passMET && passdPhis ){
           if( nTops>=1 && (nbJets==0 || (passNewTaggerReq && passMT2)) ) h1_nbJets_Nm1_baselineVec.back()->Fill(nbJets, evtWeight * scaleMC);
        }

        if( !passBJets ) continue; h1_cutFlowVec.back()->Fill("passBJets", evtWeight * scaleMC);

        if( !passHT ) continue; h1_cutFlowVec.back()->Fill("passHT", evtWeight * scaleMC);

        if( passdPhis && passTagger && passMT2 ) h1_met_Nm1_baselineVec.back()->Fill(met, evtWeight * scaleMC);

        if( !passMET ) continue; h1_cutFlowVec.back()->Fill("passMET", evtWeight * scaleMC);

        if( (!doInvDphi && !passdPhis) || (doInvDphi && passdPhis) ) continue; h1_cutFlowVec.back()->Fill("passdPhis", evtWeight * scaleMC);

        h2_nbJets_vs_nJetsVec.back()->Fill(nJetsCopy_TMP, nbJetsCopy_TMP, evtWeight * scaleMC);
        h2_nTops_vs_nJetsVec.back()->Fill(nJetsCopy_TMP, nTopsCopy_TMP, evtWeight * scaleMC);
        h2_nTops_vs_nbJetsVec.back()->Fill(nbJetsCopy_TMP, nTopsCopy_TMP, evtWeight * scaleMC);
/*
        h1_cutFlowVec.back()->Fill("Njet_4_6", (nJets>=4 && nJets<=6) * evtWeight * scaleMC);
        h1_cutFlowVec.back()->Fill("Njet_7_8", (nJets>=7 && nJets<=8) * evtWeight * scaleMC);
        h1_cutFlowVec.back()->Fill("Njet_9", (nJets>=9) * evtWeight * scaleMC);
*/
        if( passNewTaggerReq && (nTops ==0 || (nTops>=1 && passMT2)) ){
           h1_nTops_Nm1_baselineVec.back()->Fill(nTops, evtWeight * scaleMC);
        }

        if( !passTagger ) continue; h1_cutFlowVec.back()->Fill("passTagger", evtWeight * scaleMC);

        if( !(nTops>=1) ) continue; h1_cutFlowVec.back()->Fill("passnTopsLE1", evtWeight * scaleMC);

        const double alt_MT2 = mt2Calc->getMT2Hemi(jetsLVec_forTagger, metLVec);

        h2_MT2_vs_alt_MT2_looseVec.back()->Fill(alt_MT2, MT2, evtWeight * scaleMC);
        h2_MT2_vs_met_looseVec.back()->Fill(met, MT2, evtWeight * scaleMC);
        h2_alt_MT2_vs_met_looseVec.back()->Fill(met, alt_MT2, evtWeight * scaleMC);

        h1_MT2_Nm1_baselineVec.back()->Fill(MT2, evtWeight * scaleMC);

        if( !passMT2 ) continue; h1_cutFlowVec.back()->Fill("passMT2", evtWeight * scaleMC);

// No need, but store this for a cross-check
        h1_cutFlowVec.back()->Fill("passBaseline", passBaseline * evtWeight * scaleMC);

//        if( !(searchBinIdx ==0 || searchBinIdx ==5 || searchBinIdx == 11) ) continue;

        if( keyStringT.Contains("Placeholder") && nTops >=3 ) continue;

        if( keyStringT.Contains("Data_HTMHT") ){
//           if( nTops >=3 || searchBinIdx == 0 || searchBinIdx == 5 || searchBinIdx == 11 ){
           if( nTops>=2 && MT2<=350 && met<=350 ){
//           if( nTops >=3 || nTops ==1 ){
              int cntTopness = 0;
              int cnt_extra_bJets = 0;
              std::vector<int> cached_bJets_idxVec;
              std::cout<<"\n"<<keyStringT<<"  searchBinIdx : "<<searchBinIdx<<"  run : "<<run<<"  lumi : "<<lumi<<"  event : "<<event<<"  nJets : "<<nJets<<"  nTops : "<<nTops<<"  nbJets : "<<nbJets<<"  met : "<<met<<"  metphi : "<<metLVec.Phi()<<"  MT2 : "<<MT2<<"  HT : "<<HT<<"  mht : "<<mhtLVec.Pt()<<"  mhtphi : "<<mhtLVec.Phi()<<"  alt_MT2 : "<<alt_MT2<<std::endl;
              std::cout<<"  --> bJets idx : ";
              for(unsigned int ib=0; ib<recoJetsBtag_forTagger.size(); ib++){
                 if( !AnaFunctions::jetPassCuts(jetsLVec_forTagger[ib], AnaConsts::bTagArr) ) continue;
                 if( std::isnan(recoJetsBtag_forTagger[ib]) ) continue;
                 if( recoJetsBtag_forTagger[ib] <= AnaConsts::cutCSVS ) continue;
                 std::cout<<"  "<<ib;
                 cached_bJets_idxVec.push_back(ib);
                 cnt_extra_bJets ++;
              }
              std::cout<<std::endl;
              for(unsigned int it=0; it<nTops; it++){
                 int combIdx = type3Ptr->ori_pickedTopCandSortedVec[it];
                 std::cout<<"it : "<<it<<"  combIdx : "<<combIdx<<"  finalCombfatJets.size : "<<type3Ptr->finalCombfatJets[combIdx].size()<<"   -->  ";
                 for(unsigned int ic=0; ic<type3Ptr->finalCombfatJets[combIdx].size(); ic++){ std::cout<<"  "<<type3Ptr->finalCombfatJets[combIdx][ic]; }
                 std::cout<<std::endl;
                 TLorentzVector topLVec = type3Ptr->buildLVec(jetsLVec_forTagger, type3Ptr->finalCombfatJets[combIdx]);
                 std::cout<<"  --> topLVec  Pt : "<<topLVec.Pt()<<"  Eta : "<<topLVec.Eta()<<"  Phi : "<<topLVec.Phi()<<"  M : "<<topLVec.M()<<std::endl;
                 if( std::abs(topLVec.M() - 173.21) < 173.21*0.05 ){
                    cntTopness++;
                    for(unsigned int ib=0; ib< cached_bJets_idxVec.size(); ib++){
                       if( std::find(type3Ptr->finalCombfatJets[combIdx].begin(), type3Ptr->finalCombfatJets[combIdx].end(), cached_bJets_idxVec[ib]) != type3Ptr->finalCombfatJets[combIdx].end() ) cnt_extra_bJets --;
                    }
                 }
              }
              std::cout<<"==> for searchBinIdx : "<<searchBinIdx<<"  summarizing top/b ness : "<<cntTopness+cnt_extra_bJets<<"  cntTopness : "<<cntTopness<<"  cnt_extra_bJets : "<<cnt_extra_bJets<<std::endl;
              std::cout<<"cached_MT2Vec : ";
              for(unsigned int im=0; im<type3Ptr->cached_MT2Vec.size(); im++){ std::cout<<"  "<<type3Ptr->cached_MT2Vec[im]; }
              std::cout<<std::endl;

              std::cout<<"jets : "<<std::endl;
              for(unsigned int ij = 0; ij<jetsLVec_forTagger.size(); ij++){
                 std::cout<<"ij : "<<ij<<"  Pt : "<<jetsLVec_forTagger[ij].Pt()<<"  Eta : "<<jetsLVec_forTagger[ij].Eta()<<"  Phi : "<<jetsLVec_forTagger[ij].Phi()<<"  M : "<<jetsLVec_forTagger[ij].M()<<"  CSVS : "<<recoJetsBtag_forTagger[ij]<<std::endl;
              }
              std::cout<<std::endl;
           }
        }

        if( keyStringT.Contains("QCD") ){
           if( searchBinIdx == 7 || searchBinIdx == 27 ){
              std::cout<<"\n"<<keyStringT<<"  searchBinIdx : "<<searchBinIdx<<"  run : "<<run<<"  lumi : "<<lumi<<"  event : "<<event<<"  nJets : "<<nJets<<"  nTops : "<<nTops<<"  nbJets : "<<nbJets<<"  met : "<<met<<"  metphi : "<<metLVec.Phi()<<"  MT2 : "<<MT2<<"  HT : "<<HT<<"  mht : "<<mhtLVec.Pt()<<"  mhtphi : "<<mhtLVec.Phi()<<std::endl;
           }
        }

//        if( !pass_tighter_dPhis ) continue;  h1_cutFlowVec.back()->Fill("passTdPhis", evtWeight * scaleMC);

        h2_nbJets_vs_nJets_baselineVec.back()->Fill(nJetsCopy_TMP, nbJetsCopy_TMP, evtWeight * scaleMC);
        h2_nTops_vs_nJets_baselineVec.back()->Fill(nJetsCopy_TMP, nTopsCopy_TMP, evtWeight * scaleMC);
        h2_nTops_vs_nbJets_baselineVec.back()->Fill(nbJetsCopy_TMP, nTopsCopy_TMP, evtWeight * scaleMC);

        h1_nJets_baselineVec.back()->Fill(nJets, evtWeight * scaleMC);
        h1_nTops_baselineVec.back()->Fill(nTops, evtWeight * scaleMC);
        h1_nbJets_baselineVec.back()->Fill(nbJets, evtWeight * scaleMC);

        h1_met_baselineVec.back()->Fill(met, evtWeight * scaleMC);
        h1_metphi_baselineVec.back()->Fill(metphi, evtWeight * scaleMC);

        h1_genmet_baselineVec.back()->Fill(genmet, evtWeight * scaleMC);
        h1_genmetphi_baselineVec.back()->Fill(genmetphi, evtWeight * scaleMC);

        h1_met_absres_baselineVec.back()->Fill(met-genmet, evtWeight * scaleMC);
        if( genmet!=0 ) h1_met_relres_baselineVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);

        h1_mht_baselineVec.back()->Fill(mhtLVec.Pt(), evtWeight * scaleMC);
        h1_mhtphi_baselineVec.back()->Fill(mhtLVec.Phi(), evtWeight * scaleMC);
   
        h1_MT2_baselineVec.back()->Fill(MT2, evtWeight * scaleMC);
        h1_HT_baselineVec.back()->Fill(HT, evtWeight * scaleMC);

        if( is_gen_allHad ){
           h1_met_baseline_allHadVec.back()->Fill(met, evtWeight * scaleMC);
           h1_MT2_baseline_allHadVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_nJets_baseline_allHadVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_baseline_allHadVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_baseline_allHadVec.back()->Fill(nbJets, evtWeight * scaleMC);
           h1_genmet_baseline_allHadVec.back()->Fill(genmet, evtWeight * scaleMC);
           h1_met_absres_baseline_allHadVec.back()->Fill(met-genmet, evtWeight * scaleMC);
           if( genmet!=0 ) h1_met_relres_baseline_allHadVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);
        }else{
           h1_met_baseline_leptonicVec.back()->Fill(met, evtWeight * scaleMC);
           h1_MT2_baseline_leptonicVec.back()->Fill(MT2, evtWeight * scaleMC);
           h1_nJets_baseline_leptonicVec.back()->Fill(nJets, evtWeight * scaleMC);
           h1_nTops_baseline_leptonicVec.back()->Fill(nTops, evtWeight * scaleMC);
           h1_nbJets_baseline_leptonicVec.back()->Fill(nbJets, evtWeight * scaleMC);
           h1_genmet_baseline_leptonicVec.back()->Fill(genmet, evtWeight * scaleMC);
           h1_met_absres_baseline_leptonicVec.back()->Fill(met-genmet, evtWeight * scaleMC);
           if( genmet!=0 ) h1_met_relres_baseline_leptonicVec.back()->Fill((met-genmet)/genmet, evtWeight * scaleMC);
        }

        h2_MT2_vs_met_baselineVec.back()->Fill(met, MT2, evtWeight * scaleMC);

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

void basicCheck(int argc, char *argv[]){

   mt2Calc = new MT2CalcCore();

   AnaFunctions::prepareForNtupleReader();
   //AnaFunctions::prepareTopTagger();


   sb = new SearchBins();

   nTotBins = sb->nSearchBins();

   sb->build_MT2_met_Binning_forTH2Poly(out_MT2_met_Binning_forTH2Poly);

   sb->print_searchBins();
   sb->print_searchBins_latex();

   declHistGlobal();

   NTupleReader *tr = 0;

   pdfScale = new PDFUncertainty();

   int startfile = 0, filerun = -1;

   std::string selKeyStr;
   if( argc >=2 ){
      selKeyStr = argv[1];
      std::cout<<"selKeyStr : "<<selKeyStr<<std::endl;
   }

   if( usegenmet ) spec = "usegenmet";

   if( selKeyStr.find("fastsim") != std::string::npos ){
      SRblv = new BaselineVessel(*tr, spec, "fastsim");
   }else{
      SRblv = new BaselineVessel(*tr, spec);
   }

   type3Ptr = SRblv->GetType3Ptr();
   type3Ptr->setdebug(true);

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

   std::vector<int> toRunVec(keyStrVec.size(), 0);
 
   for(const auto & filelist : allCollections){

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

// Throw away low HT bin samples since we have HT>500 GeV cut...
/*
         if(filelist.first == "WJetsToLNu" && (perSubStr == "WJetsToLNu_HT_100to200" || perSubStr == "WJetsToLNu_HT_200to400") ) continue;
         if(filelist.first == "ZJetsToNuNu" && (perSubStr == "ZJetsToNuNu_HT_100to200" || perSubStr == "ZJetsToNuNu_HT_200to400") ) continue;
         if(filelist.first == "QCD" && (perSubStr == "QCD_HT100to200" || perSubStr == "QCD_HT200to300" || perSubStr == "QCD_HT300to500") ) continue;
*/
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
   tdrStyle->SetTitleBorderSize(0);
   tdrStyle->SetTitleAlign(13);
   tdrStyle->SetTitleX(0.08);
   tdrStyle->SetTitleH(0.07);
                                                                         
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

      TString pdfNameStrT = "basicCheck_allINone"+nameStr+".pdf";
   
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

      tdrStyle->SetPaintTextFormat("2.0f");

      TCanvas *c1 = new TCanvas("c1", "c1", 800, 800); 
      c1->SetBottomMargin(0.145); c1->SetLeftMargin(0.160); c1->SetTopMargin(0.110); c1->SetRightMargin(0.1);
      char names[200], dispt[200];
      for(int iSR=0; iSR<nSR; iSR++){
         sprintf(dispt, "                 N_{b}%s & N_{t}%s; #slash{E}_{T} (GeV); M_{T2} (GeV)", disStr_nbJets_SR[iSR].c_str(), disStr_nTops_SR[iSR].c_str());
         h2_poly_MT2_vs_metVec[iSR].back()->ClearBinContents();
         int ib = nbJets_SR_lo[iSR], it = nTops_SR_lo[iSR];
         std::vector<SearchBins::searchBinDef> selBinDefVec;
         for(int it=0; it<nTotBins; ++it){
            SearchBins::searchBinDef outBinDef;
            sb->find_BinBoundaries(it, outBinDef);
            if( outBinDef.idx_SR_ == iSR ) selBinDefVec.push_back(outBinDef);
         }
//         std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
         for(unsigned int is=0; is<selBinDefVec.size(); is++){
/*
            double MT2_lo = vMT2_vs_met_per_SR[0].at(is);
            double MT2_hi = vMT2_vs_met_per_SR[1].at(is);
            double met_lo = vMT2_vs_met_per_SR[2].at(is);
            double met_hi = vMT2_vs_met_per_SR[3].at(is);
*/
            double MT2_lo = selBinDefVec[is].MT2_lo_;
            double MT2_hi = selBinDefVec[is].MT2_hi_;
            double met_lo = selBinDefVec[is].met_lo_;
            double met_hi = selBinDefVec[is].met_hi_;

            double med_MT2 = MT2_hi ==-1? MT2_lo+5 : (MT2_lo+MT2_hi)/2.0;
            double med_met = met_hi ==-1? met_lo+5 : (met_lo+met_hi)/2.0;

            double idxBin = sb->find_Binning_Index(ib, it, MT2_lo, met_lo);
            if( idxBin ==0 ) idxBin = 0.1;
            int idxPoly = h2_poly_MT2_vs_metVec[iSR].back()->FindBin(med_met, med_MT2);

            h2_poly_MT2_vs_metVec[iSR].back()->SetBinContent(idxPoly, idxBin);
         }
//         TString titleT = h2_poly_MT2_vs_metVec[iSR].back()->GetTitle(); titleT.ReplaceAll("TTZ: ", "");
         h2_poly_MT2_vs_metVec[iSR].back()->SetTitle(dispt);
         h2_poly_MT2_vs_metVec[iSR].back()->GetXaxis()->SetLabelSize(h2_poly_MT2_vs_metVec[iSR].back()->GetXaxis()->GetLabelSize()*1.5);
         h2_poly_MT2_vs_metVec[iSR].back()->GetYaxis()->SetLabelSize(h2_poly_MT2_vs_metVec[iSR].back()->GetYaxis()->GetLabelSize()*1.5);
         h2_poly_MT2_vs_metVec[iSR].back()->GetXaxis()->SetTitleSize(h2_poly_MT2_vs_metVec[iSR].back()->GetXaxis()->GetTitleSize()*1.8);
         h2_poly_MT2_vs_metVec[iSR].back()->GetYaxis()->SetTitleSize(h2_poly_MT2_vs_metVec[iSR].back()->GetYaxis()->GetTitleSize()*1.8);
         h2_poly_MT2_vs_metVec[iSR].back()->GetXaxis()->SetTitleOffset(0.9);
         h2_poly_MT2_vs_metVec[iSR].back()->GetYaxis()->SetTitleOffset(1.2);
         h2_poly_MT2_vs_metVec[iSR].back()->SetMarkerSize(h2_poly_MT2_vs_metVec[iSR].back()->GetMarkerSize()*1.5);
         h2_poly_MT2_vs_metVec[iSR].back()->SetStats(0);
         h2_poly_MT2_vs_metVec[iSR].back()->Draw("text");
         char tmpStr[200];
         sprintf(tmpStr, "basicCheck_poly_MT2_vs_met_%s_%d.pdf", nameStr.c_str(), iSR); 
         c1->Print(tmpStr);
      }
   }


   TString rootNameStrT = "basicCheck"+nameStr+".root";

   TFile *basicCheckFile = new TFile(rootNameStrT, "RECREATE");

   if( h2_evtCnt_sumSM_nbJets_vs_nTops ) h2_evtCnt_sumSM_nbJets_vs_nTops->Write();
   if( h1_keyString ) h1_keyString->Write();
   for(unsigned int ih=0; ih<h1_cutFlowVec.size(); ih++){
      h1_cutFlowVec[ih]->Write();
      h1_cutFlow_auxVec[ih]->Write();
      h1_cutFlow_miscVec[ih]->Write();
      h2_evtCnt_nbJets_vs_nTopsVec[ih]->Write();

      for(int iSR = 0; iSR < nSR; iSR++){
         h2_poly_MT2_vs_metVec[iSR][ih]->Write();
      }

      h1_nJets_looseVec[ih]->Write(); h1_nTops_looseVec[ih]->Write(); h1_nbJets_looseVec[ih]->Write();
      h1_met_looseVec[ih]->Write(); h1_MT2_looseVec[ih]->Write(); h1_HT_looseVec[ih]->Write(); h1_metphi_looseVec[ih]->Write();
      h1_genmet_looseVec[ih]->Write(); h1_genmetphi_looseVec[ih]->Write();
      h1_met_absres_looseVec[ih]->Write(); h1_met_relres_looseVec[ih]->Write();
      h1_mht_looseVec[ih]->Write(); h1_mhtphi_looseVec[ih]->Write();
      h1_dphi1_looseVec[ih]->Write(); h1_dphi2_looseVec[ih]->Write(); h1_dphi3_looseVec[ih]->Write();
      h1_topMass_looseVec[ih]->Write();
      h1_vtxSize_looseVec[ih]->Write();
      h1_allJetPt_looseVec[ih]->Write(); h1_allJetEta_looseVec[ih]->Write(); h1_allJetPhi_looseVec[ih]->Write(); h1_allJetM_looseVec[ih]->Write();
      h1_leadJetPt_looseVec[ih]->Write(); h1_leadJetEta_looseVec[ih]->Write(); h1_leadJetPhi_looseVec[ih]->Write(); h1_leadJetM_looseVec[ih]->Write();
      h1_muPt_looseVec[ih]->Write(); h1_muEta_looseVec[ih]->Write(); h1_muPhi_looseVec[ih]->Write();
      h1_elePt_looseVec[ih]->Write(); h1_eleEta_looseVec[ih]->Write(); h1_elePhi_looseVec[ih]->Write();

      h1_met_loose_allHadVec[ih]->Write(); h1_MT2_loose_allHadVec[ih]->Write(); h1_nJets_loose_allHadVec[ih]->Write(); h1_nTops_loose_allHadVec[ih]->Write(); h1_nbJets_loose_allHadVec[ih]->Write();
      h1_met_loose_leptonicVec[ih]->Write(); h1_MT2_loose_leptonicVec[ih]->Write(); h1_nJets_loose_leptonicVec[ih]->Write(); h1_nTops_loose_leptonicVec[ih]->Write(); h1_nbJets_loose_leptonicVec[ih]->Write();
      h1_genmet_loose_allHadVec[ih]->Write(); h1_genmet_loose_leptonicVec[ih]->Write();
      h1_met_absres_loose_allHadVec[ih]->Write(); h1_met_relres_loose_allHadVec[ih]->Write();
      h1_met_absres_loose_leptonicVec[ih]->Write(); h1_met_relres_loose_leptonicVec[ih]->Write();

      h1_met_baseline_allHadVec[ih]->Write(); h1_MT2_baseline_allHadVec[ih]->Write(); h1_nJets_baseline_allHadVec[ih]->Write(); h1_nTops_baseline_allHadVec[ih]->Write(); h1_nbJets_baseline_allHadVec[ih]->Write();
      h1_met_baseline_leptonicVec[ih]->Write(); h1_MT2_baseline_leptonicVec[ih]->Write(); h1_nJets_baseline_leptonicVec[ih]->Write(); h1_nTops_baseline_leptonicVec[ih]->Write(); h1_nbJets_baseline_leptonicVec[ih]->Write();
      h1_genmet_baseline_allHadVec[ih]->Write(); h1_genmet_baseline_leptonicVec[ih]->Write();
      h1_met_absres_baseline_allHadVec[ih]->Write(); h1_met_relres_baseline_allHadVec[ih]->Write();
      h1_met_absres_baseline_leptonicVec[ih]->Write(); h1_met_relres_baseline_leptonicVec[ih]->Write();

      h1_mtw_looseVec[ih]->Write();

      h1_nJets_baselineVec[ih]->Write(); h1_nTops_baselineVec[ih]->Write(); h1_nbJets_baselineVec[ih]->Write();
      h1_met_baselineVec[ih]->Write(); h1_MT2_baselineVec[ih]->Write(); h1_HT_baselineVec[ih]->Write(); h1_metphi_baselineVec[ih]->Write();
      h1_genmet_baselineVec[ih]->Write(); h1_genmetphi_baselineVec[ih]->Write();
      h1_met_absres_baselineVec[ih]->Write(); h1_met_relres_baselineVec[ih]->Write();
      h1_mht_baselineVec[ih]->Write(); h1_mhtphi_baselineVec[ih]->Write();
      h1_topMass_baselineVec[ih]->Write();
      h1_dphi1_baselineVec[ih]->Write(); h1_dphi2_baselineVec[ih]->Write(); h1_dphi3_baselineVec[ih]->Write();
      h1_vtxSize_baselineVec[ih]->Write();
      h1_allJetPt_baselineVec[ih]->Write(); h1_allJetEta_baselineVec[ih]->Write(); h1_allJetPhi_baselineVec[ih]->Write(); h1_allJetM_baselineVec[ih]->Write();
      h1_leadJetPt_baselineVec[ih]->Write(); h1_leadJetEta_baselineVec[ih]->Write(); h1_leadJetPhi_baselineVec[ih]->Write(); h1_leadJetM_baselineVec[ih]->Write();
      h1_muPt_baselineVec[ih]->Write(); h1_muEta_baselineVec[ih]->Write(); h1_muPhi_baselineVec[ih]->Write();
      h1_elePt_baselineVec[ih]->Write(); h1_eleEta_baselineVec[ih]->Write(); h1_elePhi_baselineVec[ih]->Write();

      h1_nTops_Nm1_baselineVec[ih]->Write(); h1_nbJets_Nm1_baselineVec[ih]->Write();
      h1_met_Nm1_baselineVec[ih]->Write(); h1_MT2_Nm1_baselineVec[ih]->Write();

      h1_mtw_baselineVec[ih]->Write();

      h1_searchBinYieldsVec[ih]->Write();
      h1_searchBinYields_HadTauVec[ih]->Write(); h1_searchBinYields_LostLepVec[ih]->Write(); h1_searchBinYields_OverlapVec[ih]->Write();

      h1_scaleUncNominal_searchBinYieldsVec[ih]->Write(); h1_scaleUncUp_searchBinYieldsVec[ih]->Write(); h1_scaleUncDown_searchBinYieldsVec[ih]->Write();

      h1_pdfUncCentral_searchBinYieldsVec[ih]->Write(); h1_pdfUncUp_searchBinYieldsVec[ih]->Write(); h1_pdfUncDown_searchBinYieldsVec[ih]->Write();

      h2_MT2_vs_met_baselineVec[ih]->Write();

      h2_MT2_vs_met_looseVec[ih]->Write();
      h2_MT2_vs_alt_MT2_looseVec[ih]->Write();
      h2_alt_MT2_vs_met_looseVec[ih]->Write();

      h2_nbJets_vs_nJetsVec[ih]->Write();
      h2_nTops_vs_nJetsVec[ih]->Write();
      h2_nTops_vs_nbJetsVec[ih]->Write();

      h2_nbJets_vs_nJets_baselineVec[ih]->Write();
      h2_nTops_vs_nJets_baselineVec[ih]->Write();
      h2_nTops_vs_nbJets_baselineVec[ih]->Write();
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
   basicCheck(argc, argv);
}

