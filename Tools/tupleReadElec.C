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
    int max_events = -1;
    //int max_events = 30000; //Set to -1 to run over all events
    //bool multiMuTriggEff = false;
    //bool leadingMuPt50 = true;
    bool leadingElecPt40 = true;
    bool norm_cuts = true;
    bool loose_cuts = false;
    bool dryrun = false;
    
    float leptonpts[3] = {0,20,20};
    if(norm_cuts) leptonpts[0] = 40;
    if(loose_cuts) leptonpts[0] = 25;

    //Format taken from http://www.cplusplus.com/articles/DEN36Up4/ July 18 2019
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--era")
        {
            if (i + 1 < argc) // Make sure we aren't at the end of argv!
            {
                era = argv[i+1];
            }
            else // Uh-oh, there was no argument to the option.
            {
                std::cerr << "--era option requires an argument." << std::endl;
                return 1;
            }  
        }
        else if (std::string(argv[i]) == "--isData")
        {
            Data = true;
        }
        else if (std::string(argv[i]) == "--PostHEM")
        {
            PostHEM = true; 
        }
        else if (std::string(argv[i]) == "--max_events")
        {
            if (i + 1 < argc) max_events = atoi(argv[i+1]);
            else
            {
                std::cerr << "--max_events option requires an argument." << std::endl;
                return 1;
            }
        }
        else if (std::string(argv[i]) == "--dryrun")
        {
            std::cout << "Dryrun option. Ending as soon as all arguments are parsed." << std::endl;
            dryrun = true;
        }
        else if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            std::cout << "Usage: ./tupleReadNormEx <FileList> <NameOfOutputFile> <options>" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "\t--era\t\tRequires 2016, 2017, or 2018." << std::endl;
            std::cout << "\t--isData\tUse if running over data (default assumption MC). No argument." << std::endl;
            std::cout << "\t--PostHEM\tUse if running over 2018 PostHEM. Irrelevant otherwise (default false). No argument." << std::endl;
            std::cout << "\t--max_events\tSet number of max events. Default = -1 (all)." << std::endl;
            std::cout << "\t--dryrun\tUse to exit after printing arguments." << std::endl;
            std::cout << "\t--help / -h\tPrints this message and exits." << std::endl;
            return 0;
        }
    }

    //Moved after help option.
    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *outputfile   = argv[2];
    
    //Argument testing
    std::cout << "Era: " << era << "\tisData: " << Data << "\tisPostHEM: " << PostHEM << "\tMax events: " << max_events << "\tDryrun: " << dryrun << std::endl;

    if (dryrun) return 0;

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
    plotterFunctions::Gamma gamma;
    CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    RunTopTagger runTopTagger_drLeptonCleaned("TopTagger.cfg","_drLeptonCleaned",true,false);
    BaselineVessel blv(tr,"");
    BaselineVessel blv_drLeptonCleaned(tr, "_drLeptonCleaned");
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
    auto *h_num_elec = new TH1F("h_num_elec","Number of electrons",3,1,4);
    //auto *h_elec_indices = new TH1F("h_mu_indices", "Frequency of pT-ordered electron index used to reconstruct Z", 4,-1,3);
    auto *h_normalization = new TH1F("h_normalization","TTZ Normalization Exercise",4,61,141);
    auto *h_norm_pt_eff = new TH1F("h_norm_pt_eff","TTZ Normalization Exercise",4,61,141);
    auto *h_norm_1elec = new TH1F("h_norm_1elec","TTZ Normalization Exercise: 1 electron",4,61,141);
    auto *h_norm_2elec = new TH1F("h_norm_2elec","TTZ Normalization Exercise: 2 electrons",4,61,141);
    auto *h_norm_3elec = new TH1F("h_norm_3elec","TTZ Normalization Exercise: 3 electrons",4,61,141);
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

        //Get number of events
        eff_h->Fill(0);
        cutflow_h->Fill(0);
        cutflow_h->GetXaxis()->SetBinLabel(1,"Total");
        cutflow_h->GetXaxis()->SetBinLabel(2,"Single Electron");
        cutflow_h->GetXaxis()->SetBinLabel(3,">= 4 Jets");
    	cutflow_h->GetXaxis()->SetBinLabel(4,"3 Leptons");
        cutflow_h->GetXaxis()->SetBinLabel(5,"DiLep");
    	cutflow_h->GetXaxis()->SetBinLabel(6,">=2 bjets");
        cutflow_h->GetXaxis()->SetBinLabel(7,"MET > 30");
        cutflow_h->GetXaxis()->SetBinLabel(8,"Z Mass Peak");

        //HEM Veto
        if(PostHEM && (era == "2018"))
        {
            const auto& SAT_Pass_HEMVeto30_drLeptonCleaned = tr.getVar<bool>("SAT_Pass_HEMVeto30_drLeptonCleaned");
            if(!SAT_Pass_HEMVeto30_drLeptonCleaned) continue;
        }

        //Event Filter
        bool SAT_Pass_EventFilter = tr.getVar<bool>("SAT_Pass_EventFilter"); //drLeptonCleaned?
        if(!SAT_Pass_EventFilter) continue;

        //const auto& Jet_btag = tr.getVec<float>("Jet_btagCSVV2");
        const auto& Jet_btag = tr.getVec<float>("Jet_btagDeepB_drLeptonCleaned");
        //TODO: Choice of: Jet_btagStop0l_drLeptonCleaned Jet_btagDeepB_drLeptonCleaned Jet_btagCSVV2_drLeptonCleaned
        //const auto& Jet_btag = tr.getVec<float>("Jet_btagDeepB");
        const auto& Jet_LV = tr.getVec<TLorentzVector>("JetTLV_drLeptonCleaned");
        const auto& Jet_pt = tr.getVec<float>("Jet_pt_drLeptonCleaned");
        const auto& Jet_eta = tr.getVec<float>("Jet_eta_drLeptonCleaned");

        //NanoSUSY-tools uses DeepB for Stop0l_nbtags
        std::map<std::string,float> DeepCSVMediumWP ={
            {"2016" , 0.6324},
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
        //const double muTrigger  =  50.0;
        const double elecTrigger=  40.0;
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
        
        //Single Electron cut
        //if(!elecPassTrigger) continue;
        if(cutElecVec.size() < 1) continue;
        if(leadingElecPt40 && !Data)
        {
            if(cutElecVec[0].Pt() < elecTrigger) continue;
        }
        if (Data)
        {
            const auto& Pass_trigger_electron = tr.getVar<bool>("Pass_trigger_electron"); //Efficiencies calc'd by Hui, grabbed for weight below.
            if(!Pass_trigger_electron) continue;
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
        cutflow_h->Fill(2);
        //cutflow_h->GetXaxis()->SetBinLabel(2,">= 4 Jets");

        //Pass nLeptons == 3 and two above min pt for recoZ
        if(cutMuVec.size() + cutElecVec.size() != 3) continue;
        float cutMuPt1   = tr.getVar<float>("cutMuPt1");
        float cutMuPt2   = tr.getVar<float>("cutMuPt2");
        std::vector<int> bestRecoMuZindices (2,-1); //Two ints of value -1
        const auto& cutElecPt1 = tr.getVar<float>("cutElecPt1");
        const auto& cutElecPt2 = tr.getVar<float>("cutElecPt2");
        const auto& cutElecEta1 = tr.getVar<float>("cutElecEta1");
        const auto& cutMuCharge = tr.getVec<int>("cutMuCharge");
        const auto& cutElecCharge = tr.getVec<int>("cutElecCharge");
        int summedMuCharge = 5; //out-of-bounds val
        if(cutMuVec.size() == 2) summedMuCharge = cutMuCharge[0] + cutMuCharge[1];
        /*
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
        */
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


        //float leptonpts[3]; //Defined before event loop. No need to keep redefining when it's constant.
        //if(norm_cuts) leptonpts = {40,20,20};
        //if(loose_cuts) leptonpts = {25,20,20};

        //Lepton pTs 40,20,20
        if(cutMuPt1 < leptonpts[0] && cutElecPt1 < leptonpts[0]) continue;

        if(cutElecVec.size() == 1)
        {
            if(cutElecVec[0].Pt() >= leptonpts[0])
            {
                if(cutMuPt1 < leptonpts[1] || cutMuPt2 < leptonpts[2]) continue;
            }
            else if(cutElecVec[0].Pt() < leptonpts[1] || cutMuPt2 < leptonpts[2]) continue; //could be reverse
        }
        if(cutElecVec.size() == 2)
        {
            if(cutElecVec[0].Pt() >= leptonpts[0])
            {
                if(cutElecVec[1].Pt() < leptonpts[1] || cutMuPt1 < leptonpts[2]) continue; //could be reverse
            }
            else if(cutElecVec[0].Pt() < leptonpts[1] || cutElecVec[1].Pt() < leptonpts[2]) continue;
        }
        if(cutElecVec.size() == 3)
        {
            if(cutElecVec[1].Pt() < leptonpts[1] || cutElecVec[2].Pt() < leptonpts[2]) continue;
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
        const auto& ht     = tr.getVar<float>("HT_drLeptonCleaned");
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
        eff_h->Fill(1);

        //Find the weights
        float tot_lepSF = 1.0;
        //float MuonTriggerEffPt = 1.0;
        float LeadingElecTriggEffPt = 1.0;
        float LeadingElecTriggEffEta = 1.0;
        float tot_weight = 1.0;
        float pt_weight = 1.0;
        //Already have cutMuSF.size() + cutElecSF.size() == 3 from earlier cut, so just loop over all leptons
        if(!Data)
        {
            for(int i = 0; i < cutMuSF.size(); i++)
            {
                tot_lepSF *= cutMuSF[i];
            }
            for(int i = 0; i < cutElecSF.size(); i++)
            {
                tot_lepSF *= cutElecSF[i];
            }

            //MuonTriggerEffPt = tr.getVar<float>("MuonTriggerEffPt");
            LeadingElecTriggEffPt = tr.getVar<float>("LeadingElecTriggEffPt");
            LeadingElecTriggEffEta = tr.getVar<float>("LeadingElecTriggEffEta");
/*
            if (LeadingElecTriggEffEta < -1000 || LeadingElecTriggEffEta == 0)
            {
                if(era == "2016" && cutElecEta1 >= 2.0) LeadingElecTriggEffEta = 0.787019;
                else if(era == "2016" && cutElecEta1 <= -2.0) LeadingElecTriggEffEta = 0.788988;
                else if (era == "2017" && cutElecEta1 >= 2.0) LeadingElecTriggEffEta = 0.759226;
                else if (era == "2017" && cutElecEta1 <= -2.0) LeadingElecTriggEffEta = 0.789634;
                else if (era == "2018" && cutElecEta1 >= 2.0) LeadingElecTriggEffEta = 0.837097;
                else if (era == "2018" && cutElecEta1 <= -2.0) LeadingElecTriggEffEta = 0.850742;
                else
                {
                    LeadingElecTriggEffEta = 0.78;
                    std::cout << "Warning: Odd value of trigger efficiency. Eta is: " << cutElecEta1 << ". Setting trigger efficiency to 0.78 (central value)." << std::endl;
                }
            }
*/
            //if (multiMuTriggEff) tot_weight = tot_lepSF * MuonTriggerEffPt;
            //else tot_weight = tot_lepSF * LeadingElecTriggEffPt;
            tot_weight = tot_lepSF * LeadingElecTriggEffEta;
            pt_weight = tot_lepSF * LeadingElecTriggEffPt;
        }

        //Lepton pTs
        if(cutElecVec.size() == 3)
        {
            /*
            h_lep1pT->Fill(cutMuVec[bestRecoMuZindices[0]].Pt(), tot_weight);
            h_lep2pT->Fill(cutMuVec[bestRecoMuZindices[1]].Pt(), tot_weight);
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
            h_mu_indices->Fill(bestRecoMuZindices[0], tot_weight);
            h_mu_indices->Fill(bestRecoMuZindices[1], tot_weight);
            h_norm_3mu->Fill(bestRecoZM, tot_weight);
            */
            h_lep1pT->Fill(cutElecVec[0].Pt(),tot_weight);
            h_lep2pT->Fill(cutElecVec[1].Pt(),tot_weight);
            h_lep3pT->Fill(cutElecVec[2].Pt(),tot_weight);
            h_num_elec->Fill(3,tot_weight);
            h_norm_3elec->Fill(bestRecoZM, tot_weight);
        }
        if(cutElecVec.size() == 2)
        {
            h_lep1pT->Fill(cutElecVec[0].Pt(), tot_weight);
            h_lep2pT->Fill(cutElecVec[1].Pt(), tot_weight);
            h_lep3pT->Fill(cutMuVec[0].Pt(), tot_weight);
            h_num_elec->Fill(2,tot_weight);
            h_norm_2elec->Fill(bestRecoZM, tot_weight);
        }
        if(cutElecVec.size() == 1)
        {
            h_lep1pT->Fill(cutMuVec[0].Pt(), tot_weight);
            h_lep2pT->Fill(cutMuVec[1].Pt(), tot_weight);
            h_lep3pT->Fill(cutElecVec[0].Pt(), tot_weight);
            h_num_elec->Fill(1,tot_weight);
            h_norm_1elec->Fill(bestRecoZM, tot_weight);
        }

        //RecoZ
        h_recoZpt->Fill(bestRecoZPt, tot_weight);
        h_recoZM->Fill(bestRecoZM, tot_weight);
        h_normalization->Fill(bestRecoZM, tot_weight);
        h_norm_pt_eff->Fill(bestRecoZM, pt_weight);
        if (tr.getEvtNum() < 20000) std::cout << "Eta weight: " << pt_weight << "\tEta: " << cutElecVec[0].Eta() << std::endl;
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
    }

    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    eff_h->Write();
    cutflow_h->Write();
    out_file.cd("../");
    h_recoZpt->Write();
    h_recoZM->Write();
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
    h_num_elec->Write();
    //h_elec_indices->Write();
    h_normalization->Write();
    h_norm_pt_eff->Write();
    h_norm_1elec->Write();
    h_norm_2elec->Write();
    h_norm_3elec->Write();
    out_file.Close();

    return 0;
}

