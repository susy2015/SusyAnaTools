// -*- C++ -*-
//
// Package:    muonEffProducer
// Class:      muonEffProducer
//
/**\class muonEffProducer muonEffProducer.cc RA2/muonEffProducer/src/muonEffProducer.cc
 
 Description: <one line class summary>
 
 Implementation:
 <Notes on implementation>
 */
//
//         Created:  Tue Nov 10 17:58:04 CET 2009
// $Id: muonEffProducer.cc,v 1.6 2012/08/03 18:05:06 lhx Exp $
//
//


// system include files
#include <cmath>
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/METReco/interface/MET.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"
#include "TObject.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

//
// class decleration
//

typedef unsigned int size;

class muonEffProducer: public edm::EDProducer {
public:
    explicit muonEffProducer(const edm::ParameterSet&);
    ~muonEffProducer();

private:
    //  virtual void beginJob() ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

// One root file with multiple efficiency measurement
    bool initMuonIsoRecoEff();

    std::vector<TH2D*> isoEffHistVec, recoEffHistVec;

    std::string effFileName_;
    size isoEffsize, recoEffsize;
    std::vector<std::string> isoEffHistNames_, recoEffHistNames_;

    std::vector<double> isoPtLowRangeVec, isoPtUpRangeVec, isoDrLowRangeVec, isoDrUpRangeVec;
    std::vector<double> recoPtLowRangeVec, recoPtUpRangeVec, recoEtaLowRangeVec, recoEtaUpRangeVec;
    std::vector<int> isoPtnBinsVec, isoDrnBinsVec, recoPtnBinsVec, recoEtanBinsVec;

    bool doHLTeff_;
    std::vector<TH2D*> hltEffHistVec;
    std::vector<std::string> hltEffHistNames_;
    std::vector<double> hltPtLowRangeVec, hltPtUpRangeVec, hltEtaLowRangeVec, hltEtaUpRangeVec;
    std::vector<int> hltPtnBinsVec, hltEtanBinsVec;

// Indices:  0  : isolation   1 : reconstruction
// Methods for isolation: 0 : use DR calculation with a minDR requirement
//                        1 : use caloJets parameterization
// Methods for reconstruction: 0 : use pt, eta
//                             1 : use caloJets dR
//    If the recoEffHistNames_.size is NOT 1, then the value can be, e.g., recoEffHistNames_.size == 3,
//                      1      2       3
//                      0      0       1    => in order of 10
// 0 : means using (pt, eta);  1 : means using caloJets dR
    std::vector<int> applyMethods_;

    StringCutObjectSelector<reco::Jet,true> pfJetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    StringCutObjectSelector<reco::Jet,true> caloJetCut_; // lazy parsing, to allow cutting on variables not in reco::Candidate class

    double isopfJetMinDR_;
    double isocaloJetMinDR_;

    edm::InputTag jetSrc_, caloJetSrc_;
    edm::Handle<edm::View<reco::Jet> > jets, caloJets;
    size nJets, nCaloJets;
    virtual void loadRecoJets(const edm::Event& iEvent);

    edm::InputTag muonSrc_;
    edm::Handle<edm::View<reco::Muon > > muons;
    size nMuons;
    virtual void loadMuons(const edm::Event& iEvent);

    double getMuonIsoEff(const double dR, const double pt, const int doVary, double *totErrPtr, int effTypeIdx);
    double getMuonIsoEff(const std::vector<double> &isoVarsVec, const int doVary, double *totErrPtr, int effTypeIdx);
    double getMuonRecoEff(const double pt, const double eta, const int doVary, double *totErrPtr, int effTypeIdx);
    double getMuonRecoEff(const std::vector<double> &recoVarsVec, const int doVary, double *totErrPtr, int effTypeIdx);
    double getMuonHLTEff(const double pt, const double eta, const int doVary, double *totErrPtr, int effTypeIdx);
    double getMuonHLTEff(const std::vector<double> &recoVarsVec, const int doVary, double *totErrPtr, int effTypeIdx);

