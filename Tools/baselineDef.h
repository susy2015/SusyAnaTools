#ifndef ANABASELINE_BASELINEDEF_H
#define ANABASELINE_BASELINEDEF_H

#include "NTupleReader.h"
#include "customize.h"

#include <sstream>
#include <iostream>
#include <fstream>


void baselineUpdate(NTupleReader &tr)
{
    const bool debug = false;

// Form TLorentzVector of MET
    TLorentzVector metLVec; 
    metLVec.SetPtEtaPhiM(tr.getVar<double>("met"), 0, tr.getVar<double>("metphi"), 0);

// Calculate number of leptons
    int nMuons = AnaFunctions::countMuons(tr.getVec<TLorentzVector>("muonsLVec"), tr.getVec<double>("muonsRelIso"), tr.getVec<double>("muonsMtw"), AnaConsts::muonsArr);
    int nElectrons = AnaFunctions::countElectrons(tr.getVec<TLorentzVector>("elesLVec"), tr.getVec<double>("elesRelIso"), tr.getVec<double>("elesMtw"), AnaConsts::elesArr);
    int nIsoTrks = AnaFunctions::countIsoTrks(tr.getVec<TLorentzVector>("loose_isoTrksLVec"), tr.getVec<double>("loose_isoTrks_iso"), tr.getVec<double>("loose_isoTrks_mtw"), AnaConsts::isoTrksArr);

// Calculate number of jets and b-tagged jets
    const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");

    int cntCSVS = AnaFunctions::countCSVS(jetsLVec, tr.getVec<double>("recoJetsBtag_0"), AnaConsts::cutCSVS, AnaConsts::bTagArr);
    int cntNJetsPt50Eta24 = AnaFunctions::countJets(jetsLVec, AnaConsts::pt50Eta24Arr);
    int cntNJetsPt30Eta24 = AnaFunctions::countJets(jetsLVec, AnaConsts::pt30Eta24Arr);
    int cntNJetsPt30      = AnaFunctions::countJets(jetsLVec, AnaConsts::pt30Arr);

// Calculate deltaPhi
    std::vector<double> * dPhiVec = new std::vector<double>();
    (*dPhiVec) = AnaFunctions::calcDPhi(jetsLVec, tr.getVar<double>("metphi"), 3, AnaConsts::dphiArr);

// Prepare jets and b-tag working points for top tagger
    std::vector<TLorentzVector> *jetsLVec_forTagger = new std::vector<TLorentzVector>(); 
    std::vector<double> *recoJetsBtag_forTagger = new std::vector<double>();
    AnaFunctions::prepareJetsForTagger(jetsLVec, tr.getVec<double>("recoJetsBtag_0"), (*jetsLVec_forTagger), (*recoJetsBtag_forTagger));

    if( debug ) std::cout<<"\njetsLVec_forTagger->size : "<<jetsLVec_forTagger->size()<<"  recoJetsBtag_forTagger->size : "<<recoJetsBtag_forTagger->size()<<std::endl;

// Pass lepton veto?
    bool passLeptVeto = true;
    if( nMuons != AnaConsts::nMuonsSel )         passLeptVeto = false;
    if( nElectrons != AnaConsts::nElectronsSel ) passLeptVeto = false;
    if( nIsoTrks != AnaConsts::nIsoTrksSel )     passLeptVeto = false;

    if( debug ) std::cout<<"nMuons : "<<nMuons<<"  nElectrons : "<<nElectrons<<"  nIsoTrks : "<<nIsoTrks<<std::endl;

// Pass number of jets?
    bool passnJets = true;
    if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ) passnJets = false;
    if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ) passnJets = false;

    if( debug ) std::cout<<"cntNJetsPt50Eta24 : "<<cntNJetsPt50Eta24<<"  cntNJetsPt30Eta24 : "<<cntNJetsPt30Eta24<<"  cntNJetsPt30 : "<<cntNJetsPt30<<std::endl;

