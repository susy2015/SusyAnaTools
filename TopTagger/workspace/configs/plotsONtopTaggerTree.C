#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<vector<int> > >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#pragma link C++ class vector<vector<TFile*> >;
#pragma link C++ class vector<vector<TH1D*> >;
#pragma link C++ class vector< map<int, int> >;
#endif

using namespace std;

const bool doExtraCuts_ = true;
//const bool doExtraCuts_ = false;

const double cutCSVS = 0.679;
//const double cutCSVS = 0.898;
const bool applyLowMET = false;
const bool withMETcut = true;

/*
//const TString treeStrT = "topTaggerAllVeto";
//const TString treeStrT = "topTaggerAllVetoCSVT";
//const TString treeStrT = "topTaggerAllVetoCSVTRA2topEleMuVeto";
//const TString treeStrT = "topTaggerSingleMuSel";
*/
const TString treeStrT = "topTaggerAllVetoRA2topEleMuVeto";
//const TString treeStrT = "topTaggerSingleMuSelRA2topEleVeto";

const int nMC = 11;
const string      mcStr[] = {   "TTbar", "WJetsToLNu",   "Zinv_400HTinf",   "Zinv_200HT400",   "DYJets",    "QCDFlat",    "T_t",    "T_tW",    "Signal_x500_y100",  "Signal_x450_y150",  "Signal_x350_y50" };
//const double    xSecArr[] = {  225.1967,     25.22,               5.274,           41.49,     3532.8149,  2.99913994E10,   47.0,  11.1773*2.0,    0.0855847,           0.169668,           0.807323        };
const double    xSecArr[] = {  225.1967,     30.08,                6.26,           49.28,     3532.8149,  2.99913994E10,   47.0,  11.1773*2.0,    0.0855847,           0.169668,           0.807323        };
const int       nEvtArr[] = {   6722660,   1647807,             1006925,          5066601,      1000000,     9998151,      23777,   473554,          98720,              49996,              99992         };
static const int colors[] = {    kGreen,   kMagenta+1,          kTeal+4,          kTeal+4,    kYellow-7,       kBlue,    kYellow,   kGreen+2,         kRed,                kRed,              kRed         };

const double dataLumi = 698.873/1000. + 4.389; // in fb-1
const double scaledToDataLumi = 10.0; // in fb-1

double dataScale = 1.0;

vector<double> scalesVec;

vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

double calcMT2(const TLorentzVector &fatJet1LVec, const TLorentzVector &fatJet2LVec, const TLorentzVector &metLVec);

const double mTbcut_ = 500, mTtcut_ = 365, MT2cut_ = 300, mTWcut_ = 600;
const double lowTopCut_ = 80, highTopCut_ = 270, lowWCut_ = 50, highWCut_ = 120;
//const double lowTopCut_ = 120, highTopCut_ = 230, lowWCut_ = 50, highWCut_ = 120;

// PDG values (in GeV)
static const double mW_ = 80.385, mTop_ = 173.5;

static const double mWoverTop_ = mW_/mTop_;

static const double lowRatioWoverTop_ = 0.85, highRatioWoverTop_ = 1.25;
static const double lowArcTanm13overm12_ = 0.2, highArcTanm13overm12_ = 1.3, lowm23overm123_ = 0.35;

static const double Rmin_ = lowRatioWoverTop_*mWoverTop_, Rmax_ = highRatioWoverTop_*mWoverTop_;

double template_cntEventsWeighted =0, template_cntEventsWeightedSquared =0;
double template_sumpuWeight =0;
double template_cntNumEvents =0;
double template_cntAftBaselineMinusMETWeighted =0, template_cntAftBaselineMinusMETWeightedSquared =0;
double template_cntAftBaselineWeighted =0, template_cntAftBaselineWeightedSquared =0;
double template_cntAftPreCutsWeighted =0, template_cntAftPreCutsWeightedSquared =0;
double template_cntGoodFatJetPairsWeighted =0, template_cntGoodFatJetPairsWeightedSquared =0;
double template_cntGoodRemainingWmassWeighted =0, template_cntGoodRemainingWmassWeightedSquared =0;
double template_cntRemainingbPlusAJetWeighted =0, template_cntRemainingbPlusAJetWeightedSquared =0;
double template_cntRemainingbJetWeighted =0, template_cntRemainingbJetWeightedSquared =0;
double template_cntMT2Weighted =0, template_cntMT2WeightedSquared =0;
double template_cntmTbWeighted =0, template_cntmTbWeightedSquared =0;
double template_cntmTtWeighted =0, template_cntmTtWeightedSquared =0;
double template_cntmTWWeighted =0, template_cntmTWWeightedSquared =0;

double template_cntEventsWeightedScaledMC = 0;
double template_cntEventsWeightedErrorScaledMC = 0; 
double template_sumpuWeightSumSample =0;
double template_cntNumEventsSumSample =0;

double template_cntAftBaselineMinusMETWeightedScaledMC = 0, template_cntAftBaselineMinusMETWeightedErrorScaledMC = 0;
double template_cntAftBaselineWeightedScaledMC = 0, template_cntAftBaselineWeightedErrorScaledMC = 0;
double template_cntAftPreCutsWeightedScaledMC = 0, template_cntAftPreCutsWeightedErrorScaledMC = 0;
double template_cntGoodFatJetPairsWeightedScaledMC = 0, template_cntGoodFatJetPairsWeightedErrorScaledMC = 0;
double template_cntGoodRemainingWmassWeightedScaledMC =0, template_cntGoodRemainingWmassWeightedErrorScaledMC =0;
double template_cntRemainingbPlusAJetWeightedScaledMC =0, template_cntRemainingbPlusAJetWeightedErrorScaledMC =0;
double template_cntRemainingbJetWeightedScaledMC =0, template_cntRemainingbJetWeightedErrorScaledMC =0;
double template_cntMT2WeightedScaledMC = 0, template_cntMT2WeightedErrorScaledMC = 0;
double template_cntmTbWeightedScaledMC = 0, template_cntmTbWeightedErrorScaledMC = 0;
double template_cntmTtWeightedScaledMC = 0, template_cntmTtWeightedErrorScaledMC = 0;
double template_cntmTWWeightedScaledMC = 0, template_cntmTWWeightedErrorScaledMC = 0;

vector<string> keyStringCachedVec;
vector<double> scaleMCCachedVec;
vector<int> colorCachedVec;

vector<TH1D*> h1_topJetPt_aftConeAndCatsVec, h1_mfatJet_aftConeVec, h1_m12_aftConeVec, h1_m13_aftConeVec, h1_m23_aftConeVec;
vector<TH1D*> h1_m12_aftConeAndCat00Vec, h1_m13_aftConeAndCat00Vec, h1_m23_aftConeAndCat00Vec;
vector<TH1D*> h1_m12_aftConeAndCat01Vec, h1_m13_aftConeAndCat01Vec, h1_m23_aftConeAndCat01Vec;
vector<TH1D*> h1_m12_aftConeAndCat02Vec, h1_m13_aftConeAndCat02Vec, h1_m23_aftConeAndCat02Vec;
vector<TH1D*> h1_metVec, h1_met_aftGoodFatJetPairsVec, h1_met_aftMTAndMT2CutsVec; 
vector<TH1D*> h1_nCombInAConeVec, h1_nTopCandVec, h1_nTopCandWithBtagVec;
vector<TH1D*> h1_mTremainingTopJetVec, h1_mTbestTopJetVec, h1_mTbJetVec, h1_MT2Vec, h1_mTbestWJetVec, h1_mTbestbJetVec, h1_mTremainingTopJetWithGoodWmassVec, h1_mTbJetPlusmTbestTopJetVec;
vector<TH1D*> h1_mTbestTopJet_aftMTAndMT2CutsVec, h1_mTbJet_aftMTAndMT2CutsVec, h1_MT2_aftMTAndMT2CutsVec, h1_mTremainingTopJet_aftMTAndMT2CutsVec, h1_mTbJetPlusmTbestTopJet_aftMTAndMT2CutsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCatsVec, h1_mbestWJet_aftConeAndCatsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec, h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftMETVec, h1_mbestWJet_aftConeAndCats_aftMETVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftPtVec, h1_mbestWJet_aftConeAndCats_aftPtVec;
vector<TH1D*> h1_remainingmTopJet_aftConeAndCatsVec, h1_remainingmWJet_aftConeAndCatsVec;
vector<TH1D*> h1_remainingmTopJet_aftConeAndCats_WithGoodWmassVec, h1_remainingmWJet_aftConeAndCats_WithGoodWmassVec;
vector<TH1D*> h1_remainingnbJetVec, h1_bestnbJetVec, h1_totalnbJetVec;

vector<TH2D*> h2_m23overm123vsarctanm13overm12Vec, h2_mTbestWvsmTbestbVec, h2_mTbestTopJetvsmTbJetVec;
vector<TH2D*> h2_remainingmTopJetvsremainingmWJetVec, h2_bestmTopJetvsbestmWJetVec;

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH1D*> &h1_inputVec, const int nRebin =1, const TString optDrawStr ="");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH2D*> &h2_inputVec);

