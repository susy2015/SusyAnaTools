#include <memory>
#include <algorithm>
#include <assert.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/METReco/interface/MET.h"

#include "TLorentzVector.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"

class prodMuons : public edm::EDFilter
{
 public:
  explicit prodMuons(const edm::ParameterSet & iConfig);
  ~prodMuons();
 private:
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag muonSrc_;
  edm::InputTag vtxSrc_;
  edm::InputTag metSrc_;
  edm::InputTag pfCandsSrc_;
  edm::InputTag rhoSrc_;
  edm::EDGetTokenT<std::vector<pat::Muon>> MuonTok_;
  edm::EDGetTokenT< std::vector<reco::Vertex> > VtxTok_;
  edm::EDGetTokenT<edm::View<reco::MET> > MetTok_;
  edm::EDGetTokenT<pat::PackedCandidateCollection>  PfcandTok_;
  edm::EDGetTokenT<double> RhoTok_;
  bool debug_;
  bool doMuonVeto_, doMuonID_, doMuonVtx_;
  int doMuonIso_; // 0: don't do any isolation; 1: relIso;  2: miniIso
  double minMuPt_, maxMuEta_, maxMuD0_, maxMuDz_, maxMuRelIso_, maxMuMiniIso_, minMuNumHit_;
  double minMuPtForMuon2Clean_;
  bool specialFix_;
  edm::InputTag badGlobalMuonTaggerSrc_, cloneGlobalMuonTaggerSrc_;
  edm::EDGetTokenT<edm::PtrVector<reco::Muon>> badGlobalMuonTok_, cloneGlobalMuonTok_;

  bool isLooseMuon(const pat::Muon & muon);
  bool isMediumMuon(const pat::Muon & muon, const reco::Vertex::Point & vtxpos);
  bool isTightMuon(const pat::Muon & muon, const reco::Vertex::Point & vtxpos);
  bool isTightMuonOld(const pat::Muon & muon, const reco::Vertex::Point & vtxpos);
};

