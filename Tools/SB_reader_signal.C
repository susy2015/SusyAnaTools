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
//#include "ZInvisible/Tools/include/GetSearchBin.h"
#include "GetVectors.h"
//#include "CleanedJets.h"
#include "RunTopTagger.h"
#include "ZInvisible/Tools/include/Gamma.h"
#include "SB2018.h"
#include <getopt.h>
#include "units.hh"

int analyze(std::string filename, std::string era, int max_events, bool isData, bool isSignal, bool PeriodF, bool PostHEM, float SF, bool verbose)
{
    TChain *ch = new TChain("Events");
    ch->Add(filename.c_str());
    std::string samplename;
    samplename = filename.substr(filename.find("SMS_"),filename.find(".root")-filename.find("SMS_"));
    if(samplename.find("/") != std::string::npos) samplename = samplename.substr(samplename.find("/") + 1);
    if(samplename.find("Skim") != std::string::npos) samplename = samplename.substr(0,samplename.find("Skim") - 1);
    std::cout << samplename << std::endl;

    if(verbose) std::cout << "Setting up" << std::endl;

    NTupleReader tr(ch);
    GetVectors getVectors;
    plotterFunctions::BasicLepton basicLepton;
    plotterFunctions::Gamma gamma(era);
    //CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    BaselineVessel blv(tr,era,"");
    //BaselineVessel blv_jetpt30(tr,era,"_jetpt30");
    //plotterFunctions::GetSearchBin getsearchbin;
    
    if(verbose) std::cout << "Registering getVectors" << std::endl;

    tr.registerFunction(getVectors);
    tr.registerFunction(gamma);
    tr.registerFunction(basicLepton);
    //tr.registerFunction(cleanedJets);
    if(verbose) std::cout << "Registering runTopTagger" << std::endl;
    tr.registerFunction(runTopTagger);
    if(verbose) std::cout << "Registering blv" << std::endl;
    tr.registerFunction(blv);
    //if(verbose) std::cout << "Registering blv_jetpt30" << std::endl;
    //tr.registerFunction(blv_jetpt30);
    //if(verbose) std::cout << "Registering getsearchbin" << std::endl;
    //tr.registerFunction(getsearchbin);
    if(verbose) std::cout << "Done registering" << std::endl;

    auto *h_vb = new TH1F("h_vb","Validation Bins",43,0,43);
    auto *h_vb_bsf_up = new TH1F("h_vb_bsf_up","VB B SF Up",43,0,43);
    auto *h_vb_bsf_down = new TH1F("h_vb_bsf_down","VB B SF Down",43,0,43);
    auto *h_vb_trig_eff_up = new TH1F("h_vb_trig_eff_up","VB Trigger Efficiency Up",43,0,43);
    auto *h_vb_trig_eff_down = new TH1F("h_vb_trig_eff_down","VB Trigger Efficiency Down",43,0,43);
    auto *h_vb_puWeight_up = new TH1F("h_vb_puWeight_up","VB PU Weight Up",43,0,43);
    auto *h_vb_puWeight_down = new TH1F("h_vb_puWeight_down","VB PU Weight Down",43,0,43);
    auto *h_vb_PFWeight_up = new TH1F("h_vb_PFWeight_up","VB PF Weight Up",43,0,43);
    auto *h_vb_PFWeight_down = new TH1F("h_vb_PFWeight_down","VB PF Weight Down",43,0,43);
    auto *h_vb_pdfWeight_up = new TH1F("h_vb_pdfWeight_up","VB PDF Weight Up",43,0,43);
    auto *h_vb_pdfWeight_down = new TH1F("h_vb_pdfWeight_down","VB PDF Weight Down",43,0,43);
    auto *h_vb_JES_up = new TH1F("h_vb_JES_up","VB JES Up",43,0,43);
    auto *h_vb_JES_down = new TH1F("h_vb_JES_down","VB JES Down",43,0,43);
    auto *h_vb_METUnClust_up = new TH1F("h_vb_METUnClust_up","VB METUnClust Up",43,0,43);
    auto *h_vb_METUnClust_down = new TH1F("h_vb_METUnClust_down","VB METUnClust Down",43,0,43);
    auto *h_vb_ivfunc_up = new TH1F("h_vb_ivfunc_up","VB Soft B Tag SF Up",43,0,43);
    auto *h_vb_ivfunc_down = new TH1F("h_vb_ivfunc_down","VB Soft B Tag SF Down",43,0,43);
    auto *h_vb_eff_e_up = new TH1F("h_vb_eff_e_up","VB Electron Veto SF Up",43,0,43);
    auto *h_vb_eff_e_down = new TH1F("h_vb_eff_e_down","VB Electron Veto SF Down",43,0,43);
    auto *h_vb_err_mu_up = new TH1F("h_vb_err_mu_up","VB Muon Loose SF Up",43,0,43);
    auto *h_vb_err_mu_down = new TH1F("h_vb_err_mu_down","VB Muon Loose SF Down",43,0,43);
    auto *h_vb_eff_tau_up = new TH1F("h_vb_eff_tau_up","VB Tau POG SF Up",43,0,43);
    auto *h_vb_eff_tau_down = new TH1F("h_vb_eff_tau_down","VB Tau POG SF Down",43,0,43);
    auto *h_vb_eff_restoptag_up = new TH1F("h_vb_eff_restoptag_up","VB Resolved Top Tag SF Up",43,0,43);
    auto *h_vb_eff_restoptag_down = new TH1F("h_vb_eff_restoptag_down","VB Resolved Top Tag SF Down",43,0,43);
    auto *h_vb_eff_toptag_up = new TH1F("h_vb_eff_toptag_up","VB Merged Top Tag SF Up",43,0,43);
    auto *h_vb_eff_toptag_down = new TH1F("h_vb_eff_toptag_down","VB Merged Top Tag SF Down",43,0,43);
    auto *h_vb_eff_wtag_up = new TH1F("h_vb_eff_wtag_up","VB W Tag SF Up",43,0,43);
    auto *h_vb_eff_wtag_down = new TH1F("h_vb_eff_wtag_down","VB W TagSF Down",43,0,43);
    //auto *h_vb_ak8jet_sf_up = new TH1F("h_vb_ak8jet_sf_up","VB AK8Jet SF Up",43,0,43);
    //auto *h_vb_ak8jet_sf_down = new TH1F("h_vb_ak8jet_sf_down","VB AK8Jet SF Down",43,0,43);
    auto *h_vb_ISRWeight_up = new TH1F("h_vb_ISRWeight_up","VB ISR Weight Up",43,0,43);
    auto *h_vb_ISRWeight_down = new TH1F("h_vb_ISRWeight_down","VB ISR Weight Down",43,0,43);
    auto *h_vb_fastSF_up = new TH1F("h_vb_fastSF_up","VB Fastsim SF Up",43,0,43);
    auto *h_vb_fastSF_down = new TH1F("h_vb_fastSF_down","VB Fastsim SF Down",43,0,43);
    auto *h_vb_METunc_up = new TH1F("h_vb_METunc_up","VB MET uncertainty Up",43,0,43);
    auto *h_vb_METunc_down = new TH1F("h_vb_METunc_down","VB MET uncertainty Down",43,0,43);

    auto *h_sb = new TH1F("h_sb","Search Bins",183,0,183);
    auto *h_sb_bsf_up = new TH1F("h_sb_bsf_up","SB B SF Up",183,0,183);
    auto *h_sb_bsf_down = new TH1F("h_sb_bsf_down","SB B SF Down",183,0,183);
    auto *h_sb_trig_eff_up = new TH1F("h_sb_trig_eff_up","SB Trigger Efficiency Up",183,0,183);
    auto *h_sb_trig_eff_down = new TH1F("h_sb_trig_eff_down","SB Trigger Efficiency Down",183,0,183);
    auto *h_sb_puWeight_up = new TH1F("h_sb_puWeight_up","SB PU Weight Up",183,0,183);
    auto *h_sb_puWeight_down = new TH1F("h_sb_puWeight_down","SB PU Weight Down",183,0,183);
    auto *h_sb_PFWeight_up = new TH1F("h_sb_PFWeight_up","SB PF Weight Up",183,0,183);
    auto *h_sb_PFWeight_down = new TH1F("h_sb_PFWeight_down","SB PF Weight Down",183,0,183);
    auto *h_sb_pdfWeight_up = new TH1F("h_sb_pdfWeight_up","SB PDF Weight Up",183,0,183);
    auto *h_sb_pdfWeight_down = new TH1F("h_sb_pdfWeight_down","SB PDF Weight Down",183,0,183);
    auto *h_sb_JES_up = new TH1F("h_sb_JES_up","SB JES Up",183,0,183);
    auto *h_sb_JES_down = new TH1F("h_sb_JES_down","SB JES Down",183,0,183);
    auto *h_sb_METUnClust_up = new TH1F("h_sb_METUnClust_up","SB METUnClust Up",183,0,183);
    auto *h_sb_METUnClust_down = new TH1F("h_sb_METUnClust_down","SB METUnClust Down",183,0,183);
    auto *h_sb_ivfunc_up = new TH1F("h_sb_ivfunc_up","SB Soft B Tag SF Up",183,0,183);
    auto *h_sb_ivfunc_down = new TH1F("h_sb_ivfunc_down","SB Soft B Tag SF Down",183,0,183);
    auto *h_sb_eff_e_up = new TH1F("h_sb_eff_e_up","SB Electron Veto SF Up",183,0,183);
    auto *h_sb_eff_e_down = new TH1F("h_sb_eff_e_down","SB Electron Veto SF Down",183,0,183);
    auto *h_sb_err_mu_up = new TH1F("h_sb_err_mu_up","SB Muon Loose SF Up",183,0,183);
    auto *h_sb_err_mu_down = new TH1F("h_sb_err_mu_down","SB Muon Loose SF Down",183,0,183);
    auto *h_sb_eff_tau_up = new TH1F("h_sb_eff_tau_up","SB Tau POG SF Up",183,0,183);
    auto *h_sb_eff_tau_down = new TH1F("h_sb_eff_tau_down","SB Tau POG SF Down",183,0,183);
    auto *h_sb_eff_restoptag_up = new TH1F("h_sb_eff_restoptag_up","SB Resolved Top Tag SF Up",183,0,183);
    auto *h_sb_eff_restoptag_down = new TH1F("h_sb_eff_restoptag_down","SB Resolved Top Tag SF Down",183,0,183);
    auto *h_sb_eff_toptag_up = new TH1F("h_sb_eff_toptag_up","SB Merged Top Tag SF Up",183,0,183);
    auto *h_sb_eff_toptag_down = new TH1F("h_sb_eff_toptag_down","SB Merged Top Tag SF Down",183,0,183);
    auto *h_sb_eff_wtag_up = new TH1F("h_sb_eff_wtag_up","SB W Tag SF Up",183,0,183);
    auto *h_sb_eff_wtag_down = new TH1F("h_sb_eff_wtag_down","SB W TagSF Down",183,0,183);
    //auto *h_sb_ak8jet_sf_up = new TH1F("h_sb_ak8jet_sf_up","SB AK8Jet SF Up",183,0,183);
    //auto *h_sb_ak8jet_sf_down = new TH1F("h_sb_ak8jet_sf_down","SB AK8Jet SF Down",183,0,183);
    auto *h_sb_ISRWeight_up = new TH1F("h_sb_ISRWeight_up","SB ISR Weight Up",183,0,183);
    auto *h_sb_ISRWeight_down = new TH1F("h_sb_ISRWeight_down","SB ISR Weight Down",183,0,183);
    auto *h_sb_fastSF_up = new TH1F("h_sb_fastSF_up","SB Fastsim SF Up",183,0,183);
    auto *h_sb_fastSF_down = new TH1F("h_sb_fastSF_down","SB Fastsim SF Down",183,0,183);
    auto *h_sb_METunc_up = new TH1F("h_sb_METunc_up","SB MET uncertainty Up",183,0,183);
    auto *h_sb_METunc_down = new TH1F("h_sb_METunc_down","SB MET uncertainty Down",183,0,183);

    auto *h_ub = new TH1F("h_ub","LL CR Unit Bins",112,0,112);
    auto *h_ub_bsf_up = new TH1F("h_ub_bsf_up","UB B SF Up",112,0,112);
    auto *h_ub_bsf_down = new TH1F("h_ub_bsf_down","UB B SF Down",112,0,112);
    auto *h_ub_trig_eff_up = new TH1F("h_ub_trig_eff_up","UB Trigger Efficiency Up",112,0,112);
    auto *h_ub_trig_eff_down = new TH1F("h_ub_trig_eff_down","UB Trigger Efficiency Down",112,0,112);
    auto *h_ub_puWeight_up = new TH1F("h_ub_puWeight_up","UB PU Weight Up",112,0,112);
    auto *h_ub_puWeight_down = new TH1F("h_ub_puWeight_down","UB PU Weight Down",112,0,112);
    auto *h_ub_PFWeight_up = new TH1F("h_ub_PFWeight_up","UB PF Weight Up",112,0,112);
    auto *h_ub_PFWeight_down = new TH1F("h_ub_PFWeight_down","UB PF Weight Down",112,0,112);
    auto *h_ub_pdfWeight_up = new TH1F("h_ub_pdfWeight_up","UB PDF Weight Up",112,0,112);
    auto *h_ub_pdfWeight_down = new TH1F("h_ub_pdfWeight_down","UB PDF Weight Down",112,0,112);
    auto *h_ub_JES_up = new TH1F("h_ub_JES_up","UB JES Up",112,0,112);
    auto *h_ub_JES_down = new TH1F("h_ub_JES_down","UB JES Down",112,0,112);
    auto *h_ub_METUnClust_up = new TH1F("h_ub_METUnClust_up","UB METUnClust Up",112,0,112);
    auto *h_ub_METUnClust_down = new TH1F("h_ub_METUnClust_down","UB METUnClust Down",112,0,112);
    auto *h_ub_ivfunc_up = new TH1F("h_ub_ivfunc_up","UB Soft B Tag SF Up",112,0,112);
    auto *h_ub_ivfunc_down = new TH1F("h_ub_ivfunc_down","UB Soft B Tag SF Down",112,0,112);
    auto *h_ub_eff_e_up = new TH1F("h_ub_eff_e_up","UB Electron Veto SF Up",112,0,112);
    auto *h_ub_eff_e_down = new TH1F("h_ub_eff_e_down","UB Electron Veto SF Down",112,0,112);
    auto *h_ub_err_mu_up = new TH1F("h_ub_err_mu_up","UB Muon Loose SF Up",112,0,112);
    auto *h_ub_err_mu_down = new TH1F("h_ub_err_mu_down","UB Muon Loose SF Down",112,0,112);
    auto *h_ub_eff_tau_up = new TH1F("h_ub_eff_tau_up","UB Tau POG SF Up",112,0,112);
    auto *h_ub_eff_tau_down = new TH1F("h_ub_eff_tau_down","UB Tau POG SF Down",112,0,112);
    auto *h_ub_eff_restoptag_up = new TH1F("h_ub_eff_restoptag_up","UB Resolved Top Tag SF Up",112,0,112);
    auto *h_ub_eff_restoptag_down = new TH1F("h_ub_eff_restoptag_down","UB Resolved Top Tag SF Down",112,0,112);
    auto *h_ub_eff_toptag_up = new TH1F("h_ub_eff_toptag_up","UB Merged Top Tag SF Up",112,0,112);
    auto *h_ub_eff_toptag_down = new TH1F("h_ub_eff_toptag_down","UB Merged Top Tag SF Down",112,0,112);
    auto *h_ub_eff_wtag_up = new TH1F("h_ub_eff_wtag_up","UB W Tag SF Up",112,0,112);
    auto *h_ub_eff_wtag_down = new TH1F("h_ub_eff_wtag_down","UB W TagSF Down",112,0,112);
    //auto *h_ub_ak8jet_sf_up = new TH1F("h_ub_ak8jet_sf_up","UB AK8Jet SF Up",112,0,112);
    //auto *h_ub_ak8jet_sf_down = new TH1F("h_ub_ak8jet_sf_down","UB AK8Jet SF Down",112,0,112);
    auto *h_ub_ISRWeight_up = new TH1F("h_ub_ISRWeight_up","UB ISR Weight Up",112,0,112);
    auto *h_ub_ISRWeight_down = new TH1F("h_ub_ISRWeight_down","UB ISR Weight Down",112,0,112);
    auto *h_ub_fastSF_up = new TH1F("h_ub_fastSF_up","UB Fastsim SF Up",112,0,112);
    auto *h_ub_fastSF_down = new TH1F("h_ub_fastSF_down","UB Fastsim SF Down",112,0,112);
    auto *h_ub_METunc_up = new TH1F("h_ub_METunc_up","UB MET uncertainty Up",112,0,112);
    auto *h_ub_METunc_down = new TH1F("h_ub_METunc_down","UB MET uncertainty Down",112,0,112);

    auto *h_qb = new TH1F("h_qb","QCD CR Unit Bins",92,0,92);
    auto *h_qb_bsf_up = new TH1F("h_qb_bsf_up","QCD CR B SF Up",92,0,92);
    auto *h_qb_bsf_down = new TH1F("h_qb_bsf_down","QCD CR B SF Down",92,0,92);
    auto *h_qb_trig_eff_up = new TH1F("h_qb_trig_eff_up","QCD CR Trigger Efficiency Up",92,0,92);
    auto *h_qb_trig_eff_down = new TH1F("h_qb_trig_eff_down","QCD CR Trigger Efficiency Down",92,0,92);
    auto *h_qb_puWeight_up = new TH1F("h_qb_puWeight_up","QCD CR PU Weight Up",92,0,92);
    auto *h_qb_puWeight_down = new TH1F("h_qb_puWeight_down","QCD CR PU Weight Down",92,0,92);
    auto *h_qb_PFWeight_up = new TH1F("h_qb_PFWeight_up","QCD CR PF Weight Up",92,0,92);
    auto *h_qb_PFWeight_down = new TH1F("h_qb_PFWeight_down","QCD CR PF Weight Down",92,0,92);
    auto *h_qb_pdfWeight_up = new TH1F("h_qb_pdfWeight_up","QCD CR PDF Weight Up",92,0,92);
    auto *h_qb_pdfWeight_down = new TH1F("h_qb_pdfWeight_down","QCD CR PDF Weight Down",92,0,92);
    auto *h_qb_JES_up = new TH1F("h_qb_JES_up","QCD CR JES Up",92,0,92);
    auto *h_qb_JES_down = new TH1F("h_qb_JES_down","QCD CR JES Down",92,0,92);
    auto *h_qb_METUnClust_up = new TH1F("h_qb_METUnClust_up","QCD CR METUnClust Up",92,0,92);
    auto *h_qb_METUnClust_down = new TH1F("h_qb_METUnClust_down","QCD CR METUnClust Down",92,0,92);
    auto *h_qb_ivfunc_up = new TH1F("h_qb_ivfunc_up","QCD CR Soft B Tag SF Up",92,0,92);
    auto *h_qb_ivfunc_down = new TH1F("h_qb_ivfunc_down","QCD CR Soft B Tag SF Down",92,0,92);
    auto *h_qb_eff_e_up = new TH1F("h_qb_eff_e_up","QCD CR Electron Veto SF Up",92,0,92);
    auto *h_qb_eff_e_down = new TH1F("h_qb_eff_e_down","QCD CR Electron Veto SF Down",92,0,92);
    auto *h_qb_err_mu_up = new TH1F("h_qb_err_mu_up","QCD CR Muon Loose SF Up",92,0,92);
    auto *h_qb_err_mu_down = new TH1F("h_qb_err_mu_down","QCD CR Muon Loose SF Down",92,0,92);
    auto *h_qb_eff_tau_up = new TH1F("h_qb_eff_tau_up","QCD CR Tau POG SF Up",92,0,92);
    auto *h_qb_eff_tau_down = new TH1F("h_qb_eff_tau_down","QCD CR Tau POG SF Down",92,0,92);
    auto *h_qb_eff_restoptag_up = new TH1F("h_qb_eff_restoptag_up","QCD CR Resolved Top Tag SF Up",92,0,92);
    auto *h_qb_eff_restoptag_down = new TH1F("h_qb_eff_restoptag_down","QCD CR Resolved Top Tag SF Down",92,0,92);
    auto *h_qb_eff_toptag_up = new TH1F("h_qb_eff_toptag_up","QCD CR Merged Top Tag SF Up",92,0,92);
    auto *h_qb_eff_toptag_down = new TH1F("h_qb_eff_toptag_down","QCD CR Merged Top Tag SF Down",92,0,92);
    auto *h_qb_eff_wtag_up = new TH1F("h_qb_eff_wtag_up","QCD CR W Tag SF Up",92,0,92);
    auto *h_qb_eff_wtag_down = new TH1F("h_qb_eff_wtag_down","QCD CR W TagSF Down",92,0,92);
    //auto *h_qb_ak8jet_sf_up = new TH1F("h_qb_ak8jet_sf_up","QCD CR AK8Jet SF Up",92,0,92);
    //auto *h_qb_ak8jet_sf_down = new TH1F("h_qb_ak8jet_sf_down","QCD CR AK8Jet SF Down",92,0,92);
    auto *h_qb_ISRWeight_up = new TH1F("h_qb_ISRWeight_up","QCD CR ISR Weight Up",92,0,92);
    auto *h_qb_ISRWeight_down = new TH1F("h_qb_ISRWeight_down","QCD CR ISR Weight Down",92,0,92);
    auto *h_qb_fastSF_up = new TH1F("h_qb_fastSF_up","QCD CR Fastsim SF Up",92,0,92);
    auto *h_qb_fastSF_down = new TH1F("h_qb_fastSF_down","QCD CR Fastsim SF Down",92,0,92);
    auto *h_qb_METunc_up = new TH1F("h_qb_METunc_up","QCD CR MET uncertainty Up",92,0,92);
    auto *h_qb_METunc_down = new TH1F("h_qb_METunc_down","QCD CR MET uncertainty Down",92,0,92);

    if(verbose) std::cout << "bin histograms declared" << std::endl;

    //Vars will NOT be nSRUnitLowDM and nSRUnitHighDM
    //Low DM range: 0, 52 -> 53,0,53
    //High DM range: 53, 528 -> 476,53,529
    //Vars will be nCRUnitLowDM and nCRUnitHighDM
    //Range: 0,111 -> 112,0,112

    auto *h_njets_low_middphi = new TH1F("h_njets_low_middphi","NJets for Low DM mid-dphi region",25,0,25);
    auto *h_njets_high_middphi = new TH1F("h_njets_high_middphi","NJets for High DM mid-dphi region",25,0,25);

    auto *eff_h = new TH1F("eff_h","eff_h",4,0,4); //Counting events. First bin counts all, second event filter, third baselines, fourth counts total to VB.
    float numerator_genfiltereff = 0;
    float denominator_genfiltereff = 0;

    if(verbose) std::cout << "Starting while loop." << std::endl;

    while(tr.getNextEvent())
    {
        if(verbose) std::cout << "Inside while loop" << std::endl;
        if(tr.getEvtNum() == 1)
        {
            std::cout << "Outputting tuplemembers to SBR_NTupleTypes.txt" << std::endl;
            tr.printTupleMembers();
            FILE *fout = fopen("SBR_NTupleTypes.txt","w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }

        if(tr.getEvtNum() % 1000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

        if(max_events != -1 && tr.getEvtNum() > max_events) break;

        //Need sign for all events
        float sign = 1.0;

        float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
        if(Stop0l_evtWeight < 0) sign = -1.0; //Just taking the sign from Stop0l_evtWeight

        //Count all events:
        eff_h->Fill(0.,sign);

        //gen filter efficiency
        float SigWTab_SigWeightGenCutPass = 1;
        float SigWTab_SigWeightGenCut = 1;
        if(tr.checkBranch("SigWTab_SigWeightGenCut") && tr.checkBranch("SigWTab_SigWeightGenCutPass"))
        {
            SigWTab_SigWeightGenCutPass = tr.getVar<float>("SigWTab_SigWeightGenCutPass");
            SigWTab_SigWeightGenCut = tr.getVar<float>("SigWTab_SigWeightGenCut");
        }
        numerator_genfiltereff += SigWTab_SigWeightGenCutPass;
        denominator_genfiltereff += SigWTab_SigWeightGenCut;

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

        bool Pass_Baseline = tr.getVar<bool>("Pass_Baseline");
        bool Pass_Baseline_JESUp = tr.getVar<bool>("Pass_Baseline_JESUp");
        bool Pass_Baseline_JESDown = tr.getVar<bool>("Pass_Baseline_JESDown");
        bool Pass_Baseline_METUnClustUp = tr.getVar<bool>("Pass_Baseline_METUnClustUp");
        bool Pass_Baseline_METUnClustDown = tr.getVar<bool>("Pass_Baseline_METUnClustDown");
        bool Pass_LLCR = tr.getVar<bool>("Pass_LLCR");
        bool Pass_LLCR_JESUp = tr.getVar<bool>("Pass_LLCR_JESUp");
        bool Pass_LLCR_JESDown = tr.getVar<bool>("Pass_LLCR_JESDown");
        bool Pass_LLCR_METUnClustUp = tr.getVar<bool>("Pass_LLCR_METUnClustUp");
        bool Pass_LLCR_METUnClustDown = tr.getVar<bool>("Pass_LLCR_METUnClustDown");
        bool Pass_QCDCR = tr.getVar<bool>("Pass_QCDCR");
        bool Pass_QCDCR_JESUp = tr.getVar<bool>("Pass_QCDCR_JESUp");
        bool Pass_QCDCR_JESDown = tr.getVar<bool>("Pass_QCDCR_JESDown");
        bool Pass_QCDCR_METUnClustUp = tr.getVar<bool>("Pass_QCDCR_METUnClustUp");
        bool Pass_QCDCR_METUnClustDown = tr.getVar<bool>("Pass_QCDCR_METUnClustDown");
        bool Pass_dPhiMETHighDM = tr.getVar<bool>("Pass_dPhiMETHighDM"); //used in Pass_LLCR_highdm
        bool Pass_dPhiMETHighDM_JESUp = tr.getVar<bool>("Pass_dPhiMETHighDM_JESUp");
        bool Pass_dPhiMETHighDM_JESDown = tr.getVar<bool>("Pass_dPhiMETHighDM_JESDown");
        bool Pass_dPhiMETHighDM_METUnClustUp = tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustUp");
        bool Pass_dPhiMETHighDM_METUnClustDown = tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustDown");

        //Define LowDM and HighDM later (after 'fixed' restop count)

        //if(!Pass_EventFilter) continue;
        //Count events passing filter:
        //eff_h->Fill(1.,sign);

        //Gen Lepton Veto; only TTZ
        if(false)
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

        //ISR_Weight
        float ISRWeight = tr.getVar<float>("ISRWeight");
        float ISRWeight_Up = tr.getVar<float>("ISRWeight_Up");
        float ISRWeight_Down = tr.getVar<float>("ISRWeight_Down");

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
        auto ResolvedTopCandidate_pt = tr.getVec<float>("ResolvedTopCandidate_pt");
        auto ResolvedTopCandidate_eta = tr.getVec<float>("ResolvedTopCandidate_eta");
        auto ResolvedTopCandidate_phi = tr.getVec<float>("ResolvedTopCandidate_phi");
        auto ResolvedTopCandidate_mass = tr.getVec<float>("ResolvedTopCandidate_mass");

        float ResTop_SF = 1.0;
        float ResTop_SF_up = 0.0;
        float ResTop_SF_down = 0.0;

        float test_sf = 1.0;
        float test_sf_cspur_up = 1.0;
        float test_sf_stat_up = 1.0;
        float test_sf_cspur_down = 1.0;
        float test_sf_stat_down = 1.0;

        int n_falseres = 0;
        for(int v = 0; v < ResolvedTop_Stop0l.size(); v++)
        {
            if(ResolvedTop_Stop0l[v])
            {
                if(ResolvedTopCandidate_pt[v] >= 1000 && ResolvedTopCandidate_sf[v] == 0)
                {
                    n_falseres++;
                }
                else
                {
                    ResTop_SF *= ResolvedTopCandidate_sf[v];
                    ResTop_SF_up = ResTop_SF_up + std::pow(ResTopCand_CSPur_Up[v],2) + std::pow(ResTopCand_Stat_Up[v],2) + std::pow(ResTopCand_Btag_Up[v],2) + std::pow(ResTopCand_Pileup_Up[v],2);
                    ResTop_SF_down = ResTop_SF_down + std::pow(ResTopCand_CSPur_Down[v],2) + std::pow(ResTopCand_Stat_Down[v],2) + std::pow(ResTopCand_Btag_Down[v],2) + std::pow(ResTopCand_Pileup_Down[v],2);
                    if(!ResolvedTopCandidate_genMatch[v])
                    {
                        ResTop_SF_up += std::pow(ResTopCand_Closure_Up[v],2);
                        ResTop_SF_down += std::pow(ResTopCand_Closure_Down[v],2);
                    }
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

        //used for crosscheck
        //std::vector<float> Top_SF_vec = tr.getVec<float>("FatJet_SF");
        //float Top_SF = 1.0;
        //MergedTop_SF = 1.0;
        //W_SF = 1.0;
        //ResTop_SF = 1.0;
        //for(int i = 0; i < Top_SF_vec.size(); i++)
        //{Top_SF = Top_SF * Top_SF_vec.at(i);}

        //MET uncertainty
        //doesn't seem to be a SF, but rather error. Listed in MET var section.

        //fastsim SF
        auto SB_fastSF = tr.getVec<float>("SB_fastSF");
        auto SB_fastSFerr = tr.getVec<float>("SB_fastSFerr");
        auto FatJet_fastSF = tr.getVec<float>("FatJet_fastSF");
        auto FatJet_fastSFerr = tr.getVec<float>("FatJet_fastSFerr");

        float fastSF = 1.0;
        float fastSFerr = 0.0;
        for(int v = 0; v < SB_fastSF.size(); v++)
        {
            if(SB_Stop0l[v])
            {
                fastSF *= SB_fastSF[v];
                fastSFerr = fastSFerr + std::pow(SB_fastSFerr[v],2);
            }
        }
        for(int v = 0; v < FatJet_fastSF.size(); v++)
        {
            fastSF *= FatJet_fastSF[v];
            fastSFerr = fastSFerr + std::pow(FatJet_fastSFerr[v],2);
        }
        float fastSF_up = fastSF + fastSFerr;
        float fastSF_down = fastSF - fastSFerr;

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
        ntop_res -= n_falseres;
        ntop_res_JESUp -= n_falseres;
        ntop_res_JESDown -= n_falseres;

        if(ResTop_SF == 0)
        {
            std::cout << "Event Number: " << tr.getEvtNum() << "\tResTop SF: " << ResTop_SF << "\tResTop SF Up: " << ResTop_SF_up << "\tResTop SF Down: " << ResTop_SF_down << std::endl;
            std::cout << "ntop_res: " << ntop_res << "\tntop_res_JESUp: " << ntop_res_JESUp << "\tntop_res_JESDown: " << ntop_res_JESDown << std::endl;
            std::cout << "ResolvedTopCandidate pt size: " << ResolvedTopCandidate_pt.size() << "\tsf size: " << ResolvedTopCandidate_sf.size() << std::endl;
            for(int v = 0; v < ResolvedTopCandidate_pt.size(); v++)
            {
                if(ResolvedTop_Stop0l[v]) std::cout << "true ";
                else std::cout << "false ";
                std::cout << "pt: " << ResolvedTopCandidate_pt[v] << "\teta: " << ResolvedTopCandidate_eta[v] << "\tphi: " << ResolvedTopCandidate_phi[v] << "\tmass: " << ResolvedTopCandidate_mass[v] << std::endl;
            }
            for(int v = 0; v < ResolvedTopCandidate_sf.size(); v++)
            {
                std::cout << ResolvedTopCandidate_sf[v] << " ";
            }
            std::cout << std::endl;
        }

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
        auto MET_err = tr.getVar<float>("MET_pt_fasterr");
        float METunc_up = met + MET_err;
        float METunc_down = met - MET_err;

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

        //pass met: met >= 250
        bool Pass_lowDM_nodPhi_METunc_up = (Pass_EventFilter &&  tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets30") && METunc_up >= 250 && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METunc_up = Pass_EventFilter && Pass_lowDM_nodPhi_METunc_up && tr.getVar<bool>("Pass_dPhiMETLowDM");
        bool Val_bin_15_18_METunc_up = Pass_lowDM_nodPhi_METunc_up && tr.getVar<bool>("Pass_dPhiMETMedDM");
        bool Pass_highDM_no_dPhi_METunc_up = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && METunc_up >= 250 && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METunc_up = (Pass_highDM_no_dPhi_METunc_up && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm_METunc_up = Pass_EventFilter && Pass_lowDM_nodPhi_METunc_up && tr.getVar<bool>("Pass_dPhiMETLowDM");
        bool Search_highdm_METunc_up = Pass_EventFilter && Pass_highDM_no_dPhi_METunc_up && tr.getVar<bool>("Pass_dPhiMETHighDM");

        bool Pass_lowDM_nodPhi_METunc_down = (Pass_EventFilter &&  tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets30") && METunc_down >= 250 && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METunc_down = Pass_EventFilter && Pass_lowDM_nodPhi_METunc_down && tr.getVar<bool>("Pass_dPhiMETLowDM");
        bool Val_bin_15_18_METunc_down = Pass_lowDM_nodPhi_METunc_down && tr.getVar<bool>("Pass_dPhiMETMedDM");
        bool Pass_highDM_no_dPhi_METunc_down = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && METunc_down >= 250 && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METunc_down = (Pass_highDM_no_dPhi_METunc_down && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm_METunc_down = Pass_EventFilter && Pass_lowDM_nodPhi_METunc_down && tr.getVar<bool>("Pass_dPhiMETLowDM");
        bool Search_highdm_METunc_down = Pass_EventFilter && Pass_highDM_no_dPhi_METunc_down && tr.getVar<bool>("Pass_dPhiMETHighDM");

        float evtWeight = 1.0;
        float lowDMevtWeight = 1.0;
        float highDMevtWeight = 1.0;
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

            //lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * Electron_VetoSF * Muon_MediumSF * Tau_MediumSF * W_SF * MergedTop_SF * ISRWeight * fastSF * ResTop_SF;
            //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * Electron_VetoSF * Muon_MediumSF * Tau_MediumSF * W_SF * MergedTop_SF * ISRWeight * fastSF * ResTop_SF;
            //Turning off veto scale factors for full status (lepton veto as well as low dm top/w veto)
            lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * ISRWeight * fastSF;
            highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * ISRWeight * fastSF;
            //Multiply in W_SF, MergedTop_SF, and ResTop_SF as needed
            //Hui: evtWeight = evtWeight * B_SF * ISR_SF * trigger_eff * puWeight * PrefireWeight * Top_SF * fastsim_SF;
            //diff's: SB_SF, mergedtop/w sf?
            //lowDMevtWeight = evtWeight * sign * B_SF * ISRWeight * trigger_eff * puWeight * PrefireWeight * fastSF;
            //highDMevtWeight = evtWeight * sign * B_SF * ISRWeight * trigger_eff * puWeight * PrefireWeight * fastSF;
            //lowDMevtWeight = sign;
            //highDMevtWeight = sign;
        }

        //if(!Val_bin_0_14 && !Val_bin_15_18 && !Val_bin_19_42 && !Search_lowdm && !Search_highdm) continue;
        //eff_h->Fill(2.,sign);

        int bin_num = -1;
        int bin_num_JESUp = -1;
        int bin_num_JESDown = -1;
        int bin_num_METUnClustUp = -1;
        int bin_num_METUnClustDown = -1;
        int bin_num_METunc_up = -1;
        int bin_num_METunc_down = -1;
        int reset = 0;

        //Bins 0-14: int SB_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Val_bin_0_14)
        {
            bin_num = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met);
            if(bin_num != -1)
            {
                h_vb->Fill(bin_num,SF*lowDMevtWeight);
                h_vb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_vb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * W_SF_up / W_SF);
                h_vb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * W_SF_down / W_SF);
                h_vb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_up / MergedTop_SF);
                h_vb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_down / MergedTop_SF);
                h_vb_ISRWeight_up->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Up / ISRWeight);
                h_vb_ISRWeight_down->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Down / ISRWeight);
                h_vb_fastSF_up->Fill(bin_num, SF * lowDMevtWeight * fastSF_up / fastSF);
                h_vb_fastSF_down->Fill(bin_num, SF * lowDMevtWeight * fastSF_down / fastSF);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_up / ResTop_SF);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_0_14 has bin number -1." << std::endl;
        }
        if(Val_bin_0_14_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_0_14_METunc_up)
        {
            bin_num_METunc_up = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,METunc_up);
            if(bin_num_METunc_up != -1)
            {
                h_vb_METunc_up->Fill(bin_num_METunc_up,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_METunc_down)
        {
            bin_num_METunc_down = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,METunc_down);
            if(bin_num_METunc_down != -1)
            {
                h_vb_METunc_down->Fill(bin_num_METunc_down,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        //Bins 15-18: SB_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met)
        if(Val_bin_15_18)
        {
            bin_num = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met);
            if(bin_num != -1)
            {
                h_vb->Fill(bin_num,SF*lowDMevtWeight);
                h_njets_low_middphi->Fill(njets,SF*lowDMevtWeight);
                h_vb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_vb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * W_SF_up / W_SF);
                h_vb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * W_SF_down / W_SF);
                h_vb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_up / MergedTop_SF);
                h_vb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_down / MergedTop_SF);
                h_vb_ISRWeight_up->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Up / ISRWeight);
                h_vb_ISRWeight_down->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Down / ISRWeight);
                h_vb_fastSF_up->Fill(bin_num, SF * lowDMevtWeight * fastSF_up / fastSF);
                h_vb_fastSF_down->Fill(bin_num, SF * lowDMevtWeight * fastSF_down / fastSF);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_up / ResTop_SF);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_15_18 has bin number -1." << std::endl;
        }
        if(Val_bin_15_18_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation_high_MET(nb_JESUp,nSV_JESUp,ISRpt_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation_high_MET(nb_JESDown,nSV_JESDown,ISRpt_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_15_18_METunc_up)
        {
            bin_num_METunc_up = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,METunc_up);
            if(bin_num_METunc_up != -1)
            {
                h_vb_METunc_up->Fill(bin_num_METunc_up,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_METunc_down)
        {
            bin_num_METunc_down = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,METunc_down);
            if(bin_num_METunc_down != -1)
            {
                h_vb_METunc_down->Fill(bin_num_METunc_down,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        float topw_sf = 1;
        //Bins 19-42: SBv3_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met)
        if(Val_bin_19_42)
        {
            bin_num = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,met);
            if(bin_num != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_njets_high_middphi->Fill(njets,SF*highDMevtWeight * topw_sf);
                h_vb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_vb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_up / Electron_VetoSF);
                h_vb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_down / Electron_VetoSF);
                h_vb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_vb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_vb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_vb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_vb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_up / W_SF);
                h_vb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_down / W_SF);
                h_vb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_up / MergedTop_SF);
                h_vb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_down / MergedTop_SF);
                h_vb_ISRWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Up / ISRWeight);
                h_vb_ISRWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Down / ISRWeight);
                h_vb_fastSF_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_up / fastSF);
                h_vb_fastSF_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_down / fastSF);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_up / ResTop_SF);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_19_42 has bin number -1." << std::endl;
        }
        if(Val_bin_19_42_JESUp)
        {
            bin_num_JESUp = SBv3_highdm_validation(mtb_JESUp,njets_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,nb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_vb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_JESDown)
        {
            bin_num_JESDown = SBv3_highdm_validation(mtb_JESDown,njets_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,nb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_vb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustUp)
        {
            bin_num_METUnClustUp = SBv3_highdm_validation(mtb_METUnClustUp,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustDown)
        {
            bin_num_METUnClustDown = SBv3_highdm_validation(mtb_METUnClustDown,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_19_42_METunc_up)
        {
            bin_num_METunc_up = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,METunc_up);
            if(bin_num_METunc_up != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METunc_up->Fill(bin_num_METunc_up,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_METunc_down)
        {
            bin_num_METunc_down = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,METunc_down);
            if(bin_num_METunc_down != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METunc_down->Fill(bin_num_METunc_down,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        bool Pass_LowDM = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb < 175);
        bool Pass_HighDM = njets >= 5 && nb >= 1;
        bool Pass_LowDM_JESUp = ntop_res_JESUp == 0 && ntop_merge_JESUp == 0 && nw_JESUp == 0 && ISRpt_JESUp >= 200 && (nb_JESUp == 0 || mtb_JESUp < 175);
        bool Pass_LowDM_JESDown = ntop_res_JESDown == 0 && ntop_merge_JESDown == 0 && nw_JESDown == 0 && ISRpt_JESDown >= 200 && (nb_JESDown == 0 || mtb_JESDown < 175);
        bool Pass_HighDM_JESUp = njets_JESUp >= 5 && nb_JESUp >= 1;
        bool Pass_HighDM_JESDown = njets_JESDown >= 5 && nb_JESDown >= 1;
        bool Pass_LowDM_METUnClustUp = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb_METUnClustUp < 175);
        bool Pass_LowDM_METUnClustDown = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb_METUnClustDown < 175);
        bool Pass_HighDM_METUnClustUp = njets >= 5 && nb >= 1;
        bool Pass_HighDM_METUnClustDown = njets >= 5 && nb >= 1;

        //int SB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Search_lowdm)
        //if(Pass_Baseline && Pass_LowDM)
        {
            //bin_num = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //int SRbin(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = SRbin(Pass_Baseline,false,false,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                h_sb->Fill(bin_num,SF*lowDMevtWeight);
                h_sb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_sb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_sb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_sb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_sb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_sb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_sb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_sb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_sb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_sb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_sb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_sb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_sb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_sb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_sb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_sb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_sb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_sb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_sb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * W_SF_up / W_SF);
                h_sb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * W_SF_down / W_SF);
                h_sb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_up / MergedTop_SF);
                h_sb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_down / MergedTop_SF);
                h_sb_ISRWeight_up->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Up / ISRWeight);
                h_sb_ISRWeight_down->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Down / ISRWeight);
                h_sb_fastSF_up->Fill(bin_num, SF * lowDMevtWeight * fastSF_up / fastSF);
                h_sb_fastSF_down->Fill(bin_num, SF * lowDMevtWeight * fastSF_down / fastSF);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_up / ResTop_SF);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Search_lowdm_JESUp)
        {
            //bin_num_JESUp = SB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = SRbin(Pass_Baseline_JESUp,false,false,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_sb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_JESDown)
        {
            //bin_num_JESDown = SB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = SRbin(Pass_Baseline_JESDown,false,false,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_sb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = SRbin(Pass_Baseline_METUnClustUp,false,false,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_sb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = SRbin(Pass_Baseline_METUnClustDown,false,false,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_sb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_lowdm_METunc_up)
        {
            //bin_num_METunc_up = SB_lowdm(njets,nb,nSV,ISRpt,ptb,METunc_up);
            bin_num_METunc_up = SRbin(Pass_Baseline,false,false,false,false,Pass_LowDM,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                h_sb_METunc_up->Fill(bin_num_METunc_up,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_METunc_down)
        {
            //bin_num_METunc_down = SB_lowdm(njets,nb,nSV,ISRpt,ptb,METunc_down);
            bin_num_METunc_down = SRbin(Pass_Baseline,false,false,false,false,Pass_LowDM,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                h_sb_METunc_down->Fill(bin_num_METunc_down,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        //int SBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Search_highdm)
        //if(Pass_Baseline && Pass_HighDM)
        {
            //bin_num = SBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            bin_num = SRbin(Pass_Baseline,false,false,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_sb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_sb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_sb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_sb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_sb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_sb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_sb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_sb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_sb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_sb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_sb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_sb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_sb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_up / Electron_VetoSF);
                h_sb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_down / Electron_VetoSF);
                h_sb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_sb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_sb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_sb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_sb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_up / W_SF);
                h_sb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_down / W_SF);
                h_sb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_up / MergedTop_SF);
                h_sb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_down / MergedTop_SF);
                h_sb_ISRWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Up / ISRWeight);
                h_sb_ISRWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Down / ISRWeight);
                h_sb_fastSF_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_up / fastSF);
                h_sb_fastSF_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_down / fastSF);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_up / ResTop_SF);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Search_highdm_JESUp)
        {
            //bin_num_JESUp = SBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = SRbin(Pass_Baseline_JESUp,false,false,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_sb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_JESDown)
        {
            //bin_num_JESDown = SBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = SRbin(Pass_Baseline_JESDown,false,false,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_sb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = SBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = SRbin(Pass_Baseline_METUnClustUp,false,false,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = SBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = SRbin(Pass_Baseline_METUnClustDown,false,false,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_highdm_METunc_up)
        {
            //bin_num_METunc_up = SBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_up);
            bin_num_METunc_up = SRbin(Pass_Baseline,false,false,false,Pass_HighDM,false,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METunc_up->Fill(bin_num_METunc_up,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_METunc_down)
        {
            //bin_num_METunc_down = SBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_down);
            bin_num_METunc_down = SRbin(Pass_Baseline,false,false,false,Pass_HighDM,false,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METunc_down->Fill(bin_num_METunc_down,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        if(reset > 1) std::cout << "Error: Bin number reset multiple times in event " << tr.getEvtNum() << "." << std::endl;

        //int nCRUnitLowDM  = tr.getVar<int>("nCRUnitLowDM_jetpt30");
        //int nCRUnitHighDM = tr.getVar<int>("nCRUnitHighDM_jetpt30");

        bool Unit_lowdm = Pass_LLCR && Pass_LowDM && (S_met > 10);
        bool Unit_highdm = Pass_LLCR && Pass_HighDM && Pass_dPhiMETHighDM;
        bool Unit_lowdm_JESUp = Pass_LLCR_JESUp && Pass_LowDM_JESUp && (S_met_JESUp > 10);
        bool Unit_lowdm_JESDown = Pass_LLCR_JESDown && Pass_LowDM_JESDown && (S_met_JESDown > 10);
        bool Unit_highdm_JESUp = Pass_LLCR_JESUp && Pass_HighDM_JESUp && Pass_dPhiMETHighDM_JESUp;
        bool Unit_highdm_JESDown = Pass_LLCR_JESDown && Pass_HighDM_JESDown && Pass_dPhiMETHighDM_JESDown;
        bool Unit_lowdm_METUnClustUp = Pass_LLCR_METUnClustUp && Pass_LowDM_METUnClustUp && (S_met_METUnClustUp > 10);
        bool Unit_lowdm_METUnClustDown = Pass_LLCR_METUnClustDown && Pass_LowDM_METUnClustDown && (S_met_METUnClustDown > 10);
        bool Unit_highdm_METUnClustUp = Pass_LLCR_METUnClustUp && Pass_HighDM_METUnClustUp && Pass_dPhiMETHighDM_METUnClustUp;
        bool Unit_highdm_METUnClustDown = Pass_LLCR_METUnClustDown && Pass_HighDM_METUnClustDown && Pass_dPhiMETHighDM_METUnClustDown;
        bool Unit_lowdm_METuncUp = Unit_lowdm;
        bool Unit_lowdm_METuncDown = Unit_lowdm;
        bool Unit_highdm_METuncUp = Unit_highdm;
        bool Unit_highdm_METuncDown = Unit_highdm;

        //int UB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Unit_lowdm)
        {
            //bin_num = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //bin_num = nCRUnitLowDM;
            //int lepCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "Low DM UB num: " << bin_num << std::endl;
                h_ub->Fill(bin_num,SF*lowDMevtWeight);
                h_ub_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_ub_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_ub_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_ub_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_ub_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_ub_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_ub_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_ub_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_ub_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_ub_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_ub_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_ub_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_ub_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_ub_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_ub_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_ub_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_ub_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_ub_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_ub_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * W_SF_up / W_SF);
                h_ub_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * W_SF_down / W_SF);
                h_ub_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_up / MergedTop_SF);
                h_ub_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_down / MergedTop_SF);
                h_ub_ISRWeight_up->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Up / ISRWeight);
                h_ub_ISRWeight_down->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Down / ISRWeight);
                h_ub_fastSF_up->Fill(bin_num, SF * lowDMevtWeight * fastSF_up / fastSF);
                h_ub_fastSF_down->Fill(bin_num, SF * lowDMevtWeight * fastSF_down / fastSF);
                h_ub_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_up / ResTop_SF);
                h_ub_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Unit_lowdm_JESUp)
        {
            //bin_num_JESUp = UB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            //bin_num_JESUp = nCRUnitLowDM;
            //bin_num_JESUp = getsearchbin.getUnitNumLowDM("unitCRNum",njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = lepCRunit(Pass_Baseline_JESUp,false,Pass_LLCR_JESUp,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_ub_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_lowdm_JESDown)
        {
            //bin_num_JESDown = UB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            //bin_num_JESDown = nCRUnitLowDM;
            //bin_num_JESDown = getsearchbin.getUnitNumLowDM("unitCRNum",njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = lepCRunit(Pass_Baseline_JESDown,false,Pass_LLCR_JESDown,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_ub_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRUnitLowDM;
            //bin_num_METUnClustUp = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = lepCRunit(Pass_Baseline_METUnClustUp,false,Pass_LLCR_METUnClustUp,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_ub_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRUnitLowDM;
            //bin_num_METUnClustDown = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = lepCRunit(Pass_Baseline_METUnClustDown,false,Pass_LLCR_METUnClustDown,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_ub_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_lowdm_METuncUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRUnitLowDM;
            //bin_num_METunc_up = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,METunc_up);
            bin_num_METunc_up = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,false,Pass_LowDM,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                h_ub_METunc_up->Fill(bin_num_METunc_up,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_lowdm_METuncDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRUnitLowDM;
            //bin_num_METunc_down = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,METunc_down);
            bin_num_METunc_down = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,false,Pass_LowDM,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                h_ub_METunc_down->Fill(bin_num_METunc_down,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        //int UBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Unit_highdm)
        {
            //bin_num = UBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            //bin_num = nCRUnitHighDM;
            bin_num = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "High DM UB num: " << bin_num << std::endl;
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_ub_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_ub_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_ub_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_ub_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_ub_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_ub_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_ub_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_ub_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_ub_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_ub_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_ub_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_ub_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_ub_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_up / Electron_VetoSF);
                h_ub_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_down / Electron_VetoSF);
                h_ub_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_ub_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_ub_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_ub_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_ub_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_up / W_SF);
                h_ub_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_down / W_SF);
                h_ub_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_up / MergedTop_SF);
                h_ub_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_down / MergedTop_SF);
                h_ub_ISRWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Up / ISRWeight);
                h_ub_ISRWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Down / ISRWeight);
                h_ub_fastSF_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_up / fastSF);
                h_ub_fastSF_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_down / fastSF);
                h_ub_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_up / ResTop_SF);
                h_ub_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(Unit_highdm_JESUp)
        {
            //bin_num_JESUp = UBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            //TODO: need to rerun getUnitNumHighDM with other vars
            //bin_num_JESUp = nCRUnitHighDM;
            //bin_num_JESUp = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = lepCRunit(Pass_Baseline_JESUp,false,Pass_LLCR_JESUp,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_ub_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_highdm_JESDown)
        {
            //bin_num_JESDown = UBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            //bin_num_JESDown = nCRUnitHighDM;
            //bin_num_JESDown = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = lepCRunit(Pass_Baseline_JESDown,false,Pass_LLCR_JESDown,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_ub_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRUnitHighDM;
            //bin_num_METUnClustUp = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = lepCRunit(Pass_Baseline_METUnClustUp,false,Pass_LLCR_METUnClustUp,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRUnitHighDM;
            //bin_num_METUnClustDown = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = lepCRunit(Pass_Baseline_METUnClustDown,false,Pass_LLCR_METUnClustDown,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_highdm_METuncUp)
        {
            //bin_num_METunc_up = getsearchbin.getUnitNumHighDM("unitCRNum",mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_up);
            bin_num_METunc_up = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,Pass_HighDM,false,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METunc_up->Fill(bin_num_METunc_up,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_highdm_METuncDown)
        {
            //bin_num_METunc_down = getsearchbin.getUnitNumHighDM("unitCRNum",mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_down);
            bin_num_METunc_down = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,Pass_HighDM,false,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METunc_down->Fill(bin_num_METunc_down,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }

        //QCD CR
        //int QCDCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
        bool QCD_lowdm = Pass_QCDCR && Pass_LowDM && (S_met > 10);
        //bool QCD_highdm = Pass_QCDCR && Pass_HighDM && Pass_dPhiMETHighDM;
        bool QCD_highdm = Pass_QCDCR && Pass_HighDM;
        bool QCD_lowdm_JESUp = Pass_QCDCR_JESUp && Pass_LowDM_JESUp && (S_met_JESUp > 10);
        bool QCD_lowdm_JESDown = Pass_QCDCR_JESDown && Pass_LowDM_JESDown && (S_met_JESDown > 10);
        //bool QCD_highdm_JESUp = Pass_QCDCR_JESUp && Pass_HighDM_JESUp && Pass_dPhiMETHighDM_JESUp;
        //bool QCD_highdm_JESDown = Pass_QCDCR_JESDown && Pass_HighDM_JESDown && Pass_dPhiMETHighDM_JESDown;
        bool QCD_highdm_JESUp = Pass_QCDCR_JESUp && Pass_HighDM_JESUp;
        bool QCD_highdm_JESDown = Pass_QCDCR_JESDown && Pass_HighDM_JESDown;
        bool QCD_lowdm_METUnClustUp = Pass_QCDCR_METUnClustUp && Pass_LowDM_METUnClustUp && (S_met_METUnClustUp > 10);
        bool QCD_lowdm_METUnClustDown = Pass_QCDCR_METUnClustDown && Pass_LowDM_METUnClustDown && (S_met_METUnClustDown > 10);
        //bool QCD_highdm_METUnClustUp = Pass_QCDCR_METUnClustUp && Pass_HighDM_METUnClustUp && Pass_dPhiMETHighDM_METUnClustUp;
        //bool QCD_highdm_METUnClustDown = Pass_QCDCR_METUnClustDown && Pass_HighDM_METUnClustDown && Pass_dPhiMETHighDM_METUnClustDown;
        bool QCD_highdm_METUnClustUp = Pass_QCDCR_METUnClustUp && Pass_HighDM_METUnClustUp;
        bool QCD_highdm_METUnClustDown = Pass_QCDCR_METUnClustDown && Pass_HighDM_METUnClustDown;
        bool QCD_lowdm_METuncUp = QCD_lowdm;
        bool QCD_lowdm_METuncDown = QCD_lowdm;
        bool QCD_highdm_METuncUp = QCD_highdm;
        bool QCD_highdm_METuncDown = QCD_highdm;

        //int UB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(QCD_lowdm)
        {
            //bin_num = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //bin_num = nCRQCDLowDM;
            //int QCDCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "Low DM UB num: " << bin_num << std::endl;
                h_qb->Fill(bin_num,SF*lowDMevtWeight);
                h_qb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_qb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_qb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_qb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_qb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_qb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_qb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_qb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_qb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_qb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_qb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_qb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_qb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_up / Electron_VetoSF);
                h_qb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_VetoSF_down / Electron_VetoSF);
                h_qb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_qb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_qb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_qb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_qb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * W_SF_up / W_SF);
                h_qb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * W_SF_down / W_SF);
                h_qb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_up / MergedTop_SF);
                h_qb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * MergedTop_SF_down / MergedTop_SF);
                h_qb_ISRWeight_up->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Up / ISRWeight);
                h_qb_ISRWeight_down->Fill(bin_num, SF * lowDMevtWeight * ISRWeight_Down / ISRWeight);
                h_qb_fastSF_up->Fill(bin_num, SF * lowDMevtWeight * fastSF_up / fastSF);
                h_qb_fastSF_down->Fill(bin_num, SF * lowDMevtWeight * fastSF_down / fastSF);
                h_qb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_up / ResTop_SF);
                h_qb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(QCD_lowdm_JESUp)
        {
            //bin_num_JESUp = UB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            //bin_num_JESUp = nCRQCDLowDM;
            //bin_num_JESUp = getsearchbin.getQCDNumLowDM("unitCRNum",njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = QCDCRunit(Pass_Baseline_JESUp,Pass_QCDCR_JESUp,false,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_qb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_lowdm_JESDown)
        {
            //bin_num_JESDown = UB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            //bin_num_JESDown = nCRQCDLowDM;
            //bin_num_JESDown = getsearchbin.getQCDNumLowDM("unitCRNum",njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = QCDCRunit(Pass_Baseline_JESDown,Pass_QCDCR_JESDown,false,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_qb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRQCDLowDM;
            //bin_num_METUnClustUp = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = QCDCRunit(Pass_Baseline_METUnClustUp,Pass_QCDCR_METUnClustUp,false,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_qb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRQCDLowDM;
            //bin_num_METUnClustDown = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = QCDCRunit(Pass_Baseline_METUnClustDown,Pass_QCDCR_METUnClustDown,false,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_qb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_lowdm_METuncUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRQCDLowDM;
            //bin_num_METunc_up = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,METunc_up);
            bin_num_METunc_up = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,false,Pass_LowDM,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                h_qb_METunc_up->Fill(bin_num_METunc_up,SF*lowDMevtWeight);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_lowdm_METuncDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRQCDLowDM;
            //bin_num_METunc_down = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,METunc_down);
            bin_num_METunc_down = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,false,Pass_LowDM,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                h_qb_METunc_down->Fill(bin_num_METunc_down,SF*lowDMevtWeight);
                eff_h->Fill(3.,sign);
            }
        }

        //int UBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(QCD_highdm)
        {
            //bin_num = UBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            //bin_num = nCRQCDHighDM;
            bin_num = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "High DM UB num: " << bin_num << std::endl;
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_qb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_qb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_qb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_qb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_qb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_qb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_qb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_qb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_qb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_qb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_qb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_qb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_qb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_up / Electron_VetoSF);
                h_qb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_VetoSF_down / Electron_VetoSF);
                h_qb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_qb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_qb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_qb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_qb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_up / W_SF);
                h_qb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * W_SF_down / W_SF);
                h_qb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_up / MergedTop_SF);
                h_qb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * MergedTop_SF_down / MergedTop_SF);
                h_qb_ISRWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Up / ISRWeight);
                h_qb_ISRWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ISRWeight_Down / ISRWeight);
                h_qb_fastSF_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_up / fastSF);
                h_qb_fastSF_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * fastSF_down / fastSF);
                h_qb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_up / ResTop_SF);
                h_qb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * ResTop_SF_down / ResTop_SF);
                eff_h->Fill(1.,sign);
            }
        }
        if(QCD_highdm_JESUp)
        {
            //bin_num_JESUp = UBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            //TODO: need to rerun getQCDNumHighDM with other vars
            //bin_num_JESUp = nCRQCDHighDM;
            //bin_num_JESUp = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = QCDCRunit(Pass_Baseline_JESUp,Pass_QCDCR_JESUp,false,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_qb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_highdm_JESDown)
        {
            //bin_num_JESDown = UBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            //bin_num_JESDown = nCRQCDHighDM;
            //bin_num_JESDown = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = QCDCRunit(Pass_Baseline_JESDown,Pass_QCDCR_JESDown,false,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_qb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRQCDHighDM;
            //bin_num_METUnClustUp = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = QCDCRunit(Pass_Baseline_METUnClustUp,Pass_QCDCR_METUnClustUp,false,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRQCDHighDM;
            //bin_num_METUnClustDown = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = QCDCRunit(Pass_Baseline_METUnClustDown,Pass_QCDCR_METUnClustDown,false,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_highdm_METuncUp)
        {
            //bin_num_METunc_up = getsearchbin.getQCDNumHighDM("unitCRNum",mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_up);
            bin_num_METunc_up = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,Pass_HighDM,false,nb,mtb,ptb,METunc_up,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_up != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METunc_up->Fill(bin_num_METunc_up,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_highdm_METuncDown)
        {
            //bin_num_METunc_down = getsearchbin.getQCDNumHighDM("unitCRNum",mtb,njets,nb,ntop_merge,nw,ntop_res,HT,METunc_down);
            bin_num_METunc_down = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,Pass_HighDM,false,nb,mtb,ptb,METunc_down,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METunc_down != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METunc_down->Fill(bin_num_METunc_down,SF*highDMevtWeight * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
    }

    float all_events = eff_h->GetBinContent(1);
    float lumi = 1.0;
    if(era == "2016") lumi = 35.815165; //MET. SM 35.917149
    if(era == "2017")
    {
        if(PeriodF) lumi = 13.220313; //MET. SM 13.538559
        else lumi = 27.987721; //MET. SM 27.9865
    }
    if(era == "2018")
    {
        if(PostHEM) lumi = 38.630913; //MET. SM 38.65435
        else lumi = 21.068576; //MET. SM 21.071447
    }
    std::string str_mass = "";
    TString histname = "";
    if(samplename.find("mGluino") != std::string::npos)
    {
        str_mass = samplename.substr(samplename.find("mGluino") + 7,samplename.find("_mLSP") - samplename.find("mGluino") - 7);
        histname = "gluino_xsection";
    }
    if(samplename.find("mStop") != std::string::npos)
    {
        str_mass = samplename.substr(samplename.find("mStop") + 5,samplename.find("_mLSP") - samplename.find("mStop") - 5);
        histname = "stop_xsection";
    }
    if(samplename.find("mSbot") != std::string::npos) str_mass = samplename.substr(samplename.find("mSbot") + 5,samplename.find("_mLSP") - samplename.find("mSbot") - 5);

    //std::cout << str_mass << std::endl;
    std::cout << "Era: " << era << "\tLumi: " << lumi << "\tstr_mass: " << str_mass << std::endl;
    //std::string str_mass = samplename.substr(samplename.find("m"),samplename.find("mLSP") - samplename.find("m"));
    int mass = std::stoi(str_mass);
    //std::cout << mass << std::endl;

    float genfiltereff = 1;
    if(denominator_genfiltereff != 0) genfiltereff = numerator_genfiltereff / denominator_genfiltereff;
    else std::cout << "Error: no events found. denominator_genfiltereff = 0." << std::endl;

    TFile *infile = new TFile("xSec.root");
    TH1F *xsection = (TH1F*) infile->Get(histname);
    float xs = xsection->GetBinContent(xsection->GetXaxis()->FindBin(mass));

    std::cout << "Era: " << era << "\tLumi: " << lumi << "\tMass: " << mass << "\txs: " << xs << std::endl;

    h_njets_low_middphi->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_njets_high_middphi->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ISRWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ISRWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_fastSF_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_fastSF_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ISRWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ISRWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_fastSF_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_fastSF_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ISRWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ISRWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_fastSF_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_fastSF_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ISRWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ISRWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_fastSF_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_fastSF_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);

    TString outputfile = samplename + ".root";
    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    eff_h->Write();
    out_file.cd("../");
    h_njets_low_middphi->Write();
    h_njets_high_middphi->Write();
    h_vb->Write();
    h_vb_bsf_up->Write();
    h_vb_bsf_down->Write();
    h_vb_trig_eff_up->Write();
    h_vb_trig_eff_down->Write();
    h_vb_puWeight_up->Write();
    h_vb_puWeight_down->Write();
    h_vb_PFWeight_up->Write();
    h_vb_PFWeight_down->Write();
    h_vb_pdfWeight_up->Write();
    h_vb_pdfWeight_down->Write();
    h_vb_JES_up->Write();
    h_vb_JES_down->Write();
    h_vb_METUnClust_up->Write();
    h_vb_METUnClust_down->Write();
    h_vb_ivfunc_up->Write();
    h_vb_ivfunc_down->Write();
    h_vb_eff_e_up->Write();
    h_vb_eff_e_down->Write();
    h_vb_err_mu_up->Write();
    h_vb_err_mu_down->Write();
    h_vb_eff_tau_up->Write();
    h_vb_eff_tau_down->Write();
    h_vb_ISRWeight_up->Write();
    h_vb_ISRWeight_down->Write();
    h_vb_fastSF_up->Write();
    h_vb_fastSF_down->Write();
    h_vb_METunc_up->Write();
    h_vb_METunc_down->Write();
    h_vb_eff_wtag_up->Write();
    h_vb_eff_wtag_down->Write();
    h_vb_eff_toptag_up->Write();
    h_vb_eff_toptag_down->Write();
    h_vb_eff_restoptag_up->Write();
    h_vb_eff_restoptag_down->Write();
    h_sb->Write();
    h_sb_bsf_up->Write();
    h_sb_bsf_down->Write();
    h_sb_trig_eff_up->Write();
    h_sb_trig_eff_down->Write();
    h_sb_puWeight_up->Write();
    h_sb_puWeight_down->Write();
    h_sb_PFWeight_up->Write();
    h_sb_PFWeight_down->Write();
    h_sb_pdfWeight_up->Write();
    h_sb_pdfWeight_down->Write();
    h_sb_JES_up->Write();
    h_sb_JES_down->Write();
    h_sb_METUnClust_up->Write();
    h_sb_METUnClust_down->Write();
    h_sb_ivfunc_up->Write();
    h_sb_ivfunc_down->Write();
    h_sb_eff_e_up->Write();
    h_sb_eff_e_down->Write();
    h_sb_err_mu_up->Write();
    h_sb_err_mu_down->Write();
    h_sb_eff_tau_up->Write();
    h_sb_eff_tau_down->Write();
    h_sb_ISRWeight_up->Write();
    h_sb_ISRWeight_down->Write();
    h_sb_fastSF_up->Write();
    h_sb_fastSF_down->Write();
    h_sb_METunc_up->Write();
    h_sb_METunc_down->Write();
    h_sb_eff_wtag_up->Write();
    h_sb_eff_wtag_down->Write();
    h_sb_eff_toptag_up->Write();
    h_sb_eff_toptag_down->Write();
    h_sb_eff_restoptag_up->Write();
    h_sb_eff_restoptag_down->Write();
    h_ub->Write();
    h_ub_bsf_up->Write();
    h_ub_bsf_down->Write();
    h_ub_trig_eff_up->Write();
    h_ub_trig_eff_down->Write();
    h_ub_puWeight_up->Write();
    h_ub_puWeight_down->Write();
    h_ub_PFWeight_up->Write();
    h_ub_PFWeight_down->Write();
    h_ub_pdfWeight_up->Write();
    h_ub_pdfWeight_down->Write();
    h_ub_JES_up->Write();
    h_ub_JES_down->Write();
    h_ub_METUnClust_up->Write();
    h_ub_METUnClust_down->Write();
    h_ub_ivfunc_up->Write();
    h_ub_ivfunc_down->Write();
    h_ub_eff_e_up->Write();
    h_ub_eff_e_down->Write();
    h_ub_err_mu_up->Write();
    h_ub_err_mu_down->Write();
    h_ub_eff_tau_up->Write();
    h_ub_eff_tau_down->Write();
    h_ub_ISRWeight_up->Write();
    h_ub_ISRWeight_down->Write();
    h_ub_fastSF_up->Write();
    h_ub_fastSF_down->Write();
    h_ub_METunc_up->Write();
    h_ub_METunc_down->Write();
    h_ub_eff_wtag_up->Write();
    h_ub_eff_wtag_down->Write();
    h_ub_eff_toptag_up->Write();
    h_ub_eff_toptag_down->Write();
    h_ub_eff_restoptag_up->Write();
    h_ub_eff_restoptag_down->Write();
    h_qb->Write();
    h_qb_bsf_up->Write();
    h_qb_bsf_down->Write();
    h_qb_trig_eff_up->Write();
    h_qb_trig_eff_down->Write();
    h_qb_puWeight_up->Write();
    h_qb_puWeight_down->Write();
    h_qb_PFWeight_up->Write();
    h_qb_PFWeight_down->Write();
    h_qb_pdfWeight_up->Write();
    h_qb_pdfWeight_down->Write();
    h_qb_JES_up->Write();
    h_qb_JES_down->Write();
    h_qb_METUnClust_up->Write();
    h_qb_METUnClust_down->Write();
    h_qb_ivfunc_up->Write();
    h_qb_ivfunc_down->Write();
    h_qb_eff_e_up->Write();
    h_qb_eff_e_down->Write();
    h_qb_err_mu_up->Write();
    h_qb_err_mu_down->Write();
    h_qb_eff_tau_up->Write();
    h_qb_eff_tau_down->Write();
    h_qb_ISRWeight_up->Write();
    h_qb_ISRWeight_down->Write();
    h_qb_fastSF_up->Write();
    h_qb_fastSF_down->Write();
    h_qb_METunc_up->Write();
    h_qb_METunc_down->Write();
    h_qb_eff_wtag_up->Write();
    h_qb_eff_wtag_down->Write();
    h_qb_eff_toptag_up->Write();
    h_qb_eff_toptag_down->Write();
    h_qb_eff_restoptag_up->Write();
    h_qb_eff_restoptag_down->Write();
    out_file.Close();

    delete eff_h;
    delete h_njets_low_middphi;
    delete h_njets_high_middphi;
    delete h_vb;
    delete h_vb_bsf_up;
    delete h_vb_bsf_down;
    delete h_vb_trig_eff_up;
    delete h_vb_trig_eff_down;
    delete h_vb_puWeight_up;
    delete h_vb_puWeight_down;
    delete h_vb_PFWeight_up;
    delete h_vb_PFWeight_down;
    delete h_vb_pdfWeight_up;
    delete h_vb_pdfWeight_down;
    delete h_vb_JES_up;
    delete h_vb_JES_down;
    delete h_vb_METUnClust_up;
    delete h_vb_METUnClust_down;
    delete h_vb_ivfunc_up;
    delete h_vb_ivfunc_down;
    delete h_vb_eff_e_up;
    delete h_vb_eff_e_down;
    delete h_vb_err_mu_up;
    delete h_vb_err_mu_down;
    delete h_vb_eff_tau_up;
    delete h_vb_eff_tau_down;
    delete h_vb_ISRWeight_up;
    delete h_vb_ISRWeight_down;
    delete h_vb_fastSF_up;
    delete h_vb_fastSF_down;
    delete h_vb_METunc_up;
    delete h_vb_METunc_down;
    delete h_vb_eff_wtag_up;
    delete h_vb_eff_wtag_down;
    delete h_vb_eff_toptag_up;
    delete h_vb_eff_toptag_down;
    delete h_vb_eff_restoptag_up;
    delete h_vb_eff_restoptag_down;
    delete h_sb;
    delete h_sb_bsf_up;
    delete h_sb_bsf_down;
    delete h_sb_trig_eff_up;
    delete h_sb_trig_eff_down;
    delete h_sb_puWeight_up;
    delete h_sb_puWeight_down;
    delete h_sb_PFWeight_up;
    delete h_sb_PFWeight_down;
    delete h_sb_pdfWeight_up;
    delete h_sb_pdfWeight_down;
    delete h_sb_JES_up;
    delete h_sb_JES_down;
    delete h_sb_METUnClust_up;
    delete h_sb_METUnClust_down;
    delete h_sb_ivfunc_up;
    delete h_sb_ivfunc_down;
    delete h_sb_eff_e_up;
    delete h_sb_eff_e_down;
    delete h_sb_err_mu_up;
    delete h_sb_err_mu_down;
    delete h_sb_eff_tau_up;
    delete h_sb_eff_tau_down;
    delete h_sb_ISRWeight_up;
    delete h_sb_ISRWeight_down;
    delete h_sb_fastSF_up;
    delete h_sb_fastSF_down;
    delete h_sb_METunc_up;
    delete h_sb_METunc_down;
    delete h_sb_eff_wtag_up;
    delete h_sb_eff_wtag_down;
    delete h_sb_eff_toptag_up;
    delete h_sb_eff_toptag_down;
    delete h_sb_eff_restoptag_up;
    delete h_sb_eff_restoptag_down;
    delete h_ub;
    delete h_ub_bsf_up;
    delete h_ub_bsf_down;
    delete h_ub_trig_eff_up;
    delete h_ub_trig_eff_down;
    delete h_ub_puWeight_up;
    delete h_ub_puWeight_down;
    delete h_ub_PFWeight_up;
    delete h_ub_PFWeight_down;
    delete h_ub_pdfWeight_up;
    delete h_ub_pdfWeight_down;
    delete h_ub_JES_up;
    delete h_ub_JES_down;
    delete h_ub_METUnClust_up;
    delete h_ub_METUnClust_down;
    delete h_ub_ivfunc_up;
    delete h_ub_ivfunc_down;
    delete h_ub_eff_e_up;
    delete h_ub_eff_e_down;
    delete h_ub_err_mu_up;
    delete h_ub_err_mu_down;
    delete h_ub_eff_tau_up;
    delete h_ub_eff_tau_down;
    delete h_ub_ISRWeight_up;
    delete h_ub_ISRWeight_down;
    delete h_ub_fastSF_up;
    delete h_ub_fastSF_down;
    delete h_ub_METunc_up;
    delete h_ub_METunc_down;
    delete h_ub_eff_wtag_up;
    delete h_ub_eff_wtag_down;
    delete h_ub_eff_toptag_up;
    delete h_ub_eff_toptag_down;
    delete h_ub_eff_restoptag_up;
    delete h_ub_eff_restoptag_down;
    delete h_qb;
    delete h_qb_bsf_up;
    delete h_qb_bsf_down;
    delete h_qb_trig_eff_up;
    delete h_qb_trig_eff_down;
    delete h_qb_puWeight_up;
    delete h_qb_puWeight_down;
    delete h_qb_PFWeight_up;
    delete h_qb_PFWeight_down;
    delete h_qb_pdfWeight_up;
    delete h_qb_pdfWeight_down;
    delete h_qb_JES_up;
    delete h_qb_JES_down;
    delete h_qb_METUnClust_up;
    delete h_qb_METUnClust_down;
    delete h_qb_ivfunc_up;
    delete h_qb_ivfunc_down;
    delete h_qb_eff_e_up;
    delete h_qb_eff_e_down;
    delete h_qb_err_mu_up;
    delete h_qb_err_mu_down;
    delete h_qb_eff_tau_up;
    delete h_qb_eff_tau_down;
    delete h_qb_ISRWeight_up;
    delete h_qb_ISRWeight_down;
    delete h_qb_fastSF_up;
    delete h_qb_fastSF_down;
    delete h_qb_METunc_up;
    delete h_qb_METunc_down;
    delete h_qb_eff_wtag_up;
    delete h_qb_eff_wtag_down;
    delete h_qb_eff_toptag_up;
    delete h_qb_eff_toptag_down;
    delete h_qb_eff_restoptag_up;
    delete h_qb_eff_restoptag_down;
    return 0;

}

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
    bool PeriodF = false;
    bool PostHEM = false;
    bool isSignal = true;

    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *unused        = argv[2];

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


    std::cout << "Era: " << era << "\tSF: " << SF << "\tisSignal: " << isSignal << "\tisData: " << isData << "\tPeriodF: " << PeriodF << "\tPostHEM: " << PostHEM << "\tMax events: " << max_events << std::endl;

    /*
    TChain *ch = new TChain("Events");

    std::string filename;
    std::ifstream bigfile;
    std::string samplename;

    bigfile.open (inputfilelist);

    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            ch->Add(filename.c_str());
            samplename = filename.substr(filename.find("SMS_"),filename.find(".root")-filename.find("SMS_"));
            if(samplename.find("/") != std::string::npos) samplename = samplename.substr(samplename.find("/") + 1);
            std::cout << samplename << std::endl;
        }
        bigfile.close();
    }
    */
    std::string filename;
    std::ifstream bigfile;
    bigfile.open (inputfilelist);
    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            analyze(filename,era,max_events,isData,isSignal,PeriodF,PostHEM,SF,verbose);
        }
        bigfile.close();
    }
    return 0;
}

