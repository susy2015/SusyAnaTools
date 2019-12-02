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
//#include "ZInvisible/Tools/include/LepInfo.h"
#include "GetVectors.h"
//#include "CleanedJets.h"
#include "RunTopTagger.h"
#include "ZInvisible/Tools/include/Gamma.h"
#include "SB2018.h"
#include <getopt.h>

int main(int argc, char* argv[])
{
    //defaults
    std::string era = "2018";
    //float SF = 1.17; //for TTZToLLNuNu
    float SF = 1.0; //for everything else
    float SF_up = 1.0;
    float SF_down = 1.0;
    bool verbose = false;
    int max_events = -1;
    bool isData = false;
    bool isTTZ = false;
    bool PeriodF = false;
    bool PostHEM = false;
    bool isSignal = false;

    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *outputfile    = argv[2];

    int c;
    while (1)
    {
        //int option_index = 3;
        //required_argument: option takes an argument. Still optional to include.
        //no_argument: option does not take an argument
        static struct option long_options[] = {
            {"era",         required_argument,  0,  'e'},
            {"max_events",  required_argument,  0,  'm'},
            {"isData",      no_argument,        0,  'd'},
            {"isTTZ",       no_argument,        0,  'T'},
            {"isSignal",    no_argument,        0,  'S'},
            {"PeriodF",     no_argument,        0,  'F'},
            {"PostHEM",     no_argument,        0,  'H'},
            {"verbose",     no_argument,        0,  'v'},
        };
        c = getopt_long(argc,argv,"e:m:dTSFHv",long_options,NULL);
        if(c==-1) break;
        switch (c)
        {
            case 'e':
                era = optarg;
                break;
            case 'm':
                max_events = std::stoi(optarg);
                break;
            case 'd':
                isData = true;
                break;
            case 'T':
                isTTZ = true;
                break;
            case 'S':
                isSignal = true;
                break;
            case 'F':
                PeriodF = true;
                break;
            case 'H':
                PostHEM = true;
                break;
            case 'v':
                verbose = true;
                break;
        }
    }

    if(isTTZ)
    {
        SF = 1.19589; //was 1.17, then 1.26484, then 1.24827
        SF_up = 1.19589 + 0.168534; //was 1.33, then 1.09131, then 1.42179
        SF_down = 1.19589 - 0.168534; //was 1.01, then 1.43837, then 1.07475
    }

    std::cout << "Era: " << era << "\tisTTZ: " << isTTZ <<"\tSF: " << SF << "\tisSignal: " << isSignal << "\tisData: " << isData << "\tPeriodF: " << PeriodF << "\tPostHEM: " << PostHEM << "\tMax events: " << max_events << std::endl;

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

    if(verbose) std::cout << "Setting up" << std::endl;

    NTupleReader tr(ch);
    GetVectors getVectors;
    plotterFunctions::BasicLepton basicLepton;
    plotterFunctions::Gamma gamma(era);
    //CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    BaselineVessel blv(tr,era,"");
    
    if(verbose) std::cout << "Registering getVectors" << std::endl;

    tr.registerFunction(getVectors);
    tr.registerFunction(gamma);
    tr.registerFunction(basicLepton);
    //tr.registerFunction(cleanedJets);
    if(verbose) std::cout << "Registering runTopTagger" << std::endl;
    tr.registerFunction(runTopTagger);
    if(verbose) std::cout << "Registering blv" << std::endl;
    tr.registerFunction(blv);

    auto *h_vb_low = new TH1F("h_vb_low","Low dm Validation Bins",19,0,19);
    auto *h_vb_low_bsf_up = new TH1F("h_vb_low_bsf_up","VB Low B SF Up",19,0,19);
    auto *h_vb_low_bsf_down = new TH1F("h_vb_low_bsf_down","VB Low B SF Down",19,0,19);
    auto *h_vb_low_trig_eff_up = new TH1F("h_vb_low_trig_eff_up","VB Low Trigger Efficiency Up",19,0,19);
    auto *h_vb_low_trig_eff_down = new TH1F("h_vb_low_trig_eff_down","VB Low Trigger Efficiency Down",19,0,19);
    auto *h_vb_low_puWeight_up = new TH1F("h_vb_low_puWeight_up","VB Low PU Weight Up",19,0,19);
    auto *h_vb_low_puWeight_down = new TH1F("h_vb_low_puWeight_down","VB Low PU Weight Down",19,0,19);
    auto *h_vb_low_PFWeight_up = new TH1F("h_vb_low_PFWeight_up","VB Low PF Weight Up",19,0,19);
    auto *h_vb_low_PFWeight_down = new TH1F("h_vb_low_PFWeight_down","VB Low PF Weight Down",19,0,19);
    auto *h_vb_low_pdfWeight_up = new TH1F("h_vb_low_pdfWeight_up","VB Low PDF Weight Up",19,0,19);
    auto *h_vb_low_pdfWeight_down = new TH1F("h_vb_low_pdfWeight_down","VB Low PDF Weight Down",19,0,19);
    auto *h_vb_low_JES_up = new TH1F("h_vb_low_JES_up","VB Low JES Up",19,0,19);
    auto *h_vb_low_JES_down = new TH1F("h_vb_low_JES_down","VB Low JES Down",19,0,19);
    auto *h_vb_low_SF_up = new TH1F("h_vb_low_SF_up","VB Low TTZ SF Up",19,0,19);
    auto *h_vb_low_SF_down = new TH1F("h_vb_low_SF_down","VB Low TTZ SF Down",19,0,19);
    auto *h_vb_low_METUnClust_up = new TH1F("h_vb_low_METUnClust_up","VB Low METUnClust Up",19,0,19);
    auto *h_vb_low_METUnClust_down = new TH1F("h_vb_low_METUnClust_down","VB Low METUnClust Down",19,0,19);
    auto *h_vb_low_ivfunc_up = new TH1F("h_vb_low_ivfunc_up","VB Low Soft B Tag SF Up",19,0,19);
    auto *h_vb_low_ivfunc_down = new TH1F("h_vb_low_ivfunc_down","VB Low Soft B Tag SF Down",19,0,19);
    auto *h_vb_low_eff_e_up = new TH1F("h_vb_low_eff_e_up","VB Low Electron Veto SF Up",19,0,19);
    auto *h_vb_low_eff_e_down = new TH1F("h_vb_low_eff_e_down","VB Low Electron Veto SF Down",19,0,19);
    auto *h_vb_low_err_mu_up = new TH1F("h_vb_low_err_mu_up","VB Low Muon Loose SF Up",19,0,19);
    auto *h_vb_low_err_mu_down = new TH1F("h_vb_low_err_mu_down","VB Low Muon Loose SF Down",19,0,19);
    auto *h_vb_low_eff_tau_up = new TH1F("h_vb_low_eff_tau_up","VB Low Tau POG SF Up",19,0,19);
    auto *h_vb_low_eff_tau_down = new TH1F("h_vb_low_eff_tau_down","VB Low Tau POG SF Down",19,0,19);
    auto *h_vb_low_eff_restoptag_up = new TH1F("h_vb_low_eff_restoptag_up","VB Low Resolved Top Tag SF Up",19,0,19);
    auto *h_vb_low_eff_restoptag_down = new TH1F("h_vb_low_eff_restoptag_down","VB Low Resolved Top Tag SF Down",19,0,19);
    auto *h_vb_low_eff_toptag_up = new TH1F("h_vb_low_eff_toptag_up","VB Low Merged Top Tag SF Up",19,0,19);
    auto *h_vb_low_eff_toptag_down = new TH1F("h_vb_low_eff_toptag_down","VB Low Merged Top Tag SF Down",19,0,19);
    auto *h_vb_low_eff_wtag_up = new TH1F("h_vb_low_eff_wtag_up","VB Low W Tag SF Up",19,0,19);
    auto *h_vb_low_eff_wtag_down = new TH1F("h_vb_low_eff_wtag_down","VB Low W TagSF Down",19,0,19);
    auto *h_vb_low_ak8jet_sf_up = new TH1F("h_vb_low_ak8jet_sf_up","VB Low AK8Jet SF Up",19,0,19);
    auto *h_vb_low_ak8jet_sf_down = new TH1F("h_vb_low_ak8jet_sf_down","VB Low AK8Jet SF Down",19,0,19);

    auto *h_vb_high = new TH1F("h_vb_high","High dm Validation Bins",24,19,43);
    auto *h_vb_high_bsf_up = new TH1F("h_vb_high_bsf_up","VB High B SF Up",24,19,43);
    auto *h_vb_high_bsf_down = new TH1F("h_vb_high_bsf_down","VB High B SF Down",24,19,43);
    auto *h_vb_high_trig_eff_up = new TH1F("h_vb_high_trig_eff_up","VB High Trigger Efficiency Up",24,19,43);
    auto *h_vb_high_trig_eff_down = new TH1F("h_vb_high_trig_eff_down","VB High Trigger Efficiency Down",24,19,43);
    auto *h_vb_high_puWeight_up = new TH1F("h_vb_high_puWeight_up","VB High PU Weight Up",24,19,43);
    auto *h_vb_high_puWeight_down = new TH1F("h_vb_high_puWeight_down","VB High PU Weight Down",24,19,43);
    auto *h_vb_high_PFWeight_up = new TH1F("h_vb_high_PFWeight_up","VB High PF Weight Up",24,19,43);
    auto *h_vb_high_PFWeight_down = new TH1F("h_vb_high_PFWeight_down","VB High PF Weight Down",24,19,43);
    auto *h_vb_high_pdfWeight_up = new TH1F("h_vb_high_pdfWeight_up","VB High PDF Weight Up",24,19,43);
    auto *h_vb_high_pdfWeight_down = new TH1F("h_vb_high_pdfWeight_down","VB High PDF Weight Down",24,19,43);
    auto *h_vb_high_JES_up = new TH1F("h_vb_high_JES_up","VB High JES Up",24,19,43);
    auto *h_vb_high_JES_down = new TH1F("h_vb_high_JES_down","VB High JES Down",24,19,43);
    auto *h_vb_high_SF_up = new TH1F("h_vb_high_SF_up","VB High TTZ SF Up",24,19,43);
    auto *h_vb_high_SF_down = new TH1F("h_vb_high_SF_down","VB High TTZ SF Down",24,19,43);
    auto *h_vb_high_METUnClust_up = new TH1F("h_vb_high_METUnClust_up","VB High METUnClust Up",24,19,43);
    auto *h_vb_high_METUnClust_down = new TH1F("h_vb_high_METUnClust_down","VB High METUnClust Down",24,19,43);
    auto *h_vb_high_ivfunc_up = new TH1F("h_vb_high_ivfunc_up","VB High Soft B Tag SF Up",24,19,43);
    auto *h_vb_high_ivfunc_down = new TH1F("h_vb_high_ivfunc_down","VB High Soft B Tag SF Down",24,19,43);
    auto *h_vb_high_eff_e_up = new TH1F("h_vb_high_eff_e_up","VB High Electron Veto SF Up",24,19,43);
    auto *h_vb_high_eff_e_down = new TH1F("h_vb_high_eff_e_down","VB High Electron Veto SF Down",24,19,43);
    auto *h_vb_high_err_mu_up = new TH1F("h_vb_high_err_mu_up","VB High Muon Loose SF Up",24,19,43);
    auto *h_vb_high_err_mu_down = new TH1F("h_vb_high_err_mu_down","VB High Muon Loose SF Down",24,19,43);
    auto *h_vb_high_eff_tau_up = new TH1F("h_vb_high_eff_tau_up","VB High Tau POG SF Up",24,19,43);
    auto *h_vb_high_eff_tau_down = new TH1F("h_vb_high_eff_tau_down","VB High Tau POG SF Down",24,19,43);
    auto *h_vb_high_eff_restoptag_up = new TH1F("h_vb_high_eff_restoptag_up","VB High Resolved Top Tag SF Up",24,19,43);
    auto *h_vb_high_eff_restoptag_down = new TH1F("h_vb_high_eff_restoptag_down","VB High Resolved Top Tag SF Down",24,19,43);
    auto *h_vb_high_eff_toptag_up = new TH1F("h_vb_high_eff_toptag_up","VB High Merged Top Tag SF Up",24,19,43);
    auto *h_vb_high_eff_toptag_down = new TH1F("h_vb_high_eff_toptag_down","VB High Merged Top Tag SF Down",24,19,43);
    auto *h_vb_high_eff_wtag_up = new TH1F("h_vb_high_eff_wtag_up","VB High W Tag SF Up",24,19,43);
    auto *h_vb_high_eff_wtag_down = new TH1F("h_vb_high_eff_wtag_down","VB High W TagSF Down",24,19,43);
    auto *h_vb_high_ak8jet_sf_up = new TH1F("h_vb_high_ak8jet_sf_up","VB High AK8Jet SF Up",24,19,43);
    auto *h_vb_high_ak8jet_sf_down = new TH1F("h_vb_high_ak8jet_sf_down","VB High AK8Jet SF Down",24,19,43);

    auto *h_sb_low = new TH1F("h_sb_low","Low dm Search Bins",53,0,53);
    auto *h_sb_low_bsf_up = new TH1F("h_sb_low_bsf_up","SB Low B SF Up",53,0,53);
    auto *h_sb_low_bsf_down = new TH1F("h_sb_low_bsf_down","SB Low B SF Down",53,0,53);
    auto *h_sb_low_trig_eff_up = new TH1F("h_sb_low_trig_eff_up","SB Low Trigger Efficiency Up",53,0,53);
    auto *h_sb_low_trig_eff_down = new TH1F("h_sb_low_trig_eff_down","SB Low Trigger Efficiency Down",53,0,53);
    auto *h_sb_low_puWeight_up = new TH1F("h_sb_low_puWeight_up","SB Low PU Weight Up",53,0,53);
    auto *h_sb_low_puWeight_down = new TH1F("h_sb_low_puWeight_down","SB Low PU Weight Down",53,0,53);
    auto *h_sb_low_PFWeight_up = new TH1F("h_sb_low_PFWeight_up","SB Low PF Weight Up",53,0,53);
    auto *h_sb_low_PFWeight_down = new TH1F("h_sb_low_PFWeight_down","SB Low PF Weight Down",53,0,53);
    auto *h_sb_low_pdfWeight_up = new TH1F("h_sb_low_pdfWeight_up","SB Low PDF Weight Up",53,0,53);
    auto *h_sb_low_pdfWeight_down = new TH1F("h_sb_low_pdfWeight_down","SB Low PDF Weight Down",53,0,53);
    auto *h_sb_low_JES_up = new TH1F("h_sb_low_JES_up","SB Low JES Up",53,0,53);
    auto *h_sb_low_JES_down = new TH1F("h_sb_low_JES_down","SB Low JES Down",53,0,53);
    auto *h_sb_low_SF_up = new TH1F("h_sb_low_SF_up","SB Low TTZ SF Up",53,0,53);
    auto *h_sb_low_SF_down = new TH1F("h_sb_low_SF_down","SB Low TTZ SF Down",53,0,53);
    auto *h_sb_low_METUnClust_up = new TH1F("h_sb_low_METUnClust_up","SB Low METUnClust Up",53,0,53);
    auto *h_sb_low_METUnClust_down = new TH1F("h_sb_low_METUnClust_down","SB Low METUnClust Down",53,0,53);
    auto *h_sb_low_ivfunc_up = new TH1F("h_sb_low_ivfunc_up","SB Low Soft B Tag SF Up",53,0,53);
    auto *h_sb_low_ivfunc_down = new TH1F("h_sb_low_ivfunc_down","SB Low Soft B Tag SF Down",53,0,53);
    auto *h_sb_low_eff_e_up = new TH1F("h_sb_low_eff_e_up","SB Low Electron Veto SF Up",53,0,53);
    auto *h_sb_low_eff_e_down = new TH1F("h_sb_low_eff_e_down","SB Low Electron Veto SF Down",53,0,53);
    auto *h_sb_low_err_mu_up = new TH1F("h_sb_low_err_mu_up","SB Low Muon Loose SF Up",53,0,53);
    auto *h_sb_low_err_mu_down = new TH1F("h_sb_low_err_mu_down","SB Low Muon Loose SF Down",53,0,53);
    auto *h_sb_low_eff_tau_up = new TH1F("h_sb_low_eff_tau_up","SB Low Tau POG SF Up",53,0,53);
    auto *h_sb_low_eff_tau_down = new TH1F("h_sb_low_eff_tau_down","SB Low Tau POG SF Down",53,0,53);
    auto *h_sb_low_eff_restoptag_up = new TH1F("h_sb_low_eff_restoptag_up","SB Low Resolved Top Tag SF Up",53,0,53);
    auto *h_sb_low_eff_restoptag_down = new TH1F("h_sb_low_eff_restoptag_down","SB Low Resolved Top Tag SF Down",53,0,53);
    auto *h_sb_low_eff_toptag_up = new TH1F("h_sb_low_eff_toptag_up","SB Low Merged Top Tag SF Up",53,0,53);
    auto *h_sb_low_eff_toptag_down = new TH1F("h_sb_low_eff_toptag_down","SB Low Merged Top Tag SF Down",53,0,53);
    auto *h_sb_low_eff_wtag_up = new TH1F("h_sb_low_eff_wtag_up","SB Low W Tag SF Up",53,0,53);
    auto *h_sb_low_eff_wtag_down = new TH1F("h_sb_low_eff_wtag_down","SB Low W TagSF Down",53,0,53);
    auto *h_sb_low_ak8jet_sf_up = new TH1F("h_sb_low_ak8jet_sf_up","SB Low AK8Jet SF Up",53,0,53);
    auto *h_sb_low_ak8jet_sf_down = new TH1F("h_sb_low_ak8jet_sf_down","SB Low AK8Jet SF Down",53,0,53);

    auto *h_sb_high = new TH1F("h_sb_high","High dm Search Bins",130,53,183);
    auto *h_sb_high_bsf_up = new TH1F("h_sb_high_bsf_up","SB High B SF Up",130,53,183);
    auto *h_sb_high_bsf_down = new TH1F("h_sb_high_bsf_down","SB High B SF Down",130,53,183);
    auto *h_sb_high_trig_eff_up = new TH1F("h_sb_high_trig_eff_up","SB High Trigger Efficiency Up",130,53,183);
    auto *h_sb_high_trig_eff_down = new TH1F("h_sb_high_trig_eff_down","SB High Trigger Efficiency Down",130,53,183);
    auto *h_sb_high_puWeight_up = new TH1F("h_sb_high_puWeight_up","SB High PU Weight Up",130,53,183);
    auto *h_sb_high_puWeight_down = new TH1F("h_sb_high_puWeight_down","SB High PU Weight Down",130,53,183);
    auto *h_sb_high_PFWeight_up = new TH1F("h_sb_high_PFWeight_up","SB High PF Weight Up",130,53,183);
    auto *h_sb_high_PFWeight_down = new TH1F("h_sb_high_PFWeight_down","SB High PF Weight Down",130,53,183);
    auto *h_sb_high_pdfWeight_up = new TH1F("h_sb_high_pdfWeight_up","SB High PDF Weight Up",130,53,183);
    auto *h_sb_high_pdfWeight_down = new TH1F("h_sb_high_pdfWeight_down","SB High PDF Weight Down",130,53,183);
    auto *h_sb_high_JES_up = new TH1F("h_sb_high_JES_up","SB High JES Up",130,53,183);
    auto *h_sb_high_JES_down = new TH1F("h_sb_high_JES_down","SB High JES Down",130,53,183);
    auto *h_sb_high_SF_up = new TH1F("h_sb_high_SF_up","SB High TTZ SF Up",130,53,183);
    auto *h_sb_high_SF_down = new TH1F("h_sb_high_SF_down","SB High TTZ SF Down",130,53,183);
    auto *h_sb_high_METUnClust_up = new TH1F("h_sb_high_METUnClust_up","SB High METUnClust Up",130,53,183);
    auto *h_sb_high_METUnClust_down = new TH1F("h_sb_high_METUnClust_down","SB High METUnClust Down",130,53,183);
    auto *h_sb_high_ivfunc_up = new TH1F("h_sb_high_ivfunc_up","SB High Soft B Tag SF Up",130,53,183);
    auto *h_sb_high_ivfunc_down = new TH1F("h_sb_high_ivfunc_down","SB High Soft B Tag SF Down",130,53,183);
    auto *h_sb_high_eff_e_up = new TH1F("h_sb_high_eff_e_up","SB High Electron Veto SF Up",130,53,183);
    auto *h_sb_high_eff_e_down = new TH1F("h_sb_high_eff_e_down","SB High Electron Veto SF Down",130,53,183);
    auto *h_sb_high_err_mu_up = new TH1F("h_sb_high_err_mu_up","SB High Muon Loose SF Up",130,53,183);
    auto *h_sb_high_err_mu_down = new TH1F("h_sb_high_err_mu_down","SB High Muon Loose SF Down",130,53,183);
    auto *h_sb_high_eff_tau_up = new TH1F("h_sb_high_eff_tau_up","SB High Tau POG SF Up",130,53,183);
    auto *h_sb_high_eff_tau_down = new TH1F("h_sb_high_eff_tau_down","SB High Tau POG SF Down",130,53,183);
    auto *h_sb_high_eff_restoptag_up = new TH1F("h_sb_high_eff_restoptag_up","SB High Resolved Top Tag SF Up",130,53,183);
    auto *h_sb_high_eff_restoptag_down = new TH1F("h_sb_high_eff_restoptag_down","SB High Resolved Top Tag SF Down",130,53,183);
    auto *h_sb_high_eff_toptag_up = new TH1F("h_sb_high_eff_toptag_up","SB High Merged Top Tag SF Up",130,53,183);
    auto *h_sb_high_eff_toptag_down = new TH1F("h_sb_high_eff_toptag_down","SB High Merged Top Tag SF Down",130,53,183);
    auto *h_sb_high_eff_wtag_up = new TH1F("h_sb_high_eff_wtag_up","SB High W Tag SF Up",130,53,183);
    auto *h_sb_high_eff_wtag_down = new TH1F("h_sb_high_eff_wtag_down","SB High W TagSF Down",130,53,183);
    auto *h_sb_high_ak8jet_sf_up = new TH1F("h_sb_high_ak8jet_sf_up","SB High AK8Jet SF Up",130,53,183);
    auto *h_sb_high_ak8jet_sf_down = new TH1F("h_sb_high_ak8jet_sf_down","SB High AK8Jet SF Down",130,53,183);

    auto *h_njets_low_middphi = new TH1F("h_njets_low_middphi","NJets for Low DM mid-dphi region",25,0,25);
    auto *h_njets_high_middphi = new TH1F("h_njets_high_middphi","NJets for High DM mid-dphi region",25,0,25);

    auto *eff_h = new TH1F("eff_h","eff_h",4,0,4); //Counting events. First bin counts all, second event filter, third baselines, fourth counts total to VB.

    if(verbose) std::cout << "Starting while loop." << std::endl;

    while(tr.getNextEvent())
    {

        if(tr.getEvtNum() % 1000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

        if(max_events != -1 && tr.getEvtNum() > max_events) break;

        //Need sign for all events
        float sign = 1.0;

        float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
        if(Stop0l_evtWeight < 0) sign = -1.0; //Just taking the sign from Stop0l_evtWeight

        //Count all events:
        eff_h->Fill(0.,sign);

        bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
        //bool SAT_Pass_EventFilter = tr.getVar<bool>("SAT_Pass_EventFilter");
        //Since every validation bin requires the event filter:
        bool Pass_JetID = tr.getVar<bool>("Pass_JetID");
        bool Pass_CaloMETRatio = tr.getVar<bool>("Pass_CaloMETRatio");
        bool Pass_trigger_MET = true;
        if(isData) Pass_trigger_MET = tr.getVar<bool>("Pass_trigger_MET");
        bool Pass_HEMVeto30 = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30 = tr.getVar<bool>("Pass_exHEMVeto30");
        Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio && Pass_trigger_MET && Pass_HEMVeto30;

        bool Pass_EventFilter_JESUp = tr.getVar<bool>("Pass_EventFilter_JESUp");
        bool Pass_JetID_JESUp = tr.getVar<bool>("Pass_JetID_JESUp");
        bool Pass_CaloMETRatio_JESUp = tr.getVar<bool>("Pass_CaloMETRatio_JESUp");
        bool Pass_HEMVeto30_JESUp = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_JESUp = tr.getVar<bool>("Pass_exHEMVeto30_JESUp");
        Pass_EventFilter_JESUp = Pass_EventFilter_JESUp && Pass_JetID_JESUp && Pass_CaloMETRatio_JESUp && Pass_trigger_MET && Pass_HEMVeto30_JESUp;
        
        bool Pass_EventFilter_JESDown = tr.getVar<bool>("Pass_EventFilter_JESDown");
        bool Pass_JetID_JESDown = tr.getVar<bool>("Pass_JetID_JESDown");
        bool Pass_CaloMETRatio_JESDown = tr.getVar<bool>("Pass_CaloMETRatio_JESDown");
        bool Pass_HEMVeto30_JESDown = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_JESDown = tr.getVar<bool>("Pass_exHEMVeto30_JESDown");
        Pass_EventFilter_JESDown = Pass_EventFilter_JESDown && Pass_JetID_JESDown && Pass_CaloMETRatio_JESDown && Pass_trigger_MET && Pass_HEMVeto30_JESDown;

        bool Pass_EventFilter_METUnClustUp = tr.getVar<bool>("Pass_EventFilter_METUnClustUp");
        bool Pass_JetID_METUnClustUp = tr.getVar<bool>("Pass_JetID_METUnClustUp");
        bool Pass_CaloMETRatio_METUnClustUp = tr.getVar<bool>("Pass_CaloMETRatio_METUnClustUp");
        bool Pass_HEMVeto30_METUnClustUp = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_METUnClustUp = tr.getVar<bool>("Pass_exHEMVeto30_METUnClustUp");
        Pass_EventFilter_METUnClustUp = Pass_EventFilter_METUnClustUp && Pass_JetID_METUnClustUp && Pass_CaloMETRatio_METUnClustUp && Pass_trigger_MET && Pass_HEMVeto30_METUnClustUp;
        
        bool Pass_EventFilter_METUnClustDown = tr.getVar<bool>("Pass_EventFilter_METUnClustDown");
        bool Pass_JetID_METUnClustDown = tr.getVar<bool>("Pass_JetID_METUnClustDown");
        bool Pass_CaloMETRatio_METUnClustDown = tr.getVar<bool>("Pass_CaloMETRatio_METUnClustDown");
        bool Pass_HEMVeto30_METUnClustDown = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_METUnClustDown = tr.getVar<bool>("Pass_exHEMVeto30_METUnClustDown");
        Pass_EventFilter_METUnClustDown = Pass_EventFilter_METUnClustDown && Pass_JetID_METUnClustDown && Pass_CaloMETRatio_METUnClustDown && Pass_trigger_MET && Pass_HEMVeto30_METUnClustDown;

        //if(!Pass_EventFilter) continue;
        //Count events passing filter:
        //eff_h->Fill(1.,sign);

        //Gen Lepton Veto
        if(isTTZ)
        {
            if(verbose && (tr.getEvtNum() == 1)) std::cout << "Gen Lepton Veto" << std::endl;
            bool lepton = false;
            auto pdgidvec = tr.getVec<int>("GenPart_pdgId");
            for(int i = 0; i < pdgidvec.size(); i++)
            {
                //e +/- 11, mu +/- 13, tau +/- 15
                if(std::abs(pdgidvec[i]) == 11 || std::abs(pdgidvec[i]) == 13 || std::abs(pdgidvec[i]) == 15)
                {
                    lepton = true;
                    break;
                }
            }
            if(lepton) continue;
        }

        //Soft B SF
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Soft B SF" << std::endl;
        auto vec_SB_SF = tr.getVec<float>("SB_SF");
        auto vec_SB_SFerr = tr.getVec<float>("SB_SFerr");
        float SB_SF = 1.0;
        float SB_SFerr = 0.0;
        auto SB_Stop0l = tr.getVec<unsigned char>("SB_Stop0l");
        //if(verbose && tr.getEvtNum() < 1000) std::cout << "SB_Stop0l size: " << SB_Stop0l.size() <<"\tSB_SF size: " << vec_SB_SF.size() << std::endl;
        for(int v = 0; v < vec_SB_SF.size(); v++)
        {
            if(SB_Stop0l[v])
            {
                SB_SF *= vec_SB_SF[v];
                SB_SFerr += vec_SB_SFerr[v]*vec_SB_SFerr[v];
            }
        }
        float SB_SF_up = SB_SF + std::sqrt(SB_SFerr);
        float SB_SF_down = SB_SF - std::sqrt(SB_SFerr);

        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "SB_SF: " << SB_SF << "\tSB_SFerr: " << SB_SFerr << std::endl;

        //Electron SF
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Electron SF" << std::endl;
        auto vec_Electron_VetoSF = tr.getVec<float>("Electron_VetoSF");
        auto vec_Electron_VetoSFErr = tr.getVec<float>("Electron_VetoSFErr");
        float Electron_VetoSF = 1.0;
        float Electron_VetoSFErr = 0.0;
        auto e_pt = tr.getVec<float>("Electron_pt");
        auto e_eta = tr.getVec<float>("Electron_eta");
        for(int v = 0; v < vec_Electron_VetoSF.size(); v++)
        {
            if(std::abs(e_eta[v]) < 2.5 && e_pt[v] > 5)
            {
                Electron_VetoSF *= vec_Electron_VetoSF[v];
                Electron_VetoSFErr += vec_Electron_VetoSFErr[v]*vec_Electron_VetoSFErr[v];
            }
        }
        float Electron_VetoSF_up = Electron_VetoSF + std::sqrt(Electron_VetoSFErr);
        float Electron_VetoSF_down = Electron_VetoSF - std::sqrt(Electron_VetoSFErr);

        //Muon SF
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Muon SF" << std::endl;
        auto vec_Muon_MediumSF = tr.getVec<float>("Muon_MediumSF");
        auto vec_Muon_MediumSFErr = tr.getVec<float>("Muon_MediumSFErr");
        float Muon_MediumSF = 1.0;
        float Muon_MediumSFErr = 0.0;
        auto mu_pt = tr.getVec<float>("Muon_pt");
        auto mu_eta = tr.getVec<float>("Muon_eta");
        for(int v = 0; v < vec_Muon_MediumSF.size(); v++)
        {
            if(std::abs(mu_eta[v]) < 2.4 && mu_pt[v] > 5)
            {
                Muon_MediumSF *= vec_Muon_MediumSF[v];
                Muon_MediumSFErr += vec_Muon_MediumSFErr[v]*vec_Muon_MediumSFErr[v];
            }
        }
        float Muon_MediumSF_up = Muon_MediumSF + std::sqrt(Muon_MediumSFErr);
        float Muon_MediumSF_down = Muon_MediumSF - std::sqrt(Muon_MediumSFErr);

        //Tau SF
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Tau SF" << std::endl;
        auto vec_Tau_MediumSF = tr.getVec<float>("Tau_MediumSF");
        auto vec_Tau_MediumSF_Up = tr.getVec<float>("Tau_MediumSF_Up");
        auto vec_Tau_MediumSF_Down = tr.getVec<float>("Tau_MediumSF_Down");
        float Tau_MediumSF = 1.0;
        float Tau_MediumSF_Up = 1.0;
        float Tau_MediumSF_Down = 1.0;
        auto tau_pt = tr.getVec<float>("Tau_pt");
        auto tau_eta = tr.getVec<float>("Tau_eta");
        for(int v = 0; v < vec_Tau_MediumSF.size(); v++)
        {
            if(std::abs(tau_eta[v]) < 2.4 && tau_pt[v] > 20)
            {
                Tau_MediumSF *= vec_Tau_MediumSF[v];
                Tau_MediumSF_Up *= vec_Tau_MediumSF_Up[v];
                Tau_MediumSF_Down *= vec_Tau_MediumSF_Down[v];
            }
        }

        //Res top tag SF
        //Up and down values are already set to be summed in quadrature. Need to make total SF.
        auto ResolvedTopCandidate_sf = tr.getVec<float>("ResolvedTopCandidate_sf");
        auto ResTopCand_CSPur_Up = tr.getVec<float>("ResolvedTopCandidate_syst_CSPur_Up");
        auto ResTopCand_CSPur_Down = tr.getVec<float>("ResolvedTopCandidate_syst_CSPur_Down");
        auto ResTopCand_Stat_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Stat_Up");
        auto ResTopCand_Stat_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Stat_Down");
        auto ResTopCand_Btag_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Btag_Up");
        auto ResTopCand_Btag_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Btag_Down");
        auto ResTopCand_Closure_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Closure_Up");
        auto ResTopCand_Closure_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Closure_Down");
        auto ResTopCand_Pileup_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Pileup_Up");
        auto ResTopCand_Pileup_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Pileup_Down");
        auto ResolvedTop_Stop0l = tr.getVec<unsigned char>("ResolvedTop_Stop0l");
        auto ResolvedTopCandidate_genMatch = tr.getVec<unsigned char>("ResolvedTopCandidate_genMatch");

        float ResTop_SF = 1.0;
        float ResTop_SF_up = 0.0;
        float ResTop_SF_down = 0.0;

        float test_sf = 1.0;
        float test_sf_cspur_up = 1.0;
        float test_sf_stat_up = 1.0;
        float test_sf_cspur_down = 1.0;
        float test_sf_stat_down = 1.0;

        for(int v = 0; v < ResolvedTop_Stop0l.size(); v++)
        {
            if(ResolvedTop_Stop0l[v])
            {
                ResTop_SF *= ResolvedTopCandidate_sf[v];
                ResTop_SF_up = ResTop_SF_up + std::pow(ResTopCand_CSPur_Up[v],2) + std::pow(ResTopCand_Stat_Up[v],2) + std::pow(ResTopCand_Btag_Up[v],2) + std::pow(ResTopCand_Pileup_Up[v],2);
                ResTop_SF_down = ResTop_SF_down + std::pow(ResTopCand_CSPur_Down[v],2) + std::pow(ResTopCand_Stat_Down[v],2) + std::pow(ResTopCand_Btag_Down[v],2) + std::pow(ResTopCand_Pileup_Down[v],2);
                if(!ResolvedTopCandidate_genMatch[v])
                {
                    ResTop_SF_up += std::pow(ResTopCand_Closure_Up[v],2);
                    ResTop_SF_down += std::pow(ResTopCand_Closure_Down[v],2);
                }
                test_sf_cspur_up = ResTopCand_CSPur_Up[v];
                test_sf_stat_up = ResTopCand_Stat_Up[v];
                test_sf_cspur_down = ResTopCand_CSPur_Down[v];
                test_sf_stat_down = ResTopCand_Stat_Down[v];
            }
        }
        ResTop_SF_up = std::sqrt(ResTop_SF_up);
        ResTop_SF_down = std::sqrt(ResTop_SF_down);
        //ResTop_SF_up = ResTop_SF * (1 + ResTop_SF_up);
        ResTop_SF_up = ResTop_SF + ResTop_SF_up;
        //ResTop_SF_down = ResTop_SF * (1 - ResTop_SF_down);
        ResTop_SF_down = ResTop_SF - ResTop_SF_down;

        if(verbose)
        {
            std::cout << "ResTop SF: " << ResTop_SF << "\tResTop SF Down: " << ResTop_SF_down << "\tlast CSPur down: " << test_sf_cspur_down << "\tlast stat down: " << test_sf_stat_down << std::endl;
        }

        //Merged top tag SF

        //W tag SF

        //both are part of the following:
        //FatJetSF
        auto vec_FatJet_SF = tr.getVec<float>("FatJet_SF");
        auto vec_FatJet_SFerr = tr.getVec<float>("FatJet_SFerr");
        auto FatJet_Stop0l = tr.getVec<int>("FatJet_Stop0l");
        float W_SF = 1.0;
        float W_SFerr = 0.0;
        float MergedTop_SF = 1.0;
        float MergedTop_SFerr = 0.0;
        for(int v = 0; v < vec_FatJet_SF.size(); v++)
        {
            if(FatJet_Stop0l[v] == 1)
            {
                MergedTop_SF *= vec_FatJet_SF[v];
                MergedTop_SFerr += vec_FatJet_SFerr[v]*vec_FatJet_SFerr[v];
            }
            else if(FatJet_Stop0l[v] == 2)
            {
                W_SF *= vec_FatJet_SF[v];
                W_SFerr += vec_FatJet_SFerr[v]*vec_FatJet_SFerr[v];
            }
        }
        float W_SF_up = W_SF + std::sqrt(W_SFerr);
        float W_SF_down = W_SF - std::sqrt(W_SFerr);
        float MergedTop_SF_up = MergedTop_SF + std::sqrt(MergedTop_SFerr);
        float MergedTop_SF_down = MergedTop_SF - std::sqrt(MergedTop_SFerr);

        //mtb
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "mtb" << std::endl;
        float mtb = tr.getVar<float>("Stop0l_Mtb");
        float mtb_JESUp = tr.getVar<float>("Stop0l_Mtb_JESUp");
        float mtb_JESDown = tr.getVar<float>("Stop0l_Mtb_JESDown");
        float mtb_METUnClustUp = tr.getVar<float>("Stop0l_Mtb_METUnClustUp");
        float mtb_METUnClustDown = tr.getVar<float>("Stop0l_Mtb_METUnClustDown");

        //njets
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "njets" << std::endl;
        int njetspt30 = tr.getVar<int>("Stop0l_nJets");
        int njets = njetspt30;
        int njets_JESUp = tr.getVar<int>("Stop0l_nJets_JESUp");
        int njets_JESDown = tr.getVar<int>("Stop0l_nJets_JESDown");
        //int njets_METUnClustUp = tr.getVar<int>("Stop0l_nJets_METUnClustUp");
        //int njets_METUnClustDown = tr.getVar<int>("Stop0l_nJets_METUnClustDown");

        //ntop
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ntop" << std::endl;
        int ntop_merge = tr.getVar<int>("Stop0l_nTop");
        int ntop_merge_JESUp = tr.getVar<int>("Stop0l_nTop_JESUp");
        int ntop_merge_JESDown = tr.getVar<int>("Stop0l_nTop_JESDown");

        //nw
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nw" << std::endl;
        int nw = tr.getVar<int>("Stop0l_nW");
        int nw_JESUp = tr.getVar<int>("Stop0l_nW_JESUp");
        int nw_JESDown = tr.getVar<int>("Stop0l_nW_JESDown");

        //nres
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nres" << std::endl;
        int ntop_res = tr.getVar<int>("Stop0l_nResolved");
        int ntop_res_JESUp = tr.getVar<int>("Stop0l_nResolved_JESUp");
        int ntop_res_JESDown = tr.getVar<int>("Stop0l_nResolved_JESDown");

        //nb
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nb" << std::endl;
        int nbottompt20 = tr.getVar<int>("Stop0l_nbtags");
        int nb = nbottompt20;
        int nb_JESUp = tr.getVar<int>("Stop0l_nbtags_JESUp");
        int nb_JESDown = tr.getVar<int>("Stop0l_nbtags_JESDown");

        //nSV //TODO: JESUp, JESDown ?
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nSV" << std::endl;
        int nSV = tr.getVar<int>("Stop0l_nSoftb");
        int nSV_JESUp = tr.getVar<int>("Stop0l_nSoftb");
        int nSV_JESDown = tr.getVar<int>("Stop0l_nSoftb");

        //ISRpt
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ISRpt" << std::endl;
        float ISRpt = tr.getVar<float>("Stop0l_ISRJetPt");
        float ISRpt_JESUp = tr.getVar<float>("Stop0l_ISRJetPt_JESUp");
        float ISRpt_JESDown = tr.getVar<float>("Stop0l_ISRJetPt_JESDown");

        //bottompt_scalar_sum
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ptb" << std::endl;
        float ptb = tr.getVar<float>("Stop0l_Ptb");
        float ptb_JESUp = tr.getVar<float>("Stop0l_Ptb_JESUp");
        float ptb_JESDown = tr.getVar<float>("Stop0l_Ptb_JESDown");

        //HT
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "HT" << std::endl;
        float HT = tr.getVar<float>("Stop0l_HT");
        float HT_JESUp = tr.getVar<float>("Stop0l_HT_JESUp");
        float HT_JESDown = tr.getVar<float>("Stop0l_HT_JESDown");

        //met
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "met" << std::endl;
        float met = tr.getVar<float>("MET_pt");
        float met_JESUp = tr.getVar<float>("MET_pt_jesTotalUp");
        float met_JESDown = tr.getVar<float>("MET_pt_jesTotalDown");
        float met_METUnClustUp = tr.getVar<float>("MET_pt_unclustEnUp");
        float met_METUnClustDown = tr.getVar<float>("MET_pt_unclustEnDown");

        //S_met
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "S_met" << std::endl;
        float S_met = tr.getVar<float>("Stop0l_METSig");
        float S_met_JESUp = tr.getVar<float>("Stop0l_METSig_JESUp");
        float S_met_JESDown = tr.getVar<float>("Stop0l_METSig_JESDown");
        float S_met_METUnClustUp = tr.getVar<float>("Stop0l_METSig_METUnClustUp");
        float S_met_METUnClustDown = tr.getVar<float>("Stop0l_METSig_METUnClustDown");

        //Event Filters
        if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Event Filters" << std::endl;
        bool Pass_lowDM_nodPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets30") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
        bool Val_bin_0_14 = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Val_bin_15_18 = Pass_lowDM_nodPhi && tr.getVar<bool>("Pass_dPhiMETMedDM");
        bool Pass_highDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42 = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Search_highdm = Pass_EventFilter && tr.getVar<bool>("Pass_highDM");

        bool Pass_lowDM_nodPhi_JESUp = (Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_LeptonVeto_JESUp") && tr.getVar<bool>("Pass_NJets30_JESUp") && tr.getVar<bool>("Pass_MET_JESUp") && tr.getVar<bool>("Pass_HT_JESUp") && ntop_merge_JESUp == 0 && ntop_res_JESUp == 0 && nw_JESUp == 0 && mtb_JESUp < 175 && S_met_JESUp > 10 && ISRpt_JESUp >= 200);
        bool Val_bin_0_14_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_lowDM_JESUp");
        bool Val_bin_15_18_JESUp = Pass_lowDM_nodPhi_JESUp && tr.getVar<bool>("Pass_dPhiMETMedDM_JESUp");
        bool Pass_highDM_no_dPhi_JESUp = (Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_LeptonVeto_JESUp") && tr.getVar<bool>("Pass_MET_JESUp") && tr.getVar<bool>("Pass_HT_JESUp") && njets_JESUp >= 5 && nb_JESUp >= 1);
        bool Val_bin_19_42_JESUp = (Pass_highDM_no_dPhi_JESUp && tr.getVar<bool>("Pass_dPhiMET_JESUp") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_JESUp")));
        bool Search_lowdm_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_lowDM_JESUp");
        bool Search_highdm_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_highDM_JESUp");

        bool Pass_lowDM_nodPhi_JESDown = (Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_LeptonVeto_JESDown") && tr.getVar<bool>("Pass_NJets30_JESDown") && tr.getVar<bool>("Pass_MET_JESDown") && tr.getVar<bool>("Pass_HT_JESDown") && ntop_merge_JESDown == 0 && ntop_res_JESDown == 0 && nw_JESDown == 0 && mtb_JESDown < 175 && S_met_JESDown > 10 && ISRpt_JESDown >= 200);
        bool Val_bin_0_14_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_lowDM_JESDown");
        bool Val_bin_15_18_JESDown = Pass_lowDM_nodPhi_JESDown && tr.getVar<bool>("Pass_dPhiMETMedDM_JESDown");
        bool Pass_highDM_no_dPhi_JESDown = (Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_LeptonVeto_JESDown") && tr.getVar<bool>("Pass_MET_JESDown") && tr.getVar<bool>("Pass_HT_JESDown") && njets_JESDown >= 5 && nb_JESDown >= 1);
        bool Val_bin_19_42_JESDown = (Pass_highDM_no_dPhi_JESDown && tr.getVar<bool>("Pass_dPhiMET_JESDown") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_JESDown")));
        bool Search_lowdm_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_lowDM_JESDown");
        bool Search_highdm_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_highDM_JESDown");

        bool Pass_lowDM_nodPhi_METUnClustUp = (Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_LeptonVeto_METUnClustUp") && tr.getVar<bool>("Pass_NJets30_METUnClustUp") && tr.getVar<bool>("Pass_MET_METUnClustUp") && tr.getVar<bool>("Pass_HT_METUnClustUp") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb_METUnClustUp < 175 && S_met_METUnClustUp > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_lowDM_METUnClustUp");
        bool Val_bin_15_18_METUnClustUp = Pass_lowDM_nodPhi_METUnClustUp && tr.getVar<bool>("Pass_dPhiMETMedDM_METUnClustUp");
        bool Pass_highDM_no_dPhi_METUnClustUp = (Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_LeptonVeto_METUnClustUp") && tr.getVar<bool>("Pass_MET_METUnClustUp") && tr.getVar<bool>("Pass_HT_METUnClustUp") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METUnClustUp = (Pass_highDM_no_dPhi_METUnClustUp && tr.getVar<bool>("Pass_dPhiMET_METUnClustUp") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustUp")));
        bool Search_lowdm_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_lowDM_METUnClustUp");
        bool Search_highdm_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_highDM_METUnClustUp");

        bool Pass_lowDM_nodPhi_METUnClustDown = (Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_LeptonVeto_METUnClustDown") && tr.getVar<bool>("Pass_NJets30_METUnClustDown") && tr.getVar<bool>("Pass_MET_METUnClustDown") && tr.getVar<bool>("Pass_HT_METUnClustDown") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb_METUnClustDown < 175 && S_met_METUnClustDown > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_lowDM_METUnClustDown");
        bool Val_bin_15_18_METUnClustDown = Pass_lowDM_nodPhi_METUnClustDown && tr.getVar<bool>("Pass_dPhiMETMedDM_METUnClustDown");
        bool Pass_highDM_no_dPhi_METUnClustDown = (Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_LeptonVeto_METUnClustDown") && tr.getVar<bool>("Pass_MET_METUnClustDown") && tr.getVar<bool>("Pass_HT_METUnClustDown") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METUnClustDown = (Pass_highDM_no_dPhi_METUnClustDown && tr.getVar<bool>("Pass_dPhiMET_METUnClustDown") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustDown")));
        bool Search_lowdm_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_lowDM_METUnClustDown");
        bool Search_highdm_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_highDM_METUnClustDown");

        float evtWeight = 1.0;
        float B_SF = 1.0;
        float B_SF_Down = 1.0;
        float B_SF_Up = 1.0;
        float trigger_eff = 1.0;
        float trigger_eff_down = 1.0;
        float trigger_eff_up = 1.0;
        float puWeight = 1.0;
        float puWeightDown = 1.0;
        float puWeightUp = 1.0;
        float PrefireWeight = 1.0;
        float PrefireWeightDown = 1.0;
        float PrefireWeightUp = 1.0;
        float pdfWeight_Up = 1.0;
        float pdfWeight_Down = 1.0;
        
        if(!isData)
        {
            B_SF = tr.getVar<float>("BTagWeight");
            B_SF_Down = tr.getVar<float>("BTagWeight_Down");
            B_SF_Up = tr.getVar<float>("BTagWeight_Up");
            trigger_eff = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline");
            trigger_eff_down = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_down");
            trigger_eff_up = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_up");
            pdfWeight_Up = tr.getVar<float>("pdfWeight_Up");
            pdfWeight_Down = tr.getVar<float>("pdfWeight_Down");
            if(era != "2017")
            {
                puWeight = tr.getVar<float>("puWeight");
                puWeightDown = tr.getVar<float>("puWeight_Down");
                puWeightUp = tr.getVar<float>("puWeight_Up");
            }
            else
            {
                if(!PeriodF)
                {
                    puWeight = tr.getVar<float>("17BtoEpuWeight");
                    puWeightDown = tr.getVar<float>("17BtoEpuWeight_Down");
                    puWeightUp = tr.getVar<float>("17BtoEpuWeight_Up");
                }
                else
                {
                    puWeight = tr.getVar<float>("17FpuWeight");
                    puWeightDown = tr.getVar<float>("17FpuWeight_Down");
                    puWeightUp = tr.getVar<float>("17FpuWeight_Up");
                }
            }
            if(era == "2016" || era == "2017")
            {
                PrefireWeight = tr.getVar<float>("PrefireWeight");
                PrefireWeightDown = tr.getVar<float>("PrefireWeight_Down");
                PrefireWeightUp = tr.getVar<float>("PrefireWeight_Up");
            }

            evtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * Electron_VetoSF * Muon_MediumSF * Tau_MediumSF * W_SF * MergedTop_SF * ResTop_SF;

        }

        //if(!Val_bin_0_14 && !Val_bin_15_18 && !Val_bin_19_42 && !Search_lowdm && !Search_highdm) continue;
        //eff_h->Fill(2.,sign);

        int bin_num = -1;
        int bin_num_JESUp = -1;
        int bin_num_JESDown = -1;
        int bin_num_METUnClustUp = -1;
        int bin_num_METUnClustDown = -1;
        int reset = 0;

        //Bins 0-14: int SB_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Val_bin_0_14)
        {
            bin_num = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met);
            if(bin_num != -1)
            {
                h_vb_low->Fill(bin_num,SF*evtWeight);
                h_vb_low_bsf_up->Fill(bin_num, SF * evtWeight * B_SF_Up / B_SF);
                h_vb_low_bsf_down->Fill(bin_num, SF * evtWeight * B_SF_Down / B_SF);
                h_vb_low_trig_eff_up->Fill(bin_num, SF * evtWeight * trigger_eff_up / trigger_eff);
                h_vb_low_trig_eff_down->Fill(bin_num, SF * evtWeight * trigger_eff_down / trigger_eff);
                h_vb_low_puWeight_up->Fill(bin_num, SF * evtWeight * puWeightUp / puWeight);
                h_vb_low_puWeight_down->Fill(bin_num, SF * evtWeight * puWeightDown / puWeight);
                h_vb_low_PFWeight_up->Fill(bin_num, SF * evtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_low_PFWeight_down->Fill(bin_num, SF * evtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_low_pdfWeight_up->Fill(bin_num, SF * evtWeight * pdfWeight_Up);
                h_vb_low_pdfWeight_down->Fill(bin_num, SF * evtWeight * pdfWeight_Down);
                h_vb_low_SF_up->Fill(bin_num, SF_up * evtWeight);
                h_vb_low_SF_down->Fill(bin_num, SF_down * evtWeight);
                h_vb_low_ivfunc_up->Fill(bin_num, SF * evtWeight * SB_SF_up / SB_SF);
                h_vb_low_ivfunc_down->Fill(bin_num, SF * evtWeight * SB_SF_down / SB_SF);
                h_vb_low_eff_e_up->Fill(bin_num, SF * evtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_low_eff_e_down->Fill(bin_num, SF * evtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_low_err_mu_up->Fill(bin_num, SF * evtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_low_err_mu_down->Fill(bin_num, SF * evtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_low_eff_tau_up->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_low_eff_tau_down->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_low_eff_wtag_up->Fill(bin_num, SF * evtWeight * W_SF_up / W_SF);
                h_vb_low_eff_wtag_down->Fill(bin_num, SF * evtWeight * W_SF_down / W_SF);
                h_vb_low_eff_toptag_up->Fill(bin_num, SF * evtWeight * MergedTop_SF_up / MergedTop_SF);
                h_vb_low_eff_toptag_down->Fill(bin_num, SF * evtWeight * MergedTop_SF_down / MergedTop_SF);
                h_vb_low_eff_restoptag_up->Fill(bin_num, SF * evtWeight * ResTop_SF_up / ResTop_SF);
                h_vb_low_eff_restoptag_down->Fill(bin_num, SF * evtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_0_14 has bin number -1." << std::endl;
        }
        if(Val_bin_0_14_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_low_JES_up->Fill(bin_num_JESUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_low_JES_down->Fill(bin_num_JESDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_low_METUnClust_up->Fill(bin_num_METUnClustUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_low_METUnClust_down->Fill(bin_num_METUnClustDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //Bins 15-18: SB_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met)
        if(Val_bin_15_18)
        {
            bin_num = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met);
            if(bin_num != -1)
            {
                h_vb_low->Fill(bin_num,SF*evtWeight);
                h_njets_low_middphi->Fill(njets,SF*evtWeight);
                h_vb_low_bsf_up->Fill(bin_num, SF * evtWeight * B_SF_Up / B_SF);
                h_vb_low_bsf_down->Fill(bin_num, SF * evtWeight * B_SF_Down / B_SF);
                h_vb_low_trig_eff_up->Fill(bin_num, SF * evtWeight * trigger_eff_up / trigger_eff);
                h_vb_low_trig_eff_down->Fill(bin_num, SF * evtWeight * trigger_eff_down / trigger_eff);
                h_vb_low_puWeight_up->Fill(bin_num, SF * evtWeight * puWeightUp / puWeight);
                h_vb_low_puWeight_down->Fill(bin_num, SF * evtWeight * puWeightDown / puWeight);
                h_vb_low_PFWeight_up->Fill(bin_num, SF * evtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_low_PFWeight_down->Fill(bin_num, SF * evtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_low_pdfWeight_up->Fill(bin_num, SF * evtWeight * pdfWeight_Up);
                h_vb_low_pdfWeight_down->Fill(bin_num, SF * evtWeight * pdfWeight_Down);
                h_vb_low_SF_up->Fill(bin_num, SF_up * evtWeight);
                h_vb_low_SF_down->Fill(bin_num, SF_down * evtWeight);
                h_vb_low_ivfunc_up->Fill(bin_num, SF * evtWeight * SB_SF_up / SB_SF);
                h_vb_low_ivfunc_down->Fill(bin_num, SF * evtWeight * SB_SF_down / SB_SF);
                h_vb_low_eff_e_up->Fill(bin_num, SF * evtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_low_eff_e_down->Fill(bin_num, SF * evtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_low_err_mu_up->Fill(bin_num, SF * evtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_low_err_mu_down->Fill(bin_num, SF * evtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_low_eff_tau_up->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_low_eff_tau_down->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_low_eff_wtag_up->Fill(bin_num, SF * evtWeight * W_SF_up / W_SF);
                h_vb_low_eff_wtag_down->Fill(bin_num, SF * evtWeight * W_SF_down / W_SF);
                h_vb_low_eff_toptag_up->Fill(bin_num, SF * evtWeight * MergedTop_SF_up / MergedTop_SF);
                h_vb_low_eff_toptag_down->Fill(bin_num, SF * evtWeight * MergedTop_SF_down / MergedTop_SF);
                h_vb_low_eff_restoptag_up->Fill(bin_num, SF * evtWeight * ResTop_SF_up / ResTop_SF);
                h_vb_low_eff_restoptag_down->Fill(bin_num, SF * evtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_15_18 has bin number -1." << std::endl;
        }
        if(Val_bin_15_18_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation_high_MET(nb_JESUp,nSV_JESUp,ISRpt_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_low_JES_up->Fill(bin_num_JESUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation_high_MET(nb_JESDown,nSV_JESDown,ISRpt_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_low_JES_down->Fill(bin_num_JESDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_low_METUnClust_up->Fill(bin_num_METUnClustUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_low_METUnClust_down->Fill(bin_num_METUnClustDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //Bins 19-42: SBv3_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met)
        if(Val_bin_19_42)
        {
            bin_num = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,met);
            if(bin_num != -1)
            {
                h_vb_high->Fill(bin_num,SF*evtWeight);
                h_njets_high_middphi->Fill(njets,SF*evtWeight);
                h_vb_high_bsf_up->Fill(bin_num, SF * evtWeight * B_SF_Up / B_SF);
                h_vb_high_bsf_down->Fill(bin_num, SF * evtWeight * B_SF_Down / B_SF);
                h_vb_high_trig_eff_up->Fill(bin_num, SF * evtWeight * trigger_eff_up / trigger_eff);
                h_vb_high_trig_eff_down->Fill(bin_num, SF * evtWeight * trigger_eff_down / trigger_eff);
                h_vb_high_puWeight_up->Fill(bin_num, SF * evtWeight * puWeightUp / puWeight);
                h_vb_high_puWeight_down->Fill(bin_num, SF * evtWeight * puWeightDown / puWeight);
                h_vb_high_PFWeight_up->Fill(bin_num, SF * evtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_high_PFWeight_down->Fill(bin_num, SF * evtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_high_pdfWeight_up->Fill(bin_num, SF * evtWeight * pdfWeight_Up);
                h_vb_high_pdfWeight_down->Fill(bin_num, SF * evtWeight * pdfWeight_Down);
                h_vb_high_SF_up->Fill(bin_num, SF_up * evtWeight);
                h_vb_high_SF_down->Fill(bin_num, SF_down * evtWeight);
                h_vb_high_ivfunc_up->Fill(bin_num, SF * evtWeight * SB_SF_up / SB_SF);
                h_vb_high_ivfunc_down->Fill(bin_num, SF * evtWeight * SB_SF_down / SB_SF);
                h_vb_high_eff_e_up->Fill(bin_num, SF * evtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_high_eff_e_down->Fill(bin_num, SF * evtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_high_err_mu_up->Fill(bin_num, SF * evtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_high_err_mu_down->Fill(bin_num, SF * evtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_high_eff_tau_up->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_high_eff_tau_down->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_high_eff_wtag_up->Fill(bin_num, SF * evtWeight * W_SF_up / W_SF);
                h_vb_high_eff_wtag_down->Fill(bin_num, SF * evtWeight * W_SF_down / W_SF);
                h_vb_high_eff_toptag_up->Fill(bin_num, SF * evtWeight * MergedTop_SF_up / MergedTop_SF);
                h_vb_high_eff_toptag_down->Fill(bin_num, SF * evtWeight * MergedTop_SF_down / MergedTop_SF);
                h_vb_high_eff_restoptag_up->Fill(bin_num, SF * evtWeight * ResTop_SF_up / ResTop_SF);
                h_vb_high_eff_restoptag_down->Fill(bin_num, SF * evtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_19_42 has bin number -1." << std::endl;
        }
        if(Val_bin_19_42_JESUp)
        {
            bin_num_JESUp = SBv3_highdm_validation(mtb_JESUp,njets_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,nb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_high_JES_up->Fill(bin_num_JESUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_JESDown)
        {
            bin_num_JESDown = SBv3_highdm_validation(mtb_JESDown,njets_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,nb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_high_JES_down->Fill(bin_num_JESDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustUp)
        {
            bin_num_METUnClustUp = SBv3_highdm_validation(mtb_METUnClustUp,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_high_METUnClust_up->Fill(bin_num_METUnClustUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustDown)
        {
            bin_num_METUnClustDown = SBv3_highdm_validation(mtb_METUnClustDown,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_high_METUnClust_down->Fill(bin_num_METUnClustDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //int SB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Search_lowdm)
        {
            bin_num = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            if(bin_num != -1)
            {
                h_sb_low->Fill(bin_num,SF*evtWeight);
                h_sb_low_bsf_up->Fill(bin_num, SF * evtWeight * B_SF_Up / B_SF);
                h_sb_low_bsf_down->Fill(bin_num, SF * evtWeight * B_SF_Down / B_SF);
                h_sb_low_trig_eff_up->Fill(bin_num, SF * evtWeight * trigger_eff_up / trigger_eff);
                h_sb_low_trig_eff_down->Fill(bin_num, SF * evtWeight * trigger_eff_down / trigger_eff);
                h_sb_low_puWeight_up->Fill(bin_num, SF * evtWeight * puWeightUp / puWeight);
                h_sb_low_puWeight_down->Fill(bin_num, SF * evtWeight * puWeightDown / puWeight);
                h_sb_low_PFWeight_up->Fill(bin_num, SF * evtWeight * PrefireWeightUp / PrefireWeight);
                h_sb_low_PFWeight_down->Fill(bin_num, SF * evtWeight * PrefireWeightDown / PrefireWeight);
                h_sb_low_pdfWeight_up->Fill(bin_num, SF * evtWeight * pdfWeight_Up);
                h_sb_low_pdfWeight_down->Fill(bin_num, SF * evtWeight * pdfWeight_Down);
                h_sb_low_SF_up->Fill(bin_num, SF_up * evtWeight);
                h_sb_low_SF_down->Fill(bin_num, SF_down * evtWeight);
                h_sb_low_ivfunc_up->Fill(bin_num, SF * evtWeight * SB_SF_up / SB_SF);
                h_sb_low_ivfunc_down->Fill(bin_num, SF * evtWeight * SB_SF_down / SB_SF);
                h_sb_low_eff_e_up->Fill(bin_num, SF * evtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_sb_low_eff_e_down->Fill(bin_num, SF * evtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_sb_low_err_mu_up->Fill(bin_num, SF * evtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_sb_low_err_mu_down->Fill(bin_num, SF * evtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_sb_low_eff_tau_up->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_sb_low_eff_tau_down->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_sb_low_eff_wtag_up->Fill(bin_num, SF * evtWeight * W_SF_up / W_SF);
                h_sb_low_eff_wtag_down->Fill(bin_num, SF * evtWeight * W_SF_down / W_SF);
                h_sb_low_eff_toptag_up->Fill(bin_num, SF * evtWeight * MergedTop_SF_up / MergedTop_SF);
                h_sb_low_eff_toptag_down->Fill(bin_num, SF * evtWeight * MergedTop_SF_down / MergedTop_SF);
                h_sb_low_eff_restoptag_up->Fill(bin_num, SF * evtWeight * ResTop_SF_up / ResTop_SF);
                h_sb_low_eff_restoptag_down->Fill(bin_num, SF * evtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Search_lowdm_JESUp)
        {
            bin_num_JESUp = SB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_sb_low_JES_up->Fill(bin_num_JESUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_JESDown)
        {
            bin_num_JESDown = SB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_sb_low_JES_down->Fill(bin_num_JESDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_lowdm_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_sb_low_METUnClust_up->Fill(bin_num_METUnClustUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_sb_low_METUnClust_down->Fill(bin_num_METUnClustDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //int SBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Search_highdm)
        {
            bin_num = SBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            if(bin_num != -1)
            {
                h_sb_high->Fill(bin_num,SF*evtWeight);
                h_sb_high_bsf_up->Fill(bin_num, SF * evtWeight * B_SF_Up / B_SF);
                h_sb_high_bsf_down->Fill(bin_num, SF * evtWeight * B_SF_Down / B_SF);
                h_sb_high_trig_eff_up->Fill(bin_num, SF * evtWeight * trigger_eff_up / trigger_eff);
                h_sb_high_trig_eff_down->Fill(bin_num, SF * evtWeight * trigger_eff_down / trigger_eff);
                h_sb_high_puWeight_up->Fill(bin_num, SF * evtWeight * puWeightUp / puWeight);
                h_sb_high_puWeight_down->Fill(bin_num, SF * evtWeight * puWeightDown / puWeight);
                h_sb_high_PFWeight_up->Fill(bin_num, SF * evtWeight * PrefireWeightUp / PrefireWeight);
                h_sb_high_PFWeight_down->Fill(bin_num, SF * evtWeight * PrefireWeightDown / PrefireWeight);
                h_sb_high_pdfWeight_up->Fill(bin_num, SF * evtWeight * pdfWeight_Up);
                h_sb_high_pdfWeight_down->Fill(bin_num, SF * evtWeight * pdfWeight_Down);
                h_sb_high_SF_up->Fill(bin_num, SF_up * evtWeight);
                h_sb_high_SF_down->Fill(bin_num, SF_down * evtWeight);
                h_sb_high_ivfunc_up->Fill(bin_num, SF * evtWeight * SB_SF_up / SB_SF);
                h_sb_high_ivfunc_down->Fill(bin_num, SF * evtWeight * SB_SF_down / SB_SF);
                h_sb_high_eff_e_up->Fill(bin_num, SF * evtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_sb_high_eff_e_down->Fill(bin_num, SF * evtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_sb_high_err_mu_up->Fill(bin_num, SF * evtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_sb_high_err_mu_down->Fill(bin_num, SF * evtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_sb_high_eff_tau_up->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_sb_high_eff_tau_down->Fill(bin_num, SF * evtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_sb_high_eff_wtag_up->Fill(bin_num, SF * evtWeight * W_SF_up / W_SF);
                h_sb_high_eff_wtag_down->Fill(bin_num, SF * evtWeight * W_SF_down / W_SF);
                h_sb_high_eff_toptag_up->Fill(bin_num, SF * evtWeight * MergedTop_SF_up / MergedTop_SF);
                h_sb_high_eff_toptag_down->Fill(bin_num, SF * evtWeight * MergedTop_SF_down / MergedTop_SF);
                h_sb_high_eff_restoptag_up->Fill(bin_num, SF * evtWeight * ResTop_SF_up / ResTop_SF);
                h_sb_high_eff_restoptag_down->Fill(bin_num, SF * evtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Search_highdm_JESUp)
        {
            bin_num_JESUp = SBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_sb_high_JES_up->Fill(bin_num_JESUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_JESDown)
        {
            bin_num_JESDown = SBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_sb_high_JES_down->Fill(bin_num_JESDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_highdm_METUnClustUp)
        {
            bin_num_METUnClustUp = SBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_sb_high_METUnClust_up->Fill(bin_num_METUnClustUp,SF*evtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_METUnClustDown)
        {
            bin_num_METUnClustDown = SBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_sb_high_METUnClust_down->Fill(bin_num_METUnClustDown,SF*evtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        if(reset > 1) std::cout << "Error: Bin number reset multiple times in event " << tr.getEvtNum() << "." << std::endl;
    }

    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    eff_h->Write();
    out_file.cd("../");
    h_njets_low_middphi->Write();
    h_njets_high_middphi->Write();
    h_vb_low->Write();
    h_vb_low_bsf_up->Write();
    h_vb_low_bsf_down->Write();
    h_vb_low_trig_eff_up->Write();
    h_vb_low_trig_eff_down->Write();
    h_vb_low_puWeight_up->Write();
    h_vb_low_puWeight_down->Write();
    h_vb_low_PFWeight_up->Write();
    h_vb_low_PFWeight_down->Write();
    h_vb_low_pdfWeight_up->Write();
    h_vb_low_pdfWeight_down->Write();
    h_vb_low_JES_up->Write();
    h_vb_low_JES_down->Write();
    h_vb_low_SF_up->Write();
    h_vb_low_SF_down->Write();
    h_vb_low_METUnClust_up->Write();
    h_vb_low_METUnClust_down->Write();
    h_vb_low_ivfunc_up->Write();
    h_vb_low_ivfunc_down->Write();
    h_vb_low_eff_e_up->Write();
    h_vb_low_eff_e_down->Write();
    h_vb_low_err_mu_up->Write();
    h_vb_low_err_mu_down->Write();
    h_vb_low_eff_tau_up->Write();
    h_vb_low_eff_tau_down->Write();
    h_vb_low_eff_wtag_up->Write();
    h_vb_low_eff_wtag_down->Write();
    h_vb_low_eff_toptag_up->Write();
    h_vb_low_eff_toptag_down->Write();
    h_vb_low_eff_restoptag_up->Write();
    h_vb_low_eff_restoptag_down->Write();
    h_vb_high->Write();
    h_vb_high_bsf_up->Write();
    h_vb_high_bsf_down->Write();
    h_vb_high_trig_eff_up->Write();
    h_vb_high_trig_eff_down->Write();
    h_vb_high_puWeight_up->Write();
    h_vb_high_puWeight_down->Write();
    h_vb_high_PFWeight_up->Write();
    h_vb_high_PFWeight_down->Write();
    h_vb_high_pdfWeight_up->Write();
    h_vb_high_pdfWeight_down->Write();
    h_vb_high_JES_up->Write();
    h_vb_high_JES_down->Write();
    h_vb_high_SF_up->Write();
    h_vb_high_SF_down->Write();
    h_vb_high_METUnClust_up->Write();
    h_vb_high_METUnClust_down->Write();
    h_vb_high_ivfunc_up->Write();
    h_vb_high_ivfunc_down->Write();
    h_vb_high_eff_e_up->Write();
    h_vb_high_eff_e_down->Write();
    h_vb_high_err_mu_up->Write();
    h_vb_high_err_mu_down->Write();
    h_vb_high_eff_tau_up->Write();
    h_vb_high_eff_tau_down->Write();
    h_vb_high_eff_wtag_up->Write();
    h_vb_high_eff_wtag_down->Write();
    h_vb_high_eff_toptag_up->Write();
    h_vb_high_eff_toptag_down->Write();
    h_vb_high_eff_restoptag_up->Write();
    h_vb_high_eff_restoptag_down->Write();
    h_sb_low->Write();
    h_sb_low_bsf_up->Write();
    h_sb_low_bsf_down->Write();
    h_sb_low_trig_eff_up->Write();
    h_sb_low_trig_eff_down->Write();
    h_sb_low_puWeight_up->Write();
    h_sb_low_puWeight_down->Write();
    h_sb_low_PFWeight_up->Write();
    h_sb_low_PFWeight_down->Write();
    h_sb_low_pdfWeight_up->Write();
    h_sb_low_pdfWeight_down->Write();
    h_sb_low_JES_up->Write();
    h_sb_low_JES_down->Write();
    h_sb_low_SF_up->Write();
    h_sb_low_SF_down->Write();
    h_sb_low_METUnClust_up->Write();
    h_sb_low_METUnClust_down->Write();
    h_sb_low_ivfunc_up->Write();
    h_sb_low_ivfunc_down->Write();
    h_sb_low_eff_e_up->Write();
    h_sb_low_eff_e_down->Write();
    h_sb_low_err_mu_up->Write();
    h_sb_low_err_mu_down->Write();
    h_sb_low_eff_tau_up->Write();
    h_sb_low_eff_tau_down->Write();
    h_sb_low_eff_wtag_up->Write();
    h_sb_low_eff_wtag_down->Write();
    h_sb_low_eff_toptag_up->Write();
    h_sb_low_eff_toptag_down->Write();
    h_sb_low_eff_restoptag_up->Write();
    h_sb_low_eff_restoptag_down->Write();
    h_sb_high->Write();
    h_sb_high_bsf_up->Write();
    h_sb_high_bsf_down->Write();
    h_sb_high_trig_eff_up->Write();
    h_sb_high_trig_eff_down->Write();
    h_sb_high_puWeight_up->Write();
    h_sb_high_puWeight_down->Write();
    h_sb_high_PFWeight_up->Write();
    h_sb_high_PFWeight_down->Write();
    h_sb_high_pdfWeight_up->Write();
    h_sb_high_pdfWeight_down->Write();
    h_sb_high_JES_up->Write();
    h_sb_high_JES_down->Write();
    h_sb_high_SF_up->Write();
    h_sb_high_SF_down->Write();
    h_sb_high_METUnClust_up->Write();
    h_sb_high_METUnClust_down->Write();
    h_sb_high_ivfunc_up->Write();
    h_sb_high_ivfunc_down->Write();
    h_sb_high_eff_e_up->Write();
    h_sb_high_eff_e_down->Write();
    h_sb_high_err_mu_up->Write();
    h_sb_high_err_mu_down->Write();
    h_sb_high_eff_tau_up->Write();
    h_sb_high_eff_tau_down->Write();
    h_sb_high_eff_wtag_up->Write();
    h_sb_high_eff_wtag_down->Write();
    h_sb_high_eff_toptag_up->Write();
    h_sb_high_eff_toptag_down->Write();
    h_sb_high_eff_restoptag_up->Write();
    h_sb_high_eff_restoptag_down->Write();
    out_file.Close();

    return 0;

}

