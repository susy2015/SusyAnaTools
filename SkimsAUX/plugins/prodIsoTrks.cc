#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "TLorentzVector.h"

class prodIsoTrks : public edm::EDFilter {

  public:

    explicit prodIsoTrks(const edm::ParameterSet & iConfig);
    ~prodIsoTrks();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag vtxSrc_, metSrc_;

    edm::InputTag forVetoIsoTrkSrc_;
    edm::Handle<pat::PackedCandidateCollection> forVetoIsoTrks_;

    double isotrk_dR_, isotrk_dz_;

    edm::InputTag pfCandSrc_, loose_isoTrkSrc_, loose_isotrk_isoVecSrc_, loose_isotrk_dzpvVecSrc_;
    edm::Handle<pat::PackedCandidateCollection> pfCandHandle_, loose_isoTrksHandle_;

    edm::InputTag ref_all_isoTrkSrc_, ref_all_isoTrk_isoVecSrc_;
    edm::Handle<pat::PackedCandidateCollection> ref_all_isoTrksHandle_;
    edm::Handle<std::vector<double> > ref_all_isoTrks_isoVecHandle_;

    edm::InputTag W_emuVec_Src_, W_tau_emuVec_Src_, W_tau_prongsVec_Src_;
    edm::Handle<std::vector<int> > W_emuVec_, W_tau_emuVec_, W_tau_prongsVec_;

    edm::InputTag genDecayLVec_Src_;
    edm::Handle<std::vector<TLorentzVector> > genDecayLVec_;

    edm::EDGetTokenT<std::vector<int> >W_EmVec_Tok_;
    edm::EDGetTokenT<std::vector<int> >W_Tau_EmuVec_Tok_;
    edm::EDGetTokenT<std::vector<int> >W_Tau_ProngsVec_Tok_;
    edm::EDGetTokenT<std::vector<TLorentzVector> >GenDecayLVec_Tok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection>PfCandTok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> Loose_IsoTrksHandle_Tok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> Ref_All_IsoTrksHandle_Tok_;
    edm::EDGetTokenT<std::vector<double> > Ref_All_IsoTrks_IsoVecHandle_Tok_;
    edm::EDGetTokenT< std::vector<reco::Vertex> >VtxTok_;
    edm::EDGetTokenT<edm::View<reco::MET> >MetTok_;
    edm::EDGetTokenT<std::vector<double> >  Loose_Isotrk_IsoVecHandle_Tok_;
    edm::EDGetTokenT<std::vector<double> >Loose_Isotrk_DzpvVecHandle_Tok_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> ForVetoIsoTrks_Tok_;

    unsigned int loose_nIsoTrks, nIsoTrksForVeto;

    bool debug_;

    int find_idx(const reco::Candidate & target);
    int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);

    bool isData_;

    double GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track);
};


