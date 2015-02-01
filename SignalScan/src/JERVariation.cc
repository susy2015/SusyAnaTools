#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/View.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/Jet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include<algorithm>
#include<vector>

#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "TDirectory.h"

class JERVariation : public edm::EDProducer {

  public:
  explicit JERVariation(const edm::ParameterSet & iConfig);
  ~JERVariation();
  
  virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  static bool sort_jets(pat::Jet j1, pat::Jet j2);

  typedef math::XYZTLorentzVector LorentzVector;

  private:
  edm::InputTag   jetSrc_;
  std::string     DirResponseName_;
  edm::FileInPath responseHistFile_;
  
  TH1F *h_Response[23][6];
  double PtBins[24], EtaBins[7];

  double jetMaxEtaCut_, jetMinPtCut_;

};

JERVariation::JERVariation(const edm::ParameterSet & iConfig) {
  jetSrc_           = iConfig.getParameter<edm::InputTag>("JetSource");
  DirResponseName_  = iConfig.getParameter<std::string>("DirResponseName");
  responseHistFile_ = iConfig.getParameter<edm::FileInPath>("ResponseHistFile");
  jetMaxEtaCut_     = iConfig.getParameter<double>("jetMaxEtaCut");
  jetMinPtCut_     = iConfig.getParameter<double>("jetMinPtCut");

  produces<std::vector<pat::Jet> >("");

  double tempPt[24] = {00   ,16   ,30  ,43   ,55  ,
		       70   ,85   ,100 ,115  ,130 ,
		       150  ,170  ,190 ,220  ,250 ,
		       300  ,350  ,400 ,500  ,700 ,
		       1000 ,1300 ,1700, 5000.0 };
  for(int i=0; i<24; i++) PtBins[i] = tempPt[i];

  double tempEta[7] = {0.0, 0.5, 1.1, 1.7, 
		       2.3, 3.2, 5.0};
  for(int i=0; i<7; i++) EtaBins[i] = tempEta[i];

  TFile *fResponse = new TFile(responseHistFile_.fullPath().c_str());
  TDirectory *dir = (TDirectory*) fResponse->FindObjectAny(DirResponseName_.c_str());

  TH1F *hh = (TH1F*) dir->FindObjectAny("SigmaPtHist_Eta0");
  for(int ibin=1; ibin<=hh->GetNbinsX(); ibin++) {
    std::cout<< ibin << " "<< hh->GetBinWidth(ibin) << " " << hh->GetBinLowEdge(ibin) 
	     << " " << hh->GetBinLowEdge(ibin)+hh->GetBinWidth(ibin)
	     << std::endl;
  }
  
  for(int ieta=0; ieta<6; ieta++) {
    for(int ipt=0; ipt<23; ipt++) {
      char hname[200];
      sprintf(hname,"SmearedAndScaledResolution_Pt%i_Eta%i", ipt, ieta);
      h_Response[ipt][ieta] = (TH1F*) dir->FindObjectAny(hname);
    }
  }

}

JERVariation::~JERVariation() {
}

void JERVariation::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  
  using namespace reco;

  edm::Handle<edm::View<reco::GenJet> > jets;
  iEvent.getByLabel("ak5GenJets", jets);
  
  std::auto_ptr<std::vector<pat::Jet> > prod(new std::vector<pat::Jet>());

  const int NEtaBins = 6;
  const int NPtBins  = 23;

  //======= vary JER ==========
  
  for (edm::View<reco::GenJet>::const_iterator j = jets->begin(); j != jets->end(); ++j) {

    pat::Jet newJet = *j;
    
    double eta = j->eta();
    double pt  = j->pt();
    
    int iJetEtaBin=-1, iJetPtBin=-1;
    for(int ieta=0; ieta<NEtaBins; ieta++) {
      if(std::abs(eta)>=EtaBins[ieta] && std::abs(eta)<EtaBins[ieta+1] ) 
	iJetEtaBin = ieta;
    }
    for(int ipt=0;  ipt<NPtBins;   ipt++) {
      if( pt>=PtBins[ipt] && pt<PtBins[ipt+1] )
	iJetPtBin = ipt;
    }
    if( iJetPtBin<0 || iJetEtaBin<0 ) continue;

    double scale = 1.0;
    if( h_Response[iJetPtBin][iJetEtaBin]->GetEntries() > 0) 
      scale = h_Response[iJetPtBin][iJetEtaBin]->GetRandom();

    LorentzVector p4( j->px(), j->py(), j->pz(), j->energy() );    
    newJet.setP4( scale*p4 );

    if( (jetMaxEtaCut_ != -1 && fabs(newJet.eta()) > jetMaxEtaCut_) || (jetMinPtCut_ != -1 && newJet.pt() < jetMinPtCut_) ) continue;
    
    prod->push_back( newJet );
  }

  std::sort( (*prod).begin(), (*prod).end(),sort_jets );

  iEvent.put(prod);
}

bool JERVariation::sort_jets(pat::Jet j1, pat::Jet j2) {
  return j1.pt() > j2.pt();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(JERVariation);
