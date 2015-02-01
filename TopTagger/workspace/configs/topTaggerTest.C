#include "tophead.h"
#include "THStack.h"
#include "tdrstyle.C"
#include "combination.h"
#include "TColor.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<vector<TFile*> >;
#pragma link C++ class vector<vector<TH1D*> >;
#pragma link C++ class vector< map<int, int> >;
#endif

using namespace stdcomb;
using namespace std;

// PDG values (in GeV)
static const double mW = 80.385, mTop = 173.5;

static const double mWoverTop = mW/mTop;

static const double lowRatioWoverTop = 0.85, highRatioWoverTop = 1.15;
static const double lowArcTanm13overm12 = 0.2, highArcTanm13overm12 = 1.3, lowm23overm123 = 0.35;

static const double Rmin = lowRatioWoverTop*mWoverTop, Rmax = highRatioWoverTop*mWoverTop;

static const double simuCAdeltaR = 1.5;

static const double lowTopCut = 80, highTopCut = 270;

static const double CSVL = 0.244, CSVM = 0.679, CSVT = 0.898;

static const int nSubJetsDivs[] = {3, 3};
static const int nJetsInc = 6;

typedef vector<int>::iterator vii;

void fillVector(vii begin, vii end, vector<int> &outVec){
  outVec.clear();
  for (vii it=begin;it!=end;++it) outVec.push_back(*it);
}

void display(vii begin,vii end, const string trailStr ="\n"){
  for (vii it=begin;it!=end;++it) cout<<*it;
  cout<<trailStr;
}

