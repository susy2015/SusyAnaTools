// -*- C++ -*-
//
// Package:    RA2SMScanAnalysis
// Class:      RA2SMScanAnalysis
// 
/**\class RA2SMScanAnalysis RA2SMScanAnalysis.cc SusyAnalysis/RA2SMScanAnalysis/src/RA2SMScanAnalysis.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Seema Sharma
//         Created:  Mon Aug 22 20:21:59 CDT 2011
// $Id: RA2SMScanAnalysis.cc,v 1.1 2012/03/08 17:36:07 lhx Exp $
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
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

class RA2SMScanAnalysis : public edm::EDAnalyzer {
public:
  explicit RA2SMScanAnalysis(const edm::ParameterSet&);
  ~RA2SMScanAnalysis();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  void         BookHistograms();

  std::string   susyScanTopology_;
  edm::InputTag mhtSrc_;
  edm::InputTag htSrc_;
  bool          doTotal_;
  bool          doReweight_;
  edm::InputTag reWeightSrc_;
  bool          debug_;
  bool          doPDFSys_;
  edm::InputTag pdfwgt1_, pdfwgt2_, pdfwgt3_;

  edm::Service<TFileService> fs;

  // ra2 analys
  TH2F *h_NTotal;
  TH2F *h_NPassed_HT350_MHT200;
  TH2F *h_NPassed_HT800_MHT200;
  TH2F *h_NPassed_HT800_MHT500;
  TH2F *h_NPassed_HT500_MHT350;

  // pdf uncertainties
  TH2F *h_PDFWgt1_NTotal[45];
  TH2F *h_PDFWgt1_NPassed_HT350_MHT200[45];
  TH2F *h_PDFWgt1_NPassed_HT800_MHT200[45];
  TH2F *h_PDFWgt1_NPassed_HT800_MHT500[45];
  TH2F *h_PDFWgt1_NPassed_HT500_MHT350[45];

  TH2F *h_PDFWgt2_NTotal[41];
  TH2F *h_PDFWgt2_NPassed_HT350_MHT200[41];
  TH2F *h_PDFWgt2_NPassed_HT800_MHT200[41];
  TH2F *h_PDFWgt2_NPassed_HT800_MHT500[41];
  TH2F *h_PDFWgt2_NPassed_HT500_MHT350[41];

  TH2F *h_PDFWgt3_NTotal[101];
  TH2F *h_PDFWgt3_NPassed_HT350_MHT200[101];
  TH2F *h_PDFWgt3_NPassed_HT800_MHT200[101];
  TH2F *h_PDFWgt3_NPassed_HT800_MHT500[101];
  TH2F *h_PDFWgt3_NPassed_HT500_MHT350[101];
};

RA2SMScanAnalysis::RA2SMScanAnalysis(const edm::ParameterSet& iConfig) {

  susyScanTopology_ = iConfig.getParameter<std::string> ("SusyScanTopology");
  debug_            = iConfig.getParameter<bool>        ("Debug");
  doTotal_         = iConfig.getParameter<bool>         ("DoTotal");
  mhtSrc_          = iConfig.getParameter<edm::InputTag>("MHTSource");
  htSrc_           = iConfig.getParameter<edm::InputTag>("HTSource");

  // if apply weight, then get the weight
  doReweight_      = iConfig.getParameter<bool>         ("DoReweight");
  reWeightSrc_     = iConfig.getParameter<edm::InputTag>("EventWeightSrc");

  // required PDF uncertainty
  doPDFSys_        = iConfig.getParameter<bool>         ("DoPDFSystematics");
  pdfwgt1_         = iConfig.getParameter<edm::InputTag>("pdfwgt1");
  pdfwgt2_         = iConfig.getParameter<edm::InputTag>("pdfwgt2");
  pdfwgt3_         = iConfig.getParameter<edm::InputTag>("pdfwgt3");

}


RA2SMScanAnalysis::~RA2SMScanAnalysis() {

}

void RA2SMScanAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  
  edm::Handle<LHEEventProduct> product;
  iEvent.getByLabel("source", product);
  
  LHEEventProduct::comments_const_iterator c_begin = product->comments_begin();
  LHEEventProduct::comments_const_iterator c_end   = product->comments_end();
  
  double mGL  = -1.0;
  double mLSP = -1.0; 
  double xCHI = -1.0;

  // susy scan specific
  for( LHEEventProduct::comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    size_t found = (*cit).find("model");
    
    //model T1_0.0_1025.0_525.0
    if( found != std::string::npos)   {
      if(debug_) std::cout << *cit << std::endl;
      size_t foundLength = (*cit).size();
      //found = (*cit).find("T5zz");
      found = (*cit).find(susyScanTopology_);

      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);

      if( susyScanTopology_=="T1") {
	iss >> xCHI;
	iss.clear();
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
      }

      iss >> mGL;
      iss.clear();
      
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> mLSP;
      iss.clear();
    }
  }
  if(debug_) std::cout << "==> gluino " << mGL << " LSP " << mLSP  << " xFrac "<< xCHI << std::endl;

  // count the number of events here according to RA2
  // assumpion is that all the baseline cuts are already applied

  if (doPDFSys_) {//---run PDF systematics for SUSY or Zmumu
     
    edm::Handle<std::vector<double> > pdfwgt1_h;
    iEvent.getByLabel (pdfwgt1_,pdfwgt1_h);
    edm::Handle<std::vector<double> > pdfwgt2_h;
    iEvent.getByLabel (pdfwgt2_,pdfwgt2_h);
    edm::Handle<std::vector<double> > pdfwgt3_h;
    iEvent.getByLabel (pdfwgt3_,pdfwgt3_h);

    std::vector<double> wgtpdf1 = (*pdfwgt1_h);
    int npdfwgt1 = wgtpdf1.size();      
    if(debug_) {
      std::cout <<"Event weight for central pdf:"<<wgtpdf1[0]<<"  npdfwgt1 "<<npdfwgt1<<std::endl;
      for (int i=1;i<npdfwgt1;i+=2) {
	std::cout <<"Event weight for PDF variation +"<<(i+1)/2<<": "<<wgtpdf1[i]<<std::endl;
	std::cout <<"Event weight for PDF variation -"<<(i+1)/2<<": "<<wgtpdf1[i+1]<<std::endl;
      }
    }

    std::vector<double> wgtpdf2 = (*pdfwgt2_h);
    int npdfwgt2 = wgtpdf2.size();      
    if(debug_) {
      std::cout <<"Event weight for central pdf:"<<wgtpdf2[0]<<"  npdfwgt2 "<<npdfwgt2<<std::endl;
      for (int i=1;i<npdfwgt2;i+=2) {
	std::cout <<"Event weight for PDF variation +"<<(i+1)/2<<": "<<wgtpdf2[i]<<std::endl;
	std::cout <<"Event weight for PDF variation -"<<(i+1)/2<<": "<<wgtpdf2[i+1]<<std::endl;
      }
    }

    std::vector<double> wgtpdf3 = (*pdfwgt3_h);
    int npdfwgt3 = wgtpdf3.size();      
    if(debug_) {
      std::cout <<"Event weight for central pdf:"<<wgtpdf3[0]<<"  npdfwgt3 "<<npdfwgt3<<std::endl;
      for (int i=1;i<npdfwgt3;i+=2) {
	std::cout <<"Event weight for PDF variation +"<<(i+1)/2<<": "<<wgtpdf3[i]<<std::endl;
	std::cout <<"Event weight for PDF variation -"<<(i+1)/2<<": "<<wgtpdf3[i+1]<<std::endl;
      }
    }

    if(doTotal_) {

      for (int ipdf=0; ipdf<npdfwgt1; ipdf++) 
	h_PDFWgt1_NTotal[ipdf] ->Fill(mGL, mLSP, wgtpdf1[ipdf]);
      for (int ipdf=0; ipdf<npdfwgt2; ipdf++) 
	h_PDFWgt2_NTotal[ipdf] ->Fill(mGL, mLSP, wgtpdf2[ipdf]);
      for (int ipdf=0; ipdf<npdfwgt3; ipdf++) 
	h_PDFWgt3_NTotal[ipdf] ->Fill(mGL, mLSP, wgtpdf3[ipdf]);

    } else {

      // read in the MHT
      edm::Handle<edm::View<reco::MET> > mht;
      iEvent.getByLabel(mhtSrc_, mht);
      
      // read in HT
      edm::Handle<double> ht;
      iEvent.getByLabel(htSrc_, ht);
      
      double mhtVal = (*mht)[0].pt();
      double htVal  = *ht;
            
      for (int ipdf=0; ipdf<npdfwgt1; ipdf++) {
	if( htVal>350.0 && mhtVal>200.0 ) 
	  h_PDFWgt1_NPassed_HT350_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf1[ipdf]);
	if( htVal>800.0 && mhtVal>200.0 ) 
	  h_PDFWgt1_NPassed_HT800_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf1[ipdf]);      
	if( htVal>800.0 && mhtVal>500.0 ) 
	  h_PDFWgt1_NPassed_HT800_MHT500[ipdf] ->Fill(mGL, mLSP, wgtpdf1[ipdf]);
	if( htVal>500.0 && mhtVal>350.0 ) 
	  h_PDFWgt1_NPassed_HT500_MHT350[ipdf] ->Fill(mGL, mLSP, wgtpdf1[ipdf]);
      }

      for (int ipdf=0; ipdf<npdfwgt2; ipdf++) {
	if( htVal>350.0 && mhtVal>200.0 ) 
	  h_PDFWgt2_NPassed_HT350_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf2[ipdf]);
	if( htVal>800.0 && mhtVal>200.0 ) 
	  h_PDFWgt2_NPassed_HT800_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf2[ipdf]);      
	if( htVal>800.0 && mhtVal>500.0 ) 
	  h_PDFWgt2_NPassed_HT800_MHT500[ipdf] ->Fill(mGL, mLSP, wgtpdf2[ipdf]);
	if( htVal>500.0 && mhtVal>350.0 ) 
	  h_PDFWgt2_NPassed_HT500_MHT350[ipdf] ->Fill(mGL, mLSP, wgtpdf2[ipdf]);
      }

      for (int ipdf=0; ipdf<npdfwgt3; ipdf++) {
	if( htVal>350.0 && mhtVal>200.0 ) 
	  h_PDFWgt3_NPassed_HT350_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf3[ipdf]);
	if( htVal>800.0 && mhtVal>200.0 ) 
	  h_PDFWgt3_NPassed_HT800_MHT200[ipdf] ->Fill(mGL, mLSP, wgtpdf3[ipdf]);      
	if( htVal>800.0 && mhtVal>500.0 ) 
	  h_PDFWgt3_NPassed_HT800_MHT500[ipdf] ->Fill(mGL, mLSP, wgtpdf3[ipdf]);
	if( htVal>500.0 && mhtVal>350.0 ) 
	  h_PDFWgt3_NPassed_HT500_MHT350[ipdf] ->Fill(mGL, mLSP, wgtpdf3[ipdf]);
      }
    }

  } else { // do the nominal & other systematics
  
    double evtWeight = 1.0;
    if( doReweight_ ) {
      edm::Handle<double > weight;
      iEvent.getByLabel(reWeightSrc_, weight);
      evtWeight = *weight;
    }
    
    if( doTotal_ ) {
    
      h_NTotal              ->Fill(mGL, mLSP, evtWeight);
      
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
	h_NPassed_HT350_MHT200->Fill(mGL, mLSP, evtWeight);
      
      if( htVal>800.0 && mhtVal>200.0 ) 
	h_NPassed_HT800_MHT200->Fill(mGL, mLSP, evtWeight);
      
      if( htVal>800.0 && mhtVal>500.0 ) 
	h_NPassed_HT800_MHT500->Fill(mGL, mLSP, evtWeight);
      
      if( htVal>500.0 && mhtVal>350.0 ) 
	h_NPassed_HT500_MHT350->Fill(mGL, mLSP, evtWeight);
    }
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void RA2SMScanAnalysis::beginJob() {
  BookHistograms();
}

// ------------ method called once each job just after ending the event loop  ------------
void RA2SMScanAnalysis::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void RA2SMScanAnalysis::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void RA2SMScanAnalysis::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void RA2SMScanAnalysis::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void RA2SMScanAnalysis::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void RA2SMScanAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void RA2SMScanAnalysis::BookHistograms() {

  char hname[100], htit[100];  

  sprintf(hname, "h_NTotal");
  sprintf(htit,  "Total no. of events processed");
  h_NTotal               = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
  h_NTotal               ->Sumw2();

  sprintf(hname, "h_NPassed_HT350_MHT200");
  sprintf(htit,  "RA2 Baseline: HT>350 & MHT>200 GeV");
  h_NPassed_HT350_MHT200 = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
  h_NPassed_HT350_MHT200 ->Sumw2(); 

  sprintf(hname, "h_NPassed_HT800_MHT200");
  sprintf(htit,  "RA2 High HT: HT>800 & MHT>200 GeV");
  h_NPassed_HT800_MHT200 = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
  h_NPassed_HT800_MHT200 ->Sumw2();

  sprintf(hname, "h_NPassed_HT800_MHT500");
  sprintf(htit,  "RA2 High MHT: HT>800 & MHT>500 GeV");
  h_NPassed_HT800_MHT500 = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
  h_NPassed_HT800_MHT500 ->Sumw2();

  sprintf(hname, "h_NPassed_HT500_MHT350");
  sprintf(htit,  "RA2 Medium HT,MHT: HT>500 & MHT>350 GeV");
  h_NPassed_HT500_MHT350 = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
  h_NPassed_HT500_MHT350 ->Sumw2();

  if(doPDFSys_) {

    for(int ipdf=0; ipdf<45; ipdf++) {

      sprintf(hname, "h_PDFWgt1_NTotal_%i", ipdf);
      sprintf(htit,  "Total no. of events processed (pdfWgt1 %i)", ipdf);
      h_PDFWgt1_NTotal[ipdf]               = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt1_NTotal[ipdf]               ->Sumw2();

      sprintf(hname, "h_PDFWgt1_NPassed_HT350_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 Baseline: HT>350 & MHT>200 GeV (pdfWgt1 %i)", ipdf);
      h_PDFWgt1_NPassed_HT350_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt1_NPassed_HT350_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt1_NPassed_HT800_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 High HT: HT800 & MHT>200 GeV (pdfWgt1 %i)", ipdf);
      h_PDFWgt1_NPassed_HT800_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt1_NPassed_HT800_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt1_NPassed_HT800_MHT500_%i", ipdf);
      sprintf(htit,  "RA2 High MHT: HT800 & MHT>500 GeV (pdfWgt1 %i)", ipdf);
      h_PDFWgt1_NPassed_HT800_MHT500[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt1_NPassed_HT800_MHT500[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt1_NPassed_HT500_MHT350_%i", ipdf);
      sprintf(htit,  "RA2 Medium HT,MHT: HT500 & MHT>350 GeV (pdfWgt1 %i)", ipdf);
      h_PDFWgt1_NPassed_HT500_MHT350[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt1_NPassed_HT500_MHT350[ipdf] ->Sumw2(); 
    }

    for(int ipdf=0; ipdf<41; ipdf++) {

      sprintf(hname, "h_PDFWgt2_NTotal_%i", ipdf);
      sprintf(htit,  "Total no. of events processed (pdfWgt2 %i)", ipdf);
      h_PDFWgt2_NTotal[ipdf]               = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt2_NTotal[ipdf]               ->Sumw2();

      sprintf(hname, "h_PDFWgt2_NPassed_HT350_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 Baseline: HT>350 & MHT>200 GeV (pdfWgt2 %i)", ipdf);
      h_PDFWgt2_NPassed_HT350_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt2_NPassed_HT350_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt2_NPassed_HT800_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 High HT: HT800 & MHT>200 GeV (pdfWgt2 %i)", ipdf);
      h_PDFWgt2_NPassed_HT800_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt2_NPassed_HT800_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt2_NPassed_HT800_MHT500_%i", ipdf);
      sprintf(htit,  "RA2 High MHT: HT800 & MHT>500 GeV (pdfWgt2 %i)", ipdf);
      h_PDFWgt2_NPassed_HT800_MHT500[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt2_NPassed_HT800_MHT500[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt2_NPassed_HT500_MHT350_%i", ipdf);
      sprintf(htit,  "RA2 Medium HT,MHT: HT500 & MHT>350 GeV (pdfWgt2 %i)", ipdf);
      h_PDFWgt2_NPassed_HT500_MHT350[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt2_NPassed_HT500_MHT350[ipdf] ->Sumw2(); 
    }

    for(int ipdf=0; ipdf<101; ipdf++) {

      sprintf(hname, "h_PDFWgt3_NTotal_%i", ipdf);
      sprintf(htit,  "Total no. of events processed (pdfWgt3 %i)", ipdf);
      h_PDFWgt3_NTotal[ipdf]               = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt3_NTotal[ipdf]               ->Sumw2();

      sprintf(hname, "h_PDFWgt3_NPassed_HT350_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 Baseline: HT>350 & MHT>200 GeV (pdfWgt3 %i)", ipdf);
      h_PDFWgt3_NPassed_HT350_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt3_NPassed_HT350_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt3_NPassed_HT800_MHT200_%i", ipdf);
      sprintf(htit,  "RA2 High HT: HT800 & MHT>200 GeV (pdfWgt3 %i)", ipdf);
      h_PDFWgt3_NPassed_HT800_MHT200[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt3_NPassed_HT800_MHT200[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt3_NPassed_HT800_MHT500_%i", ipdf);
      sprintf(htit,  "RA2 High MHT: HT800 & MHT>500 GeV (pdfWgt3 %i)", ipdf);
      h_PDFWgt3_NPassed_HT800_MHT500[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt3_NPassed_HT800_MHT500[ipdf] ->Sumw2(); 

      sprintf(hname, "h_PDFWgt3_NPassed_HT500_MHT350_%i", ipdf);
      sprintf(htit,  "RA2 Medium HT,MHT: HT500 & MHT>350 GeV (pdfWgt3 %i)", ipdf);
      h_PDFWgt3_NPassed_HT500_MHT350[ipdf] = fs->make<TH2F>(hname, htit,  60, 0.0, 1500.0, 60, 0.0, 1500.0);
      h_PDFWgt3_NPassed_HT500_MHT350[ipdf] ->Sumw2(); 
    }
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(RA2SMScanAnalysis);
