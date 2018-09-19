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

int main(int argc, char* argv[]){

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

	int max_events = -1;
	//max_events = 10000;

	bool do_ISR_BSF = true;
	bool t2bw_study = false;

	//if(argc > 3){
	//   const char *events_input = argv[3]; // Need to Fix
	//   events = strtod(*events_input, NULL);
	//}

	// std::cout << "Will read first " << events << " events." << std::endl;
	const char *fastsim   = "";

	if (argc > 3)
	{
		fastsim = argv[3];
	}

	TChain *ch = new TChain("stopTreeMaker/AUX"); //TChain for all files
	bigfile.open (inputfilelist); //opening file list ~aarulana
	//std::cout << "inputfilelist " << *inputfilelist << std::endl;

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

	NTupleReader tr(ch);
	tr.setReThrow(false);

	//BaselineVessel blv(tr);
	BaselineVessel blv(tr, "", "fastsim");

	tr.registerFunction(blv);
	SearchBins SB("SB_v1_2017");

	bool find_B_SF = true;
	bool find_ISR_SF = true;
	bool find_fastsim = false;

	if(sample_name.find("Signal_fastsim") != std::string::npos)
	{std::cout << "find fastsim signal" << std::endl; find_fastsim = true;}

	std::cout << "======================================= ISR and B_SF start ==============================================" << std::endl;
	std::cout << "sample name is " << sample_name << std::endl; 

	BTagCorrector btagcorr = BTagCorrector("allINone_bTagEff.root");
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

	// Missing Transverse Energy (met)

	auto met_h=new TH1F("met_h","Missing E_T w/ Baseline Cut",80,0.0,1600.0);
	auto met_uc_h=new TH1F("met_uc_h","Missing E_T (No Cuts)",80,0.0,1600.0);

	// Transverse Mass (mt2)

	auto mt2_h=new TH1F("mt2_h","MT2",80,0.0,800.0);
	auto mt2_baseline_no_mt2_h=new TH1F("mt2_baseline_no_mt2_h","MT2 baseline no MT2 cut",80,0.0,800.0);

	auto mt2_140_high_h=new TH1F("mt2_140_high_h","MT2, team a bsae line when MTb > 140",80,0.0,800.0);
	auto mt2_140_low_h=new TH1F("mt2_140_low_h","MT2, team a bsae line when MTb < 140",80,0.0,800.0);
	auto mt2_175_high_h=new TH1F("mt2_175_high_h","MT2, team a bsae line when MTb > 175",80,0.0,800.0);
	auto mt2_175_low_h=new TH1F("mt2_175_low_h","MT2, team a bsae line when MTb < 175",80,0.0,800.0);
	auto mt2_140_175_h=new TH1F("mt2_140_175_h","MT2, team a bsae line when 140 < MTb < 175",80,0.0,800.0);
	auto mt2_b_h=new TH1F("mt2_b_h","MT2_b",80,0.0,800.0);

	// Transverse Mass of b-jet (mtb)

	auto mtb_h=new TH1F("mtb_h","MTb",80,0.0,800.0);
	auto mtb_highdm_h=new TH1F("mtb_highdm_h","MTb high dm",80,0.0,800.0);
	auto mtb_no_HT_h=new TH1F("mtb_no_HT_h","MTb in loose baseline without HT cut",80,0.0,800.0);
	auto mtb_uc_h=new TH1F("mtb_uc_h","MTb (No Cuts)",80,0.0,800.0);

	// Other

	auto eff_h=new TH1F("eff_h","loose baseline Efficiency",2,0.0,2.0);

	auto search_bin_h=new TH1F("search_bin_h","search bin with baseline cut",84,0.0,84.0);
	auto search_bin_8026_h=new TH1F("search_bin_8026_h","search bin with 8026 baseline cut",84,0.0,84.0);
	auto search_bin_singleMuCR_h=new TH1F("search_bin_singleMuCR_h","search bin with baseline cut, single muon control region",84,0.0,84.0);
	auto search_bin_singleElCR_h=new TH1F("search_bin_singleElCR_h","search bin with baseline cut, single electron control region",84,0.0,84.0);
	auto search_bin_MTb_h=new TH1F("search_bin_MTb_h","search bin with baseline cut + MTb > 175",84,0.0,84.0);
	auto search_bin_singleMuCR_MTb_h=new TH1F("search_bin_singleMuCR_MTb_h","search bin with baseline cut + MTb > 175, single muon control region",84,0.0,84.0);
	auto search_bin_singleElCR_MTb_h=new TH1F("search_bin_singleElCR_MTb_h","search bin with baseline cut + MTb > 175, single electron control region",84,0.0,84.0);
	auto search_bin_low_and_highdm_h=new TH1F("search_bin_low_and_highdm_h","search bin low and high dM, MTb = 175",177,0.0,177.0);
	auto search_bin_highdm_h=new TH1F("search_bin_highdm_h","search bin high dM, MTb = 175",124,53.0,177.0);
	auto search_bin_highdm_MT2_h=new TH1F("search_bin_highdm_MT2_h","search bin high dM, MTb = 175 with MT2 bins",168,53.0,221.0);
	auto search_bin_highdm_merge_HT_h=new TH1F("search_bin_highdm_merge_HT_h","search bin high dM, MTb = 175, merge HT",124,53.0,177.0);
	auto search_bin_team_A_highdm_h=new TH1F("search_bin_team_A_highdm_h","search bin team A high dM, MTb = 175",51,53.0,104.0);
	auto search_bin_team_A_highdm_merge_h=new TH1F("search_bin_team_A_highdm_merge_h","search bin team A high dM, MTb = 175, merge top",51,53.0,104.0);
	auto search_bin_low_and_highdm_singleMuCR_h=new TH1F("search_bin_low_and_highdm_singleMuCR_h","search bin low and high dM single muon control region, MTb = 175",177,0.0,177.0);
	auto search_bin_highdm_singleMuCR_h=new TH1F("search_bin_highdm_singleMuCR_h","search bin high dM single muon control region, MTb = 175",124,53.0,177.0);
	auto search_bin_highdm_singleMuCR_MT2_h=new TH1F("search_bin_highdm_singleMuCR_MT2_h","search bin high dM single muon control region, MTb = 175 with MT2 bins",168,53.0,221.0);
	auto search_bin_highdm_singleMuCR_merge_HT_h=new TH1F("search_bin_highdm_singleMuCR_merge_HT_h","search bin high dM single muon control region, MTb = 175, merge HT",124,53.0,177.0);
	auto search_bin_team_A_highdm_singleMuCR_h=new TH1F("search_bin_team_A_highdm_singleMuCR_h","search bin team A high dM single muon control region, MTb = 175",51,53.0,104.0);
	auto search_bin_team_A_highdm_singleMuCR_merge_h=new TH1F("search_bin_team_A_highdm_singleMuCR_merge_h","search bin team A high dM single muon control region, MTb = 175, merge top",51,53.0,104.0);
	auto search_bin_low_and_highdm_singleElCR_h=new TH1F("search_bin_low_and_highdm_singleElCR_h","search bin low and high dM single electron control region, MTb = 175",177,0.0,177.0);
	auto search_bin_highdm_singleElCR_h=new TH1F("search_bin_highdm_singleElCR_h","search bin high dM single electron control region, MTb = 175",124,53.0,177.0);
	auto search_bin_highdm_singleElCR_MT2_h=new TH1F("search_bin_highdm_singleElCR_MT2_h","search bin high dM single electron control region, MTb = 175 with MT2 bins",168,53.0,221.0);
	auto search_bin_highdm_singleElCR_merge_HT_h=new TH1F("search_bin_highdm_singleElCR_merge_HT_h","search bin high dM single electron control region, MTb = 175, merge HT",124,53.0,177.0);
	auto search_bin_team_A_highdm_singleElCR_h=new TH1F("search_bin_team_A_highdm_singleElCR_h","search bin team A high dM single electron control region, MTb = 175",51,53.0,104.0);
	auto search_bin_team_A_highdm_singleElCR_merge_h=new TH1F("search_bin_team_A_highdm_singleElCR_merge_h","search bin team A high dM single electron control region, MTb = 175, merge top",51,53.0,104.0);
	auto search_bin_team_A_lowdm_h=new TH1F("search_bin_team_A_lowdm_h","search bin team A low dM",53,0,53);
	auto search_bin_team_A_lowdm_singleMuCR_h=new TH1F("search_bin_team_A_lowdm_singleMuCR_h","search bin team A low dM single muon control region",53,0,53);
	auto search_bin_team_A_lowdm_singleElCR_h=new TH1F("search_bin_team_A_lowdm_singleElCR_h","search bin team A low dM single elctron control region",53,0,53);
	auto search_bin_team_A_highdm_MTb175_MT2_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_h","search bin team A high dM, MTb = 175, MT2 >200",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h","search bin team A high dM, MTb = 175, MT2 >200 single muon control region",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb175_MT2_singleElCR_h=new TH1F("search_bin_team_A_highdm_MTb175_MT2_singleElCR_h","search bin team A high dM, MTb = 175, MT2 >200 single electron control region",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb140_MT2_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_h","search bin team A high dM, MTb = 140, MT2 >200",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h","search bin team A high dM, MTb = 140, MT2 >200 single muon control region",51,53.0,104.0);
	auto search_bin_team_A_highdm_MTb140_MT2_singleElCR_h=new TH1F("search_bin_team_A_highdm_MTb140_MT2_singleElCR_h","search bin team A high dM, MTb = 140, MT2 >200 single electron control region",51,53.0,104.0);

	auto jpt_eta_2p5_3p0_h=new TH1F("jpt_eta_2p5_3p0_h","all jets Pt (Baseline Cuts), 2.5 < eta < 3.0",80,0.0,400.0);
	auto jpt_eta_2p5_3p0_no_HT_h=new TH1F("jpt_eta_2p5_3p0_no_HT_h","all jets Pt (Baseline Cuts no HT), 2.5 < eta < 3.0",80,0.0,400.0);
	auto nbottom_h=new TH1F("nbottom_h","b Jet Count (Baseline Cuts)",8,0.0,8.0);
	auto ntop_h=new TH1F("ntop_h","Number of Tops (Baseline Cuts)",8,0.0,8.0);
	auto njets_h=new TH1F("njets_h","Number of Jets (Baseline Cuts)",30,0.0,30.0);
	auto njetspt20_h=new TH1F("njetspt20_h","Number of Jets above PT=20 (Baseline Cuts)",15,0.0,15.0);
	auto njetspt30_h=new TH1F("njetspt30_h","Number of Jets above PT=30 (Baseline Cuts)",15,0.0,15.0);
	auto HT_h=new TH1F("HT_h","HT (Baseline Cuts)",100,0.0,3000.0);
	auto genHT_h=new TH1F("genHT_h","Gen Level HT (Baseline Cuts)",100,0.0,3000.0);
	auto genmet_h=new TH1F("genmet_h","Gen Level MET (Baseline Cuts)",80,0.0,1600.0);

	auto nbottompt20_no_mtb_h=new TH1F("nbottompt20_no_mtb_h","number of bottom without MTb cut",8,0.0,8.0);
	auto nbottompt20_140_h=new TH1F("nbottompt20_140_h","number of bottom when MTb > 140",8,0.0,8.0);
	auto nbottompt20_140_MT2_h=new TH1F("nbottompt20_140_MT2_h","number of bottom when MTb > 140 with MT2 cut",8,0.0,8.0);
	auto nbottompt20_175_h=new TH1F("nbottompt20_175_h","number of bottom when MTb > 175",8,0.0,8.0);
	auto nbottompt20_175_MT2_h=new TH1F("nbottompt20_175_MT2_h","number of bottom when MTb > 175 with MT2 cut",8,0.0,8.0);
	auto nbottompt20_lowdm_h=new TH1F("nbottompt20_lowdm_h","number of bottom low dm",8,0.0,8.0);
	auto nSV_lowdm_h=new TH1F("nSV_lowdm_h","number of SV low dm",8,0.0,8.0);

	auto nbottompt30_no_mtb_h=new TH1F("nbottompt30_no_mtb_h","number of bottom without MTb cut",8,0.0,8.0);
	auto nbottompt30_140_h=new TH1F("nbottompt30_140_h","number of bottom when MTb > 140",8,0.0,8.0);
	auto nbottompt30_140_MT2_h=new TH1F("nbottompt30_140_MT2_h","number of bottom when MTb > 140 with MT2 cut",8,0.0,8.0);
	auto nbottompt30_175_h=new TH1F("nbottompt30_175_h","number of bottom when MTb > 175",8,0.0,8.0);
	auto nbottompt30_175_MT2_h=new TH1F("nbottompt30_175_MT2_h","number of bottom when MTb > 175 with MT2 cut",8,0.0,8.0);

	auto ntop_no_mtb_h=new TH1F("ntop_no_mtb_h","number of top without MTb cut",8,0.0,8.0);
	auto ntop_140_h=new TH1F("ntop_140_h","number of top when MTb > 140",8,0.0,8.0);
	auto ntop_140_MT2_h=new TH1F("ntop_140_MT2_h","number of top when MTb > 140 with MT2 cut",8,0.0,8.0);
	auto ntop_175_h=new TH1F("ntop_175_h","number of top when MTb > 175",8,0.0,8.0);
	auto ntop_merge_175_h=new TH1F("ntop_merge_175_h","number of full merged top when MTb > 175",8,0.0,8.0);
	auto ntop_w_175_h=new TH1F("ntop_w_175_h","number of part merged top when MTb > 175",8,0.0,8.0);
	auto ntop_res_175_h=new TH1F("ntop_res_175_h","number of resolved top when MTb > 175",8,0.0,8.0);
	auto ntop_175_MT2_h=new TH1F("ntop_175_MT2_h","number of top when MTb > 175 with MT2 cut",8,0.0,8.0);

	auto gen_w_pt_175_h=new TH1F("gen_w_pt_175_h","gen w pt when MTb > 175",80,0.0,400.0);
	auto gen_b_pt_175_h=new TH1F("gen_b_pt_175_h","gen b pt when MTb > 175",80,0.0,400.0);
	auto gen_w_b_mass_175_h=new TH1F("gen_w_b_mass_175_h","gen w and b mass when MTb > 175",80,0.0,400.0);
	auto gen_w_b_delta_r_175_h=new TH1F("gen_w_b_delta_r_175_h","gen w and b delta R when MTb > 175",80,0.0,8.0);
	auto n_res_gen_match_h=new TH1F("n_res_gen_match_h","number of gen matched resolved jets when ntop = 1 and MTb > 175",8,0.0,8.0);
	auto n_res_b_gen_match_h=new TH1F("n_res_b_gen_match_h","number of gen matched b in resolved jets when ntop = 1 and MTb > 175",8,0.0,8.0);
	auto n_res_w_gen_match_h=new TH1F("n_res_w_gen_match_h","number of gen matched w in resolved jets when ntop = 1 and MTb > 175",8,0.0,8.0);

	auto nw_no_mtb_h=new TH1F("nw_no_mtb_h","number of w without MTb cut",8,0.0,8.0);
	auto nw_140_h=new TH1F("nw_140_h","number of w when MTb > 140",8,0.0,8.0);
	auto nw_140_MT2_h=new TH1F("nw_140_MT2_h","number of w when MTb > 140 with MT2 cut",8,0.0,8.0);
	auto nw_175_h=new TH1F("nw_175_h","number of w when MTb > 175",8,0.0,8.0);
	auto nw_175_MT2_h=new TH1F("nw_175_MT2_h","number of w when MTb > 175 with MT2 cut",8,0.0,8.0);

	auto njetspt20_no_mtb_h=new TH1F("njetspt20_no_mtb_h","number of jets (pt > 20) without MTb cut",30,0.0,30.0);
	auto njetspt20_140_h=new TH1F("njetspt20_140_h","number of jets (pt > 20) when MTb > 140",30,0.0,30.0);
	auto njetspt20_140_MT2_h=new TH1F("njetspt20_140_MT2_h","number of jets (pt > 20) when MTb > 140 with MT2 cut",30,0.0,30.0);
	auto njetspt20_175_h=new TH1F("njetspt20_175_h","number of jets (pt > 20) when MTb > 175",30,0.0,30.0);
	auto njetspt20_175_MT2_h=new TH1F("njetspt20_175_MT2_h","number of jets (pt > 20) when MTb > 175 with MT2 cut",30,0.0,30.0);
	auto njetspt20_lowdm_h=new TH1F("njetspt20_lowdm_h","number of jets (pt > 20) low dm",30,0.0,30.0);

	auto njetspt30_no_mtb_h=new TH1F("njetspt30_no_mtb_h","number of jets (pt > 30) without MTb cut",30,0.0,30.0);
	auto njetspt30_140_h=new TH1F("njetspt30_140_h","number of jets (pt > 30) when MTb > 140",30,0.0,30.0);
	auto njetspt30_140_MT2_h=new TH1F("njetspt30_140_MT2_h","number of jets (pt > 30) when MTb > 140 with MT2 cut",30,0.0,30.0);
	auto njetspt30_175_h=new TH1F("njetspt30_175_h","number of jets (pt > 30) when MTb > 175",30,0.0,30.0);
	auto njetspt30_175_MT2_h=new TH1F("njetspt30_175_MT2_h","number of jets (pt > 30) when MTb > 175 with MT2 cut",30,0.0,30.0);

	auto met_no_mtb_h=new TH1F("met_no_mtb_h","met without MTb cut",80,0.0,1600.0);
	auto met_140_h=new TH1F("met_140_h","met when MTb > 140",80,0.0,1600.0);
	auto met_140_MT2_h=new TH1F("met_140_MT2_h","met when MTb > 140 with MT2 cut",80,0.0,1600.0);
	auto met_175_h=new TH1F("met_175_h","met when MTb > 175",80,0.0,1600.0);
	auto met_175_MT2_h=new TH1F("met_175_MT2_h","met when MTb > 175 with MT2 cut",80,0.0,1600.0);
	auto met_lowdm_h=new TH1F("met_lowdm_h","met low dm",80,0.0,1600.0);
	auto ISRpt_lowdm_h=new TH1F("ISRpt_lowdm_h","ISR pt low dm",80,0.0,1600.0);
	auto bottompt_scalar_sum_lowdm_h=new TH1F("bottompt_scalar_sum_lowdm_h","bottom pt sclar sum low dm",80,0.0,1600.0);

	auto HT_no_mtb_h=new TH1F("HT_no_mtb_h","HT without MTb cut",160,0.0,3200.0);
	auto HT_140_h=new TH1F("HT_140_h","HT when MTb > 140",160,0.0,3200.0);
	auto HT_140_MT2_h=new TH1F("HT_140_MT2_h","HT when MTb > 140 with MT2 cut",160,0.0,3200.0);
	auto HT_175_h=new TH1F("HT_175_h","HT when MTb > 175",160,0.0,3200.0);
	auto HT_175_MT2_h=new TH1F("HT_175_MT2_h","HT when MTb > 175 with MT2 cut",160,0.0,3200.0);

	auto mt2_no_mtb_h=new TH1F("mt2_no_mtb_h","mt2 without MTb cut",80,0.0,800.0);
	auto mt2_140_h=new TH1F("mt2_140_h","mt2 when MTb > 140",80,0.0,800.0);
	//auto mt2_140_MT2_h=new TH1F("mt2_140_MT2_h","mt2 when MTb > 140 with MT2 cut",80,0.0,800.0);
	auto mt2_175_h=new TH1F("mt2_175_h","mt2 when MTb > 175",80,0.0,800.0);
	//auto mt2_175_MT2_h=new TH1F("mt2_175_MT2_h","mt2 when MTb > 175 with MT2 cut",80,0.0,800.0);

	// 2-D Histograms

	TH2F *mtb_mt2_uc_h = new TH2F("mtb_mt2_uc_h","MTb and MT2 (No Cuts)",70,0.0,350.0,70,0.0,350.0);
	TH2F *mtb_mt2_h = new TH2F("mtb_mt2_h","MTb and MT2 (Baseline Cuts)",70,0.0,350.0,70,0.0,350.0);
	TH2F *ntop_nw_h = new TH2F("ntop_nw_h","Ntop and Nw correlation",8,0.0,8.0,8,0.0,8.0);
	TH2F *ISRpt_MET_lowdm_h = new TH2F("ISRpt_MET_lowdm_h","ISRpt vs MET in low dm",80,0.0,1600.0,80,0.0,1600.0);

	int n_fail_mtb = 0, n_fail_met = 0, n_fail_dPhi = 0;
	int n_old_bin_is_83 = 0, n_highdm_is_176 = 0;

	// ---------- Begin Loop Over Events ----------

	while(tr.getNextEvent()){

		if(tr.getEvtNum() == 1){
			tr.printTupleMembers();
			FILE * fout = fopen("NTupleTypes.txt", "w");
			tr.printTupleMembers(fout);
			fclose(fout);
		}
		if(max_events > 0 && tr.getEvtNum() > max_events) break;

		if(tr.getEvtNum() % 1000 == 0){
			std::cout << "Event Number " << tr.getEvtNum() << std::endl;
		}

		//  std::cout << "MET " << tr.getVar<double>("met")  << " nTop" << tr.getVar<int>("nTopCandSortedCnt") << " Phi " << tr.getVar<double>("metphi") << std::endl;
		std::vector<TLorentzVector> jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");
		std::vector<TLorentzVector> jetsLVec_pt20;
		std::vector<TLorentzVector> wLVec = tr.getVec<TLorentzVector>("vWAlone");
		std::vector<TLorentzVector> softbLVec = tr.getVec<TLorentzVector>("softbLVec");
		std::vector<TLorentzVector> ISRLVec = tr.getVec<TLorentzVector>("vISRJet");
		double ISRpt =0;
		if(ISRLVec.size() == 1) ISRpt = ISRLVec.at(0).Pt();

		int nw = wLVec.size();
		int nSV = softbLVec.size();
		//std::cout << "number of w = " << wLVec.size() << std::endl;

		std::vector<TLorentzVector> b_jetsLVec;
		std::vector<double> jpt_eta_2p5_3p0;
		std::vector<double> rjbt_vec = tr.getVec<double>("recoJetsBtag_0");
		int njets = jetsLVec.size();
		double ISR_SF = 1.0;
		if (find_ISR_SF) ISR_SF = tr.getVar<double>("isr_Unc_Cent");
		double B_SF = 1.0;
		if(find_B_SF) B_SF = tr.getVar<double>("bTagSF_EventWeightSimple_Central");
		double evtWeight = tr.getVar<double>("evtWeight");
		if(do_ISR_BSF) evtWeight = evtWeight*ISR_SF*B_SF;
		double met = tr.getVar<double>("met"); 
		double genmet = tr.getVar<double>("genmet");
		if(find_fastsim) met = (met + genmet) / 2; 
		//double mt2 = tr.getVar<double>("mt2");
		double mt2 = tr.getVar<double>("best_had_brJet_MT2");
		bool passMT2 = (mt2 > 200);
		double metphi = tr.getVar<double>("metphi");
		int nElectrons = tr.getVar<int>("nElectrons_CUT");
		int nMuons = tr.getVar<int>("nMuons_CUT");
		int ntop = tr.getVar<int>("nTopCandSortedCnt");
		int nbjets = tr.getVar<int>("cntCSVS");
		double mtb=-10 , mt2_b=-10;
		int nbottompt20=0 , nbottompt30=0 , njetspt20=0 , njetspt30=0 , njetspt50=0;
		int ntop_merge=0 , ntop_w=0 , ntop_res=0;
		double HT = tr.getVar<double>("HT");
		double S_met = met / sqrt(HT);
		double genHT = tr.getVar<double>("genHT");
		double bad_b_csv = 0;
		TLorentzVector bad_b_tlv;
		double b_CSV_1 = 0, b_CSV_2 = 0;
		int b_CSV_1_index = 0, b_CSV_2_index = 0;

		int SB_index = SB.find_Binning_Index(nbjets, ntop, mt2, met, HT);
		std::map<int, std::vector<TLorentzVector>> mTopJets = tr.getMap <int, std::vector<TLorentzVector>> ("mTopJets");

		bool old_bin_is_83 = false;
		bool highdm_is_176 = false;

		const std::vector<int>& PdgId = tr.getVec<int>("genDecayPdgIdVec");
		const std::vector<int>& MomRefId = tr.getVec<int>("genDecayMomRefVec");
		const std::vector<TLorentzVector>& gen_LVec = tr.getVec<TLorentzVector>("genDecayLVec");
		int n_gen_w = 0, n_gen_b = 0;

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

		for(int index=0; index < njets ; index++)
		{
			TLorentzVector tlv = jetsLVec[index];
			double rjbt = rjbt_vec[index];		//rjbt stands for reco jet b tag value
			double tlv_Phi = tlv.Phi();
			double tlv_Pt = tlv.Pt();
			double tlv_Eta = tlv.Eta();
			double dPhi = tlv_Phi - metphi;

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

		if(nbottompt30 != nbjets) std::cout << "nbottom (pt > 30) = " << nbottompt30 << " nbjets = " << nbjets << std::endl;

		double bottompt_scalar_sum = 0;
		if(b_jetsLVec.size() == 1) bottompt_scalar_sum = b_jetsLVec.at(0).Pt();	
		if(b_jetsLVec.size() > 1) bottompt_scalar_sum = b_jetsLVec.at(0).Pt() + b_jetsLVec.at(1).Pt();	

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
			double tlv_Pt_1 = jetsLVec[b_CSV_1_index].Pt();
			double tlv_Phi_1 = jetsLVec[b_CSV_1_index].Phi();
			double MT_1 = sqrt(2*tlv_Pt_1*met*(1-cos(tlv_Phi_1 - metphi)));
			mtb = MT_1;
		}
		if (b_jetsLVec.size() >= 2)
		{
			std::vector<double> cachedMT2vec;
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

			double tlv_Pt_1 = jetsLVec[b_CSV_1_index].Pt();
			double tlv_Phi_1 = jetsLVec[b_CSV_1_index].Phi();
			double MT_1 = sqrt(2*tlv_Pt_1*met*(1-cos(tlv_Phi_1 - metphi)));
			double tlv_Pt_2 = jetsLVec[b_CSV_2_index].Pt();
			double tlv_Phi_2 = jetsLVec[b_CSV_2_index].Phi();
			double MT_2 = sqrt(2*tlv_Pt_2*met*(1-cos(tlv_Phi_2 - metphi)));
			mtb = std::min(MT_1,MT_2);
		}

		//std::cout << "no cut mtb = " << mtb << " mt2_b " << mt2_b << std::endl;

		for (int i = 0; i < ntop; i++)
		{
			//std::cout << "top index " << i << std::endl;

			if (mTopJets.at(i).size() ==1) ntop_merge++;
			if (mTopJets.at(i).size() ==2) ntop_w++;
			if (mTopJets.at(i).size() ==3) ntop_res++;

			//for (int k = 0; k < mTopJets.at(i).size(); k++) std::cout << "top daughter " << k << " mass = " << mTopJets.at(i).at(k).M() << std::endl;
		}

		int SB_highdm(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met, double ht);
		int SB_highdm_MT2(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met, double ht, double mt2);
		int SB_team_A_highdm(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met);
		int SB_highdm_index_175 = SB_highdm(175, mtb, njetspt20, ntop_merge, nw + ntop_w, ntop_res, nbottompt20, met, HT);
		int SB_highdm_index_175_MT2 = SB_highdm_MT2(175, mtb, njetspt20, ntop_merge, nw + ntop_w, ntop_res, nbottompt20, met, HT, mt2);
		int SB_highdm_index_175_merge_HT = SB_highdm(175, mtb, njetspt20, ntop_merge, nw + ntop_w, ntop_res, nbottompt20, met, 500);
		int SB_team_A_highdm_index_175 = SB_team_A_highdm(175, mtb, njetspt20, ntop_merge, nw + ntop_w, ntop_res, nbottompt20, met);
		int SB_team_A_highdm_index_175_merge = SB_team_A_highdm(175, mtb, njetspt20, 0, nw, ntop, nbottompt20, met);
		int SB_team_A_highdm_index_140 = SB_team_A_highdm(140, mtb, njetspt20, ntop_merge, nw + ntop_w, ntop_res, nbottompt20, met);
		int SB_team_A_lowdm(int njets, int nb, int nSV, double ISRpt, double bottompt_scalar_sum, double met);
		int SB_team_A_lowdm_index = SB_team_A_lowdm(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);

		bool passnJets = (njetspt50 >= 2 && njetspt30 >= 4);		//SUS-16-050, 4 jets30 and 2 jets50
		bool passdphi_highdm = (jetsLVec_pt20.size() >= 4 && fabs(jetsLVec_pt20.at(0).Phi() - metphi) > 0.5 && fabs(jetsLVec_pt20.at(1).Phi() - metphi) > 0.5 && fabs(jetsLVec_pt20.at(2).Phi() - metphi) > 0.5 && fabs(jetsLVec_pt20.at(3).Phi() - metphi) > 0.5);		//SUS-16-049, high dm, dphi(met, jet1234) > 0.5
		bool passdphi_lowdm = ( (jetsLVec_pt20.size() == 2 && fabs(jetsLVec_pt20.at(0).Phi() - metphi) > 0.5 && fabs(jetsLVec_pt20.at(1).Phi() - metphi) > 0.15) || (jetsLVec_pt20.size() >2 && fabs(jetsLVec_pt20.at(0).Phi() - metphi) > 0.5 && fabs(jetsLVec_pt20.at(1).Phi() - metphi) > 0.15 && fabs(jetsLVec_pt20.at(2).Phi() - metphi) > 0.15) );		//SUS-16-049, low dm,  dphi(met, j1) > 0.5, dphi(met, j23) > 0.15

		bool pass_loose_baseline_no_HT=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passdPhis") && tr.getVar<bool>("passMET") && njetspt20 >=2);		//baseline line for merge group 
		bool pass_loose_baseline=(pass_loose_baseline_no_HT && tr.getVar<bool>("passHT"));

		//bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passnJets") && tr.getVar<bool>("passdPhis") && tr.getVar<bool>("passBJets") && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && passnJets && tr.getVar<bool>("passdPhis") && nbjets > 0 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline=(pass_baseline_no_MT2 && passMT2);		//baseline for SUS-16-050
		bool pass_baseline_singleLeptCR = (passMT2 && passnJets && tr.getVar<bool>("passdPhis") && nbjets > 0 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") );
		bool pass_high_dM_baseline=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passMET") && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 5 && passdphi_highdm && nbottompt20 >=1 && tr.getVar<bool>("passHT"));		//baseline for SUS-16-049 high dm plus HT cut 
		bool pass_high_dM_baseline_singleLeptCR=(tr.getVar<bool>("passMET") && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 5 && passdphi_highdm && nbottompt20 >=1 && tr.getVar<bool>("passHT"));		//baseline without lept veto for SUS-16-049 high dm plus HT cut
		bool pass_MT2_highdm = (ntop == 0 || (ntop >0 && passMT2));
		bool pass_mtb_lowdm = (nbottompt20 == 0 || (nbottompt20 >0 && mtb <175)); 		//SUS-16-049, low dm, mtb cut
		bool pass_ISR = (ISRpt > 200 && fabs(ISRLVec.at(0).Eta()) < 2.4 && fabs(ISRLVec.at(0).Phi() - metphi) > 2); 		//SUS-16-049, low dm, ISR cut
		bool pass_low_dM_baseline=(tr.getVar<bool>("passLeptVeto") && ntop == 0 && nw == 0 && pass_ISR && S_met > 10 && passdphi_lowdm && pass_mtb_lowdm && tr.getVar<bool>("passMET")  && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 2); 		//baseline for SUS-16-049 low dm
		bool pass_low_dM_baseline_singleLeptCR=(ntop == 0 && nw == 0 && pass_ISR && S_met > 10 && passdphi_lowdm && pass_mtb_lowdm && tr.getVar<bool>("passMET")  && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 2); 		//baseline without lept veto for SUS-16-049 low dm
 
		if(pass_high_dM_baseline)
		{
			mtb_highdm_h->Fill(mtb,evtWeight);

			if(mtb < 140) mt2_140_low_h->Fill(mt2,evtWeight);
			if(mtb > 140) mt2_140_high_h->Fill(mt2,evtWeight);
			if(mtb < 175) mt2_175_low_h->Fill(mt2,evtWeight);
			if(mtb > 175) mt2_175_high_h->Fill(mt2,evtWeight);
			if(mtb > 140 && mtb < 175) mt2_140_175_h->Fill(mt2,evtWeight);

			if (SB_highdm_index_175 != -1)
			{
				if (SB_highdm_index_175 == 176) {highdm_is_176 = true; n_highdm_is_176++;}
				search_bin_low_and_highdm_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_MT2_h->Fill(SB_highdm_index_175_MT2,evtWeight);
				search_bin_highdm_merge_HT_h->Fill(SB_highdm_index_175_merge_HT,evtWeight);
			}

			if (SB_team_A_highdm_index_175 != -1)
			{
				search_bin_team_A_highdm_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				search_bin_team_A_highdm_merge_h->Fill(SB_team_A_highdm_index_175_merge,evtWeight);
				if(pass_MT2_highdm) 
				{
					search_bin_team_A_highdm_MTb175_MT2_h->Fill(SB_team_A_highdm_index_175,evtWeight);
					search_bin_team_A_highdm_MTb140_MT2_h->Fill(SB_team_A_highdm_index_140,evtWeight);
				}
			}
		}

		if(pass_high_dM_baseline_singleLeptCR)
		{
			if(nElectrons == 1 && SB_highdm_index_175 != -1)
			{
				search_bin_low_and_highdm_singleElCR_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_singleElCR_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_singleElCR_MT2_h->Fill(SB_highdm_index_175_MT2,evtWeight);
				search_bin_highdm_singleElCR_merge_HT_h->Fill(SB_highdm_index_175_merge_HT,evtWeight);
			}

			if(nElectrons == 1 && SB_team_A_highdm_index_175 != -1)
			{
				search_bin_team_A_highdm_singleElCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				search_bin_team_A_highdm_singleElCR_merge_h->Fill(SB_team_A_highdm_index_175_merge,evtWeight);
				if(pass_MT2_highdm) 
				{
					search_bin_team_A_highdm_MTb175_MT2_singleElCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
					search_bin_team_A_highdm_MTb140_MT2_singleElCR_h->Fill(SB_team_A_highdm_index_140,evtWeight);
				}
			}
 
			if(nMuons == 1 && SB_highdm_index_175 != -1)
			{
				search_bin_low_and_highdm_singleMuCR_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_singleMuCR_h->Fill(SB_highdm_index_175,evtWeight);
				search_bin_highdm_singleMuCR_MT2_h->Fill(SB_highdm_index_175_MT2,evtWeight);
				search_bin_highdm_singleMuCR_merge_HT_h->Fill(SB_highdm_index_175_merge_HT,evtWeight);
			}

			if(nMuons == 1 && SB_team_A_highdm_index_175 != -1)
			{
				search_bin_team_A_highdm_singleMuCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
				search_bin_team_A_highdm_singleMuCR_merge_h->Fill(SB_team_A_highdm_index_175_merge,evtWeight);
				if(pass_MT2_highdm) 
				{
					search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h->Fill(SB_team_A_highdm_index_175,evtWeight);
					search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h->Fill(SB_team_A_highdm_index_140,evtWeight);
				}
			}
		}

		if(pass_low_dM_baseline)
		{
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
				search_bin_team_A_lowdm_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}
		}

		if(pass_low_dM_baseline_singleLeptCR)
		{
			if(nElectrons == 1 && SB_team_A_lowdm_index != -1) 
			{
				search_bin_low_and_highdm_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_team_A_lowdm_singleElCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}

			if(nMuons == 1 && SB_team_A_lowdm_index != -1)
			{
				search_bin_low_and_highdm_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
				search_bin_team_A_lowdm_singleMuCR_h->Fill(SB_team_A_lowdm_index,evtWeight);
			}
		}


		if(pass_baseline_no_MT2) mt2_baseline_no_mt2_h->Fill(mt2,evtWeight);

		if(pass_baseline)
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets = " << nbjets << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
			search_bin_h->Fill(SB_index,evtWeight);
			if (SB_index == 83) {old_bin_is_83 = true; n_old_bin_is_83++;}
			if (mtb > 175) search_bin_MTb_h->Fill(SB_index,evtWeight);
		}

		if(tr.getVar<bool>("passBaseline"))
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets = " << nbjets << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
			search_bin_8026_h->Fill(SB_index,evtWeight);
		}

		if(pass_baseline_singleLeptCR)
		{
			//if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << ", nbjets = " << nbjets << ", ntop = " << ntop << ", mt2 = " << mt2 << ", met = " << met << ", HT = " << HT << std::endl;
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
			std::cout << "njetspt20 = " << njetspt20 << ", ntop = " << ntop << ", ntop_merge = " << ntop_merge << ", nw = " << nw << ", ntop_w = " << ntop_w << ", ntop_res = " << ntop_res << ", nbottompt20 = " << nbottompt20 << ", mtb = " << mtb << ", met = " << met << ", genmet = " << genmet << ", HT = " << HT << ", passdphi_highdm = " << passdphi_highdm << std::endl;
			if (mtb < 175) n_fail_mtb++;
			if (met < 400) n_fail_met++;
			if (passdphi_highdm == 0) n_fail_dPhi++;
		}

		met_uc_h->Fill(met,evtWeight);
		if (ntop == 0 && mt2 != 0) std::cout << "MT2 = " << mt2 << std::endl;
		mtb_mt2_uc_h->Fill(mtb,mt2,evtWeight);
		mtb_uc_h->Fill(mtb,evtWeight);

		eff_h->Fill(0.0,evtWeight);

		if(pass_loose_baseline)
		{
			met_h->Fill(met,evtWeight);
			mt2_h->Fill(mt2,evtWeight); 
			mt2_b_h->Fill(mt2_b,evtWeight); 
			mtb_mt2_h->Fill(mtb,mt2,evtWeight);
			mtb_h->Fill(mtb,evtWeight);
			ntop_h->Fill(ntop,evtWeight);
			nbottom_h->Fill(nbottompt20,evtWeight);
			njets_h->Fill(njets,evtWeight);
			njetspt20_h->Fill(njetspt20,evtWeight);
			njetspt30_h->Fill(njetspt30,evtWeight);
			HT_h->Fill(HT,evtWeight);
			genHT_h->Fill(genHT,evtWeight);
			genmet_h->Fill(genmet,evtWeight);
			if(jpt_eta_2p5_3p0.size() == 0) jpt_eta_2p5_3p0_h->Fill(0.0,evtWeight);
			else jpt_eta_2p5_3p0_h->Fill(jpt_eta_2p5_3p0.at(0),evtWeight);

			eff_h->Fill(1.0,evtWeight);
		}

		if(pass_loose_baseline_no_HT)
		{
			mtb_no_HT_h->Fill(mtb,evtWeight);
			mt2_no_mtb_h->Fill(mt2,evtWeight);
			nbottompt20_no_mtb_h->Fill(nbottompt20,evtWeight);
			nbottompt30_no_mtb_h->Fill(nbottompt30,evtWeight);
			ntop_no_mtb_h->Fill(ntop,evtWeight);
			nw_no_mtb_h->Fill(nw,evtWeight);
			njetspt20_no_mtb_h->Fill(njetspt20,evtWeight);
			njetspt30_no_mtb_h->Fill(njetspt30,evtWeight);
			met_no_mtb_h->Fill(met,evtWeight);
			HT_no_mtb_h->Fill(HT,evtWeight);
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
					//mt2_140_MT2_h->Fill(mt2,evtWeight);
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
					//mt2_175_MT2_h->Fill(mt2,evtWeight);
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

	TFile out_file(outputfile,"RECREATE"); // Creates TFile with name passed to outputfile from input argument

	met_uc_h->Write(); // Write (must be "->" here) to File @ebinter
	mtb_uc_h->Write();
	mtb_mt2_uc_h->Write();
	search_bin_h->Write();
	search_bin_8026_h->Write();
	search_bin_singleMuCR_h->Write();
	search_bin_singleElCR_h->Write();
	search_bin_MTb_h->Write();
	search_bin_singleMuCR_MTb_h->Write();
	search_bin_singleElCR_MTb_h->Write();
	search_bin_low_and_highdm_h->Write();
	search_bin_highdm_h->Write();
	search_bin_highdm_MT2_h->Write();
	search_bin_highdm_merge_HT_h->Write();
	search_bin_team_A_highdm_h->Write();
	search_bin_low_and_highdm_singleMuCR_h->Write();
	search_bin_highdm_singleMuCR_h->Write();
	search_bin_highdm_singleMuCR_MT2_h->Write();
	search_bin_highdm_singleMuCR_merge_HT_h->Write();
	search_bin_low_and_highdm_singleElCR_h->Write();
	search_bin_highdm_singleElCR_h->Write();
	search_bin_highdm_singleElCR_MT2_h->Write();
	search_bin_highdm_singleElCR_merge_HT_h->Write();
	search_bin_team_A_highdm_singleMuCR_h->Write();
	search_bin_team_A_highdm_singleElCR_h->Write();
	search_bin_team_A_highdm_merge_h->Write();
	search_bin_team_A_highdm_singleMuCR_merge_h->Write();
	search_bin_team_A_highdm_singleElCR_merge_h->Write();
	search_bin_team_A_lowdm_h->Write();
	search_bin_team_A_lowdm_singleMuCR_h->Write();
	search_bin_team_A_lowdm_singleElCR_h->Write();
	search_bin_team_A_highdm_MTb175_MT2_h->Write();
	search_bin_team_A_highdm_MTb140_MT2_h->Write();
	search_bin_team_A_highdm_MTb175_MT2_singleElCR_h->Write();
	search_bin_team_A_highdm_MTb140_MT2_singleElCR_h->Write();
	search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h->Write();
	search_bin_team_A_highdm_MTb140_MT2_singleMuCR_h->Write();
	mt2_140_low_h->Write();
	mt2_140_high_h->Write();
	mt2_175_low_h->Write();
	mt2_175_high_h->Write();
	mt2_140_175_h->Write();
	mt2_baseline_no_mt2_h->Write();

	jpt_eta_2p5_3p0_h->Write();
	jpt_eta_2p5_3p0_no_HT_h->Write();
	mtb_no_HT_h->Write();
	mtb_highdm_h->Write();
	mt2_no_mtb_h->Write();
	nbottompt20_no_mtb_h->Write();
	nbottompt30_no_mtb_h->Write();
	ntop_no_mtb_h->Write();
	nw_no_mtb_h->Write();
	njetspt20_no_mtb_h->Write();
	njetspt30_no_mtb_h->Write();
	met_no_mtb_h->Write();
	HT_no_mtb_h->Write();

	mt2_140_h->Write();
	nbottompt20_140_h->Write();
	nbottompt30_140_h->Write();
	ntop_140_h->Write();
	nw_140_h->Write();
	njetspt20_140_h->Write();
	njetspt30_140_h->Write();
	met_140_h->Write();
	HT_140_h->Write();
	//mt2_140_MT2_h->Write();
	nbottompt20_140_MT2_h->Write();
	nbottompt30_140_MT2_h->Write();
	ntop_140_MT2_h->Write();
	nw_140_MT2_h->Write();
	njetspt20_140_MT2_h->Write();
	njetspt30_140_MT2_h->Write();
	met_140_MT2_h->Write();
	HT_140_MT2_h->Write();

	mt2_175_h->Write();
	nbottompt20_175_h->Write();
	nbottompt30_175_h->Write();
	ntop_175_h->Write();
	ntop_merge_175_h->Write();
	ntop_w_175_h->Write();
	ntop_res_175_h->Write();
	nw_175_h->Write();

	gen_w_pt_175_h->Write();
	gen_b_pt_175_h->Write();
	gen_w_b_mass_175_h->Write();
	gen_w_b_delta_r_175_h->Write();
	n_res_gen_match_h->Write();
	n_res_b_gen_match_h->Write();
	n_res_w_gen_match_h->Write();

	njetspt20_175_h->Write();
	njetspt30_175_h->Write();
	met_175_h->Write();
	HT_175_h->Write();
	ntop_nw_h->Write();
	//mt2_175_MT2_h->Write();
	nbottompt20_175_MT2_h->Write();
	nbottompt30_175_MT2_h->Write();
	ntop_175_MT2_h->Write();
	nw_175_MT2_h->Write();
	njetspt20_175_MT2_h->Write();
	njetspt30_175_MT2_h->Write();
	met_175_MT2_h->Write();
	HT_175_MT2_h->Write();
	met_lowdm_h->Write();
	njetspt20_lowdm_h->Write();;
	nbottompt20_lowdm_h->Write();;
	nSV_lowdm_h->Write();;
	ISRpt_lowdm_h->Write();;
	bottompt_scalar_sum_lowdm_h->Write();;
	ISRpt_MET_lowdm_h->Write();;

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	met_h->Write();
	genmet_h->Write();
	nbottom_h->Write();
	ntop_h->Write();
	mt2_h->Write();
	mt2_b_h->Write();
	mtb_h->Write();
	njets_h->Write();
	njetspt20_h->Write();
	njetspt30_h->Write();
	HT_h->Write();
	genHT_h->Write();
	mtb_mt2_h->Write();

	eff_h->Write();

	std::cout << "Pre-Cut MTb-MT2 Covariance is " << mtb_mt2_uc_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_uc_h->GetCorrelationFactor() << std::endl;

	std::cout << "Post-Cut MTb-MT2 Covariance is " << mtb_mt2_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_h->GetCorrelationFactor() << std::endl;
	std::cout << "n_old_bin_is_83 = " << n_old_bin_is_83 << ", n_highdm_is_176 = " << n_highdm_is_176 << ". n_fail_mtb = " << n_fail_mtb << ", n_fail_met = " << n_fail_met << ", n_fail_dPhi = " << n_fail_dPhi << std::endl;

	out_file.Close(); // Close (must be ".") File @ebinter

	ch->Reset(); // What does this do?

	return 0;

} // End Main

