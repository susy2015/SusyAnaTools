#include "NTupleReader.h"
#include "baselineDef.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include "TH1F.h"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include "customize.h"
#include "ZInvisible/Tools/include/BasicLepton.h"
#include "ZInvisible/Tools/include/LepInfo.h"
#include "GetVectors.h"
#include "CleanedJets.h"
#include "RunTopTagger.h"
#include "ZInvisible/Tools/include/Gamma.h"
////#include "TopTagger/TopTagger/interface/TopTagger.h"
////#include "TopTagger/TopTagger/interface/TTModule.h"
////#include "TopTagger/TopTagger/interface/TopTaggerUtilities.h"
//#include "TopTagger/TopTagger/interface/TopTaggerResults.h"
////#include "TopTagger/Tools/cpp/PlotUtility.h"
//#include "TopTagger/TopTagger/interface/TopObject.h"
#include <getopt.h>

int main(int argc, char* argv[])
{
    //char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_10Feb2019_v2p7/Data_SingleMuon_2016_PeriodB/Data_SingleMuon_2016_PeriodB_0.root";
    //char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_22Feb2019_v2p7/TTbar_HT_600to800_2016/TTbar_HT_600to800_2016_0.root";
    //char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Autumn18_102X_v1/PostProcessed_22March2019_v2p9/TTbar_HT_600to800_2018/TTbar_HT_600to800_2018_0.root";
    //const char *outputfile = "test_output.root";

    //Default Configuration
    //std::string era = "2016";
    //std::string era = "2017";
    std::string era = "2018";
    bool Data = false;
    bool PostHEM = false;
    bool PeriodF = false;
    int max_events = -1;
    //int max_events = 30000; //Set to -1 to run over all events
    bool multiMuTriggEff = true;
    bool leadingMuPt = false;
    bool norm_cuts = true;
    bool loose_cuts = false;
    bool dryrun = false;
    bool noTrigEff = false;
    bool leptoncleaned = true;
    //Even if false, there is a var h_norm_notrigwt

    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *outputfile   = argv[2];
    
    //Argument testing
    std::cout << argv[1] << " " << argv[2] << std::endl;

    int c;
    while (1)
    {
        //int option_index = 3;
        static struct option long_options[] = {
            {"era",         required_argument,  0,  'e'},
            {"isData",      no_argument,        0,  'D'},
            {"PostHEM",     no_argument,        0,  'p'},
            {"PeriodF",     no_argument,        0,  'f'},
            {"max_events",  required_argument,  0,  'm'},
            {"loose_cuts",  no_argument,        0,  'l'},
            {"dryrun",      no_argument,        0,  'd'},
            {"noTrigEff",   no_argument,        0,  't'}
        };
        //c = getopt_long(argc,argv,"e:Dpm:ld",long_options,&option_index);
        c = getopt_long(argc,argv,"e:Dpfm:ldt",long_options,NULL);
        if(c==-1) break;
        switch (c)
        {
            case 'e':
                era = optarg;
                break;
            case 'D':
                Data = true;
                break;
            case 'p':
                PostHEM = true;
                break;
            case 'f':
                PeriodF = true;
                break;
            case 'm':
                max_events = std::stoi(optarg);
                break;
            case 'l':
                loose_cuts = true;
                break;
            case 'd':
                dryrun = true;
                break;
            case 't':
                noTrigEff = true;
                break;
        }
    }
    //Argument testing
    std::cout << "Era: " << era << "\tisData: " << Data << "\tisPostHEM: " << PostHEM << "\tPeriodF: " << PeriodF << "\tMax events: " << max_events << "\tloose_cuts: " << loose_cuts << "\tnoTrigEff: " << noTrigEff << std::endl;
    float leptonpts[3] = {0,20,20};
    if(norm_cuts) leptonpts[0] = 40;
    if(loose_cuts) leptonpts[0] = 25;
    std::string suffix = "";
    if(leptoncleaned) suffix = "_drLeptonCleaned";
    std::cout << "Lepton pts: " << leptonpts[0] << leptonpts[1] << leptonpts[2] << std::endl;
    
    if (dryrun) return 0;
    int temp = 0;

    TChain *ch = new TChain("Events");
    
    std::string filename;
    std::ifstream bigfile;

    bigfile.open (inputfilelist);

    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            ch->Add(filename.c_str());
        }
        bigfile.close();
    }
 
    NTupleReader tr(ch);
    //BaselineVessel blv(tr,"","fastsim");
    GetVectors getVectors;
    plotterFunctions::BasicLepton basicLepton;
    plotterFunctions::Gamma gamma(era);
    CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    RunTopTagger runTopTagger_drLeptonCleaned("TopTagger.cfg","_drLeptonCleaned",true,false);
    BaselineVessel blv(tr,era,"");
    BaselineVessel blv_drLeptonCleaned(tr, era, "_drLeptonCleaned");
    plotterFunctions::LepInfo lepInfo(era);
    
    tr.registerFunction(getVectors);
    tr.registerFunction(gamma);
    tr.registerFunction(basicLepton);
    tr.registerFunction(cleanedJets);
    tr.registerFunction(runTopTagger);
    tr.registerFunction(runTopTagger_drLeptonCleaned);
    tr.registerFunction(blv);
    tr.registerFunction(lepInfo);
    tr.registerFunction(blv_drLeptonCleaned);

    auto *h_recoZpt = new TH1F("h_recoZpt","Reconstructed Z pT;pT;Events",60,0,600);
    auto *h_recoZM = new TH1F("h_recoZM","Reconstructed Z mass;Mass;Events",60,80,110);
    auto *h_recoZM_trimmed = new TH1F("h_recoZM_trimmed","Reconstructed Z mass;Mass;Events",60,80,110); //Looking into oddities in 2018 PreHEM
    auto *eff_h = new TH1F("eff_h","eff_h",2,0,2); //Counting events. First bin counts all, second counts those that pass baseline.
    auto *cutflow_h = new TH1F("cutflow_h","Cutflow",8,0,8);
    auto *h_met = new TH1F("h_met","MET",50,0,1000);
    auto *h_ht = new TH1F("h_ht","HT",100,0,2000);
    auto *h_njet = new TH1F("h_njet","nJet",15,0,15);
    auto *h_njetpt20eta24 = new TH1F("h_njetpt20eta24","Jets with pt > 20, eta < 2.4",15,0,15);
    auto *h_njetpt30eta24 = new TH1F("h_njetpt30eta24","Jets with pt > 30, eta < 2.4",15,0,15);
    auto *h_njetpt40eta24 = new TH1F("h_njetpt40eta24","Jets with pt > 40, eta < 2.4",15,0,15);
    auto *h_njetpt40 = new TH1F("h_njetpt40", "Jets with pt > 40", 15,0,15);
    //auto *h_Stop0l_nJets = new TH1F("h_Stop0l_nJets","Jets with pT > 20 GeV, eta < 2.4",15,0,15);
    auto *h_lep1pT = new TH1F("h_lep1pT","pT of lepton 1",60,0,600);
    auto *h_lep2pT = new TH1F("h_lep2pT","pT of lepton 2",60,0,600);
    auto *h_lep3pT = new TH1F("h_lep3pT","pT of lepton 3",60,0,600);
    auto *h_3mu_lep1pT = new TH1F("h_3mu_lep1pT","pT of lepton 1 in 3 muon section",60,0,600);
    auto *h_3mu_lep2pT = new TH1F("h_3mu_lep2pT","pT of lepton 2 in 3 muon section",60,0,600);
    auto *h_3mu_lep3pT = new TH1F("h_3mu_lep3pT","pT of lepton 3 in 3 muon section",60,0,600);
    auto *h_3mu_lep1eta = new TH1F("h_3mu_lep1eta","eta of lepton 1 in 3 muon section",60,-3,3);
    auto *h_3mu_lep2eta = new TH1F("h_3mu_lep2eta","eta of lepton 2 in 3 muon section",60,-3,3);
    auto *h_3mu_lep3eta = new TH1F("h_3mu_lep3eta","eta of lepton 3 in 3 muon section",60,-3,3);
    auto *h_num_mu = new TH1F("h_num_mu","Number of muons",3,1,4);
    auto *h_num_mu_notrigwt = new TH1F("h_num_mu_notrigwt","Number of muons: no trigger weights",3,1,4);
    auto *h_num_mu_trimmed = new TH1F("h_num_mu_trimmed","Number of muons (trimmed bins)",3,1,4);
    auto *h_num_mu_pt_eff = new TH1F("h_num_mu_pt_eff","Number of muons (pt eff)",3,1,4);
    auto *h_num_mu_eta_eff = new TH1F("h_num_mu_eta_eff","Number of muons (eta eff)",3,1,4);
    auto *h_num_elec = new TH1F("h_num_elec","Number of electrons",3,0,3);
    auto *h_num_elec_notrigwt = new TH1F("h_num_elec_notrigwt","Number of electrons: no trigger weights",3,0,3);
    auto *h_mu_indices = new TH1F("h_mu_indices", "Frequency of pT-ordered muon index used to reconstruct Z", 4,-1,3);
    auto *h_normalization = new TH1F("h_normalization","TTZ Normalization Exercise",4,61,141);
    auto *h_norm_notrigwt = new TH1F("h_norm_notrigwt","TTZ Normalization: No Trigger Weight",4,61,141);
    auto *h_norm_trimmed = new TH1F("h_norm_trimmed","TTZ Normalization Exercise",4,61,141); //Looking into oddities in 2018 PreHEM
    auto *h_norm_pt_eff = new TH1F("h_norm_pt_eff","TTZ Normalization Exercise",4,61,141);
    auto *h_norm_eta_eff = new TH1F("h_norm_eta_eff","TTZ Normalization Exercise",4,61,141);
    auto *h_norm_1mu = new TH1F("h_norm_1mu","TTZ Normalization Exercise: 1 muon",4,61,141);
    auto *h_norm_2mu = new TH1F("h_norm_2mu","TTZ Normalization Exercise: 2 muons",4,61,141);
    auto *h_norm_3mu = new TH1F("h_norm_3mu","TTZ Normalization Exercise: 3 muons",4,61,141);
    //auto *h_normalization = new TH1F("h_normalization","TTZ Normalization Exercise",4,60,144);
    
    while(tr.getNextEvent())
    {
        if(tr.getEvtNum() == 1)
        {
            tr.printTupleMembers();
            FILE *fout = fopen("NTupleTypes.txt","w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }
        if(tr.getEvtNum() % 1000 == 0)
        {
           std::cout << "Event Number " << tr.getEvtNum() << std::endl; 
        }

        if(max_events != -1 && tr.getEvtNum() > max_events) break;

        //Count whether event is positive or negative (MC)
        float Stop0l_evtWeight = 1.0;
        float sign = 1.0;
        if(!Data)
        {
            Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
            if(Stop0l_evtWeight < 0) sign = -1.0;
        }

        //Get number of events
        eff_h->Fill(0.,sign);
        cutflow_h->Fill(0);
        cutflow_h->GetXaxis()->SetBinLabel(1,"Total");
        cutflow_h->GetXaxis()->SetBinLabel(2,"Single Muon");
        cutflow_h->GetXaxis()->SetBinLabel(3,">= 4 Jets");
    	cutflow_h->GetXaxis()->SetBinLabel(4,"3 Leptons");
        cutflow_h->GetXaxis()->SetBinLabel(5,"DiLep");
    	cutflow_h->GetXaxis()->SetBinLabel(6,">=2 bjets");
        cutflow_h->GetXaxis()->SetBinLabel(7,"MET > 30");
        cutflow_h->GetXaxis()->SetBinLabel(8,"Z Mass Peak");

        //HEM Veto
        //eta[-3, -1.4], phi[-1.57, -0.87], pt > 20
        if(PostHEM && (era == "2018"))
        {
            const auto& SAT_Pass_HEMVeto = tr.getVar<bool>("SAT_Pass_HEMVeto" + suffix);
            if(!SAT_Pass_HEMVeto) continue;
        }

        //Event Filter
        bool SAT_Pass_EventFilter = tr.getVar<bool>("SAT_Pass_EventFilter" + suffix); //drLeptonCleaned?
        if(!SAT_Pass_EventFilter) continue;

        //const auto& Jet_btag = tr.getVec<float>("Jet_btagCSVV2");
        const auto& Jet_btag = tr.getVec<float>("Jet_btagDeepB" + suffix);
        //Choice of: Jet_btagStop0l Jet_btagDeepB Jet_btagCSVV2
        //const auto& Jet_btag = tr.getVec<float>("Jet_btagDeepB");
        const auto& Jet_LV = tr.getVec<TLorentzVector>("JetTLV" + suffix);
        const auto& Jet_pt = tr.getVec<float>("Jet_pt" + suffix);
        const auto& Jet_eta = tr.getVec<float>("Jet_eta" + suffix);

        //NanoSUSY-tools uses DeepB for Stop0l_nbtags. Found in customize.h.
        std::map<std::string,float> DeepCSVMediumWP ={
            {"2016" , 0.6321}, //now 0.6321, was 0.6324
            {"2017" , 0.4941},
            {"2018" , 0.4184}
        };
        
        std::map<std::string,float> CSVv2MediumWP = {
            {"2016" , 0.8484},
            {"2017" , 0.8838},
            {"2018" , 0.8838} // Not recommended, use 2017 as temp
        };
        if(tr.getEvtNum() == 1) std::cout << "Era: " << era << "\nbtag WP: " << DeepCSVMediumWP[era] << std::endl;

        int nbtags = 0;
        for(unsigned int i = 0; i < Jet_btag.size(); i++)
        {
            if(Jet_btag[i] >= DeepCSVMediumWP[era] && Jet_pt[i] > 30 && abs(Jet_eta[i]) < 2.4) nbtags++;
        }
        //Sanity check:
        //if(tr.getEvtNum() < 50) std::cout << "Size of Jet_btag: " << Jet_btag.size() << "\tSize of Jet_pt: " << Jet_pt.size() << std::endl;
        if(Jet_btag.size() != Jet_pt.size()) std::cout << "Warning: Jet_btag.size != Jet_pt.size() in event " << tr.getEvtNum() << "." << std::endl;
        //const auto& Muon_pt = tr.getVec<float>("Muon_pt");
        //const auto& Muon_LV = tr.getVec_LVFromNano<float>("Muon");

        //const auto& Elec_pt = tr.getVec<float>("Electron_pt");
        //const auto& Elec_LV = tr.getVec_LVFromNano<float>("Electron");

        //Pass nJets
        //const auto& Stop0l_nJets = tr.getVar<int>("Stop0l_nJets");
        //const auto& nJet = tr.getVar<unsigned int>("nJet");
        const auto& nJet = Jet_pt.size();
        //const auto& Pass_NJets20 = tr.getVar<bool>("Pass_NJets20");
        //const auto& Stop0l_nTop = tr.getVar<int>("Stop0l_nTop");
        const auto& Stop0l_nbtags = tr.getVar<int>("Stop0l_nbtags");

        //Using lepton-cleaned jets (from customize.cc):

        //Defining leptons in BasicLepton.h:

        const auto& cutMuVec = tr.getVec<TLorentzVector>("cutMuVec");
        const auto& cutElecVec = tr.getVec<TLorentzVector>("cutElecVec");
        std::vector<float> cutMuSF(cutMuVec.size());
        std::vector<float> cutElecSF(cutElecVec.size());
        double muTrigger = 50.0;
        if (loose_cuts) muTrigger =  25.0;
        if (temp == 0)
        {
            std::cout << "muTrigger: " << muTrigger << std::endl;
            temp++;
        }
        
        //const double elecTrigger=  40.0;
        if(!Data)
        {
            cutMuSF   = tr.getVec<float>("cutMuSF");
            cutElecSF = tr.getVec<float>("cutElecSF");
        }
        else
        {
            //std::vector<float> cutMuSF(cutMuVec.size());
            for(int i=0; i < cutMuSF.size(); i++)
            {
                cutMuSF[i] = 1.0;
            }
            //std::vector<float> cutElecSF(cutElecVec.size());
            for(int i=0; i < cutElecSF.size(); i++)
            {
                cutElecSF[i] = 1.0;
            }
        }
        
        //Single Muon cut (or DiMu for loose_cuts)
        //if(!muPassTrigger) continue;
        if(cutMuVec.size() < 1) continue;
        if(loose_cuts && cutMuVec.size() < 2) continue;
        if(leadingMuPt && !Data)
        {
            if(cutMuVec[0].Pt() < muTrigger) continue;
        }
        if (Data)
        {
            const auto& Pass_trigger_muon = tr.getVar<bool>("Pass_trigger_muon"); //Efficiencies calc'd by Hui, grabbed for weight below.
            //const auto& passDiMuTrigger = tr.getVar<bool>("passDiMuTrigger");
            if(!Pass_trigger_muon && !loose_cuts) continue;
            //if(!passDiMuTrigger && loose_cuts) continue;
        }
        cutflow_h->Fill(1);
        //cutflow_h->GetXaxis()->SetBinLabel(4,"Single Muon");
    	
        //Pass nJets
        //if(Stop0l_nJets < 4) continue;
        if(Jet_pt.size() < 4) continue;
        int jetcount = 0;
        for(unsigned int i = 0; i < Jet_pt.size(); i++)
        {
            if(Jet_pt[i] > 40 && abs(Jet_eta[i]) < 2.4) jetcount++;
        }
        if(jetcount < 4) continue;
        //if(Jet_pt[3] < 40 || Jet_pt[2] < 40 || Jet_pt[1] < 40 || Jet_pt[0] < 40) continue; //Requires >=4 jets with pT >= 40 GeV.
        //if(abs(Jet_eta[3]) >= 2.4 || abs(Jet_eta[2]) >= 2.4 || abs(Jet_eta[1]) >= 2.4 || abs(Jet_eta[0]) >= 2.4) continue;
        cutflow_h->Fill(2);
        //cutflow_h->GetXaxis()->SetBinLabel(2,">= 4 Jets");

        //Pass nLeptons == 3 and two above min pt for recoZ
        if(cutMuVec.size() + cutElecVec.size() != 3) continue;
        float cutMuPt1   = tr.getVar<float>("cutMuPt1");
        float cutMuPt2   = tr.getVar<float>("cutMuPt2");
        std::vector<int> bestRecoMuZindices (2,-1); //Two ints of value -1
        const auto& cutElecPt1 = tr.getVar<float>("cutElecPt1");
        const auto& cutElecPt2 = tr.getVar<float>("cutElecPt2");
        const auto& cutMuCharge = tr.getVec<int>("cutMuCharge");
        const auto& cutElecCharge = tr.getVec<int>("cutElecCharge");
        int summedMuCharge = 5; //out-of-bounds val
        if(cutMuVec.size() == 2) summedMuCharge = cutMuCharge[0] + cutMuCharge[1];
        if(cutMuVec.size() == 3) //With three muons, the Z might not be reconstructed from the leading two.
        {
            bestRecoMuZindices = tr.getVec<int>("bestRecoMuZindices");
            //std::cout << "Indices: " << bestRecoMuZindices[0] << " " << bestRecoMuZindices[1] << std::endl;
            summedMuCharge = cutMuCharge[bestRecoMuZindices[0]] + cutMuCharge[bestRecoMuZindices[1]];
            //std::cout << "Charge:  " << summedMuCharge << std::endl;
            cutMuPt1 = cutMuVec[bestRecoMuZindices[0]].Pt();
            cutMuPt2 = cutMuVec[bestRecoMuZindices[1]].Pt();
            //std::cout << "pTs:     " << cutMuPt1 << " " << cutMuPt2 << std::endl;
        }
        int summedElecCharge = 5;
        if(cutElecVec.size() >= 2) summedElecCharge = cutElecCharge[0] + cutElecCharge[1];
        const double minMuPt    =  20.0;
        const double minElecPt  =  20.0;
        const double minZM      =  81.0;
        const double maxZM      = 101.0;

        bool muPassCharge = (summedMuCharge == 0);
        bool elecPassCharge = (summedElecCharge == 0);
        //bool muPassTrigger = (cutMuPt1 > muTrigger);
        //bool elecPassTrigger = (cutElecPt1 > elecTrigger);

        //3Lepton cut
        cutflow_h->Fill(3);
        //cutflow_h->GetXaxis()->SetBinLabel(3,"3 Leptons");

        //Lepton pTs: normal 40,20,20; loose_cuts 25,20,20
        if(cutMuVec[0].Pt() < leptonpts[0] && cutElecPt1 < leptonpts[0]) continue;

        if(cutMuVec.size() == 1)
        {
            if(cutMuVec[0].Pt() >= leptonpts[0])
            {
                if(cutElecPt1 < leptonpts[1] || cutElecPt2 < leptonpts[2]) continue;
            }
            else if(cutMuVec[0].Pt() < leptonpts[1] || cutElecPt2 < leptonpts[2]) continue; //could be reverse
        }
        if(cutMuVec.size() == 2)
        {
            if(cutMuVec[0].Pt() >= leptonpts[0])
            {
                if(cutMuVec[1].Pt() < leptonpts[1] || cutElecPt1 < leptonpts[2]) continue; //could be reverse
            }
            else if(cutMuVec[0].Pt() < leptonpts[1] || cutMuVec[1].Pt() < leptonpts[2]) continue;
        }
        if(cutMuVec.size() == 3)
        {
            if(cutMuVec[1].Pt() < leptonpts[1] || cutMuVec[2].Pt() < leptonpts[2]) continue;
        }

        //DiLepton cut
        if((cutMuPt1 < minMuPt || cutMuPt2 < minMuPt || !muPassCharge) && (cutElecPt1 < minElecPt || cutElecPt2 < minElecPt || !elecPassCharge)) continue;
        cutflow_h->Fill(4);
        //cutflow_h->GetXaxis()->SetBinLabel(5,"DiLep");

        //At least 2 bjet
        //if(Stop0l_nbtags < 2) continue;
        if(nbtags < 2) continue;
        cutflow_h->Fill(5);
        //cutflow_h->GetXaxis()->SetBinLabel(6,">=1 bjet");

        //Pass MET > 40 
        const auto& met    = tr.getVar<float>("MET_pt");
        const auto& metphi = tr.getVar<float>("MET_phi");
        //const auto& ht     = tr.getVar<float>("Stop0l_HT");
        const auto& ht     = tr.getVar<float>("HT" + suffix);
        if(met < 30) continue;
        //if(met < 40) continue;
        cutflow_h->Fill(6);
        //cutflow_h->GetXaxis()->SetBinLabel(7,"MET > 40");

        //Find a Z
        const auto& bestRecoZPt = tr.getVar<data_t>("bestRecoZPt");
        const auto& bestRecoZM  = tr.getVar<data_t>("bestRecoZM");
        //const auto& passMuZinvSel = tr.getVar<bool>("passMuZinvSel");
        //const auto& passElecZinvSel = tr.getVar<bool>("passElecZinvSel");
        //const auto& passElMuZinvSel = tr.getVar<bool>("passElMuZinvSel");
        //const auto& passMuOnZMassPeak = tr.getVar<bool>("passMuOnZMassPeak");
        //const auto& passElecOnZMassPeak = tr.getVar<bool>("passElecOnZMassPeak");
        bool passZMassPeak = (bestRecoZM > minZM) && (bestRecoZM < maxZM);

        //Include a Z on mass peak
        //if(!passMuZinvSel && !passElecZinvSel && !passElMuZinvSel) continue; //Includes dilepton selections (exactly 2 leptons), which contradicts 3lep
        //if(!passMuOnZMassPeak && !passElecOnZMassPeak) continue;
        if(!passZMassPeak) continue;
        cutflow_h->Fill(7);
        //cutflow_h->GetXaxis()->SetBinLabel(8,"Z Mass Peak");
        eff_h->Fill(1.,sign);

        //Find the weights
        float tot_lepSF = 1.0;
        float MuonTriggerEffPt = 1.0;
        float MuonTriggerEffEta = 1.0;
        float LeadingMuTriggEffPt = 1.0;
        float LeadingMuTriggEffEta = 1.0;
        float tot_weight = 1.0;
        float pt_weight = 1.0;
        float eta_weight = 1.0;
        float multi_weight = 1.0;
        float B_SF = 1.0;
        float puWeight = 1.0;
        float PrefireWeight = 1.0;
        float mostweights = 1.0;
        //Already have cutMuSF.size() + cutElecSF.size() == 3 from earlier cut, so just loop over all leptons
        if(!Data)
        {
            
            //if(tr.getEvtNum() < 1000) std::cout << "Event Number: " << tr.getEvtNum() << "Muon SF: ";
            for(int i = 0; i < cutMuSF.size(); i++)
            {
                //if(tr.getEvtNum() < 1000) std::cout << cutMuSF[i] << "\t";
                tot_lepSF *= cutMuSF[i];
            }
            //if(tr.getEvtNum() < 1000) std::cout << "\tElectron SF: ";
            for(int i = 0; i < cutElecSF.size(); i++)
            {
                //if(tr.getEvtNum() < 1000) std::cout << cutElecSF[i] << "\t";
                tot_lepSF *= cutElecSF[i];
            }
            //if(tr.getEvtNum() < 1000) std::cout << std::endl;

            //Btag
            B_SF = tr.getVar<float>("BTagWeight");
            //PUWeight
            if(era != "2017") puWeight = tr.getVar<float>("puWeight");
            else if(!PeriodF) puWeight = tr.getVar<float>("17BtoEpuWeight");
            else puWeight = tr.getVar<float>("17FpuWeight");
            //PrefireWeight
            if(era == "2016" || era == "2017") PrefireWeight = tr.getVar<float>("PrefireWeight");
            mostweights = sign * tot_lepSF * B_SF * puWeight * PrefireWeight;

            //TODO: Trigger efficiencies for dilepton triggers
            //Looks like dimuon trigger efficiency is ~92.5% across all pT above 10. Loose cuts are 25,20,20, so that falls well within that bar.
            MuonTriggerEffPt = tr.getVar<float>("MuonTriggerEffPt");
            MuonTriggerEffEta = tr.getVar<float>("MuonTriggerEffEta");
            LeadingMuTriggEffPt = tr.getVar<float>("LeadingMuTriggEffPt");
            LeadingMuTriggEffEta = tr.getVar<float>("LeadingMuTriggEffEta");
            float DiMuTriggEffPt = 0.925; //What is this?
            pt_weight = mostweights * LeadingMuTriggEffPt;
            eta_weight = mostweights * LeadingMuTriggEffEta;
            multi_weight = mostweights * MuonTriggerEffEta;
            
            if (noTrigEff) tot_weight = mostweights;
            else if (multiMuTriggEff) tot_weight = multi_weight;
            else if (!loose_cuts) tot_weight = eta_weight;
            else tot_weight = mostweights * DiMuTriggEffPt;
        }

        //Lepton pTs
        if(cutMuVec.size() == 3)
        {
            h_lep1pT->Fill(cutMuVec[bestRecoMuZindices[0]].Pt(), tot_weight);
            h_lep2pT->Fill(cutMuVec[bestRecoMuZindices[1]].Pt(), tot_weight);
            h_3mu_lep1pT->Fill(cutMuVec[0].Pt(), tot_weight);
            h_3mu_lep2pT->Fill(cutMuVec[1].Pt(), tot_weight);
            h_3mu_lep3pT->Fill(cutMuVec[2].Pt(), tot_weight);
            h_3mu_lep1eta->Fill(cutMuVec[0].Eta(), tot_weight);
            h_3mu_lep2eta->Fill(cutMuVec[1].Eta(), tot_weight);
            h_3mu_lep3eta->Fill(cutMuVec[2].Eta(), tot_weight);
            if((bestRecoMuZindices[0] == 0 || bestRecoMuZindices[1] == 0) && (bestRecoMuZindices[0] == 1 || bestRecoMuZindices[1] == 1))
            {
                h_lep3pT->Fill(cutMuVec[2].Pt(), tot_weight);
            }
            else if((bestRecoMuZindices[0] == 1 || bestRecoMuZindices[1] == 1) && (bestRecoMuZindices[0] == 2 || bestRecoMuZindices[1] == 2))
            {
                h_lep3pT->Fill(cutMuVec[0].Pt(), tot_weight);
            }
            else if((bestRecoMuZindices[0] == 2 || bestRecoMuZindices[1] == 2) && (bestRecoMuZindices[0] == 0 || bestRecoMuZindices[1] == 0))
            {
                h_lep3pT->Fill(cutMuVec[1].Pt(), tot_weight);
            }
            else std::cout << "Error: Failed to find index of third muon in 3-muon case." << std::endl;
            h_num_mu->Fill(3,tot_weight);
            h_num_mu_notrigwt->Fill(3,mostweights);
            h_num_mu_pt_eff->Fill(3,pt_weight);
            h_num_mu_eta_eff->Fill(3,eta_weight);
            h_mu_indices->Fill(bestRecoMuZindices[0], tot_weight);
            h_mu_indices->Fill(bestRecoMuZindices[1], tot_weight);
            h_norm_3mu->Fill(bestRecoZM, tot_weight);
        }
        if(cutMuVec.size() == 2)
        {
            h_lep1pT->Fill(cutMuVec[0].Pt(), tot_weight);
            h_lep2pT->Fill(cutMuVec[1].Pt(), tot_weight);
            h_lep3pT->Fill(cutElecVec[0].Pt(), tot_weight);
            h_num_mu->Fill(2,tot_weight);
            h_num_mu_notrigwt->Fill(2,mostweights);
            h_num_mu_pt_eff->Fill(2,pt_weight);
            h_num_mu_eta_eff->Fill(2,eta_weight);
            h_norm_2mu->Fill(bestRecoZM, tot_weight);
        }
        if(cutMuVec.size() == 1)
        {
            h_lep1pT->Fill(cutElecVec[0].Pt(), tot_weight);
            h_lep2pT->Fill(cutElecVec[1].Pt(), tot_weight);
            h_lep3pT->Fill(cutMuVec[0].Pt(), tot_weight);
            h_num_mu->Fill(1,tot_weight);
            h_num_mu_notrigwt->Fill(1,mostweights);
            h_num_mu_pt_eff->Fill(1,pt_weight);
            h_num_mu_eta_eff->Fill(1,eta_weight);
            h_norm_1mu->Fill(bestRecoZM, tot_weight);
        }
        //also want num electrons
        h_num_elec->Fill(cutElecVec.size(),tot_weight);
        h_num_elec_notrigwt->Fill(cutElecVec.size(),mostweights);

        //RecoZ
        h_recoZpt->Fill(bestRecoZPt, tot_weight);
        h_recoZM->Fill(bestRecoZM, tot_weight);
        if (!PostHEM && (bestRecoZM < 88 || bestRecoZM >= 90))
        {
            h_recoZM_trimmed->Fill(bestRecoZM, tot_weight);
            h_norm_trimmed->Fill(bestRecoZM, tot_weight);
            h_num_mu_trimmed->Fill(cutMuVec.size(),tot_weight);
        }
        if (PostHEM && (bestRecoZM < 96 || bestRecoZM >= 97))
        {
            h_recoZM_trimmed->Fill(bestRecoZM, tot_weight);
            h_norm_trimmed->Fill(bestRecoZM, tot_weight);
            h_num_mu_trimmed->Fill(cutMuVec.size(),tot_weight);
        }
        h_normalization->Fill(bestRecoZM, tot_weight);
        h_norm_notrigwt->Fill(bestRecoZM, mostweights);
        h_norm_pt_eff->Fill(bestRecoZM, pt_weight);
        h_norm_eta_eff->Fill(bestRecoZM, eta_weight);
        //if (tr.getEvtNum() < 20000) std::cout << "Eta weight: " << pt_weight << "\tEta: " << cutMuVec[0].Eta() << std::endl;
        //MET
        h_met->Fill(met, tot_weight);
        //HT
        h_ht->Fill(ht, tot_weight);
        //nJet
        h_njet->Fill(nJet, tot_weight);
        //h_Stop0l_nJets->Fill(Stop0l_nJets, tot_weight);
        int njetpt20eta24 = 0;
        int njetpt30eta24 = 0;
        int njetpt40eta24 = 0;
        int njetpt40 = 0;
        for(unsigned int i = 0; i < Jet_pt.size(); i++)
        {
            if(Jet_pt[i] < 20) continue;
            if(Jet_pt[i] >= 40) njetpt40++;
            if(abs(Jet_eta[i]) > 2.4) continue;
            njetpt20eta24++;
            if(Jet_pt[i] < 30) continue;
            njetpt30eta24++;
            if(Jet_pt[i] >= 40) njetpt40eta24++;
        }
        h_njetpt20eta24->Fill(njetpt20eta24, tot_weight);
        h_njetpt30eta24->Fill(njetpt30eta24, tot_weight);
        h_njetpt40eta24->Fill(njetpt40eta24, tot_weight);
        h_njetpt40->Fill(njetpt40, tot_weight);

        if(Data)
        {
            std::cout << "Found an event: " << tr.getEvtNum() << std::endl;
            std::cout << "Muon pts: ";
            for(int i = 0; i < cutMuVec.size(); i++)
            {
                std::cout << cutMuVec[i].Pt() << "\t";
            }
            std::cout << std::endl;
            std::cout << "Electron pts: ";
            for(int i = 0; i < cutElecVec.size(); i++)
            {
                std::cout << cutElecVec[i].Pt() << "\t";
            }
            std::cout << std::endl;
        }
    }

    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    eff_h->Write();
    cutflow_h->Write();
    out_file.cd("../");
    h_recoZpt->Write();
    h_recoZM->Write();
    h_recoZM_trimmed->Write();
    h_met->Write();
    h_ht->Write();
    h_njet->Write();
    h_njetpt20eta24->Write();
    h_njetpt30eta24->Write();
    h_njetpt40eta24->Write();
    h_njetpt40->Write();
    //h_Stop0l_nJets->Write();
    h_lep1pT->Write();
    h_lep2pT->Write();
    h_lep3pT->Write();
    h_3mu_lep1pT->Write();
    h_3mu_lep2pT->Write();
    h_3mu_lep3pT->Write();
    h_3mu_lep1eta->Write();
    h_3mu_lep2eta->Write();
    h_3mu_lep3eta->Write();
    h_num_mu->Write();
    h_num_mu_notrigwt->Write();
    h_num_mu_trimmed->Write();
    h_num_mu_pt_eff->Write();
    h_num_mu_eta_eff->Write();
    h_num_elec->Write();
    h_num_elec_notrigwt->Write();
    h_mu_indices->Write();
    h_normalization->Write();
    h_norm_notrigwt->Write();
    h_norm_trimmed->Write();
    h_norm_pt_eff->Write();
    h_norm_eta_eff->Write();
    h_norm_1mu->Write();
    h_norm_2mu->Write();
    h_norm_3mu->Write();
    out_file.Close();

    return 0;
}