void initMCscales(){

   dataScale = scaledToDataLumi/dataLumi;
   cout<<"\nscaledToDataLumi : "<<scaledToDataLumi<<"  dataLumi : "<<dataLumi<<"  dataScale : "<<dataScale<<endl;
   for(int ib=0; ib<nMC; ib++){
      double perScale = scaledToDataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
      scalesVec.push_back(perScale);
      printf("%20s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
   }
   cout<<endl;
}

void templatePlotsFunc(std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar"){

  TString sampleKeyStringT(sampleKeyString);

  keyStringCachedVec.push_back(sampleKeyString);
  double sampleScaleMC = 1.0; int sampleColor = 0;
  for(int ib=0; ib<nMC; ib++){
     TString permcStrT(mcStr[ib]);
     if( permcStrT.Contains(sampleKeyString) ) { sampleColor = colors[ib]; } 
  }
  scaleMCCachedVec.push_back( sampleScaleMC );
  colorCachedVec.push_back( sampleColor );

  template_cntEventsWeightedScaledMC = 0;
  template_cntEventsWeightedErrorScaledMC = 0; 

  template_sumpuWeightSumSample =0;
  template_cntNumEventsSumSample =0;

  template_cntAftBaselineMinusMETWeightedScaledMC = 0; template_cntAftBaselineMinusMETWeightedErrorScaledMC = 0;
  template_cntAftBaselineWeightedScaledMC = 0; template_cntAftBaselineWeightedErrorScaledMC = 0;
  template_cntAftPreCutsWeightedScaledMC = 0; template_cntAftPreCutsWeightedErrorScaledMC = 0;
  template_cntGoodFatJetPairsWeightedScaledMC = 0; template_cntGoodFatJetPairsWeightedErrorScaledMC = 0;
  template_cntGoodRemainingWmassWeightedScaledMC =0; template_cntGoodRemainingWmassWeightedErrorScaledMC =0;
  template_cntRemainingbPlusAJetWeightedScaledMC =0; template_cntRemainingbPlusAJetWeightedErrorScaledMC =0;
  template_cntRemainingbJetWeightedScaledMC =0; template_cntRemainingbJetWeightedErrorScaledMC =0;
  template_cntMT2WeightedScaledMC = 0; template_cntMT2WeightedErrorScaledMC = 0;
  template_cntmTbWeightedScaledMC = 0; template_cntmTbWeightedErrorScaledMC = 0;
  template_cntmTtWeightedScaledMC = 0; template_cntmTtWeightedErrorScaledMC = 0;
  template_cntmTWWeightedScaledMC = 0; template_cntmTWWeightedErrorScaledMC = 0;

  TH1D *template_h1_topJetPt_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_topJetPt_aftConeAndCats", sampleKeyStringT+":  topJetPt_aftConeAndCats; top pt", 100, 0, 1000); template_h1_topJetPt_aftConeAndCats->Sumw2();

  TH1D *template_h1_mfatJet_aftCone = new TH1D(sampleKeyStringT+"_h1_mfatJet_aftCone", sampleKeyStringT+":  fatJet mass in a cone; fatJet mass", 100, 0, 1000); template_h1_mfatJet_aftCone->Sumw2();
  TH1D *template_h1_m12_aftCone = new TH1D(sampleKeyStringT+"_h1_m12_aftCone", sampleKeyStringT+":  m12_aftCone; m12", 100, 0, 300); template_h1_m12_aftCone->Sumw2();
  TH1D *template_h1_m13_aftCone = new TH1D(sampleKeyStringT+"_h1_m13_aftCone", sampleKeyStringT+":  m13_aftCone; m13", 100, 0, 300); template_h1_m13_aftCone->Sumw2();
  TH1D *template_h1_m23_aftCone = new TH1D(sampleKeyStringT+"_h1_m23_aftCone", sampleKeyStringT+":  m23_aftCone; m23", 100, 0, 300); template_h1_m23_aftCone->Sumw2();

  TH1D *template_h1_mbestTopJet_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_mbestTopJet_aftConeAndCats", sampleKeyStringT+":  top jet mass after tagger; m_{t}", 100, 0, 380); template_h1_mbestTopJet_aftConeAndCats->Sumw2();
  TH1D *template_h1_mbestWJet_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_mbestWJet_aftConeAndCats", sampleKeyStringT+":  W mass after tagger; m_{W}", 100, 0, 200); template_h1_mbestWJet_aftConeAndCats->Sumw2();
  TH1D *template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts", sampleKeyStringT+":  top jet mass after MT2 and (mTb, mTt) cuts; m_{t}", 100, 0, 380); template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts", sampleKeyStringT+":  W mass after MT2 and (mTb, mTt) cuts; m_{W}", 100, 0, 200); template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts->Sumw2();

  TH1D *template_h1_mbestTopJet_aftConeAndCats_aftMET = new TH1D(sampleKeyStringT+"_h1_mbestTopJet_aftConeAndCats_aftMET", sampleKeyStringT+":  top jet mass after tagger & MET cut; m_{t}", 100, 0, 380); template_h1_mbestTopJet_aftConeAndCats_aftMET->Sumw2();
  TH1D *template_h1_mbestWJet_aftConeAndCats_aftMET = new TH1D(sampleKeyStringT+"_h1_mbestWJet_aftConeAndCats_aftMET", sampleKeyStringT+":  W mass after tagger & MET cut; m_{W}", 100, 0, 200); template_h1_mbestWJet_aftConeAndCats_aftMET->Sumw2();

  TH1D *template_h1_mbestTopJet_aftConeAndCats_aftPt = new TH1D(sampleKeyStringT+"_h1_mbestTopJet_aftConeAndCats_aftPt", sampleKeyStringT+":  top jet mass after tagger & top pt cut; m_{t}", 100, 0, 380); template_h1_mbestTopJet_aftConeAndCats_aftPt->Sumw2();
  TH1D *template_h1_mbestWJet_aftConeAndCats_aftPt = new TH1D(sampleKeyStringT+"_h1_mbestWJet_aftConeAndCats_aftPt", sampleKeyStringT+":  W mass after tagger & top pt cut; m_{W}", 100, 0, 200); template_h1_mbestWJet_aftConeAndCats_aftPt->Sumw2();

  TH1D *template_h1_remainingmTopJet_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_remainingmTopJet_aftConeAndCats", sampleKeyStringT+":  remaining top jet mass after tagger; m_{t}", 100, 0, 800); template_h1_remainingmTopJet_aftConeAndCats->Sumw2();
  TH1D *template_h1_remainingmWJet_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_remainingmWJet_aftConeAndCats", sampleKeyStringT+":  remaining W mass after tagger; m_{W}", 100, 0, 500); template_h1_remainingmWJet_aftConeAndCats->Sumw2();
  TH1D *template_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass = new TH1D(sampleKeyStringT+"_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass", sampleKeyStringT+":  remaining top jet mass after tagger with good W mass; m_{t}", 100, 0, 800); template_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass->Sumw2();
  TH1D *template_h1_remainingmWJet_aftConeAndCats_WithGoodWmass = new TH1D(sampleKeyStringT+"_h1_remainingmWJet_aftConeAndCats_WithGoodWmass", sampleKeyStringT+":  remaining W mass after tagger with good W mass; m_{W}", 100, 0, 500); template_h1_remainingmWJet_aftConeAndCats_WithGoodWmass->Sumw2();

  TH1D *template_h1_m12_aftConeAndCat00 = new TH1D(sampleKeyStringT+"_h1_m12_aftConeAndCat00", sampleKeyStringT+":  m12_aftConeAndCat00; m12", 100, 0, 300); template_h1_m12_aftConeAndCat00->Sumw2();
  TH1D *template_h1_m13_aftConeAndCat00 = new TH1D(sampleKeyStringT+"_h1_m13_aftConeAndCat00", sampleKeyStringT+":  m13_aftConeAndCat00; m13", 100, 0, 300); template_h1_m13_aftConeAndCat00->Sumw2();
  TH1D *template_h1_m23_aftConeAndCat00 = new TH1D(sampleKeyStringT+"_h1_m23_aftConeAndCat00", sampleKeyStringT+":  m23_aftConeAndCat00; m23", 100, 0, 300); template_h1_m23_aftConeAndCat00->Sumw2();

  TH1D *template_h1_m12_aftConeAndCat01 = new TH1D(sampleKeyStringT+"_h1_m12_aftConeAndCat01", sampleKeyStringT+":  m12_aftConeAndCat01; m12", 100, 0, 300); template_h1_m12_aftConeAndCat01->Sumw2();
  TH1D *template_h1_m13_aftConeAndCat01 = new TH1D(sampleKeyStringT+"_h1_m13_aftConeAndCat01", sampleKeyStringT+":  m13_aftConeAndCat01; m13", 100, 0, 300); template_h1_m13_aftConeAndCat01->Sumw2();
  TH1D *template_h1_m23_aftConeAndCat01 = new TH1D(sampleKeyStringT+"_h1_m23_aftConeAndCat01", sampleKeyStringT+":  m23_aftConeAndCat01; m23", 100, 0, 300); template_h1_m23_aftConeAndCat01->Sumw2();

  TH1D *template_h1_m12_aftConeAndCat02 = new TH1D(sampleKeyStringT+"_h1_m12_aftConeAndCat02", sampleKeyStringT+":  m12_aftConeAndCat02; m12", 100, 0, 300); template_h1_m12_aftConeAndCat02->Sumw2();
  TH1D *template_h1_m13_aftConeAndCat02 = new TH1D(sampleKeyStringT+"_h1_m13_aftConeAndCat02", sampleKeyStringT+":  m13_aftConeAndCat02; m13", 100, 0, 300); template_h1_m13_aftConeAndCat02->Sumw2();
  TH1D *template_h1_m23_aftConeAndCat02 = new TH1D(sampleKeyStringT+"_h1_m23_aftConeAndCat02", sampleKeyStringT+":  m23_aftConeAndCat02; m23", 100, 0, 300); template_h1_m23_aftConeAndCat02->Sumw2();

  TH2D *template_h2_m23overm123vsarctanm13overm12 = new TH2D(sampleKeyStringT+"_h2_m23overm123vsarctanm13overm12", sampleKeyStringT+":  InACone: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);

  TH1D *template_h1_met = new TH1D(sampleKeyStringT+"_h1_met", sampleKeyStringT+":  met; met", 100, 0, 1000); template_h1_met->Sumw2();
  TH1D *template_h1_met_aftGoodFatJetPairs = new TH1D(sampleKeyStringT+"_h1_met_aftGoodFatJetPairs", sampleKeyStringT+":  met aftGoodFatJetPairs; met", 100, 0, 1000); template_h1_met_aftGoodFatJetPairs->Sumw2();
  TH1D *template_h1_met_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_met_aftMTAndMT2Cuts", sampleKeyStringT+":  met aftMTAndMT2Cuts; met", 100, 0, 1000); template_h1_met_aftMTAndMT2Cuts->Sumw2();

  TH1D *template_h1_nCombInACone = new TH1D(sampleKeyStringT+"_h1_nCombInACone", sampleKeyStringT+":  number of combination in a cone; nCand", 20, 0, 20); template_h1_nCombInACone->Sumw2();
  TH1D *template_h1_nTopCand = new TH1D(sampleKeyStringT+"_h1_nTopCand", sampleKeyStringT+":  number of top candidate; nTop", 20, 0, 20); template_h1_nTopCand->Sumw2();
  TH1D *template_h1_nTopCandWithBtag = new TH1D(sampleKeyStringT+"_h1_nTopCandWithBtag", sampleKeyStringT+":  Btag: number of top candidate; nTop", 20, 0, 20); template_h1_nTopCandWithBtag->Sumw2();

  TH1D *template_h1_remainingnbJet = new TH1D(sampleKeyStringT+"_h1_remainingnbJet", sampleKeyStringT+":  number of b-jet in the remaining jets; nbJet", 10, 0, 10); template_h1_remainingnbJet->Sumw2();
  TH1D *template_h1_bestnbJet = new TH1D(sampleKeyStringT+"_h1_bestnbJet", sampleKeyStringT+":  number of b-jet in the best top jet; nbJet", 10, 0, 10); template_h1_bestnbJet->Sumw2();
  TH1D *template_h1_totalnbJet = new TH1D(sampleKeyStringT+"_h1_totalnbJet", sampleKeyStringT+":  number of b-jet in the total top jet; nbJet", 10, 0, 10); template_h1_totalnbJet->Sumw2();

  TH1D *template_h1_mTremainingTopJet = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJet", sampleKeyStringT+":  mT of remaining top jet & met; mTr", 100, 0, 1500); template_h1_mTremainingTopJet->Sumw2();
  TH1D *template_h1_mTremainingTopJetWithGoodWmass = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJetWithGoodWmass", sampleKeyStringT+":  mT of remaining top jet & met with good W mass; mTr", 100, 0, 1000); template_h1_mTremainingTopJetWithGoodWmass->Sumw2();

  TH1D *template_h1_mTbestTopJet = new TH1D(sampleKeyStringT+"_h1_mTbestTopJet", sampleKeyStringT+":  mT of best top jet to met; mTt", 100, 0, 1000); template_h1_mTbestTopJet->Sumw2();
  TH1D *template_h1_mTbJet = new TH1D(sampleKeyStringT+"_h1_mTbJet", sampleKeyStringT+":  mT of b jet to met; mTb", 100, 0, 1000); template_h1_mTbJet->Sumw2();
  TH1D *template_h1_MT2 = new TH1D(sampleKeyStringT+"_h1_MT2", sampleKeyStringT+":  MT2 of the system; MT2", 100, 0, 1000); template_h1_MT2->Sumw2();
  TH1D *template_h1_mTbJetPlusmTbestTopJet = new TH1D(sampleKeyStringT+"_h1_mTbJetPlusmTbestTopJet", sampleKeyStringT+":  mTbJet + 0.5*mTbestTopJet; mTb+0.5mTt", 100, 0, 1500); template_h1_mTbJetPlusmTbestTopJet->Sumw2();

  TH1D *template_h1_mTbestTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbestTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of best top jet to met aftMTAndMT2Cuts; mTt", 100, 0, 1000); template_h1_mTbestTopJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTbJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of b jet to met aftMTAndMT2Cuts; mTb", 100, 0, 1000); template_h1_mTbJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mTbJet + 0.5*mTbestTopJet aftMTAndMT2Cuts; mTb+0.5mTt", 100, 0, 1500); template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_MT2_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_MT2_aftMTAndMT2Cuts", sampleKeyStringT+":  MT2 of the system aftMTAndMT2Cuts; MT2", 100, 0, 1000); template_h1_MT2_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTremainingTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of remaining top jet to met aftMTAndMT2Cuts; mTr", 100, 0, 1000); template_h1_mTremainingTopJet_aftMTAndMT2Cuts->Sumw2();

  TH1D *template_h1_mTbestWJet = new TH1D(sampleKeyStringT+"_h1_mTbestWJet", sampleKeyStringT+":  mT of best W jet & met; mTW", 100, 0, 1000); template_h1_mTbestWJet->Sumw2();
  TH1D *template_h1_mTbestbJet = new TH1D(sampleKeyStringT+"_h1_mTbestbJet", sampleKeyStringT+":  mT of the other jet & met; mTo", 100, 0, 1000); template_h1_mTbestbJet->Sumw2();

  TH2D *template_h2_mTbestWvsmTbestb = new TH2D(sampleKeyStringT+"_h2_mTbestWvsmTbestb", sampleKeyStringT+":  mT of best W versus mT of the other jet; mTo; mTW", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbestTopJetvsmTbJet = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsmTbJet", sampleKeyStringT+":  mT of best top jet versus mT of b jet; mTb; mTt", 100, 0, 1000, 100, 0, 1000);

  TH2D *template_h2_remainingmTopJetvsremainingmWJet = new TH2D(sampleKeyStringT+"_h2_remainingmTopJetvsremainingmWJet", sampleKeyStringT+":  remaining top jet mass versus remaining W mass; m_{W}; m_{t}", 100, 0, 500, 100, 0, 800);
  TH2D *template_h2_bestmTopJetvsbestmWJet = new TH2D(sampleKeyStringT+"_h2_bestmTopJetvsbestmWJet", sampleKeyStringT+":  best top jet mass versus best W mass; m_{W}; m_{t}", 100, 0, 200, 100, 0, 380);

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = 1.0;
     for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ){ scaleMC = scalesVec[ib]; } }
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ) scaleMC = dataScale;

     int template_run, template_event, template_lumi, template_nm1, template_n0, template_np1, template_vtxSize;
     double template_avg_npv, template_tru_npv;
     int template_nJets;
     double template_evtWeight;
     double template_met, template_metphi;

     vector<TLorentzVector> *template_oriJetsVec =0; vector<double> *template_recoJetsBtagCSVS =0;
     vector<vector<int> > *template_finalCombfatJets =0, *template_finalRemaining =0, *template_finalRemainingCombfatJets =0, *template_finalRemaining2 =0;
     vector<vector<vector<int> > > *template_finalCombJetSubStruc =0, *template_finalRemainingCombJetSubStruc =0;
   
     vector<TLorentzVector> *template_allFatJetsInAConeLVec =0, *template_allSubStruc12LVec =0, *template_allSubStruc13LVec =0, *template_allSubStruc23LVec =0;
     vector<int> *template_allFatJetsPassCat00Status =0, *template_allFatJetsPassCat01Status =0, *template_allFatJetsPassCat02Status =0, *template_allFatJetsRemainingPassCSVS =0;
     int template_bestFatJetsPairIdx, template_pickedRemainingCombfatJetIdx, template_pickedRemainingSubStrucmxyIdx, template_pickedRemainingCombfatJetbJetIdx;
     vector<TLorentzVector> *template_remainingAllFatJetsCombLVec =0, *template_remainingAllSubStruc12LVec =0, *template_remainingAllSubStruc13LVec =0, *template_remainingAllSubStruc23LVec =0;
     vector<TLorentzVector> *template_pickedRemainingbJetLVec =0, *template_pickedRemainingbJetPlusAJetLVec =0;
     int template_pickedClosestJetTobIdx, template_closebJetIdx;
     double template_mTbestTopJet, template_mTbestWJet, template_mTbestbJet, template_mTremainingTopJet, template_closemT, template_mTbJet, template_MT2;
     vector<TLorentzVector> *template_closeToMETbJetLVec =0;
     double template_bestTopJetMass, template_pickedRemainingmxy, template_pickedRemainingfatJetMass;
   
     vector<int> *template_pickedbTaggedJetIdxVec =0;
     vector<TLorentzVector> *template_pickedbTaggedJetLVec =0;
     vector<double> *template_pickedbTaggedJetCSVS =0;
   
     vector<int> *template_pickedbTaggedbestTopJetIdxVec =0;
     vector<TLorentzVector> *template_pickedbTaggedbestTopJetLVec =0;
     vector<double> *template_pickedbTaggedbestTopJetCSVS =0;
   
     vector<TLorentzVector> *template_jetsLVec =0; vector<double> *template_jetsBtagCSVS =0;
   
     vector<TLorentzVector> *template_gentopLVec =0;
     vector<int> *template_gentoppdgIdVec =0;
     vector<vector<TLorentzVector> > *template_gentopDausLVec =0, *template_genWDausLVec =0, *template_gentauDausLVec =0;
     vector<vector<int> > *template_gentopDauspdgIdVec =0, *template_genWDauspdgIdVec =0, *template_gentauDauspdgIdVec =0;
     vector<string> *template_decayTypeVec =0;
  
     TTree * template_AUX = treeVec[ist];
     template_AUX->SetBranchAddress("run", &template_run);
     template_AUX->SetBranchAddress("event", &template_event);
     template_AUX->SetBranchAddress("lumi", &template_lumi);
     template_AUX->SetBranchAddress("avg_npv", &template_avg_npv);
     template_AUX->SetBranchAddress("nm1", &template_nm1);
     template_AUX->SetBranchAddress("n0", &template_n0);
     template_AUX->SetBranchAddress("np1", &template_np1);
     template_AUX->SetBranchAddress("tru_npv", &template_tru_npv);
     template_AUX->SetBranchAddress("vtxSize", &template_vtxSize);
     template_AUX->SetBranchAddress("nJets", &template_nJets);
     template_AUX->SetBranchAddress("jetsLVec", &template_jetsLVec);
     template_AUX->SetBranchAddress("jetsBtagCSVS", &template_jetsBtagCSVS);
     template_AUX->SetBranchAddress("oriJetsVec", &template_oriJetsVec);
     template_AUX->SetBranchAddress("recoJetsBtagCSVS", &template_recoJetsBtagCSVS);
     template_AUX->SetBranchAddress("finalCombfatJets", &template_finalCombfatJets);
     template_AUX->SetBranchAddress("finalRemaining", &template_finalRemaining);
     template_AUX->SetBranchAddress("finalRemainingCombfatJets", &template_finalRemainingCombfatJets);
     template_AUX->SetBranchAddress("finalRemaining2", &template_finalRemaining2);
     template_AUX->SetBranchAddress("finalCombJetSubStruc", &template_finalCombJetSubStruc);
     template_AUX->SetBranchAddress("finalRemainingCombJetSubStruc", &template_finalRemainingCombJetSubStruc);
     template_AUX->SetBranchAddress("evtWeight", &template_evtWeight);
     template_AUX->SetBranchAddress("met", &template_met);
     template_AUX->SetBranchAddress("metphi", &template_metphi);
     template_AUX->SetBranchAddress("allFatJetsInAConeLVec", &template_allFatJetsInAConeLVec);
     template_AUX->SetBranchAddress("allSubStruc12LVec", &template_allSubStruc12LVec);
     template_AUX->SetBranchAddress("allSubStruc13LVec", &template_allSubStruc13LVec);
     template_AUX->SetBranchAddress("allSubStruc23LVec", &template_allSubStruc23LVec);
     template_AUX->SetBranchAddress("allFatJetsPassCat00Status", &template_allFatJetsPassCat00Status);
     template_AUX->SetBranchAddress("allFatJetsPassCat01Status", &template_allFatJetsPassCat01Status);
     template_AUX->SetBranchAddress("allFatJetsPassCat02Status", &template_allFatJetsPassCat02Status);
     template_AUX->SetBranchAddress("allFatJetsRemainingPassCSVS", &template_allFatJetsRemainingPassCSVS);
     template_AUX->SetBranchAddress("bestFatJetsPairIdx", &template_bestFatJetsPairIdx);
     template_AUX->SetBranchAddress("remainingAllFatJetsCombLVec", &template_remainingAllFatJetsCombLVec);
     template_AUX->SetBranchAddress("remainingAllSubStruc12LVec", &template_remainingAllSubStruc12LVec);
     template_AUX->SetBranchAddress("remainingAllSubStruc13LVec", &template_remainingAllSubStruc13LVec);
     template_AUX->SetBranchAddress("remainingAllSubStruc23LVec", &template_remainingAllSubStruc23LVec);
     template_AUX->SetBranchAddress("pickedRemainingCombfatJetIdx", &template_pickedRemainingCombfatJetIdx);
     template_AUX->SetBranchAddress("pickedRemainingSubStrucmxyIdx", &template_pickedRemainingSubStrucmxyIdx);
     template_AUX->SetBranchAddress("pickedRemainingCombfatJetbJetIdx", &template_pickedRemainingCombfatJetbJetIdx);
     template_AUX->SetBranchAddress("pickedRemainingbJetLVec", &template_pickedRemainingbJetLVec);
     template_AUX->SetBranchAddress("pickedRemainingbJetPlusAJetLVec", &template_pickedRemainingbJetPlusAJetLVec);
     template_AUX->SetBranchAddress("pickedClosestJetTobIdx", &template_pickedClosestJetTobIdx);
     template_AUX->SetBranchAddress("closebJetIdx", &template_closebJetIdx);
     template_AUX->SetBranchAddress("mTbestTopJet", &template_mTbestTopJet);
     template_AUX->SetBranchAddress("mTbestWJet", &template_mTbestWJet);
     template_AUX->SetBranchAddress("mTbestbJet", &template_mTbestbJet);
     template_AUX->SetBranchAddress("mTremainingTopJet", &template_mTremainingTopJet);
     template_AUX->SetBranchAddress("closemT", &template_closemT);
     template_AUX->SetBranchAddress("mTbJet", &template_mTbJet);
     template_AUX->SetBranchAddress("MT2", &template_MT2);
     template_AUX->SetBranchAddress("closeToMETbJetLVec", &template_closeToMETbJetLVec);
     template_AUX->SetBranchAddress("bestTopJetMass", &template_bestTopJetMass);
     template_AUX->SetBranchAddress("pickedRemainingmxy", &template_pickedRemainingmxy);
     template_AUX->SetBranchAddress("pickedRemainingfatJetMass", &template_pickedRemainingfatJetMass);
     template_AUX->SetBranchAddress("pickedbTaggedJetIdxVec", &template_pickedbTaggedJetIdxVec);
     template_AUX->SetBranchAddress("pickedbTaggedJetLVec", &template_pickedbTaggedJetLVec);
     template_AUX->SetBranchAddress("pickedbTaggedJetCSVS", &template_pickedbTaggedJetCSVS);
     template_AUX->SetBranchAddress("pickedbTaggedbestTopJetIdxVec", &template_pickedbTaggedbestTopJetIdxVec);
     template_AUX->SetBranchAddress("pickedbTaggedbestTopJetLVec", &template_pickedbTaggedbestTopJetLVec);
     template_AUX->SetBranchAddress("pickedbTaggedbestTopJetCSVS", &template_pickedbTaggedbestTopJetCSVS);
   
     template_AUX->SetBranchAddress("gentopLVec", &template_gentopLVec);
     template_AUX->SetBranchAddress("gentoppdgIdVec", &template_gentoppdgIdVec);
     template_AUX->SetBranchAddress("gentopDausLVec", &template_gentopDausLVec);
     template_AUX->SetBranchAddress("gentopDauspdgIdVec", &template_gentopDauspdgIdVec);
     template_AUX->SetBranchAddress("genWDausLVec", &template_genWDausLVec);
     template_AUX->SetBranchAddress("genWDauspdgIdVec", &template_genWDauspdgIdVec);
     template_AUX->SetBranchAddress("gentauDausLVec", &template_gentauDausLVec);
     template_AUX->SetBranchAddress("gentauDauspdgIdVec", &template_gentauDauspdgIdVec);
     template_AUX->SetBranchAddress("decayTypeVec", &template_decayTypeVec);
   
     int template_Entries = template_AUX->GetEntries();
     std::cout<<"\n\n"<<keyString.c_str()<<"_Entries : "<<template_Entries<<"  scaleMC : "<<scaleMC<<std::endl;
   
     ofstream evtlistFile;
     if( keyStringT.Contains("Data") ) evtlistFile.open("evtlistData_aftAllCuts.txt");

     template_cntEventsWeighted =0; template_cntEventsWeightedSquared =0;
     template_sumpuWeight =0;
     template_cntNumEvents =0;
     template_cntAftBaselineMinusMETWeighted =0; template_cntAftBaselineMinusMETWeightedSquared =0;
     template_cntAftBaselineWeighted =0; template_cntAftBaselineWeightedSquared =0;
     template_cntAftPreCutsWeighted =0; template_cntAftPreCutsWeightedSquared =0;
     template_cntGoodFatJetPairsWeighted =0; template_cntGoodFatJetPairsWeightedSquared =0;
     template_cntGoodRemainingWmassWeighted =0; template_cntGoodRemainingWmassWeightedSquared =0;
     template_cntRemainingbPlusAJetWeighted =0; template_cntRemainingbPlusAJetWeightedSquared =0;
     template_cntRemainingbJetWeighted =0; template_cntRemainingbJetWeightedSquared =0;
     template_cntMT2Weighted =0; template_cntMT2WeightedSquared =0;
     template_cntmTbWeighted =0; template_cntmTbWeightedSquared =0;
     template_cntmTtWeighted =0; template_cntmTtWeightedSquared =0;
     template_cntmTWWeighted =0; template_cntmTWWeightedSquared =0;

     for(int ie=0; ie<template_Entries; ie++){

        template_AUX->GetEntry(ie);

        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(template_met, 0, template_metphi, 0);

        double puWeight = 1.0;
// Because signal is fastsim and does not have PU correctly done!
        if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
           puWeight = weightTruNPV(template_tru_npv);
        }

        int cntNJets = 0, cntNJetsPt30Eta24 =0, cntNJetsPt50Eta24 =0;
        for(unsigned int ij=0; ij<template_jetsLVec->size(); ij++){
           if( (*template_jetsLVec)[ij].Pt()>30 ){
              cntNJets ++;
           }
           if( (*template_jetsLVec)[ij].Pt()>30 && fabs( (*template_jetsLVec)[ij].Eta() )<2.4 ){
              cntNJetsPt30Eta24 ++;
           }
           if( (*template_jetsLVec)[ij].Pt()>50 && fabs( (*template_jetsLVec)[ij].Eta() )<2.4 ){
              cntNJetsPt50Eta24 ++;
           }
        }

        if( cntNJets < 6 ) continue;

        double newht=0;
        for(unsigned int ij=0; ij<template_jetsLVec->size(); ij++){ if( (*template_jetsLVec)[ij].Pt()>30 ) newht += (*template_jetsLVec)[ij].Pt(); }
        double dPhi0 = 999.0, dPhi1 = 999., dPhi2 = 999.;
        int cntPassJet = 0;
        for(unsigned int ij=0; ij<template_jetsLVec->size(); ij++){
           if( (*template_jetsLVec)[ij].Pt()>30 && fabs( (*template_jetsLVec)[ij].Eta() )<4.7 ){
           cntPassJet ++;
              if( cntPassJet ==1 ) dPhi0 = fabs(TVector2::Phi_mpi_pi((*template_jetsLVec)[ij].Phi() - template_metphi));
              if( cntPassJet ==2 ) dPhi1 = fabs(TVector2::Phi_mpi_pi((*template_jetsLVec)[ij].Phi() - template_metphi));
              if( cntPassJet ==3 ) dPhi2 = fabs(TVector2::Phi_mpi_pi((*template_jetsLVec)[ij].Phi() - template_metphi));
           }
        }
        bool passTriggerJets = true;
        cntPassJet =0;
        for(unsigned int ij=0; ij<template_jetsLVec->size(); ij++){
           if( (*template_jetsLVec)[ij].Pt()>30 ){
              cntPassJet ++;
              if( cntPassJet <=2 ){
                 if( (*template_jetsLVec)[ij].Pt() < 70 ) passTriggerJets = false;
              }
           }
        }
        if( cntPassJet < 2 ) passTriggerJets = false;
   
        int cntCSVS = 0;
        double minDeltaPhiMETb = 999.0;
        for(unsigned int ib=0; ib<template_jetsBtagCSVS->size(); ib++){
           if( fabs( (*template_jetsLVec)[ib].Eta() ) > 2.4 || (*template_jetsLVec)[ib].Pt() < 30 ) continue;
           if( (*template_jetsBtagCSVS)[ib] > cutCSVS ) cntCSVS ++;
           double deltaPhi = fabs(TVector2::Phi_mpi_pi((*template_jetsLVec)[ib].Phi() - template_metphi));
           if( minDeltaPhiMETb > deltaPhi ) minDeltaPhiMETb = deltaPhi;
        }

        if( doExtraCuts_ ){
           if( cntNJetsPt30Eta24 < 5 ) continue;
           if( cntNJetsPt50Eta24 < 3 ) continue;

           if( dPhi0 < 0.5 || dPhi1 < 0.5 || dPhi2 < 0.3 ) continue;
           if( !passTriggerJets ) continue;

           if( !cntCSVS ) continue;

           template_cntAftBaselineMinusMETWeighted += template_evtWeight * puWeight;
           template_cntAftBaselineMinusMETWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

           if( withMETcut && template_met < 175 ) continue;
//           if( withMETcut && template_met < 100 ) continue;

           if( applyLowMET ){
              if( minDeltaPhiMETb < 1.0 ) continue;
              if( ! ( (template_met>=175 && cntNJets >=7) || (template_met>=200 && cntNJets>=6) ) ) continue;
           }

           template_cntAftBaselineWeighted += template_evtWeight * puWeight;
           template_cntAftBaselineWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }

        template_cntAftPreCutsWeighted += template_evtWeight * puWeight;
        template_cntAftPreCutsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

        unsigned int nfatJetsInACone = template_allFatJetsInAConeLVec->size();

        int nTopJetCandCnt =0, nTopJetCandWithBtagCnt = 0;
        for(unsigned int ic=0; ic<nfatJetsInACone; ic++){

           const double fatJetm123 = (*template_allFatJetsInAConeLVec)[ic].M();
           const double fatJetm12 = (*template_allSubStruc12LVec)[ic].M();
           const double fatJetm13 = (*template_allSubStruc13LVec)[ic].M();
           const double fatJetm23 = (*template_allSubStruc23LVec)[ic].M();

           double bestmxy = fatJetm12;
           if( fabs(bestmxy - mW_) > fabs(fatJetm13 - mW_) ) bestmxy = fatJetm13;
           if( fabs(bestmxy - mW_) > fabs(fatJetm23 - mW_) ) bestmxy = fatJetm23;
   
           int isTopJet = 0;
           int statusCat00 = (*template_allFatJetsPassCat00Status)[ic];
           int statusCat01 = (*template_allFatJetsPassCat01Status)[ic];
           int statusCat02 = (*template_allFatJetsPassCat02Status)[ic];
           if( statusCat00 ==1 || statusCat01 ==1 || statusCat02 ==1 ) isTopJet =1;
        
           if( isTopJet ){
              template_h1_topJetPt_aftConeAndCats->Fill( (*template_allFatJetsInAConeLVec)[ic].Pt(), template_evtWeight * puWeight * scaleMC );
              nTopJetCandCnt ++;
           }
   
           int remainingPassCSVS = (*template_allFatJetsRemainingPassCSVS)[ic];
           if( isTopJet && remainingPassCSVS ) nTopJetCandWithBtagCnt++;
   
           template_h1_mfatJet_aftCone->Fill( fatJetm123, template_evtWeight * puWeight * scaleMC );
           template_h1_m12_aftCone->Fill( fatJetm12, template_evtWeight * puWeight * scaleMC );
           template_h1_m13_aftCone->Fill( fatJetm13, template_evtWeight * puWeight * scaleMC );
           template_h1_m23_aftCone->Fill( fatJetm23, template_evtWeight * puWeight * scaleMC );
           if( statusCat00 ){
              template_h1_m12_aftConeAndCat00->Fill( fatJetm12, template_evtWeight * puWeight * scaleMC );
              template_h1_m13_aftConeAndCat00->Fill( fatJetm13, template_evtWeight * puWeight * scaleMC );
              template_h1_m23_aftConeAndCat00->Fill( fatJetm23, template_evtWeight * puWeight * scaleMC );
           }
           if( statusCat01 ){
              template_h1_m12_aftConeAndCat01->Fill( fatJetm12, template_evtWeight * puWeight * scaleMC );
              template_h1_m13_aftConeAndCat01->Fill( fatJetm13, template_evtWeight * puWeight * scaleMC );
              template_h1_m23_aftConeAndCat01->Fill( fatJetm23, template_evtWeight * puWeight * scaleMC );
           }
           if( statusCat02 ){
              template_h1_m12_aftConeAndCat02->Fill( fatJetm12, template_evtWeight * puWeight * scaleMC );
              template_h1_m13_aftConeAndCat02->Fill( fatJetm13, template_evtWeight * puWeight * scaleMC );
              template_h1_m23_aftConeAndCat02->Fill( fatJetm23, template_evtWeight * puWeight * scaleMC );
           }
           if( keyStringT.Contains("QCDFlat") ){ 
              template_h2_m23overm123vsarctanm13overm12->Fill( TMath::ATan(fatJetm13/fatJetm12), fatJetm23/fatJetm123, scaleMC );
           }else{
              template_h2_m23overm123vsarctanm13overm12->Fill( TMath::ATan(fatJetm13/fatJetm12), fatJetm23/fatJetm123, template_evtWeight * puWeight * scaleMC );
           }
        }
        template_h1_nCombInACone->Fill(nfatJetsInACone, template_evtWeight * puWeight * scaleMC );
        template_h1_nTopCand->Fill(nTopJetCandCnt, template_evtWeight * puWeight * scaleMC );
        template_h1_nTopCandWithBtag->Fill(nTopJetCandWithBtagCnt, template_evtWeight * puWeight * scaleMC );
        template_h1_met->Fill(template_met, template_evtWeight * puWeight * scaleMC );
   
        if( template_bestFatJetsPairIdx == -1 ) continue;
   
        TLorentzVector bestTopJetLVec = (*template_allFatJetsInAConeLVec)[template_bestFatJetsPairIdx];
        double bestTopJetMass = bestTopJetLVec.M();
        if( fabs(bestTopJetMass - template_bestTopJetMass) > 1e-06 ) std::cout<<"WARNING ... difference between bestTopJetMass : "<<bestTopJetMass<<"  and  template_bestTopJetMass : "<<template_bestTopJetMass<<std::endl;
        const double fatJetm12 = (*template_allSubStruc12LVec)[template_bestFatJetsPairIdx].M();
        const double fatJetm13 = (*template_allSubStruc13LVec)[template_bestFatJetsPairIdx].M();
        const double fatJetm23 = (*template_allSubStruc23LVec)[template_bestFatJetsPairIdx].M();
   
        TLorentzVector bestbJetLVec, bestWJetLVec;
        double bestmxy = fatJetm12; bestWJetLVec = (*template_allSubStruc12LVec)[template_bestFatJetsPairIdx];
        if( fabs(bestmxy - mW_) > fabs(fatJetm13 - mW_) ){ bestmxy = fatJetm13; bestWJetLVec = (*template_allSubStruc13LVec)[template_bestFatJetsPairIdx]; }
        if( fabs(bestmxy - mW_) > fabs(fatJetm23 - mW_) ){ bestmxy = fatJetm23; bestWJetLVec = (*template_allSubStruc23LVec)[template_bestFatJetsPairIdx]; }
        bestbJetLVec = bestTopJetLVec - bestWJetLVec; 
   
        template_h1_mbestTopJet_aftConeAndCats->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_mbestWJet_aftConeAndCats->Fill( bestmxy, template_evtWeight * puWeight * scaleMC ); 
   
        if( template_met > 100 ){
           template_h1_mbestTopJet_aftConeAndCats_aftMET->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_mbestWJet_aftConeAndCats_aftMET->Fill( bestmxy, template_evtWeight * puWeight * scaleMC );
        }
        if( bestTopJetLVec.Pt() > 100 ){
           template_h1_mbestTopJet_aftConeAndCats_aftPt->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_mbestWJet_aftConeAndCats_aftPt->Fill( bestmxy, template_evtWeight * puWeight * scaleMC );
        }
   
        template_h2_bestmTopJetvsbestmWJet->Fill( bestmxy, bestTopJetMass, template_evtWeight * puWeight * scaleMC );
   
        if( template_pickedRemainingCombfatJetIdx == -1 ) continue;
   
        if( ! (template_bestTopJetMass > lowTopCut_ && template_bestTopJetMass < highTopCut_ ) ) continue;
     
   //     if( template_pickedbTaggedJetIdxVec->size() >=2 ) continue;
   //     if( template_pickedbTaggedbestTopJetIdxVec->size() >=2 ) continue;
   
        vector<int> bestTopJetInCombIdx = template_finalCombfatJets->at(template_bestFatJetsPairIdx);
        vector<int> pickedbJetsInCombIdxVec;
        for( unsigned itc=0; itc<bestTopJetInCombIdx.size(); itc++ ){
           for(unsigned int ibt=0; ibt<template_pickedbTaggedbestTopJetIdxVec->size(); ibt++){
              if( template_pickedbTaggedbestTopJetIdxVec->at(ibt) == bestTopJetInCombIdx[itc] ){
                 pickedbJetsInCombIdxVec.push_back(bestTopJetInCombIdx[itc]);
              }
           }
        }
        
        if( !pickedbJetsInCombIdxVec.empty() ){
           double minDeltamxy = 1e06; int minDeltamxyIdx = -1;
           for(unsigned int ipc=0; ipc<pickedbJetsInCombIdxVec.size(); ipc++){
              if( pickedbJetsInCombIdxVec[ipc] == bestTopJetInCombIdx[0] ){
                 if( minDeltamxy > fabs(fatJetm23-mW_) ){ minDeltamxy = fabs(fatJetm23-mW_); minDeltamxyIdx = ipc; }
              }
              if( pickedbJetsInCombIdxVec[ipc] == bestTopJetInCombIdx[1] ){
                 if( minDeltamxy > fabs(fatJetm13-mW_) ){ minDeltamxy = fabs(fatJetm13-mW_); minDeltamxyIdx = ipc; }
              }
              if( pickedbJetsInCombIdxVec[ipc] == bestTopJetInCombIdx[2] ){
                 if( minDeltamxy > fabs(fatJetm12-mW_) ){ minDeltamxy = fabs(fatJetm12-mW_); minDeltamxyIdx = ipc; }
              }
           }
        
           for(unsigned int itc=0; itc<bestTopJetInCombIdx.size(); itc++){
              if( pickedbJetsInCombIdxVec[minDeltamxyIdx] == bestTopJetInCombIdx[itc] ){
                 if( itc ==0 ){ bestWJetLVec = (*template_allSubStruc23LVec)[template_bestFatJetsPairIdx]; }
                 if( itc ==1 ){ bestWJetLVec = (*template_allSubStruc13LVec)[template_bestFatJetsPairIdx]; }
                 if( itc ==2 ){ bestWJetLVec = (*template_allSubStruc12LVec)[template_bestFatJetsPairIdx]; }
              }
           }
           bestbJetLVec = bestTopJetLVec - bestWJetLVec;
        }
   
        template_h1_remainingmTopJet_aftConeAndCats->Fill( template_pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_remainingmWJet_aftConeAndCats->Fill( template_pickedRemainingmxy, template_evtWeight * puWeight * scaleMC );
        template_h2_remainingmTopJetvsremainingmWJet->Fill( template_pickedRemainingmxy, template_pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
   
   //     if( template_pickedRemainingmxy > lowWCut_ && template_pickedRemainingmxy < highWCut_ && ! (template_pickedRemainingfatJetMass > lowTopCut_ && template_pickedRemainingfatJetMass < highTopCut_) ) continue;
   
        template_h1_remainingnbJet->Fill(template_pickedbTaggedJetIdxVec->size(), template_evtWeight * puWeight * scaleMC );
        template_h1_bestnbJet->Fill(template_pickedbTaggedbestTopJetIdxVec->size(), template_evtWeight * puWeight * scaleMC );
        template_h1_totalnbJet->Fill(template_pickedbTaggedJetIdxVec->size()+template_pickedbTaggedbestTopJetIdxVec->size(), template_evtWeight * puWeight * scaleMC );
   
        TLorentzVector pickedRemainingbJetPlusAJetLVec = template_pickedRemainingbJetPlusAJetLVec->front();
        TLorentzVector pickedRemainingbJetLVec = template_pickedRemainingbJetLVec->front();
   
        TLorentzVector closeToMETbJetLVec = template_closeToMETbJetLVec->front();
        
        double mTbestTopJet = sqrt( bestTopJetMass*bestTopJetMass + 2*( template_met*sqrt(bestTopJetMass*bestTopJetMass + bestTopJetLVec.Pt()*bestTopJetLVec.Pt()) -( template_met*cos(template_metphi)*bestTopJetLVec.Px() + template_met*sin(template_metphi)*bestTopJetLVec.Py() ) ) );
        if( fabs(mTbestTopJet - template_mTbestTopJet) > 1e-06 ) std::cout<<"WARNING ... difference between mTbestTopJet : "<<mTbestTopJet<<"  and  template_mTbestTopJet : "<<template_mTbestTopJet<<std::endl;
   
        double mTbJet = sqrt( closeToMETbJetLVec.M()*closeToMETbJetLVec.M() + 2*(template_met*sqrt(closeToMETbJetLVec.M()*closeToMETbJetLVec.M() + closeToMETbJetLVec.Pt()*closeToMETbJetLVec.Pt()) - (template_met*cos(template_metphi)*closeToMETbJetLVec.Px() + template_met*sin(template_metphi)*closeToMETbJetLVec.Py()) ));
   
        if( template_pickedClosestJetTobIdx != -1 && template_pickedRemainingCombfatJetbJetIdx == template_closebJetIdx ){
           mTbJet = sqrt( pickedRemainingbJetPlusAJetLVec.M()*pickedRemainingbJetPlusAJetLVec.M() + 2*( template_met*sqrt(pickedRemainingbJetPlusAJetLVec.M()*pickedRemainingbJetPlusAJetLVec.M() + pickedRemainingbJetPlusAJetLVec.Pt()*pickedRemainingbJetPlusAJetLVec.Pt()) - (template_met*cos(template_metphi)*pickedRemainingbJetPlusAJetLVec.Px() + template_met*sin(template_metphi)*pickedRemainingbJetPlusAJetLVec.Py()) ) );
        }
        if( fabs(mTbJet - template_mTbJet) > 1e-06 ) std::cout<<"WARNING ... difference between mTbJet : "<<mTbJet<<"  and  template_mTbJet : "<<template_mTbJet<<std::endl;
   
        double mTbestWJet = sqrt( bestWJetLVec.M()*bestWJetLVec.M() + 2*( template_met*sqrt(bestWJetLVec.M()*bestWJetLVec.M()+ bestWJetLVec.Pt()*bestWJetLVec.Pt()) -( template_met*cos(template_metphi)*bestWJetLVec.Px() + template_met*sin(template_metphi)*bestWJetLVec.Py() ) ) );
        if( fabs(mTbestWJet - template_mTbestWJet) > 1e-06 ) std::cout<<"WARNING ... difference between mTbestWJet : "<<mTbestWJet<<"  and  template_mTbestWJet : "<<template_mTbestWJet<<std::endl;
   
        double mTbestbJet = sqrt( bestbJetLVec.M()*bestbJetLVec.M() + 2*( template_met*sqrt(bestbJetLVec.M()*bestbJetLVec.M()+ bestbJetLVec.Pt()*bestbJetLVec.Pt()) -( template_met*cos(template_metphi)*bestbJetLVec.Px() + template_met*sin(template_metphi)*bestbJetLVec.Py() ) ) );
        if( fabs(mTbestbJet - template_mTbestbJet) > 1e-06 ) std::cout<<"WARNING ... difference between mTbestbJet : "<<mTbestbJet<<"  and  template_mTbestbJet : "<<template_mTbestbJet<<std::endl;
   
        TLorentzVector remainingTopJetLVec = (*template_remainingAllFatJetsCombLVec)[template_pickedRemainingCombfatJetIdx];
        double mTremainingTopJet = sqrt( remainingTopJetLVec.M()*remainingTopJetLVec.M() + 2*( template_met*sqrt(remainingTopJetLVec.M()*remainingTopJetLVec.M() + remainingTopJetLVec.Pt()*remainingTopJetLVec.Pt()) -( template_met*cos(template_metphi)*remainingTopJetLVec.Px() + template_met*sin(template_metphi)*remainingTopJetLVec.Py() ) ) );
        if( fabs(mTremainingTopJet - template_mTremainingTopJet) > 1e-06 ) std::cout<<"WARNING ... difference between mTremainingTopJet : "<<mTremainingTopJet<<"  and  template_mTremainingTopJet : "<<template_mTremainingTopJet<<std::endl;
   
        double MT2 = -1;
        if( template_pickedRemainingmxy < lowWCut_ || template_pickedRemainingmxy > highWCut_ ){
   // This is NOT a good W candidate & there is a jet associated with it --> possible b+tau(had) candidate
           if( template_pickedClosestJetTobIdx != -1 ){
              MT2 = calcMT2(bestTopJetLVec, pickedRemainingbJetPlusAJetLVec, metLVec);
           }else{
   // This is NOT a good W candidate & no other jet associated with it --> use only the b jet
              MT2 = calcMT2(bestTopJetLVec, pickedRemainingbJetLVec, metLVec);
           }
        }else{
   // We have a good W candidate --> use the remainingTopJetLVec
           MT2 = calcMT2(bestTopJetLVec, remainingTopJetLVec, metLVec);
        }
        if( fabs(MT2 - template_MT2) > 1e-06 ) std::cout<<"WARNING ... difference between MT2 : "<<MT2<<"  and  template_MT2 : "<<template_MT2<<std::endl;
   
        template_cntGoodFatJetPairsWeighted += template_evtWeight * puWeight;
        template_cntGoodFatJetPairsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

        if( template_pickedRemainingmxy < lowWCut_ || template_pickedRemainingmxy > highWCut_ ){
           if( template_pickedClosestJetTobIdx != -1 ){
              template_cntRemainingbPlusAJetWeighted += template_evtWeight * puWeight;
              template_cntRemainingbPlusAJetWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
           }else{
              template_cntRemainingbJetWeighted += template_evtWeight * puWeight;
              template_cntRemainingbJetWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
           }
        }else{
           template_cntGoodRemainingWmassWeighted += template_evtWeight * puWeight;
           template_cntGoodRemainingWmassWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

           template_h1_mTremainingTopJetWithGoodWmass->Fill( mTremainingTopJet, template_evtWeight * puWeight * scaleMC );
           template_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass->Fill( template_pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_remainingmWJet_aftConeAndCats_WithGoodWmass->Fill( template_pickedRemainingmxy, template_evtWeight * puWeight * scaleMC );
        }
   
        template_h1_met_aftGoodFatJetPairs->Fill(template_met, template_evtWeight * puWeight * scaleMC );
   
        if( MT2 > MT2cut_ ){
           template_cntMT2Weighted += template_evtWeight * puWeight;
           template_cntMT2WeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }
   
        if( (mTbJet + 0.5*mTbestTopJet)> mTbcut_ ){
           template_cntmTbWeighted += template_evtWeight * puWeight;
           template_cntmTbWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }
   
        if( mTbestTopJet > mTtcut_ ){
           template_cntmTtWeighted += template_evtWeight * puWeight;
           template_cntmTtWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }
   
        if( (mTbestbJet + mTbestWJet) > mTWcut_ ){
           template_cntmTWWeighted += template_evtWeight * puWeight;
           template_cntmTWWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }
   
        template_h1_mTremainingTopJet->Fill( mTremainingTopJet, template_evtWeight * puWeight * scaleMC );
   
        template_h1_mTbestTopJet->Fill( mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJet->Fill( mTbJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJetPlusmTbestTopJet->Fill( mTbJet + 0.5*mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h1_MT2->Fill( MT2, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbestWJet->Fill( mTbestWJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbestbJet->Fill( mTbestbJet, template_evtWeight * puWeight * scaleMC );
   
        template_h2_mTbestWvsmTbestb->Fill( mTbestbJet, mTbestWJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetvsmTbJet->Fill( mTbJet, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
   
   //     if( !( MT2 > MT2cut_ && (mTbJet + 0.5*mTbestTopJet)>mTbcut_ && mTbestTopJet > mTtcut_ && (mTbestbJet + mTbestWJet) > mTWcut_ ) ) continue;
   //     if( !( MT2 > MT2cut_ && (mTbJet + 0.5*mTbestTopJet)>mTbcut_ && (mTbestbJet + mTbestWJet) > mTWcut_ ) ) continue;
        if( !( MT2 > MT2cut_ && (mTbJet + 0.5*mTbestTopJet)>mTbcut_ ) ) continue;
   
        if( keyStringT.Contains("Data") ){
           char outStr[200];
           sprintf(outStr, "run: %6d  lumi : %4d  event : %10d  -->  MET : %8.3f  MT2 : %8.3f  mTb : %8.3f  mTt : %8.3f", template_run, template_lumi, template_event, template_met, MT2, mTbJet, mTbestTopJet);
           evtlistFile<<outStr<<std::endl;
        }
   
        template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestmxy, template_evtWeight * puWeight * scaleMC ); 
   
        template_h1_met_aftMTAndMT2Cuts->Fill(template_met, template_evtWeight * puWeight * scaleMC );
   
        template_h1_mTbestTopJet_aftMTAndMT2Cuts->Fill( mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJet_aftMTAndMT2Cuts->Fill( mTbJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts->Fill( mTbJet + 0.5*mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h1_MT2_aftMTAndMT2Cuts->Fill( MT2, template_evtWeight * puWeight * scaleMC );
   
        template_h1_mTremainingTopJet_aftMTAndMT2Cuts->Fill( mTremainingTopJet, template_evtWeight * puWeight * scaleMC );
   
        template_sumpuWeight += puWeight;
        template_cntEventsWeighted += template_evtWeight * puWeight;
        template_cntNumEvents ++;
   
        template_cntEventsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
     }
     if( keyStringT.Contains("Data") ) evtlistFile.close();

//     double template_cntEventsWeightedError = sqrt(template_cntEventsWeightedSquared);

     template_cntEventsWeightedScaledMC += template_cntEventsWeighted * scaleMC;
     template_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedSquared * scaleMC*scaleMC;

     template_sumpuWeightSumSample += template_sumpuWeight;
     template_cntNumEventsSumSample += template_cntNumEvents;

     template_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeighted * scaleMC; template_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedSquared * scaleMC*scaleMC;
     template_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeighted * scaleMC; template_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedSquared * scaleMC*scaleMC;
     template_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeighted * scaleMC; template_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedSquared * scaleMC*scaleMC;
     template_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeighted * scaleMC; template_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedSquared * scaleMC*scaleMC;
     template_cntGoodRemainingWmassWeightedScaledMC += template_cntGoodRemainingWmassWeighted * scaleMC; template_cntGoodRemainingWmassWeightedErrorScaledMC += template_cntGoodRemainingWmassWeightedSquared * scaleMC*scaleMC;
     template_cntRemainingbPlusAJetWeightedScaledMC += template_cntRemainingbPlusAJetWeighted * scaleMC; template_cntRemainingbPlusAJetWeightedErrorScaledMC += template_cntRemainingbPlusAJetWeightedSquared * scaleMC*scaleMC;
     template_cntRemainingbJetWeightedScaledMC += template_cntRemainingbJetWeighted * scaleMC; template_cntRemainingbJetWeightedErrorScaledMC += template_cntRemainingbJetWeightedSquared * scaleMC*scaleMC;
     template_cntMT2WeightedScaledMC +=template_cntMT2Weighted * scaleMC; template_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedSquared * scaleMC*scaleMC;
     template_cntmTbWeightedScaledMC +=template_cntmTbWeighted * scaleMC; template_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedSquared * scaleMC*scaleMC;
     template_cntmTtWeightedScaledMC +=template_cntmTtWeighted * scaleMC; template_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedSquared * scaleMC*scaleMC;
     template_cntmTWWeightedScaledMC +=template_cntmTWWeighted * scaleMC; template_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedSquared * scaleMC*scaleMC;
  } 
  template_cntEventsWeightedErrorScaledMC = sqrt(template_cntEventsWeightedErrorScaledMC);
  template_cntAftBaselineMinusMETWeightedErrorScaledMC = sqrt(template_cntAftBaselineMinusMETWeightedErrorScaledMC);
  template_cntAftBaselineWeightedErrorScaledMC = sqrt(template_cntAftBaselineWeightedErrorScaledMC);
  template_cntAftPreCutsWeightedErrorScaledMC = sqrt(template_cntAftPreCutsWeightedErrorScaledMC);
  template_cntGoodFatJetPairsWeightedErrorScaledMC = sqrt(template_cntGoodFatJetPairsWeightedErrorScaledMC);
  template_cntGoodRemainingWmassWeightedErrorScaledMC = sqrt(template_cntGoodRemainingWmassWeightedErrorScaledMC);
  template_cntRemainingbPlusAJetWeightedErrorScaledMC = sqrt(template_cntRemainingbPlusAJetWeightedErrorScaledMC);
  template_cntRemainingbJetWeightedErrorScaledMC = sqrt(template_cntRemainingbJetWeightedErrorScaledMC);
  template_cntMT2WeightedErrorScaledMC = sqrt(template_cntMT2WeightedErrorScaledMC);
  template_cntmTbWeightedErrorScaledMC = sqrt(template_cntmTbWeightedErrorScaledMC);
  template_cntmTtWeightedErrorScaledMC = sqrt(template_cntmTtWeightedErrorScaledMC);
  template_cntmTWWeightedErrorScaledMC = sqrt(template_cntmTWWeightedErrorScaledMC);

//  std::cout<<keyString.c_str()<<"_cntNumEvents : "<<template_cntNumEventsSumSample<<"  "<<keyString.c_str()<<"_cntEventsWeighted : "<<template_cntEventsWeighted<<" +- "<<template_cntEventsWeightedError<<"  ==>  "<<keyString.c_str()<<"sumpuWeight : "<<template_sumpuWeightSumSample<<"  "<<keyString.c_str()<<"_avgpuWeight : "<<template_sumpuWeightSumSample/template_cntNumEventsSumSample<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntNumEvents : "<<template_cntNumEventsSumSample<<"  ==>  "<<sampleKeyString.c_str()<<"sumpuWeight : "<<template_sumpuWeightSumSample<<"  "<<sampleKeyString.c_str()<<"_avgpuWeight : "<<template_sumpuWeightSumSample/template_cntNumEventsSumSample<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntAftBaselineMinusMETWeightedScaledMC : "<<template_cntAftBaselineMinusMETWeightedScaledMC<<" +- "<<template_cntAftBaselineMinusMETWeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntAftBaselineWeightedScaledMC : "<<template_cntAftBaselineWeightedScaledMC<<" +- "<<template_cntAftBaselineWeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntAftPreCutsWeightedScaledMC : "<<template_cntAftPreCutsWeightedScaledMC<<" +- "<<template_cntAftPreCutsWeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntGoodFatJetPairsWeightedScaledMC : "<<template_cntGoodFatJetPairsWeightedScaledMC<<" +- "<<template_cntGoodFatJetPairsWeightedErrorScaledMC<<std::endl;
  std::cout<<"  --> "<<sampleKeyString.c_str()<<"_cntGoodRemainingWmassWeightedScaledMC : "<<template_cntGoodRemainingWmassWeightedScaledMC<<" +- "<<template_cntGoodRemainingWmassWeightedErrorScaledMC<<std::endl;
  std::cout<<"  --> "<<sampleKeyString.c_str()<<"_cntRemainingbPlusAJetWeightedScaledMC : "<<template_cntRemainingbPlusAJetWeightedScaledMC<<" +- "<<template_cntRemainingbPlusAJetWeightedErrorScaledMC<<std::endl;
  std::cout<<"  --> "<<sampleKeyString.c_str()<<"_cntRemainingbJetWeightedScaledMC : "<<template_cntRemainingbJetWeightedScaledMC<<" +- "<<template_cntRemainingbJetWeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntMT2WeightedScaledMC : "<<template_cntMT2WeightedScaledMC<<" +- "<<template_cntMT2WeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntmTbWeightedScaledMC : "<<template_cntmTbWeightedScaledMC<<" +- "<<template_cntmTbWeightedErrorScaledMC<<std::endl;
//  std::cout<<sampleKeyString.c_str()<<"_cntmTtWeightedScaledMC : "<<template_cntmTtWeightedScaledMC<<" +- "<<template_cntmTtWeightedErrorScaledMC<<std::endl;
//  std::cout<<sampleKeyString.c_str()<<"_cntmTWWeightedScaledMC : "<<template_cntmTWWeightedScaledMC<<" +- "<<template_cntmTWWeightedErrorScaledMC<<std::endl;
  std::cout<<sampleKeyString.c_str()<<"_cntEventsWeightedScaledMC : "<<template_cntEventsWeightedScaledMC<<" +- "<<template_cntEventsWeightedErrorScaledMC<<std::endl;


// Cache the histograms
  h1_topJetPt_aftConeAndCatsVec.push_back((TH1D*)template_h1_topJetPt_aftConeAndCats->Clone());

  h1_mfatJet_aftConeVec.push_back((TH1D*)template_h1_mfatJet_aftCone->Clone());
  h1_m12_aftConeVec.push_back((TH1D*)template_h1_m12_aftCone->Clone());
  h1_m13_aftConeVec.push_back((TH1D*)template_h1_m13_aftCone->Clone());
  h1_m23_aftConeVec.push_back((TH1D*)template_h1_m23_aftCone->Clone());

  h1_mbestTopJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats->Clone());
  h1_mbestWJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats->Clone());

  h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts->Clone());
  h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts->Clone());

  h1_mbestTopJet_aftConeAndCats_aftMETVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats_aftMET->Clone());
  h1_mbestWJet_aftConeAndCats_aftMETVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats_aftMET->Clone());

  h1_mbestTopJet_aftConeAndCats_aftPtVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats_aftPt->Clone());
  h1_mbestWJet_aftConeAndCats_aftPtVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats_aftPt->Clone());

  h1_remainingmTopJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_remainingmTopJet_aftConeAndCats->Clone());
  h1_remainingmWJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_remainingmWJet_aftConeAndCats->Clone());
  h1_remainingmTopJet_aftConeAndCats_WithGoodWmassVec.push_back((TH1D*)template_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass->Clone());
  h1_remainingmWJet_aftConeAndCats_WithGoodWmassVec.push_back((TH1D*)template_h1_remainingmWJet_aftConeAndCats_WithGoodWmass->Clone());
  h2_remainingmTopJetvsremainingmWJetVec.push_back((TH2D*)template_h2_remainingmTopJetvsremainingmWJet->Clone());

  h2_bestmTopJetvsbestmWJetVec.push_back((TH2D*)template_h2_bestmTopJetvsbestmWJet->Clone());

  h1_m12_aftConeAndCat00Vec.push_back((TH1D*)template_h1_m12_aftConeAndCat00->Clone());
  h1_m13_aftConeAndCat00Vec.push_back((TH1D*)template_h1_m13_aftConeAndCat00->Clone());
  h1_m23_aftConeAndCat00Vec.push_back((TH1D*)template_h1_m23_aftConeAndCat00->Clone());

  h1_m12_aftConeAndCat01Vec.push_back((TH1D*)template_h1_m12_aftConeAndCat01->Clone());
  h1_m13_aftConeAndCat01Vec.push_back((TH1D*)template_h1_m13_aftConeAndCat01->Clone());
  h1_m23_aftConeAndCat01Vec.push_back((TH1D*)template_h1_m23_aftConeAndCat01->Clone());

  h1_m12_aftConeAndCat02Vec.push_back((TH1D*)template_h1_m12_aftConeAndCat02->Clone());
  h1_m13_aftConeAndCat02Vec.push_back((TH1D*)template_h1_m13_aftConeAndCat02->Clone());
  h1_m23_aftConeAndCat02Vec.push_back((TH1D*)template_h1_m23_aftConeAndCat02->Clone());

  h2_m23overm123vsarctanm13overm12Vec.push_back((TH2D*)template_h2_m23overm123vsarctanm13overm12->Clone());

  h1_metVec.push_back((TH1D*)template_h1_met->Clone());
  h1_met_aftGoodFatJetPairsVec.push_back((TH1D*)template_h1_met_aftGoodFatJetPairs->Clone());
  h1_met_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_met_aftMTAndMT2Cuts->Clone());

  h1_nCombInAConeVec.push_back((TH1D*)template_h1_nCombInACone->Clone());
  h1_nTopCandVec.push_back((TH1D*)template_h1_nTopCand->Clone());
  h1_nTopCandWithBtagVec.push_back((TH1D*)template_h1_nTopCandWithBtag->Clone());

  h1_mTremainingTopJetVec.push_back((TH1D*)template_h1_mTremainingTopJet->Clone());
  h1_mTremainingTopJetWithGoodWmassVec.push_back((TH1D*)template_h1_mTremainingTopJetWithGoodWmass->Clone());

  h1_mTbestTopJetVec.push_back((TH1D*)template_h1_mTbestTopJet->Clone());
  h1_mTbJetVec.push_back((TH1D*)template_h1_mTbJet->Clone());
  h1_mTbJetPlusmTbestTopJetVec.push_back((TH1D*)template_h1_mTbJetPlusmTbestTopJet->Clone());
  h1_MT2Vec.push_back((TH1D*)template_h1_MT2->Clone());

  h1_mTbestTopJet_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mTbestTopJet_aftMTAndMT2Cuts->Clone());
  h1_mTbJet_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mTbJet_aftMTAndMT2Cuts->Clone());
  h1_mTbJetPlusmTbestTopJet_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts->Clone());
  h1_MT2_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_MT2_aftMTAndMT2Cuts->Clone());
  h1_mTremainingTopJet_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mTremainingTopJet_aftMTAndMT2Cuts->Clone());

  h1_mTbestWJetVec.push_back((TH1D*)template_h1_mTbestWJet->Clone());
  h1_mTbestbJetVec.push_back((TH1D*)template_h1_mTbestbJet->Clone());

  h1_remainingnbJetVec.push_back((TH1D*)template_h1_remainingnbJet->Clone());
  h1_bestnbJetVec.push_back((TH1D*)template_h1_bestnbJet->Clone());
  h1_totalnbJetVec.push_back((TH1D*)template_h1_totalnbJet->Clone());

  h2_mTbestWvsmTbestbVec.push_back((TH2D*)template_h2_mTbestWvsmTbestb->Clone());
  h2_mTbestTopJetvsmTbJetVec.push_back((TH2D*)template_h2_mTbestTopJetvsmTbJet->Clone());

}


