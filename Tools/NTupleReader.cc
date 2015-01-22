#include "NTupleReader.h"

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    nEvtTotal_ = tree_->GetEntries();
    nevt_ = 0;
    isUpdateDisabled_ = false;
    clearTuple();
    
    //vectors must be initialized to 0 here to avoid segfaults.  This cannot be done 
    //in clearTuple() as it will cause memory leaks.
    muonsCharge = muonsMtw = muonsRelIso = elesCharge = elesMtw = elesRelIso = 0;
    recoJetsBtag_0 = trksForIsoVeto_charge = trksForIsoVeto_dz = 0;
    loose_isoTrks_charge = loose_isoTrks_dz = 0;
    loose_isoTrks_iso = loose_isoTrks_mtw = 0;
    recoJetsFlavor = genDecayIdxVec = genDecayPdgIdVec = genDecayMomIdxVec = 0;
    W_emuVec = W_tau_emuVec = W_tau_prongsVec = trksForIsoVeto_pdgId = 0;
    trksForIsoVeto_idx = loose_isoTrks_pdgId = loose_isoTrks_idx = 0;
    forVetoIsoTrksidx = 0;
    genDecayStrVec = 0;
    muonsLVec = elesLVec = jetsLVec = genDecayLVec = 0;
    trksForIsoVetoLVec = loose_isoTrksLVec = 0;

    activateBranches();
}

void NTupleReader::activateBranches()
{
    tree_->SetBranchStatus("*", 0);

    // Add desired branches to branchMap_
    populateBranchList();

    for(auto& iMap : branchMap_)
    {
        tree_->SetBranchStatus(iMap.first.c_str(), 1);
        tree_->SetBranchAddress(iMap.first.c_str(), iMap.second);
    }
}

void NTupleReader::populateBranchList()
{
    branchMap_["run"]   = &run;
    branchMap_["event"] = &event;
    branchMap_["lumi"]  = &lumi;    
    //branchMap_["mht"]  = &mht;
    registerBranch<double>("mht");
    branchMap_["mhtphi"]  = &mhtphi;
    branchMap_["ht"]  = &ht;
    branchMap_["met"]  = &met;
    branchMap_["metphi"]  = &metphi;
    branchMap_["dPhi0_CUT"]  = &dPhi0_CUT;
    branchMap_["dPhi1_CUT"]  = &dPhi1_CUT;
    branchMap_["dPhi2_CUT"]  = &dPhi2_CUT;
    branchMap_["tru_npv"]  = &true_npv;
    branchMap_["avg_npv"]  = &avg_npv;
    branchMap_["bestTopJetMass"]  = &bestTopJetMass;
    branchMap_["MT2"]  = &MT2;
    branchMap_["mTbestTopJet"]  = &mTbestTopJet;
    branchMap_["mTbJet"]  = &mTbJet;
    branchMap_["linearCombmTbJetPlusmTbestTopJet"]  = &linearCombmTbJetPlusmTbestTopJet;
    branchMap_["mTbestWJet"]  = &mTbestWJet;
    branchMap_["mTbestbJet"]  = &mTbestbJet;
    branchMap_["mTremainingTopJet"]  = &mTremainingTopJet;
    branchMap_["evtWeight"]  = &evtWeight;
    branchMap_["nMuons_CUT"]  = &nMuons_CUT;
    branchMap_["nMuons"]  = &nMuons;
    branchMap_["nElectrons_CUT"]  = &nElectrons_CUT;
    branchMap_["nElectrons"]  = &nElectrons;
    branchMap_["nJets"]  = &nJets;
    branchMap_["loose_nIsoTrks"]  = &loose_nIsoTrks;
    branchMap_["nIsoTrks_CUT"]  = &nIsoTrks_CUT;
    branchMap_["nJets_CUT"]  = &nJets_CUT;
    branchMap_["vtxSize"]  = &vtxSize;
    branchMap_["npv"]  = &npv;
    branchMap_["nm1"]  = &nm1;
    branchMap_["n0"]  = &n0;
    branchMap_["np1"]  = &np1;
    branchMap_["bestTopJetIdx"]  = &bestTopJetIdx;
    branchMap_["pickedRemainingCombfatJetIdx"]  = &pickedRemainingCombfatJetIdx;
    branchMap_["remainPassCSVS"]  = &remainPassCSVS;
    branchMap_["muonsCharge"]  = &muonsCharge;
    branchMap_["muonsMtw"]  = &muonsMtw;
    branchMap_["muonsRelIso"]  = &muonsRelIso;
    branchMap_["elesCharge"]  = &elesCharge;
    branchMap_["elesMtw"]  = &elesMtw;
    branchMap_["elesRelIso"]  = &elesRelIso;
    branchMap_["elesMtw"]  = &elesMtw;
    branchMap_["elesRelIso"]  = &elesRelIso;
    branchMap_["recoJetsBtag_0"]  = &recoJetsBtag_0;
    branchMap_["trksForIsoVeto_charge"]  = &trksForIsoVeto_charge;
    branchMap_["trksForIsoVeto_dz"]  = &trksForIsoVeto_dz;
    branchMap_["loose_isoTrks_charge"]  = &loose_isoTrks_charge;
    branchMap_["loose_isoTrks_dz"]  = &loose_isoTrks_dz;
    branchMap_["loose_isoTrks_iso"]  = &loose_isoTrks_iso;
    branchMap_["loose_isoTrks_mtw"]  = &loose_isoTrks_mtw;
    branchMap_["recoJetsFlavor"]  = &recoJetsFlavor;
    branchMap_["genDecayIdxVec"]  = &genDecayIdxVec;
    branchMap_["genDecayPdgIdVec"]  = &genDecayPdgIdVec;
    branchMap_["genDecayMomIdxVec"]  = &genDecayMomIdxVec;
    branchMap_["W_emuVec"]  = &W_emuVec;
    branchMap_["W_tau_emuVec"]  = &W_tau_emuVec;
    branchMap_["W_tau_prongsVec"]  = &W_tau_prongsVec;
    branchMap_["trksForIsoVeto_pdgId"]  = &trksForIsoVeto_pdgId;
    branchMap_["trksForIsoVeto_idx"]  = &trksForIsoVeto_idx;
    branchMap_["loose_isoTrks_pdgId"]  = &loose_isoTrks_pdgId;
    branchMap_["loose_isoTrks_idx"]  = &loose_isoTrks_idx;
    branchMap_["forVetoIsoTrksidx"]  = &forVetoIsoTrksidx;
    branchMap_["genDecayStrVec"]  = &genDecayStrVec;
    branchMap_["muonsLVec"]  = &muonsLVec;
    branchMap_["elesLVec"]  = &elesLVec;
    branchMap_["jetsLVec"]  = &jetsLVec;
    branchMap_["genDecayLVec"]  = &genDecayLVec;
    branchMap_["trksForIsoVetoLVec"]  = &trksForIsoVetoLVec;
    branchMap_["loose_isoTrksLVec"]  = &loose_isoTrksLVec;
}

