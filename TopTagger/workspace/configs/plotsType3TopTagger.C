#include "tophead.h"
#include "tdrstyle.C"
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"
#include "Type3TopTagger.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<vector<int> > >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#endif

using namespace std;

topTagger::type3TopTagger * type3Ptr;

const bool doHepTopTagger = false;

const bool doMoreSignals = true;
// They are percentage errors
const double sysErrBkg = 0.15, sysErrSig = 0.10;
const int nStepmStop = 25, nStepmLSP = 25;
//const int minmStop = 225, maxmStop = 1200, minmLSP = 0, maxmLSP = 1000;
const int minmStop = 200, maxmStop = 900, minmLSP = 0, maxmLSP = 400;
const int nStopPts = (maxmStop-minmStop)/nStepmStop + 1, nLSPPts = (maxmLSP-minmLSP)/nStepmLSP + 1;

const bool doExtraCuts_ = true;

const double cutCSVS = 0.679;
//const double cutCSVS = 0.898;
const bool applyLowMET = false;
const bool withMETcut = true;
const double defaultMETcut = 175;
//const double defaultMETcut = 350;

const double mTbcut_ = 500, mTtcut_ = 365, MT2cut_ = 300, mTWcut_ = 600; // baseline Kin
//const double mTbcut_ = 600, mTtcut_ = 365, MT2cut_ = 200, mTWcut_ = 600; // new highMET Kin

const bool doInvertTopCriteria = false;
//const bool doInvertTopCriteria = true;

const bool doTopMassCut = true;

static const int nJetsSel = 5, nJetsSelPt30Eta24 = 5, nJetsSelPt50Eta24 = 4, nJetsSelPt70Eta24 = 2;
static const int nJetsSelBtagged = 1;
static const double minJetPt = 30;
//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta24Arr[] = {   -1,       2.4,      30,    -1  };
static const double pt50Eta24Arr[] = {   -1,       2.4,      50,    -1  };
static const double pt70Eta24Arr[] = {   -1,       2.4,      70,    -1  };
static const double      dphiArr[] = {   -1,       4.7,      30,    -1  };
static const double      bTagArr[] = {   -1,       2.4,      30,    -1  };

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt);

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS = 0.679, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi = 3, const double minAbsEta = -1, const double maxAbsEta = 4.7, const double minPt = 30, const double maxPt = -1);

bool passOriKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut);
bool passBaselineFunc(const double met, const double MT2, const double mTt, const double mTb);
bool passHighMETFunc(const double met, const double MT2, const double mTt, const double mTb);
bool passNewKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut);
bool passNewHighMETFunc(const double met, const double MT2, const double mTt, const double mTb);

TRandom3 *picker =0;
const double maxDRtopTaggerMatch = 0.4;
const bool doEffMeasure = false;

const double simuCAdeltaR = 1.5;

const TString treeStrT = "RA2TreeMaker"; TString extraDrawStrT = "";
//const TString treeStrT = "histAndTree_recoLevelSingleMuSel"; TString extraDrawStrT = "normalization";

const string      mcStr[] = {   "TTbar", "WJetsToLNu_400HTinf", "WJetsToLNu_300HT400", "WJetsToLNu_250HT300", "Zinv_400HTinf", "Zinv_200HT400",   "DYJets",    "QCD_1000HTinf",  "QCD_500HT1000", "QCD_250HT500", "T_t",    "T_tW",    
                                "Signal_x500_y100",       "Signal_x450_y150",        "Signal_x350_y50",          "Signal_x500_y50", 
                                "Signal_mStop250_mLSP0",  "Signal_mStop250_mLSP50",
                                "Signal_mStop300_mLSP0",  "Signal_mStop300_mLSP50",  "Signal_mStop300_mLSP100",
                                "Signal_mStop350_mLSP0",  "Signal_mStop350_mLSP50",  "Signal_mStop350_mLSP100",  "Signal_mStop350_mLSP150",
                                "Signal_mStop400_mLSP150","Signal_mStop400_mLSP200",
                                "Signal_mStop450_mLSP0",  "Signal_mStop450_mLSP50",  "Signal_mStop450_mLSP100",  "Signal_mStop450_mLSP150",  "Signal_mStop450_mLSP200",  "Signal_mStop450_mLSP250",
                                "Signal_mStop500_mLSP0",  "Signal_mStop500_mLSP50",  "Signal_mStop500_mLSP100",  "Signal_mStop500_mLSP150",  "Signal_mStop500_mLSP200",  "Signal_mStop500_mLSP250",
                                "Signal_mStop550_mLSP0",  "Signal_mStop550_mLSP50",  "Signal_mStop550_mLSP100",  "Signal_mStop550_mLSP150",  "Signal_mStop550_mLSP200",  "Signal_mStop550_mLSP250",
                                "Signal_mStop600_mLSP0",  "Signal_mStop600_mLSP50",  "Signal_mStop600_mLSP100",  "Signal_mStop600_mLSP150",  "Signal_mStop600_mLSP200",  "Signal_mStop600_mLSP250",
                                "Signal_mStop650_mLSP0",  "Signal_mStop650_mLSP50",  "Signal_mStop650_mLSP100",  "Signal_mStop650_mLSP150",  "Signal_mStop650_mLSP200",  "Signal_mStop650_mLSP250",
                                "Signal_mStop700_mLSP0",  "Signal_mStop700_mLSP50",  "Signal_mStop700_mLSP100",  "Signal_mStop700_mLSP150",  "Signal_mStop700_mLSP200",  "Signal_mStop700_mLSP250",
                                "Signal_mStop750_mLSP0",  "Signal_mStop750_mLSP50",  "Signal_mStop750_mLSP100",  "Signal_mStop750_mLSP150",  "Signal_mStop750_mLSP200",  "Signal_mStop750_mLSP250",
                                "Signal_mStop850_mLSP0",  "Signal_mStop850_mLSP50",  "Signal_mStop850_mLSP100",  "Signal_mStop850_mLSP150",  "Signal_mStop850_mLSP200",  "Signal_mStop850_mLSP250" };
const double    xSecArr[] = {       234,     30.08,             45.68,            57.26,            6.26,           49.28,        3532.8149,          204,             8426,          276000,      47.0,  11.1773*2.0,    
                                        0.0855847,                0.169668,                   0.807323,                0.0855847,
/*mStop250*/                            5.57596,                  5.57596,
/*mStop300*/                            1.99608,                  1.99608,                    1.99608,
/*mStop350*/                            0.807323,                 0.807323,                   0.807323,                0.807323,
/*mStop400*/                            0.35683,                  0.35683,
/*mStop450*/                            0.169668,                 0.169668,                   0.169668,                0.169668,                    0.169668,                     0.169668,
/*mStop500*/                            0.0855847,                0.0855847,                  0.0855847,               0.0855847,                   0.0855847,                    0.0855847,
/*mStop550*/                            0.0452067,                0.0452067,                  0.0452067,               0.0452067,                   0.0452067,                    0.0452067,
/*mStop600*/                            0.0248009,                0.0248009,                  0.0248009,               0.0248009,                   0.0248009,                    0.0248009,      
/*mStop650*/                            0.0139566,                0.0139566,                  0.0139566,               0.0139566,                   0.0139566,                    0.0139566,
/*mStop700*/                            0.0081141,                0.0081141,                  0.0081141,               0.0081141,                   0.0081141,                    0.0081141,
/*mStop750*/                            0.00480639,               0.00480639,                 0.00480639,              0.00480639,                  0.00480639,                   0.00480639,
/*mStop850*/                            0.00176742,               0.00176742,                 0.00176742,              0.00176742,                  0.00176742,                   0.00176742 };
const int       nEvtArr[] = {   21671578,   6619641,           1699483,           4938508,        5095696,         5055873,        1000000,        13829995,         30599239,       27002490,     23777,    497657,          
                                        98720,                    49996,                      99992,                   99993,
/*mStop250*/                            49996,                    49995,
/*mStop300*/                            49998,                    49994,                      49997,
/*mStop350*/                            49996,                    49998,                      49994,                   49995,
/*mStop400*/                            49998,                    49994,
/*mStop450*/                            49998,                    49991,                      49998,                   49996,                         49998,                        49994,
/*mStop500*/                            49998,                    49995,                      49996,                   49995,                         49997,                        49996,
/*mStop550*/                            49996,                    49993,                      49997,                   49996,                         50000,                        49997,
/*mStop600*/                            49995,                    49996,                      49993,                   49994,                         49998,                        49999,
/*mStop650*/                            49995,                    49995,                      49996,                   49998,                         49997,                        49995,
/*mStop700*/                            49998,                    49995,                      49998,                   49997,                         49996,                        49996,
/*mStop750*/                            49997,                    49998,                      49997,                   49995,                         49996,                        49997,
/*mStop850*/                            49996,                    49998,                      49994,                   49999,                         49998,                        49999 };
static const int colors[] = {    kGreen,      kMagenta+1,            kMagenta+1,            kMagenta+1,           kTeal+4,          kTeal+4,       kYellow-7,      kBlue,            kBlue,          kBlue,      kYellow,   kGreen+2,    
                                        kRed,                     kRed,                       kRed,                    kRed,
/*mStop250*/                            kRed,                     kRed,
/*mStop300*/                            kRed,                     kRed,                       kRed,
/*mStop350*/                            kRed,                     kRed,                       kRed,                    kRed,
/*mStop400*/                            kRed,                     kRed,
/*mStop450*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop500*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop550*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop650*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop700*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop750*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed,
/*mStop850*/                            kRed,                     kRed,                       kRed,                    kRed,                          kRed,                            kRed };

const int nMC = sizeof(mcStr)/sizeof(mcStr[0]);

static const double dataLumi = (803.197 + 82.136 + 4.385*1000 + 6.397*1000 + 495.003 + 7.266*1000)/1000.; // in fb-1
const double scaledToDataLumi = dataLumi; // in fb-1

const double signifScaleToFullLumi = sqrt(dataLumi/scaledToDataLumi);

double dataScale = 1.0;

vector<double> scalesVec;

vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

double calcMT2(const TLorentzVector &fatJet1LVec, const TLorentzVector &fatJet2LVec, const TLorentzVector &metLVec);

const double lowTopCut_ = 80, highTopCut_ = 270, lowWCut_ = 50, highWCut_ = 120;

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
vector<TH1D*> h1_topJetPt_aftConeAndCats_aftMETVec, h1_topJetPt_aftConeAndCats_aftMTAndMT2CutsVec;
vector<TH1D*> h1_m12_aftConeAndCat00Vec, h1_m13_aftConeAndCat00Vec, h1_m23_aftConeAndCat00Vec;
vector<TH1D*> h1_m12_aftConeAndCat01Vec, h1_m13_aftConeAndCat01Vec, h1_m23_aftConeAndCat01Vec;
vector<TH1D*> h1_m12_aftConeAndCat02Vec, h1_m13_aftConeAndCat02Vec, h1_m23_aftConeAndCat02Vec;
vector<TH1D*> h1_metVec, h1_met_aftGoodFatJetPairsVec, h1_met_aftMTAndMT2CutsVec; 
vector<TH1D*> h1_nCombInAConeVec, h1_nTopCandVec, h1_nTopCandWithBtagVec;
vector<TH1D*> h1_nJets_bfrBaselineVec, h1_nJetsVec;
vector<TH1D*> h1_vtxSize_noPUreWeightingVec, h1_vtxSizeVec;
vector<TH1D*> h1_mTremainingTopJetVec, h1_mTbestTopJetVec, h1_mTbJetVec, h1_MT2Vec, h1_mTbestWJetVec, h1_mTbestbJetVec, h1_mTremainingTopJetWithGoodWmassVec, h1_mTbJetPlusmTbestTopJetVec;
vector<TH1D*> h1_mTbestTopJet_aftMTAndMT2CutsVec, h1_mTbJet_aftMTAndMT2CutsVec, h1_MT2_aftMTAndMT2CutsVec, h1_mTremainingTopJet_aftMTAndMT2CutsVec, h1_mTbJetPlusmTbestTopJet_aftMTAndMT2CutsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCatsVec, h1_mbestWJet_aftConeAndCatsVec, h1_ratiomWovermTop_aftConeAndCatsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec, h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec, h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2CutsVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftMETVec, h1_mbestWJet_aftConeAndCats_aftMETVec;
vector<TH1D*> h1_mbestTopJet_aftConeAndCats_aftPtVec, h1_mbestWJet_aftConeAndCats_aftPtVec;
vector<TH1D*> h1_remainingmTopJet_aftConeAndCatsVec, h1_remainingmWJet_aftConeAndCatsVec;
vector<TH1D*> h1_remainingmTopJet_aftConeAndCats_WithGoodWmassVec, h1_remainingmWJet_aftConeAndCats_WithGoodWmassVec;
vector<TH1D*> h1_remainingnbJetVec, h1_bestnbJetVec, h1_totalnbJetVec;

vector<TH2D*> h2_m23overm123vsarctanm13overm12Vec, h2_mTbestWvsmTbestbVec, h2_mTbestTopJetvsmTbJetVec, h2_mTbestTopJetvsmTremainingHadPartVec, h2_mTbestTopJetvsminDeltaPhiMETbVec, h2_mTbJetvsminDeltaPhiMETbVec, h2_mTbestTopJetvsMT2Vec, h2_mTbJetvsMT2Vec;
vector<TH2D*> h2_mTbestTopJetvsMETVec, h2_mTbJetvsMETVec, h2_MT2vsMETVec, h2_minDeltaPhiMETbvsMETVec, h2_mTbestTopJetPlusmTbJetvsMETVec;
vector<TH2D*> h2_mTbestTopJetPlusmTbJetvsMT2Vec, h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETbVec, h2_MT2vsminDeltaPhiMETbVec;
vector<TH2D*> h2_remainingmTopJetvsremainingmWJetVec, h2_bestmTopJetvsbestmWJetVec;

vector<TH1D*> h1_totGenHadTopPtVec, h1_rndmGenHadTopPtVec, h1_aftTaggerGenHadTopPtVec, h1_minDRtopTaggerMatchVec, h1_aftTaggerGenHadTopPt_aftmaxDRVec, h1_totGenHadTopPt_aftMETVec;
vector<TH1D*> h1_deltaRBetweenTwoGenTopsVec, h1_deltaRBetweenTwoAllHadGenTopsVec;
vector<TH2D*> h2_deltaRBetweenTwoGenTopsvsAvgTopPtVec, h2_deltaRBetweenTwoAllHadGenTopsvsAvgTopPtVec;
vector<TH1D*> h1_effTopTagvsPtVec, h1_effTopTagvsPt_aftmaxDRVec, h1_effmaxDRmatchvsPtVec;
vector<TH2D*> h2_aftTaggerGenHadTopPtvsRecoTopPtVec;
vector<TH2D*> h2_mbestTopJetvstopJetPtVec;

