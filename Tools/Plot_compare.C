int Plot_compare()
{
	const bool scale_no_weight = true;
	const bool plot_log = true;
	//std::vector<TString> hist_list = {"eff_h"};
	std::vector<TString> hist_list = {"loose_baseline_cutflow_h"};
	//std::vector<TString> hist_list = {"met_uc_h","HT_uc_h", "mtb_uc_h", "ntop_merge_h", "ntop_res_uc_h", "nw_h", "njetspt20_uc_h", "nbottompt20_uc_h"};
	//std::vector<TString> hist_list = {"met_h","HT_h", "mtb_h", "ntop_merge_h", "ntop_res_h", "nw_h", "njetspt20_h", "nbottompt20_h"};
	//std::vector<TString> hist_list = {"loose_baseline_cutflow_h", "ISR_SF_h", "B_SF_h"};

	//TString f1_name = "SMS_T2tt_mStop850_mLSP100_fullsim_2016_v2p6"; 
	//TString f2_name = "SMS-T2tt_mStop-850_mLSP-100_JEC"; 
	//TString f2_name = "SMS-T2tt_mStop-850_mLSP-100_wrong_JEC"; 
	TString f1_name = "TTbarSingleLepT_2016_v2p6"; 
	TString f2_name = "TTbarSingleLepT_2016_old"; 

	TFile *f1 = new TFile("results_test/" + f1_name + ".root");
	TFile *f2 = new TFile("results_test/" + f2_name + ".root");

	TString hist_weight = "Baseline_Only/eff_h";
	TH1F *h1_weight = (TH1F*)f1->Get(hist_weight);
	TH1F *h2_weight = (TH1F*)f2->Get(hist_weight);
	float f1_tot_weight = h1_weight->GetBinContent(1);
	float f2_tot_weight = h2_weight->GetBinContent(1);

	TString hist_no_weight = "loose_baseline_cutflow_h";
	TH1F *h1_no_weight = (TH1F*)f1->Get(hist_no_weight);
	TH1F *h2_no_weight = (TH1F*)f2->Get(hist_no_weight);
	int f1_tot_no_weight = h1_no_weight->GetBinContent(1);
	int f2_tot_no_weight = h2_no_weight->GetBinContent(1);

	float tot_scale = (float)f1_tot_weight/(float)f2_tot_weight;
	if(scale_no_weight) tot_scale = (float)f1_tot_no_weight/(float)f2_tot_no_weight;

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
		h1->SetLineColor(kRed);
		h1->Draw("hist");

		TH1F *h2 = (TH1F*)f2->Get(hist);

		TString leg_new = "new (" + std::to_string(f1_tot_no_weight) + ")";
		TString leg_old = "old (" + std::to_string(f2_tot_no_weight) + ")";

		h2->Scale(tot_scale);

		h2->SetLineColor(kBlue);
		h2->Draw("histsame");

		//float y_max = max(h1->GetMaximum(), h2->GetMaximum()) * 1.2;
		//h1->SetMaximum(y_max);

		TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
		leg->AddEntry(h1,leg_new,"l");
		leg->AddEntry(h2,leg_old,"l");
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
		ratio->Draw("hist");
		//ratio->SetMaximum(ratio->GetMaximum()*1.2);
		ratio->GetXaxis()->SetLabelSize(0.12);
		ratio->GetYaxis()->SetLabelSize(0.08);

		//mycanvas->SetLogy();

		mycanvas->SaveAs("plots_test/" + f1_name + "_" + hist_name + ".png");
	}

	return 0;
}