bool NTupleReader::getNextEvent()
{
    if(nevt_ >= nEvtTotal_)
    {
        return false;
    }
    //clearTuple();
    int status = tree_->GetEntry(nevt_);
    nevt_++;
    if(!isUpdateDisabled_) updateTuple();
    calculateDerivedVariables();
    return status > 0;
}

void NTupleReader::clearTuple()
{
    // Do not initialize vector pointers here!

    run = lumi = event = 0;
    mht = mhtphi = ht = met = metphi = dPhi0_CUT = dPhi1_CUT = dPhi2_CUT = true_npv = avg_npv = 0.0;
    bestTopJetMass = MT2 = mTbestTopJet = mTbJet = linearCombmTbJetPlusmTbestTopJet = 0.0;
    mTbestWJet = mTbestbJet = mTremainingTopJet = evtWeight = 0.0;
    nMuons_CUT = nMuons = nElectrons_CUT = nElectrons = nJets = loose_nIsoTrks = nIsoTrks_CUT = 0;
    nJets_CUT = vtxSize = npv = nm1 = n0 = np1 = bestTopJetIdx = pickedRemainingCombfatJetIdx = 0;
    remainPassCSVS = 0;
}

void NTupleReader::disableUpdate()
{
    isUpdateDisabled_ = true;
    printf("NTupleReader::disableUpdate(): You have disabled tuple updates.  You may therefore be using old variablre definitions.  Be sure you are ok with this!!!\n");
}

double NTupleReader::getTupleVar(const std::string var) const
{
    //This function can be used to return single variables or composite variables
    //if(var.compare("run") == 0) return run;
    //else if(var.compare("lumi") == 0) return lumi;

    // Catchall for default tuple variables.  
    std::map<std::string, void *>::const_iterator iter = branchMap_.find(var);
    if(iter != branchMap_.end())
    {
        return *static_cast<double*>(iter->second);
    }

    //get derived variables here
    auto derived_iter = derivedMap_.find(var);
    if(derived_iter != derivedMap_.end())
    {
        return *static_cast<double*>(derived_iter->second.second);
    }

    printf("NTupleReader::getTupleVar(const std::string var):  Variable not found: \"%s\"!!!\n", var.c_str());
}

void NTupleReader::calculateDerivedVariables()
{
    //My apoligies for anyone looking at this totally cryptic line of code
    for(auto& var : derivedMap_)
    {
        var.second.first(*this, var.second.second);
    }
}

void NTupleReader::updateTuple()
{
    
}
