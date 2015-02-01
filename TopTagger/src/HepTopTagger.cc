// -*- C++ -*-
//
// Package:   HepTopTagger 
// Class:     HepTopTagger 
//
/**\class HepTopTagger HepTopTagger.cc

Description: Tag the tops while rejecting SM top events

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Hongxuan Liu
//         Created:  Tue Aug 21 18:57:27 CDT 2012
// $Id: HepTopTagger.cc,v 1.3 2012/11/07 23:55:33 lhx Exp $
//

#include <memory>

#include "SusyAnaTools/TopTagger/interface/HepTopTagger.h"
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

HepTopTagger::HepTopTagger(const edm::ParameterSet & iConfig):
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
   hepTaggerJetSrc_(iConfig.getParameter<edm::InputTag>("hepTaggerJetSrc")),
   bTagKeyString_(iConfig.getUntrackedParameter<std::string>("bTagKeyString", "combinedSecondaryVertexBJetTags")),
   pfJetCutForJetCounting_(iConfig.existsAs<std::string>("pfJetCutForJetCounting") ? iConfig.getParameter<std::string>("pfJetCutForJetCounting") : "", true),
   pfJetCutForJetCombining_(iConfig.existsAs<std::string>("pfJetCutForJetCombining") ? iConfig.getParameter<std::string>("pfJetCutForJetCombining") : "", true),
   evtWeightInput_(iConfig.getParameter<edm::InputTag>("evtWeightInput")),
   vtxSrc_(iConfig.getParameter<edm::InputTag>("vtxSrc")),
   debug_(iConfig.getUntrackedParameter<bool>("debug", true)),
   taggingMode_(iConfig.getUntrackedParameter<bool>("taggingMode", true))
{
   cntPassnJetsCut =0; cntTaggedTopEvents =0; cntTaggedTopEventsWithinMassCuts =0;
   cntTaggedAllCutsPlusCSVS =0;
   cntPassingMTbestTopJetCut =0; cntPassingMTclosebJetCut =0;
   cntPassingMT2Cut =0; cntPassingMT2andMTCut =0; cntPassingMTbestWJetCut =0;
   cntTaggedbestFatJetPlusCSVS =0;

   type3HepTopTaggerPtr = new topTagger::type3TopTagger();

   type3HepTopTaggerPtr->setdoExtraCuts(doExtraCuts_);
   type3HepTopTaggerPtr->setmTbcut(mTbcut_);
   type3HepTopTaggerPtr->setmTtcut(mTtcut_);
   type3HepTopTaggerPtr->setMT2cut(MT2cut_);
   type3HepTopTaggerPtr->setmTWcut(mTWcut_);
   type3HepTopTaggerPtr->setdoMTMT2cuts(doMTMT2cuts_);
   type3HepTopTaggerPtr->setPDGmWmTop(mW_, mTop_);
   type3HepTopTaggerPtr->setlowRatioWoverTop(lowRatioWoverTop_);
   type3HepTopTaggerPtr->sethighRatioWoverTop(highRatioWoverTop_);
   type3HepTopTaggerPtr->setlowArcTanm13overm12(lowArcTanm13overm12_);
   type3HepTopTaggerPtr->sethighArcTanm13overm12(highArcTanm13overm12_);
   type3HepTopTaggerPtr->setlowm23overm123(lowm23overm123_);
   type3HepTopTaggerPtr->setdefaultJetCone(defaultJetCone_);
   type3HepTopTaggerPtr->setsimuCAdeltaR(simuCAdeltaR_);
   type3HepTopTaggerPtr->setsimuCALargerdeltaR(simuCALargerdeltaR_);
   type3HepTopTaggerPtr->setlowTopCut(lowTopCut_);
   type3HepTopTaggerPtr->sethighTopCut(highTopCut_);
   type3HepTopTaggerPtr->setlowWCut(lowWCut_);
   type3HepTopTaggerPtr->sethighWCut(highWCut_);
   type3HepTopTaggerPtr->setCSVS(CSVS_);
   type3HepTopTaggerPtr->setnSubJetsDiv(nSubJetsDiv_);
   type3HepTopTaggerPtr->setnJetsSel(nJetsSel_);
   type3HepTopTaggerPtr->setmaxEtaForbJets(maxEtaForbJets_);
   type3HepTopTaggerPtr->setorderingOptArr(orderingOptArr_);
   type3HepTopTaggerPtr->setmaxIndexForOrderingArr(maxIndexForOrderingArr_);
   type3HepTopTaggerPtr->setdebug(debug_);
   type3HepTopTaggerPtr->settaggingMode(taggingMode_);

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
}


HepTopTagger::~HepTopTagger() {
  if( debug_ ){
     std::cout<<"\ncntPassnJetsCut : "<<cntPassnJetsCut<<"  cntTaggedTopEvents : "<<cntTaggedTopEvents<<"  cntTaggedTopEventsWithinMassCuts : "<<cntTaggedTopEventsWithinMassCuts<<std::endl;
     std::cout<<"cntTaggedAllCutsPlusCSVS : "<<cntTaggedAllCutsPlusCSVS<<std::endl;
     std::cout<<"cntTaggedbestFatJetPlusCSVS : "<<cntTaggedbestFatJetPlusCSVS<<std::endl;
     std::cout<<"cntPassingMTbestTopJetCut : "<<cntPassingMTbestTopJetCut<<"  cntPassingMTclosebJetCut : "<<cntPassingMTclosebJetCut<<"  cntPassingMTbestWJetCut : "<<cntPassingMTbestWJetCut<<std::endl;
     std::cout<<"cntPassingMT2Cut : "<<cntPassingMT2Cut<<"  cntPassingMT2andMTCut : "<<cntPassingMT2andMTCut<<std::endl<<std::endl;
  }
}


bool HepTopTagger::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

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

   vector<vector<TLorentzVector> > hepTaggerJets;
   for(size_t i = 0; i < HEPTopTaggedCandidateHandle->size(); ++i){
      reco::BasicJetRef itJet (HEPTopTaggedCandidateHandle, i);
      reco::Jet::Constituents subjets = itJet->getJetConstituents();

      sort(subjets.begin(), subjets.end(), GreaterByPtCandPtr()); //Sort them by pt

      std::vector<TLorentzVector> subJetsVec;

      for(size_t is=0; is<subjets.size(); is++){
         TLorentzVector persubJetLVec;
         persubJetLVec.SetPtEtaPhiE(subjets[is]->p4().pt(), subjets[is]->p4().eta(), subjets[is]->p4().phi(), subjets[is]->p4().energy());
         subJetsVec.push_back(persubJetLVec);
      }
      hepTaggerJets.push_back(subJetsVec);
   }

   bool pass = type3HepTopTaggerPtr->processEvent(oriJetsVec, recoJetsBtagCSVS, metLVec, hepTaggerJets);
   if( pass ){/*empty to avoid a compiling error*/}

   std::auto_ptr<int> bestTopJetIdxPtr(new int(type3HepTopTaggerPtr->bestTopJetIdx));
   std::auto_ptr<double> bestTopJetMassPtr(new double(type3HepTopTaggerPtr->bestTopJetMass));
   std::auto_ptr<bool> remainPassCSVSPtr(new bool(type3HepTopTaggerPtr->remainPassCSVS));
   std::auto_ptr<int> pickedRemainingCombfatJetIdxPtr(new int(type3HepTopTaggerPtr->pickedRemainingCombfatJetIdx));
   std::auto_ptr<double> mTbestTopJetPtr(new double(type3HepTopTaggerPtr->mTbestTopJet));
   std::auto_ptr<double> mTbJetPtr(new double(type3HepTopTaggerPtr->mTbJet));
   std::auto_ptr<double> MT2Ptr(new double(type3HepTopTaggerPtr->MT2));
   std::auto_ptr<double> mTbestWJetPtr(new double(type3HepTopTaggerPtr->mTbestWJet));
   std::auto_ptr<double> mTbestbJetPtr(new double(type3HepTopTaggerPtr->mTbestbJet));
   std::auto_ptr<double> mTremainingTopJetPtr(new double(type3HepTopTaggerPtr->mTremainingTopJet));
   double linearCombmTbJetPlusmTbestTopJet = type3HepTopTaggerPtr->mTbJet + 0.5*type3HepTopTaggerPtr->mTbestTopJet;
   std::auto_ptr<double> linearCombmTbJetPlusmTbestTopJetPtr(new double(linearCombmTbJetPlusmTbestTopJet));
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

   if( type3HepTopTaggerPtr->isTopEvent ) cntTaggedTopEvents += evtWeight;

