#include "customize.h"
#include "SusyUtility.h"

namespace AnaFunctions
{

  bool jetPassCuts(const TLorentzVector& jet, const AnaConsts::AccRec& jetCutsArr)
  {
    const float minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
    float perjetpt = jet.Pt(), perjeteta = jet.Eta();
    return  ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
      && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
      && (     minPt == -1 || perjetpt >= minPt )
      && (     maxPt == -1 || perjetpt < maxPt );
  }       

  int countJets(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){
    int cntNJets =0;
    for(unsigned int i=0; i<inputJets.size(); i++){
      if(jetPassCuts(inputJets[i], jetCutsArr)) cntNJets ++;
    }
    return cntNJets;
  }

  int countCSVS(const std::vector<TLorentzVector> &inputJets, const std::vector<float> &inputCSVS, const float cutCSVS, const AnaConsts::AccRec& jetCutsArr,
      std::vector<unsigned int> *outputIdxs){
    const float minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
    int cntNJets =0;
    for(unsigned int i=0; i<inputJets.size(); i++){
      if( !jetPassCuts(inputJets[i], jetCutsArr) ) continue;
      if( std::isnan(inputCSVS[i]) ) continue;
      if( inputCSVS[i] > cutCSVS ) 
      {
        cntNJets ++;
        outputIdxs->push_back(i);
      }
    }
    return cntNJets;
  }

