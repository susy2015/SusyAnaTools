#include "tophead.h"
#include "THStack.h"
#include "TColor.h"
#include "tdrstyle.C"

#ifdef __MAKECINT__
#pragma link C++ class vector<vector<TFile*> >;
#pragma link C++ class vector<vector<TH1D*> >;
#pragma link C++ class vector< map<int, int> >;
#endif

static const double Azero = 0, mu = 1.0, TanBeta = 10.0;

static const int nJetBins = 3;
static const int cutLonJetsArr[] = { 3, 6,  8 };
static const int cutHinJetsArr[] = { 6, 8, -1 };

// Remove bins for MHT [100, 200] where no predictions from backgrounds as of now
static const double cutLoHTArr[]  = { 500, 500, 500, 500,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1500, 1500, /**/  500,  500,  500,  800,  800,  800, 1000, 1000, 1000, 1250, 1250, 1250, 1500, 1500, /**/  500,  800, 1000, 1250, 1500 };
static const double cutHiHTArr[]  = { 800, 800, 800, 800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500,   -1,   -1, /**/  800,  800,  800, 1000, 1000, 1000, 1250, 1250, 1250, 1500, 1500, 1500,   -1,   -1, /**/  800, 1000, 1250, 1500,   -1 };
static const double cutLoMHTArr[] = { 200, 300, 450, 600,  200,  300,  450,  600,  200,  300,  450,  600,  200,  300,  450,  200,  300, /**/  200,  300,  450,  200,  300,  450,  200,  300,  450,  200,  300,  450,  200,  300, /**/  200,  200,  200,  200,  200 };
static const double cutHiMHTArr[] = { 300, 450, 600,  -1,  300,  450,  600,   -1,  300,  450,  600,   -1,  300,  450,   -1,  300,   -1, /**/  300,  450,   -1,  300,  450,   -1,  300,  450,   -1,  300,  450,   -1,  300,   -1, /**/   -1,   -1,   -1,   -1,   -1 };

// Remove further bins where any of the backgrounds have no predictions (0 prediction or -1)
//static const double cutLoHTArr[]  = { 500, 500, 500, 500,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1500, 1500, /**/  500,  500,   800,  800,  800, 1000, 1000, 1250, 1250, 1500, 1500, /**/   800, 1000, 1250 };
//static const double cutHiHTArr[]  = { 800, 800, 800, 800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500,   -1,   -1, /**/  800,  800,  1000, 1000, 1000, 1250, 1250, 1500, 1500,   -1,   -1, /**/  1000, 1250, 1500 };
//static const double cutLoMHTArr[] = { 200, 300, 450, 600,  200,  300,  450,  600,  200,  300,  450,  600,  200,  300,  450,  200,  300, /**/  200,  300,   200,  300,  450,  200,  300,  200,  300,  200,  300, /**/   200,  200,  200 };
//static const double cutHiMHTArr[] = { 300, 450, 600,  -1,  300,  450,  600,   -1,  300,  450,  600,   -1,  300,  450,   -1,  300,   -1, /**/  300,  450,   300,  450,   -1,  300,  450,  300,  450,  300,   -1, /**/    -1,   -1,   -1 };

// Full binning for nJets>=3
//static const double cutLoHTArr[]  = { 500, 500, 500, 500,  800,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, /**/  500,  500,  500,  800,  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, /**/  500,  800,  800, 1000, 1000, 1250, 1250, 1500, 1500 };
//static const double cutHiHTArr[]  = { 800, 800, 800, 800, 1000, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1250, 1500, 1500, 1500, 1500,   -1,   -1,   -1, /**/  800,  800,  800, 1000, 1000, 1000, 1000, 1250, 1250, 1250, 1250, 1500, 1500, 1500, 1500,   -1,   -1,   -1, /**/  800, 1000, 1000, 1250, 1250, 1500, 1500,   -1,   -1 };
//static const double cutLoMHTArr[] = { 200, 300, 450, 600,  100,  200,  300,  450,  600,  100,  200,  300,  450,  600,  100,  200,  300,  450,  100,  200,  300, /**/  200,  300,  450,  100,  200,  300,  450,  100,  200,  300,  450,  100,  200,  300,  450,  100,  200,  300, /**/  200,  100,  200,  100,  200,  100,  200,  100,  200 };
//static const double cutHiMHTArr[] = { 300, 450, 600,  -1,  200,  300,  450,  600,   -1,  200,  300,  450,  600,   -1,  200,  300,  450,   -1,  200,  300,   -1, /**/  300,  450,   -1,  200,  300,  450,   -1,  200,  300,  450,   -1,  200,  300,  450,   -1,  200,  300,   -1, /**/   -1,  200,   -1,  200,   -1,  200,   -1,  200,   -1 };