// Must have a fat top jet
   if( !taggingMode_ && type3HepTopTaggerPtr->bestTopJetIdx == -1 ) return false;

// Some mass cuts on the fat top jet
   if( type3HepTopTaggerPtr->bestTopJetMass > lowTopCut_ && type3HepTopTaggerPtr->bestTopJetMass < highTopCut_) cntTaggedTopEventsWithinMassCuts += evtWeight;
   if( !taggingMode_ && !(type3HepTopTaggerPtr->bestTopJetMass > lowTopCut_ && type3HepTopTaggerPtr->bestTopJetMass < highTopCut_) ) return false;

   if( type3HepTopTaggerPtr->remainPassCSVS ) cntTaggedAllCutsPlusCSVS += evtWeight;
   if( type3HepTopTaggerPtr->bestPassCSVS ) cntTaggedbestFatJetPlusCSVS += evtWeight;
// Must have a jet b-tagged!
   if( !taggingMode_ && !type3HepTopTaggerPtr->remainPassCSVS ) return false;

   if( !taggingMode_ && type3HepTopTaggerPtr->pickedRemainingCombfatJetIdx == -1 && oriJetsVec.size()>=6 ) return false; 

   if( type3HepTopTaggerPtr->mTbestTopJet > mTtcut_ ) cntPassingMTbestTopJetCut += evtWeight;
   if( (type3HepTopTaggerPtr->mTbJet + 0.5*type3HepTopTaggerPtr->mTbestTopJet) > mTbcut_ ) cntPassingMTclosebJetCut += evtWeight;
         
   if( type3HepTopTaggerPtr->MT2 > MT2cut_ ) cntPassingMT2Cut += evtWeight;

   if( (type3HepTopTaggerPtr->mTbestbJet + type3HepTopTaggerPtr->mTbestWJet) > mTWcut_ ) cntPassingMTbestWJetCut += evtWeight;

   if( type3HepTopTaggerPtr->MT2 > MT2cut_ && (type3HepTopTaggerPtr->mTbJet + 0.5*type3HepTopTaggerPtr->mTbestTopJet) > mTbcut_ ){
      cntPassingMT2andMTCut += evtWeight;
   }

   if( !taggingMode_ && doMTMT2cuts_ && !(type3HepTopTaggerPtr->MT2 > MT2cut_ && (type3HepTopTaggerPtr->mTbJet + 0.5*type3HepTopTaggerPtr->mTbestTopJet) > mTbcut_) ) return false;

   return true;
}

