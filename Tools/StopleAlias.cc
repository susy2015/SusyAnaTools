// ===========================================================================
// 
//       Filename:  StopleAlias.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/22/2017 11:50:47 AM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "StopleAlias.h"

//----------------------------------------------------------------------------
//       Class:  StopleAlias
//      Method:  StopleAlias
// Description:  constructor
//----------------------------------------------------------------------------
StopleAlias::StopleAlias ()
{
  tr=NULL;
}  // -----  end of method StopleAlias::StopleAlias  (constructor)  -----

void StopleAlias::operator()(NTupleReader& tr_)
{
  tr = &tr_;

  MapGlobalVar();
  MapFilter();
  MapGen();
  
  MapMuon();
  MapElectron();
  MapIsoTrack();

  MapMET();
  MapJets();
  MapSoftb();

  // Apply AliasMap only for the first event
  if (tr->isFirstEvent())
  {
    for(auto i : AliasMap)
    {
      tr->addAlias(i.second, i.first);
    }
  }
}

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapGlobalVar
//  Description:  
// ===========================================================================
bool StopleAlias::MapGlobalVar()
{
  
  // Common variables: run, event, lumi
  //MapSingleObj<unsigned int, int>("npv", "npv"); //same name diff type, BAD

  return true;
}       // -----  end of function StopleAlias::MapGlobalVar  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapFilter
//  Description:  
// ===========================================================================
bool StopleAlias::MapFilter()
{
  return true;
}       // -----  end of function StopleAlias::MapFilter  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapMET
//  Description:  
// ===========================================================================
bool StopleAlias::MapMET()
{
  addAlias("met_pt", "met");
  addAlias("met_phi", "metphi");
  return true;
}       // -----  end of function StopleAlias::MapMET  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapJets
//  Description:  
// ===========================================================================
bool StopleAlias::MapJets()
{
  // Ak4 Jets
  MapVectorTLV("ak4_jet", "jetsLVec");
  MapVectorTLV("ak4_genjet", "genjetsLVec");
  addAlias("ak4_jet_csv", "recoJetsCSVv2");

  // Ak8 Jets
  MapVectorTLV("ak8_fatjet_puppi", "puppiJetsLVec");
  MapVectorTLV("ak8_fatjet", "ak8JetsLVec", "pt", "eta", "phi", "rawmass");
  addAlias("ak8_fatjet_puppi_sdmass", "puppisoftDropMass");

  // Ak8 Jets
  addAlias("ak8_fatjet_puppi_tau1", "puppitau1");
  addAlias("ak8_fatjet_puppi_tau2", "puppitau2");
  addAlias("ak8_fatjet_puppi_tau3", "puppitau3");
  return true;
}       // -----  end of function StopleAlias::MapJets  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapMuon
//  Description:  
// ===========================================================================
bool StopleAlias::MapMuon()
{
  MapVectorTLV("mu", "muonsLVec");
  ProdLepMtw("mu", "muonsMtw");
  addAlias("mu_miniiso", "muonsMiniIso");

  MapVectorObj<int, float>("mu_q", "muonsCharge");
  MapVectorObj<bool, int>("mu_isTight", "muonsFlagTight");
  MapVectorObj<bool, int>("mu_isMedium", "muonsFlagMedium");

  return true;
}       // -----  end of function StopleAlias::MapMuon  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapElectron
//  Description:  
// ===========================================================================
bool StopleAlias::MapElectron()
{
  MapVectorTLV("ele", "elesLVec");
  ProdLepMtw("ele", "elesMtw");
  addAlias("ele_miniiso", "elesMiniIso");
  MapVectorObj<int, float>("ele_q", "elesCharge");
  MapVectorObj<bool, int>("ele_vetoid", "elesFlagVeto");
  MapVectorObj<bool, int>("ele_mediumid", "elesFlagMedium");
  return true;
}       // -----  end of function StopleAlias::MapElectron  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapGen
//  Description:  
// ===========================================================================
bool StopleAlias::MapGen()
{
  return true;
}       // -----  end of function StopleAlias::MapGen  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapIsoTrack
//  Description:  
// ===========================================================================
bool StopleAlias::MapIsoTrack()
{
  MapVectorTLV("prodisotrks_looseIsoTrks", "loose_isoTrksLVec");
  //vector double
  addAlias("prodisotrks_looseIsoTrks_idx"       , "loose_isoTrks_idx"       );
  addAlias("prodisotrks_looseIsoTrks_pdgId"     , "loose_isoTrks_pdgId"     );
  addAlias("prodisotrks_looseIsoTrks_charge"    , "loose_isoTrks_charge"    );
  addAlias("prodisotrks_looseIsoTrks_mtw"       , "loose_isoTrks_mtw"       );
  addAlias("prodisotrks_looseIsoTrks_dz"        , "loose_isoTrks_dz"        );
  addAlias("prodisotrks_looseIsoTrks_iso"       , "loose_isoTrks_iso"       );
  addAlias("prodisotrks_looseIsoTrks_pfActivity", "loose_isoTrks_pfActivity");
  return true;
}       // -----  end of function StopleAlias::MapIsoTrack  -----

  
// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapVectorTLV
//  Description:  General function to map VectorTLV, UCSB use float by default
//  If needed, change this to a template function
// ===========================================================================
bool StopleAlias::MapVectorTLV(std::string prefix, std::string outname ,
    const std::string s_pt , const std::string s_eta ,
    const std::string s_phi , const std::string s_mass ) const
{
  std::stringstream ss("");
  ss<<prefix<<"_" <<s_eta;
  const std::vector<float>& eta=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<prefix<<"_"<<s_phi;
  const  std::vector<float>& phi=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<prefix<<"_"<<s_pt;
  const std::vector<float>& pt=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<prefix<<"_"<<s_mass;
  const std::vector<float>& mass=tr->getVec<float>(ss.str());

  std::vector<TLorentzVector> *objs = new std::vector<TLorentzVector>();
  for(unsigned int i=0; i < pt.size(); ++i)
  {
    TLorentzVector obj(0, 0, 0, 0);
    obj.SetPtEtaPhiM(pt.at(i), eta.at(i), phi.at(i), mass.at(i));
    objs->push_back(obj);
  }

  tr->registerDerivedVec(outname, objs);
  return true;
}       // -----  end of function StopleAlias::MapVectorTLV  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapSingleObj
//  Description:  
// ===========================================================================
template <class Tfrom, class Tto>
bool StopleAlias::MapSingleObj(const std::string Sfrom, const std::string Sto) const
{
  Tto obj = tr->getVar<Tfrom>(Sfrom);
  tr->registerDerivedVar(Sto, obj);
  return true;
}       // -----  end of function StopleAlias::MapSingleObj  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapVectorObj
//  Description:  
// ===========================================================================
template <class Tfrom, class Tto>
bool StopleAlias::MapVectorObj(const std::string Sfrom, const std::string Sto) const
{
  const std::vector<Tfrom> &obj = tr->getVec<Tfrom>(Sfrom);

  std::vector<Tto> *objs = new std::vector<Tto>();

  for(auto i : obj)
  {
    objs->push_back(Tto(i));
  }

  tr->registerDerivedVec(Sto, objs);
  return true;
}       // -----  end of function StopleAlias::MapVectorObj  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::ProdLepMtw
//  Description:  
//  
// ===========================================================================
bool StopleAlias::ProdLepMtw(const std::string &lep, const std::string &outname,
    const std::string s_pt , const std::string s_phi ) const
{
  const float &met = tr->getVar<float>("met_pt");
  const float &metphi = tr->getVar<float>("met_phi");

  std::stringstream ss("");
  ss<<lep<<"_" <<s_pt;
  const std::vector<float>& leppt=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<lep<<"_" <<s_phi;
  const std::vector<float>& lepphi=tr->getVec<float>(ss.str());

  std::vector<float> *lepMtw = new std::vector<float>();
  for(unsigned int i=0; i < leppt.size(); ++i)
  {
    float lpt = leppt.at(i);
    float lphi = lepphi.at(i);
    float mtw = sqrt(2 * met * lpt * (1 - cos(lphi -metphi)));
    lepMtw->push_back(mtw);
  }

  tr->registerDerivedVec(outname, lepMtw);
  return true;
}       // -----  end of function StopleAlias::ProdLepMtw  -----


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::addAlias
//  Description:  /* cursor */
// ===========================================================================
bool StopleAlias::addAlias(const std::string &Sfrom, const std::string &Sto) 
{
  if (!tr->isFirstEvent()) return false;
  AliasMap[Sto] =Sfrom;
  return true;
}       // -----  end of function StopleAlias::addAlias  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapSoftb
//  Description:  
// ===========================================================================
bool StopleAlias::MapSoftb()
{
  MapVectorTLV("sv_sv", "svLVec");
  addAlias("sv_sv_d3derr",       "svD3Derr");
  addAlias("sv_sv_d3d",          "svD3D");
  addAlias("sv_sv_dxy",          "svDXY");
  addAlias("sv_sv_ntracks",      "svNTracks");
  addAlias("sv_sv_costhetasvpv", "svCosThetaSVPS");

  addAlias("sv_sv_chi2",   "svChi2");
  addAlias("sv_sv_ndf",    "svNDF");
  addAlias("sv_sv_dxyerr", "svDXYerr");
  addAlias("sv_sv_mass",   "svMass");
  addAlias("sv_sv_pt",     "svPT");
  addAlias("sv_sv_phi",    "svPhi");
  addAlias("sv_sv_eta",    "svETA");
  // Missing the below mapping
  //std::vector<float, std::allocator<float> > sv_sv_mva
  return true;
}       // -----  end of function StopleAlias::MapSoftb  -----

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapPhoton
//  Description:
// ===========================================================================
bool StopleAlias::MapPhoton()
{
  addAlias("pho_eta", "photonEta");
  addAlias("pho_pt", "photonPt");
  addAlias("pho_phi", "photonPhi");
  MapVectorObj<bool, int>("pho_mediumid", "mediumPhotonID");
  MapVectorObj<bool, int>("pho_looseid", "loosePhotonID");
  MapVectorObj<bool, int>("pho_tightid", "tightPhotonID");
}
