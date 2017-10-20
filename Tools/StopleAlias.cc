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
  MapSingleObj<float, double>("met_pt", "met");
  MapSingleObj<float, double>("met_phi", "metphi");
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
  MapVectorObj<int, double>("mu_q", "muonsCharge");
  MapVectorObj<float, double>("mu_miniiso", "muonsMiniIso");
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

  MapVectorObj<int, double>("ele_q", "elesCharge");
  MapVectorObj<float, double>("ele_miniiso", "elesMiniIso");
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
  MapVectorTLV("pfcand", "loose_isoTrksLVec");
  

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

  std::vector<double> *lepMtw = new std::vector<double>();
  for(unsigned int i=0; i < leppt.size(); ++i)
  {
    float lpt = leppt.at(i);
    float lphi = lepphi.at(i);
    double mtw = sqrt(2 * met * lpt * (1 - cos(lphi -metphi)));
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