    std::vector<double> calcIsoVars(const reco::Muon &pickedMuon);
    std::vector<double> calcCaloIsoVars(const reco::Muon &pickedMuon);
    std::vector<double> calcRecoVars(const pat::Muon &pickedMuon);
    std::vector<double> calcCaloRecoVars(const pat::Muon &pickedMuon);
    std::vector<double> calcHLTVars(const pat::Muon &pickedMuon);

    double _totEffWeights, _totEffWeightErrs;
    int _totEffWeightsCnt, _totMuonsCnt;

    std::vector<reco::Jet> idJets_, isoJets_;
};

muonEffProducer::muonEffProducer(const edm::ParameterSet& iConfig) : 
   pfJetCut_(iConfig.existsAs<std::string>("pfJetSelection") ? iConfig.getParameter<std::string>("pfJetSelection") : "", true),
   caloJetCut_(iConfig.existsAs<std::string>("caloJetSelection") ? iConfig.getParameter<std::string>("caloJetSelection") : "", true)
{

   jetSrc_ = iConfig.getParameter<edm::InputTag>("jetSrc");
   caloJetSrc_ = iConfig.getParameter<edm::InputTag>("caloJetSrc");
   muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSrc");

   applyMethods_ = iConfig.getParameter<std::vector<int> >("applyMethods");

   std::cout<<"\napplyMethods_ : "; for(size ia=0; ia<applyMethods_.size(); ia++){ std::cout<<"  "<<applyMethods_[ia]; } std::cout<<std::endl;

//   jetCut_ = (iConfig.getParameter<std::string>("jetSelection") : true);

   effFileName_ = iConfig.getParameter<std::string>("effFileName");

   isopfJetMinDR_ = iConfig.getParameter<double>("isopfJetMinDR");
   isocaloJetMinDR_ = iConfig.getParameter<double>("isocaloJetMinDR");

   isoEffHistNames_ = iConfig.getParameter<std::vector<std::string> >("isoEffHistNames");
   recoEffHistNames_ = iConfig.getParameter<std::vector<std::string> >("recoEffHistNames");

   hltEffHistNames_ = iConfig.getParameter<std::vector<std::string> >("hltEffHistNames");

   doHLTeff_ = iConfig.getUntrackedParameter<bool>("doHLTeff", false);

   std::cout<<"\nOpening efficiency file :"<<effFileName_.c_str()<<" to apply muon efficiency"<<std::endl;

   if( !initMuonIsoRecoEff() ){ std::cout<<"\nERROR ... initMuonIsoRecoEff() !"<<std::endl; return; }

   _totEffWeights=0; _totEffWeightErrs =0; _totEffWeightsCnt =0; _totMuonsCnt =0;

// Weights are combined and inversed!
   produces<std::vector<double> >("effWeights");
   produces<std::vector<double> >("effWeightErr2s");

   produces<std::vector<std::vector<std::vector<double> > > >("recoVars");
   produces<std::vector<std::vector<std::vector<double> > > >("isoVars");

   produces<std::vector<reco::Jet> >("isoJets");
   produces<std::vector<reco::Jet> >("idJets");
}

muonEffProducer::~muonEffProducer() {
   std::cout<<"\n_totEffWeightsCnt : "<<_totEffWeightsCnt<<"  _totMuonsCnt : "<<_totMuonsCnt<<std::endl;
   printf("  ==> _totEffWeights : %7.5e  _totEffWeightErrs : %7.5e  -->  avgEffWeights : %7.5f  avgEffErrs : %7.5f\n\n", _totEffWeights, sqrt(_totEffWeightErrs), _totEffWeights/_totEffWeightsCnt, _totEffWeights/_totEffWeightsCnt*
sqrt(_totEffWeightErrs/_totEffWeights/_totEffWeights + 1./_totEffWeightsCnt) );
}

