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

int main(int argc, char* argv[]){

	const bool debug = false;
	const bool do_ISR_BSF = true;

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

	if(debug) std::cout << __LINE__ << std::endl;

	NTupleReader tr(ch);
	tr.setReThrow(false);

	if(debug) std::cout << __LINE__ << std::endl;

	auto loose_baseline_cutflow_h=new TH1F("loose_baseline_cutflow_h","0: all 1: PassEventFilter 2: PassLeptonVeto 3: PassNjets 4: PassMET 5: PassHT 6: PassdPhiLowDM",7,0.0,7.0);
	auto eff_h=new TH1F("eff_h","0: all. 1: loose baseline. 2: low dm. 3: high dm",4,0.0,4.0);
        auto ISR_SF_uc_h=new TH1F("ISR_SF_uc_h","ISR SF, no cuts",100,0.0,2.0);
        auto B_SF_uc_h=new TH1F("B_SF_uc_h","B tagging SF, no cuts",100,0.0,2.0);
        auto ISR_SF_h=new TH1F("ISR_SF_h","ISR SF, loose baseline",100,0.0,2.0);
        auto B_SF_h=new TH1F("B_SF_h","B tagging SF, loose baseline",100,0.0,2.0);

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
	auto J1dPhi_lowdm_h=new TH1F("J1dPhi_lowdm_h","Jet 1 dPhi, low dm",80,0.0,3.2);
	auto J2dPhi_lowdm_h=new TH1F("J2dPhi_lowdm_h","Jet 2 dPhi, low dm",80,0.0,3.2);
	auto J3dPhi_lowdm_h=new TH1F("J3dPhi_lowdm_h","Jet 3 dPhi, low dm",80,0.0,3.2);

	auto search_bin_v2_h=new TH1F("search_bin_v2_h","search bin v2, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleMuCR_h=new TH1F("search_bin_v2_singleMuCR_h","search bin v2, single muon control region, MTb = 175",204,0.0,204.0);
	auto search_bin_v2_singleElCR_h=new TH1F("search_bin_v2_singleElCR_h","search bin v2, single electron control region, MTb = 175",204,0.0,204.0);

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

	auto nMuons_uc_h=new TH1F("nMuons_uc_h","number of muons, no cuts",10,0.0,10.0);
	auto nElectrons_uc_h=new TH1F("nElectrons_uc_h","number of electrons, no cuts",10,0.0,10.0);

	while(tr.getNextEvent())
	{

		if(debug) std::cout << __LINE__ << std::endl;

		if(max_events > 0 && tr.getEvtNum() > max_events) break;

		if(tr.getEvtNum() % 2000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

		//only take the sign from evtWeight!
		float evtWeight = 1.0;
		if (tr.getVar<float>("Stop0l_evtWeight") < 0) evtWeight = -1.0;

		float B_SF = tr.getVar<float>("BTagWeight");
		float ISR_SF = tr.getVar<float>("ISRWeight");
		
		if(do_ISR_BSF) evtWeight = evtWeight * B_SF * ISR_SF;

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
		bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
		Pass_EventFilter = true; 

		std::vector<unsigned char> Jet_Stop0l = tr.getVec<unsigned char>("Jet_Stop0l");
		std::vector<float> Jet_dPhiMET = tr.getVec<float>("Jet_dPhiMET");
		std::vector<float> jet_dPhi;
		for (int i=0; i < Jet_dPhiMET.size(); i++)
		{if (Jet_Stop0l.at(i)) jet_dPhi.push_back(Jet_dPhiMET.at(i));}
		float min_jet_dPhi = 0;
		if(jet_dPhi.size() == 2) min_jet_dPhi = std::min(jet_dPhi.at(0), jet_dPhi.at(1));
		if(jet_dPhi.size() >= 3) min_jet_dPhi = std::min(std::min(jet_dPhi.at(0), jet_dPhi.at(1)), jet_dPhi.at(2));

		int nElectrons = 0, nMuons = 0;
		std::vector<unsigned char> Electron_Stop0l = tr.getVec<unsigned char>("Electron_Stop0l");
		for (int i = 0; i < Electron_Stop0l.size(); i++)
		{if (Electron_Stop0l.at(i)) nElectrons++;}
		std::vector<unsigned char> Muon_Stop0l = tr.getVec<unsigned char>("Muon_Stop0l");
		for (int i = 0; i < Muon_Stop0l.size(); i++)
		{if (Muon_Stop0l.at(i)) nMuons++;}

		bool Pass_lowDM_ISR_veto = (tr.getVar<bool>("Pass_Baseline") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt < 300);
		bool Pass_dPhi_QCD = (min_jet_dPhi < 0.1);
		bool Pass_lowDM_no_dPhi = (tr.getVar<bool>("Pass_EventFilter") && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt > 200);
		bool Pass_lowDM_mid_dPhi = (Pass_lowDM_no_dPhi && (!Pass_dPhi_QCD) && (!tr.getVar<bool>("Pass_dPhiMET")));

		bool Pass_highDM_no_dPhi = (tr.getVar<bool>("Pass_EventFilter") && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njetspt20 >= 5 && nbottompt20 >= 1);
		bool Pass_highDM_mid_dPhi = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));

		int SBv2_highdm(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		int SBv2_highdm_index_175 = SBv2_highdm(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		std::vector<int> SBv2_highdm_loose_bin(float mtb_cut, float mtb, int njets, int ntop, int nw, int nres, int nb, float met, float ht);
		std::vector<int> SBv2_highdm_loose_bin_index_175 = SBv2_highdm_loose_bin(175, mtb, njetspt20, ntop_merge, nw, ntop_res, nbottompt20, met, HT);
		int SBv2_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_index = SBv2_lowdm(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_more_MET(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_more_MET_index = SBv2_lowdm_more_MET(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);
		int SBv2_lowdm_high_ISRpt(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met);
		int SBv2_lowdm_high_ISRpt_index = SBv2_lowdm_high_ISRpt(njetspt20, nbottompt20, nSV, ISRpt, bottompt_scalar_sum, met);

                // cutflow
                loose_baseline_cutflow_h->Fill(0);
                if(Pass_EventFilter)loose_baseline_cutflow_h->Fill(1);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto"))loose_baseline_cutflow_h->Fill(2);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20"))loose_baseline_cutflow_h->Fill(3);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET"))loose_baseline_cutflow_h->Fill(4);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT"))loose_baseline_cutflow_h->Fill(5);
                if(Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && tr.getVar<bool>("Pass_dPhiMETLowDM"))loose_baseline_cutflow_h->Fill(6);

		// no cut
		eff_h->Fill(0., evtWeight);
                ISR_SF_uc_h->Fill(ISR_SF);
                B_SF_uc_h->Fill(B_SF);

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
			eff_h->Fill(1., evtWeight);
                        ISR_SF_h->Fill(ISR_SF);
                        B_SF_h->Fill(B_SF);

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
			eff_h->Fill(2., evtWeight);
			met_lowdm_h->Fill(met,evtWeight);
			ISRpt_lowdm_h->Fill(ISRpt,evtWeight);
			bottompt_scalar_sum_lowdm_h->Fill(bottompt_scalar_sum,evtWeight);
		
			if(SBv2_lowdm_index != -1) search_bin_v2_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_index != -1) search_bin_v2_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
			if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
			if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);
		}

		if(tr.getVar<bool>("Pass_highDM"))
		{
			eff_h->Fill(3., evtWeight);
			met_highdm_h->Fill(met,evtWeight);
		
			if(SBv2_highdm_index_175 != -1) search_bin_v2_h->Fill(SBv2_highdm_index_175,evtWeight);
			if(SBv2_highdm_index_175 != -1) search_bin_v2_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
			for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);
		}

		if(tr.getVar<bool>("Pass_LLCR_lowDM"))
		{
			if(nMuons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v2_singleMuCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleMuCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleMuCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);
			}
			if(nElectrons == 1)
			{
				if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_index != -1) search_bin_v2_singleElCR_lowdm_h->Fill(SBv2_lowdm_index,evtWeight);
				if(SBv2_lowdm_more_MET_index != -1) search_bin_v2_singleElCR_lowdm_more_MET_h->Fill(SBv2_lowdm_more_MET_index,evtWeight);
				if(SBv2_lowdm_high_ISRpt_index != -1) search_bin_v2_singleElCR_lowdm_high_ISRpt_h->Fill(SBv2_lowdm_high_ISRpt_index,evtWeight);
			}
		}

		if(tr.getVar<bool>("Pass_LLCR_highDM"))
		{
			if(nMuons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleMuCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleMuCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);
			}
			if(nElectrons == 1)
			{
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_h->Fill(SBv2_highdm_index_175,evtWeight);
				if(SBv2_highdm_index_175 != -1) search_bin_v2_singleElCR_highdm_h->Fill(SBv2_highdm_index_175,evtWeight);
				for (int i = 0; i < SBv2_highdm_loose_bin_index_175.size(); i++) search_bin_v2_singleElCR_highdm_loose_bin_h->Fill(SBv2_highdm_loose_bin_index_175.at(i),evtWeight);
			}
		}

		if(Pass_lowDM_no_dPhi) 
		{
			min_jet_dPhi_lowdm_h->Fill(min_jet_dPhi,evtWeight);
			J1dPhi_lowdm_h->Fill(jet_dPhi.at(0),evtWeight); 
			J2dPhi_lowdm_h->Fill(jet_dPhi.at(1),evtWeight); 
			if(jet_dPhi.size() >= 3) J3dPhi_lowdm_h->Fill(jet_dPhi.at(2),evtWeight);
		}
		if(Pass_lowDM_mid_dPhi) met_lowdm_mid_dPhi_h->Fill(met,evtWeight); 
		if(Pass_lowDM_ISR_veto) met_lowdm_ISR_veto_h->Fill(met,evtWeight); 
		if(Pass_highDM_no_dPhi) min_jet_dPhi_highdm_h->Fill(min_jet_dPhi,evtWeight); 
		if(Pass_highDM_mid_dPhi) met_highdm_mid_dPhi_h->Fill(met,evtWeight); 

	}// End event loop

	TFile out_file(outputfile,"RECREATE");

	loose_baseline_cutflow_h->Write();
        ISR_SF_uc_h->Write();
        B_SF_uc_h->Write();
        ISR_SF_h->Write();
        B_SF_h->Write();

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
	J1dPhi_lowdm_h->Write();
	J2dPhi_lowdm_h->Write();
	J3dPhi_lowdm_h->Write();
	min_jet_dPhi_lowdm_h->Write();
	min_jet_dPhi_highdm_h->Write();

	search_bin_v2_h->Write();
	search_bin_v2_singleMuCR_h->Write();
	search_bin_v2_singleElCR_h->Write();
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

	out_file.mkdir("Baseline_Only");
	out_file.cd("Baseline_Only");

	eff_h->Write();


	out_file.Close();

	ch->Reset();

	return 0;

} // End Main
