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
        //                  jetCollectionName, objectNameString,    jetCollectionVariables, prefix,     suffix,               doDRCleaning, doJetCuts
        cleanJetCollection("JetTLV",      "gammaLVecPassLooseID", AK4JetVariables_, "",               "_drPhotonCleaned",           true, false);
        cleanJetCollection("JetTLV",      "cutMuVec;cutElecVec",  AK4JetVariables_, "",               "_drLeptonCleaned",           true, false);
        cleanJetCollection("JetTLV",      "",                     AK4JetVariables_, "",               "_pt20eta24",                 false, true);
        //cleanJetCollection("JetTLV",      "",                     AK4JetVariables_, "prodJetsNoLep_", "_pt20eta24",                 false, true);
        cleanJetCollection("JetTLV",      "cutMuVec;cutElecVec",  AK4JetVariables_, "",               "_drLeptonCleaned_pt20eta24", true, true);
        cleanJetCollection("FatJetTLV",   "gammaLVecPassLooseID", AK8JetVariables_, "",               "_drPhotonCleaned",           true, false);
        cleanJetCollection("FatJetTLV",   "cutMuVec;cutElecVec",  AK8JetVariables_, "",               "_drLeptonCleaned",           true, false);
        cleanJetCollection("FatJetTLV",   "",                     AK8JetVariables_, "",               "_pt20eta24",                 false, true);
        //cleanJetCollection("FatJetTLV",   "",                     AK8JetVariables_, "prodJetsNoLep_", "_pt20eta24",                 false, true);
        cleanJetCollection("FatJetTLV",   "cutMuVec;cutElecVec",  AK8JetVariables_, "",               "_drLeptonCleaned_pt20eta24", true, true);
    }

    template <class type> void cleanVector(std::string vectorName, std::vector<bool> keepJet, const std::string& suffix)
    {
        const auto& vec = tr_->getVec<type>(vectorName); 
        std::vector<type>* cleanedVec = new std::vector<type>();
        if (keepJet.size() != vec.size())
        {
            //std::cout << "ERROR in cleanVector(): vector sizes do not match for " << vectorName << ": keepJet.size() = " << keepJet.size() << " and vec.size() = " << vec.size() << std::endl;
        }
        for (int i = 0; i < vec.size(); ++i)
        {
            if (keepJet[i]) cleanedVec->push_back(vec[i]);
        }
        tr_->registerDerivedVec(vectorName + suffix, cleanedVec);
    }

    // clean all variables in jet  collection
    void cleanJetCollection(const std::string& jetCollectionName, const std::string& objectNameString, const std::vector<std::string>& jetCollectionVariables, const std::string& prefix, const std::string& suffix, bool doDRCleaning, bool doJetCuts)
    {
        std::vector<std::string> objectNameVec = SusyUtility::getVecFromString(objectNameString, ';'); 

        const auto& jetsLVec  = tr_->getVec<TLorentzVector>(prefix + jetCollectionName);  // jet lorentz vector
        std::vector<float>* dRvec = new std::vector<float>();
        const float dRMax = 0.20; // dR cut between jets and object

        // vector determining which jets to keep 
        std::vector<bool> keepJet(jetsLVec.size(), true);
        // loop over objects: remove jets using DR matching
        if (doDRCleaning)
        {
            for (const auto& objectName : objectNameVec)
            {
                const auto& objectLVec = tr_->getVec<TLorentzVector>(objectName);

                for (int i = 0; i < objectLVec.size(); ++i)
                {
                    int match = AnaFunctions::jetObjectdRMatch(objectLVec[i], jetsLVec, dRMax, dRvec);
                    if (match >= 0) keepJet[match] = false;
                }
            }
        }

        // apply jet p_t and eta cuts
        if(doJetCuts)
        {
            for (int i = 0; i < jetsLVec.size(); i++)
            {
                if ( ! AnaFunctions::jetPassCuts(jetsLVec[i], AnaConsts::pt20Eta24Arr) ) keepJet[i] = false; 
            }
        }
        
        // clean all variables in jet collection
        for (const auto& jetVariable : jetCollectionVariables)
        {
            // get type
            std::string type;
            tr_->getType(jetVariable, type);
            //std::cout << jetVariable << " : " << type << std::endl; 
            // check for vector
            if (type.find("vector") != std::string::npos)
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

    CleanedJets()
    {
        // AK4 jet variables
        AK4JetVariables_ = {
                             "JetTLV",              
                             "Jet_pt",              
                             "Jet_eta",              
                             "Jet_phi",              
                             "Jet_mass",              
                             "Jet_btagDeepB",              
                             //"recoJetsCSVv2",
                             //"qgLikelihood",
                             //"qgPtD",                               
                             //"qgAxis1",                             
                             //"qgAxis2",                             
                             //"qgMult",                              
                             //"recoJetschargedHadronEnergyFraction", 
                             //"recoJetschargedEmEnergyFraction",     
                             //"recoJetsneutralEmEnergyFraction",     
                             //"recoJetsmuonEnergyFraction",          
                             //"recoJetsHFHadronEnergyFraction",     
                             //"recoJetsHFEMEnergyFraction",          
                             //"recoJetsneutralEnergyFraction",       
                             //"PhotonEnergyFraction",                
                             //"ElectronEnergyFraction",             
                             //"ChargedHadronMultiplicity",          
                             //"NeutralHadronMultiplicity",          
                             //"PhotonMultiplicity",                
                             //"ElectronMultiplicity",               
                             //"MuonMultiplicity",                    
                             //"DeepCSVb",                            
                             //"DeepCSVc",                            
                             //"DeepCSVl",                            
                             //"DeepCSVbb",                           
                             //"DeepCSVcc"
                           };
        
        // AK8 jet variables
        AK8JetVariables_ = {
                             "FatJetTLV",              
                             "FatJet_pt",              
                             "FatJet_eta",              
                             "FatJet_phi",              
                             "FatJet_mass",              
                             "SubJetTLV",              
                             "SubJet_pt",              
                             "SubJet_eta",              
                             "SubJet_phi",              
                             "SubJet_mass",              
                             //"puppisoftDropMass",              
                             //"puppitau1",
                             //"puppitau2",
                             //"puppitau3",
                             //"deepAK8btop",              
                             //"deepAK8bW"
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
