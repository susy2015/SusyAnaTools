// CleanedJets.h
// Caleb J. Smith
// October 22, 2018

// Input vector of TLorentzVector: objects (photons, leptons, etc)
// Input vector of TLorentzVector: jets (before they are clean)
// Return vector of TLorentzVector: cleaned jets (dR cleaning)


#ifndef CLEANEDJETS_H
#define CLEANEDJETS_H

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"
#include "Math/VectorUtil.h"
#include "SusyAnaTools/Tools/SusyUtility.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

class CleanedJets {

private:
    // AK4 jet variables
    std::vector<std::string> AK4JetVariables_;
    // AK8 jet variables
    std::vector<std::string> AK8JetVariables_;
    
    NTupleReader* tr_;
    void setReader(NTupleReader& tr) { tr_ = &tr; }

    void generateCleanedJets() 
    {
        // register Jet matches object variables
        registerJetMatchesObject();

        //                  jetCollectionName, jetCollectionVariables, prefix, suffix, doLeptonCleaning, doPhotonCleaning, doJetCuts
        cleanJetCollection("Jet",      AK4JetVariables_, "", "_drLeptonCleaned", true,  false, false);
        cleanJetCollection("Jet",      AK4JetVariables_, "", "_drPhotonCleaned", false, true,  false);
        cleanJetCollection("FatJet",   AK8JetVariables_, "", "_drLeptonCleaned", true,  false, false);
        cleanJetCollection("FatJet",   AK8JetVariables_, "", "_drPhotonCleaned", false, true,  false);
        
        //cleanJetCollection("JetTLV",      "",                     AK4JetVariables_, "",               "_pt20eta24",                 false, true);
        //cleanJetCollection("JetTLV",      "",                     AK4JetVariables_, "prodJetsNoLep_", "_pt20eta24",                 false, true);
        //cleanJetCollection("JetTLV",      "cutMuVec;cutElecVec",  AK4JetVariables_, "",               "_drLeptonCleaned_pt20eta24", true, true);
        //cleanJetCollection("FatJetTLV",   "",                     AK8JetVariables_, "",               "_pt20eta24",                 false, true);
        //cleanJetCollection("FatJetTLV",   "",                     AK8JetVariables_, "prodJetsNoLep_", "_pt20eta24",                 false, true);
        //cleanJetCollection("FatJetTLV",   "cutMuVec;cutElecVec",  AK8JetVariables_, "",               "_drLeptonCleaned_pt20eta24", true, true);
    
    }

    void registerJetMatchesObject()
    {
        //TODO: use photon and leptons passing cuts and ID for jet cleaning (instead of all photons and leptons)
        const auto& Jet_TLV            = tr_->getVec<TLorentzVector>("JetTLV");
        const auto& FatJet_TLV         = tr_->getVec<TLorentzVector>("FatJetTLV");
        // use all objects for cleaning
        // const auto& Photon_TLV         = tr_->getVec<TLorentzVector>("PhotonTLV");
        // const auto& Electron_TLV       = tr_->getVec<TLorentzVector>("ElectronTLV");
        // const auto& Muon_TLV           = tr_->getVec<TLorentzVector>("MuonTLV");
        // const auto& Photon_jetIdx      = tr_->getVec<int>("Photon_jetIdx");
        // const auto& Electron_jetIdx    = tr_->getVec<int>("Electron_jetIdx");
        // const auto& Muon_jetIdx        = tr_->getVec<int>("Muon_jetIdx");
        // use objects passing cuts for cleaning
        const auto& Photon_TLV         = tr_->getVec<TLorentzVector>("gammaLVecPassLooseID");
        const auto& Electron_TLV       = tr_->getVec<TLorentzVector>("cutElecVec");
        const auto& Muon_TLV           = tr_->getVec<TLorentzVector>("cutMuVec");
        const auto& Photon_jetIdx      = tr_->getVec<int>("gammaJetIndexPassLooseID");
        const auto& Electron_jetIdx    = tr_->getVec<int>("cutElecJetIndex");
        const auto& Muon_jetIdx        = tr_->getVec<int>("cutMuJetIndex");
        // jet matches object variables that we derive here
        auto& Jet_matchesPhoton        = tr_->createDerivedVec<bool>("Jet_matchesPhoton");
        auto& Jet_matchesElectron      = tr_->createDerivedVec<bool>("Jet_matchesElectron");
        auto& Jet_matchesMuon          = tr_->createDerivedVec<bool>("Jet_matchesMuon");
        auto& FatJet_matchesPhoton     = tr_->createDerivedVec<bool>("FatJet_matchesPhoton");
        auto& FatJet_matchesElectron   = tr_->createDerivedVec<bool>("FatJet_matchesElectron");
        auto& FatJet_matchesMuon       = tr_->createDerivedVec<bool>("FatJet_matchesMuon");
        
        const float DRMAX = 0.2;
        Jet_matchesPhoton      = AnaFunctions::getJetMatchesObjectVec(Jet_TLV,    Photon_TLV, Photon_jetIdx, DRMAX);
        Jet_matchesElectron    = AnaFunctions::getJetMatchesObjectVec(Jet_TLV,    Electron_TLV, Electron_jetIdx, DRMAX);
        Jet_matchesMuon        = AnaFunctions::getJetMatchesObjectVec(Jet_TLV,    Muon_TLV, Muon_jetIdx, DRMAX);
        FatJet_matchesPhoton   = AnaFunctions::getJetMatchesObjectVec(FatJet_TLV, Photon_TLV, Photon_jetIdx, DRMAX);
        FatJet_matchesElectron = AnaFunctions::getJetMatchesObjectVec(FatJet_TLV, Electron_TLV, Electron_jetIdx, DRMAX);
        FatJet_matchesMuon     = AnaFunctions::getJetMatchesObjectVec(FatJet_TLV, Muon_TLV, Muon_jetIdx, DRMAX);

    }

