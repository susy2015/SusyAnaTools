#include "LHAPDF/LHAPDF.h"
#include "NTupleReader.h"
#include "baselineDef.h"
#include "searchBins.h"
#include "TCanvas.h"
#include "TROOT.h" // ebinter
#include "TGraphErrors.h" // ebinter
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h" // ebitner
#include "TArrow.h" // ebinter
#include "TLatex.h" // ebinter
#include <iostream>
#include <cstdio>
#include <stdio.h> // printf, Null // ebinter
#include <stdlib.h> // strtod // ebinter
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "tdrstyle.h"

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
	//int max_events = 5000;
	int max_events = -1;
	//if(argc > 3){
	//   const char *events_input = argv[3]; // Need to Fix
	//   events = strtod(*events_input, NULL);
	//}
	const char *fastsim   = "";\

				 // std::cout << "Will read first " << events << " events." << std::endl;

				 if (argc > 3)
				 {
					 fastsim = argv[3];
				 }

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

	NTupleReader tr(ch);
	tr.setReThrow(false);
	//PDFweight pdfs;

	//BaselineVessel blv(tr);
	BaselineVessel blv(tr, "", "fastsim");

	tr.registerFunction(blv);
	//tr.registerFunction(pdfs);
	SearchBins SB("SB_v1_2017");

	// ---------- Make Histograms ----------

	// Missing Transverse Energy (met)

	auto met_h=new TH1F("met_h","Missing E_T w/ Baseline Cut",80,0.0,1600.0);
	auto met_uc_h=new TH1F("met_uc_h","Missing E_T (No Cuts)",80,0.0,1600.0);
	auto met_highmt2_uc_h=new TH1F("met_highmt2_uc_h","Missing E_T for mt2>175 (No BC)",80,0.0,1600.0);
	auto met_lowmt2_uc_h=new TH1F("met_lowmt2_uc_h","Missing E_T for mt2<175 (No BC)",80,0.0,1600.0);
	auto met_highmtb_uc_h=new TH1F("met_highmtb_uc_h","Missing E_T for mtb>175 (No BC)",80,0.0,1600.0);
	auto met_lowmtb_uc_h=new TH1F("met_lowmtb_uc_h","Missing E_T for mtb<175 (No BC)",80,0.0,1600.0);
	auto met_highmtb_h=new TH1F("met_highmtb_h","Missing E_T with BC & mtb>175",80,0.0,1600.0);
	auto met_lowmtb_h=new TH1F("met_lowmtb_h","Missing E_T with BC & mtb<175",80,0.0,1600.0);
	auto met_highmt2_h=new TH1F("met_highmt2_h","Missing E_T with BC & mt2>175",80,0.0,1600.0);
	auto met_lowmt2_h=new TH1F("met_lowmt2_h","Missing E_T with BC & mt2<175",80,0.0,1600.0);
	// auto met_px_h=new TH1F("met_px_h","MET Px",80,0.0,250.0);
	// auto met_py_h=new TH1F("met_py_h","MET Py",80,0.0,250.0);

	// Transverse Mass (mt2)

	auto mt2_h=new TH1F("mt2_h","MT2",80,0.0,800.0);
	auto mt2_baseline_no_mt2_h=new TH1F("mt2_baseline_no_mt2_h","MT2 baseline no MT2 cut",80,0.0,800.0);
	auto mt2_uc_h=new TH1F("mt2_uc_h","MT2",80,0.0,800.0);
	auto mt2_high_uc_h=new TH1F("mt2_high_uc_h","MT2 w/ MTb > 175 (No Cuts)",80,0.0,800.0);
	auto mt2_low_uc_h=new TH1F("mt2_low_uc_h","MTb w/ MT2 < 175 (No Cuts)",80,0.0,800.0);
	auto mt2_high_h=new TH1F("mt2_high_h","MT2 when MTb > 175",80,0.0,800.0);
	auto mt2_low_h=new TH1F("mt2_low_h","MT2 when MTb < 175",80,0.0,800.0);

	auto mt2_140_high_h=new TH1F("mt2_140_high_h","MT2, team a bsae line when MTb > 140",80,0.0,800.0);
	auto mt2_140_low_h=new TH1F("mt2_140_low_h","MT2, team a bsae line when MTb < 140",80,0.0,800.0);
	auto mt2_175_high_h=new TH1F("mt2_175_high_h","MT2, team a bsae line when MTb > 175",80,0.0,800.0);
	auto mt2_175_low_h=new TH1F("mt2_175_low_h","MT2, team a bsae line when MTb < 175",80,0.0,800.0);
	auto mt2_140_175_h=new TH1F("mt2_140_175_h","MT2, team a bsae line when 140 < MTb < 175",80,0.0,800.0);
	auto mt2_b_h=new TH1F("mt2_b_h","MT2_b",80,0.0,800.0);

	// Transverse Mass of b-jet (mtb)

	auto mtb_h=new TH1F("mtb_h","MTb",80,0.0,800.0);
	auto mtb_high_dm_h=new TH1F("mtb_high_dm_h","MTb high dm",80,0.0,800.0);
	auto mtb_no_HT_h=new TH1F("mtb_no_HT_h","MTb in loose baseline without HT cut",80,0.0,800.0);
	auto mtb_CSV_no_HT_h=new TH1F("mtb_CSV_no_HT_h","MTb_CSV in loose baseline without HT cut",80,0.0,800.0);
	auto mtb_uc_h=new TH1F("mtb_uc_h","MTb (No Cuts)",80,0.0,800.0);
	auto mtb1_h=new TH1F("mtb1_h","Leading b-Jet MT",80,0.0,800.0);
	auto mtb2_h=new TH1F("mtb2_h","Sub-Leading b-Jet MT",80,0.0,800.0);
	auto mtb_high_uc_h=new TH1F("mtb_high_uc_h","MTb w/ MT2 > 175 (No Cuts)",80,0.0,800.0);
	auto mtb_low_uc_h=new TH1F("mtb_low_uc_h","MTb w/ MT2 < 175 (No Cuts)",80,0.0,800.0);
	auto mtb_high_h=new TH1F("mtb_high_h","MTb w/ MT2 > 175 (With Cuts)",80,0.0,800.0);
	auto mtb_low_h=new TH1F("mtb_low_h","MTb w/ MT2 < 175 (With Cuts)",80,0.0,800.0);

	// Mtb Cut Efficiency

	auto eff_h=new TH1F("eff_h","b Cut Efficiency",2,0.0,2.0);
	auto eff_mtb_high_h=new TH1F("eff_mtb_high_h","MTB b Cut Efficiency for MT2 > 175",2,0.0,2.0);
	auto eff_mtb_low_h=new TH1F("eff_mtb_low_h","MTB b Cut Efficiency for MT2 < 175",2,0.0,2.0);
	auto eff_mt2_high_h=new TH1F("eff_mt2_high_h","MT2 b Cut Efficiency for MTb > 175",2,0.0,2.0);
	auto eff_mt2_low_h=new TH1F("eff_mt2_low_h","MT2 b Cut Efficiency for MTb < 175",2,0.0,2.0);

	// Other

	auto metphi_h=new TH1F("metphi_h","MET Phi",80,-3.5,3.5);

	auto jpt_uc_h=new TH1F("jpt_uc_h","Leading Jet Pt (No Cuts)",80,0.0,400.0);
	auto nbottom_uc_h=new TH1F("nbottom_uc_h","b Jet Count (No Cuts)",8,0.0,8.0);
	auto bottom_pt_uc_h=new TH1F("bottom_pt_uc_h","Leading b Jet Pt (No Cuts)",80,0.0,400.0);
	auto ntop_uc_h=new TH1F("ntop_uc_h","Number of Tops (No Cuts)",8,0.0,8.0);
	auto njets_uc_h=new TH1F("njets_uc_h","Number of Jets (No Cuts)",30,0.0,30.0);
	auto njetspt20_uc_h=new TH1F("njetspt20_uc_h","Number of Jets above PT=20 (No Cuts)",15,0.0,15.0);
	auto njetspt30_uc_h=new TH1F("njetspt30_uc_h","Number of Jets above PT=30 (No Cuts)",15,0.0,15.0);
	auto HT_uc_h=new TH1F("HT_uc_h","HT (No Cuts)",100,0.0,3000.0);
	auto genHT_uc_h=new TH1F("genHT_uc_h","Gen Level HT (No Cuts)",100,0.0,3000.0);
	auto genmet_uc_h=new TH1F("genmet_uc_h","Gen Level MET (No Cuts)",80,0.0,400.0);
	auto search_bin_h=new TH1F("search_bin_h","search bin with baseline cut",84,0.0,84.0);
	auto search_bin_team_A_h=new TH1F("search_bin_team_A_h","search bin team A high dM = 175",52,52.0,104.0);
	auto search_bin_team_A_MTb175_MT2_h=new TH1F("search_bin_team_A_MTb175_MT2_h","search bin team A high dM = 175, MT2 >200",52,52.0,104.0);
	auto search_bin_team_A_MTb140_MT2_h=new TH1F("search_bin_team_A_MTb140_MT2_h","search bin team A high dM = 140, MT2 >200",52,52.0,104.0);

	auto jpt_h=new TH1F("jpt_h","Leading Jet Pt (Baseline Cuts)",80,0.0,400.0);
	auto nbottom_h=new TH1F("nbottom_h","b Jet Count (Baseline Cuts)",8,0.0,8.0);
	auto bottom_pt_h=new TH1F("bottom_pt_h","Leading b Jet Pt (Baseline Cuts)",80,0.0,400.0);
	auto ntop_h=new TH1F("ntop_h","Number of Tops (Baseline Cuts)",8,0.0,8.0);
	auto njets_h=new TH1F("njets_h","Number of Jets (Baseline Cuts)",30,0.0,30.0);
	auto njetspt20_h=new TH1F("njetspt20_h","Number of Jets above PT=20 (Baseline Cuts)",15,0.0,15.0);
	auto njetspt30_h=new TH1F("njetspt30_h","Number of Jets above PT=30 (Baseline Cuts)",15,0.0,15.0);
	auto HT_h=new TH1F("HT_h","HT (Baseline Cuts)",100,0.0,3000.0);
	auto genHT_h=new TH1F("genHT_h","Gen Level HT (Baseline Cuts)",100,0.0,3000.0);
	auto genmet_h=new TH1F("genmet_h","Gen Level MET (Baseline Cuts)",80,0.0,400.0);
	auto search_bin_mtb_h=new TH1F("search_bin_MTb_h","search bin with baseline cut + MTb > 175",84,0.0,84.0);

	auto nbottompt20_no_mtb_h=new TH1F("nbottompt20_no_mtb_h","number of bottom without MTb cut",8,0.0,8.0);
	auto nbottompt20_140_h=new TH1F("nbottompt20_140_h","number of bottom when MTb > 140",8,0.0,8.0);
	auto nbottompt20_140_MT2_h=new TH1F("nbottompt20_140_MT2_h","number of bottom when MTb > 140 with MT2 cut",8,0.0,8.0);
	auto nbottompt20_175_h=new TH1F("nbottompt20_175_h","number of bottom when MTb > 175",8,0.0,8.0);
	auto nbottompt20_175_MT2_h=new TH1F("nbottompt20_175_MT2_h","number of bottom when MTb > 175 with MT2 cut",8,0.0,8.0);

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
		std::vector<TLorentzVector> wLVec = tr.getVec<TLorentzVector>("vWAlone");
		int nw = wLVec.size();
		//std::cout << "number of w = " << wLVec.size() << std::endl;

		std::vector<TLorentzVector> b_jetsLVec;
		std::vector<double> rjbt_vec = tr.getVec<double>("recoJetsBtag_0");
		int njets = jetsLVec.size();
		double mtb1=-10 , mtb2=-10 , mtb=-10 , mt2_b=-10 , mtb_CSV=-10;
		double evtWeight = tr.getVar<double>("evtWeight");
		double met = tr.getVar<double>("met"); 
		double genmet = tr.getVar<double>("genmet"); 
		//double mt2 = tr.getVar<double>("mt2");
		double mt2 = tr.getVar<double>("best_had_brJet_MT2");
		bool passMT2 = (mt2 > 200);
		double metphi = tr.getVar<double>("metphi");
		int ntop = tr.getVar<int>("nTopCandSortedCnt");
		int nbjets = tr.getVar<int>("cntCSVS");
		int nbottompt20=0 , nbottompt30=0 , njetspt20=0 , njetspt30=0 , njetspt50=0;
		int ntop_merge=0 , ntop_w=0 , ntop_res=0;
		double HT = tr.getVar<double>("HT");
		double genHT = tr.getVar<double>("genHT");
		double jpt = 0 , bottom_pt = 0; 
		double bad_b_csv = 0;
		TLorentzVector bad_b_tlv;
		double b_CSV_1 = 0, b_CSV_2 = 0;
		int b_CSV_1_index = 0, b_CSV_2_index = 0;

		int SB_index = SB.find_Binning_Index(nbjets, ntop, mt2, met, HT);
		std::map<int, std::vector<TLorentzVector>> mTopJets = tr.getMap <int, std::vector<TLorentzVector>> ("mTopJets");
		int n_dPhi_p5 = 0;

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

			for(int i=0; i < PdgId.size(); i++)
			{std::cout << PdgId.at(i) << " "; }
			std::cout << "\n" << std::endl;
		}

		// ---------- Begin Sub Loop Over Jets ----------

		for(int index=0; index < njets ; index++)
		{
			TLorentzVector tlv = jetsLVec[index];
			double rjbt = rjbt_vec[index];
			double tlv_Phi = tlv.Phi();
			double tlv_Pt = tlv.Pt();
			double tlv_Eta = tlv.Eta();
			double dPhi = tlv_Phi - metphi;

			if(index == 0) jpt = tlv_Pt;
			if(fabs(tlv_Eta) <= 2.4 && tlv_Pt >= 20)
			{
				njetspt20++;
				if (tlv_Pt >= 30) njetspt30++;
				if (tlv_Pt >= 50) njetspt50++;
				if (njetspt20 < 5 && fabs(dPhi) > 0.5) n_dPhi_p5 ++;
			}

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

			double MT = sqrt(2*tlv_Pt*met*(1-cos(dPhi)));

			b_jetsLVec.push_back(tlv);
			nbottompt20++;
			if(tlv_Pt > 30) nbottompt30++;
			if(nbottompt20 == 1)
			{
				bottom_pt = tlv_Pt;
				mtb1_h->Fill(MT,evtWeight);
				mtb1=MT;
			}
			if(nbottompt20 == 2)
			{
				mtb2_h->Fill(MT,evtWeight);
				mtb2=MT;
			}
		} // End Sub-Loop Over Jets

		if(nbottompt30 != nbjets) std::cout << "nbottom (pt > 30) = " << nbottompt30 << " nbjets = " << nbjets << std::endl;

		bool passnJets = (njetspt50 >= 2 && njetspt30 >= 4);

		/*for (int i=0; i<b_jetsLVec.size(); i++)
		  {
		  std::cout << "event " << tr.getEvtNum() << " # " << i << " bjet pt " << b_jetsLVec.at(i).Pt() << std::endl;
		  }*/

		// ===  FUNCTION  ============================================================
		//         Name:  BaselineVessel::CalcMT2
		//  Description:  
		// ===========================================================================

		TLorentzVector fatJet1LVec(0, 0, 0,0);
		TLorentzVector fatJet2LVec(0, 0, 0,0);

		if (b_jetsLVec.size() == 0)
		{
			mt2_b = 0.0;
			mtb= 0.0; 
			mtb_CSV= 0.0; 
		}

		if (b_jetsLVec.size() == 1)
		{
			fatJet1LVec = b_jetsLVec.at(0);
			fatJet2LVec = bad_b_tlv;

			mt2_b = blv.coreMT2calc(fatJet1LVec, fatJet2LVec);
			mtb = mtb1;
			mtb_CSV = mtb1;
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
			mtb = std::min(mtb1,mtb2);

			double tlv_Pt_1 = jetsLVec[b_CSV_1_index].Pt();
			double tlv_Phi_1 = jetsLVec[b_CSV_1_index].Phi();
			double MT_1 = sqrt(2*tlv_Pt_1*met*(1-cos(tlv_Phi_1 - metphi)));
			double tlv_Pt_2 = jetsLVec[b_CSV_2_index].Pt();
			double tlv_Phi_2 = jetsLVec[b_CSV_2_index].Phi();
			double MT_2 = sqrt(2*tlv_Pt_2*met*(1-cos(tlv_Phi_2 - metphi)));
			mtb_CSV = std::min(MT_1,MT_2);
		}

		if (b_jetsLVec.size() < 3 && mtb != mtb_CSV) std::cout << "mtb = " << mtb << " mtb_CSV = " << mtb_CSV << std::endl;

		//std::cout << "no cut mtb = " << mtb << " mt2_b " << mt2_b << std::endl;

		for (int i = 0; i < ntop; i++)
		{
			//std::cout << "top index " << i << std::endl;

			if (mTopJets.at(i).size() ==1) ntop_merge++;
			if (mTopJets.at(i).size() ==2) ntop_w++;
			if (mTopJets.at(i).size() ==3) ntop_res++;

			//for (int k = 0; k < mTopJets.at(i).size(); k++) std::cout << "top daughter " << k << " mass = " << mTopJets.at(i).at(k).M() << std::endl;
		}

		int SB_team_A(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double MET);
		int SB_team_A_index_175 = SB_team_A(175, mtb, njetspt20, ntop_merge, ntop_w, ntop_res, nbottompt20, met);
		int SB_team_A_index_140 = SB_team_A(140, mtb, njetspt20, ntop_merge, ntop_w, ntop_res, nbottompt20, met);

		bool pass_loose_baseline_no_HT=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passdPhis") && tr.getVar<bool>("passMET") && njetspt20 >=2 && nbottompt20 >=1); 
		bool pass_loose_baseline=(pass_loose_baseline_no_HT && tr.getVar<bool>("passHT"));

		//bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passnJets") && tr.getVar<bool>("passdPhis") && tr.getVar<bool>("passBJets") && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline_no_MT2=(tr.getVar<bool>("passLeptVeto") && passnJets && tr.getVar<bool>("passdPhis") && nbjets > 0 && tr.getVar<bool>("passMET") && tr.getVar<bool>("passHT") && tr.getVar<bool>("passTagger") && tr.getVar<bool>("passNoiseEventFilter") ); 
		bool pass_baseline=(pass_baseline_no_MT2 && passMT2);

		bool pass_high_dM_baseline=(tr.getVar<bool>("passLeptVeto") && tr.getVar<bool>("passMET")  && tr.getVar<bool>("passNoiseEventFilter") && njetspt20 >= 5 && n_dPhi_p5 == 4 && nbottompt20 >=1); 

		if(pass_high_dM_baseline)
		{
			if(mtb < 140) mt2_140_low_h->Fill(mt2,evtWeight);
			if(mtb > 140) mt2_140_high_h->Fill(mt2,evtWeight);
			if(mtb < 175) mt2_175_low_h->Fill(mt2,evtWeight);
			if(mtb > 175) mt2_175_high_h->Fill(mt2,evtWeight);
			if(mtb > 140 && mtb < 175) mt2_140_175_h->Fill(mt2,evtWeight);

			mtb_high_dm_h->Fill(mtb,evtWeight);	
			search_bin_team_A_h->Fill(SB_team_A_index_175,evtWeight);
			if(passMT2) 
			{
				search_bin_team_A_MTb175_MT2_h->Fill(SB_team_A_index_175,evtWeight);
				search_bin_team_A_MTb140_MT2_h->Fill(SB_team_A_index_140,evtWeight);
			}
		}

		if(pass_baseline_no_MT2) mt2_baseline_no_mt2_h->Fill(mt2,evtWeight);

		if(pass_baseline)
		{
			if (SB_index < 0 || SB_index > 83) std::cout << "SB = " << SB_index << "nbjets = " << nbjets << "ntop = " << ntop << "mt2 = " << mt2 << "met = " << met << "HT = " << HT << std::endl;
			search_bin_h->Fill(SB_index,evtWeight);
			if (mtb > 175) search_bin_mtb_h->Fill(SB_index,evtWeight);
		}

		met_uc_h->Fill(met,evtWeight);
		mt2_uc_h->Fill(mt2,evtWeight);
		if (ntop == 0 && mt2 != 0) std::cout << "MT2 = " << mt2 << std::endl;
		mtb_mt2_uc_h->Fill(mtb,mt2,evtWeight);
		mtb_uc_h->Fill(mtb,evtWeight);
		ntop_uc_h->Fill(ntop,evtWeight);
		nbottom_uc_h->Fill(nbottompt20,evtWeight);
		njets_uc_h->Fill(njets,evtWeight);
		njetspt20_uc_h->Fill(njetspt20,evtWeight);
		njetspt30_uc_h->Fill(njetspt30,evtWeight);
		HT_uc_h->Fill(HT,evtWeight);
		genHT_uc_h->Fill(genHT,evtWeight);
		genmet_uc_h->Fill(genmet,evtWeight);
		jpt_uc_h->Fill(jpt,evtWeight);
		bottom_pt_uc_h->Fill(bottom_pt,evtWeight);

		eff_h->Fill(0.0,evtWeight);

		if(mtb > 175){
			met_highmtb_uc_h->Fill(met,evtWeight); 
			mt2_high_uc_h->Fill(mt2,evtWeight);
			eff_mt2_high_h->Fill(0.0,evtWeight);
		}
		if(mtb < 175){
			met_lowmtb_uc_h->Fill(met,evtWeight); 
			mt2_low_uc_h->Fill(mt2,evtWeight);
			eff_mt2_low_h->Fill(0.0,evtWeight);
		}
		if(mt2 > 175){
			met_highmt2_uc_h->Fill(met,evtWeight); 
			mtb_high_uc_h->Fill(mtb,evtWeight);
			eff_mtb_high_h->Fill(0.0,evtWeight);
		}
		if(mt2 < 175){
			met_lowmt2_uc_h->Fill(met,evtWeight); 
			mtb_low_uc_h->Fill(mtb,evtWeight);
			eff_mtb_low_h->Fill(0.0,evtWeight);
		}

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
			jpt_h->Fill(jpt,evtWeight);
			bottom_pt_h->Fill(bottom_pt,evtWeight);

			eff_h->Fill(1.0,evtWeight);

			if(mtb > 175){
				met_highmtb_h->Fill(met,evtWeight); 
				mt2_high_h->Fill(mt2,evtWeight);
				eff_mt2_high_h->Fill(1.0,evtWeight);
			}
			if(mtb < 175){
				met_lowmtb_h->Fill(met,evtWeight); 
				mt2_low_h->Fill(mt2,evtWeight);
				eff_mt2_low_h->Fill(1.0,evtWeight);
			}
			if(mt2 > 175){
				met_highmt2_h->Fill(met,evtWeight); 
				mtb_high_h->Fill(mtb,evtWeight);
				eff_mtb_high_h->Fill(1.0,evtWeight);
			}
			if(mt2 < 175){
				met_lowmt2_h->Fill(met,evtWeight); 
				mtb_low_h->Fill(mtb,evtWeight);
				eff_mtb_low_h->Fill(1.0,evtWeight);
			}

		}

		if(pass_loose_baseline_no_HT)
		{
			mtb_no_HT_h->Fill(mtb,evtWeight);
			mtb_CSV_no_HT_h->Fill(mtb_CSV,evtWeight);
			mt2_no_mtb_h->Fill(mt2,evtWeight);
			nbottompt20_no_mtb_h->Fill(nbottompt20,evtWeight);
			nbottompt30_no_mtb_h->Fill(nbottompt30,evtWeight);
			ntop_no_mtb_h->Fill(ntop,evtWeight);
			nw_no_mtb_h->Fill(nw,evtWeight);
			njetspt20_no_mtb_h->Fill(njetspt20,evtWeight);
			njetspt30_no_mtb_h->Fill(njetspt30,evtWeight);
			met_no_mtb_h->Fill(met,evtWeight);
			HT_no_mtb_h->Fill(HT,evtWeight);

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

					if (n_gen_w != 2) std::cout << "number of gen w is " << n_gen_w << std::endl;

					if (plus_temp.size() != 2) 
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

					if (minus_temp.size() != 2) 
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

		// ---------- Fill Other Event Level Histograms ----------

		metphi_h->Fill(metphi,evtWeight);

	} // End Loop Over Events

	// ---------- Write Histograms to File ----------

	TFile out_file(outputfile,"RECREATE"); // Creates TFile with name passed to outputfile from input argument

	met_uc_h->Write(); // Write (must be "->" here) to File @ebinter
	genmet_uc_h->Write();
	jpt_uc_h->Write();
	nbottom_uc_h->Write();
	ntop_uc_h->Write();
	bottom_pt_uc_h->Write();
	mt2_uc_h->Write();
	mtb_uc_h->Write();
	njets_uc_h->Write();
	njetspt20_uc_h->Write();
	njetspt30_uc_h->Write();
	HT_uc_h->Write();
	genHT_uc_h->Write();
	mtb_mt2_uc_h->Write();
	search_bin_h->Write();
	search_bin_mtb_h->Write();
	search_bin_team_A_h->Write();
	search_bin_team_A_MTb175_MT2_h->Write();
	search_bin_team_A_MTb140_MT2_h->Write();
	mt2_140_low_h->Write();
	mt2_140_high_h->Write();
	mt2_175_low_h->Write();
	mt2_175_high_h->Write();
	mt2_140_175_h->Write();
	mt2_baseline_no_mt2_h->Write();

	mtb_no_HT_h->Write();
	mtb_CSV_no_HT_h->Write();
	mtb_high_dm_h->Write();
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

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	met_h->Write();
	genmet_h->Write();
	jpt_h->Write();
	nbottom_h->Write();
	ntop_h->Write();
	bottom_pt_h->Write();
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

	out_file.cd("../");
	out_file.mkdir("MTb>175");
	out_file.cd("MTb>175");  

	met_highmtb_uc_h->Write();
	mt2_high_uc_h->Write();

	out_file.cd("../");
	out_file.mkdir("MTb<175");
	out_file.cd("MTb<175"); 

	met_lowmtb_uc_h->Write();
	mt2_low_uc_h->Write();

	out_file.cd("../");
	out_file.mkdir("MT2>175");
	out_file.cd("MT2>175"); 

	met_highmt2_uc_h->Write();
	mtb_high_uc_h->Write();

	out_file.cd("../");
	out_file.mkdir("MT2<175");
	out_file.cd("MT2<175"); 

	met_lowmt2_uc_h->Write();
	mtb_low_uc_h->Write();

	out_file.cd("../");
	out_file.mkdir("Baseline_MTb>175");
	out_file.cd("Baseline_MTb>175");  

	met_highmtb_h->Write();
	mt2_high_h->Write();
	eff_mt2_high_h->Write();

	out_file.cd("../");
	out_file.mkdir("Baseline_MTb<175");
	out_file.cd("Baseline_MTb<175"); 

	met_lowmtb_h->Write();
	mt2_low_h->Write();
	eff_mt2_low_h->Write();

	out_file.cd("../");
	out_file.mkdir("Baseline_MT2>175");
	out_file.cd("Baseline_MT2>175"); 

	met_highmt2_h->Write();
	mtb_high_h->Write();
	eff_mtb_high_h->Write();

	out_file.cd("../");
	out_file.mkdir("Baseline_MT2<175");
	out_file.cd("Baseline_MT2<175"); 

	met_lowmt2_h->Write();
	mtb_low_h->Write();
	eff_mtb_low_h->Write();

	out_file.cd("../");

	metphi_h->Write();
	mtb1_h->Write();
	mtb2_h->Write();

	std::cout << "Pre-Cut MTb-MT2 Covariance is " << mtb_mt2_uc_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_uc_h->GetCorrelationFactor() << std::endl;

	std::cout << "Post-Cut MTb-MT2 Covariance is " << mtb_mt2_h->GetCovariance() << " AND Correlation Factor is " << mtb_mt2_h->GetCorrelationFactor() << std::endl;

	out_file.Close(); // Close (must be ".") File @ebinter

	ch->Reset(); // What does this do?

	return 0;

} // End Main

