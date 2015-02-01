#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "TGraphErrors.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TLegend.h"
#include <iostream>
#include <functional>
#include <numeric>
#include "TStopwatch.h"

// indices                         0             1                  2                   3               4               5             6         7
const string      mcStr[] = {   "TTbar",    "WJetsToLNu",   "Zinv_400HTinf",     "Zinv_200HT400",   "DYJets",        "QCDFlat",     "T_t",    "T_tW"    };
const double    xSecArr[] = {     234.,       30.08,                6.26,           49.28,          2.826*1.188,    2.99913994E10,   47.0,  11.1773*2.0 };
const int       nEvtArr[] = {  21671578,     1647804,             1006928,          5066601,         1703863,         9991674,       23777,   473554    };
const int        colors[] = {    kGreen,      kRed,              kMagenta,         kMagenta,         kGreen+2,         kTeal,       kYellow,  kYellow   };
const string   disptStr[] = {   "t#bar{t}", "W#rightarrowl#nu (l=e, #mu, #tau)",      "Z#rightarrow#nu#bar{#nu}",   "Zinv_200HT400",   "Z#rightarrowl#bar{l} (l=e, #mu, #tau)",    "QCD",        "T_t",    "T_tW"    };

const int nMC = sizeof(mcStr)/sizeof(mcStr[0]);

const double dataLumiAB = (699.002)/1000. + 4.389; // in fb-1
const double dataLumiC = 6.397; // in fb-1 for 2012C
const double dataLumi = dataLumiAB + dataLumiC;
//const double dataLumi = dataLumiAB;

vector<double> scalesVec;

vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

static const int nRangesMET = 1;
static const double divMET[nRangesMET+1] = {0, 3000};
static const int   nDivMET[nRangesMET]   = {  200  };
//static const double divPerBin = (3000-0)/200.;
static const double divPerBin = 1.0;

vector<TH1D*> stackedHistsVec;
vector<int> stackedColorsVec;
vector<string> stackedDisptVec;

static const int nJetsSel = 5, nJetsSelPt30Eta24 = 5, nJetsSelPt50Eta24 = 4, nJetsSelPt70Eta24 = 2;
int nJetsSelBtagged = 1;
static const double cutCSVS = 0.679;
static const double minJetPt = 30;
//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta24Arr[] = {   -1,       2.4,      30,    -1  };
static const double pt50Eta24Arr[] = {   -1,       2.4,      50,    -1  };
static const double pt70Eta24Arr[] = {   -1,       2.4,      70,    -1  };
static const double      dphiArr[] = {   -1,       4.7,      30,    -1  };
static const double      bTagArr[] = {   -1,       2.4,      30,    -1  };

static const double lowTopCut_ = 80, highTopCut_ = 270;
static const double MT2cut_ = 300, mTbcut_ = 500;

static const int nStages = 4;
static const int specialStageIdx = 2;
static const double cutMETArr[] ={ 200, 350, 200, 350 };

vector<string> keyStrForStages;

static const int nCompCTEQ = 45, nCompMSTW = 41, nCompNNPDF = 101;

static const double dphi0Cut = 0.5, dphi1Cut = 0.5, dphi2Cut = 0.3;

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS = 0.679, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi = 3, const double minAbsEta = -1, const double maxAbsEta = 4.7, const double minPt = 30, const double maxPt = -1);

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

UShort_t        JetFlavorNum;
Float_t         JetFlavor[50];   //[JetFlavorNum]

UShort_t        JetsNum;
Float_t         JetsPt[50];   //[JetsNum]
Float_t         JetsEta[50];   //[JetsNum]
Float_t         JetsPhi[50];   //[JetsNum]
Float_t         JetsE[50];   //[JetsNum]

// Signal scan variables
Float_t         SusyMoSumPt;
Float_t         massMom;
Float_t         massDau;
UShort_t        cteq66Num;
Float_t         cteq66[45];   //[cteq66Num]
UShort_t        MSTW2008nlo68clNum;
Float_t         MSTW2008nlo68cl[41];   //[MSTW2008nlo68clNum]
UShort_t        NNPDF20Num;
Float_t         NNPDF20[101];   //[NNPDF20Num]

Float_t         ISRWeight;

Int_t           stopremainPassCSVS;
Float_t         stopMT2;
Float_t         stopbestTopJetMass;
Float_t         stoplinearCombmTbJetPlusmTbestTopJet;
Float_t         stopmTbJet;
Float_t         stopmTbestTopJet;
Float_t         stopmTbestWJet;
Float_t         stopmTbestbJet;
Int_t           stopbestTopJetIdx;
Float_t         stopmTremainingTopJet;
Int_t           stoppickedRemainingCombfatJetIdx;

TStopwatch timer;

static const float boost_min[] = {  0.0, 120.0, 150.0, 250.0};
static const float boost_max[] = {120.0, 150.0, 250.0, 1000.0};
const int n_boost = sizeof(boost_min)/sizeof(boost_min[0]);

TH2F *h_ISRWt[n_boost], *h_ISRWt_up[n_boost], *h_ISRWt_dn[n_boost];


TH2F *h_Eff_pfBJets, *h_Eff_pfCJets, *h_Eff_pfUDSGJets;

static const float b_ptmin[]   = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
static const float b_ptmax[]   = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};
static const float SFb_error[] = {0.0209663, 0.0207019, 0.0230073,
				  0.0208719, 0.0200453, 0.0264232, 0.0240102,
				  0.0229375, 0.0184615, 0.0216242, 0.0248119,
				  0.0465748, 0.0474666, 0.0718173, 0.0717567};
const int nPt_b = sizeof(b_ptmin)/sizeof(b_ptmin[0]);

static const float l_etamin[] = {0.0, 0.8, 1.6};
static const float l_etamax[] = {0.8, 1.6, 2.4};
const int nEta_l = sizeof(l_etamin)/sizeof(l_etamin[0]);

// errors are absolute on FastSim->FullSim CFs
static const float CFb[] = {0.982194,0.980998,0.992014,0.994472,
			    0.996825,0.999822,1.00105, 1.00023,
			    0.991994,0.979123,0.947207,0.928006,
			    0.874260,0.839610,0.839610};

static const float CFb_error[] = {0.00253112,0.00296453,0.00113963,0.00128363,
				  0.00232566,0.00232353,0.00219086,0.00156856,
				  0.00322279,0.00400414,0.00737465,0.0105033,
				  0.0171706, 0.0344172, 0.0344172}; // stat + PU
static const float CFb_T2tt_syst[] = {0.00574604,0.00677246,0.00509557,0.00374240,
				      0.00314873,0.000637591,-0.000242591,-4.16636e-05,
				      -0.00292352,-0.00581479,-0.000461876,-0.00676391,
				      0.00488830,3.05474e-05,3.05474e-05};
static const float CFc[]           = {0.988545,0.981714,1.00946,1.01591,1.02810,1.02195,1.02590,1.01936,0.991228,0.955343,0.944433,0.917282,0.935018,1.06375,1.06375};
static const float CFc_error[]     = {0.00746259,0.00661831,0.00968682,0.00751322,0.00675507,0.00562821,0.00862890,0.00768003,0.0188981,0.0261163,0.0450601,0.0448453,0.148805,0.177157,0.177157}; // stat + PU
static const float CFc_T2tt_syst[] = {-0.0183669,-0.0125071,-0.0174156,-0.0164738,-0.0167200,-0.0149260,-0.0180894,-0.0154648,-0.0141536,-0.0119079,-0.0206974,0.000753522,0.0221000,0.0209901,0.0209901};
//Tagger: CSVM within 30 < pt < 670 GeV, abs(eta) < 1.2
static const float CFudsg_central[] = {1.21878,1.28615,1.37535,1.38966,1.40320,1.49835,1.44308,1.58198,1.55687,1.65790,1.90233,1.92259,2.66174,3.08688,3.08688};
static const float CFudsg_central_error[] = {0.0182686,0.0373732,0.0461870,0.0288973,0.0333528,0.0513836,0.0420353,0.106627,0.0658359,0.117285,0.185533,0.214071,0.487274,0.871502,0.871502}; // stat + PU
static const float CFudsg_central_T2tt_syst[] = {0.241447,0.297617,0.365921,0.372697,0.378869,0.434225,0.385061,0.452832,0.412124,0.498940,0.675028,0.813003,1.31961,1.57929,1.57929};
//Tagger: CSVM within 30 < pt < 670 GeV, 1.2 < abs(eta) < 2.4
static const float CFudsg_forward[] = {1.46970,1.48732,1.69024,1.64494,1.79297,1.90760,1.99867,2.21659,2.20103,2.42645,2.67594,4.24735,3.98979,15.0457,15.0457};
static const float CFudsg_forward_error[] = {0.104716,0.0392025,0.106315,0.115751,0.106807,0.0642086,0.138742,0.182345,0.169922,0.297889,0.320088,0.927736,1.24666,15.1860,15.1860}; // stat + PU
static const float CFudsg_forward_T2tt_syst[] = {0.694594,0.753930,1.01105,1.02488,1.18455,1.36025,1.57676,1.87545,2.24691,2.81635,3.46050,5.75946,6.89900,15.8855,15.8855};

float MGISRWeight(double mo, double dau, double boost, int type);

float EffBtag(double pt, double eta);
float EffCtag(double pt, double eta);
float EffUDSGtag(double pt, double eta);
float SF1b(double jetpt);
float SF1b_e(double pt);
float SF1c(double jetpt);
float SF1c_e(double pt);
float SF1udsg(double pt, double eta);
float SF1udsg_ep(double pt, double eta);
float SF1udsg_em(double pt, double eta);
float SF2b(double pt);
float SF2b_e(double pt);
float SF2c(double pt);
float SF2c_e(double pt);
float SF2udsg(double pt, double eta);
float SF2udsg_e(double pt, double eta);
double WeightBtagSF(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta);
double WeightBtagSF1_Errbc(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp);
double WeightBtagSF1_Errudsg(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp);
double WeightBtagSF2_Errbc(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp);
double WeightBtagSF2_Errudsg(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp);


