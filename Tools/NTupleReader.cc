#include "NTupleReader.h"

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    nEvtTotal_ = tree_->GetEntries();
    nevt_ = 0;
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
    forVetoIsoTrksidx = genDecayStrVec = 0;
    muonsLVec = elesLVec = jetsLVec = genDecayLVec = 0;
    trksForIsoVetoLVec = loose_isoTrksLVec = 0;

    activateBranches();
}

void NTupleReader::activateBranches()
{
    tree_->SetBranchStatus("*", 0);

    // Add desired branches to branchMap_
    populateBranchList();

    for(std::map<std::string, void *>::const_iterator iMap = branchMap_.begin(); iMap != branchMap_.end(); ++iMap)
    {
        tree_->SetBranchStatus(iMap->first.c_str(), 1);
        tree_->SetBranchAddress(iMap->first.c_str(), iMap->second);
    }

//    tree_->LoadBaskets();
}

void NTupleReader::populateBranchList()
{
    branchMap_["run"]   = &run;
    branchMap_["event"] = &event;
    branchMap_["lumi"]  = &lumi;    
    branchMap_["mht"]  = &mht;
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


double NTupleReader::getTupleVar(const std::string var) const
{
    //This function can be used to return single variables or composite variables
/*    if(var.compare("run") == 0) return run;
    else if(var.compare("lumi") == 0) return lumi;
    else if(var.compare("event") == 0) return event;
    else if(var.compare("mht") == 0) return mht;
    else if(var.compare("mhtphi") == 0) return mhtphi;
    else if(var.compare("ht") == 0) return ht;
    else if(var.compare("met") == 0) return met;
    else if(var.compare("metphi") == 0) return metphi;
    else if(var.compare("dPhi0_CUT") == 0) return dPhi0_CUT;
    else if(var.compare("dPhi1_CUT") == 0) return dPhi1_CUT;
    else if(var.compare("dPhi2_CUT") == 0) return dPhi2_CUT;
    else if(var.compare("true_npv") == 0) return true_npv;
    else if(var.compare("avg_npv") == 0) return avg_npv;
    else if(var.compare("bestTopJetMass") == 0) return bestTopJetMass;
    else if(var.compare("MT2") == 0) return MT2;
    else if(var.compare("mTbestTopJet") == 0) return mTbestTopJet;
    else if(var.compare("mTbJet") == 0) return mTbJet;
    else if(var.compare("linearCombmTbJetPlusmTbestTopJet") == 0) return linearCombmTbJetPlusmTbestTopJet;
    else if(var.compare("mTbestWJet") == 0) return mTbestWJet;
    else if(var.compare("mTbestbJet") == 0) return mTbestbJet;
    else if(var.compare("mTremainingTopJet") == 0) return mTremainingTopJet;
    else if(var.compare("evtWeight") == 0) return evtWeight;
    else if(var.compare("nMuons_CUT") == 0) return nMuons_CUT;
    else if(var.compare("nMuons") == 0) return nMuons;
    else if(var.compare("nElectrons_CUT") == 0) return nElectrons_CUT;
    else if(var.compare("nElectrons") == 0) return nElectrons;
    else if(var.compare("nJets") == 0) return nJets;
    else if(var.compare("loose_nIsoTrks") == 0) return loose_nIsoTrks;
    else if(var.compare("nIsoTrks_CUT") == 0) return nIsoTrks_CUT;
    else if(var.compare("nJets_CUT") == 0) return nJets_CUT;
    else if(var.compare("vtxSize") == 0) return vtxSize;
    else if(var.compare("npv") == 0) return npv;
    else if(var.compare("nm1") == 0) return nm1;
    else if(var.compare("n0") == 0) return n0;
    else if(var.compare("np1") == 0) return np1;
    else if(var.compare("bestTopJetIdx") == 0) return bestTopJetIdx;
    else if(var.compare("pickedRemainingCombfatJetIdx") == 0) return pickedRemainingCombfatJetIdx;
    else if(var.compare("remainPassCSVS") == 0) return remainPassCSVS;*/
    
    std::map<std::string, void *>::const_iterator iter = branchMap_.find(var);
    if(iter != branchMap_.end())
    {
        return *static_cast<double*>(iter->second);
    }
    else
    {
        printf("NTupleReader::getTupleVar(std::string var): INvalid turple varialble!!!\n");
        return -999.0;
    }
}
