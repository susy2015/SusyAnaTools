#include "baselineDef.h"

//**************************************************************************//
//                              BaselineVessel                              //
//**************************************************************************//

BaselineVessel::BaselineVessel(NTupleReader &tr_, const std::string specialization, const std::string filterString) : 
  tr(&tr_), spec(specialization), 
  UseNewTagger(true),
  type3Ptr(NULL),
  ttPtr(NULL)
{
  bToFake               = 1;
  debug                 = false;
  incZEROtop            = false;
  jetVecLabel           = "jetsLVec";
  CSVVecLabel           = "recoJetsBtag_0";
  METLabel              = "met";
  METPhiLabel           = "metphi";
  jetVecLabelAK8        = "ak8JetsLVec";
  qgLikehoodLabel       = "qgLikelihood";
  muonsFlagIDLabel      = "muonsFlagMedium";
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

  if( !spec.empty() ){
    TString stripT = spec;
    TObjArray * objArr = stripT.Tokenize(" ");
    TObjString* firstObj = dynamic_cast<TObjString*>(objArr->At(0));
    firstSpec = firstObj->GetString().Data();
    std::cout<<"\nfirstSpec : "<<firstSpec.c_str()<<"  spec : "<<spec.c_str()<<"  isfastsim : "<<isfastsim<<std::endl<<std::endl;
  }

  printOnce = false;

  PredefineSpec();

  SetupTopTagger(UseNewTagger, toptaggerCfgFile);
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::SetupTopTagger
//  Description:  
// ===========================================================================
bool BaselineVessel::SetupTopTagger(bool UseNewTagger_, std::string CfgFile_)
{
  UseNewTagger = UseNewTagger_;
  toptaggerCfgFile = CfgFile_;

  if (!UseNewTagger)
  {
    type3Ptr.reset(new topTagger::type3TopTagger);
  }
  if (UseNewTagger)
  {
    ttPtr.reset(new TopTagger);
    ttPtr->setCfgFile(toptaggerCfgFile);
  }
  
  return true;
}       // -----  end of function BaselineVessel::SetupTopTagger  -----

void BaselineVessel::prepareTopTagger()
{
// Prepare jets and b-tag working points for top tagger
  jetsLVec_forTagger     = new std::vector<TLorentzVector>();
  recoJetsBtag_forTagger = new std::vector<double>();
  qgLikelihood_forTagger = new std::vector<double>();
  std::vector<double> qgLikelihood;
  try
  {
    qgLikelihood = tr->getVec<double>(qgLikehoodLabel);
  }
  catch (const SATException& e)
  {
    e.print();
    qgLikelihood.clear();
  }
    
  AnaFunctions::prepareJetsForTagger(tr->getVec<TLorentzVector>(jetVecLabel), tr->getVec<double>(CSVVecLabel), 
      *jetsLVec_forTagger, *recoJetsBtag_forTagger, qgLikelihood, *qgLikelihood_forTagger);

  tr->registerDerivedVec("jetsLVec_forTagger" + firstSpec, jetsLVec_forTagger);
  tr->registerDerivedVec("recoJetsBtag_forTagger" + firstSpec, recoJetsBtag_forTagger);
  tr->registerDerivedVec("qgLikelihood_forTagger" + firstSpec, qgLikelihood_forTagger);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Type3 TopTagger ~~~~~
  if (!UseNewTagger)
  {
    type3Ptr->setnJetsSel(AnaConsts::nJetsSel);
    type3Ptr->setCSVS(AnaConsts::cutCSVS);
    type3Ptr->setCSVToFake(bToFake);
    type3Ptr->processEvent(*jetsLVec_forTagger, *recoJetsBtag_forTagger, metLVec);
  }
  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ New TopTagger ~~~~~
  if (UseNewTagger)
  {
    // top tagger
    //construct vector of constituents 
    ttUtility::ConstAK4Inputs myConstAK4Inputs = ttUtility::ConstAK4Inputs(*jetsLVec_forTagger, *recoJetsBtag_forTagger, *qgLikelihood_forTagger);
    ttUtility::ConstAK8Inputs myConstAK8Inputs = ttUtility::ConstAK8Inputs(
        tr->getVec<TLorentzVector>("puppiJetsLVec"), 
        tr->getVec<double>("puppitau1"), 
        tr->getVec<double>("puppitau2"), 
        tr->getVec<double>("puppitau3"), 
        tr->getVec<double>("puppisoftDropMass"), 
        tr->getVec<TLorentzVector>("puppiSubJetsLVec"));
    std::vector<Constituent> constituents = ttUtility::packageConstituents(myConstAK4Inputs, myConstAK8Inputs);
    //run tagger
    ttPtr->runTagger(constituents);
  }
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
    jetVecLabel = "jetsLVecLepCleaned";
    CSVVecLabel = "recoJetsBtag_0_LepCleaned";
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
  if (!UseNewTagger)
  {
    passTagger = type3Ptr->passNewTaggerReq() && (incZEROtop || nTopCandSortedCnt >= AnaConsts::low_nTopCandSortedSel); 
  }

  if (UseNewTagger)
  {
    passTagger = (incZEROtop || nTopCandSortedCnt >= AnaConsts::low_nTopCandSortedSel); 
  }

  if (!UseNewTagger) //This is useless and will be removed @hongxuan
  {
    tr->registerDerivedVar("passNewTaggerReq" + firstSpec, type3Ptr->passNewTaggerReq());
  }

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
  metLVec.SetPtEtaPhiM(tr->getVar<double>(METLabel), 0, tr->getVar<double>(METPhiLabel), 0);

  // Calculate number of leptons
  const std::vector<int> & muonsFlagIDVec = muonsFlagIDLabel.empty()? std::vector<int>(tr->getVec<double>("muonsMiniIso").size(), 1):tr->getVec<int>(muonsFlagIDLabel.c_str()); // We have muonsFlagTight as well, but currently use medium ID
  const std::vector<int> & elesFlagIDVec = elesFlagIDLabel.empty()? std::vector<int>(tr->getVec<double>("elesMiniIso").size(), 1):tr->getVec<int>(elesFlagIDLabel.c_str()); // Fake electrons since we don't have different ID for electrons now, but maybe later
  int nMuons = AnaFunctions::countMuons(tr->getVec<TLorentzVector>("muonsLVec"), tr->getVec<double>("muonsMiniIso"), tr->getVec<double>("muonsMtw"), muonsFlagIDVec, AnaConsts::muonsMiniIsoArr);
  int nElectrons = AnaFunctions::countElectrons(tr->getVec<TLorentzVector>("elesLVec"), tr->getVec<double>("elesMiniIso"), tr->getVec<double>("elesMtw"), tr->getVec<unsigned int>("elesisEB"), elesFlagIDVec, AnaConsts::elesMiniIsoArr);
  int nIsoTrks = AnaFunctions::countIsoTrks(tr->getVec<TLorentzVector>("loose_isoTrksLVec"), tr->getVec<double>("loose_isoTrks_iso"), tr->getVec<double>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));
  int nIsoLepTrks = AnaFunctions::countIsoLepTrks(tr->getVec<TLorentzVector>("loose_isoTrksLVec"), tr->getVec<double>("loose_isoTrks_iso"), tr->getVec<double>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));
  int nIsoPionTrks = AnaFunctions::countIsoPionTrks(tr->getVec<TLorentzVector>("loose_isoTrksLVec"), tr->getVec<double>("loose_isoTrks_iso"), tr->getVec<double>("loose_isoTrks_mtw"), tr->getVec<int>("loose_isoTrks_pdgId"));

  // Calculate number of jets and b-tagged jets
  int cntCSVS = AnaFunctions::countCSVS(tr->getVec<TLorentzVector>(jetVecLabel), tr->getVec<double>(CSVVecLabel), AnaConsts::cutCSVS, AnaConsts::bTagArr);
  int cntNJetsPt50Eta24 = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt50Eta24Arr);
  int cntNJetsPt30Eta24 = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Eta24Arr);
  int cntNJetsPt30      = AnaFunctions::countJets(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Arr);

  // Calculate deltaPhi
  std::vector<double> * dPhiVec = new std::vector<double>();
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
  if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passnJets = false; passBaselineNoLepVeto = false; }
  if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passnJets = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"cntNJetsPt50Eta24 : "<<cntNJetsPt50Eta24<<"  cntNJetsPt30Eta24 : "<<cntNJetsPt30Eta24<<"  cntNJetsPt30 : "<<cntNJetsPt30<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass deltaPhi?
  bool passdPhis = (dPhiVec->at(0) >= AnaConsts::dPhi0_CUT && dPhiVec->at(1) >= AnaConsts::dPhi1_CUT && dPhiVec->at(2) >= AnaConsts::dPhi2_CUT);
  if( dodPhis && !passdPhis ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"dPhi0 : "<<dPhiVec->at(0)<<"  dPhi1 : "<<dPhiVec->at(1)<<"  dPhi2 : "<<dPhiVec->at(2)<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass number of b-tagged jets?
  bool passBJets = true;
  if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) ){ passBaseline = false; passBJets = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"cntCSVS : "<<cntCSVS<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass the baseline MET requirement?
  bool passMET = (metLVec.Pt() >= AnaConsts::defaultMETcut);
  if( doMET && !passMET ){ passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"met : "<<tr->getVar<double>("met")<<"  defaultMETcut : "<<AnaConsts::defaultMETcut<<"  passBaseline : "<<passBaseline<<std::endl;

  // Pass the HT cut for trigger?
  double HT = AnaFunctions::calcHT(tr->getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Eta24Arr);
  bool passHT = true;
  if( HT < AnaConsts::defaultHTcut ){ passHT = false; passBaseline = false; passBaselineNoTagMT2 = false; passBaselineNoTag = false; passBaselineNoLepVeto = false; }
  if( debug ) std::cout<<"HT : "<<HT<<"  defaultHTcut : "<<AnaConsts::defaultHTcut<<"  passHT : "<<passHT<<"  passBaseline : "<<passBaseline<<std::endl;

  // Calculate top tagger related variables. 
  // Note that to save speed, only do the calculation after previous base line requirements.

  prepareTopTagger();
  bool passTagger = PassTopTagger();
  if( !passTagger ){ passBaseline = false; passBaselineNoLepVeto = false; }

  // Pass the baseline MT2 requirement?
  bool passMT2 = true;
  double MT2 = CalcMT2();
  if( MT2 < AnaConsts::defaultMT2cut ){ passBaseline = false; passBaselineNoTag = false; passMT2 = false; passBaselineNoLepVeto = false; }
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
  tr->registerDerivedVar("nMuons_CUT" + firstSpec, nMuons);
  tr->registerDerivedVar("nElectrons_CUT" + firstSpec, nElectrons);
  tr->registerDerivedVar("nIsoTrks_CUT" + firstSpec, nIsoTrks);

  tr->registerDerivedVar("cntNJetsPt50Eta24" + firstSpec, cntNJetsPt50Eta24);
  tr->registerDerivedVar("cntNJetsPt30Eta24" + firstSpec, cntNJetsPt30Eta24);

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

  if (!UseNewTagger)
  {
    nTops = type3Ptr->nTopCandSortedCnt;
    for(int it=0; it<nTops; it++)
    {
      TLorentzVector topLVec = type3Ptr->buildLVec(tr->getVec<TLorentzVector>("jetsLVec_forTagger" + firstSpec), 
          type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
      vTops->push_back(topLVec);
      std::vector<TLorentzVector> temp;
      std::vector<int> indexCombVec = type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]];
      for (size_t k = 0; k < indexCombVec.size(); ++k)
      {
        temp.push_back( (tr->getVec<TLorentzVector>("jetsLVec_forTagger"+spec)).at(indexCombVec.at(k)));
      }
      mTopJets->insert(std::make_pair(it, temp));
    }
  }
  if (UseNewTagger)
  {
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
  }

  return nTops;
}       // -----  end of function VarPerEvent::GetnTops  -----

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

      int eeBadScFilter = tr->getVar<int>("eeBadScFilter");

      passDataSpec = goodVerticesFilter && eeBadScFilter && passglobalTightHalo2016Filter;
    }

    unsigned int hbheNoiseFilter = isfastsim? 1:tr->getVar<unsigned int>("HBHENoiseFilter");
    unsigned int hbheIsoNoiseFilter = isfastsim? 1:tr->getVar<unsigned int>("HBHEIsoNoiseFilter");
    int ecalTPFilter = tr->getVar<int>("EcalDeadCellTriggerPrimitiveFilter");

    int jetIDFilter = isfastsim? 1:tr->getVar<int>("looseJetID_NoLep");
    // new filters
    const unsigned int & BadPFMuonFilter = tr->getVar<unsigned int>("BadPFMuonFilter");
    bool passBadPFMuonFilter = (&BadPFMuonFilter) != nullptr? tr->getVar<unsigned int>("BadPFMuonFilter") !=0 : true;

    const unsigned int & BadChargedCandidateFilter = tr->getVar<unsigned int>("BadChargedCandidateFilter");
    bool passBadChargedCandidateFilter = (&BadChargedCandidateFilter) != nullptr? tr->getVar<unsigned int>("BadChargedCandidateFilter") !=0 : true;

    tr->setReThrow(cached_rethrow);
    return passDataSpec && hbheNoiseFilter && hbheIsoNoiseFilter && ecalTPFilter && jetIDFilter && passBadPFMuonFilter && passBadChargedCandidateFilter;
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
  std::vector<double> recoJetsmuonEnergyFraction = tr->getVec<double>("recoJetsmuonEnergyFraction");
  double metphi = tr->getVar<double>("metphi");

  int nJetsLoop = recoJetsmuonEnergyFraction.size();
  std::vector<double> dPhisVec = AnaFunctions::calcDPhi( jetsLVec, metphi, nJetsLoop, AnaConsts::dphiArr);

  for(int i=0; i<nJetsLoop ; i++)
  {
    double thisrecoJetsmuonenergy = recoJetsmuonEnergyFraction.at(i)*(jetsLVec.at(i)).Pt();
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
    const std::vector<double> & recoJetschargedHadronEnergyFraction = tr->getVec<double>("recoJetschargedHadronEnergyFraction");

    if( recoJetschargedHadronEnergyFraction.size() != recoJetsLVec.size() ) std::cout<<"\nWARNING ... Non-equal recoJetschargedHadronEnergyFraction.size : "<<recoJetschargedHadronEnergyFraction.size()<<"  recoJetsLVec.size : "<<recoJetsLVec.size()<<std::endl<<std::endl;

    if( !recoJetsLVec.empty() && (&genjetsLVec) != nullptr ){
      for(unsigned int ij=0; ij<recoJetsLVec.size(); ij++){
        //                if( !AnaFunctions::jetPassCuts(recoJetsLVec[ij], AnaConsts::pt20Eta25Arr) ) continue;
        if( !AnaFunctions::jetPassCuts(recoJetsLVec[ij], AnaConsts::pt30Eta24Arr) ) continue;
        double mindeltaR = 999.0;
        int matchedgenJetsIdx = -1;
        for(unsigned int ig=0; ig<genjetsLVec.size(); ig++){
          double dR = recoJetsLVec[ij].DeltaR(genjetsLVec[ig]);
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
double BaselineVessel::CalcMT2() const
{
  if (!UseNewTagger)
    return type3Ptr->best_had_brJet_MT2;

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
    std::vector<double> cachedMT2vec;
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

double BaselineVessel::coreMT2calc(const TLorentzVector & fatJet1LVec, const TLorentzVector & fatJet2LVec) const
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Calc MT2 from old top tagger ~~~~~
  // For this example we will use the "Basic_Mt2_332_Calculator" which is
  // the algorithm we recommend people use by default.
  // Mt2::Basic_Mt2_332_Calculator mt2Calculator;
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

}       // -----  end of function BaselineVessel::CalcMT2  -----

void BaselineVessel::operator()(NTupleReader& tr_)
{
  tr = &tr_;
  PassBaseline();
  GetMHT();
  GetLeptons();
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::GetMHT
//  Description:  /* cursor */
// ===========================================================================
bool BaselineVessel::GetMHT() const
{
  // Calculate MHT
  TLorentzVector MHT(0, 0, 0, 0);
  double SumHT = 0.0; //Using jet > 30 , |eta| < 5
  for(auto &jet : tr->getVec<TLorentzVector>("jetsLVecLepCleaned"))
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
  tr->registerDerivedVar("METSig"+firstSpec, tr->getVar<double>(METLabel)/ sqrt(SumHT));
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

  const std::vector<TLorentzVector> &muonsLVec   = tr->getVec<TLorentzVector>("muonsLVec");
  const std::vector<double>         &muonsRelIso = tr->getVec<double>("muonsMiniIso");
  const std::vector<double>         &muonsMtw    = tr->getVec<double>("muonsMtw");
  const std::vector<int>            &muonsFlagID = tr->getVec<int>(muonsFlagIDLabel.c_str());
  for(unsigned int im=0; im<muonsLVec.size(); im++){
    if(AnaFunctions::passMuon(muonsLVec[im], muonsRelIso[im], muonsMtw[im], muonsFlagID[im], AnaConsts::muonsMiniIsoArr))
      vMuons->push_back(muonsLVec.at(im));
  }

  const std::vector<TLorentzVector> &electronsLVec = tr->getVec<TLorentzVector>("elesLVec");
  const std::vector<double> &electronsRelIso      = tr->getVec<double>("elesMiniIso");
  const std::vector<double> &electronsMtw         = tr->getVec<double>("elesMtw");
  const std::vector<unsigned int> &isEBVec        = tr->getVec<unsigned int>("elesisEB");
  const std::vector<int> &electronsFlagID         = tr->getVec<int>(elesFlagIDLabel.c_str());

  for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
    if(AnaFunctions::passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], isEBVec[ie], electronsFlagID[ie], AnaConsts::elesMiniIsoArr)) 
      vEles->push_back(electronsLVec.at(ie));
  }

  tr->registerDerivedVec("cutMuVec"+firstSpec, vMuons);
  tr->registerDerivedVec("cutEleVec"+firstSpec, vEles);

  return true;
}       // -----  end of function BaselineVessel::GetLeptons  -----

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

void stopFunctions::CleanJets::setElecPtThresh(double minPt)
{
  elecPtThresh_ = minPt;
}

void stopFunctions::CleanJets::setMuonPtThresh(double minPt)
{
  muonPtThresh_ = minPt;
}

//This option is used to clean up to 1 jet in the minDr cone around the muon if the jet is lower pt than the muon
//It is designed only for use with the z->inv background to remove muon related radiation from the event
void stopFunctions::CleanJets::setPhotoCleanThresh(double photoCleanThresh)
{
  photoCleanThresh_ = photoCleanThresh;
}

void stopFunctions::CleanJets::setJecScaleRawToFull(std::string jecScaleRawToFullLabel)
{
  recoJetsJecScaleRawToFullLabel_ = jecScaleRawToFullLabel;
}
//NOTE!!! Must add Hadron and EM fraction vectors here

//Private
int stopFunctions::CleanJets::cleanLeptonFromJet(const TLorentzVector& lep, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec, const std::vector<double>& jecScaleRawToFull, std::vector<bool>& keepJet, const std::vector<double>& neutralEmEnergyFrac, std::vector<TLorentzVector>* cleanJetVec, const double& jldRMax, const double photoCleanThresh)
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
 *int stopFunctions::CleanJets::ak8DRJet(const std::vector<TLorentzVector>& ak8JetsLVec, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec,  const double& jak8dRMax)
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
  const std::vector<double>&         elesIso          = tr.getVec<double>(elecIsoStr_);
  const std::vector<double>&         muonsIso         = tr.getVec<double>(muIsoStr_);
  const std::vector<int>&            muonsFlagIDVec   = muonsFlagIDLabel_.empty()? std::vector<int>(muonsIso.size(), 1):tr.getVec<int>(muonsFlagIDLabel_.c_str());
  const std::vector<int>&            elesFlagIDVec    = elesFlagIDLabel_.empty()? std::vector<int>(elesIso.size(), 1):tr.getVec<int>(elesFlagIDLabel_.c_str());
  const std::vector<double>&         recoJetsBtag_0   = tr.getVec<double>(bTagLabel_);
  const std::vector<double>& chargedHadronEnergyFrac  = tr.getVec<double>(chargedHadFracLabel_);
  const std::vector<double>&     neutralEmEnergyFrac  = tr.getVec<double>(neutralEMFracLabel_);
  const std::vector<double>&     chargedEmEnergyFrac  = tr.getVec<double>(chargedEMFracLabel_);
  const std::vector<int>&            muMatchedJetIdx  = tr.getVec<int>("muMatchedJetIdx");
  const std::vector<int>&            eleMatchedJetIdx = tr.getVec<int>("eleMatchedJetIdx");
  const std::vector<unsigned int>&   elesisEB         = tr.getVec<unsigned int>("elesisEB");
  const std::vector<double>& recoJetsJecScaleRawToFull = recoJetsJecScaleRawToFullLabel_.empty()? std::vector<double>(jetsLVec.size(), 1):tr.getVec<double>(recoJetsJecScaleRawToFullLabel_.c_str());

  const unsigned int& run   = tr.getVar<unsigned int>("run");
  const unsigned int& lumi  = tr.getVar<unsigned int>("lumi");
  const unsigned int& event = tr.getVar<unsigned int>("event");

  if(elesLVec.size() != elesIso.size() 
      || elesLVec.size() != eleMatchedJetIdx.size()
      || elesLVec.size() != elesisEB.size()
      || muonsLVec.size() != muonsIso.size()
      || muonsLVec.size() != muMatchedJetIdx.size()
      || jetsLVec.size() != recoJetsBtag_0.size()
      || jetsLVec.size() != chargedHadronEnergyFrac.size()
      || jetsLVec.size() != neutralEmEnergyFrac.size()
      || jetsLVec.size() != chargedEmEnergyFrac.size())
  {
    std::cout << "MISMATCH IN VECTOR SIZE!!!!! Aborting jet cleaning algorithm!!!!!!" << std::endl;
    return;
  }

  std::vector<TLorentzVector>* cleanJetVec        = new std::vector<TLorentzVector>(jetsLVec);
  std::vector<double>* cleanJetBTag               = new std::vector<double>(recoJetsBtag_0);
  std::vector<TLorentzVector>* cleanJetpt30ArrVec = new std::vector<TLorentzVector>();
  std::vector<double>* cleanJetpt30ArrBTag        = new std::vector<double>;
  std::vector<double>* cleanChargedHadEFrac       = new std::vector<double>(chargedHadronEnergyFrac);
  std::vector<double>* cleanNeutralEMEFrac        = new std::vector<double>(neutralEmEnergyFrac);
  std::vector<double>* cleanChargedEMEFrac        = new std::vector<double>(chargedEmEnergyFrac);

  std::vector<TLorentzVector>* removedJetVec      = new std::vector<TLorentzVector>();
  std::vector<double>* removedChargedHadEFrac     = new std::vector<double>();
  std::vector<double>* removedNeutralEMEFrac      = new std::vector<double>();
  std::vector<double>* removedChargedEMEFrac      = new std::vector<double>();

  std::vector<int>* rejectJetIdx_formuVec = new std::vector<int>();
  std::vector<int>* rejectJetIdx_foreleVec = new std::vector<int>();

  const double jldRMax = 0.15;

  const double HT_jetPtMin = 50;
  const double HT_jetEtaMax = 2.4;
  const double MHT_jetPtMin = 30.0;

  double HT = 0.0, HTNoIso = 0.0;
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
