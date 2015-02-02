// -*- C++ -*-
//
// Package:   TopTagger 
// Class:     TopTagger 
//
/**\class TopTagger TopTagger.cc

Description: Tag the tops while rejecting SM top events

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Hongxuan Liu
//         Created:  Tue Aug 21 18:57:27 CDT 2012
// $Id: TopTagger.cc,v 1.10 2012/12/18 04:59:32 lhx Exp $
//

#include <memory>

#include "SusyAnaTools/TopTagger/interface/TopTagger.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "recipeAUX/OxbridgeMT2/interface/Basic_Mt2_332_Calculator.h"
#include "recipeAUX/OxbridgeMT2/interface/ChengHanBisect_Mt2_332_Calculator.h"

#include "TTree.h"
#include "TLorentzVector.h"

using namespace stdindexSort;
using namespace stdcomb;
using namespace std;

TopTagger::TopTagger(const edm::ParameterSet & iConfig):
   doExtraCuts_(iConfig.getUntrackedParameter<bool>("doExtraCuts", true)),
   mTbcut_(iConfig.getUntrackedParameter<double>("mTbcut", 500)),
   mTtcut_(iConfig.getUntrackedParameter<double>("mTtcut", 365)),
   MT2cut_(iConfig.getUntrackedParameter<double>("MT2cut", 300)),
   mTWcut_(iConfig.getUntrackedParameter<double>("mTWcut", 600)),
   doMTMT2cuts_(iConfig.getUntrackedParameter<bool>("doMTMT2cuts", true)),
   mW_(iConfig.getUntrackedParameter<double>("mW", 80.385)),
   mTop_(iConfig.getUntrackedParameter<double>("mTop", 173.5)),
   mWoverTop_(mW_/mTop_),
   lowRatioWoverTop_(iConfig.getUntrackedParameter<double>("lowRatioWoverTop", 0.85)),
   highRatioWoverTop_(iConfig.getUntrackedParameter<double>("highRatioWoverTop", 1.25)),
   lowArcTanm13overm12_(iConfig.getUntrackedParameter<double>("lowArcTanm13overm12", 0.2)),
   highArcTanm13overm12_(iConfig.getUntrackedParameter<double>("highArcTanm13overm12", 1.3)),
   lowm23overm123_(iConfig.getUntrackedParameter<double>("lowm23overm123", 0.35)),
   Rmin_(lowRatioWoverTop_*mWoverTop_),
   Rmax_(highRatioWoverTop_*mWoverTop_),
   defaultJetCone_(iConfig.getUntrackedParameter<double>("defaultJetCone", 2.0)),
   simuCAdeltaR_(iConfig.getUntrackedParameter<double>("simuCAdeltaR", 1.5)),
   simuCALargerdeltaR_(iConfig.getUntrackedParameter<double>("simuCALargerdeltaR", -1)),
   lowTopCut_(iConfig.getUntrackedParameter<double>("lowTopCut", 80)),
   highTopCut_(iConfig.getUntrackedParameter<double>("highTopCut", 270)),
   lowWCut_(iConfig.getUntrackedParameter<double>("lowWCut", 50)),
   highWCut_(iConfig.getUntrackedParameter<double>("highWCut", 120)),
   CSVS_(iConfig.getUntrackedParameter<double>("CSVS", 0.679)),
   nSubJetsDiv_(iConfig.getUntrackedParameter<int>("nSubJetsDiv", 3)),
   nJetsSel_(iConfig.getUntrackedParameter<int>("nJetsSel", 5)),
   maxEtaForbJets_(iConfig.getUntrackedParameter<double>("maxEtaForbJets", 2.4)),
   defaultOrderingOptVec(defaultOrderingOptArr, defaultOrderingOptArr+sizeof(defaultOrderingOptArr)/sizeof(defaultOrderingOptArr[0])),
   orderingOptArr_(iConfig.getUntrackedParameter<std::vector<std::string> >("orderingOptArr", defaultOrderingOptVec)),
   defaultMaxIndexForOrderingVec(defaultMaxIndexForOrderingArr, defaultMaxIndexForOrderingArr+sizeof(defaultMaxIndexForOrderingArr)/sizeof(defaultMaxIndexForOrderingArr[0])),
   maxIndexForOrderingArr_(iConfig.getUntrackedParameter<std::vector<int> >("maxIndexForOrderingArr", defaultMaxIndexForOrderingVec)),
   metSrc_(iConfig.getParameter<edm::InputTag>("metSrc")),
   jetSrc_(iConfig.getParameter<edm::InputTag>("jetSrc")),
   bTagKeyString_(iConfig.getUntrackedParameter<std::string>("bTagKeyString", "combinedInclusiveSecondaryVertexV2BJetTags")),
   pfJetCutForJetCounting_(iConfig.existsAs<std::string>("pfJetCutForJetCounting") ? iConfig.getParameter<std::string>("pfJetCutForJetCounting") : "", true),
   pfJetCutForJetCombining_(iConfig.existsAs<std::string>("pfJetCutForJetCombining") ? iConfig.getParameter<std::string>("pfJetCutForJetCombining") : "", true),
   evtWeightInput_(iConfig.getParameter<edm::InputTag>("evtWeightInput")),
   vtxSrc_(iConfig.getParameter<edm::InputTag>("vtxSrc")),
   debug_(iConfig.getUntrackedParameter<bool>("debug", true)),
   taggingMode_(iConfig.getUntrackedParameter<bool>("taggingMode", true)),
   dobVetoCS_(iConfig.getUntrackedParameter<bool>("dobVetoCS", false))
{
   cntPassnJetsCut =0; cntTaggedTopEvents =0; cntTaggedTopEventsWithinMassCuts =0;
   cntTaggedAllCutsPlusCSVS =0;
   cntPassingMTbestTopJetCut =0; cntPassingMTclosebJetCut =0;
   cntPassingMT2Cut =0; cntPassingMT2andMTCut =0; cntPassingMTbestWJetCut =0;
   cntTaggedbestFatJetPlusCSVS =0;

   type3TopTaggerPtr = new topTagger::type3TopTagger();

   type3TopTaggerPtr->setdoExtraCuts(doExtraCuts_);
   type3TopTaggerPtr->setmTbcut(mTbcut_);
   type3TopTaggerPtr->setmTtcut(mTtcut_);
   type3TopTaggerPtr->setMT2cut(MT2cut_);
   type3TopTaggerPtr->setmTWcut(mTWcut_);
   type3TopTaggerPtr->setdoMTMT2cuts(doMTMT2cuts_);
   type3TopTaggerPtr->setPDGmWmTop(mW_, mTop_);
   type3TopTaggerPtr->setlowRatioWoverTop(lowRatioWoverTop_);
   type3TopTaggerPtr->sethighRatioWoverTop(highRatioWoverTop_);
   type3TopTaggerPtr->setlowArcTanm13overm12(lowArcTanm13overm12_);
   type3TopTaggerPtr->sethighArcTanm13overm12(highArcTanm13overm12_);
   type3TopTaggerPtr->setlowm23overm123(lowm23overm123_);
   type3TopTaggerPtr->setdefaultJetCone(defaultJetCone_);
   type3TopTaggerPtr->setsimuCAdeltaR(simuCAdeltaR_);
   type3TopTaggerPtr->setsimuCALargerdeltaR(simuCALargerdeltaR_);
   type3TopTaggerPtr->setlowTopCut(lowTopCut_);
   type3TopTaggerPtr->sethighTopCut(highTopCut_);
   type3TopTaggerPtr->setlowWCut(lowWCut_);
   type3TopTaggerPtr->sethighWCut(highWCut_);
   type3TopTaggerPtr->setCSVS(CSVS_);
   type3TopTaggerPtr->setnSubJetsDiv(nSubJetsDiv_);
   type3TopTaggerPtr->setnJetsSel(nJetsSel_);
   type3TopTaggerPtr->setmaxEtaForbJets(maxEtaForbJets_);
   type3TopTaggerPtr->setorderingOptArr(orderingOptArr_);
   type3TopTaggerPtr->setmaxIndexForOrderingArr(maxIndexForOrderingArr_);
   type3TopTaggerPtr->setdebug(debug_);
   type3TopTaggerPtr->settaggingMode(taggingMode_);
   type3TopTaggerPtr->setdobVetoCS(dobVetoCS_);

   produces<int>("bestTopJetIdx");
   produces<double>("bestTopJetMass");
   produces<bool>("remainPassCSVS");
   produces<int>("pickedRemainingCombfatJetIdx");
   produces<double>("mTbestTopJet");
   produces<double>("mTbJet");
   produces<double>("MT2");
   produces<double>("mTbestWJet");
   produces<double>("mTbestbJet");
   produces<double>("mTremainingTopJet");
   produces<double>("linearCombmTbJetPlusmTbestTopJet");

   produces<std::vector<std::vector<int> > >("finalCombfatJets");
   produces<std::vector<int> >("allCombPassCriteriaVec");
}


TopTagger::~TopTagger() {
  if( debug_ ){
     std::cout<<"\ncntPassnJetsCut : "<<cntPassnJetsCut<<"  cntTaggedTopEvents : "<<cntTaggedTopEvents<<"  cntTaggedTopEventsWithinMassCuts : "<<cntTaggedTopEventsWithinMassCuts<<std::endl;
     std::cout<<"cntTaggedAllCutsPlusCSVS : "<<cntTaggedAllCutsPlusCSVS<<std::endl;
     std::cout<<"cntTaggedbestFatJetPlusCSVS : "<<cntTaggedbestFatJetPlusCSVS<<std::endl;
     std::cout<<"cntPassingMTbestTopJetCut : "<<cntPassingMTbestTopJetCut<<"  cntPassingMTclosebJetCut : "<<cntPassingMTclosebJetCut<<"  cntPassingMTbestWJetCut : "<<cntPassingMTbestWJetCut<<std::endl;
     std::cout<<"cntPassingMT2Cut : "<<cntPassingMT2Cut<<"  cntPassingMT2andMTCut : "<<cntPassingMT2andMTCut<<std::endl<<std::endl;
  }
}


bool TopTagger::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

   loadEventInfo(iEvent, iSetup);

   loadRecoJets(iEvent);
   if( !isPatJet ){ std::cout<<"Not a pat::jet input! Need it for bTag information..."<<std::endl; return false; }

   double evtWeight = 1.0;
   if( evtWeight_.isValid() ) evtWeight = (*evtWeight_);

   cntPassnJetsCut += evtWeight;

   loadMETMHT(iEvent);

   TLorentzVector metLVec; metLVec.SetPtEtaPhiM(met, 0, metphi, 0);

// The index has to be the same between jets & btags!
   vector<TLorentzVector> oriJetsVec; vector<double> recoJetsBtagCSVS;
   pickJetsForCombining(oriJetsVec, recoJetsBtagCSVS);

   bool pass = type3TopTaggerPtr->processEvent(oriJetsVec, recoJetsBtagCSVS, metLVec);
   if( pass ){/*empty to avoid a compling error*/}

   std::auto_ptr<int> bestTopJetIdxPtr(new int(type3TopTaggerPtr->bestTopJetIdx));
   std::auto_ptr<double> bestTopJetMassPtr(new double(type3TopTaggerPtr->bestTopJetMass));
   std::auto_ptr<bool> remainPassCSVSPtr(new bool(type3TopTaggerPtr->remainPassCSVS));
   std::auto_ptr<int> pickedRemainingCombfatJetIdxPtr(new int(type3TopTaggerPtr->pickedRemainingCombfatJetIdx));
   std::auto_ptr<double> mTbestTopJetPtr(new double(type3TopTaggerPtr->mTbestTopJet));
   std::auto_ptr<double> mTbJetPtr(new double(type3TopTaggerPtr->mTbJet));
   std::auto_ptr<double> MT2Ptr(new double(type3TopTaggerPtr->MT2));
   std::auto_ptr<double> mTbestWJetPtr(new double(type3TopTaggerPtr->mTbestWJet));
   std::auto_ptr<double> mTbestbJetPtr(new double(type3TopTaggerPtr->mTbestbJet));
   std::auto_ptr<double> mTremainingTopJetPtr(new double(type3TopTaggerPtr->mTremainingTopJet));
   double linearCombmTbJetPlusmTbestTopJet = type3TopTaggerPtr->mTbJet + 0.5*type3TopTaggerPtr->mTbestTopJet;
   std::auto_ptr<double> linearCombmTbJetPlusmTbestTopJetPtr(new double(linearCombmTbJetPlusmTbestTopJet));

   std::auto_ptr<std::vector<std::vector<int> > > finalCombfatJetsPtr(new std::vector<std::vector<int> >());
   for(unsigned int ic=0; ic<type3TopTaggerPtr->finalCombfatJets.size(); ic++){ finalCombfatJetsPtr->push_back(type3TopTaggerPtr->finalCombfatJets[ic]); }

   std::auto_ptr<std::vector<int> > allCombPassCriteriaVecPtr(new std::vector<int>());
   for(unsigned int ic=0; ic<type3TopTaggerPtr->allCombPassCriteriaVec.size(); ic++){ allCombPassCriteriaVecPtr->push_back(type3TopTaggerPtr->allCombPassCriteriaVec[ic]); }

   iEvent.put(bestTopJetIdxPtr, "bestTopJetIdx");
   iEvent.put(bestTopJetMassPtr, "bestTopJetMass");
   iEvent.put(remainPassCSVSPtr, "remainPassCSVS");
   iEvent.put(pickedRemainingCombfatJetIdxPtr, "pickedRemainingCombfatJetIdx");
   iEvent.put(mTbestTopJetPtr, "mTbestTopJet");
   iEvent.put(mTbJetPtr, "mTbJet");
   iEvent.put(MT2Ptr, "MT2");
   iEvent.put(mTbestWJetPtr, "mTbestWJet");
   iEvent.put(mTbestbJetPtr, "mTbestbJet");
   iEvent.put(mTremainingTopJetPtr, "mTremainingTopJet");
   iEvent.put(linearCombmTbJetPlusmTbestTopJetPtr, "linearCombmTbJetPlusmTbestTopJet");

   iEvent.put(finalCombfatJetsPtr, "finalCombfatJets");
  
   iEvent.put(allCombPassCriteriaVecPtr, "allCombPassCriteriaVec");

   if( type3TopTaggerPtr->isTopEvent ) cntTaggedTopEvents += evtWeight;

