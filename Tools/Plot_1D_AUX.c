int Plot_1D_AUX_sg (double signal_scale, TString result_path, TString sp, TString full_or_fast, TString year, TString var, TString folder, TLegend* leg, Color_t color, int rebin, bool sig_scale_to_BG)
{
	TFile *f1 = new TFile(result_path + sp + full_or_fast + year + ".root");
	TH1D *h1 = (TH1D*)f1->Get(folder + var);
	//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
	TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

	double all_events = h2->GetBinContent(1);
	double left_events = h2->GetBinContent(2);

	std::cout << "left_events = " << left_events << std::endl;
	std::cout << "GetEntries = " << h1->GetEntries() << std::endl;
	std::cout << "GetIntergral = " << h1->Integral() << std::endl;
	std::cout << "all_events = " << all_events << std::endl;
	//std::cout << "GetEntries = " << h3->GetEntries() << std::endl;
	//std::cout << sp << " Xsec = " << CrossSection.at(sp) << std::endl;

	if(sig_scale_to_BG) h1->Scale(signal_scale / h1->Integral() );
	else h1->Scale(signal_scale * 50 * 1000);

	std::cout << "events rebin= " << h1->Integral()  << std::endl;

	h1->Rebin(rebin);
	h1->SetLineColor(color);
	h1->SetLineWidth(2);
	h1->Draw("hist same");
	leg->AddEntry(h1,sp,"l");
	return 0;
}

int Plot_1D_AUX_bg (double lumi, TString sp, TString year, TString var, TString folder, TLegend* leg, Color_t color, TH1D *bg, int rebin)
{
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

	bg->Add(h1);
	//h1->SetLineColor(color);
	//h1->SetLineWidth(3);
	//h1->Draw("same");
	//leg->AddEntry(h1,sp,"l");
	return 0;
}

int Plot_1D_AUX_sig (double lumi, TString result_path, TString sp, TString full_or_fast, TString year, TString var, TString folder, TLegend *leg, Color_t color, THStack *hs, int rebin, bool use_low_stat_sig)
{
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


	significance->SetLineColor(color);
	significance->SetLineWidth(2);
	significance->Draw("hist same");
	//leg->AddEntry(h1,sp,"l");
	return 0;
}