int SB_team_A_highdm(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met)
{
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 53;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 54;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 55;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 56;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 57;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 58;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 59;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 60;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 61;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 62;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 63;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 64;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 65;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 66;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 67;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 68;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 69;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 650) return 70;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 250 && met < 350) return 71;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 350 && met < 450) return 72;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 450 && met < 550) return 73;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 550 && met < 650) return 74;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 650) return 75;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && met > 550) return 76;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 250 && met < 350) return 77;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 350 && met < 450) return 78;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 450 && met < 550) return 79;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 550) return 80;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 81;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 650) return 82;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350) return 83;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 450) return 84;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 450 && met < 550) return 85;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && met < 650) return 86;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 650) return 87;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350) return 88;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 450) return 89;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 450 && met < 550) return 90;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && met < 650) return 91;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 650) return 92;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 550) return 93;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350) return 94;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 450) return 95;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 450 && met < 550) return 96;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 550) return 97;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350) return 98;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 450) return 99;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 450) return 100;
	if (nb >=2 && mtb > mtb_cut && ntop >=2 && nres ==0 && nw ==0 && met > 250) return 101;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres >=2 && nw ==0 && met > 250) return 102;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw >=2 && met > 250) return 103;
	return -1;
}

int SB_team_A_lowdm(int njets, int nb, int nSV, double ISRpt, double bottompt_scalar_sum, double met)
{
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets <= 5 && met > 450 && met < 550) return 0;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets <= 5 && met > 550 && met < 650) return 1;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets <= 5 && met > 650 && met < 750) return 2;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets <= 5 && met > 750) return 3;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets >= 6 && met > 450 && met < 550) return 4;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets >= 6 && met > 550 && met < 650) return 5;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets >= 6 && met > 650 && met < 750) return 6;
	if (nb == 0 && nSV == 0 && ISRpt > 500 && njets >= 6 && met > 750) return 7;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets <= 5 && met > 450 && met < 550) return 8;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets <= 5 && met > 550 && met < 650) return 9;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets <= 5 && met > 650 && met < 750) return 10;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets <= 5 && met > 750) return 11;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets >= 6 && met > 450 && met < 550) return 12;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets >= 6 && met > 550 && met < 650) return 13;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets >= 6 && met > 650 && met < 750) return 14;
	if (nb == 0 && nSV >= 1 && ISRpt > 500 && njets >= 6 && met > 750) return 15;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 40 && met > 300 && met < 400) return 16;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 40 && met > 400 && met < 500) return 17;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 40 && met > 500 && met < 600) return 18;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 40 && met > 600) return 19;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 300 && met < 400) return 20;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 400 && met < 500) return 21;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 500 && met < 600) return 22;
	if (nb == 1 && nSV == 0 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 600) return 23;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum < 40 && met > 450 && met < 550) return 24;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum < 40 && met > 550 && met < 650) return 25;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum < 40 && met > 650 && met < 750) return 26;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum < 40 && met > 750) return 27;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 450 && met < 550) return 28;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 550 && met < 650) return 29;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 650 && met < 750) return 30;
	if (nb == 1 && nSV == 0 && ISRpt > 500 && bottompt_scalar_sum > 40 && bottompt_scalar_sum < 70 && met > 750) return 31;
	if (nb == 1 && nSV >= 1 && bottompt_scalar_sum < 40 && met > 300 && met < 400) return 32;
	if (nb == 1 && nSV >= 1 && bottompt_scalar_sum < 40 && met > 400 && met < 500) return 33;
	if (nb == 1 && nSV >= 1 && bottompt_scalar_sum < 40 && met > 500) return 34;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 80 && met > 300 && met < 400) return 35;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 80 && met > 400 && met < 500) return 36;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum < 80 && met > 500) return 37;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 300 && met < 400) return 38;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 400 && met < 500) return 39;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 500) return 40;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 300 && met < 400) return 41;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 400 && met < 500) return 42;
	if (nb >= 2 && ISRpt > 300 && ISRpt < 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 500) return 43;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum < 80 && met > 450 && met < 550) return 44;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum < 80 && met > 550 && met < 650) return 45;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum < 80 && met > 650) return 46;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 450 && met < 550) return 47;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 550 && met < 650) return 48;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 80 && bottompt_scalar_sum < 140 && met > 650) return 49;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 450 && met < 550) return 50;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 550 && met < 650) return 51;
	if (nb >= 2 && ISRpt > 500 && bottompt_scalar_sum > 140 && njets >= 7 && met > 650) return 52;
	return -1;
}