prodMuons::prodMuons(const edm::ParameterSet & iConfig) 
{
  muonSrc_      = iConfig.getParameter<edm::InputTag>("MuonSource");
  vtxSrc_       = iConfig.getParameter<edm::InputTag>("VertexSource");
  metSrc_       = iConfig.getParameter<edm::InputTag>("metSource");
  pfCandsSrc_   = iConfig.getParameter<edm::InputTag>("PFCandSource");
  rhoSrc_       = iConfig.getParameter<edm::InputTag>("RhoSource");
  minMuPt_      = iConfig.getParameter<double>("MinMuPt");
  maxMuEta_     = iConfig.getParameter<double>("MaxMuEta");
  maxMuD0_      = iConfig.getParameter<double>("MaxMuD0");
  maxMuDz_      = iConfig.getParameter<double>("MaxMuDz");
  maxMuRelIso_  = iConfig.getParameter<double>("MaxMuRelIso");
  maxMuMiniIso_ = iConfig.getParameter<double>("MaxMuMiniIso");
  minMuNumHit_  = iConfig.getParameter<double>("MinMuNumHit");
  doMuonVeto_   = iConfig.getParameter<bool>("DoMuonVeto");
  doMuonID_     = iConfig.getParameter<bool>("DoMuonID");
  doMuonVtx_    = iConfig.getParameter<bool>("DoMuonVtxAssociation");
  doMuonIso_    = iConfig.getParameter<int>("DoMuonIsolation");
  debug_        = iConfig.getParameter<bool>("Debug");
  specialFix_   = iConfig.getParameter<bool>("specialFix");

  if( specialFix_ ){
     badGlobalMuonTaggerSrc_ = iConfig.getParameter<edm::InputTag>("badGlobalMuonTaggerSrc");   
     cloneGlobalMuonTaggerSrc_ = iConfig.getParameter<edm::InputTag>("cloneGlobalMuonTaggerSrc");   
     badGlobalMuonTok_ = consumes<edm::PtrVector<reco::Muon>>(badGlobalMuonTaggerSrc_);
     cloneGlobalMuonTok_ = consumes<edm::PtrVector<reco::Muon>>(cloneGlobalMuonTaggerSrc_);
  }

  minMuPtForMuon2Clean_ = iConfig.getUntrackedParameter<double>("minMuPtForMuon2Clean", 10);

  MuonTok_ = consumes<std::vector<pat::Muon>>(muonSrc_);
  VtxTok_ = consumes<std::vector<reco::Vertex>>(vtxSrc_);
  MetTok_ = consumes<edm::View<reco::MET>>(metSrc_);
  PfcandTok_ = consumes<pat::PackedCandidateCollection>(pfCandsSrc_);
  RhoTok_ = consumes<double>(rhoSrc_);

  produces<std::vector<pat::Muon> >("");
  produces<std::vector<pat::Muon> >("mu2Clean");
  //0: loose;  1: medium;  2: tight 
  produces<std::vector<int> >("muonsIDtype");
  produces<std::vector<int> >("muonsFlagLoose");
  produces<std::vector<int> >("muonsFlagMedium");
  produces<std::vector<int> >("muonsFlagTight");
  produces<std::vector<TLorentzVector> >("muonsLVec");
  produces<std::vector<double> >("muonsCharge");
  produces<std::vector<double> >("muonsMtw");
  produces<std::vector<double> >("muonsRelIso");
  produces<std::vector<double> >("muonsMiniIso");
  produces<std::vector<double> >("muonspfActivity");
  produces<int>("nMuons");
  if( specialFix_ ){
// 0 : good muon wrt the specialFix  1 : badGlobalMuon  2 : cloneGlobalMuon  3 : badGlobalMuon & cloneGlobalMuon
// 00                               01                 10                   11                    
// Only store bad muons and store their LorentzVector and charge -> so should be no 0 in the specialFixtype vector
     produces<std::vector<int>>("specialFixtype");
     produces<std::vector<TLorentzVector>>("specialFixMuonsLVec");
     produces<std::vector<double>>("specialFixMuonsCharge");
  }
}

prodMuons::~prodMuons()
{
}

