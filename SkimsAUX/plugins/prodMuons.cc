#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/METReco/interface/MET.h"

#include "TLorentzVector.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"

class prodMuons : public edm::EDFilter {

  public:

    explicit prodMuons(const edm::ParameterSet & iConfig);
    ~prodMuons();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag muonSrc_;
    edm::InputTag vtxSrc_;
    edm::InputTag metSrc_;
    edm::InputTag pfCandsSrc_;
    bool debug_;
    bool doMuonVeto_, doMuonID_, doMuonVtx_, doMuonIso_;
    double minMuPt_, maxMuEta_, maxMuD0_, maxMuDz_, maxMuRelIso_, minMuNumHit_;

};


prodMuons::prodMuons(const edm::ParameterSet & iConfig) {
  muonSrc_     = iConfig.getParameter<edm::InputTag>("MuonSource");
  vtxSrc_      = iConfig.getParameter<edm::InputTag>("VertexSource");
  metSrc_      = iConfig.getParameter<edm::InputTag>("metSource");
  pfCandsSrc_  = iConfig.getParameter<edm::InputTag>("packedPFCandidates");
  minMuPt_     = iConfig.getParameter<double>("MinMuPt");
  maxMuEta_    = iConfig.getParameter<double>("MaxMuEta");
  maxMuD0_     = iConfig.getParameter<double>("MaxMuD0");
  maxMuDz_     = iConfig.getParameter<double>("MaxMuDz");
  maxMuRelIso_ = iConfig.getParameter<double>("MaxMuRelIso");
  minMuNumHit_ = iConfig.getParameter<double>("MinMuNumHit");
  doMuonVeto_  = iConfig.getParameter<bool>("DoMuonVeto");
  doMuonID_    = iConfig.getParameter<bool>("DoMuonID");
  doMuonVtx_   = iConfig.getParameter<bool>("DoMuonVtxAssociation");
  doMuonIso_   = iConfig.getParameter<bool>("DoMuonIsolation");
  debug_       = iConfig.getParameter<bool>("Debug");
  produces<std::vector<pat::Muon> >("");
  produces<std::vector<TLorentzVector> >("muonsLVec");
  produces<std::vector<double> >("muonsCharge");
  produces<std::vector<double> >("muonsMtw");
  produces<std::vector<double> >("muonsRelIso");
  produces<std::vector<double> >("muonsMiniIso");
  produces<int>("nMuons");
}


prodMuons::~prodMuons() {
}


