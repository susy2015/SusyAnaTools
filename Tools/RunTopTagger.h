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
    std::string jetsLabel_;
    std::string taggerCfg_;
    std::string suffix_;
    bool doLeptonCleaning_ = false;
    bool doPhotonCleaning_ = false;

    std::string getSATVar(std::string collection, std::string var)
    {
        // only use SAT version of variable when doing lepton/photon cleaning (using top candidates which are already saved)
        // otherwise, we run the top tagger with Tensorflow and do not use the top candidates
        bool useSAT = doLeptonCleaning_ || doPhotonCleaning_;
        std::string final_var = collection + var;
        if (useSAT) 
        {
            final_var = "SAT_" + collection + var + "_jetpt30";
            if (suffix_.find("jesTotalUp") != std::string::npos)
            {
                final_var += "_jesTotalUp";
            }
            else if (suffix_.find("jesTotalDown") != std::string::npos)
            {
                final_var += "_jesTotalDown";
            }
        }
        else
        {
            if (suffix_.find("jesTotalUp") != std::string::npos)
            {
                final_var = collection + "_JESUp" + var;
            }
            else if (suffix_.find("jesTotalDown") != std::string::npos)
            {
                final_var = collection + "_JESDown" + var;
            }
        }
        return final_var;
    }

    void runTopTagger(NTupleReader& tr) 
    {
        // print for testing
        std::cout << "Running top tagger: suffix_ = " << suffix_ << std::endl;

        //get necessary tagger input variables 

        //AK4 jets
        const auto& Jet_LV          = tr.getVec<TLorentzVector>(jetsLabel_);
        const auto& Jet_btagDeepB   = tr.getVec<float>("Jet_btagDeepB");
        const auto& Jet_qgl         = tr.getVec<float>("Jet_qgl");
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
        const auto& FatJet_SF              = tr.getVec<float>("FatJet_SF");
        const auto& FatJet_SFerr           = tr.getVec<float>("FatJet_SFerr");
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
        
        // --- old version --- //
        //resolved top candidates
        //const auto& ResTopCand_LV            = tr.getVec_LVFromNano<float>("ResolvedTopCandidate");
        //const auto& ResTopCand_discriminator = tr.getVec<float>("ResolvedTopCandidate_discriminator");
        //const auto& ResTopCand_j1Idx         = tr.getVec<int>("ResolvedTopCandidate_j1Idx");
        //const auto& ResTopCand_j2Idx         = tr.getVec<int>("ResolvedTopCandidate_j2Idx");
        //const auto& ResTopCand_j3Idx         = tr.getVec<int>("ResolvedTopCandidate_j3Idx");
        
        // --- new version --- //
        // load previously run top tagger candidates to fix double truncation/rounding
        // resolved top candidates
        // std::string getSATVar(std::string collection, std::string var)
        const auto& ResTopCand_LV            = tr.getVec<TLorentzVector>(   getSATVar("ResolvedTopCandidate", "TLV")                );
        const auto& ResTopCand_discriminator = tr.getVec<float>(            getSATVar("ResolvedTopCandidate", "_discriminator")     );
        const auto& ResTopCand_j1Idx         = tr.getVec<int>(              getSATVar("ResolvedTopCandidate", "_j1Idx")             );
        const auto& ResTopCand_j2Idx         = tr.getVec<int>(              getSATVar("ResolvedTopCandidate", "_j2Idx")             );
        const auto& ResTopCand_j3Idx         = tr.getVec<int>(              getSATVar("ResolvedTopCandidate", "_j3Idx")             );

        auto* MergedTopsTLV                             = new std::vector<TLorentzVector>();
        auto* MergedTops_disc                           = new std::vector<float>();
        auto* MergedTops_JetsMap                        = new std::map< int , std::vector<TLorentzVector> >();
        auto* WTLV                                      = new std::vector<TLorentzVector>();
        auto* W_disc                                    = new std::vector<float>();
        auto* W_JetsMap                                 = new std::map< int , std::vector<TLorentzVector> >();
        auto* ResolvedTopsTLV                           = new std::vector<TLorentzVector>();
        auto* ResolvedTops_disc                         = new std::vector<float>();
        auto* ResolvedTops_JetsMap                      = new std::map< int , std::vector<TLorentzVector> >();
        auto& SAT_ResolvedTopCandidate_TLV              = tr.createDerivedVec<TLorentzVector>("SAT_ResolvedTopCandidateTLV"   + suffix_);
        auto& SAT_ResolvedTopCandidate_discriminator    = tr.createDerivedVec<float>("SAT_ResolvedTopCandidate_discriminator" + suffix_);
        auto& SAT_ResolvedTopCandidate_j1Idx            = tr.createDerivedVec<int>("SAT_ResolvedTopCandidate_j1Idx"           + suffix_);
        auto& SAT_ResolvedTopCandidate_j2Idx            = tr.createDerivedVec<int>("SAT_ResolvedTopCandidate_j2Idx"           + suffix_);
        auto& SAT_ResolvedTopCandidate_j3Idx            = tr.createDerivedVec<int>("SAT_ResolvedTopCandidate_j3Idx"           + suffix_);
        
        int nMergedTops                 = 0;
        int nWs                         = 0;
        int nResolvedTops               = 0;
        float MergedTopTotalSF          = 1.0;
        float MergedTopTotalSF_Up       = 1.0;
        float MergedTopTotalSF_Down     = 1.0;
        float WTotalSF                  = 1.0;
        float WTotalSF_Up               = 1.0;
        float WTotalSF_Down             = 1.0;
        float ResolvedTopTotalSF        = 1.0;
        float ResolvedTopTotalSF_Up     = 1.0;
        float ResolvedTopTotalSF_Down   = 1.0;

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

            //do some logic here to decide which jet was lepton/photon matched
            if (doLeptonCleaning_)
            {
                if (FatJet_matchesElectron[i] || FatJet_matchesMuon[i]) ak8Filter[i] = false;
            }
            if (doPhotonCleaning_)
            {
                if (FatJet_matchesPhoton[i]) ak8Filter[i] = false;
            }
            //use post-processed selections to calcualte number of merged tops and Ws 
            if(ak8Filter[i])
            {
                if(FatJet_Stop0l[i] == 1)
                {
                    if (verbose_ >= 2)
                    {
                        printf("Merged Top: FatJet_SF[%d] = %f,  FatJet_SFerr[%d] = %f\n", i, FatJet_SF[i], i, FatJet_SFerr[i]);
                    }
                    nMergedTops             += 1;
                    MergedTopTotalSF        *= FatJet_SF[i];
                    MergedTopTotalSF_Up     *= (FatJet_SF[i] + FatJet_SFerr[i]);
                    MergedTopTotalSF_Down   *= (FatJet_SF[i] - FatJet_SFerr[i]);
                }
                if(FatJet_Stop0l[i] == 2)
                {
                    if (verbose_ >= 2)
                    {
                        printf("W: FatJet_SF[%d] = %f,  FatJet_SFerr[%d] = %f\n", i, FatJet_SF[i], i, FatJet_SFerr[i]);
                    }
                    nWs             += 1;
                    WTotalSF        *= FatJet_SF[i];
                    WTotalSF_Up     *= (FatJet_SF[i] + FatJet_SFerr[i]);
                    WTotalSF_Down   *= (FatJet_SF[i] - FatJet_SFerr[i]);
                }
            }
        }

        //Correlate AK8 jets and their subjets
        unsigned int nFatJets = FatJet_LV.size();
        unsigned int nSubJets = SubJet_LV.size();
        std::vector<std::vector<TLorentzVector>> subjets(nFatJets);
        for(int i = 0; i < nFatJets; ++i)
        {
            if(FatJet_subJetIdx1[i] >= 0 && FatJet_subJetIdx1[i] < nSubJets) subjets[i].push_back(SubJet_LV[FatJet_subJetIdx1[i]]);
            if(FatJet_subJetIdx2[i] >= 0 && FatJet_subJetIdx2[i] < nSubJets) subjets[i].push_back(SubJet_LV[FatJet_subJetIdx2[i]]);
        }
        
        // --------------------------------------- // 
        // --- Create top tagger input helpers --- //
        // --------------------------------------- // 

        // use gen info to get proper scale factors
        ttUtility::ConstAK4Inputs<float> *ak4Inputs                         = nullptr;
        std::vector<TLorentzVector> *genTops                                = nullptr;
        std::vector<std::vector<const TLorentzVector*>> *genTopDaughters    = nullptr;

        bool isData = ! tr.checkBranch("GenPart_pt");

        if (! isData)
        {
            const auto& GenPartTLV                  = tr.getVec<TLorentzVector>("GenPartTLV"); 
            const auto& GenPart_pdgId               = tr.getVec<int>("GenPart_pdgId");
            const auto& GenPart_statusFlags         = tr.getVec<int>("GenPart_statusFlags");
            const auto& GenPart_genPartIdxMother    = tr.getVec<int>("GenPart_genPartIdxMother");
            auto genMatchingInfo                    = ttUtility::GetTopdauGenLVecFromNano(GenPartTLV, GenPart_pdgId, GenPart_statusFlags, GenPart_genPartIdxMother);
            genTops                                 = new std::vector<TLorentzVector>(std::move(genMatchingInfo.first));
            genTopDaughters                         = new std::vector<std::vector<const TLorentzVector*>>(std::move(genMatchingInfo.second));
            ak4Inputs                               = new ttUtility::ConstAK4Inputs<float>(Jet_LV, Jet_btagDeepB, Jet_qgl, *genTops, *genTopDaughters);
        }
        else
        {
            //no gen info is avaliable for data
            genTops   = new std::vector<TLorentzVector>();
            ak4Inputs = new ttUtility::ConstAK4Inputs<float>(Jet_LV, Jet_btagDeepB, Jet_qgl);
        }

        //Helper function to turn int vectors into float vectors
        auto convertTofloatandRegister = [](NTupleReader& tr, const std::string& name)
        {
            const std::vector<int>& intVec = tr.getVec<int>(name);
            std::vector<float>* floatVec = new std::vector<float>(intVec.begin(), intVec.end());
            tr.registerDerivedVec(name+"ConvertedTofloat3", floatVec);
            return floatVec;
        };

        ak4Inputs->addSupplamentalVector("qgLikelihood",                         tr.getVec<float>("Jet_qgl"));
        ak4Inputs->addSupplamentalVector("qgPtD",                                tr.getVec<float>("Jet_qgptD"));
        ak4Inputs->addSupplamentalVector("qgAxis1",                              tr.getVec<float>("Jet_qgAxis1"));
        ak4Inputs->addSupplamentalVector("qgAxis2",                              tr.getVec<float>("Jet_qgAxis2"));
        ak4Inputs->addSupplamentalVector("recoJetschargedHadronEnergyFraction",  tr.getVec<float>("Jet_chHEF"));
        ak4Inputs->addSupplamentalVector("recoJetschargedEmEnergyFraction",      tr.getVec<float>("Jet_chEmEF"));
        ak4Inputs->addSupplamentalVector("recoJetsneutralEmEnergyFraction",      tr.getVec<float>("Jet_neEmEF"));
        ak4Inputs->addSupplamentalVector("recoJetsmuonEnergyFraction",           tr.getVec<float>("Jet_muEF"));
        ak4Inputs->addSupplamentalVector("recoJetsHFHadronEnergyFraction",       tr.getVec<float>("Jet_hfHadEF"));
        ak4Inputs->addSupplamentalVector("recoJetsHFEMEnergyFraction",           tr.getVec<float>("Jet_hfEMEF"));
        ak4Inputs->addSupplamentalVector("recoJetsneutralEnergyFraction",        tr.getVec<float>("Jet_neHEF"));
        ak4Inputs->addSupplamentalVector("PhotonEnergyFraction",                 tr.getVec<float>("Jet_phEF"));
        ak4Inputs->addSupplamentalVector("ElectronEnergyFraction",               tr.getVec<float>("Jet_elEF"));
        ak4Inputs->addSupplamentalVector("ChargedHadronMultiplicity",            tr.getVec<float>("Jet_chHadMult"));
        ak4Inputs->addSupplamentalVector("NeutralHadronMultiplicity",            tr.getVec<float>("Jet_neHadMult"));
        ak4Inputs->addSupplamentalVector("PhotonMultiplicity",                   tr.getVec<float>("Jet_phMult"));
        ak4Inputs->addSupplamentalVector("ElectronMultiplicity",                 tr.getVec<float>("Jet_elMult"));
        ak4Inputs->addSupplamentalVector("MuonMultiplicity",                     tr.getVec<float>("Jet_muMult"));
        ak4Inputs->addSupplamentalVector("DeepCSVb",                             tr.getVec<float>("Jet_deepCSVb"));
        ak4Inputs->addSupplamentalVector("DeepCSVc",                             tr.getVec<float>("Jet_deepCSVc"));
        ak4Inputs->addSupplamentalVector("DeepCSVl",                             tr.getVec<float>("Jet_deepCSVudsg"));
        ak4Inputs->addSupplamentalVector("DeepCSVbb",                            tr.getVec<float>("Jet_deepCSVbb"));
        ak4Inputs->addSupplamentalVector("CvsL",                                 tr.getVec<float>("Jet_CvsL"));
        ak4Inputs->addSupplamentalVector("qgMult",                               *convertTofloatandRegister(tr, "Jet_qgMult"));


        ak4Inputs->setFilterVector(ak4Filter);
        ttUtility::ConstAK8Inputs<float> ak8Inputs(FatJet_LV, FatJet_deepAK8_t, FatJet_deepAK8_w, FatJet_msoftdrop, subjets);
        ak8Inputs.setFilterVector(ak8Filter);
        ttUtility::ConstResolvedCandInputs<float> resInputs(ResTopCand_LV, ResTopCand_discriminator, ResTopCand_j1Idx, ResTopCand_j2Idx, ResTopCand_j3Idx);

        //make top tagger constituents, order matters here, ak4Inputs must not come after resInputs as the ak4Inputs are needed to build the resolved top candidates 
        std::vector<Constituent> constituents = packageConstituents(*ak4Inputs, resInputs, ak8Inputs);
        
        //run top tager
        try {
            tt_->runTagger(constituents);
        }
        catch (TTException e)
        {
            std::cout << "Here is your TTException: " << e << std::endl;
        }


        // delete pointers
        delete ak4Inputs;
        if(genTopDaughters) delete genTopDaughters;


        //get tagger results 
        const TopTaggerResults& ttr = tt_->getResults();
        
        //get tops and top candidates
        const std::vector<TopObject*>& tops           = ttr.getTops();
        const std::vector<TopObject>&  topCandidates  = ttr.getTopCandidates();
        
        /////////////////////////////////////////
        // save top tagger candidate variables //
        /////////////////////////////////////////
        for(const TopObject topCand : topCandidates)
        {
            int nConstituents = topCand.getNConstituents();
            //printf("nConstituents = %d\n", nConstituents);
            std::vector<int> jetIdxs(3, -1);
            for (int i = 0; i < 3; ++i)
            {
                // check that there are enough constituents before accessing
                if (nConstituents > i)
                {
                    jetIdxs[i] = constituents[i].getIndex();
                }
            }
            const std::vector<Constituent const *>& constituents = topCand.getConstituents();
            SAT_ResolvedTopCandidate_TLV.push_back(topCand.p());
            SAT_ResolvedTopCandidate_discriminator.push_back(topCand.getDiscriminator());
            SAT_ResolvedTopCandidate_j1Idx.push_back(jetIdxs[0]);
            SAT_ResolvedTopCandidate_j2Idx.push_back(jetIdxs[1]);
            SAT_ResolvedTopCandidate_j3Idx.push_back(jetIdxs[2]);
        }

        bool printTops = false;

        if (printTops) std::cout << "----------------------------------------------------------------------" << std::endl;
        unsigned int MergedTopIdx   = 0;
        unsigned int WIdx           = 0;
        unsigned int ResolvedTopIdx = 0;
        for(const TopObject* top : tops)
        {
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
            
            TopObject::Type type = top->getType();
            //if (tops.size() > 1) std::cout << "  top type: " << type << std::endl;
            
            if (type == TopObject::Type::MERGED_TOP)        
            {
                MergedTopsTLV->push_back(top->p());
                MergedTops_disc->push_back(top->getDiscriminator());
                MergedTops_JetsMap->insert(std::make_pair(MergedTopIdx, temp));
                ++MergedTopIdx;
            }
            if (type == TopObject::Type::MERGED_W)          
            {
                WTLV->push_back(top->p());
                W_disc->push_back(top->getDiscriminator());
                W_JetsMap->insert(std::make_pair(WIdx, temp));
                ++WIdx;
            }
            if (type == TopObject::Type::RESOLVED_TOP)      
            {
                // systematic uncertainties
                double totalUp   = 0.0;
                double totalDown = 0.0;
                const auto& syst_map = top->getAllSystematicUncertainties();
                if (verbose_ >= 2) printf("--- resolved top systematics ---\n");
                for (const auto& syst : syst_map)
                {
                    if (verbose_ >= 2) printf("%s : %f\n", syst.first.c_str(), syst.second);
                    if (syst.first.find("_Up") != std::string::npos)        totalUp   += syst.second * syst.second;
                    else if (syst.first.find("_Down") != std::string::npos) totalDown += syst.second * syst.second;
                    // symmetric uncertainty without Up/Down
                    else
                    {
                        totalUp   += syst.second * syst.second;
                        totalDown += syst.second * syst.second;
                    }
                }
                // scale factor
                double sf       = top->getMCScaleFactor();
                double systUp   = sqrt(totalUp);
                double systDown = sqrt(totalDown);
                if (verbose_ >= 2) printf("sf = %f, systUp = %f, systDown = %f\n", sf, systUp, systDown);
                ResolvedTopTotalSF      *= sf;
                ResolvedTopTotalSF_Up   *= (sf + systUp);
                ResolvedTopTotalSF_Down *= (sf - systDown);
                ResolvedTopsTLV->push_back(top->p());
                ResolvedTops_disc->push_back(top->getDiscriminator());
                ResolvedTops_JetsMap->insert(std::make_pair(ResolvedTopIdx, temp));
                ++ResolvedTopIdx;
            }

        }

        // --- WARNING: Testing fat jets --- //
        // --- testing number of merged tops and Ws calculated in this module ---
        nMergedTops     = MergedTopsTLV->size();
        nWs             = WTLV->size();
        // number of resolved tops
        nResolvedTops   = ResolvedTopsTLV->size();
        
        //print the number of tops found in the event 
        if (printTops)
        {
            printf("tops.size() =  %ld ",      tops.size());
            printf("nMergedTops =  %ld ",      nMergedTops);
            printf("nWs =  %ld ",              nWs);
            printf("nResolvedTops =  %ld ",    nResolvedTops);
            std::cout << std::endl;
        }
        
        tr.registerDerivedVar("nMergedTops" + suffix_,                              nMergedTops);
        tr.registerDerivedVar("MergedTopTotalSF" + suffix_,                         MergedTopTotalSF);
        tr.registerDerivedVar("MergedTopTotalSF_Up" + suffix_,                      MergedTopTotalSF_Up);
        tr.registerDerivedVar("MergedTopTotalSF_Down" + suffix_,                    MergedTopTotalSF_Down);
        tr.registerDerivedVec("MergedTopsTLV" + suffix_,                            MergedTopsTLV);
        tr.registerDerivedVec("MergedTops_disc" + suffix_,                          MergedTops_disc);
        tr.registerDerivedVec("MergedTops_JetsMap" + suffix_,                       MergedTops_JetsMap);
        tr.registerDerivedVar("nWs" + suffix_,                                      nWs);
        tr.registerDerivedVar("WTotalSF" + suffix_,                                 WTotalSF);
        tr.registerDerivedVar("WTotalSF_Up" + suffix_,                              WTotalSF_Up);
        tr.registerDerivedVar("WTotalSF_Down" + suffix_,                            WTotalSF_Down);
        tr.registerDerivedVec("WTLV" + suffix_,                                     WTLV);
        tr.registerDerivedVec("W_disc" + suffix_,                                   W_disc);
        tr.registerDerivedVec("W_JetsMap" + suffix_,                                W_JetsMap);
        tr.registerDerivedVar("nResolvedTops" + suffix_,                            nResolvedTops);
        tr.registerDerivedVar("ResolvedTopTotalSF" + suffix_,                       ResolvedTopTotalSF);
        tr.registerDerivedVar("ResolvedTopTotalSF_Up" + suffix_,                    ResolvedTopTotalSF_Up);
        tr.registerDerivedVar("ResolvedTopTotalSF_Down" + suffix_,                  ResolvedTopTotalSF_Down);
        tr.registerDerivedVec("ResolvedTopsTLV" + suffix_,                          ResolvedTopsTLV);
        tr.registerDerivedVec("ResolvedTops_disc" + suffix_,                        ResolvedTops_disc);
        tr.registerDerivedVec("ResolvedTops_JetsMap" + suffix_,                     ResolvedTops_JetsMap);
        tr.registerDerivedVar("ttr" + suffix_,                                      &ttr);
        tr.registerDerivedVec("genTops" + suffix_,                                  genTops);
    }
    