static const int nsels = sizeof(cutLoHTArr)/sizeof(cutLoHTArr[0]);
static const int nuisances = 2; // stat; sys;

vector<string> keyStrForStages;

// From AN
static const double dataLumi = 19461.5; // in pb-1
static const double dataLumiRelUnc = 0.044;

static const double filterRelUnc = 0.03;
// Adapt to Christian's ...  And it's not included in the sigal cards since Christian is doing it himself.
static const double triggerRelUnc = 0.02;

// Read xsec from Maria's root file
// Only depends on m_gluino and/or m_squark
vector<double> xsecVec;
vector< vector< vector<double> > > scanEffvec, scanEffErrvec, scanEffStatErrvec, scanEffSystErrvec;

// Definition from Maria, since her maximum range of m_gluino is 1000 GeV.
// m0 is x axis --> m_gluino (or m_squark)
// m12 is y axis --> m_LSP
static const int nRangem0 = 1;

static const double divm0[nRangem0+1] = {0, 1500};
static const int ndivm0[nRangem0]   =   {  60   };
double avgDivm0 = (divm0[1]-divm0[0])/ndivm0[0];

static const int nRangem12 = 1;
static const double divm12[nRangem12+1] = {0, 1500};
static const int ndivm12[nRangem12]   =   {  60   };
double avgDivm12 = (divm12[1]-divm12[0])/ndivm12[0];

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