bool prodMuons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //read in the objects
  //edm::Handle<edm::View<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Muon> > muons;
  iEvent.getByToken(MuonTok_, muons);
  //edm::Handle<std::vector<pat::Muon> > muons;
  //edm::Handle<edm::View<pat::Muon> > muons;
  //iEvent.getByToken(muonSrc_, muons);
  //iEvent.getByLabel(MuonTok_, muons);
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);
  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());
  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByToken(MetTok_, met);

  edm::Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByToken(PfcandTok_, pfcands);

  edm::Handle< double > rho_;
  iEvent.getByToken(RhoTok_,rho_); 
  //iEvent.getByToken("fixedGridRhoFastjetCentralNeutral", rho_); // Central rho recommended for SUSY
  double rho = *rho_;

  //check which ones to keep
  std::unique_ptr<std::vector<pat::Muon> > prod(new std::vector<pat::Muon>());
  std::unique_ptr<std::vector<pat::Muon> > mu2Clean(new std::vector<pat::Muon>());
  std::unique_ptr<std::vector<TLorentzVector> > muonsLVec(new std::vector<TLorentzVector>());
  std::unique_ptr<std::vector<double> > muonsCharge(new std::vector<double>());
  std::unique_ptr<std::vector<double> > muonsMtw(new std::vector<double>());
  std::unique_ptr<std::vector<double> > muonsRelIso(new std::vector<double>());
  std::unique_ptr<std::vector<double> > muonsMiniIso(new std::vector<double>());
  std::unique_ptr<std::vector<double> > muonspfActivity(new std::vector<double>());

  std::unique_ptr<std::vector<int> > muonsIDtype(new std::vector<int>());
  std::unique_ptr<std::vector<int> > muonsFlagLoose(new std::vector<int>());
  std::unique_ptr<std::vector<int> > muonsFlagMedium(new std::vector<int>());
  std::unique_ptr<std::vector<int> > muonsFlagTight(new std::vector<int>());

  std::unique_ptr<std::vector<int>> specialFixtype(new std::vector<int>());
  std::unique_ptr<std::vector<TLorentzVector>> specialFixMuonsLVec(new std::vector<TLorentzVector>());
  std::unique_ptr<std::vector<double>> specialFixMuonsCharge(new std::vector<double>());

  if( specialFix_ ){
     edm::Handle<edm::PtrVector<reco::Muon>> badGlobalMuonPtr_handle, cloneGlobalMuonPtr_handle;
     iEvent.getByToken(badGlobalMuonTok_, badGlobalMuonPtr_handle);
     iEvent.getByToken(cloneGlobalMuonTok_, cloneGlobalMuonPtr_handle);
     std::vector<TLorentzVector> dupVec;
     for(edm::PtrVector<reco::Muon>::const_iterator itr_badGlobalMuonPtr = badGlobalMuonPtr_handle->begin(); itr_badGlobalMuonPtr != badGlobalMuonPtr_handle->end(); ++itr_badGlobalMuonPtr){
        const reco::Muon & perMuon = **itr_badGlobalMuonPtr;
        bool isdup = false;
        for(edm::PtrVector<reco::Muon>::const_iterator itr_cloneGlobalMuonPtr = cloneGlobalMuonPtr_handle->begin(); itr_cloneGlobalMuonPtr != cloneGlobalMuonPtr_handle->end(); ++itr_cloneGlobalMuonPtr){
           if( *itr_cloneGlobalMuonPtr == *itr_badGlobalMuonPtr ){ isdup = true; break; }
        }
        if( isdup ) dupVec.emplace_back(TLorentzVector(perMuon.px(), perMuon.py(), perMuon.pz(), perMuon.energy()));
        else{
           specialFixtype->push_back(1);
           specialFixMuonsLVec->emplace_back(TLorentzVector(perMuon.px(), perMuon.py(), perMuon.pz(), perMuon.energy()));
           specialFixMuonsCharge->push_back(perMuon.charge());
        }
     }
     for(edm::PtrVector<reco::Muon>::const_iterator itr_cloneGlobalMuonPtr = cloneGlobalMuonPtr_handle->begin(); itr_cloneGlobalMuonPtr != cloneGlobalMuonPtr_handle->end(); ++itr_cloneGlobalMuonPtr){
        const reco::Muon & perMuon = **itr_cloneGlobalMuonPtr;
        int specialFix_type = 2;
        for(auto dup: dupVec){
           if( perMuon.px() == dup.Px() && perMuon.py() == dup.Py() && perMuon.pz() == dup.Pz() && perMuon.energy() == dup.Energy() ) specialFix_type += 1;
        }
        assert( specialFix_type <= 3 ); 
        specialFixtype->push_back( specialFix_type );
        specialFixMuonsLVec->emplace_back(TLorentzVector(perMuon.px(), perMuon.py(), perMuon.pz(), perMuon.energy()));
        specialFixMuonsCharge->push_back(perMuon.charge());
     }
  }

  if (vertices->size() > 0) 
  {
    //for (std::vector<int> m = muons->begin(); m != muons->end(); ++m) {
    for (std::vector<pat::Muon>::const_iterator m = muons->begin(); m != muons->end(); ++m)
    {
      //acceptance cuts
      if (m->pt() < minMuPt_) continue;
      if (std::abs(m->eta()) >= maxMuEta_) continue;

      bool isLooseID = isLooseMuon((*m));
      bool isMediumID = isMediumMuon((*m), vtxpos);
      bool isTightID = isTightMuon((*m), vtxpos);

      // only store muons passing medium or tight ID
      if ( !(isMediumID || isTightID) ) continue;

      // isolation cuts
      double muRelIso = (m->pfIsolationR04().sumChargedHadronPt + std::max(0., m->pfIsolationR04().sumNeutralHadronEt + m->pfIsolationR04().sumPhotonEt - 0.5*m->pfIsolationR04().sumPUPt) ) / m->pt();
      double miniIso = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*m)), "muon", rho);
      double pfActivity = commonFunctions::GetMiniIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*m)), "muon", rho, true);

      if (doMuonIso_ ==1 ) 
      {
        //if (m->pfCandidateRef().isNull()) { // check wether this is a std or PF muon
        if (muRelIso >= maxMuRelIso_) continue;
        if(debug_) {std::cout << "PassedMuon Isolation" << std::endl;}
      } 
      else if (doMuonIso_ ==2 )
      {
        if (miniIso >= maxMuMiniIso_ ) continue;
      }

      // muon is ID'd and isolated! - only accept if vertex present
      if (vertices->size() > 0)
      {
        prod->push_back(pat::Muon(*m));
        TLorentzVector perLVec; perLVec.SetPtEtaPhiE(m->pt(), m->eta(), m->phi(), m->energy());
        muonsLVec->push_back(perLVec);

        double mtw = sqrt( 2*( (*met)[0].pt()*m->pt() -( (*met)[0].px()*m->px() + (*met)[0].py()*m->py() ) ) );

        muonsCharge->push_back(m->charge());
        muonsMtw->push_back(mtw);
        muonsRelIso->push_back(muRelIso);
        muonsMiniIso->push_back(miniIso);

        int idtype =-1;
        if( isMediumID ) idtype = 1;
        if( isTightID ) idtype = 2;

        muonsIDtype->push_back(idtype); 

        if( isLooseID ) muonsFlagLoose->push_back(1); else muonsFlagLoose->push_back(0);
        if( isMediumID ) muonsFlagMedium->push_back(1); else muonsFlagMedium->push_back(0);
        if( isTightID ) muonsFlagTight->push_back(1); else muonsFlagTight->push_back(0);

        muonspfActivity->push_back(pfActivity);
      }
      //add muons to clean from jets 
      if(isMediumID && miniIso < maxMuMiniIso_ && m->pt() > minMuPtForMuon2Clean_) mu2Clean->push_back(*m);
    }
  }
    
  // determine result before losing ownership of the pointer
  bool result = (doMuonVeto_ ? (prod->size() == 0) : true);

  std::unique_ptr<int> nMuons (new int);

  *nMuons = prod->size();

  // store in the event
  iEvent.put(std::move(prod));
  iEvent.put(std::move(mu2Clean), "mu2Clean");
  iEvent.put(std::move(muonsIDtype), "muonsIDtype");
  iEvent.put(std::move(muonsFlagLoose), "muonsFlagLoose");
  iEvent.put(std::move(muonsFlagMedium), "muonsFlagMedium");
  iEvent.put(std::move(muonsFlagTight), "muonsFlagTight");
  iEvent.put(std::move(muonsLVec), "muonsLVec");
  iEvent.put(std::move(muonsCharge), "muonsCharge");
  iEvent.put(std::move(muonsMtw), "muonsMtw");
  iEvent.put(std::move(muonsRelIso), "muonsRelIso");
  iEvent.put(std::move(muonsMiniIso), "muonsMiniIso");
  iEvent.put(std::move(muonspfActivity), "muonspfActivity");
  iEvent.put(std::move(nMuons), "nMuons");
  if( specialFix_ ){
     iEvent.put(std::move(specialFixtype), "specialFixtype");
     iEvent.put(std::move(specialFixMuonsLVec), "specialFixMuonsLVec");
     iEvent.put(std::move(specialFixMuonsCharge), "specialFixMuonsCharge");
  }

  return result;
}

