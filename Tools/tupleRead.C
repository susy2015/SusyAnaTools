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
	//max_events = 50000;

	// ---------- Input & Output File Arguments ----------

	if (argc != 3)
	{
		std::cerr <<"Please give 2 arguments " << "File List " << "Name of output file" << std::endl;
		return -1;
	}
	const char *inputfilelist = argv[1];
	const char *outputfile   = argv[2];

	if(debug) std::cout << __LINE__ << std::endl;

	bool apply_SF = true;		//turn off when running on data!!
	bool apply_ISR_SF = false;	//only for signal and 2016 TTbar!

	std::string sample_name(inputfilelist);
	if(sample_name.find("Data") != std::string::npos)
	{
		std::cout << "=======================find data, turn off SF================================" << std::endl;
		apply_SF = false;
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
	auto eff_h=new TH1F("eff_h","0: all. 1: loose baseline. 2: low dm. 3: high dm",4,0.0,4.0);
        auto ISR_SF_uc_h=new TH1F("ISR_SF_uc_h","ISR SF, no cuts",100,0.0,2.0);
        auto B_SF_uc_h=new TH1F("B_SF_uc_h","B tagging SF, no cuts",100,0.0,2.0);
        auto Trigger_SF_uc_h=new TH1F("Trigger_SF_uc_h","Trigger efficiency, no cuts",100,0.0,2.0);
        auto PU_SF_uc_h=new TH1F("PU_SF_uc_h","pileup weight, no cuts",100,0.0,2.0);
        auto preFire_SF_uc_h=new TH1F("preFire_SF_uc_h","preFire weight, no cuts",100,0.0,2.0);
        auto ISR_SF_h=new TH1F("ISR_SF_h","ISR SF, loose baseline",100,0.0,2.0);
        auto B_SF_h=new TH1F("B_SF_h","B tagging SF, loose baseline",100,0.0,2.0);
        auto Trigger_SF_h=new TH1F("Trigger_SF_h","Trigger efficiency, loose baseline",100,0.0,2.0);
        auto PU_SF_h=new TH1F("PU_SF_h","pileup weight, loose baseline",100,0.0,2.0);
        auto preFire_SF_h=new TH1F("preFire_SF_h","preFire weight, loose baseline",100,0.0,2.0);

	auto met_uc_h=new TH1F("met_uc_h","MET, no cuts",80,0.0,1600.0);
	auto met_h=new TH1F("met_h","MET, loose baseline",80,0.0,1600.0);
	auto met_lowdm_h=new TH1F("met_lowdm_h","MET, low dm",80,0.0,1600.0);
	auto met_lowdm_ISR_veto_h=new TH1F("met_lowdm_ISR_veto_h","MET, low dm, ISR veto",80,0.0,1600.0);
	auto met_lowdm_mid_dPhi_h=new TH1F("met_lowdm_mid_dPhi_h","MET, low dm, mid dPhi",80,0.0,1600.0);
	auto met_highdm_h=new TH1F("met_highdm_h","MET, high dm",80,0.0,1600.0);
	auto met_highdm_mid_dPhi_h=new TH1F("met_highdm_mid_dPhi_h","MET, high dm, mid dPhi",80,0.0,1600.0);

	auto mtb_uc_h=new TH1F("mtb_uc_h","MTb, no cuts",80,0.0,800.0);
	auto mtb_h=new TH1F("mtb_h","MTb, loose baseline",80,0.0,800.0);

	auto njetspt20_uc_h=new TH1F("njetspt20_uc_h","njets, no cuts, pt > 20",15,0.0,15.0);
	auto njetspt20_h=new TH1F("njetspt20_h","njets, loose baseline, pt > 20",15,0.0,15.0);
	auto njetspt20_lowdm_h=new TH1F("njetspt20_lowdm_h","njets, low dm, pt > 20",15,0.0,15.0);
	auto njetspt30_lowdm_h=new TH1F("njetspt30_lowdm_h","njets, low dm, pt > 30",15,0.0,15.0);

	auto jet_pt_uc_h=new TH1F("jet_pt_uc_h","pt of jet, pt > 20, eta < 2.4",100,20.0,40.0);

	auto nbottompt20_uc_h=new TH1F("nbottompt20_uc_h","nbottom, no cuts, pt > 20",8,0.0,8.0);
	auto nbottompt20_h=new TH1F("nbottompt20_h","nbottom, loose baseline, pt > 20",8,0.0,8.0);

	auto ntop_merge_uc_h=new TH1F("ntop_merge_uc_h","number of fully merged top, no cuts",8,0.0,8.0);
	auto ntop_merge_h=new TH1F("ntop_merge_h","number of fully merged top, loose baseline",8,0.0,8.0);

	auto ntop_res_uc_h=new TH1F("ntop_res_uc_h","number of resolved top, no cuts",8,0.0,8.0);
	auto ntop_res_h=new TH1F("ntop_res_h","number of resolved top, loose baseline",8,0.0,8.0);

	auto nw_uc_h=new TH1F("nw_uc_h","nW, no cuts",8,0.0,8.0);
	auto nw_h=new TH1F("nw_h","nW, loose baseline",8,0.0,8.0);

	auto HT_uc_h=new TH1F("HT_uc_h","HT, no cuts",100,0.0,3000.0);
	auto HT_h=new TH1F("HT_h","HT, loose baseline",100,0.0,3000.0);

	auto ISRpt_lowdm_h=new TH1F("ISRpt_lowdm_h","ISR pt, low dm",80,0.0,1600.0);
	auto bottompt_scalar_sum_lowdm_h=new TH1F("bottompt_scalar_sum_lowdm_h","bottom pt sclar sum, low dm",80,0.0,1600.0);
	auto min_jet_dPhi_lowdm_h=new TH1F("min_jet_dPhi_lowdm_h","min jet dPhi, low dm",80,0.0,3.2);
	auto min_jet_dPhi_highdm_h=new TH1F("min_jet_dPhi_highdm_h","min jet dPhi, high dm",80,0.0,3.2);

	auto search_bin_v2_h=new TH1F("search_bin_v2_h","search bin v2, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleMuCR_h=new TH1F("search_bin_v2_singleMuCR_h","search bin v2, single muon control region, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleElCR_h=new TH1F("search_bin_v2_singleElCR_h","search bin v2, single electron control region, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_QCD_CR_h=new TH1F("search_bin_v2_QCD_CR_h","search bin v2, QCD control region, MTb = 175",204,0.0,204.0);

	auto search_bin_v3_h=new TH1F("search_bin_v3_h","search bin v3",204,0.0,204.0);
	auto search_bin_v3_singleMuCR_h=new TH1F("search_bin_v3_singleMuCR_h","search bin v3, single muon control region",204,0.0,204.0);
	auto search_bin_v3_singleElCR_h=new TH1F("search_bin_v3_singleElCR_h","search bin v3, single electron control region",204,0.0,204.0);
	auto search_bin_v3_QCD_CR_h=new TH1F("search_bin_v3_QCD_CR_h","search bin v3, QCD control region",204,0.0,204.0);

	auto search_bin_v2_Stop0l_evtWeight_h=new TH1F("search_bin_v2_Stop0l_evtWeight_h","search bin v2 with Stop0l_evtWeight, MTb = 175",204,0.0,204.0);
	auto search_bin_v3_Stop0l_evtWeight_h=new TH1F("search_bin_v3_Stop0l_evtWeight_h","search bin v3 with Stop0l_evtWeight",204,0.0,204.0);
	//auto search_bin_v2_singleMuCR_Stop0l_evtWeight_h=new TH1F("search_bin_v2_singleMuCR_Stop0l_evtWeight_h","search bin v2 Stop0l_evtWeight, single muon control region, MTb = 175",204,0.0,204.0);
	//auto search_bin_v2_singleElCR_Stop0l_evtWeight_h=new TH1F("search_bin_v2_singleElCR_Stop0l_evtWeight_h","search bin v2 Stop0l_evtWeight, single electron control region, MTb = 175",204,0.0,204.0);

	auto search_bin_v2_lowdm_h=new TH1F("search_bin_v2_lowdm_h","search bin v2 low dm, MTb = 175",53,0.0,53.0);
	auto search_bin_v2_singleMuCR_lowdm_h=new TH1F("search_bin_v2_singleMuCR_lowdm_h","search bin v2 low dm, single muon control region, MTb = 175",53,0.0,53.0);
	auto search_bin_v2_singleElCR_lowdm_h=new TH1F("search_bin_v2_singleElCR_lowdm_h","search bin v2 low dm, single electron control region, MTb = 175",53,0.0,53.0);

	auto search_bin_v2_lowdm_more_MET_h=new TH1F("search_bin_v2_lowdm_more_MET_h","search bin v2 low dm more MET bins, MTb = 175",68,0.0,68.0);
	auto search_bin_v2_singleMuCR_lowdm_more_MET_h=new TH1F("search_bin_v2_singleMuCR_lowdm_more_MET_h","search bin v2 low dm more MET bins, single muon control region, MTb = 175",68,0.0,68.0);
	auto search_bin_v2_singleElCR_lowdm_more_MET_h=new TH1F("search_bin_v2_singleElCR_lowdm_more_MET_h","search bin v2 low dm more MET bins, single electron control region, MTb = 175",68,0.0,68.0);

	auto search_bin_v2_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, MTb = 175",53,0.0,53.0);
	auto search_bin_v2_singleMuCR_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_singleMuCR_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, single muon control region, MTb = 175",53,0.0,53.0);
	auto search_bin_v2_singleElCR_lowdm_high_ISRpt_h=new TH1F("search_bin_v2_singleElCR_lowdm_high_ISRpt_h","search bin v2 low dm higher ISRpt bins, single electron control region, MTb = 175",53,0.0,53.0);

	auto search_bin_v2_highdm_h=new TH1F("search_bin_v2_highdm_h","search bin v2 high dm, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleMuCR_highdm_h=new TH1F("search_bin_v2_singleMuCR_highdm_h","search bin v2 high dm, single muon control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleElCR_highdm_h=new TH1F("search_bin_v2_singleElCR_highdm_h","search bin v2 high dm, single electron control region, MTb = 175",151,53.0,204.0);

	auto search_bin_v2_highdm_loose_bin_h=new TH1F("search_bin_v2_highdm_loose_bin_h","search bin v2 high dm loose bins, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleMuCR_highdm_loose_bin_h=new TH1F("search_bin_v2_singleMuCR_highdm_loose_bin_h","search bin v2 high dm loose bins, single muon control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_singleElCR_highdm_loose_bin_h=new TH1F("search_bin_v2_singleElCR_highdm_loose_bin_h","search bin v2 high dm loose bins, single electron control region, MTb = 175",151,53.0,204.0);
	auto search_bin_v2_QCD_CR_highdm_loose_bin_h=new TH1F("search_bin_v2_QCD_CR_highdm_loose_bin_h","search bin v2 high dm loose bins, QCD control region, MTb = 175",151,53.0,204.0);

	auto search_bin_v2_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, MTb = 175, jet pt 30",151,53.0,204.0);
	auto search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, single muon control region, MTb = 175, jet pt 30",151,53.0,204.0);
	auto search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, single electron control region, MTb = 175, jet pt 30",151,53.0,204.0);
	auto search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h=new TH1F("search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h","search bin v2 high dm loose bins, QCD control region, MTb = 175, jetpt30",151,53.0,204.0);

	auto search_bin_v2_jetpt30_h=new TH1F("search_bin_v2_jetpt30_h","search bin v2, MTb = 175, jet pt 30",204,0.0,204.0);
	auto search_bin_v2_singleMuCR_jetpt30_h=new TH1F("search_bin_v2_singleMuCR_jetpt30_h","search bin v2, single muon control region, MTb = 175, jet pt 30",204,0.0,204.0);
	auto search_bin_v2_singleElCR_jetpt30_h=new TH1F("search_bin_v2_singleElCR_jetpt30_h","search bin v2, single electron control region, MTb = 175, jet pt 30",204,0.0,204.0);

	auto search_bin_v2_Stop0l_evtWeight_jetpt30_h=new TH1F("search_bin_v2_Stop0l_evtWeight_jetpt30_h","search bin v2 with Stop0l_evtWeight, MTb = 175, jet pt 30",204,0.0,204.0);
	//auto search_bin_v2_singleMuCR_Stop0l_evtWeight_jetpt30_h=new TH1F("search_bin_v2_singleMuCR_Stop0l_evtWeight_jetpt30_h","search bin v2 Stop0l_evtWeight, single muon control region, MTb = 175, jet pt 30",204,0.0,204.0);
	//auto search_bin_v2_singleElCR_Stop0l_evtWeight_jetpt30_h=new TH1F("search_bin_v2_singleElCR_Stop0l_evtWeight_jetpt30_h","search bin v2 Stop0l_evtWeight, single electron control region, MTb = 175, jet pt 30",204,0.0,204.0);

	auto search_bin_v2_lowdm_validation_0p1_h=new TH1F("search_bin_v2_lowdm_validation_0p1_h","search bin v2 low dm validation, dPhi 0.1",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_0p15_h=new TH1F("search_bin_v2_lowdm_validation_0p15_h","search bin v2 low dm validation, dPhi 0.15",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_0p2_h=new TH1F("search_bin_v2_lowdm_validation_0p2_h","search bin v2 low dm validation, dPhi 0.2",22,0.0,22.0);
	auto search_bin_v2_lowdm_validation_h=new TH1F("search_bin_v2_lowdm_validation_h","search bin v2 low dm validation",19,0.0,19.0);
	auto search_bin_v2_highdm_validation_h=new TH1F("search_bin_v2_highdm_validation_h","search bin v2 high dm validation, MTb = 175",24,22.0,46.0);
	auto search_bin_v2_lowdm_validation_pass_trigger_h=new TH1F("search_bin_v2_lowdm_validation_pass_trigger_h","search bin v2 low dm validation, pass trigger",19,0.0,19.0);
	auto search_bin_v2_highdm_validation_pass_trigger_h=new TH1F("search_bin_v2_highdm_validation_pass_trigger_h","search bin v2 high dm validation, MTb = 175, pass trigger",24,22.0,46.0);

	auto nMuons_uc_h=new TH1F("nMuons_uc_h","number of muons, no cuts",10,0.0,10.0);
	auto nElectrons_uc_h=new TH1F("nElectrons_uc_h","number of electrons, no cuts",10,0.0,10.0);

	while(tr.getNextEvent())
	{

		if(debug) std::cout << __LINE__ << std::endl;

		if(max_events > 0 && tr.getEvtNum() > max_events) break;

		if(tr.getEvtNum() % 2000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

		//if(tr.getEvtNum() < 350000) continue;
		//if (tr.getVar<unsigned long long>("event") != 519215141) continue;

		//only take the sign from evtWeight!
		float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
		float evtWeight = 1.0;
		if (Stop0l_evtWeight < 0) evtWeight = -1.0;

		float B_SF = 1.0;
		float ISR_SF = 1.0;
		float trigger_eff = 1.0;
		float puWeight = 1.0;
		float PrefireWeight = 1.0;
		
		if(apply_SF)
		{
			B_SF = tr.getVar<float>("BTagWeight");
			trigger_eff = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline");
			puWeight = tr.getVar<float>("puWeight");
			PrefireWeight = tr.getVar<float>("PrefireWeight");

			if(apply_ISR_SF) ISR_SF = tr.getVar<float>("ISRWeight");
			evtWeight = evtWeight * B_SF * ISR_SF * trigger_eff * puWeight * PrefireWeight;
		}

		float met = tr.getVar<float>("MET_pt"); 
		float HT = tr.getVar<float>("Stop0l_HT"); 
		float mtb = tr.getVar<float>("Stop0l_Mtb"); 
		int njetspt20 = tr.getVar<int>("Stop0l_nJets");
		int nbottompt20 = tr.getVar<int>("Stop0l_nbtags");
		int ntop_merge = tr.getVar<int>("Stop0l_nTop");
		int ntop_res = tr.getVar<int>("Stop0l_nResolved");
		int nw = tr.getVar<int>("Stop0l_nW");
		int nSV = tr.getVar<int>("Stop0l_nSoftb");
		float ISRpt = tr.getVar<float>("Stop0l_ISRJetPt"); 
		float bottompt_scalar_sum = tr.getVar<float>("Stop0l_Ptb");
		float S_met = tr.getVar<float>("Stop0l_METSig");
		bool Pass_trigger_MET = tr.getVar<bool>("Pass_trigger_MET");

		bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
		bool Pass_JetID = tr.getVar<bool>("Pass_JetID");
		bool Pass_CaloMETRatio = tr.getVar<bool>("Pass_CaloMETRatio");
		Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio; 

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
						jet_pt_uc_h->Fill(Jet_pt.at(i));
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

		int nElectrons = 0, nMuons = 0;
		std::vector<unsigned char> Electron_Stop0l = tr.getVec<unsigned char>("Electron_Stop0l");
		for (int i = 0; i < Electron_Stop0l.size(); i++)
		{if (Electron_Stop0l.at(i)) nElectrons++;}
		std::vector<unsigned char> Muon_Stop0l = tr.getVec<unsigned char>("Muon_Stop0l");
		for (int i = 0; i < Muon_Stop0l.size(); i++)
		{if (Muon_Stop0l.at(i)) nMuons++;}

		bool Pass_lowDM_ISR_veto = (tr.getVar<bool>("Pass_Baseline") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt < 300);
		bool Pass_dPhi_QCD = (min_jet_dPhi < 0.1);
		bool Pass_dPhi_0p15 = (min_jet_dPhi < 0.15);
		bool Pass_dPhi_0p2 = (min_jet_dPhi < 0.2);
		bool Pass_lowDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt > 200);
		bool Pass_lowDM_mid_dPhi = (Pass_lowDM_no_dPhi && (!Pass_dPhi_QCD) && (!tr.getVar<bool>("Pass_dPhiMET")));
		bool Pass_lowDM_mid_dPhi_0p15 = (Pass_lowDM_no_dPhi && (!Pass_dPhi_0p15) && (!tr.getVar<bool>("Pass_dPhiMET")));
		bool Pass_lowDM_mid_dPhi_0p2 = (Pass_lowDM_no_dPhi && (!Pass_dPhi_0p2) && (!tr.getVar<bool>("Pass_dPhiMET")));
		//bool Pass_lowDM_pt30 = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && njetspt30 >=2 && tr.getVar<bool>("Pass_MET") && HTpt30 >= 300 && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && met/sqrt(HTpt30) > 10 && ISRpt > 200 && pass_dPhi_jetpt30);

		bool Pass_highDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njetspt20 >= 5 && nbottompt20 >= 1);
		bool Pass_highDM_mid_dPhi = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));

		int SBv2_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SBv2_highdm_index_175 = SBv2_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		int SBv2_highdm_index_175_jetpt30 = SBv2_highdm(175, mtb, njetspt30, ntop_merge, nw, ntop_res, nbottompt20, met, HTpt30);
		std::vector<int> SBv2_highdm_loose_bin(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		std::vector<int> SBv2_highdm_loose_bin_index_175 = SBv2_highdm_loose_bin(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		std::vector<int> SBv2_highdm_loose_bin_index_175_jetpt30 = SBv2_highdm_loose_bin(175, mtb, njetspt30, ntop_merge, nw, ntop_res, nbottompt20, met, HTpt30);
		int SBv3_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met);
		int SBv3_highdm_index = SBv3_highdm(mtb, njetspt20, nbottompt20, ntop_merge, nw, ntop_res, HT, met);
		int SBv2_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_index = SBv2_lowdm(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_index_jetpt30 = SBv2_lowdm(njetspt30, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_more_MET(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_more_MET_index = SBv2_lowdm_more_MET(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_high_ISRpt(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_high_ISRpt_index = SBv2_lowdm_high_ISRpt(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);

		int SBv2_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met);
		int SBv2_highdm_validation_index = SBv2_highdm_validation(mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met);
		int SBv2_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_validation_index = SBv2_lowdm_validation(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met);
		int SBv2_lowdm_validation_high_MET_index = SBv2_lowdm_validation_high_MET(nbottompt20, nSV, ISRpt, met);
		int SBv2_lowdm_validation_mid_dPhi(int njets, int nb, int nSV, float ISRpt, float met);
		int SBv2_lowdm_validation_mid_dPhi_index = SBv2_lowdm_validation_mid_dPhi(njetspt20, nbottompt20, nSV, ISRpt, met);

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
                loose_baseline_cutflow_h->Fill(0.);
                if(Pass_EventFilter)loose_baseline_cutflow_h->Fill(1.);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto"))loose_baseline_cutflow_h->Fill(2.);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20"))loose_baseline_cutflow_h->Fill(3.);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET"))loose_baseline_cutflow_h->Fill(4.);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT"))loose_baseline_cutflow_h->Fill(5.);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && tr.getVar<bool>("Pass_dPhiMETLowDM"))loose_baseline_cutflow_h->Fill(6.);

		// no cut
		eff_h->Fill(0.);
                ISR_SF_uc_h->Fill(ISR_SF);
                B_SF_uc_h->Fill(B_SF);
                Trigger_SF_uc_h->Fill(trigger_eff);
                PU_SF_uc_h->Fill(puWeight);
                preFire_SF_uc_h->Fill(PrefireWeight);

		met_uc_h->Fill(met,evtWeight);
		HT_uc_h->Fill(HT,evtWeight);
		mtb_uc_h->Fill(mtb,evtWeight);
		njetspt20_uc_h->Fill(njetspt20,evtWeight);
		nbottompt20_uc_h->Fill(nbottompt20,evtWeight);
		ntop_merge_uc_h->Fill(ntop_merge,evtWeight);
		ntop_res_uc_h->Fill(ntop_res,evtWeight);
		nw_uc_h->Fill(nw,evtWeight);
		nMuons_uc_h->Fill(nMuons,evtWeight);
		nElectrons_uc_h->Fill(nElectrons,evtWeight);

		if(tr.getVar<bool>("Pass_Baseline")) 
		{
			eff_h->Fill(1.);
                        ISR_SF_h->Fill(ISR_SF);
                        B_SF_h->Fill(B_SF);
                	Trigger_SF_h->Fill(trigger_eff);
                	PU_SF_h->Fill(puWeight);
                	preFire_SF_h->Fill(PrefireWeight);

			met_h->Fill(met,evtWeight);
			HT_h->Fill(HT,evtWeight);
			mtb_h->Fill(mtb,evtWeight);
			njetspt20_h->Fill(njetspt20,evtWeight);
			nbottompt20_h->Fill(nbottompt20,evtWeight);
			ntop_merge_h->Fill(ntop_merge,evtWeight);
			ntop_res_h->Fill(ntop_res,evtWeight);
			nw_h->Fill(nw,evtWeight);
		}

		if(tr.getVar<bool>("Pass_lowDM"))
		{
			eff_h->Fill(2.);
			met_lowdm_h->Fill(met,evtWeight);
			ISRpt_lowdm_h->Fill(ISRpt,evtWeight);
			bottompt_scalar_sum_lowdm_h->Fill(bottompt_scalar_sum,evtWeight);
			njetspt20_lowdm_h->Fill(njetspt20,evtWeight);
		
			if(SBv2_lowdm_index != -1) search_bin_v2_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v2_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv2_lowdm_index != -1) search_bin_v3_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v3_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv2_lowdm_index != -1) search_bin_v2_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
			if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);
			if(SBv2_lowdm_validation_index != -1) 
			{
				search_bin_v2_lowdm_validation_h->Fill(SBv2_lowdm_validation_index,evtWeight);
				if(Pass_trigger_MET) search_bin_v2_lowdm_validation_pass_trigger_h->Fill(SBv2_lowdm_validation_index,evtWeight);
			}

			if(Pass_lowDM_pt30)
			{
				njetspt30_lowdm_h->Fill(njetspt30,evtWeight);
				if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
				if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
			}
		}

		if(tr.getVar<bool>("Pass_highDM"))
		{
			eff_h->Fill(3.);
			met_highdm_h->Fill(met,evtWeight);
		
			if(SBv2_highdm_index_175 != -1) search_bin_v2_h->Fill(SBv2_highdm_index_175,evtWeight);
			if(SBv2_highdm_index_175 != -1) search_bin_v2_Stop0l_evtWeight_h->Fill(SBv2_highdm_index_175,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv3_highdm_index != -1) search_bin_v3_h->Fill(SBv3_highdm_index,evtWeight);
			if(SBv3_highdm_index != -1) search_bin_v3_Stop0l_evtWeight_h->Fill(SBv3_highdm_index,evtWeight * fabs(Stop0l_evtWeight));
			if(SBv2_highdm_index_175 != -1) search_bin_v2_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);

			//if(SBv2_highdm_index_175 == 203) std::cout << "evtWeight = evtWeight * B_SF * ISR_SF * trigger_eff * puWeight * PrefireWeight" << evtWeight << std::endl;
			//if(SBv2_highdm_index_175 != -1 && SBv3_highdm_index == -1) std::cout << "more v2 bin " << SBv2_highdm_index_175 << " ht " << HT << std::endl;
			//if(SBv2_highdm_index_175 == -1 && SBv3_highdm_index != -1) std::cout << "more v3 bin " << SBv3_highdm_index << " Njet " << njetspt20 << std::endl;
	
			for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

			if(Pass_highDM_pt30)
			{
				if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
				if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight * fabs(Stop0l_evtWeight));
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
			}
		}

		if(tr.getVar<bool>("Pass_LLCR_lowDM"))
		{
			if(nMuons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v3_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,Stop0l_evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleMuCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleMuCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);

				if(Pass_lowDM_pt30)
				{
					if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleMuCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
					//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleMuCR_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,Stop0l_evtWeight);
				}
			}
			if(nElectrons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v3_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				//if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_Stop0l_evtWeight_h->Fill(SBv2_lowdm_index,Stop0l_evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleElCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleElCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);

				if(Pass_lowDM_pt30)
				{
					if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleElCR_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,evtWeight);
					//if(SBv2_lowdm_index_jetpt30 != -1) search_bin_v2_singleElCR_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_lowdm_index_jetpt30,Stop0l_evtWeight);
				}
			}
		}

		if(tr.getVar<bool>("Pass_LLCR_highDM"))
		{
			if(nMuons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv3_highdm_index != -1) search_bin_v3_singleMuCR_h->Fill(SBv3_highdm_index,evtWeight);
				//if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_Stop0l_evtWeight_h->Fill(SBv2_highdm_index_175,Stop0l_evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleMuCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

				if(Pass_highDM_pt30)
				{
					if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleMuCR_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
					for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
					//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleMuCR_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,Stop0l_evtWeight);
				}
			}
			if(nElectrons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv3_highdm_index != -1) search_bin_v3_singleElCR_h->Fill(SBv3_highdm_index,evtWeight);
				//if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_Stop0l_evtWeight_h->Fill(SBv2_highdm_index_175,Stop0l_evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleElCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);

				if(Pass_highDM_pt30)
				{
					if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleElCR_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,evtWeight);
					for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
					//if(SBv2_highdm_index_175_jetpt30 != -1) search_bin_v2_singleElCR_Stop0l_evtWeight_jetpt30_h->Fill(SBv2_highdm_index_175_jetpt30,Stop0l_evtWeight);
				}
			}
		}

		if(tr.getVar<bool>("Pass_QCDCR_lowDM"))
		{
			if(SBv2_lowdm_index != -1) search_bin_v2_QCD_CR_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v3_QCD_CR_h->Fill(SBv2_lowdm_index,evtWeight);
		}

		if(tr.getVar<bool>("Pass_QCDCR_highDM"))
		{
			if(SBv2_highdm_index_175 != -1) search_bin_v2_QCD_CR_h->Fill(SBv2_highdm_index_175,evtWeight);
			if(SBv3_highdm_index != -1) search_bin_v3_QCD_CR_h->Fill(SBv3_highdm_index,evtWeight);
			for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_QCD_CR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);
			if(Pass_highDM_QCD_pt30)
			{
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175_jetpt30.size(); i++) search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h->Fill(SBv2_highdm_loose_bin_index_175_jetpt30.at(i),evtWeight);
			}
		}

		if(Pass_lowDM_no_dPhi) 
		{
			min_jet_dPhi_lowdm_h->Fill(min_jet_dPhi,evtWeight);
		}
		if(Pass_lowDM_mid_dPhi)
		{
			met_lowdm_mid_dPhi_h->Fill(met,evtWeight); 
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p1_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
		}
		if(Pass_lowDM_mid_dPhi_0p15)
		{
			//std::cout << tr.getEvtNum() << std::endl;
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p15_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
			if(SBv2_lowdm_validation_high_MET_index != -1)
			{
				search_bin_v2_lowdm_validation_h->Fill(SBv2_lowdm_validation_high_MET_index,evtWeight);
				if(Pass_trigger_MET)
				{
					search_bin_v2_lowdm_validation_pass_trigger_h->Fill(SBv2_lowdm_validation_high_MET_index,evtWeight);
					//if(SBv2_lowdm_validation_high_MET_index == 15) std::cout << "find bin 15, event " << tr.getEvtNum() << " event number " << tr.getVar<unsigned long long>("event") << std::endl; 
					//if(SBv2_lowdm_validation_high_MET_index == 17) std::cout << "find bin 17, event " << tr.getEvtNum() << " event number " << tr.getVar<unsigned long long>("event") << std::endl; 
				}
			}
		}
		if(Pass_lowDM_mid_dPhi_0p2)
		{
			if(SBv2_lowdm_validation_mid_dPhi_index != -1) search_bin_v2_lowdm_validation_0p2_h->Fill(SBv2_lowdm_validation_mid_dPhi_index,evtWeight);
		}
		if(Pass_lowDM_ISR_veto) met_lowdm_ISR_veto_h->Fill(met,evtWeight); 
		if(Pass_highDM_no_dPhi) min_jet_dPhi_highdm_h->Fill(min_jet_dPhi,evtWeight); 
		if(Pass_highDM_mid_dPhi)
		{
			met_highdm_mid_dPhi_h->Fill(met,evtWeight); 
			if(SBv2_highdm_validation_index != -1)
			{
				search_bin_v2_highdm_validation_h->Fill(SBv2_highdm_validation_index,evtWeight);
				if(Pass_trigger_MET) search_bin_v2_highdm_validation_pass_trigger_h->Fill(SBv2_highdm_validation_index,evtWeight);
			}
		}

	}// End event loop

	TFile out_file(outputfile,"RECREATE");

	loose_baseline_cutflow_h->Write();
        ISR_SF_uc_h->Write();
        B_SF_uc_h->Write();
        Trigger_SF_uc_h->Write();
        PU_SF_uc_h->Write();
        preFire_SF_uc_h->Write();
        ISR_SF_h->Write();
        B_SF_h->Write();
        Trigger_SF_h->Write();
        PU_SF_h->Write();
        preFire_SF_h->Write();

	met_uc_h->Write();
	met_h->Write();
	met_lowdm_h->Write();
	met_lowdm_ISR_veto_h->Write();
	met_lowdm_mid_dPhi_h->Write();
	met_highdm_h->Write();
	met_highdm_mid_dPhi_h->Write();
	HT_uc_h->Write();
	HT_h->Write();
	mtb_uc_h->Write();
	mtb_h->Write();
	njetspt20_uc_h->Write();
	njetspt20_h->Write();
	njetspt20_lowdm_h->Write();
	njetspt30_lowdm_h->Write();
	jet_pt_uc_h->Write();
	nbottompt20_uc_h->Write();
	nbottompt20_h->Write();
	ntop_merge_uc_h->Write();
	ntop_merge_h->Write();
	ntop_res_uc_h->Write();
	ntop_res_h->Write();
	nw_uc_h->Write();
	nw_h->Write();

	nMuons_uc_h->Write();
	nElectrons_uc_h->Write();
	ISRpt_lowdm_h->Write();
	bottompt_scalar_sum_lowdm_h->Write();
	min_jet_dPhi_lowdm_h->Write();
	min_jet_dPhi_highdm_h->Write();

	search_bin_v2_h->Write();
	search_bin_v2_singleMuCR_h->Write();
	search_bin_v2_singleElCR_h->Write();
	search_bin_v2_Stop0l_evtWeight_h->Write();
	search_bin_v2_QCD_CR_h->Write();
	search_bin_v3_h->Write();
	search_bin_v3_singleMuCR_h->Write();
	search_bin_v3_singleElCR_h->Write();
	search_bin_v3_Stop0l_evtWeight_h->Write();
	search_bin_v3_QCD_CR_h->Write();
	//search_bin_v2_singleMuCR_Stop0l_evtWeight_h->Write();
	//search_bin_v2_singleElCR_Stop0l_evtWeight_h->Write();
	search_bin_v2_lowdm_h->Write();
	search_bin_v2_singleMuCR_lowdm_h->Write();
	search_bin_v2_singleElCR_lowdm_h->Write();
	search_bin_v2_lowdm_more_MET_h->Write();
	search_bin_v2_singleMuCR_lowdm_more_MET_h->Write();
	search_bin_v2_singleElCR_lowdm_more_MET_h->Write();
	search_bin_v2_lowdm_high_ISRpt_h->Write();
	search_bin_v2_singleMuCR_lowdm_high_ISRpt_h->Write();
	search_bin_v2_singleElCR_lowdm_high_ISRpt_h->Write();
	search_bin_v2_highdm_h->Write();
	search_bin_v2_singleMuCR_highdm_h->Write();
	search_bin_v2_singleElCR_highdm_h->Write();

	search_bin_v2_highdm_loose_bin_h->Write();
	search_bin_v2_singleMuCR_highdm_loose_bin_h->Write();
	search_bin_v2_singleElCR_highdm_loose_bin_h->Write();
	search_bin_v2_QCD_CR_highdm_loose_bin_h->Write();
	search_bin_v2_highdm_loose_bin_jetpt30_h->Write();
	search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h->Write();
	search_bin_v2_singleElCR_highdm_loose_bin_jetpt30_h->Write();
	search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h->Write();

	search_bin_v2_jetpt30_h->Write();
	search_bin_v2_singleMuCR_jetpt30_h->Write();
	search_bin_v2_singleElCR_jetpt30_h->Write();
	search_bin_v2_Stop0l_evtWeight_jetpt30_h->Write();
	//search_bin_v2_singleMuCR_Stop0l_evtWeight_jetpt30_h->Write();
	//search_bin_v2_singleElCR_Stop0l_evtWeight_jetpt30_h->Write();
	search_bin_v2_lowdm_validation_0p1_h->Write();
	search_bin_v2_lowdm_validation_0p15_h->Write();
	search_bin_v2_lowdm_validation_0p2_h->Write();
	search_bin_v2_lowdm_validation_h->Write();
	search_bin_v2_highdm_validation_h->Write();
	search_bin_v2_lowdm_validation_pass_trigger_h->Write();
	search_bin_v2_highdm_validation_pass_trigger_h->Write();

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	eff_h->Write();


	out_file.Close();

	ch->Reset();

	return 0;

} // End Main
