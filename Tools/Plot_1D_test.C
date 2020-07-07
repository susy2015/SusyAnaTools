#include "CrossSection.C"
#include "Plot_1D_AUX.C"
int Plot_1D_test()
{
	//gROOT->ForceStyle();

	double lumi = 36;
	lumi = 137;
	TString year = "_2017";
        int bin_test = 8;

	bool plot_log = false;
	//plot_log = true;
	bool plot_sig_pad = false;
	bool plot_sig = true;
	bool plot_BG = true;
	bool use_low_stat_sig = false;
	//use_low_stat_sig = true;
	bool sig_scale_to_BG = true;
	bool use_original_title = false;
	bool use_smear_QCD = true;

	TString result_path = "results/SMS_";
	TString full_or_fast = "_fullsim"; 
	full_or_fast = "_fastsim"; 

	//TString signal_name = "T2fbd_test";
	//TString signal_name = "T2tt_test";
	//TString signal_name = "T2tt_and_T1tttt_high";
	//TString signal_name = "T2tt_only";
	//TString signal_name = "T2bt_and_T2bw";
	//TString signal_name = "T1tttt_and_T5ttcc";
	TString signal_name = "T2fbd_and_T2bwC";
	//TString signal_name = "T2cc_and_T2tt";

	bool plot_MTb = false;
	bool plot_SB_v2 = false;
	bool plot_SB_v3 = false;
	bool plot_SB_v4 = false;
	bool plot_SB_v4_jetpt30 = false;
	bool plot_SB_v2_highdm_validation = false;
	bool plot_SB_v2_lowdm_validation = false;
	bool plot_SB_v2_lowdm_validation_0p1 = false;
	bool plot_SB_v2_lowdm_validation_0p15 = false;
	bool plot_nbottompt20 = false;
	bool plot_nbottompt20_lowdm = false;
	bool plot_nbottompt30 = false;
	bool plot_ntop = false;
	bool plot_ntop_merge = false;
	bool plot_ntop_w = false;
	bool plot_ntop_res = false;
	bool plot_nw = false;
	bool plot_njetspt20 = false;
	bool plot_njetspt20_lowdm = false;
	bool plot_njetspt30 = false;
	bool plot_nSV_lowdm = false;
	bool plot_HT = false;
	bool plot_MET = false;
	bool plot_MET_lowdm = false;
	bool plot_MET_lowdm_mid_dPhi = false;
	bool plot_MET_lowdm_ISR_veto = false;
	bool plot_MET_highdm = false;
	bool plot_MET_highdm_mid_dPhi = false;
	bool plot_MET_binning_study = false;
	bool plot_S_met_lowdm = true;
	bool plot_ISR_pt_lowdm = false;
	bool plot_bottom_pt_lowdm = false;
	bool plot_min_jet_dPhi_highdm = false;
	bool plot_min_jet_dPhi_highdm_zoom = false;
	bool plot_min_jet_dPhi_lowdm = false;
	bool plot_min_jet_dPhi_lowdm_zoom = false;
	bool plot_J1dPhi_lowdm = false;
	bool plot_J2dPhi_lowdm = false;
	bool plot_J3dPhi_lowdm = false;
	bool plot_jpt_eta_2p5_3p0 = false;
	bool plot_jpt_eta_2p5_3p0_no_HT = false;

	float xmin =0, xmax = 800, ymin = 0, ymax = -1, sig_ymax = 1;
	int hs_tot = 1, hs_tot_plot = 1;
	float padup_height = 0.3, padup_margin = 0, leg_height = 0.5, leg_size = 0.04;
	if(!plot_sig_pad) {padup_height = 0; padup_margin = 0.1, leg_height = 0.6, leg_size = 0.03;}
	if(plot_log) ymin = 0.01;

	TString title = "MET";
	TString var = "met_h";
	TString var_weight = "";
	TString folder = "";

	int rebin = 1;

	if (plot_MET_binning_study)
	{	
		//var = "mt2_h";
		title = "";
		var = "met_1b_011_h";
		folder = "";
		//rebin = 5;
		xmin = 200;
		xmax = 800;
		ymax = -1;
	}


	if (plot_MTb)
	{
		title = "MTb [GeV]";
		var = "mtb_h";
		folder = "";
		rebin = 1;
		xmin = 10;
		xmax = 350;
		ymax = 10000;
		sig_ymax = 0.3;
	}

	if (plot_SB_v2)
	{
		title = "search bin v2";
		var = "search_bin_v2_h";
		folder = "";
		rebin = 1;
		xmax = 204;
		ymax = 1000000;
		sig_ymax = 2;
	}

	if (plot_SB_v3)
	{
		title = "search bin v3";
		var = "search_bin_v3_h";
		var_weight = "search_bin_v3_Stop0l_evtWeight_h";
		folder = "";
		rebin = 1;
		xmax = 204;
		ymax = 0.1;
		ymin = -0.1;
		sig_ymax = 2;
	}

	if (plot_SB_v4)
	{
		title = "search bin v4";
		var = "search_bin_v4_h";
		var_weight = "search_bin_v4_Stop0l_evtWeight_h";
		folder = "";
		rebin = 1;
		xmax = 183;
		ymax = 100000000;
		ymin = 0.1;
		sig_ymax = 2;
	}

	if (plot_SB_v4_jetpt30)
	{
		title = "search bin v4(jet pt 30)";
		var = "search_bin_v4_jetpt30_h";
		var_weight = "search_bin_v4_Stop0l_evtWeight_jetpt30_h";
		folder = "";
		rebin = 1;
		xmax = 183;
		ymax = 100000000;
		ymin = 0.1;
		sig_ymax = 2;
	}

	if (plot_SB_v2_highdm_validation)
	{
		title = "SB v2 highdm validation";
		var = "search_bin_v2_highdm_validation_h";
		folder = "";
		rebin = 1;
		xmax = 46;
		ymax = 1000000000;
		sig_ymax = 1;
	}

	if (plot_SB_v2_lowdm_validation)
	{
		title = "SB v2 lowdm validation";
		var = "search_bin_v2_lowdm_validation_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 1000000000;
		sig_ymax = 1;
	}

	if (plot_SB_v2_lowdm_validation_0p1)
	{
		title = "SB v2 lowdm validation, dPhi 0.1";
		var = "search_bin_v2_lowdm_validation_0p1_h";
		folder = "";
		rebin = 1;
		xmax = 22;
		ymax = 8000;
		sig_ymax = 1;
	}

	if (plot_SB_v2_lowdm_validation_0p15)
	{
		title = "SB v2 lowdm validation, dPhi 0.15";
		var = "search_bin_v2_lowdm_validation_0p15_h";
		folder = "";
		rebin = 1;
		xmax = 22;
		ymax = 800;
		sig_ymax = 1;
	}

	if (plot_nbottompt20)
	{
		title = "number of b jets (MTb > 175 GeV)";
		var = "nbottompt20_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 100000;
		sig_ymax = 0.3;
	}

	if (plot_nbottompt20_lowdm)
	{
		title = "number of b jets, low dm";
		var = "nbottompt20_lowdm_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 1000000000;
		sig_ymax = 0.6;
	}

	if (plot_nbottompt30)
	{
		title = "number of b jets (MTb > 175 GeV)";
		var = "nbottompt30_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 10000000;
		ymax = 100000;
		sig_ymax = 0.3;
	}

	if (plot_ntop)
	{
		title = "ntop, loose baseline";
		var = "ntop_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 1000000;
		ymax = 30000;
		sig_ymax = 3;
	}

	if (plot_ntop_merge)
	{
		title = "number of fully merged Top (MTb > 175 GeV)";
		var = "ntop_merge_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 1;
		ymax = 10000000;
		sig_ymax = 3;
	}

	if (plot_ntop_w)
	{
		title = "number of partialy merged top when MTb > 175";
		var = "ntop_w_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 1;
		//ymax = 35000;
		sig_ymax = 3;
	}

	if (plot_ntop_res)
	{
		title = "number of resolved Top (MTb > 175 GeV)";
		var = "ntop_res_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 1;
		ymax = 10000000;
		sig_ymax = 0.6;
	}

	if (plot_nw)
	{
		title = "number of W (MTb > 175 GeV)";
		var = "nw_175_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 1000000;
		sig_ymax = 0.3;
	}

	if (plot_njetspt20)
	{
		title = "number of jets (pt20) loose baseline";
		var = "njetspt20_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 350000;
		sig_ymax = 0.15;
	}

	if (plot_njetspt20_lowdm)
	{
		title = "number of jets, low dm";
		var = "njetspt20_lowdm_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 300000;
		sig_ymax = 0.3;
	}

	if (plot_njetspt30)
	{
		title = "number of jets (MTb > 175 GeV)";
		var = "njetspt30_175_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 50000;
		sig_ymax = 0.6;
	}

	if (plot_nSV_lowdm)
	{
		title = "number of soft-b jets, low dm";
		var = "nSV_lowdm_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 100000000;
		sig_ymax = 0.5;
	}

	if (plot_HT)
	{
		title = "HT (GeV) when MTb > 175";
		var = "HT_175_h";
		folder = "";
		rebin = 5;
		xmax = 3200;
		ymax = 10000;
	}

	if (plot_MET)
	{
		title = "MET [GeV] (MTb > 175 GeV)";
		var = "met_175_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 40000;
		sig_ymax = 0.5;
	}

	if (plot_min_jet_dPhi_highdm)
	{
		title = "min jet dPhi, high dm";
		var = "min_jet_dPhi_highdm_h";
		folder = "";
		rebin = 4;
		xmin = 0;
		xmax = 3.2;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_min_jet_dPhi_highdm_zoom)
	{
		title = "min jet dPhi, high dm";
		var = "min_jet_dPhi_highdm_h";
		folder = "";
		rebin = 1;
		xmin = 0;
		xmax = 1;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_min_jet_dPhi_lowdm)
	{
		title = "min jet dPhi, low dm";
		var = "min_jet_dPhi_lowdm_h";
		folder = "";
		rebin = 4;
		xmin = 0;
		xmax = 3.2;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_min_jet_dPhi_lowdm_zoom)
	{
		title = "min jet dPhi, low dm";
		var = "min_jet_dPhi_lowdm_h";
		folder = "";
		rebin = 1;
		xmin = 0;
		xmax = 1;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_J1dPhi_lowdm)
	{
		title = "Jet 1 dPhi, low dm";
		var = "J1dPhi_lowdm_h";
		folder = "";
		rebin = 4;
		xmin = 0;
		xmax = 3.2;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_J2dPhi_lowdm)
	{
		title = "Jet 2 dPhi, low dm";
		var = "J2dPhi_lowdm_h";
		folder = "";
		rebin = 4;
		xmin = 0;
		xmax = 3.2;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_J3dPhi_lowdm)
	{
		title = "Jet 3 dPhi, low dm";
		var = "J3dPhi_lowdm_h";
		folder = "";
		rebin = 4;
		xmin = 0;
		xmax = 3.2;
		ymax = 100000000000;
		ymin = 1;
		sig_ymax = 0.05;
	}

	if (plot_MET_lowdm)
	{
		title = "MET [GeV] low dm";
		var = "met_lowdm_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 100000000;
		ymin = 10;
		sig_ymax = 0.1;
	}

	if (plot_MET_lowdm_mid_dPhi)
	{
		title = "MET [GeV] low dm, mid dPhi";
		var = "met_lowdm_mid_dPhi_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 100000000;
		ymin = 10;
		sig_ymax = 0.1;
	}

	if (plot_MET_lowdm_ISR_veto)
	{
		title = "MET [GeV] low dm, ISR veto";
		var = "met_lowdm_ISR_veto_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 100000000;
		ymin = 10;
		sig_ymax = 0.1;
	}

	if (plot_MET_highdm)
	{
		title = "MET [GeV] high dm";
		var = "met_highdm_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 100000000;
		ymin = 1;
		sig_ymax = 1;
	}

	if (plot_MET_highdm_mid_dPhi)
	{
		title = "MET [GeV] high dm, mid dPhi";
		var = "met_highdm_mid_dPhi_h";
		folder = "";
		rebin = 2;
		xmin = 250;
		xmax = 800;
		ymax = 100000000;
		ymin = 1;
		sig_ymax = 1;
	}

	if (plot_S_met_lowdm)
	{
		title = "MET / sqrt(HT)";
		var = "S_met_lowdm_no_S_met_h";
		var_weight = "S_met_lowdm_no_S_met_Stop0l_evtWeight_h";
		folder = "";
		rebin = 1;
		xmax = 50;
		//ymax = 100000000;
		//ymin = 0.1;
		sig_ymax = 1;
	}

	if (plot_ISR_pt_lowdm)
	{
		title = "ISR pt [GeV] low dm";
		var = "ISRpt_lowdm_h";
		folder = "";
		rebin = 2;
		xmax = 1000;
		ymax = 200000;
		sig_ymax = 0.8;
	}

	if (plot_bottom_pt_lowdm)
	{
		title = "bottom pt [GeV] sclar sum low dm";
		var = "bottompt_scalar_sum_lowdm_h";
		folder = "";
		rebin = 1;
		xmin = 20;
		xmax = 200;
		ymax = 20000;
		sig_ymax = 0.5;
	}

	if (plot_jpt_eta_2p5_3p0)
	{
		title = "jet(2.5 < eta < 3.0) pt (GeV)";
		var = "jpt_eta_2p5_3p0_h";
		folder = "";
		rebin = 4;
		xmin = 20;
		ymax = 0.2;
	}

	if (plot_jpt_eta_2p5_3p0_no_HT)
	{
		title = "jet(2.5 < eta < 3.0) pt (GeV), no HT cut";
		var = "jpt_eta_2p5_3p0_no_HT_h";
		folder = "";
		rebin = 4;
		xmin = 20;
		ymax = 0.2;
	}

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.4,leg_height,0.9,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(leg_size);

	TPad *padup = new TPad("padup", "padup", 0, padup_height, 1, 1.0);
	padup -> SetBottomMargin(padup_margin);
	padup -> Draw();
	padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "QCD_HT_2000toInf";
			if(use_smear_QCD) sp = "QCD_Smear_HT_2000toInf";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			if(use_smear_QCD) h1 = (TH1D*)f1->Get(folder + var_weight);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			if(use_original_title) title = h1->GetTitle();

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "unscale bin test error " << h1->GetBinError(bin_test) << std::endl;

			if(!use_smear_QCD)h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			else h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "scaled bin test error " << h1->GetBinError(bin_test) << std::endl;

			pro = (TH1D*)h1->Clone("QCD");
		}

		if (true)
		{
			TString sp = "QCD_HT_200to300";
			if(use_smear_QCD) sp = "QCD_Smear_HT_200to300";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "QCD_HT_300to500";
			if(use_smear_QCD) sp = "QCD_Smear_HT_300to500";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "QCD_HT_500to700";
			if(use_smear_QCD) sp = "QCD_Smear_HT_500to700";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "QCD_HT_700to1000";
			if(use_smear_QCD) sp = "QCD_Smear_HT_700to1000";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "QCD_HT_1000to1500";
			if(use_smear_QCD) sp = "QCD_Smear_HT_1000to1500";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "QCD_HT_1500to2000";
			if(use_smear_QCD) sp = "QCD_Smear_HT_1500to2000";
			if(!use_smear_QCD)Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
			else Plot_1D_AUX_bg_with_weight (lumi, sp, year, var_weight, folder, leg, kGreen, pro, rebin, bin_test);
		}

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kYellow-9);
		leg->AddEntry(pro,"QCD","f");

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_100to200";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "unscale bin test error " << h1->GetBinError(bin_test) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "scaled bin test error " << h1->GetBinError(bin_test) << std::endl;

			pro = (TH1D*)h1->Clone("zjets");
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_200to400";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_400to600";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_600to800";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_800to1200";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_1200to2500";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_2500toInf";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kOrange-3);
		leg->AddEntry(pro,"z+jets","f");
		std::cout << "zinv total ====================================== " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "WJetsToLNu_HT_70to100";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "unscale bin test error " << h1->GetBinError(bin_test) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "scaled bin test error " << h1->GetBinError(bin_test) << std::endl;

			pro = (TH1D*)h1->Clone("wjets");
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_100to200";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_200to400";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_400to600";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_600to800";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_800to1200";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_1200to2500";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_2500toInf";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kSpring-9);
		leg->AddEntry(pro,"w+jets","f");

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "TTbarSingleLepT";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "unscale bin test error " << h1->GetBinError(bin_test) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin test content " << h1->GetBinContent(bin_test) << std::endl;
			std::cout << "scaled bin test error " << h1->GetBinError(bin_test) << std::endl;

			pro = (TH1D*)h1->Clone("ttbar");
		}

		if (true)
		{
			TString sp = "TTbarSingleLepTbar";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "TTbarDiLep";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ST_s_lep";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ST_t_antitop";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ST_t_top";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ST_tW_antitop_incl";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "ST_tW_top_incl";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "TTZToLLNuNu";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		if (true)
		{
			TString sp = "TTZToQQ";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin, bin_test);
		}

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kRed-9);
		leg->AddEntry(pro,"ttbar/single t/ttz","f");

		hs->Add(pro);
	}

	if(plot_BG)
	{
		hs->SetMinimum(ymin);
		if(ymax == -1) ymax = hs-> GetMaximum() * 1.2; 
		hs->SetMaximum(ymax);
		hs->Draw("hist");

		//hs->SetTitle("");
		hs->GetYaxis()->SetTitle("events");
		hs->GetXaxis()->SetRangeUser(xmin,xmax);
		hs->GetYaxis()->SetRangeUser(ymin,ymax);
		hs->GetXaxis()->SetTitle(title);
		//hs->GetYaxis()->SetMaxDigits(2);
		hs->GetYaxis()->SetTitleSize(0.045);
		hs->GetYaxis()->SetTitleOffset(1.2);

		if(plot_BG)
		{
			hs_tot = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral();
			int NSB = ((TH1D*)(hs -> GetStack() -> Last())) -> GetSize() - 2;
			int bin_xmin = ((TH1D*)(hs -> GetStack() -> Last())) -> FindBin(xmin);
			int bin_xmax = ((TH1D*)(hs -> GetStack() -> Last())) -> FindBin(xmax);
			hs_tot_plot = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral(bin_xmin,bin_xmax);
			double hs_bin1 = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(1);
			double hs_bin2 = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(2);
			std::cout << "NSB = " << NSB <<" total BG = " << hs_tot << " bin 1 = " << hs_bin1 << " bin 2 = " << hs_bin2 << "\n" << std::endl;
			std::cout << "==========================================================================" << std::endl;
			for(int i = 0; i < NSB; i++)
			{
				std::cout << "bin " << i << " " << ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(i+1) << std::endl;
				//if(((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(i+1) == 0) std::cout << "empty bin " << i << std::endl;
			}
			std::cout << "==========================================================================" << std::endl;
			//int n_bins = 80/rebin;
			//TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin, xmax);
		}
	}

	TString sp;
	double signal_scale = lumi;
	if(sig_scale_to_BG) signal_scale = hs_tot_plot; 

	if (plot_sig)
	{
		if(signal_name == "T2fbd_test") sp = "T2fbd_mStop600_mLSP520";
		if(signal_name == "T2tt_test") sp = "T2tt_mStop1000_mLSP50";
		if(signal_name == "T1tttt_and_T5ttcc") sp = "T1tttt_mGluino1200_mLSP800";
		if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2200_mLSP100";
		if(signal_name == "T2tt_only") sp = "T2tt_mStop425_mLSP325";
		if(signal_name == "T2bt_and_T2bw") sp = "T2bt_mStop500_mLSP300";
		if(signal_name == "T2fbd_and_T2bwC") sp = "T2fbd_mStop600_mLSP590";
		if(signal_name == "T2cc_and_T2tt") sp = "T2cc_mStop500_mLSP490";

		TFile *f1 = new TFile(result_path + sp + full_or_fast + year + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		//TH1D *h3 = (TH1D*)f1.Get("mt2_uc_h");

		double all_events = h2->GetBinContent(1);
		double left_events = h2->GetBinContent(2);

		std::cout << "left_events = " << left_events << std::endl;
		std::cout << "GetEntries = " << h1->GetEntries() << std::endl;
		std::cout << "GetIntegral = " << h1->Integral() << std::endl;
		std::cout << "all_events = " << all_events << std::endl;
		//std::cout << "GetEntries = " << h3->GetEntries() << std::endl;
		//std::cout << sp << " Xsec = " << CrossSection.at(sp) << std::endl;

		if(sig_scale_to_BG) h1->Scale(signal_scale / h1->Integral() );
		else h1->Scale(signal_scale * 50 * 1000);
		//h1->Scale(signal_scale * CrossSection.at(sp) * 1000 / all_events );

		std::cout << "events rebin= " << h1->Integral()  << std::endl;

		h1->Rebin(rebin);
		h1->SetLineColor(kRed);
		h1->SetLineWidth(2);
		if(plot_BG) h1->Draw("hist same");
		else{
			h1->SetMinimum(ymin);
			h1->SetMaximum(ymax);
			h1->Draw("hist");

			h1->SetTitle("");
			h1->GetYaxis()->SetTitle("events");
			h1->GetXaxis()->SetRangeUser(xmin,xmax);
			h1->GetYaxis()->SetRangeUser(ymin,ymax);
			h1->GetXaxis()->SetTitle(title);
			h1->GetYaxis()->SetTitleSize(0.045);
			h1->GetYaxis()->SetTitleOffset(1.2);
		}
		leg->AddEntry(h1,sp,"l");
	}

	if (plot_sig)
	{
		if(signal_name == "T2fbd_test") sp = "T2fbd_mStop600_mLSP590";
		if(signal_name == "T2tt_test") sp = "T2tt_mStop1000_mLSP500";
		if(signal_name == "T1tttt_and_T5ttcc") sp = "T1tttt_mGluino1500_mLSP100";
		if(signal_name == "T2tt_and_T1tttt_high") sp = "T2tt_mStop1200_mLSP50";
		if(signal_name == "T2tt_only") sp = "T2tt_mStop500_mLSP325";
		if(signal_name == "T2bt_and_T2bw") sp = "T2bt_mStop800_mLSP100";
		if(signal_name == "T2fbd_and_T2bwC") sp = "T2fbd_mStop600_mLSP520";
		if(signal_name == "T2cc_and_T2tt") sp = "T2cc_mStop500_mLSP420";
		Plot_1D_AUX_sg (signal_scale, result_path, sp, full_or_fast, year, var, folder, leg, kAzure+6, rebin, sig_scale_to_BG);
	}

	if (plot_sig)
	{
		if(signal_name == "T1tttt_and_T5ttcc") sp = "T5ttcc_mGluino1000_mLSP800";
		if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2000_mLSP1000";
		if(signal_name == "T2tt_only") sp = "T2tt_mStop850_mLSP350";
		if(signal_name == "T2bt_and_T2bw") sp = "T2bw_mStop500_mLSP325";
		if(signal_name == "T2fbd_and_T2bwC") sp = "T2bwC_mStop600_mLSP590";
		if(signal_name == "T2cc_and_T2tt") sp = "T2tt_mStop250_mLSP150";
		Plot_1D_AUX_sg (signal_scale, result_path, sp, full_or_fast, year, var, folder, leg, kBlue, rebin, sig_scale_to_BG);
	}

	if (plot_sig)
	{
		if(signal_name == "T1tttt_and_T5ttcc") sp = "T5ttcc_mGluino1500_mLSP100";
		if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2000_mLSP100";
		if(signal_name == "T2tt_only") sp = "T2tt_mStop850_mLSP100";
		if(signal_name == "T2bt_and_T2bw") sp = "T2bw_mStop800_mLSP100";
		if(signal_name == "T2fbd_and_T2bwC") sp = "T2bwC_mStop600_mLSP520";
		if(signal_name == "T2cc_and_T2tt") sp = "T2tt_mStop425_mLSP325";
		Plot_1D_AUX_sg (signal_scale, result_path, sp, full_or_fast, year, var, folder, leg, kViolet, rebin, sig_scale_to_BG);
	}

	TLatex latex;
	latex.SetTextSize(leg_size);
	latex.SetNDC();
	//latex.DrawLatex(0.5,ymax+0.4,"#bf{CMS} Preliminary, 2017 data");
	latex.DrawLatex(0.1,0.91,"CMS #bf{Simulation}");
	latex.SetTextAlign(31);  //align at right bottom
	TString lumi_and_energy = "#bf{" + std::to_string((int)lumi) + " fb^{-1} (13TeV)}";
	if(plot_BG) latex.DrawLatex(0.9,0.91,lumi_and_energy);
	else latex.DrawLatex(0.9,0.91,"#bf{13TeV}");
	latex.SetTextAlign(13);  //align at left top
	TString sig_scale = "#splitline{signals scaled to}{BG sum =" + std::to_string(hs_tot_plot) + "}";
	if(plot_BG)
	{
		if(sig_scale_to_BG) latex.DrawLatex(0.15,0.89,"#bf{#splitline{signals scaled to}{BG sum}}");
		//if(sig_scale_to_BG) latex.DrawLatex(0.12,0.82,sig_scale);
		else latex.DrawLatex(0.15,0.89,"#bf{signals scaled up X50}");
	}

	leg->Draw("same");

	if(plot_sig_pad)
	{
		mycanvas -> cd();

		TPad *paddown = new TPad("paddown", "paddown", 0, 0, 1, 0.3);
		paddown -> SetTopMargin(0);
		paddown -> SetBottomMargin(0.3);
		paddown -> SetGrid();
		paddown -> Draw();
		paddown -> cd();

		if (true)
		{
			if(signal_name == "T2fbd_test") sp = "T2fbd_mStop600_mLSP520";
			if(signal_name == "T2tt_test") sp = "T2tt_mStop1000_mLSP50";
			if(signal_name == "T1tttt_and_T5ttcc") sp = "T1tttt_mGluino1200_mLSP800";
			if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2200_mLSP100";
			if(signal_name == "T2tt_only") sp = "T2tt_mStop425_mLSP325";
			if(signal_name == "T2bt_and_T2bw") sp = "T2bt_mStop500_mLSP300";
			if(signal_name == "T2fbd_and_T2bwC") sp = "T2fbd_mStop600_mLSP590";
			if(signal_name == "T2cc_and_T2tt") sp = "T2cc_mStop500_mLSP490";

			TFile *f1 = new TFile(result_path + sp + full_or_fast + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);
			//int n_bins = ((TH1D*)(hs -> GetStack() -> Last())) -> GetSize() - 2;
			//TH1D *significance = new TH1D ("significance", title, n_bins, xmin_h, xmax_h);

			h1->Sumw2();
			h1->Rebin(rebin);

			//h1->Scale(lumi / h1->Integral() );
			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			TH1D *significance = (TH1D*)h1->Clone("significance");;
			int n_bins = significance-> GetSize() - 2;

			std::cout << "\n" << sp << std::endl;

			for (int i = 1; i <= n_bins; i++)
			{
				double signal = h1->GetBinContent(i);
				double back_ground = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(i);
				if (back_ground < 0) back_ground = 0;
				double signal_uc = sqrt (pow(h1->GetBinError(i),2) + pow(0.16*signal,2));  //signal sys unc = 16%
				double back_ground_uc = sqrt (pow(((TH1D*)(hs -> GetStack() -> Last())) -> GetBinError(i),2) + pow(0.2*back_ground,2));  //bg sys unc = 20%
				if (back_ground == 0) back_ground_uc = 1.8;  //unc for empty bin is [0,1.8]
				double sigma = signal + back_ground + signal_uc*signal_uc + back_ground_uc*back_ground_uc;
				double ratio = 0;
				if (sigma > 0) ratio = signal / sqrt(sigma);
				double minus = 2 * sqrt (sigma) - 2 * sqrt (back_ground + back_ground_uc*back_ground_uc);
				if (use_low_stat_sig) significance->SetBinContent(i,minus);
				else significance->SetBinContent(i,ratio);

				std::cout << "bin " << i << " signal = " << signal << " signal uc = " << signal_uc << " BG = " << back_ground << " BG unc = " << back_ground_uc << " significance = " << significance->GetBinContent(i) << std::endl;
			}

			significance->SetTitle("");
			significance->GetYaxis()->SetTitle("significance");
			significance->GetXaxis()->SetRangeUser(xmin,xmax);
			significance->GetXaxis()->SetTitle(title);
			significance->GetXaxis()->SetTitleSize(0.1);
			significance->GetXaxis()->SetLabelSize(0.08);
			significance->GetYaxis()->SetTitleOffset(0.5);
			significance->GetYaxis()->SetTitleSize(0.1);
			significance->GetYaxis()->SetLabelSize(0.08);

			significance->SetMaximum(sig_ymax);
			significance->SetLineColor(kRed);
			significance->SetLineWidth(2);
			significance->Draw("hist");
			//leg->AddEntry(h1,sp,"l");
		}

		if (true)
		{
			if(signal_name == "T2fbd_test") sp = "T2fbd_mStop600_mLSP590";
			if(signal_name == "T2tt_test") sp = "T2tt_mStop1000_mLSP500";
			if(signal_name == "T1tttt_and_T5ttcc") sp = "T1tttt_mGluino1500_mLSP100";
			if(signal_name == "T2tt_and_T1tttt_high") sp = "T2tt_mStop1200_mLSP50";
			if(signal_name == "T2tt_only") sp = "T2tt_mStop500_mLSP325";
			if(signal_name == "T2bt_and_T2bw") sp = "T2bt_mStop800_mLSP100";
			if(signal_name == "T2fbd_and_T2bwC") sp = "T2fbd_mStop600_mLSP520";
			if(signal_name == "T2cc_and_T2tt") sp = "T2cc_mStop500_mLSP420";
			Plot_1D_AUX_sig (lumi, result_path, sp, full_or_fast, year, var, folder, leg, kAzure+6, hs, rebin, use_low_stat_sig);
			//Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kGreen, rebin);
		}

		if (true)
		{
			if(signal_name == "T1tttt_and_T5ttcc") sp = "T5ttcc_mGluino1000_mLSP800";
			if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2000_mLSP1000";
			if(signal_name == "T2tt_only") sp = "T2tt_mStop850_mLSP350";
			if(signal_name == "T2bt_and_T2bw") sp = "T2bw_mStop500_mLSP325";
			if(signal_name == "T2fbd_and_T2bwC") sp = "T2bwC_mStop600_mLSP590";
			if(signal_name == "T2cc_and_T2tt") sp = "T2tt_mStop250_mLSP150";
			Plot_1D_AUX_sig (lumi, result_path, sp, full_or_fast, year, var, folder, leg, kBlue, hs, rebin, use_low_stat_sig);
			//Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kYellow + 1, rebin);
		}

		if (true)
		{
			if(signal_name == "T1tttt_and_T5ttcc") sp = "T5ttcc_mGluino1500_mLSP100";
			if(signal_name == "T2tt_and_T1tttt_high") sp = "T1tttt_mGluino2000_mLSP100";
			if(signal_name == "T2tt_only") sp = "T2tt_mStop850_mLSP100";
			if(signal_name == "T2bt_and_T2bw") sp = "T2bw_mStop800_mLSP100";
			if(signal_name == "T2fbd_and_T2bwC") sp = "T2bwC_mStop600_mLSP520";
			if(signal_name == "T2cc_and_T2tt") sp = "T2tt_mStop425_mLSP325";
			Plot_1D_AUX_sig (lumi, result_path, sp, full_or_fast, year, var, folder, leg, kViolet, hs, rebin, use_low_stat_sig);
			//Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kBlue, rebin);
		}
	}

	if(use_low_stat_sig)
	{
		//mycanvas->SetLogy();
		mycanvas->SaveAs("plots/" + signal_name + "_" + var + "_use_low_stat_sig.png");
	}
	else
		mycanvas->SaveAs("plots/" + signal_name + "_" + var + ".png");

	return 0;
}