bool prodMuons::isLooseMuon(const pat::Muon & muon)
{
  bool isLoose = true;
  if(doMuonID_)
  {
    isLoose = muon.isLooseMuon();
  }
  return isLoose;
}

bool prodMuons::isMediumMuon(const pat::Muon & muon, const reco::Vertex::Point & vtxPos )
{
  //Always default to true. If don't do muon ID (i.e., doMuonID_ = false) or don't do muon vtx (i.e., doMuonVtx_ = false), then the muon passes!
  bool isMedium = true, isMediumVtx = true;
  //medium WP + dz/dxy cuts
  bool goodGlob = muon.isGlobalMuon() && 
                  muon.globalTrack()->normalizedChi2() < 3 && 
                  muon.combinedQuality().chi2LocalPosition < 12 && 
                  muon.combinedQuality().trkKink < 20; 
  if(doMuonID_)
  {
    isMedium = muon.isLooseMuon() && 
               muon.innerTrack()->validFraction() > 0.8 && //revert back to standard before we introduce the HIPs and mitigation, see email in : https://hypernews.cern.ch/HyperNews/CMS/get/susy/2250.html
               //muon.innerTrack()->validFraction() > 0.49 && // Short term ID tunning to mitigate the HIP impact on lower tracker hit efficiencies
               muon.segmentCompatibility() > (goodGlob ? 0.303 : 0.451);
  }

  if(doMuonVtx_)
  {
    isMediumVtx = muon.dB() < maxMuD0_ && fabs(muon.muonBestTrack()->dz(vtxPos)) < maxMuDz_;
  }

  bool isMediumPlus = isMedium && isMediumVtx;
  return isMediumPlus; 
}

