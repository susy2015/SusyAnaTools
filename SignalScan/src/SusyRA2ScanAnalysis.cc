// -*- C++ -*-
//
// Package:    SusyRA2ScanAnalysis
// Class:      SusyRA2ScanAnalysis
// 
/**\class SusyRA2ScanAnalysis SusyRA2ScanAnalysis.cc SusyAnalysis/SusyRA2ScanAnalysis/src/SusyRA2ScanAnalysis.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Seema Sharma
//         Created:  Thu Jan  6 10:15:37 CST 2011
// $Id: SusyRA2ScanAnalysis.cc,v 1.1 2012/03/08 17:36:08 lhx Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <map>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

// TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1F.h"
#include "TH2F.h"

//
// class declaration
//

class SusyRA2ScanAnalysis : public edm::EDAnalyzer {
public:
  explicit SusyRA2ScanAnalysis(const edm::ParameterSet&);
  ~SusyRA2ScanAnalysis();
 
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void         BookHistograms();
  
  edm::InputTag mhtSrc_;
  edm::InputTag htSrc_;
  edm::InputTag susyScanXSecSrc_;
  edm::InputTag susyScanMCHSrc_;
  edm::InputTag susyScanMGSrc_;
  edm::InputTag susyScanMLSPSrc_;
  edm::InputTag susyScanRunSrc_;
  std::string   susyScanTopology_;
  bool          doTotal_;
  bool          doReweight_;
  edm::InputTag reWeightSrc_;

  edm::Service<TFileService> fs;

  // ra2 analys
  TH2F *h_NTotal;
  TH2F *h_NPassed_HT350_MHT200;
  TH2F *h_NPassed_HT800_MHT200;
  TH2F *h_NPassed_HT800_MHT500;
  TH2F *h_NPassed_HT500_MHT350;

  /// PDF 187 histo
};

SusyRA2ScanAnalysis::SusyRA2ScanAnalysis(const edm::ParameterSet& iConfig) {

  mhtSrc_          = iConfig.getParameter<edm::InputTag>("MHTSource");
  htSrc_           = iConfig.getParameter<edm::InputTag>("HTSource");
  susyScanXSecSrc_ = iConfig.getParameter<edm::InputTag>("susyScanXSecSrc");
  susyScanMCHSrc_  = iConfig.getParameter<edm::InputTag>("susyScanMCHSrc");
  susyScanMGSrc_   = iConfig.getParameter<edm::InputTag>("susyScanMGSrc");
  susyScanMLSPSrc_ = iConfig.getParameter<edm::InputTag>("susyScanMLSPSrc");
  susyScanRunSrc_  = iConfig.getParameter<edm::InputTag>("susyScanRunSrc");
  susyScanTopology_= iConfig.getParameter<std::string>  ("susyScanTopology");
  doTotal_         = iConfig.getParameter<bool> ("DoTotal");
  // if apply weight, then get the weight
  doReweight_      = iConfig.getParameter<bool> ("DoReweight");
  reWeightSrc_     = iConfig.getParameter<edm::InputTag>("EventWeightSrc");
}

SusyRA2ScanAnalysis::~SusyRA2ScanAnalysis() {

}

// ------------ method called to for each event  ------------
void SusyRA2ScanAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

   using namespace edm;
 
   // susy scan specifics
   // read in cross-section
   edm::Handle<double > xsec;
   iEvent.getByLabel(susyScanXSecSrc_, xsec );
   
   // read in mother mass (gluino or squark)
   edm::Handle<double > mg;
   iEvent.getByLabel(susyScanMGSrc_, mg );
   
   // read in LSP mass 
   edm::Handle<double > mlsp;
   iEvent.getByLabel(susyScanMLSPSrc_, mlsp );

   // read in chargino mass 
   edm::Handle<double > mch;
   if( susyScanTopology_=="T6") 
     iEvent.getByLabel(susyScanMCHSrc_, mch );

   // read in run number (was unique to each job earlier)
   edm::Handle<double > run;
   iEvent.getByLabel(susyScanRunSrc_, run );

   //std::cout<<"run " << *run <<" xsec "<< *xsec<<" MG " << *mg <<" MLSP " << *mlsp<<" MCH " << *mch<< std::endl;

   double evtWeight = 1.0;
   if( doReweight_ ) {
     edm::Handle<double > weight;
     iEvent.getByLabel(reWeightSrc_, weight);
     evtWeight = *weight;
   }

   if( doTotal_ ) {
     
     h_NTotal              ->Fill(*mg, *mlsp, evtWeight);
     
   } else {

     // read in the MHT
     edm::Handle<edm::View<reco::MET> > mht;
     iEvent.getByLabel(mhtSrc_, mht);
     
     // read in HT
     edm::Handle<double> ht;
     iEvent.getByLabel(htSrc_, ht);
     
     double mhtVal = (*mht)[0].pt();
     double htVal  = *ht;
     
     if( htVal>350.0 && mhtVal>200.0 ) 
       h_NPassed_HT350_MHT200->Fill(*mg, *mlsp, evtWeight);
     
     if( htVal>800.0 && mhtVal>200.0 ) 
       h_NPassed_HT800_MHT200->Fill(*mg, *mlsp, evtWeight);
     
     if( htVal>800.0 && mhtVal>500.0 ) 
       h_NPassed_HT800_MHT500->Fill(*mg, *mlsp, evtWeight);
     
     if( htVal>500.0 && mhtVal>350.0 ) 
       h_NPassed_HT500_MHT350->Fill(*mg, *mlsp, evtWeight);
   }

}


// ------------ method called once each job just before starting event loop  ------------
void SusyRA2ScanAnalysis::beginJob() {
  BookHistograms();
}

// ------------ method called once each job just after ending the event loop  ------------
void SusyRA2ScanAnalysis::endJob() {

}

void SusyRA2ScanAnalysis::BookHistograms() {

  
  char hname[100], htit[100];  

  sprintf(hname, "h_NTotal");
  sprintf(htit,  "Total no. of events processed");
  h_NTotal               = fs->make<TH2F>(hname, htit,  20, 200.0, 1200.0, 50, 0.0, 1250.0);
  h_NTotal               ->Sumw2();

  sprintf(hname, "h_NPassed_HT350_MHT200");
  sprintf(htit,  "RA2 Baseline: HT>350 & MHT>200 GeV");
  h_NPassed_HT350_MHT200 = fs->make<TH2F>(hname, htit,  20, 200.0, 1200.0, 50, 0.0, 1250.0);
  h_NPassed_HT350_MHT200 ->Sumw2(); 

  sprintf(hname, "h_NPassed_HT800_MHT200");
  sprintf(htit,  "RA2 High HT: HT>800 & MHT>200 GeV");
  h_NPassed_HT800_MHT200 = fs->make<TH2F>(hname, htit,  20, 200.0, 1200.0, 50, 0.0, 1250.0);
  h_NPassed_HT800_MHT200 ->Sumw2();

  sprintf(hname, "h_NPassed_HT800_MHT500");
  sprintf(htit,  "RA2 High MHT: HT>800 & MHT>500 GeV");
  h_NPassed_HT800_MHT500 = fs->make<TH2F>(hname, htit,  20, 200.0, 1200.0, 50, 0.0, 1250.0);
  h_NPassed_HT800_MHT500 ->Sumw2();

  sprintf(hname, "h_NPassed_HT500_MHT350");
  sprintf(htit,  "RA2 Medium HH,MHT: HT>500 & MHT>350 GeV");
  h_NPassed_HT500_MHT350 = fs->make<TH2F>(hname, htit,  20, 200.0, 1200.0, 50, 0.0, 1250.0);
  h_NPassed_HT500_MHT350 ->Sumw2();

}

//define this as a plug-in
DEFINE_FWK_MODULE(SusyRA2ScanAnalysis);
