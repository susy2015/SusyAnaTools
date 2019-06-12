int Plot_validation()
{
	//gROOT->ForceStyle();

	double lumi = 36;
	//lumi = 137;
	TString year = "_2016";
	bool plot_log = false;
	plot_log = true;
	bool plot_sig_pad = true;
	bool plot_BG = true;
	TString low_or_high = "_lowdm";
	//low_or_high = "_highdm";

	float ymin = 0.1, ymax = 100000;

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.7,0.6,0.9,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);

	TPad *padup = new TPad("padup", "padup", 0, 0.3, 1, 1.0);
	padup -> SetBottomMargin(0);
	padup -> Draw();
	padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/Rare_validation" + low_or_high + year + ".root");
		TH1D *pro = (TH1D*)f1->Get("rare");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kYellow);
		leg->AddEntry(pro,"Rare","f");
		std::cout << "\n Rare total " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/TTZ_validation" + low_or_high + year + ".root");
		TH1D *pro = (TH1D*)f1->Get("TTZ");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kViolet);
		leg->AddEntry(pro,"TTZ","f");
		std::cout << "\n TTZ total " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/QCD_validation" + low_or_high + year + ".root");
		TH1D *pro = (TH1D*)f1->Get("QCD");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kGreen);
		leg->AddEntry(pro,"QCD","f");
		std::cout << "\n QCD total " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		//TFile *f1 = new TFile("validation/Zinv_validation" + low_or_high + year + ".root");
		//TH1D *pro = (TH1D*)f1->Get("zjets");
		TFile *f1 = new TFile("validation/validationBinsZinv_2016.root");
		TH1D *pro = (TH1D*)f1->Get("validation" + low_or_high);

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kRed);
		leg->AddEntry(pro,"Zinv","f");
		std::cout << "\n Zinv total " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/LL_validation" + low_or_high + year + ".root");
		TH1D *pro = (TH1D*)f1->Get("LL_SR");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		//pro->Draw("bsame");
		pro->SetFillColor(kBlue);
		leg->AddEntry(pro,"LL","f");
		std::cout << "\n LL total " << pro->Integral() << std::endl;

		hs->Add(pro);
	}

	hs->Draw("hist");

	hs->GetYaxis()->SetTitle("events");
	hs->GetYaxis()->SetTitleSize(0.045);
	hs->GetYaxis()->SetTitleOffset(0.8);
	//hs->GetYaxis()->SetRangeUser(ymin,ymax);
	hs->SetMinimum(ymin);
	hs->SetMaximum(ymax);

	TFile *f_data = new TFile("validation/Data_validation" + low_or_high + year + ".root");
	TH1D *h_data = (TH1D*)f_data->Get("search_bin_v2" + low_or_high + "_validation_h");

	h_data->SetLineColor(kBlack);
	h_data->SetMarkerStyle(20);
	h_data->SetMarkerColor(kBlack);
	//h_data->SetLineWidth(2);
	h_data->Draw("psame");
	leg->AddEntry(h_data,"Data","lp");

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

		TH1D *ratio = (TH1D*)h_data->Clone();
		ratio->Sumw2();
		ratio->Divide((TH1D*)(hs -> GetStack() -> Last()));
		ratio->Draw("pe");

		ratio->SetTitle("");
		ratio->GetXaxis()->SetTitle("validation bins");
		ratio->GetXaxis()->SetTitleSize(0.1);
		ratio->GetXaxis()->SetLabelSize(0.08);
		ratio->GetXaxis()->SetNdivisions(30);
		ratio->GetYaxis()->SetTitle("Data / Pred");
		ratio->GetYaxis()->SetTitleOffset(0.4);
		ratio->GetYaxis()->SetTitleSize(0.1);
		ratio->GetYaxis()->SetLabelSize(0.08);
	}

	mycanvas->SaveAs("plots/validation" + low_or_high + year + ".png");

	return 0;
}
