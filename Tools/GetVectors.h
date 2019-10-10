// GetVectors.h
// Caleb J. Smith
// February 5, 2019

// For use with Nano AOD
// Register vector of TLorentzVector from vectors of pt, eta, phi, and mass components

#ifndef GETVECTORS_H
#define GETVECTORS_H

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"
#include "Math/VectorUtil.h"
#include "SusyAnaTools/Tools/SusyUtility.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

class GetVectors {

private:
    NTupleReader* tr_;
    void setReader(NTupleReader& tr) { tr_ = &tr; }

    void generateGetVectors() 
    {
        // register vector<TLorentzVector>
        registerTLV("Jet");                     // AK4 jets
        registerTLV("FatJet");                  // AK8 jets 
        registerTLV("SubJet");                  // AK8 subjets
        registerTLV("ResolvedTopCandidate");    // resolved tops
        registerTLV("SB");                      // soft (low pt) bottom quarks
        
        // assign subjets to their AK8 jets
        // register vector<vector<TLorentzVector>> of subjets
        registerSubJets("_AK8Matched");
        
        if (tr_->checkBranch("GenJet_pt"))
        {
            registerTLV("GenJet");      // generated AK4 jets
            registerTLV("GenJetAK8");   // generated AK8 jets
            registerTLV("GenPart");     // interesting gen particles
        }
        
        registerTLV("Electron");    // electrons
        registerTLV("Muon");        // muons
        registerTLV("Photon");      // photons
        // you need to register IsoTrack by hand because IsoTrack_mass does not exist
    }

    void registerSubJets(const std::string& tag)
    {
        //AK8 jets
        const auto& FatJet_TLV        = tr_->getVec<TLorentzVector>("FatJetTLV");
        const auto& FatJet_subJetIdx1 = tr_->getVec<int>("FatJet_subJetIdx1");
        const auto& FatJet_subJetIdx2 = tr_->getVec<int>("FatJet_subJetIdx2");
        //AK8 subjets
        const auto& SubJet_TLV        = tr_->getVec<TLorentzVector>("SubJetTLV");
        
        //Corrrolate AK8 jets and their subjets
        unsigned int nFatJets = FatJet_TLV.size();
        unsigned int nSubJets = SubJet_TLV.size();
        auto& subjets = tr_->createDerivedVec<std::vector<TLorentzVector>>("SubJetTLV" + tag, nFatJets);
        for(int i = 0; i < nFatJets; ++i)
        {
            int subJetIdx1 = FatJet_subJetIdx1[i];
            int subJetIdx2 = FatJet_subJetIdx2[i];
            if(subJetIdx1 >= 0 && subJetIdx1 < nSubJets) subjets[i].push_back(SubJet_TLV[subJetIdx1]);
            if(subJetIdx2 >= 0 && subJetIdx2 < nSubJets) subjets[i].push_back(SubJet_TLV[subJetIdx2]);
        }
    }

    void registerTLV(const std::string& objectName)
    {
        // print statement for testing
        if (verbose_)   std::cout << "Calling registerTLV(" << objectName << ") to register " << objectName << "TLV" << std::endl;
        // new vector of TLorentzVector
        std::vector<TLorentzVector>* VectorTLV = new std::vector<TLorentzVector>();
        // get pt, eta, phi, mass
        const auto& vec_pt   = tr_->getVec<float>(objectName + "_pt");
        const auto& vec_eta  = tr_->getVec<float>(objectName + "_eta");
        const auto& vec_phi  = tr_->getVec<float>(objectName + "_phi");
        const auto& vec_mass = tr_->getVec<float>(objectName + "_mass");
        const int vec_size = vec_pt.size();
        if ( vec_eta.size() != vec_size || vec_phi.size() != vec_size || vec_mass.size() != vec_size )
        {
            std::cout << "ERROR: " << objectName << " component (pt, eta, phi, mass) vectors are not the same size." << std::endl;
            return;
        }
        // loop over objects
        for (int i = 0; i < vec_size; i++)
        {
            TLorentzVector TLV;
            // set pt, eta, phi, mass
            TLV.SetPtEtaPhiM(vec_pt[i], vec_eta[i], vec_phi[i], vec_mass[i]);
            // insert into final vector
            VectorTLV->push_back(TLV);
        }
        // register vector of TLorentzVector
        tr_->registerDerivedVec(objectName + "TLV", VectorTLV);
    }

public:
    bool verbose_; 

    GetVectors(){}

    ~GetVectors(){}
    
    void setVerbosity(bool v) { verbose_ = v; }

    void operator()(NTupleReader& tr)
    {
        // first set NTupleReader
        setReader(tr);
        setVerbosity(false);
        generateGetVectors();
    }
};

#endif