// ------------ method called to produce the data  ------------
void muonEffProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm;

   loadRecoJets(iEvent);
   loadMuons(iEvent);

   idJets_.clear(); isoJets_.clear();

   std::auto_ptr<std::vector<double> > effWeightsPtr(new std::vector<double> );
   std::auto_ptr<std::vector<double> > effWeightErr2sPtr(new std::vector<double> );

   std::auto_ptr<std::vector<std::vector<std::vector<double> > > > recoVarsPtr(new std::vector<std::vector<std::vector<double> > >);
   std::auto_ptr<std::vector<std::vector<std::vector<double> > > > isoVarsPtr(new std::vector<std::vector<std::vector<double> > >);

   std::auto_ptr<std::vector<reco::Jet> > idJetsPtr(new std::vector<reco::Jet>);
   std::auto_ptr<std::vector<reco::Jet> > isoJetsPtr(new std::vector<reco::Jet>);

   for(size im=0; im<nMuons; im++){
      std::vector<std::vector<double> > isoVarsVec(isoEffsize);
      int steps = 10, statusVal = applyMethods_[0];
      for(size is=0; is<isoEffsize; is++){
         if( statusVal%steps == 0 ) isoVarsVec[isoEffsize-1-is] = calcIsoVars( (*muons)[im] );
         else if( statusVal%steps == 1 ) isoVarsVec[isoEffsize-1-is] = calcCaloIsoVars( (*muons)[im] );
         statusVal /= steps;
      }
      double isoEff = 1.0, isoEffErr =0.0;
      for(size is=0; is<isoEffHistVec.size(); is++){
         double perIsoEffErr;
         double perIsoEff = getMuonIsoEff(isoVarsVec[is], 0, &perIsoEffErr, is);
         isoEff *= perIsoEff;
         isoEffErr += (perIsoEffErr/perIsoEff)*(perIsoEffErr/perIsoEff);
      }
      isoEffErr = sqrt(isoEffErr)*isoEff;

      std::vector<std::vector<double> > recoVarsVec(recoEffsize);
      statusVal = applyMethods_[1];
      for(size ir=0; ir<recoEffsize; ir++){
         if( statusVal%steps == 0 ) recoVarsVec[recoEffsize-1-ir] = calcRecoVars( (*muons)[im] );
         else if( statusVal%steps == 1 ) recoVarsVec[recoEffsize-1-ir] = calcCaloRecoVars( (*muons)[im] );
         statusVal /= steps;
      }

      double recoEff = 1.0, recoEffErr = 0.0;
      for(size ir=0; ir<recoEffHistVec.size(); ir++){
         double perRecoEffErr;
         double perRecoEff = getMuonRecoEff(recoVarsVec[ir], 0, &perRecoEffErr, ir);
         recoEff *= perRecoEff;
         recoEffErr += (perRecoEffErr/perRecoEff)*(perRecoEffErr/perRecoEff);
      }
      recoEffErr = sqrt(recoEffErr)*recoEff;

      double hltEff = 1.0, hltEffErr = 0.0;
      if( doHLTeff_ ){
         std::vector<double> hltVarsVec = calcHLTVars( (*muons)[im] );
         for(size ir=0; ir<hltEffHistVec.size(); ir++){
            double perHLTEffErr;
            double perHLTEff = getMuonHLTEff(hltVarsVec, 0, &perHLTEffErr, ir);
            hltEff *= perHLTEff;
            hltEffErr += (perHLTEffErr/perHLTEff)*(perHLTEffErr/perHLTEff);
         }
         hltEffErr = sqrt(hltEffErr)*hltEff;
      }

      double combEff = isoEff*recoEff*hltEff;
      double inverseCombEff = 1.0, inverseCombEffErr2 = 0;
      if( combEff !=0 ){
         inverseCombEff = 1.0/combEff;
         inverseCombEffErr2 = inverseCombEff*inverseCombEff*( recoEffErr*recoEffErr/recoEff/recoEff + isoEffErr*isoEffErr/isoEff/isoEff + hltEffErr*hltEffErr/hltEff/hltEff );
      }

      isoVarsPtr->push_back(isoVarsVec); recoVarsPtr->push_back(recoVarsVec);

      effWeightsPtr->push_back(inverseCombEff); effWeightErr2sPtr->push_back(inverseCombEffErr2);
   }

   double perEventWeights = 1.0, perEventWeightErr2 =0;
   for(size im=0; im<nMuons; im++){
      perEventWeights *= (*effWeightsPtr)[im];
      perEventWeightErr2 += (*effWeightErr2sPtr)[im]/(*effWeightsPtr)[im]/(*effWeightsPtr)[im];
   }

   if( nMuons != 0 ){
      perEventWeightErr2 = perEventWeights*perEventWeights*perEventWeightErr2;
      _totEffWeights += perEventWeights;  _totEffWeightErrs += perEventWeightErr2; _totEffWeightsCnt++; _totMuonsCnt+= (int)nMuons;
   }

   for(size ij=0; ij<idJets_.size(); ij++){ idJetsPtr->push_back( idJets_[ij] ); }
   for(size ij=0; ij<isoJets_.size(); ij++){ isoJetsPtr->push_back( isoJets_[ij] ); }
 
   iEvent.put(effWeightsPtr, "effWeights");
   iEvent.put(effWeightErr2sPtr, "effWeightErr2s");

   iEvent.put(recoVarsPtr, "recoVars");
   iEvent.put(isoVarsPtr, "isoVars");

   iEvent.put(idJetsPtr, "idJets");
   iEvent.put(isoJetsPtr, "isoJets");
}