void draw1DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH1D*> &h1_inputVec, const int nRebin =1, const TString optDrawStr ="");
void draw2DallINone(TCanvas *cs, const int lastPadIdx, const vector<TH2D*> &h2_inputVec);

void dataDrivenZinv();

// Make the variables global so that they can be reused by different trees.
// Idea is that the variables will be updated by each tree each entry
// and used only locally for each update.
// Basic event information
UInt_t          RunNum;
UInt_t          LumiBlockNum;
UInt_t          EvtNum;
UShort_t        NVtx;
Float_t         Weight;
Float_t         NumPUInteractions;

// Filter information
UChar_t         Filter_eeNoiseFilter;
UChar_t         Filter_trackingFailureFilter;      
UChar_t         Filter_inconsistentMuons;
UChar_t         Filter_greedyMuons;
UChar_t         Filter_ra2EcalTPFilter;  
UChar_t         Filter_ra2EcalBEFilter;  
UChar_t         Filter_hcalLaserEventFilter;
UChar_t         Filter_ecalLaserCorrFilter;
UChar_t         Filter_eeBadScFilter;
UChar_t         Filter_PBNRFilter;
// Variables used for our searches
UShort_t        PATElectronsIDIsoNum; // electron veto
UShort_t        PATMuonsPFIDIsoNum; // muon veto
Float_t         HT;
Float_t         MHT;
UShort_t        NJets;
Float_t         DeltaPhi1;
Float_t         DeltaPhi2;
Float_t         DeltaPhi3;

UShort_t        StopSAKLooseElectronSelectorNum;
Float_t         StopSAKLooseElectronSelectorPt[50];   //[StopSAKLooseElectronSelectorNum]
Float_t         StopSAKLooseElectronSelectorEta[50];   //[StopSAKLooseElectronSelectorNum]
Float_t         StopSAKLooseElectronSelectorPhi[50];   //[StopSAKLooseElectronSelectorNum]
Float_t         StopSAKLooseElectronSelectorE[50];   //[StopSAKLooseElectronSelectorNum]
UShort_t        StopSAKLooseMuonSelectorNum;
Float_t         StopSAKLooseMuonSelectorPt[50];   //[StopSAKLooseMuonSelectorNum]
Float_t         StopSAKLooseMuonSelectorEta[50];   //[StopSAKLooseMuonSelectorNum]
Float_t         StopSAKLooseMuonSelectorPhi[50];   //[StopSAKLooseMuonSelectorNum]
Float_t         StopSAKLooseMuonSelectorE[50];   //[StopSAKLooseMuonSelectorNum]
UShort_t        IsolatedTracksNum;
Float_t         IsolatedTracksPt[50];   //[IsolatedTracksNum]
Float_t         IsolatedTracksEta[50];   //[IsolatedTracksNum]
Float_t         IsolatedTracksPhi[50];   //[IsolatedTracksNum]
Float_t         IsolatedTracksE[50];   //[IsolatedTracksNum]

UShort_t        PATMETsPFNum;
Float_t         PATMETsPFPt[50];   //[PATMETsPFNum]
Float_t         PATMETsPFEta[50];   //[PATMETsPFNum]
Float_t         PATMETsPFPhi[50];   //[PATMETsPFNum]
Float_t         PATMETsPFE[50];   //[PATMETsPFNum]
UShort_t        PATMETsPFPhiCorrNum;
Float_t         PATMETsPFPhiCorrPt[50];   //[PATMETsPFPhiCorrNum]
Float_t         PATMETsPFPhiCorrEta[50];   //[PATMETsPFPhiCorrNum]
Float_t         PATMETsPFPhiCorrPhi[50];   //[PATMETsPFPhiCorrNum]
Float_t         PATMETsPFPhiCorrE[50];   //[PATMETsPFPhiCorrNum]

UShort_t        BTagCSVNum;
Float_t         BTagCSV[50];   //[BTagCSVNum]

UShort_t        JetsNum;
Float_t         JetsPt[50];   //[JetsNum]
Float_t         JetsEta[50];   //[JetsNum]
Float_t         JetsPhi[50];   //[JetsNum]
Float_t         JetsE[50];   //[JetsNum]

// Signal scan variables
Float_t         massMom;
Float_t         massDau;
UShort_t        cteq66Num;
Float_t         cteq66[45];   //[cteq66Num]
UShort_t        MSTW2008nlo68clNum;
Float_t         MSTW2008nlo68cl[41];   //[MSTW2008nlo68clNum]
UShort_t        NNPDF20Num;
Float_t         NNPDF20[101];   //[NNPDF20Num]

Float_t         ISRWeight;

Float_t         stopremainPassCSVS;
Float_t         stopMT2;
Float_t         stopbestTopJetMass;
Float_t         stoplinearCombmTbJetPlusmTbestTopJet;
Float_t         stopmTbJet;
Float_t         stopmTbestTopJet;
Float_t         stopmTbestWJet;
Float_t         stopmTbestbJet;
Float_t         stopbestTopJetIdx;
Float_t         stopmTremainingTopJet;
Float_t         stoppickedRemainingCombfatJetIdx;

bool doSignalScanUnc = true;

// Standard RA2
TH2D * h2_NTotal = 0;
std::vector<TH2D*> h2_NPassed_stdRA2;
// For PDF sys.
static const int nCompCTEQ = 45, nCompMSTW = 41, nCompNNPDF = 101;
std::vector<TH2D*> h2_NTotal_CTEQ, h2_NTotal_MSTW, h2_NTotal_NNPDF;
std::vector<std::vector<TH2D*> > h2_NPassed_CTEQ, h2_NPassed_MSTW, h2_NPassed_NNPDF;
// ISR/FSR
TH2D * h2_NTotal_ISRFSR = 0;
std::vector<TH2D*> h2_NPassed_ISRFSR;
// JES UP/DN
TH2D * h2_NTotal_JESUP = 0, * h2_NTotal_JESDN = 0;
std::vector<TH2D*> h2_NPassed_JESUP, h2_NPassed_JESDN;
// JER UP/DN
TH2D * h2_NTotal_JER   = 0, * h2_NTotal_JERUP = 0, * h2_NTotal_JERDN = 0;
std::vector<TH2D*> h2_NPassed_JER, h2_NPassed_JERUP, h2_NPassed_JERDN;

string keyStrForTheStage = "optimization";

std::vector<ULong64_t> evtIDvec;
std::vector<bool> evtPassBasicvec, evtPassStopvec;

TStopwatch timer;