float MGISRWeight(double mo, double dau, double boost, int type) {

  int iboost=-1;
  if(boost > 1000.0) boost = 999.0;
  
  // find boost range
  for(int ibb=0; ibb<n_boost; ibb++){
    if( boost>boost_min[ibb] && boost<boost_max[ibb])
      iboost=ibb;
    //break;
  }

  //std::cout << "mo " << mo << " dau " << dau << " boost " << boost << " iboost " << iboost << std::endl;

  float weight = 1.0;
  int   ibin   = h_ISRWt[iboost]->FindBin(mo,dau);
  if(type==1) {
    weight = h_ISRWt_up[iboost]->GetBinContent(ibin);
    //std::cout << "in MGISRWeight " << mo << " " << dau << " " << boost << " iboost " << iboost << " " << weight << std::endl;
  } else if(type==2) {
    weight = h_ISRWt_dn[iboost]->GetBinContent(ibin);
    //std::cout << "in MGISRWeight " << mo << " " << dau << " " << boost << " iboost " << iboost << " " << weight << std::endl;
  } else {
    weight = h_ISRWt[iboost]->GetBinContent(ibin);
    //std::cout << "in MGISRWeight " << mo << " " << dau << " " << boost << " iboost " << iboost << " " << weight << std::endl;
  }
  return weight;
}

float EffBtag(double pt, double eta) {
  int    ibin = h_Eff_pfBJets->FindBin(pt,eta);
  double eff  = h_Eff_pfBJets->GetBinContent(ibin);
  return eff;
}
float EffCtag(double pt, double eta) {
  int    ibin = h_Eff_pfCJets->FindBin(pt,eta);
  double eff  = h_Eff_pfCJets->GetBinContent(ibin);
  return eff;
}
float EffUDSGtag(double pt, double eta) {
  int    ibin = h_Eff_pfUDSGJets->FindBin(pt,eta);
  double eff  = h_Eff_pfUDSGJets->GetBinContent(ibin);
  return eff;
}
// calculate parametrized SF for b-jets
float SF1b(double pt) {
  double sfb = 0.726981*((1.+(0.253238*pt))/(1.+(0.188389*pt)));
  return sfb;
}
float SF1c(double pt) {
  double sfc = SF1b(pt);
  return sfc;
}
float SF1udsg(double pt, double eta){
  double x = pt;
  double sfl = 1.0;
  if( std::abs(eta)>l_etamin[0] &&  std::abs(eta)<l_etamax[0] )
    sfl = ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x))) ;
  else if ( std::abs(eta)>l_etamin[1] &&  std::abs(eta)<l_etamax[1] )
    sfl = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x))) ;
  else if ( std::abs(eta)>l_etamin[2] &&  std::abs(eta)<l_etamax[2] )
    sfl = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x))) ;
  return sfl;
}

float SF1b_e(double pt) {
  if(pt>800.0) pt = 799.9;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  return SFb_error[ibin];
}
float SF1c_e(double pt) {
  float sfc_e = SF1b_e(pt);
  return 2.0*sfc_e;
}
float SF1udsg_ep(double pt, double eta){
  double x = pt;
  double sfl_e = 0.0;
  if( std::abs(eta)>l_etamin[0] &&  std::abs(eta)<l_etamax[0] )
    sfl_e = ((1.15201+(0.00292575*x))+(-7.41497e-06*(x*x)))+(5.0512e-09*(x*(x*x)));
  else if ( std::abs(eta)>l_etamin[1] &&  std::abs(eta)<l_etamax[1] )
    sfl_e = ((1.17735+(0.00156533*x))+(-4.32257e-06*(x*x)))+(3.18197e-09*(x*(x*x)));
  else if ( std::abs(eta)>l_etamin[2] &&  std::abs(eta)<l_etamax[2] )
    sfl_e = ((1.17671+(0.0010147*x))+(-3.66269e-06*(x*x)))+(2.88425e-09*(x*(x*x)));
  
  if( std::abs(eta)<1.5 && pt>800.0)      sfl_e = 2.0*sfl_e;
  else if( std::abs(eta)>1.5 && pt>700.0) sfl_e = 2.0*sfl_e;

  return sfl_e;
}
float SF1udsg_em(double pt, double eta){
  double x = pt;
  double sfl_e = 0.0;
  if( std::abs(eta)>l_etamin[0] &&  std::abs(eta)<l_etamax[0] )
    sfl_e = ((0.972746+(0.00104424*x))+(-2.36081e-06*(x*x)))+(1.53438e-09*(x*(x*x)));
  else if ( std::abs(eta)>l_etamin[1] &&  std::abs(eta)<l_etamax[1] )
    sfl_e = ((0.9836+(0.000649761*x))+(-1.59773e-06*(x*x)))+(1.14324e-09*(x*(x*x)));
  else if ( std::abs(eta)>l_etamin[2] &&  std::abs(eta)<l_etamax[2] )
    sfl_e = ((1.00616+(0.000358884*x))+(-1.23768e-06*(x*x)))+(6.86678e-10*(x*(x*x)));

  if( std::abs(eta)<1.5 && pt>800.0)      sfl_e = 2.0*sfl_e;
  else if( std::abs(eta)>1.5 && pt>700.0) sfl_e = 2.0*sfl_e;

  return sfl_e;
}

float SF2b(double pt) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  return CFb[ibin];
}

float SF2b_e(double pt) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  float err = std::sqrt(CFb_error[ibin]*CFb_error[ibin] + CFb_T2tt_syst[ibin]*CFb_T2tt_syst[ibin]);
  return err;
}

float SF2c(double pt) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  return CFc[ibin];
}

float SF2c_e(double pt) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  float err = std::sqrt(CFc_error[ibin]*CFc_error[ibin] + CFc_T2tt_syst[ibin]*CFc_T2tt_syst[ibin]);
  return err;
}
float SF2udsg(double pt, double eta) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;

  float cf = 1.0;
  if(std::abs(eta)<1.2) cf = CFudsg_central[ibin];
  else  cf = CFudsg_forward[ibin];
  return cf;
}

float SF2udsg_e(double pt, double eta) {
  if(pt>799.0) pt = 799.0;
  int ibin = -1;
  for(int ipt=0; ipt<nPt_b; ipt++){
    if( pt>b_ptmin[ipt] && pt<b_ptmax[ipt]) ibin = ipt;
  }
  if( ibin<0 ) std::cout <<"check pt of jets " << std::endl;
  float err = 0.0;
  if(std::abs(eta)<1.2) err = std::sqrt(CFudsg_central_error[ibin]*CFudsg_central_error[ibin] + CFudsg_central_T2tt_syst[ibin]*CFudsg_central_T2tt_syst[ibin]);
  else                  err = std::sqrt(CFudsg_forward_error[ibin]*CFudsg_forward_error[ibin] + CFudsg_forward_T2tt_syst[ibin]*CFudsg_forward_T2tt_syst[ibin]);
  return err;
}

//===  calculate weight due to b-tagging scaling factors
double WeightBtagSF(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta){
  
  double prob1 = 1.0;
  double prob2 = 1.0;
  for(unsigned int ij=0; ij<inputJets.size(); ij++){
    double pt     = inputJets[ij].Pt();
    double eta    = std::abs(inputJets[ij].Eta());
    int    flavor = newJetsFlavor[ij];
    double batg   = inputCSVS[ij];
    if( pt<minPt || eta>maxAbsEta) continue;
    if(flavor==5) {
      if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *= SF1b(pt)*SF2b(pt) * EffBtag(pt, eta);}
      else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 - SF1b(pt)*SF2b(pt)*EffBtag(pt, eta);}
    } else if (flavor==4) {
      if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= SF1c(pt)*SF2c(pt) * EffCtag(pt, eta);}
      else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - SF1c(pt)*SF2c(pt)*EffCtag(pt, eta);}
    } else if (flavor<=3 || flavor==21 ) {
      if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= SF1udsg(pt,eta)*SF2udsg(pt,eta) * EffUDSGtag(pt, eta);}
      else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - SF1udsg(pt,eta)*SF2udsg(pt,eta)*EffUDSGtag(pt, eta);}
    }
  }//loop over jets
  
  double weight = 1.0;
  if(prob1) weight = prob2/prob1;
  return weight;
}

double WeightBtagSF1_Errbc(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp){

  //vary b & c simultaneously while keeping udsf SF to nominal values

  double prob1 = 1.0;
  double prob2 = 1.0;
  
  for(unsigned int ij=0; ij<inputJets.size(); ij++){
    double pt     = inputJets[ij].Pt();
    double eta    = std::abs(inputJets[ij].Eta());
    int    flavor = newJetsFlavor[ij];
    double batg   = inputCSVS[ij];
    if( pt<minPt || eta>maxAbsEta) continue;

    if(flavor==5) {
      // vary SF with errors
      if(errUp) {
 	if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *= (SF1b(pt)+SF1b_e(pt))*SF2b(pt) * EffBtag(pt, eta);}
	else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 - (SF1b(pt)+SF1b_e(pt))*SF2b(pt)*EffBtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *= (SF1b(pt)-SF1b_e(pt))*SF2b(pt) * EffBtag(pt, eta);}
	else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 - (SF1b(pt)-SF1b_e(pt))*SF2b(pt)*EffBtag(pt, eta);}
      }
    } else if (flavor==4) {
      // vary SF with errors
      if(errUp) {
	if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= (SF1c(pt)+SF1c_e(pt))*SF2c(pt) * EffCtag(pt, eta);}
	else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - (SF1c(pt)+SF1c_e(pt))*SF2c(pt)*EffCtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= (SF1c(pt)-SF1c_e(pt))*SF2c(pt) * EffCtag(pt, eta);}
	else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - (SF1c(pt)-SF1c_e(pt))*SF2c(pt)*EffCtag(pt, eta);}
      }
    } else if (flavor<=3 || flavor==21 ) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= SF1udsg(pt,eta)*SF2udsg(pt,eta) * EffUDSGtag(pt, eta);}
      else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - SF1udsg(pt,eta)*SF2udsg(pt,eta)*EffUDSGtag(pt, eta);}
    }
 
  }//loop over jets
  
  double weight = 1.0;
  if(prob1) weight = prob2/prob1;
  return weight;
}

