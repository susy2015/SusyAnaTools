#include "tophead.h"
#include "tdrstyle.C"
#include "Type3TopTagger.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

using namespace std;

topTagger::type3TopTagger * type3Ptr;

const bool doWjetfatJets = true;
const bool doTopjetfatJets = true;
//const double lowgroomedWjetMass_ = 60, highgroomedWjetMass_ = 100;
const double lowWjetMass_ = 70, highWjetMass_ = 110;
//const double lowgroomedTopjetMass_ = 120, highgroomedTopjetMass_ = 210;
const double lowTopjetMass_ = 130, highTopjetMass_ = 220;

const double cutCSVS = 0.679;
const bool applyLowMET = false;
const bool withMETcut = true;
const double defaultMETcut = 200;
const double highMETcut = 350;

const double mTbcut_ = 500, mTtcut_ = 365, MT2cut_ = 300, mTWcut_ = 600, mTcombcut_ = 500; // baseline Kin

const bool doTopMassCut = true;

static const int nMuonsSel = 0, nElectronsSel = 0;

static const int nJetsSel = 5, nJetsSelPt30Eta24 = 5, nJetsSelPt50Eta24 = 2, nJetsSelPt70Eta24 = 2;
static const int low_nJetsSelBtagged = 2, high_nJetsSelBtagged = -1;
static const double minJetPt = 30;
//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta24Arr[] = {   -1,       2.4,      30,    -1  };
static const double pt50Eta24Arr[] = {   -1,       2.4,      50,    -1  };
static const double pt70Eta24Arr[] = {   -1,       2.4,      70,    -1  };
static const double      dphiArr[] = {   -1,       4.7,      30,    -1  };
static const double      bTagArr[] = {   -1,       2.4,      30,    -1  };

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
int countJets(const vector<TLorentzVector> &inputJets, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS, const double *jetCutsArr);
int countCSVS(const vector<TLorentzVector> &inputJets, const vector<double> &inputCSVS, const double CSVS = 0.679, const double minAbsEta = -1.0, const double maxAbsEta = -1.0, const double minPt = 30.0, const double maxPt = -1.0);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const double *jetCutsArr);
vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi = 3, const double minAbsEta = -1, const double maxAbsEta = 4.7, const double minPt = 30, const double maxPt = -1);

bool passOriKinCuts(const double met, const double MT2, const double mTt, const double mTb, const double metCut);
bool passBaselineFunc(const double met, const double MT2, const double mTt, const double mTb);
bool passHighMETFunc(const double met, const double MT2, const double mTt, const double mTb);

const double maxDRtopTaggerMatch = 0.4;

const double simuCAdeltaR = 1.5;

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

int checkTopCriteria(const vector<double> &tmpFinalSubStrucMass, const double &fatJetMass, vector<int> &passStatusVec);

bool find_mother(int momPdgId, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec);
bool find_mother(int momIdx, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec);
void find_mother(std::vector<int> & momIdxVec, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec);
int find_idx(int genIdx, const vector<int> &genDecayIdxVec);
void find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec);

TStopwatch timer;

void drawOverFlowBin(TH1 *histToAdjust){
   int nbins = histToAdjust->GetXaxis()->GetNbins();
   double overflow = histToAdjust->GetBinContent(nbins+1);
   double lastCont = histToAdjust->GetBinContent(nbins);
   histToAdjust->SetBinContent(nbins, overflow+lastCont);
}