void initMCscales(){

   dataScale = scaledToDataLumi/dataLumi;
   cout<<"\nscaledToDataLumi : "<<scaledToDataLumi<<"  dataLumi : "<<dataLumi<<"  dataScale : "<<dataScale<<endl;
   for(int ib=0; ib<nMC; ib++){
      double perScale = scaledToDataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
      scalesVec.push_back(perScale);
      printf("%30s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
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
  TH1D *template_h1_topJetPt_aftConeAndCats_aftMET = new TH1D(sampleKeyStringT+"_h1_topJetPt_aftConeAndCats_aftMET", sampleKeyStringT+":  topJetPt_aftConeAndCats_aftMET; top pt", 100, 0, 1000); template_h1_topJetPt_aftConeAndCats_aftMET->Sumw2();
  TH1D *template_h1_topJetPt_aftConeAndCats_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_topJetPt_aftConeAndCats_aftMTAndMT2Cuts", sampleKeyStringT+":  topJetPt_aftConeAndCats_aftMTAndMT2Cuts; top pt", 100, 0, 1000); template_h1_topJetPt_aftConeAndCats_aftMTAndMT2Cuts->Sumw2();

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
  TH1D *template_h1_nJets_bfrBaseline = new TH1D(sampleKeyStringT+"_h1_nJets_bfrBaseline", sampleKeyStringT+":  number of jets before baseline; nJets", 20, 0, 20); template_h1_nJets_bfrBaseline->Sumw2();
  TH1D *template_h1_nJets = new TH1D(sampleKeyStringT+"_h1_nJets", sampleKeyStringT+":  number of jets; nJets", 20, 0, 20); template_h1_nJets->Sumw2();
  TH1D *template_h1_vtxSize = new TH1D(sampleKeyStringT+"_h1_vtxSize", sampleKeyStringT+":  number of vertices; vtxSize", 50, 0, 50); template_h1_vtxSize->Sumw2();
  TH1D *template_h1_vtxSize_noPUreWeighting = new TH1D(sampleKeyStringT+"_h1_vtxSize_noPUreWeighting", sampleKeyStringT+":  number of vertices without PU reweighting; vtxSize", 50, 0, 50); template_h1_vtxSize_noPUreWeighting->Sumw2();

  TH1D *template_h1_remainingnbJet = new TH1D(sampleKeyStringT+"_h1_remainingnbJet", sampleKeyStringT+":  number of b-jet in the remaining jets; nbJet", 10, 0, 10); template_h1_remainingnbJet->Sumw2();
  TH1D *template_h1_bestnbJet = new TH1D(sampleKeyStringT+"_h1_bestnbJet", sampleKeyStringT+":  number of b-jet in the best top jet; nbJet", 10, 0, 10); template_h1_bestnbJet->Sumw2();
  TH1D *template_h1_totalnbJet = new TH1D(sampleKeyStringT+"_h1_totalnbJet", sampleKeyStringT+":  number of b-jet in the total top jet; nbJet", 10, 0, 10); template_h1_totalnbJet->Sumw2();

  TH1D *template_h1_mTremainingTopJet = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJet", sampleKeyStringT+":  mT of remaining top jet & met; mTr", 150, 0, 1500); template_h1_mTremainingTopJet->Sumw2();
  TH1D *template_h1_mTremainingTopJetWithGoodWmass = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJetWithGoodWmass", sampleKeyStringT+":  mT of remaining top jet & met with good W mass; mTr", 100, 0, 1000); template_h1_mTremainingTopJetWithGoodWmass->Sumw2();

  TH1D *template_h1_mTbestTopJet = new TH1D(sampleKeyStringT+"_h1_mTbestTopJet", sampleKeyStringT+":  mT of best top jet to met; mTt", 100, 0, 1000); template_h1_mTbestTopJet->Sumw2();
  TH1D *template_h1_mTbJet = new TH1D(sampleKeyStringT+"_h1_mTbJet", sampleKeyStringT+":  mT of b jet to met; mTb", 100, 0, 1000); template_h1_mTbJet->Sumw2();
  TH1D *template_h1_MT2 = new TH1D(sampleKeyStringT+"_h1_MT2", sampleKeyStringT+":  MT2 of the system; MT2", 100, 0, 1000); template_h1_MT2->Sumw2();
  TH1D *template_h1_mTbJetPlusmTbestTopJet = new TH1D(sampleKeyStringT+"_h1_mTbJetPlusmTbestTopJet", sampleKeyStringT+":  mTbJet + 0.5*mTbestTopJet; mTb+0.5mTt", 150, 0, 1500); template_h1_mTbJetPlusmTbestTopJet->Sumw2();

  TH1D *template_h1_mTbestTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbestTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of best top jet to met aftMTAndMT2Cuts; mTt", 100, 0, 1000); template_h1_mTbestTopJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTbJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of b jet to met aftMTAndMT2Cuts; mTb", 100, 0, 1000); template_h1_mTbJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mTbJet + 0.5*mTbestTopJet aftMTAndMT2Cuts; mTb+0.5mTt", 150, 0, 1500); template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_MT2_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_MT2_aftMTAndMT2Cuts", sampleKeyStringT+":  MT2 of the system aftMTAndMT2Cuts; MT2", 100, 0, 1000); template_h1_MT2_aftMTAndMT2Cuts->Sumw2();
  TH1D *template_h1_mTremainingTopJet_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_mTremainingTopJet_aftMTAndMT2Cuts", sampleKeyStringT+":  mT of remaining top jet to met aftMTAndMT2Cuts; mTr", 100, 0, 1000); template_h1_mTremainingTopJet_aftMTAndMT2Cuts->Sumw2();

  TH1D *template_h1_mTbestWJet = new TH1D(sampleKeyStringT+"_h1_mTbestWJet", sampleKeyStringT+":  mT of best W jet & met; mTW", 100, 0, 1000); template_h1_mTbestWJet->Sumw2();
  TH1D *template_h1_mTbestbJet = new TH1D(sampleKeyStringT+"_h1_mTbestbJet", sampleKeyStringT+":  mT of the other jet & met; mTo", 100, 0, 1000); template_h1_mTbestbJet->Sumw2();

  TH2D *template_h2_mTbestWvsmTbestb = new TH2D(sampleKeyStringT+"_h2_mTbestWvsmTbestb", sampleKeyStringT+":  mT of best W versus mT of the other jet; mTo; mTW", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbestTopJetvsmTbJet = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsmTbJet", sampleKeyStringT+":  mT of best top jet versus mT of b jet; mTb; mTt", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbestTopJetvsmTremainingHadPart = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsmTremainingHadPart", sampleKeyStringT+":  mT of best top jet versus mT of remaining hadronic decay part; mTr; mTt", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbestTopJetvsMT2 = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsMT2", sampleKeyStringT+":  mT of best top jet versus MT2; MT2; mTt", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbJetvsMT2 = new TH2D(sampleKeyStringT+"_h2_mTbJetvsMT2", sampleKeyStringT+":  mT of b jet versus MT2; MT2; mTb", 100, 0, 1000, 100, 0, 1000);

  TH2D *template_h2_remainingmTopJetvsremainingmWJet = new TH2D(sampleKeyStringT+"_h2_remainingmTopJetvsremainingmWJet", sampleKeyStringT+":  remaining top jet mass versus remaining W mass; m_{W}; m_{t}", 100, 0, 500, 100, 0, 800);
  TH2D *template_h2_bestmTopJetvsbestmWJet = new TH2D(sampleKeyStringT+"_h2_bestmTopJetvsbestmWJet", sampleKeyStringT+":  best top jet mass versus best W mass; m_{W}; m_{t}", 100, 0, 200, 100, 0, 380);

  TH2D *template_h2_mTbestTopJetvsminDeltaPhiMETb = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsminDeltaPhiMETb", sampleKeyStringT+":  mT of best top jet versus minDeltaPhiMETb; min(#Delta#phi(MET,b)); mTt", 100, 0, 3.2, 100, 0, 1000);
  TH2D *template_h2_mTbJetvsminDeltaPhiMETb = new TH2D(sampleKeyStringT+"_h2_mTbJetvsminDeltaPhiMETb", sampleKeyStringT+":  mT of b jet versus minDeltaPhiMETb; min(#Delta#phi(MET,b)); mTb", 100, 0, 3.2, 100, 0, 1000);

  TH2D *template_h2_mTbestTopJetPlusmTbJetvsMT2 = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetPlusmTbJetvsMT2", sampleKeyStringT+":  linear combination of mTt and mTb versus MT2; MT2; a*mTt+b*mTb", 100, 0, 1000, 150, 0, 1500); 
  TH2D *template_h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETb = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETb", sampleKeyStringT+":  linear combination of mTt and mTb versus minDeltaPhiMETb; min(#Delta#phi(MET,b)); a*mTt+b*mTb", 100, 0, 3.2, 150, 0, 1500); 
  TH2D *template_h2_MT2vsminDeltaPhiMETb = new TH2D(sampleKeyStringT+"_h2_MT2vsminDeltaPhiMETb", sampleKeyStringT+":  MT2 versus minDeltaPhiMETb; min(#Delta#phi(MET,b)); MT2", 100, 0, 3.2, 100, 0, 1000);

  TH2D *template_h2_mTbestTopJetvsMET = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetvsMET", sampleKeyStringT+":  mT of best top jet versus MET; MET; mTt", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_mTbJetvsMET = new TH2D(sampleKeyStringT+"_h2_mTbJetvsMET", sampleKeyStringT+":  mT of b jet versus MET; MET; mTb", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_MT2vsMET = new TH2D(sampleKeyStringT+"_h2_MT2vsMET", sampleKeyStringT+":  MT2 versus MET; MET; MT2", 100, 0, 1000, 100, 0, 1000);
  TH2D *template_h2_minDeltaPhiMETbvsMET = new TH2D(sampleKeyStringT+"_h2_minDeltaPhiMETbvsMET", sampleKeyStringT+":  minDeltaPhiMETb versus MET; MET; min(#Delta#phi(MET,b))", 100, 0, 1000, 100, 0, 3.2);
  TH2D *template_h2_mTbestTopJetPlusmTbJetvsMET = new TH2D(sampleKeyStringT+"_h2_mTbestTopJetPlusmTbJetvsMET", sampleKeyStringT+":  linear combination of mTt and mTb versus MET; MET; a*mTt+b*mTb", 100, 0, 1000, 150, 0, 1500);

  TH1D *template_h1_totGenHadTopPt = new TH1D(sampleKeyStringT+"_h1_totGenHadTopPt", sampleKeyStringT+": generator level top pt; top pt", 100, 0, 1000); template_h1_totGenHadTopPt->Sumw2();
  TH1D *template_h1_totGenHadTopPt_aftMET = new TH1D(sampleKeyStringT+"_h1_totGenHadTopPt_aftMET", sampleKeyStringT+": generator level top pt aftMET; top pt", 100, 0, 1000); template_h1_totGenHadTopPt_aftMET->Sumw2();
  TH1D *template_h1_rndmGenHadTopPt = new TH1D(sampleKeyStringT+"_h1_rndmGenHadTopPt", sampleKeyStringT+": generator level top pt randomly picking one; top pt", 100, 0, 1000); template_h1_rndmGenHadTopPt->Sumw2();
  TH1D *template_h1_aftTaggerGenHadTopPt = new TH1D(sampleKeyStringT+"_h1_aftTaggerGenHadTopPt", sampleKeyStringT+": generator level top pt after top tagger; top pt", 100, 0, 1000); template_h1_aftTaggerGenHadTopPt->Sumw2();
  TH1D *template_h1_aftTaggerGenHadTopPt_aftmaxDR = new TH1D(sampleKeyStringT+"_h1_aftTaggerGenHadTopPt_aftmaxDR", sampleKeyStringT+": generator level top pt after top tagger; top pt", 100, 0, 1000); template_h1_aftTaggerGenHadTopPt_aftmaxDR->Sumw2();
  TH1D *template_h1_minDRtopTaggerMatch = new TH1D(sampleKeyStringT+"_h1_minDRtopTaggerMatch", sampleKeyStringT+": minDR of top tagger matching; minDR", 100, 0, 3.2); template_h1_minDRtopTaggerMatch->Sumw2();
  TH2D *template_h2_aftTaggerGenHadTopPtvsRecoTopPt = new TH2D(sampleKeyStringT+"_h2_aftTaggerGenHadTopPtvsRecoTopPt", sampleKeyStringT+": generator level top pt after top tagger versus reco top pt; Reco top pt; Gen top pt", 100, 0, 1000, 100, 0, 1000);

  TH2D *template_h2_mbestTopJetvstopJetPt = new TH2D(sampleKeyStringT+"_h2_mbestTopJetvstopJetPt", sampleKeyStringT+": top jet mass versus top jet pt; top pt; m_{t}", 100, 0, 1000, 100, 0, 380);

  TH1D *template_h1_ratiomWovermTop_aftConeAndCats = new TH1D(sampleKeyStringT+"_h1_ratiomWovermTop_aftConeAndCats", sampleKeyStringT+": ratio of W mass over Top mass; m_{W}/m_{t}", 100, 0, 1.5); template_h1_ratiomWovermTop_aftConeAndCats->Sumw2();
  TH1D *template_h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2Cuts = new TH1D(sampleKeyStringT+"_h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2Cuts", sampleKeyStringT+": ratio of W mass over Top mass aftMTAndMT2Cuts; m_{W}/m_{t}", 100, 0, 1.5); template_h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2Cuts->Sumw2();

  TH1D *template_h1_deltaRBetweenTwoGenTops = new TH1D(sampleKeyStringT+"_h1_deltaRBetweenTwoGenTops", sampleKeyStringT+": #DeltaR between two generator tops; #DeltaR", 100, 0, 5.0); template_h1_deltaRBetweenTwoGenTops->Sumw2();
  TH1D *template_h1_deltaRBetweenTwoAllHadGenTops = new TH1D(sampleKeyStringT+"_h1_deltaRBetweenTwoAllHadGenTops", sampleKeyStringT+": #DeltaR between two all hadronic generator tops; #DeltaR", 100, 0, 5.0); template_h1_deltaRBetweenTwoAllHadGenTops->Sumw2();

  TH2D *template_h2_deltaRBetweenTwoGenTopsvsAvgTopPt = new TH2D(sampleKeyStringT+"_h2_deltaRBetweenTwoGenTopsvsAvgTopPt", sampleKeyStringT+":  #DeltaR between two generator tops versus average top pt; top pt; #DeltaR", 100, 0, 1000, 100, 0, 5.0); 
  TH2D *template_h2_deltaRBetweenTwoAllHadGenTopsvsAvgTopPt = new TH2D(sampleKeyStringT+"_h2_deltaRBetweenTwoAllHadGenTopsvsAvgTopPt", sampleKeyStringT+":  #DeltaR between two all hadronic generator tops versus average top pt; top pt; #DeltaR", 100, 0, 1000, 100, 0, 5.0); 

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){

     std::string keyString = subSampleKeysVec[ist];

     double scaleMC = 1.0;
     for(int ib=0; ib<nMC; ib++){ if( mcStr[ib] == keyString ){ scaleMC = scalesVec[ib]; } }
     TString keyStringT(keyString);
     if( keyStringT.Contains("Data") ) scaleMC = dataScale;

     vector<TLorentzVector> *template_oriJetsVec = new vector<TLorentzVector>(); vector<double> *template_recoJetsBtagCSVS = new vector<double>();
     vector<TLorentzVector> *template_jetsLVec = new vector<TLorentzVector>(); vector<double> *template_jetsBtagCSVS = new vector<double>();
/*
     vector<TLorentzVector> *template_gentopLVec =0;
     vector<int> *template_gentoppdgIdVec =0;
     vector<vector<TLorentzVector> > *template_gentopDausLVec =0, *template_genWDausLVec =0, *template_gentauDausLVec =0;
     vector<vector<int> > *template_gentopDauspdgIdVec =0, *template_genWDauspdgIdVec =0, *template_gentauDauspdgIdVec =0;
     vector<string> *template_decayTypeVec =0;

     vector<vector<TLorentzVector> > *template_hepTaggerSubJets =0;
 */ 

     TTree * template_AUX = treeVec[ist];
     template_AUX->SetBranchStatus("*", 0);

     template_AUX->SetBranchStatus("RunNum", 1); template_AUX->SetBranchAddress("RunNum", &RunNum);
     template_AUX->SetBranchStatus("LumiBlockNum", 1); template_AUX->SetBranchAddress("LumiBlockNum", &LumiBlockNum);
     template_AUX->SetBranchStatus("EvtNum", 1); template_AUX->SetBranchAddress("EvtNum", &EvtNum);
     template_AUX->SetBranchStatus("NVtx", 1); template_AUX->SetBranchAddress("NVtx", &NVtx);
     template_AUX->SetBranchStatus("NumPUInteractions", 1); template_AUX->SetBranchAddress("NumPUInteractions", &NumPUInteractions);

     if( doSignalScanUnc && keyStringT.Contains("Signal") ){
        template_AUX->SetBranchStatus("Weight", 1); template_AUX->SetBranchAddress("Weight", &Weight);
        template_AUX->SetBranchStatus("massMom", 1); template_AUX->SetBranchAddress("massMom", &massMom);
        template_AUX->SetBranchStatus("massDau", 1); template_AUX->SetBranchAddress("massDau", &massDau);
     }

     template_AUX->SetBranchStatus("Filter_eeNoiseFilter", 1); template_AUX->SetBranchAddress("Filter_eeNoiseFilter", &Filter_eeNoiseFilter);
     template_AUX->SetBranchStatus("Filter_trackingFailureFilter", 1); template_AUX->SetBranchAddress("Filter_trackingFailureFilter", &Filter_trackingFailureFilter);
     template_AUX->SetBranchStatus("Filter_inconsistentMuons", 1); template_AUX->SetBranchAddress("Filter_inconsistentMuons", &Filter_inconsistentMuons);
     template_AUX->SetBranchStatus("Filter_greedyMuons", 1); template_AUX->SetBranchAddress("Filter_greedyMuons", &Filter_greedyMuons);
     template_AUX->SetBranchStatus("Filter_ra2EcalTPFilter", 1); template_AUX->SetBranchAddress("Filter_ra2EcalTPFilter", &Filter_ra2EcalTPFilter);
     template_AUX->SetBranchStatus("Filter_ra2EcalBEFilter", 1); template_AUX->SetBranchAddress("Filter_ra2EcalBEFilter", &Filter_ra2EcalBEFilter);
     template_AUX->SetBranchStatus("Filter_hcalLaserEventFilter", 1); template_AUX->SetBranchAddress("Filter_hcalLaserEventFilter", &Filter_hcalLaserEventFilter);
     template_AUX->SetBranchStatus("Filter_ecalLaserCorrFilter", 1); template_AUX->SetBranchAddress("Filter_ecalLaserCorrFilter", &Filter_ecalLaserCorrFilter);
     template_AUX->SetBranchStatus("Filter_eeBadScFilter", 1); template_AUX->SetBranchAddress("Filter_eeBadScFilter", &Filter_eeBadScFilter);
     template_AUX->SetBranchStatus("Filter_PBNRFilter", 1); template_AUX->SetBranchAddress("Filter_PBNRFilter", &Filter_PBNRFilter);
     template_AUX->SetBranchStatus("PATElectronsIDIsoNum", 1); template_AUX->SetBranchAddress("PATElectronsIDIsoNum", &PATElectronsIDIsoNum); // electron veto
     template_AUX->SetBranchStatus("PATMuonsPFIDIsoNum", 1); template_AUX->SetBranchAddress("PATMuonsPFIDIsoNum", &PATMuonsPFIDIsoNum); // muon veto
     template_AUX->SetBranchStatus("PATMETsPFNum", 1); template_AUX->SetBranchAddress("PATMETsPFNum", &PATMETsPFNum);
     
     if( doSignalScanUnc && keyStringT.Contains("Signal") ){ 
        template_AUX->SetBranchStatus("cteq66Num", 1); template_AUX->SetBranchAddress("cteq66Num", &cteq66Num);
        template_AUX->SetBranchStatus("cteq66", 1); template_AUX->SetBranchAddress("cteq66", cteq66);
        template_AUX->SetBranchStatus("MSTW2008nlo68clNum", 1); template_AUX->SetBranchAddress("MSTW2008nlo68clNum", &MSTW2008nlo68clNum);
        template_AUX->SetBranchStatus("MSTW2008nlo68cl", 1); template_AUX->SetBranchAddress("MSTW2008nlo68cl", MSTW2008nlo68cl);
        template_AUX->SetBranchStatus("NNPDF20Num", 1); template_AUX->SetBranchAddress("NNPDF20Num", &NNPDF20Num);
        template_AUX->SetBranchStatus("NNPDF20", 1); template_AUX->SetBranchAddress("NNPDF20", NNPDF20);
        template_AUX->SetBranchStatus("ISRWeight", 1); template_AUX->SetBranchAddress("ISRWeight", &ISRWeight);
     }

     template_AUX->SetBranchStatus("JetsNum", 1); template_AUX->SetBranchAddress("JetsNum", &JetsNum);
     template_AUX->SetBranchStatus("JetsPt", 1); template_AUX->SetBranchAddress("JetsPt", &JetsPt);
     template_AUX->SetBranchStatus("JetsEta", 1); template_AUX->SetBranchAddress("JetsEta", &JetsEta);
     template_AUX->SetBranchStatus("JetsPhi", 1); template_AUX->SetBranchAddress("JetsPhi", &JetsPhi);
     template_AUX->SetBranchStatus("JetsE", 1); template_AUX->SetBranchAddress("JetsE", &JetsE);

     template_AUX->SetBranchStatus("StopSAKLooseElectronSelectorNum", 1); template_AUX->SetBranchAddress("StopSAKLooseElectronSelectorNum", &StopSAKLooseElectronSelectorNum);
     template_AUX->SetBranchStatus("StopSAKLooseElectronSelectorPt", 1); template_AUX->SetBranchAddress("StopSAKLooseElectronSelectorPt", &StopSAKLooseElectronSelectorPt);
     template_AUX->SetBranchStatus("StopSAKLooseElectronSelectorEta", 1); template_AUX->SetBranchAddress("StopSAKLooseElectronSelectorEta", &StopSAKLooseElectronSelectorEta);
     template_AUX->SetBranchStatus("StopSAKLooseElectronSelectorPhi", 1); template_AUX->SetBranchAddress("StopSAKLooseElectronSelectorPhi", &StopSAKLooseElectronSelectorPhi);
     template_AUX->SetBranchStatus("StopSAKLooseElectronSelectorE", 1); template_AUX->SetBranchAddress("StopSAKLooseElectronSelectorE", &StopSAKLooseElectronSelectorE);
     template_AUX->SetBranchStatus("StopSAKLooseMuonSelectorNum", 1); template_AUX->SetBranchAddress("StopSAKLooseMuonSelectorNum", &StopSAKLooseMuonSelectorNum);
     template_AUX->SetBranchStatus("StopSAKLooseMuonSelectorPt", 1); template_AUX->SetBranchAddress("StopSAKLooseMuonSelectorPt", &StopSAKLooseMuonSelectorPt);
     template_AUX->SetBranchStatus("StopSAKLooseMuonSelectorEta", 1); template_AUX->SetBranchAddress("StopSAKLooseMuonSelectorEta", &StopSAKLooseMuonSelectorEta);
     template_AUX->SetBranchStatus("StopSAKLooseMuonSelectorPhi", 1); template_AUX->SetBranchAddress("StopSAKLooseMuonSelectorPhi", &StopSAKLooseMuonSelectorPhi);
     template_AUX->SetBranchStatus("StopSAKLooseMuonSelectorE", 1); template_AUX->SetBranchAddress("StopSAKLooseMuonSelectorE", &StopSAKLooseMuonSelectorE);
     template_AUX->SetBranchStatus("IsolatedTracksNum", 1); template_AUX->SetBranchAddress("IsolatedTracksNum", &IsolatedTracksNum);
     template_AUX->SetBranchStatus("IsolatedTracksPt", 1); template_AUX->SetBranchAddress("IsolatedTracksPt", &IsolatedTracksPt);
     template_AUX->SetBranchStatus("IsolatedTracksEta", 1); template_AUX->SetBranchAddress("IsolatedTracksEta", &IsolatedTracksEta);
     template_AUX->SetBranchStatus("IsolatedTracksPhi", 1); template_AUX->SetBranchAddress("IsolatedTracksPhi", &IsolatedTracksPhi);
     template_AUX->SetBranchStatus("IsolatedTracksE", 1); template_AUX->SetBranchAddress("IsolatedTracksE", &IsolatedTracksE);
     template_AUX->SetBranchStatus("PATMETsPFNum", 1); template_AUX->SetBranchAddress("PATMETsPFNum", &PATMETsPFNum);
     template_AUX->SetBranchStatus("PATMETsPFPt", 1); template_AUX->SetBranchAddress("PATMETsPFPt", &PATMETsPFPt);
     template_AUX->SetBranchStatus("PATMETsPFEta", 1); template_AUX->SetBranchAddress("PATMETsPFEta", &PATMETsPFEta);
     template_AUX->SetBranchStatus("PATMETsPFPhi", 1); template_AUX->SetBranchAddress("PATMETsPFPhi", &PATMETsPFPhi);
     template_AUX->SetBranchStatus("PATMETsPFE", 1); template_AUX->SetBranchAddress("PATMETsPFE", &PATMETsPFE);
     template_AUX->SetBranchStatus("PATMETsPFPhiCorrNum", 1); template_AUX->SetBranchAddress("PATMETsPFPhiCorrNum", &PATMETsPFPhiCorrNum);
     template_AUX->SetBranchStatus("PATMETsPFPhiCorrPt", 1); template_AUX->SetBranchAddress("PATMETsPFPhiCorrPt", &PATMETsPFPhiCorrPt);
     template_AUX->SetBranchStatus("PATMETsPFPhiCorrEta", 1); template_AUX->SetBranchAddress("PATMETsPFPhiCorrEta", &PATMETsPFPhiCorrEta);
     template_AUX->SetBranchStatus("PATMETsPFPhiCorrPhi", 1); template_AUX->SetBranchAddress("PATMETsPFPhiCorrPhi", &PATMETsPFPhiCorrPhi);
     template_AUX->SetBranchStatus("PATMETsPFPhiCorrE", 1); template_AUX->SetBranchAddress("PATMETsPFPhiCorrE", &PATMETsPFPhiCorrE);

     template_AUX->SetBranchStatus("BTagCSVNum", 1); template_AUX->SetBranchAddress("BTagCSVNum", &BTagCSVNum);
     template_AUX->SetBranchStatus("BTagCSV", 1); template_AUX->SetBranchAddress("BTagCSV", &BTagCSV);

     template_AUX->SetBranchStatus("stopremainPassCSVS", 1); template_AUX->SetBranchAddress("stopremainPassCSVS", &stopremainPassCSVS);
     template_AUX->SetBranchStatus("stopMT2", 1); template_AUX->SetBranchAddress("stopMT2", &stopMT2);
     template_AUX->SetBranchStatus("stopbestTopJetMass", 1); template_AUX->SetBranchAddress("stopbestTopJetMass", &stopbestTopJetMass);
     template_AUX->SetBranchStatus("stoplinearCombmTbJetPlusmTbestTopJet", 1); template_AUX->SetBranchAddress("stoplinearCombmTbJetPlusmTbestTopJet", &stoplinearCombmTbJetPlusmTbestTopJet);
     template_AUX->SetBranchStatus("stopmTbJet", 1); template_AUX->SetBranchAddress("stopmTbJet", &stopmTbJet);
     template_AUX->SetBranchStatus("stopmTbestTopJet", 1); template_AUX->SetBranchAddress("stopmTbestTopJet", &stopmTbestTopJet);
     template_AUX->SetBranchStatus("stopmTbestWJet", 1); template_AUX->SetBranchAddress("stopmTbestWJet", &stopmTbestWJet);
     template_AUX->SetBranchStatus("stopmTbestbJet", 1); template_AUX->SetBranchAddress("stopmTbestbJet", &stopmTbestbJet);
     template_AUX->SetBranchStatus("stopbestTopJetIdx", 1); template_AUX->SetBranchAddress("stopbestTopJetIdx", &stopbestTopJetIdx);
     template_AUX->SetBranchStatus("stopmTremainingTopJet", 1); template_AUX->SetBranchAddress("stopmTremainingTopJet", &stopmTremainingTopJet);
     template_AUX->SetBranchStatus("stoppickedRemainingCombfatJetIdx", 1); template_AUX->SetBranchAddress("stoppickedRemainingCombfatJetIdx", &stoppickedRemainingCombfatJetIdx);

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
        double template_evtWeight = 1.0;

        bool passBasic = false;
        if(    Filter_trackingFailureFilter
            && Filter_inconsistentMuons && Filter_greedyMuons
            && Filter_ra2EcalTPFilter && Filter_ra2EcalBEFilter
            && Filter_hcalLaserEventFilter
            && Filter_eeBadScFilter 
//            && Filter_ecalLaserCorrFilter 
            && Filter_PBNRFilter
            && !StopSAKLooseElectronSelectorNum && !StopSAKLooseMuonSelectorNum){
           passBasic = true;
        }

        if( doSignalScanUnc && keyStringT.Contains("Signal") ){

           h2_NTotal->Fill(massMom, massDau, Weight);

           h2_NTotal_ISRFSR->Fill(massMom, massDau, Weight*ISRWeight);

           for(int ic=0; ic<nCompCTEQ; ic++){
              h2_NTotal_CTEQ[ic]->Fill(massMom, massDau, Weight*cteq66[ic]);
           }

           for(int im=0; im<nCompMSTW; im++){
              h2_NTotal_MSTW[im]->Fill(massMom, massDau, Weight*MSTW2008nlo68cl[im]);
           }

           for(int in=0; in<nCompNNPDF; in++){
              h2_NTotal_NNPDF[in]->Fill(massMom, massDau, Weight*NNPDF20[in]);
           }

           UInt_t runPLUSlumi = RunNum*10000 + LumiBlockNum;
           ULong64_t comb = ((ULong64_t)runPLUSlumi << 32) | EvtNum;

           evtIDvec.push_back(comb); 
           evtPassBasicvec.push_back(passBasic); evtPassStopvec.push_back(false);
        }

        if( !passBasic ) continue;

        template_oriJetsVec->clear(); template_recoJetsBtagCSVS->clear();

        if( JetsNum != BTagCSVNum ) std::cout<<"ERROR ... ie : "<<ie<<"  JetsNum : "<<JetsNum<<"  !=  BTagCSVNum : "<<BTagCSVNum<<std::endl;

        for(int ij=0; ij<JetsNum; ij++){
           double jetPt = JetsPt[ij], jetEta = JetsEta[ij], jetPhi = JetsPhi[ij], jetE = JetsE[ij];
           TLorentzVector perJetLVec; perJetLVec.SetPtEtaPhiE(jetPt, jetEta, jetPhi, jetE);
           std::vector<TLorentzVector> dummyJetsVec;
           dummyJetsVec.push_back(perJetLVec);
           int dummycntNJetsPt30 = countJets(dummyJetsVec, pt30Arr);
           if( !dummycntNJetsPt30 ) continue;
           template_oriJetsVec->push_back(perJetLVec);
           template_recoJetsBtagCSVS->push_back(BTagCSV[ij]);
        }

        double met = PATMETsPFPhiCorrPt[0], metphi = PATMETsPFPhiCorrPhi[0];

        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);

        double puWeight = 1.0;
// Because signal is fastsim and does not have PU correctly done!
        if( !keyStringT.Contains("Signal") && !keyStringT.Contains("Data") ){
           puWeight = weightTruNPV(NumPUInteractions);
        }

        int cntCSVS = countCSVS((*template_oriJetsVec), (*template_recoJetsBtagCSVS), cutCSVS, bTagArr);
        int cntNJetsPt30 = countJets((*template_oriJetsVec), pt30Arr);
        int cntNJetsPt30Eta24 = countJets((*template_oriJetsVec), pt30Eta24Arr);
        int cntNJetsPt50Eta24 = countJets((*template_oriJetsVec), pt50Eta24Arr);
        int cntNJetsPt70Eta24 = countJets((*template_oriJetsVec), pt70Eta24Arr);
        vector<double> dPhiVec = calcDPhi((*template_oriJetsVec), metphi, 3, dphiArr);

        template_h1_nJets_bfrBaseline->Fill(cntNJetsPt30, template_evtWeight * puWeight * scaleMC);

        double dPhi0 = dPhiVec[0], dPhi1 = dPhiVec[1], dPhi2 = dPhiVec[2];
        
        double minDeltaPhiMETb = 999.0;
        for(unsigned int ib=0; ib<template_recoJetsBtagCSVS->size(); ib++){
           if( fabs( (*template_oriJetsVec)[ib].Eta() ) > 2.4 || (*template_oriJetsVec)[ib].Pt() < 30 ) continue;
           double deltaPhi = fabs(TVector2::Phi_mpi_pi((*template_oriJetsVec)[ib].Phi() - metphi));
           if( minDeltaPhiMETb > deltaPhi ) minDeltaPhiMETb = deltaPhi;
        }

        if( cntNJetsPt30 < nJetsSel ) continue;

        bool passExtraCuts = true;
        if( cntNJetsPt70Eta24 < nJetsSelPt70Eta24 ) passExtraCuts = false;
        if( cntNJetsPt50Eta24 < nJetsSelPt50Eta24 ) passExtraCuts = false;
        if( cntNJetsPt30Eta24 < nJetsSelPt30Eta24 ) passExtraCuts = false;
        if( dPhi0 < 0.5 || dPhi1 < 0.5 || dPhi2 < 0.3 ) passExtraCuts = false;
        if( !cntCSVS ) passExtraCuts = false;

        if( passExtraCuts ){
           template_cntAftBaselineMinusMETWeighted += template_evtWeight * puWeight;
           template_cntAftBaselineMinusMETWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }

        if( withMETcut && met < defaultMETcut ) passExtraCuts = false;

        if( applyLowMET ){
           if( minDeltaPhiMETb < 1.0 ) continue;
           if( ! ( (met>=175 && cntNJetsPt30>=7) || (met>=200 && cntNJetsPt30>=6) ) ) continue;
        }

        if( passExtraCuts ){
           template_cntAftBaselineWeighted += template_evtWeight * puWeight;
           template_cntAftBaselineWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }

        if( !doExtraCuts_ || (doExtraCuts_ && passExtraCuts) ){
           template_cntAftPreCutsWeighted += template_evtWeight * puWeight;
           template_cntAftPreCutsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }

        if( doExtraCuts_ && !passExtraCuts ) continue;

        template_h1_nJets->Fill(cntNJetsPt30, template_evtWeight * puWeight * scaleMC);
        template_h1_vtxSize->Fill(NVtx, template_evtWeight * puWeight * scaleMC);
        template_h1_vtxSize_noPUreWeighting->Fill(NVtx, template_evtWeight * scaleMC);

        type3Ptr->setnJetsSel(nJetsSel); type3Ptr->setsimuCAdeltaR(simuCAdeltaR);

        bool pass = type3Ptr->processEvent((*template_oriJetsVec), (*template_recoJetsBtagCSVS), metLVec);

        if( stopbestTopJetIdx != type3Ptr->bestTopJetIdx || stopbestTopJetMass != type3Ptr->bestTopJetMass || stopremainPassCSVS != type3Ptr->remainPassCSVS || stopMT2 != type3Ptr->MT2 || stopmTbJet != type3Ptr->mTbJet || stopmTbestTopJet != type3Ptr->mTbestTopJet ){
//            std::cout<<"\nWARNING ... "<<std::setw(8)<<ie<<" event   in tree  bestTopJetIdx : "<<stopbestTopJetIdx<<"  bestTopJetMass : "<<stopbestTopJetMass<<"  remainPassCSVS : "<<stopremainPassCSVS<<"  MT2 : "<<stopMT2<<"  mTbJet : "<<stopmTbJet<<"  mTbestTopJet : "<<stopmTbestTopJet<<std::endl;
//            std::cout<<"            from tagger               bestTopJetIdx : "<<type3Ptr->bestTopJetIdx<<"  bestTopJetMass : "<<type3Ptr->bestTopJetMass<<"  remainPassCSVS : "<<type3Ptr->remainPassCSVS<<"  MT2 : "<<type3Ptr->MT2<<"  mTbJet : "<<type3Ptr->mTbJet<<"  mTbestTopJet : "<<type3Ptr->mTbestTopJet<<std::endl;
         }

        int combSize = type3Ptr->combSize;

        int nTopJetCandCnt =0, nTopJetCandWithBtagCnt = 0;
        for(int ic=0; ic<combSize; ic++){

           const TLorentzVector fatJet = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombfatJets[ic]);
           const TLorentzVector subJet12 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][0]);
           const TLorentzVector subJet13 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][1]);
           const TLorentzVector subJet23 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][2]);

           const double fatJetm123 = fatJet.M();
           const double fatJetm12  = subJet12.M();
           const double fatJetm13  = subJet13.M();
           const double fatJetm23  = subJet23.M();

           const double fatJetPt = fatJet.Pt();
         
           int isTopJet = 0;
           int statusCat00 = type3Ptr->finalCombfatJetsPassStatusVec[ic][0];
           int statusCat01 = type3Ptr->finalCombfatJetsPassStatusVec[ic][1];
           int statusCat02 = type3Ptr->finalCombfatJetsPassStatusVec[ic][2];
           if( statusCat00 ==1 || statusCat01 ==1 || statusCat02 ==1 ) isTopJet =1;

           if( isTopJet ){
              template_h1_topJetPt_aftConeAndCats->Fill( fatJetPt, template_evtWeight * puWeight * scaleMC );
              if( met > defaultMETcut ){
                 template_h1_topJetPt_aftConeAndCats_aftMET->Fill( fatJetPt, template_evtWeight * puWeight * scaleMC );
              }
              nTopJetCandCnt ++;
           }
   
           int remainingPassCSVS = type3Ptr->cntRemainingPassCSVSVec[ic];
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

        template_h1_nCombInACone->Fill(combSize, template_evtWeight * puWeight * scaleMC );
        template_h1_nTopCand->Fill(nTopJetCandCnt, template_evtWeight * puWeight * scaleMC );
        template_h1_nTopCandWithBtag->Fill(nTopJetCandWithBtagCnt, template_evtWeight * puWeight * scaleMC );
        template_h1_met->Fill(met, template_evtWeight * puWeight * scaleMC );

        if( type3Ptr->bestTopJetIdx == -1 ) continue;
   
        TLorentzVector bestTopJetLVec = type3Ptr->bestTopJetLVec;
        double bestTopJetMass = bestTopJetLVec.M();
        const double fatJetm12 = type3Ptr->bestSubMassVec[0];
        const double fatJetm13 = type3Ptr->bestSubMassVec[1];
        const double fatJetm23 = type3Ptr->bestSubMassVec[2];
   
        TLorentzVector bestbJetLVec = type3Ptr->bestbJetLVec;
        TLorentzVector bestWJetLVec = type3Ptr->bestCloseTomWLVec;
        double bestmxy = bestWJetLVec.M();
   
        template_h1_mbestTopJet_aftConeAndCats->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_mbestWJet_aftConeAndCats->Fill( bestmxy, template_evtWeight * puWeight * scaleMC ); 
        template_h1_ratiomWovermTop_aftConeAndCats->Fill( bestmxy/bestTopJetMass, template_evtWeight * puWeight * scaleMC );
        template_h2_mbestTopJetvstopJetPt->Fill( bestTopJetLVec.Pt(), bestTopJetMass, template_evtWeight * puWeight * scaleMC );
   
        if( met > defaultMETcut ){
           template_h1_mbestTopJet_aftConeAndCats_aftMET->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_mbestWJet_aftConeAndCats_aftMET->Fill( bestmxy, template_evtWeight * puWeight * scaleMC );
        }
        if( bestTopJetLVec.Pt() > 100 ){
           template_h1_mbestTopJet_aftConeAndCats_aftPt->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_mbestWJet_aftConeAndCats_aftPt->Fill( bestmxy, template_evtWeight * puWeight * scaleMC );
        }
   
        template_h2_bestmTopJetvsbestmWJet->Fill( bestmxy, bestTopJetMass, template_evtWeight * puWeight * scaleMC );

        if( ! type3Ptr->remainPassCSVS ) continue;
   
        if( type3Ptr->pickedRemainingCombfatJetIdx == -1 && template_oriJetsVec->size()>=6 ) continue;
   
        if( doTopMassCut && ! (bestTopJetMass > lowTopCut_ && bestTopJetMass < highTopCut_ ) ) continue;
     
        template_h1_remainingmTopJet_aftConeAndCats->Fill( type3Ptr->pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_remainingmWJet_aftConeAndCats->Fill( type3Ptr->pickedRemainingmxy, template_evtWeight * puWeight * scaleMC );
        template_h2_remainingmTopJetvsremainingmWJet->Fill( type3Ptr->pickedRemainingmxy, type3Ptr->pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
   
        template_h1_remainingnbJet->Fill(type3Ptr->pickedbTaggedJetIdxVec.size(), template_evtWeight * puWeight * scaleMC );
        template_h1_bestnbJet->Fill(type3Ptr->pickedbTaggedbestTopJetIdxVec.size(), template_evtWeight * puWeight * scaleMC );
        template_h1_totalnbJet->Fill(type3Ptr->pickedbTaggedJetIdxVec.size()+type3Ptr->pickedbTaggedbestTopJetIdxVec.size(), template_evtWeight * puWeight * scaleMC );
   
        TLorentzVector pickedRemainingbJetPlusAJetLVec = type3Ptr->pickedRemainingbJetPlusAJetLVec;
        TLorentzVector pickedRemainingbJetLVec = type3Ptr->pickedRemainingbJetLVec;
   
        TLorentzVector closeToMETbJetLVec = type3Ptr->closebJetLVec;

        double mTbestTopJet = type3Ptr->mTbestTopJet;
        
        double mTbJet = type3Ptr->mTbJet;
   
        double mTbestWJet = type3Ptr->mTbestWJet;
   
        double mTbestbJet = type3Ptr->mTbestbJet;
   
        double mTremainingTopJet = type3Ptr->mTremainingTopJet;
   
        double MT2 = type3Ptr->MT2;
   
        template_cntGoodFatJetPairsWeighted += template_evtWeight * puWeight;
        template_cntGoodFatJetPairsWeightedSquared += pow(template_evtWeight * puWeight, 2.0);

        if( type3Ptr->pickedRemainingmxy < lowWCut_ || type3Ptr->pickedRemainingmxy > highWCut_ ){
           if( type3Ptr->pickedClosestJetTobIdx != -1 ){
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
           template_h1_remainingmTopJet_aftConeAndCats_WithGoodWmass->Fill( type3Ptr->pickedRemainingfatJetMass, template_evtWeight * puWeight * scaleMC );
           template_h1_remainingmWJet_aftConeAndCats_WithGoodWmass->Fill( type3Ptr->pickedRemainingmxy, template_evtWeight * puWeight * scaleMC );
        }
   
        template_h1_met_aftGoodFatJetPairs->Fill(met, template_evtWeight * puWeight * scaleMC );

        double mTcomb = mTbJet + 0.5*mTbestTopJet;
//        double mTcomb = mTbJet + mTbestTopJet;
   
        if( MT2 > MT2cut_ ){
           template_cntMT2Weighted += template_evtWeight * puWeight;
           template_cntMT2WeightedSquared += pow(template_evtWeight * puWeight, 2.0);
        }
   
        if( mTcomb > mTbcut_ ){
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
        template_h1_mTbJetPlusmTbestTopJet->Fill( mTcomb, template_evtWeight * puWeight * scaleMC );
        template_h1_MT2->Fill( MT2, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbestWJet->Fill( mTbestWJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbestbJet->Fill( mTbestbJet, template_evtWeight * puWeight * scaleMC );

        bool passBaseline = passBaselineFunc(met, MT2, mTbestTopJet, mTbJet);
        bool passHighMET = passHighMETFunc(met, MT2, mTbestTopJet, mTbJet);
        bool passNewHighMET = passNewHighMETFunc(met, MT2, mTbestTopJet, mTbJet);

        if( !passBaseline ) continue;
//        if( !passHighMET ) continue;
//        if( !passNewHighMET ) continue;

//        if( doTopMassCut && !pass ) std::cout<<" not passing ... "<<std::endl;

// Final passing
        if( doSignalScanUnc && keyStringT.Contains("Signal") ){
           evtPassStopvec.back() = true;
           h2_NPassed_stdRA2[0]->Fill(massMom, massDau, Weight);
           h2_NPassed_ISRFSR[0]->Fill(massMom, massDau, Weight*ISRWeight);
           for(int ic=0; ic<nCompCTEQ; ic++){
              h2_NPassed_CTEQ[ic][0]->Fill(massMom, massDau, Weight*cteq66[ic]);
           }
           for(int im=0; im<nCompMSTW; im++){
              h2_NPassed_MSTW[im][0]->Fill(massMom, massDau, Weight*MSTW2008nlo68cl[im]);
           }
           for(int in=0; in<nCompNNPDF; in++){
              h2_NPassed_NNPDF[in][0]->Fill(massMom, massDau, Weight*NNPDF20[in]);
           }
        }
   
        if( keyStringT.Contains("Data") ){
           char outStr[200];
           sprintf(outStr, "run: %6d  lumi : %4d  event : %10d  -->  MET : %8.3f  MT2 : %8.3f  mTb : %8.3f  mTt : %8.3f", RunNum, LumiBlockNum, EvtNum, met, MT2, mTbJet, mTbestTopJet);
           evtlistFile<<outStr<<std::endl;
        }

        template_h2_mTbestWvsmTbestb->Fill( mTbestbJet, mTbestWJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetvsmTbJet->Fill( mTbJet, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetvsmTremainingHadPart->Fill( mTremainingTopJet, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetvsminDeltaPhiMETb->Fill( minDeltaPhiMETb, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbJetvsminDeltaPhiMETb->Fill( minDeltaPhiMETb, mTbJet, template_evtWeight * puWeight * scaleMC );

        template_h2_mTbestTopJetvsMT2->Fill( MT2, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbJetvsMT2->Fill( MT2, mTbJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetPlusmTbJetvsMT2->Fill( MT2, mTcomb, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETb->Fill( minDeltaPhiMETb, mTcomb, template_evtWeight * puWeight * scaleMC );
        template_h2_MT2vsminDeltaPhiMETb->Fill( minDeltaPhiMETb, MT2, template_evtWeight * puWeight * scaleMC );

        template_h2_mTbestTopJetvsMET->Fill( met, mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbJetvsMET->Fill( met, mTbJet, template_evtWeight * puWeight * scaleMC );
        template_h2_MT2vsMET->Fill( met, MT2, template_evtWeight * puWeight * scaleMC );
        template_h2_minDeltaPhiMETbvsMET->Fill( met, minDeltaPhiMETb, template_evtWeight * puWeight * scaleMC );
        template_h2_mTbestTopJetPlusmTbJetvsMET->Fill( met, mTcomb, template_evtWeight * puWeight * scaleMC );
   
        template_h1_topJetPt_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestTopJetLVec.Pt(), template_evtWeight * puWeight * scaleMC );
   
        template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestTopJetMass, template_evtWeight * puWeight * scaleMC );
        template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestmxy, template_evtWeight * puWeight * scaleMC ); 
        template_h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2Cuts->Fill( bestmxy/bestTopJetMass, template_evtWeight * puWeight * scaleMC );
   
        template_h1_met_aftMTAndMT2Cuts->Fill(met, template_evtWeight * puWeight * scaleMC );
   
        template_h1_mTbestTopJet_aftMTAndMT2Cuts->Fill( mTbestTopJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJet_aftMTAndMT2Cuts->Fill( mTbJet, template_evtWeight * puWeight * scaleMC );
        template_h1_mTbJetPlusmTbestTopJet_aftMTAndMT2Cuts->Fill( mTcomb, template_evtWeight * puWeight * scaleMC );
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
  h1_topJetPt_aftConeAndCats_aftMETVec.push_back((TH1D*)template_h1_topJetPt_aftConeAndCats_aftMET->Clone());
  h1_topJetPt_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_topJetPt_aftConeAndCats_aftMTAndMT2Cuts->Clone());

  h1_mfatJet_aftConeVec.push_back((TH1D*)template_h1_mfatJet_aftCone->Clone());
  h1_m12_aftConeVec.push_back((TH1D*)template_h1_m12_aftCone->Clone());
  h1_m13_aftConeVec.push_back((TH1D*)template_h1_m13_aftCone->Clone());
  h1_m23_aftConeVec.push_back((TH1D*)template_h1_m23_aftCone->Clone());

  h1_mbestTopJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats->Clone());
  h1_mbestWJet_aftConeAndCatsVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats->Clone());
  h1_ratiomWovermTop_aftConeAndCatsVec.push_back((TH1D*)template_h1_ratiomWovermTop_aftConeAndCats->Clone());

  h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mbestTopJet_aftConeAndCats_aftMTAndMT2Cuts->Clone());
  h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_mbestWJet_aftConeAndCats_aftMTAndMT2Cuts->Clone());
  h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2CutsVec.push_back((TH1D*)template_h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2Cuts->Clone());

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
  h1_nJets_bfrBaselineVec.push_back((TH1D*)template_h1_nJets_bfrBaseline->Clone());
  h1_nJetsVec.push_back((TH1D*)template_h1_nJets->Clone());
  h1_vtxSizeVec.push_back((TH1D*)template_h1_vtxSize->Clone());
  h1_vtxSize_noPUreWeightingVec.push_back((TH1D*)template_h1_vtxSize_noPUreWeighting->Clone());

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
  h2_mTbestTopJetvsmTremainingHadPartVec.push_back((TH2D*)template_h2_mTbestTopJetvsmTremainingHadPart->Clone());

  h2_mTbestTopJetvsminDeltaPhiMETbVec.push_back((TH2D*)template_h2_mTbestTopJetvsminDeltaPhiMETb->Clone());
  h2_mTbJetvsminDeltaPhiMETbVec.push_back((TH2D*)template_h2_mTbJetvsminDeltaPhiMETb->Clone());

  h2_mTbestTopJetvsMT2Vec.push_back((TH2D*)template_h2_mTbestTopJetvsMT2->Clone());
  h2_mTbJetvsMT2Vec.push_back((TH2D*)template_h2_mTbJetvsMT2->Clone());
  h2_mTbestTopJetPlusmTbJetvsMT2Vec.push_back((TH2D*)template_h2_mTbestTopJetPlusmTbJetvsMT2->Clone());
  h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETbVec.push_back((TH2D*)template_h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETb->Clone());
  h2_MT2vsminDeltaPhiMETbVec.push_back((TH2D*)template_h2_MT2vsminDeltaPhiMETb->Clone());

  h2_mTbestTopJetvsMETVec.push_back((TH2D*)template_h2_mTbestTopJetvsMET->Clone());
  h2_mTbJetvsMETVec.push_back((TH2D*)template_h2_mTbJetvsMET->Clone());
  h2_MT2vsMETVec.push_back((TH2D*)template_h2_MT2vsMET->Clone());
  h2_minDeltaPhiMETbvsMETVec.push_back((TH2D*)template_h2_minDeltaPhiMETbvsMET->Clone());
  h2_mTbestTopJetPlusmTbJetvsMETVec.push_back((TH2D*)template_h2_mTbestTopJetPlusmTbJetvsMET->Clone());

  h2_mbestTopJetvstopJetPtVec.push_back((TH2D*)template_h2_mbestTopJetvstopJetPt->Clone());
}

void plotsType3TopTagger(){

  std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

  picker = new TRandom3(glbfSeed);

  type3Ptr = new topTagger::type3TopTagger();

  type3Ptr->setdoInvertTopCriteria(doInvertTopCriteria);

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

  std::vector<TTree*> treeVec;
  std::vector<std::string> subSampleKeysVec;

  char filenames[200], names[200];
  std::vector<std::string> filesDataVec;
  std::vector<std::string> filesTTJetsVec, filesWJetsToLNu_400HTinfVec, filesWJetsToLNu_300HT400Vec, filesWJetsToLNu_250HT300Vec, filesZinv_400HTinfVec, filesZinv_200HT400Vec, filesQCD_1000HTinfVec, filesQCD_500HT1000Vec, filesQCD_250HT500Vec, filesT_tWVec;

  ifstream finData("rootlist_Data.txt"); while( finData.getline(filenames, 200) ){ filesDataVec.push_back(filenames); }
  ifstream finTTJets("rootlist_TTJets.txt"); while( finTTJets.getline(filenames, 200) ){ filesTTJetsVec.push_back(filenames); }
  ifstream finWJetsToLNu_400HTinf("rootlist_WJetsToLNu_400HTinf.txt"); while( finWJetsToLNu_400HTinf.getline(filenames, 200) ){ filesWJetsToLNu_400HTinfVec.push_back(filenames); }
  ifstream finWJetsToLNu_300HT400("rootlist_WJetsToLNu_300HT400.txt"); while( finWJetsToLNu_300HT400.getline(filenames, 200) ){ filesWJetsToLNu_300HT400Vec.push_back(filenames); }
  ifstream finWJetsToLNu_250HT300("rootlist_WJetsToLNu_250HT300.txt"); while( finWJetsToLNu_250HT300.getline(filenames, 200) ){ filesWJetsToLNu_250HT300Vec.push_back(filenames); }
  ifstream finZinv_400HTinf("rootlist_Zinv_400HTinf.txt"); while( finZinv_400HTinf.getline(filenames, 200) ){ filesZinv_400HTinfVec.push_back(filenames); }
  ifstream finZinv_200HT400("rootlist_Zinv_200HT400.txt"); while( finZinv_200HT400.getline(filenames, 200) ){ filesZinv_200HT400Vec.push_back(filenames); }
  ifstream finQCD_1000HTinf("rootlist_QCD_1000HTinf.txt"); while( finQCD_1000HTinf.getline(filenames, 200) ){ filesQCD_1000HTinfVec.push_back(filenames); }
  ifstream finQCD_500HT1000("rootlist_QCD_500HT1000.txt"); while( finQCD_500HT1000.getline(filenames, 200) ){ filesQCD_500HT1000Vec.push_back(filenames); }
  ifstream finQCD_250HT500("rootlist_QCD_250HT500.txt"); while( finQCD_250HT500.getline(filenames, 200) ){ filesQCD_250HT500Vec.push_back(filenames); }
  ifstream finT_tW("rootlist_T_tW.txt"); while( finT_tW.getline(filenames, 200) ){ filesT_tWVec.push_back(filenames); }

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
  std::cout<<"\nProcessing Data ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Data_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesDataVec.size(); in++){ Data_AUX->Add(filesDataVec.at(in).c_str()); }
  treeVec.push_back(Data_AUX); subSampleKeysVec.push_back("Data");
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
  std::cout<<"\nProcessing TTJets ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *TTJets_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesTTJetsVec.size(); in++){ TTJets_AUX->Add(filesTTJetsVec.at(in).c_str()); }
  treeVec.push_back(TTJets_AUX); subSampleKeysVec.push_back("TTbar");
  templatePlotsFunc(treeVec, subSampleKeysVec, "TTbar");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

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
  std::cout<<"\nProcessing WJetsToLNu ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *WJetsToLNu_400HTinf_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesWJetsToLNu_400HTinfVec.size(); in++){ WJetsToLNu_400HTinf_AUX->Add(filesWJetsToLNu_400HTinfVec.at(in).c_str()); }
  treeVec.push_back(WJetsToLNu_400HTinf_AUX); subSampleKeysVec.push_back("WJetsToLNu_400HTinf");
  TChain *WJetsToLNu_300HT400_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesWJetsToLNu_300HT400Vec.size(); in++){ WJetsToLNu_300HT400_AUX->Add(filesWJetsToLNu_300HT400Vec.at(in).c_str()); }
  treeVec.push_back(WJetsToLNu_300HT400_AUX); subSampleKeysVec.push_back("WJetsToLNu_300HT400");
  TChain *WJetsToLNu_250HT300_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesWJetsToLNu_250HT300Vec.size(); in++){ WJetsToLNu_250HT300_AUX->Add(filesWJetsToLNu_250HT300Vec.at(in).c_str()); }
  treeVec.push_back(WJetsToLNu_250HT300_AUX); subSampleKeysVec.push_back("WJetsToLNu_250HT300");
  templatePlotsFunc(treeVec, subSampleKeysVec, "WJetsToLNu");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

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
  std::cout<<"\nProcessing Zinv ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Zinv_400HTinf_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesZinv_400HTinfVec.size(); in++){ Zinv_400HTinf_AUX->Add(filesZinv_400HTinfVec.at(in).c_str()); }
  treeVec.push_back(Zinv_400HTinf_AUX); subSampleKeysVec.push_back("Zinv_400HTinf");
  TChain *Zinv_200HT400_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesZinv_200HT400Vec.size(); in++){ Zinv_200HT400_AUX->Add(filesZinv_200HT400Vec.at(in).c_str()); }
  treeVec.push_back(Zinv_200HT400_AUX); subSampleKeysVec.push_back("Zinv_200HT400");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Zinv");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

// QCD
  std::cout<<"\nProcessing QCD ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *QCD_1000HTinf_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesQCD_1000HTinfVec.size(); in++){ QCD_1000HTinf_AUX->Add(filesQCD_1000HTinfVec.at(in).c_str()); }
  treeVec.push_back(QCD_1000HTinf_AUX); subSampleKeysVec.push_back("QCD_1000HTinf");
  TChain *QCD_500HT1000_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesQCD_500HT1000Vec.size(); in++){ QCD_500HT1000_AUX->Add(filesQCD_500HT1000Vec.at(in).c_str()); }
  treeVec.push_back(QCD_500HT1000_AUX); subSampleKeysVec.push_back("QCD_500HT1000");
  TChain *QCD_250HT500_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesQCD_250HT500Vec.size(); in++){ QCD_250HT500_AUX->Add(filesQCD_250HT500Vec.at(in).c_str()); }
  treeVec.push_back(QCD_250HT500_AUX); subSampleKeysVec.push_back("QCD_250HT500");
  templatePlotsFunc(treeVec, subSampleKeysVec, "QCD");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

  sumSM_cntAftBaselineMinusMETWeightedScaledMC += template_cntAftBaselineMinusMETWeightedScaledMC; sumSM_cntAftBaselineMinusMETWeightedErrorScaledMC += template_cntAftBaselineMinusMETWeightedErrorScaledMC * template_cntAftBaselineMinusMETWeightedErrorScaledMC;
  sumSM_cntAftBaselineWeightedScaledMC += template_cntAftBaselineWeightedScaledMC; sumSM_cntAftBaselineWeightedErrorScaledMC += template_cntAftBaselineWeightedErrorScaledMC * template_cntAftBaselineWeightedErrorScaledMC;
  sumSM_cntAftPreCutsWeightedScaledMC += template_cntAftPreCutsWeightedScaledMC; sumSM_cntAftPreCutsWeightedErrorScaledMC += template_cntAftPreCutsWeightedErrorScaledMC * template_cntAftPreCutsWeightedErrorScaledMC;
  sumSM_cntGoodFatJetPairsWeightedScaledMC += template_cntGoodFatJetPairsWeightedScaledMC; sumSM_cntGoodFatJetPairsWeightedErrorScaledMC += template_cntGoodFatJetPairsWeightedErrorScaledMC * template_cntGoodFatJetPairsWeightedErrorScaledMC;
  sumSM_cntMT2WeightedScaledMC += template_cntMT2WeightedScaledMC; sumSM_cntMT2WeightedErrorScaledMC += template_cntMT2WeightedErrorScaledMC * template_cntMT2WeightedErrorScaledMC;
  sumSM_cntmTbWeightedScaledMC += template_cntmTbWeightedScaledMC; sumSM_cntmTbWeightedErrorScaledMC += template_cntmTbWeightedErrorScaledMC * template_cntmTbWeightedErrorScaledMC;
  sumSM_cntmTtWeightedScaledMC += template_cntmTtWeightedScaledMC; sumSM_cntmTtWeightedErrorScaledMC += template_cntmTtWeightedErrorScaledMC * template_cntmTtWeightedErrorScaledMC;
  sumSM_cntmTWWeightedScaledMC += template_cntmTWWeightedScaledMC; sumSM_cntmTWWeightedErrorScaledMC += template_cntmTWWeightedErrorScaledMC * template_cntmTWWeightedErrorScaledMC;
  sumSM_cntEventsWeightedScaledMC += template_cntEventsWeightedScaledMC; sumSM_cntEventsWeightedErrorScaledMC += template_cntEventsWeightedErrorScaledMC * template_cntEventsWeightedErrorScaledMC;

// T_tW
  std::cout<<"\nProcessing T_tW ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *T_tW_AUX = new TChain(treeStrT+"/RA2PreSelection");
  for(unsigned int in=0; in<filesT_tWVec.size(); in++){ T_tW_AUX->Add(filesT_tWVec.at(in).c_str()); }
  treeVec.push_back(T_tW_AUX); subSampleKeysVec.push_back("T_tW");
  templatePlotsFunc(treeVec, subSampleKeysVec, "T_tW");

  std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

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

// For signals ...

  bool cacheddoSignalScanUnc = doSignalScanUnc; doSignalScanUnc = false;

// Signal mStop550_mLSP50
  TFile *Signal_mStop550_mLSP50_File = new TFile("T2tt/scanTree_T2tt_mother550_LSP50_06Mar2013.root");
  TTree *Signal_mStop550_mLSP50_AUX = (TTree*) Signal_mStop550_mLSP50_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop550_mLSP50_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop550_mLSP50");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop550_mLSP50");
  double signif_mStop550_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop550_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop550_mLSP50, signif_sysErr_mStop550_mLSP50);