double WeightBtagSF1_Errudsg(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp){

  // vary udsg while keeping b & c SF to nominal values

  double prob1 = 1.0;
  double prob2 = 1.0;
  
  for(unsigned int ij=0; ij<inputJets.size(); ij++){
    double pt     = inputJets[ij].Pt();
    double eta    = std::abs(inputJets[ij].Eta());
    int    flavor = newJetsFlavor[ij];
    double batg   = inputCSVS[ij];
    if( pt<minPt || eta>maxAbsEta) continue;

    if(flavor==5) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *= SF1b(pt)*SF2b(pt) * EffBtag(pt, eta);}
      else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 - SF1b(pt)*SF2b(pt)*EffBtag(pt, eta);}
    } else if (flavor==4) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= SF1c(pt)*SF2c(pt) * EffCtag(pt, eta);}
      else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - SF1c(pt)*SF2c(pt)*EffCtag(pt, eta);}
    } else if (flavor<=3 || flavor==21 ) {
      // vary SF with errors
      if(errUp) {
	if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= (SF1udsg(pt,eta)+SF1udsg_ep(pt,eta))*SF2udsg(pt,eta) * EffUDSGtag(pt, eta);}
	else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - (SF1udsg(pt,eta)+SF1udsg_ep(pt,eta))*EffUDSGtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= (SF1udsg(pt,eta)-SF1udsg_em(pt,eta))*SF2udsg(pt,eta) * EffUDSGtag(pt, eta);}
	else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - (SF1udsg(pt,eta)-SF1udsg_em(pt,eta))*EffUDSGtag(pt, eta);}
      }
    }
  }//loop over jets
  
  double weight = 1.0;
  if(prob1) weight = prob2/prob1;
  return weight;
}

double WeightBtagSF2_Errbc(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp){

  //vary b & c simultaneously while keeping udsf SF to nominal values

  double prob1 = 1.0;
  double prob2 = 1.0;
  
  for(unsigned int ij=0; ij<inputJets.size(); ij++){
    double pt     = inputJets[ij].Pt();
    double eta    = std::abs(inputJets[ij].Eta());
    int    flavor = newJetsFlavor[ij];
    double batg   = inputCSVS[ij];
    if( pt<minPt || eta>maxAbsEta) continue;

    if(flavor==5) {
      // vary SF with errors
      if(errUp) {
 	if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *=  SF1b(pt)*(SF2b(pt)+SF2b_e(pt)) * EffBtag(pt, eta);}
	else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 -  SF1b(pt)*(SF2b(pt)+SF2b_e(pt))*EffBtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *=  SF1b(pt)*(SF2b(pt)-SF2b_e(pt)) * EffBtag(pt, eta);}
	else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 -  SF1b(pt)*(SF2b(pt)-SF2b_e(pt))*EffBtag(pt, eta);}
      }
    } else if (flavor==4) {
      // vary SF with errors
      if(errUp) {
	if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= SF1c(pt)*(SF2c(pt)+SF2c_e(pt)) * EffCtag(pt, eta);}
	else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - SF1c(pt)*(SF2c(pt)+SF2c_e(pt))*EffCtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= SF1c(pt)*(SF2c(pt)-SF2c_e(pt)) * EffCtag(pt, eta);}
	else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - SF1c(pt)*(SF2c(pt)-SF2c_e(pt))*EffCtag(pt, eta);}
      }
    } else if (flavor<=3 || flavor==21 ) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= SF1udsg(pt,eta)*SF2udsg(pt,eta) * EffUDSGtag(pt, eta);}
      else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - SF1udsg(pt,eta)*SF2udsg(pt,eta)*EffUDSGtag(pt, eta);}
    }
 
  }//loop over jets
  
  double weight = 1.0;
  if(prob1) weight = prob2/prob1;
  return weight;
}

double WeightBtagSF2_Errudsg(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const vector<double> &newJetsFlavor, const double minPt, const double maxAbsEta, bool errUp){

  // vary udsg while keeping b & c SF to nominal values

  double prob1 = 1.0;
  double prob2 = 1.0;
  
  for(unsigned int ij=0; ij<inputJets.size(); ij++){
    double pt     = inputJets[ij].Pt();
    double eta    = std::abs(inputJets[ij].Eta());
    int    flavor = newJetsFlavor[ij];
    double batg   = inputCSVS[ij];
    if( pt<minPt || eta>maxAbsEta) continue;

    if(flavor==5) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffBtag(pt, eta);          prob2 *= SF1b(pt)*SF2b(pt) * EffBtag(pt, eta);}
      else             {prob1 *= 1.0 - EffBtag(pt, eta);    prob2 *= 1.0 -  SF1b(pt)*SF2b(pt)*EffBtag(pt, eta);}
    } else if (flavor==4) {
      // use nominal SFs
      if(batg>cutCSVS) {prob1 *= EffCtag(pt, eta);          prob2 *= SF1c(pt)*SF2c(pt) * EffCtag(pt, eta);}
      else             {prob1 *= 1.0 - EffCtag(pt, eta);    prob2 *= 1.0 - SF1c(pt)*SF2c(pt)*EffCtag(pt, eta);}
    } else if (flavor<=3 || flavor==21 ) {
      // vary SF with errors
      if(errUp) {
	if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= SF1udsg(pt,eta)*(SF2udsg(pt,eta)+SF2udsg_e(pt,eta)) * EffUDSGtag(pt, eta);}
	else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - SF1udsg(pt,eta)*(SF2udsg(pt,eta)+SF2udsg_e(pt,eta))*EffUDSGtag(pt, eta);}
      } else {
	if(batg>cutCSVS) {prob1 *= EffUDSGtag(pt, eta);       prob2 *= SF1udsg(pt,eta)*(SF2udsg(pt,eta)-SF2udsg_e(pt,eta)) * EffUDSGtag(pt, eta);}
	else             {prob1 *= 1.0 - EffUDSGtag(pt, eta); prob2 *= 1.0 - SF1udsg(pt,eta)*(SF2udsg(pt,eta)-SF2udsg_e(pt,eta))*EffUDSGtag(pt, eta);}
      }
    }
  }//loop over jets
  
  double weight = 1.0;
  if(prob1) weight = prob2/prob1;
  return weight;
}
//=================================

