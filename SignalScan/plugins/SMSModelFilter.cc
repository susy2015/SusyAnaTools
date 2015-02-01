// -*- C++ -*-
//
// Package:    SMSModelFilter
// Class:      SMSModelFilter
// 
/**\class SMSModelFilter SMSModelFilter.cc UserCode/SMSModelFilter/src/SMSModelFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/

// system include files
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// LHE Event
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include "FWCore/Framework/interface/FileBlock.h"


class SMSModelFilter : public edm::EDFilter {
   public:
      explicit SMSModelFilter(const edm::ParameterSet&);
      ~SMSModelFilter();

      typedef std::vector<std::string>::const_iterator
      comments_const_iterator;

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      

  std::string   susyScanTopology_;
  double        susyScanMotherMass_;
  double        susyScanLSPMass_;
  double        susyScanFracLSP_;
  bool          debug_;

  virtual void respondToOpenInputFile(edm::FileBlock const&);
  virtual void respondToCloseInputFile(edm::FileBlock const&);

  std::string inputFileNameStr;

};

SMSModelFilter::SMSModelFilter(const edm::ParameterSet& iConfig) {
   //now do what ever initialization is needed
  susyScanTopology_   = iConfig.getParameter<std::string> ("SusyScanTopology");
  susyScanMotherMass_ = iConfig.getParameter<double>      ("SusyScanMotherMass");
  susyScanLSPMass_    = iConfig.getParameter<double>      ("SusyScanLSPMass");
  susyScanFracLSP_    = iConfig.getParameter<double>      ("SusyScanFracLSP");
  debug_              = iConfig.getParameter<bool>        ("Debug");

  produces<std::vector<std::string> >("fileNameStr");
  produces<std::vector<std::string> >("smsModelStr");
  produces<double>("smsMotherMass");
  produces<double>("smsDaughterMass");
}


SMSModelFilter::~SMSModelFilter() {
}


bool SMSModelFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  Handle<LHEEventProduct> product;
  iEvent.getByLabel("source", product);
  
  comments_const_iterator c_begin = product->comments_begin();
  comments_const_iterator c_end = product->comments_end();
   
  double mGL  = -1.0;
//  double mSQ  = -1.0;
  double mLSP = -1.0; 
  double xCHI = -1.0;

  std::auto_ptr<std::vector<std::string> > fileNamePtr (new std::vector<std::string> );
  std::auto_ptr<std::vector<std::string> > smsModelPtr (new std::vector<std::string> );

  // susy scan specific
  for( LHEEventProduct::comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    size_t found = (*cit).find("model");
    
    //model T1_0.0_1025.0_525.0
    //model T2_550.0_425.0
    //model TGQ_0.8_900.0_850.0

    if( found != std::string::npos){
      if(debug_) std::cout << *cit << std::endl;
      size_t foundLength = (*cit).size();
      //found = (*cit).find("T5zz");
      found = (*cit).find(susyScanTopology_);

//      std::string smaller = (*cit).substr(found+1,foundLength);
      std::string smaller = (*cit).substr(found,foundLength);
      smsModelPtr->push_back(smaller);
      if(debug_) std::cout<<smaller.c_str()<<std::endl;
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);

      if(susyScanTopology_=="T5ZZ" || susyScanTopology_=="T2bw") {
	iss >> mGL;
	iss.clear();

	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> xCHI;
	iss.clear();

      } else if (susyScanTopology_=="T2" || susyScanTopology_=="T2tt" || susyScanTopology_ == "T2tb" || susyScanTopology_=="T1" || susyScanTopology_=="T1tttt" || susyScanTopology_=="T1bbbb" || susyScanTopology_=="T1qqqq" || susyScanTopology_=="T2qq" ) {
	iss >> mGL;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();
      }
    }
  }
  std::auto_ptr<double> smsMotherMassPtr (new double(mGL));
  std::auto_ptr<double> smsDaughterMassPtr (new double(mLSP));

  bool foundModel = false;
  if( (susyScanMotherMass_!= -1 && mGL==susyScanMotherMass_) && (susyScanLSPMass_!=-1 && mLSP==susyScanLSPMass_) ) {
    //std::cout << std::setw(9) << iEvent.id().run()<< ":" << iEvent.luminosityBlock()<<":"<<iEvent.id().event() << std::setw(9) <<  susyScanTopology_ << " mother " << mGL << " LSP "<< mLSP << std::endl;
    foundModel = true;
  }else if( susyScanMotherMass_ ==-1 && mLSP==susyScanLSPMass_ ){
    foundModel = true;
  }else if( mGL==susyScanMotherMass_ && susyScanLSPMass_==-1 ){
    foundModel = true;
  }else if( susyScanMotherMass_ ==-1 && susyScanLSPMass_==-1 ){
    foundModel = true;
  }

  fileNamePtr->push_back(inputFileNameStr);

  iEvent.put(fileNamePtr, "fileNameStr");
  iEvent.put(smsModelPtr, "smsModelStr");
  iEvent.put(smsMotherMassPtr, "smsMotherMass");
  iEvent.put(smsDaughterMassPtr, "smsDaughterMass");
  
  return foundModel;
}

void SMSModelFilter::beginJob() {
}

void SMSModelFilter::endJob() {
}

void SMSModelFilter::respondToOpenInputFile(edm::FileBlock const& fb){
   inputFileNameStr = fb.fileName();
}

void SMSModelFilter::respondToCloseInputFile(edm::FileBlock const& fb){
}

//define this as a plug-in
DEFINE_FWK_MODULE(SMSModelFilter);