// Signal mStop600_mLSP50
  TFile *Signal_mStop600_mLSP50_File = new TFile("T2tt/scanTree_T2tt_mother600_LSP50_06Mar2013.root");
  TTree *Signal_mStop600_mLSP50_AUX = (TTree*) Signal_mStop600_mLSP50_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop600_mLSP50_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop600_mLSP50");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop600_mLSP50");
  double signif_mStop600_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop600_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop600_mLSP50, signif_sysErr_mStop600_mLSP50);

// Signal mStop650_mLSP50
  TFile *Signal_mStop650_mLSP50_File = new TFile("T2tt/scanTree_T2tt_mother650_LSP50_06Mar2013.root");
  TTree *Signal_mStop650_mLSP50_AUX = (TTree*) Signal_mStop650_mLSP50_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop650_mLSP50_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop650_mLSP50");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop650_mLSP50");
  double signif_mStop650_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop650_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop650_mLSP50, signif_sysErr_mStop650_mLSP50);

// Signal mStop700_mLSP50
  TFile *Signal_mStop700_mLSP50_File = new TFile("T2tt/scanTree_T2tt_mother700_LSP50_06Mar2013.root");
  TTree *Signal_mStop700_mLSP50_AUX = (TTree*) Signal_mStop700_mLSP50_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop700_mLSP50_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop700_mLSP50");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop700_mLSP50");
  double signif_mStop700_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop700_mLSP50 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop700_mLSP50, signif_sysErr_mStop700_mLSP50);

