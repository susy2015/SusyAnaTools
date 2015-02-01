// -*- C++ -*-
//
// Package:    RA2SignalScan
// Class:      RA2SignalScan
// 
/**\class RA2SignalScan RA2SignalScan.cc 

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Hongxuan Liu
//         Created:  Sun Oct 02 20:30:13 CDT 2011
// $Id: RA2SignalScan.cc,v 1.2 2012/08/10 03:02:01 lhx Exp $
//
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/Ptr.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "FWCore/Framework/interface/FileBlock.h"

// TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include "TH2D.h"
//
// class declaration
//

typedef unsigned int size;

class RA2SignalScan : public edm::EDAnalyzer {
public:
  explicit RA2SignalScan(const edm::ParameterSet&);
  ~RA2SignalScan();
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  virtual void respondToOpenInputFile(edm::FileBlock const&);
  virtual void respondToCloseInputFile(edm::FileBlock const&);

// "mSUGRA" : for mSUGRA scan
// "SMS" : for Simplified Model
  std::string  susyScanType_;

// If "SMS" type, then use this to specify the topology
// "T1", "T2" and so on
  std::string   susyScanTopology_;

  std::vector<double> getLHEmSUGRApars(const LHEEventProduct& lhep);
  std::vector<double> getLHESMSpars(const LHEEventProduct& lhep);

  size run, event, ls; bool isData;
  edm::InputTag vtxSrc_;
  edm::Handle<edm::View<reco::Vertex> > vertices;
  size nVtxPUcut_, vtxSize;
  edm::InputTag evtWeightInput_;
  edm::Handle<double> evtWeight_;
  void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

// 1 : hadTau only  2 : lostLep only  3 : both
  int doSignalSC_;
  edm::InputTag hadTauSCweightInputTag_;
  edm::Handle<std::vector<double> > hadTauSCweights_;

  edm::InputTag lostLepSCweightInputTag_;
  edm::Handle<double> lostLepSCweights_;

  edm::InputTag metSrc_;
  edm::Handle<edm::View<reco::MET> > met;
  edm::InputTag mhtSrc_, mht_forSgnfSrc_;
  edm::Handle<edm::View<reco::MET> > mht, mht_forSgnf;
  virtual void loadMETMHT(const edm::Event& iEvent);
    
  edm::InputTag htSrc_;
  edm::Handle<double> ht;
  virtual void loadHT(const edm::Event& iEvent);

  edm::InputTag forVetoMuonSrc_;
  edm::Handle<edm::View<reco::Muon > > forVetoMuons;
  edm::InputTag forVetoElectronSrc_;
  edm::Handle<edm::View<pat::Electron > > forVetoElectrons;
  size nMuonsForVeto, nElectronsForVeto;
  reco::Muon muon1, muon2; int muon1Charge, muon2Charge;
  pat::Electron ele1, ele2; int ele1Charge, ele2Charge;
  int multipliedCharge;
  virtual void loadLeptons(const edm::Event& iEvent);

// (0, 0): no lepton veto (important that veto can be controled by external configuration)
// (1, 1) exclusive                          (1, 0) inclusive
// (2, 1) same sign  (2, -1) opposite sign   (2, 0) inclusive
// (3, 1) exclusive                          (3, 0) inclusive
// Generally abs(2nd index) ==1 means inclusive;  0 means exclusive
// For our current purpose, we should have:
// (1, 1), (2, 1) (2, -1) (3, 0) to cover all possibility
// Default should be (0, 0)
  std::vector<int> nVetoLeptons_;
    
// Values used for MHT and HT cuts for all sets of searching regions
// Always 1-1 pairs and always HT first then MHT
// If any of the HT or MHT values is -1, it will not be applied
// Lo:  -1                   100          200             -1    ...
// Hi:  -1                   -1           300            400    ...
//   (no cuts at all)     (>=100)   (>=200 && <300)  (>=-1 && <400)
  std::vector<double> cutLoMHTvals_, cutLoHTvals_;
  std::vector<double> cutHiMHTvals_, cutHiHTvals_;
  size nSearchRegions;
// String names for all the search regions
  std::vector<std::string> fullStageStrVec;

  bool debug_;

  edm::Service<TFileService> fs;

  TH2D *h2_NTotal;
// nSearchRegions of TH2D where nSearchRegions will be calculated after
// getting the cutXXMHTvals_ and cutXXHTvals_ inputs
  std::vector<TH2D*> h2_NPassedVec;
  std::vector<double> xAxisDivs_, yAxisDivs_;
  std::vector<int> xAxisBins_, yAxisBins_;

// [indexNumberOfPDFweight, indexPDFcomponent, indexCutFlow]
  std::vector<std::vector<TH2D*> > h2_PDFWgt_NTotalVec; std::vector<std::vector<std::vector<TH2D*> > > h2_PDFWgt_NPassedVec;
  std::vector<int> nCompPDFvec_; int nCompPDFsize_; // store size of each PDF weight
  std::vector<edm::InputTag> pdfwgts_;

  std::vector<double> makeDivBins(const std::vector<double> &divPts, const std::vector<int> &nDivs);

  bool fillTotalOnly_;

  void bookHistograms();

// To print out xpar and ypar in each root file
// Only print them out when the values change to save frequence of printout
  double prexpar, preypar;
  int nEvtPerPairPars;

  bool fastScan_;
 
  bool printINFO_;

  bool doPDFSys_;

  bool doISRweight_;
  edm::InputTag ISRweightSrc_;

  std::string inputFileNameStr;
};


RA2SignalScan::RA2SignalScan(const edm::ParameterSet& iConfig) {

  susyScanType_     = iConfig.getParameter<std::string> ("susyScanType");
  susyScanTopology_ = iConfig.getParameter<std::string> ("susyScanTopology");

  fillTotalOnly_ = iConfig.getParameter<bool>("fillTotalOnly");

  metSrc_ = iConfig.getParameter<edm::InputTag> ("metSrc");
  mhtSrc_ = iConfig.getParameter<edm::InputTag> ("mhtSrc");
  mht_forSgnfSrc_ = iConfig.getParameter<edm::InputTag> ("mht_forSgnfSrc");
  htSrc_ = iConfig.getParameter<edm::InputTag> ("htSrc");

  forVetoMuonSrc_ = iConfig.getParameter<edm::InputTag>("forVetoMuonSrc");
  forVetoElectronSrc_ = iConfig.getParameter<edm::InputTag>("forVetoElectronSrc");
  nVetoLeptons_ = iConfig.getParameter<std::vector<int> >("nVetoLeptons");

  vtxSrc_ = iConfig.getParameter<edm::InputTag>("vtxSrc");
  evtWeightInput_ = iConfig.getParameter<edm::InputTag>("evtWeightInput");
  nVtxPUcut_ = iConfig.getParameter<unsigned int>("nVtxPUcut");

  doSignalSC_ = iConfig.getParameter<int>("doSignalSC");
  hadTauSCweightInputTag_ = iConfig.getParameter<edm::InputTag>("hadTauSCweightInputTag");
  lostLepSCweightInputTag_ = iConfig.getParameter<edm::InputTag>("lostLepSCweightInputTag");

  fastScan_ = iConfig.getUntrackedParameter<bool>("fastScan", false);
  printINFO_ = iConfig.getUntrackedParameter<bool>("printINFO", false);

  doPDFSys_ = iConfig.getUntrackedParameter<bool>("doPDFSys", false);

  nCompPDFvec_ = iConfig.getParameter<std::vector<int> >("nCompPDFvec");
  pdfwgts_     = iConfig.getParameter<std::vector<edm::InputTag> >("pdfwgts");

  debug_ = iConfig.getParameter<bool>("debug");

  // if apply weight, then get the weight
  doISRweight_      = iConfig.getUntrackedParameter<bool> ("doISRweight", false);
  ISRweightSrc_     = iConfig.getParameter<edm::InputTag>("ISRweightSrc");

  if( (int)(doISRweight_) + (int)(doPDFSys_) + (int)(doSignalSC_!=0) >1 ){
     std::cout<<"ERROR ... cannot do more than 1 of the following  doISRweight_ : "<<doISRweight_<<"  doPDFSys_ : "<<doPDFSys_<<"  doSignalSC_ : "<<doSignalSC_<<std::endl;
     return;
  }

  nCompPDFsize_ = (int)nCompPDFvec_.size();
  if( pdfwgts_.size() != nCompPDFvec_.size() ){ std::cout<<"ERROR ... pdfwgts_.size : "<<pdfwgts_.size()<<"  NOT equal to  nCompPDFsize : "<<nCompPDFsize_<<std::endl; return; }
  std::cout<<"INFO ... nCompPDFsize : "<<nCompPDFsize_;
  for(int ip =0; ip < nCompPDFsize_; ip++){ std::cout<<"  "<<nCompPDFvec_[ip]<<"/"<<pdfwgts_[ip].instance(); } std::cout<<std::endl;

  cutLoMHTvals_ = iConfig.getParameter<std::vector<double> > ("cutLoMHTvals");
  cutLoHTvals_  = iConfig.getParameter<std::vector<double> > ("cutLoHTvals");
  cutHiMHTvals_ = iConfig.getParameter<std::vector<double> > ("cutHiMHTvals");
  cutHiHTvals_  = iConfig.getParameter<std::vector<double> > ("cutHiHTvals");

  if( cutLoMHTvals_.size() != cutLoHTvals_.size() || cutHiMHTvals_.size() != cutHiHTvals_.size() || cutLoMHTvals_.size() != cutHiMHTvals_.size() ){
     std::cout<<"ERROR ... mismatching between  cutLoMHTvals_.size : "<<cutLoMHTvals_.size()<<"  cutLoHTvals_.size : "<<cutLoHTvals_.size()<<std::endl;
     std::cout<<"                           OR  cutHiMHTvals_.size : "<<cutHiMHTvals_.size()<<"  cutHiHTvals_.size : "<<cutHiHTvals_.size()<<std::endl;
     return;
  }
  nSearchRegions = cutLoMHTvals_.size();
  std::cout<<"INFO ... nSearchRegions : "<<nSearchRegions<<std::endl;

  fullStageStrVec.clear(); 
  char namesStage[200];
  for(size is=0; is<nSearchRegions; is++){
     sprintf(namesStage, "%3.0fHT%3.0f_%3.0fMHT%3.0f", cutLoHTvals_[is], cutHiHTvals_[is], cutLoMHTvals_[is], cutHiMHTvals_[is]);
     TString strTmp(namesStage); strTmp.ReplaceAll(" -1", "inf"); strcpy(namesStage, strTmp.Data());
     if( debug_ ) printf("  %2dth stage : %s\n", is, namesStage);
     fullStageStrVec.push_back(namesStage);
  }
  if( debug_ ) std::cout<<std::endl;

  xAxisDivs_ = iConfig.getParameter<std::vector<double> > ("xAxisDivs");
  yAxisDivs_ = iConfig.getParameter<std::vector<double> > ("yAxisDivs");
  xAxisBins_ = iConfig.getParameter<std::vector<int> > ("xAxisBins");
  yAxisBins_ = iConfig.getParameter<std::vector<int> > ("yAxisBins");

  if( xAxisDivs_.size() -1 != xAxisBins_.size() || yAxisDivs_.size() -1 != yAxisBins_.size() ){
     std::cout<<"ERROR ... mismatching between  xAxisDivs_.size -1 : "<<xAxisDivs_.size()-1<<"  xAxisBins_.size : "<<xAxisBins_.size()<<std::endl;
     std::cout<<"                           OR  yAxisDivs_.size -1 : "<<yAxisDivs_.size()-1<<"  yAxisBins_.size : "<<yAxisBins_.size()<<std::endl;
     return;
  }

  prexpar = -1; preypar = -1; nEvtPerPairPars =0;
  if( debug_ ) std::cout<<"Initial values  prexpar : "<<prexpar<<"  preypar : "<<preypar<<std::endl;
}


RA2SignalScan::~RA2SignalScan() {
/*
   if( printINFO_ ){
      printf("INFO ... RA2SignalScan   lastxpar : %4.0f  lastypar : %4.0f  final nEvtPerPairPars : %d\n", lastxpar, lastypar, nEvtPerPairPars);
   }
*/
}

