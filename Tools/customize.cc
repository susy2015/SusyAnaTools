// ===========================================================================
// 
//       Filename:  customize.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/30/2015 15:41:08
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "customize.h"

namespace AnaFunctions{

   bool jetPassCuts(const TLorentzVector& jet, const AnaConsts::AccRec& jetCutsArr)
   {
       const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
       double perjetpt = jet.Pt(), perjeteta = jet.Eta();
       return  ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
           && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
           && (     minPt == -1 || perjetpt >= minPt )
           && (     maxPt == -1 || perjetpt < maxPt );
   }       

   int countJets(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){
      int cntNJets =0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
          if(jetPassCuts(inputJets[ij], jetCutsArr)) cntNJets ++;
      }
      return cntNJets;
   }

   int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<double> &inputCSVS, const double cutCSVS, const AnaConsts::AccRec& jetCutsArr){
      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
      int cntNJets =0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         if( !jetPassCuts(inputJets[ij], jetCutsArr) ) continue;
         if( std::isnan(inputCSVS[ij]) ) continue;
         if( inputCSVS[ij] > cutCSVS ) cntNJets ++;
      }
      return cntNJets;
   }

   std::vector<double> calcDPhi(const std::vector<TLorentzVector> &inputJets, const double metphi, const int nDPhi, const AnaConsts::AccRec& jetCutsArr){
      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
      int cntNJets =0;
      std::vector<double> outDPhiVec(nDPhi, 999);
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         if( !jetPassCuts(inputJets[ij], jetCutsArr) ) continue;
         if( cntNJets < nDPhi ){
            double perDPhi = fabs(TVector2::Phi_mpi_pi( inputJets[ij].Phi() - metphi ));
            outDPhiVec[cntNJets] = perDPhi;
         }
         cntNJets ++;
      }
      return outDPhiVec;
   }

   double calcDeltaT(unsigned int pickedJetIdx, const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){
      double deltaT = 0;
      double jres = 0.1;
      double sum = 0.0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         if( ij == pickedJetIdx ) continue;
         if( !jetPassCuts(inputJets.at(ij), jetCutsArr) ) continue;
         sum = sum + ( inputJets.at(pickedJetIdx).Px()*inputJets.at(ij).Py() - inputJets.at(ij).Px()*inputJets.at(pickedJetIdx).Py() ) * ( inputJets.at(pickedJetIdx).Px()*inputJets.at(ij).Py() - inputJets.at(ij).Px()*inputJets.at(pickedJetIdx).Py() );
      }
      deltaT = jres*sqrt(sum)/inputJets.at(pickedJetIdx).Pt();

      return deltaT;
   }
   
   std::vector<double> calcDPhiN(const std::vector<TLorentzVector> &inputJets, const TLorentzVector &metLVec, const int nDPhi, const AnaConsts::AccRec& jetCutsArr){
      int cntNJets =0;
      std::vector<double> outDPhiVec(nDPhi, 999);
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         if( !jetPassCuts(inputJets.at(ij), jetCutsArr) ) continue;
         if( cntNJets < nDPhi ){
            double perDphi = std::abs(TVector2::Phi_mpi_pi( inputJets.at(ij).Phi() - metLVec.Phi() ));
            double dT = calcDeltaT(ij, inputJets, jetCutsArr);
            if( dT/metLVec.Pt()>=1.0 ) outDPhiVec[cntNJets] = perDphi/(TMath::Pi()/2.0);
            else outDPhiVec[cntNJets] = perDphi/std::asin(dT/metLVec.Pt());
         }
         cntNJets ++; 
      }
      return outDPhiVec;
   }

   bool passMuon(const TLorentzVector& muon, const double& muonIso, const double& muonMtw, int flagID, const AnaConsts::IsoAccRec& muonsArr)
   {
      const double minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt, maxIso = muonsArr.maxIso, maxMtw = muonsArr.maxMtw; 
      double permuonpt = muon.Pt(), permuoneta = muon.Eta();
      return ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
          && (     minPt == -1 || permuonpt >= minPt )
          && (     maxPt == -1 || permuonpt < maxPt )
          && (    maxIso == -1 || muonIso < maxIso )
          && (    maxMtw == -1 || muonMtw < maxMtw )
          && flagID;
   }

   bool passMuonAccOnly(const TLorentzVector& muon, const AnaConsts::IsoAccRec& muonsArr)
   {
      const double minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt;
      double permuonpt = muon.Pt(), permuoneta = muon.Eta();
      return ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
          && (     minPt == -1 || permuonpt >= minPt )
          && (     maxPt == -1 || permuonpt < maxPt );
   }
   
   int countMuons(const std::vector<TLorentzVector> &muonsLVec, const std::vector<double> &muonsRelIso, const std::vector<double> &muonsMtw, const std::vector<int> &muonsFlagID, const AnaConsts::IsoAccRec& muonsArr){
      
      int cntNMuons = 0;
      for(unsigned int im=0; im<muonsLVec.size(); im++){
          if(passMuon(muonsLVec[im], muonsRelIso[im], muonsMtw[im], muonsFlagID[im], muonsArr)) cntNMuons ++;
      }
      return cntNMuons;
   }

   bool passElectron(const TLorentzVector& elec, const double electronIso, const double electronMtw, bool isEB, int flagID, const AnaConsts::ElecIsoAccRec& elesArr)
   {
       const double minAbsEta = elesArr.minAbsEta, maxAbsEta = elesArr.maxAbsEta, minPt = elesArr.minPt, maxPt = elesArr.maxPt, maxIso = (isEB)?(elesArr.maxIsoEB):(elesArr.maxIsoEE), maxMtw = elesArr.maxMtw;
       double perelectronpt = elec.Pt(), perelectroneta = elec.Eta();
       return ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
           && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
           && (     minPt == -1 || perelectronpt >= minPt )
           && (     maxPt == -1 || perelectronpt < maxPt ) 
           && (    maxIso == -1 || electronIso < maxIso )
           && (    maxMtw == -1 || electronMtw < maxMtw )
           && flagID;
   }

   bool passElectronAccOnly(const TLorentzVector& elec, const AnaConsts::ElecIsoAccRec& elesArr)
   {
       const double minAbsEta = elesArr.minAbsEta, maxAbsEta = elesArr.maxAbsEta, minPt = elesArr.minPt, maxPt = elesArr.maxPt;
       double perelectronpt = elec.Pt(), perelectroneta = elec.Eta();
       return ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
           && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
           && (     minPt == -1 || perelectronpt >= minPt )
           && (     maxPt == -1 || perelectronpt < maxPt );
   }

   int countOldElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<double> &electronsRelIso, const std::vector<double> &electronsMtw, const std::vector<int> &electronsFlagID, const AnaConsts::ElecIsoAccRec& elesArr){

      int cntNElectrons = 0;
      for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
          if(passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], true, electronsFlagID[ie], elesArr)) cntNElectrons ++;
      }
      return cntNElectrons;
   }

   int countElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<double> &electronsRelIso, const std::vector<double> &electronsMtw, const std::vector<unsigned int>& isEBVec, const std::vector<int> &electronsFlagID, const AnaConsts::ElecIsoAccRec& elesArr){

      int cntNElectrons = 0;
      for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
          if(passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], isEBVec[ie], electronsFlagID[ie], elesArr)) cntNElectrons ++;
      }
      return cntNElectrons;
   }

   double getElectronActivity(const TLorentzVector& elec, const std::vector<TLorentzVector>& jetLVec, const std::vector<double>& recoJetschargedHadronEnergyFraction, const AnaConsts::ActRec& elesAct)
   {
       double activity = 0;
       for( unsigned int i = 0 ; i < jetLVec.size() ; i++ )
       {
           if( ROOT::Math::VectorUtil::DeltaR(elec , jetLVec.at(i)) < elesAct.maxdR && jetLVec.at(i).Pt() > elesAct.minPt )
//for now, we study the activity integral over jetpt > 10, we will lower the pt cut later
           {
               activity += jetLVec.at(i).Pt() * recoJetschargedHadronEnergyFraction.at(i);
           }
           else
               continue;
       }
       return activity;
   }

   double getMuonActivity(const TLorentzVector& muon, const std::vector<TLorentzVector>& jetLVec, const std::vector<double>& recoJetschargedHadronEnergyFraction, const std::vector<double>& recoJetschargedEmEnergyFraction, const AnaConsts::ActRec& muonsAct)
   {
       double activity = 0;
       for( unsigned int i = 0 ; i < jetLVec.size() ; i++ )
       {
           if( ROOT::Math::VectorUtil::DeltaR(muon , jetLVec.at(i)) < muonsAct.maxdR && jetLVec.at(i).Pt() > muonsAct.minPt )
//for now, we study the activity integral over jetpt > 10, we will lower the pt cut later
           {
               activity += jetLVec.at(i).Pt() * (recoJetschargedEmEnergyFraction.at(i) + recoJetschargedHadronEnergyFraction.at(i));
           }
           else
               continue;
       }
       return activity;
   }


   bool passIsoTrk(const TLorentzVector& isoTrk, const double isoTrkIso, const double isoTrkMtw, const AnaConsts::IsoAccRec& isoTrksArr)
   {
      const double minAbsEta = isoTrksArr.minAbsEta, maxAbsEta = isoTrksArr.maxAbsEta, minPt = isoTrksArr.minPt, maxPt = isoTrksArr.maxPt, maxIso = isoTrksArr.maxIso, maxMtw = isoTrksArr.maxMtw;
      double perisotrkpt = isoTrk.Pt(), perisotrketa = isoTrk.Eta();
      return ( minAbsEta == -1 || fabs(perisotrketa) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(perisotrketa) < maxAbsEta )
          && (     minPt == -1 || perisotrkpt >= minPt )
          && (     maxPt == -1 || perisotrkpt < maxPt ) 
          && (    maxIso == -1 || isoTrkIso/perisotrkpt < maxIso )
          && (    maxMtw == -1 || isoTrkMtw < maxMtw );
   }

   int countIsoTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<double> &isoTrksIso, const std::vector<double> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

      int cntNIsoTrks = 0;
      for(unsigned int is=0; is<isoTrksLVec.size(); is++){
         if( std::abs(isoTrkspdgId[is]) == 11 || std::abs(isoTrkspdgId[is]) == 13 ){
            if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoLepTrksArr ) ) cntNIsoTrks ++;
         }
         if( std::abs(isoTrkspdgId[is]) == 211 ){
            if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoHadTrksArr ) ) cntNIsoTrks ++;
         }
      }
      return cntNIsoTrks;
   }

   int countIsoLepTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<double> &isoTrksIso, const std::vector<double> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

      int cntNIsoTrks = 0;
      for(unsigned int is=0; is<isoTrksLVec.size(); is++){
         if( std::abs(isoTrkspdgId[is]) == 11 || std::abs(isoTrkspdgId[is]) == 13 ){
            if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoLepTrksArr ) ) cntNIsoTrks ++;
         }
      }
      return cntNIsoTrks;
   }

   int countIsoPionTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<double> &isoTrksIso, const std::vector<double> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

      int cntNIsoTrks = 0;
      for(unsigned int is=0; is<isoTrksLVec.size(); is++){
         if( std::abs(isoTrkspdgId[is]) == 211 ){
            if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoHadTrksArr ) ) cntNIsoTrks ++;
         }
      }
      return cntNIsoTrks;
   }

   void prepareJetsForTagger(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<double> &inirecoJetsBtag, std::vector<TLorentzVector> &jetsLVec_forTagger, std::vector<double> &recoJetsBtag_forTagger){

      jetsLVec_forTagger.clear(); recoJetsBtag_forTagger.clear();

      for(unsigned int ij=0; ij<inijetsLVec.size(); ij++){
         if( !jetPassCuts(inijetsLVec[ij], AnaConsts::pt30Arr) ) continue; 
         jetsLVec_forTagger.push_back(inijetsLVec.at(ij));
         recoJetsBtag_forTagger.push_back(inirecoJetsBtag.at(ij));
      }
   }