bool prodMuons::isTightMuon(const pat::Muon & muon, const reco::Vertex::Point & vtxpos)
{
  bool isTight = true;
  if(debug_ && (muon.muonID("AllGlobalMuons") != 0) ) 
  {
    std::cout << " (pt,eta,phi) "<<muon.pt()<<", "<<muon.eta()<<", "<<muon.phi() << " "
              << " isPFMuon " << muon.isPFMuon() << "\n"
              << " NormChi2 "<<muon.globalTrack()->normalizedChi2() 
              << ", NValidMuonHits " << muon.globalTrack()->hitPattern().numberOfValidMuonHits()
              << ", MMatchedStations " << muon.numberOfMatchedStations()
              << ", NValidPixelHits " << muon.innerTrack()->hitPattern().numberOfValidPixelHits()
              << ", trackerLayersWithMeasurement "  << muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() 
              << "\n"
              << ", dxy " << std::abs(muon.innerTrack()->dxy(vtxpos))
              << ", dz " << std::abs(muon.vz() - vtxpos.z() )
              << std::endl;

    std::cout << " sumChargedHadronPt " << muon.pfIsolationR04().sumChargedHadronPt
              << ", sumNeutralHadronEt " << muon.pfIsolationR04().sumNeutralHadronEt
              << ", sumPhotonEt " << muon.pfIsolationR04().sumPhotonEt
              << ", sumPUPt " << muon.pfIsolationR04().sumPUPt 
              << ", relIso " <<  (muon.pfIsolationR04().sumChargedHadronPt + std::max(0., muon.pfIsolationR04().sumNeutralHadronEt + muon.pfIsolationR04().sumPhotonEt - 0.5*muon.pfIsolationR04().sumPUPt) )/ muon.pt()
              << std::endl;
  }

  // ID cuts - always ask isGlobalMuon()
  if (muon.muonID("AllGlobalMuons") == 0){ isTight = false; return isTight; }
  if (doMuonID_) 
  {
    if(!muon.isPFMuon() ) isTight = false; 
    if( muon.globalTrack()->normalizedChi2() >= 10. ) isTight = false;
    if( muon.globalTrack()->hitPattern().numberOfValidMuonHits() <=0 ) isTight = false;
    if( muon.numberOfMatchedStations() <=1 ) isTight = false;
    if( muon.innerTrack()->hitPattern().numberOfValidPixelHits() == 0) isTight = false;
    if( muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() <=5 ) isTight = false;
    if(debug_) {std::cout << "PassedMuon ID" << std::endl;}
  }

  // vertex association cuts - ignore if no vertex (see further)
  if (doMuonVtx_) 
  {
    if (std::abs(muon.innerTrack()->dxy(vtxpos)) >= maxMuD0_) isTight = false;
    if (std::abs(muon.innerTrack()->dz(vtxpos))  >= maxMuDz_) isTight = false;
    if(debug_) {std::cout << "PassedMuon Vtx Association" << std::endl;}
  }
  return isTight; 
}

