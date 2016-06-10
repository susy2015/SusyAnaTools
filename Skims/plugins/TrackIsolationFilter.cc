//-----------------------------------------------------------------------------------------
//
// Computation of the trackIsolation, for use with the isolated track veto 
// used for the stop quark search in the single lepton channel
// Author: Ben Hooberman
//
// For each PFCandidate above threshold minPt_PFCandidate store 4 quantities:
// pT of PFCandidate
// charge of PFCandidate
// dz of PFCandidate w.r.t. the 1st good vertex
// the trackIsolation value
//
// In the analysis, we veto any event containing IN ADDITION TO the selected lepton a charged PFCandidate with:
// pT > 10 GeV, dz < 0.05 cm, and trackIso/pT < 0.1
//
//-----------------------------------------------------------------------------------------

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SusyAnaTools/Skims/plugins/TrackIsolationFilter.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "TMath.h"
#include "TLorentzVector.h"
#include "TTree.h"

using namespace reco;
using namespace edm;
using namespace std;

//
// class decleration
//

//
// constructors and destructor
//

TrackIsolationFilter::TrackIsolationFilter(const edm::ParameterSet& iConfig) {

  pfCandidatesTag_		= iConfig.getParameter<InputTag>	("pfCandidatesTag");
  vertexInputTag_               = iConfig.getParameter<InputTag>        ("vertexInputTag");
  
  dR_               = iConfig.getParameter<double>          ("dR_ConeSize");       // dR value used to define the isolation cone                (default 0.3 )
  dzcut_            = iConfig.getParameter<double>          ("dz_CutValue");       // cut value for dz(trk,vtx) for track to include in iso sum (default 0.05)
  minPt_            = iConfig.getParameter<double>          ("minPt_PFCandidate"); // store PFCandidates with pt above this cut                 (default 0   )
  isoCut_           = iConfig.getParameter<double>          ("isoCut"); // isolation cut value
  doTrkIsoVeto_     = iConfig.getParameter<bool>            ("doTrkIsoVeto");

  exclPdgIdVec_     = iConfig.getParameter<std::vector<int> > ("exclPdgIdVec"); 
  PfcandTok_ = consumes<pat::PackedCandidateCollection>(pfCandidatesTag_);
  VertexInputTok_ =consumes<edm::View<reco::Vertex> >(vertexInputTag_);
  produces<std::vector<pat::PackedCandidate> >(""); 
  produces<vector<double> >("pfcandstrkiso").setBranchAlias("pfcands_trkiso");
  produces<vector<double> >("pfcandsdzpv"  ).setBranchAlias("pfcands_dzpv");
  produces<vector<double> >("pfcandspt"    ).setBranchAlias("pfcands_pt");
  produces<vector<double> >("pfcandschg"   ).setBranchAlias("pfcands_chg");
  produces<int>("Isotracksize").setBranchAlias("Isotrack_size");

}

TrackIsolationFilter::~TrackIsolationFilter() {

}

// ------------ method called to produce the data  ------------

bool TrackIsolationFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  auto_ptr<vector<double> >  pfcands_trkiso(new vector<double>);
  auto_ptr<vector<double> >  pfcands_dzpv  (new vector<double>);
  auto_ptr<vector<double> >  pfcands_pt    (new vector<double>);
  auto_ptr<vector<double> >  pfcands_chg   (new vector<double>);
  auto_ptr<int> Isotrack_size (new int);

  //---------------------------------
  // get PFCandidate collection
  //---------------------------------
 
  edm::Handle<pat::PackedCandidateCollection> pfCandidatesHandle;
  iEvent.getByToken(PfcandTok_, pfCandidatesHandle);

  //---------------------------------
  // get Vertex Collection
  //---------------------------------

  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByToken(VertexInputTok_, vertices);
//  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  vtxSize = vertices->size();
  
  //-------------------------------------------------------------------------------------------------
  // loop over PFCandidates and calculate the trackIsolation and dz w.r.t. 1st good PV for each one
  // for neutral PFCandidates, store trkiso = 999 and dzpv = 999
  //-------------------------------------------------------------------------------------------------

  std::auto_ptr<std::vector<pat::PackedCandidate> > prod(new std::vector<pat::PackedCandidate>());

  if( vertices->size() > 0) {

     for( pat::PackedCandidateCollection::const_iterator pf_it = pfCandidatesHandle->begin(); pf_it != pfCandidatesHandle->end(); pf_it++ ) {

        //-------------------------------------------------------------------------------------
        // only store PFCandidate values if pt > minPt
        //-------------------------------------------------------------------------------------

// to save space, only store charged particles
        if( pf_it->charge() != 0 ){
           pfcands_pt->push_back(pf_it->pt());
           pfcands_chg->push_back(pf_it->charge());
        }

        if( std::isnan(pf_it->pt()) || std::isinf(pf_it->pt()) ) continue;

        if( pf_it->pt() < minPt_ ) continue;

        //-------------------------------------------------------------------------------------
        // store pt and charge of PFCandidate
        //-------------------------------------------------------------------------------------

        //-------------------------------------------------------------------------------------
        // if there's no good vertex in the event, we can't calculate anything so store 999999
        //-------------------------------------------------------------------------------------
    
        //-------------------------------------------------------
        // require PFCandidate is charged, otherwise store 999 
        //-------------------------------------------------------

        if( pf_it->charge() != 0 ){

           //----------------------------------------------------------------------------
           // now loop over other PFCandidates in the event to calculate trackIsolation
           //----------------------------------------------------------------------------

           double trkiso = 0.0;

           for( pat::PackedCandidateCollection::const_iterator pf_other = pfCandidatesHandle->begin(); pf_other != pfCandidatesHandle->end(); pf_other++ ) {

              // don't count the PFCandidate in its own isolation sum
              if( pf_it == pf_other       ) continue;

	      // require the PFCandidate to be charged
	      if( pf_other->charge() == 0 ) continue;

              // cut on dR between the PFCandidates
              double dR = deltaR(pf_it->eta(), pf_it->phi(), pf_other->eta(), pf_other->phi());
              if( dR > dR_ ) continue;

	      // cut on the PFCandidate dz
	      double dz_other = pf_other->dz();

	      if( fabs(dz_other) > dzcut_ ) continue;

              if( std::find( exclPdgIdVec_.begin(), exclPdgIdVec_.end(), pf_other->pdgId() ) != exclPdgIdVec_.end() ) continue;

	      trkiso += pf_other->pt();
           }

           // calculate the dz of this candidate
           double dz_it = pf_it->dz();

           if( trkiso/pf_it->pt() > isoCut_ ) continue;
           if( std::abs(dz_it) > dzcut_ ) continue;

           // store trkiso and dz values
           pfcands_dzpv->push_back(dz_it);
           pfcands_trkiso->push_back(trkiso);
           prod->push_back( (*pf_it) );

        }else{
           //neutral particle, set trkiso and dzpv to 9999
        }

     }
      
  }
  *Isotrack_size = prod->size();

  bool result = (doTrkIsoVeto_ ? (prod->size() == 0) : true);

  // put trkiso and dz values back into event
  iEvent.put(pfcands_trkiso,"pfcandstrkiso");
  iEvent.put(pfcands_dzpv  ,"pfcandsdzpv"  );
  iEvent.put(pfcands_pt    ,"pfcandspt"    );
  iEvent.put(pfcands_chg   ,"pfcandschg"   );

  iEvent.put(prod);
  iEvent.put(Isotrack_size, "Isotracksize");

  return result;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackIsolationFilter);

