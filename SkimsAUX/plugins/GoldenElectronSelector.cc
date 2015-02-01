
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "recipeAUX/Electron/interface/ElectronEffectiveArea.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/Provenance/interface/ProductID.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"

//#include "recipeAUX/Electron/interface/RadialIsoCalculator.h"

typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;

class GoldenElectronSelector : public edm::EDFilter {

  public:

    explicit GoldenElectronSelector(const edm::ParameterSet & iConfig);
    ~GoldenElectronSelector();

    typedef unsigned long key_type;

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    int searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr);

    edm::InputTag electronSrc_;
    edm::InputTag vtxSrc_;
    bool doEleVeto_, doEleID_, doEleVtx_, doEleIso_;
    double minElePt_, maxEleEta_, maxEleD0_, maxEleDz_, maxEleRelIso_;

//    RadialIsoCalculator*    RadialCutBasedCalculator;

    std::string wkPt_;

   std::vector<double> cut_dEtaIn;
   std::vector<double> cut_dPhiIn;
   std::vector<double> cut_sigmaIEtaIEta;
   std::vector<double> cut_hoe;
   std::vector<double> cut_ooemoop;
   std::vector<double> cut_d0vtx;
   std::vector<double> cut_dzvtx;
   std::vector<double> cut_iso;
   std::vector<bool> cut_vtxFit;
   std::vector<unsigned int> cut_mHits;

   edm::InputTag               conversionsInputTag_;
   std::vector<edm::InputTag>  isoValInputTags_;
   edm::InputTag               beamSpotInputTag_;
   edm::InputTag               rhoIsoInputTag_;

// 0 : Traditional relative iso as last year --> sumChargedHadronPt depends on if PFnoPU is used or not
// 1 : Official recommended DeltaBeta PU correction
// 2 : Effective Area PU correction
// 3 : Radial PU correction
   int eleIsoSel_;

   bool doEleProvInfo_;

   edm::InputTag inputProductIDSrc_, inputKeySrc_;
   edm::Handle<std::vector<edm::ProductID> > inputProductID_;
   edm::Handle<std::vector<key_type> > inputKey_;

};


GoldenElectronSelector::GoldenElectronSelector(const edm::ParameterSet & iConfig) {
  electronSrc_  = iConfig.getParameter<edm::InputTag>("ElectronSource");
  vtxSrc_       = iConfig.getParameter<edm::InputTag>("VertexSource");
  minElePt_     = iConfig.getParameter<double>("MinElePt");
  maxEleEta_    = iConfig.getParameter<double>("MaxEleEta");
  maxEleD0_     = iConfig.getParameter<double>("MaxEleD0");
  maxEleDz_     = iConfig.getParameter<double>("MaxEleDz");
  maxEleRelIso_ = iConfig.getParameter<double>("MaxEleRelIso");
  doEleVeto_    = iConfig.getParameter<bool>("DoElectronVeto");
  doEleID_      = iConfig.getParameter<bool>("DoElectronID");
  doEleVtx_     = iConfig.getParameter<bool>("DoElectronVtxAssociation");
  doEleIso_     = iConfig.getParameter<bool>("DoElectronIsolation");
  wkPt_         = iConfig.getParameter<std::string>("WorkingPoint");
  conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsInput");
  isoValInputTags_     = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");
  beamSpotInputTag_    = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");
  rhoIsoInputTag_      = iConfig.getParameter<edm::InputTag>("rhoIsoInputTag");
 
  doEleProvInfo_ = iConfig.getParameter<bool>("DoElectronProvInfo");

  eleIsoSel_   = iConfig.getParameter<int>("eleIsoSel");

  inputProductIDSrc_ = iConfig.getParameter<edm::InputTag>("inputProductIDSrc");
  inputKeySrc_ = iConfig.getParameter<edm::InputTag>("inputKeySrc");

//  RadialCutBasedCalculator = new RadialIsoCalculator (iConfig.getParameter<edm::ParameterSet>("RadialIsoCalculator"));

  produces<std::vector<pat::Electron> >("");
  if( doEleProvInfo_ ){
     produces<std::vector<edm::ProductID> >("elePFbaseProductID");
     produces<std::vector<key_type> >("elePFbaseKey");
  }
}


GoldenElectronSelector::~GoldenElectronSelector() {
}