void RA2SignalScan::respondToOpenInputFile(edm::FileBlock const& fb){
   prexpar = -1; preypar = -1; nEvtPerPairPars =0;
   inputFileNameStr = fb.fileName();
}

void RA2SignalScan::respondToCloseInputFile(edm::FileBlock const& fb){
   if( printINFO_ ){
      printf("INFO ... RA2SignalScan   end  xpar : %4.0f  ypar : %4.0f  nEvtPerPairPars : %d  -->  %s\n\n", prexpar, preypar, nEvtPerPairPars, inputFileNameStr.c_str());
   }
}

void RA2SignalScan::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  loadEventInfo(iEvent, iSetup);
  if( debug_ ) std::cout<<"run : "<<run<<"  lumi : "<<ls<<"  event : "<<event<<std::endl; 

  if( !fillTotalOnly_ && !fastScan_ ){ 
// Event setup
     loadMETMHT(iEvent);
     loadHT(iEvent);
     loadLeptons(iEvent);

     if( doSignalSC_ == 1 || doSignalSC_ == 3 ){
        iEvent.getByLabel(hadTauSCweightInputTag_, hadTauSCweights_);
        if( hadTauSCweights_->size() != nSearchRegions ){
           std::cout<<"ERROR ... hadTauSCweights_->size : "<<hadTauSCweights_->size()<<"  NOT equal to nSearchRegions : "<<nSearchRegions<<std::endl;
           return;
        }
     }
     if( doSignalSC_ == 2 || doSignalSC_ == 3 ){
        iEvent.getByLabel(lostLepSCweightInputTag_, lostLepSCweights_);
     }
  }

  edm::Handle<LHEEventProduct> lheprod;
  iEvent.getByLabel("source", lheprod);

  std::vector<double> LHEpars;
  if( susyScanType_ == "mSUGRA" ){
     LHEpars = getLHEmSUGRApars( *lheprod );
  }else if( susyScanType_ == "SMS" ){
     LHEpars = getLHESMSpars( *lheprod );
  }else{
     std::cout<<"ERROR ... cannot recoganize susyScanType : "<<susyScanType_.c_str()<<std::endl;
     return;
  }

  double xpar = LHEpars[0], ypar = LHEpars[1];
  if( debug_ ) std::cout<<"xpar : "<<xpar<<"  ypar : "<<ypar<<std::endl;

  if( prexpar != xpar || preypar != ypar ){
     if( printINFO_ ){
        if( !(prexpar == -1 && preypar == -1) ){
           printf("INFO ... RA2SignalScan   end  xpar : %4.0f  ypar : %4.0f  nEvtPerPairPars : %d  -->  %s\n", prexpar, preypar, nEvtPerPairPars, inputFileNameStr.c_str());
           printf("INFO ... RA2SignalScan   bgn  xpar : %4.0f  ypar : %4.0f  nEvtPerPairPars : %d  -->  %s\n", xpar, ypar, 1, inputFileNameStr.c_str());
        }else{
           printf("\nINFO ... RA2SignalScan   bgn  xpar : %4.0f  ypar : %4.0f  nEvtPerPairPars : %d  -->  %s\n", xpar, ypar, 1, inputFileNameStr.c_str());
        }
     }
     prexpar = xpar; preypar = ypar; nEvtPerPairPars =1;
  }else{ nEvtPerPairPars ++; }

  if( !fastScan_ ){

     if( !fillTotalOnly_ ){
        int nVeto1stVal = nVetoLeptons_[0], nVeto2ndVal = nVetoLeptons_[1];
        
        int totLeptonsForVeto = (int) (nMuonsForVeto + nElectronsForVeto);
        if( (abs(nVeto2ndVal) ==1 && totLeptonsForVeto != nVeto1stVal) || (abs(nVeto2ndVal) ==0 && totLeptonsForVeto < nVeto1stVal) ) return;
        if( nVeto1stVal == 2 && abs(nVeto2ndVal) ==1 && multipliedCharge != nVeto2ndVal ) return;
     }

     double evtWeight = (*evtWeight_);
     if( doISRweight_ ){
        edm::Handle<double > ISRweight;
        iEvent.getByLabel(ISRweightSrc_, ISRweight);
        evtWeight *= (*ISRweight);
     }

     if( fillTotalOnly_ ){
        h2_NTotal->Fill(xpar, ypar, evtWeight);
     }else{
        double htVal = (*ht), mhtVal = (*mht)[0].pt();
     
        if( doSignalSC_ == 1 || doSignalSC_ == 3 ){
           for(size is=0; is<nSearchRegions; is++){
              double tmpWeight = evtWeight * (*hadTauSCweights_)[is];
              h2_NPassedVec[is]->Fill(xpar, ypar, tmpWeight);
           }
        }
 
        for(size is=0; is<nSearchRegions; is++){
           if(    (cutLoHTvals_[is]  == -1 || htVal  >=  cutLoHTvals_[is]) && (cutHiHTvals_[is]  == -1 || htVal   <  cutHiHTvals_[is])
               && (cutLoMHTvals_[is] == -1 || mhtVal >= cutLoMHTvals_[is]) && (cutHiMHTvals_[is] == -1 || mhtVal  < cutHiMHTvals_[is]) ){
          
              double tmpWeight = evtWeight; 
              if( doSignalSC_ ==2 || doSignalSC_ == 3 ){
                 tmpWeight *= (*lostLepSCweights_);
              }
// Already filled for had tau SC if only requiring filling of had tau SC, i.e., doSignalSC_ ==1, so don't fill it again here!
              if( doSignalSC_ != 1 ){
                 h2_NPassedVec[is]->Fill(xpar, ypar, tmpWeight);
              }
           }
        }
     }
     
     if( doPDFSys_ ){
        for(int ic=0; ic<nCompPDFsize_; ic++){
           int iComp = nCompPDFvec_[ic];

           edm::Handle<std::vector<double> > pdfwgt_tmp;
           iEvent.getByLabel( pdfwgts_[ic], pdfwgt_tmp );
           std::vector<double> wgtpdfvec = (*pdfwgt_tmp);
           int npdf = wgtpdfvec.size();
           if( npdf != iComp ){
              std::cout<<"ERROR ... "<<ic<<"th  npdf : "<<npdf<<" NOT equal to pre-input : "<<iComp<<std::endl;
              return;
           }

           for(int ip=0; ip<iComp; ip++){
              if( fillTotalOnly_ ){
                 h2_PDFWgt_NTotalVec[ic][ip]->Fill(xpar, ypar,  (*evtWeight_)*wgtpdfvec[ip]);
              }else{
                 double htVal = (*ht), mhtVal = (*mht)[0].pt();

                 for(size is=0; is<nSearchRegions; is++){
                    if(    (cutLoHTvals_[is]  == -1 || htVal  >=  cutLoHTvals_[is]) && (cutHiHTvals_[is]  == -1 || htVal   <  cutHiHTvals_[is])
                        && (cutLoMHTvals_[is] == -1 || mhtVal >= cutLoMHTvals_[is]) && (cutHiMHTvals_[is] == -1 || mhtVal  < cutHiMHTvals_[is]) ){

                       h2_PDFWgt_NPassedVec[ic][ip][is]->Fill(xpar, ypar, (*evtWeight_)*wgtpdfvec[ip]);
                    }
                 }
              }
           }
        }
     }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void RA2SignalScan::beginJob() {
  bookHistograms();
}

// ------------ method called once each job just after ending the event loop  ------------
void RA2SignalScan::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void RA2SignalScan::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void RA2SignalScan::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void RA2SignalScan::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void RA2SignalScan::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

void RA2SignalScan::bookHistograms() {

   std::vector<double> xDivBinsVec = makeDivBins(xAxisDivs_, xAxisBins_);
   int totxBins = (int)xDivBinsVec.size() - 1; 
   if( debug_ ){
      std::cout<<"totxBins : "<<totxBins<<std::endl;
      for(int ib=0; ib<totxBins+1; ib++){
         std::cout<<"  ib : "<<ib<<"  xDivBins : "<<xDivBinsVec[ib]<<std::endl;
      }
      std::cout<<std::endl;
   }

   std::vector<double> yDivBinsVec = makeDivBins(yAxisDivs_, yAxisBins_);
   int totyBins = (int)yDivBinsVec.size() - 1;
   if( debug_ ){
      std::cout<<"totyBins : "<<totyBins<<std::endl;
      for(int ib=0; ib<totyBins+1; ib++){
         std::cout<<"  ib : "<<ib<<"  yDivBins : "<<yDivBinsVec[ib]<<std::endl;
      }
      std::cout<<std::endl;
   }

   char hname[100], htit[100];  

   if( fillTotalOnly_ ){
      sprintf(hname, "NTotal");
      sprintf(htit,  "Total no. of events processed");
      h2_NTotal = fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]);
      h2_NTotal->Sumw2();
   }else{
      h2_NPassedVec.clear();
      for(size is=0; is<nSearchRegions; is++){
         sprintf(hname, "NPassed_%s", fullStageStrVec[is].c_str());
         sprintf(htit, "Passed : %s", fullStageStrVec[is].c_str());
     
         h2_NPassedVec.push_back( fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]) );
         h2_NPassedVec.back()->Sumw2();
      }
   }

   if( doPDFSys_ ){

      h2_PDFWgt_NTotalVec.clear(); h2_PDFWgt_NPassedVec.clear();

      if( fillTotalOnly_ ){ h2_PDFWgt_NTotalVec.resize(nCompPDFsize_); }else{ h2_PDFWgt_NPassedVec.resize(nCompPDFsize_); }

      for(int ic=0; ic<nCompPDFsize_; ic++){
         int iComp = nCompPDFvec_[ic];

         if( !fillTotalOnly_ ) h2_PDFWgt_NPassedVec[ic].resize(iComp);
         for(int ip =0; ip<iComp; ip++){
            if( fillTotalOnly_ ){
               sprintf(hname, "PDFWgt%d_NTotal_%d", ic+1, ip);
               sprintf(htit, "Total no. of events processed (pdfWgt%d %d)", ic+1, ip);
               h2_PDFWgt_NTotalVec[ic].push_back( fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]) );
               h2_PDFWgt_NTotalVec[ic].back()->Sumw2();
            }else{
               for(size is=0; is<nSearchRegions; is++){

                  sprintf(hname, "PDFWgt%d_NPassed_%s_%d", ic+1, fullStageStrVec[is].c_str(), ip);
                  sprintf(htit, "Passed : %s (pdfWgt%d %d)", fullStageStrVec[is].c_str(), ic+1, ip);

                  h2_PDFWgt_NPassedVec[ic][ip].push_back( fs->make<TH2D>(hname, htit, totxBins, &xDivBinsVec[0], totyBins, &yDivBinsVec[0]) );
                  h2_PDFWgt_NPassedVec[ic][ip].back()->Sumw2();
               }
            }
         }
      } 
   }
}

