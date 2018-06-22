#include "baselineDef.h"

#include "TFile.h"
#include "TF1.h"

#include "lester_mt2_bisect.h"

//**************************************************************************//
//                              BaselineVessel                              //
//**************************************************************************//

BaselineVessel::BaselineVessel(NTupleReader &tr_, const std::string specialization, const std::string filterString) : 
  tr(&tr_), spec(specialization), 
  //type3Ptr(NULL),
  ttPtr(NULL),
  WMassCorFile(NULL)
{
  bToFake               = 1;
  debug                 = false;
  incZEROtop            = false;
  UseLepCleanJet        = false;
  jetVecLabel           = "jetsLVec";
  CSVVecLabel           = "recoJetsCSVv2";
  METLabel              = "met";
  METPhiLabel           = "metphi";
  jetVecLabelAK8        = "ak8JetsLVec";
  qgLikehoodLabel       = "qgLikelihood";
  muonsFlagIDLabel      = "muonsFlagMedium";  //TODO: moving to loose ID for 2017
  elesFlagIDLabel       = "elesFlagVeto";
  toptaggerCfgFile      = "TopTagger.cfg";
  doIsoTrksVeto         = true;
  doMuonVeto            = true;
  doEleVeto             = true;
  doMET                 = true;
  dodPhis               = true;
  passBaseline          = true;
  passBaselineNoTagMT2  = true;
  passBaselineNoTag     = true;
  passBaselineNoLepVeto = true;
  metLVec.SetPtEtaPhiM(0, 0, 0, 0);

  if(filterString.compare("fastsim") ==0) isfastsim = true; else isfastsim = false; 

  //Check if simplified tagger is called for
  std::string taggerLabel = "";
  const std::string aggBinLabel = "AggregatedBins";
  size_t loc = spec.find(aggBinLabel);
  if(loc != std::string::npos)
  {
    toptaggerCfgFile = "TopTagger_Simplified.cfg";
    taggerLabel = "AggBins";
    //Remove aggBinLabel from spec
    spec.erase(loc, aggBinLabel.size());
    //Strip any white space ledt in spec
    spec.erase(spec.begin(), std::find_if(spec.begin(), spec.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    spec.erase(std::find_if(spec.rbegin(), spec.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), spec.end());
  }

  if( !spec.empty() ){
    TString stripT = spec;
    TObjArray * objArr = stripT.Tokenize(" ");
    TObjString* firstObj = dynamic_cast<TObjString*>(objArr->At(0));
    firstSpec = firstObj->GetString().Data();
    std::cout<<"\nfirstSpec : "<<firstSpec.c_str()<<"  spec : "<<spec.c_str()<<"  isfastsim : "<<isfastsim<<std::endl<<std::endl;
  }
  firstSpec += taggerLabel;

  printOnce = false;

  PredefineSpec();

  SetupTopTagger(toptaggerCfgFile);
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::UseLepCleanJets
//  Description:  By default no Lep clean in Jets. Call this function to
//  switch input labels
// ===========================================================================
bool BaselineVessel::UseLepCleanJets() 
{
  UseLepCleanJet        = true;
  jetVecLabel           = "prodJetsNoLep_jetsLVec";
  CSVVecLabel           = "prodJetsNoLep_recoJetsCSVv2";
  qgLikehoodLabel       = "prodJetsNoLep_qgLikelihood";
  return true;
}       // -----  end of function BaselineVessel::UseLepCleanJets  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::OpenWMassCorrFile
//  Description:  
// ===========================================================================
bool BaselineVessel::OpenWMassCorrFile()
{
  std::string puppiCorr = "puppiCorr.root";
  WMassCorFile = TFile::Open(puppiCorr.c_str(),"READ");
  if (!WMassCorFile)
    std::cout << "W mass correction file not found w mass!!!!!!! " << puppiCorr <<" Will not correct W mass" << std::endl;
  else{
    puppisd_corrGEN      = (TF1*)WMassCorFile->Get("puppiJECcorr_gen");
    puppisd_corrRECO_cen = (TF1*)WMassCorFile->Get("puppiJECcorr_reco_0eta1v3");
    puppisd_corrRECO_for = (TF1*)WMassCorFile->Get("puppiJECcorr_reco_1v3eta2v5");
  }
  return true;
}       // -----  end of function BaselineVessel::OpenWMassCorrFile  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::SetupTopTagger
//  Description:  
// ===========================================================================
bool BaselineVessel::SetupTopTagger(std::string CfgFile_)
{
  toptaggerCfgFile = CfgFile_;

  ttPtr.reset(new TopTagger);
  ttPtr->setCfgFile(toptaggerCfgFile);
  OpenWMassCorrFile();
  
  return true;
}       // -----  end of function BaselineVessel::SetupTopTagger  -----

void BaselineVessel::prepareTopTagger()
{
// Prepare jets and b-tag working points for top tagger
  jetsLVec_forTagger     = new std::vector<TLorentzVector>();
  recoJetsBtag_forTagger = new std::vector<float>();
  qgLikelihood_forTagger = new std::vector<float>();
  std::vector<float> qgLikelihood;
  try
  {
    qgLikelihood = tr->getVec<float>(qgLikehoodLabel);
  }
  catch (const SATException& e)
  {
    e.print();
    qgLikelihood.clear();
  }
    
  AnaFunctions::prepareJetsForTagger(tr->getVec<TLorentzVector>(jetVecLabel), tr->getVec<float>(CSVVecLabel), 
      *jetsLVec_forTagger, *recoJetsBtag_forTagger, qgLikelihood, *qgLikelihood_forTagger);

  tr->registerDerivedVec("jetsLVec_forTagger" + firstSpec, jetsLVec_forTagger);
  tr->registerDerivedVec("recoJetsBtag_forTagger" + firstSpec, recoJetsBtag_forTagger);
  tr->registerDerivedVec("qgLikelihood_forTagger" + firstSpec, qgLikelihood_forTagger);

  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ New TopTagger ~~~~~
    // top tagger
    //construct vector of constituents 
    ttUtility::ConstAK4Inputs<float> myConstAK4Inputs(*jetsLVec_forTagger, *recoJetsBtag_forTagger, *qgLikelihood_forTagger);
    ttUtility::ConstAK8Inputs<float> myConstAK8Inputs(
        tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiJetsLVec" : "puppiJetsLVec"), 
        tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau1" : "puppitau1"),
        tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau2" : "puppitau2"),
        tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau3" : "puppitau3"),
        tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppisoftDropMass" : "puppisoftDropMass"),
        tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiSubJetsLVec" : "puppiSubJetsLVec"));
    if (WMassCorFile != NULL)
    {
      myConstAK8Inputs.setWMassCorrHistos (puppisd_corrGEN     , puppisd_corrRECO_cen, puppisd_corrRECO_for);
    }
    std::vector<Constituent> constituents = ttUtility::packageConstituents(myConstAK4Inputs, myConstAK8Inputs);
    //run tagger
    ttPtr->runTagger(constituents);
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::~BaselineVessel
//  Description:  
// ===========================================================================
BaselineVessel::~BaselineVessel() 
{
}       // -----  end of function BaselineVessel::~BaselineVessel  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::PredefineSpec
//  Description:  
// ===========================================================================
bool BaselineVessel::PredefineSpec()
{

  if( spec.compare("noIsoTrksVeto") == 0)
  {
    doIsoTrksVeto = false;
  }
  else if( spec.compare("incZEROtop") == 0)
  {
    incZEROtop = true;
  }
  else if( spec.compare("hadtau") == 0)
  {
    doMuonVeto = false;
    doIsoTrksVeto = false;
    METLabel = "met_hadtau";
    METPhiLabel = "metphi_hadtau";
    jetVecLabel = "jetsLVec_hadtau";
    CSVVecLabel = "recoJetsBtag_0_hadtau";
  }
  else if( spec.compare("lostlept") == 0)
  {
    doMuonVeto = false;
    doEleVeto = false; 
    doIsoTrksVeto = false;
  }
  else if(spec.compare("Zinv") == 0 || spec.compare("Zinv1b") == 0 || spec.compare("Zinv2b") == 0 || spec.compare("Zinv3b") == 0 || spec.compare("ZinvJEUUp") == 0 || spec.compare("ZinvJEUDn") == 0 || spec.compare("ZinvMEUUp") == 0 || spec.compare("ZinvMEUDn") == 0) 
  {
    jetVecLabel = "prodJetsNoLep_jetsLVec";
    CSVVecLabel = "prodJetsNoLep_recoJetsCSVv2";
    METLabel    = "cleanMetPt";
    METPhiLabel = "cleanMetPhi";
    doMuonVeto  = false;
    doEleVeto   = false;
    doIsoTrksVeto = false;
    if(spec.compare("Zinv1b") == 0)
    {
      CSVVecLabel = "cleanJetpt30ArrBTag1fake";
      bToFake = 1;
    }
    else if(spec.compare("Zinv2b") == 0)
    {
      CSVVecLabel = "cleanJetpt30ArrBTag2fake";
      bToFake = 1; // This is not a typo
    }
    else if(spec.compare("Zinv3b") == 0)
    {
      CSVVecLabel = "cleanJetpt30ArrBTag3fake";
      bToFake = 1; // This is not a typo
    }
    else if(spec.compare("ZinvJEUUp") == 0)
    {
      jetVecLabel = "jetLVecUp";
    }
    else if(spec.compare("ZinvJEUDn") == 0)
    {
      jetVecLabel = "jetLVecDn";
    }
    else if(spec.compare("ZinvMEUUp") == 0)
    {
      METLabel    = "metMEUUp";
    }
    else if(spec.compare("ZinvMEUDn") == 0)
    {
      METLabel    = "metMEUDn";
    }
  }
  else if(spec.compare("QCD") == 0)
  {
    doMET = false;
    dodPhis = false;
  }else if( spec.find("jecUp") != std::string::npos || spec.find("jecDn") != std::string::npos || spec.find("metMagUp") != std::string::npos || spec.find("metMagDn") != std::string::npos || spec.find("metPhiUp") != std::string::npos || spec.find("metPhiDn") != std::string::npos ){
    if( spec.find("jecUp") != std::string::npos ){
      jetVecLabel = "jetLVec_jecUp";
      CSVVecLabel = "recoJetsBtag_jecUp";
    }else if(spec.find("jecDn") != std::string::npos ){
      jetVecLabel = "jetLVec_jecDn";
      CSVVecLabel = "recoJetsBtag_jecDn";
    }else if(spec.find("metMagUp") != std::string::npos ){
      METLabel = "met_metMagUp";
    }else if(spec.find("metMagDn") != std::string::npos ){
      METLabel = "met_metMagDn";
    }else if(spec.find("metPhiUp") != std::string::npos ){
      METPhiLabel = "metphi_metPhiUp";
    }else if(spec.find("metPhiDn") != std::string::npos ){
      METPhiLabel = "metphi_metPhiDn";
    }
    if( spec.find("usegenmet") != std::string::npos ){
      METLabel = "genmet";
      METPhiLabel = "genmetphi";
    } 
  }else if( spec.compare("usegenmet") == 0 ){
    METLabel = "genmet";
    METPhiLabel = "genmetphi";
  }

  if( !printOnce ){
    printOnce = true;
    std::cout<<"spec : "<<spec.c_str()<<"  jetVecLabel : "<<jetVecLabel.c_str() <<"  CSVVecLabel : "<<CSVVecLabel.c_str() <<"  METLabel : "<<METLabel.c_str()<< std::endl;
  }  
  

  return true;
}       // -----  end of function BaselineVessel::PredefineSpec  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::PassTopTagger
//  Description:  
// ===========================================================================
bool BaselineVessel::PassTopTagger()
{
  int nTopCandSortedCnt = -1;
  bool passTagger = false;
  vTops = new std::vector<TLorentzVector>();
  mTopJets = new std::map<int, std::vector<TLorentzVector> >();

  nTopCandSortedCnt = GetnTops();
  passTagger = (incZEROtop || nTopCandSortedCnt >= AnaConsts::low_nTopCandSortedSel); 

  tr->registerDerivedVar("nTopCandSortedCnt" + firstSpec, nTopCandSortedCnt);
  tr->registerDerivedVec("vTops"+firstSpec, vTops);
  tr->registerDerivedVec("mTopJets"+firstSpec, mTopJets);

  return passTagger;
}       // -----  end of function BaselineVessel::PassTopTagger  -----

void BaselineVessel::PassBaseline()
{
  // Initial value
  passBaseline          = true;
  passBaselineNoTagMT2  = true;
  passBaselineNoTag     = true;
  passBaselineNoLepVeto = true;


  // Form TLorentzVector of MET
  if (tr->getVar<unsigned int>("run") > 312526) // Set weight to 1 for Data
    metLVec.SetPtEtaPhiM(tr->getVar<float>(METLabel), 0, tr->getVar<float>(METPhiLabel), 0);
  else
    metLVec.SetPtEtaPhiM(tr->getVar<float>(METLabel), 0, tr->getVar<float>(METPhiLabel), 0);

  // Calculate number of leptons
  const std::vector<int> & muonsFlagIDVec = muonsFlagIDLabel.empty()? std::vector<int>(tr->getVec<float>("muonsMiniIso").size(), 1):tr->getVec<int>(muonsFlagIDLabel.c_str()); // We have muonsFlagTight as well, but currently use medium ID
  const std::vector<int> & elesFlagIDVec = elesFlagIDLabel.empty()? std::vector<int>(tr->getVec<float>("elesMiniIso").size(), 1):tr->getVec<int>(elesFlagIDLabel.c_str()); // Fake electrons since we don't have different ID for electrons now, but maybe later
  int nMuons = AnaFunctions::countMuons(tr->getVec<TLorentzVector>("muonsLVec"), tr->getVec<float>("muonsMiniIso"), tr->getVec<float>("muonsMtw"), muonsFlagIDVec, AnaConsts::muonsMiniIsoArr);
  int nElectrons = AnaFunctions::countElectrons(tr->getVec<TLorentzVector>("elesLVec"), tr->getVec<float>("elesMiniIso"), tr->getVec<float>("elesMtw"), tr->getVec<unsigned int>("elesisEB"), elesFlagIDVec, AnaConsts::elesMiniIsoArr);
  int nIsoTrks = AnaFunctions::countIsoTrks(tr->getVec<TLorentzVector>("Tauloose_isoTrksLVec"), tr->getVec<float>("loose_isoTrks_iso"), tr->getVec<float>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));
  int nIsoLepTrks = AnaFunctions::countIsoLepTrks(tr->getVec<TLorentzVector>("Tauloose_isoTrksLVec"), tr->getVec<float>("loose_isoTrks_iso"), tr->getVec<float>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));
  int nIsoPionTrks = AnaFunctions::countIsoPionTrks(tr->getVec<TLorentzVector>("Tauloose_isoTrksLVec"), tr->getVec<float>("loose_isoTrks_iso"), tr->getVec<float>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));

  // Calculate number of jets and b-tagged jets
  int cntCSVS = AnaFunctions::countCSVS(tr->getVec<TLorentzVector>(jetVecLabel), tr->getVec<float>(CSVVecLabel), AnaConsts::cutCSVS, AnaConsts::bTagArr);
  int cntNJetsPt50Eta24 = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt50Eta24Arr);
  int cntNJetsPt30Eta24 = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Eta24Arr);
  int cntNJetsPt20Eta24 = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt20Eta24Arr);
  int cntNJetsPt30      = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Arr);

  // Calculate deltaPhi
  std::vector<float> * dPhiVec = new std::vector<float>();
  (*dPhiVec) = AnaFunctions::calcDPhi(tr->getVec<TLorentzVector>(jetVecLabel), metLVec.Phi(), 3, AnaConsts::dphiArr);

  // Pass lepton veto?
  bool passMuonVeto = (nMuons == AnaConsts::nMuonsSel), passEleVeto = (nElectrons == AnaConsts::nElectronsSel), passIsoTrkVeto = (nIsoTrks == AnaConsts::nIsoTrksSel);
  bool passIsoLepTrkVeto = (nIsoLepTrks == AnaConsts::nIsoTrksSel), passIsoPionTrkVeto = (nIsoPionTrks == AnaConsts::nIsoTrksSel);
  bool passLeptVeto = passMuonVeto && passEleVeto && passIsoTrkVeto;
  if( doMuonVeto && !passMuonVeto ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; }
  if( doEleVeto && !passEleVeto ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; }
  // Isolated track veto is disabled for now
  if( doIsoTrksVeto && !passIsoTrkVeto ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; }

  if( debug ) std::cout<<"nMuons : "<<nMuons<<"  nElectrons : "<<nElectrons<<"  nIsoTrks : "<<nIsoTrks<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass number of jets?
  bool passnJets = true;
  //if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passnJets = false; passBaselineNoLepVeto = false; }
  //if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passnJets = false; passBaselineNoLepVeto = false; }
  if( cntNJetsPt20Eta24 < AnaConsts::nJetsSelPt20Eta24 ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passnJets = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"cntNJetsPt50Eta24 : "<<cntNJetsPt50Eta24<<"  cntNJetsPt30Eta24 : "<<cntNJetsPt30Eta24<<"  cntNJetsPt30 : "<<cntNJetsPt30<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass deltaPhi?
  bool passdPhis = (dPhiVec->at(0) >= AnaConsts::dPhi0_CUT && dPhiVec->at(1) >= AnaConsts::dPhi1_CUT && dPhiVec->at(2) >= AnaConsts::dPhi2_CUT);
  if( dodPhis && !passdPhis ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"dPhi0 : "<<dPhiVec->at(0)<<"  dPhi1 : "<<dPhiVec->at(1)<<"  dPhi2 : "<<dPhiVec->at(2)<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass number of b-tagged jets? 
  // No b-tag in baseline
  bool passBJets = true;
  //if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) ){ passBaseline = false; passBJets = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"cntCSVS : "<<cntCSVS<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass the baseline MET requirement?
  bool passMET = (metLVec.Pt() >= AnaConsts::defaultMETcut);
  if( doMET && !passMET ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"met : "<<tr->getVar<float>("met")<<"  defaultMETcut : "<<AnaConsts::defaultMETcut<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass the HT cut for trigger?
  float HT = AnaFunctions::calcHT(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt20Eta24Arr);
  bool passHT = true;
  if( HT < AnaConsts::defaultHTcut ){ passHT = false; passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"HT : "<<HT<<"  defaultHTcut : "<<AnaConsts::defaultHTcut<<"  passHT : "<<passHT<<"  passBaseline : "<<passBaseline<<std::endl;

  // Calculate top tagger related variables. 
  // Note that to save speed, only do the calculation after previous base line requirements.

  prepareTopTagger();
  bool passTagger = PassTopTagger();
  //if( !passTagger ){ passBaseline = false; passBaselineNoLepVeto = false; }

  // Pass the baseline MT2 requirement?
  bool passMT2 = true;
  float MT2 = CalcMT2();
  //if( MT2 < AnaConsts::defaultMT2cut ){ passBaseline = false; passBaselineNoTag = false; passMT2 = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"MT2 : "<<MT2 <<"  defaultMT2cut : "<<AnaConsts::defaultMT2cut<<"  passBaseline : "<<passBaseline<<std::endl;

  bool passNoiseEventFilter = true;
  if( !passNoiseEventFilterFunc() ) { passNoiseEventFilter = false; passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"passNoiseEventFilterFunc : "<<passNoiseEventFilterFunc()<<"  passBaseline : "<<passBaseline<<std::endl;

  // pass QCD high MET filter
  bool passQCDHighMETFilter = true;
  if( !passQCDHighMETFilterFunc() ) { passQCDHighMETFilter = false; }
  if( debug ) std::cout<<"passQCDHighMETFilter : "<< passQCDHighMETFilter <<"  passBaseline : "<<passBaseline<<std::endl;

  // pass the special filter for fastsim
  bool passFastsimEventFilter = true;
  if( !passFastsimEventFilterFunc() ) { passFastsimEventFilter = false; passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"passFastsimEventFilterFunc : "<<passFastsimEventFilterFunc()<<"  passBaseline : "<<passBaseline<<std::endl;

  // Register all the calculated variables
  //tr->registerDerivedVar("nMuons_CUT" + firstSpec, nMuons);           // error: do not redefine  
  //tr->registerDerivedVar("nElectrons_CUT" + firstSpec, nElectrons);   // error: do not redefine
  //tr->registerDerivedVar("nIsoTrks_CUT" + firstSpec, nIsoTrks);       // error: do not redefine

  tr->registerDerivedVar("cntNJetsPt50Eta24" + firstSpec, cntNJetsPt50Eta24);
  tr->registerDerivedVar("cntNJetsPt30Eta24" + firstSpec, cntNJetsPt30Eta24);
  tr->registerDerivedVar("cntNJetsPt20Eta24" + firstSpec, cntNJetsPt20Eta24);

  tr->registerDerivedVec("dPhiVec" + firstSpec, dPhiVec);

  tr->registerDerivedVar("cntCSVS" + firstSpec, cntCSVS);


  tr->registerDerivedVar("cntNJetsPt30" + firstSpec, cntNJetsPt30);

  tr->registerDerivedVar("passLeptVeto" + firstSpec, passLeptVeto);
  tr->registerDerivedVar("passMuonVeto" + firstSpec, passMuonVeto);
  tr->registerDerivedVar("passEleVeto" + firstSpec, passEleVeto);
  tr->registerDerivedVar("passIsoTrkVeto" + firstSpec, passIsoTrkVeto);
  tr->registerDerivedVar("passIsoLepTrkVeto" + firstSpec, passIsoLepTrkVeto);
  tr->registerDerivedVar("passIsoPionTrkVeto" + firstSpec, passIsoPionTrkVeto);
  tr->registerDerivedVar("passnJets" + firstSpec, passnJets);
  tr->registerDerivedVar("passdPhis" + firstSpec, passdPhis);
  tr->registerDerivedVar("passBJets" + firstSpec, passBJets);
  tr->registerDerivedVar("passMET" + firstSpec, passMET);
  tr->registerDerivedVar("passMT2" + firstSpec, passMT2);
  tr->registerDerivedVar("passHT" + firstSpec, passHT);
  tr->registerDerivedVar("passTagger" + firstSpec, passTagger);
  tr->registerDerivedVar("passNoiseEventFilter" + firstSpec, passNoiseEventFilter);
  tr->registerDerivedVar("passQCDHighMETFilter" + firstSpec, passQCDHighMETFilter);
  tr->registerDerivedVar("passFastsimEventFilter" + firstSpec, passFastsimEventFilter);
  tr->registerDerivedVar("passBaseline" + firstSpec, passBaseline);
  tr->registerDerivedVar("passBaselineNoTagMT2" + firstSpec, passBaselineNoTagMT2);
  tr->registerDerivedVar("passBaselineNoTag" + firstSpec, passBaselineNoTag);
  tr->registerDerivedVar("passBaselineNoLepVeto" + firstSpec, passBaselineNoLepVeto);

  tr->registerDerivedVar("best_had_brJet_MT2" + firstSpec,    MT2);

  tr->registerDerivedVar("HT" + firstSpec, HT);

  if( debug ) std::cout<<"passBaseline : "<<passBaseline<<"  passBaseline : "<<passBaseline<<std::endl;
} 

int BaselineVessel::GetnTops() const
{
  int nTops = 0;

  //get output of tagger
  const TopTaggerResults& ttr = ttPtr->getResults();
  //Use result for top var
  std::vector<TopObject*> Ntop = ttr.getTops();  
  nTops = Ntop.size();
  for(int it=0; it<nTops; it++)
  {
      vTops->push_back(Ntop.at(it)->P());
      std::vector<TLorentzVector> temp;
      for(auto j : Ntop.at(it)->getConstituents())
      {
          temp.push_back(j->P());
      }
      mTopJets->insert(std::make_pair(it, temp));
  }

  return nTops;
}       // -----  end of function VarPerEvent::GetnTops  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::FlagAK8Jets
//  Description:  Provide a flag for each AK8 jets
//  Frist digit: Notag, top tag, W in top, W alone
//  Second digit: Nobjet, Has Loose b-jet, Has medium bjet
// ===========================================================================
bool BaselineVessel::FlagAK8Jets()
{
  // AK8 + Ak4 for W + jet
  ttUtility::ConstAK8Inputs<float> myConstAK8Inputs(
      tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiJetsLVec" : "puppiJetsLVec"), 
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau1" : "puppitau1"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau2" : "puppitau2"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau3" : "puppitau3"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppisoftDropMass" : "puppisoftDropMass"),
      tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiSubJetsLVec" : "puppiSubJetsLVec"));
  if (WMassCorFile != NULL)
  {
    myConstAK8Inputs.setWMassCorrHistos (puppisd_corrGEN     , puppisd_corrRECO_cen, puppisd_corrRECO_for);
  }
  std::vector<Constituent> AK8constituents;
  myConstAK8Inputs.packageConstituents(AK8constituents);

  vAK8Flag = new std::vector<unsigned>();

  for(auto ak8_ : AK8constituents)
  {
    unsigned flag = FlagAK8FromTagger(ak8_);
    if (flag == NoTag)
    {
      flag = FlagAK8FromCSV(ak8_);
    }
    vAK8Flag->push_back(flag);
  }

  tr->registerDerivedVec("vAK8Flag"+spec, vAK8Flag);

  GetWAlone();
  GetISRJet();
  return true;
}       // -----  end of function BaselineVessel::FlagAK8Jets  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::FlagAK8FromCSV
//  Description:  /* cursor */
// ===========================================================================
AK8Flag BaselineVessel::FlagAK8FromCSV(Constituent &ak8) const
{
  unsigned loosebcnt =0 ;
  unsigned mediumbcnt = 0;
  const std::vector<TLorentzVector> &jets = tr->getVec<TLorentzVector>(jetVecLabel);
  const std::vector<float> &CSV = tr->getVec<float>(CSVVecLabel);

  for(auto sub : ak8.getSubjets())
  {
    for(unsigned int ij=0; ij<jets.size(); ij++)
    {
      if (sub.p().DeltaR(jets.at(ij)) < 0.4)
      {
        if (jets.at(ij).Pt() < 20 || fabs(jets.at(ij).Eta()) > 2.4) continue;
        if (CSV.at(ij) > AnaConsts::cutCSVS ) mediumbcnt ++;
        if (CSV.at(ij) > AnaConsts::cutCSVL ) loosebcnt ++;
      }
    }
    
  }

  if (mediumbcnt > 0 ) return NoTagMediumb;
  if (loosebcnt > 0 ) return NoTagLooseb;
  return NoTagNob;
}       // -----  end of function BaselineVessel::FlagAK8FromCSV  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::FlagAK8FromTagger
//  Description:  
// ===========================================================================
AK8Flag BaselineVessel::FlagAK8FromTagger(Constituent &ak8 )
{

  for(auto t : *mTopJets)
  {
    // Find the mono top jet from tagger
    if (t.second.size() == 1)
    {
      if (t.second.at(0).Pt() == ak8.P().Pt()
          && t.second.at(0).Eta() == ak8.P().Eta()
          && t.second.at(0).Phi() == ak8.P().Phi())
      {
        return TopTag;
      }
    }
    // Find the W jet from tagger
    if (t.second.size() == 2)
    {
      for(auto w : t.second)
      {
        if (w.Pt() == ak8.P().Pt()
            && w.Eta() == ak8.P().Eta()
            && w.Phi() == ak8.P().Phi())
        {
          return WinTopTag;
        }
      }
    }
    // Find the W jet from 3jet tagger
    if (t.second.size() == 3)
    {
      for(auto tri : t.second)
      {
        for(auto sub : ak8.getSubjets())
        {
          if (tri.DeltaR(sub.p())<0.4)
          {
            return WinTopTag;
          }
        }
      }
    }
  }

  // Looking for stand alone W tagger
  float corrSDMass = ak8.getSoftDropMass() * ak8.getWMassCorr();
  float tau21 = ak8.getTau2()/ak8.getTau1();
  if ( corrSDMass > 65 && corrSDMass < 100 &&
      tau21 < 0.6 && ak8.p().Pt() > 200)
  {
    return WAloneTag;
  }

  return NoTag;
}       // -----  end of function BaselineVessel::FlagAK8FromTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetWAlone
//  Description:  
// ===========================================================================
bool BaselineVessel::GetWAlone() const
{
  const std::vector<TLorentzVector> &AK8 = tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiJetsLVec" : "puppiJetsLVec");
  std::vector<TLorentzVector> *WAlone= new std::vector<TLorentzVector>();
  for(unsigned int i=0; i < vAK8Flag->size(); ++i)
  {
    if (vAK8Flag->at(i) == WAloneTag)
    {
      WAlone->push_back(AK8.at(i));
    }
  }
  tr->registerDerivedVec("vWAlone"+spec, WAlone);
  return true;
}       // -----  end of function BaselineVessel::GetWAlone  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetISRJet
//  Description:  Following the ISR defition from SUS-16-049
//  The hardest of large-R jets with pT > 200GeV
//  Failed loose b-tagging, nor top/W tagging
// ===========================================================================
bool BaselineVessel::GetISRJet() const
{
  const std::vector<TLorentzVector> &AK8 = tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiJetsLVec" : "puppiJetsLVec");
  std::vector<TLorentzVector> *ISRJet = new std::vector<TLorentzVector>();
  std::map<float, TLorentzVector> ISRJetAll;

  for(unsigned int i=0; i < vAK8Flag->size(); ++i)
  {
    if (vAK8Flag->at(i) == NoTagNob)
    {
      float pt = AK8.at(i).Pt();
      if (pt > 200)
        ISRJetAll[pt] = AK8.at(i);
    }
  }
  if (!ISRJetAll.empty())
  {
    ISRJet->push_back(ISRJetAll.rbegin()->second);
  }

  tr->registerDerivedVec("vISRJet"+spec, ISRJet);

  return true;
}       // -----  end of function BaselineVessel::GetISRJet  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetTopCombs
//  Description:  
// ===========================================================================
bool BaselineVessel::GetTopCombs() const
{
  std::vector<TLorentzVector> *vCombs = new std::vector<TLorentzVector>();
  std::map<int, std::vector<TLorentzVector> > *vCombJets = new std::map<int, std::vector<TLorentzVector> >();

  //get output of tagger
  //Only MVA combs so far
  const TopTaggerResults& ttr = ttPtr->getResults();
  int i = 0;
  for(auto tr : ttr.getTopCandidates() )
  {
      if (tr.getNConstituents() != 3) continue;
      vCombs->push_back(tr.P());
      std::vector<TLorentzVector> temp;
      for(auto cons : tr.getConstituents())
      {
          temp.push_back(cons->P());
      }
      vCombJets->insert(std::make_pair(i, temp));
      i++;
  }

  // AK8 + Ak4 for W + jet
  ttUtility::ConstAK8Inputs<float> myConstAK8Inputs(
      tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiJetsLVec" : "puppiJetsLVec"), 
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau1" : "puppitau1"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau2" : "puppitau2"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppitau3" : "puppitau3"),
      tr->getVec<float>(UseLepCleanJet ? "prodJetsNoLep_puppisoftDropMass" : "puppisoftDropMass"),
      tr->getVec<TLorentzVector>(UseLepCleanJet ? "prodJetsNoLep_puppiSubJetsLVec" : "puppiSubJetsLVec"));
  std::vector<Constituent> AK8constituents;
  myConstAK8Inputs.packageConstituents(AK8constituents);

  for(auto ak8_ : AK8constituents)
  {
      auto ak8 = ak8_.P();
      if (ak8.Pt() < 200 ) continue;
      if (ak8.M() < 65 ) continue;
      for(auto ak4 : GetAK4NoSubjet(ak8_, *jetsLVec_forTagger))
      { 
          if (ak4.Pt() < 30 ) continue; // Tight working point
          TLorentzVector sumTop = ak4 + ak8;
          if (sumTop.M() < 100 || sumTop.M() > 250) continue;
          if (sumTop.DeltaR(ak8) > 1.0 || sumTop.DeltaR(ak4) > 1.0 ) continue; // Tight working point
          vCombs->push_back(sumTop);
          std::vector<TLorentzVector> temp;
          temp.push_back(ak8);
          temp.push_back(ak4);
          vCombJets->insert(std::make_pair(i, temp));
          i++;
      }
 
    // Ak8 only for top
    const std::vector<TLorentzVector>  & AK8 = tr->getVec<TLorentzVector>("puppiJetsLVec");
    for(auto ak8 : AK8)
    {
      if (ak8.Pt() < 400 ) continue;
      if (ak8.M() < 100 ) continue;
      vCombs->push_back(ak8);
      std::vector<TLorentzVector> temp;
      temp.push_back(ak8);
      vCombJets->insert(std::make_pair(i, temp));
      i++;
    }
  }

  tr->registerDerivedVec("vCombs"+spec, vCombs);
  tr->registerDerivedVec("mCombJets"+spec, vCombJets);

  return true;
}       // -----  end of function BaselineVessel::GetTopCombs  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetAK4NoSubjet
//  Description:  /* cursor */
// ===========================================================================
std::vector<TLorentzVector>  BaselineVessel::GetAK4NoSubjet(Constituent &ak8, std::vector<TLorentzVector> &ak4jets) const
{
  std::vector<TLorentzVector>  temp;
  for(auto ak4 : ak4jets)
  {
    bool ismatched = false;
    for(auto sub : ak8.getSubjets())
    {
      if (ak4.DeltaR(sub.p())<0.4)
      {
        ismatched = true;
        break;
      }
    }

    if (!ismatched)
    {
      temp.push_back(ak4);
    }
  }

  return temp;
}       // -----  end of function BaselineVessel::GetAK4NoSubjet  -----

bool BaselineVessel::passNoiseEventFilterFunc()
{
  // According to https://twiki.cern.ch/twiki/bin/view/CMS/SUSRecommendationsICHEP16#Filters_to_be_applied,
  // "Do not apply filters to signal monte carlo (fastsim)"
  if( isfastsim ) return true;

  try
  {
    bool cached_rethrow = tr->getReThrow();
    tr->setReThrow(false);

    bool passDataSpec = true;
    if( tr->getVar<unsigned int>("run") >= 100000 ){ // hack to know if it's data or MC...
      int goodVerticesFilter = tr->getVar<int>("goodVerticesFilter");
      // new filters
      const int & globalTightHalo2016Filter = tr->getVar<int>("globalTightHalo2016Filter");
      bool passglobalTightHalo2016Filter = (&globalTightHalo2016Filter) != nullptr? tr->getVar<int>("globalTightHalo2016Filter") !=0 : true;

      //int eeBadScFilter = tr->getVar<int>("eeBadScFilter");
      int eeBadScFilter = true;

      passDataSpec = goodVerticesFilter && eeBadScFilter && passglobalTightHalo2016Filter;
    }

    unsigned int hbheNoiseFilter = isfastsim? 1:tr->getVar<unsigned int>("HBHENoiseFilter");
    unsigned int hbheIsoNoiseFilter = isfastsim? 1:tr->getVar<unsigned int>("HBHEIsoNoiseFilter");
    int ecalTPFilter = tr->getVar<int>("EcalDeadCellTriggerPrimitiveFilter");

    int jetIDFilter = isfastsim? 1:tr->getVar<unsigned int>("AK4NoLeplooseJetID");
    // new filters
    const unsigned int & BadPFMuonFilter = tr->getVar<unsigned int>("BadPFMuonFilter");
    bool passBadPFMuonFilter = (&BadPFMuonFilter) != nullptr? tr->getVar<unsigned int>("BadPFMuonFilter") !=0 : true;

    const unsigned int & BadChargedCandidateFilter = tr->getVar<unsigned int>("BadChargedCandidateFilter");
    bool passBadChargedCandidateFilter = (&BadChargedCandidateFilter) != nullptr? tr->getVar<unsigned int>("BadChargedCandidateFilter") !=0 : true;

    bool passMETratioFilter = tr->getVar<float>("calomet")!=0 ? tr->getVar<float>("met")/tr->getVar<float>("calomet") < 5 : true;

    tr->setReThrow(cached_rethrow);
    return passDataSpec && hbheNoiseFilter && hbheIsoNoiseFilter && ecalTPFilter && jetIDFilter && passBadPFMuonFilter && passBadChargedCandidateFilter && passMETratioFilter;
  }
  catch (std::string var)
  {
    if(tr->isFirstEvent()) 
    {
      printf("NTupleReader::getTupleObj(const std::string var):  Variable not found: \"%s\"!!!\n", var.c_str());
      printf("Running with PHYS14 Config\n");
    }
  }
  return true;
}

bool BaselineVessel::passQCDHighMETFilterFunc()
{
  std::vector<TLorentzVector> jetsLVec = tr->getVec<TLorentzVector>("jetsLVec");
  std::vector<float> recoJetsmuonEnergyFraction = tr->getVec<float>("recoJetsmuonEnergyFraction");
  float metphi =0;
  if (tr->getVar<unsigned int>("run") > 312526) // Set weight to 1 for Data
  {
    float metphiv = tr->getVar<float>("metphi");
    metphi = static_cast<float>(metphiv);
  }
  else
    metphi = tr->getVar<float>("metphi");

  int nJetsLoop = recoJetsmuonEnergyFraction.size();
  std::vector<float> dPhisVec = AnaFunctions::calcDPhi( jetsLVec, metphi, nJetsLoop, AnaConsts::dphiArr);

  for(int i=0; i<nJetsLoop ; i++)
  {
    float thisrecoJetsmuonenergy = recoJetsmuonEnergyFraction.at(i)*(jetsLVec.at(i)).Pt();
    if( (recoJetsmuonEnergyFraction.at(i)>0.5) && (thisrecoJetsmuonenergy>200) && (std::abs(dPhisVec.at(i)-3.1416)<0.4) ) return false;
  }

  return true;
}

bool BaselineVessel::passFastsimEventFilterFunc()
{
  bool passFilter = true;

  if( isfastsim ){
    bool cached_rethrow = tr->getReThrow();
    tr->setReThrow(false);
    const std::vector<TLorentzVector> & genjetsLVec = tr->getVec<TLorentzVector>("genjetsLVec");
    const std::vector<TLorentzVector> & recoJetsLVec = tr->getVec<TLorentzVector>("jetsLVec");
    const std::vector<float> & recoJetschargedHadronEnergyFraction = tr->getVec<float>("recoJetschargedHadronEnergyFraction");

    if( recoJetschargedHadronEnergyFraction.size() != recoJetsLVec.size() ) std::cout<<"\nWARNING ... Non-equal recoJetschargedHadronEnergyFraction.size : "<<recoJetschargedHadronEnergyFraction.size()<<"  recoJetsLVec.size : "<<recoJetsLVec.size()<<std::endl<<std::endl;

    if( !recoJetsLVec.empty() && (&genjetsLVec) != nullptr ){
      for(unsigned int ij=0; ij<recoJetsLVec.size(); ij++){
        //                if( !AnaFunctions::jetPassCuts(recoJetsLVec[ij], AnaConsts::pt20Eta25Arr) ) continue;
        if( !AnaFunctions::jetPassCuts(recoJetsLVec[ij], AnaConsts::pt30Eta24Arr) ) continue;
        float mindeltaR = 999.0;
        int matchedgenJetsIdx = -1;
        for(unsigned int ig=0; ig<genjetsLVec.size(); ig++){
          float dR = recoJetsLVec[ij].DeltaR(genjetsLVec[ig]);
          if( mindeltaR > dR ){ mindeltaR = dR; matchedgenJetsIdx = (int)ig; }
        }
        if( matchedgenJetsIdx != -1 && mindeltaR > 0.3 && recoJetschargedHadronEnergyFraction[ij] < 0.1 ) passFilter = false;
      }
    }
    tr->setReThrow(cached_rethrow);
  }
  return passFilter;
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::CalcMT2
//  Description:  
// ===========================================================================
float BaselineVessel::CalcMT2() const
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initial the input ~~~~~
  TLorentzVector fatJet1LVec(0, 0, 0,0);
  TLorentzVector fatJet2LVec(0, 0, 0,0);
  //get output of tagger
  const TopTaggerResults& ttr = ttPtr->getResults();
  //Use result for top var
  const std::vector<TopObject*> &Ntop = ttr.getTops();  

  if (Ntop.size() == 0)
  {
    return 0.0;
  }

  if (Ntop.size() == 1)
  {
    fatJet1LVec = Ntop.at(0)->P();
    fatJet2LVec = ttr.getRsys().P();
     
    return coreMT2calc(fatJet1LVec, fatJet2LVec);
  }

  if (Ntop.size() >= 2)
  {
    std::vector<float> cachedMT2vec;
    for(unsigned int it=0; it<Ntop.size(); it++)
    {
       for(unsigned int jt=it+1; jt<Ntop.size(); jt++)
       {
          cachedMT2vec.push_back(coreMT2calc(Ntop.at(it)->P(), Ntop.at(jt)->P()));
       } 
    }
    std::sort(cachedMT2vec.begin(), cachedMT2vec.end());

    return cachedMT2vec.front();
//    return cachedMT2vec.back();
  }

  return 0.0;
}

float BaselineVessel::coreMT2calc(const TLorentzVector & fatJet1LVec, const TLorentzVector & fatJet2LVec) const
{
  
  // The input parameters associated with the particle
  // (or collection of particles) associated with the
  // first "side" of the event: 
  const float massOfSystemA =  fatJet1LVec.M(); // GeV
  const float pxOfSystemA   =  fatJet1LVec.Px(); // GeV
  const float pyOfSystemA   =  fatJet1LVec.Py(); // GeV
  
  // The input parameters associated with the particle
  // (or collection of particles) associated with the
  // second "side" of the event:
  const float massOfSystemB =  fatJet2LVec.M(); // GeV
  const float pxOfSystemB   =  fatJet2LVec.Px(); // GeV
  const float pyOfSystemB   =  fatJet2LVec.Py(); // GeV
  
  // The missing transverse momentum:
  const float pxMiss        = metLVec.Px(); // GeV
  const float pyMiss        = metLVec.Py(); // GeV
  
  // The mass of the "inivisible" particle presumed to have
  // been produced at the end of the decay chain in each
  // "half" of the event:    
  const float invis_mass    = metLVec.M(); // GeV

  float desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

  asymm_mt2_lester_bisect::disableCopyrightMessage();

  float mt2 =  asymm_mt2_lester_bisect::get_mT2(
      massOfSystemA, pxOfSystemA, pyOfSystemA,
      massOfSystemB, pxOfSystemB, pyOfSystemB,
      pxMiss, pyMiss,
      invis_mass, invis_mass,
      desiredPrecisionOnMt2);

  return mt2;

}       // -----  end of function BaselineVessel::CalcMT2  -----

void BaselineVessel::operator()(NTupleReader& tr_)
{
  tr = &tr_;
  PassBaseline();
  FlagAK8Jets();
  GetSoftbJets();
  GetMHT();
  GetLeptons();
  GetRecoZ(81, 101);
  GetTopCombs();
}


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetSoftbJets
//  Description:  
// ===========================================================================
bool BaselineVessel::GetSoftbJets() 
{
  if (!tr->checkBranch("svLVec"))
  {
    return false;
  }

  std::vector<TLorentzVector> *SoftbLVec = new std::vector<TLorentzVector>();


  const std::vector<TLorentzVector> &svLVec   = tr->getVec<TLorentzVector>("svLVec");
  const std::vector<TLorentzVector> &jetsLVec   = tr->getVec<TLorentzVector>("jetsLVec");
  const std::vector<float> &svPT   = tr->getVec<float>("svPT");
  const std::vector<float> &svDXY   = tr->getVec<float>("svDXY");
  const std::vector<float> &svD3D   = tr->getVec<float>("svD3D");
  const std::vector<float> &svD3Derr   = tr->getVec<float>("svD3Derr");
  const std::vector<float> &svNTracks   = tr->getVec<float>("svNTracks");
  const std::vector<float> &svCosThetaSVPS   = tr->getVec<float>("svCosThetaSVPS");

  for(unsigned int i =0; i<svLVec.size(); i++)
  {
    if(svPT.at(i) >= 20.0 ) continue;

    if(svDXY.at(i) >= 3.0) continue;
    if(svD3D.at(i)/svD3Derr.at(i) <= 4.0) continue;
    if(svCosThetaSVPS.at(i) <= 0.98) continue;
    if(svNTracks.at(i)<3) continue;

    bool failDR = false;
    for(unsigned int j=0; j<jetsLVec.size(); j++)
    {
      if (jetsLVec.at(j).Pt() < 20 || fabs(jetsLVec.at(j).Eta()) > 2.4) continue;

      if( ROOT::Math::VectorUtil::DeltaR( svLVec.at(i), jetsLVec.at(j) ) <= 0.4 ) {
        failDR = true;
        break;
      }
    }
    if(failDR) continue;

    SoftbLVec->push_back(svLVec.at(i));
  }

  tr->registerDerivedVec("softbLVec"+firstSpec, SoftbLVec);
  return true;
}       // -----  end of function BaselineVessel::GetSoftbJets  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetMHT
//  Description:  /* cursor */
// ===========================================================================
bool BaselineVessel::GetMHT() const
{
  // Calculate MHT
  TLorentzVector MHT(0, 0, 0, 0);
  float SumHT = 0.0; //Using jet > 30 , |eta| < 5
  for(auto &jet : tr->getVec<TLorentzVector>("prodJetsNoLep_jetsLVec"))
  {
    if (jet.Pt() >= 30)
    {
      MHT -= jet;
      SumHT += jet.Pt();
    }
  }
  tr->registerDerivedVar("MHT"+firstSpec, MHT.Pt());
  tr->registerDerivedVar("MHTPhi"+firstSpec, MHT.Phi());
  tr->registerDerivedVar("MHTSig"+firstSpec, MHT.Pt()/ sqrt(SumHT));

  if (tr->getVar<unsigned int>("run") > 3125261) // Set weight to 1 for Data
    tr->registerDerivedVar("METSig"+firstSpec, tr->getVar<float>(METLabel)/ sqrt(SumHT));
  else
    tr->registerDerivedVar("METSig"+firstSpec, tr->getVar<float>(METLabel)/ sqrt(SumHT));
  return true;
}       // -----  end of function BaselineVessel::GetMHT  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetLeptons
//  Description:  
// ===========================================================================
bool BaselineVessel::GetLeptons() const
{
  std::vector<TLorentzVector> *vMuons = new std::vector<TLorentzVector> ();
  std::vector<TLorentzVector> *vEles = new std::vector<TLorentzVector> ();
  std::vector<int> *vMuonChg = new std::vector<int> ();
  std::vector<int> *vEleChg = new std::vector<int> ();

  const std::vector<TLorentzVector> &muonsLVec   = tr->getVec<TLorentzVector>("muonsLVec");
  const std::vector<float>         &muonsRelIso = tr->getVec<float>("muonsMiniIso");
  const std::vector<float>         &muonsMtw    = tr->getVec<float>("muonsMtw");
  const std::vector<int>            &muonsFlagID = tr->getVec<int>(muonsFlagIDLabel.c_str());
  const std::vector<float>         &muonsCharge = tr->getVec<float>("muonsCharge");
  for(unsigned int im=0; im<muonsLVec.size(); im++){
    if(AnaFunctions::passMuon(muonsLVec[im], muonsRelIso[im], muonsMtw[im], muonsFlagID[im], AnaConsts::muonsMiniIsoArr))
    {
      if (!vMuons->empty()) // Making sure the vMuons are sorted in Pt
        assert(muonsLVec.at(im).Pt() <= vMuons->back().Pt());
      vMuons->push_back(muonsLVec.at(im));
      vMuonChg->push_back(muonsCharge.at(im));
    }
  }

  const std::vector<TLorentzVector> &electronsLVec   = tr->getVec<TLorentzVector>("elesLVec");
  const std::vector<float> &electronsRelIso         = tr->getVec<float>("elesMiniIso");
  const std::vector<float> &electronsMtw            = tr->getVec<float>("elesMtw");
  const std::vector<unsigned int> &isEBVec           = tr->getVec<unsigned int>("elesisEB");
  const std::vector<int> &electronsFlagID            = tr->getVec<int>(elesFlagIDLabel.c_str());
  const std::vector<float>         &electronsCharge = tr->getVec<float>("elesCharge");
  for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
    if(AnaFunctions::passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], isEBVec[ie], electronsFlagID[ie], AnaConsts::elesMiniIsoArr)) 
    {
      if (!vEles->empty()) // Making sure the vEles are sorted in Pt
        assert(electronsLVec.at(ie).Pt() <= vEles->back().Pt());
      vEles->push_back(electronsLVec.at(ie));
      vEleChg->push_back(electronsCharge.at(ie));

    }
  }

  tr->registerDerivedVar("cutMuID"+firstSpec, muonsFlagIDLabel);
  tr->registerDerivedVar("cutEleID"+firstSpec, elesFlagIDLabel);
  tr->registerDerivedVec("cutMuVec"+firstSpec, vMuons);
  tr->registerDerivedVec("cutEleVec"+firstSpec, vEles);
  tr->registerDerivedVec("cutMuCharge"+firstSpec, vMuonChg);
  tr->registerDerivedVec("cutEleCharge"+firstSpec, vEleChg);

  return true;
}       // -----  end of function BaselineVessel::GetLeptons  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetRecoZ
//  Description:  
// ===========================================================================
bool BaselineVessel::GetRecoZ( const int zMassMin, const int zMassMax) const
{
  std::vector<TLorentzVector>* recoZVec = new std::vector<TLorentzVector>();
  std::map<unsigned int, std::pair<unsigned int, unsigned int> > *ZLepIdx = 
    new std::map<unsigned int, std::pair<unsigned int, unsigned int> >();

  GetRecoZ("cutMuVec"+firstSpec,"cutMuCharge"+firstSpec, recoZVec, ZLepIdx, zMassMin, zMassMax );
  GetRecoZ("cutEleVec"+firstSpec,"cutEleCharge"+firstSpec, recoZVec, ZLepIdx, zMassMin, zMassMax );

  tr->registerDerivedVec("recoZVec"+spec, recoZVec);
  tr->registerDerivedVec("ZLepIdx"+spec, ZLepIdx);
  return true;
}       // -----  end of function BaselineVessel::GetRecoZ  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetRecoZ
//  Description:  
// ===========================================================================
bool BaselineVessel::GetRecoZ(const std::string leptype, const std::string lepchg, 
    std::vector<TLorentzVector>* recoZVec ,
    std::map<unsigned int, std::pair<unsigned int, unsigned int> > *ZLepIdx,
    const int zMassMin, const int zMassMax) const
{
  
  const std::vector<TLorentzVector> & LepVec = tr->getVec<TLorentzVector>(leptype);
  const std::vector<int> & LepChg = tr->getVec<int>(lepchg);

  for(unsigned int i = 0; i < LepVec.size(); ++i)
  {
    for(unsigned int j = i; j < LepVec.size(); ++j)
    {
      float zm = (LepVec.at(i) + LepVec.at(j)).M();
      int sumchg = LepChg.at(i) + LepChg.at(j); 
      if (sumchg == 0 && zm > zMassMin && zm < zMassMax)
      {
        recoZVec->push_back((LepVec.at(i) + LepVec.at(j)));
        if (leptype.find("Mu") != std::string::npos)
        {
          ZLepIdx->insert(std::make_pair( recoZVec->size(), 
                std::make_pair(i, j)));
        }
        if (leptype.find("Ele") != std::string::npos) // offset by 100
        {
          ZLepIdx->insert(std::make_pair( recoZVec->size(), 
                std::make_pair(i+100, j+100)));
        }
      }
    }
  }
  return true;
}       // -----  end of function BaselineVessel::GetRecoZ  -----


//**************************************************************************//
//                                 CleanJets                                //
//**************************************************************************//

void stopFunctions::CleanJets::setMuonIso(const std::string muIsoFlag) 
{
  if(muIsoFlag.compare("mini") == 0)
  {
    muIsoStr_ = "muonsMiniIso";
    muIsoReq_ = AnaConsts::muonsMiniIsoArr;
  }
  else if(muIsoFlag.compare("rel") == 0)
  {
    muIsoStr_ = "muonsRelIso";
    muIsoReq_ = AnaConsts::muonsArr;
  }
  else
  {
    std::cout << "cleanJets(...):  muon iso mode not recognized!!!  Using \"rel iso\" settings." << std::endl;
    muIsoStr_ = "muonsRelIso";
    muIsoReq_ = AnaConsts::muonsArr;
  }
}

void stopFunctions::CleanJets::setElecIso(const std::string elecIsoFlag)
{
  if(elecIsoFlag.compare("mini") == 0)
  {
    elecIsoStr_ = "elesMiniIso";
    elecIsoReq_ = AnaConsts::elesMiniIsoArr;
  }
  else if(elecIsoFlag.compare("rel") == 0)
  {
    elecIsoStr_ = "elesRelIso";
    elecIsoReq_ = AnaConsts::elesArr;
  }
  else
  {
    std::cout << "cleanJets(...):  muon iso mode not recognized!!!  Using \"rel iso\" settings." << std::endl;
    elecIsoStr_ = "elesRelIso";
    elecIsoReq_ = AnaConsts::elesArr;
  }
}

void stopFunctions::CleanJets::setJetCollection(std::string jetVecLabel)
{
  jetVecLabel_ = jetVecLabel;
}

void stopFunctions::CleanJets::setBTagCollection(std::string bTagLabel)
{
  bTagLabel_ = bTagLabel;
}

void stopFunctions::CleanJets::setMuonsFlagID(std::string muonsFlagIDLabel)
{
  muonsFlagIDLabel_ = muonsFlagIDLabel;
}

void stopFunctions::CleanJets::setElesFlagID(std::string elesFlagIDLabel)
{
  elesFlagIDLabel_ = elesFlagIDLabel;
}

void stopFunctions::CleanJets::setEnergyFractionCollections(std::string chargedEMfrac, std::string neutralEMfrac, std::string chargedHadfrac)
{
  chargedEMFracLabel_ = chargedEMfrac;
  neutralEMFracLabel_ = neutralEMfrac;
  chargedHadFracLabel_ = chargedHadfrac;
}

void stopFunctions::CleanJets::setForceDr(bool forceDr)
{
  forceDr_ = forceDr;
}

void stopFunctions::CleanJets::setDisable(bool disable)
{
  disableMuon_ = disable;
  disableElec_ = disable;
}

void stopFunctions::CleanJets::setDisableElec(bool disable)
{
  disableElec_ = disable;
}

void stopFunctions::CleanJets::setDisableMuon(bool disable)
{
  disableMuon_ = disable;
}

void stopFunctions::CleanJets::setRemove(bool remove)
{
  remove_ = remove;
}

void stopFunctions::CleanJets::setElecPtThresh(float minPt)
{
  elecPtThresh_ = minPt;
}

void stopFunctions::CleanJets::setMuonPtThresh(float minPt)
{
  muonPtThresh_ = minPt;
}

//This option is used to clean up to 1 jet in the minDr cone around the muon if the jet is lower pt than the muon
//It is designed only for use with the z->inv background to remove muon related radiation from the event
void stopFunctions::CleanJets::setPhotoCleanThresh(float photoCleanThresh)
{
  photoCleanThresh_ = photoCleanThresh;
}

void stopFunctions::CleanJets::setJecScaleRawToFull(std::string jecScaleRawToFullLabel)
{
  recoJetsJecScaleRawToFullLabel_ = jecScaleRawToFullLabel;
}
//NOTE!!! Must add Hadron and EM fraction vectors here

//Private
int stopFunctions::CleanJets::cleanLeptonFromJet(const TLorentzVector& lep, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec, const std::vector<float>& jecScaleRawToFull, std::vector<bool>& keepJet, const std::vector<float>& neutralEmEnergyFrac, std::vector<TLorentzVector>* cleanJetVec, const float& jldRMax, const float photoCleanThresh)
{
  int match = lepMatchedJetIdx;
  if(match < 0)
  {
    //If muon matching to PF candidate has failed, use dR matching as fallback
    match = AnaFunctions::jetLepdRMatch(lep, jetsLVec, jldRMax);
  }

  if(match >= 0)
  {
    if(remove_ || (photoCleanThresh > 0.0 && neutralEmEnergyFrac[match] > photoCleanThresh) )
    {
      keepJet[match] = false;
    }
    else
    {
      (*cleanJetVec)[match] -= lep * jecScaleRawToFull[match];
    }
  }

  return match;
}

/*
 *int stopFunctions::CleanJets::ak8DRJet(const std::vector<TLorentzVector>& ak8JetsLVec, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec,  const float& jak8dRMax)
 *{
 *  int match1 = lepMatchedJetIdx;
 *  if(match1 < 0)
 *  {
 *    //If muon matching to PF candidate has failed, use dR matching as fallback
 *    match1 = AnaFunctions::jetdRMatch(ak8JetsLVec, jetsLVec, jak8dRMax);
 *  }
 *  return match1;
 *}
 */

void stopFunctions::CleanJets::internalCleanJets(NTupleReader& tr)
{
  const std::vector<TLorentzVector>& jetsLVec         = tr.getVec<TLorentzVector>(jetVecLabel_);
  const std::vector<TLorentzVector>& elesLVec         = tr.getVec<TLorentzVector>("elesLVec");
  const std::vector<TLorentzVector>& muonsLVec        = tr.getVec<TLorentzVector>("muonsLVec");
  const std::vector<float>&         elesIso          = tr.getVec<float>(elecIsoStr_);
  const std::vector<float>&         muonsIso         = tr.getVec<float>(muIsoStr_);
  const std::vector<int>&            muonsFlagIDVec   = muonsFlagIDLabel_.empty()? std::vector<int>(muonsIso.size(), 1):tr.getVec<int>(muonsFlagIDLabel_.c_str());
  const std::vector<int>&            elesFlagIDVec    = elesFlagIDLabel_.empty()? std::vector<int>(elesIso.size(), 1):tr.getVec<int>(elesFlagIDLabel_.c_str());
  const std::vector<float>&         recoJetsCSVv2   = tr.getVec<float>(bTagLabel_);
  const std::vector<float>& chargedHadronEnergyFrac  = tr.getVec<float>(chargedHadFracLabel_);
  const std::vector<float>&     neutralEmEnergyFrac  = tr.getVec<float>(neutralEMFracLabel_);
  const std::vector<float>&     chargedEmEnergyFrac  = tr.getVec<float>(chargedEMFracLabel_);
  const std::vector<int>&            muMatchedJetIdx  = tr.getVec<int>("muMatchedJetIdx");
  const std::vector<int>&            eleMatchedJetIdx = tr.getVec<int>("eleMatchedJetIdx");
  const std::vector<unsigned int>&   elesisEB         = tr.getVec<unsigned int>("elesisEB");
  const std::vector<float>& recoJetsJecScaleRawToFull = recoJetsJecScaleRawToFullLabel_.empty()? std::vector<float>(jetsLVec.size(), 1):tr.getVec<float>(recoJetsJecScaleRawToFullLabel_.c_str());

  const unsigned int& run   = tr.getVar<unsigned int>("run");
  const unsigned int& lumi  = tr.getVar<unsigned int>("lumi");
  const unsigned int& event = tr.getVar<unsigned int>("event");

  if(elesLVec.size() != elesIso.size() 
      || elesLVec.size() != eleMatchedJetIdx.size()
      || elesLVec.size() != elesisEB.size()
      || muonsLVec.size() != muonsIso.size()
      || muonsLVec.size() != muMatchedJetIdx.size()
      || jetsLVec.size() != recoJetsCSVv2.size()
      || jetsLVec.size() != chargedHadronEnergyFrac.size()
      || jetsLVec.size() != neutralEmEnergyFrac.size()
      || jetsLVec.size() != chargedEmEnergyFrac.size())
  {
    std::cout << "MISMATCH IN VECTOR SIZE!!!!! Aborting jet cleaning algorithm!!!!!!" << std::endl;
    return;
  }

  std::vector<TLorentzVector>* cleanJetVec        = new std::vector<TLorentzVector>(jetsLVec);
  std::vector<float>* cleanJetBTag               = new std::vector<float>(recoJetsCSVv2);
  std::vector<TLorentzVector>* cleanJetpt30ArrVec = new std::vector<TLorentzVector>();
  std::vector<float>* cleanJetpt30ArrBTag        = new std::vector<float>;
  std::vector<float>* cleanChargedHadEFrac       = new std::vector<float>(chargedHadronEnergyFrac);
  std::vector<float>* cleanNeutralEMEFrac        = new std::vector<float>(neutralEmEnergyFrac);
  std::vector<float>* cleanChargedEMEFrac        = new std::vector<float>(chargedEmEnergyFrac);

  std::vector<TLorentzVector>* removedJetVec      = new std::vector<TLorentzVector>();
  std::vector<float>* removedChargedHadEFrac     = new std::vector<float>();
  std::vector<float>* removedNeutralEMEFrac      = new std::vector<float>();
  std::vector<float>* removedChargedEMEFrac      = new std::vector<float>();

  std::vector<int>* rejectJetIdx_formuVec = new std::vector<int>();
  std::vector<int>* rejectJetIdx_foreleVec = new std::vector<int>();

  const float jldRMax = 0.15;

  const float HT_jetPtMin = 50;
  const float HT_jetEtaMax = 2.4;
  const float MHT_jetPtMin = 30.0;

  float HT = 0.0, HTNoIso = 0.0;
  TLorentzVector MHT;

  std::vector<bool> keepJetPFCandMatch(jetsLVec.size(), true);

  if(!disableMuon_)
  {
    for(int iM = 0; iM < muonsLVec.size() && iM < muonsIso.size() && iM < muMatchedJetIdx.size(); ++iM)
    {
      if(!AnaFunctions::passMuon(muonsLVec[iM], muonsIso[iM], 0.0, muonsFlagIDVec[iM], muIsoReq_) && muonsLVec[iM].Pt() > muonPtThresh_) 
      {
        rejectJetIdx_formuVec->push_back(-1);
        continue;
      }

      int match = -1;
      if(forceDr_) match = cleanLeptonFromJet(muonsLVec[iM],                  -1, jetsLVec, recoJetsJecScaleRawToFull, keepJetPFCandMatch, neutralEmEnergyFrac, cleanJetVec, jldRMax, photoCleanThresh_);
      else         match = cleanLeptonFromJet(muonsLVec[iM], muMatchedJetIdx[iM], jetsLVec, recoJetsJecScaleRawToFull, keepJetPFCandMatch, neutralEmEnergyFrac, cleanJetVec, jldRMax, photoCleanThresh_);

      if( match >= 0 ) rejectJetIdx_formuVec->push_back(match);
      else rejectJetIdx_formuVec->push_back(-1);
    }
  }

  if(!disableElec_)
  {
    for(int iE = 0; iE < elesLVec.size() && iE < elesIso.size() && iE < eleMatchedJetIdx.size(); ++iE)
    {
      if(!AnaFunctions::passElectron(elesLVec[iE], elesIso[iE], 0.0, elesisEB[iE], elesFlagIDVec[iE], elecIsoReq_) && elesLVec[iE].Pt() > elecPtThresh_) 
      {
        rejectJetIdx_foreleVec->push_back(-1);
        continue;
      }

      int match = -1;
      if(forceDr_) match = cleanLeptonFromJet(elesLVec[iE],                   -1, jetsLVec, recoJetsJecScaleRawToFull, keepJetPFCandMatch, neutralEmEnergyFrac, cleanJetVec, jldRMax);
      else         match = cleanLeptonFromJet(elesLVec[iE], eleMatchedJetIdx[iE], jetsLVec, recoJetsJecScaleRawToFull, keepJetPFCandMatch, neutralEmEnergyFrac, cleanJetVec, jldRMax);

      if( match >= 0 ) rejectJetIdx_foreleVec->push_back(match);
      else rejectJetIdx_foreleVec->push_back(-1);
    }
  }

  int jetsKept = 0;
  auto iJet = cleanJetVec->begin();
  auto iOrigJet = jetsLVec.begin();
  auto iBTag = cleanJetBTag->begin();
  auto iKeep = keepJetPFCandMatch.begin();
  auto iCHF = cleanChargedHadEFrac->begin();
  auto iNEMF = cleanNeutralEMEFrac->begin();
  auto iCEMF = cleanChargedEMEFrac->begin();
  for(; iJet != cleanJetVec->end() && iBTag != cleanJetBTag->end() && iKeep != keepJetPFCandMatch.end() && iOrigJet != jetsLVec.end(); ++iKeep, ++iOrigJet)
  {
    if(!(*iKeep))
    {
      removedJetVec->push_back(*iOrigJet);
      removedChargedHadEFrac->push_back(*iCHF);
      removedNeutralEMEFrac->push_back(*iNEMF);
      removedChargedEMEFrac->push_back(*iCEMF);
      iJet = cleanJetVec->erase(iJet);
      iBTag = cleanJetBTag->erase(iBTag);
      iCHF = cleanChargedHadEFrac->erase(iCHF);
      iNEMF = cleanNeutralEMEFrac->erase(iNEMF);
      iCEMF = cleanChargedEMEFrac->erase(iCEMF);
      continue;
    }

    ++jetsKept;
    if(AnaFunctions::jetPassCuts(*iJet, AnaConsts::pt30Arr))
    {
      cleanJetpt30ArrVec->push_back(*iJet);
      cleanJetpt30ArrBTag->push_back(*iBTag);
    }
    if(iJet->Pt() > HT_jetPtMin && fabs(iJet->Eta()) < HT_jetEtaMax) HT += iJet->Pt();
    if(iJet->Pt() > MHT_jetPtMin) MHT += *iJet;

    ++iJet;
    ++iBTag;
    ++iCHF;
    ++iNEMF;
    ++iCEMF;
  }

  tr.registerDerivedVar("nJetsRemoved", static_cast<int>(jetsLVec.size() - jetsKept));
  tr.registerDerivedVar("cleanHt", HT);
  tr.registerDerivedVar("cleanMHt", MHT.Pt());
  tr.registerDerivedVar("cleanMHtPhi", MHT.Phi());
  tr.registerDerivedVec("removedJetVec", removedJetVec);
  tr.registerDerivedVec("cleanJetVec", cleanJetVec);
  tr.registerDerivedVec("cleanJetBTag", cleanJetBTag);
  tr.registerDerivedVec("cleanJetpt30ArrVec", cleanJetpt30ArrVec);
  tr.registerDerivedVec("cleanJetpt30ArrBTag", cleanJetpt30ArrBTag);
  tr.registerDerivedVec("cleanChargedHadEFrac", cleanChargedHadEFrac);
  tr.registerDerivedVec("cleanNeutralEMEFrac", cleanNeutralEMEFrac);
  tr.registerDerivedVec("cleanChargedEMEFrac", cleanChargedEMEFrac);
  tr.registerDerivedVec("removedChargedHadEFrac", removedChargedHadEFrac);
  tr.registerDerivedVec("removedNeutralEMEFrac",  removedNeutralEMEFrac);
  tr.registerDerivedVec("removedChargedEMEFrac",  removedChargedEMEFrac);
  tr.registerDerivedVec("rejectJetIdx_formuVec", rejectJetIdx_formuVec);
  tr.registerDerivedVec("rejectJetIdx_foreleVec", rejectJetIdx_foreleVec);
}
