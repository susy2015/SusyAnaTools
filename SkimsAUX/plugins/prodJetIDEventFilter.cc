
#include <memory>
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"


class prodJetIDEventFilter : public edm::EDFilter {

  public:

    explicit prodJetIDEventFilter(const edm::ParameterSet & iConfig);
    ~prodJetIDEventFilter();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup) override;

    edm::EDGetTokenT<edm::View<pat::Jet> > theJetToken_;
// Default are for loose ID (https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_data)
    const double minJetPt_, maxJetEta_, maxNeutHadF_, maxNeutEmF_;
    const int minNumCon_;
    const double minChargHadF_;  
    const int minChargMulti_;
    const double maxChargEmF_, maxNeutEmFHF_; 
    const int minNumNeutPart_;
    const double maxNeutHadFTight_, maxNeutEmFTight_;
    const bool debug_;
    const bool taggingMode_;
};



prodJetIDEventFilter::prodJetIDEventFilter(const edm::ParameterSet & iConfig)
   : theJetToken_ (consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("JetSource") ))
   , minJetPt_ (iConfig.getParameter<double>("MinJetPt") )
   , maxJetEta_ (iConfig.getParameter<double>("MaxJetEta") )

   , maxNeutHadF_ (iConfig.getUntrackedParameter<double>("MaxNeutralHadFrac", 0.99) )
   , maxNeutEmF_  (iConfig.getUntrackedParameter<double>("MaxNeutralEMFrac", 0.99) )
   , minNumCon_ (iConfig.getUntrackedParameter<int>("MinNumberOfConstituents", 1) )

   , minChargHadF_ (iConfig.getUntrackedParameter<double>("MinChargedHadFrac", 0) )
   , minChargMulti_ (iConfig.getUntrackedParameter<int>("MinChargedMultiplicity", 0) )
   , maxChargEmF_ (iConfig.getUntrackedParameter<double>("MaxChargedEMFrac", 0.99) ) 

   , maxNeutEmFHF_ (iConfig.getUntrackedParameter<double>("MaxNeutralEMFracHF", 0.90) )
   , minNumNeutPart_ (iConfig.getUntrackedParameter<int>("MinNumberOfNeutralParticles", 10) )

   , maxNeutHadFTight_ (iConfig.getUntrackedParameter<double>("MaxNeutralHadFracTight", 0.90) )
   , maxNeutEmFTight_  (iConfig.getUntrackedParameter<double>("MaxNeutralEMFracTight", 0.90) )

   , debug_ (iConfig.getParameter<bool>("debug") )
   , taggingMode_ (iConfig.getParameter<bool>("taggingMode") )
{
   produces<bool>("looseJetID");
   produces<bool>("tightJetID");
}


prodJetIDEventFilter::~prodJetIDEventFilter() {
}


bool prodJetIDEventFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByToken(theJetToken_, jets);

  bool goodJetID = true, goodJetIDtight = true;

  int jetIdx =-1;

  for (edm::View<pat::Jet>::const_iterator j = jets->begin(); j != jets->end(); ++j) {

    if (j->isCaloJet()) {
      std::cout << "No JetId is applied to CaloJets for time being !!! " << std::endl;
    } else if (j->isPFJet()) {
      if (j->pt() > minJetPt_ && fabs(j->eta()) < maxJetEta_) {

        jetIdx++;

        double eta = j->eta();

        double NHF = j->neutralHadronEnergyFraction();
        double NEMF = j->neutralEmEnergyFraction();
        double CHF = j->chargedHadronEnergyFraction();
//        double MUF = j->muonEnergyFraction();
        double CEMF = j->chargedEmEnergyFraction();

        int NumConst = j->chargedMultiplicity() + j->neutralMultiplicity();
        int NumNeutralParticle = j->neutralMultiplicity();
        int CHM = j->chargedMultiplicity(); 

        bool perlooseJetID = true, pertightJetID = true;
        if( std::abs(eta) <= 3.0 ){
           perlooseJetID = NHF<maxNeutHadF_ && NEMF<maxNeutEmF_ && NumConst>minNumCon_;
           pertightJetID = NHF<maxNeutHadFTight_ && NEMF<maxNeutEmFTight_ && NumConst>minNumCon_;
           if( std::abs(eta) <= 2.4 ){
              perlooseJetID &= CHF>minChargHadF_ && CHM>minChargMulti_ && CEMF<maxChargEmF_;
              pertightJetID &= CHF>minChargHadF_ && CHM>minChargMulti_ && CEMF<maxChargEmF_;
           }
        }else{
           perlooseJetID = NEMF<maxNeutEmFHF_ && NumNeutralParticle>minNumNeutPart_;
           pertightJetID = NEMF<maxNeutEmFHF_ && NumNeutralParticle>minNumNeutPart_;
        }
/*
        bool perlooseJetID =  ( (NHF<maxNeutHadF_ && NEMF<maxNeutEmF_ && NumConst>minNumCon_) && ((abs(eta)<=2.4 && CHF>minChargHadF_ && CHM>minChargMulti_ && CEMF<maxChargEmF_) || abs(eta)>2.4) && abs(eta)<=3.0 )
                            || ( NEMF<maxNeutEmFHF_ && NumNeutralParticle>minNumNeutPart_ && abs(eta)>3.0 );

        bool pertightJetID =  ( (NHF<maxNeutHadFTight_ && NEMF<maxNeutEmFTight_ && NumConst>minNumCon_) && ((abs(eta)<=2.4 && CHF>minChargHadF_ && CHM>minChargMulti_ && CEMF<maxChargEmF_) || abs(eta)>2.4) && abs(eta)<=3.0 )
                            || ( NEMF<maxNeutEmFHF_ && NumNeutralParticle>minNumNeutPart_ && abs(eta)>3.0 );

        if( !perlooseJetID ) goodJetID = false;
        if( !pertightJetID ) goodJetIDtight = false;
*/        
        goodJetID &= perlooseJetID;
        goodJetIDtight &= pertightJetID;
      }
    }
  }

  iEvent.put( std::auto_ptr<bool>(new bool(goodJetID)), "looseJetID" );
  iEvent.put( std::auto_ptr<bool>(new bool(goodJetIDtight)), "tightJetID" );

// default is loose ID, but in general we use tagging mode
  return taggingMode_ || goodJetID;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodJetIDEventFilter);