// Signal mStop650_mLSP0
  TFile *Signal_mStop650_mLSP0_File = new TFile("T2tt/scanTree_T2tt_mother650_LSP0_06Mar2013.root");
  TTree *Signal_mStop650_mLSP0_AUX = (TTree*) Signal_mStop650_mLSP0_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop650_mLSP0_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop650_mLSP0");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop650_mLSP0");
  double signif_mStop650_mLSP0 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop650_mLSP0 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop650_mLSP0, signif_sysErr_mStop650_mLSP0);

// Signal mStop700_mLSP0
  TFile *Signal_mStop700_mLSP0_File = new TFile("T2tt/scanTree_T2tt_mother700_LSP0_06Mar2013.root");
  TTree *Signal_mStop700_mLSP0_AUX = (TTree*) Signal_mStop700_mLSP0_File->Get(treeStrT+"/RA2PreSelection");
  treeVec.clear(); treeVec.push_back(Signal_mStop700_mLSP0_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back("Signal_mStop700_mLSP0");
  templatePlotsFunc(treeVec, subSampleKeysVec, "Signal_mStop700_mLSP0");
  double signif_mStop700_mLSP0 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
  double signif_sysErr_mStop700_mLSP0 = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
  printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_mStop700_mLSP0, signif_sysErr_mStop700_mLSP0);

