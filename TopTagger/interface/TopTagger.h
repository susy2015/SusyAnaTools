#ifndef TOPTAGGER_H
#define TOPTAGGER_H 

#include <memory>

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

#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#include "combination.h"
#include "indexSort.h"

#include "TTree.h"
#include "TLorentzVector.h"

using namespace stdindexSort;
using namespace stdcomb;
using namespace std;

typedef unsigned int size;
typedef std::vector<int>::iterator vii;

const std::string defaultOrderingOptArr[] = {"mass", "mass"};
const int defaultMaxIndexForOrderingArr[] = { 1,         1 };

class TopTagger : public edm::EDFilter {

  public:

    explicit TopTagger(const edm::ParameterSet & iConfig);
    ~TopTagger();

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

// doExtraCuts: extra cuts other than the top tagger related, e.g., dphi cuts, HT cut and so on.
    const bool doExtraCuts_;

    const double mTbcut_, mTtcut_, MT2cut_, mTWcut_;
// doMTMT2cuts: numbers are counted. But if enabled, some plots will be filled after all the cuts.
    const bool doMTMT2cuts_;

// PDG values (in GeV)
    const double mW_, mTop_;

    const double mWoverTop_;

// const double lowRatioWoverTop = 0.85, highRatioWoverTop = 1.15;
    const double lowRatioWoverTop_, highRatioWoverTop_;
    const double lowArcTanm13overm12_, highArcTanm13overm12_, lowm23overm123_;

    const double Rmin_, Rmax_;

    const double defaultJetCone_;
    const double simuCAdeltaR_;
// Eta ranges from 0 to 5, phi ranges from 0 to 3.14.
// sqrt(5./2.*5./2. + 3.14/2. * 3.14/2.) ~ 2.95
// const double simuCALargerdeltaR = 3.0;
    const double simuCALargerdeltaR_; // -1 means no deltaR requirement

    const double lowTopCut_, highTopCut_;
    const double lowWCut_, highWCut_;

// Choose CSVM point for now
// --> A good top fat jet might indicate a b-jet already,
// so requiring another tight b-jet is NOT good. (TODO: to be studied)
//    const double CSVL = 0.244, CSVM = 0.679, CSVT = 0.898;
    const double CSVS_;

    const int nSubJetsDiv_;

    const int nJetsSel_;

    const double maxEtaForbJets_;

// mass   : mass ordering --> pick the one with mass closest to the norminal mass
// pt     : pt ordering --> pick the one with larger pt (for the first two fat jets)
// hybrid : pt ordering + mass ordering --> if both of the first two fat jets
//          satisfying criteria, pick the one closer to the norminal mass
    const std::vector<std::string> defaultOrderingOptVec;
//                                                     best   remaining
    const std::vector<std::string>  orderingOptArr_; // (mass, mass) is the best?
    const std::vector<int> defaultMaxIndexForOrderingVec;
    const std::vector<int> maxIndexForOrderingArr_;

    const edm::InputTag metSrc_, jetSrc_;
    const std::string bTagKeyString_;

    edm::Handle<edm::View<reco::Jet> > jets;
    edm::Handle<std::vector<pat::Jet> > patjets;
    size nJets;
    bool isPatJet;
    virtual void loadRecoJets(const edm::Event& iEvent);

    edm::Handle<edm::View<reco::MET> > metHandle;
    double met, metphi;
    virtual void loadMETMHT(const edm::Event& iEvent);

    StringCutObjectSelector<reco::Jet,true> pfJetCutForJetCounting_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    StringCutObjectSelector<reco::Jet,true> pfJetCutForJetCombining_; // lazy parsing, to allow cutting on variables not in reco::Candidate class
    const edm::InputTag evtWeightInput_;
    edm::Handle<double> evtWeight_;

    unsigned int run, event, lumi; bool isData;
    const edm::InputTag vtxSrc_;
    unsigned int vtxSize;
    edm::Handle<edm::View<reco::Vertex> > vertices;
    void loadEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

    const bool debug_;
    const bool taggingMode_;
 
    const bool dobVetoCS_;

    int pickJetsForCombining(std::vector<TLorentzVector>& oriJetsVec, std::vector<double> &recoJetsBtagCSVS);
//    int pickJetsForCombining(std::vector<TLorentzVector>& oriJetsVec, std::vector<double> &recoJetsBtagCSVS, std::vector<std::vector<TLorentzVector> > & oriJetsConstituentsVec, std::vector<std::vector<int> > & oriJetsConstituentsChargeVec, std::vector<std::vector<double> > & oriJetsConstituentsDzVec, std::vector<std::vector<double> > & oriJetsConstituentsD0Vec);
    int countNJets();

// The counters
    double cntPassnJetsCut, cntTaggedTopEvents, cntTaggedTopEventsWithinMassCuts;
    double cntTaggedAllCutsPlusCSVS;
    double cntPassingMTbestTopJetCut, cntPassingMTclosebJetCut, cntPassingMTbestWJetCut;
    double cntPassingMT2Cut, cntPassingMT2andMTCut;
    double cntTaggedbestFatJetPlusCSVS;

    topTagger::type3TopTagger *type3TopTaggerPtr;
};

#endif
