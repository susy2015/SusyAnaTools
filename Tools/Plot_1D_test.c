#include "CrossSection.C"
#include "Plot_1D_AUX.c"
int Plot_1D_test()
{
	const double lumi = 36; 
	bool plot_log = false;
	bool plot_BG = false;
	//plot_log = true;

	bool plot_MT2 = false;
	bool plot_MT2_baseline_no_mt2 = false;
	bool plot_MTb = false;
	bool plot_MTb_CSV = false;
	bool plot_MT2_b = false;
	bool plot_SB = false;
	bool plot_SB_team_A = false;
	bool plot_nbottompt20 = false;
	bool plot_nbottompt30 = false;
	bool plot_ntop = false;
	bool plot_ntop_merge = false;
	bool plot_ntop_w = false;
	bool plot_ntop_res = false;
	bool plot_nw = false;
	bool plot_gen_w_pt = false;
	bool plot_gen_b_pt = false;
	bool plot_gen_w_b_mass = false;
	bool plot_gen_w_b_delta_r = false;
	bool plot_n_res_b_gen_match = false;
	bool plot_n_res_w_gen_match = true;
	bool plot_n_res_gen_match = false;
	bool plot_njetspt20 = false;
	bool plot_njetspt30 = false;
	bool plot_HT = false;
	bool plot_MET = false;
	bool plot_MT2_different_MTb = false;

	double xmin =0, xmax = 800, ymin = 0, ymax = 35000;
	if (plot_log) ymin = 0.1;

	TString title = "MT2";
	TString x_title = "GeV";
	TString var = "mt2_h";
	TString folder = "Baseline_Only/";

	int rebin = 1;

	if (plot_MT2)
	{	
		//title = "MT2";
		//var = "mt2_h";
		title = "MT2 when MTb > 175";
		var = "mt2_high_h";
		folder = "Baseline_MTb>175/";
		rebin = 5;
		//xmin = 50;
		//ymax = 10000;
	}

	if (plot_MT2_baseline_no_mt2)
	{	
		title = "";
		var = "mt2_baseline_no_mt2_h";
		//title = "MT2 when MTb > 140";
		//var = "mt2_140_high_h";
		folder = "";
		rebin = 5;
		//xmin = 100;
		//xmax = 500;
		//ymax = 250;
		ymax = 4000;
	}

	if (plot_MTb)
	{
		title = "MTb (2 leading pt if nb >2)";
		//title = "MTb (0 - 300 GeV)";
		//var = "mtb_h";
		var = "mtb_no_HT_h";
		//title = "MTb high dm";
		//var = "mtb_high_dm_h";
		folder = "";
		rebin = 5;
		//xmax = 300;
		ymax = 14000;
	}

	if (plot_MTb_CSV)
	{
		title = "MTb (2 leading CSV if nb > 2)";
		//title = "MTb (0 - 300 GeV)";
		//var = "mtb_h";
		var = "mtb_CSV_no_HT_h";
		//title = "MTb high dm";
		//var = "mtb_high_dm_h";
		folder = "";
		rebin = 5;
		//xmax = 300;
		ymax = 14000;
	}

	if (plot_MT2_b)
	{
		title = "MT2 with b";
		var = "mt2_b_h";
		rebin = 1;
		xmax = 100;
		ymax = 25000;
	}

	if (plot_SB)
	{
		title = "search bin";
		var = "search_bin_h";
		folder = "";
		rebin = 1;
		xmax = 84;
		ymax = 2000;
	}

	if (plot_SB_team_A)
	{
		//title = "high dm search bin with MT2 cut";
		//var = "search_bin_team_A_MTb175_MT2_h";
		title = "high dm search bin";
		var = "search_bin_team_A_h";
		folder = "";
		rebin = 1;
		xmin = 53;
		xmax = 104;
		ymax = 1500;
		x_title = "";
	}

	if (plot_nbottompt20)
	{
		//title = "number of bottom (pt > 20) without MTb cut";
		//var = "nbottompt20_no_mtb_h";
		title = "number of bottom (pt > 20) when MTb > 175";
		var = "nbottompt20_175_h";
		//title = "number of bottom (pt > 20) when MTb > 175 with MT2 cut";
		//var = "nbottompt20_175_MT2_h";
		//title = "number of bottom (pt > 20) when MTb > 140 with MT2 cut";
		//var = "nbottompt20_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 40000;
		x_title = "";
	}

	if (plot_nbottompt30)
	{
		//title = "number of bottom (pt > 30) without MTb cut";
		//var = "nbottompt30_no_mtb_h";
		title = "number of bottom (pt > 30) when MTb > 175";
		var = "nbottompt30_175_h";
		//title = "number of bottom (pt > 30) when MTb > 175 with MT2 cut";
		//var = "nbottompt30_175_MT2_h";
		//title = "number of bottom (pt > 30) when MTb > 140 with MT2 cut";
		//var = "nbottompt30_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 10000000;
		ymax = 40000;
		x_title = "";
	}

	if (plot_ntop)
	{
		//title = "number of top without MTb cut";
		//var = "ntop_no_mtb_h";
		title = "number of top when MTb > 175";
		var = "ntop_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		//ymax = 1000000;
		ymax = 30000;
		x_title = "";
	}

	if (plot_ntop_merge)
	{
		//title = "number of top without MTb cut";
		//var = "ntop_no_mtb_h";
		title = "number of fully merged top when MTb > 175";
		var = "ntop_merge_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 4;
		//ymax = 1000000;
		ymax = 1;
		x_title = "";
	}

	if (plot_ntop_w)
	{
		//title = "number of top without MTb cut";
		//var = "ntop_no_mtb_h";
		title = "number of partialy merged top when MTb > 175";
		var = "ntop_w_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 4;
		//ymax = 1000000;
		ymax = 1;
		x_title = "";
	}

	if (plot_ntop_res)
	{
		//title = "number of top without MTb cut";
		//var = "ntop_no_mtb_h";
		title = "number of resolved top when MTb > 175";
		var = "ntop_res_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 4;
		//ymax = 1000000;
		ymax = 1;
		x_title = "";
	}

	if (plot_nw)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "number of W when MTb > 175";
		var = "nw_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 8;
		ymax = 50000;
		x_title = "";
	}

	if (plot_gen_w_pt)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "gen w pt when MTb > 175";
		var = "gen_w_pt_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 8;
		xmax = 400;
		ymax = 0.4;
		x_title = "";
	}

	if (plot_gen_b_pt)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "gen b pt when MTb > 175";
		var = "gen_b_pt_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 8;
		xmax = 400;
		ymax = 0.4;
		x_title = "";
	}

	if (plot_gen_w_b_mass)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "gen w and b mass when MTb > 175";
		var = "gen_w_b_mass_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 400;
		ymax = 0.7;
		x_title = "";
	}

	if (plot_gen_w_b_delta_r)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "gen w and b delta R when MTb > 175";
		var = "gen_w_b_delta_r_175_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 8;
		xmax = 400;
		ymax = 0.6;
		x_title = "";
	}

	if (plot_n_res_gen_match)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "number of gen matched resolved jets when ntop = 1 and MTb > 175";
		var = "n_res_gen_match_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 400;
		ymax = 0.7;
		x_title = "";
	}

	if (plot_n_res_b_gen_match)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "number of gen matched b in resolved jets when ntop = 1 and MTb > 175";
		var = "n_res_b_gen_match_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 400;
		ymax = 0.9;
		x_title = "";
	}

	if (plot_n_res_w_gen_match)
	{
		//title = "number of W without MTb cut";
		//var = "nw_no_mtb_h";
		title = "number of gen matched w in resolved jets when ntop = 1 and MTb > 175";
		var = "n_res_w_gen_match_h";
		//title = "number of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "number of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 400;
		ymax = 0.9;
		x_title = "";
	}

	if (plot_njetspt20)
	{
		//title = "number of jets (pt > 20) without MTb cut";
		//var = "njetspt20_no_mtb_h";
		title = "number of jets (pt > 20) when MTb > 175";
		var = "njetspt20_175_h";
		//title = "number of jets (pt > 20) when MTb > 175 with MT2 cut";
		//var = "njetspt20_175_MT2_h";
		//title = "number of jetspt (pt > 20) when MTb > 140 with MT2 cut";
		//var = "njetspt20_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 15000;
		x_title = "";
	}

	if (plot_njetspt30)
	{
		//title = "number of jets (pt > 30) without MTb cut";
		//var = "njetspt30_no_mtb_h";
		//title = "number of jets (pt > 30) when MTb > 175";
		//var = "njetspt30_175_h";
		//title = "number of jets (pt > 30) when MTb > 175 with MT2 cut";
		//var = "njetspt30_175_MT2_h";
		title = "number of jetspt (pt > 30) when MTb > 140 with MT2 cut";
		var = "njetspt30_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax = 20;
		ymax = 1500;
		x_title = "";
	}

	if (plot_HT)
	{
		//title = "HT without MTb cut";
		//var = "HT_no_mtb_h";
		title = "HT when MTb > 175";
		var = "HT_175_h";
		//title = "HT when MTb > 175 with MT2 cut";
		//var = "HT_175_MT2_h";
		//title = "HT when MTb > 140 with MT2 cut";
		//var = "HT_140_MT2_h";
		folder = "";
		rebin = 5;
		xmax = 3200;
		ymax = 10000;
		x_title = "";
	}

	if (plot_MET)
	{
		//title = "MET without MTb cut";
		//var = "met_no_mtb_h";
		//title = "MET when MTb > 175";
		//var = "met_175_h";
		//title = "MET when MTb > 175 with MT2 cut";
		//var = "met_175_MT2_h";
		title = "MET when MTb > 140 with MT2 cut";
		var = "met_140_MT2_h";
		folder = "";
		rebin = 2;
		xmax = 1000;
		ymax = 1500;
		x_title = "";
	}

	if (plot_MT2_different_MTb)
	{
		//title = "MT2 (0 - 800) without MTb cut";
		//var = "mt2_no_mtb_h";
		//title = "MT2 (50 - 800) without MTb cut";
		//var = "mt2_no_mtb_h";
		title = "MT2 (50 - 800) when MTb > 175";
		var = "mt2_175_h";
		//title = "MT2 (50 - 800) when MTb > 140";
		//var = "mt2_140_h";
		folder = "";
		rebin = 5;
		xmin = 50;
		ymax = 10000;
		x_title = "";
	}

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 1280, 720);
	//TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 800, 600);
	gStyle->SetOptStat(kFALSE);

	THStack *hs = new THStack();

	TLegend* leg = new TLegend(0.55,0.6,0.9,0.89);
	leg->SetBorderSize(0);
	//leg->SetTextFont(42);
	//leg->SetFillColor(0);
	//TLegend* leg2 = new TLegend(0.8,0.9,0.9,0.95);
	TLegend* leg2 = new TLegend(0.2,0.8,0.55,0.89);
	leg2->SetBorderSize(0);
	//leg->SetTextFont(42);
	//leg->SetFillColor(0);
	//leg2->AddEntry((TObject*)0,"36 fb ^ -1","");
	//leg2->AddEntry((TObject*)0,"signal is multiplied by 50","");
	leg2->AddEntry((TObject*)0,"each signal scaled to total background","");
	//leg2->AddEntry((TObject*)0,"","");
	leg->SetTextSize(0.03);
	leg2->SetTextSize(0.03);
	TLegend* leg3 = new TLegend(0.7,0.9,0.89,0.95);
	leg3->SetBorderSize(0);
	leg3->AddEntry((TObject*)0,"36 fb^{-1} (13TeV)","");
	
	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "QCD_HT300to500";

			TFile *f1 = new TFile("results/" + sp + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//ttbar->Add(h1);
			h1->Rebin(rebin);
			pro = (TH1D*)h1->Clone("QCD");
			//h1->SetLineColor(kOrange);
			//h1->SetLineWidth(3);
			//h1->Draw("same");
			//leg->AddEntry(h1,sp,"l");
		}

		if (false)
		{
			TString sp = "QCD_HT200to300";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (false)
		{
			TString sp = "QCD_HT500to700";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "QCD_HT700to1000";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "QCD_HT1000to1500";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "QCD_HT1500to2000";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "QCD_HT2000toInf";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		pro->SetLineColor(kBlack);
		pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kRed);
		leg->AddEntry(pro,"QCD","f");

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_100to200";

			TFile *f1 = new TFile("results/" + sp + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//ttbar->Add(h1);
			h1->Rebin(rebin);
			pro = (TH1D*)h1->Clone("zjets");
			//h1->SetLineColor(kOrange);
			//h1->SetLineWidth(3);
			//h1->Draw("same");
			//leg->AddEntry(h1,sp,"l");
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_200to400";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_400to600";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_600to800";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_800to1200";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_1200to2500";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_2500toInf";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		pro->SetLineColor(kBlack);
		pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kGreen);
		leg->AddEntry(pro,"z+jets","f");

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "WJetsToLNu_HT_70to100";

			TFile *f1 = new TFile("results/" + sp + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//ttbar->Add(h1);
			h1->Rebin(rebin);
			pro = (TH1D*)h1->Clone("wjets");
			//h1->SetLineColor(kOrange);
			//h1->SetLineWidth(3);
			//h1->Draw("same");
			//leg->AddEntry(h1,sp,"l");
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_100to200";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_200to400";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_400to600";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_600to800";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_800to1200";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_1200to2500";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_2500toInf";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		pro->SetLineColor(kBlack);
		pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kYellow);
		leg->AddEntry(pro,"w+jets","f");

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TH1D * pro = NULL;

		if (true)
		{
			TString sp = "TTbarSingleLepT";

			TFile *f1 = new TFile("results/" + sp + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//ttbar->Add(h1);
			h1->Rebin(rebin);
			pro = (TH1D*)h1->Clone("ttbar");
			//h1->SetLineColor(kYellow);
			//h1->SetLineWidth(3);
			//h1->Draw("same");
			//leg->AddEntry(h1,sp,"l");
		}

		if (true)
		{
			TString sp = "TTbarSingleLepTbar";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "TTbarDiLep";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ST_s";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ST_t_antitop";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "ST_t_top";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "tW_antitop_incl";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (false)
		{
			TString sp = "tW_top_incl";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "TTZToLLNuNu";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		if (true)
		{
			TString sp = "TTZToQQ";
			Plot_1D_AUX_bg (lumi, sp, var, folder, leg, kGreen, pro, rebin);
		}

		pro->SetLineColor(kBlack);
		pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kViolet);
		leg->AddEntry(pro,"ttbar + single t + ttz","f");

		hs->Add(pro);
	}

	/*hs->SetMinimum(ymin);
	hs->SetMaximum(ymax);
	hs->Draw("axis");
	
	hs->SetTitle(title);
	hs->GetYaxis()->SetTitle("events");
	hs->GetXaxis()->SetRangeUser(xmin,xmax);
	if(plot_SB_team_A) hs->GetXaxis()->SetNdivisions(26);
	//hs->GetXaxis()->SetTitle("MT2 [GeV]");
	hs->GetXaxis()->SetTitle(x_title);
	hs->GetYaxis()->SetTitleOffset(1.5);
	
	double hs_events = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral();
	std::cout << "total BG = " << hs_events << std::endl;
	//int n_bins = 80/rebin;
	//TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin, xmax);

	leg->Draw("same");
	leg2->Draw("same");
	leg3->Draw("same");*/

	if (true)
	{
		//TString sp = "Signal_T1tttt_mGluino1200_mLSP800";
		//TString sp = "Signal_T5ttcc_mGluino1200_mLSP800";
		TString sp = "Signal_T2tt_mStop500_mLSP325";

		TFile *f1 = new TFile("results/" + sp + ".root");
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
		std::cout << sp << " Xsec = " << CrossSection.at(sp) << std::endl;

		h1->Scale(1.0 / h1->Integral() );
		//h1->Scale(hs_events / h1->Integral() );
		//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
		//h1->Scale(lumi * 50 * CrossSection.at(sp) * 1000 / all_events );

		std::cout << "events rebin= " << h1->Integral()  << std::endl;

		h1->Rebin(rebin);
		h1->SetLineColor(kRed);
		h1->SetLineWidth(3);
		//h1->Draw("hist same");
		h1->SetMaximum(ymax);
		h1->Draw("hist");
		leg->AddEntry(h1,sp,"l");
	}

	if (true)
	{
		//TString sp = "Signal_T1tttt_mGluino1500_mLSP100";
		//TString sp = "Signal_T5ttcc_mGluino1500_mLSP100";
		TString sp = "Signal_T2tt_mStop850_mLSP100";
		//Plot_1D_AUX_sg (hs_events, sp, var, folder, leg, kGreen, rebin);
		Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kGreen, rebin);
	}

	if (true)
	{
		//TString sp = "Signal_T2tt_mStop500_mLSP325";
		TString sp = "Signal_T2bw_mStop500_mLSP325";
		//Plot_1D_AUX_sg (hs_events, sp, var, folder, leg, kYellow, rebin);
		Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kYellow, rebin);
	}

	if (true)
	{
		//TString sp = "Signal_T2tt_mStop850_mLSP100";
		TString sp = "Signal_T2bw_mStop850_mLSP100";
		//Plot_1D_AUX_sg (hs_events, sp, var, folder, leg, kBlue, rebin);
		Plot_1D_AUX_sg (1.0, sp, var, folder, leg, kBlue, rebin);
	}

	leg->Draw("same");

	if(plot_log)
	{
		mycanvas->SetLogy();
		mycanvas->SaveAs(var + "log.png");
	}
	else
		mycanvas->SaveAs(var + ".png");

	return 0;
}
