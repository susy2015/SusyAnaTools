#include "CrossSection.C"
#include "Plot_1D_AUX.c"
int Plot_1D_TTZ_comp()
{
	//gROOT->ForceStyle();

	double lumi = 36; //2016
	//lumi = 42; //2017
	//lumi = 59; //2018
    //std::vector<TString> years = {"_2016","_2017"};
    std::vector<TString> years = {"_2016","_2017","_2018_PreHEM","_2018_PostHEM"};
    //std::vector<TString> years = {"_2016","_2017","_2018"};
	TString year = "_2016";
	bool plot_log = false;
	bool plot_sig_pad = true;

	bool plot_BG = false;
    bool plot_TTZ = false;
    bool plot_Rare = false;
    bool plot_NonPrompt = false;
    bool plot_Data = true;

	bool use_original_title = false;

    if(plot_TTZ || plot_Rare || plot_NonPrompt)
    {
        plot_BG = true;
        plot_Data = false;
        years = {"_2016","_2017","_2018"};
    }
    if(plot_Data)
    {
        plot_BG = false;
        years = {"_2016","_2017","_2018_PreHEM","_2018_PostHEM"};
    }

    //Unused, since normalizing to 2016
    if(year == "_2017") lumi = 42;
    if(year == "_2018") lumi = 59;
    if(year == "_2018_PreHEM") lumi = 21;
    if(year == "_2018_PostHEM") lumi = 38;

	TString result_path = "results/";
	TString data_name = "SingleMuon";

    bool plot_recoZpt = true;
    bool plot_recoZM = false;
    bool plot_met = false;
    bool plot_ht = false;
    bool plot_njet = false;
    bool plot_njetpt20eta24 = false;
    bool plot_njetpt40 = false;
    bool plot_Stop0l_nJets = false;
    bool plot_num_mu = false;
    bool plot_mu_indices = false;
    bool plot_lep1pT = false;
    bool plot_lep2pT = false;
    bool plot_lep3pT = false;
    bool plot_numZ = false;
    bool plot_numZ_noweights = false;
    bool plot_norm_1mu = false;
    bool plot_norm_2mu = false;
    bool plot_norm_3mu = false;
    bool plot_cutflow = false;

	float xmin =0, xmax = 800, ymin = 0, ymax = 35000, sig_ymax = 1;
	int hs_tot = 1, hs_tot_plot = 1;
	float padup_height = 0.3, padup_margin = 0, leg_height = 0.6;
	if(!plot_sig_pad) {padup_height = 0; padup_margin = 0.1, leg_height = 0.6;}
	if(plot_log) ymin = 0.1;

	TString title = "MET";
	TString var = "met_h";
	TString folder = "";

	int rebin = 1;

	if (plot_recoZpt)
	{
		title = "Reconstructed Z pT";
		var = "h_recoZpt";
		folder = "";
		rebin = 1;
		ymin = 0;
		ymax = 12; //10 for TTZ, 20 for Data
        if(plot_log)
        {
            ymin = 0.1;
            ymax = 100;
        }
	}
	if (plot_recoZM)
	{
		title = "Reconstructed Z Mass";
		var = "h_recoZM";
		folder = "";
		rebin = 1;
        rebin = 2;
        ymin = 0;
        ymax = 12; //6 for TTZ, 12 for Data
		if (plot_log)
        {
            ymin = 0.1;
		    ymax = 100;
        }
	}
	if (plot_met)
	{
		title = "MET";
		var = "h_met";
		folder = "";
		rebin = 1;
		xmax = 500;
        ymin = 0;
        ymax = 20; //8 for TTZ, 20 for Data
		if (plot_log)
        {
            ymin = 0.1;
		    ymax = 100;
        }
	}
	if (plot_ht)
	{
		title = "HT";
        var = "h_ht";
        folder = "";
        rebin = 1;
        xmax = 2000;
        ymin = 0;
        ymax = 10; //2 for TTZ, 10 for Data
        if (plot_log)
        {
            ymin = 0.1;
            ymax = 100;
        }
	}
	if (plot_njet)
	{
		title = "nJets";
		var = "h_njet";
		folder = "";
		rebin = 1;
		ymin = 1;
		ymax = 1000;
	}
	if (plot_njetpt20eta24)
	{
		title = "nJets pt > 20 eta < 2.4";
		var = "h_njetpt20eta24";
		folder = "";
		rebin = 1;
        ymin = 0;
        ymax = 20; //10 for TTZ, 20 for Data
		if (plot_log)
        {
            ymin = 1;
		    ymax = 1000;
        }
	}
	if (plot_njetpt40)
	{
		title = "nJets pt > 40";
		var = "h_njetpt40";
		folder = "";
		rebin = 1;
        ymin = 0;
        ymax = 30; //15 for TTZ, 30 for Data
		if (plot_log)
        {
            ymin = 1;
		    ymax = 1000;
        }
	}
	if (plot_Stop0l_nJets)
	{
		title = "Stop0l nJets";
		var = "h_Stop0l_nJets";
		folder = "";
		rebin = 1;
		ymin = 1;
		ymax = 1000;
	}
    if (plot_num_mu)
    {
        title = "nMuons";
        var = "h_num_mu";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 30; //20 for TTZ, 30 for Data
        if (plot_log)
        {
            ymin = 1;
            ymax = 50;
        }
    }
    if (plot_mu_indices)
    {
        title = "Muon collection index";
        var = "h_mu_indices";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 25; //16 for TTZ, 25 for Data
        if (plot_log)
        {
            ymin = 0.1;
            ymax = 1000;
        }
    }
	if(plot_lep1pT)
	{
		title = "pT of Leading Lepton in Reconstructed Z";
		var = "h_lep1pT";
		folder = "";
		rebin = 1;
		ymin = 0;
        ymax = 15; //4 for TTZ, 15 for Data
        if (plot_log)
        {
            ymin = 0.01;
		    ymax = 100;
        }
	}
    if(plot_lep2pT)
    {
        title = "pT of Non-Leading Lepton in Reconstructed Z";
        var = "h_lep2pT";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 15;
        if (plot_log)
        {
            ymin = 0.01;
            ymax = 100;
        }
    }
    if(plot_lep3pT)
    {
        title = "pT of Third Lepton";
        var = "h_lep3pT";
        folder = "";
        rebin = 1;
        ymax = 15;
        if (plot_log)
        {
            ymin = 0.01;
            ymax = 100;
        }
    }
    if(plot_numZ)
    {
        title = "Normalization";
        var = "h_normalization";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 100;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_numZ_noweights)
    {
        title = "Normalization (no weights)";
        var = "eff_h";
        folder = "Baseline_Only/";
        rebin = 1;
        xmin = 1;
        ymin = 0;
        ymax = 150;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_1mu)
    {
        title = "Normalization: 1 muon";
        var = "h_norm_1mu";
        folder = "";
        rebin = 1;
        ymin = 0;
        if (plot_TTZ) ymax = 15;
        if (plot_Data) ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_2mu)
    {
        title = "Normalization: 2 muons";
        var = "h_norm_2mu";
        folder = "";
        rebin = 1;
        ymin = 0;
        if (plot_TTZ) ymax = 15;
        if (plot_Data) ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_3mu)
    {
        title = "Normalization: 3 muons";
        var = "h_norm_3mu";
        folder = "";
        rebin = 1;
        ymin = 0;
        if (plot_TTZ) ymax = 15;
        if (plot_Data) ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_cutflow)
    {
        title = "Cutflow";
        var = "cutflow_h";
        folder = "Baseline_Only/";
        rebin = 1;
        ymin = 0;
        ymax = 10000; //full (TTZ)
        //ymax = 1000; //trimmed
        //ymax = 200; //trimmed more
        if (plot_log)
        {
            ymin = 1; 
            ymax = 100000000;
        }
    }


	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.6,leg_height,0.9,0.89);
    //TLegend* leg = new TLegend(0.3,0.89-leg_height);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);

	TPad *padup = new TPad("padup", "padup", 0, padup_height, 1, 1.0);
	padup -> SetBottomMargin(padup_margin);
	padup -> Draw();
	padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();

    for(int i = 0; i < years.size(); i++)
    {
        year = years[i];
        if (plot_TTZ)
        {
            TH1D * pro = NULL;

            if (true)
            {
                    TString sp = "TTZToLLNuNu";

                    TFile *f1 = new TFile("results/" + sp + year + ".root");
                    TH1D *h1 = (TH1D*)f1->Get(folder + var);
                    TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

                    double all_events = h2->GetBinContent(1);
                    double left_events = h2->GetBinContent(2);

                    h1->Sumw2();
                    h1->Rebin(rebin);

                    std::cout << sp << std::endl;
                    std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
                    std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
                    std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
                    std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

                    h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
                    //h1->Scale(lumi * 1000);

                    std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
                    std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
                    std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
                    std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

                    pro = (TH1D*)h1->Clone("ttztollnunu");
            }


            if (i == 0) pro->SetLineColor(kBlack);
            if (i == 1) pro->SetLineColor(kRed);
            if (i == 2) pro->SetLineColor(kGreen);
            if (i == 3) pro->SetLineColor(kBlue);
            //pro->SetLineWidth(3);
            if (plot_met) pro->GetXaxis()->SetRangeUser(xmin,xmax);
            pro->GetYaxis()->SetRangeUser(ymin,ymax);
            pro->GetXaxis()->SetTitle(title);
            pro->GetYaxis()->SetTitleSize(0.045);
            pro->GetYaxis()->SetTitleOffset(1.2);
            //pro->Draw("bsame");
            //pro->SetFillColor(kViolet);
            leg->AddEntry(pro,"ttz" + year,"l");
            if (i == 0) pro->Draw();
            else pro->Draw("same");
        }

        if (plot_Rare)
        {
            TH1D * pro = NULL;

            if (true)
            {
                TString sp = "ttHToNonbb";

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
                std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
                std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
                std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

                h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
                //h1->Scale(lumi * 1000);

                std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
                std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
                std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
                std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

                pro = (TH1D*)h1->Clone("Rare");
            }

            if (true)
            {
                TString sp = "VHToNonbb";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }

            if (year != "_2017")
            {
                TString sp = "GluGluHToZZTo4L";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }

            if (year != "_2017")
            {
                TString sp = "ST_tWll";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }

            if (true)
            {
                TString sp = "ST_tZq_ll";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "TTWJetsToLNu";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "WZTo3LNu";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "ZZTo4L";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (year != "_2017")
            {
                TString sp = "WWZ";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "WZZ";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "ZZZ";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "WZG";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "WWG";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "WWW";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }
            if (true)
            {
                TString sp = "TTTT";
                Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
            }

            pro->SetLineColor(kBlack);
            //pro->SetLineWidth(3);
            //pro->Draw("bsame");
            pro->SetFillColor(kBlue);
            leg->AddEntry(pro,"Rare","f");

        }


        if (plot_NonPrompt)
        {
            TH1D * pro = NULL;

            if (true)
            {
                    TString sp = "DYJetsToLL_HT_70to100";

                    TFile *f1 = new TFile("results/" + sp + year + ".root");
                    TH1D *h1 = (TH1D*)f1->Get(folder + var);
                    //TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
                    TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

                    if(use_original_title) title = h1->GetTitle();

                    double all_events = h2->GetBinContent(1);
                    double left_events = h2->GetBinContent(2);

                    h1->Sumw2();
                    h1->Rebin(rebin);

                    std::cout << sp << std::endl;
                    std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
                    std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
                    std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
                    std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

                    h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
                    //h1->Scale(lumi * 1000);

                    std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
                    std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
                    std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
                    std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

                    pro = (TH1D*)h1->Clone("DY");
            }

            if (true)
            {
                    TString sp = "DYJetsToLL_HT_100to200";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_200to400";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_400to600";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_600to800";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_800to1200";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_1200to2500";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "DYJetsToLL_HT_2500toInf";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }
            if (true)
            {
                    TString sp = "TTbarDiLep";
                    Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kOrange+1, pro, rebin);
            }

            pro->SetLineColor(kBlack);
            //pro->SetLineWidth(3);
            //pro->Draw("bsame");
            pro->SetFillColor(kOrange+1);
            leg->AddEntry(pro,"NonPrompt","f");

            //hs->Add(pro);
        }
    }
/*
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
		hs->GetYaxis()->SetTitleSize(0.045);
		hs->GetYaxis()->SetTitleOffset(1.2);

		if(plot_BG)
		{
			hs_tot = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral();
			int bin_xmin = ((TH1D*)(hs -> GetStack() -> Last())) -> FindBin(xmin);
			int bin_xmax = ((TH1D*)(hs -> GetStack() -> Last())) -> FindBin(xmax);
			hs_tot_plot = ((TH1D*)(hs -> GetStack() -> Last())) -> Integral(bin_xmin,bin_xmax);
			double hs_bin1 = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(1);
			double hs_bin2 = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(2);
			std::cout << "\ntotal BG = " << hs_tot << " bin 1 = " << hs_bin1 << " bin 2 = " << hs_bin2 << "\n" << std::endl;
			//int n_bins = 80/rebin;
			//TH1D *significance = new TH1D ("significance", "significance", n_bins, xmin, xmax);
		}
	}
*/
	TString sp;
	double signal_scale = lumi;

    for(int i = 0; i < years.size(); i++)
    {
        year = years[i];
        if (plot_Data)
        {
            if(data_name == "SingleMuon") sp = "Data_SingleMuon";
            
            TFile *f1 = new TFile(result_path + sp + year + ".root");
            TH1D *h1 = (TH1D*)f1->Get(folder + var);
            TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
            
            double all_events  = h2->GetBinContent(1);
            double left_events = h2->GetBinContent(2);

            std::cout << "left_events = " << left_events << std::endl;
            std::cout << "GetEntries  = " << h1->GetEntries() << std::endl;
            std::cout << "GetIntegral = " << h1->Integral() << std::endl;
            std::cout << "all_events  = " << all_events << std::endl;

            h1->Rebin(rebin);
            //h1->SetLineColor(kBlack);
            if (i == 0) h1->SetLineColor(kBlack);
            if (i == 1) h1->SetLineColor(kRed);
            if (i == 2) h1->SetLineColor(kGreen);
            if (i == 3) h1->SetLineColor(kBlue);
            h1->SetLineWidth(2);

            /*
            //if(plot_BG) h1->Draw("hist same");
            if(plot_BG) h1->Draw("same P L E"); //Add "TEXT" for written number vals. P: point; L: horiz. line; E: error bars
            else
            {
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
            */
            //if(plot_recoZpt) h1->GetYaxis()->SetRangeUser(0,10);
            h1->GetYaxis()->SetRangeUser(ymin,ymax);
            if(i==0) h1->Draw("ple");
            else h1->Draw("ple same");
            leg->AddEntry(h1,sp + year,"l");
        }
    }


	TLatex latex;
	latex.SetTextSize(0.04);
	latex.SetNDC();
	//latex.SetTextAlign(13);  //align at top
	//latex.DrawLatex(0.5,ymax+0.4,"#bf{CMS} Preliminary, 2017 data");
	latex.DrawLatex(0.1,0.91,"CMS #bf{Simulation}");
	TString lumi_and_energy = "#bf{" + std::to_string((int)lumi) + " fb^{-1} (13TeV)}";
	if(plot_BG) latex.DrawLatex(0.74,0.91,lumi_and_energy);
	else latex.DrawLatex(0.80,0.91,"#bf{13TeV}");

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
            for (int it = 1; it < years.size(); it++)
            {
                year = years[it];
                if(plot_TTZ) sp = "TTZToLLNuNu";
                if(plot_Data) sp = "Data_SingleMuon";

                TFile *f1 = new TFile(result_path + sp + year + ".root");
                TH1D *h1 = (TH1D*)f1->Get(folder + var);
                TH1D *h1_b = (TH1D*)f1->Get("Baseline_Only/eff_h");
                TFile *f2 = new TFile(result_path + sp + years[0] + ".root");
                TH1D *h2 = (TH1D*)f2->Get(folder + var);
                TH1D *h2_b = (TH1D*)f2->Get("Baseline_Only/eff_h");


                double all_events_1 = h1_b->GetBinContent(1);
                double all_events_2 = h2_b->GetBinContent(1);
               
                if(plot_TTZ)
                { 
                    h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events_1 );
                    h2->Scale(lumi * CrossSection.at(sp) * 1000 / all_events_2 );
                }
                if(plot_Data)
                {
                    //if (year == "_2017") h2->Scale(36/42);
                }
                std::cout << sp << " " << year << " " << folder << " " << var << std::endl;

                h1->Sumw2();
                h1->Rebin(rebin);

                TH1D *ratio = (TH1D*)h1->Clone("ratio");
                int n_bins = ratio->GetSize() - 2;

                std::cout << "\n" << sp << std::endl;

                for (int i = 1; i <= n_bins; i++)
                {
                    double numerator = h1->GetBinContent(i);
                    double denominator = h2 -> GetBinContent(i);
                    if (denominator < 0) denominator = 0;
                    if (denominator != 0)
                    {
                        double data_bg_ratio = numerator / denominator;
                        ratio -> SetBinContent(i,data_bg_ratio);
                    }
                    else ratio -> SetBinContent(i,0);

                    std::cout << "bin " << i << " numerator = " << numerator << " denominator = " << denominator << " ratio = " << ratio->GetBinContent(i) << std::endl;
                }

                ratio->SetTitle("");
                ratio->GetYaxis()->SetTitle("ratio");
                if (plot_met) ratio->GetXaxis()->SetRangeUser(xmin,xmax);
                ratio->GetXaxis()->SetTitle(title);
                ratio->GetXaxis()->SetTitleSize(0.1);
                ratio->GetXaxis()->SetLabelSize(0.08);
                ratio->GetYaxis()->SetTitleOffset(0.5);
                ratio->GetYaxis()->SetTitleSize(0.1);
                ratio->GetYaxis()->SetLabelSize(0.08);
                if (plot_num_mu || plot_numZ || plot_norm_1mu || plot_norm_2mu || plot_norm_3mu || plot_mu_indices) ratio->GetYaxis()->SetRangeUser(0.8,1.2);

                //ratio->SetMaximum(sig_ymax);
                //ratio->SetMaximum(1);
                //ratio->SetLineColor(kRed);
                //ratio->SetLineColor(kBlack);
                if (it == 0) ratio->SetLineColor(kBlack);
                if (it == 1) ratio->SetLineColor(kRed);
                if (it == 2) ratio->SetLineColor(kGreen);
                if (it == 3) ratio->SetLineColor(kBlue);
                ratio->SetLineWidth(2);
                if (it == 1) ratio->Draw("hist");
                else ratio->Draw("hist same");
                //ratio->Draw("PLE");
                //leg->AddEntry(h1,sp,"l");
            }
		}
	}

    if (plot_log)
    {
        if (plot_Data) mycanvas->SaveAs("plots/" + data_name + "_" + var + "_logY.png");
        if (plot_TTZ) mycanvas->SaveAs("plots/TTZ_" + var + "_logY.png");
    }
    else
    {
        if (plot_Data) mycanvas->SaveAs("plots/" + data_name + "_" + var + ".png");
        if (plot_TTZ) mycanvas->SaveAs("plots/TTZ_" + var + ".png");
    }

	return 0;
}