// Must have a fat top jet
   if( !taggingMode_ && type3TopTaggerPtr->bestTopJetIdx == -1 ) return false;

// Some mass cuts on the fat top jet
   if( type3TopTaggerPtr->bestTopJetMass > lowTopCut_ && type3TopTaggerPtr->bestTopJetMass < highTopCut_) cntTaggedTopEventsWithinMassCuts += evtWeight;
   if( !taggingMode_ && !(type3TopTaggerPtr->bestTopJetMass > lowTopCut_ && type3TopTaggerPtr->bestTopJetMass < highTopCut_) ) return false;

   if( type3TopTaggerPtr->remainPassCSVS ) cntTaggedAllCutsPlusCSVS += evtWeight;
   if( type3TopTaggerPtr->bestPassCSVS ) cntTaggedbestFatJetPlusCSVS += evtWeight;
// Must have a jet b-tagged!
   if( !taggingMode_ && !type3TopTaggerPtr->remainPassCSVS ) return false;

   if( !taggingMode_ && type3TopTaggerPtr->pickedRemainingCombfatJetIdx == -1 && oriJetsVec.size()>=6 ) return false; 

   if( type3TopTaggerPtr->mTbestTopJet > mTtcut_ ) cntPassingMTbestTopJetCut += evtWeight;
   if( (type3TopTaggerPtr->mTbJet + 0.5*type3TopTaggerPtr->mTbestTopJet) > mTbcut_ ) cntPassingMTclosebJetCut += evtWeight;
         
   if( type3TopTaggerPtr->MT2 > MT2cut_ ) cntPassingMT2Cut += evtWeight;

   if( (type3TopTaggerPtr->mTbestbJet + type3TopTaggerPtr->mTbestWJet) > mTWcut_ ) cntPassingMTbestWJetCut += evtWeight;

   if( type3TopTaggerPtr->MT2 > MT2cut_ && (type3TopTaggerPtr->mTbJet + 0.5*type3TopTaggerPtr->mTbestTopJet) > mTbcut_ ){
      cntPassingMT2andMTCut += evtWeight;
   }

   if( !taggingMode_ && doMTMT2cuts_ && !(type3TopTaggerPtr->MT2 > MT2cut_ && (type3TopTaggerPtr->mTbJet + 0.5*type3TopTaggerPtr->mTbestTopJet) > mTbcut_) ) return false;

   return true;
}