    template <class type> void cleanVector(const std::string& vectorName, const std::vector<bool>& keepJet, const std::string& suffix)
    {
        const auto& vec = tr_->getVec<type>(vectorName); 
        std::vector<type>* cleanedVec = new std::vector<type>();
        if (keepJet.size() != vec.size())
        {
            std::cout << "ERROR in cleanVector(): vector sizes do not match for " << vectorName << ": keepJet.size() = " << keepJet.size() << " and vec.size() = " << vec.size() << std::endl;
        }
        for (int i = 0; i < vec.size(); ++i)
        {
            if (keepJet[i]) cleanedVec->push_back(vec[i]);
        }
        tr_->registerDerivedVec(vectorName + suffix, cleanedVec);
    }

    // clean all variables in jet  collection
    void cleanJetCollection(const std::string& jetCollectionName, const std::vector<std::string>& jetCollectionVariables, const std::string& prefix, const std::string& suffix, bool doLeptonCleaning, bool doPhotonCleaning, bool doJetCuts)
    {

        const auto& Jet_TLV  = tr_->getVec<TLorentzVector>(prefix + jetCollectionName + "TLV");  // jet lorentz vector
        const int& nJets = Jet_TLV.size();
        std::vector<float>* dRvec = new std::vector<float>();
        const float dRMax = 0.20; // dR cut between jets and object

        // vector determining which jets to keep 
        std::vector<bool> keepJet(nJets, true);
        
        // remove jets matched to leptons
        if (doLeptonCleaning)
        {
            const auto& Jet_matchesElectron = tr_->getVec<bool>(prefix + jetCollectionName + "_matchesElectron");
            const auto& Jet_matchesMuon     = tr_->getVec<bool>(prefix + jetCollectionName + "_matchesMuon");
            
            // check lengths
            if (Jet_matchesElectron.size() != nJets || Jet_matchesMuon.size() != nJets)
            {
                std::cout << "ERROR in " << __func__ << ": variables for jet collection " << jetCollectionName << " do not have the same length." << std::endl;
                return;
            }

            for (int i = 0; i < nJets; ++i)
            {
                if (Jet_matchesElectron[i] || Jet_matchesMuon[i]) keepJet[i] = false;
            }
        }
        
        // remove jets matched to photons
        if (doPhotonCleaning)
        {
            const auto& Jet_matchesPhoton = tr_->getVec<bool>(prefix + jetCollectionName + "_matchesPhoton");
            
            // check lengths
            if (Jet_matchesPhoton.size() != nJets)
            {
                std::cout << "ERROR in " << __func__ << ": variables for jet collection " << jetCollectionName << " do not have the same length." << std::endl;
                return;
            }

            for (int i = 0; i < nJets; ++i)
            {
                if (Jet_matchesPhoton[i]) keepJet[i] = false;
            }
        }

        // apply jet p_t and eta cuts
        if(doJetCuts)
        {
            for (int i = 0; i < nJets; i++)
            {
                if ( ! AnaFunctions::jetPassCuts(Jet_TLV[i], AnaConsts::pt20Eta24Arr) ) keepJet[i] = false; 
            }
        }
        
        // clean all variables in jet collection
        for (const auto& jetVariable : jetCollectionVariables)
        {
            // get type
            std::string type;
            // check if branch exists; skip it if it does not (important for data, which does not have generator levels variables)
            if (! tr_->checkBranch(jetVariable) ) continue;
            tr_->getType(jetVariable, type);
            // for testing
            //std::cout << jetVariable << " : " << type << std::endl;
            // check for vector
            if (type.find("std::vector<std::vector") != std::string::npos)
            {
                if (type.find("TLorentzVector") != std::string::npos) cleanVector<std::vector<TLorentzVector>>(prefix + jetVariable, keepJet, suffix);
            }
            else if (type.find("vector") != std::string::npos)
            {
                if      (type.find("TLorentzVector")    != std::string::npos) cleanVector<TLorentzVector>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("float")             != std::string::npos) cleanVector<float>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("double")            != std::string::npos) cleanVector<double>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("unsigned int")      != std::string::npos) cleanVector<unsigned int>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("int")               != std::string::npos) cleanVector<int>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("unsigned char")     != std::string::npos) cleanVector<unsigned char>(prefix + jetVariable, keepJet, suffix);
                else if (type.find("char")              != std::string::npos) cleanVector<char>(prefix + jetVariable, keepJet, suffix);
                else
                {
                    std::cout << "The variable " << jetVariable << "with type " << type << " has an unusual type. Jet cleaning will not be applied to it. You may add this type to SusyAnaTools/Tools/CleanedJets.h if you need this variable." << std::endl;
                }
            }
            else
            {
                std::cout << "The variable " << jetVariable << " with type " << type << " is not a vector. Jet cleaning will not be applied to it." << std::endl;
            }
        }
        // dR between jets and object
        tr_->registerDerivedVec("dR_" + prefix + jetCollectionName + suffix, dRvec);
    }