// Standard RA2
  h2_NTotal = new TH2D("NTotal", "NTotal", 60, 0, 1500, 60, 0, 1500); h2_NTotal->Sumw2();
  TString tmpKeyStrT = keyStrForTheStage;
  TString tmpKeyStrNPassedT = "NPassed_"+tmpKeyStrT;
  TH2D * h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
  h2_NPassed_stdRA2.push_back((TH2D*)h2_tmp->Clone());
// For PDF sys.
  for(int ic=0; ic<nCompCTEQ; ic++){
     sprintf(names, "PDFWgt1_NTotal_%d", ic);
     TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
     h2_NTotal_CTEQ.push_back((TH2D*)h2_tmpTotal->Clone());
     std::vector<TH2D*> h2_tmpVec;
     sprintf(names, "PDFWgt1_NPassed_%d_", ic);
     tmpKeyStrNPassedT = names+tmpKeyStrT;
     TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
     h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
     h2_NPassed_CTEQ.push_back(h2_tmpVec);
  }

  for(int im=0; im<nCompMSTW; im++){
     sprintf(names, "PDFWgt2_NTotal_%d", im);
     TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
     h2_NTotal_MSTW.push_back((TH2D*)h2_tmpTotal->Clone());
     std::vector<TH2D*> h2_tmpVec;
     sprintf(names, "PDFWgt2_NPassed_%d_", im);
     tmpKeyStrNPassedT = names+tmpKeyStrT;
     TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
     h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
     h2_NPassed_MSTW.push_back(h2_tmpVec);
  }

  for(int in=0; in<nCompNNPDF; in++){
     sprintf(names, "PDFWgt3_NTotal_%d", in);
     TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
     h2_NTotal_NNPDF.push_back((TH2D*)h2_tmpTotal->Clone());
     std::vector<TH2D*> h2_tmpVec;
     sprintf(names, "PDFWgt3_NPassed_%d_", in);
     tmpKeyStrNPassedT = names+tmpKeyStrT;
     TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
     h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
     h2_NPassed_NNPDF.push_back(h2_tmpVec);
  }

