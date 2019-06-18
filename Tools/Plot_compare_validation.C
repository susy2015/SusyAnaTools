int Plot_compare_validation()
{
	const bool plot_log = true;

	bool plot_zinv_highdm = false;
	bool plot_LL_lowdm = false;
	bool plot_LL_highdm = false;
	bool plot_data_lowdm = true;
	bool plot_data_highdm = false;

	TString f1_name = "Zinv_validation_lowdm_2016";
	TString hist1 = "zjets"; 
	TString f1_leg = "MC"; 
	TString f2_name = "validationBinsZinv_2016"; 
	TString hist2 = "validation_lowdm";
	TString f2_leg = "Caleb"; 

	if(plot_zinv_highdm)
	{
		f1_name = "Zinv_validation_highdm_2016";
		hist1 = "zjets"; 
		f1_leg = "MC"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "validation_highdm";
		TString f2_leg = "Caleb"; 
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

	if(plot_data_lowdm)
	{
		f1_name = "Data_validation_lowdm_2016";
		hist1 = "search_bin_v2_lowdm_validation_pass_trigger_h"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_LM_2016"; 
		hist2 = "data";
		f2_leg = "Matt_data"; 
	}

	if(plot_data_highdm)
	{
		f1_name = "Data_validation_highdm_2016";
		hist1 = "search_bin_v2_highdm_validation_pass_trigger_h"; 
		f1_leg = "Data"; 
		f2_name = "std_pred_trad_HM_2016"; 
		hist2 = "data";
		f2_leg = "Matt_data"; 
	}

	TFile *f1 = new TFile("validation/" + f1_name + ".root");
	TFile *f2 = new TFile("validation/" + f2_name + ".root");

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
	ratio->Divide(h2);
	ratio->Draw("hist");
	//ratio->SetMaximum(ratio->GetMaximum()*1.2);
	ratio->GetXaxis()->SetLabelSize(0.12);
	ratio->GetYaxis()->SetLabelSize(0.08);

	//mycanvas->SetLogy();

	float y_max = max(h1->GetMaximum(), h2->GetMaximum()) * 1.2;
	h1->SetMaximum(y_max);

	mycanvas->SaveAs("plots_test/" + f1_name + ".png");

	return 0;
}
