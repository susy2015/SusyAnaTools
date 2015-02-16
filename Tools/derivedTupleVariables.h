#include "NTupleReader.h"
#include "customize.h"

#include "TH1.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

#include <vector>
#include <iostream>

namespace plotterFunctions
{
    //Ugly global variables here
    static TH1* muEffPt;
    static TH1* muAccPt;

    void generateWeight(NTupleReader& tr)
    {
        // Calculate PU weight

        // Calculate Z-eff weight
        
    }

    void muInfo(NTupleReader& tr)
    {
        const std::vector<int>& genDecayPdgIdVec        = tr.getVec<int>("genDecayPdgIdVec");
        const std::vector<TLorentzVector>& genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
        const std::vector<TLorentzVector>& muonsLVec    = tr.getVec<TLorentzVector>("muonsLVec");
        const std::vector<double>& muonsRelIso          = tr.getVec<double>("muonsRelIso");
        const std::vector<int>& W_emuVec                = tr.getVec<int>("W_emuVec");
        const std::vector<double>& muonsCharge          = tr.getVec<double>("muonsCharge");
        const std::vector<double>& muonsMtw             = tr.getVec<double>("muonsMtw");

        const double& met                            = tr.getVar<double>("met");
        const double& metphi                         = tr.getVar<double>("metphi");

        std::vector<const TLorentzVector*>* genMatchMuInAcc = new std::vector<const TLorentzVector*>();
        std::vector<const TLorentzVector*>* genMuInAcc = new std::vector<const TLorentzVector*>();
        std::vector<const TLorentzVector*>* genMu = new std::vector<const TLorentzVector*>();
        std::vector<TLorentzVector>* cutMuVec = new std::vector<TLorentzVector>();
        std::vector<double>* cutMuCharge = new std::vector<double>();

        int sumCharge = 0;
        for(int i = 0; i < muonsLVec.size(); ++i)
        {
            if(AnaFunctions::passMuon( muonsLVec[i], muonsRelIso[i], muonsMtw[i], AnaConsts::muonsArr))
            {
                cutMuVec->push_back(muonsLVec[i]);
                cutMuCharge->push_back(muonsCharge[i]);
                if(muonsCharge[i] > 0) sumCharge++;
                else                   sumCharge--;
            }
        }

        for(int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
        {
            if(abs(genDecayPdgIdVec[i]) == 13)
            {
                genMu->push_back(&genDecayLVec[i]);
                if(AnaFunctions::passMuonAccOnly(genDecayLVec[i], AnaConsts::muonsArr))
                {
                    genMuInAcc->push_back(&genDecayLVec[i]);
                    double dRMin = 999.9;
                    for(int j = 0; j < cutMuVec->size(); ++j)
                    {
                        double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], (*cutMuVec)[j]);
                        if(dR < dRMin)
                        {
                            dRMin = dR;
                        }
                    }
                    if(dRMin < 0.02)
                    {
                        genMatchMuInAcc->push_back(&genDecayLVec[i]);
                    }
                }
            }
        }

        double genZPt = -999.9, genZEta = -999.9, genZmass = -999.9;
        int nZ = 0;
        for(int j = 0; j <  genDecayPdgIdVec.size(); ++j)
        {
            if(abs(genDecayPdgIdVec[j]) == 23)
            {
                nZ++;
                genZPt = genDecayLVec[j].Pt();
                genZEta = genDecayLVec[j].Eta();
                genZmass = genDecayLVec[j].M();
                
            }
        }
        if(nZ > 1) std::cout << "!!!WARNING MORE THAN 1 Z FOUND!!!" << std::endl;