int SB_team_A(double mtb_cut, double mtb, int njets, int ntop, int nw, int nres, int nb, double MET)
{
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 250 && MET < 300) return 53;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 300 && MET < 400) return 54;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 400 && MET < 500) return 55;
	if (nb ==1 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 500) return 56;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 250 && MET < 300) return 57;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 300 && MET < 400) return 58;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 400 && MET < 500) return 59;
	if (nb >=2 && mtb < mtb_cut && njets >=7 && nres >=1 && MET > 500) return 60;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 250 && MET < 350) return 61;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 350 && MET < 450) return 62;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 450 && MET < 550) return 63;
	if (nb ==1 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 550) return 64;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 250 && MET < 350) return 65;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 350 && MET < 450) return 66;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 450 && MET < 550) return 67;
	if (nb >=2 && mtb > mtb_cut && njets >=7 && ntop ==0 && nres ==0 && nw ==0 && MET > 550) return 68;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && MET > 550 && MET < 650) return 69;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw ==0 && MET > 650) return 70;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && MET > 250 && MET < 350) return 71;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && MET > 350 && MET < 450) return 72;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && MET > 450 && MET < 550) return 73;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && MET > 550 && MET < 650) return 74;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw ==0 && MET > 650) return 75;
	if (nb ==1 && mtb > mtb_cut && ntop >=1 && nres ==0 && nw >=1 && MET > 550) return 76;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && MET > 250 && MET < 350) return 77;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && MET > 350 && MET < 450) return 78;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && MET > 450 && MET < 550) return 79;
	if (nb ==1 && mtb > mtb_cut && ntop ==0 && nres >=1 && nw >=1 && MET > 550) return 80;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && MET > 550 && MET < 650) return 81;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==0 && MET > 650) return 82;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && MET > 250 && MET < 350) return 83;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && MET > 350 && MET < 450) return 84;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && MET > 450 && MET < 550) return 85;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && MET > 550 && MET < 650) return 86;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==0 && MET > 650) return 87;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && MET > 250 && MET < 350) return 88;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && MET > 350 && MET < 450) return 89;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && MET > 450 && MET < 550) return 90;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && MET > 550 && MET < 650) return 91;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==1 && MET > 650) return 92;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==0 && nw ==1 && MET > 550) return 93;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && MET > 250 && MET < 350) return 94;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && MET > 350 && MET < 450) return 95;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && MET > 450 && MET < 550) return 96;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==1 && nw ==1 && MET > 550) return 97;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && MET > 250 && MET < 350) return 98;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && MET > 350 && MET < 450) return 99;
	if (nb >=2 && mtb > mtb_cut && ntop ==1 && nres ==1 && nw ==0 && MET > 450) return 100;
	if (nb >=2 && mtb > mtb_cut && ntop ==2 && nres ==0 && nw ==0 && MET > 250) return 101;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==2 && nw ==0 && MET > 250) return 102;
	if (nb >=2 && mtb > mtb_cut && ntop ==0 && nres ==0 && nw ==2 && MET > 450) return 103;
	return 52;
}
