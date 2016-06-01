#include <iostream>
#include <cmath>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/GetterOfProducts.h"
#include "FWCore/Framework/interface/ProcessMatch.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "SusyAnaTools/SkimsAUX/plugins/common.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "TVector2.h"

//
// class declaration
//
class PDFWeightProducer : public edm::EDProducer {
public:
   explicit PDFWeightProducer(const edm::ParameterSet&);
   ~PDFWeightProducer();
      
   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      
private:
   generatorSource= cms.InputTag("generator")
   virtual void beginJob() ;
   virtual void produce(edm::Event&, const edm::EventSetup&);
   virtual void endJob() ;
      
   virtual void beginRun(edm::Run&, edm::EventSetup const&);
   virtual void endRun(edm::Run&, edm::EventSetup const&);
   virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
   virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
   edm::GetterOfProducts<LHEEventProduct> getterOfProducts_;
   edm::EDGetTokenT<GenEventInfoProduct> GenTok_;
   edm::InputTag GenSrc_; 
// ----------member data ---------------------------
};
namespace LHAPDF {
   void initPDFSet(int nset, const std::string& filename, int member=0);
   int numberPDF(int nset);
   void usePDFMember(int nset, int member);
   double xfx(int nset, double x, double Q, int fl);
   double getXmin(int nset, int member);
   double getXmax(int nset, int member);
   double getQ2min(int nset, int member);
   double getQ2max(int nset, int member);
   void extrapolate(bool extrapolate=true);
}

PDFWeightProducer::PDFWeightProducer(const edm::ParameterSet& iConfig) : getterOfProducts_(edm::ProcessMatch("*"), this)
{
   callWhenNewProductsRegistered(getterOfProducts_);
   GenSrc_== iConfig.getParameter<GenEventInfoProduct>("generatorSource");
   GenTok_ = consumes<GenEventInfoProduct>(GenSrc_);
     
   //From LHAPDF Grid
   produces<std::vector<double> >("PDFweights");
   produces<std::vector<int> >("PDFids");
      
   //_MiniAOD corresponds to Weights and scales taken from
   //MiniAOD not from LHAPDF Grid.
   produces<std::vector<double> >("ScaleWeightsMiniAOD");
   produces<std::vector<double> >("PDFweightsMiniAOD");
   produces<std::vector<int> >("PDFidsMiniAOD");

   //For x1 and x2
   produces<double>("x1");
   produces<double>("x2");      
   produces<int>("id2");
   produces<int>("id1");
   produces<double>("q");
}

PDFWeightProducer::~PDFWeightProducer()
{
      
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
      
}



void PDFWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
      
   //For MiniAOD
   std::vector<edm::Handle<LHEEventProduct> > handles;
   getterOfProducts_.fillHandles(iEvent, handles);
      
   //For LHAPDF Grid
      
   edm::Handle<GenEventInfoProduct> GenInfo;
   iEvent.getByTokenGenTok_,GenInfo);
      
   //***************************
   // From LHAPDF Grid
   //***************************
   std::vector<double> pdfweights;
   std::vector<int> pdfids;
   pdfweights.clear();
   pdfids.clear();
      
   // pdf inputs
   double q = GenInfo->pdf()->scalePDF;
   int id1 = GenInfo->pdf()->id.first;
   int id2 = GenInfo->pdf()->id.second;
   double x1 = GenInfo->pdf()->x.first;
   double x2 = GenInfo->pdf()->x.second;

   std::auto_ptr<int> id1Ptr_(new int);                                                                                         
   std::auto_ptr<int> id2Ptr_(new int);      
   *id1Ptr_ = id1;                                                                                                                   
   *id2Ptr_ = id2;    
   iEvent.put(id1Ptr_, "id1");
   iEvent.put(id2Ptr_, "id2");    

   // weirdo LHA conventions, gluons are 0
   if (id1 == 21) id1 = 0;
   if (id2 == 21) id2 = 0;
      
   int N_pdfsets = 3; // hard-coded for now...
   int pdfset_internalIdNumber = -99;
   int pdfid = -99;
   for (int i = 1; i <= N_pdfsets; i++){
      // std::cout << "inputs for PDF weights = " << q << "," << id1 << "," << id2 << "," << x1 << "," << x2 << std::endl;
      // std::cout << "...there are this many members = " << LHAPDF::numberPDF(i) << std::endl;
      // get central value
      LHAPDF::usePDFMember(i,0);
      double xpdf1 = LHAPDF::xfx(i, x1, q, id1);
      double xpdf2 = LHAPDF::xfx(i, x2, q, id2);
      double w0 = xpdf1 * xpdf2;
            
      pdfset_internalIdNumber = i*1000;
      double lhaweight= 0.0;            
      int N_eigensets = LHAPDF::numberPDF(i);
      for (int j = 0; j < N_eigensets; j++){
                  
         LHAPDF::usePDFMember(i,j);
         double xpdf1_new = LHAPDF::xfx(i, x1, q, id1);
         double xpdf2_new = LHAPDF::xfx(i, x2, q, id2);
         double w_new = xpdf1_new * xpdf2_new;
         lhaweight = w_new / w0;

         //pdf_weights.push_back(weight);
         pdfid = pdfset_internalIdNumber + j;
                  
         // std::cout << "lhaweight " << pdfid << " = " << lhaweight << "..." << w_new << "," << w0 << std::endl;
                  
         pdfweights.push_back(lhaweight);
         pdfids.push_back(pdfid);
      }
   } // End from LHAPDF Grid
      
   //**************************************
   // From MiniAOD
   //**************************************
   std::vector<double> scaleweightsMiniAOD;
   std::vector<double> pdfweightsMiniAOD;
   std::vector<int> pdfidsMIniAOD;
      
   if(!handles.empty()){
      edm::Handle<LHEEventProduct> LheInfo = handles[0];
         
      std::vector< gen::WeightsInfo > lheweights = LheInfo->weights();
      double lhaweight = 0.0;
      if(!lheweights.empty()){
         // these numbers are hard-coded by the LHEEventInfo
         //renormalization/factorization scale weights
         for (unsigned int i = 0; i < 9; i++){
            // std::cout << "lheweights " << i << " = " << lheweights[i].id << ", " << lheweights[i].wgt/lheweights[9].wgt << std::endl;
            scaleweightsMiniAOD.push_back(lheweights[i].wgt/lheweights[0].wgt);
         }
         //pdf weights
         for (unsigned int i = 9; i < 110; i++){
         //std::cout << "lheweights " << i << " = " << lheweights[i].id << ", " << lheweights[i].wgt/lheweights[9].wgt << std::endl;
            lhaweight = lheweights[i].wgt/lheweights[9].wgt;
	    pdfweightsMiniAOD.push_back(lhaweight);
            pdfidsMIniAOD.push_back(i-9);
        }
     }
  } //End from MiniAOD
      
  //For x1 and x2
  std::auto_ptr<double> x1Ptr_(new double);
  std::auto_ptr<double> x2Ptr_(new double);
  std::auto_ptr<double> qPtr_(new double);

  *x1Ptr_ = x1;
  *x2Ptr_ = x2;
  *qPtr_ = q;

  iEvent.put(x1Ptr_, "x1");
  iEvent.put(x2Ptr_, "x2");
  iEvent.put(qPtr_, "q");

  // From LHAPDF Grid
  std::auto_ptr<std::vector<double> > pdfweights_(new std::vector<double>(pdfweights));
  iEvent.put(pdfweights_,"PDFweights");
      
  std::auto_ptr<std::vector<int> > pdfids_(new std::vector<int>(pdfids));
  iEvent.put(pdfids_,"PDFids");
      
  // From MiniAOD
  std::auto_ptr<std::vector<double> > scaleweightsMiniAOD_(new std::vector<double>(scaleweightsMiniAOD));
  iEvent.put(scaleweightsMiniAOD_,"ScaleWeightsMiniAOD");
      
  std::auto_ptr<std::vector<double> > pdfweightsMiniAOD_(new std::vector<double>(pdfweightsMiniAOD));
  iEvent.put(pdfweightsMiniAOD_,"PDFweightsMiniAOD");
      
  std::auto_ptr<std::vector<int> > pdfidsMiniAOD_(new std::vector<int>(pdfidsMIniAOD));
  iEvent.put(pdfidsMiniAOD_,"PDFidsMiniAOD");
}

// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just before starting event loop  ------------
void
PDFWeightProducer::beginJob()
{
   LHAPDF::initPDFSet(1, "NNPDF30_nlo_as_0118.LHgrid");
   LHAPDF::initPDFSet(2, "CT10nlo.LHgrid");
   LHAPDF::initPDFSet(3, "MMHT2014nlo68cl.LHgrid");
}

      // ------------ method called once each job just after ending the event loop  ------------
void
PDFWeightProducer::endJob() {
}

      // ------------ method called when starting to processes a run  ------------
void
PDFWeightProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

      // ------------ method called when ending the processing of a run  ------------
void
PDFWeightProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

      // ------------ method called when starting to processes a luminosity block  ------------
void
PDFWeightProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

      // ------------ method called when ending the processing of a luminosity block  ------------
void
PDFWeightProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

      // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PDFWeightProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      //The following says we do not know what parameters are allowed so do no validation
      // Please change this to state exactly what you do use, even if it is no parameters
   edm::ParameterSetDescription desc;
   desc.setUnknown();
   descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PDFWeightProducer);
