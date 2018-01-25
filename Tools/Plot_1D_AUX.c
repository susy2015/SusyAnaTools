int Plot_1D_AUX_sg (double lumi, TString sp, TString var, TString folder, TLegend* leg, Color_t color, int rebin)
{
	TFile *f1 = new TFile("results/" + sp + ".root");
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
	std::cout << sp << " Xsec = " << CrossSection.at(sp) << std::endl;

	h1->Scale(lumi / h1->Integral() );
	//h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );

	std::cout << "events rebin= " << h1->Integral()  << std::endl;

	h1->Rebin(rebin);
	h1->SetLineColor(color);
	h1->SetLineWidth(3);
	h1->Draw("hist same");
	leg->AddEntry(h1,sp,"l");
	return 0;
}

int Plot_1D_AUX_bg (double lumi, TString sp, TString var, TString folder, TLegend* leg, Color_t color, TH1D * bg, int rebin)
{
	TFile *f1 = new TFile("results/" + sp + ".root");
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