// Pass deltaPhi?
    bool passdPhis = true;
    if( dPhiVec->at(0) < AnaConsts::dPhi0_CUT || dPhiVec->at(1) < AnaConsts::dPhi1_CUT || dPhiVec->at(2) < AnaConsts::dPhi2_CUT ) passdPhis = false;

    if( debug ) std::cout<<"dPhi0 : "<<dPhiVec->at(0)<<"  dPhi1 : "<<dPhiVec->at(1)<<"  dPhi2 : "<<dPhiVec->at(2)<<std::endl;

// Pass number of b-tagged jets?
    bool passBJets = true;
    if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) ) passBJets = false;

    if( debug ) std::cout<<"cntCSVS : "<<cntCSVS<<std::endl;

// Pass the baseline MET requirement?
    bool passMET = true;
    if( tr.getVar<double>("met") < AnaConsts::defaultMETcut ) passMET = false;

    if( debug ) std::cout<<"met : "<<tr.getVar<double>("met")<<"  defaultMETcut : "<<AnaConsts::defaultMETcut<<std::endl;

// Calculate top tagger related variables. 
// Note that to save speed, only do the calculation after previous base line requirements.
    int bestTopJetIdx = -1;
    bool remainPassCSVS = false;
    int pickedRemainingCombfatJetIdx = -1;
    double bestTopJetMass = -1;
    int nTopCandSortedCnt = 0;
    double MT2 = -1;
    double mTcomb = -1;

//   if( cntNJetsPt30 >= AnaConsts::nJetsSel ){
    bool passPreTTag = passLeptVeto && passnJets && passdPhis && passBJets && passMET && cntNJetsPt30 >= AnaConsts::nJetsSel;
    if(passPreTTag)
    {
        type3Ptr->processEvent((*jetsLVec_forTagger), (*recoJetsBtag_forTagger), metLVec);
        bestTopJetIdx = type3Ptr->bestTopJetIdx;
        remainPassCSVS = type3Ptr->remainPassCSVS;
        pickedRemainingCombfatJetIdx = type3Ptr->pickedRemainingCombfatJetIdx;
        if( bestTopJetIdx != -1 ) bestTopJetMass = type3Ptr->bestTopJetLVec.M();

        nTopCandSortedCnt = type3Ptr->nTopCandSortedCnt;
        MT2 = type3Ptr->MT2;
        mTcomb = type3Ptr->mTbJet + 0.5*type3Ptr->mTbestTopJet;
    }

// Pass top tagger requirement?
    bool passTagger = true;
// bestTopJetIdx != -1 means at least 1 top candidate!
    if( bestTopJetIdx == -1 )                                                                    passTagger = false;
    if( ! remainPassCSVS )                                                                       passTagger = false;
    if( pickedRemainingCombfatJetIdx == -1 && jetsLVec_forTagger->size()>=6 )                    passTagger = false;
    if( ! (bestTopJetMass > AnaConsts::lowTopCut_ && bestTopJetMass < AnaConsts::highTopCut_ ) ) passTagger = false;

    bool passBaseline = passLeptVeto && passnJets && passdPhis && passBJets && passMET && passTagger;

    if( debug ) std::cout<<"bestTopJetidx : "<<bestTopJetIdx<<"  remainPassCSVS : "<<remainPassCSVS<<"  pickedRemainingCombfatJetIdx : "<<pickedRemainingCombfatJetIdx<<"  bestTopJetMass : "<<bestTopJetMass<<"  passBaseline : "<<passBaseline<<std::endl;