int HepTopTagger::pickJetsForCombining(std::vector<TLorentzVector>& oriJetsVec, std::vector<double> &recoJetsBtagCSVS){

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

int HepTopTagger::countNJets(){
   int cntNJets =0;
   for(size ij=0; ij<nJets; ij++){
      const reco::Jet & jet = (*jets)[ij];
      if( !pfJetCutForJetCounting_(jet) ) continue;
      cntNJets ++;
   }
   return cntNJets;
}

void HepTopTagger::loadRecoJets(const edm::Event& iEvent){
   iEvent.getByLabel(jetSrc_, jets); nJets = jets->size();

   isPatJet = false;
   edm::ProductID jetProdID = jets.id();
   const edm::Provenance & jetProv = iEvent.getProvenance(jetProdID);
   const std::string jetclassName = jetProv.className();
   TString jetclassNameT(jetclassName);
   if( jetclassNameT.Contains("pat::Jet") ) isPatJet = true;

   if( isPatJet ) iEvent.getByLabel(jetSrc_, patjets);

   iEvent.getByLabel(hepTaggerJetSrc_, HEPTopTaggedCandidateHandle);
}

void HepTopTagger::loadMETMHT(const edm::Event& iEvent){
   iEvent.getByLabel(metSrc_, metHandle);
   met = (*metHandle)[0].pt(); metphi = (*metHandle)[0].phi();
}

void HepTopTagger::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){
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

DEFINE_FWK_MODULE(HepTopTagger);