void templatePlotsFunc(std::vector<TTree *> treeVec, const std::vector<std::string> &subSampleKeysVec, const std::string sampleKeyString="ttbar", int verbose=0){

  for(unsigned int ist=0; ist<subSampleKeysVec.size(); ist++){
 
     TTree * template_AUX = treeVec[ist];

     int template_Entries = template_AUX->GetEntries();
   
     for(int ie=0; ie<template_Entries; ie++){

        template_AUX->GetEntry(ie);

        if( ie==0 || ie == template_Entries-1 || ie%(template_Entries/10) == 0 ) std::cout<<"\n   Processing the "<<ie<<"th event ..."<<std::endl;

        double puWeight = 1.0;
        puWeight = weightTruNPV(template_tru_npv);

// template_oriJetsVec has one to one correpondance to template_recoJetsBtagCSVS (stores btag CSV values)
// One has to place a jet pt cut (>30 GeV) in the template_oriJetsVec before feeding it into the top tagger since in the tagger the jet pt will not be checked
        if( template_oriJetsVec->size() != template_recoJetsBtagCSVS->size() ){
           std::cout<<"template_oriJetsVec->size : "<<template_oriJetsVec->size()<<"  template_recoJetsBtagCSVS : "<<template_recoJetsBtagCSVS->size()<<std::endl;
        }

        TLorentzVector metLVec; metLVec.SetPtEtaPhiM(template_met, 0, template_metphi, 0);

        int cntCSVS = countCSVS((*template_oriJetsVec), (*template_recoJetsBtagCSVS), cutCSVS, bTagArr);
        int cntNJetsPt30 = countJets((*template_oriJetsVec), pt30Arr);
        int cntNJetsPt30Eta24 = countJets((*template_oriJetsVec), pt30Eta24Arr);
        int cntNJetsPt50Eta24 = countJets((*template_oriJetsVec), pt50Eta24Arr);
        int cntNJetsPt70Eta24 = countJets((*template_oriJetsVec), pt70Eta24Arr);
        vector<double> dPhiVec = calcDPhi((*template_oriJetsVec), template_metphi, 3, dphiArr);
 
/* In case of top tagger, do the following:
        type3Ptr->setnJetsSel(nJetsSel); type3Ptr->setsimuCAdeltaR(simuCAdeltaR);
        bool pass = type3Ptr->processEvent((*template_oriJetsVec), (*template_recoJetsBtagCSVS), metLVec);

// All the essential top tagger related cuts as in 8 TeV

        if( type3Ptr->bestTopJetIdx == -1 ) continue;
        if( ! type3Ptr->remainPassCSVS ) continue;

        if( type3Ptr->pickedRemainingCombfatJetIdx == -1 && template_oriJetsVec->size()>=6 ) continue;

        if( doTopMassCut && ! (bestTopJetMass > lowTopCut_ && bestTopJetMass < highTopCut_ ) ) continue;

        double mTbestTopJet = type3Ptr->mTbestTopJet;

        double mTbJet = type3Ptr->mTbJet;

        double MT2 = type3Ptr->MT2;

        double mTcomb = mTbJet + 0.5*mTbestTopJet;

        bool passBaseline = passBaselineFunc(template_met, MT2, mTbestTopJet, mTbJet);
        bool passHighMET = passHighMETFunc(template_met, MT2, mTbestTopJet, mTbJet);

        if( ! passBaseline ) continue;
        if( ! passHighMET ) continue;
*/
// An example for top veto case
        int tmpbestTopJetIdx = -1; int tmpcombSize = 0;
        TLorentzVector tmpbestTopJetLVec, tmpbestWJetLVec;

        if( cntNJetsPt30 >=3 ){
           type3Ptr->setsimuCAdeltaR(simuCAdeltaR);
// The runOnlyTopTaggerPart function disables the entangled simutaneous "one triplet + >=1 b in the remaining jets" requirement when using the type3Ptr->processEvent() function.
// You'll get purely all combination of triplets.
           type3Ptr->runOnlyTopTaggerPart((*template_oriJetsVec), (*template_recoJetsBtagCSVS));

           tmpbestTopJetIdx = type3Ptr->bestTopJetIdx;
           tmpbestTopJetLVec = type3Ptr->bestTopJetLVec;
           tmpbestWJetLVec = type3Ptr->bestCloseTomWLVec;
           tmpcombSize = type3Ptr->combSize;
        }

// You can check the combination of triplets outside of the Type3TopTagger class as shown below
        int nTopCandCnt =0;
        for(int ic=0; ic<tmpcombSize; ic++){

           TLorentzVector fatJet, subJet12, subJet13, subJet23;

           fatJet = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombfatJets[ic]);
// If containing merged W or top quark, the finalCombJetSubStruc[ic] size can be different than usual case where it has (1, 2), (1, 3) and (2, 3) substructure combinations
           if( type3Ptr->finalCombJetSubStruc[ic].size() >=1 ) subJet12 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][0]);
           if( type3Ptr->finalCombJetSubStruc[ic].size() >=2 ) subJet13 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][1]); else subJet13 = subJet12;
           if( type3Ptr->finalCombJetSubStruc[ic].size() >=3 ) subJet23 = type3Ptr->buildLVec( (*template_oriJetsVec), type3Ptr->finalCombJetSubStruc[ic][2]); else subJet23 = subJet12;

           const double fatJetm123 = fatJet.M();
           const double fatJetm12  = subJet12.M();
           const double fatJetm13  = subJet13.M();
           const double fatJetm23  = subJet23.M();

           const double fatJetPt = fatJet.Pt();

           vector<int> passStatusVec;