public:

    // ------------------------------------------------------------------------------------------- //
    // --- see Nano AOD documentation for Jet and FatJet variables: ------------------------------ //
    // --- https://cms-nanoaod-integration.web.cern.ch/integration/master-102X/mc102X_doc.html --- //
    // ------------------------------------------------------------------------------------------- //
    CleanedJets()
    {
        // AK4 jet variables
        AK4JetVariables_ = {
                             // custom
                             "JetTLV",              
                             "Jet_btagStop0l",
                             // from Nano AOD 
                             "Jet_area",
                             "Jet_bRegCorr",
                             "Jet_bRegRes",
                             // not a vector... cannot clean
                             //"Jet_btagCMVA",
                             "Jet_btagCSVV2",
                             "Jet_btagDeepB",
                             "Jet_btagDeepC",
                             "Jet_btagDeepFlavB",
                             "Jet_chEmEF",
                             "Jet_chHEF",
                             "Jet_cleanmask",
                             "Jet_electronIdx1",
                             "Jet_electronIdx2",
                             "Jet_eta",
                             "Jet_genJetIdx",
                             "Jet_hadronFlavour",
                             "Jet_jetId",
                             "Jet_mass",
                             "Jet_muEF",
                             "Jet_muonIdx1",
                             "Jet_muonIdx2",
                             "Jet_nConstituents",
                             "Jet_nElectrons",
                             "Jet_nMuons",
                             "Jet_neEmEF",
                             "Jet_neHEF",
                             "Jet_partonFlavour",
                             "Jet_phi",
                             "Jet_pt",
                             "Jet_puId",
                             "Jet_qgl",
                             "Jet_rawFactor",
                           };
        
        // AK8 jet variables
        AK8JetVariables_ = {
                             // custom
                             "FatJetTLV",              
                             "SubJetTLV_AK8Matched",              
                             // from Nano AOD 
                             "FatJet_area",
                             // not a vector... cannot clean
                             //"FatJet_btagCMVA",
                             "FatJet_btagCSVV2",
                             "FatJet_btagDeepB",
                             "FatJet_btagHbb",
                             // not a vector... cannot clean
                             //"FatJet_deepTagMD_H4qvsQCD",
                             //"FatJet_deepTagMD_HbbvsQCD",
                             //"FatJet_deepTagMD_TvsQCD",
                             //"FatJet_deepTagMD_WvsQCD",
                             //"FatJet_deepTagMD_ZHbbvsQCD",
                             //"FatJet_deepTagMD_ZHccvsQCD",
                             //"FatJet_deepTagMD_ZbbvsQCD",
                             //"FatJet_deepTagMD_ZvsQCD",
                             //"FatJet_deepTagMD_bbvsLight",
                             //"FatJet_deepTagMD_ccvsLight",
                             //"FatJet_deepTag_H",
                             //"FatJet_deepTag_QCD",
                             //"FatJet_deepTag_QCDothers",
                             "FatJet_deepTag_TvsQCD",
                             "FatJet_deepTag_WvsQCD",
                             "FatJet_deepTag_ZvsQCD",
                             "FatJet_eta",
                             "FatJet_jetId",
                             "FatJet_mass",
                             "FatJet_msoftdrop",
                             "FatJet_n2b1",
                             "FatJet_n3b1",
                             "FatJet_phi",
                             "FatJet_pt",
                             "FatJet_rawFactor",
                             "FatJet_subJetIdx1",
                             "FatJet_subJetIdx2",
                             "FatJet_tau1",
                             "FatJet_tau2",
                             "FatJet_tau3",
                             "FatJet_tau4",
                           };
    }

    ~CleanedJets(){}

    void operator()(NTupleReader& tr)
    {
        // first set NTupleReader
        setReader(tr);
        generateCleanedJets();
    }
};

#endif
