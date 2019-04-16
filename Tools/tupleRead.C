#include "NTupleReader.h"
#include "baselineDef.h"
#include "searchBins.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include <cstdio>
#include <stdio.h> // printf, Null // ebinter
#include <stdlib.h> // strtod // ebinter
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "ISRCorrector.h"
#include "BTagCorrector.h"
#include "SB2018.h"
#include "my_dPhi.C"

int main(int argc, char* argv[]){

	const bool debug = false;
	const bool use_new_tagger = true;
	const bool use_deepCSV = true;
	const bool do_ISR_BSF = true;
	const bool t2bw_study = false;

	//const TString new_tagger_cfg = "default";
	const TString new_tagger_cfg = "res_tight_ak8_tight";
	//const TString new_tagger_cfg = "res_tight_ak8_tight_W_loose";
	//const TString new_tagger_cfg = "res_tight_ak8_loose";
	//const TString new_tagger_cfg = "res_mid_ak8_tight";
	//const TString new_tagger_cfg = "res_mid_ak8_loose";

	int max_events = -1;
	max_events = 10000;

	// ---------- Input & Output File Arguments ----------

	std::string filename; //name of each root file ~aarulana
	std::ifstream bigfile; //name of file list ~aarulana

	if (argc < 3)
	{
		std::cerr <<"Please give 2 arguments " << "File List " << "Name of output file" << std::endl;
		return -1;
	}
	const char *inputfilelist = argv[1];
	const char *outputfile   = argv[2];
	std::string sample_name(inputfilelist);

	//if(argc > 3){
	//   const char *events_input = argv[3]; // Need to Fix
	//   events = strtod(*events_input, NULL);
	//}

	const char *fastsim   = "";

	if (argc > 3)
	{
		fastsim = argv[3];
	}

	if(debug) std::cout << __LINE__ << std::endl;

	TChain *ch = new TChain("stopTreeMaker/AUX"); //TChain for all files
	bigfile.open (inputfilelist); //opening file list ~aarulana

	if (bigfile.is_open()) //adding each file to TChain
	{
		while (getline(bigfile,filename))
		{
			std::cout << filename << std::endl;		
			ch->Add(filename.c_str());
		}
		bigfile.close();
	}

	// ---------- Reading Ntuple (Original Code) ----------

	if(debug) std::cout << __LINE__ << std::endl;

	NTupleReader tr(ch);
	tr.setReThrow(false);

	if(debug) std::cout << __LINE__ << std::endl;

	bool find_fastsim = false;
	std::string filterString = "";

	if(sample_name.find("Signal_fastsim") != std::string::npos)
	{
		std::cout << "find fastsim signal" << std::endl;
		find_fastsim = true;
		filterString = "fastsim";
	}

	BaselineVessel blv(tr, "", filterString);
	//BaselineVessel blv(tr, "", "fastsim");
	if(!use_deepCSV) blv.UseDeepCSV = false;
	if(use_new_tagger)
	{
		blv.UseDeepTagger = true;
		if(new_tagger_cfg == "default") blv.SetupTopTagger("./TopTagger_Deep.cfg");
		if(new_tagger_cfg == "res_tight_ak8_tight") blv.SetupTopTagger("./TopTagger_res_tight_ak8_tight.cfg");
		if(new_tagger_cfg == "res_tight_ak8_tight_W_loose") blv.SetupTopTagger("./TopTagger_res_tight_ak8_tight_W_loose.cfg");
		if(new_tagger_cfg == "res_tight_ak8_loose") blv.SetupTopTagger("./TopTagger_res_tight_ak8_loose.cfg");
		if(new_tagger_cfg == "res_mid_ak8_tight") blv.SetupTopTagger("./TopTagger_res_mid_ak8_tight.cfg");
		if(new_tagger_cfg == "res_mid_ak8_loose") blv.SetupTopTagger("./TopTagger_res_mid_ak8_loose.cfg");
	}

	if(debug) std::cout << __LINE__ << std::endl;

	tr.registerFunction(blv);
	SearchBins SB("SB_v1_2017");

	if(debug) std::cout << __LINE__ << std::endl;

	bool find_B_SF = true;
	bool find_ISR_SF = true;

	std::cout << "======================================= ISR and B_SF start ==============================================" << std::endl;
	std::cout << "sample name is " << sample_name << std::endl; 

	if(debug) std::cout << __LINE__ << std::endl;

	BTagCorrector btagcorr = BTagCorrector("allINone_bTagEff.root", "", false);
	//if(filename.find("TTJets_SingleLeptFromT_") != std::string::npos)
	if(sample_name.find("TTbarSingleLepT.") != std::string::npos)		//don't forget the . after TTbarSingleLepT
	{std::cout << "find TTbarSingleLepT" << std::endl; btagcorr.resetEffs("TTbarSingleLepT");}
	//else if(filename.find("TTJets_SingleLeptFromTbar_") != std::string::npos)
	else if(sample_name.find("TTbarSingleLepTbar.") != std::string::npos)
	{std::cout << "find TTbarSingleLepTbar" << std::endl; btagcorr.resetEffs("TTbarSingleLepTbar");}
	//else if(filename.find("TTJets_DiLept_") != std::string::npos)
	else if(sample_name.find("TTbarDiLep.") != std::string::npos)
	{std::cout << "find TTbarDiLep" << std::endl; btagcorr.resetEffs("TTbarDiLep");}
	//else if(filename.find("WJetsToLNu_HT-600To800_") != std::string::npos) std::cout << "find WJetsToLNu_HT_600to800\n" << std::endl;
	else find_B_SF = false;
	tr.registerFunction(btagcorr);

	if(debug) std::cout << __LINE__ << std::endl;

	ISRCorrector *isrcorr = new ISRCorrector("allINone_ISRJets.root", "", "");
	//if(filename.find("TTJets_SingleLeptFromT_") != std::string::npos) isrcorr->resetSample("TTbarSingleLepT");
	if(sample_name.find("TTbarSingleLepT.") != std::string::npos) isrcorr->resetSample("TTbarSingleLepT");
	//else if(filename.find("TTJets_SingleLeptFromTbar_") != std::string::npos) isrcorr->resetSample("TTbarSingleLepTbar");
	else if(sample_name.find("TTbarSingleLepTbar.") != std::string::npos) isrcorr->resetSample("TTbarSingleLepTbar");
	//else if(filename.find("TTJets_DiLept_") != std::string::npos) isrcorr->resetSample("TTbarDiLep");
	else if(sample_name.find("TTbarDiLep.") != std::string::npos) isrcorr->resetSample("TTbarDiLep");
	//else if(filename.find("WJetsToLNu_HT-600To800_") != std::string::npos) isrcorr->resetSample("WJetsToLNu_HT_600to800");
	else find_ISR_SF = false;
	tr.registerFunction((*isrcorr));
	std::cout << "======================================= ISR and B_SF end ==============================================\n" << std::endl;

	// ---------- Make Histograms ----------

	if(debug) std::cout << __LINE__ << std::endl;

	// MET
	auto met_uc_h=new TH1F("met_uc_h","MET, no cuts",80,0.0,1600.0);
	auto met_h=new TH1F("met_h","MET, loose baseline",80,0.0,1600.0);
	auto met_no_HT_h=new TH1F("met_no_HT_h","MET, loose baseline no HT cut",80,0.0,1600.0);
	auto met_140_h=new TH1F("met_140_h","MET, loose baseline no HT cut, MTb > 140",80,0.0,1600.0);
	auto met_140_MT2_h=new TH1F("met_140_MT2_h","MET, loose baseline no HT cut, MTb > 140, MT2 cut",80,0.0,1600.0);
	auto met_175_h=new TH1F("met_175_h","MET, loose baseline no HT cut, MTb > 175",80,0.0,1600.0);
	auto met_175_MT2_h=new TH1F("met_175_MT2_h","MET, loose baseline no HT cut, MTb > 175, MT2 cut",80,0.0,1600.0);
	auto met_lowdm_h=new TH1F("met_lowdm_h","MET, low dm",80,0.0,1600.0);
	auto met_highdm_h=new TH1F("met_highdm_h","MET, high dm",80,0.0,1600.0);
	auto genmet_h=new TH1F("genmet_h","genMET, loose baseline",80,0.0,1600.0);

	auto met_1b_100_h=new TH1F("met_1b_100_h","MET, b=1, t>=1, w=0, res=0",80,0.0,1600.0);
	auto met_1b_010_h=new TH1F("met_1b_010_h","MET, b=1, t=0, w>=1, res=0",80,0.0,1600.0);
	auto met_1b_001_h=new TH1F("met_1b_001_h","MET, b=1, t=0, w=0, res>=1",80,0.0,1600.0);
	auto met_1b_110_h=new TH1F("met_1b_110_h","MET, b=1, t>=1, w>=1, res=0",80,0.0,1600.0);
	auto met_1b_011_h=new TH1F("met_1b_011_h","MET, b=1, t=0, w>=1, res>=1",80,0.0,1600.0);
	auto met_1b_101_h=new TH1F("met_1b_101_h","MET, b=1, t>=1, w=0, res>=1",80,0.0,1600.0);

	auto met_2b_100_h=new TH1F("met_2b_100_h","MET, b=2, t=1, w=0, res=0",80,0.0,1600.0);
	auto met_2b_010_h=new TH1F("met_2b_010_h","MET, b=2, t=0, w=1, res=0",80,0.0,1600.0);
	auto met_2b_001_h=new TH1F("met_2b_001_h","MET, b=2, t=0, w=0, res=1",80,0.0,1600.0);
	auto met_2b_110_h=new TH1F("met_2b_110_h","MET, b=2, t=1, w=1, res=0",80,0.0,1600.0);
	auto met_2b_011_h=new TH1F("met_2b_011_h","MET, b=2, t=0, w=1, res=1",80,0.0,1600.0);
	auto met_2b_101_h=new TH1F("met_2b_101_h","MET, b=2, t=1, w=0, res=1",80,0.0,1600.0);
	auto met_2b_200_h=new TH1F("met_2b_200_h","MET, b=2, t=2, w=0, res=0",80,0.0,1600.0);
	auto met_2b_020_h=new TH1F("met_2b_020_h","MET, b=2, t=0, w=2, res=0",80,0.0,1600.0);
	auto met_2b_002_h=new TH1F("met_2b_002_h","MET, b=2, t=0, w=0, res=2",80,0.0,1600.0);
	auto met_2b_3_h=new TH1F("met_2b_3_h","MET, b=2, t+res+w >= 3",80,0.0,1600.0);

	auto met_3b_100_h=new TH1F("met_3b_100_h","MET, b>=3, t=1, w=0, res=0",80,0.0,1600.0);
	auto met_3b_010_h=new TH1F("met_3b_010_h","MET, b>=3, t=0, w=1, res=0",80,0.0,1600.0);
	auto met_3b_001_h=new TH1F("met_3b_001_h","MET, b>=3, t=0, w=0, res=1",80,0.0,1600.0);
	auto met_3b_110_h=new TH1F("met_3b_110_h","MET, b>=3, t=1, w=1, res=0",80,0.0,1600.0);
	auto met_3b_011_h=new TH1F("met_3b_011_h","MET, b>=3, t=0, w=1, res=1",80,0.0,1600.0);
	auto met_3b_101_h=new TH1F("met_3b_101_h","MET, b>=3, t=1, w=0, res=1",80,0.0,1600.0);
	auto met_3b_200_h=new TH1F("met_3b_200_h","MET, b>=3, t=2, w=0, res=0",80,0.0,1600.0);
	auto met_3b_020_h=new TH1F("met_3b_020_h","MET, b>=3, t=0, w=2, res=0",80,0.0,1600.0);
	auto met_3b_002_h=new TH1F("met_3b_002_h","MET, b>=3, t=0, w=0, res=2",80,0.0,1600.0);
	auto met_3b_3_h=new TH1F("met_3b_3_h","MET, b>=3, t+res+w >= 3",80,0.0,1600.0);

	// MT2
	auto mt2_h=new TH1F("mt2_h","MT2, loose baseline",80,0.0,800.0);
	auto mt2_no_HT_h=new TH1F("mt2_no_HT_h","MT2, loose baseline no HT cut",80,0.0,800.0);
	auto mt2_140_h=new TH1F("mt2_140_h","MT2, loose baseline no HT cut, MTb > 140",80,0.0,800.0);
	auto mt2_175_h=new TH1F("mt2_175_h","MT2, loose baseline no HT cut, MTb > 175",80,0.0,800.0);
	auto mt2_140_high_h=new TH1F("mt2_140_high_h","MT2, team A bsaeline when MTb > 140",80,0.0,800.0);
	auto mt2_140_low_h=new TH1F("mt2_140_low_h","MT2, team A bsaeline when MTb < 140",80,0.0,800.0);
	auto mt2_175_high_h=new TH1F("mt2_175_high_h","MT2, team A bsaeline when MTb > 175",80,0.0,800.0);
	auto mt2_175_low_h=new TH1F("mt2_175_low_h","MT2, team A bsaeline when MTb < 175",80,0.0,800.0);
	auto mt2_140_175_h=new TH1F("mt2_140_175_h","MT2, team A bsaeline when 140 < MTb < 175",80,0.0,800.0);
	auto mt2_baseline_no_mt2_h=new TH1F("mt2_baseline_no_mt2_h","MT2 baseline no MT2 cut",80,0.0,800.0);
	auto mt2_b_h=new TH1F("mt2_b_h","MT2_b, loose baseline",80,0.0,800.0);

	// MTb
	auto mtb_uc_h=new TH1F("mtb_uc_h","MTb, no cuts",80,0.0,800.0);
	auto mtb_h=new TH1F("mtb_h","MTb, loose baseline",80,0.0,800.0);
	auto mtb_no_HT_h=new TH1F("mtb_no_HT_h","MTb, loose baseline no HT cut",80,0.0,800.0);
	auto mtb_highdm_h=new TH1F("mtb_highdm_h","MTb high dm",80,0.0,800.0);

	// Jets
	auto jpt_eta_2p5_3p0_h=new TH1F("jpt_eta_2p5_3p0_h","Leading jet pt, loose baseline, 2.5 < eta < 3.0",80,0.0,400.0);
	auto jpt_eta_2p5_3p0_no_HT_h=new TH1F("jpt_eta_2p5_3p0_no_HT_h","Leading jet pt, loose baseline no HT cut, 2.5 < eta < 3.0",80,0.0,400.0);
	auto njets_uc_h=new TH1F("njets_uc_h","number of all jets, no cut",30,0.0,30.0);
	auto njets_h=new TH1F("njets_h","number of all jets, loose baseline",30,0.0,30.0);
	auto njetspt20_lept_veto_h=new TH1F("njetspt20_lept_veto_h","njets, only lepton veto, pt > 20",15,0.0,15.0);
	auto njetspt20_uc_h=new TH1F("njetspt20_uc_h","njets, no cuts, pt > 20",15,0.0,15.0);
	auto njetspt20_h=new TH1F("njetspt20_h","njets, loose baseline, pt > 20",15,0.0,15.0);
	auto njetspt20_no_HT_h=new TH1F("njetspt20_no_HT_h","njets, loose baseline no HT cut, pt > 20, no MTb cut",30,0.0,30.0);
	auto njetspt20_140_h=new TH1F("njetspt20_140_h","njets, loose baseline no HT cut, pt > 20, MTb > 140",30,0.0,30.0);
	auto njetspt20_140_MT2_h=new TH1F("njetspt20_140_MT2_h","njets, loose baseline no HT cut, pt > 20, MTb > 140, MT2 cut",30,0.0,30.0);
	auto njetspt20_175_h=new TH1F("njetspt20_175_h","njets, loose baseline no HT cut, pt > 20, MTb > 175",30,0.0,30.0);
	auto njetspt20_175_MT2_h=new TH1F("njetspt20_175_MT2_h","njets, loose baseline no HT cut, pt > 20, MTb > 175, MT2 cut",30,0.0,30.0);
	auto njetspt20_lowdm_h=new TH1F("njetspt20_lowdm_h","njets, low dm, pt > 20",30,0.0,30.0);
	auto njetspt30_h=new TH1F("njetspt30_h","njets, loose baseline, pt > 30",15,0.0,15.0);
	auto njetspt30_no_HT_h=new TH1F("njetspt30_no_HT_h","njets, loose baseline no HT cut, pt > 30, no MTb cut",30,0.0,30.0);
	auto njetspt30_140_h=new TH1F("njetspt30_140_h","njets, loose baseline no HT cut, pt > 30, MTb > 140",30,0.0,30.0);
	auto njetspt30_140_MT2_h=new TH1F("njetspt30_140_MT2_h","njets, loose baseline no HT cut, pt > 30, MTb > 140, MT2 cut",30,0.0,30.0);
	auto njetspt30_175_h=new TH1F("njetspt30_175_h","loose baseline no HT cut, pt > 30, MTb > 175",30,0.0,30.0);
	auto njetspt30_175_MT2_h=new TH1F("njetspt30_175_MT2_h","njets, loose baseline no HT cut, pt > 30, MTb > 175, MT2 cut",30,0.0,30.0);

	// b jets
	auto nbottompt20_uc_h=new TH1F("nbottompt20_uc_h","nbottom, no cuts, pt > 20",8,0.0,8.0);
	auto nbottompt20_h=new TH1F("nbottompt20_h","nbottom, loose baseline, pt > 20",8,0.0,8.0);
	auto nbottompt20_no_HT_h=new TH1F("nbottompt20_no_HT_h","nbottom, loose baseline no HT cut, pt > 20, no mtb cut",8,0.0,8.0);
	auto nbottompt20_140_h=new TH1F("nbottompt20_140_h","nbottom, loose baseline no HT cut, pt > 20, mtb > 140",8,0.0,8.0);
	auto nbottompt20_140_MT2_h=new TH1F("nbottompt20_140_MT2_h","nbottom, loose baseline no HT cut, pt > 20, mtb > 140, MT2 cut",8,0.0,8.0);
	auto nbottompt20_175_h=new TH1F("nbottompt20_175_h","nbottom, loose baseline no HT cut, pt > 20, mtb > 175",8,0.0,8.0);
	auto nbottompt20_175_MT2_h=new TH1F("nbottompt20_175_MT2_h","nbottom, loose baseline no HT cut, pt > 20, mtb > 175, MT2 cut",8,0.0,8.0);
	auto nbottompt20_lowdm_h=new TH1F("nbottompt20_lowdm_h","nbottom, low dm",8,0.0,8.0);
	auto nbottompt30_no_HT_h=new TH1F("nbottompt30_no_HT_h","nbottom, loose baseline no HT cut, pt > 30, no mtb cut",8,0.0,8.0);
	auto nbottompt30_140_h=new TH1F("nbottompt30_140_h","nbottom, loose baseline no HT cut, pt > 30, mtb > 140",8,0.0,8.0);
	auto nbottompt30_140_MT2_h=new TH1F("nbottompt30_140_MT2_h","nbottom, loose baseline no HT cut, pt > 30, mtb > 140, MT2 cut",8,0.0,8.0);
	auto nbottompt30_175_h=new TH1F("nbottompt30_175_h","nbottom, loose baseline no HT cut, pt > 30, mtb > 175",8,0.0,8.0);
	auto nbottompt30_175_MT2_h=new TH1F("nbottompt30_175_MT2_h","nbottom, loose baseline no HT cut, pt > 30, mtb > 175, MT2 cut",8,0.0,8.0);
	auto nSV_lowdm_h=new TH1F("nSV_lowdm_h","nSV, low dm",8,0.0,8.0);

	// top
	auto ntop_h=new TH1F("ntop_h","ntop, loose baseline",8,0.0,8.0);
	auto ntop_no_HT_h=new TH1F("ntop_no_HT_h","ntop, loose baseline no HT cut, no MTb cut",8,0.0,8.0);
	auto ntop_140_h=new TH1F("ntop_140_h","ntop, loose baseline no HT cut, MTb > 140",8,0.0,8.0);
	auto ntop_140_MT2_h=new TH1F("ntop_140_MT2_h","ntop, loose baseline no HT cut, MTb > 140, MT2 cut",8,0.0,8.0);
	auto ntop_175_h=new TH1F("ntop_175_h","ntop, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto ntop_175_MT2_h=new TH1F("ntop_175_MT2_h","ntop, loose baseline no HT cut, MTb > 175, MT2 cut",8,0.0,8.0);
	auto ntop_merge_uc_h=new TH1F("ntop_merge_uc_h","number of fully merged top, no cuts",8,0.0,8.0);
	auto ntop_merge_h=new TH1F("ntop_merge_h","number of fully merged top, loose baseline",8,0.0,8.0);
	auto ntop_merge_175_h=new TH1F("ntop_merge_175_h","number of fully merged top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto ntop_w_h=new TH1F("ntop_w_h","number of partially merged top, loose baseline",8,0.0,8.0);
	auto ntop_w_175_h=new TH1F("ntop_w_175_h","number of partially merged top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto ntop_res_uc_h=new TH1F("ntop_res_uc_h","number of resolved top, no cuts",8,0.0,8.0);
	auto ntop_res_h=new TH1F("ntop_res_h","number of resolved top, loose baseline",8,0.0,8.0);
	auto ntop_res_175_h=new TH1F("ntop_res_175_h","number of resolved top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);

	auto gen_w_pt_175_h=new TH1F("gen_w_pt_175_h","gen W pt, loose baseline no HT cut, MTb > 175",80,0.0,400.0);
	auto gen_b_pt_175_h=new TH1F("gen_b_pt_175_h","gen bottom pt, loose baseline no HT cut, MTb > 175",80,0.0,400.0);
	auto gen_w_b_mass_175_h=new TH1F("gen_w_b_mass_175_h","sum of gen W and bottom mass, loose baseline no HT cut, MTb > 175",80,0.0,400.0);
	auto gen_w_b_delta_r_175_h=new TH1F("gen_w_b_delta_r_175_h","deltaR between gen W and bottom, loose baseline no HT cut, MTb > 175",80,0.0,8.0);
	auto n_res_gen_match_h=new TH1F("n_res_gen_match_h","number of gen matched W and bottom in one resolved top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto n_res_b_gen_match_h=new TH1F("n_res_b_gen_match_h","number of gen matched bottom in one resolved top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto n_res_w_gen_match_h=new TH1F("n_res_w_gen_match_h","number of gen matched W in one resolved top, loose baseline no HT cut, MTb > 175",8,0.0,8.0);

	// W
	auto nw_uc_h=new TH1F("nw_uc_h","nW, no cuts",8,0.0,8.0);
	auto nw_h=new TH1F("nw_h","nW, loose baseline",8,0.0,8.0);
	auto nw_no_HT_h=new TH1F("nw_no_HT_h","nW, loose baseline no HT cut",8,0.0,8.0);
	auto nw_140_h=new TH1F("nw_140_h","nW, loose baseline no HT cut, MTb > 140",8,0.0,8.0);
	auto nw_140_MT2_h=new TH1F("nw_140_MT2_h","nW, loose baseline no HT cut, MTb > 140, MT2 cut",8,0.0,8.0);
	auto nw_175_h=new TH1F("nw_175_h","nW, loose baseline no HT cut, MTb > 175",8,0.0,8.0);
	auto nw_175_MT2_h=new TH1F("nw_175_MT2_h","nW, loose baseline no HT cut, MTb > 175, MT2 cut",8,0.0,8.0);

	// HT
	auto HT_uc_h=new TH1F("HT_uc_h","HT, no cuts",100,0.0,3000.0);
	auto HT_h=new TH1F("HT_h","HT, loose baseline",100,0.0,3000.0);
	auto HT_no_HT_h=new TH1F("HT_no_HT_h","HT, loose baseline no HT cut",100,0.0,3000.0);
	auto HT_140_h=new TH1F("HT_140_h","HT, loose baseline no HT cut, MTb > 140",100,0.0,3000.0);
	auto HT_140_MT2_h=new TH1F("HT_140_MT2_h","HT, loose baseline no HT cut, MTb > 140, MT2 cut",100,0.0,3000.0);
	auto HT_175_h=new TH1F("HT_175_h","HT, loose baseline no HT cut, MTb > 175",100,0.0,3000.0);
	auto HT_175_MT2_h=new TH1F("HT_175_MT2_h","HT, loose baseline no HT cut, MTb > 175, MT2 cut",100,0.0,3000.0);
	auto genHT_h=new TH1F("genHT_h","genHT, loose baseline",100,0.0,3000.0);

	// Other
	auto ISRpt_lowdm_h=new TH1F("ISRpt_lowdm_h","ISR pt, low dm",80,0.0,1600.0);
	auto bottompt_scalar_sum_lowdm_h=new TH1F("bottompt_scalar_sum_lowdm_h","bottom pt sclar sum, low dm",80,0.0,1600.0);

	auto ISR_SF_uc_h=new TH1F("ISR_SF_uc_h","ISR SF, no evtWeight, no cuts",100,0.0,2.0);
	auto B_SF_uc_h=new TH1F("B_SF_uc_h","B tagging SF, no, evtWeight, no cuts",100,0.0,2.0);
	auto ISR_SF_h=new TH1F("ISR_SF_h","ISR SF, no evtWeight, loose baseline",100,0.0,2.0);
	auto B_SF_h=new TH1F("B_SF_h","B tagging SF, no, evtWeight, loose baseline",100,0.0,2.0);

	auto nMuons_uc_h=new TH1F("nMuons_uc_h","number of muons, no cuts",10,0.0,10.0);
	auto nElectrons_uc_h=new TH1F("nElectrons_uc_h","number of electrons, no cuts",10,0.0,10.0);

	auto loose_baseline_cutflow_h=new TH1F("loose_baseline_cutflow_h","0: all 1: PassEventFilter 2: PassLeptonVeto 3: PassNjets 4: PassMET 5: PassHT 6: PassdPhiLowDM",7,0.0,7.0);

	auto eff_h=new TH1F("eff_h","0: all. 1: loose baseline. 2: low dm. 3: high dm",4,0.0,4.0);

	// Search bin
	auto search_bin_h=new TH1F("search_bin_h","search bin with baseline cut",84,0.0,84.0);
	auto search_bin_singleMuCR_h=new TH1F("search_bin_singleMuCR_h","search bin with baseline cut, single muon control region",84,0.0,84.0);
	auto search_bin_singleElCR_h=new TH1F("search_bin_singleElCR_h","search bin with baseline cut, single electron control region",84,0.0,84.0);
	auto search_bin_8026_h=new TH1F("search_bin_8026_h","search bin with 8026 baseline cut",84,0.0,84.0);

	auto search_bin_MTb_h=new TH1F("search_bin_MTb_h","search bin with baseline cut + MTb > 175",84,0.0,84.0);
	auto search_bin_singleMuCR_MTb_h=new TH1F("search_bin_singleMuCR_MTb_h","search bin with baseline cut + MTb > 175, single muon control region",84,0.0,84.0);
	auto search_bin_singleElCR_MTb_h=new TH1F("search_bin_singleElCR_MTb_h","search bin with baseline cut + MTb > 175, single electron control region",84,0.0,84.0);

	auto search_bin_low_and_highdm_h=new TH1F("search_bin_low_and_highdm_h","search bin low and high dM, MTb = 175",177,0.0,177.0);
	auto search_bin_low_and_highdm_singleMuCR_h=new TH1F("search_bin_low_and_highdm_singleMuCR_h","search bin low and high dM, single muon control region, MTb = 175",177,0.0,177.0);
	auto search_bin_low_and_highdm_singleElCR_h=new TH1F("search_bin_low_and_highdm_singleElCR_h","search bin low and high dM, single electron control region, MTb = 175",177,0.0,177.0);

	auto search_bin_v2_h=new TH1F("search_bin_v2_h","search bin v2, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleMuCR_h=new TH1F("search_bin_v2_singleMuCR_h","search bin v2, single muon control region, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleElCR_h=new TH1F("search_bin_v2_singleElCR_h","search bin v2, single electron control region, MTb = 175",204,0.0,204.0);

	auto search_bin_more_HT_bins_h=new TH1F("search_bin_more_HT_bins_h","search bin with more HT bins, MTb = 175",239,0.0,239.0);
	auto search_bin_more_HT_bins_singleMuCR_h=new TH1F("search_bin_more_HT_bins_singleMuCR_h","search bin with more HT bins, single muon control region, MTb = 175",239,0.0,239.0);
	auto search_bin_more_HT_bins_singleElCR_h=new TH1F("search_bin_more_HT_bins_singleElCR_h","search bin with more HT bins, single electron control region, MTb = 175",239,0.0,239.0);

	auto search_bin_more_HT_bins_merge_h=new TH1F("search_bin_more_HT_bins_merge_h","search bin with more HT bins, MTb = 175, merge top",239,0.0,239.0);
	auto search_bin_more_HT_bins_singleMuCR_merge_h=new TH1F("search_bin_more_HT_bins_singleMuCR_merge_h","search bin with more HT bins, single muon control region, MTb = 175, merge top",239,0.0,239.0);
	auto search_bin_more_HT_bins_singleElCR_merge_h=new TH1F("search_bin_more_HT_bins_singleElCR_merge_h","search bin with more HT bins, single electron control region, MTb = 175, merge top",239,0.0,239.0);

	auto search_bin_highdm_h=new TH1F("search_bin_highdm_h","search bin high dM, MTb = 175",124,53.0,177.0);
	auto search_bin_highdm_singleMuCR_h=new TH1F("search_bin_highdm_singleMuCR_h","search bin high dM single muon control region, MTb = 175",124,53.0,177.0);
	auto search_bin_highdm_singleElCR_h=new TH1F("search_bin_highdm_singleElCR_h","search bin high dM single electron control region, MTb = 175",124,53.0,177.0);

	auto search_bin_highdm_MT2_h=new TH1F("search_bin_highdm_MT2_h","search bin high dM, MTb = 175 with MT2 bins",168,53.0,221.0);
	auto search_bin_highdm_singleMuCR_MT2_h=new TH1F("search_bin_highdm_singleMuCR_MT2_h","search bin high dM single muon control region, MTb = 175 with MT2 bins",168,53.0,221.0);
	auto search_bin_highdm_singleElCR_MT2_h=new TH1F("search_bin_highdm_singleElCR_MT2_h","search bin high dM single electron control region, MTb = 175 with MT2 bins",168,53.0,221.0);

	auto search_bin_highdm_merge_HT_h=new TH1F("search_bin_highdm_merge_HT_h","search bin high dM, MTb = 175, merge HT",124,53.0,177.0);
	auto search_bin_highdm_singleMuCR_merge_HT_h=new TH1F("search_bin_highdm_singleMuCR_merge_HT_h","search bin high dM single muon control region, MTb = 175, merge HT",124,53.0,177.0);
	auto search_bin_highdm_singleElCR_merge_HT_h=new TH1F("search_bin_highdm_singleElCR_merge_HT_h","search bin high dM single electron control region, MTb = 175, merge HT",124,53.0,177.0);

	auto search_bin_team_A_h=new TH1F("search_bin_team_A_h","search bin team A, MTb = 175",104,0.0,104.0);
	auto search_bin_team_A_singleMuCR_h=new TH1F("search_bin_team_A_singleMuCR_h","search bin team A single muon control region, MTb = 175",104,0.0,104.0);
	auto search_bin_team_A_singleElCR_h=new TH1F("search_bin_team_A_singleElCR_h","search bin team A single electron control region, MTb = 175",104,0.0,104.0);

	auto search_bin_team_A_highdm_merge_h=new TH1F("search_bin_team_A_highdm_merge_h","search bin team A high dM, MTb = 175, merge top",51,53.0,104.0);
	auto search_bin_team_A_highdm_singleMuCR_merge_h=new TH1F("search_bin_team_A_highdm_singleMuCR_merge_h","search bin team A high dM single muon control region, MTb = 175, merge top",51,53.0,104.0);
	auto search_bin_team_A_highdm_singleElCR_merge_h=new TH1F("search_bin_team_A_highdm_singleElCR_merge_h","search bin team A high dM single electron control region, MTb = 175, merge top",51,53.0,104.0);

	auto search_bin_team_A_highdm_MTb175_MT2_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_h","search bin team A high dM, MTb = 175, MT2 >200",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h","search bin team A high dM, MTb = 175, MT2 >200 single muon control region",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb175_MT2_singleElCR_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_singleElCR_h","search bin team A high dM, MTb = 175, MT2 >200 single electron control region",51,53.0,104.0);

	auto search_bin_team_A_highdm_MTb140_MT2_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_h","search bin team A high dM, MTb = 140, MT2 >200",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h","search bin team A high dM, MTb = 140, MT2 >200 single muon control region",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb140_MT2_singleElCR_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_singleElCR_h","search bin team A high dM, MTb = 140, MT2 >200 single electron control region",51,53.0,104.0);

	// 2-D Histograms

	TH2F *mtb_mt2_uc_h = new TH2F("mtb_mt2_uc_h","MTb and MT2 (No Cuts)",70,0.0,350.0,70,0.0,350.0);
	TH2F *mtb_mt2_h = new TH2F("mtb_mt2_h","MTb and MT2 (Baseline Cuts)",70,0.0,350.0,70,0.0,350.0);
	TH2F *ntop_nw_h = new TH2F("ntop_nw_h","Ntop and Nw correlation",8,0.0,8.0,8,0.0,8.0);
	TH2F *ISRpt_MET_lowdm_h = new TH2F("ISRpt_MET_lowdm_h","ISRpt vs MET in low dm",80,0.0,1600.0,80,0.0,1600.0);

	int n_fail_mtb = 0, n_fail_met = 0, n_fail_dPhi = 0;
	int n_old_bin_is_83 = 0, n_highdm_is_176 = 0;

	// ---------- Begin Loop Over Events ----------

	if(debug) std::cout << __LINE__ << std::endl;

	while(tr.getNextEvent()){

		/*if(tr.getEvtNum() == 1){
		  tr.printTupleMembers();
		  FILE * fout = fopen("NTupleTypes.txt", "w");
		  tr.printTupleMembers(fout);
		  fclose(fout);
		  }*/

		if(debug) std::cout << __LINE__ << std::endl;

		if(max_events > 0 && tr.getEvtNum() > max_events) break;

		if(tr.getEvtNum() % 1000 == 0){
			std::cout << "Event Number " << tr.getEvtNum() << std::endl;
		}

		//  std::cout << "MET " << tr.getVar<float>("met")  << " nTop" << tr.getVar<int>("nTopCandSortedCnt") << " Phi " << tr.getVar<float>("metphi") << std::endl;
		if(debug) std::cout << __LINE__ << std::endl;

		std::vector<TLorentzVector> jetsLVec = tr.getVec<TLorentzVector>("prodJetsNoLep_jetsLVec");
		std::vector<TLorentzVector> jetsLVec_pt20;
		std::vector<TLorentzVector> wLVec;
		if(use_new_tagger) wLVec = tr.getVec<TLorentzVector>("vWs");
		else wLVec = tr.getVec<TLorentzVector>("vWAlone");
		std::vector<TLorentzVector> softbLVec = tr.getVec<TLorentzVector>("softbLVec");
		std::vector<TLorentzVector> ISRLVec = tr.getVec<TLorentzVector>("vISRJet");
		float ISRpt =0;
		if(ISRLVec.size() == 1) ISRpt = ISRLVec.at(0).Pt();

		int nw_tagger = wLVec.size();
		int nSV = softbLVec.size();
		//std::cout << "number of w = " << wLVec.size() << std::endl;

		std::vector<TLorentzVector> b_jetsLVec;
		std::vector<float> jpt_eta_2p5_3p0;
		std::vector<float> rjbt_vec = tr.getVec<float>("prodJetsNoLep_recoJetsCSVv2");
		int njets = jetsLVec.size();
		float ISR_SF = 1.0;
		if (do_ISR_BSF && find_ISR_SF) ISR_SF = tr.getVar<float>("isr_Unc_Cent");
		float B_SF = 1.0;
		if(do_ISR_BSF && find_B_SF) B_SF = tr.getVar<float>("bTagSF_EventWeightSimple_Central");
		float evtWeight = tr.getVar<float>("evtWeight");
		if(do_ISR_BSF) evtWeight = evtWeight*ISR_SF*B_SF;
		float met = tr.getVar<float>("met"); 
		float genmet = tr.getVar<float>("genmet");
		if(find_fastsim) met = (met + genmet) / 2; 
		//float mt2 = tr.getVar<float>("mt2");
		float mt2 = tr.getVar<float>("best_had_brJet_MT2");
		bool passMT2 = (mt2 > 200);
		float metphi = tr.getVar<float>("metphi");
		int nElectrons = tr.getVar<int>("nElectrons_CUT");
		int nMuons = tr.getVar<int>("nMuons_CUT");
		int ntop = tr.getVar<int>("nTopCandSortedCnt");
		int nbjets20_from_baseline = tr.getVar<int>("cntCSVS");
		int njets20_from_baseline = tr.getVar<int>("cntNJetsPt20Eta24");
		float mtb=0 , mt2_b=0;
		int nbottompt20=0 , nbottompt30=0 , njetspt20=0 , njetspt30=0 , njetspt50=0;
		int ntop_merge=0 , ntop_w=0 , ntop_res=0;
		int nw=0;
		float HT = tr.getVar<float>("HT");
		float S_met = met / sqrt(HT);
		float genHT = tr.getVar<float>("genHT");
		float bad_b_csv = 0;
		TLorentzVector bad_b_tlv;
		float b_CSV_1 = 0, b_CSV_2 = 0;
		int b_CSV_1_index = 0, b_CSV_2_index = 0;

		int SB_index = SB.find_Binning_Index(nbjets20_from_baseline, ntop, mt2, met, HT);
		std::map<int, std::vector<TLorentzVector>> mTopJets = tr.getMap <int, std::vector<TLorentzVector>> ("mTopJets");

		bool old_bin_is_83 = false;
		bool highdm_is_176 = false;

		const std::vector<int>& PdgId = tr.getVec<int>("genDecayPdgIdVec");
		const std::vector<int>& MomRefId = tr.getVec<int>("genDecayMomRefVec");
		const std::vector<TLorentzVector>& gen_LVec = tr.getVec<TLorentzVector>("genDecayLVec");
		int n_gen_w = 0, n_gen_b = 0;

		if(debug) std::cout << __LINE__ << std::endl;

		if(tr.getEvtNum() < 10)
		{
			if(tr.getEvtNum() == 1)
			{
				std::cout << "\n" << std::endl;
				std::cout << "NEVTS: " << tr.getNEntries() << std::endl;
				std::cout << "\n" << std::endl;
			}

			std::cout << "event " << tr.getEvtNum() << " ISR_SF " << ISR_SF << " B_SF " << B_SF << " PdgId " << std::endl; 
			for(int i=0; i < PdgId.size(); i++)
			{std::cout << PdgId.at(i) << " "; }
			std::cout << "\n" << std::endl;
		}

		// ---------- Begin Sub Loop Over Jets ----------
		if(debug) std::cout << __LINE__ << std::endl;

		for(int index=0; index < njets ; index++)
		{
			TLorentzVector tlv = jetsLVec[index];
			float rjbt = rjbt_vec[index];		//rjbt stands for reco jet b tag value
			float tlv_Phi = tlv.Phi();
			float tlv_Pt = tlv.Pt();
			float tlv_Eta = tlv.Eta();
			float dPhi = tlv_Phi - metphi;

			if(fabs(tlv_Eta) <= 2.4 && tlv_Pt >= 20)
			{
				njetspt20++;
				jetsLVec_pt20.push_back(tlv);
				if (tlv_Pt >= 30) njetspt30++;
				if (tlv_Pt >= 50) njetspt50++;
			}

			if(fabs(tlv_Eta) > 2.5 && fabs(tlv_Eta) < 3.0 && tlv_Pt >= 20) jpt_eta_2p5_3p0.push_back(tlv_Pt); 

			if(rjbt < 0.8484 || tlv_Pt < 20 || fabs(tlv_Eta) > 2.4)
			{
				if (rjbt > bad_b_csv)
				{
					bad_b_csv = rjbt;
					bad_b_tlv = tlv;
				}
				continue;
			}

			if(rjbt > b_CSV_1)
			{
				b_CSV_2 = b_CSV_1;
				b_CSV_2_index = b_CSV_1_index;
				b_CSV_1 = rjbt;
				b_CSV_1_index = index;
			}

			if(rjbt < b_CSV_1 && rjbt > b_CSV_2)
			{
				b_CSV_2 = rjbt;
				b_CSV_2_index = index;
			}

			b_jetsLVec.push_back(tlv);
			nbottompt20++;
			if(tlv_Pt > 30) nbottompt30++;
		} // End Sub-Loop Over Jets

		if(njetspt20 != njets20_from_baseline) std::cout << "njetspt20 = " << njetspt20 << " njets20_from_baseline = " << njets20_from_baseline << std::endl;
		if(!use_deepCSV && (nbottompt20 != nbjets20_from_baseline)) std::cout << "nbottom (pt > 20) = " << nbottompt20 << " nbjets20_from_baseline = " << nbjets20_from_baseline << std::endl;
		if(use_deepCSV) nbottompt20 = nbjets20_from_baseline;

		float bottompt_scalar_sum = 0;
		if(b_jetsLVec.size() == 1) bottompt_scalar_sum = b_jetsLVec.at(0).Pt();	
		if(b_jetsLVec.size() > 1) bottompt_scalar_sum = b_jetsLVec.at(0).Pt() + b_jetsLVec.at(1).Pt();
		if(use_deepCSV) bottompt_scalar_sum = tr.getVar<float>("Ptb");
		//if(bottompt_scalar_sum != tr.getVar<float>("Ptb")) std::cout << "bottompt_scalar_sum = " << bottompt_scalar_sum << " Ptb = " << tr.getVar<float>("Ptb") << std::endl;
		//if(tr.getEvtNum() < 4) std::cout << "bottompt_scalar_sum = " << bottompt_scalar_sum << " Ptb = " << tr.getVar<float>("Ptb") << std::endl;

		if(debug) std::cout << __LINE__ << std::endl;
		/*for (int i=0; i<b_jetsLVec.size(); i++)
		  {
		  std::cout << "event " << tr.getEvtNum() << " # " << i << " bjet pt " << b_jetsLVec.at(i).Pt() << std::endl;
		  }*/

		// ===  FUNCTION  ============================================================
		//         Name:  BaselineVessel::CalcMT2
		//  Description:  
		// ===========================================================================

		TLorentzVector fatJet1LVec(0, 0, 0, 0);
		TLorentzVector fatJet2LVec(0, 0, 0, 0);

		if (b_jetsLVec.size() == 0)
		{
			mt2_b = 0.0;
			mtb = 0.0; 
		}

		if (b_jetsLVec.size() == 1)
		{
			fatJet1LVec = b_jetsLVec.at(0);
			fatJet2LVec = bad_b_tlv;

			mt2_b = blv.coreMT2calc(fatJet1LVec, fatJet2LVec);
			float tlv_Pt_1 = jetsLVec[b_CSV_1_index].Pt();
			float tlv_Phi_1 = jetsLVec[b_CSV_1_index].Phi();
			float MT_1 = sqrt(2*tlv_Pt_1*met*(1-cos(tlv_Phi_1 - metphi)));
			mtb = MT_1;
		}

		if (b_jetsLVec.size() >= 2)
		{
			std::vector<float> cachedMT2vec;
			for(unsigned int it=0; it<b_jetsLVec.size(); it++)
			{    
				for(unsigned int jt=it+1; jt<b_jetsLVec.size(); jt++)
				{
					cachedMT2vec.push_back(blv.coreMT2calc(b_jetsLVec.at(it), b_jetsLVec.at(jt)));
				}
			}
			std::sort(cachedMT2vec.begin(), cachedMT2vec.end());

			mt2_b = cachedMT2vec.front();
			//    return cachedMT2vec.back();

			float tlv_Pt_1 = jetsLVec[b_CSV_1_index].Pt();
			float tlv_Phi_1 = jetsLVec[b_CSV_1_index].Phi();
			float MT_1 = sqrt(2*tlv_Pt_1*met*(1-cos(tlv_Phi_1 - metphi)));
			float tlv_Pt_2 = jetsLVec[b_CSV_2_index].Pt();
			float tlv_Phi_2 = jetsLVec[b_CSV_2_index].Phi();
			float MT_2 = sqrt(2*tlv_Pt_2*met*(1-cos(tlv_Phi_2 - metphi)));
			mtb = std::min(MT_1,MT_2);
		}

		if(use_deepCSV) mtb = tr.getVar<float>("Mtb");
		//if(mtb != tr.getVar<float>("Mtb")) std::cout << "mtb = " << mtb << " Mtb = " << tr.getVar<float>("Mtb") << " b_jetsLVec.size() = " << b_jetsLVec.size() << " tr.getVec<float>('vBjs').size() = " << tr.getVec<float>("vBjs").size() << std::endl;
		//if(tr.getEvtNum() < 4) std::cout << "mtb = " << mtb << " Mtb = " << tr.getVar<float>("Mtb") << " b_jetsLVec.size() = " << b_jetsLVec.size() << " tr.getVec<TLorentzVector>('vBjs').size() = " << tr.getVec<TLorentzVector>("vBjs").size() << std::endl;
		if(debug) std::cout << __LINE__ << std::endl;

		//if(ntop > 0)
		//{
		//std::cout << "nTopCandSortedCnt = " << ntop << " mTopJets.size() = " << mTopJets.size() << std::endl;
		//std::cout << "mTopJets.begin()->first = " << mTopJets.begin()->first  << std::endl; 
		//std::cout << "nTopCandSortedCnt = " << ntop << " mTopJets.size() = " << mTopJets.size() << " mTopJets.at(0).size() = " << mTopJets.at(0).size() << std::endl;
		//}
		for (int i = 0; i < ntop; i++)
		{
			//std::cout << "top index " << i << std::endl;
			if (mTopJets.at(i).size() ==1) ntop_merge++;
			if (mTopJets.at(i).size() ==2) ntop_w++;
			if (mTopJets.at(i).size() ==3) ntop_res++;
			//for (int k = 0; k < mTopJets.at(i).size(); k++) std::cout << "top daughter " << k << " mass = " << mTopJets.at(i).at(k).M() << std::endl;
		}

		if(use_new_tagger && ntop != ntop_merge + ntop_res) std::cout << "ntop = " << ntop << " ntop_merge = " << ntop_merge << " ntop_res = " << ntop_res << std::endl;
		if(use_new_tagger) nw = nw_tagger;
		else nw = nw_tagger + ntop_w;	
		if(debug) std::cout << __LINE__ << std::endl;

		int SB_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SBv2_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SB_highdm_more_HT_bins(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SB_highdm_MT2(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht, float mt2);
		int SB_team_A_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met);

		int SB_highdm_index_175 = SB_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		int SBv2_highdm_index_175 = SBv2_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		int SB_highdm_more_HT_bins_index_175 = SB_highdm_more_HT_bins(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		int SB_highdm_more_HT_bins_merge_index_175 = SB_highdm_more_HT_bins(175, mtb, njetspt20, 0, nw, ntop, nbottompt20, met, HT);
		int SB_highdm_MT2_index_175 = SB_highdm_MT2(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT, mt2);
		int SB_highdm_merge_HT_index_175 = SB_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, 500);
		int SB_team_A_highdm_index_175 = SB_team_A_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met);
		int SB_team_A_highdm_merge_index_175 = SB_team_A_highdm(175, mtb, njetspt20, 0, nw, ntop, nbottompt20, met);
		int SB_team_A_highdm_index_140 = SB_team_A_highdm(140, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met);

		int SB_team_A_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SB_team_A_lowdm_index = SB_team_A_lowdm(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);

		bool passnJets = (njetspt50 >= 2 && njetspt30 >= 4);		//SUS-16-050, 4 jets30 and 2 jets50
		bool passdphi_highdm = (jetsLVec_pt20.size() >= 4 && my_dPhi(jetsLVec_pt20.at(0).Phi(), metphi) > 0.5 && my_dPhi(jetsLVec_pt20.at(1).Phi(), metphi) > 0.5 && my_dPhi(jetsLVec_pt20.at(2).Phi(), metphi) > 0.5 && my_dPhi(jetsLVec_pt20.at(3).Phi(), metphi) > 0.5);		//SUS-16-049, high dm, dphi(met, jet1234) > 0.5
		bool passdphi_lowdm = ( (jetsLVec_pt20.size() == 2 && my_dPhi(jetsLVec_pt20.at(0).Phi(), metphi) > 0.5 && my_dPhi(jetsLVec_pt20.at(1).Phi(), metphi) > 0.15) || (jetsLVec_pt20.size() >2 && my_dPhi(jetsLVec_pt20.at(0).Phi(), metphi) > 0.5 && my_dPhi(jetsLVec_pt20.at(1).Phi(), metphi) > 0.15 && my_dPhi(jetsLVec_pt20.at(2).Phi(), metphi) > 0.15) );		//SUS-16-049, low dm, dphi(met, j1) > 0.5, dphi(met, j23) > 0.15

		bool pass_loose_baseline_no_HT=(tr.getVar<bool>("passLeptVeto") && passdphi_lowdm && tr.getVar<bool>("passMET") && njetspt20 >=2 && tr.getVar<bool>("passNoiseEventFilter"));		//baseline line for merge group 
		bool pass_loose_baseline=(pass_loose_baseline_no_HT && tr.getVar<bool>("passHT"));

		//bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passnJets") && tr.getVar<bool>("passdPhis") && tr.getVar<bool>("passBJets") && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && passnJets && tr.getVar<bool>("passdPhis") && nbjets20_from_baseline > 0 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline=(pass_baseline_no_MT2 && passMT2);		//baseline for SUS-16-050
		bool pass_baseline_singleLeptCR = (passMT2 && passnJets && tr.getVar<bool>("passdPhis") && nbjets20_from_baseline > 0 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") );
		bool pass_high_dM_baseline=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passMET") && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 5 && passdphi_highdm && nbottompt20 >=1 && tr.getVar<bool>("passHT"));		//baseline for SUS-16-049 high dm plus HT cut 
		bool pass_high_dM_baseline_singleLeptCR=(tr.getVar<bool>("passMET") && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 5 && passdphi_highdm && nbottompt20 >=1 && tr.getVar<bool>("passHT"));		//baseline without lept veto for SUS-16-049 high dm plus HT cut
		bool pass_MT2_highdm = (ntop == 0 || (ntop >0 && passMT2));
		bool pass_mtb_lowdm = (nbottompt20 == 0 || (nbottompt20 >0 && mtb <175)); 		//SUS-16-049, low dm, mtb cut
		bool pass_ISR = (ISRpt > 200 && fabs(ISRLVec.at(0).Eta()) < 2.4 && fabs(ISRLVec.at(0).Phi() - metphi) > 2); 		//SUS-16-049, low dm, ISR cut
		bool pass_low_dM_baseline=(tr.getVar<bool>("passLeptVeto") && ntop == 0 && nw == 0 && pass_ISR && S_met > 10 && passdphi_lowdm && pass_mtb_lowdm && tr.getVar<bool>("passMET")  && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 2); 		//baseline for SUS-16-049 low dm
		bool pass_low_dM_baseline_singleLeptCR=(ntop == 0 && nw == 0 && pass_ISR && S_met > 10 && passdphi_lowdm && pass_mtb_lowdm && tr.getVar<bool>("passMET")  && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 2); 		//baseline without lept veto for SUS-16-049 low dm
		if(debug) std::cout << __LINE__ << std::endl;

		// ---------- Fill Histograms ----------
		// cutflow
		loose_baseline_cutflow_h->Fill(0);
		if(tr.getVar<bool>("passNoiseEventFilter"))loose_baseline_cutflow_h->Fill(1);
		if(tr.getVar<bool>("passNoiseEventFilter") && tr.getVar<bool>("passLeptVeto"))loose_baseline_cutflow_h->Fill(2);
		if(tr.getVar<bool>("passNoiseEventFilter") && tr.getVar<bool>("passLeptVeto") && njetspt20 >=2)loose_baseline_cutflow_h->Fill(3);
		if(tr.getVar<bool>("passNoiseEventFilter") && tr.getVar<bool>("passLeptVeto") && njetspt20 >=2 && tr.getVar<bool>("passMET"))loose_baseline_cutflow_h->Fill(4);
		if(tr.getVar<bool>("passNoiseEventFilter") && tr.getVar<bool>("passLeptVeto") && njetspt20 >=2 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT"))loose_baseline_cutflow_h->Fill(5);
		if(tr.getVar<bool>("passNoiseEventFilter") && tr.getVar<bool>("passLeptVeto") && njetspt20 >=2 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && passdphi_lowdm)loose_baseline_cutflow_h->Fill(6);

		// no cut
		HT_uc_h->Fill(HT,evtWeight);
		met_uc_h->Fill(met,evtWeight);
		njets_uc_h->Fill(njets,evtWeight);
		njetspt20_uc_h->Fill(njetspt20,evtWeight);
		nbottompt20_uc_h->Fill(nbottompt20,evtWeight);
		ntop_merge_uc_h->Fill(ntop_merge,evtWeight);
		ntop_res_uc_h->Fill(ntop_res,evtWeight);
		nw_uc_h->Fill(nw,evtWeight);
		mtb_mt2_uc_h->Fill(mtb,mt2,evtWeight);
		mtb_uc_h->Fill(mtb,evtWeight);
		nMuons_uc_h->Fill(nMuons,evtWeight);
		nElectrons_uc_h->Fill(nElectrons,evtWeight);
		ISR_SF_uc_h->Fill(ISR_SF);
		B_SF_uc_h->Fill(B_SF);
		if(tr.getVar<bool>("passLeptVeto")) njetspt20_lept_veto_h->Fill(njetspt20,evtWeight);
		//MT2 calculation is currently wrong in baseline. Hope can be fixed
		//if (ntop == 0 && mt2 != 0) std::cout << "MT2 = " << mt2 << std::endl;

		eff_h->Fill(0.0,evtWeight);

		if(pass_high_dM_baseline)
		{
			met_highdm_h->Fill(met,evtWeight);
			mtb_highdm_h->Fill(mtb,evtWeight);
			eff_h->Fill(3.0,evtWeight);

			if(mtb < 140) mt2_140_low_h->Fill(mt2,evtWeight);
			if(mtb > 140) mt2_140_high_h->Fill(mt2,evtWeight);
			if(mtb < 175) mt2_175_low_h->Fill(mt2,evtWeight);
			if(mtb > 175) mt2_175_high_h->Fill(mt2,evtWeight);
			if(mtb > 140 && mtb < 175) mt2_140_175_h->Fill(mt2,evtWeight);

			if(SB_highdm_more_HT_bins_index_175 != -1 && SB_highdm_more_HT_bins_merge_index_175 == -1) std::cout << "mtb, " << mtb << " njetspt20, " << njetspt20 << " ntop_merge, " << ntop_merge << " nw, " << nw << " ntop_res, " << ntop_res << " nbottompt20, " << nbottompt20 << " met, " << met << " HT, " << HT  << std::endl;

			if(SB_highdm_index_175 != -1)
			{
				if(SB_highdm_index_175 == 176) {highdm_is_176 = true; n_highdm_is_176++;}
				search_bin_low_and_highdm_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_h->Fill(SB_highdm_index_175,evtWeight);
			}
			if(SBv2_highdm_index_175 != -1) search_bin_v2_h->Fill(SBv2_highdm_index_175,evtWeight);
			if(met > 250 && mtb > 175 && !(ntop_merge == 0 && nw == 0 && ntop_res == 0) && SBv2_highdm_index_175 == -1)
			std::cout << "SBv2_highdm check: nbottompt20 " << nbottompt20 << " ntop_merge " << ntop_merge << " nw " << nw << " ntop_res " << ntop_res << " met " << met << " HT " << HT << std::endl;
 
			if(SB_highdm_more_HT_bins_index_175 != -1) search_bin_more_HT_bins_h->Fill(SB_highdm_more_HT_bins_index_175,evtWeight);
			if(SB_highdm_more_HT_bins_merge_index_175 != -1) search_bin_more_HT_bins_merge_h->Fill(SB_highdm_more_HT_bins_merge_index_175,evtWeight);
			if(SB_highdm_MT2_index_175 != -1) search_bin_highdm_MT2_h->Fill(SB_highdm_MT2_index_175,evtWeight);
			if(SB_highdm_merge_HT_index_175 != -1) search_bin_highdm_merge_HT_h->Fill(SB_highdm_merge_HT_index_175,evtWeight);

			if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_h->Fill(SB_team_A_highdm_index_175,evtWeight);
			if(SB_team_A_highdm_merge_index_175 != -1) search_bin_team_A_highdm_merge_h->Fill(SB_team_A_highdm_merge_index_175,evtWeight);
			if(pass_MT2_highdm) 
			{
				if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_highdm_MTb175_MT2_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				if(SB_team_A_highdm_index_140 != -1) search_bin_team_A_highdm_MTb140_MT2_h->Fill(SB_team_A_highdm_index_140,evtWeight);
			}
			//=================================== MET binning study ==========================================
			if(mtb > 175)
			{
				if(nbottompt20 == 1 && ntop_merge >= 1 && nw == 0 && ntop_res == 0) met_1b_100_h->Fill(met,evtWeight);
				if(nbottompt20 == 1 && ntop_merge == 0 && nw >= 1 && ntop_res == 0) met_1b_010_h->Fill(met,evtWeight);
				if(nbottompt20 == 1 && ntop_merge == 0 && nw == 0 && ntop_res >= 1) 
				{met_1b_001_h->Fill(met,evtWeight);
				if(met > 250 && (SB_highdm_more_HT_bins_index_175 < 75 || SB_highdm_more_HT_bins_index_175 > 89))
				std::cout << "SB_highdm_more_HT_bins: nbottompt20 " << nbottompt20 << " ntop_merge " << ntop_merge << " nw " << nw << " ntop_res " << ntop_res << " met " << met << " HT " << HT << std::endl;
				}
				if(nbottompt20 == 1 && ntop_merge >= 1 && nw >= 1 && ntop_res == 0) met_1b_110_h->Fill(met,evtWeight);
				if(nbottompt20 == 1 && ntop_merge == 0 && nw >= 1 && ntop_res >= 1) met_1b_011_h->Fill(met,evtWeight);
				if(nbottompt20 == 1 && ntop_merge >= 1 && nw == 0 && ntop_res >= 1) met_1b_101_h->Fill(met,evtWeight);

				if(nbottompt20 == 2 && ntop_merge == 1 && nw == 0 && ntop_res == 0) met_2b_100_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 0 && nw == 1 && ntop_res == 0) met_2b_010_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 0 && nw == 0 && ntop_res == 1) met_2b_001_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 1 && nw == 1 && ntop_res == 0) met_2b_110_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 0 && nw == 1 && ntop_res == 1) met_2b_011_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 1 && nw == 0 && ntop_res == 1) met_2b_101_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 2 && nw == 0 && ntop_res == 0) met_2b_200_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 0 && nw == 2 && ntop_res == 0) met_2b_020_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge == 0 && nw == 0 && ntop_res == 2) met_2b_002_h->Fill(met,evtWeight);
				if(nbottompt20 == 2 && ntop_merge + nw + ntop_res >= 3) met_2b_3_h->Fill(met,evtWeight);

				if(nbottompt20 >= 3 && ntop_merge == 1 && nw == 0 && ntop_res == 0) met_3b_100_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 0 && nw == 1 && ntop_res == 0) met_3b_010_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 0 && nw == 0 && ntop_res == 1) met_3b_001_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 1 && nw == 1 && ntop_res == 0) met_3b_110_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 0 && nw == 1 && ntop_res == 1) met_3b_011_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 1 && nw == 0 && ntop_res == 1) met_3b_101_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 2 && nw == 0 && ntop_res == 0) met_3b_200_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 0 && nw == 2 && ntop_res == 0) met_3b_020_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge == 0 && nw == 0 && ntop_res == 2) met_3b_002_h->Fill(met,evtWeight);
				if(nbottompt20 >= 3 && ntop_merge + nw + ntop_res >= 3) met_3b_3_h->Fill(met,evtWeight);
			}

		}

		if(pass_high_dM_baseline_singleLeptCR)
		{
			if(nElectrons == 1)
			{
				if(SB_highdm_index_175 != -1) search_bin_low_and_highdm_singleElCR_h->Fill(SB_highdm_index_175,evtWeight);
				if(SB_highdm_index_175 != -1) search_bin_highdm_singleElCR_h->Fill(SB_highdm_index_175,evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SB_highdm_more_HT_bins_index_175 != -1) search_bin_more_HT_bins_singleElCR_h->Fill(SB_highdm_more_HT_bins_index_175,evtWeight);
				if(SB_highdm_more_HT_bins_merge_index_175 != -1) search_bin_more_HT_bins_singleElCR_merge_h->Fill(SB_highdm_more_HT_bins_merge_index_175,evtWeight);
				if(SB_highdm_MT2_index_175 != -1) search_bin_highdm_singleElCR_MT2_h->Fill(SB_highdm_MT2_index_175,evtWeight);
				if(SB_highdm_merge_HT_index_175 != -1) search_bin_highdm_singleElCR_merge_HT_h->Fill(SB_highdm_merge_HT_index_175,evtWeight);

				if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_singleElCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				if(SB_team_A_highdm_merge_index_175 != -1) search_bin_team_A_highdm_singleElCR_merge_h->Fill(SB_team_A_highdm_merge_index_175,evtWeight);
				if(pass_MT2_highdm) 
				{
					if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_highdm_MTb175_MT2_singleElCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
					if(SB_team_A_highdm_index_140 != -1) search_bin_team_A_highdm_MTb140_MT2_singleElCR_h->Fill(SB_team_A_highdm_index_140,evtWeight);
				}
			}

			if(nMuons == 1)
			{
				if(SB_highdm_index_175 != -1) search_bin_low_and_highdm_singleMuCR_h->Fill(SB_highdm_index_175,evtWeight);
				if(SB_highdm_index_175 != -1) search_bin_highdm_singleMuCR_h->Fill(SB_highdm_index_175,evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SB_highdm_more_HT_bins_index_175 != -1) search_bin_more_HT_bins_singleMuCR_h->Fill(SB_highdm_more_HT_bins_index_175,evtWeight);
				if(SB_highdm_more_HT_bins_merge_index_175 != -1) search_bin_more_HT_bins_singleMuCR_merge_h->Fill(SB_highdm_more_HT_bins_merge_index_175,evtWeight);
				if(SB_highdm_MT2_index_175 != -1) search_bin_highdm_singleMuCR_MT2_h->Fill(SB_highdm_MT2_index_175,evtWeight);
				if(SB_highdm_merge_HT_index_175 != -1) search_bin_highdm_singleMuCR_merge_HT_h->Fill(SB_highdm_merge_HT_index_175,evtWeight);

				if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_singleMuCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				if(SB_team_A_highdm_merge_index_175 != -1) search_bin_team_A_highdm_singleMuCR_merge_h->Fill(SB_team_A_highdm_merge_index_175,evtWeight);
				if(pass_MT2_highdm) 
				{
					if(SB_team_A_highdm_index_175 != -1) search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
					if(SB_team_A_highdm_index_140 != -1) search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h->Fill(SB_team_A_highdm_index_140,evtWeight);
				}
			}
		}

		if(pass_low_dM_baseline)
		{
			eff_h->Fill(2.0,evtWeight);
			met_lowdm_h->Fill(met,evtWeight);
			njetspt20_lowdm_h->Fill(njetspt20,evtWeight);
			nbottompt20_lowdm_h->Fill(nbottompt20,evtWeight);
			nSV_lowdm_h->Fill(nSV,evtWeight);
			ISRpt_lowdm_h->Fill(ISRpt,evtWeight);
			bottompt_scalar_sum_lowdm_h->Fill(bottompt_scalar_sum,evtWeight);
			ISRpt_MET_lowdm_h->Fill(ISRpt,met,evtWeight);

			if(SB_team_A_lowdm_index != -1) 
			{
				search_bin_low_and_highdm_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_v2_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_merge_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_team_A_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}
		}

		if(pass_low_dM_baseline_singleLeptCR)
		{
			if(nElectrons == 1 && SB_team_A_lowdm_index != -1) 
			{
				search_bin_low_and_highdm_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_v2_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_singleElCR_merge_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_team_A_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}

			if(nMuons == 1 && SB_team_A_lowdm_index != -1)
			{
				search_bin_low_and_highdm_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_v2_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_more_HT_bins_singleMuCR_merge_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_team_A_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}
		}


		if(pass_baseline_no_MT2) mt2_baseline_no_mt2_h->Fill(mt2,evtWeight);

		if(pass_baseline)
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets20_from_baseline = " << nbjets20_from_baseline << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
			search_bin_h->Fill(SB_index,evtWeight);
			if (SB_index == 83) {old_bin_is_83 = true; n_old_bin_is_83++;}
			if (mtb > 175) search_bin_MTb_h->Fill(SB_index,evtWeight);
		}

		if(tr.getVar<bool>("passBaseline"))
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets20_from_baseline = " << nbjets20_from_baseline << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
			search_bin_8026_h->Fill(SB_index,evtWeight);
		}

		if(pass_baseline_singleLeptCR)
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets20_from_baseline = " << nbjets20_from_baseline << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
			if(nElectrons == 1)
			{
				search_bin_singleElCR_h->Fill(SB_index,evtWeight);
				if (mtb > 175) search_bin_singleElCR_MTb_h->Fill(SB_index,evtWeight);
			}
			if(nMuons == 1)
			{
				search_bin_singleMuCR_h->Fill(SB_index,evtWeight);
				if (mtb > 175) search_bin_singleMuCR_MTb_h->Fill(SB_index,evtWeight);
			}
		}

		if(old_bin_is_83 && (!highdm_is_176))
		{
			//std::cout << "njetspt20 = " << njetspt20 << ", ntop = " << ntop << ", ntop_merge = " << ntop_merge << ", nw = " << nw << ", nw_tagger = " << nw_tagger << ", ntop_w = " << ntop_w << ", ntop_res = " << ntop_res << ", nbottompt20 = " << nbottompt20 << ", mtb = " << mtb << ", met = " << met << ", genmet = " << genmet << ", HT = " << HT << ", passdphi_highdm = " << passdphi_highdm << std::endl;
			if (mtb < 175) n_fail_mtb++;
			if (met < 400) n_fail_met++;
			if (passdphi_highdm == 0) n_fail_dPhi++;
		}


		if(pass_loose_baseline)
		{
			met_h->Fill(met,evtWeight);
			mt2_h->Fill(mt2,evtWeight); 
			mt2_b_h->Fill(mt2_b,evtWeight); 
			mtb_mt2_h->Fill(mtb,mt2,evtWeight);
			mtb_h->Fill(mtb,evtWeight);
			ntop_h->Fill(ntop,evtWeight);
			ntop_merge_h->Fill(ntop_merge,evtWeight);
			ntop_w_h->Fill(ntop_w,evtWeight);
			ntop_res_h->Fill(ntop_res,evtWeight);
			nw_h->Fill(nw,evtWeight);
			nbottompt20_h->Fill(nbottompt20,evtWeight);
			njets_h->Fill(njets,evtWeight);
			njetspt20_h->Fill(njetspt20,evtWeight);
			njetspt30_h->Fill(njetspt30,evtWeight);
			HT_h->Fill(HT,evtWeight);
			genHT_h->Fill(genHT,evtWeight);
			genmet_h->Fill(genmet,evtWeight);
			if(jpt_eta_2p5_3p0.size() == 0) jpt_eta_2p5_3p0_h->Fill(0.0,evtWeight);
			else jpt_eta_2p5_3p0_h->Fill(jpt_eta_2p5_3p0.at(0),evtWeight);

			ISR_SF_h->Fill(ISR_SF);
			B_SF_h->Fill(B_SF);

			eff_h->Fill(1.0,evtWeight);
		}

		if(pass_loose_baseline_no_HT)
		{
			mtb_no_HT_h->Fill(mtb,evtWeight);
			mt2_no_HT_h->Fill(mt2,evtWeight);
			nbottompt20_no_HT_h->Fill(nbottompt20,evtWeight);
			nbottompt30_no_HT_h->Fill(nbottompt30,evtWeight);
			ntop_no_HT_h->Fill(ntop,evtWeight);
			nw_no_HT_h->Fill(nw,evtWeight);
			njetspt20_no_HT_h->Fill(njetspt20,evtWeight);
			njetspt30_no_HT_h->Fill(njetspt30,evtWeight);
			met_no_HT_h->Fill(met,evtWeight);
			HT_no_HT_h->Fill(HT,evtWeight);
			if(jpt_eta_2p5_3p0.size() == 0) jpt_eta_2p5_3p0_no_HT_h->Fill(0.0,evtWeight);
			else jpt_eta_2p5_3p0_no_HT_h->Fill(jpt_eta_2p5_3p0.at(0),evtWeight);

			if(mtb > 140)
			{
				mt2_140_h->Fill(mt2,evtWeight);
				nbottompt20_140_h->Fill(nbottompt20,evtWeight);
				nbottompt30_140_h->Fill(nbottompt30,evtWeight);
				ntop_140_h->Fill(ntop,evtWeight);
				nw_140_h->Fill(nw,evtWeight);
				njetspt20_140_h->Fill(njetspt20,evtWeight);
				njetspt30_140_h->Fill(njetspt30,evtWeight);
				met_140_h->Fill(met,evtWeight);
				HT_140_h->Fill(HT,evtWeight);
				if(mt2 > 200)
				{
					nbottompt20_140_MT2_h->Fill(nbottompt20,evtWeight);
					nbottompt30_140_MT2_h->Fill(nbottompt30,evtWeight);
					ntop_140_MT2_h->Fill(ntop,evtWeight);
					nw_140_MT2_h->Fill(nw,evtWeight);
					njetspt20_140_MT2_h->Fill(njetspt20,evtWeight);
					njetspt30_140_MT2_h->Fill(njetspt30,evtWeight);
					met_140_MT2_h->Fill(met,evtWeight);
					HT_140_MT2_h->Fill(HT,evtWeight);
				}
			}

			if(mtb > 175)
			{
				mt2_175_h->Fill(mt2,evtWeight);
				nbottompt20_175_h->Fill(nbottompt20,evtWeight);
				nbottompt30_175_h->Fill(nbottompt30,evtWeight);
				ntop_175_h->Fill(ntop,evtWeight);
				ntop_merge_175_h->Fill(ntop_merge,evtWeight);
				ntop_w_175_h->Fill(ntop_w,evtWeight);
				ntop_res_175_h->Fill(ntop_res,evtWeight);
				nw_175_h->Fill(nw,evtWeight);
				njetspt20_175_h->Fill(njetspt20,evtWeight);
				njetspt30_175_h->Fill(njetspt30,evtWeight);
				met_175_h->Fill(met,evtWeight);
				HT_175_h->Fill(HT,evtWeight);

				ntop_nw_h->Fill(ntop,nw,evtWeight);

				std::vector<TLorentzVector> plus_temp;
				std::vector<TLorentzVector> minus_temp;
				std::vector<TLorentzVector> plus_w_jets;
				std::vector<TLorentzVector> minus_w_jets;
				std::vector<TLorentzVector> plus_b_jets;
				std::vector<TLorentzVector> minus_b_jets;
				TLorentzVector plus_sum_temp;
				TLorentzVector minus_sum_temp;
				int n_plus_gen_match = 0, n_minus_gen_match = 0;
				int n_plus_b_gen_match = 0, n_minus_b_gen_match = 0;
				int n_plus_w_gen_match = 0, n_minus_w_gen_match = 0;
				if(debug) std::cout << __LINE__ << std::endl;

				if(ntop > 0)
				{	
					for(int i=0; i < PdgId.size(); i++)
					{
						if (PdgId.at(i) == 24)
						{
							n_gen_w++;
							plus_temp.push_back(gen_LVec.at(i));
							gen_w_pt_175_h->Fill(gen_LVec.at(i).Pt(),evtWeight);
						}

						if (PdgId.at(i) == 5 && MomRefId.at(i) != -1 && abs(PdgId.at(MomRefId.at(i))) != 5)
						{
							n_gen_b++;
							plus_temp.push_back(gen_LVec.at(i));
							plus_b_jets.push_back(gen_LVec.at(i));
							gen_b_pt_175_h->Fill(gen_LVec.at(i).Pt(),evtWeight);
						}

						if (PdgId.at(i) == -24)
						{
							n_gen_w++;
							minus_temp.push_back(gen_LVec.at(i));
							gen_w_pt_175_h->Fill(gen_LVec.at(i).Pt(),evtWeight);
						}

						if (PdgId.at(i) == -5 && MomRefId.at(i) != -1 && abs(PdgId.at(MomRefId.at(i))) != 5)
						{
							n_gen_b++;
							minus_temp.push_back(gen_LVec.at(i));
							minus_b_jets.push_back(gen_LVec.at(i));
							gen_b_pt_175_h->Fill(gen_LVec.at(i).Pt(),evtWeight);
						}

						if (abs(PdgId.at(i)) < 5 && MomRefId.at(i) != -1 && PdgId.at(MomRefId.at(i)) == 24)
							plus_w_jets.push_back(gen_LVec.at(i));
						if (abs(PdgId.at(i)) < 5 && MomRefId.at(i) != -1 && PdgId.at(MomRefId.at(i)) == -24)
							minus_w_jets.push_back(gen_LVec.at(i));
					}

					if(ntop == 1 && mTopJets.at(0).size() == 3)
					{
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < plus_b_jets.size(); j++)
							{
								if(mTopJets.at(0).at(i).DeltaR(plus_b_jets.at(j)) < 0.4)
								{n_plus_gen_match++;
									n_plus_b_gen_match++;}
							}

							for (int j = 0; j < plus_w_jets.size(); j++)
							{
								if(mTopJets.at(0).at(i).DeltaR(plus_w_jets.at(j)) < 0.4)
								{n_plus_gen_match++;
									n_plus_w_gen_match++;}
							}

							for (int j = 0; j < minus_b_jets.size(); j++)
							{
								if(mTopJets.at(0).at(i).DeltaR(minus_b_jets.at(j)) < 0.4)
								{n_minus_gen_match++;
									n_minus_b_gen_match++;}
							}

							for (int j = 0; j < minus_w_jets.size(); j++)
							{
								if(mTopJets.at(0).at(i).DeltaR(minus_w_jets.at(j)) < 0.4)
								{n_minus_gen_match++;
									n_minus_w_gen_match++;}
							}
						}
						n_res_gen_match_h->Fill(std::max(n_plus_gen_match, n_minus_gen_match), evtWeight);
						if(std::max(n_plus_gen_match, n_minus_gen_match) == 2)
						{
							//std::cout << "plus_b_jets.size() = " << plus_b_jets.size() << " n_plus_b_gen_match = " << n_plus_b_gen_match << " minus_b_jets.size() = " << minus_b_jets.size() <<" n_minus_b_gen_match = " << n_minus_b_gen_match << std::endl;
							//std::cout << "plus_w_jets.size() = " << plus_w_jets.size() << " n_plus_w_gen_match = " << n_plus_w_gen_match << " minus_w_jets.size() = " << minus_w_jets.size() << " n_minus_w_gen_match = " << n_minus_w_gen_match << std::endl;
							n_res_b_gen_match_h->Fill(std::max(n_plus_b_gen_match, n_minus_b_gen_match), evtWeight);
							n_res_w_gen_match_h->Fill(std::max(n_plus_w_gen_match, n_minus_w_gen_match), evtWeight);
						}
					}

					if(plus_temp.size() >= 2)
					{
						plus_sum_temp = plus_temp.at(0) + plus_temp.at(1);
						gen_w_b_mass_175_h->Fill(plus_sum_temp.M(),evtWeight);
						gen_w_b_delta_r_175_h->Fill(plus_temp.at(0).DeltaR(plus_temp.at(1)),evtWeight);
					}

					if (minus_temp.size() >= 2)
					{
						minus_sum_temp = minus_temp.at(0) + minus_temp.at(1);
						gen_w_b_mass_175_h->Fill(minus_sum_temp.M(),evtWeight);
						gen_w_b_delta_r_175_h->Fill(minus_temp.at(0).DeltaR(minus_temp.at(1)),evtWeight);
					}

					if (t2bw_study && n_gen_w != 2) std::cout << "number of gen w is " << n_gen_w << std::endl;

					if (t2bw_study && plus_temp.size() != 2) 
					{
						std::cout << "number of gen w+ and b is " << plus_temp.size() << std::endl;
						for(int i=0; i < PdgId.size(); i++)
						{
							std::cout << PdgId.at(i) << " ";
							if(MomRefId.at(i) != -1)
								std::cout << "(index = " << i << ", pt = " << gen_LVec.at(i).Pt() << ", mom id = " << PdgId.at(MomRefId.at(i)) << ", mom index = " << MomRefId.at(i) << ") ";
						}
						std::cout << "\n" << std::endl;
					}

					if (t2bw_study && minus_temp.size() != 2) 
					{
						std::cout << "number of gen w- and b_bar is " << minus_temp.size() << std::endl;
						for(int i=0; i < PdgId.size(); i++)
						{
							std::cout << PdgId.at(i) << " ";
							if(MomRefId.at(i) != -1)
								std::cout << "(index = " << i << ", pt = " << gen_LVec.at(i).Pt() << ", mom id = " << PdgId.at(MomRefId.at(i)) << ", mom index = " << MomRefId.at(i) << ") ";
						}
						std::cout << "\n" << std::endl;
					}
				}

				if(mt2 > 200)
				{
					nbottompt20_175_MT2_h->Fill(nbottompt20,evtWeight);
					nbottompt30_175_MT2_h->Fill(nbottompt30,evtWeight);
					ntop_175_MT2_h->Fill(ntop,evtWeight);
					nw_175_MT2_h->Fill(nw,evtWeight);
					njetspt20_175_MT2_h->Fill(njetspt20,evtWeight);
					njetspt30_175_MT2_h->Fill(njetspt30,evtWeight);
					met_175_MT2_h->Fill(met,evtWeight);
					HT_175_MT2_h->Fill(HT,evtWeight);
				}
			}
		}

	} // End Loop Over Events

	// ---------- Write Histograms to File ----------
	if(debug) std::cout << __LINE__ << std::endl;

	TFile out_file(outputfile,"RECREATE"); // Creates TFile with name passed to outputfile from input argument

	// MET
	met_uc_h->Write();
	met_h->Write();
	met_no_HT_h->Write();
	met_140_h->Write();
	met_140_MT2_h->Write();
	met_175_h->Write();
	met_175_MT2_h->Write();
	met_lowdm_h->Write();
	met_highdm_h->Write();
	genmet_h->Write();

	met_1b_100_h->Write();
	met_1b_010_h->Write();
	met_1b_001_h->Write();
	met_1b_110_h->Write();
	met_1b_011_h->Write();
	met_1b_101_h->Write();

	met_2b_100_h->Write();
	met_2b_010_h->Write();
	met_2b_001_h->Write();
	met_2b_110_h->Write();
	met_2b_011_h->Write();
	met_2b_101_h->Write();
	met_2b_200_h->Write();
	met_2b_020_h->Write();
	met_2b_002_h->Write();
	met_2b_3_h->Write();

	met_3b_100_h->Write();
	met_3b_010_h->Write();
	met_3b_001_h->Write();
	met_3b_110_h->Write();
	met_3b_011_h->Write();
	met_3b_101_h->Write();
	met_3b_200_h->Write();
	met_3b_020_h->Write();
	met_3b_002_h->Write();
	met_3b_3_h->Write();


	// MT2
	mt2_h->Write();
	mt2_no_HT_h->Write();
	mt2_140_h->Write();
	mt2_175_h->Write();
	mt2_140_high_h->Write();
	mt2_140_low_h->Write();
	mt2_175_high_h->Write();
	mt2_175_low_h->Write();
	mt2_140_175_h->Write();
	mt2_baseline_no_mt2_h->Write();
	mt2_b_h->Write();

	// MTb
	mtb_uc_h->Write();
	mtb_h->Write();
	mtb_no_HT_h->Write();
	mtb_highdm_h->Write();

	// Jets
	jpt_eta_2p5_3p0_h->Write();
	jpt_eta_2p5_3p0_no_HT_h->Write();
	njets_uc_h->Write();
	njets_h->Write();
	njetspt20_lept_veto_h->Write();
	njetspt20_uc_h->Write();
	njetspt20_h->Write();
	njetspt20_no_HT_h->Write();
	njetspt20_140_h->Write();
	njetspt20_140_MT2_h->Write();
	njetspt20_175_h->Write();
	njetspt20_175_MT2_h->Write();
	njetspt20_lowdm_h->Write();
	njetspt30_h->Write();
	njetspt30_no_HT_h->Write();
	njetspt30_140_h->Write();
	njetspt30_140_MT2_h->Write();
	njetspt30_175_h->Write();
	njetspt30_175_MT2_h->Write();

	// b jets
	nbottompt20_uc_h->Write();
	nbottompt20_h->Write();
	nbottompt20_no_HT_h->Write();
	nbottompt20_140_h->Write();
	nbottompt20_140_MT2_h->Write();
	nbottompt20_175_h->Write();
	nbottompt20_175_MT2_h->Write();
	nbottompt20_lowdm_h->Write();
	nbottompt30_no_HT_h->Write();
	nbottompt30_140_h->Write();
	nbottompt30_140_MT2_h->Write();
	nbottompt30_175_h->Write();
	nbottompt30_175_MT2_h->Write();
	nSV_lowdm_h->Write();

	// top
	ntop_h->Write();
	ntop_merge_uc_h->Write();
	ntop_merge_h->Write();
	ntop_w_h->Write();
	ntop_res_uc_h->Write();
	ntop_res_h->Write();
	ntop_no_HT_h->Write();
	ntop_140_h->Write();
	ntop_140_MT2_h->Write();
	ntop_175_h->Write();
	ntop_175_MT2_h->Write();
	ntop_merge_175_h->Write();
	ntop_w_175_h->Write();
	ntop_res_175_h->Write();

	gen_w_pt_175_h->Write();
	gen_b_pt_175_h->Write();
	gen_w_b_mass_175_h->Write();
	gen_w_b_delta_r_175_h->Write();
	n_res_gen_match_h->Write();
	n_res_b_gen_match_h->Write();
	n_res_w_gen_match_h->Write();

	// W
	nw_uc_h->Write();
	nw_h->Write();
	nw_no_HT_h->Write();
	nw_140_h->Write();
	nw_140_MT2_h->Write();
	nw_175_h->Write();
	nw_175_MT2_h->Write();

	// HT
	HT_uc_h->Write();
	HT_h->Write();
	HT_no_HT_h->Write();
	HT_140_h->Write();
	HT_140_MT2_h->Write();
	HT_175_h->Write();
	HT_175_MT2_h->Write();
	genHT_h->Write();

	// Other
	loose_baseline_cutflow_h->Write();
	ISRpt_lowdm_h->Write();
	bottompt_scalar_sum_lowdm_h->Write();
	nMuons_uc_h->Write();
	nElectrons_uc_h->Write();

	ISR_SF_uc_h->Write();
	B_SF_uc_h->Write();
	ISR_SF_h->Write();
	B_SF_h->Write();

	// search bin
	search_bin_h->Write();
	search_bin_singleMuCR_h->Write();
	search_bin_singleElCR_h->Write();
	search_bin_8026_h->Write();

	search_bin_MTb_h->Write();
	search_bin_singleMuCR_MTb_h->Write();
	search_bin_singleElCR_MTb_h->Write();

	search_bin_low_and_highdm_h->Write();
	search_bin_low_and_highdm_singleMuCR_h->Write();
	search_bin_low_and_highdm_singleElCR_h->Write();

	search_bin_v2_h->Write();
	search_bin_v2_singleMuCR_h->Write();
	search_bin_v2_singleElCR_h->Write();

	search_bin_more_HT_bins_h->Write();
	search_bin_more_HT_bins_singleMuCR_h->Write();
	search_bin_more_HT_bins_singleElCR_h->Write();

	search_bin_more_HT_bins_merge_h->Write();
	search_bin_more_HT_bins_singleMuCR_merge_h->Write();
	search_bin_more_HT_bins_singleElCR_merge_h->Write();

	search_bin_highdm_h->Write();
	search_bin_highdm_singleMuCR_h->Write();
	search_bin_highdm_singleElCR_h->Write();

	search_bin_highdm_MT2_h->Write();
	search_bin_highdm_singleMuCR_MT2_h->Write();
	search_bin_highdm_singleElCR_MT2_h->Write();

	search_bin_highdm_merge_HT_h->Write();
	search_bin_highdm_singleMuCR_merge_HT_h->Write();
	search_bin_highdm_singleElCR_merge_HT_h->Write();

	search_bin_team_A_h->Write();
	search_bin_team_A_singleMuCR_h->Write();
	search_bin_team_A_singleElCR_h->Write();

	search_bin_team_A_highdm_merge_h->Write();
	search_bin_team_A_highdm_singleMuCR_merge_h->Write();
	search_bin_team_A_highdm_singleElCR_merge_h->Write();

	search_bin_team_A_highdm_MTb175_MT2_h->Write();
	search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h->Write();
	search_bin_team_A_highdm_MTb175_MT2_singleElCR_h->Write();

	search_bin_team_A_highdm_MTb140_MT2_h->Write();
	search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h->Write();
	search_bin_team_A_highdm_MTb140_MT2_singleElCR_h->Write();

	// 2-D histograms
	mtb_mt2_uc_h->Write();
	mtb_mt2_h->Write();
	ntop_nw_h->Write();
	ISRpt_MET_lowdm_h->Write();

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	eff_h->Write();

	std::cout << "Pre-Cut MTb-MT2 Covariance is " << mtb_mt2_uc_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_uc_h->GetCorrelationFactor() << std::endl;

	std::cout << "Post-Cut MTb-MT2 Covariance is " << mtb_mt2_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_h->GetCorrelationFactor() << std::endl;
	std::cout << "n_old_bin_is_83 = " << n_old_bin_is_83 << ", n_highdm_is_176 = " << n_highdm_is_176 << ". n_fail_mtb = " << n_fail_mtb << ", n_fail_met = " << n_fail_met << ", n_fail_dPhi = " << n_fail_dPhi << std::endl;

	out_file.Close(); // Close (must be ".") File @ebinter

	ch->Reset(); // What does this do?

	return 0;

} // End Main