// There is a checkTopCriteria in the Type3TopTagger class as well. Here is a copy of it removing the m23/m123 requirement for b) and c) criteria.
           int isTopJet = checkTopCriteria(type3Ptr->fatJetSubMassVec[ic], fatJetm123, passStatusVec);

           int statusCat00 =-1, statusCat01 =-1, statusCat02 =-1;
           if( type3Ptr->finalCombfatJetsPassStatusVec[ic].size() >=1 ) statusCat00 = type3Ptr->finalCombfatJetsPassStatusVec[ic][0];
           if( type3Ptr->finalCombfatJetsPassStatusVec[ic].size() >=2 ) statusCat01 = type3Ptr->finalCombfatJetsPassStatusVec[ic][1];
           if( type3Ptr->finalCombfatJetsPassStatusVec[ic].size() >=3 ) statusCat02 = type3Ptr->finalCombfatJetsPassStatusVec[ic][2];

           if( isTopJet ){
// Counting number of top quark candidates with mass requirement
              if( doTopMassCut && (fatJetm123 > lowTopCut_ && fatJetm123 < highTopCut_ ) ){
                 nTopCandCnt ++;
              }
           }
        }

        double dPhi0 = dPhiVec[0], dPhi1 = dPhiVec[1], dPhi2 = dPhiVec[2];
        
        if( cntNJetsPt50Eta24 < nJetsSelPt50Eta24 ) continue; 
        if( dPhi0 < 0.5 || dPhi1 < 0.5 || dPhi2 < 0.3 ) continue;
        if( !( (low_nJetsSelBtagged == -1 || cntCSVS >= low_nJetsSelBtagged) && (high_nJetsSelBtagged == -1 || cntCSVS < high_nJetsSelBtagged ) ) ) continue;

// Requireing NO top quark candidates in the event
        if( cntNJetsPt30Eta24 >=3 && nTopCandCnt ) continue;
     }
}

void example(){

  std::cout<<"\n"<<std::endl; timer.Print(); timer.Start();

  type3Ptr = new topTagger::type3TopTagger();

// Enable merged W and top quark cases
  type3Ptr->setdoWjetfatJets(doWjetfatJets);
  type3Ptr->setdoTopjetfatJets(doTopjetfatJets);

// Initialize calcMT2 first to make the printout look better
  TLorentzVector dummyLVec; dummyLVec.SetPtEtaPhiE(0, 0, 0, 0);
  calcMT2(dummyLVec, dummyLVec, dummyLVec);

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

  templatePlotsFunc( ... );

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
   if( mTcomb < mTcombcut_ ) pass = false;
   if( MT2 < MT2cut_ ) pass = false;
   if( met < metCut ) pass = false;

   return pass;
}

bool passBaselineFunc(const double met, const double MT2, const double mTt, const double mTb){
   bool pass = passOriKinCuts(met, MT2, mTt, mTb, defaultMETcut);
   return pass;
}

bool passHighMETFunc(const double met, const double MT2, const double mTt, const double mTb){
   bool pass = passOriKinCuts(met, MT2, mTt, mTb, highMETcut);
   return pass;
}