// Register all the calculated variables
    tr.updateTupleVar("nMuons_CUT", nMuons);
    tr.updateTupleVar("nElectrons_CUT", nElectrons);
    tr.updateTupleVar("nIsoTrks_CUT", nIsoTrks);

    tr.registerDerivedVar("cntNJetsPt50Eta24", cntNJetsPt50Eta24);
    tr.registerDerivedVar("cntNJetsPt30Eta24", cntNJetsPt30Eta24);

    tr.registerDerivedVec("dPhiVec", dPhiVec);

    tr.registerDerivedVar("cntCSVS", cntCSVS);

    tr.registerDerivedVec("jetsLVec_forTagger", jetsLVec_forTagger);
    tr.registerDerivedVec("recoJetsBtag_forTagger", recoJetsBtag_forTagger);

    tr.registerDerivedVar("cntNJetsPt30", cntNJetsPt30);

    tr.updateTupleVar("bestTopJetIdx", bestTopJetIdx);
    tr.updateTupleVar("remainPassCSVS", remainPassCSVS);
    tr.updateTupleVar("pickedRemainingCombfatJetIdx", pickedRemainingCombfatJetIdx);
    tr.updateTupleVar("bestTopJetMass", bestTopJetMass);

    tr.registerDerivedVar("passLeptVeto", passLeptVeto);
    tr.registerDerivedVar("passnJets", passnJets);
    tr.registerDerivedVar("passdPhis", passdPhis);
    tr.registerDerivedVar("passBJets", passBJets);
    tr.registerDerivedVar("passMET", passMET);
    tr.registerDerivedVar("passTagger", passTagger);
    tr.registerDerivedVar("passPreTTag", passPreTTag);
    tr.registerDerivedVar("passBaseline", passBaseline);

    if( debug ) std::cout<<"nTopCandSortedCnt : "<<nTopCandSortedCnt<<"  MT2 : "<<MT2<<"  mTcomb : "<<mTcomb<<"  passBaseline : "<<passBaseline<<std::endl;

    tr.updateTupleVar("nTopCandSortedCnt", nTopCandSortedCnt);
    tr.updateTupleVar("MT2", MT2);
    tr.updateTupleVar("mTcomb", mTcomb);

    if( debug ) std::cout<<"passBaseline : "<<passBaseline<<"  passBaseline : "<<passBaseline<<std::endl;
}