// type  0 : the standard one
//       1 : jes/jer
void activateVariablesForAtree(TChain *tmpChn, const int type =0 ){
   tmpChn->SetBranchStatus("*", 0);

   if( type == 0 || type == 1 ){   
      tmpChn->SetBranchStatus("RunNum", 1); tmpChn->SetBranchAddress("RunNum", &RunNum);
      tmpChn->SetBranchStatus("LumiBlockNum", 1); tmpChn->SetBranchAddress("LumiBlockNum", &LumiBlockNum);
      tmpChn->SetBranchStatus("EvtNum", 1); tmpChn->SetBranchAddress("EvtNum", &EvtNum);
      tmpChn->SetBranchStatus("NVtx", 1); tmpChn->SetBranchAddress("NVtx", &NVtx);
      tmpChn->SetBranchStatus("Weight", 1); tmpChn->SetBranchAddress("Weight", &Weight);
      tmpChn->SetBranchStatus("NumPUInteractions", 1); tmpChn->SetBranchAddress("NumPUInteractions", &NumPUInteractions);

      tmpChn->SetBranchStatus("HT", 1); tmpChn->SetBranchAddress("HT", &HT);
      tmpChn->SetBranchStatus("MHT", 1); tmpChn->SetBranchAddress("MHT", &MHT);
      tmpChn->SetBranchStatus("NJets", 1); tmpChn->SetBranchAddress("NJets", &NJets);
      tmpChn->SetBranchStatus("DeltaPhi1", 1); tmpChn->SetBranchAddress("DeltaPhi1", &DeltaPhi1); 
      tmpChn->SetBranchStatus("DeltaPhi2", 1); tmpChn->SetBranchAddress("DeltaPhi2", &DeltaPhi2);
      tmpChn->SetBranchStatus("DeltaPhi3", 1); tmpChn->SetBranchAddress("DeltaPhi3", &DeltaPhi3);

      tmpChn->SetBranchStatus("massMom", 1); tmpChn->SetBranchAddress("massMom", &massMom);
      tmpChn->SetBranchStatus("massDau", 1); tmpChn->SetBranchAddress("massDau", &massDau);
   }
   if( type == 0 ){
      tmpChn->SetBranchStatus("Filter_eeNoiseFilter", 1); tmpChn->SetBranchAddress("Filter_eeNoiseFilter", &Filter_eeNoiseFilter);
      tmpChn->SetBranchStatus("Filter_trackingFailureFilter", 1); tmpChn->SetBranchAddress("Filter_trackingFailureFilter", &Filter_trackingFailureFilter);
      tmpChn->SetBranchStatus("Filter_inconsistentMuons", 1); tmpChn->SetBranchAddress("Filter_inconsistentMuons", &Filter_inconsistentMuons);
      tmpChn->SetBranchStatus("Filter_greedyMuons", 1); tmpChn->SetBranchAddress("Filter_greedyMuons", &Filter_greedyMuons);
      tmpChn->SetBranchStatus("Filter_ra2EcalTPFilter", 1); tmpChn->SetBranchAddress("Filter_ra2EcalTPFilter", &Filter_ra2EcalTPFilter);
      tmpChn->SetBranchStatus("Filter_ra2EcalBEFilter", 1); tmpChn->SetBranchAddress("Filter_ra2EcalBEFilter", &Filter_ra2EcalBEFilter);
      tmpChn->SetBranchStatus("Filter_hcalLaserEventFilter", 1); tmpChn->SetBranchAddress("Filter_hcalLaserEventFilter", &Filter_hcalLaserEventFilter);
      tmpChn->SetBranchStatus("Filter_ecalLaserCorrFilter", 1); tmpChn->SetBranchAddress("Filter_ecalLaserCorrFilter", &Filter_ecalLaserCorrFilter);
      tmpChn->SetBranchStatus("Filter_eeBadScFilter", 1); tmpChn->SetBranchAddress("Filter_eeBadScFilter", &Filter_eeBadScFilter);
      tmpChn->SetBranchStatus("Filter_PBNRFilter", 1); tmpChn->SetBranchAddress("Filter_PBNRFilter", &Filter_PBNRFilter);
      tmpChn->SetBranchStatus("PATElectronsIDIsoNum", 1); tmpChn->SetBranchAddress("PATElectronsIDIsoNum", &PATElectronsIDIsoNum); // electron veto
      tmpChn->SetBranchStatus("PATMuonsPFIDIsoNum", 1); tmpChn->SetBranchAddress("PATMuonsPFIDIsoNum", &PATMuonsPFIDIsoNum); // muon veto
      tmpChn->SetBranchStatus("PATMETsPFNum", 1); tmpChn->SetBranchAddress("PATMETsPFNum", &PATMETsPFNum);

      tmpChn->SetBranchStatus("cteq66Num", 1); tmpChn->SetBranchAddress("cteq66Num", &cteq66Num);
      tmpChn->SetBranchStatus("cteq66", 1); tmpChn->SetBranchAddress("cteq66", cteq66);
      tmpChn->SetBranchStatus("MSTW2008nlo68clNum", 1); tmpChn->SetBranchAddress("MSTW2008nlo68clNum", &MSTW2008nlo68clNum);
      tmpChn->SetBranchStatus("MSTW2008nlo68cl", 1); tmpChn->SetBranchAddress("MSTW2008nlo68cl", MSTW2008nlo68cl);
      tmpChn->SetBranchStatus("NNPDF20Num", 1); tmpChn->SetBranchAddress("NNPDF20Num", &NNPDF20Num);
      tmpChn->SetBranchStatus("NNPDF20", 1); tmpChn->SetBranchAddress("NNPDF20", NNPDF20);

      tmpChn->SetBranchStatus("ISRWeight", 1); tmpChn->SetBranchAddress("ISRWeight", &ISRWeight);
      tmpChn->SetBranchStatus("SusyMoSumPt", 1); tmpChn->SetBranchAddress("SusyMoSumPt", &SusyMoSumPt);

      tmpChn->SetBranchStatus("JetsNum", 1); tmpChn->SetBranchAddress("JetsNum", &JetsNum);
      tmpChn->SetBranchStatus("JetsPt", 1); tmpChn->SetBranchAddress("JetsPt", &JetsPt);
      tmpChn->SetBranchStatus("JetsEta", 1); tmpChn->SetBranchAddress("JetsEta", &JetsEta);
      tmpChn->SetBranchStatus("JetsPhi", 1); tmpChn->SetBranchAddress("JetsPhi", &JetsPhi);
      tmpChn->SetBranchStatus("JetsE", 1); tmpChn->SetBranchAddress("JetsE", &JetsE);

      tmpChn->SetBranchStatus("StopSAKLooseElectronSelectorNum", 1); tmpChn->SetBranchAddress("StopSAKLooseElectronSelectorNum", &StopSAKLooseElectronSelectorNum);
      tmpChn->SetBranchStatus("StopSAKLooseElectronSelectorPt", 1); tmpChn->SetBranchAddress("StopSAKLooseElectronSelectorPt", &StopSAKLooseElectronSelectorPt);
      tmpChn->SetBranchStatus("StopSAKLooseElectronSelectorEta", 1); tmpChn->SetBranchAddress("StopSAKLooseElectronSelectorEta", &StopSAKLooseElectronSelectorEta);
      tmpChn->SetBranchStatus("StopSAKLooseElectronSelectorPhi", 1); tmpChn->SetBranchAddress("StopSAKLooseElectronSelectorPhi", &StopSAKLooseElectronSelectorPhi);
      tmpChn->SetBranchStatus("StopSAKLooseElectronSelectorE", 1); tmpChn->SetBranchAddress("StopSAKLooseElectronSelectorE", &StopSAKLooseElectronSelectorE);
      tmpChn->SetBranchStatus("StopSAKLooseMuonSelectorNum", 1); tmpChn->SetBranchAddress("StopSAKLooseMuonSelectorNum", &StopSAKLooseMuonSelectorNum);
      tmpChn->SetBranchStatus("StopSAKLooseMuonSelectorPt", 1); tmpChn->SetBranchAddress("StopSAKLooseMuonSelectorPt", &StopSAKLooseMuonSelectorPt);
      tmpChn->SetBranchStatus("StopSAKLooseMuonSelectorEta", 1); tmpChn->SetBranchAddress("StopSAKLooseMuonSelectorEta", &StopSAKLooseMuonSelectorEta);
      tmpChn->SetBranchStatus("StopSAKLooseMuonSelectorPhi", 1); tmpChn->SetBranchAddress("StopSAKLooseMuonSelectorPhi", &StopSAKLooseMuonSelectorPhi);
      tmpChn->SetBranchStatus("StopSAKLooseMuonSelectorE", 1); tmpChn->SetBranchAddress("StopSAKLooseMuonSelectorE", &StopSAKLooseMuonSelectorE);
      tmpChn->SetBranchStatus("IsolatedTracksNum", 1); tmpChn->SetBranchAddress("IsolatedTracksNum", &IsolatedTracksNum);
      tmpChn->SetBranchStatus("IsolatedTracksPt", 1); tmpChn->SetBranchAddress("IsolatedTracksPt", &IsolatedTracksPt);
      tmpChn->SetBranchStatus("IsolatedTracksEta", 1); tmpChn->SetBranchAddress("IsolatedTracksEta", &IsolatedTracksEta);
      tmpChn->SetBranchStatus("IsolatedTracksPhi", 1); tmpChn->SetBranchAddress("IsolatedTracksPhi", &IsolatedTracksPhi);
      tmpChn->SetBranchStatus("IsolatedTracksE", 1); tmpChn->SetBranchAddress("IsolatedTracksE", &IsolatedTracksE);
      tmpChn->SetBranchStatus("PATMETsPFNum", 1); tmpChn->SetBranchAddress("PATMETsPFNum", &PATMETsPFNum);
      tmpChn->SetBranchStatus("PATMETsPFPt", 1); tmpChn->SetBranchAddress("PATMETsPFPt", &PATMETsPFPt);
      tmpChn->SetBranchStatus("PATMETsPFEta", 1); tmpChn->SetBranchAddress("PATMETsPFEta", &PATMETsPFEta);
      tmpChn->SetBranchStatus("PATMETsPFPhi", 1); tmpChn->SetBranchAddress("PATMETsPFPhi", &PATMETsPFPhi);
      tmpChn->SetBranchStatus("PATMETsPFE", 1); tmpChn->SetBranchAddress("PATMETsPFE", &PATMETsPFE);
      tmpChn->SetBranchStatus("PATMETsPFPhiCorrNum", 1); tmpChn->SetBranchAddress("PATMETsPFPhiCorrNum", &PATMETsPFPhiCorrNum);
      tmpChn->SetBranchStatus("PATMETsPFPhiCorrPt", 1); tmpChn->SetBranchAddress("PATMETsPFPhiCorrPt", &PATMETsPFPhiCorrPt);
      tmpChn->SetBranchStatus("PATMETsPFPhiCorrEta", 1); tmpChn->SetBranchAddress("PATMETsPFPhiCorrEta", &PATMETsPFPhiCorrEta);
      tmpChn->SetBranchStatus("PATMETsPFPhiCorrPhi", 1); tmpChn->SetBranchAddress("PATMETsPFPhiCorrPhi", &PATMETsPFPhiCorrPhi);
      tmpChn->SetBranchStatus("PATMETsPFPhiCorrE", 1); tmpChn->SetBranchAddress("PATMETsPFPhiCorrE", &PATMETsPFPhiCorrE);
      tmpChn->SetBranchStatus("BTagCSVNum", 1); tmpChn->SetBranchAddress("BTagCSVNum", &BTagCSVNum);
      tmpChn->SetBranchStatus("BTagCSV", 1); tmpChn->SetBranchAddress("BTagCSV", &BTagCSV);
      tmpChn->SetBranchStatus("JetFlavorNum", 1); tmpChn->SetBranchAddress("JetFlavorNum", &JetFlavorNum);
      tmpChn->SetBranchStatus("JetFlavor", 1); tmpChn->SetBranchAddress("JetFlavor", &JetFlavor);

      tmpChn->SetBranchStatus("stopremainPassCSVS", 1); tmpChn->SetBranchAddress("stopremainPassCSVS", &stopremainPassCSVS);
      tmpChn->SetBranchStatus("stopMT2", 1); tmpChn->SetBranchAddress("stopMT2", &stopMT2);
      tmpChn->SetBranchStatus("stopbestTopJetMass", 1); tmpChn->SetBranchAddress("stopbestTopJetMass", &stopbestTopJetMass);
      tmpChn->SetBranchStatus("stoplinearCombmTbJetPlusmTbestTopJet", 1); tmpChn->SetBranchAddress("stoplinearCombmTbJetPlusmTbestTopJet", &stoplinearCombmTbJetPlusmTbestTopJet);
      tmpChn->SetBranchStatus("stopmTbJet", 1); tmpChn->SetBranchAddress("stopmTbJet", &stopmTbJet);
      tmpChn->SetBranchStatus("stopmTbestTopJet", 1); tmpChn->SetBranchAddress("stopmTbestTopJet", &stopmTbestTopJet);
      tmpChn->SetBranchStatus("stopmTbestWJet", 1); tmpChn->SetBranchAddress("stopmTbestWJet", &stopmTbestWJet);
      tmpChn->SetBranchStatus("stopmTbestbJet", 1); tmpChn->SetBranchAddress("stopmTbestbJet", &stopmTbestbJet);
      tmpChn->SetBranchStatus("stopbestTopJetIdx", 1); tmpChn->SetBranchAddress("stopbestTopJetIdx", &stopbestTopJetIdx);
      tmpChn->SetBranchStatus("stopmTremainingTopJet", 1); tmpChn->SetBranchAddress("stopmTremainingTopJet", &stopmTremainingTopJet);
      tmpChn->SetBranchStatus("stoppickedRemainingCombfatJetIdx", 1); tmpChn->SetBranchAddress("stoppickedRemainingCombfatJetIdx", &stoppickedRemainingCombfatJetIdx);
   }
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

bool passStopJetRelated(const double met, const double metphi){
   std::vector<TLorentzVector> newJetsLVec;
   std::vector<double> newJetsCSVS;
   //std::vector<double> newJetsFlavor;
   if( JetsNum != BTagCSVNum ) std::cout<<"ERROR ... JetsNum : "<<JetsNum<<"  !=  BTagCSVNum : "<<BTagCSVNum<<std::endl;
   for(int ij=0; ij<JetsNum; ij++){
      double jetPt = JetsPt[ij], jetEta = JetsEta[ij], jetPhi = JetsPhi[ij], jetE = JetsE[ij];
      if( jetPt < minJetPt ) continue;
      TLorentzVector perJetLVec; perJetLVec.SetPtEtaPhiE(jetPt, jetEta, jetPhi, jetE);
      newJetsLVec.push_back(perJetLVec); 
      newJetsCSVS.push_back(BTagCSV[ij]);
      //newJetsFlavor.push_back(std::abs(JetFlavor[ij]));
   }

   int cntCSVS = countCSVS(newJetsLVec, newJetsCSVS, cutCSVS, bTagArr);
   int cntNJetsPt30 = countJets(newJetsLVec, pt30Arr);
   int cntNJetsPt30Eta24 = countJets(newJetsLVec, pt30Eta24Arr);
   int cntNJetsPt50Eta24 = countJets(newJetsLVec, pt50Eta24Arr);
   int cntNJetsPt70Eta24 = countJets(newJetsLVec, pt70Eta24Arr);
   vector<double> dPhiVec = calcDPhi(newJetsLVec, metphi, 3, dphiArr);

   bool passJetsRelated = true;

   if( cntCSVS < nJetsSelBtagged ) passJetsRelated = false;
   if( cntNJetsPt30 < nJetsSel ) passJetsRelated = false;
   if( cntNJetsPt30Eta24 < nJetsSelPt30Eta24 ) passJetsRelated = false;
   if( cntNJetsPt50Eta24 < nJetsSelPt50Eta24 ) passJetsRelated = false;
   if( cntNJetsPt70Eta24 < nJetsSelPt70Eta24 ) passJetsRelated = false;
   if( dPhiVec[0] < dphi0Cut || dPhiVec[1] < dphi1Cut || dPhiVec[2] < dphi2Cut ) passJetsRelated = false;

   bool passTopTagger = true;
   if( stopbestTopJetIdx == -1 ) passTopTagger = false;
   if( !(stopbestTopJetMass > lowTopCut_ && stopbestTopJetMass < highTopCut_) ) passTopTagger = false;
   if( !stopremainPassCSVS ) passTopTagger = false;
   if( stoppickedRemainingCombfatJetIdx == -1 && newJetsLVec.size() >=6 ) passTopTagger = false;

   return (passJetsRelated && passTopTagger);
}

bool passStopSearch(const double met, const double metphi){
  
   bool passJetsRelated = passStopJetRelated(met, metphi);

   bool passTopTagger = true;
   if( !( stopMT2 > MT2cut_ && (stopmTbJet + 0.5*stopmTbestTopJet) > mTbcut_ ) ) passTopTagger = false;

   return (passJetsRelated && passTopTagger);
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

bool passStopSearchNew(const double met, const double metphi){
  
   int cachednJetsSelBtagged = nJetsSelBtagged;
   nJetsSelBtagged = 2;
   bool passJetsRelated = passStopJetRelated(met, metphi);
   nJetsSelBtagged = cachednJetsSelBtagged;

   bool passTopTagger = true;
   if( !( stopMT2 > MT2cut_ && (stopmTbJet + 0.5*stopmTbestTopJet) > mTbcut_ ) ) passTopTagger = false;

   return (passJetsRelated && passTopTagger);
}

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

void initMCscales(){

   cout<<"\ndataLumi : "<<dataLumi<<endl;
   for(int ib=0; ib<nMC; ib++){
      double perScale = dataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
      scalesVec.push_back(perScale);
      printf("%30s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
   }
   cout<<endl;
}

void makeDivBins( const int nRangesPt, const double *divPts, const int *nDivPts, vector<double> &divBins){
   int totBinsPt = 0;
   for(int ib=0; ib<nRangesPt; ib++){ totBinsPt += nDivPts[ib]; }
   divBins.clear(); divBins.resize(totBinsPt+1);

   int reBinIdxPt = -1;
   for(int ib=0; ib<nRangesPt; ib++){
      for(int id=0; id<nDivPts[ib]; id++){
         double perDiv = (divPts[ib+1]-divPts[ib])/nDivPts[ib];
         double divVal = divPts[ib] + perDiv*id;
         reBinIdxPt ++;
         divBins[reBinIdxPt] = divVal;
         if( ib==nRangesPt-1 && id == nDivPts[ib] -1 ) divBins[totBinsPt] = divPts[ib+1];
      }
   }
}

void prodSignalHist(){

  // get btag efficiency histograms for b, c and udsg
  TFile *fbtagEff = new TFile("BTagEfficiency_mother600_LSP25to150.root");
  h_Eff_pfBJets    = (TH2F*)fbtagEff->FindObjectAny("h_Eff_pfBJets");
  h_Eff_pfCJets    = (TH2F*)fbtagEff->FindObjectAny("h_Eff_pfCJets");
  h_Eff_pfUDSGJets = (TH2F*)fbtagEff->FindObjectAny("h_Eff_pfUDSGJets");
  for(int ieta=1; ieta<=h_Eff_pfBJets->GetYaxis()->GetNbins(); ieta++) {
    for(int ipt=1; ipt<=h_Eff_pfBJets->GetXaxis()->GetNbins(); ipt++) {
      //std::cout << h_Eff_pfBJets->GetBinContent(ipt, ieta) << ", ";
      std::cout << "pt "  << h_Eff_pfBJets->GetXaxis()->GetBinLowEdge(ipt)  << " "
		<< "eta " << h_Eff_pfBJets->GetYaxis()->GetBinLowEdge(ieta) << " "
		<< "effB " << h_Eff_pfBJets->GetBinContent(ipt, ieta) << " " 
		<< "effC " << h_Eff_pfCJets->GetBinContent(ipt, ieta) << " " 
		<< "effUDSG " << h_Eff_pfUDSGJets->GetBinContent(ipt, ieta) 
		<< std::endl;
    }
    //std::cout << std::endl;
  }
  //fbtagEff->Close(); // closing this file here results in seg. fault

  //get ISR nominal, up and down weights
  TFile *fISR = new TFile("ISRWeightsUncFromRoberto.root");
  for(int ibb=0; ibb<n_boost; ibb++){
    char htit[200], tit[200];
    if(ibb==0)      sprintf(tit, "120");
    else if(ibb==1) sprintf(tit, "120_150_");
    else if(ibb==2) sprintf(tit, "150_250_");
    else if(ibb==3) sprintf(tit, "250");
    sprintf(htit, "hCF__%s__nominal", tit);
    std::cout << htit << std::endl;
    h_ISRWt[ibb] = (TH2F*) fISR->FindObjectAny(htit);
    sprintf(htit, "hCF__%s__up", tit);
    std::cout << htit << std::endl;
    h_ISRWt_up[ibb] = (TH2F*) fISR->FindObjectAny(htit);
    sprintf(htit, "hCF__%s__down", tit);
    std::cout << htit << std::endl;
    h_ISRWt_dn[ibb] = (TH2F*) fISR->FindObjectAny(htit);   
  }
  /*
  for(int ibb=0; ibb<n_boost; ibb++){
    std::cout << "boost range " << boost_min[ibb] << " " << boost_max[ibb] << std::endl;
    for(int imo=1; imo<=h_ISRWt[ibb]->GetXaxis()->GetNbins(); imo++) {
      for(int idau=1; idau<=h_ISRWt[ibb]->GetYaxis()->GetNbins(); idau++) {
	double mo  = h_ISRWt[ibb]->GetXaxis()->GetBinCenter(imo);
	double dau = h_ISRWt[ibb]->GetYaxis()->GetBinCenter(idau);
	int ibin = h_ISRWt[ibb]->FindBin(mo,dau);
	std::cout << "mo "  << h_ISRWt[ibb]->GetXaxis()->GetBinCenter(imo)  << " "
		  << "dau " << h_ISRWt[ibb]->GetYaxis()->GetBinCenter(idau) << " "
		  << h_ISRWt[ibb]->GetBinContent(ibin) 
		  << " + " << h_ISRWt_up[ibb]->GetBinContent(ibin) 
		  << " - " << h_ISRWt_dn[ibb]->GetBinContent(ibin) 
		  << std::endl;
      }
    }
  }
  */

   std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

   gROOT->ProcessLine(".L tdrstyle.C");
   setTDRStyle();

//   initMCscales();

   initPUinput("PUData_dist.root", "pileup");

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

/*
   tdrStyle->SetHistLineWidth(2);
   tdrStyle->SetFrameLineWidth(2);
*/

      //  For the axis
   tdrStyle->SetAxisColor(1,"XYZ");
   tdrStyle->SetTickLength(0.03,"XYZ");
   tdrStyle->SetNdivisions(510,"XYZ");
   tdrStyle->SetPadTickX(1);
   tdrStyle->SetPadTickY(1);
   tdrStyle->SetStripDecimals(kFALSE);

   tdrStyle->SetPalette(1);

   tdrStyle->SetTitleX(0.09);
   tdrStyle->SetTitleY(0.94);
   tdrStyle->SetTitleH(0.053);
   tdrStyle->SetTitleW(0.85);
  
//   tdrStyle->SetTitleFontSize(0.9);

   tdrStyle->SetTitleSize(0.045,"XYZ");
   tdrStyle->SetPadTopMargin(0.12);
//   tdrStyle->SetPadBottomMargin(0.25);
   tdrStyle->SetPadLeftMargin(0.125);
   tdrStyle->SetPadRightMargin(0.10);

   tdrStyle->SetLabelSize(0.030, "XYZ");

   char names[200];
   for(int is=0; is<nStages; is++){
      sprintf(names, "%3.0fMETinf", cutMETArr[is]);
      if(is>=specialStageIdx) sprintf(names, "%3.0fMETinfOpt", cutMETArr[is]);
      keyStrForStages.push_back(names);
   }

   ifstream fin("rootlist.txt");
   char filenames[200];
   std::vector<std::string> filesVec;
   while( fin.getline(filenames, 200) ){
      filesVec.push_back(filenames);
//      std::cout<<"filenames : "<<filenames<<std::endl;
   }

   TChain *ra2StdChn = new TChain("RA2TreeMaker/RA2PreSelection");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2StdChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2StdChn, 0);

   TChain *ra2JesUpChn = new TChain("RA2TreeMakerJESUp/RA2PreSelectionJESUp");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2JesUpChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2JesUpChn, 1);
   
   TChain *ra2JesDnChn = new TChain("RA2TreeMakerJESDn/RA2PreSelectionJESDn");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2JesDnChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2JesDnChn, 1);

   TChain *ra2JerChn = new TChain("RA2TreeMakerJER/RA2PreSelectionJER");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2JerChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2JerChn, 1);
   
   TChain *ra2JerUpChn = new TChain("RA2TreeMakerJERUp/RA2PreSelectionJERUp");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2JerUpChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2JerUpChn, 1);
      
   TChain *ra2JerDnChn = new TChain("RA2TreeMakerJERDn/RA2PreSelectionJERDn");
   for(unsigned int in=0; in<filesVec.size(); in++){ ra2JerDnChn->Add(filesVec.at(in).c_str()); }
   activateVariablesForAtree(ra2JerDnChn, 1);

