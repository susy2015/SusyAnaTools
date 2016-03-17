// -*- C++ -*-
//
// Package:    Mt2Producer
// Class:      Mt2Producer
// 
/**\class Mt2Producer Mt2Producer.cc RA2Classic/Mt2Producer/src/Mt2Producer.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
// Original Author:  Arne-Rasmus Draeger,68/111,4719,
// Translated from HeppyTools to EDProducer by Sam Bein, Jan 2016

#include <memory>
//#include "DataFormats/JetReco/interface/Jet.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
//#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "TLorentzVector.h"
#include "PhysicsTools/Heppy/interface/Davismt2.h"
#include "PhysicsTools/Heppy/interface/Hemisphere.h"
#include "PhysicsTools/Heppy/interface/ReclusterJets.h"
//#include "PhysicsTools/Heppy/src/classes.h"
//#include <ReclusterJets.h>
//#include <Hemisphere.h>
//
// class declaration
//

class MT2CalcCore{

public:

   MT2CalcCore();
   ~MT2CalcCore();

   heppy::Davismt2 davismt2_;
   double getMT2Hemi(const std::vector<TLorentzVector> & jets, TLorentzVector metVec);

private:
   double computeMT2(TLorentzVector visaVec, TLorentzVector visbVec, TLorentzVector metVec);

};

MT2CalcCore::MT2CalcCore(){
}

double MT2CalcCore::computeMT2(TLorentzVector visaVec, TLorentzVector visbVec, TLorentzVector metVec)
{  
   double metVector[3]={0,metVec.Px(),metVec.Py()};
   double visaVector[3]={0,visaVec.Px(),visaVec.Py()};
   double visbVector[3]={0,visbVec.Px(),visbVec.Py()};
   davismt2_.set_momenta(visaVector,visbVector,metVector);
   davismt2_.set_mn(0);
   return davismt2_.get_mt2();
}

double MT2CalcCore::getMT2Hemi(const std::vector<TLorentzVector> & jets, TLorentzVector metVec){
   if(!(jets.size()>=2)){
      return -1;
   }
   std::vector<float> pxvec, pyvec, pzvec, Evec; 
   std::vector<int> grouping;
           
   for(unsigned int j=0; j< jets.size(); j++){
      pxvec.push_back(jets[j].Px());
      pyvec.push_back(jets[j].Py());
      pzvec.push_back(jets[j].Pz());
      Evec.push_back(jets[j].E());
   }
   heppy::Hemisphere hemisphere(pxvec, pyvec, pzvec, Evec, 2, 3);
   grouping=hemisphere.getGrouping();

   float pseudoJet1px = 0;
   float pseudoJet1py = 0; 
   float pseudoJet1pz = 0;
   float pseudoJet1energy = 0; 
   float multPSJ1 = 0;
   float pseudoJet2px = 0; 
   float pseudoJet2py = 0; 
   float pseudoJet2pz = 0;
   float pseudoJet2energy = 0; 
   float multPSJ2 = 0;
                
   for(unsigned int index=0; index<pxvec.size();index++){
      if(grouping[index]==1){
         pseudoJet1px += pxvec[index];
         pseudoJet1py += pyvec[index];
         pseudoJet1pz += pzvec[index];
         pseudoJet1energy += Evec[index];
	 multPSJ1 += 1;
      }
      if(grouping[index]==2){
         pseudoJet2px += pxvec[index];
         pseudoJet2py += pyvec[index];
         pseudoJet2pz += pzvec[index];
         pseudoJet2energy += Evec[index];
         multPSJ2 += 1;
      }
   }
   float pseudoJet1pt2 = pseudoJet1px*pseudoJet1px + pseudoJet1py*pseudoJet1py;
   float pseudoJet2pt2 = pseudoJet2px*pseudoJet2px + pseudoJet2py*pseudoJet2py;
   TLorentzVector tlv1; tlv1.SetPxPyPzE(pseudoJet1px,pseudoJet1py,pseudoJet1pz,pseudoJet1energy);
   TLorentzVector tlv2; tlv2.SetPxPyPzE(pseudoJet2px,pseudoJet2py,pseudoJet2pz,pseudoJet2energy);
   float mt2 = -1;
   if (pseudoJet1pt2 >= pseudoJet2pt2) mt2=computeMT2(tlv1,tlv2,metVec);
   else mt2=computeMT2(tlv2,tlv1,metVec);
   return mt2;
}