void prodLimitInputs(const std::string topo="T1qqqq"){

   gROOT->ProcessLine(".L tdrstyle.C");
   setTDRStyle();

   char names[200];
   int cntJetBin = 0;
   for(int is=0; is<nsels; is++){
      sprintf(names, "%dNJet%d_%3.0fHT%3.0f_%3.0fMHT%3.0f", cutLonJetsArr[cntJetBin], cutHinJetsArr[cntJetBin], cutLoHTArr[is], cutHiHTArr[is], cutLoMHTArr[is], cutHiMHTArr[is]);
      TString namesT = names;
      namesT.ReplaceAll(" -1", "inf"); namesT.ReplaceAll("-1", "inf");
      std::cout<<"is : "<<is<<"  namesT : "<<namesT<<std::endl;
      keyStrForStages.push_back(namesT.Data());
      if( cutHiHTArr[is] ==-1 && cutHiMHTArr[is] == -1 ) cntJetBin++;
   }

   vector<double> divBinsm0Vec; makeDivBins(nRangem0, divm0, ndivm0, divBinsm0Vec);
   int totDivBinsm0 = (int)divBinsm0Vec.size()-1; double divBinsm0[totDivBinsm0+1];
   cout<<"\ndivBins m0 --> totDivBinsm0 : "<<totDivBinsm0<<"  avgDivm0 : "<<avgDivm0<<endl;
   for(int ib=0; ib<totDivBinsm0+1; ib++){ divBinsm0[ib] = divBinsm0Vec[ib]; cout<<"  "<<divBinsm0Vec[ib]<<endl; }

   vector<double> divBinsm12Vec; makeDivBins(nRangem12, divm12, ndivm12, divBinsm12Vec);
   int totDivBinsm12 = (int)divBinsm12Vec.size()-1; double divBinsm12[totDivBinsm12+1];
   cout<<"\ndivBins m12 --> totDivBinsm12 : "<<totDivBinsm12<<"  avgDivm12 : "<<avgDivm12<<endl;
   for(int ib=0; ib<totDivBinsm12+1; ib++){ divBinsm12[ib] = divBinsm12Vec[ib]; cout<<"  "<<divBinsm12Vec[ib]<<endl; }

   xsecVec.clear(); xsecVec.resize(totDivBinsm0);
   vector<double> dummyVec; dummyVec.resize(totDivBinsm0);
   vector<vector<double> > dummy2Vec;
   for(int ib=0; ib<totDivBinsm12; ib++){ dummy2Vec.push_back(dummyVec); }
   for(int is=0; is<nsels; is++){ scanEffvec.push_back(dummy2Vec); scanEffErrvec.push_back(dummy2Vec); scanEffStatErrvec.push_back(dummy2Vec); scanEffSystErrvec.push_back(dummy2Vec); }

   double m0, m12, tanbeta;

   TH1D * xSecProspino =0;

   TFile *xSecFile = new TFile("reference_xSec.root");
   if( topo == "T1tttt" || topo == "T1" || topo == "T5ZZInc" || topo == "T1bbbb" || topo == "T1qqqq" ){
      if( xSecFile) xSecProspino = (TH1D*)xSecFile->Get("gluino_xsection"); 
   }
   if( topo == "T2tt" ){
      if( xSecFile) xSecProspino = (TH1D*)xSecFile->Get("stop_xsection");
   }
   if( topo == "T2" || topo == "T2qq" || topo == "TGQ" ){
      if( xSecFile) xSecProspino = (TH1D*)xSecFile->Get("squark_xsection");
   }

   for(int im0=0; im0<totDivBinsm0; im0++){
      xsecVec[im0] = 1.0; //xsec does not matter in the limit calculation
      if( xSecProspino ){
         m0 = divBinsm0Vec[im0];
         int ibm0 = xSecProspino->GetXaxis()->FindBin(m0);
         double xSec = xSecProspino->GetBinContent(ibm0);
         xsecVec[im0] = xSec;
      }
   }

   char filenames[200];
   sprintf(filenames, "EffUnc_SMS_%s.root", topo.c_str());
   cout<<"\nOpen "<<filenames<<endl<<endl;
   TFile *scanFile = new TFile(filenames);

   vector< vector<double> > sumEffWeightsStatErrVec, sumEffWeightsAccPDFErrVec, sumEffWeightsJerErrVec, sumEffWeightsJesErrVec, sumEffWeightsIsrFsrErrVec;
   vector< vector<double> > sumEffWeightsVec;
   for(int ib=0; ib<totDivBinsm12+1; ib++){
      sumEffWeightsStatErrVec.push_back(dummyVec);
      sumEffWeightsAccPDFErrVec.push_back(dummyVec);
      sumEffWeightsJerErrVec.push_back(dummyVec);
      sumEffWeightsJesErrVec.push_back(dummyVec);
      sumEffWeightsIsrFsrErrVec.push_back(dummyVec);
      sumEffWeightsVec.push_back(dummyVec);
   }

   for(int is=0; is<nsels; is++){

      TString tmpExclStrT(keyStrForStages[is]);
//      std::cout<<"stage : "<<tmpExclStrT<<std::endl;

      sprintf(names, "h_RA2Eff_%s", tmpExclStrT.Data());
      TH2D *effHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2EffUnc_%s", tmpExclStrT.Data());
      TH2D *effUncHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2Unc_StatAccep_%s", tmpExclStrT.Data());
      TH2D *effStatUncHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2Unc_IsrFsr_%s", tmpExclStrT.Data());
      TH2D *effIsrFsrHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2Unc_JerUpDn_%s", tmpExclStrT.Data());
      TH2D *effJerUpDnHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2Unc_JesUpDn_%s", tmpExclStrT.Data());
      TH2D *effJesUpDnHist = (TH2D*) scanFile->Get(names);

      sprintf(names, "h_RA2Unc_PDF_%s", tmpExclStrT.Data());
      TH2D *effPDFHist = (TH2D*) scanFile->Get(names);

      TAxis *xAxis = effHist->GetXaxis();
      TAxis *yAxis = effHist->GetYaxis();

      double minStatErr = 1e06, maxStatErr = 0.0; int m0_minStatErr=-1, m12_minStatErr=-1, m0_maxStatErr = -1, m12_maxStatErr =-1;
      double minAccPDFErr = 1e06, maxAccPDFErr = 0.0; int m0_minAccPDFErr=-1, m12_minAccPDFErr=-1, m0_maxAccPDFErr = -1, m12_maxAccPDFErr =-1;
      double minJerErr = 1e06, maxJerErr = 0.0; int m0_minJerErr=-1, m12_minJerErr=-1, m0_maxJerErr = -1, m12_maxJerErr =-1;
      double minJesErr = 1e06, maxJesErr = 0.0; int m0_minJesErr=-1, m12_minJesErr=-1, m0_maxJesErr = -1, m12_maxJesErr =-1;
      double minIsrFsrErr = 1e06, maxIsrFsrErr = 0.0; int m0_minIsrFsrErr=-1, m12_minIsrFsrErr=-1, m0_maxIsrFsrErr = -1, m12_maxIsrFsrErr =-1;

      for(int i0=0; i0<totDivBinsm0; i0++){
         for(int i12=0; i12<totDivBinsm12; i12++){
            m0 = divBinsm0Vec[i0]; m12 = divBinsm12Vec[i12];
            int xBinIdx = xAxis->FindFixBin(m0);
            int yBinIdx = yAxis->FindFixBin(m12);
            double eff = effHist->GetBinContent(xBinIdx, yBinIdx);
            double relEffErr = effUncHist->GetBinContent(xBinIdx, yBinIdx);
            double relStatEffErr = effStatUncHist->GetBinContent(xBinIdx, yBinIdx);
            double relSystEffErr = 0;
            relSystEffErr = sqrt( relEffErr*relEffErr - relStatEffErr*relStatEffErr );
            if( relSystEffErr < 0 ){
               std::cout<<"WARNING ... NEGATIVE eff  i0 : "<<i0<<"  i12 : "<<i12<<"  relSystEffErr : "<<relSystEffErr<<std::endl;
               relSystEffErr =0;
            }

            scanEffvec[is][i12][i0] = eff;
            scanEffErrvec[is][i12][i0] = relEffErr;
            scanEffStatErrvec[is][i12][i0] = relStatEffErr;
            scanEffSystErrvec[is][i12][i0] = relSystEffErr;

            double relAccPDFErr = effPDFHist->GetBinContent(xBinIdx, yBinIdx);
            double relJerErr = effJerUpDnHist->GetBinContent(xBinIdx, yBinIdx);
            double relJesErr = effJesUpDnHist->GetBinContent(xBinIdx, yBinIdx);
            double relIsrFsrErr = effIsrFsrHist->GetBinContent(xBinIdx, yBinIdx);

            int xBinLargery =4;
            const int moreISRremoval = 3;
            xBinLargery += moreISRremoval;

            sumEffWeightsVec[i12][i0] += eff;
            sumEffWeightsStatErrVec[i12][i0] += eff*relStatEffErr;
            sumEffWeightsAccPDFErrVec[i12][i0] += eff*relAccPDFErr;
            sumEffWeightsJerErrVec[i12][i0] += eff*relJerErr;
            sumEffWeightsJesErrVec[i12][i0] += eff*relJesErr;
            sumEffWeightsIsrFsrErrVec[i12][i0] += eff*relIsrFsrErr;
           
            if( minStatErr > relStatEffErr && relStatEffErr!= 0 ){ minStatErr = relStatEffErr; m0_minStatErr = m0; m12_minStatErr = m12; }
            if( maxStatErr < relStatEffErr && relStatEffErr!= 0 ){ maxStatErr = relStatEffErr; m0_maxStatErr = m0; m12_maxStatErr = m12; }

            if( minAccPDFErr > relAccPDFErr && relAccPDFErr!= 0 ){ minAccPDFErr = relAccPDFErr; m0_minAccPDFErr = m0; m12_minAccPDFErr = m12; }
            if( maxAccPDFErr < relAccPDFErr && relAccPDFErr!= 0 ){ maxAccPDFErr = relAccPDFErr; m0_maxAccPDFErr = m0; m12_maxAccPDFErr = m12; }

            if( minJerErr > relJerErr && relJerErr> 0 ){ minJerErr = relJerErr; m0_minJerErr = m0; m12_minJerErr = m12; }
            if( maxJerErr < relJerErr && relJerErr> 0 ){ maxJerErr = relJerErr; m0_maxJerErr = m0; m12_maxJerErr = m12; }

            if( minJesErr > relJesErr && relJesErr> 0 ){ minJesErr = relJesErr; m0_minJesErr = m0; m12_minJesErr = m12; }
            if( maxJesErr < relJesErr && relJesErr> 0 ){ maxJesErr = relJesErr; m0_maxJesErr = m0; m12_maxJesErr = m12; }

            if( minIsrFsrErr > relIsrFsrErr && relIsrFsrErr> 0 ){ minIsrFsrErr = relIsrFsrErr; m0_minIsrFsrErr = m0; m12_minIsrFsrErr = m12; }
            if( maxIsrFsrErr < relIsrFsrErr && relIsrFsrErr> 0 ){ maxIsrFsrErr = relIsrFsrErr; m0_maxIsrFsrErr = m0; m12_maxIsrFsrErr = m12; }
         }
      }
      std::cout<<"\nFor the "<<is<<"th bin : "<<keyStrForStages[is].c_str()<<std::endl;
      printf("m0_minStatErr    : %4d  m12_minStatErr    : %4d  minStatErr    : %5.3e <--> m0_maxStatErr    : %4d  m12_maxStatErr    : %4d  maxStatErr    : %5.3e\n", m0_minStatErr, m12_minStatErr, minStatErr, m0_maxStatErr, m12_maxStatErr, maxStatErr);
      printf("m0_minAccPDFErr  : %4d  m12_minAccPDFErr  : %4d  minAccPDFErr  : %5.3e <--> m0_maxAccPDFErr  : %4d  m12_maxAccPDFErr  : %4d  maxAccPDFErr  : %5.3e\n", m0_minAccPDFErr, m12_minAccPDFErr, minAccPDFErr, m0_maxAccPDFErr, m12_maxAccPDFErr, maxAccPDFErr);
      printf("m0_minJerErr     : %4d  m12_minJerErr     : %4d  minJerErr     : %5.3e <--> m0_maxJerErr     : %4d  m12_maxJerErr     : %4d  maxJerErr     : %5.3e\n", m0_minJerErr, m12_minJerErr, minJerErr, m0_maxJerErr, m12_maxJerErr, maxJerErr);
      printf("m0_minJesErr     : %4d  m12_minJesErr     : %4d  minJesErr     : %5.3e <--> m0_maxJesErr     : %4d  m12_maxJesErr     : %4d  maxJesErr     : %5.3e\n", m0_minJesErr, m12_minJesErr, minJesErr, m0_maxJesErr, m12_maxJesErr, maxJesErr);
      printf("m0_minIsrFsrErr  : %4d  m12_minIsrFsrErr  : %4d  minIsrFsrErr  : %5.3e <--> m0_maxIsrFsrErr  : %4d  m12_maxIsrFsrErr  : %4d  maxIsrFsrErr  : %5.3e\n", m0_minIsrFsrErr, m12_minIsrFsrErr, minIsrFsrErr, m0_maxIsrFsrErr, m12_maxIsrFsrErr, maxIsrFsrErr);
      std::cout<<std::endl;
   }

   cout<<"\nSummarize of results ... "<<endl;
/*
   for(int is=0; is<nsels; is++){
      cout<<"\nfor the region of "<<keyStrForStages[is].c_str()<<endl;
      for(int i0=0; i0<totDivBinsm0; i0++){
         for(int i12=0; i12<totDivBinsm12; i12++){
            if( divBinsm12Vec[i12] > divBinsm0Vec[i0] -50 ) continue;
            printf("m0 : %4.0f  m12 : %3.0f  xsec : %5.3e  eff : %5.3e  relEffErr : %5.3e  effErr : %5.3e\n", divBinsm0Vec[i0], divBinsm12Vec[i12], xsecVec[i0], scanEffvec[is][i12][i0], scanEffErrvec[is][i12][i0], scanEffvec[is][i12][i0]*scanEffErrvec[is][i12][i0]);
         }
      }
   }
   cout<<endl;
*/
   TH2D *h2_effWeightsStatErr = new TH2D("effWeightsStatErr", "effWeightsStatErr", totDivBinsm0, &divBinsm0Vec[0], totDivBinsm12, &divBinsm12Vec[0]);
   TAxis *effWeightsXaxis = (TAxis*) h2_effWeightsStatErr->GetXaxis();
   TAxis *effWeightsYaxis = (TAxis*) h2_effWeightsStatErr->GetYaxis();
   TH2D *h2_effWeightsAccPDFErr = new TH2D("effWeightsAccPDFErr", "effWeightsAccPDFErr", totDivBinsm0, &divBinsm0Vec[0], totDivBinsm12, &divBinsm12Vec[0]);
   TH2D *h2_effWeightsJerErr = new TH2D("effWeightsJerErr", "effWeightsJerErr", totDivBinsm0, &divBinsm0Vec[0], totDivBinsm12, &divBinsm12Vec[0]);
   TH2D *h2_effWeightsJesErr = new TH2D("effWeightsJesErr", "effWeightsJesErr", totDivBinsm0, &divBinsm0Vec[0], totDivBinsm12, &divBinsm12Vec[0]);
   TH2D *h2_effWeightsIsrFsrErr = new TH2D("effWeightsIsrFsrErr", "effWeightsIsrFsrErr", totDivBinsm0, &divBinsm0Vec[0], totDivBinsm12, &divBinsm12Vec[0]);

   if(1){
      double minStatErr = 1e06, maxStatErr = 0.0; int m0_minStatErr=-1, m12_minStatErr=-1, m0_maxStatErr = -1, m12_maxStatErr =-1;
      double minAccPDFErr = 1e06, maxAccPDFErr = 0.0; int m0_minAccPDFErr=-1, m12_minAccPDFErr=-1, m0_maxAccPDFErr = -1, m12_maxAccPDFErr =-1;
      double minJerErr = 1e06, maxJerErr = 0.0; int m0_minJerErr=-1, m12_minJerErr=-1, m0_maxJerErr = -1, m12_maxJerErr =-1;
      double minJesErr = 1e06, maxJesErr = 0.0; int m0_minJesErr=-1, m12_minJesErr=-1, m0_maxJesErr = -1, m12_maxJesErr =-1;
      double minIsrFsrErr = 1e06, maxIsrFsrErr = 0.0; int m0_minIsrFsrErr=-1, m12_minIsrFsrErr=-1, m0_maxIsrFsrErr = -1, m12_maxIsrFsrErr =-1;

      double avgStatErr = 0, avgAccPDFErr =0, avgJerErr =0, avgJesErr =0, avgIsrFsrErr =0;
      int validPtsCnt =0;
      
      for(int i0=0; i0<totDivBinsm0; i0++){
         for(int i12=0; i12<totDivBinsm12; i12++){
            m0 = divBinsm0Vec[i0]; m12 = divBinsm12Vec[i12];

            double sumEffWeights = sumEffWeightsVec[i12][i0];
            double sumEffWeightsStatErr = sumEffWeightsStatErrVec[i12][i0];
            double sumEffWeightsAccPDFErr = sumEffWeightsAccPDFErrVec[i12][i0];
            double sumEffWeightsJerErr = sumEffWeightsJerErrVec[i12][i0];
            double sumEffWeightsJesErr = sumEffWeightsJesErrVec[i12][i0];
            double sumEffWeightsIsrFsrErr = sumEffWeightsIsrFsrErrVec[i12][i0];

            int xBinIdx = effWeightsXaxis->FindFixBin(m0);
            int yBinIdx = effWeightsYaxis->FindFixBin(m12);

            if( sumEffWeights == 0 ) continue;

            int xBinLargery =4;
            const int moreISRremoval = 3;
            xBinLargery += moreISRremoval;

            if( xBinIdx < xBinLargery || xBinIdx - yBinIdx < xBinLargery || m0<300 ) continue;

            double relStatEffErr = sumEffWeightsStatErr/sumEffWeights;
            double relAccPDFErr  = sumEffWeightsAccPDFErr/sumEffWeights;
            double relJerErr     = sumEffWeightsJerErr/sumEffWeights;
            double relJesErr     = sumEffWeightsJesErr/sumEffWeights;
            double relIsrFsrErr  = sumEffWeightsIsrFsrErr/sumEffWeights;

            h2_effWeightsStatErr->SetBinContent( xBinIdx, yBinIdx, relStatEffErr);
            h2_effWeightsAccPDFErr->SetBinContent( xBinIdx, yBinIdx, relAccPDFErr);
            h2_effWeightsJerErr->SetBinContent( xBinIdx, yBinIdx, relJerErr);
            h2_effWeightsJesErr->SetBinContent( xBinIdx, yBinIdx, relJesErr);
            h2_effWeightsIsrFsrErr->SetBinContent( xBinIdx, yBinIdx, relIsrFsrErr);

            validPtsCnt ++;
            avgStatErr += relStatEffErr;
            avgAccPDFErr += relAccPDFErr;
            avgJerErr += relJerErr;
            avgJesErr += relJesErr;
            avgIsrFsrErr += relIsrFsrErr;

            if( minStatErr > relStatEffErr && relStatEffErr!= 0 ){ minStatErr = relStatEffErr; m0_minStatErr = m0; m12_minStatErr = m12; }
            if( maxStatErr < relStatEffErr && relStatEffErr!= 0 ){ maxStatErr = relStatEffErr; m0_maxStatErr = m0; m12_maxStatErr = m12; }

            if( minAccPDFErr > relAccPDFErr && relAccPDFErr!= 0 ){ minAccPDFErr = relAccPDFErr; m0_minAccPDFErr = m0; m12_minAccPDFErr = m12; }
            if( maxAccPDFErr < relAccPDFErr && relAccPDFErr!= 0 ){ maxAccPDFErr = relAccPDFErr; m0_maxAccPDFErr = m0; m12_maxAccPDFErr = m12; }

            if( minJerErr > relJerErr && relJerErr> 0 ){ minJerErr = relJerErr; m0_minJerErr = m0; m12_minJerErr = m12; }
            if( maxJerErr < relJerErr && relJerErr> 0 ){ maxJerErr = relJerErr; m0_maxJerErr = m0; m12_maxJerErr = m12; }

            if( minJesErr > relJesErr && relJesErr> 0 ){ minJesErr = relJesErr; m0_minJesErr = m0; m12_minJesErr = m12; }
            if( maxJesErr < relJesErr && relJesErr> 0 ){ maxJesErr = relJesErr; m0_maxJesErr = m0; m12_maxJesErr = m12; }

            if( minIsrFsrErr > relIsrFsrErr && relIsrFsrErr> 0 ){ minIsrFsrErr = relIsrFsrErr; m0_minIsrFsrErr = m0; m12_minIsrFsrErr = m12; }
            if( maxIsrFsrErr < relIsrFsrErr && relIsrFsrErr> 0 ){ maxIsrFsrErr = relIsrFsrErr; m0_maxIsrFsrErr = m0; m12_maxIsrFsrErr = m12; }

         }
      }

      avgStatErr /= validPtsCnt;
      avgAccPDFErr /= validPtsCnt;
      avgJerErr /= validPtsCnt;
      avgJesErr /= validPtsCnt;
      avgIsrFsrErr /= validPtsCnt;

      std::cout<<"\nFor the efficiency weighted for validPtsCnt : "<<validPtsCnt<<std::endl;
      printf("m0_minStatErr    : %4d  m12_minStatErr    : %4d  minStatErr    : %5.3e <--> m0_maxStatErr    : %4d  m12_maxStatErr    : %4d  maxStatErr    : %5.3e\n", m0_minStatErr, m12_minStatErr, minStatErr, m0_maxStatErr, m12_maxStatErr, maxStatErr);
      printf("m0_minAccPDFErr  : %4d  m12_minAccPDFErr  : %4d  minAccPDFErr  : %5.3e <--> m0_maxAccPDFErr  : %4d  m12_maxAccPDFErr  : %4d  maxAccPDFErr  : %5.3e\n", m0_minAccPDFErr, m12_minAccPDFErr, minAccPDFErr, m0_maxAccPDFErr, m12_maxAccPDFErr, maxAccPDFErr);
      printf("m0_minJerErr     : %4d  m12_minJerErr     : %4d  minJerErr     : %5.3e <--> m0_maxJerErr     : %4d  m12_maxJerErr     : %4d  maxJerErr     : %5.3e\n", m0_minJerErr, m12_minJerErr, minJerErr, m0_maxJerErr, m12_maxJerErr, maxJerErr);
      printf("m0_minJesErr     : %4d  m12_minJesErr     : %4d  minJesErr     : %5.3e <--> m0_maxJesErr     : %4d  m12_maxJesErr     : %4d  maxJesErr     : %5.3e\n", m0_minJesErr, m12_minJesErr, minJesErr, m0_maxJesErr, m12_maxJesErr, maxJesErr);
      printf("m0_minIsrFsrErr  : %4d  m12_minIsrFsrErr  : %4d  minIsrFsrErr  : %5.3e <--> m0_maxIsrFsrErr  : %4d  m12_maxIsrFsrErr  : %4d  maxIsrFsrErr  : %5.3e\n", m0_minIsrFsrErr, m12_minIsrFsrErr, minIsrFsrErr, m0_maxIsrFsrErr, m12_maxIsrFsrErr, maxIsrFsrErr);
      printf("avgStatErr : %5.3e  avgAccPDFErr : %5.3e  avgJerErr : %5.3e  avgJesErr : %5.3e  avgIsrFsrErr : %5.3e\n", avgStatErr, avgAccPDFErr, avgJerErr, avgJesErr, avgIsrFsrErr);
      std::cout<<std::endl;
   }

   struct stat stFile;
   char commandline[200];

   int idxCnt = 0;
   char dirnames[200];
   sprintf(dirnames, "%s", topo.c_str()); 
   if( stat(dirnames, &stFile) == -1 ){ sprintf(commandline, "mkdir %s", dirnames); system(commandline); }
   for(int i0=0; i0<totDivBinsm0; i0++){
      for(int i12=0; i12<totDivBinsm12; i12++){
         m0 = divBinsm0Vec[i0]; m12 = divBinsm12Vec[i12];
         if( m12 > m0 -50 ) continue;
         idxCnt++;
         sprintf(filenames, "%s/mSUGRA_%d_%d_10_1_0_idx%d.dat", dirnames, (int)m0, (int)m12, idxCnt);
         ofstream fout(filenames);

         fout<<"luminosity = "<<dataLumi<<endl;
         fout<<"channels = "<<nsels<<endl;
         fout<<"nuisances = "<<nuisances<<endl;
         fout<<"sample = signal"<<endl<<endl;

         fout<<"Mzero = "<<m0<<endl;
         fout<<"Mhalf = "<<m12<<endl;
         fout<<"TanBeta = "<<TanBeta<<endl;
         fout<<"Azero = "<<Azero<<endl;
         fout<<"Mu = "<<mu<<endl<<endl;

         fout<<"Xsection.LO = "<<xsecVec[i0]<<endl;
         fout<<"Xsection.NLO = "<<xsecVec[i0]<<endl<<endl;

         fout<<"channel = ";
         for(int is=0; is<nsels; is++){
            if( is!= nsels-1 ) fout<<"bin"<<is+1<<"; ";
            else fout<<"bin"<<is+1<<endl;
         }

         fout<<"signal_events = ";
         for(int is=0; is<nsels; is++){
            double nSigNLO = xsecVec[i0]*dataLumi*scanEffvec[is][i12][i0];
            fout<<nSigNLO<<" ";
         }
         fout<<endl<<endl;

// Forced to be ZERO!
         fout<<"signal_contamination = ";
         for(int is=0; is<nsels; is++){
            double nSC = 0.0;
            fout<<nSC<<" ";
         }
         fout<<endl<<endl;

         fout<<"nuisance = signal stat; signal systematic"<<endl;
         fout<<"signal_uncertaintyDistribution_1 = lnN"<<endl;
         fout<<"signal_uncertaintyDistribution_2 = lnN"<<endl<<endl;

         double relErrxsec = 0.0;
         
         fout<<"signal_uncertainty_1 = ";
         for(int is=0; is<nsels; is++){
            double nSigNLO = xsecVec[i0]*dataLumi*scanEffvec[is][i12][i0];
            double errSigNLO = nSigNLO*sqrt( scanEffStatErrvec[is][i12][i0]*scanEffStatErrvec[is][i12][i0] );
            fout<<errSigNLO<<" ";
         }
         fout<<endl;

         fout<<"signal_uncertainty_2 = ";
         for(int is=0; is<nsels; is++){
            double nSigNLO = xsecVec[i0]*dataLumi*scanEffvec[is][i12][i0];
            double errSigNLO = nSigNLO*sqrt(
                                              relErrxsec*relErrxsec
                                            + filterRelUnc*filterRelUnc
                                            + scanEffSystErrvec[is][i12][i0]*scanEffSystErrvec[is][i12][i0]
                                            + dataLumiRelUnc*dataLumiRelUnc
                                           );
            fout<<errSigNLO<<" ";
         }
         fout<<endl;
         
         fout.close();
      }
   }

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
   tdrStyle->SetTitleSize(0.055,"XYZ");
   tdrStyle->SetTitleXOffset(1.02); tdrStyle->SetTitleYOffset(1.55); tdrStyle->SetTitleOffset(1.28, "Z");
   tdrStyle->SetPadLeftMargin(0.170); tdrStyle->SetPadRightMargin(0.150);
   tdrStyle->SetPadTopMargin(0.05); tdrStyle->SetPadBottomMargin(0.125);

   TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

   TString topoStr = topo;

   c1->Print("effWeightsErrs_"+topoStr+".pdf[");
   c1->cd(); gPad->SetLogz();
   c1->cd(); h2_effWeightsStatErr->Draw("colz"); c1->Print("effWeightsErrs_"+topoStr+".pdf");
   c1->cd(); h2_effWeightsAccPDFErr->Draw("colz"); c1->Print("effWeightsErrs_"+topoStr+".pdf");
   c1->cd(); h2_effWeightsJerErr->Draw("colz"); c1->Print("effWeightsErrs_"+topoStr+".pdf");
   c1->cd(); h2_effWeightsJesErr->Draw("colz"); c1->Print("effWeightsErrs_"+topoStr+".pdf");
   c1->cd(); h2_effWeightsIsrFsrErr->Draw("colz"); c1->Print("effWeightsErrs_"+topoStr+".pdf");
   c1->Print("effWeightsErrs_"+topoStr+".pdf]");

}
