int Plot_loose_bins()
{
	//gROOT->ForceStyle();

	double lumi = 36;
	lumi = 78;
	//lumi = 137;
	TString year = "_2016";
	TString year2 = "_2017";
	bool plot_log = true;
	bool plot_LL = false;

	//TString h1 = "search_bin_v2_QCD_CR_highdm_loose_bin_h";
	//if(plot_LL) h1 = "search_bin_v2_singleMuCR_highdm_loose_bin_h";
	TString h1 = "search_bin_v2_QCD_CR_highdm_loose_bin_jetpt30_h";
	if(plot_LL) h1 = "search_bin_v2_singleMuCR_highdm_loose_bin_jetpt30_h";

	float ymin = 1, ymax = 100000;

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.6,0.7,0.9,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.03);

	//TPad *padup = new TPad("padup", "padup", 0, 0.3, 1, 1.0);
	//padup -> SetBottomMargin(0);
	//padup -> Draw();
	//padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();

	if (true)
	{
		TFile *f1 = new TFile("results/Data_MET" + year + ".root");
		TH1D *pro = (TH1D*)f1->Get(h1);

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kYellow);
		//leg->AddEntry(pro, "single Mu" + year,"f");
		leg->AddEntry(pro, "QCD CR" + year,"f");

		hs->Add(pro);
	}

	if (plot_LL)
	{
		TFile *f1 = new TFile("results/Data_MET" + year + ".root");
		TH1D *pro = (TH1D*)f1->Get("search_bin_v2_singleElCR_highdm_loose_bin_h");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kViolet);
		leg->AddEntry(pro, "single Ele" + year,"f");

		hs->Add(pro);
	}

	if (true)
	{
		TFile *f1 = new TFile("results/Data_MET" + year2 + ".root");
		TH1D *pro = (TH1D*)f1->Get(h1);

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kGreen);
		//leg->AddEntry(pro, "single Mu" + year2,"f");
		leg->AddEntry(pro, "QCD CR" + year2,"f");

		hs->Add(pro);
	}

	if (plot_LL)
	{
		TFile *f1 = new TFile("results/Data_MET" + year2 + ".root");
		TH1D *pro = (TH1D*)f1->Get("search_bin_v2_singleElCR_highdm_loose_bin_h");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kRed);
		leg->AddEntry(pro, "single Ele" + year2,"f");

		hs->Add(pro);
	}

	hs->Draw("hist");

	int n_bins = ((TH1D*)(hs -> GetStack() -> Last())) -> GetSize() - 2;
	for(int i=1; i <= n_bins; i++)
	{
		float bin_content = ((TH1D*)(hs -> GetStack() -> Last())) -> GetBinContent(i);
		std::cout << i+52 << ": " << bin_content << std::endl;
		//if(bin_content ==0) std::cout << i+52 << ": " << bin_content << std::endl;
	}

	hs->GetYaxis()->SetTitle("events");
	//hs->GetYaxis()->SetTitleSize(0.045);
	//hs->GetYaxis()->SetTitleOffset(0.95);
	hs->GetXaxis()->SetTitle("loose bin");
	//hs->GetYaxis()->SetRangeUser(ymin,ymax);
	hs->SetMinimum(ymin);
	hs->SetMaximum(ymax);

	TLatex latex;
	latex.SetTextSize(0.04);
	latex.SetNDC();
	//latex.SetTextAlign(13);  //align at top
	//latex.DrawLatex(0.5,ymax+0.4,"#bf{CMS} Preliminary, 2017 data");
	latex.DrawLatex(0.1,0.91,"CMS #bf{Simulation}");
	TString lumi_and_energy = "#bf{" + std::to_string((int)lumi) + " fb^{-1} (13TeV)}";
	latex.DrawLatex(0.7,0.91,lumi_and_energy);

	leg->Draw("same");

	//mycanvas->SaveAs("plots/loose_bins_LL_CR_jetpt30" + year + year2 + ".png");
	mycanvas->SaveAs("plots/loose_bins_QCD_CR_jetpt30" + year + year2 + ".png");

	return 0;
}