// Standard RA2
   TH2D * h2_NTotal = new TH2D("NTotal", "NTotal", 60, 0, 1500, 60, 0, 1500); h2_NTotal->Sumw2();
   TH2D * h2_NTotal_very = new TH2D("NTotal_very", "NTotal_very", 60, 0, 1500, 60, 0, 1500); h2_NTotal_very->Sumw2();
   std::vector<TH2D*> h2_NPassed_stdRA2NoBtagNoISRWt;
   std::vector<TH2D*> h2_NPassed_stdRA2ISRWt;
   std::vector<TH2D*> h2_NPassed_stdRA2BtagWt;
   std::vector<TH2D*> h2_NPassed_stdRA2;
   std::vector<TH2D*> h2_NPassed_ISRWeight_up;
   std::vector<TH2D*> h2_NPassed_ISRWeight_dn;
   std::vector<TH2D*> h2_NPassed_BtagSF1_bcup;
   std::vector<TH2D*> h2_NPassed_BtagSF1_bcdn;
   std::vector<TH2D*> h2_NPassed_BtagSF1_udsgup;
   std::vector<TH2D*> h2_NPassed_BtagSF1_udsgdn;
   std::vector<TH2D*> h2_NPassed_BtagSF2_bcup;
   std::vector<TH2D*> h2_NPassed_BtagSF2_bcdn;
   std::vector<TH2D*> h2_NPassed_BtagSF2_udsgup;
   std::vector<TH2D*> h2_NPassed_BtagSF2_udsgdn;
   std::vector<TH2D*> h2_NPassed_very;
   for(int is=0; is<nStages; is++){
      TString tmpKeyStrT = keyStrForStages[is];
      TString tmpKeyStrNPassedT = "NPassed_"+tmpKeyStrT;
      TH2D * h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_stdRA2.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_ISRWt_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_stdRA2ISRWt.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagWt_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_stdRA2BtagWt.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_NoBtagWtNoISRWt_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_stdRA2NoBtagNoISRWt.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_ISRWeight_up_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_ISRWeight_up.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_ISRWeight_dn_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_ISRWeight_dn.push_back((TH2D*)h2_tmp->Clone());
      
      tmpKeyStrNPassedT = "NPassed_BtagSF1_bcup_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF1_bcup.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF1_bcdn_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF1_bcdn.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF1_udsgup_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF1_udsgup.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF1_udsgdn_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF1_udsgdn.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF2_bcup_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF2_bcup.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF2_bcdn_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF2_bcdn.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF2_udsgup_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF2_udsgup.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_BtagSF2_udsgdn_"+tmpKeyStrT;
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_BtagSF2_udsgdn.push_back((TH2D*)h2_tmp->Clone());

      tmpKeyStrNPassedT = "NPassed_"+tmpKeyStrT+"_very";
      h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_very.push_back((TH2D*)h2_tmp->Clone());
   }
   
   double massMom_very, massDau_very; double num_very;
   ifstream totalFile("out_total.txt");
   while(totalFile >> massMom_very >> massDau_very >> num_very){
      h2_NTotal_very->Fill(massMom_very, massDau_very, num_very);
   }

   ifstream baselineFile("out_baseline.txt");
   while(baselineFile >> massMom_very >> massDau_very >> num_very){
      h2_NPassed_very[0]->Fill(massMom_very, massDau_very, num_very);
   }

   ifstream highMETFile("out_highMET.txt");
   while(highMETFile >> massMom_very >> massDau_very >> num_very){
      h2_NPassed_very[1]->Fill(massMom_very, massDau_very, num_very);
   }