prodIsoTrks::prodIsoTrks(const edm::ParameterSet & iConfig) {

  isData_ = true;

  vtxSrc_      = iConfig.getParameter<edm::InputTag>("vtxSrc");

  metSrc_      = iConfig.getParameter<edm::InputTag>("metSrc");

  forVetoIsoTrkSrc_ = iConfig.getParameter<edm::InputTag>("forVetoIsoTrkSrc");

  pfCandSrc_        = iConfig.getParameter<edm::InputTag>("pfCandSrc");

  isotrk_dR_        = iConfig.getParameter<double>("isotrk_dR");
  isotrk_dz_        = iConfig.getParameter<double>("isotrk_dz");

  loose_isoTrkSrc_  = iConfig.getParameter<edm::InputTag>("loose_isoTrkSrc");
  loose_isotrk_isoVecSrc_ = iConfig.getParameter<edm::InputTag>("loose_isotrk_isoVecSrc");
  loose_isotrk_dzpvVecSrc_ = iConfig.getParameter<edm::InputTag>("loose_isotrk_dzpvVecSrc");

  ref_all_isoTrkSrc_ = iConfig.getParameter<edm::InputTag>("ref_all_isoTrkSrc");
  ref_all_isoTrk_isoVecSrc_ = iConfig.getParameter<edm::InputTag>("ref_all_isoTrk_isoVecSrc");

  W_emuVec_Src_ = iConfig.getParameter<edm::InputTag>("W_emuVec");
  W_tau_emuVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tau_emuVec");
  W_tau_prongsVec_Src_ = iConfig.getParameter<edm::InputTag>("W_tau_prongsVec");

  genDecayLVec_Src_ = iConfig.getParameter<edm::InputTag>("genDecayLVec");

  debug_       = iConfig.getParameter<bool>("debug");

  W_EmVec_Tok_=consumes<std::vector<int> > (W_emuVec_Src_);
  W_Tau_EmuVec_Tok_=consumes<std::vector<int> >  (W_tau_emuVec_Src_);
  W_Tau_ProngsVec_Tok_=consumes<std::vector<int> >(W_tau_prongsVec_Src_);
  GenDecayLVec_Tok_=consumes<std::vector<TLorentzVector> > (genDecayLVec_Src_);
  PfCandTok_=consumes<pat::PackedCandidateCollection> (pfCandSrc_);
  Loose_IsoTrksHandle_Tok_=consumes<pat::PackedCandidateCollection> (loose_isoTrkSrc_);
  Ref_All_IsoTrksHandle_Tok_=consumes<pat::PackedCandidateCollection> (ref_all_isoTrkSrc_);
  Ref_All_IsoTrks_IsoVecHandle_Tok_=consumes<std::vector<double> > (ref_all_isoTrk_isoVecSrc_);
  VtxTok_=consumes< std::vector<reco::Vertex> >(vtxSrc_);
  MetTok_=consumes<edm::View<reco::MET> >(metSrc_);
  Loose_Isotrk_IsoVecHandle_Tok_=consumes<std::vector<double> >  (loose_isotrk_isoVecSrc_),
  Loose_Isotrk_DzpvVecHandle_Tok_=consumes<std::vector<double> >  (loose_isotrk_dzpvVecSrc_);
  ForVetoIsoTrks_Tok_=consumes<pat::PackedCandidateCollection> (forVetoIsoTrkSrc_); 

  produces<std::vector<TLorentzVector> >("trksForIsoVetoLVec");
  produces<std::vector<double> >("trksForIsoVetocharge");
  produces<std::vector<double> >("trksForIsoVetodz");
  produces<std::vector<int> >("trksForIsoVetopdgId");
  produces<std::vector<int> >("trksForIsoVetoidx");
  produces<std::vector<double> >("trksForIsoVetoiso");
  produces<std::vector<double> >("trksForIsoVetopfActivity");

  produces<std::vector<TLorentzVector> >("looseisoTrksLVec");
  produces<std::vector<double> >("looseisoTrkscharge");
  produces<std::vector<double> >("looseisoTrksdz");
  produces<std::vector<int> >("looseisoTrkspdgId");
  produces<std::vector<int> >("looseisoTrksidx");
  produces<std::vector<double> >("looseisoTrksiso");
  produces<std::vector<double> >("looseisoTrksmtw");
  produces<std::vector<double> >("looseisoTrkspfActivity");

  produces<std::vector<int> >("forVetoIsoTrksidx");

  produces<int>("loosenIsoTrks");
  produces<int>("nIsoTrksForVeto");
}


prodIsoTrks::~prodIsoTrks() {
}


bool prodIsoTrks::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData_ = false;

  std::auto_ptr<std::vector<TLorentzVector> > trksForIsoVetoLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<double> > trksForIsoVeto_charge(new std::vector<double>());
  std::auto_ptr<std::vector<double> > trksForIsoVeto_dz(new std::vector<double>());
  std::auto_ptr<std::vector<int> > trksForIsoVeto_pdgId(new std::vector<int>());
  std::auto_ptr<std::vector<int> > trksForIsoVeto_idx(new std::vector<int>());
  std::auto_ptr<std::vector<double> > trksForIsoVeto_iso(new std::vector<double>());
  std::auto_ptr<std::vector<double> > trksForIsoVeto_pfActivity(new std::vector<double>());

  std::auto_ptr<std::vector<TLorentzVector> > loose_isoTrksLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<double> > loose_isoTrks_charge(new std::vector<double>());
  std::auto_ptr<std::vector<double> > loose_isoTrks_dz(new std::vector<double>());
  std::auto_ptr<std::vector<int> > loose_isoTrks_pdgId(new std::vector<int>());
  std::auto_ptr<std::vector<int> > loose_isoTrks_idx(new std::vector<int>());
  std::auto_ptr<std::vector<double> > loose_isoTrks_iso(new std::vector<double>());
  std::auto_ptr<std::vector<double> > loose_isoTrks_mtw(new std::vector<double>());
  std::auto_ptr<std::vector<double> > loose_isoTrks_pfActivity(new std::vector<double>());

  std::auto_ptr<std::vector<int> > forVetoIsoTrks_idx(new std::vector<int>());

  if( !isData_ ){
     iEvent.getByToken(W_EmVec_Tok_, W_emuVec_);
     iEvent.getByToken(W_Tau_EmuVec_Tok_, W_tau_emuVec_);
     iEvent.getByToken(W_Tau_ProngsVec_Tok_, W_tau_prongsVec_);
     iEvent.getByToken(GenDecayLVec_Tok_, genDecayLVec_);
  }

  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);
