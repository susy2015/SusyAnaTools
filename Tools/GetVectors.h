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
        // register vector of TLorentzVectors
        registerTLV("Jet");       // AK4 jets
        registerTLV("FatJet");    // AK8 jets 
        registerTLV("SubJet");    // AK8 subjets
        registerTLV("GenJet");    // generated AK4 jets
        registerTLV("GenJetAK8"); // generated AK8 jets
        registerTLV("Electron");  // electrons
        registerTLV("Muon");      // muons
        registerTLV("Photon");    // photons
    }

    void registerTLV(const std::string& objectName)
    {
        // print statement for testing
        std::cout << "registerTLV(" << objectName << ")" << std::endl;
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

    GetVectors(){}

    ~GetVectors(){}

    void operator()(NTupleReader& tr)
    {
        // first set NTupleReader
        setReader(tr);
        generateGetVectors();
    }
};

#endif
