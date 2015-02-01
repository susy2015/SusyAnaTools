#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "combination.h"
#include "TColor.h"
#include "indexSort.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<vector<TFile*> >;
#pragma link C++ class vector<vector<TH1D*> >;
#pragma link C++ class vector< map<int, int> >;
#endif

using namespace stdindexSort;
using namespace stdcomb;
using namespace std;

// doExtraCuts: extra cuts other than the top tagger related, e.g., dphi cuts, HT cut and so on.
static const bool doExtraCuts = true;

static const double mTbcut = 175, mTtcut = 365, MT2cut = 300;
static const double mTjcut = 175;
// doMTMTjcuts: numbers are counted. But if enabled, some plots will be filled after all the cuts.
static const bool doMTMTjcuts = true;

// PDG values (in GeV)
static const double mW = 80.385, mTop = 173.5;

static const double mWoverTop = mW/mTop;

//static const double lowRatioWoverTop = 0.85, highRatioWoverTop = 1.15;
static const double lowRatioWoverTop = 0.85, highRatioWoverTop = 1.25;
static const double lowArcTanm13overm12 = 0.2, highArcTanm13overm12 = 1.3, lowm23overm123 = 0.35;
//static const double lowArcTanm13overm12 = 0.2, highArcTanm13overm12 = 1.3, lowm23overm123 = 0.30;

static const double Rmin = lowRatioWoverTop*mWoverTop, Rmax = highRatioWoverTop*mWoverTop;

static const double defaultJetCone = 2.0;
static const double simuCAdeltaR = -1;
// Eta ranges from 0 to 5, phi ranges from 0 to 3.14.
// sqrt(5./2.*5./2. + 3.14/2. * 3.14/2.) ~ 2.95
//static const double simuCALargerdeltaR = 3.0;
static const double simuCALargerdeltaR = -1; // -1 means no deltaR requirement

static const double lowTopCut = 80, highTopCut = 270;
static const double lowWCut = 50, highWCut = 120;

static const double CSVL = 0.244, CSVM = 0.679, CSVT = 0.898;
// Choose CSVM point for now 
// --> A good top fat jet might indicate a b-jet already,
// so requiring another tight b-jet is NOT good. (TODO: to be studied)
static const double CSVS = CSVM;

static const int nSubJetsDiv =3;

static const int nJetsSel = 6;

static const double maxEtaForbJets = 2.4;

// mass   : mass ordering --> pick the one with mass closest to the norminal mass
// pt     : pt ordering --> pick the one with larger pt (for the first two fat jets)
// hybrid : pt ordering + mass ordering --> if both of the first two fat jets 
//          satisfying criteria, pick the one closer to the norminal mass  
//                                            best   remaining
static const string      orderingOptArr[] = {"dr", "dr"}; // (mass, mass) is the best?
static const int maxIndexForOrderingArr[] = {  1,   1  };

typedef vector<int>::iterator vii;

void fillVector(vii begin, vii end, vector<int> &outVec){
  outVec.clear();
  for (vii it=begin;it!=end;++it) outVec.push_back(*it);
}

void display(vii begin,vii end, const string trailStr ="\n"){
  for (vii it=begin;it!=end;++it) cout<<*it;
  cout<<trailStr;
}

void buildOneFatJetsIndexComb(vector<int> indexVec, vector<int> &iniIndexComb, vector<vector<int> > &finalCombJets, vector<vector<int> > &finalRemaining){

  finalCombJets.clear(); finalRemaining.clear();

  vector<vector<int> > combHolder;

  do{
     combHolder.push_back(iniIndexComb);
  }while(next_combination(indexVec.begin(),indexVec.end(),iniIndexComb.begin(),iniIndexComb.end()) );

  unsigned int combSize = combHolder.size();
  for(unsigned int ic=0; ic<combSize; ic++){
     finalCombJets.push_back(combHolder[ic]);
     finalRemaining.push_back(indexVec);
     for(vii it=finalCombJets.back().begin(); it!=finalCombJets.back().end(); it++){
        vii jt = find(finalRemaining.back().begin(), finalRemaining.back().end(), (*it));
        if( jt!= finalRemaining.back().end() ) finalRemaining.back().erase(jt); 
     }
  }
}

void buildSubStrucIndexComb(vector<int> indexVec, vector<vector<int> > &finalCombSubStruc){

   finalCombSubStruc.clear();

   if( indexVec.size() <=2 ){ finalCombSubStruc.push_back(indexVec); return; }

   vector<int> iniIndexVec; iniIndexVec.push_back(indexVec[0]); iniIndexVec.push_back(indexVec[1]);
   
   do{
      finalCombSubStruc.push_back(iniIndexVec);
   }while(next_combination(indexVec.begin(),indexVec.end(),iniIndexVec.begin(),iniIndexVec.end()) );

}

TLorentzVector buildLVec(const vector<TLorentzVector> &oriJetsVec, const vector<int> &indexCombVec){

   TLorentzVector combLVec;
   for(unsigned int ic=0; ic<indexCombVec.size(); ic++){
      int idx = indexCombVec[ic];
      combLVec += oriJetsVec[idx];
   }
   return combLVec;
}

void calcSubStrucMass(const vector<TLorentzVector> &oriJetsVec, const vector<vector<int> > &finalCombSubStruc, vector<double> &finalSubStrucMass){

   finalSubStrucMass.clear();
   for(unsigned int is=0; is<finalCombSubStruc.size(); is++){
      double perMass = buildLVec(oriJetsVec, finalCombSubStruc[is]).M();
//      double perMass = perCombLVec.M();
      finalSubStrucMass.push_back(perMass);
   }
}

bool filterfatJetWithDRcone(const vector<TLorentzVector> &oriJetsVec, const vector<int> &indexCombVec, const double drCut){

   if( drCut == -1 ) return true;

   bool withinCone = true;
   TLorentzVector centralLVec = buildLVec(oriJetsVec, indexCombVec);
   for(unsigned int ic=0; ic<indexCombVec.size(); ic++){
      int idx = indexCombVec[ic];
      double deltaR = centralLVec.DeltaR(oriJetsVec[idx]);
      if( deltaR > drCut ) withinCone = false;
   }
   return withinCone;

}

int checkTopCriteria(const vector<double> &finalSubStrucMass, const double &fatJetMass, vector<int> &passStatusVec){

   passStatusVec.clear();
   int passCriteria =0;
   const double m12 = finalSubStrucMass[0], m13 = finalSubStrucMass[1], m23 = finalSubStrucMass[2];
   const double m123 = fatJetMass;
   
   if(    TMath::ATan(m13/m12) > lowArcTanm13overm12 && TMath::ATan(m13/m12) < highArcTanm13overm12
       && m23/m123 > Rmin && m23/m123 < Rmax ){
      passCriteria = 1;
      passStatusVec.push_back(1);
   }else{
      passStatusVec.push_back(0);
   }

   if(     Rmin*Rmin *(1+ (m13/m12)*(m13/m12)) < 1- (m23/m123)*(m23/m123) 
       &&  1-(m23/m123)*(m23/m123) < Rmax*Rmax*(1+(m13/m12)*(m13/m12) ) 
       &&  m23/m123 > lowm23overm123 ){
      passCriteria = 1;
      passStatusVec.push_back(1);
   }else{
      passStatusVec.push_back(0);
   }

   if(     Rmin*Rmin *(1+ (m12/m13)*(m12/m13)) < 1- (m23/m123)*(m23/m123) 
       &&  1-(m23/m123)*(m23/m123) < Rmax*Rmax*(1+(m12/m13)*(m12/m13) ) 
       &&  m23/m123 > lowm23overm123 ){
      passCriteria = 1;
      passStatusVec.push_back(1);
   }else{
      passStatusVec.push_back(0);
   }

   return passCriteria;
}