void RA2SignalScan::loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup){
      
// Determine if it's data
   if( !iEvent.isRealData() ) isData = false;
      
// Get run, event, lumi info
   run = iEvent.id().run();
   event = iEvent.id().event();
   ls = iEvent.luminosityBlock();

// Get vertices
   if( !fastScan_ ){ iEvent.getByLabel(vtxSrc_, vertices); vtxSize = vertices->size(); }
            
// Get event weight
   if( !fastScan_ ) iEvent.getByLabel(evtWeightInput_, evtWeight_);
      
}

std::vector<double> RA2SignalScan::getLHEmSUGRApars(const LHEEventProduct& lhep){

   typedef std::vector<std::string>::const_iterator comments_const_iterator;

   comments_const_iterator c_begin = lhep.comments_begin();
   comments_const_iterator c_end = lhep.comments_end();

   double mzero, mhalf, tanb, azero, mu=1.0;
   double signMu;

   std::vector<double> resVec;

   for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
      size_t found = (*cit).find("model");

      if( found != std::string::npos)   {
         size_t foundLength = (*cit).size();
         found = (*cit).find("=");
         std::string smaller = (*cit).substr(found+1,foundLength);
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
      
         std::istringstream iss(smaller);
         iss >> mzero;
         iss.clear();
      
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> mhalf;
         iss.clear();
      
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> tanb;
         iss.clear();
      
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> azero;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> signMu;
         iss.clear();
         mu *= signMu;
      }
   }

   if(debug_ ) std::cout<<" ==> mzero : "<<mzero<<"  mhalf : "<<mhalf<<"  tanb : "<<tanb<<"  azero : "<<azero<<"  mu : "<<mu<<std::endl;
   resVec.push_back(mzero); resVec.push_back(mhalf); resVec.push_back(tanb); resVec.push_back(azero); resVec.push_back(mu);
   return resVec;
}


