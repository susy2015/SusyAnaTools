#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <vector>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<vector<int> > >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<string>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#endif


/* This class is designed to be a simple interface to reading stop NTuples
   
   To use this class simply open the desired Tree as a TTree or TChain and pass it 
   to the constructor.  Then the tuple contents may be accessed as follows

   NTupleReader tr(tree);
   while(tr.getNextEvent())
   {
       int run = tr.run;
   }
   
   
   To add a variable to the reader it should be declared in the public section of the class definition,
   and added to the branchMap_ map in the populateBranchList().  Vectors they must be declared
   as pointers and initialized to zero in the constructor but not in clearTuple().  For simple variables 
   they shoud be initialized to an approperiate value in clearTuple().  
 */

class NTupleReader
{

public:
    // List of all variables used in tuple
    unsigned int run, lumi, event;
    double mht, mhtphi, ht, met, metphi, dPhi0_CUT, dPhi1_CUT, dPhi2_CUT, true_npv, avg_npv;
    double bestTopJetMass, MT2, mTbestTopJet, mTbJet, linearCombmTbJetPlusmTbestTopJet;
    double mTbestWJet, mTbestbJet, mTremainingTopJet, evtWeight;
    int nMuons_CUT, nMuons, nElectrons_CUT, nElectrons, nJets, loose_nIsoTrks, nIsoTrks_CUT;
    int nJets_CUT, vtxSize, npv, nm1, n0, np1, bestTopJetIdx, pickedRemainingCombfatJetIdx;
    unsigned int remainPassCSVS;
    std::vector<double> *muonsCharge, *muonsMtw, *muonsRelIso, *elesCharge, *elesMtw, *elesRelIso;
    std::vector<double> *recoJetsBtag_0, *trksForIsoVeto_charge, *trksForIsoVeto_dz;
    std::vector<double> *loose_isoTrks_charge, *loose_isoTrks_dz;
    std::vector<double> *loose_isoTrks_iso, *loose_isoTrks_mtw;
    std::vector<int> *recoJetsFlavor, *genDecayIdxVec, *genDecayPdgIdVec, *genDecayMomIdxVec;
    std::vector<int> *W_emuVec, *W_tau_emuVec, *W_tau_prongsVec, *trksForIsoVeto_pdgId;
    std::vector<int> *trksForIsoVeto_idx, *loose_isoTrks_pdgId, *loose_isoTrks_idx;
    std::vector<int> *forVetoIsoTrksidx;
    std::vector<std::string> *genDecayStrVec;
    std::vector<TLorentzVector> *muonsLVec, *elesLVec, *jetsLVec, *genDecayLVec;
    std::vector<TLorentzVector> *trksForIsoVetoLVec, *loose_isoTrksLVec;

    NTupleReader(TTree * tree);

    int getEvtNum() const
    {
        return nevt_;
    }

    int getNEntries() const
    {
        if(tree_) return nEvtTotal_;
        else 
        {
            printf("NTupleReader::getNEntries() - NO tree defined yet!!!\n");
            return -1;
        }
    }

    bool getNextEvent();
    void disableUpdate();

    template<typename T> void registerDerivedVar(const std::string name, void (*f)(const NTupleReader&, void* const))
    {
        if(branchMap_.find(name) != branchMap_.end())
        {
            printf("NTupleReader::registerDerivedVar(...): You are trying to redefine a base tuple var: \"%s\".  This is not allowed!  Please choose a unique name.\n", name.c_str());
            return;
        }
        derivedMap_[name] = std::make_pair(f, new T());
    }

    template<typename T> void registerDerivedVec(const std::string name, void (*f)(const NTupleReader&, void* const))
    {
        if(branchVecMap_.find(name) != branchVecMap_.end())
        {
            printf("NTupleReader::registerDerivedVec(...): You are trying to redefine a base tuple vec: \"%s\".  This is not allowed!  Please choose a unique name.\n", name.c_str());
            return;
        }
        derivedVecMap_[name] = std::make_pair(f, new std::vector<T>*());
    }

    template<typename T = double> T getVar(const std::string var) const
    {
        //This function can be used to return single variables

        return getTupleObj<T>(var, branchMap_, derivedMap_);
    }

    template<typename T> const std::vector<T>& getVec(const std::string var) const
    {
        //This function can be used to return vectors

        return *getTupleObj<std::vector<T>*>(var, branchVecMap_, derivedVecMap_);
    }

    inline double operator()(const std::string var) const {return getVar(var);}

    template<typename T> inline static void setDerived(const T& retval, void* const loc)
    {
        *static_cast<T*>(loc) = retval;
    }

private:
    // private variables for internal use
    TTree *tree_;
    int nevt_, nEvtTotal_;
    bool isUpdateDisabled_;

    // Maps to hold branch list 
    std::map<std::string, void *> branchMap_;
    std::map<std::string, void *> branchVecMap_;
    std::map<std::string, std::pair<void (*)(const NTupleReader&, void*), void*>> derivedMap_;
    std::map<std::string, std::pair<void (*)(const NTupleReader&, void*), void*>> derivedVecMap_;

    void activateBranches();
    void populateBranchList();
    
    void calculateDerivedVariables();

    void clearTuple();
    void updateTuple();

    template<typename T> void registerBranch(std::string name)
    {
        branchMap_[name] = new T();
    }
    
    template<typename T> void registerVecBranch(std::string name)
    {
        branchVecMap_[name] = new std::vector<T>*();
    }
    
    template<typename T, typename V1, typename V2> T getTupleObj(const std::string var, V1 v_tuple, V2 v_derived) const
    {
        // Check for default tuple variables first.
        auto tuple_iter = v_tuple.find(var);
        if(tuple_iter != v_tuple.end())
        {
            return *static_cast<T*>(tuple_iter->second);
        }

        //get derived variables here
        auto derived_iter = v_derived.find(var);
        if(derived_iter != v_derived.end())
        {
            return *static_cast<T*>(derived_iter->second.second);
        }

        printf("NTupleReader::getTupleObj(const std::string var):  Variable not found: \"%s\"!!!\n", var.c_str());
        return T(0);
    }

};

#endif
