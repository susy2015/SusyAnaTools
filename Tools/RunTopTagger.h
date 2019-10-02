// RunTopTagger.h
// Caleb J. Smith, Nathaniel J. Pastika
// February 15, 2019

// run top tagger using reolved top candidates

#ifndef RUNTOPTAGGER_H
#define RUNTOPTAGGER_H

#include "NTupleReader.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <cstdio>

#include "TopTagger/TopTagger/interface/TopTagger.h"
#include "TopTagger/TopTagger/interface/TopTaggerResults.h"
#include "TopTagger/TopTagger/interface/TopTaggerUtilities.h"
#include "TopTagger/CfgParser/include/TTException.h"


class RunTopTagger {

private:
    std::shared_ptr<TopTagger> tt_; // std::unique_ptr gives a compile error
    std::string taggerCfg_;
    std::string suffix_;
    bool doLeptonCleaning_;
    bool doPhotonCleaning_;

    void runTopTagger(NTupleReader& tr) 
    {
        //get necessary tagger input variables 

        //AK4 jets
        const auto& Jet_LV          = tr.getVec_LVFromNano<float>("Jet");
        const auto& Jet_btagDeepB   = tr.getVec<float>("Jet_btagDeepB");
        std::vector<bool> Jet_matchesPhoton;
        std::vector<bool> Jet_matchesElectron;
        std::vector<bool> Jet_matchesMuon;
        if (doPhotonCleaning_)
        {
            Jet_matchesPhoton   = tr.getVec<bool>("Jet_matchesPhoton");
        }
        if (doLeptonCleaning_)
        {
            Jet_matchesElectron = tr.getVec<bool>("Jet_matchesElectron");
            Jet_matchesMuon     = tr.getVec<bool>("Jet_matchesMuon");
        }

        //AK8 jets
        const auto& FatJet_LV              = tr.getVec_LVFromNano<float>("FatJet");
        const auto& FatJet_deepAK8_t       = tr.getVec<float>("FatJet_deepTag_TvsQCD");
        const auto& FatJet_deepAK8_w       = tr.getVec<float>("FatJet_deepTag_WvsQCD");
        const auto& FatJet_msoftdrop       = tr.getVec<float>("FatJet_msoftdrop");
        const auto& FatJet_subJetIdx1      = tr.getVec<int>("FatJet_subJetIdx1");
        const auto& FatJet_subJetIdx2      = tr.getVec<int>("FatJet_subJetIdx2");
        const auto& FatJet_Stop0l          = tr.getVec<int>("FatJet_Stop0l");
        std::vector<bool> FatJet_matchesPhoton;
        std::vector<bool> FatJet_matchesElectron;
        std::vector<bool> FatJet_matchesMuon;
        if (doPhotonCleaning_)
        {
            FatJet_matchesPhoton   = tr.getVec<bool>("FatJet_matchesPhoton");
        }
        if (doLeptonCleaning_)
        {
            FatJet_matchesElectron = tr.getVec<bool>("FatJet_matchesElectron");
            FatJet_matchesMuon     = tr.getVec<bool>("FatJet_matchesMuon");
        }

        //AK8 subjets 
        const auto& SubJet_LV = tr.getVec_LVFromNano<float>("SubJet");
        
        //resolved top candidates
        const auto& ResTopCand_LV            = tr.getVec_LVFromNano<float>("ResolvedTopCandidate");
        const auto& ResTopCand_discriminator = tr.getVec<float>("ResolvedTopCandidate_discriminator");
        const auto& ResTopCand_j1Idx         = tr.getVec<int>("ResolvedTopCandidate_j1Idx");
        const auto& ResTopCand_j2Idx         = tr.getVec<int>("ResolvedTopCandidate_j2Idx");
        const auto& ResTopCand_j3Idx         = tr.getVec<int>("ResolvedTopCandidate_j3Idx");



        auto* AllTopsTLV        = new std::vector<TLorentzVector>();
        auto* MergedTopsTLV     = new std::vector<TLorentzVector>();
        auto* SemiMergedTopsTLV = new std::vector<TLorentzVector>();
        auto* ResolvedTopsTLV   = new std::vector<TLorentzVector>();
        auto* WTLV              = new std::vector<TLorentzVector>();
        auto* TopJetsMap        = new std::map< int , std::vector<TLorentzVector> >();
        int nAllTops        = -1;
        int nMergedTops     = -1;
        int nSemiMergedTops = -1;
        int nResolvedTops   = -1;
        int nWs             = -1;

        //Select AK4 jets to use in tagger
        //When reading from the resolvedTopCandidate collection from nanoAOD you must pass ALL ak4 jets to ttUtility::ConstAK4Inputs below, 
        //but we can specify a filter vector to tell it to ignore jets we don't want 
        // true: use jet; false: ignore jet
        std::vector<uint8_t> ak4Filter(Jet_LV.size(), true);
        for(int i = 0; i < ak4Filter.size(); ++i)
        {
            //no need to slow the tagger down considering low pt jets
            if(Jet_LV[i].Pt() < 20.0) ak4Filter[i] = false;

            //do some logic here to decide which jet was lepton/photon matched
            if (doLeptonCleaning_)
            {
                if (Jet_matchesElectron[i] || Jet_matchesMuon[i]) ak4Filter[i] = false;
            }
            if (doPhotonCleaning_)
            {
                if (Jet_matchesPhoton[i]) ak4Filter[i] = false;
            }
        }

        //Select AK8 jets to use in tagger
        std::vector<uint8_t> ak8Filter(FatJet_LV.size(), true);
        for(int i = 0; i < ak8Filter.size(); ++i)
        {
            //no need to slow the tagger down considering low pt jets
            if(FatJet_LV[i].Pt() < 200.0) ak8Filter[i] = false;
            // apply Stop0l fat jet selection
            //if(! FatJet_Stop0l[i])        ak8Filter[i] = false;

            //do some logic here to decide which jet was lepton/photon matched
            if (doLeptonCleaning_)
            {
                if (FatJet_matchesElectron[i] || FatJet_matchesMuon[i]) ak8Filter[i] = false;
            }
            if (doPhotonCleaning_)
            {
                if (FatJet_matchesPhoton[i]) ak8Filter[i] = false;
            }
        }

        //Correlate AK8 jets and their subjets
        unsigned int nFatJets = FatJet_LV.size();
        unsigned int nSubJets = SubJet_LV.size();
        std::vector<std::vector<TLorentzVector>> subjets(nFatJets);
        for(int i = 0; i < nFatJets; ++i)
        {
            if(FatJet_subJetIdx1[i] >= 0 && FatJet_subJetIdx1[i] < nSubJets) subjets[i].push_back(SubJet_LV[i]);
            if(FatJet_subJetIdx2[i] >= 0 && FatJet_subJetIdx2[i] < nSubJets) subjets[i].push_back(SubJet_LV[i]);
        }

        //Create top tagger input helpers
        ttUtility::ConstAK4Inputs<float> ak4Inputs(Jet_LV, Jet_btagDeepB);
        ak4Inputs.setFilterVector(ak4Filter);
        ttUtility::ConstAK8Inputs<float> ak8Inputs(FatJet_LV, FatJet_deepAK8_t, FatJet_deepAK8_w, FatJet_msoftdrop, subjets);
        ak8Inputs.setFilterVector(ak8Filter);
        ttUtility::ConstResolvedCandInputs<float> resInputs(ResTopCand_LV, ResTopCand_discriminator, ResTopCand_j1Idx, ResTopCand_j2Idx, ResTopCand_j3Idx);

        //make top tagger constituents, order matters here, ak4Inputs must not come after resInputs as the ak4Inputs are needed to build the resolved top candidates 
        std::vector<Constituent> constituents = packageConstituents(ak4Inputs, resInputs, ak8Inputs);
        
        // TopTagger
        //std::cout << "Create TopTagger object" << std::endl;
        //TopTagger tt("TopTagger.cfg", ".");

        //run top tager
        //std::cout << "tt_->runTagger(constituents)" << std::endl;
        //tt.runTagger(constituents);
        tt_->runTagger(constituents);


        //get tagger results 
        //std::cout << "Get TopTagger results" << std::endl;
        const TopTaggerResults& ttr = tt_->getResults();

        //print top properties
        //get reconstructed tops
        const std::vector<TopObject*>& tops = ttr.getTops();
        
        // --- version using topsByType map
        // --- requires different loops and reordering of AllTops
        //
        // std::map<TopObject::Type, std::vector<TopObject*>> topsByType = ttr.getTopsByType();
        // const std::vector<TopObject*>& MergedTops     = topsByType[TopObject::Type::MERGED_TOP];
        // const std::vector<TopObject*>& SemiMergedTops = topsByType[TopObject::Type::SEMIMERGEDWB_TOP];
        // const std::vector<TopObject*>& ResolvedTops   = topsByType[TopObject::Type::RESOLVED_TOP];
        // const std::vector<TopObject*>& Ws             = topsByType[TopObject::Type::MERGED_W];
        // nMergedTops = MergedTops.size();
        // nSemiMergedTops = SemiMergedTops.size();
        // nResolvedTops = ResolvedTops.size();
        // nAllTops = nMergedTops + nSemiMergedTops + nResolvedTops;
        // nWs = Ws.size();
        // for (const auto* top : MergedTops)      MergedTopsTLV->push_back(top->p());
        // for (const auto* top : SemiMergedTops)  SemiMergedTopsTLV->push_back(top->p());
        // for (const auto* top : ResolvedTops)    ResolvedTopsTLV->push_back(top->p());
        // for (const auto* top : Ws)              WTLV->push_back(top->p());
        //
        // --- version using topsByType map

        bool printTops = false;

        if (printTops) std::cout << "----------------------------------------------------------------------" << std::endl;
        unsigned int topidx = 0;
        for(const TopObject* top : tops)
        {
            TopObject::Type type = top->getType();
            //if (tops.size() > 1) std::cout << "  top type: " << type << std::endl;
            //std::cout << "  top type: " << type << std::endl;
            
            if (type == TopObject::Type::MERGED_W)          WTLV->push_back(top->p());

            //std::cout << "  top type: " << type << std::endl;
            if (   type == TopObject::Type::MERGED_TOP
                || type == TopObject::Type::SEMIMERGEDWB_TOP
                || type == TopObject::Type::RESOLVED_TOP)
            {
                AllTopsTLV->push_back(top->p());
                if (type == TopObject::Type::MERGED_TOP)        MergedTopsTLV->push_back(top->p());
                if (type == TopObject::Type::SEMIMERGEDWB_TOP)  SemiMergedTopsTLV->push_back(top->p());
                if (type == TopObject::Type::RESOLVED_TOP)      ResolvedTopsTLV->push_back(top->p());

                //print basic top properties (top->p() gives a TLorentzVector)
                //N constituents refers to the number of jets included in the top
                //3 for resolved tops 
                //2 for W+jet tops
                //1 for fully merged AK8 tops
                
                if (printTops) printf("\tTop properties: Type: %3d,   Pt: %6.1lf,   Eta: %7.3lf,   Phi: %7.3lf,   M: %7.3lf\n", static_cast<int>(top->getType()), top->p().Pt(), top->p().Eta(), top->p().Phi(), top->p().M());

                //get vector of top constituents 
                const std::vector<Constituent const *>& constituents = top->getConstituents();
                std::vector<TLorentzVector> temp;

                //Print properties of individual top constituent jets 
                for(const Constituent* constituent : constituents)
                {
                    if (printTops) printf("\t\tConstituent properties: Constituent type: %3d,   Pt: %6.1lf,   Eta: %7.3lf,   Phi: %7.3lf\n", constituent->getType(), constituent->p().Pt(), constituent->p().Eta(), constituent->p().Phi());
                    temp.push_back(constituent->p());
                }                
                TopJetsMap->insert(std::make_pair(topidx, temp));
                ++topidx;
            }
        }

        // number of tops
        nAllTops        = AllTopsTLV->size();
        nMergedTops     = MergedTopsTLV->size();
        nSemiMergedTops = SemiMergedTopsTLV->size();
        nResolvedTops   = ResolvedTopsTLV->size();
        nWs             = WTLV->size();
        
        //print the number of tops found in the event 
        //if (tops.size() > 1)
        if (printTops)
        {
            printf("tops.size() =  %ld ",      tops.size());
            printf("nAllTops =  %ld ",         nAllTops);
            printf("nMergedTops =  %ld ",      nMergedTops);
            printf("nSemiMergedTops =  %ld ",  nSemiMergedTops);
            printf("nResolvedTops =  %ld ",    nResolvedTops);
            printf("nWs =  %ld ",              nWs);
            std::cout << std::endl;
        }
        
        tr.registerDerivedVec("AllTopsTLV" + suffix_,           AllTopsTLV);
        tr.registerDerivedVec("MergedTopsTLV" + suffix_,        MergedTopsTLV);
        tr.registerDerivedVec("SemiMergedTopsTLV" + suffix_,    SemiMergedTopsTLV);
        tr.registerDerivedVec("ResolvedTopsTLV" + suffix_,      ResolvedTopsTLV);
        tr.registerDerivedVec("WTLV" + suffix_,                 WTLV);
        tr.registerDerivedVec("TopJetsMap" + suffix_,           TopJetsMap);
        tr.registerDerivedVar("nAllTops" + suffix_,             nAllTops);
        tr.registerDerivedVar("nMergedTops" + suffix_,          nMergedTops);
        tr.registerDerivedVar("nSemiMergedTops" + suffix_,      nSemiMergedTops);
        tr.registerDerivedVar("nResolvedTops" + suffix_,        nResolvedTops);
        tr.registerDerivedVar("nWs" + suffix_,                  nWs);
    }
    
public:

    bool verbose_ = false;

    RunTopTagger(std::string taggerCfg = "TopTagger.cfg", std::string suffix = "", bool doLeptonCleaning = false, bool doPhotonCleaning = false) :
        taggerCfg_ (taggerCfg),
        suffix_ (suffix),
        doLeptonCleaning_ (doLeptonCleaning),
        doPhotonCleaning_ (doPhotonCleaning),
        tt_ (new TopTagger())
    {
        if (verbose_) std::cout << "Constructing RunTopTagger; taggerCfg_ = " << taggerCfg_ << ", suffix_ = " << suffix_ << ", doLeptonCleaning_ = " << doLeptonCleaning_ << ", doPhotonCleaning_ = " << doPhotonCleaning_ << std::endl;
        tt_->setCfgFile(taggerCfg_);
    }
    
    ~RunTopTagger(){}

    void operator()(NTupleReader& tr)
    {
        runTopTagger(tr);
    }
};


#endif