bool prodMuons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // read in the objects
  edm::Handle<std::vector<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_, muons);
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxSrc_, vertices);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByLabel(metSrc_, met);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByLabel(pfCandsSrc_, pfcands);

  // check which ones to keep
  std::auto_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());
  std::auto_ptr<std::vector<TLorentzVector> > muonsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<double> > muonsCharge(new std::vector<double>());
  std::auto_ptr<std::vector<double> > muonsMtw(new std::vector<double>());
  std::auto_ptr<std::vector<double> > muonsRelIso(new std::vector<double>());
  std::auto_ptr<std::vector<double> > muonsMiniIso(new std::vector<double>());

  int imuon =0;

  if (vertices->size() > 0) {
    for (std::vector<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m) {

      // acceptance cuts
      if (m->pt() < minMuPt_) continue;
      if (std::abs(m->eta()) >= maxMuEta_) continue;

      if(debug_ && (m->muonID("AllGlobalMuons") != 0) ) {
	std::cout << "iMuon " << imuon << ": "
		  << " (pt,eta,phi) "<<m->pt()<<", "<<m->eta()<<", "<<m->phi() << " "
		  << " isPFMuon " << m->isPFMuon() << "\n"
	          << " NormChi2 "<<m->globalTrack()->normalizedChi2() 
		  << ", NValidMuonHits " << m->globalTrack()->hitPattern().numberOfValidMuonHits()
		  << ", MMatchedStations " << m->numberOfMatchedStations()
		  << ", NValidPixelHits " << m->innerTrack()->hitPattern().numberOfValidPixelHits()
		  << ", trackerLayersWithMeasurement "  << m->innerTrack()->hitPattern().trackerLayersWithMeasurement() 
		  << "\n"
		  << " vertexSize " << vertices->size() << ", dxy " << std::abs(m->innerTrack()->dxy(vtxpos))
		  << ", dz " << std::abs(m->vz() - vtxpos.z() )
		  << std::endl;

	std::cout << " sumChargedHadronPt " << m->pfIsolationR04().sumChargedHadronPt
		  << ", sumNeutralHadronEt " << m->pfIsolationR04().sumNeutralHadronEt
		  << ", sumPhotonEt " << m->pfIsolationR04().sumPhotonEt
		  << ", sumPUPt " << m->pfIsolationR04().sumPUPt 
		  << ", relIso " <<  (m->pfIsolationR04().sumChargedHadronPt + std::max(0., m->pfIsolationR04().sumNeutralHadronEt + m->pfIsolationR04().sumPhotonEt - 0.5*m->pfIsolationR04().sumPUPt) )/ m->pt()
		  << std::endl;

      }

      // ID cuts - always ask isGlobalMuon()
      if (m->muonID("AllGlobalMuons") == 0) continue;
      if (doMuonID_) {
	if(!m->isPFMuon() ) continue; 
	if( m->globalTrack()->normalizedChi2() >= 10. ) continue;
	if( m->globalTrack()->hitPattern().numberOfValidMuonHits() <=0 ) continue;
	if( m->numberOfMatchedStations() <=1 ) continue;
        if( m->innerTrack()->hitPattern().numberOfValidPixelHits() == 0) continue;
	if( m->innerTrack()->hitPattern().trackerLayersWithMeasurement() <=5 ) continue;
	if(debug_) {std::cout << "PassedMuon ID" << std::endl;}
      }
      // vertex association cuts - ignore if no vertex (see further)
      if (doMuonVtx_) {
        if (std::abs(m->innerTrack()->dxy(vtxpos)) >= maxMuD0_ && vertices->size() > 0) continue;
	if (std::abs(m->innerTrack()->dz(vtxpos))  >= maxMuDz_ && vertices->size() > 0) continue;
	if(debug_) {std::cout << "PassedMuon Vtx Association" << std::endl;}
      }
      // isolation cuts
      float muRelIso = -1;
      muRelIso = (m->pfIsolationR04().sumChargedHadronPt + std::max(0., m->pfIsolationR04().sumNeutralHadronEt + m->pfIsolationR04().sumPhotonEt - 0.5*m->pfIsolationR04().sumPUPt) )/ m->pt();
      if (doMuonIso_) {
        //if (m->pfCandidateRef().isNull()) { // check wether this is a std or PF muon
        if (muRelIso >= maxMuRelIso_) continue;
	if(debug_) {std::cout << "PassedMuon Isolation" << std::endl;}
      }

      double miniIso = commonFunctions::getPFIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*m)), 0.05, 0.2, 10., false, false);

      // muon is ID'd and isolated! - only accept if vertex present
      if (vertices->size() > 0){
         prod->push_back(pat::Muon(*m));
         TLorentzVector perLVec; perLVec.SetPtEtaPhiE(m->pt(), m->eta(), m->phi(), m->energy());
         muonsLVec->push_back(perLVec);

         double mtw = sqrt( 2*( (*met)[0].pt()*m->pt() -( (*met)[0].px()*m->px() + (*met)[0].py()*m->py() ) ) );

         muonsCharge->push_back(m->charge());
         muonsMtw->push_back(mtw);
         muonsRelIso->push_back(muRelIso);
         muonsMiniIso->push_back(miniIso);
      }
    }
  }
    
  // determine result before losing ownership of the pointer
  bool result = (doMuonVeto_ ? (prod->size() == 0) : true);

  std::auto_ptr<int> nMuons (new int);

  *nMuons = prod->size();

  // store in the event
  iEvent.put(prod);
  iEvent.put(muonsLVec, "muonsLVec");
  iEvent.put(muonsCharge, "muonsCharge");
  iEvent.put(muonsMtw, "muonsMtw");
  iEvent.put(muonsRelIso, "muonsRelIso");
  iEvent.put(muonsMiniIso, "muonsMiniIso");
  iEvent.put(nMuons, "nMuons");

  return result;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodMuons);