bool GoldenElectronSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read in the objects
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(electronSrc_, electrons);
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  // conversions
  edm::Handle<reco::ConversionCollection> conversions;
  iEvent.getByLabel(conversionsInputTag_, conversions);

  // iso deposits
  IsoDepositVals isoVals(isoValInputTags_.size());
  for (size_t j = 0; j < isoValInputTags_.size(); ++j) {
     iEvent.getByLabel(isoValInputTags_[j], isoVals[j]);
  }

  // beam spot
  edm::Handle<reco::BeamSpot> beamspots;
  iEvent.getByLabel(beamSpotInputTag_, beamspots);
  const reco::BeamSpot &beamSpot = *(beamspots.product());

  // rho for isolation
  edm::Handle<double> rhoIso_h;
  iEvent.getByLabel(rhoIsoInputTag_, rhoIso_h);
  double rhoIso = *(rhoIso_h.product());

  reco::CandidatePtr leptonOutPtr;
  unsigned long dummyKey = 999999999;
  edm::ProductID dummyProductID;

  iEvent.getByLabel(inputProductIDSrc_, inputProductID_);
  iEvent.getByLabel(inputKeySrc_, inputKey_);

  bool validInputProvInfo = false;
  if( inputProductID_.isValid() && inputKey_.isValid() ) validInputProvInfo = true;

  std::auto_ptr<std::vector<edm::ProductID> > elePFbaseProductIDptr(new std::vector<edm::ProductID>() );
  std::auto_ptr<std::vector<key_type> > elePFbaseKeyptr(new std::vector<key_type>() );

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Electron> > prod(new std::vector<pat::Electron>());
  if (vertices->size() > 0) {
    unsigned int iele = 0;
    edm::View<pat::Electron>::const_iterator eleItr;
    for (eleItr = electrons->begin(), iele =0; eleItr != electrons->end(); ++eleItr, iele++) {

      // kinematic variables
      bool isEB           = eleItr->isEB() ? true : false;
      float pt            = eleItr->pt();
      float eta           = eleItr->superCluster()->eta();
  
      // id variables
      float dEtaIn        = eleItr->deltaEtaSuperClusterTrackAtVtx();
      float dPhiIn        = eleItr->deltaPhiSuperClusterTrackAtVtx();
      float sigmaIEtaIEta = eleItr->sigmaIetaIeta();
      float hoe           = eleItr->hadronicOverEm();
      float ooemoop       = (1.0/eleItr->ecalEnergy() - eleItr->eSuperClusterOverP()/eleItr->ecalEnergy());
  
      // impact parameter variables
      float d0vtx         = 0.0;
      float dzvtx         = 0.0;

      if (vertices->size() > 0) {
          d0vtx = eleItr->gsfTrack()->dxy(vtxpos);
          dzvtx = eleItr->gsfTrack()->dz(vtxpos);
      } else {
          d0vtx = eleItr->gsfTrack()->dxy();
          dzvtx = eleItr->gsfTrack()->dz();
      }

      // choose cut if barrel or endcap
      unsigned int idx = isEB ? 0 : 1;

      edm::ProductID tofillProductID = dummyProductID;
      key_type tofillKey = dummyKey;

      const reco::CandidatePtr& elePtr =eleItr->pfCandidateRef()->sourceCandidatePtr(0);
//      const reco::CandidatePtr& elePtr =eleItr->sourceCandidatePtr(0);

      if( doEleProvInfo_ && ! eleItr->pfCandidateRef().isNull() ){
         if( validInputProvInfo ){
            tofillProductID = (*inputProductID_)[iele];
            tofillKey = (*inputKey_)[iele];
         }else{
            if( !searchToPFbase(iEvent, elePtr, leptonOutPtr) ){ std::cout<<"\nERROR  cannot find particleFlow base for ele?!"<<std::endl<<std::endl; }

            if( doEleProvInfo_ && leptonOutPtr.isNonnull() ){
               tofillProductID = leptonOutPtr.id();
               tofillKey = leptonOutPtr.key();
            }
         }
      }

      double iso_ch =  (*(isoVals)[0])[elePtr];
      double iso_em = (*(isoVals)[1])[elePtr];
      double iso_nh = (*(isoVals)[2])[elePtr];

      float mHits = eleItr->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);

      cut_dEtaIn.clear(); cut_dEtaIn.resize(2, 999.9);
      cut_dPhiIn.clear(); cut_dPhiIn.resize(2, 999.9);
      cut_sigmaIEtaIEta.clear(); cut_sigmaIEtaIEta.resize(2, 999.9);
      cut_hoe.clear(); cut_hoe.resize(2, 999.9);
      cut_ooemoop.clear(); cut_ooemoop.resize(2, 999.9);
      cut_d0vtx.clear(); cut_d0vtx.resize(2, 999.9);
      cut_dzvtx.clear(); cut_dzvtx.resize(2, 999.9);
      cut_iso.clear(); cut_iso.resize(2, 999.9);
      cut_vtxFit.clear(); cut_vtxFit.resize(2, false);
      cut_mHits.clear(); cut_mHits.resize(2, 999);
    
      if( wkPt_ == "VETO" ){
         cut_dEtaIn[0]        = 0.007; cut_dEtaIn[1]        = 0.010;
         cut_dPhiIn[0]        = 0.800; cut_dPhiIn[1]        = 0.700;
         cut_sigmaIEtaIEta[0] = 0.010; cut_sigmaIEtaIEta[1] = 0.030;
         cut_hoe[0]           = 0.150; cut_hoe[1]           = 999.9;
         cut_ooemoop[0]       = 999.9; cut_ooemoop[1]       = 999.9;
         cut_d0vtx[0]         = 0.040; cut_d0vtx[1]         = 0.040;
         cut_dzvtx[0]         = 0.200; cut_dzvtx[1]         = 0.200;
         cut_vtxFit[0]        = false; cut_vtxFit[1]        = false;
         cut_mHits[0]         = 999  ; cut_mHits[1]         = 999;
         cut_iso[0]           = 0.150; cut_iso[1]           = 0.150;
      }else if( wkPt_ == "LOOSE" ){
         cut_dEtaIn[0]        = 0.007; cut_dEtaIn[1]        = 0.009;
         cut_dPhiIn[0]        = 0.150; cut_dPhiIn[1]        = 0.100;
         cut_sigmaIEtaIEta[0] = 0.010; cut_sigmaIEtaIEta[1] = 0.030;
         cut_hoe[0]           = 0.120; cut_hoe[1]           = 0.100;
         cut_ooemoop[0]       = 0.050; cut_ooemoop[1]       = 0.050;
         cut_d0vtx[0]         = 0.020; cut_d0vtx[1]         = 0.020;
         cut_dzvtx[0]         = 0.200; cut_dzvtx[1]         = 0.200;
         cut_vtxFit[0]        = true ; cut_vtxFit[1]        = true;
         cut_mHits[0]         = 1    ; cut_mHits[1]         = 1;
         if (pt >= 20.0) {
            cut_iso[0] = 0.150; cut_iso[1] = 0.150;
         }else {
            cut_iso[0] = 0.150; cut_iso[1] = 0.100;
         }
      }else if( wkPt_ == "MEDIUM" ){
         cut_dEtaIn[0]        = 0.004; cut_dEtaIn[1]        = 0.007;
         cut_dPhiIn[0]        = 0.060; cut_dPhiIn[1]        = 0.030;
         cut_sigmaIEtaIEta[0] = 0.010; cut_sigmaIEtaIEta[1] = 0.030;
         cut_hoe[0]           = 0.120; cut_hoe[1]           = 0.100;
         cut_ooemoop[0]       = 0.050; cut_ooemoop[1]       = 0.050;
         cut_d0vtx[0]         = 0.020; cut_d0vtx[1]         = 0.020;
         cut_dzvtx[0]         = 0.100; cut_dzvtx[1]         = 0.100;
         cut_vtxFit[0]        = true ; cut_vtxFit[1]        = true;
         cut_mHits[0]         = 1    ; cut_mHits[1]         = 1;
         if (pt >= 20.0) {
            cut_iso[0] = 0.150; cut_iso[1] = 0.150;
         }else {
            cut_iso[0] = 0.150; cut_iso[1] = 0.100;
         }
      }else if( wkPt_ == "TIGHT" ){
         cut_dEtaIn[0]        = 0.004; cut_dEtaIn[1]        = 0.005;
         cut_dPhiIn[0]        = 0.030; cut_dPhiIn[1]        = 0.020;
         cut_sigmaIEtaIEta[0] = 0.010; cut_sigmaIEtaIEta[1] = 0.030;
         cut_hoe[0]           = 0.120; cut_hoe[1]           = 0.100;
         cut_ooemoop[0]       = 0.050; cut_ooemoop[1]       = 0.050;
         cut_d0vtx[0]         = 0.020; cut_d0vtx[1]         = 0.020;
         cut_dzvtx[0]         = 0.100; cut_dzvtx[1]         = 0.100;
         cut_vtxFit[0]        = true ; cut_vtxFit[1]        = true;
         cut_mHits[0]         = 0    ; cut_mHits[1]         = 0;
         if (pt >= 20.0) {
            cut_iso[0] = 0.100; cut_iso[1] = 0.100;
         }else {
            cut_iso[0] = 0.100; cut_iso[1] = 0.070;
         }
      }else{
         std::cout << "Undefined working point" << std::endl;
      }
    
      // acceptance cuts
      if (pt < minElePt_) continue;
      if (std::abs(eta) >= maxEleEta_) continue;
//      if (std::abs(eta) >= 1.4442 && std::abs(eta) <= 1.566) continue;

      // ID cuts
      if (doEleID_) {
         if( fabs(dEtaIn) >= cut_dEtaIn[idx] ) continue;
         if( fabs(dPhiIn) >= cut_dPhiIn[idx] ) continue;
         if( sigmaIEtaIEta >= cut_sigmaIEtaIEta[idx] ) continue;
         if( hoe >= cut_hoe[idx] ) continue;
         if( fabs(ooemoop) >= cut_ooemoop[idx] ) continue;
         if( mHits > cut_mHits[idx]) continue;
      }
      // vertex association cuts - ignore if no vertex (see further)
      if (doEleVtx_) {
        if( fabs(d0vtx) >= cut_d0vtx[idx] ) continue;
        if( fabs(dzvtx) >= cut_dzvtx[idx] ) continue;
      }
      // conversion rejection variables
      bool vtxFitConversion = ConversionTools::hasMatchedConversion((*eleItr), conversions, beamSpot.position());
      if( cut_vtxFit[idx] && vtxFitConversion ) continue;

      // isolation cuts
      if (doEleIso_) {
        float eleRelIso = -1;
 
        if( eleIsoSel_ == 0 ){
//          eleRelIso = (eleItr->chargedHadronIso()+eleItr->photonIso()+eleItr->neutralHadronIso()) / eleItr->pt();
          eleRelIso = (iso_ch + iso_em + iso_nh) / eleItr->pt();
        }

// Effective Areas
        if( eleIsoSel_ == 2 ){
          // effective area for isolation
          float AEff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, eta, ElectronEffectiveArea::kEleEAData2011);
          // apply to neutrals
          double rhoPrime = std::max(rhoIso, 0.0);
          double iso_n = std::max(iso_nh + iso_em - rhoPrime * AEff, 0.0);
          // compute final isolation
          eleRelIso = (iso_n + iso_ch) / pt;
        }

// Radial isolation
        if( eleIsoSel_ == 3 ){
//          eleRelIso = RadialCutBasedCalculator->GetIsolationValue(iEvent, iSetup, (*eleItr));
        }

        if (eleRelIso > cut_iso[idx] ) continue;
      }

      // electron is ID'd and isolated! - only accept if vertex present
      if (vertices->size() > 0){
         prod->push_back(pat::Electron(*eleItr));
         if( doEleProvInfo_ ){ elePFbaseProductIDptr->push_back(tofillProductID); elePFbaseKeyptr->push_back(tofillKey); }
      }
    }
  }

  // determine result before losing ownership of the pointer
  bool result = (doEleVeto_ ? (prod->size() == 0) : true);
  // store in the event
  iEvent.put(prod);
  if( doEleProvInfo_ ){
     iEvent.put( elePFbaseProductIDptr, "elePFbaseProductID" );
     iEvent.put( elePFbaseKeyptr, "elePFbaseKey" );
  }
  return result;

}

int GoldenElectronSelector::searchToPFbase(const edm::Event &iEvent, const reco::CandidatePtr& inPtr, reco::CandidatePtr & outPtr){
  outPtr = inPtr;
  if( inPtr.isNull() ){ return 0; }
  std::string transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
  while ( transModuleLabel != "particleFlow" ){
     outPtr = outPtr->sourceCandidatePtr(0);
     if( outPtr.isNonnull() ){
        transModuleLabel = iEvent.getProvenance(outPtr.id()).branchDescription().moduleLabel();
     }else{
        break;
     }
  }
  if( outPtr.isNull() ){ return 0; }
  return 1;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GoldenElectronSelector);