void muonEffProducer::loadRecoJets(const edm::Event& iEvent){
   iEvent.getByLabel(jetSrc_, jets); nJets = jets->size();
   if( applyMethods_[0] ==1 || applyMethods_[1] ==1 ){ iEvent.getByLabel(caloJetSrc_, caloJets); nCaloJets = caloJets->size(); /*std::cout<<"INFO ... nCaloJets : "<<nCaloJets<<"  nJets : "<<nJets<<std::endl;*/ }
}

void muonEffProducer::loadMuons(const edm::Event& iEvent){
   iEvent.getByLabel(muonSrc_, muons); nMuons = muons->size();
}

// Always first X, second Y variables
// Get the muon isolation efficiency as a function of dR and pt
// totErrPtr value ONLY makes sense when doVary = 0
double muonEffProducer::getMuonIsoEff(const double dR, const double pt, const int doVary, double *totErrPtr, int effTypeIdx){

   int binDR = isoEffHistVec[effTypeIdx]->GetXaxis()->FindFixBin(dR);
   if( dR > isoDrUpRangeVec[effTypeIdx] ) binDR = isoDrnBinsVec[effTypeIdx];
   int binPT = isoEffHistVec[effTypeIdx]->GetYaxis()->FindFixBin(pt);
   if( pt > isoPtUpRangeVec[effTypeIdx] ) binPT = isoPtnBinsVec[effTypeIdx];
   double eff = isoEffHistVec[effTypeIdx]->GetBinContent(binDR, binPT);
   double err = isoEffHistVec[effTypeIdx]->GetBinError(binDR, binPT);

   eff += doVary*err;

   if( totErrPtr ) (*totErrPtr) = err;

   return eff;
}

// doVary :  0 : central value   1 : plus error   2 : minus error
double muonEffProducer::getMuonIsoEff(const std::vector<double> &isoVarsVec, const int doVary =0, double *totErrPtr =0, int effTypeIdx =0){
   if( isoVarsVec.empty() ) return 1.0;
   double dR = isoVarsVec[0], pt = isoVarsVec[1];

// Out of range handling
//    if( pt < isoPtLowRange || pt >= isoPtUpRange || dR < isoDrLowRange || dR >= isoDrUpRange ){ if( totErrPtr ) (*totErrPtr) =0; return 1.0; }

   return getMuonIsoEff(dR, pt, doVary, totErrPtr, effTypeIdx);
}

// Get the muon reconstruction efficiency as a function of Pt and Eta
// totErrPtr value ONLY makes sense when doVary = 0
double muonEffProducer::getMuonRecoEff(const double pt, const double eta, const int doVary, double *totErrPtr, int effTypeIdx){

   double eff = 1.0;
   double sumErr2 = 0.0;

   int binPT = recoEffHistVec[effTypeIdx]->GetXaxis()->FindFixBin(pt);
   if( binPT > recoPtUpRangeVec[effTypeIdx] ) binPT = recoPtnBinsVec[effTypeIdx];
   int binETA = recoEffHistVec[effTypeIdx]->GetYaxis()->FindFixBin(eta);
   double central = recoEffHistVec[effTypeIdx]->GetBinContent(binPT, binETA);
   double error = recoEffHistVec[effTypeIdx]->GetBinError(binPT, binETA);

   eff *= (central + doVary*error);
   sumErr2 += error*error/central/central;

   if( totErrPtr ) (*totErrPtr) = eff*sqrt(sumErr2);

   return eff;
}

double muonEffProducer::getMuonRecoEff(const std::vector<double> &recoVarsVec, const int doVary =0, double *totErrPtr =0, int effTypeIdx =0){
   if( recoVarsVec.empty() ) return 1.0;
   double pt = recoVarsVec[0], eta = recoVarsVec[1];

// Out of range handling
//    if( pt < recoPtLowRange || pt >= recoPtUpRange || eta < recoEtaLowRange || eta >= recoEtaUpRange ){ if( totErrPtr ) (*totErrPtr) = 0; return 1.0; }

   return getMuonRecoEff(pt, eta, doVary, totErrPtr, effTypeIdx);
}


std::vector<double> muonEffProducer::calcCaloIsoVars(const reco::Muon &pickedMuon){

   std::vector<double> isoVarsVec; isoVarsVec.clear();
   const double etaMuon = pickedMuon.eta(), phiMuon = pickedMuon.phi(), ptMuon = pickedMuon.pt();

   double drmin = 5, ptRel = 100; int pickedIdx = -1;

   for (unsigned int ij=0; ij<nCaloJets; ij++) {
      const reco::Jet & jet = (*caloJets)[ij];
      if( !caloJetCut_(jet) ) continue;
      const double etaJet = jet.eta(), phiJet = jet.phi(), ptJet = jet.pt();
      const double dR = reco::deltaR(etaJet, phiJet, etaMuon, phiMuon);
      if( dR < isocaloJetMinDR_ ) continue;
      if( dR < drmin ){ drmin = dR; ptRel = ptMuon/ptJet; pickedIdx = (int)ij; }
   }

   isoVarsVec.push_back(drmin); isoVarsVec.push_back(ptRel);
   if( pickedIdx !=-1 ) isoJets_.push_back( (*caloJets)[pickedIdx] );

   return isoVarsVec;
}


std::vector<double> muonEffProducer::calcIsoVars(const reco::Muon &pickedMuon){

   std::vector<double> isoVarsVec; isoVarsVec.clear();

   const double etaMuon = pickedMuon.eta(), phiMuon = pickedMuon.phi(), ptMuon = pickedMuon.pt();
   double minDR = 99.0; int pickedIdx = -1;
   for(size ij=0; ij<nJets; ij++){
      const reco::Jet & jet = (*jets)[ij];
      if( !pfJetCut_(jet) ) continue;
      const double etaJet = jet.eta(), phiJet = jet.phi();
      const double dR = reco::deltaR(etaJet, phiJet, etaMuon, phiMuon);
      if( dR < isopfJetMinDR_ ) continue;
      if( minDR > dR ){ minDR = dR; pickedIdx = (int)ij; }
   }
   
//   if( minDR == 99.0 ) return isoVarsVec; 
   isoVarsVec.push_back(minDR); isoVarsVec.push_back(ptMuon);
   if( pickedIdx != -1 ) isoJets_.push_back( (*jets)[pickedIdx] );

   return isoVarsVec;
}

