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


// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapMET
//  Description:  
// ===========================================================================
bool StopleAlias::MapMET() const
{
  MapSingleObj<float, double>("met_pt", "met");
  MapSingleObj<float, double>("met_phi", "metphi");
  return true;
}       // -----  end of function StopleAlias::MapMET  -----

void StopleAlias::operator()(NTupleReader& tr_)
{
  tr = &tr_;
  MapMET();
  MapJets();
}

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapJets
//  Description:  
// ===========================================================================
bool StopleAlias::MapJets() const
{
  MapVectorTLV("ak4_jet", "jetsLVec");
  return true;
}       // -----  end of function StopleAlias::MapJets  -----

  
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
  ss<<prefix<<"_eta";
  const std::vector<float>& eta=tr->getVec<float>(ss.str());

  ss.str("");
  ss<<prefix<<"_phi";
  std::cout << ss.str() << std::endl;
  const  std::vector<float>& phi=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<prefix<<"_pt";
  std::cout << ss.str() << std::endl;
  const std::vector<float>& pt=tr->getVec<float>(ss.str());
  ss.str("");
  ss<<prefix<<"_mass";
  std::cout << ss.str() << std::endl;
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
