// -*- C++ -*-
//
// Package:    SusyAnaTools/SkimsAUX
// Class:      prodSecondaryVertex
// 
/**\class prodSecondaryVertex prodSecondaryVertex.cc SusyAnaTools/SkimsAUX/plugins/prodSecondaryVertex.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  scarlet norberg
//         Created:  Sat, 02 Dec 2017 10:18:11 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include <vector>
#include <TMath.h>
#include <Math/SVector.h>

#include "TLorentzVector.h"
//
// class declaration
//

class prodSecondaryVertex : public edm::stream::EDFilter<> {

   public:
      explicit prodSecondaryVertex(const edm::ParameterSet&);
      ~prodSecondaryVertex();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    edm::Handle<reco::VertexCollection> pvs_;
    edm::Handle<reco::VertexCompositePtrCandidateCollection> svs_;
    edm::Handle<std::vector<pat::Jet> > jets;
/*
   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
*/
  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag vtxSrc_, secvtxSrc_, jetSrc_;
    edm::EDGetTokenT <std::vector<reco::Vertex> > VtxTok_;
    edm::EDGetTokenT < std::vector<reco::VertexCompositePtrCandidate> > SecVtxTok_;//<reco::VertexCompositePtrCandidate>  SecVtxTok_;
    //StringCutObjectSelector<reco::Vertex,true> vtxCut_; // lazy par

   edm::EDGetTokenT<std::vector<pat::Jet> >JetTok_;

    static MagneticField *paramField_;
    Measurement1D IP2D(const reco::Track &tk, const reco::Vertex &vtx) const ;
    Measurement1D IP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const ;
    Measurement1D IP3D(const reco::Track &tk, const reco::Vertex &vtx) const ;
    Measurement1D IP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const ;
    Measurement1D signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D signedIP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &vtx, const reco::Track::Vector jetdir) const ;
    //Measurement1D signedIP2D(const reco::Track &tk, reco::Vertex &vtx, const reco::Track::Vector jetdir) const ;
    //Measurement1D signedIP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D vertexD3d(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;
    Measurement1D vertexDxy(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;
    float vertexDdotP(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;

   // MagneticField *prodSecondaryVertex::paramField_ = 0;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
prodSecondaryVertex::prodSecondaryVertex(const edm::ParameterSet& iConfig)//:
  // vtxCut_(iConfig.existsAs<std::string>("vtxCut") ? iConfig.getParameter<std::string>("vtxCut") : "", true)
{
  produces<int>("");
   //now do what ever initialization is needed
  vtxSrc_ = iConfig.getParameter<edm::InputTag>("vtxSrc");
  VtxTok_ = consumes<std::vector<reco::Vertex> >(vtxSrc_);
  secvtxSrc_ = iConfig.getParameter<edm::InputTag>("secvtxSrc");
  SecVtxTok_ = consumes<std::vector<reco::VertexCompositePtrCandidate>>(secvtxSrc_);//<reco::VertexCompositePtrCandidate>(secvtxSrc_);

  jetSrc_      = iConfig.getParameter<edm::InputTag>("jetSrc");
  JetTok_ = consumes<std::vector<pat::Jet> >(jetSrc_);
  
  produces<std::vector<TLorentzVector> >("jetsLVec");
  produces<std::vector<TLorentzVector> >("svLVec");
  produces<std::vector<TLorentzVector> >("svSoftLVec");
  produces<std::vector<double> >("svPT");
  produces<std::vector<double> >("svETA");
  produces<std::vector<double> >("svPhi");
  produces<std::vector<double> >("svMass");
  produces<std::vector<double> >("svNTracks");
  produces<std::vector<double> >("svChi2");
  produces<std::vector<double> >("svNDF");
  produces<std::vector<double> >("svDXY");
  produces<std::vector<double> >("svDXYerr");
  produces<std::vector<double> >("svD3D");
  produces<std::vector<double> >("svD3Derr");
  produces<std::vector<double> >("svCosThetaSVPS");

}


prodSecondaryVertex::~prodSecondaryVertex()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
prodSecondaryVertex::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
/*
  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);

  int vtxCnt =0;
  for(unsigned int iv=0; iv<vertices->size(); iv++){
     const reco::Vertex & vtx = (*vertices)[iv];
     if( !vtxCut_(vtx) ) continue;
     vtxCnt++;
  }

  std::auto_ptr<int> prod(new int(vtxCnt));
  iEvent.put(prod);
*/

  iEvent.getByToken(VtxTok_, pvs_);
  iEvent.getByToken(SecVtxTok_, svs_);
  iEvent.getByToken(JetTok_, jets);

  std::vector<pat::Jet> extJets = (*jets);

  std::auto_ptr<std::vector<double> > svPT(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svETA(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svPhi(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svMass(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svNTracks(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svChi2(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svNDF(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svDXY(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svDXYerr(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svD3D(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svD3Derr(new std::vector<double>());
  std::auto_ptr<std::vector<double> > svCosThetaSVPS(new std::vector<double>());
  std::auto_ptr<std::vector<TLorentzVector> > jetsLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<TLorentzVector> > svLVec(new std::vector<TLorentzVector>());
  std::auto_ptr<std::vector<TLorentzVector> >  svSoftLVec(new std::vector<TLorentzVector>());

int primaryVertexIndex_ = 0;
  bool hasgoodvtx = false;//pvs_//vertices->size
  if (pvs_->size()==0 ) { hasgoodvtx = false; }
  else if (!(*pvs_)[primaryVertexIndex_].isFake() &&
	   (*pvs_)[primaryVertexIndex_].ndof() > 4. &&
	   (*pvs_)[primaryVertexIndex_].position().Rho() <= 2.0 &&
	   fabs((*pvs_)[primaryVertexIndex_].position().Z())<=24.0) { hasgoodvtx = true; }

  
  if (hasgoodvtx) {
   
    for (unsigned int i0 = 0; i0<svs_->size(); ++i0) {
  
      const reco::VertexCompositePtrCandidate &sv = (*svs_)[i0];//std::vector<reco::VertexCompositePtrCandidate> &sv = (*svs_)[i0];//reco::VertexCompositePtrCandidate &sv = (*svs_)[i0]; 
      const reco::Vertex & pv = (*pvs_)[0];

      if ( ((vertexDxy(sv,pv).value())>4.) || (vertexDdotP(sv,pv)<0.9) ) { continue; }
	svPT->push_back(sv.pt());
        svETA->push_back(sv.eta());
        svPhi->push_back(sv.phi());
	svMass->push_back(sv.mass());
	svNTracks->push_back(sv.numberOfDaughters());
	svChi2->push_back(sv.vertexChi2()); 
	svNDF->push_back(sv.vertexNdof());
	svDXY->push_back(vertexDxy(sv,pv).value());
	svDXYerr->push_back(vertexDxy(sv,pv).error()); 
	svD3D->push_back(vertexD3d(sv,pv).value());
	svD3Derr->push_back(vertexD3d(sv,pv).error());
	svCosThetaSVPS->push_back(vertexDdotP(sv,pv));
        TLorentzVector inbetweensvLVec;
        inbetweensvLVec.SetPtEtaPhiE(sv.pt(),sv.eta(),sv.phi(),sv.mass());
        svLVec->push_back(inbetweensvLVec);
 
        //std::cout<<"svPT "<<sv.pt()<<std::endl;
        }
  }

  for(unsigned int ij=0; ij < extJets.size(); ij++)
  {
    const pat::Jet& jet = extJets[ij];

    TLorentzVector perJetLVec;
    perJetLVec.SetPtEtaPhiE( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
    jetsLVec->push_back(perJetLVec);

  }

 for(unsigned int i =0; i<svLVec->size(); i++){
  // Check DeltaR with jets in the event
  if(svPT->at(i)>=20.0) continue;
  if(svNTracks->at(i)<3) continue;
  if(svDXY->at(i)<=4.0) continue;
  if(svCosThetaSVPS->at(i)<=0.98) continue;

  bool failDR = false;
  for(unsigned int j=0; j<jetsLVec->size(); j++){

  	if( ROOT::Math::VectorUtil::DeltaR( svLVec->at(i), jetsLVec->at(j) ) <= 0.4 ) {
	    failDR = true;
	    break;
	            }
	 }
      if(failDR) continue;
      svSoftLVec->push_back(svLVec->at(i));
}
      //std::cout<<"svPT"<<svPT->at(0)<<std::endl;
 
  iEvent.put(svPT, "svPT");
  iEvent.put(svETA, "svETA");
  iEvent.put(svPhi, "svPhi");
  iEvent.put(svMass, "svMass");
  iEvent.put(svNTracks, "svNTracks");
  iEvent.put(svChi2, "svChi2");
  iEvent.put(svNDF, "svNDF");
  iEvent.put(svDXY, "svDXY");
  iEvent.put(svDXYerr, "svDXYerr");
  iEvent.put(svD3D, "svD3D");
  iEvent.put(svD3Derr, "svD3Derr");
  iEvent.put(svCosThetaSVPS, "svCosThetaSVPS");
  iEvent.put(svLVec, "svLVec");
  iEvent.put(svSoftLVec, "svSoftLVec");

   using namespace edm;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
/*void
prodSecondaryVertex::beginStream(edm::StreamID)
{
}*/

// ------------ method called once each stream after processing all runs, lumis and events  ------------
/*void
prodSecondaryVertex::endStream() {
}*/

// ------------ method called when starting to processes a run  ------------
/*
void
prodSecondaryVertex::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
prodSecondaryVertex::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
prodSecondaryVertex::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
prodSecondaryVertex::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
prodSecondaryVertex::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in

 MagneticField *prodSecondaryVertex::paramField_ = 0;

Measurement1D prodSecondaryVertex::IP2D(const reco::Track &tk, const reco::Vertex &vtx) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::absoluteTransverseImpactParameter(ttk,vtx).second;
}

Measurement1D prodSecondaryVertex::IP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return IP2D(tk, svtx);
}

Measurement1D prodSecondaryVertex::IP3D(const reco::Track &tk, const reco::Vertex &vtx) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::absoluteImpactParameter3D(ttk,vtx).second;
}

Measurement1D prodSecondaryVertex::IP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return IP3D(tk, svtx);
}
/*
Measurement1D prodSecondaryVertex::signedIP2D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::signedTransverseImpactParameter(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}

Measurement1D prodSecondaryVertex::signedIP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv, const reco::Track::Vector jetdir) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return signedIP2D(tk, svtx, jetdir);
}
*/
Measurement1D prodSecondaryVertex::signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::signedImpactParameter3D(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}

Measurement1D prodSecondaryVertex::signedIP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv, const reco::Track::Vector jetdir) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return signedIP3D(tk, svtx, jetdir);
}

Measurement1D prodSecondaryVertex::vertexD3d(const reco::VertexCompositePtrCandidate &svcand, const reco::Vertex &pv) const {
  VertexDistance3D dist;
  reco::Vertex::CovarianceMatrix csv; svcand.fillVertexCovariance(csv);
  reco::Vertex svtx(svcand.vertex(), csv);
  return dist.distance(svtx, pv);
}

Measurement1D prodSecondaryVertex::vertexDxy(const reco::VertexCompositePtrCandidate &svcand, const reco::Vertex &pv) const {
  VertexDistanceXY dist;
  reco::Vertex::CovarianceMatrix csv; svcand.fillVertexCovariance(csv);
  reco::Vertex svtx(svcand.vertex(), csv);
  return dist.distance(svtx, pv);
}

float prodSecondaryVertex::vertexDdotP(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const {
  reco::Candidate::Vector p = sv.momentum();
  reco::Candidate::Vector d(sv.vx() - pv.x(), sv.vy() - pv.y(), sv.vz() - pv.z());
  return p.Unit().Dot(d.Unit());
}

DEFINE_FWK_MODULE(prodSecondaryVertex);