// isTightMuonOld is a backup of old method but currently it's the same as isTightMuon
bool prodMuons::isTightMuonOld(const pat::Muon & muon, const reco::Vertex::Point & vtxpos)
{
  bool isTight = true;
  if(debug_ && (muon.muonID("AllGlobalMuons") != 0) ) 
  {
    std::cout << " (pt,eta,phi) "<<muon.pt()<<", "<<muon.eta()<<", "<<muon.phi() << " "
              << " isPFMuon " << muon.isPFMuon() << "\n"
              << " NormChi2 "<<muon.globalTrack()->normalizedChi2() 
              << ", NValidMuonHits " << muon.globalTrack()->hitPattern().numberOfValidMuonHits()
              << ", MMatchedStations " << muon.numberOfMatchedStations()
              << ", NValidPixelHits " << muon.innerTrack()->hitPattern().numberOfValidPixelHits()
              << ", trackerLayersWithMeasurement "  << muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() 
              << "\n"
              << ", dxy " << std::abs(muon.innerTrack()->dxy(vtxpos))
              << ", dz " << std::abs(muon.vz() - vtxpos.z() )
              << std::endl;

    std::cout << " sumChargedHadronPt " << muon.pfIsolationR04().sumChargedHadronPt
              << ", sumNeutralHadronEt " << muon.pfIsolationR04().sumNeutralHadronEt
              << ", sumPhotonEt " << muon.pfIsolationR04().sumPhotonEt
              << ", sumPUPt " << muon.pfIsolationR04().sumPUPt 
              << ", relIso " <<  (muon.pfIsolationR04().sumChargedHadronPt + std::max(0., muon.pfIsolationR04().sumNeutralHadronEt + muon.pfIsolationR04().sumPhotonEt - 0.5*muon.pfIsolationR04().sumPUPt) ) / muon.pt()
              << std::endl;
  }

  // ID cuts - always ask isGlobalMuon()
  if (muon.muonID("AllGlobalMuons") == 0){ isTight = false; return isTight; }
  if (doMuonID_) 
  {
    if(!muon.isPFMuon() ) isTight = false; 
    if( muon.globalTrack()->normalizedChi2() >= 10. ) isTight = false;
    if( muon.globalTrack()->hitPattern().numberOfValidMuonHits() <=0 ) isTight = false;
    if( muon.numberOfMatchedStations() <=1 ) isTight = false;
    if( muon.innerTrack()->hitPattern().numberOfValidPixelHits() == 0) isTight = false;
    if( muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() <=5 ) isTight = false;
    if(debug_) {std::cout << "PassedMuon ID" << std::endl;}
  }

  // vertex association cuts - ignore if no vertex (see further)
  if (doMuonVtx_) 
  {
    if (std::abs(muon.innerTrack()->dxy(vtxpos)) >= maxMuD0_) isTight = false;
    if (std::abs(muon.innerTrack()->dz(vtxpos))  >= maxMuDz_) isTight = false;
    if(debug_) {std::cout << "PassedMuon Vtx Association" << std::endl;}
  }

  return isTight; 
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodMuons);
