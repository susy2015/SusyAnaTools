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
  double met = tr->getVar<float>("met_pt");
  tr->registerDerivedVar("met", met);

  double metphi = tr->getVar<float>("met_phi");
  tr->registerDerivedVar("metphi", metphi);
  return true;
}       // -----  end of function StopleAlias::MapMET  -----

void StopleAlias::operator()(NTupleReader& tr_)
{
  tr = &tr_;
  MapMET();
  //MapJets();
}

// ===  FUNCTION  ============================================================
//         Name:  StopleAlias::MapJets
//  Description:  
// ===========================================================================
bool StopleAlias::MapJets() const
{
  std::vector<float> eta=tr->getVec<float>("jet_eta");
  std::vector<float> phi=tr->getVec<float>("jet_phi");
  std::vector<float> pt=tr->getVec<float>("jet_pt");
  std::vector<float> mass=tr->getVec<float>("jet_mass");

  std::vector<TLorentzVector> *jets = new std::vector<TLorentzVector>();
  std::map<float, TLorentzVector> mjets;

  for(unsigned int i=0; i < pt.size(); ++i)
  {
    TLorentzVector jet(0, 0, 0, 0);
    jet.SetPtEtaPhiM(pt.at(i), eta.at(i), phi.at(i), mass.at(i));
    jets->push_back(jet);
    //mjets[pt.at(i)] = jet;
  }

  //for(auto i=mjets.rbegin(); i!=mjets.rend(); ++i)
  //{
    //std::cout << i->first << std::endl;
    //jets->push_back(i->second);
  //}
  
  tr->registerDerivedVec("jetsLVec", jets);

  return true;
}       // -----  end of function StopleAlias::MapJets  -----