// For PDF sys.
   std::vector<TH2D*> h2_NTotal_CTEQ, h2_NTotal_MSTW, h2_NTotal_NNPDF;
   std::vector<std::vector<TH2D*> > h2_NPassed_CTEQ, h2_NPassed_MSTW, h2_NPassed_NNPDF;
   for(int ic=0; ic<nCompCTEQ; ic++){
      sprintf(names, "PDFWgt1_NTotal_%d", ic);
      TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
      h2_NTotal_CTEQ.push_back((TH2D*)h2_tmpTotal->Clone());
      std::vector<TH2D*> h2_tmpVec;
      sprintf(names, "PDFWgt1_NPassed_%d_", ic);
      for(int is=0; is<nStages; is++){
         TString tmpKeyStrT = keyStrForStages[is];
         TString tmpKeyStrNPassedT = names+tmpKeyStrT;
         TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
         h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
      }
      h2_NPassed_CTEQ.push_back(h2_tmpVec);
   }

   for(int im=0; im<nCompMSTW; im++){
      sprintf(names, "PDFWgt2_NTotal_%d", im);
      TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
      h2_NTotal_MSTW.push_back((TH2D*)h2_tmpTotal->Clone());
      std::vector<TH2D*> h2_tmpVec;
      sprintf(names, "PDFWgt2_NPassed_%d_", im);
      for(int is=0; is<nStages; is++){
         TString tmpKeyStrT = keyStrForStages[is];
         TString tmpKeyStrNPassedT = names+tmpKeyStrT;
         TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
         h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
      }
      h2_NPassed_MSTW.push_back(h2_tmpVec);
   }

   for(int in=0; in<nCompNNPDF; in++){
      sprintf(names, "PDFWgt3_NTotal_%d", in);
      TH2D * h2_tmpTotal = new TH2D(names, names, 60, 0, 1500, 60, 0, 1500); h2_tmpTotal->Sumw2();
      h2_NTotal_NNPDF.push_back((TH2D*)h2_tmpTotal->Clone());
      std::vector<TH2D*> h2_tmpVec;
      sprintf(names, "PDFWgt3_NPassed_%d_", in);
      for(int is=0; is<nStages; is++){
         TString tmpKeyStrT = keyStrForStages[is];
         TString tmpKeyStrNPassedT = names+tmpKeyStrT;
         TH2D * h2_tmpNPassed = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmpNPassed->Sumw2();
         h2_tmpVec.push_back((TH2D*)h2_tmpNPassed->Clone());
      }
      h2_NPassed_NNPDF.push_back(h2_tmpVec);
   }

// ISR/FSR
   TH2D * h2_NTotal_ISRFSR = new TH2D("NTotal_ISRFSR", "NTotal_ISRFSR", 60, 0, 1500, 60, 0, 1500); h2_NTotal_ISRFSR->Sumw2();
   std::vector<TH2D*> h2_NPassed_ISRFSR;
   for(int is=0; is<nStages; is++){
      TString tmpKeyStrT = keyStrForStages[is];
      TString tmpKeyStrNPassedT = "NPassed_ISRFSR_"+tmpKeyStrT;
      TH2D * h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp->Sumw2();
      h2_NPassed_ISRFSR.push_back((TH2D*)h2_tmp->Clone());
   }

