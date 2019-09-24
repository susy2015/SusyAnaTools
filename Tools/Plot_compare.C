int Plot_compare()
{
	const bool plot_log = false;
	std::vector<TString> hist_list = {"met_uc_h"};
	//std::vector<TString> hist_list = {"loose_baseline_cutflow_h"};
	//std::vector<TString> hist_list = {"met_h","HT_h", "mtb_h", "ntop_merge_h", "ntop_res_h", "nw_h", "njetspt20_h", "nbottompt20_h"};
	//std::vector<TString> hist_list = {"met_uc_h","HT_uc_h", "mtb_uc_h", "ntop_merge_h", "ntop_res_uc_h", "nw_h", "njetspt20_uc_h", "nbottompt20_uc_h"};
	//std::vector<TString> hist_list = {"loose_baseline_cutflow_h", "ISR_SF_h", "B_SF_h"};

	//TString f1_name = "SMS_T2tt_mStop_175_mLSP_1_fullsim_2017"; 
	//TString f1_name = "SMS_T2tt_mStop_250_mLSP_50_fullsim_2017"; 
	//TString f1_name = "SMS_T2tt_mStop_250_mLSP_75_fullsim_2017"; 
	TString f1_name = "SMS_T2tt_mStop_250_mLSP_100_fullsim_2017"; 
	TString f1_lag = "fullsim"; 
	//TString f2_name = "SMS_T2tt_mStop175_mLSP0_fastsim_2017"; 
	//TString f2_name = "SMS_T2tt_mStop250_mLSP50_fastsim_2017"; 
	//TString f2_name = "SMS_T2tt_mStop250_mLSP75_fastsim_2017"; 
	TString f2_name = "SMS_T2tt_mStop250_mLSP100_fastsim_2017"; 
	TString f2_lag = "fastsim";
	int rebin = 1; 

	TFile *f1 = new TFile("results/" + f1_name + ".root");
	TFile *f2 = new TFile("results/" + f2_name + ".root");

	TString hist_raw = "Baseline_Only/eff_h";
	TH1F *h1_raw = (TH1F*)f1->Get(hist_raw);
	TH1F *h2_raw = (TH1F*)f2->Get(hist_raw);
	float f1_tot_raw = h1_raw->GetBinContent(1);
	float f2_tot_raw = h2_raw->GetBinContent(1);

	float tot_scale = (float)f1_tot_raw/(float)f2_tot_raw;
	std::cout << "f1_tot_raw " << f1_tot_raw << " f2_tot_raw " << f2_tot_raw << " f2 scale " << tot_scale << std::endl;

	for(int i = 0; i < hist_list.size(); i++)
	{
		TString hist_folder = "";
		TString hist_name = hist_list.at(i);
		if (hist_name == "eff_h") hist_folder = "Baseline_Only/";
		TString hist = hist_folder + hist_name;

		TCanvas* mycanvas = new TCanvas();
		gStyle->SetOptStat(kFALSE);
		TPad *padup = new TPad("padup", "padup", 0, 0.3, 1, 1.0);
		padup -> SetBottomMargin(0);
		padup -> Draw();
		padup -> cd();

		if(plot_log) gPad-> SetLogy();

		TH1F *h1 = (TH1F*)f1->Get(hist);
		h1->Rebin(rebin);
		h1->SetLineColor(kRed);
		if(hist_name == "met_gen_uc_h" || hist_name == "met_uc_h") h1->GetXaxis()->SetRangeUser(0,400);
		h1->Draw("el");
		h1->SetLineWidth(3);

		TH1F *h2 = (TH1F*)f2->Get(hist);
		if(hist_name == "met_h")
		{
			h1->GetXaxis()->SetRangeUser(0,800);
			TH1F *h2_gen = (TH1F*)f2->Get("met_gen_h");
			for(int i = 1; i <= h2->GetSize() - 2; i++)
			{
				float origin_err = h2->GetBinError(i);
				float fastsim_err = fabs(h2->GetBinContent(i) - h2_gen->GetBinContent(i));
				//std::cout << "bin " << i << " origin_err " << origin_err << " fastsim_err " << fastsim_err << std::endl;
				h2->SetBinError(i, sqrt(origin_err * origin_err + fastsim_err * fastsim_err));
			}
		}
		h2->Rebin(rebin);

		h2->Scale(tot_scale);
		float y_max = max(h1->GetMaximum(), h2->GetMaximum()) * 1.2;
		h1->SetMaximum(y_max);

		TString f1_lag_temp = f1_lag;// + " (" + std::to_string(int(h1->Integral())) + ")";
		TString f2_lag_temp = f2_lag;// + " (" + std::to_string(int(h2->Integral())) + ")";

		h2->SetLineColor(kBlue);
		h2->Draw("histsame");
		h2->SetLineWidth(3);

		TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
		leg->AddEntry(h1,f1_lag_temp,"l");
		leg->AddEntry(h2,f2_lag_temp,"l");
		leg->Draw("same");

		mycanvas -> cd();
		TPad *paddown = new TPad("paddown", "paddown", 0, 0, 1, 0.3);
		paddown -> SetTopMargin(0);
		paddown -> SetBottomMargin(0.3);
		paddown -> SetGrid();
		paddown -> Draw();
		paddown -> cd();

		TH1F *ratio = (TH1F*)h1->Clone();
		ratio->SetTitle("");
		ratio->Sumw2();
		ratio->Divide(h2);
		ratio->Draw("el");
		ratio->SetMinimum(0);
		ratio->SetMaximum(2);
		ratio->GetXaxis()->SetLabelSize(0.12);
		ratio->GetYaxis()->SetLabelSize(0.08);

		mycanvas->SaveAs("plots_test/" + f1_name + "_" + hist_name + ".png");
	}

	return 0;
}