int SB_highdm(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met, double ht)
{
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 53;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 54;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 55;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 56;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 57;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 58;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 59;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 60;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 61;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 62;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 63;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 64;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 65;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 66;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 67;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 68;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 69;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 650) return 70;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 250 && met < 350) return 71;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 350 && met < 450) return 72;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 450 && met < 550) return 73;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 550 && met < 650) return 74;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 650) return 75;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && met > 550) return 76;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 250 && met < 350) return 77;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 350 && met < 450) return 78;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 450 && met < 550) return 79;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 550) return 80;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 81;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 650) return 82;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350) return 83;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 450) return 84;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 450 && met < 550) return 85;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && met < 650) return 86;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 650) return 87;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350) return 88;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 450) return 89;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 450 && met < 550) return 90;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && met < 650) return 91;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 650) return 92;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 550) return 93;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350) return 94;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 450) return 95;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 450 && met < 550) return 96;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 550) return 97;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350) return 98;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 450) return 99;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 450) return 100;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 450) return 101;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 450 && met < 600) return 102;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 600) return 103;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 450) return 104;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 450 && met < 600) return 105;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 600) return 106;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 450) return 107;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 450 && met < 600) return 108;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 600) return 109;
	if (nb ==2 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 250 && met < 400) return 110;
	if (nb ==2 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 400) return 111;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht < 1000) return 112;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 113;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1500) return 114;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht < 1000) return 115;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 116;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht > 1500) return 117;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht < 1000) return 118;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht > 1000 && ht < 1500) return 119;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht > 1500) return 120;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht < 1000) return 121;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 122;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1500) return 123;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht < 1000) return 124;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 125;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht > 1500) return 126;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht < 1000) return 127;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht > 1000 && ht < 1500) return 128;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht > 1500) return 129;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht < 1000) return 130;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 131;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1500) return 132;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht < 1000) return 133;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 134;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht > 1500) return 135;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht < 1000) return 136;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht > 1000 && ht < 1500) return 137;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht > 1500) return 138;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 139;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 140;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 141;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 142;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 500 && ht < 1300) return 143;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 500 && ht > 1300) return 144;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && ht < 1300) return 145;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && ht > 1300) return 146;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 500 && ht < 1300) return 147;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 500 && ht > 1300) return 148;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 500 && ht < 1300) return 149;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 500 && ht > 1300) return 150;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht < 1300) return 151;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1300) return 152;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 350 && met < 500 && ht < 1300) return 153;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 350 && met < 500 && ht > 1300) return 154;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 500 && ht < 1300) return 155;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 500 && ht > 1300) return 156;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 157;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 158;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 159;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 160;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 500 && ht < 1300) return 161;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 500 && ht > 1300) return 162;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 163;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 164;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 165;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 166;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 500 && ht < 1300) return 167;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 500 && ht > 1300) return 168;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 350 && ht < 1300) return 169;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 350 && ht > 1300) return 170;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 350 && met < 500 && ht < 1300) return 171;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 350 && met < 500 && ht > 1300) return 172;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 500 && ht < 1300) return 173;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 500 && ht > 1300) return 174;
	if (nb >=3 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 250 && met < 400) return 175;
	if (nb >=3 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 400) return 176;
	return -1;
}

