#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "RecoMET/METAlgorithms/interface/significanceAlgo.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"


class signifMHTProducer : public edm::EDProducer {

  public:

    explicit signifMHTProducer(const edm::ParameterSet & iConfig);
    ~signifMHTProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    edm::InputTag theJetLabel_;
    double minJetPt_;
    double maxJetEta_;

    JetResolution *ptResol_;
    JetResolution *phiResol_;
};



signifMHTProducer::signifMHTProducer(const edm::ParameterSet & iConfig) {
  theJetLabel_ = iConfig.getParameter<edm::InputTag>("JetCollection");
  minJetPt_    = iConfig.getParameter<double>("MinJetPt");
  maxJetEta_   = iConfig.getParameter<double>("MaxJetEta");

  //initialize jet pt and phi resolution objects
  using namespace std;
  string alg  = iConfig.getParameter<std::string>("resolutionsAlgo");     
  string era   = iConfig.getParameter<std::string>("resolutionsEra");     

  string path = "CondFormats/JetMETObjects/data";
  string ptFileName  = path + "/" + era + "_PtResolution_" +alg+".txt";
  string phiFileName = path + "/" + era + "_PhiResolution_"+alg+".txt";

  edm::FileInPath fpt(ptFileName);
  edm::FileInPath fphi(phiFileName);
    
  ptResol_ = new JetResolution(fpt.fullPath().c_str(),false);
  phiResol_ = new JetResolution(fphi.fullPath().c_str(),false);

  produces<std::vector<reco::MET> >("");
}


signifMHTProducer::~signifMHTProducer() {
}


void signifMHTProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // read in the objects
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(theJetLabel_, jets);

  std::vector<metsig::SigInputObj> vobj;

  // calculate MHT
  std::auto_ptr<std::vector<reco::MET> > mhtp(new std::vector<reco::MET>());
  reco::MET::LorentzVector mht(0,0,0,0);
  for (edm::View<reco::Jet>::const_iterator it = jets->begin(); it != jets->end(); ++it) {
    if (it->pt() > minJetPt_ && fabs(it->eta()) < maxJetEta_) {

      mht -= it->p4();

      double jpt  = it->pt();
      double jphi = it->phi();
      double jeta = it->eta();

      TF1* fPtEta  = ptResol_->parameterEta("sigma",jeta);
      TF1* fPhiEta = phiResol_->parameterEta("sigma",jeta);
      double jdpt  = jpt*fPtEta->Eval(jpt);
      double jdphi = jpt*fPhiEta->Eval(jpt);
      delete fPtEta;
      delete fPhiEta;

      std::string inputtype = "jet";
      metsig::SigInputObj obj(inputtype, jpt, jphi, jdpt,jdphi);

      vobj.push_back(obj);
    }
  }

  metsig::significanceAlgo signAlgo_;
  signAlgo_.addObjects(vobj);

  reco::MET met(mht, reco::MET::Point());
  met.setSignificanceMatrix(signAlgo_.getSignifMatrix());

  mhtp->push_back(met);

  iEvent.put(mhtp);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(signifMHTProducer);