//  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByToken(MetTok_, met);

  iEvent.getByToken(Loose_Isotrk_IsoVecHandle_Tok_, loose_isoTrksHandle_); if( loose_isoTrksHandle_.isValid() ) loose_nIsoTrks = loose_isoTrksHandle_->size(); else loose_nIsoTrks =0;
  iEvent.getByToken(ForVetoIsoTrks_Tok_, forVetoIsoTrks_); 
  if( forVetoIsoTrks_.isValid() ) nIsoTrksForVeto = forVetoIsoTrks_->size(); else nIsoTrksForVeto =0;

  iEvent.getByToken(Ref_All_IsoTrksHandle_Tok_, ref_all_isoTrksHandle_); 
  iEvent.getByToken(Ref_All_IsoTrks_IsoVecHandle_Tok_, ref_all_isoTrks_isoVecHandle_); 

  edm::Handle<std::vector<double> >  loose_isotrk_isoVecHandle, loose_isotrk_dzpvVecHandle;
  iEvent.getByToken(Loose_Isotrk_IsoVecHandle_Tok_, loose_isotrk_isoVecHandle);
  iEvent.getByToken(Loose_Isotrk_DzpvVecHandle_Tok_, loose_isotrk_dzpvVecHandle);
  if( loose_isoTrksHandle_.isValid() && loose_isotrk_isoVecHandle.isValid() && loose_isotrk_dzpvVecHandle.isValid() ){
     if( loose_nIsoTrks != loose_isotrk_isoVecHandle->size() || loose_nIsoTrks != loose_isotrk_dzpvVecHandle->size() ){
        std::cout<<"ERROR ... mis-matching between loose_nIsoTrks : "<<loose_nIsoTrks<<"  loose_isotrk_isoVecHandle->size : "<<loose_isotrk_isoVecHandle->size()<<"  loose_isotrk_dzpvVecHandle->size : "<<loose_isotrk_dzpvVecHandle->size()<<std::endl;
     }
  }

  if( debug_ ) std::cout<<"\nloose_nIsoTrks : "<<loose_nIsoTrks<<"  nIsoTrksForVeto : "<<nIsoTrksForVeto<<std::endl;
  for(unsigned int is=0; is<loose_nIsoTrks; is++){
     const pat::PackedCandidate isoTrk = (*loose_isoTrksHandle_)[is];
     double isoTrkpt = isoTrk.pt(), isoTrketa = isoTrk.eta(), isoTrkphi = isoTrk.phi(), isoTrkenergy = isoTrk.energy();
     double isoTrkcharge = isoTrk.charge();

     TLorentzVector perIsoTrkLVec;
     perIsoTrkLVec.SetPtEtaPhiE(isoTrkpt, isoTrketa, isoTrkphi, isoTrkenergy);
     loose_isoTrksLVec->push_back(perIsoTrkLVec);

     double mtw = sqrt( 2*( (*met)[0].pt()*(*loose_isoTrksHandle_)[is].pt() -( (*met)[0].px()*(*loose_isoTrksHandle_)[is].px() + (*met)[0].py()*(*loose_isoTrksHandle_)[is].py() ) ) );

     loose_isoTrks_charge->push_back(isoTrkcharge);
     loose_isoTrks_dz->push_back((*loose_isotrk_dzpvVecHandle)[is]);
     loose_isoTrks_pdgId->push_back((*loose_isoTrksHandle_)[is].pdgId());
     loose_isoTrks_iso->push_back((*loose_isotrk_isoVecHandle)[is]);
     loose_isoTrks_mtw->push_back(mtw);

     if( debug_ ){
        std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrkpt<<"/"<<isoTrketa<<"/"<<isoTrkphi<<"/"<<isoTrkcharge<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : "<<(*loose_isotrk_dzpvVecHandle)[is]<<"  iso/pt : "<<(*loose_isotrk_isoVecHandle)[is]/isoTrkpt<<std::endl;
     }
  }
  if( debug_ ) std::cout<<std::endl;

  iEvent.getByToken(PfCandTok_, pfCandHandle_);
  if( pfCandHandle_.isValid() ){
     for(unsigned int ip=0; ip<pfCandHandle_->size(); ip++){

        if( std::isnan((*pfCandHandle_)[ip].pt()) ) continue;

        double perIso = 9999.0;
        for(unsigned int is=0; is< ref_all_isoTrksHandle_->size(); is++){
           if( (*ref_all_isoTrksHandle_)[is].pt() == (*pfCandHandle_)[ip].pt() && (*ref_all_isoTrksHandle_)[is].eta() == (*pfCandHandle_)[ip].eta() && (*ref_all_isoTrksHandle_)[is].phi() == (*pfCandHandle_)[ip].phi() && (*ref_all_isoTrksHandle_)[is].energy() == (*pfCandHandle_)[ip].energy() && (*ref_all_isoTrksHandle_)[is].pdgId() == (*pfCandHandle_)[ip].pdgId() ){
              perIso = (*ref_all_isoTrks_isoVecHandle_)[is];
           }
        }

        TLorentzVector perLVec;
        perLVec.SetPtEtaPhiE( (*pfCandHandle_)[ip].pt(), (*pfCandHandle_)[ip].eta(), (*pfCandHandle_)[ip].phi(), (*pfCandHandle_)[ip].energy() );

        for(unsigned int is=0; is<forVetoIsoTrks_->size(); is++){
           if( (*forVetoIsoTrks_)[is].pt() == (*pfCandHandle_)[ip].pt() && (*forVetoIsoTrks_)[is].eta() == (*pfCandHandle_)[ip].eta() && (*forVetoIsoTrks_)[is].phi() == (*pfCandHandle_)[ip].phi() && (*forVetoIsoTrks_)[is].energy() == (*pfCandHandle_)[ip].energy() && (*forVetoIsoTrks_)[is].pdgId() == (*pfCandHandle_)[ip].pdgId() ){
              forVetoIsoTrks_idx->push_back(ip);
           }
        }

        int perCharge = pfCandHandle_->at(ip).charge(); 
        if( perCharge ==0 ) continue;

        double dz = (*pfCandHandle_)[ip].dz();
        if( fabs(dz) > isotrk_dz_ ) continue;

        double pfActivity = GetTrackActivity(pfCandHandle_, &(*pfCandHandle_)[ip]);

        int matched = 0;
        for(unsigned int is=0; is<loose_nIsoTrks; is++){
           double perdeltaR = perLVec.DeltaR(loose_isoTrksLVec->at(is));
           if( perdeltaR < isotrk_dR_ ) matched ++;
           if( (*loose_isoTrksHandle_)[is].pt() == (*pfCandHandle_)[ip].pt() && (*loose_isoTrksHandle_)[is].eta() == (*pfCandHandle_)[ip].eta() && (*loose_isoTrksHandle_)[is].phi() == (*pfCandHandle_)[ip].phi() && (*loose_isoTrksHandle_)[is].energy() == (*pfCandHandle_)[ip].energy() && (*loose_isoTrksHandle_)[is].pdgId() == (*pfCandHandle_)[ip].pdgId() ){
              loose_isoTrks_idx->push_back(ip);
              loose_isoTrks_pfActivity->push_back(pfActivity);
           }
        }
        if( !isData_ ){
           for(unsigned int ig=0; ig<W_emuVec_->size(); ig++){
              int perIdx = W_emuVec_->at(ig);
              TLorentzVector genLVec = genDecayLVec_->at(perIdx);
              double perdeltaR = perLVec.DeltaR(genLVec);
              if( perdeltaR < isotrk_dR_ ) matched ++;
           }
   
           for(unsigned int ig=0; ig<W_tau_emuVec_->size(); ig++){
              int perIdx = W_tau_emuVec_->at(ig);
              TLorentzVector genLVec = genDecayLVec_->at(perIdx);
              double perdeltaR = perLVec.DeltaR(genLVec);
              if( perdeltaR < isotrk_dR_ ) matched ++;
           }
   
           for(unsigned int ig=0; ig<W_tau_prongsVec_->size(); ig++){
              int perIdx = W_tau_prongsVec_->at(ig);
              TLorentzVector genLVec = genDecayLVec_->at(perIdx);
              double perdeltaR = perLVec.DeltaR(genLVec);
              if( perdeltaR < isotrk_dR_ ) matched ++;
           }
        }

        if( !matched ) continue;

        trksForIsoVetoLVec->push_back(perLVec);
        trksForIsoVeto_charge->push_back(perCharge);
        trksForIsoVeto_dz->push_back(dz);
        trksForIsoVeto_pdgId->push_back(pfCandHandle_->at(ip).pdgId());
        trksForIsoVeto_idx->push_back(ip);
        trksForIsoVeto_iso->push_back(perIso);
        trksForIsoVeto_pfActivity->push_back(pfActivity);
     }
  }
  if( debug_ ){
     std::cout<<"\nntrksForIsoVeto : "<<trksForIsoVetoLVec->size()<<std::endl;
     std::cout<<"idx of forVetoIsoTrks : ";
     for(unsigned int is=0; is<nIsoTrksForVeto; is++){
        std::cout<<"  "<<forVetoIsoTrks_idx->at(is);
     }
     std::cout<<std::endl;
     std::cout<<"idx of loose_isoTrks : ";
     for(unsigned int is=0; is<loose_nIsoTrks; is++){
        std::cout<<"  "<<loose_isoTrks_idx->at(is);
     }
     std::cout<<std::endl;
  }

  // store in the event

  std::auto_ptr<int>  loose_nIsoTrksPtr(new int);
  *loose_nIsoTrksPtr = loose_nIsoTrks;

  std::auto_ptr<int> nIsoTrksForVetoPtr(new int);
  *nIsoTrksForVetoPtr = nIsoTrksForVeto;
   
  iEvent.put(trksForIsoVetoLVec, "trksForIsoVetoLVec");
  iEvent.put(trksForIsoVeto_charge, "trksForIsoVetocharge");
  iEvent.put(trksForIsoVeto_dz, "trksForIsoVetodz");
  iEvent.put(trksForIsoVeto_pdgId, "trksForIsoVetopdgId");
  iEvent.put(trksForIsoVeto_idx, "trksForIsoVetoidx");
  iEvent.put(trksForIsoVeto_iso, "trksForIsoVetoiso");
  iEvent.put(trksForIsoVeto_pfActivity, "trksForIsoVetopfActivity");

  iEvent.put(loose_isoTrksLVec, "looseisoTrksLVec");
  iEvent.put(loose_isoTrks_charge, "looseisoTrkscharge");
  iEvent.put(loose_isoTrks_dz, "looseisoTrksdz");
  iEvent.put(loose_isoTrks_pdgId, "looseisoTrkspdgId");
  iEvent.put(loose_isoTrks_idx, "looseisoTrksidx");
  iEvent.put(loose_isoTrks_iso, "looseisoTrksiso");
  iEvent.put(loose_isoTrks_mtw, "looseisoTrksmtw");
  iEvent.put(loose_isoTrks_pfActivity, "looseisoTrkspfActivity");

  iEvent.put(forVetoIsoTrks_idx, "forVetoIsoTrksidx");

  iEvent.put(loose_nIsoTrksPtr, "loosenIsoTrks");
  iEvent.put(nIsoTrksForVetoPtr, "nIsoTrksForVeto");

  return true;
}

double prodIsoTrks::GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track) {
  if (track->pt()<5.) return -1.0;
  double trkiso(0.); 
  double r_iso = 0.3;
  for (const pat::PackedCandidate &other_pfc : *other_pfcands) {
      if (other_pfc.charge()==0) continue;
      double dr = deltaR(other_pfc, *track);
      if (dr < r_iso || dr > 0.4) continue; // activity annulus
      float dz_other = other_pfc.dz();
      if( fabs(dz_other) > 0.1 ) continue;
      trkiso += other_pfc.pt();
    }
    double activity = trkiso/track->pt();
    return activity;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(prodIsoTrks);