// cntNJetsVec stores number of jets counters from tightest requirement to loosest requirement.
// cutCSVS is the CSV cut value
   void preparecntNJets(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<double> &inirecoJetsBtag, const double cutCSVS, std::vector<int> &cntNJetsVec, std::vector<int> &cntNbJetsVec){
      cntNJetsVec.clear(); cntNbJetsVec.clear();
      int cntNJetsPt50Eta24 = countJets(inijetsLVec, AnaConsts::pt50Eta24Arr);
      int cntNJetsPt30Eta24 = countJets(inijetsLVec, AnaConsts::pt30Eta24Arr);
      int cntNJetsPt30      = countJets(inijetsLVec, AnaConsts::pt30Arr);
      cntNJetsVec.push_back(cntNJetsPt50Eta24);
      cntNJetsVec.push_back(cntNJetsPt30Eta24);
      cntNJetsVec.push_back(cntNJetsPt30);

      int cntCSVS = countCSVS(inijetsLVec, inirecoJetsBtag, cutCSVS, AnaConsts::bTagArr);
      cntNbJetsVec.push_back(cntCSVS);
   }

   void preparecalcDPhi(const std::vector<TLorentzVector> &inijetsLVec, const double metphi, std::vector<double> &outDPhiVec){
      outDPhiVec.clear();
      outDPhiVec = calcDPhi(inijetsLVec, metphi, 3, AnaConsts::dphiArr);
   }

   void prepareForNtupleReader(){
      using namespace AnaConsts;
      activatedBranchNames.insert(activatedBranchNames_DataOnly.begin(), activatedBranchNames_DataOnly.end());
   }

   double calcHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){

      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;

      double ht = 0;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
         if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
            && (     minPt == -1 || perjetpt >= minPt )
            && (     maxPt == -1 || perjetpt < maxPt ) ){

            ht += perjetpt;
         }
      }
      return ht;
   }

   TLorentzVector calcMHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){

      const double minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;

      TLorentzVector mhtLVec;
      for(unsigned int ij=0; ij<inputJets.size(); ij++){
         double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
         if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
            && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
            && (     minPt == -1 || perjetpt >= minPt )
            && (     maxPt == -1 || perjetpt < maxPt ) ){

            TLorentzVector tmpLVec;
            tmpLVec.SetPtEtaPhiM( inputJets[ij].Pt(), 0, inputJets[ij].Phi(), 0 );
            mhtLVec -= tmpLVec;
         }
      }

      return mhtLVec;
   }

   bool passBaseline(){

     return true;
   }

   int jetLepdRMatch(const TLorentzVector& lep, const std::vector<TLorentzVector>& jetsLVec, const double jldRMax)
   {
       double dRmin = 999.0;
       int minJMatch = -1;
                
       for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
       {
           double dR = ROOT::Math::VectorUtil::DeltaR(jetsLVec[iJet], lep);
           if(dR < dRmin)
           {
               dRmin = dR;
               minJMatch = iJet;
           }
       }
       if(dRmin < jldRMax) return minJMatch;
       else                return -1;
   }


   void prepareTopTagger(topTagger::type3TopTagger * type3Ptr){
     if( !type3Ptr ) type3Ptr = new topTagger::type3TopTagger();
     type3Ptr->setnJetsSel(AnaConsts::nJetsSel);
     type3Ptr->setCSVS(AnaConsts::cutCSVS);
   }

}
