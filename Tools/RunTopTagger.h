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

    void runTopTagger(NTupleReader& tr) 
    {
        //get necessary tagger input variables 

        //AK4 jets
        const auto& Jet_LV    = tr.getVec_LVFromNano<float>("Jet");
        const auto& Jet_csvv2 = tr.getVec<float>("Jet_btagCSVV2");
        
        //resolved top candidates
        const auto& ResTopCand_LV = tr.getVec_LVFromNano<float>("ResolvedTopCandidate");
        const auto& ResTopCand_discriminator = tr.getVec<float>("ResolvedTopCandidate_discriminator");
        const auto& ResTopCand_j1Idx = tr.getVec<int>("ResolvedTopCandidate_j1Idx");
        const auto& ResTopCand_j2Idx = tr.getVec<int>("ResolvedTopCandidate_j2Idx");
        const auto& ResTopCand_j3Idx = tr.getVec<int>("ResolvedTopCandidate_j3Idx");

        //AK8 jets
        const auto& FatJet_LV = tr.getVec_LVFromNano<float>("FatJet");
        const auto& FatJet_deepAK8_t = tr.getVec<float>("FatJet_deepTag_TvsQCD");
        const auto& FatJet_deepAK8_w = tr.getVec<float>("FatJet_deepTag_WvsQCD");
        const auto& FatJet_msoftdrop = tr.getVec<float>("FatJet_msoftdrop");
        const auto& FatJet_subJetIdx1 = tr.getVec<int>("FatJet_subJetIdx1");
        const auto& FatJet_subJetIdx2 = tr.getVec<int>("FatJet_subJetIdx2");

        //AK8 subjets 
        const auto& SubJet_LV = tr.getVec_LVFromNano<float>("SubJet");

        //Select AK4 jets to use in tagger
        //When reading from the resolvedTopCandidate collection from nanoAOD you must pass ALL ak4 jets to ttUtility::ConstAK4Inputs below, 
        //but we can specify a filter vector to tell it to ignore jets we don't want 
        std::vector<uint8_t> ak4Filter(Jet_LV.size(), true);
        for(int i = 0; i < ak4Filter.size(); ++i)
        {
            //no need to slow the tagger down considering low pt jets
            if(Jet_LV[i].Pt() < 20.0) ak4Filter[i] = false;

            //do some logic here to decide which jet was lepton/photon matched
            bool isLepPhotonMatch = false;
            if(isLepPhotonMatch) ak4Filter[i] = false;
        }

        //Select AK8 jets to use in tagger
        std::vector<uint8_t> ak8Filter(FatJet_LV.size(), true);
        for(int i = 0; i < ak8Filter.size(); ++i)
        {
            //no need to slow the tagger down considering low pt jets
            if(FatJet_LV[i].Pt() < 200.0) ak8Filter[i] = false;

            //do some logic here to decide which jet was lepton/photon matched
            bool isLepPhotonMatch = false;
            if(isLepPhotonMatch) ak8Filter[i] = false;
        }

        //Corrrolate AK8 jets and their subjets
        unsigned int nFatJets = FatJet_LV.size();
        unsigned int nSubJets = SubJet_LV.size();
        std::vector<std::vector<TLorentzVector>> subjets(nFatJets);
        for(int i = 0; i < nFatJets; ++i)
        {
            if(FatJet_subJetIdx1[i] >= 0 && FatJet_subJetIdx1[i] < nSubJets) subjets[i].push_back(SubJet_LV[i]);
            if(FatJet_subJetIdx2[i] >= 0 && FatJet_subJetIdx2[i] < nSubJets) subjets[i].push_back(SubJet_LV[i]);
        }

        //Create top tagger input helpers
        ttUtility::ConstAK4Inputs<float> ak4Inputs(Jet_LV, Jet_csvv2);
        ak4Inputs.setFilterVector(ak4Filter);
        ttUtility::ConstAK8Inputs<float> ak8Inputs(FatJet_LV, FatJet_deepAK8_t, FatJet_deepAK8_w, FatJet_msoftdrop, subjets);
        ak8Inputs.setFilterVector(ak8Filter);
        ttUtility::ConstResolvedCandInputs<float> resInputs(ResTopCand_LV, ResTopCand_discriminator, ResTopCand_j1Idx, ResTopCand_j2Idx, ResTopCand_j3Idx);

        //make top tagger constituents, order matters here, ak4Inputs must not come after resInputs as the ak4Inputs are needed to build the resolved top candidates 
        std::vector<Constituent> constituents = packageConstituents(ak4Inputs, resInputs, ak8Inputs);
        
        // TopTagger
        //std::cout << "Create TopTagger object" << std::endl;
        TopTagger tt("TopTagger.cfg", ".");

        //run top tager
        //std::cout << "Run TopTagger" << std::endl;
        tt.runTagger(constituents);

        //get tagger results 
        //std::cout << "Get TopTagger results" << std::endl;
        const TopTaggerResults& ttr = tt.getResults();

        //print top properties
        //get reconstructed top
        const std::vector<TopObject*>& tops = ttr.getTops();

        //print the number of tops found in the event 
        if (tops.size() > 1) printf("\tN tops: %ld\n", tops.size());
        //printf("\tN tops: %ld\n", tops.size());


        for(const TopObject* top : tops)
        {
            //print basic top properties (top->p() gives a TLorentzVector)
            //N constituents refers to the number of jets included in the top
            //3 for resolved tops 
            //2 for W+jet tops
            //1 for fully merged AK8 tops
            
            //printf("\tTop properties: Type: %3d,   Pt: %6.1lf,   Eta: %7.3lf,   Phi: %7.3lf,   M: %7.3lf\n", static_cast<int>(top->getType()), top->p().Pt(), top->p().Eta(), top->p().Phi(), top->p().M());

            //get vector of top constituents 
            const std::vector<Constituent const *>& constituents = top->getConstituents();

            //Print properties of individual top constituent jets 
            for(const Constituent* constituent : constituents)
            {
                //printf("\t\tConstituent properties: Constituent type: %3d,   Pt: %6.1lf,   Eta: %7.3lf,   Phi: %7.3lf\n", constituent->getType(), constituent->p().Pt(), constituent->p().Eta(), constituent->p().Phi());
            }                
        }
    }
    
public:

    RunTopTagger(){}
    
    ~RunTopTagger(){}

    void operator()(NTupleReader& tr)
    {
        runTopTagger(tr);
    }
};


#endif