public:

    int verbose_ = 0;

    RunTopTagger(std::string taggerCfg = "TopTagger.cfg", std::string suffix = "", bool doLeptonCleaning = false, bool doPhotonCleaning = false) :
        taggerCfg_ (taggerCfg),
        suffix_ (suffix),
        doLeptonCleaning_ (doLeptonCleaning),
        doPhotonCleaning_ (doPhotonCleaning),
        tt_ (new TopTagger())
    {
        jetsLabel_           = "JetTLV";
        if (suffix_.find("jesTotalUp") != std::string::npos)
        {
            jetsLabel_              += "_jesTotalUp";
        }
        else if (suffix_.find("jesTotalDown") != std::string::npos)
        {
            jetsLabel_              += "_jesTotalDown";
        }
        if (verbose_) 
        {
            std::cout << "Constructing RunTopTagger; taggerCfg_ = " << taggerCfg_ << ", suffix_ = " << suffix_ << ", jetsLabel_ = " << jetsLabel_ 
                      << ", doLeptonCleaning_ = " << doLeptonCleaning_ << ", doPhotonCleaning_ = " << doPhotonCleaning_ << std::endl;
        }
        tt_->setCfgFile(taggerCfg_);
    }
    
    ~RunTopTagger(){}

    void operator()(NTupleReader& tr)
    {
        runTopTagger(tr);
    }
};


#endif