int checkTopCriteria(const vector<double> &tmpFinalSubStrucMass, const double &fatJetMass, vector<int> &passStatusVec){

   passStatusVec.clear();
   int passCriteria =0;

   if( tmpFinalSubStrucMass.size() ==3 ){
      const double m12 = tmpFinalSubStrucMass[0], m13 = tmpFinalSubStrucMass[1], m23 = tmpFinalSubStrucMass[2];
      const double m123 = fatJetMass;

      if(    TMath::ATan(m13/m12) > lowArcTanm13overm12_ && TMath::ATan(m13/m12) < highArcTanm13overm12_
          && m23/m123 > Rmin_ && m23/m123 < Rmax_ ){
         passCriteria = 1;
         passStatusVec.push_back(1);
      }else{
         passStatusVec.push_back(0);
      }

      if(     Rmin_*Rmin_ *(1+ (m13/m12)*(m13/m12)) < 1- (m23/m123)*(m23/m123)
          &&  1-(m23/m123)*(m23/m123) < Rmax_*Rmax_*(1+(m13/m12)*(m13/m12) )
          ){
//          &&  m23/m123 > lowm23overm123_ ){
         passCriteria = 1;
         passStatusVec.push_back(1);
      }else{
         passStatusVec.push_back(0);
      }

      if(     Rmin_*Rmin_ *(1+ (m12/m13)*(m12/m13)) < 1- (m23/m123)*(m23/m123)
          &&  1-(m23/m123)*(m23/m123) < Rmax_*Rmax_*(1+(m12/m13)*(m12/m13) )
          ){
//          &&  m23/m123 > lowm23overm123_ ){
         passCriteria = 1;
         passStatusVec.push_back(1);
      }else{
         passStatusVec.push_back(0);
      }
   }else if( tmpFinalSubStrucMass.size() ==1 ){
      const double m23 = tmpFinalSubStrucMass[0];
      const double m123 = fatJetMass;
      if( m23/m123 > Rmin_ && m23/m123 < Rmax_ ){
         passCriteria = 1;
         passStatusVec.push_back(1);
      }else{
         passStatusVec.push_back(0);
      }
   }else if( tmpFinalSubStrucMass.size() ==0 ){
      passCriteria = 1;
      passStatusVec.push_back(1);
   }

   return passCriteria;
}

bool find_mother(int momPdgId, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec){
   if( dauIdx == -1 ) return false;

   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx != -1 ){
         if( abs(genDecayPdgIdVec[thisIdx]) == momPdgId ) return true;
      }
   }
   return false;
}

bool find_mother(int momIdx, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec){
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

void find_mother(std::vector<int> & momIdxVec, int dauIdx, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec){
   momIdxVec.clear();
   if( dauIdx == -1 ) return;
   int thisIdx = dauIdx;
   while( thisIdx >=0 ){
      int momGenIdx = genDecayMomIdxVec[thisIdx];
      thisIdx = find_idx(momGenIdx, genDecayIdxVec);
      if( thisIdx != -1 ) momIdxVec.push_back(thisIdx);
   }
   return;
}

int find_idx(int genIdx, const vector<int> &genDecayIdxVec){
   for(int ig=0; ig<(int)genDecayIdxVec.size(); ig++){
      if( genDecayIdxVec[ig] == genIdx ) return ig;
   }
   return -1;
}

void find_W_emu_tauprongs(std::vector<int> &W_emuVec, std::vector<int> &W_tau_emuVec, std::vector<int> &W_tau_prongsVec, const vector<int> &genDecayIdxVec, const vector<int> &genDecayMomIdxVec, const vector<int> &genDecayPdgIdVec){
   W_emuVec.clear(); W_tau_emuVec.clear(); W_tau_prongsVec.clear();
   if( !genDecayPdgIdVec.empty() ){
      for(unsigned int ig=0; ig<genDecayPdgIdVec.size(); ig++){
         int pdgId = genDecayPdgIdVec.at(ig);
         if( abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 211 || abs(pdgId) == 321 ){
            vector<int> momIdxVec;
            find_mother(momIdxVec, ig, genDecayIdxVec, genDecayMomIdxVec);
            int cntMomW = 0, cntMomTau = 0;
            for(unsigned int im=0; im<momIdxVec.size(); im++){
// Make this one general for both W and Z decays (need to be careful when both Z and W appear in the decay chain, 
// but this is unlikely given currently how the MC are generated and how the genDecayXXX vectors are produced)
//               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 24 ) cntMomW ++;
               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 24 || abs(genDecayPdgIdVec[momIdxVec[im]]) == 23 ) cntMomW ++;
               if( abs(genDecayPdgIdVec[momIdxVec[im]]) == 15 ) cntMomTau ++;
            }
            if( cntMomW ){
               if( cntMomTau ){
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_tau_emuVec.push_back(ig);
                  else W_tau_prongsVec.push_back(ig);
               }else{
                  if( abs(pdgId) == 11 || abs(pdgId) == 13 ) W_emuVec.push_back(ig);
               }
            }
         }
      }
   }
   return;
}
