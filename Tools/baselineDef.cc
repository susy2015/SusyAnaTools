#include "baselineDef.h"
#include "SusyUtility.h"
#include "TFile.h"
#include "TF1.h"
#include "lester_mt2_bisect.h"

//**************************************************************************//
//                              BaselineVessel                              //
//**************************************************************************//

BaselineVessel::BaselineVessel(NTupleReader &tr_, const std::string year, const std::string specialization, const std::string filterString) : tr(&tr_), year(year), spec(specialization), ttPtr(NULL), WMassCorFile(NULL)
{
  bToFake               = 1;
  debug                 = false;
  printConfig           = false;
  incZEROtop            = false;
  UseDRLeptonCleanJet   = false;
  UseDRPhotonCleanJet   = false;
  UseDeepTagger         = true;
  UseDeepCSV            = true;
  jetVecLabel           = "JetTLV";
  jetVecLabelAK8        = "FatJetTLV";
  CSVVecLabel           = "Jet_btagCSVV2";
  METLabel              = "MET_pt";
  METPhiLabel           = "MET_phi";
  qgLikehoodLabel       = "qgLikelihood";
  muonsFlagIDLabel      = "Muon_Stop0l"; 
  elesFlagIDLabel       = "Electron_Stop0l";
  toptaggerCfgFile      = "TopTagger.cfg";
  doLeptonVeto          = true;
  doMET                 = true;
  SAT_Pass_lowDM        = false;
  SAT_Pass_highDM       = false;
  SAT_Pass_lowDM_Mid    = false;
  SAT_Pass_highDM_Mid   = false;
  SAT_Pass_lowDM_Loose  = false;
  SAT_Pass_highDM_Loose  = false;
  metLVec.SetPtEtaPhiM(0, 0, 0, 0);
  min_jet_pt = 20.0;
  JetCutArrary  = AnaConsts::pt20Eta24Arr;
  dPhiCutArrary = AnaConsts::pt20Eta47Arr;
  
  // check year
  if (year.compare("2016") != 0 && year.compare("2017") != 0 && year.compare("2018") != 0)
  {
    printf("ERROR: year is set to %s, but it should be 2016, 2017, or 2018.\n", year.c_str());
  }
  
  if (UseDeepCSV)
  {
    CSVVecLabel           = "Jet_btagDeepB";
  }
  if (UseDeepTagger)
  {
    toptaggerCfgFile      = "TopTagger.cfg";
  }

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
  }
  firstSpec += taggerLabel;

  //TODO: not updated yet, plan to remove
  printOnce = false;
  PredefineSpec();

  //SetupTopTagger(toptaggerCfgFile);
}

// constructor without nullptr as argument
BaselineVessel::BaselineVessel(const std::string year, const std::string specialization, const std::string filterString) : BaselineVessel(*static_cast<NTupleReader*>(nullptr), year, specialization, filterString) {}