void plotsONtopTaggerTree(){

// Initialize calcMT2 first to make the printout look better
  TLorentzVector dummyLVec; dummyLVec.SetPtEtaPhiE(0, 0, 0, 0);
  calcMT2(dummyLVec, dummyLVec, dummyLVec);

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  initMCscales();

  initPUinput("PUData_dist.root", "pileup");

  TF1 *fn_RminCat01 = new TF1("fn_RminCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RminCat01->SetParameter(0, Rmin_);
  TF1 *fn_RcenCat01 = new TF1("fn_RcenCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RcenCat01->SetParameter(0, mWoverTop_);
  TF1 *fn_RmaxCat01 = new TF1("fn_RmaxCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RmaxCat01->SetParameter(0, Rmax_);

  TF1 *fn_RminCat02 = new TF1("fn_RminCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RminCat02->SetParameter(0, Rmin_);
  TF1 *fn_RcenCat02 = new TF1("fn_RcenCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RcenCat02->SetParameter(0, mWoverTop_);
  TF1 *fn_RmaxCat02 = new TF1("fn_RmaxCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RmaxCat02->SetParameter(0, Rmax_);

  TF1 *fn_RminCat00 = new TF1("fn_RminCat00", "[0]", 0, 1.5); fn_RminCat00->SetParameter(0, Rmin_);
  TF1 *fn_RcenCat00 = new TF1("fn_RcenCat00", "[0]", 0, 1.5); fn_RcenCat00->SetParameter(0, mWoverTop_);
  TF1 *fn_RmaxCat00 = new TF1("fn_RmaxCat00", "[0]", 0, 1.5); fn_RmaxCat00->SetParameter(0, Rmax_);

/*
// Testing
  TFile *Test_File = new TFile("test/directStop.root");
  TTree *Test_AUX = (TTree*) Test_File->Get(treeStrT+"/AUX");
  templatePlotsFunc(Test_AUX, "Test");
*/
  std::vector<TTree*> treeVec;
  std::vector<std::string> subSampleKeysVec;

  double Data_cntAftBaselineMinusMETWeightedScaledLumi =0, Data_cntAftBaselineMinusMETWeightedErrorScaledLumi =0;
  double Data_cntAftBaselineWeightedScaledLumi =0, Data_cntAftBaselineWeightedErrorScaledLumi =0;
  double Data_cntAftPreCutsWeightedScaledLumi =0, Data_cntAftPreCutsWeightedErrorScaledLumi =0;
  double Data_cntGoodFatJetPairsWeightedScaledLumi =0, Data_cntGoodFatJetPairsWeightedErrorScaledLumi =0;
  double Data_cntMT2WeightedScaledLumi =0, Data_cntMT2WeightedErrorScaledLumi =0;
  double Data_cntmTbWeightedScaledLumi =0, Data_cntmTbWeightedErrorScaledLumi =0;
  double Data_cntmTtWeightedScaledLumi =0, Data_cntmTtWeightedErrorScaledLumi =0;
  double Data_cntmTWWeightedScaledLumi =0, Data_cntmTWWeightedErrorScaledLumi =0;
  double Data_cntEventsWeightedScaledLumi =0, Data_cntEventsWeightedErrorScaledLumi =0;

// Data
  TFile *Data_File = new TFile("Data/directStop_merged.root");
  TTree *Data_AUX = (TTree*) Data_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(Data_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Data");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Data");
 
  Data_cntAftBaselineMinusMETWeightedScaledLumi += template_cntAftBaselineMinusMETWeightedScaledMC; Data_cntAftBaselineMinusMETWeightedErrorScaledLumi += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  Data_cntAftBaselineWeightedScaledLumi += template_cntAftBaselineWeightedScaledMC; Data_cntAftBaselineWeightedErrorScaledLumi += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  Data_cntAftPreCutsWeightedScaledLumi += template_cntAftPreCutsWeightedScaledMC; Data_cntAftPreCutsWeightedErrorScaledLumi += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  Data_cntGoodFatJetPairsWeightedScaledLumi += template_cntGoodFatJetPairsWeightedScaledMC; Data_cntGoodFatJetPairsWeightedErrorScaledLumi += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  Data_cntMT2WeightedScaledLumi += template_cntMT2WeightedScaledMC; Data_cntMT2WeightedErrorScaledLumi += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  Data_cntmTbWeightedScaledLumi += template_cntmTbWeightedScaledMC; Data_cntmTbWeightedErrorScaledLumi += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  Data_cntmTtWeightedScaledLumi += template_cntmTtWeightedScaledMC; Data_cntmTtWeightedErrorScaledLumi += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  Data_cntmTWWeightedScaledLumi += template_cntmTWWeightedScaledMC; Data_cntmTWWeightedErrorScaledLumi += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  Data_cntEventsWeightedScaledLumi += template_cntEventsWeightedScaledMC; Data_cntEventsWeightedErrorScaledLumi += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

  Data_cntAftBaselineMinusMETWeightedErrorScaledLumi = sqrt(Data_cntAftBaselineMinusMETWeightedErrorScaledLumi);
  Data_cntAftBaselineWeightedErrorScaledLumi = sqrt(Data_cntAftBaselineWeightedErrorScaledLumi);
  Data_cntAftPreCutsWeightedErrorScaledLumi = sqrt(Data_cntAftPreCutsWeightedErrorScaledLumi);
  Data_cntGoodFatJetPairsWeightedErrorScaledLumi = sqrt(Data_cntGoodFatJetPairsWeightedErrorScaledLumi);
  Data_cntMT2WeightedErrorScaledLumi = sqrt(Data_cntMT2WeightedErrorScaledLumi);
  Data_cntmTbWeightedErrorScaledLumi = sqrt(Data_cntmTbWeightedErrorScaledLumi);
  Data_cntmTtWeightedErrorScaledLumi = sqrt(Data_cntmTtWeightedErrorScaledLumi);
  Data_cntmTWWeightedErrorScaledLumi = sqrt(Data_cntmTWWeightedErrorScaledLumi);
  Data_cntEventsWeightedErrorScaledLumi = sqrt(Data_cntEventsWeightedErrorScaledLumi);

// Signal x500_y100
  TFile *Signal_x500_y100_File = new TFile("xpar500_ypar100/directStop_merged.root");
  TTree *Signal_x500_y100_AUX = (TTree*) Signal_x500_y100_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(Signal_x500_y100_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_x500_y100");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_x500_y100");

// Signal x450_y150
  TFile *Signal_x450_y150_File = new TFile("xpar450_ypar150/directStop_merged.root");
  TTree *Signal_x450_y150_AUX = (TTree*) Signal_x450_y150_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(Signal_x450_y150_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_x450_y150");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_x450_y150");

// Signal x350_y50
  TFile *Signal_x350_y50_File = new TFile("xpar350_ypar50/directStop_merged.root");
  TTree *Signal_x350_y50_AUX = (TTree*) Signal_x350_y50_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(Signal_x350_y50_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_x350_y50");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_x350_y50");

  double sumSM_cntAftBaselineMinusMETWeightedScaledMC =0, sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC =0;
  double sumSM_cntAftBaselineWeightedScaledMC =0, sumSM_cntAftBaselineWeightedErrorScaledMC =0;
  double sumSM_cntAftPreCutsWeightedScaledMC =0, sumSM_cntAftPreCutsWeightedErrorScaledMC =0;
  double sumSM_cntGoodFatJetPairsWeightedScaledMC =0, sumSM_cntGoodFatJetPairsWeightedErrorScaledMC =0;
  double sumSM_cntMT2WeightedScaledMC =0, sumSM_cntMT2WeightedErrorScaledMC =0;
  double sumSM_cntmTbWeightedScaledMC =0, sumSM_cntmTbWeightedErrorScaledMC =0;
  double sumSM_cntmTtWeightedScaledMC =0, sumSM_cntmTtWeightedErrorScaledMC =0;
  double sumSM_cntmTWWeightedScaledMC =0, sumSM_cntmTWWeightedErrorScaledMC =0;
  double sumSM_cntEventsWeightedScaledMC =0, sumSM_cntEventsWeightedErrorScaledMC =0;

// TTJets
  TFile *TTJets_File = new TFile("TTJets_onSkims/directStopTagger/directStop_merged.root");
  TTree *TTJets_AUX = (TTree*) TTJets_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(TTJets_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("TTbar");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

// WJetsToLNu
  TFile *WJetsToLNu_File = new TFile("WJetsToLNu_onSkims/directStopTagger/directStop_merged.root");
  TTree *WJetsToLNu_AUX = (TTree*) WJetsToLNu_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(WJetsToLNu_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("WJetsToLNu");
  templatePlotsFunc(treeVec, subSampleKeysVec, "WJetsToLNu");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

// Zinv
  TFile *Zinv_400HTinf_File = new TFile("Zinv_onSkims/directStopTagger/directStop_merged.root");
  TTree *Zinv_400HTinf_AUX = (TTree*) Zinv_400HTinf_File->Get(treeStrT+"/AUX");
  TFile *Zinv_200HT400_File = new TFile("Zinv_onSkims/directStopTagger/HT200_400/directStop_merged.root");
  TTree *Zinv_200HT400_AUX = (TTree*) Zinv_200HT400_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(Zinv_400HTinf_AUX); treeVec.push_back(Zinv_200HT400_AUX);
  subSampleKeysVec.clear(); subSampleKeysVec.push_back("Zinv_400HTinf"); subSampleKeysVec.push_back("Zinv_200HT400");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Zinv");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

// QCDFlat
  TFile *QCDFlat_File = new TFile("QCDFlat_onSkims/directStopTagger/directStop_merged.root");
  TTree *QCDFlat_AUX = (TTree*) QCDFlat_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(QCDFlat_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("QCDFlat");
  templatePlotsFunc(treeVec, subSampleKeysVec, "QCDFlat");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;
/*
// DYJets
  TFile *DYJets_File = new TFile("DYJets/directStopTagger/directStop_merged.root");
  TTree *DYJets_AUX = (TTree*) DYJets_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(DYJets_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("DYJets");
  templatePlotsFunc(treeVec, subSampleKeysVec, "DYJets");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;
*/
/*
// T_t 
  TFile *T_t_File = new TFile("SingleTop_t-channel/directStopTagger/directStop_merged.root");
  TTree *T_t_AUX = (TTree*) T_t_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(T_t_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("T_t");
  templatePlotsFunc(treeVec, subSampleKeysVec, "T_t");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;
*/
// T_tW
  TFile *T_tW_File = new TFile("SingleTop_tW-channel/directStopTagger/directStop_merged.root");
  TTree *T_tW_AUX = (TTree*) T_tW_File->Get(treeStrT+"/AUX");
  treeVec.clear(); treeVec.push_back(T_tW_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("T_tW");
  templatePlotsFunc(treeVec, subSampleKeysVec, "T_tW");

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

  sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC = sqrt(sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC);
  sumSM_cntAftBaselineWeightedErrorScaledMC = sqrt(sumSM_cntAftBaselineWeightedErrorScaledMC);
  sumSM_cntAftPreCutsWeightedErrorScaledMC = sqrt(sumSM_cntAftPreCutsWeightedErrorScaledMC);
  sumSM_cntGoodFatJetPairsWeightedErrorScaledMC = sqrt(sumSM_cntGoodFatJetPairsWeightedErrorScaledMC);
  sumSM_cntMT2WeightedErrorScaledMC = sqrt(sumSM_cntMT2WeightedErrorScaledMC);
  sumSM_cntmTbWeightedErrorScaledMC = sqrt(sumSM_cntmTbWeightedErrorScaledMC);
  sumSM_cntmTtWeightedErrorScaledMC = sqrt(sumSM_cntmTtWeightedErrorScaledMC);
  sumSM_cntmTWWeightedErrorScaledMC = sqrt(sumSM_cntmTWWeightedErrorScaledMC);
  sumSM_cntEventsWeightedErrorScaledMC = sqrt(sumSM_cntEventsWeightedErrorScaledMC);

  std::cout<<std::endl;
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntAftBaselineMinusMET", sumSM_cntAftBaselineMinusMETWeightedScaledMC, sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC, Data_cntAftBaselineMinusMETWeightedScaledLumi, Data_cntAftBaselineMinusMETWeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntAftBaseline", sumSM_cntAftBaselineWeightedScaledMC, sumSM_cntAftBaselineWeightedErrorScaledMC, Data_cntAftBaselineWeightedScaledLumi, Data_cntAftBaselineWeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntAftPreCuts", sumSM_cntAftPreCutsWeightedScaledMC, sumSM_cntAftPreCutsWeightedErrorScaledMC, Data_cntAftPreCutsWeightedScaledLumi, Data_cntAftPreCutsWeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntGoodFatJetPairs", sumSM_cntGoodFatJetPairsWeightedScaledMC, sumSM_cntGoodFatJetPairsWeightedErrorScaledMC, Data_cntGoodFatJetPairsWeightedScaledLumi, Data_cntGoodFatJetPairsWeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntMT2", sumSM_cntMT2WeightedScaledMC, sumSM_cntMT2WeightedErrorScaledMC, Data_cntMT2WeightedScaledLumi, Data_cntMT2WeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntmTb", sumSM_cntmTbWeightedScaledMC, sumSM_cntmTbWeightedErrorScaledMC, Data_cntmTbWeightedScaledLumi, Data_cntmTbWeightedErrorScaledLumi);
//  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntmTt", sumSM_cntmTtWeightedScaledMC, sumSM_cntmTtWeightedErrorScaledMC, Data_cntmTtWeightedScaledLumi, Data_cntmTtWeightedErrorScaledLumi);
//  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntmTW", sumSM_cntmTWWeightedScaledMC, sumSM_cntmTWWeightedErrorScaledMC, Data_cntmTWWeightedScaledLumi, Data_cntmTWWeightedErrorScaledLumi);
  printf("%30s   sumSM : %10.5f +- %9.5f  data : %10.5f +- %9.5f\n", "cntEvents", sumSM_cntEventsWeightedScaledMC, sumSM_cntEventsWeightedErrorScaledMC, Data_cntEventsWeightedScaledLumi, Data_cntEventsWeightedErrorScaledLumi);

// Plotting

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

  tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

  tdrStyle->SetOptStat(1111);
 
  tdrStyle->SetHistLineWidth(1);

  unsigned int ntype = keyStringCachedVec.size();
  std::cout<<"\nntype : "<<ntype<<std::endl;

  TCanvas *cs = new TCanvas("cs", "cs", 1200, 800);
  int divW=4, divH=3;
  cs->Divide(divW, divH);

  TString extraDrawStrT = "normalization";

  cs->Print("allINone_"+treeStrT+".pdf[");
  draw2DallINone(cs, divW*divH, h2_m23overm123vsarctanm13overm12Vec); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mfatJet_aftConeVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nCombInAConeVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nTopCandVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nTopCandWithBtagVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_mbestTopJet_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestWJet_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_topJetPt_aftConeAndCatsVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_remainingmTopJet_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_remainingmWJet_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_remainingmTopJet_aftConeAndCats_WithGoodWmassVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_remainingmWJet_aftConeAndCats_WithGoodWmassVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_remainingnbJetVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_bestnbJetVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_totalnbJetVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_metVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_met_aftGoodFatJetPairsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_met_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_mTbestTopJetVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTbJetVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTbJetPlusmTbestTopJetVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_MT2Vec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTremainingTopJetVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTremainingTopJetWithGoodWmassVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_mTbestTopJet_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTbJet_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTbJetPlusmTbestTopJet_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_MT2_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mTremainingTopJet_aftMTAndMT2CutsVec, 4, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw2DallINone(cs, divW*divH, h2_mTbestWvsmTbestbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsmTbJetVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_bestmTopJetvsbestmWJetVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_remainingmTopJetvsremainingmWJetVec); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_m23_aftConeVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m23_aftConeAndCat00Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m23_aftConeAndCat01Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m23_aftConeAndCat02Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_m13_aftConeVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m13_aftConeAndCat00Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m13_aftConeAndCat01Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m13_aftConeAndCat02Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_m12_aftConeVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m12_aftConeAndCat00Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m12_aftConeAndCat01Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_m12_aftConeAndCat02Vec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  cs->Print("allINone_"+treeStrT+".pdf]");

//  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(2, 2);

  for(unsigned int it=0; it<ntype; it++){
     
     TString keyStringT(keyStringCachedVec[it]);
//     TObjArray * vlist = keyStringT.Tokenize("_");
//     TString sharedKeyT = dynamic_cast<TObjString*>(vlist->At(0))->GetString();
     
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf[");
     c1->cd(1); TPad *pad1 = (TPad*) c1->GetPad(1); pad1->Clear();
     h2_m23overm123vsarctanm13overm12Vec[it]->Draw("colz");
     TH1 *h1_RminCat00 = fn_RminCat00->GetHistogram(); h1_RminCat00->SetLineColor(kRed); h1_RminCat00->SetLineWidth(1); h1_RminCat00->Draw("same c");
     TH1 *h1_RcenCat00 = fn_RcenCat00->GetHistogram(); h1_RcenCat00->SetLineColor(kRed); h1_RcenCat00->SetLineWidth(1); h1_RcenCat00->Draw("same c");
     TH1 *h1_RmaxCat00 = fn_RmaxCat00->GetHistogram(); h1_RmaxCat00->SetLineColor(kRed); h1_RmaxCat00->SetLineWidth(1); h1_RmaxCat00->Draw("same c");
     TH1 *h1_RminCat01 = fn_RminCat01->GetHistogram(); h1_RminCat01->SetLineColor(kRed); h1_RminCat01->SetLineWidth(1); h1_RminCat01->Draw("same c");
     TH1 *h1_RcenCat01 = fn_RcenCat01->GetHistogram(); h1_RcenCat01->SetLineColor(kRed); h1_RcenCat01->SetLineWidth(1); h1_RcenCat01->Draw("same c");
     TH1 *h1_RmaxCat01 = fn_RmaxCat01->GetHistogram(); h1_RmaxCat01->SetLineColor(kRed); h1_RmaxCat01->SetLineWidth(1); h1_RmaxCat01->Draw("same c");
     TH1 *h1_RminCat02 = fn_RminCat02->GetHistogram(); h1_RminCat02->SetLineColor(kRed); h1_RminCat02->SetLineWidth(1); h1_RminCat02->Draw("same c");
     TH1 *h1_RcenCat02 = fn_RcenCat02->GetHistogram(); h1_RcenCat02->SetLineColor(kRed); h1_RcenCat02->SetLineWidth(1); h1_RcenCat02->Draw("same c");
     TH1 *h1_RmaxCat02 = fn_RmaxCat02->GetHistogram(); h1_RmaxCat02->SetLineColor(kRed); h1_RmaxCat02->SetLineWidth(1); h1_RmaxCat02->Draw("same c");
     c1->cd(2); TPad *pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
     h1_mfatJet_aftConeVec[it]->Draw("hist");
     c1->cd(3); TPad *pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
     c1->cd(4); TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");

     c1->cd(1); pad1->Clear();
     h1_nCombInAConeVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_nTopCandVec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_nTopCandWithBtagVec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_mbestTopJet_aftConeAndCatsVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_mbestWJet_aftConeAndCatsVec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_remainingmTopJet_aftConeAndCatsVec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_remainingmWJet_aftConeAndCatsVec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_mbestTopJet_aftConeAndCats_aftMETVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_mbestWJet_aftConeAndCats_aftMETVec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_mbestTopJet_aftConeAndCats_aftPtVec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_mbestWJet_aftConeAndCats_aftPtVec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_remainingnbJetVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_bestnbJetVec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_totalnbJetVec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_m23_aftConeVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_m23_aftConeAndCat00Vec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_m23_aftConeAndCat01Vec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_m23_aftConeAndCat02Vec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_m13_aftConeVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_m13_aftConeAndCat00Vec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_m13_aftConeAndCat01Vec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_m13_aftConeAndCat02Vec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->cd(1); pad1->Clear();
     h1_m12_aftConeVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_m12_aftConeAndCat00Vec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_m12_aftConeAndCat01Vec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_m12_aftConeAndCat02Vec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");

     c1->cd(1); pad1->Clear();
     h1_mTbestTopJetVec[it]->Rebin(2); h1_mTbestTopJetVec[it]->Draw("hist");
     c1->cd(2); pad2->Clear();
     h1_MT2Vec[it]->Rebin(2); h1_MT2Vec[it]->Draw("hist");
     c1->cd(3); pad3->Clear();
     h1_mTbJetVec[it]->Rebin(2); h1_mTbJetVec[it]->Draw("hist");
     c1->cd(4); pad4->Clear();
     h1_mTremainingTopJetVec[it]->Rebin(2); h1_mTremainingTopJetVec[it]->Draw("hist");
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");

     c1->cd(1); pad1->Clear();
     h2_mTbestWvsmTbestbVec[it]->Draw("colz"); 
     c1->cd(2); pad2->Clear();
     h2_mTbestTopJetvsmTbJetVec[it]->Draw("colz"); 
     c1->cd(3); pad3->Clear();
     h2_remainingmTopJetvsremainingmWJetVec[it]->Draw("colz");
     c1->cd(4); pad4->Clear();
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf");
     
     c1->Print("topTagger_"+keyStringT+"_"+treeStrT+".pdf]");

  }

}


double weightTruNPV(int trunpv){

   double w = 1.;
   if (trunpv < static_cast<int> (puWeights_.size())) {
      w = puWeights_.at(trunpv);
   } else {
      std::cerr << "WARNING ... number of PU vertices = " << trunpv
            << " out of histogram binning." << std::endl;
   }

   return w;
}


int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName){

   puWeights_.clear();

   TFile *puDatafile = new TFile(puDatafileName.c_str());
   TH1D *data_npu_estimated = (TH1D*) puDatafile->Get(puDatahistName.c_str());
 
// Distribution used for Summer2012 MC.
   Double_t Summer2012[60] = {
      2.344E-05,
      2.344E-05,
      2.344E-05,
      2.344E-05,
      4.687E-04,
      4.687E-04,
      7.032E-04,
      9.414E-04,
      1.234E-03,
      1.603E-03,
      2.464E-03,
      3.250E-03,
      5.021E-03,
      6.644E-03,
      8.502E-03,
      1.121E-02,
      1.518E-02,
      2.033E-02,
      2.608E-02,
      3.171E-02,
      3.667E-02,
      4.060E-02,
      4.338E-02,
      4.520E-02,
      4.641E-02,
      4.735E-02,
      4.816E-02,
      4.881E-02,
      4.917E-02,
      4.909E-02,
      4.842E-02,
      4.707E-02,
      4.501E-02,
      4.228E-02,
      3.896E-02,
      3.521E-02,
      3.118E-02,
      2.702E-02,
      2.287E-02,
      1.885E-02,
      1.508E-02,
      1.166E-02,
      8.673E-03,
      6.190E-03,
      4.222E-03,
      2.746E-03,
      1.698E-03,
      9.971E-04,
      5.549E-04,
      2.924E-04,
      1.457E-04,
      6.864E-05,
      3.054E-05,
      1.282E-05,
      5.081E-06,
      1.898E-06,
      6.688E-07,
      2.221E-07,
      6.947E-08,
      2.047E-08
   };

   puWeights_.resize(60);
   double s = 0.0;
   for (int npu = 0; npu < 60; ++npu) {
      double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));
      puWeights_[npu] = npu_estimated / Summer2012[npu];
      s += npu_estimated;
   }

   // normalize weights such that the total sum of weights over the whole sample is 1.0, i.e., sum_i  puWeights_[i] * Summer2012[i] should be 1.0 (!)
   for (int npu = 0; npu < 60; ++npu) {
      puWeights_[npu] /= s;
   }

   return 1;
}


double calcMT2(const TLorentzVector &fatJet1LVec, const TLorentzVector &fatJet2LVec, const TLorentzVector &metLVec){

  // For this example we will use the "Basic_Mt2_332_Calculator" which is
  // the algorithm we recommend people use by default.
//  Mt2::Basic_Mt2_332_Calculator mt2Calculator;
  Mt2::ChengHanBisect_Mt2_332_Calculator mt2Calculator;

  // Could tell the MT2 calculating object to be verbose, and print out
  // debug messages while it is thinking ... but we won't:

  // mt2Calculator.setDebug(true);

  // The input parameters associated with the particle
  // (or collection of particles) associated with the 
  // first "side" of the event:
  const double massOfSystemA =  fatJet1LVec.M(); // GeV
  const double pxOfSystemA   =  fatJet1LVec.Px(); // GeV
  const double pyOfSystemA   =  fatJet1LVec.Py(); // GeV

  // The input parameters associated with the particle
  // (or collection of particles) associated with the 
  // second "side" of the event:
  const double massOfSystemB =  fatJet2LVec.M(); // GeV
  const double pxOfSystemB   =  fatJet2LVec.Px(); // GeV
  const double pyOfSystemB   =  fatJet2LVec.Py(); // GeV

  // The missing transverse momentum:
  const double pxMiss        = metLVec.Px(); // GeV
  const double pyMiss        = metLVec.Py(); // GeV

  // The mass of the "inivisible" particle presumed to have
  // been produced at the end of the decay chain in each
  // "half" of the event:
  const double invis_mass    = metLVec.M(); // GeV
  // Now put the inputs together into the input structures that the library wants.

  /*
      Note: in the next two lines (constructing "vis_A" and "vis_B"),
      the ORDER of the arguments to the constructor of
      Mt2::LorentzTransverseVector is very important. 
      You need to be careful as, when the TwoVector comes
      first, the second arguments is taken to be a mass:
      
      LorentzTransverseVector(const TwoVector& momentum, double mass);
      
      but when the TwoVector comes second, the first arguemt is an ET=Sqrt(m^2+pt^2):
      
      LorentzTransverseVector(double Et, const TwoVector& momentum);
      
      You have been warned!
  */

  Mt2::LorentzTransverseVector  vis_A(Mt2::TwoVector(pxOfSystemA, pyOfSystemA), massOfSystemA);
  Mt2::LorentzTransverseVector  vis_B(Mt2::TwoVector(pxOfSystemB, pyOfSystemB), massOfSystemB);
  Mt2::TwoVector                pT_Miss(pxMiss, pyMiss);

  // Now that we have some visiable momenta and some missing transverse
  // momentum we can calculate MT2.

  const double mt2 = mt2Calculator.mt2_332(vis_A, vis_B, pT_Miss, invis_mass);

  return mt2; 
}


void draw2DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH2D*> &h2_inputVec){

  int cntPadIdx = 0;
  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){
     if( it == 0 ) cntPadIdx = 0;
     TString keyStringT(keyStringCachedVec[it]); 
     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" ) continue; 
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl; cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(0);
     h2_inputVec[it]->Draw("colz");
  }

  for(int ic=cntPadIdx+1; ic<=lastPadIdx; ic++){ cs->cd(ic); TPad * pad = (TPad*) cs->GetPad(ic); pad->Clear(); }
}


void draw1DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH1D*> &h1_inputVec, const int nRebin, const TString optDrawStr){
  
  int statusLogy = 0;
  if( optDrawStr.Contains("SetLogy") ) statusLogy =1;
  int doNormalization = 0;
  if( optDrawStr.Contains("normalization") ) doNormalization =1;

  int cntPadIdx = 0;
  vector<TH1D*> h1_stackedVec; TH1D * h1_dataCached =0, * h1_signal1Cached =0, *h1_signal2Cached =0;

  vector<TString> stackedKeyStringTVec;

  unsigned int ntype = keyStringCachedVec.size();

  for(unsigned int it=0; it<ntype; it++){

     TString keyStringT(keyStringCachedVec[it]); 

     if( it == 0 ){ cntPadIdx = 0; h1_stackedVec.clear(); }

     if( keyStringT.Contains("DYJets") || keyStringCachedVec[it] == "T_t" ) continue; 
     cntPadIdx ++;
     if( cntPadIdx >= lastPadIdx ){ std::cout<<"Overwritten the last pad with index : "<<lastPadIdx<<"! Please make sure enough pads are created!"<<std::endl; cntPadIdx = lastPadIdx; }

     cs->cd(cntPadIdx); TPad * pad = (TPad*) cs->GetPad(cntPadIdx); pad->Clear(); pad->SetLogy(statusLogy);

     TH1D *tmpHist = (TH1D*) h1_inputVec[it]->Clone(); 

     tmpHist->Rebin(nRebin); tmpHist->Scale(scaleMCCachedVec[it]); 

     tmpHist->SetLineColor(colorCachedVec[it]); tmpHist->SetMarkerColor(colorCachedVec[it]); tmpHist->SetMarkerStyle(6); tmpHist->SetMarkerSize(0.25);

     if( keyStringT.Contains("Data") ){ tmpHist->SetLineColor(kBlack); tmpHist->SetMarkerColor(kBlack); }

     if( !statusLogy ){
        tmpHist->SetAxisRange(0, tmpHist->GetMaximum()*1.5, "Y");
     }else{
        tmpHist->SetAxisRange(1e-1, tmpHist->GetMaximum()*32, "Y");
     }

     if( keyStringT.Contains("Data") || keyStringT.Contains("Signal") ){
        tmpHist->Draw("e");
     }else{
        tmpHist->Draw("hist");
     }
     
     if( !keyStringT.Contains("Data") && !keyStringT.Contains("Signal") ){
        stackedKeyStringTVec.push_back(keyStringT);
        if( h1_stackedVec.empty() ){
           h1_stackedVec.push_back( (TH1D*) tmpHist->Clone() ); 
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] ); 
        }else{
           TH1D *tmpSumHist = (TH1D*) tmpHist->Clone();
           tmpSumHist->Add(h1_stackedVec.back(), 1); 
           h1_stackedVec.push_back( (TH1D*) tmpSumHist->Clone() ); 
           h1_stackedVec.back()->SetFillColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetLineColor( colorCachedVec[it] ); 
           h1_stackedVec.back()->SetMarkerColor( colorCachedVec[it] ); 
        }
     }
     if( keyStringT.Contains("Data") ){ h1_dataCached = (TH1D*) tmpHist->Clone(); }
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
/*
  if( !statusLogy ){
     h1_dataCached->SetAxisRange(0, h1_dataCached->GetMaximum()*1.5, "Y");
  }else{
     h1_dataCached->SetAxisRange(1e-1, h1_dataCached->GetMaximum()*32, "Y");
  }
*/
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
}