std::vector<double> RA2SignalScan::getLHESMSpars(const LHEEventProduct& lhep){

   typedef std::vector<std::string>::const_iterator comments_const_iterator;

   comments_const_iterator c_begin = lhep.comments_begin();
   comments_const_iterator c_end = lhep.comments_end();

   double mGL = -1.0, mLSP = -1.0, xCHI = -1.0;

   std::vector<double> resVec;

   TString susyScanTopologyT(susyScanTopology_);

   for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
      size_t found = (*cit).find("model");

      //model T1_0.0_1025.0_525.0
      if( found != std::string::npos)   {
         size_t foundLength = (*cit).size();

         found = (*cit).find(susyScanTopology_);

         std::string smaller = (*cit).substr(found+1,foundLength);
         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
      
         std::istringstream iss(smaller);

// See string identifier in https://twiki.cern.ch/twiki/bin/view/CMS/SUSYSimplifiedModelsPAS
         if( susyScanTopologyT.Contains("T1") || susyScanTopologyT.Contains("T3") || ( susyScanTopologyT.Contains("T5") && !susyScanTopologyT.Contains("T5zzlnu") ) ) {
            iss >> xCHI; 
            iss.clear();
            found = smaller.find("_");
            smaller = smaller.substr(found+1,smaller.size());
            iss.str(smaller);
         }

         iss >> mGL;
         iss.clear();

         found = smaller.find("_");
         smaller = smaller.substr(found+1,smaller.size());
         iss.str(smaller);
         iss >> mLSP;
         iss.clear();
      }
   }

   if(debug_ ) std::cout<<" ==> gluino : "<<mGL<<"  LSP : "<<mLSP<<"  xFrac : "<<xCHI <<std::endl;
   resVec.push_back(mGL); resVec.push_back(mLSP); resVec.push_back(xCHI);
   return resVec;
}


