int Plot_compare_validation()
{
	const bool plot_log = true;

	bool plot_SBv3 = false;
	bool plot_SBv4 = false;
	bool plot_zinv_highdm = false;
	bool plot_LL_lowdm = false;
	bool plot_LL_highdm = false;
	bool plot_data_Matt_lowdm = true;
	bool plot_data_Matt_highdm = false;
	bool plot_data_Dipika_lowdm = false;
	bool plot_data_Dipika_highdm = false;
	bool plot_data_Caleb_lowdm = false;
	bool plot_data_Caleb_highdm = false;
	bool plot_data_Anna_lowdm = false;
	bool plot_data_Anna_highdm = false;
	bool plot_met_LLCR_lowdm = false;
	bool plot_met_LLCR_highdm = false;

	TString year = "";
	//year = "_2016";
	TString folder = "";
	//folder = "validation_2016/";

	TString f1_name = "Zinv_validation_lowdm_2016";
	TString hist1 = "zjets"; 
	TString f1_leg = "MC"; 
	TString f2_name = "validationBinsZinv_2016"; 
	TString hist2 = "validation_mc_lowdm";
	TString f2_leg = "Caleb_MC"; 

	if(plot_SBv3)
	{
		f1_name = "T1tttt_2016";
		hist1 = "search_bin_v3_h"; 
		f1_leg = "Mine"; 
		f2_name = "std_pred_SBv3"; 
		hist2 = "T1tttt_2000_100";
		f2_leg = "Matt"; 
	}

	if(plot_SBv4)
	{
		f1_name = "SMS_T1tttt_fastsim_2016";
		hist1 = "search_bin_v4_h"; 
		f1_leg = "Mine"; 
		f2_name = "std_pred_SBv4"; 
		hist2 = "T1tttt";
		f2_leg = "Matt"; 
	}

	if(plot_zinv_highdm)
	{
		f1_name = "Zinv_validation_highdm_2016";
		hist1 = "zjets"; 
		f1_leg = "MC"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "validation_mc_highdm";
		f2_leg = "Caleb_MC"; 
	}

	if(plot_LL_lowdm)
	{
		f1_name = "LL_validation_lowdm_2016";
		hist1 = "LL_SR"; 
		f1_leg = "MC"; 
		f2_name = "std_pred_trad_LM_2016"; 
		//hist2 = "ttbarplusw_pred";
		hist2 = "ttbarplusw_mc";
		//f2_leg = "Matt"; 
		f2_leg = "Matt_MC"; 
	}

	if(plot_LL_highdm)
	{
		f1_name = "LL_validation_highdm_2016";
		hist1 = "LL_SR"; 
		f1_leg = "MC"; 
		f2_name = "std_pred_trad_HM_2016"; 
		//hist2 = "ttbarplusw_pred";
		hist2 = "ttbarplusw_mc";
		//f2_leg = "Matt"; 
		f2_leg = "Matt_MC"; 
	}

	if(plot_data_Matt_lowdm)
	{
		f1_name = "Data_validation_lowdm";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_LM"; 
		hist2 = "data";
		f2_leg = "Matt_data"; 
	}

	if(plot_data_Matt_highdm)
	{
		f1_name = "Data_validation_highdm";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_HM"; 
		hist2 = "data";
		f2_leg = "Matt_data"; 
	}

	if(plot_data_Dipika_lowdm)
	{
		f1_name = "Data_validation_lowdm";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_LM_QCD"; 
		hist2 = "data";
		f2_leg = "Dipika_data"; 
	}

	if(plot_data_Dipika_highdm)
	{
		f1_name = "Data_validation_highdm";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_HM_QCD"; 
		hist2 = "data";
		f2_leg = "Dipika_data"; 
	}

	if(plot_data_Caleb_lowdm)
	{
		f1_name = "Data_validation_lowdm_2016";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "data_lowdm";
		f2_leg = "Caleb_data"; 
	}

	if(plot_data_Caleb_highdm)
	{
		f1_name = "Data_validation_highdm_2016";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "data_highdm";
		f2_leg = "Caleb_data"; 
	}

	if(plot_data_Anna_lowdm)
	{
		f1_name = "Data_validation_lowdm_2016";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "new_Data_MET_2016"; 
		hist2 = "h_low_dm_validation";
		f2_leg = "Anna_data"; 
	}

	if(plot_data_Anna_highdm)
	{
		f1_name = "Data_validation_highdm_2016";
		hist1 = "data"; 
		f1_leg = "Data"; 
		f2_name = "new_Data_MET_2016"; 
		hist2 = "h_high_dm_validation";
		f2_leg = "Anna_data"; 
	}

	if(plot_met_LLCR_lowdm)
	{
		f1_name = "SMS_T2tt_mStop_175_mLSP_1_fullsim_2017_0";
		f1_name = "SMS_T2fbd_mStop600_mLSP520_fastsim_2017";
		f1_name = "SMS_T2fbd_mStop600_mLSP590_fastsim_2017";
		hist1 = "met_lowdm_LLCR_h"; 
		//hist1 = "met_lowdm_LLCR_jetpt30_h"; 
		f1_leg = "LLCR_LM"; 
		//f1_leg = "LLCR_LM_jetpt30"; 
		f2_name = "SMS_T2tt_mStop_175_mLSP_1_fullsim_2017_0"; 
		f2_name = "SMS_T2fbd_mStop600_mLSP520_fastsim_2017";
		f2_name = "SMS_T2fbd_mStop600_mLSP590_fastsim_2017";
		hist2 = "met_lowdm_h";
		//hist2 = "met_lowdm_jetpt30_h";
		f2_leg = "SR_LM"; 
		//f2_leg = "SR_LM_jetpt30"; 
	}

	if(plot_met_LLCR_highdm)
	{
		f1_name = "SMS_T2tt_mStop_175_mLSP_1_fullsim_2017_0";
		//f1_name = "SMS_T1tttt_mGluino2200_mLSP100_fastsim_2016";
		hist1 = "met_highdm_LLCR_h"; 
		f1_leg = "LLCR_HM"; 
		f2_name = "SMS_T2tt_mStop_175_mLSP_1_fullsim_2017_0"; 
		//f2_name = "SMS_T1tttt_mGluino2200_mLSP100_fastsim_2016"; 
		hist2 = "met_highdm_h";
		f2_leg = "SR_HM"; 
	}

	TFile *f1 = new TFile("validation/" + folder + f1_name + year + ".root");
	TFile *f2 = new TFile("validation/" + folder + f2_name + year + ".root");

	TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);
	TPad *padup = new TPad("padup", "padup", 0, 0.3, 1, 1.0);
	padup -> SetBottomMargin(0);
	padup -> Draw();
	padup -> cd();

	if(plot_log) gPad-> SetLogy();

	TH1F *h1 = (TH1F*)f1->Get(hist1);
	std::cout << "hist 1 total bins " << h1->GetSize() - 2 << std::endl;
	h1->SetLineColor(kRed);
	h1->Draw("hist");
	//h1->SetTitle("");
	//h1->GetXaxis()->SetRangeUser(200,500);

	TH1F *h2 = (TH1F*)f2->Get(hist2);
	std::cout << "hist 2 total bins " << h2->GetSize() - 2 << std::endl;
	TH1F *h2_clone = (TH1F*)h1->Clone();
	for(int i = 1; i <= h2->GetSize() - 2; i++)
	{
		h2_clone->SetBinContent(i, h2->GetBinContent(i));
		h2_clone->SetBinError(i, h2->GetBinError(i));
	}
	h2_clone->SetLineColor(kBlue);
	h2_clone->Draw("plsame");

	TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
	leg->AddEntry(h1,f1_leg,"l");
	leg->AddEntry(h2_clone,f2_leg,"l");
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
	ratio->Divide(h2_clone);
	ratio->Draw("hist");
	//ratio->SetMaximum(ratio->GetMaximum()*1.2);
	ratio->GetXaxis()->SetLabelSize(0.12);
	ratio->GetYaxis()->SetLabelSize(0.08);
	//ratio->GetXaxis()->SetTitle("MET [GeV]");
	//ratio->GetXaxis()->SetTitleOffset(1);
	//ratio->GetXaxis()->SetTitleSize(0.12);
	for(int i = 1; i <= ratio->GetSize() - 2; i++)
	{
		if (ratio->GetBinContent(i) != 1) std::cout << i-1 << " " << ratio->GetBinContent(i) << " h1 " << h1->GetBinContent(i) << " h2 " << h2_clone->GetBinContent(i) << std::endl;
	}

	//mycanvas->SetLogy();

	float y_max = max(h1->GetMaximum(),h2->GetMaximum());
	if (plot_log) y_max = y_max * 10;
	else y_max = y_max * 1.2;
	h1->SetMaximum(y_max);

	mycanvas->SaveAs("plots_test/" + f1_name + f2_leg + year + ".png");

	return 0;
}
