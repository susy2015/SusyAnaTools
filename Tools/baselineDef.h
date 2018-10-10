#ifndef ANABASELINE_BASELINEDEF_H
#define ANABASELINE_BASELINEDEF_H

#include "NTupleReader.h"
#include "customize.h"
#include "EventListFilter.h"

#include "Math/VectorUtil.h"

#include <memory>
#include <iostream>

class TFile;
class TF1;

enum AK8Flag : unsigned
{
  NoTag,
  TopTag,
  WinTopTag,
  WAloneTag,
  NoTagLooseb,
  NoTagMediumb,
  NoTagNob,
};

class BaselineVessel
{
private:
    std::string spec;
    bool isfastsim;
    std::string firstSpec;
    bool printOnce;

    NTupleReader *tr;
    TFile *WMassCorFile;
    TF1   *puppisd_corrGEN;
    TF1   *puppisd_corrRECO_cen;
    TF1   *puppisd_corrRECO_for;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TopTagger ~~~~~
    std::shared_ptr<TopTagger> ttPtr;

    //  container
    TLorentzVector metLVec; 
    std::vector<TLorentzVector> *jetsLVec_forTagger;
    std::vector<float> *recoJetsBtag_forTagger;
    std::vector<float> *qgLikelihood_forTagger;
    std::vector<TLorentzVector> *vTops;
    std::map<int, std::vector<TLorentzVector> > *mTopJets;
    std::vector<unsigned> * vAK8Flag;

    std::vector<TLorentzVector> GetAK4NoSubjet(Constituent &ak8, 
        std::vector<TLorentzVector> &ak4jets) const;
public:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Member ~~~~~
    int  bToFake;
    bool debug;
    bool incZEROtop;
    bool UseLepCleanJet;

    std::string jetVecLabel;
    std::string CSVVecLabel;
    std::string METLabel;
    std::string METPhiLabel;
    std::string jetVecLabelAK8;
    std::string muonsFlagIDLabel;
    std::string elesFlagIDLabel;
    std::string qgLikehoodLabel;
    std::string toptaggerCfgFile;
    bool doIsoTrksVeto;
    bool doMuonVeto;
    bool doEleVeto;
    bool doMET;
    bool dodPhis;
    bool passBaseline;
    bool passBaselineNoTagMT2;
    bool passBaselineNoTag;
    bool passBaselineNoLepVeto;


    BaselineVessel(NTupleReader &tr_, const std::string specialization = "", const std::string filterString = "");
    ~BaselineVessel();

    void PassBaseline();
    bool passNoiseEventFilterFunc();
    bool passQCDHighMETFilterFunc();
    bool passFastsimEventFilterFunc();
    bool PredefineSpec();
    bool UseLepCleanJets();
    bool OpenWMassCorrFile();

    void operator()(NTupleReader& tr);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TopTagger ~~~~~
    bool SetupTopTagger(std::string CfgFile_ = "TopTagger.cfg");
    bool PassTopTagger();
    bool GetMHT() const;
    bool GetLeptons() const;
    bool GetRecoZ( const int zMassMin, const int zMassMax) const;
    bool GetRecoZ(const std::string leptype, const std::string lepchg, std::vector<TLorentzVector>* recoZVec,
        std::map<unsigned int, std::pair<unsigned int, unsigned int> > *TypeZLepIdx, 
        const int zMassMin, const int zMassMax) const;
    void prepareTopTagger();
    std::shared_ptr<TopTagger> GetTopTaggerPtr() const {return ttPtr;};
    int GetnTops() const;
    bool GetTopCombs() const;
    float CalcMT2() const;
    float coreMT2calc(const TLorentzVector & fatJet1LVec, const TLorentzVector & fatJet2LVec) const;
    bool FlagAK8Jets();
    AK8Flag FlagAK8FromCSV(Constituent &ak8) const;
    AK8Flag FlagAK8FromTagger(Constituent &ak8 );
    bool GetISRJet() const;
    bool GetWAlone() const;
    bool GetSoftbJets();
};

inline void passBaselineFunc(NTupleReader &tr, std::string filterstring)
{
  BaselineVessel blv(tr, "", filterstring);
  blv.PassBaseline();
  blv.GetMHT();
}

namespace stopFunctions
{
    class CleanJets
    {
    public:        
        void operator()(NTupleReader& tr) {internalCleanJets(tr);}

        void setMuonIso(const std::string muIsoFlag);
        void setElecIso(const std::string elecIsoFlag);
        void setJetCollection(std::string jetVecLabel);
        void setBTagCollection(std::string bTagLabel);
        void setMuonsFlagID(std::string muonsFlagIDLabel);
        void setElesFlagID(std::string elesFlagIDLabel);
        void setEnergyFractionCollections(std::string chargedEMfrac, std::string neutralEMfrac, std::string chargedHadfrac);
        void setForceDr(bool forceDr);
        void setDisable(bool disable);
        void setRemove(bool remove);
        void setElecPtThresh(float minPt);
        void setMuonPtThresh(float minPt);
        void setDisableElec(bool disable);
        void setDisableMuon(bool disable);
        //This option is used to clean up to 1 jet in the minDr cone around the muon if the jet is lower pt than the muon
        //It is designed only for use with the z->inv background to remove muon related radiation from the event
        void setJecScaleRawToFull(std::string jecScaleRawToFullLabel);
        void setPhotoCleanThresh(float photoCleanThresh);

        //NOTE!!! Must add Hadron and EM fraction vectors here

        CleanJets()
        {
            setMuonIso("mini");
            setElecIso("mini");
            setJetCollection("jetsLVec");
            setBTagCollection("recoJetsCSVv2");
//            setBTagCollection("recoJetsBtag_0");
            setMuonsFlagID("muonsFlagMedium");
            setElesFlagID("elesFlagVeto");
            setEnergyFractionCollections("recoJetschargedHadronEnergyFraction", "recoJetsneutralEmEnergyFraction", "recoJetschargedEmEnergyFraction");    
            setForceDr(false);
            setRemove(false);
            setDisable(false);
            setElecPtThresh(0.0);
            setMuonPtThresh(0.0);
            setPhotoCleanThresh(-999.9);
            setJecScaleRawToFull("recoJetsJecScaleRawToFull");
        }
        
    private:
        std::string muIsoStr_, elecIsoStr_, jetVecLabel_, bTagLabel_, chargedEMFracLabel_, neutralEMFracLabel_, chargedHadFracLabel_;
        std::string muonsFlagIDLabel_, elesFlagIDLabel_;
        std::string recoJetsJecScaleRawToFullLabel_;
        AnaConsts::IsoAccRec muIsoReq_;
        AnaConsts::ElecIsoAccRec elecIsoReq_;
        float elecPtThresh_;
        float muonPtThresh_;
        float photoCleanThresh_;
        bool remove_;
        bool disableMuon_, disableElec_;
        bool forceDr_;

        int cleanLeptonFromJet(const TLorentzVector& lep, const int& lepMatchedJetIdx, const std::vector<TLorentzVector>& jetsLVec, const std::vector<float>& jecScaleRawToFull, std::vector<bool>& keepJet, const std::vector<float>& neutralEmEnergyFrac, std::vector<TLorentzVector>* cleanJetVec, const float& jldRMax, const float photoCleanThresh = -999.9);
        void internalCleanJets(NTupleReader& tr);
    };

    inline void cleanJets(NTupleReader& tr)
    {
      CleanJets cjh;
      cjh(tr);
    }
}

#endif