void prepareCombination(const int nJetsInc, vector<vector<int> > &finalCombfatJets, vector<vector<int> > &finalRemaining, vector<vector<vector<int> > > &finalCombJetSubStruc){

  vector<int> indexVec; for(int ij=0; ij<nJetsInc; ij++) indexVec.push_back(ij);
  vector<int> iniIndexComb; for(int ij=0; ij<nSubJetsDiv; ij++) iniIndexComb.push_back(ij);

  finalCombfatJets.clear(); finalRemaining.clear();
  buildOneFatJetsIndexComb(indexVec, iniIndexComb, finalCombfatJets, finalRemaining);

  finalCombJetSubStruc.clear();

  unsigned int combSize = finalCombfatJets.size();

  for(unsigned int ic=0; ic<combSize; ic++){
      vector<vector<int> > perJetSub;

      buildSubStrucIndexComb(finalCombfatJets[ic], perJetSub); finalCombJetSubStruc.push_back(perJetSub);

  }
}

void ptOrderedfatJets(const vector<TLorentzVector> &oriJetsVec, vector<vector<int> > &finalCombfatJets, vector<vector<int> > &finalRemaining, vector<vector<vector<int> > > &finalCombJetSubStruc){
   const vector<vector<int> > cachedfinalCombfatJets = finalCombfatJets;
   const vector<vector<int> > cachedfinalRemaining = finalRemaining;
   const vector<vector<vector<int> > > cachedfinalCombJetSubStruc = finalCombJetSubStruc;

   finalCombfatJets.clear(); finalRemaining.clear(); finalCombJetSubStruc.clear();

   unsigned int combSize = cachedfinalCombfatJets.size();

// Get the index after pt ordered
   vector<double> tempfatJetPtVec;
   for(unsigned int ic=0; ic<combSize; ic++){ tempfatJetPtVec.push_back(buildLVec(oriJetsVec, cachedfinalCombfatJets[ic]).Pt()); }
   std::vector<size_t> ptindexes;
   argsort(tempfatJetPtVec.begin(), tempfatJetPtVec.end(), std::greater<double>(), ptindexes);

// Transfer the fat jets to pt ordered
   for(unsigned int ii=0; ii<ptindexes.size(); ii++){
      unsigned int indexMapped = ptindexes[ii];
      finalCombfatJets.push_back( cachedfinalCombfatJets[indexMapped] );
      finalRemaining.push_back( cachedfinalRemaining[indexMapped] );
      finalCombJetSubStruc.push_back( cachedfinalCombJetSubStruc[indexMapped] );
   }
}