bool BaselineVessel::getBool(const std::string& var)
{
  bool val = false;
  if (tr->checkBranch(var))
  {
    val = tr->getVar<bool>(var);
  }
  return val;
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::UseCleanedJets
//  Description:  By default no Lep clean in Jets. Call this function to
//  switch input labels
// ===========================================================================
bool BaselineVessel::UseCleanedJets() 
{
  std::string prefix = "";
  std::string suffix = "";
  if      (UseDRPhotonCleanJet) suffix = "_drPhotonCleaned";
  else if (UseDRLeptonCleanJet) suffix = "_drLeptonCleaned";
  jetVecLabel     = prefix + "JetTLV"        + suffix;
  jetVecLabelAK8  = prefix + "FatJetTLV"     + suffix;
  CSVVecLabel     = prefix + "Jet_btagCSVV2" + suffix;
  qgLikehoodLabel = prefix + "qgLikelihood"  + suffix;
  if (UseDeepCSV)
  {
    // Note that DeepCSVcomb is a derived variable... but it is derived with cleaned variables 
    CSVVecLabel   = prefix + "Jet_btagDeepB" + suffix;
  }
  return true;
}       // -----  end of function BaselineVessel::UseCleanedJets  -----

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
//         Name:  BaselineVessel::UseCleanedJetsVar
//  Description:  /* cursor */
// ===========================================================================
std::string BaselineVessel::UseCleanedJetsVar(std::string varname) const
{
  std::string prefix = "";
  std::string suffix = "";
  if      (UseDRPhotonCleanJet) suffix = "_drPhotonCleaned";
  else if (UseDRLeptonCleanJet) suffix = "_drLeptonCleaned";
  return prefix + varname + suffix;
}       // -----  end of function BaselineVessel::UseCleanedJetsVar  -----

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

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::prepareDeepTopTagger
//  Description:  
// ===========================================================================
void BaselineVessel::prepareDeepTopTagger()
{
  jetsLVec_forTagger      = new std::vector<TLorentzVector>();
  recoJetsBtag_forTagger  = new std::vector<float>();
  qgLikelihood_forTagger  = new std::vector<float>();
  *jetsLVec_forTagger     = tr->getVec<TLorentzVector>(jetVecLabel);
  *recoJetsBtag_forTagger = tr->getVec<float>(CSVVecLabel);
  *qgLikelihood_forTagger = tr->getVec<float>(qgLikehoodLabel);

  tr->registerDerivedVec("jetsLVec_forTagger"     + firstSpec, jetsLVec_forTagger);
  tr->registerDerivedVec("recoJetsBtag_forTagger" + firstSpec, recoJetsBtag_forTagger);
  tr->registerDerivedVec("qgLikelihood_forTagger" + firstSpec, qgLikelihood_forTagger);

  // ----- AK4 Jets -----
  
  // AK4 jet variables
  std::map<std::string, std::string> AK4Variables;
  //            name for top tagger                     name in ntuple
  AK4Variables["qgPtD"]                               = "qgPtD"; 
  AK4Variables["qgAxis1"]                             = "qgAxis1"; 
  AK4Variables["qgAxis2"]                             = "qgAxis2"; 
  AK4Variables["recoJetschargedHadronEnergyFraction"] = "recoJetschargedHadronEnergyFraction"; 
  AK4Variables["recoJetschargedEmEnergyFraction"]     = "recoJetschargedEmEnergyFraction"; 
  AK4Variables["recoJetsneutralEmEnergyFraction"]     = "recoJetsneutralEmEnergyFraction"; 
  AK4Variables["recoJetsmuonEnergyFraction"]          = "recoJetsmuonEnergyFraction"; 
  AK4Variables["recoJetsHFHadronEnergyFraction"]      = "recoJetsHFHadronEnergyFraction";
  AK4Variables["recoJetsHFEMEnergyFraction"]          = "recoJetsHFEMEnergyFraction"; 
  AK4Variables["recoJetsneutralEnergyFraction"]       = "recoJetsneutralEnergyFraction"; 
  AK4Variables["PhotonEnergyFraction"]                = "PhotonEnergyFraction"; 
  AK4Variables["ElectronEnergyFraction"]              = "ElectronEnergyFraction";
  AK4Variables["ChargedHadronMultiplicity"]           = "ChargedHadronMultiplicity";
  AK4Variables["NeutralHadronMultiplicity"]           = "NeutralHadronMultiplicity";
  AK4Variables["PhotonMultiplicity"]                  = "PhotonMultiplicity"; 
  AK4Variables["ElectronMultiplicity"]                = "ElectronMultiplicity"; 
  AK4Variables["MuonMultiplicity"]                    = "MuonMultiplicity"; 
  AK4Variables["DeepCSVb"]                            = "DeepCSVb"; 
  AK4Variables["DeepCSVc"]                            = "DeepCSVc"; 
  AK4Variables["DeepCSVl"]                            = "DeepCSVl"; 
  AK4Variables["DeepCSVbb"]                           = "DeepCSVbb";     
  AK4Variables["DeepCSVcc"]                           = "DeepCSVcc";      
  
  ttUtility::ConstAK4Inputs<float> AK4Inputs(*jetsLVec_forTagger, *recoJetsBtag_forTagger);
  
  // convert qgMult to float and then add to AK4Inputs
  const std::vector<int>   &qgMult_i = tr->getVec<int>(UseCleanedJetsVar("qgMult"));
  const std::vector<float> qgMult_f(qgMult_i.begin(), qgMult_i.end());
  AK4Inputs.addSupplamentalVector("qgMult", qgMult_f);
  
  // loop over variables and add to AK4Inputs
  for (const auto& variable : AK4Variables)
  {
    //                              first: name for top tagger                          second: name in ntuple
    AK4Inputs.addSupplamentalVector(variable.first, tr->getVec<float>(UseCleanedJetsVar(variable.second)));
  }

  // ----- AK8 Jets -----

  const std::vector<TLorentzVector>              &AK8JetLV           = tr->getVec<TLorentzVector>(jetVecLabelAK8);
  const std::vector<float>                       &AK8JetSoftdropMass = tr->getVec<float>(UseCleanedJetsVar("puppisoftDropMass"));
  const std::vector<float>                       &AK8JetDeepAK8Top   = tr->getVec<float>(UseCleanedJetsVar("deepAK8btop"));
  const std::vector<float>                       &AK8JetDeepAK8W     = tr->getVec<float>(UseCleanedJetsVar("deepAK8bW"));
  const std::vector<std::vector<TLorentzVector>> &AK8SubjetLV        = tr->getVec<std::vector<TLorentzVector>>(UseCleanedJetsVar("puppiAK8SubjetLVec"));

  //Create AK8 inputs object
  ttUtility::ConstAK8Inputs<float> AK8Inputs(
      AK8JetLV,
      AK8JetDeepAK8Top,
      AK8JetDeepAK8W,
      AK8JetSoftdropMass,
      AK8SubjetLV
      );

  //Create jets constituents list combining AK4 and AK8 jets, these are used to construct top candiates
  //The vector of input constituents can also be constructed "by hand"
  std::vector<Constituent> constituents = ttUtility::packageConstituents(AK4Inputs, AK8Inputs);

  //run the top tagger
  ttPtr->runTagger(constituents);

}       // -----  end of function BaselineVessel::prepareDeepTopTagger  -----

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

  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ New TopTagger ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // top tagger
    //construct vector of constituents 
    ttUtility::ConstAK4Inputs<float> myConstAK4Inputs(*jetsLVec_forTagger, *recoJetsBtag_forTagger, *qgLikelihood_forTagger);
    ttUtility::ConstAK8Inputs<float> myConstAK8Inputs(
        tr->getVec<TLorentzVector>(UseCleanedJetsVar("puppiJetsLVec")),
        tr->getVec<float>(UseCleanedJetsVar("puppitau1")),
        tr->getVec<float>(UseCleanedJetsVar("puppitau2")),
        tr->getVec<float>(UseCleanedJetsVar("puppitau3")),
        tr->getVec<float>(UseCleanedJetsVar("puppisoftDropMass")),
        tr->getVec<std::vector<TLorentzVector>>(UseCleanedJetsVar("puppiAK8SubjetLVec")));
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
  // Specify lepton or photon jet cleaning
  if (spec.find("drLeptonCleaned") != std::string::npos)
  {
    METLabel            = "metWithLL";
    METPhiLabel         = "metphiWithLL";
    UseDeepCSV          = true; 
    UseDRPhotonCleanJet = false;
    UseDRLeptonCleanJet = true;
    doLeptonVeto        = false;
  }
  else if (spec.find("drPhotonCleaned") != std::string::npos)
  {
    METLabel            = "metWithPhoton";
    METPhiLabel         = "metphiWithPhoton";
    UseDeepCSV          = true; 
    UseDRPhotonCleanJet = true;
    UseDRLeptonCleanJet = false;
    doLeptonVeto        = true;
  }
  // Specify Jet pt cut
  if (spec.find("jetpt20") != std::string::npos)
  {
    min_jet_pt          = 20.0;
    JetCutArrary        = AnaConsts::pt20Eta24Arr;
    dPhiCutArrary       = AnaConsts::pt20Eta47Arr;
  }
  else if (spec.find("jetpt30") != std::string::npos)
  {
    min_jet_pt          = 30.0;
    JetCutArrary        = AnaConsts::pt30Eta24Arr;
    dPhiCutArrary       = AnaConsts::pt30Eta47Arr;
  }
  else if (spec.find("jetpt40") != std::string::npos)
  {
    min_jet_pt          = 40.0;
    JetCutArrary        = AnaConsts::pt40Eta24Arr;
    dPhiCutArrary       = AnaConsts::pt40Eta47Arr;
  }
  // SUS-16-050 specialization for reference 
  if(spec.compare("Zinv") == 0 || spec.compare("ZinvJEUUp") == 0 || spec.compare("ZinvJEUDn") == 0 || spec.compare("ZinvMEUUp") == 0 || spec.compare("ZinvMEUDn") == 0) 
  {
    UseDeepCSV          = true;
    UseDRPhotonCleanJet = false;
    UseDRLeptonCleanJet = true;
    doLeptonVeto        = false;
    if(spec.compare("ZinvJEUUp") == 0)
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
  else if( spec.find("jecUp") != std::string::npos || spec.find("jecDn") != std::string::npos || spec.find("metMagUp") != std::string::npos || spec.find("metMagDn") != std::string::npos || spec.find("metPhiUp") != std::string::npos || spec.find("metPhiDn") != std::string::npos ){
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
  }
  if( !printOnce ){
    printOnce = true;
    //std::cout<<"spec : "<<spec.c_str()<<"  jetVecLabel : "<<jetVecLabel.c_str() <<"  CSVVecLabel : "<<CSVVecLabel.c_str() <<"  METLabel : "<<METLabel.c_str()<< std::endl;
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
  int nWs = -1;
  int nResolvedTops = -1;
  bool passTagger = false;
  vTops = new std::vector<TLorentzVector>();
  vWs = new std::vector<TLorentzVector>();
  vResolvedTops = new std::vector<TLorentzVector>();
  mTopJets = new std::map<int, std::vector<TLorentzVector> >();

  nTopCandSortedCnt = GetnTops();
  nWs = vWs->size();
  nResolvedTops = vResolvedTops->size();
  passTagger = (incZEROtop || nTopCandSortedCnt >= AnaConsts::low_nTopCandSortedSel); 

  tr->registerDerivedVar("nTopCandSortedCnt" + firstSpec, nTopCandSortedCnt);
  tr->registerDerivedVar("nWs"+firstSpec, nWs);
  tr->registerDerivedVar("nResolvedTops"+firstSpec, nResolvedTops);
  tr->registerDerivedVec("vTops"+firstSpec, vTops);
  tr->registerDerivedVec("vWs"+firstSpec, vWs);
  tr->registerDerivedVec("mTopJets"+firstSpec, mTopJets);

  return passTagger;
}       // -----  end of function BaselineVessel::PassTopTagger  -----


// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::PrintoutConfig
//  Description:  
// ===========================================================================
bool BaselineVessel::PrintoutConfig() const
{
  if (!tr->isFirstEvent()) return false;
  
  std::cout << "=== Config for BaselineVessel ===" << std::endl;
  std::cout << "|   Year           : " << year                                      << std::endl;
  std::cout << "|   Specialization : " << spec                                      << std::endl;
  std::cout << "|   AK4Jet Label   : " << jetVecLabel                               << std::endl;
  std::cout << "|   b-tag Label    : " << CSVVecLabel                               << std::endl;
  std::cout << "|   b-tag WP       : " << AnaConsts::DeepCSV.at(year).at("cutM")    << std::endl;
  std::cout << "|   top-tag config : " << toptaggerCfgFile                          << std::endl;
  std::cout << "|   MET Label      : " << METLabel                                  << std::endl;
  std::cout << "|   MET phi Label  : " << METPhiLabel                               << std::endl;
  std::cout << "=================================" << std::endl;
  return true;
}       // -----  end of function BaselineVessel::PrintoutConfig  -----

void BaselineVessel::PassBaseline()
{
  if (printConfig) PrintoutConfig();
  bool verbose = false;
  
  // Get jet collection
  const auto& Jets        = tr->getVec<TLorentzVector>(jetVecLabel);
  const auto& FatJets     = tr->getVec<TLorentzVector>(jetVecLabelAK8);
  const auto& met         = tr->getVar<float>(METLabel); 
  const auto& metphi      = tr->getVar<float>(METPhiLabel); 


  // lepton vetos
  int nElectrons_Stop0l = 0;
  int nMuons_Stop0l     = 0;
  int nIsoTracks_Stop0l = 0;
  const auto& Electron_Stop0l   = tr->getVec<unsigned char>("Electron_Stop0l");
  const auto& Muon_Stop0l       = tr->getVec<unsigned char>("Muon_Stop0l");
  const auto& IsoTrack_Stop0l   = tr->getVec<unsigned char>("IsoTrack_Stop0l");
  bool Pass_ElecVeto   = tr->getVar<bool>("Pass_ElecVeto");
  bool Pass_MuonVeto   = tr->getVar<bool>("Pass_MuonVeto");
  bool Pass_IsoTrkVeto = tr->getVar<bool>("Pass_IsoTrkVeto");
  for (const auto& pass : Electron_Stop0l)  if(pass) ++nElectrons_Stop0l;
  for (const auto& pass : Muon_Stop0l)      if(pass) ++nMuons_Stop0l;
  for (const auto& pass : IsoTrack_Stop0l)  if(pass) ++nIsoTracks_Stop0l;

  // Set TLorentzVector for MET
  metLVec.SetPtEtaPhiM(tr->getVar<float>(METLabel), 0, tr->getVar<float>(METPhiLabel), 0);
  // Calculate deltaPhi
  std::vector<float> * dPhiVec = new std::vector<float>();
  (*dPhiVec) = AnaFunctions::calcDPhi(Jets, metLVec, 5, dPhiCutArrary);
  // more vars
  int nJets     = AnaFunctions::countJets(Jets,     JetCutArrary);  
  int nFatJets  = AnaFunctions::countJets(FatJets,  AnaConsts::pt200Eta24Arr);  
  float HT      = AnaFunctions::calcHT(Jets,        JetCutArrary);
  float S_met   = met / sqrt(HT);

  //---------------------------------------//
  //--- Updated Baseline (January 2019) ---//
  //---------------------------------------//

  // low dm and high dm baselines from Hui Wang, branch hui_new_tagger
  // https://github.com/susy2015/SusyAnaTools/blob/hui_new_tagger/Tools/tupleRead.C#L629-L639
  // https://github.com/susy2015/SusyAnaTools/blob/5e4f54e1aa985daff90f1ad7a220b8d17e4b7290/Tools/tupleRead.C#L629-L639
  
  // variables for SAT_Pass_lowDM and SAT_Pass_highDM
  
  const auto& event             = tr->getVar<unsigned long long>("event");
  const auto& nMergedTops       = tr->getVar<int>(UseCleanedJetsVar("nMergedTops"));
  const auto& nResolvedTops     = tr->getVar<int>(UseCleanedJetsVar("nResolvedTops"));
  const auto& nWs               = tr->getVar<int>(UseCleanedJetsVar("nWs"));
  const auto& nBottoms          = tr->getVar<int>(UseCleanedJetsVar("nBottoms"));
  const auto& mtb               = tr->getVar<float>(UseCleanedJetsVar("mtb"));
  const auto& ptb               = tr->getVar<float>(UseCleanedJetsVar("ptb"));
  const auto& ISRJetPt          = tr->getVar<float>(UseCleanedJetsVar("ISRJetPt"));
  const auto& ISRJetIdx         = tr->getVar<int>(UseCleanedJetsVar("ISRJetIdx"));
  const auto& nSoftBottoms      = tr->getVar<int>("Stop0l_nSoftb");;
  const auto& Stop0l_ISRJetPt   = tr->getVar<float>("Stop0l_ISRJetPt");
  const auto& Stop0l_ISRJetIdx  = tr->getVar<int>("Stop0l_ISRJetIdx");
  const auto& Stop0l_Mtb        = tr->getVar<float>("Stop0l_Mtb");
  const auto& Stop0l_Ptb        = tr->getVar<float>("Stop0l_Ptb");
  const auto& Stop0l_nTop       = tr->getVar<int>("Stop0l_nTop");
  const auto& Stop0l_nResolved  = tr->getVar<int>("Stop0l_nResolved");
  const auto& Stop0l_nW         = tr->getVar<int>("Stop0l_nW");
  
  bool SAT_Pass_MET         = (met >= 250);
  bool SAT_Pass_MET_Mid     = (met >= 160);
  bool SAT_Pass_MET_Loose   = (met >= 110);
  bool SAT_Pass_MET_Tight   = (met >= 300);
  bool SAT_Pass_HT          = (HT  >= 300);
  bool SAT_Pass_HT_Mid      = (HT  >= 200);
  bool SAT_Pass_HT_Loose    = (HT  >= 100);
  bool SAT_Pass_NJets     = nJets >= 2;
  bool SAT_Pass_LeptonVeto  = (Pass_ElecVeto && Pass_MuonVeto && Pass_IsoTrkVeto);
  bool SAT_Pass_JetID       = tr->getVar<bool>("SAT_Pass_JetID"+firstSpec);
  bool SAT_Pass_EventFilter = tr->getVar<bool>("SAT_Pass_EventFilter"+firstSpec);
  bool Pass_JetID           = tr->getVar<bool>("Pass_JetID");
  bool Pass_EventFilter     = tr->getVar<bool>("Pass_EventFilter");
  bool Pass_LeptonVeto      = tr->getVar<bool>("Pass_LeptonVeto");
  bool Pass_CaloMETRatio    = tr->getVar<bool>("Pass_CaloMETRatio"); //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (Pass_LeptonVeto != SAT_Pass_LeptonVeto) std::cout << "ERROR: Lepton vetos do not agree. Pass_LeptonVeto=" << Pass_LeptonVeto << " and SAT_Pass_LeptonVeto=" << SAT_Pass_LeptonVeto << std::endl;
 
  // get ISR jet
  TLorentzVector ISRJet;
  if (ISRJetIdx >= 0 && ISRJetIdx < FatJets.size()) ISRJet = FatJets[ISRJetIdx];
  
  //SUS-16-049, low dm, ISR cut
  // see GetISRJetIdx() and CalcISRJetVars() for details
  bool SAT_Pass_ISR         = (ISRJetPt >= 200);
  bool SAT_Pass_ISR_Tight   = (ISRJetPt >= 300);
  bool SAT_Pass_S_MET       = (S_met >= 10);
  
  // ----------------------- // 
  // --- For Search Bins --- //
  // ----------------------- // 
  
  //SUS-16-049, low dm, mtb cut
  bool SAT_Pass_MTB_LowDM = (nBottoms == 0 || (nBottoms > 0 && mtb < 175));  

  //SUS-16-049, low dm, dphi(met, j1) > 0.5, dphi(met, j23) > 0.15
  bool SAT_Pass_dPhiMETLowDM = ( 
                                    (dPhiVec->size() == 2 && dPhiVec->at(0) > 0.5 && dPhiVec->at(1) > 0.15)
                                 || (dPhiVec->size()  > 2 && dPhiVec->at(0) > 0.5 && dPhiVec->at(1) > 0.15 && dPhiVec->at(2) > 0.15)
                               );
  //SUS-16-049, high dm, dphi(met, jet1234) > 0.5
  bool SAT_Pass_dPhiMETHighDM = (
                                     dPhiVec->size() >= 4 
                                  && dPhiVec->at(0) > 0.5 
                                  && dPhiVec->at(1) > 0.5 
                                  && dPhiVec->at(2) > 0.5 
                                  && dPhiVec->at(3) > 0.5
                                );
  
  // --------------------------- // 
  // --- For Validation Bins --- //
  // --------------------------- // 
  // Low dm validation bins
  // use normal low dm selection
  
  // Low dm high met validation bins
  // use low dm selection but replace the dPhi cut in low dm by mid dPhi cut:
  // !(min( dPhi[jet12(3),MET] ) < 0.15) && (!tr.getVar<bool>("Pass_dPhiMET"))
  
  // High dm validation bins
  // Cut: replace the dPhi cut in high dm by mid dPhi cut:
  // tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM"))
  
  bool SAT_Pass_mid_dPhiMETLowDM = (
                                        (dPhiVec->size() == 2 && (dPhiVec->at(0) < 0.15 || dPhiVec->at(1) < 0.15))
                                     || (dPhiVec->size()  > 2 && (dPhiVec->at(0) < 0.15 || dPhiVec->at(1) < 0.15 || dPhiVec->at(2) < 0.15))
                                   );
  
  SAT_Pass_mid_dPhiMETLowDM  = ! SAT_Pass_mid_dPhiMETLowDM && ! SAT_Pass_dPhiMETLowDM;
  bool SAT_Pass_mid_dPhiMETHighDM = SAT_Pass_dPhiMETLowDM && ! SAT_Pass_dPhiMETHighDM;
  
  // ----------------------- // 
  // --- For Search Bins --- //
  // ----------------------- // 
  
  // standard baseline
  SAT_Pass_Baseline = (
                            SAT_Pass_JetID 
                         && SAT_Pass_EventFilter
                         && SAT_Pass_MET 
                         && SAT_Pass_HT
                         && SAT_Pass_NJets
                         && SAT_Pass_dPhiMETLowDM
              //           && Pass_CaloMETRatio
                      );
  
  //baseline for SUS-16-049 low dm plus HT cut
  SAT_Pass_lowDM = (
                        SAT_Pass_Baseline
                     && nMergedTops == 0
                     && nResolvedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && SAT_Pass_S_MET
                     && SAT_Pass_MTB_LowDM
                   );      
  
  
  //baseline for SUS-16-049 high dm plus HT cut
  SAT_Pass_highDM = (
                         SAT_Pass_Baseline
                      && SAT_Pass_dPhiMETHighDM
                      && nBottoms >= 1
                      && nJets >= 5
                    );      

  //baseline for shapeFactor calculation (Mid)
  SAT_Pass_lowDM_Mid = (
                        SAT_Pass_JetID
                     && SAT_Pass_EventFilter
                     && SAT_Pass_MET_Mid
                     && SAT_Pass_HT_Mid
                     && SAT_Pass_NJets
                     && SAT_Pass_dPhiMETLowDM
                     && nMergedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && S_met > 10
                     && SAT_Pass_MTB_LowDM
            //         && Pass_CaloMETRatio
                  );

  //baseline for shapeFactor calculation (Mid)
  SAT_Pass_highDM_Mid = (
                        SAT_Pass_JetID
                     && SAT_Pass_EventFilter
                     && SAT_Pass_MET_Mid
                     && SAT_Pass_HT_Mid
                     && SAT_Pass_NJets
                     && SAT_Pass_dPhiMETHighDM
                     && nBottoms >= 1
                     && nJets >= 5
            //         && Pass_CaloMETRatio
                  );      

  //baseline for shapeFactor calculation (Loose)
  SAT_Pass_lowDM_Loose = (
                        SAT_Pass_JetID
                     && SAT_Pass_EventFilter
                     && SAT_Pass_MET_Loose
                     && SAT_Pass_HT_Loose
                     && SAT_Pass_NJets
                     && SAT_Pass_dPhiMETLowDM
                     && nMergedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && S_met > 10
                     && SAT_Pass_MTB_LowDM
            //         && Pass_CaloMETRatio
                  );

  //baseline for shapeFactor calculation (Loose)
  SAT_Pass_highDM_Loose = (
                         SAT_Pass_JetID 
                      && SAT_Pass_EventFilter
                      && SAT_Pass_MET_Loose
                      && SAT_Pass_HT_Loose
                      && SAT_Pass_NJets
                      && SAT_Pass_dPhiMETHighDM
                      && nBottoms >= 1
                      && nJets >= 5
            //          && Pass_CaloMETRatio                   
                  );      

  // standard baseline
  SAT_Pass_Baseline_Tight = (
                            SAT_Pass_JetID 
                         && SAT_Pass_EventFilter
                         && SAT_Pass_MET_Tight
                         && SAT_Pass_HT
                         && SAT_Pass_NJets
                         && SAT_Pass_dPhiMETLowDM
                      );
  
  //low dm with tigher MET and ISR PT cuts
  SAT_Pass_lowDM_Tight = (
                        SAT_Pass_Baseline_Tight
                     && nMergedTops == 0
                     && nResolvedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR_Tight
                     && SAT_Pass_S_MET
                     && SAT_Pass_MTB_LowDM
                   );      
  
  // ----------------------------------- // 
  // --- Apply Lepton Veto if needed --- //
  // ----------------------------------- // 
  if (doLeptonVeto)
  {
      SAT_Pass_lowDM        = SAT_Pass_lowDM         && Pass_LeptonVeto;
      SAT_Pass_highDM       = SAT_Pass_highDM        && Pass_LeptonVeto;
      SAT_Pass_lowDM_Mid    = SAT_Pass_lowDM_Mid     && Pass_LeptonVeto;
      SAT_Pass_highDM_Mid   = SAT_Pass_highDM_Mid    && Pass_LeptonVeto;
      SAT_Pass_lowDM_Loose  = SAT_Pass_lowDM_Loose   && Pass_LeptonVeto;
      SAT_Pass_highDM_Loose = SAT_Pass_highDM_Loose  && Pass_LeptonVeto;
  }
  
  // --------------------------- // 
  // --- For Validation Bins --- //
  // --------------------------- // 
  // without dPhi
  bool SAT_Pass_Baseline_no_dPhi = (
                            SAT_Pass_JetID 
                         && SAT_Pass_EventFilter
                         && SAT_Pass_MET 
                         && SAT_Pass_HT
                         && SAT_Pass_NJets
                      );
  //baseline for SUS-16-049 low dm plus HT cut
  bool SAT_Pass_lowDM_mid_dPhi = (
                        SAT_Pass_Baseline_no_dPhi
                     && SAT_Pass_mid_dPhiMETLowDM 
                     && nMergedTops == 0
                     && nResolvedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && SAT_Pass_S_MET
                     && SAT_Pass_MTB_LowDM
                   );      
  
  //baseline for SUS-16-049 high dm plus HT cut
  bool SAT_Pass_highDM_mid_dPhi = (
                         SAT_Pass_Baseline_no_dPhi
                      && SAT_Pass_mid_dPhiMETHighDM 
                      && nBottoms >= 1
                      && nJets >= 5
                    );      

  //Loose version
  bool SAT_Pass_Baseline_no_dPhi_Loose = (
                            SAT_Pass_JetID 
                         && SAT_Pass_EventFilter
                         && SAT_Pass_MET_Loose 
                         && SAT_Pass_HT
                         && SAT_Pass_NJets
                      );

  //baseline for SUS-16-049 low dm plus HT cut
  bool SAT_Pass_lowDM_mid_dPhi_Mid = (
                        SAT_Pass_JetID 
                     && SAT_Pass_EventFilter
                     && SAT_Pass_MET_Mid
                     && SAT_Pass_HT_Mid
                     && SAT_Pass_NJets
                     && SAT_Pass_mid_dPhiMETLowDM 
                     && nMergedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && SAT_Pass_S_MET
                     && SAT_Pass_MTB_LowDM
                   );      
  //baseline for SUS-16-049 high dm plus HT cut
  bool SAT_Pass_highDM_mid_dPhi_Mid = (
                         SAT_Pass_JetID 
                      && SAT_Pass_EventFilter
                      && SAT_Pass_MET_Mid
                      && SAT_Pass_HT_Mid
                      && SAT_Pass_NJets
                      && SAT_Pass_mid_dPhiMETHighDM 
                      && nBottoms >= 1
                      && nJets >= 5
                    );      

  //Mid version
  bool SAT_Pass_Baseline_no_dPhi_Mid = (
                            SAT_Pass_JetID 
                         && SAT_Pass_EventFilter
                         && SAT_Pass_MET_Mid 
                         && SAT_Pass_HT
                         && SAT_Pass_NJets
                      );
  //baseline for SUS-16-049 low dm plus HT cut
  bool SAT_Pass_lowDM_mid_dPhi_Loose = (
                        SAT_Pass_JetID 
                     && SAT_Pass_EventFilter
                     && SAT_Pass_MET_Loose
                     && SAT_Pass_HT_Loose
                     && SAT_Pass_NJets
                     && SAT_Pass_mid_dPhiMETLowDM 
                     && nMergedTops == 0
                     && nWs == 0
                     && SAT_Pass_ISR
                     && SAT_Pass_S_MET
                     && SAT_Pass_MTB_LowDM
                   );      
  //baseline for SUS-16-049 high dm plus HT cut
  bool SAT_Pass_highDM_mid_dPhi_Loose = (
                         SAT_Pass_JetID 
                      && SAT_Pass_EventFilter
                      && SAT_Pass_MET_Loose
                      && SAT_Pass_HT_Loose
                      && SAT_Pass_NJets
                      && SAT_Pass_mid_dPhiMETHighDM 
                      && nBottoms >= 1
                      && nJets >= 5
                    );      

  // ----------------------------------- // 
  // --- Apply Lepton Veto if needed --- //
  // ----------------------------------- // 
  if (doLeptonVeto)
  {
      SAT_Pass_lowDM_mid_dPhi         =  SAT_Pass_lowDM_mid_dPhi  && Pass_LeptonVeto;
      SAT_Pass_highDM_mid_dPhi        =  SAT_Pass_highDM_mid_dPhi && Pass_LeptonVeto;
      SAT_Pass_lowDM_mid_dPhi_Mid     =  SAT_Pass_lowDM_mid_dPhi_Mid  && Pass_LeptonVeto;
      SAT_Pass_highDM_mid_dPhi_Mid    =  SAT_Pass_highDM_mid_dPhi_Mid && Pass_LeptonVeto;
      SAT_Pass_lowDM_mid_dPhi_Loose   =  SAT_Pass_lowDM_mid_dPhi_Loose  && Pass_LeptonVeto;
      SAT_Pass_highDM_mid_dPhi_Loose  =  SAT_Pass_highDM_mid_dPhi_Loose && Pass_LeptonVeto;
  }
  
  // ------------------------------ //
  // --- print info for testing --- //
  // ------------------------------ //
  //unsigned long long CMS_event = tr->getVar<unsigned long long>("event");
  //printf("CMS event: %d ntuple event: %d\n", CMS_event, tr->getEvtNum());
  //if (tr->getEvtNum() == 7217)
  //if (CMS_event == 519215141)
  if (false)
  {
    printf("CMS event: ntuple event: %d; %d\n", event, tr->getEvtNum());
    printf("SAT_Pass_dPhiMETLowDM: %d\n", SAT_Pass_dPhiMETLowDM);
    printf("SAT_Pass_dPhiMETHighDM: %d\n", SAT_Pass_dPhiMETHighDM);
    // dPhi
    printf("dPhi_0: %f ",           dPhiVec->at(0));
    printf("dPhi_1: %f ",           dPhiVec->at(1));
    printf("dPhi_2: %f ",           dPhiVec->at(2));
    printf("dPhi_3: %f ",           dPhiVec->at(3));
    printf("met = %f ",             met);
    printf("metphi = %f ",          metphi);
    printf("HT = %f ",              HT);
    printf("mtb = %f ",             mtb);
    printf("ptb = %f ",             ptb);
    printf("ISRJetPt = %f ",        ISRJetPt);
    printf("S_met = %f ",           S_met);
    printf("nJets = %d ",           nJets);
    printf("nMergedTops = %d ",     nMergedTops);
    printf("nBottoms = %d ",        nBottoms);
    printf("nWs = %d ",             nWs);
    printf("\n");
    int i = 0;
    for (const auto& Jet : Jets)
    {
      printf("Jet %d: pt=%f, eta=%f, phi=%f, mass=%f\n", i, Jet.Pt(), Jet.Eta(), Jet.Phi(), Jet.M());
      ++i;
    }
  }

  // compare original variables to those with cleaned jets
  if (verbose)
  {
    // ISR jet variables
    printf("nFatJets: %d ",         nFatJets);
    printf("Stop0l_nTop: %d ",      Stop0l_nTop);
    printf("nMergedTops: %d ",      nMergedTops);
    printf("Stop0l_nResolved: %d ", Stop0l_nResolved);
    printf("nResolvedTops: %d ",    nResolvedTops);
    printf("Stop0l_nW: %d ",        Stop0l_nW);
    printf("nWs: %d ",              nWs);
    printf("Stop0l_ISRJetPt: %f ",  Stop0l_ISRJetPt);
    printf("ISRJetPt: %f ",         ISRJetPt);
    printf("Stop0l_ISRJetIdx: %d ", Stop0l_ISRJetIdx);
    printf("ISRJetIdx: %d ",        ISRJetIdx);
    printf("SAT_Pass_ISR: %d ",     SAT_Pass_ISR);
    // mtb and ptb variables
    printf("met = %f, metphi = %f\n",     met,        metphi);
    printf("Stop0l_Mtb = %f, mtb = %f\n", Stop0l_Mtb, mtb);
    printf("Stop0l_Ptb = %f, ptb = %f\n", Stop0l_Ptb, ptb);
    printf("\n");
  }

  // Register all the calculated variables
  tr->registerDerivedVec("dPhiVec" + firstSpec, dPhiVec);
  tr->registerDerivedVar("ISRJet" + firstSpec, ISRJet);
  tr->registerDerivedVar("nSoftBottoms" + firstSpec, nSoftBottoms);
  tr->registerDerivedVar("nJets" + firstSpec, nJets);
  tr->registerDerivedVar("nFatJets" + firstSpec, nFatJets);
  tr->registerDerivedVar("nElectrons_Stop0l" + firstSpec, nElectrons_Stop0l);
  tr->registerDerivedVar("nMuons_Stop0l" + firstSpec, nMuons_Stop0l);
  tr->registerDerivedVar("nIsoTracks_Stop0l" + firstSpec, nIsoTracks_Stop0l);
  tr->registerDerivedVar("HT" + firstSpec, HT);
  tr->registerDerivedVar("S_met" + firstSpec, S_met);
  tr->registerDerivedVar("SAT_Pass_MET" + firstSpec, SAT_Pass_MET);
  tr->registerDerivedVar("SAT_Pass_HT" + firstSpec, SAT_Pass_HT);
  tr->registerDerivedVar("SAT_Pass_NJets" + firstSpec, SAT_Pass_NJets);
  tr->registerDerivedVar("SAT_Pass_ISR" + firstSpec, SAT_Pass_ISR);
  tr->registerDerivedVar("SAT_Pass_S_MET" + firstSpec, SAT_Pass_S_MET);
  tr->registerDerivedVar("SAT_Pass_MTB_LowDM" + firstSpec, SAT_Pass_MTB_LowDM);
  tr->registerDerivedVar("SAT_Pass_Baseline"  + firstSpec, SAT_Pass_Baseline);
  tr->registerDerivedVar("SAT_Pass_dPhiMETLowDM" + firstSpec,  SAT_Pass_dPhiMETLowDM);
  tr->registerDerivedVar("SAT_Pass_dPhiMETHighDM" + firstSpec, SAT_Pass_dPhiMETHighDM);
  tr->registerDerivedVar("SAT_Pass_mid_dPhiMETLowDM" + firstSpec,  SAT_Pass_mid_dPhiMETLowDM);
  tr->registerDerivedVar("SAT_Pass_mid_dPhiMETHighDM" + firstSpec, SAT_Pass_mid_dPhiMETHighDM);
  tr->registerDerivedVar("SAT_Pass_lowDM"  + firstSpec, SAT_Pass_lowDM);
  tr->registerDerivedVar("SAT_Pass_highDM" + firstSpec, SAT_Pass_highDM);
  tr->registerDerivedVar("SAT_Pass_lowDM_Mid"  + firstSpec, SAT_Pass_lowDM_Mid);
  tr->registerDerivedVar("SAT_Pass_highDM_Mid" + firstSpec, SAT_Pass_highDM_Mid);
  tr->registerDerivedVar("SAT_Pass_lowDM_Loose"  + firstSpec, SAT_Pass_lowDM_Loose);
  tr->registerDerivedVar("SAT_Pass_highDM_Loose" + firstSpec, SAT_Pass_highDM_Loose);
  tr->registerDerivedVar("SAT_Pass_lowDM_Tight"  + firstSpec, SAT_Pass_lowDM_Tight);
  tr->registerDerivedVar("SAT_Pass_lowDM_mid_dPhi"  + firstSpec, SAT_Pass_lowDM_mid_dPhi);
  tr->registerDerivedVar("SAT_Pass_highDM_mid_dPhi"  + firstSpec, SAT_Pass_highDM_mid_dPhi);
  tr->registerDerivedVar("SAT_Pass_lowDM_mid_dPhi_Mid"  + firstSpec, SAT_Pass_lowDM_mid_dPhi_Mid);
  tr->registerDerivedVar("SAT_Pass_highDM_mid_dPhi_Mid"  + firstSpec, SAT_Pass_highDM_mid_dPhi_Mid);
  tr->registerDerivedVar("SAT_Pass_lowDM_mid_dPhi_Loose"  + firstSpec, SAT_Pass_lowDM_mid_dPhi_Loose);
  tr->registerDerivedVar("SAT_Pass_highDM_mid_dPhi_Loose"  + firstSpec, SAT_Pass_highDM_mid_dPhi_Loose);
} 


int BaselineVessel::GetnTops() const
{
  //get output of tagger
  const TopTaggerResults& ttr = ttPtr->getResults();
  //Use result for top var
  std::vector<TopObject*> Ntop = ttr.getTops();  
  unsigned int topidx = 0;

  for(unsigned int it=0; it<Ntop.size(); it++)
  {
    TopObject::Type  type = Ntop.at(it)->getType() ;
    if ( type == TopObject::Type::MERGED_TOP
    || type   == TopObject::Type::SEMIMERGEDWB_TOP
    || type   == TopObject::Type::RESOLVED_TOP)
    {
      vTops->push_back(Ntop.at(it)->P());
      std::vector<TLorentzVector> temp;
      for(auto j : Ntop.at(it)->getConstituents())
      {
        temp.push_back(j->P());
      }
      mTopJets->insert(std::make_pair(topidx++, temp));
    }
    if ( type == TopObject::Type::MERGED_W ) vWs->push_back(Ntop.at(it)->P());
    if ( type == TopObject::Type::RESOLVED_TOP ) vResolvedTops->push_back(Ntop.at(it)->P());
  }

  int nTops = vTops->size();
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
      tr->getVec<TLorentzVector>(UseCleanedJetsVar("puppiJetsLVec")),
      tr->getVec<float>(UseCleanedJetsVar("puppitau1")),
      tr->getVec<float>(UseCleanedJetsVar("puppitau2")),
      tr->getVec<float>(UseCleanedJetsVar("puppitau3")),
      tr->getVec<float>(UseCleanedJetsVar("puppisoftDropMass")),
      tr->getVec<std::vector<TLorentzVector>>(UseCleanedJetsVar("puppiAK8SubjetLVec")));
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


  GetWAlone();
  GetISRJet();
  return true;
}       // -----  end of function BaselineVessel::FlagAK8Jets  -----

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::FlagDeepAK8Jets
//  Description:  
// ===========================================================================
bool BaselineVessel::FlagDeepAK8Jets()
{
  const std::vector<TLorentzVector> &ak8s =  tr->getVec<TLorentzVector>(UseCleanedJetsVar("puppiJetsLVec"));
  const std::vector<float> &btops =  tr->getVec<float>(UseCleanedJetsVar("deepAK8btop"));
  const std::vector<float> &bWs =  tr->getVec<float>(UseCleanedJetsVar("deepAK8bW"));
  vAK8Flag = new std::vector<unsigned>(ak8s.size(), NoTag);
  
  std::vector<TLorentzVector> topjets;
  // Cross check with top tagger
  for(auto tops : *mTopJets)
  {
    if (tops.second.size() == 1)
      topjets.push_back(tops.second.front());
  }

  for (unsigned int i = 0; i < ak8s.size(); ++i)
  {
    bool matched = false;
    for(auto t : topjets)
    {
      if (t == ak8s.at(i))
      {
        vAK8Flag->at(i) = TopTag;
        matched = true;
        break;
      }
    }
    if (matched) continue;
    for(auto t : *vWs)
    {
      if (t == ak8s.at(i))
      {
        vAK8Flag->at(i) = WTag;
        matched = true;
        break;
      }
    }
  }

  for (unsigned int i = 0; i < ak8s.size(); ++i)
  {
    if (vAK8Flag->at(i) != NoTag) continue;
    vAK8Flag->at(i) = FlagAK8DeepFromCSV(i);
  }

  GetISRJet();
  return true;
}       // -----  end of function BaselineVessel::FlagDeepAK8Jets  -----

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
//         Name:  BaselineVessel::FlagAK8DeepFromCSV
//  Description:  
// ===========================================================================
AK8Flag BaselineVessel::FlagAK8DeepFromCSV(unsigned int AK8index) const
{
  unsigned loosebcnt =0 ;
  unsigned mediumbcnt = 0;

  const std::vector<std::vector<TLorentzVector> > &subjets = tr->getVec<std::vector<TLorentzVector> >(UseCleanedJetsVar("puppiAK8SubjetLVec"));
  const std::vector<TLorentzVector> &jets = tr->getVec<TLorentzVector>(jetVecLabel);
  const std::vector<float> &CSV = tr->getVec<float>(CSVVecLabel);

  for(auto sub : subjets.at(AK8index))
  {
    for(unsigned int ij=0; ij<jets.size(); ij++)
    {
      if (sub.DeltaR(jets.at(ij)) < 0.4)
      {
        if (jets.at(ij).Pt() < 20 || fabs(jets.at(ij).Eta()) > 2.4) continue;
        if (UseDeepCSV)
        {
          if (CSV.at(ij) > AnaConsts::DeepCSV.at(year).at("cutM")) mediumbcnt ++;
          if (CSV.at(ij) > AnaConsts::DeepCSV.at(year).at("cutL")) loosebcnt ++;
        }
        else{
          if (CSV.at(ij) > AnaConsts::CSVv2.at(year).at("cutM")) mediumbcnt ++;
          if (CSV.at(ij) > AnaConsts::CSVv2.at(year).at("cutM")) loosebcnt ++;
        }
      }
    }
    
  }

  if (mediumbcnt > 0 ) return NoTagMediumb;
  if (loosebcnt > 0 ) return NoTagLooseb;
  return NoTagNob;
}       // -----  end of function BaselineVessel::FlagAK8DeepFromCSV  -----

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
  const std::vector<TLorentzVector> &AK8 = tr->getVec<TLorentzVector>(UseCleanedJetsVar("puppiJetsLVec"));
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
  const std::vector<TLorentzVector> &AK8 = tr->getVec<TLorentzVector>(UseCleanedJetsVar("puppiJetsLVec"));
  std::vector<TLorentzVector> *ISRJet = new std::vector<TLorentzVector>();
  std::map<float, TLorentzVector> ISRJetAll;

  for(unsigned int i=0; i < vAK8Flag->size(); ++i)
  {
    if (vAK8Flag->at(i) == NoTagNob)
    {
      float pt = AK8.at(i).Pt();
      if (pt > 200 && fabs(AK8.at(i).Eta()) < 2.4
          && fabs(TVector2::Phi_mpi_pi(AK8.at(i).Phi()- tr->getVar<float>(METPhiLabel))) > 2)
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

  std::vector<float> *vDeepResCand = new std::vector<float>();
  std::vector<float> *vDeepTopCand = new std::vector<float>();
  std::vector<float> *vDeepWCand = new std::vector<float>();
  //get output of tagger
  //Only MVA combs so far
  const TopTaggerResults& ttr = ttPtr->getResults();
  int i = 0;
  std::vector<TLorentzVector> temp;
  for(auto topr : ttr.getTopCandidates() )
  {
    vCombs->push_back(topr.P());
    temp.clear();
    const std::vector<const Constituent*>& constituents =  topr.getConstituents();
    for(auto cons : constituents)
    {
      //std::cout << " top score? "  << cons->getTopDisc() << std::endl;
      temp.push_back(cons->P());
    }
    if (constituents.size() == 3)
    {
      vDeepResCand->push_back( topr.getDiscriminator() );
    }
    if (constituents.size() == 1)
    {
      vDeepTopCand->push_back(constituents.front()->getTopDisc());
    }
    vCombJets->insert(std::make_pair(i, temp));
    i++;
  }


  tr->registerDerivedVec("vCombs"+spec, vCombs);
  tr->registerDerivedVec("mCombJets"+spec, vCombJets);
  tr->registerDerivedVec("vDeepResCand"+spec, vDeepResCand);
  tr->registerDerivedVec("vDeepTopCand"+spec, vDeepTopCand);

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
      int goodVerticesFilter = true;
      if (tr->checkBranch("goodVerticesFilter"))
      {
          goodVerticesFilter = tr->getVar<int>("goodVerticesFilter");
      }
      // new filters
      //const int & globalTightHalo2016Filter = tr->getVar<int>("globalTightHalo2016Filter");
      //bool passGlobalTightHalo2016Filter = (&globalTightHalo2016Filter) != nullptr? tr->getVar<int>("globalTightHalo2016Filter") !=0 : true;
      bool globalTightHalo2016Filter = true;
      if (tr->checkBranch("globalTightHalo2016Filter"))
      {
          globalTightHalo2016Filter = tr->getVar<bool>("globalTightHalo2016Filter");
      }
      bool passGlobalTightHalo2016Filter = globalTightHalo2016Filter;

      //int eeBadScFilter = tr->getVar<int>("eeBadScFilter");
      int eeBadScFilter = true;

      passDataSpec = goodVerticesFilter && eeBadScFilter && passGlobalTightHalo2016Filter;
    }

    bool hbheNoiseFilter = isfastsim? 1:tr->getVar<bool>("Flag_HBHENoiseFilter");
    bool hbheIsoNoiseFilter = isfastsim? 1:tr->getVar<bool>("Flag_HBHENoiseIsoFilter");
    bool ecalTPFilter = tr->getVar<bool>("Flag_EcalDeadCellTriggerPrimitiveFilter");

    //bool jetIDFilter = isfastsim? 1:tr->getVar<bool>("AK4looseJetID");
    //bool jetIDFilter = true;
    // new filters

    bool BadPFMuonFilter = true;
    std::string Flag_BadPFMuonFilter_type;
    tr->getType("Flag_BadPFMuonFilter", Flag_BadPFMuonFilter_type);
    if (Flag_BadPFMuonFilter_type.compare("bool") == 0 || Flag_BadPFMuonFilter_type.compare("Bool_t") == 0)
    {
        BadPFMuonFilter = tr->getVar<bool>("Flag_BadPFMuonFilter");
    }
    else if (Flag_BadPFMuonFilter_type.compare("char") == 0 || Flag_BadPFMuonFilter_type.compare("UChar_t") == 0)
    {
        BadPFMuonFilter = bool(tr->getVar<char>("Flag_BadPFMuonFilter"));
    }

    //const auto& BadPFMuonFilter = bool(tr->getVar<char>("Flag_BadPFMuonFilter"));
    //const auto& BadPFMuonFilter = bool(tr->getVar<bool>("Flag_BadPFMuonFilter"));
    bool passBadPFMuonFilter = &BadPFMuonFilter != nullptr ? BadPFMuonFilter : true;

    bool BadChargedCandidateFilter = true;
    std::string Flag_BadChargedCandidateFilter_type;
    tr->getType("Flag_BadChargedCandidateFilter", Flag_BadChargedCandidateFilter_type);
    if (Flag_BadChargedCandidateFilter_type.compare("bool") == 0 || Flag_BadChargedCandidateFilter_type.compare("Bool_t") == 0)
    {
        BadChargedCandidateFilter = tr->getVar<bool>("Flag_BadChargedCandidateFilter");
    }
    else if (Flag_BadChargedCandidateFilter_type.compare("char") == 0 || Flag_BadChargedCandidateFilter_type.compare("UChar_t") == 0)
    {
        BadChargedCandidateFilter = bool(tr->getVar<char>("Flag_BadChargedCandidateFilter"));
    }
    //const auto& BadChargedCandidateFilter = bool(tr->getVar<char>("Flag_BadChargedCandidateFilter"));
    bool passBadChargedCandidateFilter = &BadChargedCandidateFilter != nullptr ? BadChargedCandidateFilter : true;

    //bool passMETratioFilter = tr->getVar<float>("calomet")!=0 ? tr->getVar<float>("met")/tr->getVar<float>("calomet") < 5 : true;

    tr->setReThrow(cached_rethrow);
    //return passDataSpec && hbheNoiseFilter && hbheIsoNoiseFilter && ecalTPFilter && jetIDFilter && passBadPFMuonFilter && passBadChargedCandidateFilter && passMETratioFilter;
    return passDataSpec && hbheNoiseFilter && hbheIsoNoiseFilter && ecalTPFilter && passBadPFMuonFilter && passBadChargedCandidateFilter;
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
  std::vector<TLorentzVector> jetsLVec = tr->getVec<TLorentzVector>(jetVecLabel);
  std::vector<float> recoJetsmuonEnergyFraction = tr->getVec<float>("recoJetsmuonEnergyFraction");
  metLVec.SetPtEtaPhiM(tr->getVar<float>(METLabel), 0, tr->getVar<float>(METPhiLabel), 0);

  int nJetsLoop = recoJetsmuonEnergyFraction.size();
  std::vector<float> dPhisVec = AnaFunctions::calcDPhi( jetsLVec, metLVec, nJetsLoop, dPhiCutArrary);

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

// ---------------------------------- //
// --- Run functions for baseline --- //
// ---------------------------------- //

void BaselineVessel::operator()(NTupleReader& tr_)
{
  tr = &tr_;
  UseCleanedJets();
  CalcBottomVars();
  CalcISRJetVars();
  //PassTrigger(); // now done in post-processing as of v2.7
  PassJetID();
  PassEventFilter();
  PassHEMVeto();
  PassBaseline();
}

void BaselineVessel::PassTrigger()
{
    bool passElectronTrigger    = false;
    bool passMuonTrigger        = false;
    bool passPhotonTrigger      = false;
    
    // ------------------------ //
    // --- Electron Trigger --- //
    // ------------------------ //
    
    passElectronTrigger = ( getBool("HLT_Ele105_CaloIdVT_GsfTrkIdT") || 
                            getBool("HLT_Ele115_CaloIdVT_GsfTrkIdT") ||
                            getBool("HLT_Ele135_CaloIdVT_GsfTrkIdT") ||
                            getBool("HLT_Ele145_CaloIdVT_GsfTrkIdT") ||
                            getBool("HLT_Ele25_eta2p1_WPTight_Gsf") ||
                            getBool("HLT_Ele20_eta2p1_WPLoose_Gsf") ||
                            getBool("HLT_Ele27_eta2p1_WPLoose_Gsf") ||
                            getBool("HLT_Ele27_WPTight_Gsf") ||
                            getBool("HLT_Ele35_WPTight_Gsf") ||
                            getBool("HLT_Ele20_WPLoose_Gsf") ||
                            getBool("HLT_Ele45_WPLoose_Gsf") ||
                            getBool("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL") ||
                            getBool("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") ||
                            getBool("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL") ||
                            getBool("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW") ||
                            getBool("HLT_DoubleEle25_CaloIdL_MW") ||
                            getBool("HLT_DoubleEle33_CaloIdL_MW")
                          );
    
    // -------------------- //
    // --- Muon Trigger --- //
    // -------------------- //

    passMuonTrigger = ( getBool("HLT_IsoMu20") || 
                        getBool("HLT_IsoMu22") ||
                        getBool("HLT_IsoMu24") ||
                        getBool("HLT_IsoMu27") ||
                        getBool("HLT_IsoMu22_eta2p1") ||
                        getBool("HLT_IsoMu24_eta2p1") ||
                        getBool("HLT_IsoTkMu22") ||
                        getBool("HLT_IsoTkMu24") ||
                        getBool("HLT_Mu50") ||
                        getBool("HLT_Mu55")
                      );
    
    // ---------------------- //
    // --- Photon Trigger --- //
    // ---------------------- //
    
    passPhotonTrigger = (getBool("HLT_Photon175") || getBool("HLT_Photon200"));
    
    tr->registerDerivedVar("passElectronTrigger",   passElectronTrigger);
    tr->registerDerivedVar("passMuonTrigger",       passMuonTrigger);
    tr->registerDerivedVar("passPhotonTrigger",     passPhotonTrigger);
}

void BaselineVessel::PassEventFilter()
{
    bool SAT_Pass_EventFilter = false;
    const auto& Flag_goodVertices                           = tr->getVar<bool>("Flag_goodVertices");
    const auto& Flag_HBHENoiseFilter                        = tr->getVar<bool>("Flag_HBHENoiseFilter");
    const auto& Flag_HBHENoiseIsoFilter                     = tr->getVar<bool>("Flag_HBHENoiseIsoFilter");
    const auto& Flag_EcalDeadCellTriggerPrimitiveFilter     = tr->getVar<bool>("Flag_EcalDeadCellTriggerPrimitiveFilter");
    const auto& Flag_BadPFMuonFilter                        = tr->getVar<bool>("Flag_BadPFMuonFilter");
    const auto& Flag_BadChargedCandidateFilter              = tr->getVar<bool>("Flag_BadChargedCandidateFilter");
    const auto& Flag_ecalBadCalibFilter                     = tr->getVar<bool>("Flag_ecalBadCalibFilter");
    const auto& Flag_globalSuperTightHalo2016Filter         = tr->getVar<bool>("Flag_globalSuperTightHalo2016Filter");
    const auto& Flag_eeBadScFilter                          = tr->getVar<bool>("Flag_eeBadScFilter");
    // Note: Don't apply Flag_globalSuperTightHalo2016Filter to fastsim samples if you use fastsim
    // Note: Apply Flag_eeBadScFilter to Data but not MC
    // Note: Don't apply Flag_ecalBadCalibFilter to 2016, but apply it to 2017 and 2018
    SAT_Pass_EventFilter =   Flag_goodVertices && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter
                          && Flag_BadPFMuonFilter && Flag_BadChargedCandidateFilter && Flag_globalSuperTightHalo2016Filter;
    tr->registerDerivedVar("SAT_Pass_EventFilter"+firstSpec, SAT_Pass_EventFilter);
}

// calculate SAT_Pass_JetID using cleaned jet collection (without leptons or photons)
void BaselineVessel::PassJetID()
{
    bool SAT_Pass_JetID = false;
    const auto& Jets      = tr->getVec<TLorentzVector>(jetVecLabel);
    const auto& Jet_jetId = tr->getVec<int>(UseCleanedJetsVar("Jet_jetId"));
    int jetId = 1;
    int i = 0;
    float jet_pt_cut = 30.0;
    for (const auto& Jet : Jets)
    {
        if (Jet.Pt() > jet_pt_cut)
        {
            // Jet_jetId : Int_t Jet ID flags bit1 is loose, bit2 is tight  
            jetId *= (Jet_jetId[i] & 2);
        }
        ++i;
    }
    SAT_Pass_JetID = bool(jetId);
    tr->registerDerivedVar("SAT_Pass_JetID"+firstSpec, SAT_Pass_JetID);
}

bool BaselineVessel::PassObjectVeto(std::vector<TLorentzVector> objects, float eta_low, float eta_high, float phi_low, float phi_high, float pt_low)
{
    for (const auto& object : objects)
    {
        if ( 
                object.Eta() >= eta_low
             && object.Eta() <= eta_high
             && object.Phi() >= phi_low
             && object.Phi() <= phi_high
             && object.Pt()  >  pt_low
           )
        {
            return false;
        }
    }
    return true;
}

void BaselineVessel::PassHEMVeto()
{
    // PassHEMVeto eta, phi, and pt cuts
    // https://github.com/susy2015/NanoSUSY-tools/blob/master/python/modules/Stop0lBaselineProducer.py#L236-L239
    const auto& Jets        = tr->getVec<TLorentzVector>(jetVecLabel);
    const auto& Electrons   = tr->getVec<TLorentzVector>("cutElecVec");
    const auto& Photons     = tr->getVec<TLorentzVector>("cutPhotonTLV");
    // use exact (narrow) window for electrons and photons: eta_low, eta_high, phi_low, phi_high = -3.0, -1.4, -1.57, -0.87
    // use extended (wide) window for jets:                 eta_low, eta_high, phi_low, phi_high = -3.2, -1.2, -1.77, -0.67
    float narrow_eta_low  = -3.0;
    float narrow_eta_high = -1.4;
    float narrow_phi_low  = -1.57;
    float narrow_phi_high = -0.87;
    float wide_eta_low    = -3.2;
    float wide_eta_high   = -1.2;
    float wide_phi_low    = -1.77;
    float wide_phi_high   = -0.67;
    float min_electron_pt =  20.0;
    float min_photon_pt   = 220.0;
    float jet_pt_cut      = 30.0;
    // bool PassObjectVeto(std::vector<TLorentzVector> objects, float eta_low, float eta_high, float phi_low, float phi_high, float pt_low) 
    bool SAT_Pass_HEMVeto = true;
    SAT_Pass_HEMVeto = SAT_Pass_HEMVeto && PassObjectVeto(Electrons, narrow_eta_low, narrow_eta_high, narrow_eta_low, narrow_eta_high, min_electron_pt);
    SAT_Pass_HEMVeto = SAT_Pass_HEMVeto && PassObjectVeto(Photons,   narrow_eta_low, narrow_eta_high, narrow_eta_low, narrow_eta_high, min_photon_pt);
    SAT_Pass_HEMVeto = SAT_Pass_HEMVeto && PassObjectVeto(Jets,      wide_eta_low,   wide_eta_high,   wide_eta_low,   wide_eta_high,   jet_pt_cut);
    tr->registerDerivedVar("SAT_Pass_HEMVeto"+firstSpec,   SAT_Pass_HEMVeto);
}

// ===  FUNCTION  ============================================================
//         Name:  BaselineVessel::CombDeepCSV
//  Description:  
// ===========================================================================
bool BaselineVessel::CombDeepCSV()
{
  std::vector<float> *DeepCSVcomb = new std::vector<float>();
  const std::vector<float> &DeepCSVb = tr->getVec<float>(UseCleanedJetsVar("DeepCSVb"));
  const std::vector<float> &DeepCSVbb = tr->getVec<float>(UseCleanedJetsVar("DeepCSVbb"));
  for (int i = 0; i < DeepCSVb.size(); ++i)
  {
    //printf("DEBUG: DeepCSVb.at(%d) + DeepCSVbb.at(%d) = %f + %f = %f\n", i, i, DeepCSVb.at(i), DeepCSVbb.at(i), DeepCSVb.at(i) + DeepCSVbb.at(i));
    DeepCSVcomb->push_back(DeepCSVb.at(i) + DeepCSVbb.at(i));
  }

  tr->registerDerivedVec(UseCleanedJetsVar("DeepCSVcomb"), DeepCSVcomb);
  return true;
}       // -----  end of function BaselineVessel::CombDeepCSV  -----

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
  for(auto &jet : tr->getVec<TLorentzVector>(jetVecLabel))
  {
    if (jet.Pt() >= 30)
    {
      MHT -= jet;
      SumHT += jet.Pt();
    }
  }
  tr->registerDerivedVar("MHT"+firstSpec, static_cast<float>(MHT.Pt()));
  tr->registerDerivedVar("MHTPhi"+firstSpec, static_cast<float>(MHT.Phi()));
  tr->registerDerivedVar("MHTSig"+firstSpec, static_cast<float>(MHT.Pt()/ sqrt(SumHT)));

  tr->registerDerivedVar("METSig"+firstSpec, static_cast<float>(tr->getVar<float>(METLabel)/ sqrt(SumHT)));
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

  const std::vector<TLorentzVector> &MuonTLV   = tr->getVec<TLorentzVector>("MuonTLV");
  const std::vector<float>          &muonsRelIso = tr->getVec<float>("muonsMiniIso");
  const std::vector<float>          &muonsMtw    = tr->getVec<float>("muonsMtw");
  const std::vector<int>            &muonsFlagID = tr->getVec<int>(muonsFlagIDLabel.c_str());
  const std::vector<float>          &muonsCharge = tr->getVec<float>("muonsCharge");
  for(unsigned int im=0; im<MuonTLV.size(); im++){
    if(AnaFunctions::passMuon(MuonTLV[im], muonsRelIso[im], muonsMtw[im], muonsFlagID[im], AnaConsts::muonsMiniIsoArr))
    {
      if (!vMuons->empty()) // Making sure the vMuons are sorted in Pt
        assert(MuonTLV.at(im).Pt() <= vMuons->back().Pt());
      vMuons->push_back(MuonTLV.at(im));
      vMuonChg->push_back(muonsCharge.at(im));
    }
  }

  const std::vector<TLorentzVector> &electronsLVec   = tr->getVec<TLorentzVector>("ElectronTLV");
  const std::vector<float> &electronsRelIso         = tr->getVec<float>("elesMiniIso");
  const std::vector<float> &electronsMtw            = tr->getVec<float>("elesMtw");
  const std::vector<int> &electronsFlagID            = tr->getVec<int>(elesFlagIDLabel.c_str());
  const std::vector<float>         &electronsCharge = tr->getVec<float>("elesCharge");
  for(unsigned int ie=0; ie<electronsLVec.size(); ie++){
    if(AnaFunctions::passElectron(electronsLVec[ie], electronsRelIso[ie], electronsMtw[ie], electronsFlagID[ie], AnaConsts::elesMiniIsoArr)) 
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

// calculate bottom quark variables
// n_bottoms, mtb, ptb
bool BaselineVessel::CalcBottomVars()
{
  const auto& jets           = tr->getVec<TLorentzVector>(jetVecLabel);
  const auto& Jet_btagDisc   = tr->getVec<float>(CSVVecLabel);
  const auto& Jet_btagStop0l = tr->getVec<unsigned char>(UseCleanedJetsVar("Jet_btagStop0l"));
  const auto& met            = tr->getVar<float>(METLabel); 
  const auto& metphi         = tr->getVar<float>(METPhiLabel); 
  const auto& event          = tr->getVar<unsigned long long>("event");

  bool verbose = false;
  float pt_cut = 20.0;
  float eta_cut = 2.4;
  float mtb = INFINITY;
  float ptb = 0.0;
  int nBottoms = 0;
  int i = 0;
  
  std::vector<std::pair<TLorentzVector, unsigned>> sorted_jets;
  std::vector<std::pair<float, unsigned>> disc_vec;
  i = 0;
  for (const auto& jet : jets)
  {
    sorted_jets.push_back({jet, i});
    ++i;
  }
  
  // sort jets by pt (since JECs change jet pt)
  std::sort(sorted_jets.begin(), sorted_jets.end(), SusyUtility::greaterThan<TLorentzVector, unsigned>);
  
  for (const auto& p : sorted_jets)
  {
    // jet index sorted by pt
    i = p.second; 
    // check if it passes pt, eta, and b requirement
    if (jets[i].Pt() > pt_cut && fabs(jets[i].Eta()) < eta_cut && Jet_btagStop0l[i])
    {
      if (verbose) printf("event %d, jet %d: Jet_btagDisc = %f, Jet_btagStop0l = %s, Jet_pt = %f\n", event, i, Jet_btagDisc[i], Jet_btagStop0l[i] ? "true" : "false", jets[i].Pt());
      ++nBottoms;
      // only use first two b-jets (ordered by p_t) for ptb
      if (nBottoms < 3)
      {
        ptb += jets[i].Pt();
      }
      disc_vec.push_back({Jet_btagDisc[i], i});
    }
  }

  // sort by btag discriminator
  std::sort(disc_vec.begin(), disc_vec.end(), SusyUtility::greaterThan<float, unsigned>);
  
  // calculate mtb
  if (verbose && disc_vec.size() > 0) 
  {
    printf("jets.size() = %d, disc_vec.size() = %d\n", jets.size(), disc_vec.size());
  }
  
  i = 0;
  for (const auto& d : disc_vec)
  {
    if (verbose) printf("d = %f, index = %d\n", d.first, d.second);
    // only use first two b-jets (ordered by discriminator) for mtb
    if (i > 1) break;
    const TLorentzVector& b_jet = jets.at(d.second); 
    float dPhi = fabs(TVector2::Phi_mpi_pi(b_jet.Phi() - metphi));
    float temp = sqrt( 2 * b_jet.Pt() * met * (1 - cos(dPhi)) );
    mtb = std::min(mtb, temp);
    ++i;
  }

  // set mtb to 0 if mtb was not changed
  if (mtb == INFINITY) mtb = 0;
 
  // register variables
  tr->registerDerivedVar("mtb"+firstSpec, mtb);
  tr->registerDerivedVar("ptb"+firstSpec, ptb);
  tr->registerDerivedVar("nBottoms"+firstSpec, nBottoms);
}



int BaselineVessel::GetISRJetIdx()
{
  bool verbose = false;
  const auto& fat_jets           = tr->getVec<TLorentzVector>(jetVecLabelAK8);
  const auto& FatJet_btagDeepB   = tr->getVec<float>(UseCleanedJetsVar("FatJet_btagDeepB"));
  const auto& FatJet_subJetIdx1  = tr->getVec<int>(UseCleanedJetsVar("FatJet_subJetIdx1"));
  const auto& FatJet_subJetIdx2  = tr->getVec<int>(UseCleanedJetsVar("FatJet_subJetIdx2"));
  const auto& SubJet_btagDeepB   = tr->getVec<float>("SubJet_btagDeepB");
  const auto& metphi             = tr->getVar<float>(METPhiLabel); 
  const auto& nMergedTops        = tr->getVar<int>(UseCleanedJetsVar("nMergedTops"));
  const auto& nResolvedTops      = tr->getVar<int>(UseCleanedJetsVar("nResolvedTops"));
  const auto& nWs                = tr->getVar<int>(UseCleanedJetsVar("nWs"));
  int nFatJets = fat_jets.size();
  int nSubJets = SubJet_btagDeepB.size();
  int i = 0; // only use leading fat jet (ordered by pt, index 0)
  if (verbose) printf("FatJet %d: p_t = %f, eta = %f, phi = %f, mass = %f, btag_disc = %f\n", i, fat_jets[i].Pt(), fat_jets[i].Eta(), fat_jets[i].Phi(), fat_jets[i].M(), FatJet_btagDeepB[i]);
  // require that there are no merged or resolved tops and no Ws
  if (nMergedTops + nResolvedTops + nWs != 0) 
  {
    if (verbose) printf("FAIL number of tops and Ws requirement\n");
    return -1;
  }
  // require that there is at least one fat jet 
  if (nFatJets == 0)
  {
    if (verbose) printf("FAIL number of fat jets requirement\n");
    return -1;
  }
  // pt > 200
  if (fat_jets[i].Pt() < 200.0)
  {
    if (verbose) printf("FAIL fat jet pt requirement\n");
    return -1;
  }
  // |eta| < 2.4
  if (fabs(fat_jets[i].Eta()) > 2.4)
  {
    if (verbose) printf("FAIL fat jet eta requirement\n");
    return -1;
  }
  // require that ISR jet is not a a b-jet
  if (FatJet_btagDeepB[i] > AnaConsts::DeepCSV.at(year).at("cutM"))
  {
    if (verbose) printf("FAIL fat jet btag requirement\n");
    return -1;
  }
  // require that sub-jets are not b-jets
  int subJetIdx1 = FatJet_subJetIdx1[i];
  int subJetIdx2 = FatJet_subJetIdx2[i];
  if (subJetIdx1 >= 0 && subJetIdx1 < nSubJets && SubJet_btagDeepB[subJetIdx1] > AnaConsts::DeepCSV.at(year).at("cutM"))
  {
    if (verbose) printf("FAIL subjet 1 btag requirement\n"); 
    return -1; 
  }  
  if (subJetIdx2 >= 0 && subJetIdx2 < nSubJets && SubJet_btagDeepB[subJetIdx2] > AnaConsts::DeepCSV.at(year).at("cutM"))
  {
    if (verbose) printf("FAIL subjet 2 btag requirement\n"); 
    return -1; 
  }  
  // require dPhi(fat_jet, met) > 2
  float dPhi = fabs(TVector2::Phi_mpi_pi(fat_jets[i].Phi() - metphi));
  if (dPhi < 2.0)
  {
    if (verbose) printf("FAIL dPhi requirement: fat_jet_phi = %f, metphi = %f, dPhi = %f\n", fat_jets[i].Phi(), metphi, dPhi);
    return -1;
  }
  // return index 
  return i;
}

bool BaselineVessel::CalcISRJetVars()
{
  const auto& fat_jets = tr->getVec<TLorentzVector>(jetVecLabelAK8);
  int ISRJetIdx = GetISRJetIdx();
  int nFatJets  = fat_jets.size();
  float ISRJetPt = 0.0;
  if (ISRJetIdx >= 0 && ISRJetIdx < nFatJets) ISRJetPt = fat_jets[ISRJetIdx].Pt();
  tr->registerDerivedVar("ISRJetPt"+firstSpec,  ISRJetPt);
  tr->registerDerivedVar("ISRJetIdx"+firstSpec, ISRJetIdx);
}