std::vector<double> muonEffProducer::calcRecoVars(const pat::Muon &pickedMuon){

   std::vector<double> recoVarsVec; recoVarsVec.clear();

   const double etaMuon = pickedMuon.eta(), ptMuon = pickedMuon.pt();
   recoVarsVec.push_back(ptMuon); recoVarsVec.push_back(etaMuon);

   return recoVarsVec;
}

std::vector<double> muonEffProducer::calcCaloRecoVars(const pat::Muon &pickedMuon){

   std::vector<double> recoVarsVec; recoVarsVec.clear();
   const double etaMuon = pickedMuon.eta(), phiMuon = pickedMuon.phi(), ptMuon = pickedMuon.pt();

   double drmin = 5; int pickedIdx = -1;

   for (unsigned int ij=0; ij<nCaloJets; ij++) {
      const reco::Jet & jet = (*caloJets)[ij];
      if( !caloJetCut_(jet) ) continue;
      const double etaJet = jet.eta(), phiJet = jet.phi();
      const double dR = reco::deltaR(etaJet, phiJet, etaMuon, phiMuon);
      if( dR < drmin ){ drmin = dR; pickedIdx = (int)ij; }
   }
   
   recoVarsVec.push_back(drmin); recoVarsVec.push_back(ptMuon);
   if( pickedIdx != -1 ) idJets_.push_back( (*caloJets)[pickedIdx] );

   return recoVarsVec;
}