void onlyfatJetsCloseInAClone(const unsigned int nJetsInc, const vector<TLorentzVector> &oriJetsVec, vector<vector<int> > &finalCombfatJets, vector<vector<int> > &finalRemaining, vector<vector<vector<int> > > &finalCombJetSubStruc){

   finalCombfatJets.clear(); finalRemaining.clear(); finalCombJetSubStruc.clear();

   vector<vector<int> > tempCombfatJets; vector<vector<int> > tempRemaining; vector<vector<vector<int> > > tempCombJetSubStruc;

   prepareCombination(nJetsInc, tempCombfatJets, tempRemaining, tempCombJetSubStruc);

   unsigned int combSize = tempCombfatJets.size();

   for(unsigned int ic=0; ic<combSize; ic++){
      if( filterfatJetWithDRcone(oriJetsVec, tempCombfatJets[ic], simuCAdeltaR) ){
         finalCombfatJets.push_back(tempCombfatJets[ic]);
         finalRemaining.push_back(tempRemaining[ic]);
         finalCombJetSubStruc.push_back(tempCombJetSubStruc[ic]);
      }
   }

   ptOrderedfatJets(oriJetsVec, finalCombfatJets, finalRemaining, finalCombJetSubStruc);

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

void topTaggerAtlasMethod(){

  TFile *studyFile = new TFile("directStop_merged.root");
  TTree *AUXtree = (TTree*) studyFile->Get("histAndTree_recoLevelEleMuTauVeto/AUX");
  double jet1pt_TR, jet1eta_TR, jet1phi_TR, jet1energy_TR;
  double jet2pt_TR, jet2eta_TR, jet2phi_TR, jet2energy_TR;
  double jet3pt_TR, jet3eta_TR, jet3phi_TR, jet3energy_TR;
  std::vector<double> *otherJetspt_TR=0, *otherJetseta_TR=0, *otherJetsphi_TR=0, *otherJetsenergy_TR=0;
  int nJets, nJets_CUT; 
  double met, mht, ht, dPhi0_CUT, dPhi1_CUT, dPhi2_CUT;
  double metphi;
  double metSgnf;
  double evtWeight;
  std::vector<std::string> *bTagKeyString =0;
  std::vector<double> *recoJetsBtag_CSV =0;
 
  AUXtree->SetBranchAddress("nJets", &nJets);
  AUXtree->SetBranchAddress("nJets_CUT", &nJets_CUT);
  AUXtree->SetBranchAddress("jet1pt", &jet1pt_TR);
  AUXtree->SetBranchAddress("jet1eta", &jet1eta_TR);
  AUXtree->SetBranchAddress("jet1phi", &jet1phi_TR);
  AUXtree->SetBranchAddress("jet1energy", &jet1energy_TR);
  AUXtree->SetBranchAddress("jet2pt", &jet2pt_TR);
  AUXtree->SetBranchAddress("jet2eta", &jet2eta_TR);
  AUXtree->SetBranchAddress("jet2phi", &jet2phi_TR);
  AUXtree->SetBranchAddress("jet2energy", &jet2energy_TR);
  AUXtree->SetBranchAddress("jet3pt", &jet3pt_TR);
  AUXtree->SetBranchAddress("jet3eta", &jet3eta_TR);
  AUXtree->SetBranchAddress("jet3phi", &jet3phi_TR);
  AUXtree->SetBranchAddress("jet3energy", &jet3energy_TR);
  AUXtree->SetBranchAddress("otherJetspt", &otherJetspt_TR);
  AUXtree->SetBranchAddress("otherJetseta", &otherJetseta_TR);
  AUXtree->SetBranchAddress("otherJetsphi", &otherJetsphi_TR);
  AUXtree->SetBranchAddress("otherJetsenergy", &otherJetsenergy_TR);

  AUXtree->SetBranchAddress("met", &met);
  AUXtree->SetBranchAddress("metphi", &metphi);
  AUXtree->SetBranchAddress("metSgnf", &metSgnf);
  AUXtree->SetBranchAddress("mht", &mht);
  AUXtree->SetBranchAddress("ht", &ht);
  AUXtree->SetBranchAddress("dPhi0_CUT", &dPhi0_CUT);
  AUXtree->SetBranchAddress("dPhi1_CUT", &dPhi1_CUT);
  AUXtree->SetBranchAddress("dPhi2_CUT", &dPhi2_CUT);

  AUXtree->SetBranchAddress("bTagKeyString", &bTagKeyString);
  AUXtree->SetBranchAddress("recoJetsBtag_6", &recoJetsBtag_CSV);

  AUXtree->SetBranchAddress("evtWeight", &evtWeight);

  int nEntries = AUXtree->GetEntries();
  std::cout<<"\nnEntries : "<<nEntries<<std::endl;

  TF1 *fn_RminCat01 = new TF1("fn_RminCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RminCat01->SetParameter(0, Rmin);
  TF1 *fn_RcenCat01 = new TF1("fn_RcenCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RcenCat01->SetParameter(0, mWoverTop);
  TF1 *fn_RmaxCat01 = new TF1("fn_RmaxCat01", "sqrt(1-[0]*[0]/cos(x)/cos(x))", 0, 1.5); fn_RmaxCat01->SetParameter(0, Rmax);

  TF1 *fn_RminCat02 = new TF1("fn_RminCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RminCat02->SetParameter(0, Rmin);
  TF1 *fn_RcenCat02 = new TF1("fn_RcenCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RcenCat02->SetParameter(0, mWoverTop);
  TF1 *fn_RmaxCat02 = new TF1("fn_RmaxCat02", "sqrt(1-[0]*[0]/sin(x)/sin(x))", 0, 1.5); fn_RmaxCat02->SetParameter(0, Rmax);

  TF1 *fn_RminCat00 = new TF1("fn_RminCat00", "[0]", 0, 1.5); fn_RminCat00->SetParameter(0, Rmin);
  TF1 *fn_RcenCat00 = new TF1("fn_RcenCat00", "[0]", 0, 1.5); fn_RcenCat00->SetParameter(0, mWoverTop);
  TF1 *fn_RmaxCat00 = new TF1("fn_RmaxCat00", "[0]", 0, 1.5); fn_RmaxCat00->SetParameter(0, Rmax);

  TH2D *h2_m23overm123vsarctanm13overm12 = new TH2D("h2_m23overm123vsarctanm13overm12", "InACone: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);

//  TH1D *h1_mfatJet = new TH1D("h1_mfatJet", "fatJet mass; fatJet", 100, 0, 1000);
  TH1D *h1_mfatJetInACone = new TH1D("h1_mfatJetInACone", "fatJet mass in a cone; fatJet", 100, 0, 1000);

//  TH1D *h1_m23 = new TH1D("h1_m23", "m23; m23", 100, 0, 300);
  TH1D *h1_m23InACone = new TH1D("h1_m23InACone", "m23InACone; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat00 = new TH1D("h1_m23InAConeAndWTopCat00", "m23InAConeAndWTopCat00; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat01 = new TH1D("h1_m23InAConeAndWTopCat01", "m23InAConeAndWTopCat01; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat02 = new TH1D("h1_m23InAConeAndWTopCat02", "m23InAConeAndWTopCat02; m23", 100, 0, 300);

//  TH1D *h1_m12 = new TH1D("h1_m12", "m12; m12", 100, 0, 300);
  TH1D *h1_m12InACone = new TH1D("h1_m12InACone", "m12InACone; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat00 = new TH1D("h1_m12InAConeAndWTopCat00", "m12InAConeAndWTopCat00; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat01 = new TH1D("h1_m12InAConeAndWTopCat01", "m12InAConeAndWTopCat01; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat02 = new TH1D("h1_m12InAConeAndWTopCat02", "m12InAConeAndWTopCat02; m12", 100, 0, 300);

//  TH1D *h1_m13 = new TH1D("h1_m13", "m13; m13", 100, 0, 300);
  TH1D *h1_m13InACone = new TH1D("h1_m13InACone", "m13InACone; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat00 = new TH1D("h1_m13InAConeAndWTopCat00", "m13InAConeAndWTopCat00; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat01 = new TH1D("h1_m13InAConeAndWTopCat01", "m13InAConeAndWTopCat01; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat02 = new TH1D("h1_m13InAConeAndWTopCat02", "m13InAConeAndWTopCat02; m13", 100, 0, 300);

  TH1D *h1_met = new TH1D("h1_met", "met; met", 100, 0, 1000);
  TH1D *h1_metInAConeAndWTopCats = new TH1D("h1_metInAConeAndWTopCats", "metInAConeAndWTopCats; met", 100, 0, 1000);
  TH1D *h1_topJetPtInAConeAndWTopCats = new TH1D("h1_topJetPtInAConeAndWTopCats", "topJetPtInAConeAndWTopCats; top pt", 100, 0, 1000);

  TH1D *h1_nTopCand = new TH1D("h1_nTopCand", "number of top candidate; nTop", 20, 0, 20);
  TH1D *h1_nTopCandWithBtag = new TH1D("h1_nTopCandWithBtag", "Btag: number of top candidate; nTop", 20, 0, 20);

  TH2D *h2_bestm23overm123vsarctanm13overm12 = new TH2D("h2_bestm23overm123vsarctanm13overm12", "best: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);
  TH1D *h1_bestfatJet = new TH1D("h1_bestfatJet", "best fatJet mass; fatJet", 100, 0, 300);
  TH1D *h1_bestm12 = new TH1D("h1_bestm12", "best m12; m12", 100, 0, 300);
  TH1D *h1_bestm23 = new TH1D("h1_bestm23", "best m23; m23", 100, 0, 300);
  TH1D *h1_bestm13 = new TH1D("h1_bestm13", "best m13; m13", 100, 0, 300);
  TH1D *h1_bestCloseTomW = new TH1D("h1_bestCloseTomW", "best mjj; mjj", 100, 0, 200);

  TH1D *h1_bestnBtags = new TH1D("h1_bestnBtags", "best n of btagged jets; nbtag", 10, 0, 10);

  TH1D *h1_remainingfatJet = new TH1D("h1_remainingfatJet", "remaining fatJet mass; fatJet", 100, 0, 1000);
  TH1D *h1_remainingm12 = new TH1D("h1_remainingm12", "remaining m12; m12", 100, 0, 300);
  TH1D *h1_remainingm23 = new TH1D("h1_remainingm23", "remaining m23; m23", 100, 0, 300);
  TH1D *h1_remainingm13 = new TH1D("h1_remainingm13", "remaining m13; m13", 100, 0, 300);
  TH1D *h1_remainingCloseTomW = new TH1D("h1_remainingCloseTomW", "remaining mjj; mjj", 100, 0, 300);

  TH2D *h2_remainingmxyNobvsremainingm123 = new TH2D("h2_remainingmxyNobvsremainingm123", "remaining mxy (no b) versus m123; m123; mxy", 100, 0, 1000, 100, 0, 300);

  TH1D *h1_mTbestTopJet = new TH1D("h1_mTbestTopJet", "mT of best top jet & met; mT", 100, 0, 1000);
  TH1D *h1_mTremainingTopJet = new TH1D("h1_mTremainingTopJet", "mT of remaining top jet & met; mT", 100, 0, 1000);
  TH1D *h1_closemT = new TH1D("h1_closemT", "mT of closer fat jet to met; mT", 100, 0, 1000);
  TH1D *h1_mTbJet = new TH1D("h1_mTbJet", "mT of b jet to met; mT", 100, 0, 1000);
  TH1D *h1_MT2 = new TH1D("h1_MT2", "MT2 of the system; MT2", 100, 0, 1000);

  TH1D *h1_mTbestWJet = new TH1D("h1_mTbestWJet", "mT of best W jet & met; mT", 100, 0, 1000);
  TH1D *h1_mTbestbJet = new TH1D("h1_mTbestbJet", "mT of best b jet & met; mT", 100, 0, 1000);

  TH2D *h2_mTt1vsmTt2 = new TH2D("h2_mTt1vsmTt2", "mT of best top jet versus mT of remaining top jet; mTt2; mTt1", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTt1vsmTb = new TH2D("h2_mTt1vsmTb", "mT of best top jet versus mT of b jet; mTb; mTt1", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTt1vsMT2 = new TH2D("h2_mTt1vsMT2", "mT of best top jet versus MT2; MT2; mTt1", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTt2vsmTb = new TH2D("h2_mTt2vsmTb", "mT of remaining top jet versus mT of b jet; mTb; mTt2", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTt2vsMT2 = new TH2D("h2_mTt2vsMT2", "mT of remaining top jet versus MT2; MT2; mTt2", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTbvsMT2 = new TH2D("h2_mTbvsMT2", "mT of b jet versus MT2; MT2; mTb", 100, 0, 1000, 100, 0, 1000);

  TH2D *h2_mTbestWvsmTbestb = new TH2D("h2_mTbestWvsmTbestb", "mT of best W versus mT of best b jet; mTb; mTW", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTbestWvsmTbestTop = new TH2D("h2_mTbestWvsmTbestTop", "mT of best W versus mT of best Top jet; mTt; mTW", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_mTbestbvsmTbestTop = new TH2D("h2_mTbestbvsmTbestTop", "mT of best b versus mT of best Top jet; mTt; mTb", 100, 0, 1000, 100, 0, 1000);

  TH1D *h1_bestfatJet1DRremainingbJet = new TH1D("h1_bestfatJet1DRremainingbJet", "DR between best top jet and the b jet in the remaining jets; #DeltaR", 100, 0, 10);
  TH1D *h1_bestfatJet1DRremainingfatJet = new TH1D("h1_bestfatJet1DRremainingfatJet", "DR between best top jet and the remaining fat jet; #DeltaR", 100, 0, 10);

  TH1D *h1_bestfatJet1DPHIremainingbJet = new TH1D("h1_bestfatJet1DPHIremainingbJet", "DPHI between best top jet and the b jet in the remaining jets; #Delta#phi", 100, -3.2, 3.2);
  TH1D *h1_bestfatJet1DPHIremainingfatJet = new TH1D("h1_bestfatJet1DPHIremainingfatJet", "DPHI between best top jet and the remaining fat jet; #Delta#phi", 100, -3.2, 3.2);

  TH1D *h1_bestWJetDRremainingbJet = new TH1D("h1_bestWJetDRremainingbJet", "DR between best W jet and the b jet in the remaining jets; #DeltaR", 100, 0, 10);
  TH1D *h1_bestWJetDPHIremainingbJet = new TH1D("h1_bestWJetDPHIremainingbJet", "DPHI between best W jet and the b jet in the remaining jets; #Delta#phi", 100, -3.2, 3.2);

  double cntPassnJetsCut = 0, cntTaggedTopEvents =0, cntTaggedTopEventsWithinMassCuts =0, cntTwoTaggedTopJetsWithinMassCuts =0;
  double cntTaggedAllCutsPlusCSVL =0, cntTaggedAllCutsPlusCSVS =0, cntTaggedAllCutsPlusCSVT =0;
  double cntPassingMTbestTopJetCut = 0, cntPassingMTclosefatJetCut =0, cntPassingMTclosebJetCut = 0;
  double cntPassingMTjCut =0, cntPassingMTjandMTbCut =0;
  double cntTaggedbestFatJetPlusCSVL =0, cntTaggedbestFatJetPlusCSVS =0, cntTaggedbestFatJetPlusCSVT =0;

  for(int ie=0; ie<nEntries; ie++){

     AUXtree->GetEntry(ie);

     if( ie==0 || ie== nEntries-1 || ie%10000 == 0 ){
        std::cout<<"Processing the "<<ie<<"th event"<<std::endl;
     }

     TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);

     TLorentzVector jet1LVec, jet2LVec, jet3LVec;
     jet1LVec.SetPtEtaPhiE(jet1pt_TR, jet1eta_TR, jet1phi_TR, jet1energy_TR);
     jet2LVec.SetPtEtaPhiE(jet2pt_TR, jet2eta_TR, jet2phi_TR, jet2energy_TR);
     jet3LVec.SetPtEtaPhiE(jet3pt_TR, jet3eta_TR, jet3phi_TR, jet3energy_TR);

     if( doExtraCuts ){
        double newht =0;
        if( fabs(jet1eta_TR) < 2.5 ) newht += jet1pt_TR;
        if( fabs(jet2eta_TR) < 2.5 ) newht += jet2pt_TR;
        if( fabs(jet3eta_TR) < 2.5 ) newht += jet3pt_TR;
        for(unsigned int io=0; io<otherJetspt_TR->size(); io++){ if( fabs( (*otherJetseta_TR)[io] ) < 2.5 ) newht += (*otherJetspt_TR)[io]; }
// This one is NOT used by ATLAS
//        if( met/sqrt(newht) < 5 ) continue;
//// The metSgnf has to be studied, but it has a big potential.
////        if( metSgnf < 100 ) continue;
        double dPhi0 = 999., dPhi1 = 999., dPhi2 = 999.;
        if( nJets>=1 ) dPhi0 = fabs(jet1LVec.DeltaPhi(metLVec)); 
        if( nJets>=2 ) dPhi1 = fabs(jet2LVec.DeltaPhi(metLVec)); 
        if( nJets>=3 ) dPhi2 = fabs(jet3LVec.DeltaPhi(metLVec)); 
// dPhi cuts have to be used to remove QCD
        if( dPhi0 < 0.4 || dPhi1 < 0.4 || dPhi2 < 0.4 ) continue;
// Current has to be used, since the HT400 WJetsToLNu sample is used.
// This cut does NOT matter too much for the final result
        if( newht < 400 ) continue;
// Mainly for triggers (See recommendations from SUSY-3G twiki)
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSYstop
        if( nJets>=1 && jet1pt_TR < 70 ) continue;
        if( nJets>=2 && jet2pt_TR < 70 ) continue;
// met cut might NOT be necessary, but let's see
//        if( met < 300 ) continue;
        if( met < 175 ) continue;
     }

     int cntnJetsPt30Eta25 = 0;

// The index has to be the same between jets & btags!
     vector<TLorentzVector> oriJetsVec;
     vector<double> recoJetsBtagCSVS;

     if( jet1pt_TR>30 && fabs(jet1eta_TR) < 2.5 ) cntnJetsPt30Eta25++;
     if( jet2pt_TR>30 && fabs(jet2eta_TR) < 2.5 ) cntnJetsPt30Eta25++;
     if( jet3pt_TR>30 && fabs(jet3eta_TR) < 2.5 ) cntnJetsPt30Eta25++;
     oriJetsVec.push_back(jet1LVec); recoJetsBtagCSVS.push_back( (*recoJetsBtag_CSV)[0] );
     oriJetsVec.push_back(jet2LVec); recoJetsBtagCSVS.push_back( (*recoJetsBtag_CSV)[1] ); 
     oriJetsVec.push_back(jet3LVec); recoJetsBtagCSVS.push_back( (*recoJetsBtag_CSV)[2] ); 
   
     for(int io=0; io<nJets-3; io++){
        TLorentzVector jetLVec;
        jetLVec.SetPtEtaPhiE((*otherJetspt_TR)[io], (*otherJetseta_TR)[io], (*otherJetsphi_TR)[io], (*otherJetsenergy_TR)[io]);
        oriJetsVec.push_back(jetLVec); recoJetsBtagCSVS.push_back( (*recoJetsBtag_CSV)[3+io] );
        if( (*otherJetspt_TR)[io] > 30 && fabs((*otherJetseta_TR)[io])<2.5 ){
           cntnJetsPt30Eta25++;
        }
     }
 
//     cntnJetsPt30Eta25 = nJets;
     if( cntnJetsPt30Eta25<nJetsSel ) continue;

     cntPassnJetsCut += evtWeight;

// Find all three-jet triplets ("fat" jets) with a cone requirement
// Already pt ordered!
     vector<vector<int> > finalCombfatJets; vector<vector<int> > finalRemaining; vector<vector<vector<int> > > finalCombJetSubStruc;
     onlyfatJetsCloseInAClone(oriJetsVec.size(), oriJetsVec, finalCombfatJets, finalRemaining, finalCombJetSubStruc);
     
     unsigned int combSize = finalCombfatJets.size();

     vector<double> finalCombMaxDR;
     for(unsigned int ic=0; ic<combSize; ic++){
        TLorentzVector centralLVec = buildLVec(oriJetsVec, finalCombfatJets[ic]);
        double maxDR = 0.0;
        for(unsigned int is=0; is<finalCombfatJets[ic].size(); is++){
           TLorentzVector perJetLVec = oriJetsVec[finalCombfatJets[ic][is]];
           double perDR = perJetLVec.DeltaR(centralLVec);
           if( maxDR < perDR ) maxDR = perDR;
        }
        finalCombMaxDR.push_back(maxDR);
     }

// Cache the mass and sub-structure masses
     vector<double> fatJetMassVec; vector<vector<double> > fatJetSubMassVec;
     for(unsigned int ic=0; ic<combSize; ic++){
        fatJetMassVec.push_back(buildLVec(oriJetsVec, finalCombfatJets[ic]).M());

        vector<double> perfatJetSubMassVec;
        calcSubStrucMass(oriJetsVec, finalCombJetSubStruc[ic], perfatJetSubMassVec);
        fatJetSubMassVec.push_back(perfatJetSubMassVec);
     }

     bool isTopEvent = false;
     int nTopJetCandCnt = 0, nTopJetCandWithBtagCnt =0;
// Find the best top candidate!
     int bestTopJetIdx = -1;
     for(unsigned int ic=0; ic<combSize; ic++){

        double fatJetm12 = fatJetSubMassVec[ic][0], fatJetm13 = fatJetSubMassVec[ic][1], fatJetm23 = fatJetSubMassVec[ic][2];
        double fatJetm123 = fatJetMassVec[ic];

// Find a top fat jet passing at least one of the three criteria
        isTopEvent = true;
        nTopJetCandCnt ++; 
        h1_topJetPtInAConeAndWTopCats->Fill(buildLVec(oriJetsVec, finalCombfatJets[ic]).Pt(), evtWeight);

// We can have multiple such combination in one event
        if( orderingOptArr[0] == "dr" ){ 
           if( bestTopJetIdx == -1 ){
              bestTopJetIdx = ic;
           }else{
              double pickedmaxDR = finalCombMaxDR[bestTopJetIdx];
              if( pickedmaxDR > finalCombMaxDR[ic] ){
                 bestTopJetIdx = ic;
              }
           }
        }else if( orderingOptArr[0] == "mass" ){
           if( bestTopJetIdx == -1 ){
              bestTopJetIdx = ic;
           }else{
              double pickedTopMass = fatJetMassVec[bestTopJetIdx];
// Find the configuration of an event (with a <top fat jet, tagged b-jet> combination) with 
// the best top mass of the top fat jet
              if( fabs(pickedTopMass - mTop) > fabs( fatJetm123 - mTop ) ){
                 bestTopJetIdx = ic;
              }
           }
        }else if( orderingOptArr[0] == "pt" ){
           if( ic<=maxIndexForOrderingArr[0] ){
              if( bestTopJetIdx == -1 ){
                 bestTopJetIdx = ic;
              }
           }
        }else if( orderingOptArr[0] == "hybrid" ){
           if( ic<=maxIndexForOrderingArr[0] ){
              if( bestTopJetIdx == -1 ){
                 bestTopJetIdx = ic;
              }else{
                 double pickedTopMass = fatJetMassVec[bestTopJetIdx];
                 if( fabs(pickedTopMass - mTop) > fabs( fatJetm123 - mTop ) ){
                    bestTopJetIdx = ic;
                 }
              }
           }
        }

        h1_mfatJetInACone->Fill(fatJetm123, evtWeight);
        h1_m23InACone->Fill(fatJetm23, evtWeight);
        h1_m12InACone->Fill(fatJetm12, evtWeight);
        h1_m13InACone->Fill(fatJetm13, evtWeight);

        h2_m23overm123vsarctanm13overm12->Fill( TMath::ATan(fatJetm13/fatJetm12), fatJetm23/fatJetm123, evtWeight);
     }

     h1_nTopCand->Fill(nTopJetCandCnt, evtWeight);
     h1_nTopCandWithBtag->Fill(nTopJetCandWithBtagCnt, evtWeight);

     h1_met->Fill(met, evtWeight);
     if( isTopEvent ) cntTaggedTopEvents += evtWeight;

// Must have a fat top jet
     if( bestTopJetIdx == -1 ) continue;

     vector<int> bestTopJetComb = finalCombfatJets[bestTopJetIdx];
     vector<vector<int> > bestTopJetSubStruc = finalCombJetSubStruc[bestTopJetIdx];

     vector<int> remainTopJetComb = finalRemaining[bestTopJetIdx];

     TLorentzVector bestTopJetLVec = buildLVec(oriJetsVec, bestTopJetComb);
     
     double bestTopJetMass = bestTopJetLVec.M();

// Some mass cuts on the fat top jet
     if( ! (bestTopJetMass > lowTopCut && bestTopJetMass < highTopCut) ) continue;
     cntTaggedTopEventsWithinMassCuts += evtWeight;

     vector<double> bestSubMassVec;
     calcSubStrucMass(oriJetsVec, bestTopJetSubStruc, bestSubMassVec);

     double bestm12 = bestSubMassVec[0], bestm13 = bestSubMassVec[1], bestm23 = bestSubMassVec[2];

     double bestCloseTomW = -1; TLorentzVector bestCloseTomWLVec, bestbJetLVec;
     for(unsigned int ib=0; ib<bestSubMassVec.size(); ib++){
        double perMjj = bestSubMassVec[ib];
        if( bestCloseTomW == -1 ){
           bestCloseTomW = perMjj;
           bestCloseTomWLVec = buildLVec(oriJetsVec, bestTopJetSubStruc[ib]);
           for(unsigned jb=0; jb<bestTopJetComb.size(); jb++){
              TLorentzVector perbestbJetLVec = oriJetsVec[bestTopJetComb[jb]];
              bool isFound = false;
              for(unsigned jr=0; jr<bestTopJetSubStruc[ib].size(); jr++){
                 if( bestTopJetSubStruc[ib][jr] == bestTopJetComb[jb] ) isFound = true;
              }
              if( !isFound ) bestbJetLVec = perbestbJetLVec;
           }
        }else if( fabs( bestCloseTomW - mW ) > fabs( perMjj - mW ) ){
           bestCloseTomW = perMjj;
           bestCloseTomWLVec = buildLVec(oriJetsVec, bestTopJetSubStruc[ib]);
           for(unsigned jb=0; jb<bestTopJetComb.size(); jb++){
              TLorentzVector perbestbJetLVec = oriJetsVec[bestTopJetComb[jb]];
              bool isFound = false;
              for(unsigned jr=0; jr<bestTopJetSubStruc[ib].size(); jr++){
                 if( bestTopJetSubStruc[ib][jr] == bestTopJetComb[jb] ) isFound = true;
              }
              if( !isFound ) bestbJetLVec = perbestbJetLVec;
           }
        }
     }

     bool remainPassCSVL = false, remainPassCSVS = false, remainPassCSVT = false;
     vector<int> pickedbTaggedJetIdxVec;
     int pickedbTaggedJetIdx = -1; double maxCSVS = -1;
     for(unsigned int ir=0; ir<remainTopJetComb.size(); ir++){
        int idx = remainTopJetComb[ir];
        if( recoJetsBtagCSVS[idx] > CSVL && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ) remainPassCSVL = true; 
        if( recoJetsBtagCSVS[idx] > CSVS && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ){
           remainPassCSVS = true;
           pickedbTaggedJetIdxVec.push_back(idx);
           if( pickedbTaggedJetIdx == -1 ){
              pickedbTaggedJetIdx = idx;
              maxCSVS = recoJetsBtagCSVS[idx];
           }else if( maxCSVS < recoJetsBtagCSVS[idx] && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ){
              pickedbTaggedJetIdx = idx;
              maxCSVS = recoJetsBtagCSVS[idx];
           }
        }
        if( recoJetsBtagCSVS[idx] > CSVT && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ) remainPassCSVT = true; 
     }

    
     bool bestPassCSVL = false, bestPassCSVS = false, bestPassCSVT = false;
     vector<int> pickedbTaggedbestTopJetIdxVec;
     for(unsigned int ib=0; ib<bestTopJetComb.size(); ib++){
        int idx = bestTopJetComb[ib];
        if( recoJetsBtagCSVS[idx] > CSVL && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ) bestPassCSVL = true;
        if( recoJetsBtagCSVS[idx] > CSVS && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ){
           bestPassCSVS = true;
           pickedbTaggedbestTopJetIdxVec.push_back(idx);
        }
        if( recoJetsBtagCSVS[idx] > CSVT && fabs(oriJetsVec[idx].Eta())<maxEtaForbJets ) bestPassCSVT = true;
     }
    
     vector<int> bTaggedCSVSJetsIdxVec, bTaggedCSVTJetsIdxVec;

     for(unsigned int ij=0; ij<oriJetsVec.size(); ij++){
        if( recoJetsBtagCSVS[ij] > CSVS && fabs(oriJetsVec[ij].Eta())<maxEtaForbJets ) bTaggedCSVSJetsIdxVec.push_back(ij);
        if( recoJetsBtagCSVS[ij] > CSVT && fabs(oriJetsVec[ij].Eta())<maxEtaForbJets ) bTaggedCSVTJetsIdxVec.push_back(ij);
     } 
     

     if( remainPassCSVL ) cntTaggedAllCutsPlusCSVL += evtWeight;
     if( remainPassCSVS ) cntTaggedAllCutsPlusCSVS += evtWeight;
     if( remainPassCSVT ) cntTaggedAllCutsPlusCSVT += evtWeight;

     if( bestPassCSVL ) cntTaggedbestFatJetPlusCSVL += evtWeight;
     if( bestPassCSVS ) cntTaggedbestFatJetPlusCSVS += evtWeight;
     if( bestPassCSVT ) cntTaggedbestFatJetPlusCSVT += evtWeight;

     h1_bestnBtags->Fill( pickedbTaggedJetIdxVec.size(), evtWeight);

     int nRemaining = (int)remainTopJetComb.size();

// Find a good top candidate in the remaining jets
     vector<vector<int> > tempRemainingCombfatJets; vector<vector<int> > tempRemaining2; vector<vector<vector<int> > > tempRemainingCombJetSubStruc;
// Prepare all possible combination in the remaining jets
// XXX: the index stored in the "temp" are all relative index to the nRemaining, i.e., no knowledge of the jet pt sorting
// TODO: No requirement of the three jets in a cone --> efficiency is way too low for this requirement?
     prepareCombination(nRemaining, tempRemainingCombfatJets, tempRemaining2, tempRemainingCombJetSubStruc);
// Transfer the combination index to the index in oriJetVec
     vector<vector<int> > finalRemainingCombfatJets; vector<vector<int> > finalRemaining2; vector<vector<vector<int> > > finalRemainingCombJetSubStruc;
     for(unsigned int ir=0; ir<tempRemainingCombfatJets.size(); ir++){
        vector<int> perComb;
        for(unsigned int ip=0; ip<tempRemainingCombfatJets[ir].size(); ip++){
           int truIdx = remainTopJetComb[tempRemainingCombfatJets[ir][ip]];
           perComb.push_back(truIdx);
        }

        finalRemainingCombfatJets.push_back(perComb);

        vector<int> perRemaining;
        for(unsigned int ip=0; ip<tempRemaining2[ir].size(); ip++){
           int truIdx = remainTopJetComb[tempRemaining2[ir][ip]];
           perRemaining.push_back(truIdx);
        }

        finalRemaining2.push_back(perRemaining);

        vector<vector<int> > perSubStruc;
        for(unsigned int is=0; is<tempRemainingCombJetSubStruc[ir].size(); is++){
           vector<int> perSubComb;
           for(unsigned int isc=0; isc<tempRemainingCombJetSubStruc[ir][is].size(); isc++){
              int truIdx = remainTopJetComb[tempRemainingCombJetSubStruc[ir][is][isc]];
              perSubComb.push_back(truIdx);
           }
           perSubStruc.push_back(perSubComb);
        }
        finalRemainingCombJetSubStruc.push_back(perSubStruc);
     }

// Order the combination in pt!
     ptOrderedfatJets(oriJetsVec, finalRemainingCombfatJets, finalRemaining2, finalRemainingCombJetSubStruc);

     vector<double> finalRemainingCombMaxDR;
     for(unsigned int ir=0; ir<finalRemainingCombfatJets.size(); ir++){
        TLorentzVector centralLVec = buildLVec(oriJetsVec, finalRemainingCombfatJets[ir]);
        double maxDR = 0.0;
        for(unsigned int is=0; is<finalRemainingCombfatJets[ir].size(); is++){
           TLorentzVector perJetLVec = oriJetsVec[finalRemainingCombfatJets[ir][is]];
           double perDR = perJetLVec.DeltaR(centralLVec);
           if( maxDR < perDR ) maxDR = perDR;
        }
        finalRemainingCombMaxDR.push_back(maxDR);
     }

// Find the best top candidate in the fat jets combindation from the remaining jets
     int pickedRemainingCombfatJetIdx = -1; double pickedRemainingCombfatJetMass = -1;
     int cntbTaggedIndex = -1;
     for(unsigned int ir=0; ir<finalRemainingCombfatJets.size(); ir++){
        double tmpTopMass = buildLVec(oriJetsVec, finalRemainingCombfatJets[ir]).M();

        if( !filterfatJetWithDRcone(oriJetsVec, finalRemainingCombfatJets[ir], simuCALargerdeltaR) ) continue;

        if( orderingOptArr[1] == "dr" ){
           if( pickedRemainingCombfatJetIdx == -1 ){
              pickedRemainingCombfatJetIdx = ir;
              pickedRemainingCombfatJetMass = tmpTopMass;
           }else{
              double pickedmaxDR = finalRemainingCombMaxDR[pickedRemainingCombfatJetIdx];
              if( pickedmaxDR > finalRemainingCombMaxDR[ir] ){
                 pickedRemainingCombfatJetIdx = ir;
                 pickedRemainingCombfatJetMass = tmpTopMass;
              }
           }
        }else if( orderingOptArr[1] == "mass" ){
           if( pickedRemainingCombfatJetIdx == -1 ){
              pickedRemainingCombfatJetIdx = ir;
              pickedRemainingCombfatJetMass = tmpTopMass;
           }else if( fabs(pickedRemainingCombfatJetMass - mTop ) > fabs(tmpTopMass - mTop) ){
              pickedRemainingCombfatJetIdx = ir;
              pickedRemainingCombfatJetMass = tmpTopMass;
           }
        }else if( orderingOptArr[1] == "pt" ){
           if( cntbTaggedIndex <=maxIndexForOrderingArr[1] ){
              if( pickedRemainingCombfatJetIdx == -1 ){
                 pickedRemainingCombfatJetIdx = ir;
                 pickedRemainingCombfatJetMass = tmpTopMass;
              }
           }
        }else if( orderingOptArr[1] == "hybrid" ){
           if( cntbTaggedIndex <=maxIndexForOrderingArr[1] ){
              if( pickedRemainingCombfatJetIdx == -1 ){
                 pickedRemainingCombfatJetIdx = ir;
                 pickedRemainingCombfatJetMass = tmpTopMass;
              }else if( fabs(pickedRemainingCombfatJetMass - mTop ) > fabs(tmpTopMass - mTop) ){
                 pickedRemainingCombfatJetIdx = ir;
                 pickedRemainingCombfatJetMass = tmpTopMass;
              }
           }
        }
     }

// Must have another fat jet in the remaining part of the events
// Well, this is definitely true for a "t + tbar" event
     if( pickedRemainingCombfatJetIdx == -1 ) continue; 

     vector<int> pickedRemainingCombfatJet; vector<vector<int> > pickedRemainingCombJetSubStruc;
     pickedRemainingCombfatJet = finalRemainingCombfatJets[pickedRemainingCombfatJetIdx];
     pickedRemainingCombJetSubStruc = finalRemainingCombJetSubStruc[pickedRemainingCombfatJetIdx];

     TLorentzVector remainingTopJetLVec = buildLVec(oriJetsVec, pickedRemainingCombfatJet);

     double pickedRemainingfatJetMass = remainingTopJetLVec.M();
     vector<double> pickedRemainingSubMassVec;
     calcSubStrucMass(oriJetsVec, pickedRemainingCombJetSubStruc, pickedRemainingSubMassVec);

     double pickedRemainingm12 = pickedRemainingSubMassVec[0], pickedRemainingm13 = pickedRemainingSubMassVec[1], pickedRemainingm23 = pickedRemainingSubMassVec[2];

     if( ! (pickedRemainingfatJetMass > lowTopCut && pickedRemainingfatJetMass < highTopCut) ) continue;
     cntTwoTaggedTopJetsWithinMassCuts += evtWeight;
     
     double mTbestTopJet = sqrt( bestTopJetMass*bestTopJetMass + 2*( met*sqrt(bestTopJetMass*bestTopJetMass + bestTopJetLVec.Pt()*bestTopJetLVec.Pt()) -( met*cos(metphi)*bestTopJetLVec.Px() + met*sin(metphi)*bestTopJetLVec.Py() ) ) );

     double mTremainingTopJet = sqrt( pickedRemainingfatJetMass*pickedRemainingfatJetMass + 2*( met*sqrt(pickedRemainingfatJetMass*pickedRemainingfatJetMass + remainingTopJetLVec.Pt()*remainingTopJetLVec.Pt()) -( met*cos(metphi)*remainingTopJetLVec.Px() + met*sin(metphi)*remainingTopJetLVec.Py() ) ) );

     int closebJetIdx = -1; double closebJetDphi = -1;
     for(unsigned int ipb=0; ipb<bTaggedCSVSJetsIdxVec.size(); ipb++){
        if( closebJetIdx == -1 ){
           closebJetIdx = bTaggedCSVSJetsIdxVec[ipb];
           closebJetDphi = fabs(TVector2::Phi_mpi_pi( oriJetsVec[bTaggedCSVSJetsIdxVec[ipb]].Phi() - metphi));
        }else if( closebJetDphi > fabs(TVector2::Phi_mpi_pi( oriJetsVec[bTaggedCSVSJetsIdxVec[ipb]].Phi() - metphi)) ){
           closebJetIdx = bTaggedCSVSJetsIdxVec[ipb];
           closebJetDphi = fabs(TVector2::Phi_mpi_pi( oriJetsVec[bTaggedCSVSJetsIdxVec[ipb]].Phi() - metphi));
        }
     }

     double mTbJet = -1;
     if( closebJetIdx != -1 ) mTbJet = sqrt( oriJetsVec[closebJetIdx].M()*oriJetsVec[closebJetIdx].M() + 2*(met*sqrt(oriJetsVec[closebJetIdx].M()*oriJetsVec[closebJetIdx].M() + oriJetsVec[closebJetIdx].Pt()*oriJetsVec[closebJetIdx].Pt()) - (met*cos(metphi)*oriJetsVec[closebJetIdx].Px() + met*sin(metphi)*oriJetsVec[closebJetIdx].Py()) ));

     double mTJet0 = sqrt( oriJetsVec[0].M()*oriJetsVec[0].M() + 2*(met*sqrt(oriJetsVec[0].M()*oriJetsVec[0].M() + oriJetsVec[0].Pt()*oriJetsVec[0].Pt()) - (met*cos(metphi)*oriJetsVec[0].Px() + met*sin(metphi)*oriJetsVec[0].Py()) ) );
     double mTJet1 = sqrt( oriJetsVec[1].M()*oriJetsVec[1].M() + 2*(met*sqrt(oriJetsVec[1].M()*oriJetsVec[1].M() + oriJetsVec[1].Pt()*oriJetsVec[1].Pt()) - (met*cos(metphi)*oriJetsVec[1].Px() + met*sin(metphi)*oriJetsVec[1].Py()) ) );
     double mTJet2 = sqrt( oriJetsVec[2].M()*oriJetsVec[2].M() + 2*(met*sqrt(oriJetsVec[2].M()*oriJetsVec[2].M() + oriJetsVec[2].Pt()*oriJetsVec[2].Pt()) - (met*cos(metphi)*oriJetsVec[2].Px() + met*sin(metphi)*oriJetsVec[2].Py()) ) );
     double mTJet3 = sqrt( oriJetsVec[3].M()*oriJetsVec[3].M() + 2*(met*sqrt(oriJetsVec[3].M()*oriJetsVec[3].M() + oriJetsVec[3].Pt()*oriJetsVec[3].Pt()) - (met*cos(metphi)*oriJetsVec[3].Px() + met*sin(metphi)*oriJetsVec[3].Py()) ) );

     h1_mTbestTopJet->Fill(mTbestTopJet, evtWeight);
     h1_mTremainingTopJet->Fill(mTremainingTopJet, evtWeight);
     h1_mTbJet->Fill( mTbJet, evtWeight);

     if( bTaggedCSVSJetsIdxVec.size() >=2 && mTbJet > mTbcut ){
        cntPassingMTclosebJetCut += evtWeight;
     }

     if( bTaggedCSVTJetsIdxVec.size() ==1 && mTJet0 > mTjcut && mTJet1 > mTjcut && mTJet2 > mTjcut && mTJet3 > mTjcut ){
        cntPassingMTjCut += evtWeight;
     }

     if(   ( bTaggedCSVSJetsIdxVec.size() >=2 && mTbJet > mTbcut )
         || ( bTaggedCSVTJetsIdxVec.size() ==1 && mTJet0 > mTjcut && mTJet1 > mTjcut && mTJet2 > mTjcut && mTJet3 > mTjcut ) ){
           cntPassingMTjandMTbCut += evtWeight;
     }

     if( doMTMTjcuts && 
         ! ( ( bTaggedCSVSJetsIdxVec.size() >=2 && mTbJet > mTbcut ) || ( bTaggedCSVTJetsIdxVec.size() ==1 && mTJet0 > mTjcut && mTJet1 > mTjcut && mTJet2 > mTjcut && mTJet3 > mTjcut ) ) 
     ) continue;

     h2_mTt1vsmTt2->Fill(mTremainingTopJet, mTbestTopJet);
     h2_mTt1vsmTb->Fill(mTbJet, mTbestTopJet);
     h2_mTt2vsmTb->Fill(mTbJet, mTremainingTopJet);

     h1_metInAConeAndWTopCats->Fill(met, evtWeight);

     h2_bestm23overm123vsarctanm13overm12->Fill(TMath::ATan(bestm13/bestm12), bestm23/bestTopJetMass, evtWeight);
     h1_bestfatJet->Fill(bestTopJetMass, evtWeight);
     h1_bestm12->Fill(bestm12, evtWeight); h1_bestm13->Fill(bestm13, evtWeight); h1_bestm23->Fill(bestm23, evtWeight);
     h1_bestCloseTomW->Fill(bestCloseTomW, evtWeight);

     h1_remainingfatJet->Fill(pickedRemainingfatJetMass, evtWeight);
     h1_remainingm12->Fill(pickedRemainingm12, evtWeight);
     h1_remainingm13->Fill(pickedRemainingm13, evtWeight);
     h1_remainingm23->Fill(pickedRemainingm23, evtWeight);
  }

  std::cout<<"cntPassnJetsCut : "<<cntPassnJetsCut<<"  cntTaggedTopEvents : "<<cntTaggedTopEvents<<"  cntTaggedTopEventsWithinMassCuts : "<<cntTaggedTopEventsWithinMassCuts<<"  cntTwoTaggedTopJetsWithinMassCuts : "<<cntTwoTaggedTopJetsWithinMassCuts<<std::endl;
  std::cout<<"cntTaggedAllCutsPlusCSVL : "<<cntTaggedAllCutsPlusCSVL<<"  cntTaggedAllCutsPlusCSVS : "<<cntTaggedAllCutsPlusCSVS<<"  cntTaggedAllCutsPlusCSVT : "<<cntTaggedAllCutsPlusCSVT<<std::endl;
  std::cout<<"cntTaggedbestFatJetPlusCSVL : "<<cntTaggedbestFatJetPlusCSVL<<"  cntTaggedbestFatJetPlusCSVS : "<<cntTaggedbestFatJetPlusCSVS<<"  cntTaggedbestFatJetPlusCSVT : "<<cntTaggedbestFatJetPlusCSVT<<std::endl;
  std::cout<<"cntPassingMTbestTopJetCut : "<<cntPassingMTbestTopJetCut<<"  cntPassingMTclosefatJetCut : "<<cntPassingMTclosefatJetCut<<"  cntPassingMTclosebJetCut : "<<cntPassingMTclosebJetCut<<std::endl;
  std::cout<<"cntPassingMTjCut : "<<cntPassingMTjCut<<"  cntPassingMTjandMTbCut : "<<cntPassingMTjandMTbCut<<std::endl;

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();

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

  tdrStyle->SetPadTopMargin(0.1); tdrStyle->SetPadBottomMargin(0.1);
  tdrStyle->SetPadLeftMargin(0.1); tdrStyle->SetPadRightMargin(0.12);

  tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50); 

  tdrStyle->SetOptStat(1111);

  int nColors = 999;
  Double_t r[]    = {0.0, 0.0, 0.0, 1.0, 1.0};
  Double_t g[]    = {0.0, 1.0, 1.0, 1.0, 0.0};
  Double_t b[]    = {1.0, 1.0, 0.0, 0.0, 0.0};
  Double_t stop[] = {0.000, 0.200, 0.400, 0.600, 0.800, 1.000};
  TColor::CreateGradientColorTable(5, stop, r, g, b, nColors);

  tdrStyle->SetNumberContours(nColors);

  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(2, 2);
 
  c1->Print("topTaggerAtlasMethod.pdf[");

  c1->cd(1);
  h2_m23overm123vsarctanm13overm12->Draw("colz");
  TH1 *h1_RminCat00 = fn_RminCat00->GetHistogram(); h1_RminCat00->SetLineColor(kRed); h1_RminCat00->SetLineWidth(1); h1_RminCat00->Draw("same c");
  TH1 *h1_RcenCat00 = fn_RcenCat00->GetHistogram(); h1_RcenCat00->SetLineColor(kRed); h1_RcenCat00->SetLineWidth(1); h1_RcenCat00->Draw("same c");
  TH1 *h1_RmaxCat00 = fn_RmaxCat00->GetHistogram(); h1_RmaxCat00->SetLineColor(kRed); h1_RmaxCat00->SetLineWidth(1); h1_RmaxCat00->Draw("same c");
  TH1 *h1_RminCat01 = fn_RminCat01->GetHistogram(); h1_RminCat01->SetLineColor(kRed); h1_RminCat01->SetLineWidth(1); h1_RminCat01->Draw("same c");
  TH1 *h1_RcenCat01 = fn_RcenCat01->GetHistogram(); h1_RcenCat01->SetLineColor(kRed); h1_RcenCat01->SetLineWidth(1); h1_RcenCat01->Draw("same c");
  TH1 *h1_RmaxCat01 = fn_RmaxCat01->GetHistogram(); h1_RmaxCat01->SetLineColor(kRed); h1_RmaxCat01->SetLineWidth(1); h1_RmaxCat01->Draw("same c");
  TH1 *h1_RminCat02 = fn_RminCat02->GetHistogram(); h1_RminCat02->SetLineColor(kRed); h1_RminCat02->SetLineWidth(1); h1_RminCat02->Draw("same c");
  TH1 *h1_RcenCat02 = fn_RcenCat02->GetHistogram(); h1_RcenCat02->SetLineColor(kRed); h1_RcenCat02->SetLineWidth(1); h1_RcenCat02->Draw("same c");
  TH1 *h1_RmaxCat02 = fn_RmaxCat02->GetHistogram(); h1_RmaxCat02->SetLineColor(kRed); h1_RmaxCat02->SetLineWidth(1); h1_RmaxCat02->Draw("same c");
  c1->cd(2);
  h1_mfatJetInACone->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  h1_m23InACone->Draw();
  c1->cd(2);
  h1_m23InAConeAndWTopCat00->Draw();
  c1->cd(3);
  h1_m23InAConeAndWTopCat01->Draw();
  c1->cd(4);
  h1_m23InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  h1_m13InACone->Draw();
  c1->cd(2);
  h1_m13InAConeAndWTopCat00->Draw();
  c1->cd(3);
  h1_m13InAConeAndWTopCat01->Draw();
  c1->cd(4);
  h1_m13InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  h1_m12InACone->Draw();
  c1->cd(2);
  h1_m12InAConeAndWTopCat00->Draw();
  c1->cd(3);
  h1_m12InAConeAndWTopCat01->Draw();
  c1->cd(4);
  h1_m12InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  TPad *pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy();
  h1_metInAConeAndWTopCats->Draw();
  c1->cd(2); 
  TPad *pad2 = (TPad*) c1->GetPad(2); pad2->SetLogy();
  h1_topJetPtInAConeAndWTopCats->Draw();
  c1->cd(3); 
  TPad *pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h1_nTopCand->Draw();
  c1->cd(4); 
  TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h1_nTopCandWithBtag->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h2_bestm23overm123vsarctanm13overm12->Draw("colz");
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->SetLogy(0);
  h1_bestfatJet->Draw();
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h1_remainingfatJet->Draw();
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h1_bestnBtags->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  h1_bestm12->Draw();
  c1->cd(2);
  h1_bestm13->Draw();
  c1->cd(3);
  h1_bestm23->Draw();
  c1->cd(4);
  h1_bestCloseTomW->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1);
  h1_remainingm12->Draw();
  c1->cd(2);
  h1_remainingm13->Draw();
  c1->cd(3);
  h1_remainingm23->Draw();
  c1->cd(4);
  h1_remainingCloseTomW->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h1_mTbestTopJet->Rebin(2);
  h1_mTbestTopJet->Draw();
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->SetLogy(0);
  h1_mTremainingTopJet->Draw();
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->SetLogy(0);
  h1_closemT->Draw();
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->SetLogy(0);
  h1_mTbJet->Rebin(2);
  h1_mTbJet->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h1_MT2->Rebin(2);
  h1_MT2->Draw();
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
  h2_remainingmxyNobvsremainingm123->Draw("colz");
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h1_mTbestWJet->Rebin(2);
  h1_mTbestWJet->Draw();
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h1_mTbestbJet->Rebin(2);
  h1_mTbestbJet->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h2_mTt1vsmTt2->Draw("colz");
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
  h2_mTt1vsmTb->Draw("colz");
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h2_mTt1vsMT2->Draw("colz");
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h2_mTt2vsmTb->Draw("colz");
  c1->Print("topTaggerAtlasMethod.pdf");
  
  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h2_mTt2vsMT2->Draw("colz");
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
  h2_mTbvsMT2->Draw("colz");
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h2_mTbestWvsmTbestb->Draw("colz");
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h2_mTbestWvsmTbestTop->Draw("colz");
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h1_bestfatJet1DRremainingbJet->Draw();
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
  h1_bestfatJet1DRremainingfatJet->Draw();
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h1_bestfatJet1DPHIremainingbJet->Draw();
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h1_bestfatJet1DPHIremainingfatJet->Draw();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->cd(1); pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h1_bestWJetDRremainingbJet->Draw();
  c1->cd(2); pad2 = (TPad*) c1->GetPad(2); pad2->Clear();
  h1_bestWJetDPHIremainingbJet->Draw();
  c1->cd(3); pad3 = (TPad*) c1->GetPad(3); pad3->Clear();
  h2_mTbestbvsmTbestTop->Draw("colz");
  c1->cd(4); pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  c1->Print("topTaggerAtlasMethod.pdf");

  c1->Print("topTaggerAtlasMethod.pdf]");

}