// JES UP/DN
   TH2D * h2_NTotal_JESUP = new TH2D("NTotal_JESUP", "NTotal_JESUP", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JESUP->Sumw2();
   TH2D * h2_NTotal_JESDN = new TH2D("NTotal_JESDN", "NTotal_JESDN", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JESDN->Sumw2();
   std::vector<TH2D*> h2_NPassed_JESUP, h2_NPassed_JESDN;
   for(int is=0; is<nStages; is++){
      TString tmpKeyStrT = keyStrForStages[is];
      TString tmpKeyStrNPassedT = "NPassed_JESUP_"+tmpKeyStrT;
      TH2D * h2_tmp_jesup = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jesup->Sumw2();
      h2_NPassed_JESUP.push_back((TH2D*)h2_tmp_jesup->Clone());

      tmpKeyStrNPassedT = "NPassed_JESDN_"+tmpKeyStrT;
      TH2D * h2_tmp_jesdn = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jesdn->Sumw2();
      h2_NPassed_JESDN.push_back((TH2D*)h2_tmp_jesdn->Clone());
   }

// JER UP/DN
   TH2D * h2_NTotal_JER   = new TH2D("NTotal_JER", "NTotal_JER", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JER->Sumw2();
   TH2D * h2_NTotal_JERUP = new TH2D("NTotal_JERUP", "NTotal_JERUP", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JERUP->Sumw2();
   TH2D * h2_NTotal_JERDN = new TH2D("NTotal_JERDN", "NTotal_JERDN", 60, 0, 1500, 60, 0, 1500); h2_NTotal_JERDN->Sumw2();
   std::vector<TH2D*> h2_NPassed_JER, h2_NPassed_JERUP, h2_NPassed_JERDN;
   for(int is=0; is<nStages; is++){
      TString tmpKeyStrT = keyStrForStages[is];
      TString tmpKeyStrNPassedT = "NPassed_JER_"+tmpKeyStrT;
      TH2D * h2_tmp_jer = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jer->Sumw2();
      h2_NPassed_JER.push_back((TH2D*)h2_tmp_jer->Clone());

      tmpKeyStrNPassedT = "NPassed_JERUP_"+tmpKeyStrT;
      TH2D * h2_tmp_jerup = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jerup->Sumw2();
      h2_NPassed_JERUP.push_back((TH2D*)h2_tmp_jerup->Clone());

      tmpKeyStrNPassedT = "NPassed_JERDN_"+tmpKeyStrT;
      TH2D * h2_tmp_jerdn = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 60, 0, 1500, 60, 0, 1500); h2_tmp_jerdn->Sumw2();
      h2_NPassed_JERDN.push_back((TH2D*)h2_tmp_jerdn->Clone());
   }

   std::vector<ULong64_t> evtIDvec;
   std::vector<bool> evtPassBasicvec, evtPassStopvec, evtPassStopNewvec;
   std::vector<double> evtmetvec, evtmetphivec;

// Use the standard tree
   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
   std::cout<<"\nra2StdChn->GetEntries : "<<ra2StdChn->GetEntries()<<std::endl;
   for(unsigned int ie=0; ie<ra2StdChn->GetEntries(); ie++){
      ra2StdChn->GetEntry(ie);

      if( ie%100000 == 0 || ie == ra2StdChn->GetEntries()-1 ) std::cout<<"Processing the "<<ie<<"th event ... "<<std::endl;

      // current ISR weights from Roberto do not go above LSP mass>400 GeV
      if(massDau > 400 ) continue;

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

      //===== calculate btag reweighing factor ======
      std::vector<TLorentzVector> newJetsLVec;
      std::vector<double> newJetsCSVS;
      std::vector<double> newJetsFlavor;
      if( JetsNum != BTagCSVNum ) std::cout<<"ERROR ... JetsNum : "<<JetsNum<<"  !=  BTagCSVNum : "<<BTagCSVNum<<std::endl;
      for(int ij=0; ij<JetsNum; ij++){
	double jetPt = JetsPt[ij], jetEta = JetsEta[ij], jetPhi = JetsPhi[ij], jetE = JetsE[ij];
	if( jetPt < minJetPt || std::abs(jetEta)>2.4 ) continue;
	TLorentzVector perJetLVec; perJetLVec.SetPtEtaPhiE(jetPt, jetEta, jetPhi, jetE);
	newJetsLVec.push_back(perJetLVec); 
	newJetsCSVS.push_back(BTagCSV[ij]);
	newJetsFlavor.push_back(std::abs(JetFlavor[ij]));
      }
      double weightBtagSF         = WeightBtagSF         (newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4);
      double weightBtagSF1_epbc   = WeightBtagSF1_Errbc  (newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, true);
      double weightBtagSF1_embc   = WeightBtagSF1_Errbc  (newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, false);
      double weightBtagSF1_epudsg = WeightBtagSF1_Errudsg(newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, true);
      double weightBtagSF1_emudsg = WeightBtagSF1_Errudsg(newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, false);
      double weightBtagSF2_epbc   = WeightBtagSF2_Errbc  (newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, true);
      double weightBtagSF2_embc   = WeightBtagSF2_Errbc  (newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, false);
      double weightBtagSF2_epudsg = WeightBtagSF2_Errudsg(newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, true);
      double weightBtagSF2_emudsg = WeightBtagSF2_Errudsg(newJetsLVec, newJetsCSVS, newJetsFlavor, 30.0, 2.4, false);

      /*
      std::cout << "WeightBtagSF " << weightBtagSF <<" "
		<< "err1bc "   << weightBtagSF1_epbc   << " " << weightBtagSF1_embc   << " "
		<< "err1udsg " << weightBtagSF1_epudsg << " " << weightBtagSF1_emudsg << " "
		<< "err2bc "   << weightBtagSF2_epbc   << " " << weightBtagSF2_embc   << " "
		<< "err2udsg " << weightBtagSF2_epudsg << " " << weightBtagSF2_emudsg << " "
		<< std::endl;
      */
      //===== ===== ===== =====  ===== 
      // get ISR weight for madgraph samples
      double weightISRmg    = MGISRWeight(massMom, massDau, SusyMoSumPt, 0);
      double weightISRmg_up = MGISRWeight(massMom, massDau, SusyMoSumPt, 1);
      double weightISRmg_dn = MGISRWeight(massMom, massDau, SusyMoSumPt, 2);
      /*
      std::cout << "mo " << massMom << " dau " << massDau << " boost " << SusyMoSumPt << " "
		<< "weightISRmg " << weightISRmg << " "
		<< " + " << weightISRmg_up << " - " << weightISRmg_dn
		<< std::endl;
      */
      //std::cout << "================ "<<std::endl;

      bool passBasic = false;
      if(    Filter_trackingFailureFilter
          && Filter_inconsistentMuons && Filter_greedyMuons
          && Filter_ra2EcalTPFilter && Filter_ra2EcalBEFilter
          && Filter_hcalLaserEventFilter 
          && Filter_eeBadScFilter 
//          && Filter_ecalLaserCorrFilter 
          && Filter_PBNRFilter
          && !StopSAKLooseElectronSelectorNum && !StopSAKLooseMuonSelectorNum){
         passBasic = true;
      }
      evtIDvec.push_back(comb); evtPassBasicvec.push_back(passBasic);

      double met = PATMETsPFPhiCorrPt[0], metphi = PATMETsPFPhiCorrPhi[0];
//      double met = PATMETsPFPt[0], metphi = PATMETsPFPhi[0];
      bool passStop    = passStopSearch(met, metphi); 
      bool passStopNew = passStopSearchNew(met, metphi); 
      evtPassStopvec.push_back(passStop);
      evtPassStopNewvec.push_back(passStopNew);
      evtmetvec.push_back(met); evtmetphivec.push_back(metphi);

      if( !passBasic ) continue;

      //if( !passStop ) continue;
     
      for(int is=0; is<nStages; is++){
	if(is<specialStageIdx) {
	  if( passStop &&  met>cutMETArr[is]){
	    
	    //old nominal 
	    h2_NPassed_stdRA2NoBtagNoISRWt[is]->Fill(massMom, massDau, Weight);
	    
	    //varying only ISR weight to check the effect
	    h2_NPassed_stdRA2ISRWt   [is]->Fill(massMom, massDau, Weight*weightISRmg);

	    //varying only Btag eff/mistag weight to check the effect
	    h2_NPassed_stdRA2BtagWt  [is]->Fill(massMom, massDau, Weight*weightBtagSF);
	   
	   //nominal acceptance is one corrected for btag & isr weights
            h2_NPassed_stdRA2        [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF);

	    h2_NPassed_ISRWeight_up  [is]->Fill(massMom, massDau, Weight*weightISRmg_up*weightBtagSF);
	    h2_NPassed_ISRWeight_dn  [is]->Fill(massMom, massDau, Weight*weightISRmg_dn*weightBtagSF);

	    h2_NPassed_BtagSF1_bcup  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_epbc  );
	    h2_NPassed_BtagSF1_bcdn  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_embc  );
	    h2_NPassed_BtagSF1_udsgup[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_epudsg);
	    h2_NPassed_BtagSF1_udsgdn[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_emudsg);
	    h2_NPassed_BtagSF2_bcup  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_epbc  );
	    h2_NPassed_BtagSF2_bcdn  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_embc  );
	    h2_NPassed_BtagSF2_udsgup[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_epudsg);
	    h2_NPassed_BtagSF2_udsgdn[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_emudsg);
	    
            h2_NPassed_ISRFSR[is]->Fill(massMom, massDau, Weight*ISRWeight);
            for(int ic=0; ic<nCompCTEQ; ic++){
               h2_NPassed_CTEQ[ic][is]->Fill(massMom, massDau, Weight*cteq66[ic]);
            }
            for(int im=0; im<nCompMSTW; im++){
               h2_NPassed_MSTW[im][is]->Fill(massMom, massDau, Weight*MSTW2008nlo68cl[im]);
            }
            for(int in=0; in<nCompNNPDF; in++){
               h2_NPassed_NNPDF[in][is]->Fill(massMom, massDau, Weight*NNPDF20[in]);
            }
         }
	} else {
	  if(passStopNew && met>cutMETArr[is]) {

	    //old nominal 
	    h2_NPassed_stdRA2NoBtagNoISRWt[is]->Fill(massMom, massDau, Weight);
	    
	    //varying only ISR weight to check the effect
	    h2_NPassed_stdRA2ISRWt   [is]->Fill(massMom, massDau, Weight*weightISRmg);

	    //varying only Btag eff/mistag weight to check the effect
	    h2_NPassed_stdRA2BtagWt  [is]->Fill(massMom, massDau, Weight*weightBtagSF);
	   
	   //nominal acceptance is one corrected for btag & isr weights
            h2_NPassed_stdRA2        [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF);

	    h2_NPassed_ISRWeight_up  [is]->Fill(massMom, massDau, Weight*weightISRmg_up*weightBtagSF);
	    h2_NPassed_ISRWeight_dn  [is]->Fill(massMom, massDau, Weight*weightISRmg_dn*weightBtagSF);

	    h2_NPassed_BtagSF1_bcup  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_epbc  );
	    h2_NPassed_BtagSF1_bcdn  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_embc  );
	    h2_NPassed_BtagSF1_udsgup[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_epudsg);
	    h2_NPassed_BtagSF1_udsgdn[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF1_emudsg);
	    h2_NPassed_BtagSF2_bcup  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_epbc  );
	    h2_NPassed_BtagSF2_bcdn  [is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_embc  );
	    h2_NPassed_BtagSF2_udsgup[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_epudsg);
	    h2_NPassed_BtagSF2_udsgdn[is]->Fill(massMom, massDau, Weight*weightISRmg*weightBtagSF2_emudsg);

            h2_NPassed_ISRFSR[is]->Fill(massMom, massDau, Weight*ISRWeight);
            for(int ic=0; ic<nCompCTEQ; ic++){
               h2_NPassed_CTEQ[ic][is]->Fill(massMom, massDau, Weight*cteq66[ic]);
            }
            for(int im=0; im<nCompMSTW; im++){
               h2_NPassed_MSTW[im][is]->Fill(massMom, massDau, Weight*MSTW2008nlo68cl[im]);
            }
            for(int in=0; in<nCompNNPDF; in++){
               h2_NPassed_NNPDF[in][is]->Fill(massMom, massDau, Weight*NNPDF20[in]);
            }
	  }
	}
      } // loop over nStages
   } // loop over STD tree