int TopTagger::pickJetsForCombining(std::vector<TLorentzVector>& oriJetsVec, std::vector<double> &recoJetsBtagCSVS){

   oriJetsVec.clear(); recoJetsBtagCSVS.clear();

//   reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

   int cntNJets =0;
   for(size ij=0; ij<nJets; ij++){
      const reco::Jet & jet = (*jets)[ij];
      if( !pfJetCutForJetCombining_(jet) ) continue;
      TLorentzVector jetLVec;
      jetLVec.SetPtEtaPhiE(jet.pt(), jet.eta(), jet.phi(), jet.energy());
      oriJetsVec.push_back(jetLVec);
      const pat::Jet & patjet = (*patjets)[ij];
      double btag = patjet.bDiscriminator(bTagKeyString_.c_str());
      recoJetsBtagCSVS.push_back(btag);
      cntNJets ++;
   }

   return cntNJets;
}

int TopTagger::countNJets(){
   int cntNJets =0;
   for(size ij=0; ij<nJets; ij++){
      const reco::Jet & jet = (*jets)[ij];
      if( !pfJetCutForJetCounting_(jet) ) continue;
      cntNJets ++;
   }
   return cntNJets;
}

void TopTagger::loadRecoJets(const edm::Event& iEvent){
   iEvent.getByLabel(jetSrc_, jets); nJets = jets->size();

   isPatJet = false;
   edm::ProductID jetProdID = jets.id();
   const edm::Provenance & jetProv = iEvent.getProvenance(jetProdID);
   const std::string jetclassName = jetProv.className();
   TString jetclassNameT(jetclassName);
   if( jetclassNameT.Contains("pat::Jet") ) isPatJet = true;

   if( isPatJet ) iEvent.getByLabel(jetSrc_, patjets);
}

void TopTagger::loadMETMHT(const edm::Event& iEvent){
   iEvent.getByLabel(metSrc_, metHandle);
   met = (*metHandle)[0].pt(); metphi = (*metHandle)[0].phi();
}

void TopTagger::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){
// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;

// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   lumi = iEvent.luminosityBlock();

// Get vertices
   iEvent.getByLabel(vtxSrc_, vertices); vtxSize = vertices->size();

// Get event weight
   iEvent.getByLabel(evtWeightInput_, evtWeight_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TopTagger);