void buildTwoFatJetsIndexComb(vector<int> indexVec, vector<int> &iniIndexComb, vector<vector<int> > &finalCombJet1, vector<vector<int> > &finalCombJet2){

  finalCombJet1.clear(); finalCombJet2.clear();

  vector<vector<int> > combHolder;

  do{
     combHolder.push_back(iniIndexComb);
  }while(next_combination(indexVec.begin(),indexVec.end(),iniIndexComb.begin(),iniIndexComb.end()) );

  unsigned int combSize = combHolder.size();
  for(unsigned int ic=0; ic<combSize; ic++){
     finalCombJet1.push_back(combHolder[ic]);
     finalCombJet2.push_back(indexVec);
     if( nSubJetsDivs[0] == nSubJetsDivs[1] ) finalCombJet1.back().insert( finalCombJet1.back().begin(), 0 );
     for(vii it=finalCombJet1.back().begin(); it!=finalCombJet1.back().end(); it++){
        vii jt = find(finalCombJet2.back().begin(), finalCombJet2.back().end(), (*it));
        if( jt!= finalCombJet2.back().end() ) finalCombJet2.back().erase(jt); 
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

void topTaggerTest(){

  if( nSubJetsDivs[0] + nSubJetsDivs[1] != nJetsInc ){ std::cout<<"ERROR  initialization for nJetsInc & nSubJetsDivs!"<<std::endl; return; }

  vector<int> indexVec; for(int ij=0; ij<nJetsInc; ij++) indexVec.push_back(ij);
  vector<int> iniIndexComb; for(int ij=0; ij<nSubJetsDivs[0]; ij++) iniIndexComb.push_back(ij);
  if( nSubJetsDivs[0] == nSubJetsDivs[1] ){ iniIndexComb.erase(iniIndexComb.begin()); }

  vector<vector<int> > finalCombJet1, finalCombJet2;
  buildTwoFatJetsIndexComb(indexVec, iniIndexComb, finalCombJet1, finalCombJet2 );

  vector<vector<vector<int> > > finalCombJet1SubStruc, finalCombJet2SubStruc;

  unsigned int combSize = finalCombJet1.size();

  for(unsigned int ic=0; ic<combSize; ic++){
      vector<vector<int> > perJet1Sub, perJet2Sub;

      buildSubStrucIndexComb(finalCombJet1[ic], perJet1Sub); finalCombJet1SubStruc.push_back(perJet1Sub);
      buildSubStrucIndexComb(finalCombJet2[ic], perJet2Sub); finalCombJet2SubStruc.push_back(perJet2Sub);

      display(finalCombJet1[ic].begin(), finalCombJet1[ic].end(), "         ");
      display(finalCombJet2[ic].begin(), finalCombJet2[ic].end());

      for(unsigned int isub=0; isub<perJet1Sub.size(); isub++){
         display(perJet1Sub[isub].begin(), perJet1Sub[isub].end(), " ");
      }
      std::cout<<"   ";

      for(unsigned int isub=0; isub<perJet2Sub.size(); isub++){
         display(perJet2Sub[isub].begin(), perJet2Sub[isub].end(), " ");
      }

      std::cout<<std::endl<<std::endl;
  }

  TFile *studyFile = new TFile("studyTopTagger.root");
  TTree *AUXtree = (TTree*) studyFile->Get("histAndTree_genLevelEleMuTauVeto/AUX");
  double jet1pt_TR, jet1eta_TR, jet1phi_TR, jet1energy_TR;
  double jet2pt_TR, jet2eta_TR, jet2phi_TR, jet2energy_TR;
  double jet3pt_TR, jet3eta_TR, jet3phi_TR, jet3energy_TR;
  std::vector<double> *otherJetspt_TR=0, *otherJetseta_TR=0, *otherJetsphi_TR=0, *otherJetsenergy_TR=0;
  int nJets, nJets_CUT; 
  double met, mht, ht, dPhi0_CUT, dPhi1_CUT, dPhi2_CUT;
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
  AUXtree->SetBranchAddress("mht", &mht);
  AUXtree->SetBranchAddress("ht", &ht);
  AUXtree->SetBranchAddress("dPhi0_CUT", &dPhi0_CUT);
  AUXtree->SetBranchAddress("dPhi1_CUT", &dPhi1_CUT);
  AUXtree->SetBranchAddress("dPhi2_CUT", &dPhi2_CUT);

  AUXtree->SetBranchAddress("bTagKeyString", &bTagKeyString);
  AUXtree->SetBranchAddress("recoJetsBtag_6", &recoJetsBtag_CSV);

  int nEntries = AUXtree->GetEntries();
  std::cout<<"\nnEntries : "<<nEntries<<std::endl;

  TH2D *h2_mfatJet1vsmfatJet2 = new TH2D("h2_mfatJet1vsmfatJet2", "InACone: fatJet2 mass versus fatJet1 mass; fatJet1; fatJet2", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_m23overm123vsarctanm13overm12 = new TH2D("h2_m23overm123vsarctanm13overm12", "InACone: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);

  TH1D *h1_mfatJet = new TH1D("h1_mfatJet", "fatJet mass; fatJet", 100, 0, 1000);
  TH1D *h1_mfatJetInACone = new TH1D("h1_mfatJetInACone", "fatJet mass in a cone; fatJet", 100, 0, 1000);

  TH1D *h1_m23 = new TH1D("h1_m23", "m23; m23", 100, 0, 300);
  TH1D *h1_m23InACone = new TH1D("h1_m23InACone", "m23InACone; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat00 = new TH1D("h1_m23InAConeAndWTopCat00", "m23InAConeAndWTopCat00; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat01 = new TH1D("h1_m23InAConeAndWTopCat01", "m23InAConeAndWTopCat01; m23", 100, 0, 300);
  TH1D *h1_m23InAConeAndWTopCat02 = new TH1D("h1_m23InAConeAndWTopCat02", "m23InAConeAndWTopCat02; m23", 100, 0, 300);

  TH1D *h1_m12 = new TH1D("h1_m12", "m12; m12", 100, 0, 300);
  TH1D *h1_m12InACone = new TH1D("h1_m12InACone", "m12InACone; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat00 = new TH1D("h1_m12InAConeAndWTopCat00", "m12InAConeAndWTopCat00; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat01 = new TH1D("h1_m12InAConeAndWTopCat01", "m12InAConeAndWTopCat01; m12", 100, 0, 300);
  TH1D *h1_m12InAConeAndWTopCat02 = new TH1D("h1_m12InAConeAndWTopCat02", "m12InAConeAndWTopCat02; m12", 100, 0, 300);

  TH1D *h1_m13 = new TH1D("h1_m13", "m13; m13", 100, 0, 300);
  TH1D *h1_m13InACone = new TH1D("h1_m13InACone", "m13InACone; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat00 = new TH1D("h1_m13InAConeAndWTopCat00", "m13InAConeAndWTopCat00; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat01 = new TH1D("h1_m13InAConeAndWTopCat01", "m13InAConeAndWTopCat01; m13", 100, 0, 300);
  TH1D *h1_m13InAConeAndWTopCat02 = new TH1D("h1_m13InAConeAndWTopCat02", "m13InAConeAndWTopCat02; m13", 100, 0, 300);

  TH1D *h1_met = new TH1D("h1_met", "met; met", 100, 0, 1000);
  TH1D *h1_metInAConeAndWTopCats = new TH1D("h1_metInAConeAndWTopCats", "metInAConeAndWTopCats; met", 100, 0, 1000);
  TH1D *h1_topJetPtInAConeAndWTopCats = new TH1D("h1_topJetPtInAConeAndWTopCats", "topJetPtInAConeAndWTopCats; top pt", 100, 0, 1000);

  TH1D *h1_nTopCand = new TH1D("h1_nTopCand", "number of top candidate; nTop", 20, 0, 20);

  TH2D *h2_bestmfatJet1vsmfatJet2 = new TH2D("h2_bestmfatJet1vsmfatJet2", "best: fatJet2 mass versus fatJet1 mass; bestJet1; remainJet2", 100, 0, 1000, 100, 0, 1000);
  TH2D *h2_bestm23overm123vsarctanm13overm12 = new TH2D("h2_bestm23overm123vsarctanm13overm12", "best: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);
  TH2D *h2_remainm23overm123vsarctanm13overm12 = new TH2D("h2_remainm23overm123vsarctanm13overm12", "remain: m23/m123 versus arctan m13/m12; arctan m13/m12; m23/m123", 100, 0, 1.5, 100, 0, 1.0);
  TH1D *h1_bestfatJet = new TH1D("h1_bestfatJet", "best fatJet mass; fatJet", 100, 0, 1000);
  TH1D *h1_remainfatJet = new TH1D("h1_remainfatJet", "remain fastJet mass; fatJet", 100, 0, 1000);
  TH1D *h1_bestm12 = new TH1D("h1_bestm12", "best m12; m12", 100, 0, 300);
  TH1D *h1_remainm12 = new TH1D("h1_remainm12", "remain m12; m12", 100, 0, 300);
  TH1D *h1_bestm23 = new TH1D("h1_bestm23", "best m23; m23", 100, 0, 300);
  TH1D *h1_remainm23 = new TH1D("h1_remainm23", "remain m23; m23", 100, 0, 300);
  TH1D *h1_bestm13 = new TH1D("h1_bestm13", "best m13; m13", 100, 0, 300);
  TH1D *h1_remainm13 = new TH1D("h1_remainm13", "remain m13; m13", 100, 0, 300);


  int cntPassnJetsCut = 0, cntTaggedTopEvents =0, cntTaggedTwoTopsEvents =0, cntTaggedTopEventsWithinMassCuts =0, cntTaggedTwoTopsEventsWithinMassCuts =0;
  int cntTaggedAllCutsPlusCSVL =0, cntTaggedAllCutsPlusCSVM =0, cntTaggedAllCutsPlusCSVT =0;
  for(int ie=0; ie<nEntries; ie++){

     AUXtree->GetEntry(ie);
/*
     double newht =0;
     newht += jet1pt_TR;
     newht += jet2pt_TR;
     newht += jet3pt_TR;
     for(unsigned int io=0; io<otherJetspt_TR->size(); io++){ newht += (*otherJetspt_TR)[io]; }
     if( met/sqrt(newht) < 5 ) continue;
     if( dPhi0_CUT < 0.4 || dPhi1_CUT < 0.4 || dPhi2_CUT < 0.4 ) continue;
     if( newht < 275 ) continue;
     if( met < 175 ) continue;
*/
     if( nJets<nJetsInc ) continue;

     cntPassnJetsCut ++;

     vector<TLorentzVector> oriJetsVec;
     TLorentzVector jet1LVec, jet2LVec, jet3LVec;
     jet1LVec.SetPtEtaPhiE(jet1pt_TR, jet1eta_TR, jet1phi_TR, jet1energy_TR);
     jet2LVec.SetPtEtaPhiE(jet2pt_TR, jet2eta_TR, jet2phi_TR, jet2energy_TR);
     jet3LVec.SetPtEtaPhiE(jet3pt_TR, jet3eta_TR, jet3phi_TR, jet3energy_TR);
     oriJetsVec.push_back(jet1LVec);
     oriJetsVec.push_back(jet2LVec);
     oriJetsVec.push_back(jet3LVec);
   
     for(unsigned int io=0; io<nJetsInc-3; io++){
        TLorentzVector jetLVec;
        jetLVec.SetPtEtaPhiE((*otherJetspt_TR)[io], (*otherJetseta_TR)[io], (*otherJetsphi_TR)[io], (*otherJetsenergy_TR)[io]);
        oriJetsVec.push_back(jetLVec);
     } 

     vector<double> fatJet1MassVec, fatJet2MassVec;
     vector<bool> fatJet1closeInAConeVec, fatJet2closeInAConeVec;
     vector<vector<double> > fatJet1SubMassVec, fatJet2SubMassVec;
   
     for(unsigned int ic=0; ic<combSize; ic++){
        fatJet1MassVec.push_back(buildLVec(oriJetsVec, finalCombJet1[ic]).M());
        fatJet2MassVec.push_back(buildLVec(oriJetsVec, finalCombJet2[ic]).M());

        fatJet1closeInAConeVec.push_back(filterfatJetWithDRcone(oriJetsVec, finalCombJet1[ic], simuCAdeltaR));
        fatJet2closeInAConeVec.push_back(filterfatJetWithDRcone(oriJetsVec, finalCombJet2[ic], simuCAdeltaR));
   
        vector<double> perfatJet1SubMassVec;
        calcSubStrucMass(oriJetsVec, finalCombJet1SubStruc[ic], perfatJet1SubMassVec);
        fatJet1SubMassVec.push_back(perfatJet1SubMassVec);
   
        vector<double> perfatJet2SubMassVec;
        calcSubStrucMass(oriJetsVec, finalCombJet2SubStruc[ic], perfatJet2SubMassVec);
        fatJet2SubMassVec.push_back(perfatJet2SubMassVec);
     }
   
     bool isTopEvent = false, isTwoTopsEvent = false, isTopEventWithinMassCuts = false, isTwoTopsEventWithinMassCuts = false;
     vector<int> bestTopJetIdx; // first index is for ic, second index is for jet1 or jet2
     int nTopJetCandCnt =0;
     if( nSubJetsDivs[0] == nSubJetsDivs[1] && nSubJetsDivs[0] == 3 ){

        for(unsigned int ic=0; ic<combSize; ic++){
           double fatJet1m12 = fatJet1SubMassVec[ic][0], fatJet1m13 = fatJet1SubMassVec[ic][1], fatJet1m23 = fatJet1SubMassVec[ic][2];
           double fatJet1m123 = fatJet1MassVec[ic];

           double fatJet2m12 = fatJet2SubMassVec[ic][0], fatJet2m13 = fatJet2SubMassVec[ic][1], fatJet2m23 = fatJet2SubMassVec[ic][2];
           double fatJet2m123 = fatJet2MassVec[ic];

           vector<int> isTopJetVec;
           vector<int> fatJet1PassStatusVec; isTopJetVec.push_back(checkTopCriteria(fatJet1SubMassVec[ic], fatJet1m123, fatJet1PassStatusVec));
           vector<int> fatJet2PassStatusVec; isTopJetVec.push_back(checkTopCriteria(fatJet2SubMassVec[ic], fatJet2m123, fatJet2PassStatusVec));

           if( fatJet1closeInAConeVec[ic] && isTopJetVec[0] ){
              isTopEvent = true;
              if( fatJet1m123 > lowTopCut && fatJet1m123 < highTopCut ){
                 isTopEventWithinMassCuts = true; nTopJetCandCnt ++;
                 h1_topJetPtInAConeAndWTopCats->Fill(buildLVec(oriJetsVec, finalCombJet1[ic]).Pt());

                 bool passCSVM = false;
                 for(unsigned int ir=0; ir<finalCombJet2[ic].size(); ir++){
                    int idx = finalCombJet2[ic][ir];
                    if( (*recoJetsBtag_CSV)[idx] > CSVM ) passCSVM = true;
                 }
                 if( passCSVM ){
                    if( bestTopJetIdx.empty() ){
                       bestTopJetIdx.resize(2); bestTopJetIdx[0] = ic; bestTopJetIdx[1] = 1;
                    }else{
                       double pickedTopMass = bestTopJetIdx[1] ==1 ? fatJet1MassVec[bestTopJetIdx[0]] : fatJet2MassVec[bestTopJetIdx[0]];
                       if( fabs(pickedTopMass - mTop) > fabs( fatJet1m123 - mTop ) ){
                          bestTopJetIdx[0] = ic; bestTopJetIdx[1] = 1;
                       }
                    }
                 }
              }
           }

           if( fatJet2closeInAConeVec[ic] && isTopJetVec[1] ){
              isTopEvent = true;
              if( fatJet2m123 > lowTopCut && fatJet2m123 < highTopCut ){
                 isTopEventWithinMassCuts = true; nTopJetCandCnt ++;
                 h1_topJetPtInAConeAndWTopCats->Fill(buildLVec(oriJetsVec, finalCombJet2[ic]).Pt());

                 bool passCSVM = false;
                 for(unsigned int ir=0; ir<finalCombJet1[ic].size(); ir++){
                    int idx = finalCombJet1[ic][ir];
                    if( (*recoJetsBtag_CSV)[idx] > CSVM ) passCSVM = true;
                 }

                 if( passCSVM ){
                    if( bestTopJetIdx.empty() ){
                       bestTopJetIdx.resize(2); bestTopJetIdx[0] = ic; bestTopJetIdx[1] = 2;
                    }else{
                       double pickedTopMass = bestTopJetIdx[1] ==1 ? fatJet1MassVec[bestTopJetIdx[0]] : fatJet2MassVec[bestTopJetIdx[0]];
                       if( fabs(pickedTopMass - mTop) > fabs( fatJet2m123 - mTop ) ){
                          bestTopJetIdx[0] = ic; bestTopJetIdx[1] = 2;
                       }
                    }
                 }
              }
           }
 
           if( fatJet1closeInAConeVec[ic] && fatJet2closeInAConeVec[ic] && isTopJetVec[0] && isTopJetVec[1] ){
              isTwoTopsEvent = true;
              if(    fatJet1m123 > lowTopCut && fatJet1m123 < highTopCut
                  && fatJet2m123 > lowTopCut && fatJet2m123 < highTopCut ) isTwoTopsEventWithinMassCuts = true;
           }

           h1_mfatJet->Fill(fatJet1m123); h1_mfatJet->Fill(fatJet2m123);
           h1_m23->Fill(fatJet1m23); h1_m23->Fill(fatJet2m23);
           h1_m12->Fill(fatJet1m12); h1_m12->Fill(fatJet2m12);
           h1_m13->Fill(fatJet1m13); h1_m13->Fill(fatJet2m13);

           if( fatJet1closeInAConeVec[ic] ){
              h1_mfatJetInACone->Fill(fatJet1m123);
              h1_m23InACone->Fill(fatJet1m23);
              h1_m12InACone->Fill(fatJet1m12);
              h1_m13InACone->Fill(fatJet1m13);
              if( fatJet1PassStatusVec[0] ){
                 h1_m23InAConeAndWTopCat00->Fill(fatJet1m23);
                 h1_m12InAConeAndWTopCat00->Fill(fatJet1m12);
                 h1_m13InAConeAndWTopCat00->Fill(fatJet1m13);
              }
              if( fatJet1PassStatusVec[1] ){
                 h1_m23InAConeAndWTopCat01->Fill(fatJet1m23);
                 h1_m12InAConeAndWTopCat01->Fill(fatJet1m12);
                 h1_m13InAConeAndWTopCat01->Fill(fatJet1m13);
              }
              if( fatJet1PassStatusVec[2] ){
                 h1_m23InAConeAndWTopCat02->Fill(fatJet1m23);
                 h1_m12InAConeAndWTopCat02->Fill(fatJet1m12);
                 h1_m13InAConeAndWTopCat02->Fill(fatJet1m13);
              }

              h2_m23overm123vsarctanm13overm12->Fill( TMath::ATan(fatJet1m13/fatJet1m12), fatJet1m23/fatJet1m123 );
              h2_mfatJet1vsmfatJet2->Fill(fatJet1m123, fatJet2m123);
           }

           if( fatJet2closeInAConeVec[ic] ){

              h1_mfatJetInACone->Fill(fatJet2m123);
              h1_m23InACone->Fill(fatJet2m23);
              h1_m12InACone->Fill(fatJet2m12);
              h1_m13InACone->Fill(fatJet2m13);
              if( fatJet2PassStatusVec[0] ){
                 h1_m23InAConeAndWTopCat00->Fill(fatJet2m23);
                 h1_m12InAConeAndWTopCat00->Fill(fatJet2m12);
                 h1_m13InAConeAndWTopCat00->Fill(fatJet2m13);
              }
              if( fatJet2PassStatusVec[1] ){
                 h1_m23InAConeAndWTopCat01->Fill(fatJet2m23);
                 h1_m12InAConeAndWTopCat01->Fill(fatJet2m12);
                 h1_m13InAConeAndWTopCat01->Fill(fatJet2m13);
              }
              if( fatJet2PassStatusVec[2] ){
                 h1_m23InAConeAndWTopCat02->Fill(fatJet2m23);
                 h1_m12InAConeAndWTopCat02->Fill(fatJet2m12);
                 h1_m13InAConeAndWTopCat02->Fill(fatJet2m13);
              }

              h2_m23overm123vsarctanm13overm12->Fill( TMath::ATan(fatJet2m13/fatJet2m12), fatJet2m23/fatJet2m123 );
              h2_mfatJet1vsmfatJet2->Fill(fatJet1m123, fatJet2m123);
           }

        }
     }

     h1_nTopCand->Fill(nTopJetCandCnt); 

     h1_met->Fill(met);
     if( isTopEvent ) cntTaggedTopEvents++;
     if( isTwoTopsEvent ) cntTaggedTwoTopsEvents++;
     if( isTopEventWithinMassCuts ){
        cntTaggedTopEventsWithinMassCuts++;
        h1_metInAConeAndWTopCats->Fill(met);
     }
     if( isTwoTopsEventWithinMassCuts ) cntTaggedTwoTopsEventsWithinMassCuts++;

     if( bestTopJetIdx.empty() ) continue;

     vector<int> bestTopJetComb = bestTopJetIdx[1] ==1 ? finalCombJet1[bestTopJetIdx[0]] : finalCombJet2[bestTopJetIdx[0]];
     vector<vector<int> > bestTopJetSubStruc = bestTopJetIdx[1] ==1 ? finalCombJet1SubStruc[bestTopJetIdx[0]] : finalCombJet2SubStruc[bestTopJetIdx[0]];

     vector<int> remainTopJetComb = bestTopJetIdx[1] ==1 ? finalCombJet2[bestTopJetIdx[0]] : finalCombJet1[bestTopJetIdx[0]];
     vector<vector<int> > remainTopJetSubStruc = bestTopJetIdx[1] ==1 ? finalCombJet2SubStruc[bestTopJetIdx[0]] : finalCombJet1SubStruc[bestTopJetIdx[0]];

     double bestTopJetMass = buildLVec(oriJetsVec, bestTopJetComb).M();
     double remainTopJetMass = buildLVec(oriJetsVec, remainTopJetComb).M();

     vector<double> bestSubMassVec, remainSubMassVec;
     calcSubStrucMass(oriJetsVec, bestTopJetSubStruc, bestSubMassVec);
     calcSubStrucMass(oriJetsVec, remainTopJetSubStruc, remainSubMassVec);

     double bestm12 = bestSubMassVec[0], bestm13 = bestSubMassVec[1], bestm23 = bestSubMassVec[2];
     double remainm12 = remainSubMassVec[0], remainm13 = remainSubMassVec[1], remainm23 = remainSubMassVec[2];

     bool remainPassCSVL = false, remainPassCSVM = false, remainPassCSVT = false;
     for(unsigned int ir=0; ir<remainTopJetComb.size(); ir++){
        int idx = remainTopJetComb[ir];
        if( (*recoJetsBtag_CSV)[idx] > CSVL ) remainPassCSVL = true; 
        if( (*recoJetsBtag_CSV)[idx] > CSVM ) remainPassCSVM = true; 
        if( (*recoJetsBtag_CSV)[idx] > CSVT ) remainPassCSVT = true; 
     }

     if( remainPassCSVL ) cntTaggedAllCutsPlusCSVL ++;
     if( remainPassCSVM ) cntTaggedAllCutsPlusCSVM ++;
     if( remainPassCSVT ) cntTaggedAllCutsPlusCSVT ++;

     if( !remainPassCSVM ) continue;

     h2_bestmfatJet1vsmfatJet2->Fill(bestTopJetMass, remainTopJetMass);
     h2_bestm23overm123vsarctanm13overm12->Fill(TMath::ATan(bestm13/bestm12), bestm23/bestTopJetMass);
     h2_remainm23overm123vsarctanm13overm12->Fill(TMath::ATan(remainm13/remainm12), remainm23/remainTopJetMass);

     h1_bestfatJet->Fill(bestTopJetMass);
     h1_remainfatJet->Fill(remainTopJetMass);

     h1_bestm12->Fill(bestm12);
     h1_remainm12->Fill(remainm12);

     h1_bestm13->Fill(bestm13);
     h1_remainm13->Fill(remainm13);

     h1_bestm23->Fill(bestm23);
     h1_remainm23->Fill(remainm23);

  }

  std::cout<<"cntPassnJetsCut : "<<cntPassnJetsCut<<"  cntTaggedTopEvents : "<<cntTaggedTopEvents<<"  cntTaggedTwoTopsEvents : "<<cntTaggedTwoTopsEvents<<"  cntTaggedTopEventsWithinMassCuts : "<<cntTaggedTopEventsWithinMassCuts<<"  cntTaggedTwoTopsEventsWithinMassCuts : "<<cntTaggedTwoTopsEventsWithinMassCuts<<std::endl;
  std::cout<<"cntTaggedAllCutsPlusCSVL : "<<cntTaggedAllCutsPlusCSVL<<"  cntTaggedAllCutsPlusCSVM : "<<cntTaggedAllCutsPlusCSVM<<"  cntTaggedAllCutsPlusCSVT : "<<cntTaggedAllCutsPlusCSVT<<std::endl;

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

  int nColors = 999;
  Double_t r[]    = {0.0, 0.0, 0.0, 1.0, 1.0};
  Double_t g[]    = {0.0, 1.0, 1.0, 1.0, 0.0};
  Double_t b[]    = {1.0, 1.0, 0.0, 0.0, 0.0};
  Double_t stop[] = {0.000, 0.200, 0.400, 0.600, 0.800, 1.000};
  TColor::CreateGradientColorTable(5, stop, r, g, b, nColors);

  tdrStyle->SetNumberContours(nColors);

  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(2, 3);
 
  c1->Print("topTaggerTest.pdf[");

  c1->cd(1);
  h2_mfatJet1vsmfatJet2->Draw("colz");
  c1->cd(2);
  h2_m23overm123vsarctanm13overm12->Draw("colz");
  c1->cd(3);
  h1_mfatJet->Draw();
  c1->cd(4);
  h1_mfatJetInACone->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  h1_m23->Draw();
  c1->cd(2);
  h1_m23InACone->Draw();
  c1->cd(3);
  h1_m23InAConeAndWTopCat00->Draw();
  c1->cd(4);
  h1_m23InAConeAndWTopCat01->Draw();
  c1->cd(5);
  h1_m23InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  h1_m13->Draw();
  c1->cd(2);
  h1_m13InACone->Draw();
  c1->cd(3);
  h1_m13InAConeAndWTopCat00->Draw();
  c1->cd(4);
  h1_m13InAConeAndWTopCat01->Draw();
  c1->cd(5);
  h1_m13InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  h1_m12->Draw();
  c1->cd(2);
  h1_m12InACone->Draw();
  c1->cd(3);
  h1_m12InAConeAndWTopCat00->Draw();
  c1->cd(4);
  h1_m12InAConeAndWTopCat01->Draw();
  c1->cd(5);
  h1_m12InAConeAndWTopCat02->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  TPad *pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy();
  h1_met->Draw();
  c1->cd(2);
  TPad *pad2 = (TPad*) c1->GetPad(2); pad2->SetLogy();
  h1_metInAConeAndWTopCats->Draw();
  c1->cd(3); TPad *pad3 = (TPad*) c1->GetPad(3); pad3->SetLogy();
  h1_topJetPtInAConeAndWTopCats->Draw();
  c1->cd(4); TPad *pad4 = (TPad*) c1->GetPad(4); pad4->Clear();
  h1_nTopCand->Draw();
  c1->cd(5); TPad *pad5 = (TPad*) c1->GetPad(5); pad5->Clear();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  pad1 = (TPad*) c1->GetPad(1); pad1->SetLogy(0);
  h2_bestmfatJet1vsmfatJet2->Draw("colz");
  c1->cd(2);
  pad2 = (TPad*) c1->GetPad(2); pad2->SetLogy(0);
  h2_bestm23overm123vsarctanm13overm12->Draw("colz");
  c1->cd(3);
  pad3 = (TPad*) c1->GetPad(3); pad3->SetLogy(0);
  h2_remainm23overm123vsarctanm13overm12->Draw("colz");
  c1->cd(4);
  h1_bestfatJet->Draw();
  c1->cd(5);
  h1_remainfatJet->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->cd(1);
  h1_bestm12->Draw();
  c1->cd(2);
  h1_remainm12->Draw();
  c1->cd(3);
  h1_bestm13->Draw();
  c1->cd(4);
  h1_remainm13->Draw();
  c1->cd(5);
  h1_bestm23->Draw();
  c1->cd(6);
  h1_remainm23->Draw();
  c1->Print("topTaggerTest.pdf");

  c1->Print("topTaggerTest.pdf]");

}
