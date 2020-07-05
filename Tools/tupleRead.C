#include "NTupleReader.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "SB2018.h"
#include "my_Aux.h"

int main(int argc, char* argv[]){

	bool debug = false;

	int max_events = -1;
	//max_events = 1;

	// ---------- Input & Output File Arguments ----------

	if (argc != 3)
	{
		std::cerr <<"Please give 2 arguments " << "File List " << "Name of output file" << std::endl;
		return -1;
	}
	const char *inputfilelist = argv[1];
	const char *outputfile   = argv[2];

	bool apply_pass_HEM_veto = true;//force to apply HEM veto on 2018 MC
	bool apply_SF = true;		//turn off when running on data
	bool apply_prefire_SF = true;	//turn off when running 2018 sample
	bool apply_ISR_SF = false;	//only for signal and 2016 TTbar
	bool apply_pass_trigger = false;//only for data
	bool apply_fastsim_SF = false;	//only for fastsim

	std::string sample_name(inputfilelist);
	if(sample_name.find("Data") != std::string::npos)
	{
		std::cout << "=======================find data, turn off SF================================" << std::endl;
		apply_SF = false;
		apply_pass_trigger = true;
	}
	if(sample_name.find("SMS") != std::string::npos)
	{
		std::cout << "=======================find SMS, turn on ISR SF==============================" << std::endl;
		apply_ISR_SF = true;
	}
	if(sample_name.find("TTbar") != std::string::npos && sample_name.find("2016") != std::string::npos)
	{
		std::cout << "=======================find 2016 TTbar, turn on ISR SF=======================" << std::endl;
		apply_ISR_SF = true;
	}
	if(sample_name.find("2018") != std::string::npos)
	{
		std::cout << "=======================find 2018 sample, turn off prefire SF=======================" << std::endl;
		apply_prefire_SF = false;
	}
	if(sample_name.find("fastsim") != std::string::npos)
	{
		std::cout << "=======================find fastsim, turn on fastsim SF=======================" << std::endl;
		apply_fastsim_SF = true;
	}

	std::ifstream bigfile;
	std::string filename;
	bigfile.open (inputfilelist);
	TChain *ch = new TChain("Events");

	if (bigfile.is_open())
	{
		while (getline(bigfile,filename))
		{
			std::cout << filename << std::endl;		
			ch->Add(filename.c_str());
		}
		bigfile.close();
	}

	if(debug) std::cout << __LINE__ << std::endl;

	NTupleReader tr(ch);
	tr.setReThrow(false);

	if(debug) std::cout << __LINE__ << std::endl;

	auto loose_baseline_cutflow_h=new TH1F("loose_baseline_cutflow_h","0: all 1: PassEventFilter 2: PassLeptonVeto 3: PassNjets 4: PassMET 5: PassHT 6: PassdPhiLowDM",7,0.0,7.0);
	auto photon_trigger_cutflow_h=new TH1F("photon_trigger_cutflow_h","0: all 1: PassEventFilter 2: PassLeptonVeto 3: PassNjets 4: PassMET 5: PassHT 6: Pass_dPhi_QCD 7: n_photon_mid >=1 8: PassTrigger",9,0.0,9.0);
	auto eff_h=new TH1F("eff_h","0: all. 1: loose baseline. 2: low dm. 3: high dm",4,0.0,4.0);
	auto gen_filter_eff_h=new TH1F("gen_filter_eff_h","0: SigWTab_SigWeightGenCut. 1: SigWTab_SigWeightGenCutPass",2,0.0,2.0);
        auto ISR_SF_uc_h=new TH1F("ISR_SF_uc_h","ISR SF, no cuts",100,0.0,2.0);
        auto B_SF_uc_h=new TH1F("B_SF_uc_h","B tagging SF, no cuts",100,0.0,2.0);
        auto Trigger_SF_uc_h=new TH1F("Trigger_SF_uc_h","Trigger efficiency, no cuts",100,0.0,2.0);
        auto PU_SF_uc_h=new TH1F("PU_SF_uc_h","pileup weight, no cuts",100,0.0,2.0);
        auto preFire_SF_uc_h=new TH1F("preFire_SF_uc_h","preFire weight, no cuts",100,0.0,2.0);
        auto Res_SF_uc_h=new TH1F("Res_SF_uc_h","Res top tagging SF, no cuts",100,0.0,2.0);
        auto Top_SF_uc_h=new TH1F("Top_SF_uc_h","Top tagging SF, no cuts",100,0.0,2.0);
        auto fastsim_SF_uc_h=new TH1F("fastsim_SF_uc_h","fastsim SF, no cuts",100,0.0,2.0);
        auto ISR_SF_h=new TH1F("ISR_SF_h","ISR SF, loose baseline",100,0.0,2.0);
        auto B_SF_h=new TH1F("B_SF_h","B tagging SF, loose baseline",100,0.0,2.0);
        auto Trigger_SF_h=new TH1F("Trigger_SF_h","Trigger efficiency, loose baseline",100,0.0,2.0);
        auto PU_SF_h=new TH1F("PU_SF_h","pileup weight, loose baseline",100,0.0,2.0);
        auto preFire_SF_h=new TH1F("preFire_SF_h","preFire weight, loose baseline",100,0.0,2.0);
        auto Res_SF_h=new TH1F("Res_SF_h","Res top tagging SF, loose baseline",100,0.0,2.0);
        auto Top_SF_h=new TH1F("Top_SF_h","Top tagging SF, loose baseline",100,0.0,2.0);
        auto fastsim_SF_h=new TH1F("fastsim_SF_h","fastsim SF, loose baseline",100,0.0,2.0);

	auto met_uc_h=new TH1F("met_uc_h","MET, no cuts",80,0.0,1600.0);
	auto met_h=new TH1F("met_h","MET, loose baseline",80,0.0,1600.0);
	auto met_lowdm_h=new TH1F("met_lowdm_h","MET, low dm",80,0.0,1600.0);
	auto met_lowdm_LLCR_h=new TH1F("met_lowdm_LLCR_h","MET, low dm, LL CR",80,0.0,1600.0);
	auto met_lowdm_jetpt30_h=new TH1F("met_lowdm_jetpt30_h","MET, low dm, jet pt 30",80,0.0,1600.0);
	auto met_lowdm_LLCR_jetpt30_h=new TH1F("met_lowdm_LLCR_jetpt30_h","MET, low dm, LL CR, jet pt 30",80,0.0,1600.0);
	auto met_lowdm_mid_dPhi_h=new TH1F("met_lowdm_mid_dPhi_h","MET, low dm, mid dPhi",80,0.0,1600.0);
	auto met_highdm_h=new TH1F("met_highdm_h","MET, high dm",80,0.0,1600.0);
	auto met_highdm_LLCR_h=new TH1F("met_highdm_LLCR_h","MET, high dm, LL CR",80,0.0,1600.0);
	auto met_highdm_mid_dPhi_h=new TH1F("met_highdm_mid_dPhi_h","MET, high dm, mid dPhi",80,0.0,1600.0);

	auto met_gen_uc_h=new TH1F("met_gen_uc_h","genMET, no cuts",80,0.0,1600.0);
	auto met_gen_h=new TH1F("met_gen_h","genMET, loose baseline",80,0.0,1600.0);

	auto mtb_uc_h=new TH1F("mtb_uc_h","MTb, no cuts",80,0.0,800.0);
	auto mtb_h=new TH1F("mtb_h","MTb, loose baseline",80,0.0,800.0);

	auto njets_uc_h=new TH1F("njets_uc_h","njets, no cuts, pt > 20",15,0.0,15.0);
	auto njets_h=new TH1F("njets_h","njets, loose baseline, pt > 20",15,0.0,15.0);
	auto njets_lowdm_h=new TH1F("njets_lowdm_h","njets, low dm, pt > 20",15,0.0,15.0);
	auto njetspt30_lowdm_h=new TH1F("njetspt30_lowdm_h","njets, low dm, pt > 30",15,0.0,15.0);

	auto jet_pt_uc_h=new TH1F("jet_pt_uc_h","pt of jet, no cut",100,20.0,40.0);
	auto jet_pt_h=new TH1F("jet_pt_h","pt of jet, loose baseline",100,20.0,40.0);
	auto jet_pt_lowdm_h=new TH1F("jet_pt_lowdm_h","pt of jet, low dm",100,20.0,40.0);

	auto nb_uc_h=new TH1F("nb_uc_h","nbottom, no cuts, pt > 20",8,0.0,8.0);
	auto nb_h=new TH1F("nb_h","nbottom, loose baseline, pt > 20",8,0.0,8.0);

	auto ntop_merge_uc_h=new TH1F("ntop_merge_uc_h","number of fully merged top, no cuts",8,0.0,8.0);
	auto ntop_merge_h=new TH1F("ntop_merge_h","number of fully merged top, loose baseline",8,0.0,8.0);

	auto ntop_res_uc_h=new TH1F("ntop_res_uc_h","number of resolved top, no cuts",8,0.0,8.0);
	auto ntop_res_h=new TH1F("ntop_res_h","number of resolved top, loose baseline",8,0.0,8.0);

	auto nw_uc_h=new TH1F("nw_uc_h","nW, no cuts",8,0.0,8.0);
	auto nw_h=new TH1F("nw_h","nW, loose baseline",8,0.0,8.0);

	auto HT_uc_h=new TH1F("HT_uc_h","HT, no cuts",100,0.0,3000.0);
	auto HT_h=new TH1F("HT_h","HT, loose baseline",100,0.0,3000.0);

	auto S_met_lowdm_no_S_met_h=new TH1F("S_met_lowdm_no_S_met_h","MET sig, low dm, no MET sig cut",100,0.0,100.0);
	auto S_met_lowdm_no_S_met_Stop0l_evtWeight_h=new TH1F("S_met_lowdm_no_S_met_Stop0l_evtWeight_h","MET sig with Stop0l_evtWeight, low dm, no MET sig cut",100,0.0,100.0);
	auto ISRpt_lowdm_h=new TH1F("ISRpt_lowdm_h","ISR pt, low dm",80,0.0,1600.0);
	auto bottompt_scalar_sum_lowdm_h=new TH1F("bottompt_scalar_sum_lowdm_h","bottom pt sclar sum, low dm",80,0.0,1600.0);
	auto min_jet_dPhi_lowdm_h=new TH1F("min_jet_dPhi_lowdm_h","min jet dPhi, low dm",80,0.0,3.2);
	auto min_jet_dPhi_highdm_h=new TH1F("min_jet_dPhi_highdm_h","min jet dPhi, high dm",80,0.0,3.2);

	auto search_bin_v2_h=new TH1F("search_bin_v2_h","search bin v2, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_Stop0l_evtWeight_h=new TH1F("search_bin_v2_Stop0l_evtWeight_h","search bin v2 with Stop0l_evtWeight, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleMuCR_h=new TH1F("search_bin_v2_singleMuCR_h","search bin v2, single muon control region, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleElCR_h=new TH1F("search_bin_v2_singleElCR_h","search bin v2, single electron control region, MTb = 175",204,0.0,204.0);

	auto search_bin_v3_h=new TH1F("search_bin_v3_h","search bin v3",204,0.0,204.0);
	auto search_bin_v3_Stop0l_evtWeight_h=new TH1F("search_bin_v3_Stop0l_evtWeight_h","search bin v3 with Stop0l_evtWeight",204,0.0,204.0);
	auto search_bin_v3_singleMuCR_h=new TH1F("search_bin_v3_singleMuCR_h","search bin v3, single muon control region",204,0.0,204.0);
	auto search_bin_v3_singleElCR_h=new TH1F("search_bin_v3_singleElCR_h","search bin v3, single electron control region",204,0.0,204.0);

	auto search_bin_v4_h=new TH1F("search_bin_v4_h","search bin v4",183,0.0,183.0);
	auto search_bin_v4_Stop0l_evtWeight_h=new TH1F("search_bin_v4_Stop0l_evtWeight_h","search bin v4 with Stop0l_evtWeight",183,0.0,183.0);
	auto search_bin_v4_singleMuCR_h=new TH1F("search_bin_v4_singleMuCR_h","search bin v4, single muon control region",183,0.0,183.0);
	auto search_bin_v4_singleElCR_h=new TH1F("search_bin_v4_singleElCR_h","search bin v4, single electron control region",183,0.0,183.0);

	auto search_bin_v4_jetpt30_h=new TH1F("search_bin_v4_jetpt30_h","search bin v4 jet pt 30",183,0.0,183.0);
	auto search_bin_v4_Stop0l_evtWeight_jetpt30_h=new TH1F("search_bin_v4_Stop0l_evtWeight_jetpt30_h","search bin v4 with Stop0l_evtWeight, jet pt 30",183,0.0,183.0);
	auto search_bin_v4_singleMuCR_jetpt30_h=new TH1F("search_bin_v4_singleMuCR_jetpt30_h","search bin v4, single muon control region, jet pt 30",183,0.0,183.0);
	auto search_bin_v4_singleElCR_jetpt30_h=new TH1F("search_bin_v4_singleElCR_jetpt30_h","search bin v4, single electron control region, jet pt 30",183,0.0,183.0);

	//auto search_bin_v2_lowdm_h=new TH1F("search_bin_v2_lowdm_h","search bin v2 low dm, MTb = 175",53,0.0,53.0);
	//auto search_bin_v2_singleMuCR_lowdm_h=new TH1F("search_bin_v2_singleMuCR_lowdm_h","search bin v2 low dm, single muon control region, MTb = 175",53,0.0,53.0);
	//auto search_bin_v2_singleElCR_lowdm_h=new TH1F("search_bin_v2_singleElCR_lowdm_h","search bin v2 low dm, single electron control region, MTb = 175",53,0.0,53.0);

	//auto search_bin_v2_lowdm_more_MET_h=new TH1F("search_bin_v2_lowdm_more_MET_h","search bin v2 low dm more MET bins, MTb = 175",68,0.0,68.0);
	//auto search_bin_v2_singleMuCR_lowdm_more_MET_h=new TH1F("search_bin_v2_singleMuCR_lowdm_more_MET_h","search bin v2 low dm more MET bins, single muon control region, MTb = 175",68,0.0,68.0);
	//auto search_bin_v2_singleElCR_lowdm_more_MET_h=new TH1F("search_bin_v2_singleElCR_lowdm_more_MET_h","search bin v2 low dm more MET bins, single electron control region, MTb = 175",68,0.0,68.0);

	//auto search_bin_v2_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, MTb = 175",53,0.0,53.0);
	//auto search_bin_v2_singleMuCR_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_singleMuCR_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, single muon control region, MTb = 175",53,0.0,53.0);
	//auto search_bin_v2_singleElCR_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_singleElCR_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, single electron control region, MTb = 175",53,0.0,53.0);

	auto search_bin_v2_highdm_h=new TH1F("search_bin_v2_highdm_h","search bin v2 high dm, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleMuCR_highdm_h=new TH1F("search_bin_v2_singleMuCR_highdm_h","search bin v2 high dm, single muon control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleElCR_highdm_h=new TH1F("search_bin_v2_singleElCR_highdm_h","search bin v2 high dm, single electron control region, MTb = 175",151,53.0,204.0);

	auto search_bin_v2_highdm_loose_bin_h=new TH1F("search_bin_v2_highdm_loose_bin_h","search bin v2 high dm loose bins, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleMuCR_highdm_loose_bin_h=new TH1F("search_bin_v2_singleMuCR_highdm_loose_bin_h","search bin v2 high dm loose bins, single muon control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleElCR_highdm_loose_bin_h=new TH1F("search_bin_v2_singleElCR_highdm_loose_bin_h","search bin v2 high dm loose bins, single electron control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_QCD_CR_highdm_loose_bin_h=new TH1F("search_bin_v2_QCD_CR_highdm_loose_bin_h","search bin v2 high dm loose bins, QCD control region, MTb = 175",151,53.0,204.0);

	//auto search_bin_v2_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, MTb = 175, jet pt 30",151,53.0,204.0);
	//auto search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, single muon control region, MTb = 175, jet pt 30",151,53.0,204.0);
	//auto search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, single electron control region, MTb = 175, jet pt 30",151,53.0,204.0);
	//auto search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, QCD control region, MTb = 175, jetpt30",151,53.0,204.0);

	//auto search_bin_v2_jetpt30_h=new TH1F("search_bin_v2_jetpt30_h","search bin v2, MTb = 175, jet pt 30",204,0.0,204.0);
	//auto search_bin_v2_singleMuCR_jetpt30_h=new TH1F("search_bin_v2_singleMuCR_jetpt30_h","search bin v2, single muon control region, MTb = 175, jet pt 30",204,0.0,204.0);
	//auto search_bin_v2_singleElCR_jetpt30_h=new TH1F("search_bin_v2_singleElCR_jetpt30_h","search bin v2, single electron control region, MTb = 175, jet pt 30",204,0.0,204.0);

	//auto search_bin_v2_Stop0l_evtWeight_jetpt30_h=new TH1F("search_bin_v2_Stop0l_evtWeight_jetpt30_h","search bin v2 with Stop0l_evtWeight, MTb = 175, jet pt 30",204,0.0,204.0);

	auto search_bin_v2_lowdm_validation_0p1_h=new TH1F("search_bin_v2_lowdm_validation_0p1_h","search bin v2 low dm validation, dPhi 0.1",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_0p15_h=new TH1F("search_bin_v2_lowdm_validation_0p15_h","search bin v2 low dm validation, dPhi 0.15",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_0p2_h=new TH1F("search_bin_v2_lowdm_validation_0p2_h","search bin v2 low dm validation, dPhi 0.2",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_h=new TH1F("search_bin_v2_lowdm_validation_h","search bin v2 low dm validation",19,0.0,19.0);
	auto search_bin_v2_lowdm_validation_Stop0l_evtWeight_h=new TH1F("search_bin_v2_lowdm_validation_Stop0l_evtWeight_h","search bin v2 low dm validation with Stop0l_evtWeight",19,0.0,19.0);
	auto search_bin_v2_lowdm_validation_MET_h=new TH1F("search_bin_v2_lowdm_validation_MET_h","low dm validation MET study, 0: 250-300, 1: 300-400, 2: 400-500, 3: >500",4,0.0,4.0);
	auto search_bin_v2_lowdm_validation_MET_Stop0l_evtWeight_h=new TH1F("search_bin_v2_lowdm_validation_MET_Stop0l_evtWeight_h","low dm validation MET study, 0: 250-300, 1: 300-400, 2: 400-500, 3: >500",4,0.0,4.0);
	auto search_bin_v2_highdm_validation_h=new TH1F("search_bin_v2_highdm_validation_h","search bin v2 high dm validation",24,19.0,43.0);
	auto search_bin_v2_highdm_validation_Stop0l_evtWeight_h=new TH1F("search_bin_v2_highdm_validation_Stop0l_evtWeight_h","search bin v2 high dm validation with Stop0l_evtWeight",24,19.0,43.0);
	auto search_bin_v3_highdm_validation_h=new TH1F("search_bin_v3_highdm_validation_h","search bin v3 high dm validation",24,19.0,43.0);
	auto search_bin_v3_highdm_validation_Stop0l_evtWeight_h=new TH1F("search_bin_v3_highdm_validation_Stop0l_evtWeight_h","search bin v3 high dm validation with Stop0l_evtWeight",24,19.0,43.0);
	auto search_bin_v3_highdm_validation_MET_h=new TH1F("search_bin_v3_highdm_validation_MET_h","high dm validation MET study, 0: 250-350, 1: 350-450, 2: 450-550, 3: 550-650, 4:>650",5,0.0,5.0);
	auto search_bin_v3_highdm_validation_MET_Stop0l_evtWeight_h=new TH1F("search_bin_v3_highdm_validation_MET_Stop0l_evtWeight_h","high dm validation MET study, 0: 250-350, 1: 350-450, 2: 450-550, 3: 550-650, 4:>650",5,0.0,5.0);

	auto nMuons_uc_h=new TH1F("nMuons_uc_h","number of muons, no cuts",10,0.0,10.0);
	auto nElectrons_uc_h=new TH1F("nElectrons_uc_h","number of electrons, no cuts",10,0.0,10.0);

	//int bin68_all = 0, bin68_HT_MET = 0;

	while(tr.getNextEvent())
	{

		if(debug) std::cout << __LINE__ << std::endl;
		if(max_events > 0 && tr.getEvtNum() > max_events) break;

		//if(tr.getEvtNum() == 1) tr.printTupleMembers();
		if(tr.getEvtNum() % 2000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

		//if(tr.getEvtNum() < 350000) continue;
		//if (tr.getVar<unsigned long long>("event") != 519215141) continue;

		//only take the sign from evtWeight!
		float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
		float evtWeight = 1.0, evtWeight_sign = 1.0;
		if (Stop0l_evtWeight < 0) evtWeight_sign = -1.0;
		if(tr.checkBranch("Stop0l_smearWeight"))
		{Stop0l_evtWeight = Stop0l_evtWeight * tr.getVar<float>("Stop0l_smearWeight"); }

		float SigWTab_SigWeightGenCut = 1, SigWTab_SigWeightGenCutPass = 1;
		if(tr.checkBranch("SigWTab_SigWeightGenCut") && tr.checkBranch("SigWTab_SigWeightGenCutPass"))
		{
			SigWTab_SigWeightGenCut = tr.getVar<float>("SigWTab_SigWeightGenCut");
			SigWTab_SigWeightGenCutPass = tr.getVar<float>("SigWTab_SigWeightGenCutPass");
		}

		float B_SF = 1.0;
		float ISR_SF = 1.0;
		float trigger_eff = 1.0;
		float puWeight = 1.0;
		float PrefireWeight = 1.0;
		float Res_SF = 1.0;
		float Top_SF = 1.0;
		float fastsim_SF = 1.0;
		
		if(apply_SF)
		{
			B_SF = tr.getVar<float>("BTagWeight");
			trigger_eff = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline");
			puWeight = tr.getVar<float>("puWeight");
			
			Res_SF = tr.getVar<float>("Stop0l_ResTopWeight");
			std::vector<float> Top_SF_vec = tr.getVec<float>("FatJet_SF");
			for(int i = 0; i < Top_SF_vec.size(); i++)
			{Top_SF = Top_SF * Top_SF_vec.at(i);}

			if(apply_fastsim_SF)
			{
				std::vector<float> fastsim_SF_vec = tr.getVec<float>("FatJet_fastSF");
				for(int i = 0; i < fastsim_SF_vec.size(); i++)
				{fastsim_SF = fastsim_SF * fastsim_SF_vec.at(i);}
			}
			if(apply_prefire_SF) PrefireWeight = tr.getVar<float>("PrefireWeight");
			if(apply_ISR_SF) ISR_SF = tr.getVar<float>("ISRWeight");
			evtWeight = evtWeight_sign * B_SF * ISR_SF * trigger_eff * puWeight * PrefireWeight * Res_SF * Top_SF * fastsim_SF;
		}

		bool Pass_trigger_MET = true;
		if(apply_pass_trigger) Pass_trigger_MET = tr.getVar<bool>("Pass_trigger_MET");
		bool Pass_exHEMVeto30 = true;
		if((sample_name.find("2018") != std::string::npos && apply_pass_HEM_veto) || (sample_name.find("Data") != std::string::npos && tr.getVar<unsigned int>("run") >= 319077)) Pass_exHEMVeto30 = tr.getVar<bool>("Pass_exHEMVeto30");
		//if (Pass_exHEMVeto30 == false) std::cout << "veto a HEM event" << std::endl;
		bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
		bool Pass_JetID = tr.getVar<bool>("Pass_JetID");
		bool Pass_CaloMETRatio = tr.getVar<bool>("Pass_CaloMETRatio");
		Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio && Pass_trigger_MET && Pass_exHEMVeto30; 
		//Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio && Pass_exHEMVeto30; 

		//bool Pass_Baseline = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && met >= 250 && tr.getVar<bool>("Pass_HT") && tr.getVar<bool>("Pass_dPhiMET"));
		bool Pass_Baseline = (Pass_EventFilter && tr.getVar<bool>("Pass_Baseline"));
		bool Pass_lowDM = (Pass_EventFilter && tr.getVar<bool>("Pass_lowDM"));
		bool Pass_highDM = (Pass_EventFilter && tr.getVar<bool>("Pass_highDM"));
		bool Pass_LLCR_lowDM = (Pass_EventFilter && tr.getVar<bool>("Pass_LLCR_lowDM"));
		bool Pass_LLCR_highDM = (Pass_EventFilter && tr.getVar<bool>("Pass_LLCR_highDM"));
		bool Pass_QCDCR_highDM = (Pass_EventFilter && tr.getVar<bool>("Pass_QCDCR_highDM"));

		float met = tr.getVar<float>("MET_pt"); 
		float genmet = 0;
		float recomet = 0;
		if(tr.checkBranch("GenMET_pt"))
		{
			genmet = tr.getVar<float>("GenMET_pt");
			recomet = 2 * met - genmet;
		}
		float HT = tr.getVar<float>("Stop0l_HT"); 
		float mtb = tr.getVar<float>("Stop0l_Mtb"); 
		int njets = tr.getVar<int>("Stop0l_nJets");
		int nb = tr.getVar<int>("Stop0l_nbtags");
		int ntop_merge = tr.getVar<int>("Stop0l_nTop");
		int ntop_res = tr.getVar<int>("Stop0l_nResolved");
		int nw = tr.getVar<int>("Stop0l_nW");
		int nSV = tr.getVar<int>("Stop0l_nSoftb");
		float ISRpt = tr.getVar<float>("Stop0l_ISRJetPt"); 
		float bottompt_scalar_sum = tr.getVar<float>("Stop0l_Ptb");
		float S_met = tr.getVar<float>("Stop0l_METSig");

		std::vector<unsigned char> Jet_Stop0l = tr.getVec<unsigned char>("Jet_Stop0l");
		std::vector<float> Jet_pt = tr.getVec<float>("Jet_pt");
		std::vector<float> Jet_eta = tr.getVec<float>("Jet_eta");
		std::vector<float> Jet_dPhiMET = tr.getVec<float>("Jet_dPhiMET");
		std::vector<my_jet_struct> jet_struct_vec;

		int njetspt30 = 0;
		float HTpt30 = 0;
		std::vector<float> Jetpt30_dPhiMET;

		for (int i=0; i < Jet_dPhiMET.size(); i++)
		{
			if (Jet_pt.at(i) >= 20 && fabs(Jet_eta.at(i)) < 4.7)
			{
				my_jet_struct jet_struct_temp = {Jet_pt.at(i), Jet_dPhiMET.at(i)};
				jet_struct_vec.push_back(jet_struct_temp);
				if (Jet_pt.at(i) >= 30)
				{
					Jetpt30_dPhiMET.push_back(Jet_dPhiMET.at(i));
					if(fabs(Jet_eta.at(i)) < 2.4)
					{
						njetspt30++;
						HTpt30 = HTpt30 + Jet_pt.at(i);
					}
				}
			}
		}
		if (jet_struct_vec.size() >=2) std::sort (jet_struct_vec.begin(), jet_struct_vec.end(), pt_descending_sort);

		float min_jet_dPhi = 0;
		if(jet_struct_vec.size() == 2) min_jet_dPhi = std::min(jet_struct_vec.at(0).jet_dPhi, jet_struct_vec.at(1).jet_dPhi);
		if(jet_struct_vec.size() >= 3) min_jet_dPhi = std::min(std::min(jet_struct_vec.at(0).jet_dPhi, jet_struct_vec.at(1).jet_dPhi), jet_struct_vec.at(2).jet_dPhi);

		bool pass_dPhi_jetpt30 = false;
		bool pass_dPhi_QCD_jetpt30 = false;
		if (Jetpt30_dPhiMET.size() >=2)
		{
			pass_dPhi_jetpt30 = Jetpt30_dPhiMET.at(0) > 0.5 && Jetpt30_dPhiMET.at(1) > 0.15;
			if (Jetpt30_dPhiMET.size() >=3) pass_dPhi_jetpt30 = pass_dPhi_jetpt30 && Jetpt30_dPhiMET.at(2) > 0.15;
			pass_dPhi_QCD_jetpt30 = Jetpt30_dPhiMET.at(0) < 0.1 || Jetpt30_dPhiMET.at(1) < 0.1;
			if (Jetpt30_dPhiMET.size() >=3) pass_dPhi_QCD_jetpt30 = pass_dPhi_QCD_jetpt30 || Jetpt30_dPhiMET.at(2) < 0.1;
		}
		bool Pass_lowDM_pt30 = (njetspt30 >=2 && HTpt30 >= 300 && pass_dPhi_jetpt30);
		bool pass_dPhi_highdm_jetpt30 = false;
		if (Jetpt30_dPhiMET.size() >=4) pass_dPhi_highdm_jetpt30 = Jetpt30_dPhiMET.at(0) > 0.5 && Jetpt30_dPhiMET.at(1) > 0.5 && Jetpt30_dPhiMET.at(2) > 0.5 && Jetpt30_dPhiMET.at(3) > 0.5;
		bool Pass_highDM_pt30 = (njetspt30 >=5 && HTpt30 >= 300 && pass_dPhi_highdm_jetpt30);
		bool Pass_highDM_QCD_pt30 = (njetspt30 >=5 && HTpt30 >= 300 && pass_dPhi_QCD_jetpt30);

		//==================== a test for AK8 top tagger, to be commented out=================
		/*float tight_WP = 0.937;
		if(sample_name.find("2017") != std::string::npos || sample_name.find("2018") != std::string::npos) tight_WP = 0.895;
		std::vector<float> FatJet_pt = tr.getVec<float>("FatJet_pt");
		std::vector<float> FatJet_eta = tr.getVec<float>("FatJet_eta");
		std::vector<float> FatJet_disc = tr.getVec<float>("FatJet_deepTag_TvsQCD");
		std::vector<float> FatJet_SD = tr.getVec<float>("FatJet_msoftdrop");
		ntop_merge = 0;
		for (int i=0; i < FatJet_pt.size(); i++)
		{
			//if(FatJet_pt.at(i) > 400 && fabs(FatJet_eta.at(i)) < 2 && FatJet_SD.at(i) > 105 && FatJet_SD.at(i) < 210 && FatJet_disc.at(i) > 0.937) ntop_merge++;
			if(FatJet_pt.at(i) > 400 && fabs(FatJet_eta.at(i)) < 2 && FatJet_SD.at(i) > 105 && FatJet_disc.at(i) > tight_WP) ntop_merge++;
		}*/
		//=================== test end =======================================================

		//==================== a test for TTbar, to be commented out==========================
		/*if(sample_name.find("TTbarSingleLep") != std::string::npos)
		{
			std::vector<int> GenPart_pdgId_vec = tr.getVec<int>("GenPart_pdgId");
			bool found_Z = false;
			for(int i=0; i < GenPart_pdgId_vec.size(); i++)
			{
				if(GenPart_pdgId_vec.at(i) == 23) found_Z = true;
			}
			if(found_Z)
			{
				std::cout << "found Z in TTbar event" << std::endl;
				std::cout << "event number " << tr.getVar<unsigned long long>("event") << std::endl;
				for(int i=0; i < GenPart_pdgId_vec.size(); i++){std::cout << GenPart_pdgId_vec.at(i) << ", ";}
			}
		}*/
		//=================== test end =======================================================

		int nElectrons = 0, nMuons = 0, nPhotons = 0;
		std::vector<unsigned char> Electron_Stop0l = tr.getVec<unsigned char>("Electron_Stop0l");
		std::vector<float> Electron_MtW = tr.getVec<float>("Electron_MtW");
		for (int i = 0; i < Electron_Stop0l.size(); i++)
		{if (Electron_Stop0l.at(i) && Electron_MtW.at(i) < 100) nElectrons++;}
		std::vector<unsigned char> Muon_Stop0l = tr.getVec<unsigned char>("Muon_Stop0l");
		std::vector<float> Muon_MtW = tr.getVec<float>("Muon_MtW");
		for (int i = 0; i < Muon_Stop0l.size(); i++)
		{if (Muon_Stop0l.at(i) && Muon_MtW.at(i) < 100) nMuons++;}
		std::vector<unsigned char> Photon_Stop0l = tr.getVec<unsigned char>("Photon_Stop0l");
		for (int i = 0; i < Photon_Stop0l.size(); i++)
		{ nPhotons++;}

		bool Pass_dPhi_QCD = (min_jet_dPhi < 0.1);
		bool Pass_dPhi_0p15 = (min_jet_dPhi < 0.15);
		bool Pass_dPhi_0p2 = (min_jet_dPhi < 0.2);
		bool Pass_lowDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
		bool Pass_lowDM_no_S_met = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && ISRpt >= 200 && tr.getVar<bool>("Pass_dPhiMET"));
		bool Pass_lowDM_mid_dPhi_0p1 = (Pass_lowDM_no_dPhi && (!Pass_dPhi_QCD) && (!tr.getVar<bool>("Pass_dPhiMET")));
		//bool Pass_lowDM_mid_dPhi_0p15 = (Pass_lowDM_no_dPhi && (!Pass_dPhi_0p15) && (!tr.getVar<bool>("Pass_dPhiMET")));
		bool Pass_lowDM_mid_dPhi_0p15 = (Pass_lowDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMETMedDM"));
		bool Pass_lowDM_mid_dPhi_0p2 = (Pass_lowDM_no_dPhi && (!Pass_dPhi_0p2) && (!tr.getVar<bool>("Pass_dPhiMET")));

		bool Pass_highDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1);
		bool Pass_highDM_mid_dPhi = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));

		//==================temp test=================================
		//bool Pass_LeptonVeto = tr.getVar<bool>("Pass_ElecVeto") && tr.getVar<bool>("Pass_MuonVeto") && tr.getVar<bool>("Pass_IsoTrkVeto");
		//Pass_lowDM = (Pass_EventFilter && Pass_LeptonVeto && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200 && tr.getVar<bool>("Pass_dPhiMET"));
		//Pass_highDM = (Pass_EventFilter && Pass_LeptonVeto && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1 && tr.getVar<bool>("Pass_dPhiMETHighDM"));
		//==================end test==================================
		int SBv2_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SBv2_highdm_index_175 = SBv2_highdm(175, mtb, njets, ntop_merge, nw, ntop_res, nb, met, HT);
		//int SBv2_highdm_index_175_jetpt30 = SBv2_highdm(175, mtb, njetspt30, ntop_merge, nw, ntop_res, nb, met, HTpt30);
		std::vector<int> SBv2_highdm_loose_bin(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		std::vector<int> SBv2_highdm_loose_bin_index_175 = SBv2_highdm_loose_bin(175, mtb, njets, ntop_merge, nw, ntop_res, nb, met, HT);
		//std::vector<int> SBv2_highdm_loose_bin_index_175_jetpt30 = SBv2_highdm_loose_bin(175, mtb, njetspt30, ntop_merge, nw, ntop_res, nb, met, HTpt30);
		int SBv3_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met);
		int SBv3_highdm_index = SBv3_highdm(mtb, njets, nb, ntop_merge, nw, ntop_res, HT, met);
		int SBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met);
		int SBv4_highdm_index = SBv4_highdm(mtb, njets, nb, ntop_merge, nw, ntop_res, HT, met);
		int SBv4_highdm_index_jetpt30 = SBv4_highdm(mtb, njetspt30, nb, ntop_merge, nw, ntop_res, HTpt30, met);
		int SBv2_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_index = SBv2_lowdm(njets, nb, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_index_jetpt30 = SBv2_lowdm(njetspt30, nb, nSV, ISRpt, bottompt_scalar_sum, met);
		//int SBv2_lowdm_more_MET(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		//int SBv2_lowdm_more_MET_index = SBv2_lowdm_more_MET(njets, nb, nSV, ISRpt, bottompt_scalar_sum, met);
		//int SBv2_lowdm_high_ISRpt(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		//int SBv2_lowdm_high_ISRpt_index = SBv2_lowdm_high_ISRpt(njets, nb, nSV, ISRpt, bottompt_scalar_sum, met);

		int SBv2_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met);
		int SBv2_highdm_validation_index = SBv2_highdm_validation(mtb, njets, ntop_merge, nw, ntop_res, nb, met);
		int SBv3_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met);
		int SBv3_highdm_validation_index = SBv3_highdm_validation(mtb, njets, ntop_merge, nw, ntop_res, nb, met);
		int highdm_validation_MET(float mtb, int ntop, int nw, int nres, float met);
		int highdm_validation_MET_index = highdm_validation_MET(mtb, ntop_merge, nw, ntop_res, met);
		int SBv2_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_validation_index = SBv2_lowdm_validation(njets, nb, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met);
		int SBv2_lowdm_validation_high_MET_index = SBv2_lowdm_validation_high_MET(nb, nSV, ISRpt, met);
		int lowdm_validation_MET(float ISRpt, float met);
		int lowdm_validation_MET_index = lowdm_validation_MET(ISRpt, met);
		int SBv2_lowdm_validation_mid_dPhi(int njets, int nb, int nSV, float ISRpt, float met);
		int SBv2_lowdm_validation_mid_dPhi_index = SBv2_lowdm_validation_mid_dPhi(njets, nb, nSV, ISRpt, met);

		//if (tr.getEvtNum() == 29144)
		/*if (tr.getVar<unsigned long long>("event") == 519215141)
		{
			std::cout << "find event 519215141" << std::endl;
			std::cout << "met " << met << std::endl;
			std::cout << "HT " << HT << std::endl;
			std::cout << "ISRpt " << ISRpt << std::endl;
			std::cout << "min_jet_dPhi " << min_jet_dPhi << std::endl;
			std::cout << "Pass_dPhi_0p15 " << Pass_dPhi_0p15 << std::endl;
			std::cout << "Pass_dPhiMET " << tr.getVar<bool>("Pass_dPhiMET") << std::endl;
			std::cout << "Pass_dPhiMETLowDM " << tr.getVar<bool>("Pass_dPhiMETLowDM") << std::endl;
			for (int i = 0; i < Jet_dPhiMET.size(); i++)
			{std::cout << "jet_" << i << " pt " << Jet_pt.at(i) << " eta " << Jet_eta.at(i) << " dPhi " << Jet_dPhiMET.at(i) << std::endl;}
			for (int i = 0; i < jet_struct_vec.size(); i++)
			{std::cout << "jet_struct_" << i << " pt " << jet_struct_vec.at(i).jet_pt << " dPhi " << jet_struct_vec.at(i).jet_dPhi << std::endl;}
		}*/

                // cutflow
                loose_baseline_cutflow_h->Fill(0., evtWeight_sign);
		photon_trigger_cutflow_h->Fill(0., evtWeight_sign);
                if(Pass_EventFilter)
		{
			loose_baseline_cutflow_h->Fill(1., evtWeight_sign);
			photon_trigger_cutflow_h->Fill(1., evtWeight_sign);
		}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto"))
		{
			loose_baseline_cutflow_h->Fill(2., evtWeight_sign);
			photon_trigger_cutflow_h->Fill(2., evtWeight_sign);
		}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2)
		{
			loose_baseline_cutflow_h->Fill(3., evtWeight_sign);
			photon_trigger_cutflow_h->Fill(3., evtWeight_sign);
		}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250)
		{
			loose_baseline_cutflow_h->Fill(4., evtWeight_sign);
			photon_trigger_cutflow_h->Fill(4., evtWeight_sign);
		}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250 && tr.getVar<bool>("Pass_HT"))
		{
			loose_baseline_cutflow_h->Fill(5., evtWeight_sign);
			photon_trigger_cutflow_h->Fill(5., evtWeight_sign);
		}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250 && tr.getVar<bool>("Pass_HT") && tr.getVar<bool>("Pass_dPhiMETLowDM"))
		{loose_baseline_cutflow_h->Fill(6., evtWeight_sign);}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250 && tr.getVar<bool>("Pass_HT") && Pass_dPhi_QCD)
		{photon_trigger_cutflow_h->Fill(6., evtWeight_sign);}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250 && tr.getVar<bool>("Pass_HT") && Pass_dPhi_QCD && nPhotons >=1)
		{photon_trigger_cutflow_h->Fill(7., evtWeight_sign);}
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njets >= 2 && met >= 250 && tr.getVar<bool>("Pass_HT") && Pass_dPhi_QCD && nPhotons >=1 && tr.getVar<bool>("Pass_trigger_MET"))
		{photon_trigger_cutflow_h->Fill(8., evtWeight_sign);}

		// no cut
		eff_h->Fill(0., evtWeight_sign);
		gen_filter_eff_h->Fill(0., evtWeight_sign * SigWTab_SigWeightGenCut);
		gen_filter_eff_h->Fill(1., evtWeight_sign * SigWTab_SigWeightGenCutPass);

                ISR_SF_uc_h->Fill(ISR_SF);
                B_SF_uc_h->Fill(B_SF);
                Trigger_SF_uc_h->Fill(trigger_eff);
                PU_SF_uc_h->Fill(puWeight);
                preFire_SF_uc_h->Fill(PrefireWeight);
                Res_SF_uc_h->Fill(Res_SF);
                Top_SF_uc_h->Fill(Top_SF);
                fastsim_SF_uc_h->Fill(fastsim_SF);

		for (int i=0; i < Jet_dPhiMET.size(); i++)
		{
			if (Jet_pt.at(i) >= 20 && fabs(Jet_eta.at(i)) < 2.4) jet_pt_uc_h->Fill(Jet_pt.at(i));
		}
		met_uc_h->Fill(met);
		//met_uc_h->Fill(recomet);
		met_gen_uc_h->Fill(genmet);
		HT_uc_h->Fill(HT,evtWeight);
		mtb_uc_h->Fill(mtb,evtWeight);
		njets_uc_h->Fill(njets,evtWeight);
		nb_uc_h->Fill(nb,evtWeight);
		ntop_merge_uc_h->Fill(ntop_merge,evtWeight);
		ntop_res_uc_h->Fill(ntop_res,evtWeight);
		nw_uc_h->Fill(nw,evtWeight);
		nMuons_uc_h->Fill(nMuons,evtWeight);
		nElectrons_uc_h->Fill(nElectrons,evtWeight);

		if(Pass_Baseline) 
		{
			eff_h->Fill(1., evtWeight_sign);
                        ISR_SF_h->Fill(ISR_SF);
                        B_SF_h->Fill(B_SF);
                	Trigger_SF_h->Fill(trigger_eff);
                	PU_SF_h->Fill(puWeight);
                	preFire_SF_h->Fill(PrefireWeight);
                        Res_SF_h->Fill(Res_SF);
                        Top_SF_h->Fill(Top_SF);
                        fastsim_SF_h->Fill(fastsim_SF);


			for (int i=0; i < Jet_dPhiMET.size(); i++)
			{
				if (Jet_pt.at(i) >= 20 && fabs(Jet_eta.at(i)) < 2.4) jet_pt_h->Fill(Jet_pt.at(i));
			}
			met_h->Fill(met,evtWeight);
			met_gen_h->Fill(genmet);
			HT_h->Fill(HT,evtWeight);
			mtb_h->Fill(mtb,evtWeight);
			njets_h->Fill(njets,evtWeight);
			nb_h->Fill(nb,evtWeight);
			ntop_merge_h->Fill(ntop_merge,evtWeight);
			ntop_res_h->Fill(ntop_res,evtWeight);
			nw_h->Fill(nw,evtWeight);
		}

		if(Pass_lowDM)
		{
			eff_h->Fill(2., evtWeight_sign);
			met_lowdm_h->Fill(met,evtWeight);
			ISRpt_lowdm_h->Fill(ISRpt,evtWeight);
			bottompt_scalar_sum_lowdm_h->Fill(bottompt_scalar_sum,evtWeight);
			njets_lowdm_h->Fill(njets,evtWeight);
		

			for (int i=0; i < Jet_dPhiMET.size(); i++)
			{
				if (Jet_pt.at(i) >= 20 && fabs(Jet_eta.at(i)) < 2.4) jet_pt_lowdm_h->Fill(Jet_pt.at(i));
			}
			if(SBv2_lowdm_index != -1) search_bin_v2_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v2_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv2_lowdm_index != -1) search_bin_v3_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v3_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv2_lowdm_index != -1) search_bin_v4_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v4_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,evtWeight * fabs(Stop0l_evtWeight));
			//if(SBv2_lowdm_index != -1) search_bin_v2_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
			//if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
			//if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);
			if(SBv2_lowdm_validation_index != -1) search_bin_v2_lowdm_validation_h->Fill(SBv2_lowdm_validation_index,evtWeight);
			//if(SBv2_lowdm_validation_index == 0) std::cout << "validation bin 0, run " << tr.getVar<unsigned int>("run") << " event " << tr.getVar<unsigned long long>("event") << std::endl;
			if(SBv2_lowdm_validation_index != -1) search_bin_v2_lowdm_validation_Stop0l_evtWeight_h->Fill(SBv2_lowdm_validation_index,evtWeight * fabs(Stop0l_evtWeight));

			if(Pass_lowDM_pt30)
			{
				met_lowdm_jetpt30_h->Fill(met,evtWeight);
				njetspt30_lowdm_h->Fill(njetspt30,evtWeight);
				//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
				//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
				if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v4_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
				if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v4_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
			}
		}

		if(Pass_highDM)
		{
			eff_h->Fill(3., evtWeight_sign);
			met_highdm_h->Fill(met,evtWeight);
		
			if(SBv2_highdm_index_175 != -1) search_bin_v2_h->Fill(SBv2_highdm_index_175,evtWeight);
			if(SBv2_highdm_index_175 != -1) search_bin_v2_Stop0l_evtWeight_h->Fill(SBv2_highdm_index_175,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv3_highdm_index != -1) search_bin_v3_h->Fill(SBv3_highdm_index,evtWeight);
			if(SBv3_highdm_index != -1) search_bin_v3_Stop0l_evtWeight_h->Fill(SBv3_highdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv4_highdm_index != -1) search_bin_v4_h->Fill(SBv4_highdm_index,evtWeight);
			if(SBv4_highdm_index != -1) search_bin_v4_Stop0l_evtWeight_h->Fill(SBv4_highdm_index,evtWeight * fabs(Stop0l_evtWeight));

			if(SBv2_highdm_index_175 != -1) search_bin_v2_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
			//========================a test of v4 vs v5 ntuple, to be commented out======================
			/*if(SBv4_highdm_index == 182)
			{
				//std::cout << "find event in SB 182, tr.getVar<unsigned long long>(\"event\")= " << tr.getVar<unsigned long long>("event") << ", evtWeight = " << evtWeight << std::endl;
			}
			std::vector<int> last_bin_event_number = {23334112,176434507,56460412,15444274,199321902,2991396,37509395,77730017,92439059,84576925,85900697,80753297,45360420,50582049,102101863,99503863,122912266,38412301,31931149,18607018,78574167,10394824,52507884,53657196,177709245};
			for (int i=0; i < last_bin_event_number.size(); i++)
			{
				if(last_bin_event_number.at(i) == tr.getVar<unsigned long long>("event"))
				{
					std::cout << "find event " << last_bin_event_number.at(i) << std::endl;
					std::cout << "nt " << ntop_merge << " nw " << nw << " nres " << ntop_res << std::endl;
				}
			}*/
			//===========================================test end===========================================
			//if(SBv2_highdm_index_175 != -1 && SBv3_highdm_index == -1) std::cout << "more v2 bin " << SBv2_highdm_index_175 << " ht " << HT << std::endl;
			//if(SBv2_highdm_index_175 == -1 && SBv3_highdm_index != -1) std::cout << "more v3 bin " << SBv3_highdm_index << " Njet " << njets << std::endl;
			//========================a test of 2016 TTbar, to be commented out=============================
			/*if(sample_name.find("TTbarSingleLepTbar") != std::string::npos && SBv4_highdm_index == 178)
			{
				std::cout << "found TTbarSingleLepTbar event in bin 178" << std::endl;
				std::cout << "event number " << tr.getVar<unsigned long long>("event") << std::endl;
				std::cout << "nt " << ntop_merge << " nw " << nw << " nres " << ntop_res << std::endl;
			}*/
			/*if(sample_name.find("TTbarSingleLepTbar") != std::string::npos && tr.getVar<unsigned long long>("event") == 38155774)
			{
				std::cout << "found TTbarSingleLepTbar event 38155774" << std::endl;
				std::cout << "bin number " << SBv4_highdm_index << std::endl;
				std::cout << "nt " << ntop_merge << " nw " << nw << " nres " << ntop_res << std::endl;
				std::cout << "GenPart_pdgId" << std::endl;
				std::vector<int> GenPart_pdgId_vec = tr.getVec<int>("GenPart_pdgId");
				for(int i=0; i < GenPart_pdgId_vec.size(); i++)
				{std::cout << GenPart_pdgId_vec.at(i) << ", ";}
			}*/
			//===========================================test end===========================================
			/*if(SBv4_highdm_index == 68)
			{
				bin68_all++;
				if(HT > 1500 && met > 650) bin68_HT_MET++;
			}*/
	
			for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

			if(Pass_highDM_pt30)
			{
				//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
				//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
				if(SBv4_highdm_index_jetpt30 != -1) search_bin_v4_jetpt30_h->Fill(SBv4_highdm_index_jetpt30,evtWeight);
				if(SBv4_highdm_index_jetpt30 != -1) search_bin_v4_Stop0l_evtWeight_jetpt30_h->Fill(SBv4_highdm_index_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
				//for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
			}
		}

		if(Pass_LLCR_lowDM)
		{
			met_lowdm_LLCR_h->Fill(met,evtWeight);
			if(Pass_lowDM_pt30) met_lowdm_LLCR_jetpt30_h->Fill(met,evtWeight);
			if(nMuons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v3_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v4_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleMuCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				//if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleMuCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);

				if(Pass_lowDM_pt30)
				{
					//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleMuCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
					if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v4_singleMuCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
				}
			}
			if(nElectrons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v3_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v4_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleElCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				//if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleElCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);

				if(Pass_lowDM_pt30)
				{
					//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleElCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
					if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v4_singleElCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
				}
			}
		}

		if(Pass_LLCR_highDM)
		{
			met_highdm_LLCR_h->Fill(met,evtWeight);
			if(nMuons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv3_highdm_index != -1) search_bin_v3_singleMuCR_h->Fill(SBv3_highdm_index,evtWeight);
				if(SBv4_highdm_index != -1) search_bin_v4_singleMuCR_h->Fill(SBv4_highdm_index,evtWeight);

				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleMuCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

				if(Pass_highDM_pt30)
				{
					//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleMuCR_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
					if(SBv4_highdm_index_jetpt30 != -1) search_bin_v4_singleMuCR_jetpt30_h->Fill(SBv4_highdm_index_jetpt30,evtWeight);
					//for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
				}
			}
			if(nElectrons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv3_highdm_index != -1) search_bin_v3_singleElCR_h->Fill(SBv3_highdm_index,evtWeight);
				if(SBv4_highdm_index != -1) search_bin_v4_singleElCR_h->Fill(SBv4_highdm_index,evtWeight);

				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleElCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

				if(Pass_highDM_pt30)
				{
					//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleElCR_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
					if(SBv4_highdm_index_jetpt30 != -1) search_bin_v4_singleElCR_jetpt30_h->Fill(SBv4_highdm_index_jetpt30,evtWeight);
					//for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
				}
			}
		}

		if(Pass_QCDCR_highDM)
		{
			for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_QCD_CR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);
			//if(Pass_highDM_QCD_pt30)
			//{
			//	for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
			//}
		}

		if(Pass_lowDM_no_S_met) 
		{
			S_met_lowdm_no_S_met_h->Fill(S_met,evtWeight);
			S_met_lowdm_no_S_met_Stop0l_evtWeight_h->Fill(S_met,evtWeight * fabs(Stop0l_evtWeight));
		}

		if(Pass_lowDM_no_dPhi) 
		{
			min_jet_dPhi_lowdm_h->Fill(min_jet_dPhi,evtWeight);
		}
		if(Pass_lowDM_mid_dPhi_0p1)
		{
			met_lowdm_mid_dPhi_h->Fill(met,evtWeight); 
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p1_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
		}
		if(Pass_lowDM_mid_dPhi_0p15)
		{
			//std::cout << tr.getEvtNum() << std::endl;
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p15_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
			if(SBv2_lowdm_validation_high_MET_index != -1) search_bin_v2_lowdm_validation_h->Fill(SBv2_lowdm_validation_high_MET_index,evtWeight);
			if(SBv2_lowdm_validation_high_MET_index != -1) search_bin_v2_lowdm_validation_Stop0l_evtWeight_h->Fill(SBv2_lowdm_validation_high_MET_index,evtWeight * fabs(Stop0l_evtWeight));
			if(lowdm_validation_MET_index != -1) search_bin_v2_lowdm_validation_MET_h->Fill(lowdm_validation_MET_index,evtWeight);
			if(lowdm_validation_MET_index != -1) search_bin_v2_lowdm_validation_MET_Stop0l_evtWeight_h->Fill(lowdm_validation_MET_index,evtWeight * fabs(Stop0l_evtWeight));
		}
		if(Pass_lowDM_mid_dPhi_0p2)
		{
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p2_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
		}
		if(Pass_highDM_no_dPhi)
		{
			min_jet_dPhi_highdm_h->Fill(min_jet_dPhi,evtWeight);
		}
		if(Pass_highDM_mid_dPhi)
		{
			met_highdm_mid_dPhi_h->Fill(met,evtWeight); 
			if(SBv2_highdm_validation_index != -1) search_bin_v2_highdm_validation_h->Fill(SBv2_highdm_validation_index,evtWeight);
			if(SBv2_highdm_validation_index != -1) search_bin_v2_highdm_validation_Stop0l_evtWeight_h->Fill(SBv2_highdm_validation_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv3_highdm_validation_index != -1) search_bin_v3_highdm_validation_h->Fill(SBv3_highdm_validation_index,evtWeight);
			//if(SBv3_highdm_validation_index == 23) std::cout << "validation bin 23, run " << tr.getVar<unsigned int>("run") << " event " << tr.getVar<unsigned long long>("event") << std::endl;
			if(SBv3_highdm_validation_index != -1) search_bin_v3_highdm_validation_Stop0l_evtWeight_h->Fill(SBv3_highdm_validation_index,evtWeight * fabs(Stop0l_evtWeight));
			if(highdm_validation_MET_index != -1) search_bin_v3_highdm_validation_MET_h->Fill(highdm_validation_MET_index,evtWeight);
			if(highdm_validation_MET_index != -1) search_bin_v3_highdm_validation_MET_Stop0l_evtWeight_h->Fill(highdm_validation_MET_index,evtWeight * fabs(Stop0l_evtWeight));
		}

	}// End event loop

	TFile out_file(outputfile,"RECREATE");

	loose_baseline_cutflow_h->Write();
	photon_trigger_cutflow_h->Write();
        ISR_SF_uc_h->Write();
        B_SF_uc_h->Write();
        Trigger_SF_uc_h->Write();
        PU_SF_uc_h->Write();
        preFire_SF_uc_h->Write();
        Res_SF_uc_h->Write();
        Top_SF_uc_h->Write();
        fastsim_SF_uc_h->Write();
        ISR_SF_h->Write();
        B_SF_h->Write();
        Trigger_SF_h->Write();
        PU_SF_h->Write();
        preFire_SF_h->Write();
        Res_SF_h->Write();
        Top_SF_h->Write();
        fastsim_SF_h->Write();

	met_uc_h->Write();
	met_h->Write();
	met_lowdm_h->Write();
	met_lowdm_LLCR_h->Write();
	met_lowdm_jetpt30_h->Write();
	met_lowdm_LLCR_jetpt30_h->Write();
	met_lowdm_mid_dPhi_h->Write();
	met_highdm_h->Write();
	met_highdm_LLCR_h->Write();
	met_highdm_mid_dPhi_h->Write();

	met_gen_uc_h->Write();
	met_gen_h->Write();

	HT_uc_h->Write();
	HT_h->Write();
	mtb_uc_h->Write();
	mtb_h->Write();
	njets_uc_h->Write();
	njets_h->Write();
	njets_lowdm_h->Write();
	njetspt30_lowdm_h->Write();
	jet_pt_uc_h->Write();
	jet_pt_h->Write();
	jet_pt_lowdm_h->Write();
	nb_uc_h->Write();
	nb_h->Write();
	ntop_merge_uc_h->Write();
	ntop_merge_h->Write();
	ntop_res_uc_h->Write();
	ntop_res_h->Write();
	nw_uc_h->Write();
	nw_h->Write();

	nMuons_uc_h->Write();
	nElectrons_uc_h->Write();
	S_met_lowdm_no_S_met_h->Write();
	S_met_lowdm_no_S_met_Stop0l_evtWeight_h->Write();
	ISRpt_lowdm_h->Write();
	bottompt_scalar_sum_lowdm_h->Write();
	min_jet_dPhi_lowdm_h->Write();
	min_jet_dPhi_highdm_h->Write();

	search_bin_v2_h->Write();
	search_bin_v2_singleMuCR_h->Write();
	search_bin_v2_singleElCR_h->Write();
	search_bin_v2_Stop0l_evtWeight_h->Write();
	search_bin_v3_h->Write();
	search_bin_v3_singleMuCR_h->Write();
	search_bin_v3_singleElCR_h->Write();
	search_bin_v3_Stop0l_evtWeight_h->Write();
	search_bin_v4_h->Write();
	search_bin_v4_singleMuCR_h->Write();
	search_bin_v4_singleElCR_h->Write();
	search_bin_v4_Stop0l_evtWeight_h->Write();
	search_bin_v4_jetpt30_h->Write();
	search_bin_v4_singleMuCR_jetpt30_h->Write();
	search_bin_v4_singleElCR_jetpt30_h->Write();
	search_bin_v4_Stop0l_evtWeight_jetpt30_h->Write();
	//search_bin_v2_lowdm_h->Write();
	//search_bin_v2_singleMuCR_lowdm_h->Write();
	//search_bin_v2_singleElCR_lowdm_h->Write();
	//search_bin_v2_lowdm_more_MET_h->Write();
	//search_bin_v2_singleMuCR_lowdm_more_MET_h->Write();
	//search_bin_v2_singleElCR_lowdm_more_MET_h->Write();
	//search_bin_v2_lowdm_high_ISRpt_h->Write();
	//search_bin_v2_singleMuCR_lowdm_high_ISRpt_h->Write();
	//search_bin_v2_singleElCR_lowdm_high_ISRpt_h->Write();
	search_bin_v2_highdm_h->Write();
	search_bin_v2_singleMuCR_highdm_h->Write();
	search_bin_v2_singleElCR_highdm_h->Write();

	search_bin_v2_highdm_loose_bin_h->Write();
	search_bin_v2_singleMuCR_highdm_loose_bin_h->Write();
	search_bin_v2_singleElCR_highdm_loose_bin_h->Write();
	search_bin_v2_QCD_CR_highdm_loose_bin_h->Write();
	//search_bin_v2_highdm_loose_bin_jetpt30_h->Write();
	//search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h->Write();
	//search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h->Write();
	//search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h->Write();

	//search_bin_v2_jetpt30_h->Write();
	//search_bin_v2_singleMuCR_jetpt30_h->Write();
	//search_bin_v2_singleElCR_jetpt30_h->Write();
	//search_bin_v2_Stop0l_evtWeight_jetpt30_h->Write();
	search_bin_v2_lowdm_validation_0p1_h->Write();
	search_bin_v2_lowdm_validation_0p15_h->Write();
	search_bin_v2_lowdm_validation_0p2_h->Write();
	search_bin_v2_lowdm_validation_h->Write();
	search_bin_v2_lowdm_validation_Stop0l_evtWeight_h->Write();
	search_bin_v2_lowdm_validation_MET_h->Write();
	search_bin_v2_lowdm_validation_MET_Stop0l_evtWeight_h->Write();
	search_bin_v2_highdm_validation_h->Write();
	search_bin_v2_highdm_validation_Stop0l_evtWeight_h->Write();
	search_bin_v3_highdm_validation_h->Write();
	search_bin_v3_highdm_validation_Stop0l_evtWeight_h->Write();
	search_bin_v3_highdm_validation_MET_h->Write();
	search_bin_v3_highdm_validation_MET_Stop0l_evtWeight_h->Write();

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	eff_h->Write();
	gen_filter_eff_h->Write();

	out_file.Close();

	ch->Reset();

	//std::cout << "bin68_all " << bin68_all << " bin68_HT_MET " << bin68_HT_MET << std::endl;
	return 0;

} // End Main