void RA2SignalScan::loadMETMHT(const edm::Event& iEvent){
   iEvent.getByLabel(metSrc_, met);
   iEvent.getByLabel(mhtSrc_, mht);
   iEvent.getByLabel(mht_forSgnfSrc_, mht_forSgnf);
}

void RA2SignalScan::loadHT(const edm::Event& iEvent){
   iEvent.getByLabel(htSrc_, ht);
}

void RA2SignalScan::loadLeptons(const edm::Event& iEvent){
   iEvent.getByLabel(forVetoMuonSrc_, forVetoMuons); nMuonsForVeto = forVetoMuons->size();
   iEvent.getByLabel(forVetoElectronSrc_, forVetoElectrons); nElectronsForVeto = forVetoElectrons->size();

   multipliedCharge = 1;

   for( size im=0; im<nMuonsForVeto; im++){
      if( im ==0 ){ muon1 = (*forVetoMuons)[im]; muon2 = (*forVetoMuons)[im]; multipliedCharge *= muon1.charge(); }
      if( im ==1 ){ muon2 = (*forVetoMuons)[im]; multipliedCharge *= muon2.charge(); }
   }

   muon1Charge = muon1.charge(); muon2Charge = muon2.charge();

   for( size ie=0; ie<nElectronsForVeto; ie++){
      if( ie ==0 ){ ele1 = (*forVetoElectrons)[ie]; ele2 = (*forVetoElectrons)[ie]; multipliedCharge *= ele1.charge(); }
      if( ie ==1 ){ ele2 = (*forVetoElectrons)[ie]; multipliedCharge *= ele2.charge(); }
   }

   ele1Charge = ele1.charge(); ele2Charge = ele2.charge();
}

std::vector<double> RA2SignalScan::makeDivBins(const std::vector<double> &divPts, const std::vector<int> &nDivs){

   std::vector<double> divBins;

   const int nRangesPt = (int)nDivs.size();

   int totBinsPt = 0;
   for(int ib=0; ib<nRangesPt; ib++){ totBinsPt += nDivs[ib]; }
   divBins.clear(); divBins.resize(totBinsPt+1);

   int reBinIdxPt = -1;
   for(int ib=0; ib<nRangesPt; ib++){
      for(int id=0; id<nDivs[ib]; id++){
         double perDiv = (divPts[ib+1]-divPts[ib])/nDivs[ib];
         double divVal = divPts[ib] + perDiv*id;
         reBinIdxPt ++;
         divBins[reBinIdxPt] = divVal;
         if( ib==nRangesPt-1 && id == nDivs[ib] -1 ) divBins[totBinsPt] = divPts[ib+1];
      }
   }

   return divBins;

}

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE(RA2SignalScan);