// ISR/FSR
  h2_NTotal_ISRFSR = new TH2D("NTotal_ISRFSR", "NTotal_ISRFSR", 60, 0, 1500, 60, 0, 1500); h2_NTotal_ISRFSR->Sumw2();
  tmpKeyStrNPassedT = "NPassed_ISRFSR_"+tmpKeyStrT;
  h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
  h2_NPassed_ISRFSR.push_back((TH2D*)h2_tmp->Clone());

// JES UP/DN
  h2_NTotal_JESUP = new TH2D("NTotal_JESUP", "NTotal_JESUP", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JESUP->Sumw2();
  h2_NTotal_JESDN = new TH2D("NTotal_JESDN", "NTotal_JESDN", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JESDN->Sumw2();
  tmpKeyStrNPassedT = "NPassed_JESUP_"+tmpKeyStrT;
  TH2D * h2_tmp_jesup = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jesup->Sumw2();
  h2_NPassed_JESUP.push_back((TH2D*)h2_tmp_jesup->Clone());

  tmpKeyStrNPassedT = "NPassed_JESDN_"+tmpKeyStrT;
  TH2D * h2_tmp_jesdn = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jesdn->Sumw2();
  h2_NPassed_JESDN.push_back((TH2D*)h2_tmp_jesdn->Clone());

// JER UP/DN
  h2_NTotal_JER   = new TH2D("NTotal_JER", "NTotal_JER", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JER->Sumw2();
  h2_NTotal_JERUP = new TH2D("NTotal_JERUP", "NTotal_JERUP", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JERUP->Sumw2();
  h2_NTotal_JERDN = new TH2D("NTotal_JERDN", "NTotal_JERDN", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JERDN->Sumw2();
  tmpKeyStrNPassedT = "NPassed_JER_"+tmpKeyStrT;
  TH2D * h2_tmp_jer = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jer->Sumw2();
  h2_NPassed_JER.push_back((TH2D*)h2_tmp_jer->Clone());

  tmpKeyStrNPassedT = "NPassed_JERUP_"+tmpKeyStrT;
  TH2D * h2_tmp_jerup = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jerup->Sumw2();
  h2_NPassed_JERUP.push_back((TH2D*)h2_tmp_jerup->Clone());

  tmpKeyStrNPassedT = "NPassed_JERDN_"+tmpKeyStrT;
  TH2D * h2_tmp_jerdn = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jerdn->Sumw2();
  h2_NPassed_JERDN.push_back((TH2D*)h2_tmp_jerdn->Clone());

  vector<double> signifVec, signif_sysErrVec;
  vector<double> mStopVec, mLSPVec;

  doSignalScanUnc = cacheddoSignalScanUnc;

  if( doMoreSignals ){
     char signalKeyStr[200];

     for(int is=0; is<nStopPts; is++){
        int perStop = minmStop + is*nStepmStop;
        for(int il=0; il<nLSPPts; il++){
           int perLSP = minmLSP + il*nStepmLSP;
//           if( perLSP > perStop - 200 ) continue;

           sprintf(signalKeyStr, "mStop%d_mLSP%d", perStop, perLSP);
           TString signalKeyStrT(signalKeyStr), signalKeyStrTwtSignal = "Signal_"+signalKeyStrT;

           bool isFound = false;
           for(int imc=0; imc<nMC; imc++){
              TString perMCStrT(mcStr[imc]);
              if( perMCStrT == signalKeyStrTwtSignal ){ isFound = true; break; }
           }
           if( !isFound ) continue;

           mStopVec.push_back(perStop); mLSPVec.push_back(perLSP);

           sprintf(filenames, "T2tt/scanTree_T2tt_mother%d_LSP%d_06Mar2013.root", perStop, perLSP);
            
           TFile *Signal_tmp_File = new TFile(filenames);
           TTree *Signal_tmp_AUX = (TTree*) Signal_tmp_File->Get(treeStrT+"/RA2PreSelection");
           treeVec.clear(); treeVec.push_back(Signal_tmp_AUX); subSampleKeysVec.clear(); subSampleKeysVec.push_back(signalKeyStrTwtSignal.Data());

           evtIDvec.clear(); evtPassBasicvec.clear(); evtPassStopvec.clear();

           templatePlotsFunc(treeVec, subSampleKeysVec, signalKeyStrTwtSignal.Data());
           double signif_tmp = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC);
           double signif_sysErr_tmp = template_cntEventsWeightedScaledMC/sqrt(template_cntEventsWeightedScaledMC + sumSM_cntEventsWeightedScaledMC + sysErrSig*template_cntEventsWeightedScaledMC*sysErrSig*template_cntEventsWeightedScaledMC + sysErrBkg*sumSM_cntEventsWeightedScaledMC*sysErrBkg*sumSM_cntEventsWeightedScaledMC + 3/2);
           printf("  S/sqrt(S+B) :  %5.3f  S/sqrt(S+B+dS^2+dB^2+3/2) :  %5.3f", signif_tmp, signif_sysErr_tmp);
           signifVec.push_back(signif_tmp); signif_sysErrVec.push_back(signif_sysErr_tmp);
        }
     }
  }

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

  tdrStyle->SetOptStat(1111);
 
  tdrStyle->SetHistLineWidth(1);

  tdrStyle->SetPaintTextFormat("4.2f");

  TCanvas *c1sgnf = new TCanvas("c1sgnf", "c1sgnf", 1600, 800);
  c1sgnf->Divide(2, 1);

// Draw the significance plots
  TH2D *h2_sgnfDefault = new TH2D("sgnfDefault", "Significance: S/Sqrt(S+B) in 20fb^{-1}; m_{#tilde{t}} [GeV]; m_{LSP} [GeV]", nStopPts, minmStop-12.5, maxmStop+12.5, nLSPPts, minmLSP-12.5, maxmLSP+12.5);  
  TH2D *h2_sgnfSysErr = new TH2D("sgnfSysErr", "Significance: S/Sqrt(S+B+dS^{2}+dB^{2}+3/2) in 20fb^{-1}; m_{#tilde{t}} [GeV]; m_{LSP} [GeV]", nStopPts, minmStop-12.5, maxmStop+12.5, nLSPPts, minmLSP-12.5, maxmLSP+12.5);

  for(int ip=0; ip<(int)mStopVec.size(); ip++){
     double perStop = mStopVec[ip];
     double perLSP  = mLSPVec[ip];
     
     double signif  = signifVec[ip];
     double signif_sysErr = signif_sysErrVec[ip];

     h2_sgnfDefault->Fill(perStop, perLSP, signif*signifScaleToFullLumi);
     h2_sgnfSysErr->Fill(perStop, perLSP, signif_sysErr*signifScaleToFullLumi);
  }

  c1sgnf->cd(1);
  h2_sgnfDefault->SetStats(kFALSE);
  h2_sgnfDefault->GetZaxis()->SetRangeUser(0, 12.0);
  h2_sgnfDefault->DrawCopy("colz");
  h2_sgnfDefault->DrawCopy("text same");

  c1sgnf->cd(2);
  h2_sgnfSysErr->SetStats(kFALSE);
  h2_sgnfSysErr->GetZaxis()->SetRangeUser(0, 8.0);
  h2_sgnfSysErr->DrawCopy("colz");
  h2_sgnfSysErr->DrawCopy("text same");

  c1sgnf->Print("sgnf_allINone_"+treeStrT+".pdf");

  tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

  unsigned int ntype = keyStringCachedVec.size();
  std::cout<<"\nntype : "<<ntype<<std::endl;

  TCanvas *cs = new TCanvas("cs", "cs", 1200, 800);
  int divW=4, divH=3;
  cs->Divide(divW, divH);

  cs->Print("allINone_"+treeStrT+".pdf[");
 
  draw2DallINone(cs, divW*divH, h2_m23overm123vsarctanm13overm12Vec); cs->Print("allINone_"+treeStrT+".pdf");

  if( doEffMeasure ){
     draw1DallINone(cs, divW*divH, h1_totGenHadTopPtVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_totGenHadTopPt_aftMETVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_rndmGenHadTopPtVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_aftTaggerGenHadTopPtVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_minDRtopTaggerMatchVec, 5, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_effTopTagvsPtVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_effTopTagvsPt_aftmaxDRVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_effmaxDRmatchvsPtVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw2DallINone(cs, divW*divH, h2_aftTaggerGenHadTopPtvsRecoTopPtVec); cs->Print("allINone_"+treeStrT+".pdf");

     draw1DallINone(cs, divW*divH, h1_deltaRBetweenTwoGenTopsVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw1DallINone(cs, divW*divH, h1_deltaRBetweenTwoAllHadGenTopsVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
     draw2DallINone(cs, divW*divH, h2_deltaRBetweenTwoGenTopsvsAvgTopPtVec); cs->Print("allINone_"+treeStrT+".pdf");
     draw2DallINone(cs, divW*divH, h2_deltaRBetweenTwoAllHadGenTopsvsAvgTopPtVec); cs->Print("allINone_"+treeStrT+".pdf");
  }

  draw1DallINone(cs, divW*divH, h1_mfatJet_aftConeVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nCombInAConeVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nTopCandVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nTopCandWithBtagVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nJets_bfrBaselineVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_nJetsVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_vtxSizeVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_vtxSize_noPUreWeightingVec, 1, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

  draw1DallINone(cs, divW*divH, h1_mbestTopJet_aftConeAndCatsVec, 5, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestWJet_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_ratiomWovermTop_aftConeAndCatsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_topJetPt_aftConeAndCatsVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_topJetPt_aftConeAndCats_aftMETVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_topJetPt_aftConeAndCats_aftMTAndMT2CutsVec, 5, "SetLogy " + extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec, 5, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_mbestWJet_aftConeAndCats_aftMTAndMT2CutsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");
  draw1DallINone(cs, divW*divH, h1_ratiomWovermTop_aftConeAndCats_aftMTAndMT2CutsVec, 4, extraDrawStrT); cs->Print("allINone_"+treeStrT+".pdf");

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

  draw2DallINone(cs, divW*divH, h2_mbestTopJetvstopJetPtVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_bestmTopJetvsbestmWJetVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_remainingmTopJetvsremainingmWJetVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestWvsmTbestbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsmTbJetVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsmTremainingHadPartVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsminDeltaPhiMETbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbJetvsminDeltaPhiMETbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsMT2Vec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbJetvsMT2Vec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetPlusmTbJetvsMT2Vec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetPlusmTbJetvsminDeltaPhiMETbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_MT2vsminDeltaPhiMETbVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetvsMETVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbJetvsMETVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_MT2vsMETVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_minDeltaPhiMETbvsMETVec); cs->Print("allINone_"+treeStrT+".pdf");
  draw2DallINone(cs, divW*divH, h2_mTbestTopJetPlusmTbJetvsMETVec); cs->Print("allINone_"+treeStrT+".pdf");

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

//  dataDrivenZinv();
/*
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
*/

  if( doSignalScanUnc ){
     TFile *prodSignalFile = new TFile("prodSignal.root", "RECREATE");
     prodSignalFile->cd();
     TDirectory *stdDir = prodSignalFile->mkdir("STD");
     TDirectory *jesDir = prodSignalFile->mkdir("JES");
     TDirectory *jerDir = prodSignalFile->mkdir("JER");
     TDirectory *isrDir = prodSignalFile->mkdir("ISR");
     TDirectory *pdfDir = prodSignalFile->mkdir("PDF");

     stdDir->cd();
     h2_NTotal->Write(); h2_NPassed_stdRA2[0]->Write();

     jesDir->cd();
     h2_NTotal_JESUP->Write(); h2_NTotal_JESDN->Write();
     h2_NPassed_JESUP[0]->Write(); h2_NPassed_JESDN[0]->Write();

     jerDir->cd();
     h2_NTotal_JER->Write(); h2_NTotal_JERUP->Write(); h2_NTotal_JERDN->Write();
     h2_NPassed_JER[0]->Write(); h2_NPassed_JERUP[0]->Write(); h2_NPassed_JERDN[0]->Write();

     isrDir->cd();
     h2_NTotal_ISRFSR->Write();
     h2_NPassed_ISRFSR[0]->Write();

     pdfDir->cd();
     for(int ic=0; ic<nCompCTEQ; ic++){
        h2_NTotal_CTEQ[ic]->Write();
        h2_NPassed_CTEQ[ic][0]->Write();
     }
     for(int im=0; im<nCompMSTW; im++){
        h2_NTotal_MSTW[im]->Write();
        h2_NPassed_MSTW[im][0]->Write();
     }
     for(int in=0; in<nCompNNPDF; in++){
        h2_NTotal_NNPDF[in]->Write();
        h2_NPassed_NNPDF[in][0]->Write();
     }
     prodSignalFile->Write(); prodSignalFile->Close();
  }

}

// Handle my default value of trunpv==-1, this is the case for data.
double weightTruNPV(int trunpv){
      
   double w = 1.;
   if (trunpv < static_cast<int> (puWeights_.size()) && trunpv >=0) {
      w = puWeights_.at(trunpv);
   } else if( trunpv >= static_cast<int> (puWeights_.size()) ) {
      std::cerr << "WARNING ... number of PU vertices = " << trunpv
            << " out of histogram binning." << std::endl;
   }  
      
   return w;
}     
      
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName){

   puWeights_.clear();

   TFile *puDatafile = new TFile(puDatafileName.c_str());
   TH1D *data_npu_estimated = (TH1D*) puDatafile->Get(puDatahistName.c_str());

   double Summer12_S10[60] = {
      2.560E-06,
      5.239E-06,
      1.420E-05,
      5.005E-05,
      1.001E-04,
      2.705E-04,
      1.999E-03,
      6.097E-03,
      1.046E-02,
      1.383E-02,
      1.685E-02,
      2.055E-02,
      2.572E-02,
      3.262E-02,
      4.121E-02,
      4.977E-02,
      5.539E-02,
      5.725E-02,
      5.607E-02,
      5.312E-02,
      5.008E-02,
      4.763E-02,
      4.558E-02,
      4.363E-02,
      4.159E-02,
      3.933E-02,
      3.681E-02,
      3.406E-02,
      3.116E-02,
      2.818E-02,
      2.519E-02,
      2.226E-02,
      1.946E-02,
      1.682E-02,
      1.437E-02,
      1.215E-02,
      1.016E-02,
      8.400E-03,
      6.873E-03,
      5.564E-03,
      4.457E-03,
      3.533E-03,
      2.772E-03,
      2.154E-03,
      1.656E-03,
      1.261E-03,
      9.513E-04,
      7.107E-04,
      5.259E-04,
      3.856E-04,
      2.801E-04,
      2.017E-04,
      1.439E-04,
      1.017E-04,
      7.126E-05,
      4.948E-05,
      3.405E-05,
      2.322E-05,
      1.570E-05,
      5.005E-06
   };

   puWeights_.resize(60);
   double s = 0.0;
   for (int npu = 0; npu < 60; ++npu) {
      double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));
      puWeights_[npu] = npu_estimated / Summer12_S10[npu];
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
        tmpHist->SetAxisRange(1e-1, tmpHist->GetMaximum()*32*5, "Y");
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

  TPad *lastPad = (TPad*) cs->GetPad(lastPadIdx);
  lastPad->RedrawAxis();

}


void dataDrivenZinv(){

  TCanvas *cs = new TCanvas("cs", "cs", 1200, 800);
  int divW=4, divH=3;
  cs->Divide(divW, divH);

  unsigned int ntype = keyStringCachedVec.size();

// Study the mass sideband method
  int pickedSignalIdx = -1, pickedTTbarIdx = -1, pickedZinvIdx = -1, pickedWJetsIdx = -1;
  for(unsigned int it=0; it<ntype; it++){
           
     TString keyStringT(keyStringCachedVec[it]);

     if( keyStringT == "TTbar" ) pickedTTbarIdx = (int)it;
     if( keyStringT == "Zinv" ) pickedZinvIdx = (int)it;
     if( keyStringT == "Signal_x500_y100" ) pickedSignalIdx = (int)it;
     if( keyStringT == "WJetsToLNu" ) pickedWJetsIdx = (int)it;
  }

  int nRebinForZinv = 5;

  TH1D* h1_mBestTopJet_TTbar = (TH1D*) h1_mbestTopJet_aftConeAndCatsVec[pickedTTbarIdx]->Clone(); h1_mBestTopJet_TTbar->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_Zinv = (TH1D*) h1_mbestTopJet_aftConeAndCatsVec[pickedZinvIdx]->Clone(); h1_mBestTopJet_Zinv->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_Signal = (TH1D*) h1_mbestTopJet_aftConeAndCatsVec[pickedSignalIdx]->Clone(); h1_mBestTopJet_Signal->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_WJets = (TH1D*) h1_mbestTopJet_aftConeAndCatsVec[pickedWJetsIdx]->Clone(); h1_mBestTopJet_WJets->Rebin(nRebinForZinv);

  TH1D* h1_mBestTopJet_TTbar_aftMTAndMT2Cuts = (TH1D*) h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec[pickedTTbarIdx]->Clone(); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_Zinv_aftMTAndMT2Cuts = (TH1D*) h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec[pickedZinvIdx]->Clone(); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_Signal_aftMTAndMT2Cuts = (TH1D*) h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec[pickedSignalIdx]->Clone(); h1_mBestTopJet_Signal_aftMTAndMT2Cuts->Rebin(nRebinForZinv);
  TH1D* h1_mBestTopJet_WJets_aftMTAndMT2Cuts = (TH1D*) h1_mbestTopJet_aftConeAndCats_aftMTAndMT2CutsVec[pickedWJetsIdx]->Clone(); h1_mBestTopJet_WJets_aftMTAndMT2Cuts->Rebin(nRebinForZinv);

  TH1D* h1_mBestTopJet_TTbar_Cloned = (TH1D*) h1_mBestTopJet_TTbar->Clone(); h1_mBestTopJet_TTbar_Cloned->Scale(1.0/h1_mBestTopJet_TTbar_Cloned->Integral());
  TH1D* h1_mBestTopJet_Zinv_Cloned = (TH1D*) h1_mBestTopJet_Zinv->Clone(); h1_mBestTopJet_Zinv_Cloned->Scale(1.0/h1_mBestTopJet_Zinv_Cloned->Integral());
  TH1D* h1_mBestTopJet_Signal_Cloned = (TH1D*) h1_mBestTopJet_Signal->Clone(); h1_mBestTopJet_Signal_Cloned->Scale(1.0/h1_mBestTopJet_Signal_Cloned->Integral());
  TH1D* h1_mBestTopJet_WJets_Cloned = (TH1D*) h1_mBestTopJet_WJets->Clone(); h1_mBestTopJet_WJets_Cloned->Scale(1.0/h1_mBestTopJet_WJets_Cloned->Integral());
 
  TH1D* h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned = (TH1D*) h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->Clone(); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->Scale(1.0/h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->Integral());
  TH1D* h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned = (TH1D*) h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->Clone(); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->Scale(1.0/h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->Integral());
  TH1D* h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned = (TH1D*) h1_mBestTopJet_Signal_aftMTAndMT2Cuts->Clone(); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->Scale(1.0/h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->Integral());
  TH1D* h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned = (TH1D*) h1_mBestTopJet_WJets_aftMTAndMT2Cuts->Clone(); h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->Scale(1.0/h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->Integral());
 
  cs->Print("ZinvEstimation_"+treeStrT+".pdf[");
  TPad *padZinv01 =0, *padZinv02 = 0, *padZinv03 =0, *padZinv04 =0;

  cs->cd(1); padZinv01 = (TPad*) cs->GetPad(1); padZinv01->Clear();
  h1_mBestTopJet_TTbar->SetAxisRange(0, 1.5*h1_mBestTopJet_TTbar->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar->SetLineColor(kBlue); h1_mBestTopJet_TTbar->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar->SetMarkerStyle(6); h1_mBestTopJet_TTbar->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar->Draw("hist");
  h1_mBestTopJet_Zinv->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv->SetMarkerStyle(6); h1_mBestTopJet_Zinv->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv->Draw("hist same");
  h1_mBestTopJet_Signal->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal->SetMarkerStyle(6); h1_mBestTopJet_Signal->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal->Draw("hist same");

  cs->cd(2); padZinv02 = (TPad*) cs->GetPad(2); padZinv02->Clear();
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->SetAxisRange(0, 1.5*h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->SetLineColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->SetMarkerStyle(6); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts->Draw("hist");
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->SetMarkerStyle(6); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts->Draw("hist same");
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts->SetMarkerStyle(6); h1_mBestTopJet_Signal_aftMTAndMT2Cuts->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts->Draw("hist same");

  cs->cd(3); padZinv03 = (TPad*) cs->GetPad(3); padZinv03->Clear();
  h1_mBestTopJet_TTbar_Cloned->SetAxisRange(0, 1.5*h1_mBestTopJet_TTbar_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar_Cloned->SetLineColor(kBlue); h1_mBestTopJet_TTbar_Cloned->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar_Cloned->SetMarkerStyle(6); h1_mBestTopJet_TTbar_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_Cloned->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_Cloned->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Zinv_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv_Cloned->Draw("hist same");
  h1_mBestTopJet_Signal_Cloned->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_Cloned->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Signal_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal_Cloned->Draw("same");

  cs->cd(4); padZinv04 = (TPad*) cs->GetPad(4); padZinv04->Clear();
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetAxisRange(0, 1.5*h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetLineColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->Draw("hist same");
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->Draw("same");

  cs->Print("ZinvEstimation_"+treeStrT+".pdf");

  cs->cd(1); padZinv01 = (TPad*) cs->GetPad(1); padZinv01->SetLogy();
  h1_mBestTopJet_TTbar_Cloned->SetAxisRange(0.5e-3, 1.5*h1_mBestTopJet_TTbar_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar_Cloned->SetLineColor(kBlue); h1_mBestTopJet_TTbar_Cloned->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar_Cloned->SetMarkerStyle(6); h1_mBestTopJet_TTbar_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_Cloned->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_Cloned->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Zinv_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv_Cloned->Draw("hist same");
  h1_mBestTopJet_Signal_Cloned->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_Cloned->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Signal_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal_Cloned->Draw("same");

  cs->cd(2); padZinv02 = (TPad*) cs->GetPad(2); padZinv02->SetLogy();
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetAxisRange(0.5e-3, 1.5*h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetLineColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerColor(kBlue); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_TTbar_aftMTAndMT2Cuts_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetLineColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerColor(colorCachedVec[pickedZinvIdx]); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->Draw("hist same");
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetLineColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerColor(colorCachedVec[pickedSignalIdx]); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_Signal_aftMTAndMT2Cuts_Cloned->Draw("same");

  cs->cd(3); padZinv03 = (TPad*) cs->GetPad(3); padZinv03->Clear();
  h1_mBestTopJet_WJets_Cloned->SetAxisRange(0.5e-3, 1.5*h1_mBestTopJet_WJets_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_WJets_Cloned->SetLineColor(colorCachedVec[pickedWJetsIdx]); h1_mBestTopJet_WJets_Cloned->SetMarkerColor(colorCachedVec[pickedWJetsIdx]); h1_mBestTopJet_WJets_Cloned->SetMarkerStyle(6); h1_mBestTopJet_WJets_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_WJets_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_Cloned->Draw("hist same");

  cs->cd(4); padZinv04 = (TPad*) cs->GetPad(4); padZinv04->Clear();
  h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->SetAxisRange(0.5e-3, 1.5*h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->GetMaximum(), "Y");
  h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->SetLineColor(colorCachedVec[pickedWJetsIdx]); h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->SetMarkerColor(colorCachedVec[pickedWJetsIdx]); h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->SetMarkerStyle(6); h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->SetMarkerSize(0.25);
  h1_mBestTopJet_WJets_aftMTAndMT2Cuts_Cloned->Draw("hist");
  h1_mBestTopJet_Zinv_aftMTAndMT2Cuts_Cloned->Draw("hist same");

  cs->Print("ZinvEstimation_"+treeStrT+".pdf");

  cs->Print("ZinvEstimation_"+treeStrT+".pdf]");

}

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   return countJets(inputJets, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){
         cntNJets ++;
      }
   }
   return cntNJets;
}

vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr){
   return calcDPhi(inputJets, metphi, nDPhi, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   vector<double> outDPhiVec(nDPhi, 999);
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( cntNJets < nDPhi ){
            double perDPhi = fabs(TVector2::Phi_mpi_pi( inputJets[ij].Phi() - metphi ));
            outDPhiVec[cntNJets] = perDPhi;
         }
         cntNJets ++;
      }
   }

   return outDPhiVec;
}

int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr){
   return countCSVS(inputJets, inputCSVS, CSVS, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( inputCSVS[ij] > CSVS ) cntNJets ++;
      }
   }
   return cntNJets;
}


bool passOriKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut){
   bool pass = true;

   const double mTcomb = mTb + 0.5*mTt;
   if( mTcomb < 500 ) pass = false;
   if( MT2 < 300 ) pass = false;
   if( met < metCut ) pass = false;

   return pass;
}

bool passBaselineFunc(const double met, const double MT2, const double mTt, const double mTb){
   bool pass = passOriKinCuts(met, MT2, mTt, mTb, 175);
   return pass;
}

bool passHighMETFunc(const double met, const double MT2, const double mTt, const double mTb){
   bool pass = passOriKinCuts(met, MT2, mTt, mTb, 350);
   return pass;
}

bool passNewKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut){
   bool pass = true;

// Cuts in (mTb, mTt) plane   
   const double mTcomb = mTb + mTt;
   if( mTb < 200 ) pass = false;
   if( mTt < 350 ) pass = false;
   if( mTcomb < 600 ) pass = false;

// Cuts in (met, MT2) plane
   if( MT2 < 200 ) pass = false;
   if( 0.6*met + 0.4*MT2 < 240 ) pass = false;
   if( -0.8*met + MT2 > 200 ) pass = false;

// Cuts in (met, mTb) plane
   if( met + mTb < 800 ) pass = false;

   if( met < metCut ) pass = false;

   return pass;
}

bool passNewHighMETFunc(const double met, const double MT2, const double mTt, const double mTb){
   bool pass = passNewKinCuts(met, MT2, mTt, mTb, 350);
   return pass;
}

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr){
   return adjustbJetsCSVS(inputJets, inputCSVS, CSVS, jetCutsArr[0], jetCutsArr[1], jetCutsArr[2], jetCutsArr[3]);
}

int adjustbJetsCSVS(const vector<TLorentzVector> &inputJets, vector<double> &inputCSVS, const double CSVS, const double minAbsEta, const double maxAbsEta, const double minPt, const double maxPt){

   int adjustJetIdx = -1;
   double maxCSVS = -1;
   for(unsigned int ij=0; ij<inputJets.size(); ij++){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( adjustJetIdx == -1 ){ maxCSVS = inputCSVS[ij]; adjustJetIdx = ij; }
         else if( maxCSVS < inputCSVS[ij] ){ maxCSVS = inputCSVS[ij]; adjustJetIdx = ij; }
      }
   }
   if( maxCSVS >= CSVS ){
      std::cout<<"ERROR ... adjustbJetsCSVS : already found jets with maxCSVS : "<<maxCSVS<<"  larger than the CSVS cut : "<<CSVS<<std::endl;
      return -1;
   }
   if( adjustJetIdx != -1 ){
      inputCSVS[adjustJetIdx] = 1.0;
   }

   return adjustJetIdx;
}