namespace stopFunctions
{
    void cleanJets(NTupleReader& tr)
    {
        const std::vector<TLorentzVector>& jetsLVec         = tr.getVec<TLorentzVector>("jetsLVec");
        const std::vector<TLorentzVector>& elesLVec         = tr.getVec<TLorentzVector>("elesLVec");
        const std::vector<TLorentzVector>& muonsLVec        = tr.getVec<TLorentzVector>("muonsLVec");
        const std::vector<double>&         elesRelIso       = tr.getVec<double>("elesRelIso");
        const std::vector<double>&         muonsMiniIso     = tr.getVec<double>("muonsMiniIso");
        const std::vector<double>&         recoJetsBtag_0   = tr.getVec<double>("recoJetsBtag_0");
        const std::vector<int>&            muMatchedJetIdx  = tr.getVec<int>("muMatchedJetIdx");
        const std::vector<int>&            eleMatchedJetIdx = tr.getVec<int>("eleMatchedJetIdx");

        if(elesLVec.size() != elesRelIso.size() 
           || elesLVec.size() != eleMatchedJetIdx.size()
           || muonsLVec.size() != muonsMiniIso.size()
           || muonsLVec.size() != muMatchedJetIdx.size()
           || jetsLVec.size() != recoJetsBtag_0.size())
        {
            std::cout << "MISMATCH IN VECTOR SIZE!!!!! Aborting jet cleaning algorithm!!!!!!" << std::endl;
            return;
        }

        std::vector<TLorentzVector>* cleanJetVec        = new std::vector<TLorentzVector>();
        std::vector<double>* cleanJetBTag               = new std::vector<double>;
        std::vector<TLorentzVector>* cleanJetpt30ArrVec = new std::vector<TLorentzVector>();
        std::vector<double>* cleanJetpt30ArrBTag        = new std::vector<double>;

        const double jldRMax = 0.15;

        const double HT_jetPtMin = 50;
        const double HT_jetEtaMax = 2.4;
        const double MTH_jetPtMin = 30.0;

        double HT = 0.0, HTNoIso = 0.0;
        TLorentzVector MHT;

        std::vector<bool> keepJetPFCandMatch(jetsLVec.size(), true);

        for(int iM = 0; iM < muonsLVec.size() && iM < muonsMiniIso.size() && iM < muMatchedJetIdx.size(); ++iM)
        {
            if(!AnaFunctions::passMuon(muonsLVec[iM], muonsMiniIso[iM], 0.0, AnaConsts::muonsMiniIsoArr)) continue;

            if(muMatchedJetIdx[iM] >= 0) keepJetPFCandMatch[muMatchedJetIdx[iM]] = false;
            else                         
            {
                //If muon matching to PF candidate has failed, use dR matching as fallback
                double dRmin = 999.0;
                int minJMatch = -1;
                
                for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
                {
                    if(!keepJetPFCandMatch[iJet]) continue;

                    double dR = ROOT::Math::VectorUtil::DeltaR(jetsLVec[iJet], muonsLVec[iM]);
                    if(dR < dRmin)
                    {
                        dRmin = dR;
                        minJMatch = iJet;
                    }
                }

                if(dRmin < jldRMax) keepJetPFCandMatch[minJMatch] = false;
            }

        }

        for(int iE = 0; iE < elesLVec.size() && iE < elesRelIso.size() && iE < eleMatchedJetIdx.size(); ++iE)
        {
            if(!AnaFunctions::passElectron(elesLVec[iE], elesRelIso[iE], 0.0, AnaConsts::elesArr)) continue;

            if(eleMatchedJetIdx[iE] >= 0) keepJetPFCandMatch[eleMatchedJetIdx[iE]] = false;
            else
            {
                //If electron matching to PF candidate has failed, use dR matching as fallback
                double dRmin = 999.0;
                int minJMatch = -1;

                for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
                {
                    if(!keepJetPFCandMatch[iJet]) continue;

                    double dR = ROOT::Math::VectorUtil::DeltaR(jetsLVec[iJet], elesLVec[iE]);
                    if(dR < dRmin)
                    {
                        dRmin = dR;
                        minJMatch = iJet;
                    }
                }

                if(dRmin < jldRMax) keepJetPFCandMatch[minJMatch] = false;
            }
        }

        int jetsKept = 0;
        for(int iJet = 0; iJet < jetsLVec.size(); ++iJet)
        {
            if(keepJetPFCandMatch[iJet])
            {
                ++jetsKept;
                cleanJetVec->push_back(jetsLVec[iJet]);
                cleanJetBTag->push_back(recoJetsBtag_0[iJet]);
                if(AnaFunctions::jetPassCuts(jetsLVec[iJet], AnaConsts::pt30Arr))
                {
                    cleanJetpt30ArrVec->push_back(jetsLVec[iJet]);
                    cleanJetpt30ArrBTag->push_back(recoJetsBtag_0[iJet]);
                }
                if(jetsLVec[iJet].Pt() > HT_jetPtMin && fabs(jetsLVec[iJet].Eta()) < HT_jetEtaMax) HT += jetsLVec[iJet].Pt();
                if(jetsLVec[iJet].Pt() > MTH_jetPtMin) MHT += jetsLVec[iJet];
            }
        }

        tr.registerDerivedVar("nJetsRemoved", static_cast<int>(jetsLVec.size() - jetsKept));
        tr.registerDerivedVar("cleanHt", HT);
        tr.registerDerivedVar("cleanMHt", MHT.Pt());
        tr.registerDerivedVar("cleanMHtPhi", MHT.Phi());
        tr.registerDerivedVec("cleanJetVec", cleanJetVec);
        tr.registerDerivedVec("cleanJetBTag", cleanJetBTag);
        tr.registerDerivedVec("cleanJetpt30ArrVec", cleanJetpt30ArrVec);
        tr.registerDerivedVec("cleanJetpt30ArrBTag", cleanJetpt30ArrBTag);
    }
}

#endif
