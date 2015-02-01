// -*- C++ -*-
//
// Package:    ISRHistoWeightProducer
// Class:      ISRHistoWeightProducer
// 
/**\class ISRHistoWeightProducer ISRHistoWeightProducer.cc UserCode/ISRHistoWeightProducer/src/ISRHistoWeightProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephen Mrenna
//         Created:  Sat Feb  5 07:55:58 CST 2011
// $Id: ISRHistoWeightProducer.cc,v 1.1 2012/03/08 17:36:07 lhx Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"


class ISRHistoWeightProducer : public edm::EDProducer {
public:
  explicit ISRHistoWeightProducer(const edm::ParameterSet&);
  ~ISRHistoWeightProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  edm::InputTag src_;
  //std::string m_inputFile;
  edm::FileInPath m_inputFile;
  std::string m_treeName;
  std::string   susyScanTopology_;
  bool          debug_;
  
  TFile* m_inputTFile;
  //TFile* m_outputTFile;
  //TTree* m_inputTree;
  
  TH1* horg_;
  TH1* hnew_;
  
  StringObjectFunction<reco::Candidate> fReweight_;
 
};

ISRHistoWeightProducer::ISRHistoWeightProducer(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src")),
  //m_inputFile(iConfig.getUntrackedParameter< std::string > ("inputFile")),
  m_inputFile(iConfig.getUntrackedParameter< edm::FileInPath > ("inputFile")),
  fReweight_(iConfig.getUntrackedParameter< std::string > ("reweightVariable"))
{
  
  susyScanTopology_ = iConfig.getParameter<std::string> ("SusyScanTopology");
  debug_            = iConfig.getParameter<bool>        ("Debug");

  produces<double>("");
 
}


ISRHistoWeightProducer::~ISRHistoWeightProducer() {

}

void ISRHistoWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

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
      if(debug_) std::cout << "ISRHistoWeightProducer " << *cit << std::endl;
      size_t foundLength = (*cit).size();

      found = (*cit).find(susyScanTopology_);

      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);

      if( susyScanTopology_=="T1" || susyScanTopology_=="T5zz") {
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
  if(debug_) std::cout << "ISRHistoWeightProducer ==> gluino " << mGL << " LSP " << mLSP  << " xFrac "<< xCHI << std::endl;


  char hname1[200], hname2[200];
  sprintf(hname1, "orgISR_lastPt_%i_%i", (int)mGL, (int)mLSP);
  //std::cout << "hname1 "<<hname1 << std::endl;
  //m_inputTFile->GetObject(hname1.c_str(),horg_);
  //m_inputTFile->GetObject(hname1,horg_);

  TDirectory *dir_step1 = (TDirectory*)m_inputTFile->FindObjectAny("originalISR");
  horg_ = (TH1F*) dir_step1->FindObjectAny(hname1);

  sprintf(hname2, "newISR_lastPt_%i_%i", (int)mGL, (int)mLSP);
  //m_inputTFile->GetObject(hname2.c_str(),hnew_);
  //m_inputTFile->GetObject(hname2,hnew_);
  //std::cout << "hname2 "<<hname2 << std::endl;

  TDirectory *dir_step2 = (TDirectory*)m_inputTFile->FindObjectAny("newISR");
  hnew_ = (TH1F*) dir_step2->FindObjectAny(hname2);


  hnew_ -> Divide( horg_ );

  
  Handle<std::vector<reco::CompositeCandidate> > pIn;
  iEvent.getByLabel(src_,pIn);

  std::vector<reco::CompositeCandidate>::const_iterator pit=pIn->begin();

  //Double_t htvalue = fReweight_(*pit);
  Double_t htvalue = (*pit).pt();
 
  Int_t iBin = hnew_->FindBin(htvalue);
 
  Double_t weight = hnew_->GetBinContent(iBin);
  
  std::auto_ptr<double> outWeight(new double(weight));
  
  iEvent.put(outWeight);
 
}

// ------------ method called once each job just before starting event loop  ------------
void  ISRHistoWeightProducer::beginJob() {

  //m_inputTFile = new TFile(m_inputFile.c_str());
   m_inputTFile  = new TFile(m_inputFile.fullPath().c_str());

}

// ------------ method called once each job just after ending the event loop  ------------
void ISRHistoWeightProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ISRHistoWeightProducer);
