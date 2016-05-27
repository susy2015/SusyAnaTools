// -*- C++ -*-
//
// Package:    preScaleProducer
// Class:      preScaleProducer
//
/**\class preScaleProducer preScaleProducer.cc RA2/preScaleProducer/src/preScaleProducer.cc
 
 Description: <one line class summary>
 
 Implementation:
 <Notes on implementation>
 */
//
//         Created:  Tue Nov 10 17:58:04 CET 2009
// $Id: preScaleProducer.cc,v 1.1 2012/08/03 18:05:06 lhx Exp $
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

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TString.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TObjString.h"

//
// class decleration
//

typedef unsigned int size;

class preScaleProducer: public edm::EDProducer {
public:
    explicit preScaleProducer(const edm::ParameterSet&);
    ~preScaleProducer();

private:
    //  virtual void beginJob() ;
    virtual void produce(edm::Event&, const edm::EventSetup&);

    edm::InputTag triggerResultsLabel_;
    edm::Handle<edm::TriggerResults> triggerResults_;

    size run, event, ls; bool isData;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

// runRanges_      ==>  "xxx:yyy" where xxx is the start run and yyy is the end run (all inclusive)
// trgInRunRanges_ ==>  "HLT_xxx:HLT_yyy:HLT_zzz" where HLT_xxx(yyy,zzz) are the triggers used
//                      in the corresponding run ranges 
// (XXX: The xxx(yyy,zzz) should be the keyword for the triggers used, since there could serveral 
//       verions, e.g., HLT_IsoMu17_DiCentralJet30_v5(6,7). In this case, only input "HLT_IsoMu17_DiCentralJet30_v". ) 
// (XXX: By assumption, all the triggers xxx(yyy,zzz) are used for the particular run ranges.
//       Then, the smallest pre-scale value is chosen. Because if all triggers are valid for an event,
//       this passed event should have the pre-scale corresponding to the tightest trigger where the 
//       pre-scale value is the smallest. )
    std::vector<std::string> runRanges_;
    std::vector<std::string> trgInRunRanges_;

    HLTConfigProvider hltConfig_;

    int prescale_;
    int cntPrintOut_; 
    double accupreScale_;
};

preScaleProducer::preScaleProducer(const edm::ParameterSet& iConfig):
        triggerResultsLabel_(iConfig.getParameter<edm::InputTag> ("triggerResultsLabel")),
        runRanges_(iConfig.getParameter<std::vector<std::string> > ("runRanges")),
        trgInRunRanges_(iConfig.getParameter<std::vector<std::string> > ("trgInRunRanges")){

    cntPrintOut_  = 0;
    accupreScale_ = 0;

//register your products
    produces<int> ("prescale");
}

preScaleProducer::~preScaleProducer() {
   printf("\n==> preScaleProducer   cntPrintOut_ : %10d  accupreScale_ : %10.8f  avgpreScale_ : %10.8f\n\n", cntPrintOut_, accupreScale_, accupreScale_/cntPrintOut_);
}

// ------------ method called to produce the data  ------------
void preScaleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm;

   loadEventInfo(iEvent, iSetup);
   iEvent.getByLabel(triggerResultsLabel_, triggerResults_);

   if( triggerResults_.failedToGet() ) {
      std::cout <<"ERROR ... NO TRIGGER RESULTS !!" << std::endl;
      return;
   }

   int sizeRunRanges = (int)runRanges_.size();
   if( runRanges_.size() != trgInRunRanges_.size() ){
      std::cout<<"ERROR ... runRanges_.size : "<<runRanges_.size()<<"  MIS-MATCH   trgInRunRanges_.size : "<<trgInRunRanges_.size()<<std::endl;
      return;
   }

   int pickedRunRange = -1, cntPickedRunRanges = 0;
   for(int ir=0; ir<sizeRunRanges; ir++){
      TString perRunRangeT = runRanges_[ir];
      TObjArray * vlistRunRange = perRunRangeT.Tokenize(": ");
      int pernRuns = vlistRunRange->GetEntries();
      if( pernRuns != 2 ){
         std::cout<<"ERROR ... number of runs specified for the "<<ir<<"th range is : "<<pernRuns<<" which is NOT 2?!"<<std::endl;
         return;
      }
      TString startRunT = dynamic_cast<TObjString*>(vlistRunRange->At(0))->GetString();
      TString endRunT = dynamic_cast<TObjString*>(vlistRunRange->At(1))->GetString();
      if( (startRunT.Atoi() == -1 || (int)run >= startRunT.Atoi() ) && ( endRunT.Atoi() == -1 || (int)run <= endRunT.Atoi() ) ){
         pickedRunRange = ir;
         cntPickedRunRanges ++;
      }
   }
   if( cntPickedRunRanges !=1 ){
      std::cout<<"ERROR ... this run : "<<run<<" is NOT covered by the run ranges specified. And cntPickedRunRanges : "<<cntPickedRunRanges<<"?!"<<std::endl;
      return;
   }
   TString trgInRunRangesT = trgInRunRanges_[pickedRunRange];
   TObjArray * vlistTrgInRunRange = trgInRunRangesT.Tokenize(": ");
   int nInputTrgs = vlistTrgInRunRange->GetEntries();

   prescale_ = -999;

   bool isDefaultStar = false;
   if( nInputTrgs == 1 ){
      TString perTrgInputT = dynamic_cast<TObjString*>(vlistTrgInRunRange->At(0))->GetString();
      if( perTrgInputT.CompareTo("*") ==0 ){
         prescale_ = 1;
         isDefaultStar = true;
      }
   }
 
   if( !triggerResults_.failedToGet() && !isDefaultStar ) {
 
      const unsigned int N_Triggers = triggerResults_->size();

      const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResults_);

      for(unsigned int triggerItemIndex=0; triggerItemIndex<N_Triggers; ++triggerItemIndex){
         int nMatchCnt =0;
         for(int it=0; it<nInputTrgs; it++){
            TString perTrgInputT = dynamic_cast<TObjString*>(vlistTrgInRunRange->At(it))->GetString();
            if (triggerNames.triggerName(triggerItemIndex).std::string::find(perTrgInputT.Data())!=std::string::npos) {
               nMatchCnt ++;
  	       const std::string& trigger=triggerNames.triggerName(triggerItemIndex);
               int perPreScale = hltConfig_.prescaleValue(iEvent,iSetup,trigger);
               if( prescale_ == -999 ){
                  prescale_ = perPreScale; 
               }else if( prescale_ > perPreScale ){
                  prescale_ = perPreScale;
               }
            }
         }
         if( nMatchCnt > 1 ){
            std::cout<<"WARNING ... one trigger : "<<triggerNames.triggerName(triggerItemIndex).c_str()<<"  MATCHing to "<<nMatchCnt<<"  input triggers?!"<<std::endl;
            std::cout<<"            please check your input trigger keyword to be unique!"<<std::endl<<std::endl;
         }
      } // triggerItemIndex
      if (prescale_ == -999) {
         std::cout << "WARNING ... trigger not found. the prescale is set to 0" << std::endl;
         prescale_ = 0;
      }
   } // !triggerResults_.failedToGet()

// put prescale into the Event
   std::auto_ptr<int> prescalePtr(new int(prescale_));
   iEvent.put(prescalePtr, "prescale");

   cntPrintOut_ ++; 
   accupreScale_ += prescale_;

}

void preScaleProducer::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();

}

//define this as a plug-in
DEFINE_FWK_MODULE( preScaleProducer);