  std::vector<float> calcDPhi(const std::vector<TLorentzVector> &inputJets, const TLorentzVector &metLVec, const int nDPhi, const AnaConsts::AccRec& jetCutsArr)
  {
    const float minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;
    int cntNJets =0;
    int i = 0;
    
    std::vector<std::pair<TLorentzVector, unsigned>> sorted_jets;
    for (const auto& jet : inputJets)
    {
      sorted_jets.push_back({jet, i});
      ++i;
    }
    
    // sort jets by pt (since JEC change jet pt)
    std::sort(sorted_jets.begin(), sorted_jets.end(), SusyUtility::greaterThan<TLorentzVector, unsigned>);
    
    std::vector<float> outDPhiVec(nDPhi, 999);
    //for(unsigned int i=0; i<inputJets.size(); i++){
    for (const auto& p : sorted_jets)
    {
      // jet index sorted by pt
      i = p.second; 
      if( !jetPassCuts(inputJets[i], jetCutsArr) ) continue;
      if( cntNJets < nDPhi )
      {
        //float perDPhi = fabs(TVector2::Phi_mpi_pi( inputJets[i].Phi() - metphi ));   // using metphi
        float perDPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(inputJets[i], metLVec)); // using metLVec
        outDPhiVec[cntNJets] = perDPhi;
      }
      cntNJets ++;
    }
    return outDPhiVec;
  }

  float calcDeltaT(unsigned int pickedJetIdx, const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){
    float deltaT = 0;
    float jres = 0.1;
    float sum = 0.0;
    for(unsigned int i=0; i<inputJets.size(); i++){
      if( i == pickedJetIdx ) continue;
      if( !jetPassCuts(inputJets.at(i), jetCutsArr) ) continue;
      sum = sum + ( inputJets.at(pickedJetIdx).Px()*inputJets.at(i).Py() - inputJets.at(i).Px()*inputJets.at(pickedJetIdx).Py() ) * ( inputJets.at(pickedJetIdx).Px()*inputJets.at(i).Py() - inputJets.at(i).Px()*inputJets.at(pickedJetIdx).Py() );
    }
    deltaT = jres*sqrt(sum)/inputJets.at(pickedJetIdx).Pt();

    return deltaT;
  }

  std::vector<float> calcDPhiN(const std::vector<TLorentzVector> &inputJets, const TLorentzVector &metLVec, const int nDPhi, const AnaConsts::AccRec& jetCutsArr){
    int cntNJets =0;
    std::vector<float> outDPhiVec(nDPhi, 999);
    for(unsigned int i=0; i<inputJets.size(); i++){
      if( !jetPassCuts(inputJets.at(i), jetCutsArr) ) continue;
      if( cntNJets < nDPhi ){
        float perDphi = std::abs(TVector2::Phi_mpi_pi( inputJets.at(i).Phi() - metLVec.Phi() ));
        float dT = calcDeltaT(i, inputJets, jetCutsArr);
        if( dT/metLVec.Pt()>=1.0 ) outDPhiVec[cntNJets] = perDphi/(TMath::Pi()/2.0);
        else outDPhiVec[cntNJets] = perDphi/std::asin(dT/metLVec.Pt());
      }
      cntNJets ++; 
    }
    return outDPhiVec;
  }

  bool passMuon(const TLorentzVector& muon, const float& muonIso, const float& muonMtw, unsigned char flagID, const AnaConsts::IsoAccRec& muonsArr)
  {
    const float minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt, maxIso = muonsArr.maxIso, maxMtw = muonsArr.maxMtw; 
    float permuonpt = muon.Pt(), permuoneta = muon.Eta();
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
    const float minAbsEta = muonsArr.minAbsEta, maxAbsEta = muonsArr.maxAbsEta, minPt = muonsArr.minPt, maxPt = muonsArr.maxPt;
    float permuonpt = muon.Pt(), permuoneta = muon.Eta();
    return ( minAbsEta == -1 || fabs(permuoneta) >= minAbsEta )
      && ( maxAbsEta == -1 || fabs(permuoneta) < maxAbsEta )
      && (     minPt == -1 || permuonpt >= minPt )
      && (     maxPt == -1 || permuonpt < maxPt );
  }

  int countMuons(const std::vector<TLorentzVector> &muonsLVec, const std::vector<float> &muonsRelIso, const std::vector<float> &muonsMtw, const std::vector<unsigned char> &muonsFlagID, const AnaConsts::IsoAccRec& muonsArr){

    int cntNMuons = 0;
    for(unsigned int im=0; im<muonsLVec.size(); im++){
      if(passMuon(muonsLVec[im], muonsRelIso[im], muonsMtw[im], muonsFlagID[im], muonsArr)) cntNMuons ++;
    }
    return cntNMuons;
  }

  bool passElectron(const TLorentzVector& elec, const float electronIso, const float electronMtw, unsigned char flagID, const AnaConsts::IsoAccRec& elesArr)
  {
    const float minAbsEta = elesArr.minAbsEta, maxAbsEta = elesArr.maxAbsEta, minPt = elesArr.minPt, maxPt = elesArr.maxPt, maxIso = elesArr.maxIso, maxMtw = elesArr.maxMtw;
    float perelectronpt = elec.Pt(), perelectroneta = elec.Eta();
    return ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
      && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
      && (     minPt == -1 || perelectronpt >= minPt )
      && (     maxPt == -1 || perelectronpt < maxPt ) 
      && (    maxIso == -1 || electronIso < maxIso )
      && (    maxMtw == -1 || electronMtw < maxMtw )
      && flagID;
  }

  bool passElectronAccOnly(const TLorentzVector& elec, const AnaConsts::IsoAccRec& elesArr)
  {
    const float minAbsEta = elesArr.minAbsEta, maxAbsEta = elesArr.maxAbsEta, minPt = elesArr.minPt, maxPt = elesArr.maxPt;
    float perelectronpt = elec.Pt(), perelectroneta = elec.Eta();
    return ( minAbsEta == -1 || fabs(perelectroneta) >= minAbsEta )
      && ( maxAbsEta == -1 || fabs(perelectroneta) < maxAbsEta )
      && (     minPt == -1 || perelectronpt >= minPt )
      && (     maxPt == -1 || perelectronpt < maxPt );
  }

  int countOldElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<float> &electronsRelIso, const std::vector<float> &electronsMtw, const std::vector<int> &electronsFlagID, const AnaConsts::IsoAccRec& elesArr){

    int cntNElectrons = 0;
    for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
      if(passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], electronsFlagID[ie], elesArr)) cntNElectrons ++;
    }
    return cntNElectrons;
  }

  int countElectrons(const std::vector<TLorentzVector> &electronsLVec, const std::vector<float> &electronsRelIso, const std::vector<float> &electronsMtw, const std::vector<unsigned char> &electronsFlagID, const AnaConsts::IsoAccRec& elesArr){

    int cntNElectrons = 0;
    for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
      if(passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], electronsFlagID[ie], elesArr)) cntNElectrons ++;
    }
    return cntNElectrons;
  }

  float getElectronActivity(const TLorentzVector& elec, const std::vector<TLorentzVector>& jetLVec, const std::vector<float>& recoJetschargedHadronEnergyFraction, const AnaConsts::ActRec& elesAct)
  {
    float activity = 0;
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

  float getMuonActivity(const TLorentzVector& muon, const std::vector<TLorentzVector>& jetLVec, const std::vector<float>& recoJetschargedHadronEnergyFraction, const std::vector<float>& recoJetschargedEmEnergyFraction, const AnaConsts::ActRec& muonsAct)
  {
    float activity = 0;
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


  bool passIsoTrk(const TLorentzVector& isoTrk, const float isoTrkIso, const float isoTrkMtw, const AnaConsts::IsoAccRec& isoTrksArr)
  {
    const float minAbsEta = isoTrksArr.minAbsEta, maxAbsEta = isoTrksArr.maxAbsEta, minPt = isoTrksArr.minPt, maxPt = isoTrksArr.maxPt, maxIso = isoTrksArr.maxIso, maxMtw = isoTrksArr.maxMtw;
    float perisotrkpt = isoTrk.Pt(), perisotrketa = isoTrk.Eta();
    return ( minAbsEta == -1 || fabs(perisotrketa) >= minAbsEta )
      && ( maxAbsEta == -1 || fabs(perisotrketa) < maxAbsEta )
      && (     minPt == -1 || perisotrkpt >= minPt )
      && (     maxPt == -1 || perisotrkpt < maxPt ) 
      && (    maxIso == -1 || isoTrkIso/perisotrkpt < maxIso )
      && (    maxMtw == -1 || isoTrkMtw < maxMtw );
  }

  int countIsoTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

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

  int countIsoLepTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

    int cntNIsoTrks = 0;
    for(unsigned int is=0; is<isoTrksLVec.size(); is++){
      if( std::abs(isoTrkspdgId[is]) == 11 || std::abs(isoTrkspdgId[is]) == 13 ){
        if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoLepTrksArr ) ) cntNIsoTrks ++;
      }
    }
    return cntNIsoTrks;
  }

  int countIsoPionTrks(const std::vector<TLorentzVector> &isoTrksLVec, const std::vector<float> &isoTrksIso, const std::vector<float> &isoTrksMtw, const std::vector<int> &isoTrkspdgId){

    int cntNIsoTrks = 0;
    for(unsigned int is=0; is<isoTrksLVec.size(); is++){
      if( std::abs(isoTrkspdgId[is]) == 211 ){
        if( passIsoTrk(isoTrksLVec[is], isoTrksIso[is], isoTrksMtw[is], AnaConsts::isoHadTrksArr ) ) cntNIsoTrks ++;
      }
    }
    return cntNIsoTrks;
  }

  void prepareJetsForTagger(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<float> &inirecoJetsBtag, std::vector<TLorentzVector> &jetsLVec_forTagger, std::vector<float> &recoJetsBtag_forTagger, const std::vector<float>& qgLikelihood, std::vector<float>& qgLikelihood_forTagger){

    jetsLVec_forTagger.clear(); recoJetsBtag_forTagger.clear();

    for(unsigned int ij=0; ij<inijetsLVec.size(); ij++)
    {
      if( !jetPassCuts(inijetsLVec[ij], AnaConsts::pt30Arr) ) continue;

      jetsLVec_forTagger.push_back(inijetsLVec.at(ij));
      recoJetsBtag_forTagger.push_back(inirecoJetsBtag.at(ij));
      if(&qgLikelihood && &qgLikelihood_forTagger) qgLikelihood_forTagger.push_back(qgLikelihood[ij]);
    }
  }
  // cntNJetsVec stores number of jets counters from tightest requirement to loosest requirement.
  // cutCSVS is the CSV cut value
  void preparecntNJets(const std::vector<TLorentzVector> &inijetsLVec, const std::vector<float> &inirecoJetsBtag, const float cutCSVS, std::vector<int> &cntNJetsVec, std::vector<int> &cntNbJetsVec){
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

  void preparecalcDPhi(const std::vector<TLorentzVector> &inijetsLVec, const TLorentzVector &metLVec, std::vector<float> &outDPhiVec){
    outDPhiVec.clear();
    outDPhiVec = calcDPhi(inijetsLVec, metLVec, 5, AnaConsts::pt20Eta47Arr);
  }

  void prepareForNtupleReader(){
    using namespace AnaConsts;
    activatedBranchNames.insert(activatedBranchNames_DataOnly.begin(), activatedBranchNames_DataOnly.end());
  }

  float calcHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){

    const float minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;

    float ht = 0;
    for(unsigned int i=0; i<inputJets.size(); i++){
      float perjetpt = inputJets[i].Pt(), perjeteta = inputJets[i].Eta();
      if(    ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
          && (     minPt == -1 || perjetpt >= minPt )
          && (     maxPt == -1 || perjetpt < maxPt ) )
      {
        ht += perjetpt;
      }
    }
    return ht;
  }

  TLorentzVector calcMHT(const std::vector<TLorentzVector> &inputJets, const AnaConsts::AccRec& jetCutsArr){

    const float minAbsEta = jetCutsArr.minAbsEta, maxAbsEta = jetCutsArr.maxAbsEta, minPt = jetCutsArr.minPt, maxPt = jetCutsArr.maxPt;

    TLorentzVector mhtLVec;
    for(unsigned int i=0; i<inputJets.size(); i++){
      float perjetpt = inputJets[i].Pt(), perjeteta = inputJets[i].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
          && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
          && (     minPt == -1 || perjetpt >= minPt )
          && (     maxPt == -1 || perjetpt < maxPt ) ){

        TLorentzVector tmpLVec;
        tmpLVec.SetPtEtaPhiM( inputJets[i].Pt(), 0, inputJets[i].Phi(), 0 );
        mhtLVec -= tmpLVec;
      }
    }

    return mhtLVec;
  }

  bool passBaseline(){

    return true;
  }

  // match dR between jet and object (photon, lepton, etc)
  int jetObjectdRMatch(const TLorentzVector& object, const std::vector<TLorentzVector>& jetsLVec, const float jetObjectdRMax, std::vector<float>* dRvec)
  {
    float dRmin = 999.0;
    int minJMatch = -1;

    for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
    {
      float dR = ROOT::Math::VectorUtil::DeltaR(jetsLVec[iJet], object);
      if (dRvec) dRvec->push_back(dR);
      if(dR < dRmin)
      {
        dRmin = dR;
        minJMatch = iJet;
      }
    }
    if(dRmin < jetObjectdRMax) return minJMatch;
    else                       return -1;
  }
  
  // true if jet matches object, false otherwise
  // version using Object_JetIndex and dR < DRMAX
  std::vector<bool> getJetMatchesObjectVec(const std::vector<TLorentzVector>& Jet_TLV, const std::vector<TLorentzVector>& Object_TLV, const std::vector<int>& Object_JetIndex, const float& DRMAX)
  {
    int nJets    = Jet_TLV.size();
    int nObjects = Object_TLV.size();
    std::vector<bool> Jet_MatchesObject(nJets, false);
    if (Object_JetIndex.size() != nObjects)
    {
      std::cout << "EROR in " << __func__ << ": Object vectors do not have the same size." << std::endl;
      return Jet_MatchesObject;
    }
    for (int i = 0; i < nObjects; ++i)
    {
      int matchingJetIndex = Object_JetIndex[i];
      // matchingJetIndex = -1: not matched to jet; also, matchingJetIndex should be less than nJets
      if (matchingJetIndex > -1 && matchingJetIndex < nJets)
      {
        float dR = ROOT::Math::VectorUtil::DeltaR(Object_TLV[i], Jet_TLV[matchingJetIndex]);
        if (dR < DRMAX)
        {
          Jet_MatchesObject[matchingJetIndex] = true;
        }
      }
    }
    return Jet_MatchesObject;
  }
  
  // true if jet matches object, false otherwise
  // version using dR < DRMAX only
  std::vector<bool> getJetMatchesObjectVec(const std::vector<TLorentzVector>& Jet_TLV, const std::vector<TLorentzVector>& Object_TLV, const float& DRMAX)
  {
    int nJets    = Jet_TLV.size();
    int nObjects = Object_TLV.size();
    std::vector<bool> Jet_MatchesObject(nJets, false);
    for (int jet_i = 0; jet_i < nJets; ++jet_i)
    {
      for (int obj_i = 0; obj_i < nObjects; ++obj_i)
      {
        float dR = ROOT::Math::VectorUtil::DeltaR(Object_TLV[obj_i], Jet_TLV[jet_i]);
        if (dR < DRMAX)
        {
          Jet_MatchesObject[jet_i] = true;
          break; 
        }
      }
    }
    return Jet_MatchesObject;
  }

}