bool muonEffProducer::initMuonIsoRecoEff(){

   isoEffHistVec.clear(); recoEffHistVec.clear(); hltEffHistVec.clear();
   TFile *effFile = new TFile(effFileName_.c_str()); if( !effFile ) return false;

   for(size is=0; is<isoEffHistNames_.size(); is++){
      TObject * isoObj = effFile->Get(isoEffHistNames_[is].c_str());
      if( isoObj->IsA()->InheritsFrom("TH1") ){
         isoEffHistVec.push_back( (TH2D*) isoObj->Clone() );
      }
      if( isoObj->IsA()->InheritsFrom("TPad") ){
         TString strT(isoEffHistNames_[is]);
         TObjArray * vlist = strT.Tokenize("/");
         TString histNameT = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-1))->GetString();
         isoEffHistVec.push_back( (TH2D*) isoObj->FindObject(histNameT.Data())->Clone() );
      }
   }

   for(size ir=0; ir<recoEffHistNames_.size(); ir++){
      TObject * isoObj = effFile->Get(recoEffHistNames_[ir].c_str());
      if( isoObj->IsA()->InheritsFrom("TH1") ){
         recoEffHistVec.push_back( (TH2D*) isoObj->Clone() );
      }
      if( isoObj->IsA()->InheritsFrom("TPad") ){
         TString strT(recoEffHistNames_[ir]);
         TObjArray * vlist = strT.Tokenize("/");
         TString histNameT = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-1))->GetString();
         recoEffHistVec.push_back( (TH2D*) isoObj->FindObject(histNameT.Data())->Clone() );
      }
   }

   isoEffsize = isoEffHistNames_.size(), recoEffsize = recoEffHistNames_.size();

   isoDrnBinsVec.clear(); isoPtnBinsVec.clear();
   isoDrLowRangeVec.clear(); isoDrUpRangeVec.clear();
   isoPtLowRangeVec.clear(); isoPtUpRangeVec.clear();

   recoEtanBinsVec.clear(); recoPtnBinsVec.clear();
   recoEtaLowRangeVec.clear(); recoEtaUpRangeVec.clear();
   recoPtLowRangeVec.clear(); recoPtUpRangeVec.clear();

   isoDrnBinsVec.resize(isoEffsize); isoPtnBinsVec.resize(isoEffsize);
   isoDrLowRangeVec.resize(isoEffsize); isoDrUpRangeVec.resize(isoEffsize);
   isoPtLowRangeVec.resize(isoEffsize); isoPtUpRangeVec.resize(isoEffsize);
       
   recoEtanBinsVec.resize(recoEffsize); recoPtnBinsVec.resize(recoEffsize); 
   recoEtaLowRangeVec.resize(recoEffsize); recoEtaUpRangeVec.resize(recoEffsize);
   recoPtLowRangeVec.resize(recoEffsize); recoPtUpRangeVec.resize(recoEffsize);
  
   for(size is=0; is<isoEffsize; is++){
      TAxis *isoDraxis = isoEffHistVec[is]->GetXaxis(), *isoPtaxis = isoEffHistVec[is]->GetYaxis();
      isoDrnBinsVec[is] = isoDraxis->GetNbins(); isoPtnBinsVec[is] = isoPtaxis->GetNbins(); 
      isoDrLowRangeVec[is] = isoDraxis->GetBinLowEdge(1); isoDrUpRangeVec[is] = isoDraxis->GetBinUpEdge(isoDrnBinsVec[is]);
      isoPtLowRangeVec[is] = isoPtaxis->GetBinLowEdge(1); isoPtUpRangeVec[is] = isoPtaxis->GetBinUpEdge(isoPtnBinsVec[is]);

      printf("\n%s  ==>iso   PtnBins : %2d  DrnBins  : %2d  PtLow : %7.3f  PtUp : %7.3f  DrLow  :  %5.3f  DrUp  : %5.3f\n", isoEffHistNames_[is].c_str(), isoPtnBinsVec[is], isoDrnBinsVec[is], isoPtLowRangeVec[is], isoPtUpRangeVec[is], isoDrLowRangeVec[is], isoDrUpRangeVec[is]);
   } 

   for(size ir=0; ir<recoEffsize; ir++){
      TAxis *recoPtaxis = recoEffHistVec[ir]->GetXaxis(), *recoEtaaxis = recoEffHistVec[ir]->GetYaxis();
      recoPtnBinsVec[ir] = recoPtaxis->GetNbins(); recoEtanBinsVec[ir] = recoEtaaxis->GetNbins();
      recoPtLowRangeVec[ir] = recoPtaxis->GetBinLowEdge(1); recoPtUpRangeVec[ir] = recoPtaxis->GetBinUpEdge(recoPtnBinsVec[ir]); 
      recoEtaLowRangeVec[ir] = recoEtaaxis->GetBinLowEdge(1); recoEtaUpRangeVec[ir] = recoEtaaxis->GetBinUpEdge(recoEtanBinsVec[ir]);

      printf("%s ==>reco   PtnBins : %2d  etanBins  : %2d  PtLow : %7.3f  PtUp : %7.3f  etaLow  :  %5.3f  etaUp  : %5.3f\n", recoEffHistNames_[ir].c_str(), recoPtnBinsVec[ir], recoEtanBinsVec[ir], recoPtLowRangeVec[ir], recoPtUpRangeVec[ir], recoEtaLowRangeVec[ir], recoEtaUpRangeVec[ir]);
   }

   if( doHLTeff_ ){
      for(size ih=0; ih<hltEffHistNames_.size(); ih++){
         TObject * isoObj = effFile->Get(hltEffHistNames_[ih].c_str());
         if( isoObj->IsA()->InheritsFrom("TH1") ){
            hltEffHistVec.push_back( (TH2D*) isoObj->Clone() );
         }
         if( isoObj->IsA()->InheritsFrom("TPad") ){
            TString strT(hltEffHistNames_[ih]);
            TObjArray * vlist = strT.Tokenize("/");
            TString histNameT = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-1))->GetString();
            hltEffHistVec.push_back( (TH2D*) isoObj->FindObject(histNameT.Data())->Clone() );
         }
      }

      size hltEffsize = hltEffHistNames_.size();
   
      hltEtanBinsVec.clear(); hltPtnBinsVec.clear();
      hltEtaLowRangeVec.clear(); hltEtaUpRangeVec.clear();
      hltPtLowRangeVec.clear(); hltPtUpRangeVec.clear();
   
      hltEtanBinsVec.resize(hltEffsize); hltPtnBinsVec.resize(hltEffsize); 
      hltEtaLowRangeVec.resize(hltEffsize); hltEtaUpRangeVec.resize(hltEffsize);
      hltPtLowRangeVec.resize(hltEffsize); hltPtUpRangeVec.resize(hltEffsize);
     
      for(size ir=0; ir<hltEffsize; ir++){
         TAxis *hltPtaxis = hltEffHistVec[ir]->GetXaxis(), *hltEtaaxis = hltEffHistVec[ir]->GetYaxis();
         hltPtnBinsVec[ir] = hltPtaxis->GetNbins(); hltEtanBinsVec[ir] = hltEtaaxis->GetNbins();
         hltPtLowRangeVec[ir] = hltPtaxis->GetBinLowEdge(1); hltPtUpRangeVec[ir] = hltPtaxis->GetBinUpEdge(hltPtnBinsVec[ir]); 
         hltEtaLowRangeVec[ir] = hltEtaaxis->GetBinLowEdge(1); hltEtaUpRangeVec[ir] = hltEtaaxis->GetBinUpEdge(hltEtanBinsVec[ir]);
   
         printf("%s ==>hlt   PtnBins : %2d  etanBins  : %2d  PtLow : %7.3f  PtUp : %7.3f  etaLow  :  %5.3f  etaUp  : %5.3f\n", hltEffHistNames_[ir].c_str(), hltPtnBinsVec[ir], hltEtanBinsVec[ir], hltPtLowRangeVec[ir], hltPtUpRangeVec[ir], hltEtaLowRangeVec[ir], hltEtaUpRangeVec[ir]);
      }
   }

   return true;

}

