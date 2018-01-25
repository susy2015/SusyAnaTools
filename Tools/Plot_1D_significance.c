#include "CrossSection.C"
#include "Plot_1D_AUX.c"
int Plot_1D_significance()
{
	const double lumi = 36; 
	bool plot_log = false;
	//plot_log=true;

	bool plot_MT2 = false;
	bool plot_MTb = false;
	bool plot_MT2_b = false;
        bool plot_SB = false;
        bool plot_SB_team_A = false;
	bool plot_nbottompt20 = false;
	bool plot_nbottompt30 = false;
	bool plot_ntop = true;
	bool plot_njetspt20 = false;
	bool plot_njetspt30 = false;
	bool plot_HT = false;
	bool plot_MET = false;
	bool plot_MT2_different_MTb = false;

	double xmin_h =0, xmin_plot = 0, xmax_h = 800, xmax_plot = 800, ymin = 0, ymax = 1;
	if (plot_log) ymin = 0.0001;

	TString title = "sigbificance of MT2";
	TString x_title = "GeV";
	TString var = "mt2_h";
	TString folder = "Baseline_Only/";

	int rebin = 1;

	if (plot_MT2)
	{	
		title = "significance of MT2 when MTb > 175";
		var = "mt2_high_h";
		folder = "Baseline_MTb>175/";
		rebin = 5;
		//xmin_h = 50;
		//ymin = 0.001; 
		ymax = 0.05;
	}

	if (plot_MTb)
	{
		title = "significance of MTb (0 - 800)";
		//title = "significance of MTb (0 - 300)";
		//var = "mtb_h";
		var = "mtb_no_HT_h";
		folder = "";
		rebin = 5;
		//xmax_plot = 300;
		ymax = 0.1;
	}

	if (plot_MT2_b)
	{
		title = "MT2 with b";
		var = "mt2_b_h";
		rebin = 5;
		xmax_h = 100;
		ymax = 25000;
	}

        if (plot_SB)
        {
                //title = "significance of each search bin";
                //var = "search_bin_h";
                title = "significance of each search bin when MTb > 175";
                var = "search_bin_MTb_h";
                folder = ""; 
                rebin = 1;
                xmax_h = 84;
		ymin = 0.001; 
                ymax = 0.5;
        }

        if (plot_SB_team_A)
        {
                //title = "significance of each search bin";
                //var = "search_bin_team_A_h";
                //title = "significance of each search bin, MTb 140 and MT2 cut";
                //var = "search_bin_team_A_MTb140_MT2_h";
                title = "significance of each search bin with MT2 cut";
                var = "search_bin_team_A_MTb175_MT2_h";
                folder = ""; 
                rebin = 1;
                xmin_h = 52;
                xmax_h = 104;
                xmin_plot = 53;
                xmax_plot = 104;
		//ymin = 0.001; 
                ymax = 3;
        }

	if (plot_nbottompt20)
	{
		//title = "significance of bottom (pt > 20) without MTb cut";
		//var = "nbottompt20_no_mtb_h";
		title = "significance of bottom (pt > 20) when MTb > 175";
		var = "nbottompt20_175_h";
		//title = "significance of bottom (pt > 20) when MTb > 175 with MT2 cut";
		//var = "nbottompt20_175_MT2_h";
		//title = "significance of bottom (pt > 20) when MTb > 140 with MT2 cut";
		//var = "nbottompt20_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax_h = 8;
		xmax_plot = 8;
		ymax = 2;
		x_title = "";
	}

	if (plot_nbottompt30)
	{
		//title = "significance of bottom (pt > 30) without MTb cut";
		//var = "nbottompt30_no_mtb_h";
		//title = "significance of bottom (pt > 30) when MTb > 175";
		//var = "nbottompt30_175_h";
		//title = "significance of bottom (pt > 30) when MTb > 175 with MT2 cut";
		//var = "nbottompt30_175_MT2_h";
		title = "significance of bottom (pt > 30) when MTb > 140 with MT2 cut";
		var = "nbottompt30_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax_h = 8;
		xmax_plot = 8;
		ymax = 2;
		x_title = "";
	}

	if (plot_ntop)
	{
		title = "significance of top without MTb cut";
		var = "ntop_no_mtb_h";
		//title = "significance of top when MTb > 175";
		//var = "ntop_175_h";
		//title = "significance of top when MTb > 175 with MT2 cut";
		//var = "ntop_175_MT2_h";
		//title = "significance of top when MTb > 140 with MT2 cut";
		//var = "ntop_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax_h = 8;
		xmax_plot = 8;
		ymax = 4;
		x_title = "";
	}

	if (plot_njetspt20)
	{
		//title = "significance of jets (pt > 20) without MTb cut";
		//var = "njetspt20_no_mtb_h";
		title = "significance of jets (pt > 20) when MTb > 175";
		var = "njetspt20_175_h";
		//title = "significance of jets (pt > 20) when MTb > 175 with MT2 cut";
		//var = "njetspt20_175_MT2_h";
		//title = "significance of jetspt (pt > 20) when MTb > 140 with MT2 cut";
		//var = "njetspt20_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax_h = 30;
		xmax_plot = 20;
		ymax = 3;
		x_title = "";
	}

	if (plot_njetspt30)
	{
		title = "significance of jets (pt > 30) without MTb cut";
		var = "njetspt30_no_mtb_h";
		//title = "significance of jets (pt > 30) when MTb > 175";
		//var = "njetspt30_175_h";
		//title = "significance of jets (pt > 30) when MTb > 175 with MT2 cut";
		//var = "njetspt30_175_MT2_h";
		//title = "significance of jetspt (pt > 30) when MTb > 140 with MT2 cut";
		//var = "njetspt30_140_MT2_h";
		folder = "";
		rebin = 1;
		xmax_h = 30;
		xmax_plot = 20;
		ymax = 3;
		x_title = "";
	}

	if (plot_HT)
	{
		title = "significance of HT without MTb cut";
		var = "HT_no_mtb_h";
		//title = "significance of HT when MTb > 175";
		//var = "HT_175_h";
		//title = "significance of HT when MTb > 175 with MT2 cut";
		//var = "HT_175_MT2_h";
		//title = "significance of HT when MTb > 140 with MT2 cut";
		//var = "HT_140_MT2_h";
		folder = "";
		rebin = 5;
		xmax_h = 3200;
		xmax_plot = 3200;
		ymax = 2;
		x_title = "";
	}

	if (plot_MET)
	{
		title = "significance of MET without MTb cut";
		var = "met_no_mtb_h";
		//title = "significance of MET when MTb > 175";
		//var = "met_175_h";
		//title = "significance of MET when MTb > 175 with MT2 cut";
		//var = "met_175_MT2_h";
		//title = "significance of MET when MTb > 140 with MT2 cut";
		//var = "met_140_MT2_h";
		folder = "";
		rebin = 2;
		xmax_h = 1600;
		xmax_plot = 1000;
		ymax = 2;
		x_title = "";
	}

	if (plot_MT2_different_MTb)
	{
		title = "significance of MT2 (0 - 800) without MTb cut";
		//title = "significance of MT2 (50 - 800) without MTb cut";
		var = "mt2_no_mtb_h";
		//title = "significance of MT2 (50 - 800) when MTb > 175";
		//var = "mt2_175_h";
		//title = "significance of MT2 (50 - 800) when MTb > 140";
		//var = "mt2_140_h";
		folder = "";
		rebin = 5;
		//xmin_plot = 50;
		ymax = 2;
		x_title = "";
	}

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 1280, 720);
	gStyle->SetOptStat(kFALSE);

	THStack *hs = new THStack();

	//TLegend* leg = new TLegend(0.13,0.7,0.45,0.89);
	TLegend* leg = new TLegend(0.55,0.7,0.85,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.03);

	TLegend* leg3 = new TLegend(0.7,0.9,0.89,0.95);
	leg3->SetBorderSize(0);
	leg3->AddEntry((TObject*)0,"36 fb^{-1} (13TeV)","");

	if (true)
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
			//double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
		
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

		if (true)
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
		//leg->AddEntry(pro,"QCD","f");

		hs->Add(pro);
	}

	if (true)
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
			//double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

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
		//leg->AddEntry(pro,"z+jets","f");

		hs->Add(pro);
	}

	if (true)
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
			//double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

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
		//leg->AddEntry(pro,"w+jets","f");

		hs->Add(pro);
	}

	if (true)
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
			//double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			std::cout << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

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

		if (true)
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
		//leg->AddEntry(pro,"ttbar + single t + ttz","f");

		hs->Add(pro);
	}

	double hs_events = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral();
	int n_bins = ((TH1D*)(hs -> GetStack() -> Last())) -> GetSize() - 2;

	if (true)
	{
		//TString sp = "Signal_T1tttt_mGluino1200_mLSP800";
		TString sp = "Signal_T5ttcc_mGluino1200_mLSP800";

		TFile *f1 = new TFile("results/" + sp + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		h1->Sumw2();
		//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		//TH1D *h3 = (TH1D*)f1.Get("mt2_uc_h");
		TH1D *significance = new TH1D ("significance", title, n_bins, xmin_h, xmax_h);

		double all_events = h2->GetBinContent(1);
		//double left_events = h2->GetBinContent(2);

		h1->Rebin(rebin);

		std::cout << "unscale bin 1 signal " << h1->GetBinContent(1) << std::endl;
		std::cout << "unscale bin 2 signal " << h1->GetBinContent(2) << std::endl;

		h1->Scale (lumi * CrossSection.at(sp) * 1000 / all_events);

		std::cout << "scaled bin 1 signal " << h1->GetBinContent(1) << std::endl;
		std::cout << "scaled bin 1 BG " << ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(1) << std::endl;
		std::cout << "scaled bin 1 BG stat uc " << ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinError(1) << std::endl;
		std::cout << "scaled bin 1 BG sys uc " << 0.2* ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(1)<< std::endl;

		std::cout << "scaled bin 2 signal " << h1->GetBinContent(2) << std::endl;
		std::cout << "scaled bin 2 BG " << ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(2) << std::endl;
		std::cout << "scaled bin 2 BG stat uc " << ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinError(2) << std::endl;
		std::cout << "scaled bin 2 BG sys uc " << 0.2* ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(2)<< std::endl;
		std::cout << "\n" << sp << "\n" << std::endl;

		for (int i = 1; i < n_bins + 1; i++)
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
 
			significance->SetBinContent(i,ratio);
			std::cout << "bin " << i << " signal = " << signal << "signal uc = " << signal_uc << " BG = " << back_ground << " BG unc = " << back_ground_uc << " significance = " << significance->GetBinContent(i) << std::endl;
		}

		//h1->Scale(hs_events / h1->Integral() );
		//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

		//std::cout << "events rebin= " << h1->Integral()  << std::endl;
		significance->SetMaximum(ymax);
		significance->SetMinimum(ymin);
		significance->GetYaxis()->SetTitle("significance");
		significance->GetXaxis()->SetRangeUser(xmin_plot, xmax_plot);
		significance->GetXaxis()->SetTitle(x_title);
		if(plot_SB_team_A)
		{
			significance->GetXaxis()->SetTitle("Search Bin");
			significance->GetXaxis()->SetNdivisions(26);
		}

		significance->SetLineColor(kRed);
		significance->SetLineWidth(3);
		significance->Draw();
		leg->AddEntry(significance,sp,"l");
	}

	if (true)
	{
		//TString sp = "Signal_T1tttt_mGluino1500_mLSP100";
		TString sp = "Signal_T5ttcc_mGluino1500_mLSP100";

		TFile *f1 = new TFile("results/" + sp + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		h1->Sumw2();
		//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		//TH1D *h3 = (TH1D*)f1.Get("mt2_uc_h");
		TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin_h, xmax_h);

		double all_events = h2->GetBinContent(1);
		//double left_events = h2->GetBinContent(2);

		h1->Rebin(rebin);
		h1->Scale (lumi * CrossSection.at(sp) * 1000 / all_events);
		std::cout << "\n" << sp << "\n" << std::endl;

		for (int i = 1; i < n_bins + 1; i++)
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
 
			significance->SetBinContent(i,ratio);
			std::cout << "bin " << i << " signal = " << signal << "signal uc = " << signal_uc << " BG = " << back_ground << " BG unc = " << back_ground_uc << " significance = " << significance->GetBinContent(i) << std::endl;
		}

		//h1->Scale(hs_events / h1->Integral() );
		//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

		//std::cout << "events rebin= " << h1->Integral()  << std::endl;

		significance->SetLineColor(kGreen);
		significance->SetLineWidth(3);
		significance->Draw("same");
		leg->AddEntry(significance,sp,"l");
	}

	if (true)
	{
		//TString sp = "Signal_T2tt_mStop500_mLSP325";
		TString sp = "Signal_T2bw_mStop500_mLSP325";

		TFile *f1 = new TFile("results/" + sp + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		h1->Sumw2();
		//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		//TH1D *h3 = (TH1D*)f1.Get("mt2_uc_h");
		TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin_h, xmax_h);

		double all_events = h2->GetBinContent(1);
		//double left_events = h2->GetBinContent(2);

		h1->Rebin(rebin);
		h1->Scale (lumi * CrossSection.at(sp) * 1000 / all_events);
		std::cout << "\n" << sp << "\n" << std::endl;

		for (int i = 1; i < n_bins + 1; i++)
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
 
			significance->SetBinContent(i,ratio);
			std::cout << "bin " << i << " signal = " << signal << "signal uc = " << signal_uc << " BG = " << back_ground << " BG unc = " << back_ground_uc << " significance = " << significance->GetBinContent(i) << std::endl;
		}

		//h1->Scale(hs_events / h1->Integral() );
		//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

		//std::cout << "events rebin= " << h1->Integral()  << std::endl;

		significance->SetLineColor(kYellow);
		significance->SetLineWidth(3);
		significance->Draw("same");
		leg->AddEntry(significance,sp,"l");
	}

	if (true)
	{
		//TString sp = "Signal_T2tt_mStop850_mLSP100";
		TString sp = "Signal_T2bw_mStop800_mLSP100";

		TFile *f1 = new TFile("results/" + sp + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		h1->Sumw2();
		//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		//TH1D *h3 = (TH1D*)f1.Get("mt2_uc_h");
		TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin_h, xmax_h);

		double all_events = h2->GetBinContent(1);
		//double left_events = h2->GetBinContent(2);

		h1->Rebin(rebin);
		h1->Scale (lumi * CrossSection.at(sp) * 1000 / all_events);
		std::cout << "\n" << sp << "\n" << std::endl;

		for (int i = 1; i < n_bins + 1; i++)
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
 
			significance->SetBinContent(i,ratio);
			std::cout << "bin " << i << " signal = " << signal << "signal uc = " << signal_uc << " BG = " << back_ground << " BG unc = " << back_ground_uc << " significance = " << significance->GetBinContent(i) << std::endl;
		}

		//h1->Scale(hs_events / h1->Integral() );
		//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

		//std::cout << "events rebin= " << h1->Integral()  << std::endl;

		significance->SetLineColor(kBlue);
		significance->SetLineWidth(3);
		significance->Draw("same");
		leg->AddEntry(significance,sp,"l");
	}

	leg->Draw("same");
	leg3->Draw("same");

	if(plot_log)
	{
		mycanvas->SetLogy();
		mycanvas->SaveAs(var + "_sig_log.png");
	}
	else
		mycanvas->SaveAs(var + "_sig.png");

	return 0;
}
