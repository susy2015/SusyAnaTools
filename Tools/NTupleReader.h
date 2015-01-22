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

//#include "

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

    int getEvtNum()
    {
        return nevt_;
    }

    int getNEntries()
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

    template<typename T> void registerDerivedVar(std::string name, void (*f)(const NTupleReader&, void*))
    {
        derivedMap_[name] = std::make_pair(f, new T());
    }

    double getTupleVar(const std::string var) const;
    inline double operator()(const std::string var) const {return getTupleVar(var);}
    template<typename T> static void setDerived(const T& retval, void* loc)
    {
        *static_cast<T*>(loc) = retval;
    }

private:
    // private variabls for internal use
    TTree *tree_;
    int nevt_, nEvtTotal_;
    bool isUpdateDisabled_;

    // Map to hold branch list 
    std::map<std::string, void *> branchMap_;
    std::map<std::string, std::pair<void (*)(const NTupleReader&, void*), void*>> derivedMap_;

    void activateBranches();
    void populateBranchList();
    
    template<typename T> void registerBranch(std::string name)
    {
        branchMap_[name] = new T();
    }

    void calculateDerivedVariables();

    void clearTuple();
    void updateTuple();
};

#endif
