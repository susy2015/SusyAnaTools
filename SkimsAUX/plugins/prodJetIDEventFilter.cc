
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
    const double minJetPt_, maxJetEta_, maxNeutHadF_, maxNeutEmF_;
    const bool debug_;
    const bool taggingMode_;
};



prodJetIDEventFilter::prodJetIDEventFilter(const edm::ParameterSet & iConfig)
   : theJetToken_ (consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("JetSource") ))
   , minJetPt_ (iConfig.getParameter<double>("MinJetPt") )
   , maxJetEta_ (iConfig.getParameter<double>("MaxJetEta") )
   , maxNeutHadF_ (iConfig.getParameter<double>("MaxNeutralHadFrac") )
   , maxNeutEmF_  (iConfig.getParameter<double>("MaxNeutralEMFrac") )
   , debug_ (iConfig.getParameter<bool>("debug") )
   , taggingMode_ (iConfig.getParameter<bool>("taggingMode") )
{
   produces<int>();
}


prodJetIDEventFilter::~prodJetIDEventFilter() {
}


bool prodJetIDEventFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByToken(theJetToken_, jets);

  int goodJetID=1;

  int jetIdx =-1;

  for (edm::View<pat::Jet>::const_iterator j = jets->begin(); j != jets->end(); ++j) {

    if (j->isCaloJet()) {
      std::cout << "No JetId is applied to CaloJets for time being !!! " << std::endl;
    } else if (j->isPFJet()) {
      if (j->pt() > minJetPt_ && fabs(j->eta()) < maxJetEta_) {

        jetIdx++;

	// neutral hadron fraction already calculated on uncorrected jet energy
	double nhf = j->neutralHadronEnergyFraction();

	// charged hadron fraction, uncorrected jet energy
	double nem = j->neutralEmEnergyFraction();

        if( debug_ ){
           printf("DEBUG ... idx : %3d  pt : %8.3f  eta : % 6.3f  phi : % 6.3f  nhf : %5.3f  nem : %5.3f\n", jetIdx, j->pt(), j->eta(), j->phi(), nhf, nem);
        }

	if(nhf > maxNeutHadF_ || nem > maxNeutEmF_ ) {
	  goodJetID = 0;
	}
      }

    }
  }

  iEvent.put( std::auto_ptr<int>(new int(goodJetID)) );

  return taggingMode_ || goodJetID;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodJetIDEventFilter);