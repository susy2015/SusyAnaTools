#include "CrossSection.C"
#include "Plot_1D_AUX.c"
int Plot_1D_TTZ_norm()
{
	//gROOT->ForceStyle();

	double lumi = 36; //2016
	//lumi = 42; //2017
	//lumi = 59; //2018
	TString year = "_2018";
    TString outputfile = "output" + year + "_norm.root";
	bool plot_log = false;
	bool plot_sig_pad = true;
	bool plot_BG = true;
    bool PostHEM = true;
	bool use_low_stat_sig = false;
	//use_low_stat_sig = true;
	bool use_original_title = false;

    if(year == "_2016") lumi = 36;
    if(year == "_2017") lumi = 42;
    if(year == "_2018")
    {
        if (PostHEM)
        {
            lumi = 38;
            outputfile = "output_2018_PostHEM_norm.root";
        }
        else lumi = 21;
    }

	TString result_path = "results/";
	TString data_name = "SingleMuon";
    //TString data_name = "EGamma";

    bool plot_recoZpt = false;
    bool plot_recoZM = false;
    bool plot_met = false;
    bool plot_ht = false;
    bool plot_njet = false;
    bool plot_njetpt20eta24 = false;
    bool plot_njetpt30eta24 = false;
    bool plot_njetpt40eta24 = false;
    bool plot_njetpt40 = false;
    bool plot_Stop0l_nJets = false;
    bool plot_num_mu = false;
    bool plot_num_mu_notrigwt = false;
    bool plot_num_mu_trimmed = false;
    bool plot_num_elec = false;
    bool plot_mu_indices = false;
    bool plot_lep1pT = false;
    bool plot_lep2pT = false;
    bool plot_lep3pT = false;
    bool plot_norm = false;
    bool plot_norm_eta_eff = false;
    bool plot_norm_notrigwt = true;
    bool plot_numZ_noweights = false;
    bool plot_norm_pt_eff = false;
    bool plot_norm_trimmed = false;
    bool plot_norm_1mu = false;
    bool plot_norm_2mu = false;
    bool plot_norm_3mu = false;
    bool plot_norm_1elec = false;
    bool plot_norm_2elec = false;
    bool plot_norm_3elec = false;
    bool plot_recoZM_stripped = false;
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
        ymax = 10;
        if (plot_log)
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
        ymax = 10;
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
        ymax = 10;
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
        ymax = 10;
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
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
	}
	if (plot_njetpt30eta24)
	{
		title = "nJets pt > 30 eta < 2.4";
		var = "h_njetpt30eta24";
		folder = "";
		rebin = 1;
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
	}
	if (plot_njetpt40eta24)
	{
		title = "nJets pt > 40 eta < 2.4";
		var = "h_njetpt40eta24";
		folder = "";
		rebin = 1;
        ymax = 30;
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
        ymax = 30;
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
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if (plot_num_mu_notrigwt)
    {
        title = "nMuons: No Trigger SF";
        var = "h_num_mu_notrigwt";
        folder = "";
        rebin = 1;
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if (plot_num_mu_trimmed)
    {
        title = "nMuons (trimmed bins)";
        var = "h_num_mu_trimmed";
        folder = "";
        rebin = 1;
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if (plot_num_elec)
    {
        title = "nElectrons";
        var = "h_num_elec";
        folder = "";
        rebin = 1;
        ymax = 30;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if (plot_mu_indices)
    {
        title = "Muon collection index";
        var = "h_mu_indices";
        folder = "";
        rebin = 1;
        ymax = 20;
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
        ymax = 20;
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
        ymax = 20;
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
        ymax = 20;
        if (plot_log)
        {
            ymin = 0.01;
            ymax = 100;
        }
    }
    if(plot_norm)
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
    if(plot_norm_notrigwt)
    {
        title = "Normalization: No Trigger SF";
        var = "h_norm_notrigwt";
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
    if(plot_norm_pt_eff)
    {
        title = "Normalization using pt-based efficiency";
        var = "h_norm_pt_eff";
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
    if(plot_norm_eta_eff)
    {
        title = "Normalization using eta-based efficiency";
        var = "h_norm_eta_eff";
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
    if(plot_norm_trimmed)
    {
        title = "Normalization without (88,90] GeV mass range";
        var = "h_norm_trimmed";
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
    if(plot_norm_1mu)
    {
        title = "Normalization: 1 muon";
        var = "h_norm_1mu";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 40;
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
        ymax = 40;
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
        ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_1elec)
    {
        title = "Normalization: 1 electron";
        var = "h_norm_1elec";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_2elec)
    {
        title = "Normalization: 2 electrons";
        var = "h_norm_2elec";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_norm_3elec)
    {
        title = "Normalization: 3 electrons";
        var = "h_norm_3elec";
        folder = "";
        rebin = 1;
        ymin = 0;
        ymax = 40;
        if (plot_log)
        {
            ymin = 1;
            ymax = 1000;
        }
    }
    if(plot_recoZM_stripped)
    {
        title = "ZMass with removed bins";
        var = "h_recoZM_stripped";
        folder = "";
        rebin = 2;
        ymin = 0;
        ymax = 40;
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
        ymin = 1; 
        ymax = 1000000000;
        plot_log = true;
    }


	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.6,leg_height,0.9,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);

	TPad *padup = new TPad("padup", "padup", 0, padup_height, 1, 1.0);
	padup -> SetBottomMargin(padup_margin);
	padup -> Draw();
	padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();
    TH1D *h_TTZ = NULL;
    TH1D *h_other = NULL;
    TH1D *h_Data = NULL;
    TH1D *h_SF = NULL;


    if (plot_BG)
    {
        TH1D * pro = NULL;

        if (true)
        {
                TString sp = "TTZToLLNuNu";

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

                pro = (TH1D*)h1->Clone("ttztollnunu");
                h_TTZ = (TH1D*)h1->Clone("TTZ");
        }


		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kViolet);
		leg->AddEntry(pro,"ttz","f");

		hs->Add(pro);
	}
    if (plot_BG)
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

		if (true) //prev not in 2017
		{
			TString sp = "GluGluHToZZTo4L";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
		}

		if (true) //prev not in 2017
		{
			TString sp = "ST_tWll";
			Plot_1D_AUX_bg (lumi, sp, year, var, folder, leg, kGreen, pro, rebin);
		}

		if (year != "_2017") //now not in 2017 :(
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
        if (true) //prev not in 2017
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

        h_other = (TH1D*)pro->Clone("other");

        pro->SetLineColor(kBlack);
        //pro->SetLineWidth(3);
        //pro->Draw("bsame");
        pro->SetFillColor(kBlue);
        leg->AddEntry(pro,"Rare","f");

        hs->Add(pro);
    }


    if (plot_BG)
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

        h_other->Add(pro);

        pro->SetLineColor(kBlack);
        //pro->SetLineWidth(3);
        //pro->Draw("bsame");
        pro->SetFillColor(kOrange+1);
        leg->AddEntry(pro,"NonPrompt","f");

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

	TString sp;
	double signal_scale = lumi;

	if (true)
	{
		if(data_name == "SingleMuon") sp = "Data_SingleMuon";
        if(data_name == "EGamma") sp = "Data_EGamma";

        TFile *f1 = new TFile();	    
        if (year == "_2018")
        {
            if(PostHEM) f1 = new TFile(result_path + sp + year + "_PostHEM.root");
            else f1 = new TFile(result_path + sp + year + "_PreHEM.root");
        }	
		else f1 = new TFile(result_path + sp + year + ".root");
		TH1D *h1 = (TH1D*)f1->Get(folder + var);
		TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
		
		double all_events  = h2->GetBinContent(1);
		double left_events = h2->GetBinContent(2);

		std::cout << "left_events = " << left_events << std::endl;
		std::cout << "GetEntries  = " << h1->GetEntries() << std::endl;
		std::cout << "GetIntegral = " << h1->Integral() << std::endl;
		std::cout << "all_events  = " << all_events << std::endl;

        h_Data = (TH1D*)h1->Clone("Data");

		h1->Rebin(rebin);
		h1->SetLineColor(kBlack);
		h1->SetLineWidth(2);

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
        leg->AddEntry(h1,sp,"l");
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
			if(data_name == "SingleMuon") sp = "Data_SingleMuon";
            if(data_name == "EGamma") sp = "Data_EGamma";
            TFile *f1 = new TFile();
            if (year == "_2018")
            {
                if(PostHEM) f1 = new TFile(result_path + sp + year + "_PostHEM.root");
                else f1 = new TFile(result_path + sp + year + "_PreHEM.root");
            }	
            else f1 = new TFile(result_path + sp + year + ".root");
			TH1D *h1 = (TH1D*)f1->Get(folder + var);
			TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);
			double left_events = h2->GetBinContent(2);

			h1->Sumw2();
			h1->Rebin(rebin);

			TH1D *ratio = (TH1D*)h1->Clone("ratio");
			int n_bins = ratio->GetSize() - 2;
            float TTZ_ratio = 1.0;

			std::cout << "\n" << sp << std::endl;

            for (int i = 1; i <= n_bins; i++)
            {
                double data = h1->GetBinContent(i);
                double back_ground = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(i);
                if (back_ground < 0) back_ground = 0;
				if (back_ground != 0)
				{
					double data_bg_ratio = data / back_ground;
					ratio -> SetBinContent(i,data_bg_ratio);
                    if (i==2 && (plot_norm || plot_numZ_noweights || plot_norm_notrigwt))
                    {
                        TTZ_ratio = data_bg_ratio;
                        TString TTZ_ratio_str = std::to_string((float)TTZ_ratio);
                        std::cout << TTZ_ratio_str << std::endl;
                        latex.DrawLatex(0.3,0.3,"#bf{" + TTZ_ratio_str + "}");
                    }
				}
                //double signal_uc = sqrt (pow(h1->GetBinError(i),2) + pow(0.16*signal,2));  //signal sys unc = 16%
                //double back_ground_uc = sqrt (pow(((TH1D*)(hs -> GetStack() -> Last())) -> GetBinError(i),2) + pow(0.2*back_ground,2));  //bg sys unc = 20%
                //if (back_ground == 0) back_ground_uc = 1.8;  //unc for empty bin is [0,1.8]
                //double sigma = signal + back_ground + signal_uc*signal_uc + back_ground_uc*back_ground_uc;
                //double ratio = 0;
                //if (sigma > 0) ratio = signal / sqrt(sigma);
                //double minus = 2 * sqrt (sigma) - 2 * sqrt (back_ground + back_ground_uc*back_ground_uc);
                //if (use_low_stat_sig) significance->SetBinContent(i,minus);
                //else significance->SetBinContent(i,ratio);

                std::cout << "bin " << i << " data = " << data << " BG = " << back_ground << " ratio = " << ratio->GetBinContent(i) << std::endl;
            }

            ratio->SetTitle("");
            ratio->GetYaxis()->SetTitle("ratio");
            ratio->GetXaxis()->SetRangeUser(xmin,xmax);
            ratio->GetXaxis()->SetTitle(title);
            ratio->GetXaxis()->SetTitleSize(0.1);
            ratio->GetXaxis()->SetLabelSize(0.08);
            ratio->GetYaxis()->SetTitleOffset(0.5);
            ratio->GetYaxis()->SetTitleSize(0.1);
            ratio->GetYaxis()->SetLabelSize(0.08);

            //ratio->SetMaximum(sig_ymax);
            //ratio->SetMaximum(1);
            //ratio->SetLineColor(kRed);
            ratio->SetLineColor(kBlack);
            ratio->SetLineWidth(2);
            ratio->Draw("hist");
            //ratio->Draw("PLE");
            //leg->AddEntry(h1,sp,"l");
		}

		if (false)
		{
            if (data_name == "SingleMuon") sp = "Data_SingleMuon";
            if(data_name == "EGamma") sp = "Data_EGamma";

            TFile *f1 = new TFile();
            if (year == "_2018")
            {
                if(PostHEM) f1 = new TFile(result_path + sp + year + "_PostHEM.root");
                else f1 = new TFile(result_path + sp + year + "_PreHEM.root");
            }	
            else f1 = new TFile(result_path + sp + year + ".root");
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
			//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
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

	}

    h_SF = (TH1D*)h_Data->Clone("SF");
    h_SF->Add(h_other,-1);
    h_SF->Divide(h_TTZ);
    std::cout << "Data: " << h_Data->GetBinContent(2) << std::endl;
    std::cout << "TTZ: " << h_TTZ->GetBinContent(2) << std::endl;
    std::cout << "Other: " << h_other->GetBinContent(2) << std::endl;
    std::cout << "SF: " << h_SF->GetBinContent(2) << " +/- " << h_SF->GetBinError(2) << std::endl;

    TFile out_file(outputfile,"RECREATE");
    h_Data->Write();
    h_TTZ->Write();
    h_other->Write();
    h_SF->Write();
    out_file.Close();

	if(use_low_stat_sig)
	{
		//mycanvas->SetLogy();
		if (year == "_2018")
        {
            if (PostHEM) mycanvas->SaveAs("plots/" + data_name + "_" + var + year + "_PreHEM_use_low_stat_sig.png");
		    else mycanvas->SaveAs("plots/" + data_name + "_" + var + year + "_PostHEM_use_low_stat_sig.png");
        }
		else mycanvas->SaveAs("plots/" + data_name + "_" + var + year + "_use_low_stat_sig.png");
	}
	else
    {
		if (year == "_2018") 
        {
            if (PostHEM) mycanvas->SaveAs("plots/" + data_name + "_" + var + year + "_PostHEM.png");
		    else mycanvas->SaveAs("plots/" + data_name + "_" + var + year + "_PreHEM.png");
		}
        else mycanvas->SaveAs("plots/" + data_name + "_" + var + year + ".png");
    }

	return 0;
}