std::vector<double> muonEffProducer::calcHLTVars(const pat::Muon &pickedMuon){

   std::vector<double> hltVarsVec; hltVarsVec.clear();

   const double etaMuon = pickedMuon.eta(), ptMuon = pickedMuon.pt();
   hltVarsVec.push_back(ptMuon); hltVarsVec.push_back(etaMuon);

   return hltVarsVec;
}

// Get the muon hlt efficiency as a function of Pt and Eta
// totErrPtr value ONLY makes sense when doVary = 0
double muonEffProducer::getMuonHLTEff(const double pt, const double eta, const int doVary, double *totErrPtr, int effTypeIdx){

   double eff = 1.0;
   double sumErr2 = 0.0;

   int binPT = hltEffHistVec[effTypeIdx]->GetXaxis()->FindFixBin(pt);
   if( binPT > hltPtUpRangeVec[effTypeIdx] ) binPT = hltPtnBinsVec[effTypeIdx];
   int binETA = hltEffHistVec[effTypeIdx]->GetYaxis()->FindFixBin(eta);
   double central = hltEffHistVec[effTypeIdx]->GetBinContent(binPT, binETA);
   double error = hltEffHistVec[effTypeIdx]->GetBinError(binPT, binETA);

   eff *= (central + doVary*error);
   sumErr2 += error*error/central/central;

   if( totErrPtr ) (*totErrPtr) = eff*sqrt(sumErr2);

   return eff;
}

double muonEffProducer::getMuonHLTEff(const std::vector<double> &hltVarsVec, const int doVary =0, double *totErrPtr =0, int effTypeIdx =0){
   if( hltVarsVec.empty() ) return 1.0;
   double pt = hltVarsVec[0], eta = hltVarsVec[1];

// Out of range handling
//    if( pt < hltPtLowRange || pt >= hltPtUpRange || eta < hltEtaLowRange || eta >= hltEtaUpRange ){ if( totErrPtr ) (*totErrPtr) = 0; return 1.0; }

   return getMuonHLTEff(pt, eta, doVary, totErrPtr, effTypeIdx);
}


// ------------ method called once each job just after ending the event loop  ------------
void muonEffProducer::endJob() { }

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE( muonEffProducer);