int SB_highdm_MT2(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double met, double ht, double mt2)
{
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 53;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 54;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 55;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 56;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 250 && met < 300) return 57;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 300 && met < 400) return 58;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 400 && met < 500) return 59;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && met > 500) return 60;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 61;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 62;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 63;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 64;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 250 && met < 350) return 65;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 350 && met < 450) return 66;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 450 && met < 550) return 67;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && met > 550) return 68;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 69;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && met > 650) return 70;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 250 && met < 350) return 71;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 350 && met < 450) return 72;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 450 && met < 550) return 73;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 550 && met < 650) return 74;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && met > 650) return 75;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && met > 550 && mt2 < 300) return 76;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && met > 550 && mt2 > 300 && mt2 < 450) return 77;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && met > 550 && mt2 > 450) return 78;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 250 && met < 350 && mt2 < 300) return 79;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 250 && met < 350 && mt2 > 300 && mt2 < 450) return 80;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 250 && met < 350 && mt2 > 450) return 81;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 350 && met < 450 && mt2 < 300) return 82;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 350 && met < 450 && mt2 > 300 && mt2 < 450) return 83;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 350 && met < 450 && mt2 > 450) return 84;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 450 && met < 550 && mt2 < 300) return 85;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 450 && met < 550 && mt2 > 300 && mt2 < 450) return 86;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 450 && met < 550 && mt2 > 450) return 87;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 550 && mt2 < 300) return 88;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 550 && mt2 > 300 && mt2 < 450) return 89;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && met > 550 && mt2 > 450) return 90;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && met < 650) return 91;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 650) return 92;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350) return 93;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 450) return 94;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 450 && met < 550) return 95;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && met < 650) return 96;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 650) return 97;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350) return 98;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 450) return 99;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 450 && met < 550) return 100;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && met < 650) return 101;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 650) return 102;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 550 && mt2 < 300) return 103;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 550 && mt2 > 300 && mt2 < 400) return 104;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 550 && mt2 > 400) return 105;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && mt2 < 300) return 106;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && mt2 > 300 && mt2 < 400) return 107;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && mt2 > 400) return 108;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 450 && mt2 < 300) return 109;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 450 && mt2 > 300 && mt2 < 400) return 110;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 450 && mt2 > 400) return 111;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 450 && met < 550 && mt2 < 300) return 112;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 450 && met < 550 && mt2 > 300 && mt2 < 400) return 113;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 450 && met < 550 && mt2 > 400) return 114;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 550 && mt2 < 300) return 115;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 550 && mt2 > 300 && mt2 < 400) return 116;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 550 && mt2 > 400) return 117;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && mt2 < 300) return 118;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && mt2 > 300 && mt2 < 400) return 119;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && mt2 > 400) return 120;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 450 && mt2 < 300) return 121;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 450 && mt2 > 300 && mt2 < 400) return 122;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 450 && mt2 > 400) return 123;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 450 && mt2 < 300) return 124;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 450 && mt2 > 300 && mt2 < 400 ) return 125;
	if (nb ==2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 450 && mt2 > 400) return 126;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 450 && mt2 < 300) return 127;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 450 && mt2 > 300 && mt2 < 400) return 128;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 450 && mt2 > 400) return 129;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 450 && met < 600 && mt2 < 300) return 130;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 450 && met < 600 && mt2 > 300 & mt2 < 400) return 131;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 450 && met < 600 && mt2 > 400) return 132;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 600 && mt2 < 300) return 133;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 600 && mt2 > 300 && mt2 < 400) return 134;
	if (nb ==2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 600 && mt2 > 400) return 135;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 450 && mt2 < 300) return 136;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 450 && mt2 > 300 && mt2 < 400) return 137;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 450 && mt2 > 400) return 138;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 450 && met < 600 && mt2 < 300) return 139;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 450 && met < 600 && mt2 > 300 && mt2 < 400) return 140;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 450 && met < 600 && mt2 > 400) return 141;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 600 && mt2 < 300) return 142;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 600 && mt2 > 300 && mt2 < 400) return 143;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 600 && mt2 > 400) return 144;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 450 && mt2 < 300) return 145;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 450 && mt2 > 300 && mt2 < 400) return 146;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 450 && mt2 > 400) return 147;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 450 && met < 600 && mt2 < 300) return 148;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 450 && met < 600 && mt2 > 300 && mt2 < 400) return 149;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 450 && met < 600 && mt2 > 400) return 150;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 600 && mt2 < 300) return 151;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 600 && mt2 > 300 && mt2 < 400) return 152;
	if (nb ==2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 600 && mt2 > 400) return 153;
	if (nb ==2 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 250 && met < 400) return 154;
	if (nb ==2 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 400) return 155;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht < 1000) return 156;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 157;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1500) return 158;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht < 1000) return 159;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 160;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 350 && met < 550 && ht > 1500) return 161;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht < 1000) return 162;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht > 1000 && ht < 1500) return 163;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && met > 550 && ht > 1500) return 164;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht < 1000) return 165;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 166;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1500) return 167;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht < 1000) return 168;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 169;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 350 && met < 550 && ht > 1500) return 170;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht < 1000) return 171;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht > 1000 && ht < 1500) return 172;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && met > 550 && ht > 1500) return 173;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht < 1000) return 174;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1000 && ht < 1500) return 175;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1500) return 176;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht < 1000) return 177;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht > 1000 && ht < 1500) return 178;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 350 && met < 550 && ht > 1500) return 179;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht < 1000) return 180;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht > 1000 && ht < 1500) return 181;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && met > 550 && ht > 1500) return 182;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 183;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 184;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 185;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 186;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 500 && ht < 1300) return 187;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && met > 500 && ht > 1300) return 188;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && ht < 1300) return 189;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 250 && met < 350 && ht > 1300) return 190;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 500 && ht < 1300) return 191;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 350 && met < 500 && ht > 1300) return 192;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 500 && ht < 1300) return 193;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && met > 500 && ht > 1300) return 194;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht < 1300) return 195;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 250 && met < 350 && ht > 1300) return 196;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 350 && met < 500 && ht < 1300) return 197;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 350 && met < 500 && ht > 1300) return 198;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 500 && ht < 1300) return 199;
	if (nb >=3 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && met > 500 && ht > 1300) return 200;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 201;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 202;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 203;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 204;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 500 && ht < 1300) return 205;
	if (nb >=3 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && met > 500 && ht > 1300) return 206;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 350 && ht < 1300) return 207;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 250 && met < 350 && ht > 1300) return 208;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 350 && met < 500 && ht < 1300) return 209;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 350 && met < 500 && ht > 1300) return 210;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 500 && ht < 1300) return 211;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && met > 500 && ht > 1300) return 212;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 350 && ht < 1300) return 213;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 250 && met < 350 && ht > 1300) return 214;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 350 && met < 500 && ht < 1300) return 215;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 350 && met < 500 && ht > 1300) return 216;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 500 && ht < 1300) return 217;
	if (nb >=3 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && met > 500 && ht > 1300) return 218;
	if (nb >=3 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 250 && met < 400) return 219;
	if (nb >=3 && mtb > mtb_cut && ntop+nres+nw >=3 && met > 400) return 220;
	return -1;
}