        int pdgIdZDec = 0;
        if(W_emuVec.size() == 0) pdgIdZDec = 15;
        else if(W_emuVec.size() == 2)
        {
            if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 11) pdgIdZDec = 11;
            else if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 13) pdgIdZDec = 13;
        }

        bool passMuZinvSel = true;
        if(cutMuVec->size() != 2) passMuZinvSel = false;
        if(sumCharge != 0)       passMuZinvSel = false;

        
        const double zMassMin = 71.0;
        const double zMass    = 91.0;
        const double zMassMax = 111.0;

        double zMassCurrent = 1.0e300, zEff = 1.0e100, zAcc = 1.0e100;
        TLorentzVector bestRecoZ;
        for(int i = 0; i < cutMuVec->size(); ++i)
        {
            if((*cutMuVec)[i].Pt() < 10) continue;
            for(int j = 0; j < i && j < cutMuVec->size(); ++j)
            {
                if((*cutMuVec)[j].Pt() < 10) continue;
                double zm = ((*cutMuVec)[i] + (*cutMuVec)[j]).M();
                if(/*zm > zMassMin && zm < zMassMax && */fabs(zm - zMass) < fabs(zMassCurrent - zMass))
                {
                    bestRecoZ = (*cutMuVec)[i] + (*cutMuVec)[j];
                    zMassCurrent = zm;
                    if(muEffPt)
                    {
                        //Get mu efficiencies
                        double muEff1 = 0.0, muEff2 = 0.0;
                        // Eff without applying acceptance
                        //const double p0 = 0.952453;
                        //const double p1 = -9.32974e-05;
                        // Eff with acceptance applied
                        const double p0 = 0.976727;
                        const double p1 = -0.000129567;
                        if((*cutMuVec)[i].Pt() > 400) muEff1 = p0 + (*cutMuVec)[i].Pt() * p1;
                        else                          muEff1 = muEffPt->GetBinContent(muEffPt->FindBin((*cutMuVec)[i].Pt()));

                        if((*cutMuVec)[j].Pt() > 400) muEff2 = p0 + (*cutMuVec)[j].Pt() * p1;
                        else                          muEff2 = muEffPt->GetBinContent(muEffPt->FindBin((*cutMuVec)[j].Pt()));

                        if(zEff < 0.1) 
                        {
                            std::cout << "WARNING: Z efficiency < 0.1, forcing weight to zero! Eff_Z: " << zEff << "\t Pt_mu1:" << (*cutMuVec)[i].Pt() <<  "\t Eff_mu1: " << muEff1 << "\t Pt_mu2: " << (*cutMuVec)[j].Pt() << "\t Eff_mu2: " << muEff2 << std::endl;
                            zEff = 1.0e101;
                        }

                        if(muEff1 < 1.0e-5 || muEff2 < 1.0e-5) zEff = 1.0e-10;
                        else                                   zEff = muEff1 * muEff2;

                        //Get mu acceptance
                        double muAcc1 = 0.0, muAcc2 = 0.0;

                        if((*cutMuVec)[i].Pt() > 600) muAcc1 = 1.0;
                        else                          muAcc1 = muAccPt->GetBinContent(muAccPt->FindBin((*cutMuVec)[i].Pt()));

                        if((*cutMuVec)[j].Pt() > 600) muAcc2 = 1.0;
                        else                          muAcc2 = muAccPt->GetBinContent(muAccPt->FindBin((*cutMuVec)[j].Pt()));
                

                        if(zAcc < 0.1) 
                        {
                            std::cout << "WARNING: Z efficiency < 0.1, forcing weight to zero! Eff_Z: " << zAcc << "\t Pt_mu1:" << (*cutMuVec)[i].Pt() <<  "\t Eff_mu1: " << muAcc1 << "\t Pt_mu2: " << (*cutMuVec)[j].Pt() << "\t Eff_mu2: " << muAcc2 << std::endl;
                            zAcc = 1.0e101;
                        }

                        if(muAcc1 < 1.0e-5 || muAcc2 < 1.0e-5) zAcc = 1.0e-10;
                        else                                   zAcc = muAcc1 * muAcc2;

                    }
                }
            }
        }

        TLorentzVector metV, metZ;
        metV.SetPtEtaPhiM(met, 0.0, metphi, 0.0);
        metZ.SetPtEtaPhiM(bestRecoZ.Pt(), 0.0, bestRecoZ.Phi(), 0.0);
        TLorentzVector cleanMet = metV + metZ;

        double bestRecoZPt = bestRecoZ.Pt();
        double cleanMetPt = cleanMet.Pt();
        tr.registerDerivedVar("bestRecoZPt", bestRecoZPt);
        tr.registerDerivedVar("bestRecoZM", bestRecoZ.M());
        tr.registerDerivedVar("cleanMetPt", cleanMetPt);
        tr.registerDerivedVar("cleanMetPhi", cleanMet.Phi());
        tr.registerDerivedVar("zEff", zEff);
        tr.registerDerivedVar("zEffWgt", 1.0/zEff);
        tr.registerDerivedVar("zAcc", zAcc);
        tr.registerDerivedVar("zAccWgt", 1.0/zAcc);

        tr.registerDerivedVec("cutMuVec", cutMuVec);
        tr.registerDerivedVec("genMu", genMu);
        tr.registerDerivedVec("genMuInAcc", genMuInAcc);
        tr.registerDerivedVec("genMatchMuInAcc", genMatchMuInAcc);
        tr.registerDerivedVar("genZPt", genZPt);
        tr.registerDerivedVar("genZEta", genZEta);
        tr.registerDerivedVar("genZmass", genZmass);
        tr.registerDerivedVar("pdgIdZDec", pdgIdZDec);
        tr.registerDerivedVar("passMuZinvSel", passMuZinvSel);
    }

    void cleanJets(NTupleReader& tr)
    {
        const std::vector<TLorentzVector>& jetsLVec  = tr.getVec<TLorentzVector>("jetsLVec");
        const std::vector<TLorentzVector>& elesLVec  = tr.getVec<TLorentzVector>("elesLVec");
        const std::vector<TLorentzVector>& muonsLVec = tr.getVec<TLorentzVector>("muonsLVec");
        const std::vector<double>& elesRelIso        = tr.getVec<double>("elesRelIso");
        const std::vector<double>& muonsRelIso       = tr.getVec<double>("muonsRelIso");
        const std::vector<double>& recoJetsBtag_0    = tr.getVec<double>("recoJetsBtag_0");

        if(elesLVec.size() != elesRelIso.size() || muonsLVec.size() != muonsRelIso.size())
        {
            std::cout << "MISMATCH IN VECTOR SIZE!!!!!" << std::endl;
            return;
        }

        std::vector<const TLorentzVector*>* cleanJetVec = new std::vector<const TLorentzVector*>();
        std::vector<TLorentzVector>* zinvJetVec = new std::vector<TLorentzVector>();
        std::vector<double>* zinvBTag = new std::vector<double>;

        const double jldRMax = 0.3;

        const double HT_jetPtMin = 50;
        const double HT_jetEtaMax = 2.4;
        const double MTH_jetPtMin = 30.0;

        double HT = 0.0;
        TLorentzVector MHT;

        int iJet = 0;
        for(auto& jet : jetsLVec)
        {
            double dRmin = 999.0;

            for(int i = 0; i < elesLVec.size() && i < elesRelIso.size(); ++i)
            {
                if(!AnaFunctions::passElectron(elesLVec[i], elesRelIso[i], 0.0, AnaConsts::elesArr)) continue;
                double dR = ROOT::Math::VectorUtil::DeltaR(jet, elesLVec[i]);
                dRmin = std::min(dRmin, dR);
            }

            for(int i = 0; i < muonsLVec.size() && i < muonsRelIso.size(); ++i)
            {
                if(!AnaFunctions::passMuon(muonsLVec[i], muonsRelIso[i], 0.0, AnaConsts::muonsArr)) continue;
                double dR = ROOT::Math::VectorUtil::DeltaR(jet, muonsLVec[i]);
                dRmin = std::min(dRmin, dR);
            }

            if(dRmin > jldRMax) 
            {
                cleanJetVec->push_back(&jet);
                if(AnaFunctions::jetPassCuts(jet, AnaConsts::pt30Arr))
                {
                    zinvJetVec->push_back(jet);
                    zinvBTag->push_back(recoJetsBtag_0[iJet]);
                }
                if(jet.Pt() > HT_jetPtMin && fabs(jet.Eta()) < HT_jetEtaMax) HT += jet.Pt();
                if(jet.Pt() > MTH_jetPtMin) MHT += jet;
            }
            ++iJet;
        }
        tr.registerDerivedVar("cleanHt", HT);
        tr.registerDerivedVar("cleanMHt", MHT.Pt());
        tr.registerDerivedVar("cleanMHtPhi", MHT.Phi());
        tr.registerDerivedVec("cleanJetVec", cleanJetVec);
        tr.registerDerivedVec("zinvJetVec", zinvJetVec);
        tr.registerDerivedVec("zinvBTagVec", zinvBTag);
    }

    void zinvBaseline(NTupleReader& tr)
    {
        const bool& passMuZinvSel = tr.getVar<bool>("passMuZinvSel");

        const double& cleanMetPt  = tr.getVar<double>("cleanMetPt");
        const double& cleanMetPhi = tr.getVar<double>("cleanMetPhi");

        const std::vector<TLorentzVector>& zinvJetVec  = tr.getVec<TLorentzVector>("zinvJetVec");
        const std::vector<double>&         zinvBTagVec = tr.getVec<double>("zinvBTagVec");

        TLorentzVector metLVec;
        metLVec.SetPtEtaPhiM(tr.getVar<double>("cleanMetPt"), 0.0, cleanMetPhi, 0.0);

        // Calculate number of jets and b-tagged jets
        int cntCSVS = AnaFunctions::countCSVS(zinvJetVec, zinvBTagVec, AnaConsts::cutCSVS, AnaConsts::bTagArr);
        int cntNJetsPt50Eta24 = AnaFunctions::countJets(zinvJetVec, AnaConsts::pt50Eta24Arr);
        int cntNJetsPt30Eta24 = AnaFunctions::countJets(zinvJetVec, AnaConsts::pt30Eta24Arr);
        int cntNJetsPt30      = AnaFunctions::countJets(zinvJetVec, AnaConsts::pt30Arr);

        // Recalculate deltaPhi
        std::vector<double> dPhiVec = AnaFunctions::calcDPhi(zinvJetVec, cleanMetPhi, 3, AnaConsts::dphiArr);

        // Calculate number of leptons
        //int nMuons = AnaFunctions::countMuons(tr.getVec<TLorentzVector>("muonsLVec"), tr.getVec<double>("muonsRelIso"), tr.getVec<double>("muonsMtw"), AnaConsts::muonsArr);
        int nElectrons = AnaFunctions::countElectrons(tr.getVec<TLorentzVector>("elesLVec"), tr.getVec<double>("elesRelIso"), tr.getVec<double>("elesMtw"), AnaConsts::elesArr);
        int nIsoTrks = AnaFunctions::countIsoTrks(tr.getVec<TLorentzVector>("loose_isoTrksLVec"), tr.getVec<double>("loose_isoTrks_iso"), tr.getVec<double>("loose_isoTrks_mtw"), AnaConsts::isoTrksArr);


        // Pass cuts

        bool passZinvdPhis = true;
        if( dPhiVec.at(0) < AnaConsts::dPhi0_CUT || dPhiVec.at(1) < AnaConsts::dPhi1_CUT || dPhiVec.at(2) < AnaConsts::dPhi2_CUT ) passZinvdPhis = false;

        bool passZinvnJets = true;
        if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ) passZinvnJets = false;
        if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ) passZinvnJets = false;

        bool passZinvBJets = true;
        if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) ) passZinvBJets = false;

        bool passZinvLeptVeto = true;
        if( nElectrons != AnaConsts::nElectronsSel ) passZinvLeptVeto = false;
        if( nIsoTrks != AnaConsts::nIsoTrksSel )     passZinvLeptVeto = false;

        bool passZinvMET = true;
        if( tr.getVar<double>("cleanMetPt") < AnaConsts::defaultMETcut ) passZinvMET = false;

        // Calculate top tagger related variables. 
        // Note that to save speed, only do the calculation after previous base line requirements.
        int bestTopJetIdx = -1;
        bool remainPassCSVS = false;
        int pickedRemainingCombfatJetIdx = -1;
        double bestTopJetMass = -1;
        int nTopCandSortedCnt = 0;
        double MT2 = -1;
        double mTcomb = -1;

        bool passPreTTag = passZinvLeptVeto && passZinvnJets && passZinvdPhis && passZinvBJets && passZinvMET && cntNJetsPt30 >= AnaConsts::nJetsSel;
        if(passPreTTag)
        {
            type3Ptr->processEvent(zinvJetVec, zinvBTagVec, metLVec);
            bestTopJetIdx = type3Ptr->bestTopJetIdx;
            remainPassCSVS = type3Ptr->remainPassCSVS;
            pickedRemainingCombfatJetIdx = type3Ptr->pickedRemainingCombfatJetIdx;
            if( bestTopJetIdx != -1 ) bestTopJetMass = type3Ptr->bestTopJetLVec.M();

            nTopCandSortedCnt = type3Ptr->nTopCandSortedCnt;
            MT2 = type3Ptr->MT2;
            mTcomb = type3Ptr->mTbJet + 0.5*type3Ptr->mTbestTopJet;
        }

        bool passZinvTagger = true;
        if( bestTopJetIdx == -1 )                                                                    passZinvTagger = false;
        if( ! remainPassCSVS )                                                                       passZinvTagger = false;
        if( pickedRemainingCombfatJetIdx == -1 && zinvJetVec.size()>=6 )                             passZinvTagger = false;
        if( ! (bestTopJetMass > AnaConsts::lowTopCut_ && bestTopJetMass < AnaConsts::highTopCut_ ) ) passZinvTagger = false;

        bool passZinvBaseline =  passZinvLeptVeto && passZinvnJets && passZinvdPhis && passZinvBJets && passZinvMET && passZinvTagger;
        bool passZinvBaselineNoTag = passZinvLeptVeto && passZinvnJets && passZinvdPhis && passZinvMET;

        tr.registerDerivedVar("bestTopJetIdxZinv", bestTopJetIdx);
        tr.registerDerivedVar("remainPassCSVSZinv", remainPassCSVS);
        tr.registerDerivedVar("pickedRemainingCombfatJetIdxZinv", pickedRemainingCombfatJetIdx);
        tr.registerDerivedVar("bestTopJetMassZinv", bestTopJetMass);

        tr.registerDerivedVar("passZinvMET", passZinvMET);
        tr.registerDerivedVar("passZinvLeptVeto", passZinvLeptVeto);
        tr.registerDerivedVar("passZinvnJets", passZinvnJets);
        tr.registerDerivedVar("passZinvdPhis", passZinvdPhis);
        tr.registerDerivedVar("passZinvBJets", passZinvBJets);
        tr.registerDerivedVar("passZinvTagger", passZinvTagger);
        tr.registerDerivedVar("passZinvBaseline", passZinvBaseline);
        tr.registerDerivedVar("passZinvBaselineNoTag", passZinvBaselineNoTag);

        tr.registerDerivedVar("cntNJetsPt30Eta24Zinv", cntNJetsPt30Eta24);
        tr.registerDerivedVar("cntCSVSZinv", cntCSVS);

        tr.registerDerivedVar("nTopCandSortedCntZinv", nTopCandSortedCnt);
        tr.registerDerivedVar("MT2Zinv", MT2);
        tr.registerDerivedVar("mTcombZinv", mTcomb);
    }

    void printInterestingEvents(NTupleReader& tr)
    {
        const unsigned int& run   = tr.getVar<unsigned int>("run");
        const unsigned int& event = tr.getVar<unsigned int>("event");

        const double& met                            = tr.getVar<double>("met");
        const double& metphi                         = tr.getVar<double>("metphi");

        const int& nMuons_CUT        = tr.getVar<int>("nMuons_CUT");
        const int& nElectrons_CUT    = tr.getVar<int>("nElectrons_CUT");
        const int& cntNJetsPt50Eta24 = tr.getVar<int>("cntNJetsPt50Eta24");
        const int& cntNJetsPt30Eta24 = tr.getVar<int>("cntNJetsPt30Eta24");
        const int& cntNJetsPt30      = tr.getVar<int>("cntNJetsPt30");

        const double& mht    = tr.getVar<double>("mht");
        const double& mhtphi = tr.getVar<double>("mhtphi");
        const double& ht     = tr.getVar<double>("ht");

        if(met > 1000) std::cout << "run: " << run << "\tevent: " << event << "\tmet: " << met << "\tmetphi: " << metphi << "\tnMuons_CUT: " << nMuons_CUT << "\t nElectrons_CUT: " << nElectrons_CUT << "\tcntNJetsPt30: " << cntNJetsPt30 << "\tcntNJetsPt30Eta24: " << cntNJetsPt30Eta24 << "\tcntNJetsPt50Eta24: " << cntNJetsPt50Eta24 << "\tmht: " << mht << "\tmhtphi: " << mhtphi << "\tht: " << ht << std::endl;
    }
    
    void registerFunctions(NTupleReader& tr)
    {
        //Make some global "constants" here
        TH1::AddDirectory(false);
        TFile *f = new TFile("muEffHists.root");
        if(f)
        {
            muEffPt = static_cast<TH1*>(f->Get("muEffPt"));
            muAccPt = static_cast<TH1*>(f->Get("muAccPt"));
            f->Close();
            delete f;
        }

        //register functions with NTupleReader
        tr.registerFunction(&cleanJets);
        tr.registerFunction(&generateWeight);
        tr.registerFunction(&muInfo);
        tr.registerFunction(&zinvBaseline);
    }

    void activateBranches(std::set<std::string>& activeBranches)
    {
        activeBranches.insert("genDecayPdgIdVec");
        activeBranches.insert("genDecayLVec");
        activeBranches.insert("muonsLVec");
        activeBranches.insert("muonsRelIso");
        activeBranches.insert("W_emuVec");
        activeBranches.insert("muonsCharge");
        activeBranches.insert("muonsMtw");
        activeBranches.insert("met");
        activeBranches.insert("metphi");
        activeBranches.insert("jetsLVec");
        activeBranches.insert("elesLVec");
        activeBranches.insert("elesRelIso");
        activeBranches.insert("recoJetsBtag_0");
        activeBranches.insert("loose_isoTrks_mtw");
        activeBranches.insert("elesMtw");
        activeBranches.insert("loose_isoTrks_iso");
        activeBranches.insert("loose_isoTrksLVec");
    }
}
