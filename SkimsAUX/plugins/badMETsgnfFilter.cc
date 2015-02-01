#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"


class badMETsgnfFilter : public edm::EDFilter {

  public:

    explicit badMETsgnfFilter(const edm::ParameterSet & iConfig);
    ~badMETsgnfFilter();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag metSrc_;
    double maxSigmaX2_, maxSigmaY2_;

    int cntBadMETsgnf;

    unsigned int run, event, ls; bool isData;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

};


badMETsgnfFilter::badMETsgnfFilter(const edm::ParameterSet & iConfig) {
  metSrc_ = iConfig.getParameter<edm::InputTag>("metSrc");
  maxSigmaX2_ = iConfig.getParameter<double>("maxSigmaX2");
  maxSigmaY2_ = iConfig.getParameter<double>("maxSigmaY2");
  cntBadMETsgnf = 0;
}


badMETsgnfFilter::~badMETsgnfFilter(){
   std::cout<<"\ncntBadMETsgnf : "<<cntBadMETsgnf<<std::endl<<std::endl;
}


bool badMETsgnfFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the MET
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  bool pass = true;
  double sigmaX2 = met->front().getSignificanceMatrix()(0,0);
  double sigmaY2 = met->front().getSignificanceMatrix()(1,1);
  if( sigmaX2 >= maxSigmaX2_ || sigmaY2 >= maxSigmaY2_ ){
     pass = false; cntBadMETsgnf ++;
 
     loadEventInfo(iEvent, iSetup);
     std::cout<<"\nINFO ... badMETsgnfFilter  run : "<<run<<"  lumi : "<<ls<<"  event : "<<event<<"  sigmaX2 : "<<sigmaX2<<"  sigmaY2 : "<<sigmaY2<<std::endl<<std::endl;
  }

  return pass;

}

void badMETsgnfFilter::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){

// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(badMETsgnfFilter);