// Use JESUP tree
   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
   std::cout<<"\nra2JesUpChn->GetEntries : "<<ra2JesUpChn->GetEntries()<<std::endl;
   for(unsigned int ie=0; ie<ra2JesUpChn->GetEntries(); ie++){
      ra2JesUpChn->GetEntry(ie);

      h2_NTotal_JESUP->Fill(massMom, massDau, Weight);
      
      UInt_t runPLUSlumi = RunNum*10000 + LumiBlockNum;
      ULong64_t comb = ((ULong64_t)runPLUSlumi << 32) | EvtNum;
      
      std::vector<ULong64_t>::iterator low;
      low = std::lower_bound(evtIDvec.begin(), evtIDvec.end(), comb);
      int posLow = std::distance(evtIDvec.begin(), low); 
      bool passBasic = evtPassBasicvec[posLow];
      bool passStop = evtPassStopvec[posLow];
      bool passStopNew = evtPassStopNewvec[posLow];

      double met = evtmetvec[posLow];
            
      if( !passBasic ) continue;
      //if( !passStop ) continue;

      for(int is=0; is<nStages; is++){
	if(is<specialStageIdx) {
	  if( passStop &&  met > cutMETArr[is] ){
            h2_NPassed_JESUP[is]->Fill(massMom, massDau, Weight);
	  }
	} else {
	  if(passStopNew && met>cutMETArr[is]) {
            h2_NPassed_JESUP[is]->Fill(massMom, massDau, Weight);
	   }
	 }
      }
   } // JESUP tree

// Use JESDN tree
   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
   std::cout<<"\nra2JesDnChn->GetEntries : "<<ra2JesDnChn->GetEntries()<<std::endl;
   for(unsigned int ie=0; ie<ra2JesDnChn->GetEntries(); ie++){
      ra2JesDnChn->GetEntry(ie);

      h2_NTotal_JESDN->Fill(massMom, massDau, Weight);
      
      UInt_t runPLUSlumi = RunNum*10000 + LumiBlockNum;
      ULong64_t comb = ((ULong64_t)runPLUSlumi << 32) | EvtNum;
      
      std::vector<ULong64_t>::iterator low;
      low = std::lower_bound(evtIDvec.begin(), evtIDvec.end(), comb);
      int posLow = std::distance(evtIDvec.begin(), low); 
      bool passBasic = evtPassBasicvec[posLow];
      bool passStop = evtPassStopvec[posLow];
      bool passStopNew = evtPassStopNewvec[posLow];
            
      if( !passBasic ) continue;
      //if( !passStop ) continue;
 
      double met = evtmetvec[posLow];
            
      for(int is=0; is<nStages; is++){
	if(is<specialStageIdx) {
	  if( passStop &&  met > cutMETArr[is] ){
            h2_NPassed_JESDN[is]->Fill(massMom, massDau, Weight);
	  }
	} else {
	  if(passStopNew && met>cutMETArr[is]) {
	    h2_NPassed_JESUP[is]->Fill(massMom, massDau, Weight);
	  }
	}
      }
   } // JESDN tree

   TFile *prodSignalFile = new TFile("prodSignal.root", "RECREATE");
   prodSignalFile->cd();
   TDirectory *stdDir = prodSignalFile->mkdir("STD");
   TDirectory *jesDir = prodSignalFile->mkdir("JES");
   TDirectory *jerDir = prodSignalFile->mkdir("JER");
   TDirectory *isrDir = prodSignalFile->mkdir("ISR");
   TDirectory *pdfDir = prodSignalFile->mkdir("PDF");

   TDirectory *veryDir = prodSignalFile->mkdir("VERY");

   veryDir->cd();
   h2_NTotal_very->Write();
   for(int is=0; is<nStages; is++){ h2_NPassed_very[is]->Write(); }

   stdDir->cd();
   h2_NTotal->Write();
   for(int is=0; is<nStages; is++){ h2_NPassed_stdRA2[is]->Write(); }
   for(int is=0; is<nStages; is++){ 
     h2_NPassed_stdRA2NoBtagNoISRWt[is]->Write(); 
     h2_NPassed_stdRA2ISRWt   [is]->Write(); 
     h2_NPassed_stdRA2BtagWt  [is]->Write(); 
     h2_NPassed_ISRWeight_up  [is]->Write();
     h2_NPassed_ISRWeight_dn  [is]->Write();
     h2_NPassed_BtagSF1_bcup  [is]->Write();
     h2_NPassed_BtagSF1_bcdn  [is]->Write();
     h2_NPassed_BtagSF1_udsgup[is]->Write();
     h2_NPassed_BtagSF1_udsgdn[is]->Write();
     h2_NPassed_BtagSF2_bcup  [is]->Write();
     h2_NPassed_BtagSF2_bcdn  [is]->Write();
     h2_NPassed_BtagSF2_udsgup[is]->Write();
     h2_NPassed_BtagSF2_udsgdn[is]->Write();
   }

   jesDir->cd();
   h2_NTotal_JESUP->Write(); h2_NTotal_JESDN->Write();
   for(int is=0; is<nStages; is++){ h2_NPassed_JESUP[is]->Write(); h2_NPassed_JESDN[is]->Write(); }

   jerDir->cd();
   h2_NTotal_JER->Write(); h2_NTotal_JERUP->Write(); h2_NTotal_JERDN->Write();
   for(int is=0; is<nStages; is++){ h2_NPassed_JER[is]->Write(); h2_NPassed_JERUP[is]->Write(); h2_NPassed_JERDN[is]->Write(); }

   isrDir->cd();
   h2_NTotal_ISRFSR->Write();
   for(int is=0; is<nStages; is++){ h2_NPassed_ISRFSR[is]->Write(); }

   pdfDir->cd();
   for(int is=0; is<nStages; is++){
      for(int ic=0; ic<nCompCTEQ; ic++){
         if( is==0 ){
            h2_NTotal_CTEQ[ic]->Write();
         }
         h2_NPassed_CTEQ[ic][is]->Write();
      }
      for(int im=0; im<nCompMSTW; im++){
         if( is==0 ){
            h2_NTotal_MSTW[im]->Write();
         }
         h2_NPassed_MSTW[im][is]->Write();
      }
      for(int in=0; in<nCompNNPDF; in++){
         if( is==0 ){
            h2_NTotal_NNPDF[in]->Write();
         }
         h2_NPassed_NNPDF[in][is]->Write(); 
      }
   }
   prodSignalFile->Write(); prodSignalFile->Close();

   TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

   Float_t legendX1 = .50;
   Float_t legendX2 = .85;
   Float_t legendY1 = .62;
   Float_t legendY2 = .85;
   TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
   catLeg1->SetTextSize(0.025);

   c1->cd(); //gPad->SetLogy();
   gPad->RedrawAxis();

   c1->Print("signalScan.pdf[");

   c1->cd();
   h2_NTotal->SetAxisRange(200, 900, "X");
   h2_NTotal->SetAxisRange(0, 400, "Y");
   h2_NTotal->Draw("text");
   c1->Print("signalScan.pdf");

   c1->cd();
   h2_NTotal_very->SetAxisRange(200, 900, "X");
   h2_NTotal_very->SetAxisRange(0, 400, "Y");
   h2_NTotal_very->Draw("text");
   c1->Print("signalScan.pdf");

   for(int is=0; is<nStages; is++){
      c1->cd();
      h2_NPassed_stdRA2[is]->SetAxisRange(200, 900, "X");
      h2_NPassed_stdRA2[is]->SetAxisRange(0, 400, "Y");
      h2_NPassed_stdRA2[is]->Draw("text");
      c1->Print("signalScan.pdf");
      c1->cd();
      h2_NPassed_very[is]->SetAxisRange(200, 900, "X");
      h2_NPassed_very[is]->SetAxisRange(0, 400, "Y");
      h2_NPassed_very[is]->Draw("text");
      c1->Print("signalScan.pdf");
   }

   c1->Print("signalScan.pdf]");

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

