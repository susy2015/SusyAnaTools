int Plot_validation()
{
	//gROOT->ForceStyle();

	double lumi = 137;
	//lumi = 36;
	//lumi = 42;
	lumi = 59;

	TString year = "";
	//year = "_2016";
	//year = "_2017";
	year = "_2018";
	TString folder = "";
	//folder = "validation_2016/";
	//folder = "validation_2017/";
	folder = "validation_2018/";

	bool plot_log = false;
	plot_log = true;
	bool plot_sig_pad = true;
	bool plot_BG = true;
	bool plot_pull = false;
	TString pull = "";
	if(plot_pull) pull = "_pull";

	TString low_or_high = "_lowdm";
	low_or_high = "_highdm";
	TString people = "";
	people = "_people";

	int bin_test = 19;
	bin_test = 24;
	bin_test = 5;

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
		TFile *f1 = new TFile("validation/" + folder + "Rare_validation" + low_or_high + year + people + ".root");
		TH1D *pro = (TH1D*)f1->Get("rare");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kYellow);
		leg->AddEntry(pro,"Rare","f");
		std::cout << "Rare total " << pro->Integral() << std::endl;
		std::cout << "Rare bin " << bin_test << " " << pro->GetBinContent(bin_test) << " +- " << pro->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "TTZ_validation" + low_or_high + year + people + ".root");
		TH1D *pro = (TH1D*)f1->Get("TTZ");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kViolet);
		leg->AddEntry(pro,"TTZ","f");
		std::cout << "TTZ total " << pro->Integral() << std::endl;
		std::cout << "TTZ bin " << bin_test << " " << pro->GetBinContent(bin_test) << " +- " << pro->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "QCD_validation" + low_or_high + year + people + ".root");
		TH1D *pro = (TH1D*)f1->Get("QCD");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kGreen);
		leg->AddEntry(pro,"QCD","f");
		std::cout << "QCD total " << pro->Integral() << std::endl;
		std::cout << "QCD bin " << bin_test << " " << pro->GetBinContent(bin_test) << " +- " << pro->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "Zinv_validation" + low_or_high + year + people + ".root");
		TH1D *pro = (TH1D*)f1->Get("zjets");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kRed);
		leg->AddEntry(pro,"Zinv","f");
		std::cout << "Zinv total " << pro->Integral() << std::endl;
		std::cout << "Zinv bin " << bin_test << " " << pro->GetBinContent(bin_test) << " +- " << pro->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "LL_validation" + low_or_high + year + people + ".root");
		TH1D *pro = (TH1D*)f1->Get("LL_SR");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro->SetFillColor(kBlue);
		leg->AddEntry(pro,"LL","f");
		std::cout << "LL total " << pro->Integral() << std::endl;
		std::cout << "LL bin " << bin_test << " " << pro->GetBinContent(bin_test) << " +- " << pro->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
	}

	hs->Draw("hist");

	hs->GetYaxis()->SetTitle("events");
	hs->GetYaxis()->SetTitleSize(0.045);
	hs->GetYaxis()->SetTitleOffset(0.8);
	//hs->GetYaxis()->SetRangeUser(ymin,ymax);
	hs->SetMinimum(ymin);
	hs->SetMaximum(ymax);

	TFile *f_data = new TFile("validation/" + folder + "Data_validation" + low_or_high + year + ".root");
	TH1D *h_data = (TH1D*)f_data->Get("data");

	h_data->Sumw2();
	h_data->SetBinErrorOption(TH1::kPoisson);
	h_data->SetLineColor(kBlack);
	h_data->SetMarkerStyle(20);
	h_data->SetMarkerColor(kBlack);
	//h_data->SetLineWidth(2);
	h_data->Draw("psame");
	leg->AddEntry(h_data,"Data","lp");

	std::cout << "BG total " << ((TH1D*)(hs->GetStack()->Last()))->Integral() << std::endl;
	std::cout << "BG bin " << bin_test << " " << ((TH1D*)(hs->GetStack()->Last()))->GetBinContent(bin_test) << " +- " << ((TH1D*)(hs->GetStack()->Last()))->GetBinError(bin_test) << "\n" << std::endl;
	std::cout << "Data total " << h_data->Integral() << std::endl;
	std::cout << "Data bin " << bin_test << " " << h_data->GetBinContent(bin_test) << " +- " << h_data->GetBinError(bin_test) << "\n" << std::endl;

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

		if(!plot_pull)
		{
			TH1D *ratio = (TH1D*)h_data->Clone();
			ratio->Sumw2();
			TH1D *BG_central = (TH1D*)(hs->GetStack()->Last())->Clone();
			TH1D *BG_err = (TH1D*)(hs->GetStack()->Last())->Clone();
			for(int i = 1; i <= ratio->GetSize() - 2; i++)
			{
				BG_central->SetBinError(i,0);
				BG_err->SetBinError(i,BG_err->GetBinError(i)/BG_err->GetBinContent(i));
				BG_err->SetBinContent(i,1);
			}

			ratio->Divide(BG_central);
			BG_err->Draw("E2");
			ratio->Draw("E0same");

			std::cout << "Ratio bin " << bin_test << " " << ratio->GetBinContent(bin_test) << " +- " << ratio->GetBinError(bin_test) << "\n" << std::endl;

			BG_err->SetTitle("");
			BG_err->GetXaxis()->SetTitle("validation bins");
			BG_err->GetXaxis()->SetTitleSize(0.1);
			BG_err->GetXaxis()->SetLabelSize(0.08);
			BG_err->GetXaxis()->SetNdivisions(30);
			BG_err->GetYaxis()->SetTitle("Data / Pred");
			BG_err->GetYaxis()->SetTitleOffset(0.4);
			BG_err->GetYaxis()->SetTitleSize(0.1);
			BG_err->GetYaxis()->SetLabelSize(0.08);
			BG_err->GetYaxis()->SetRangeUser(0,2);

	                TLine *l=new TLine(ratio->GetXaxis()->GetXmin(), 1.0,ratio->GetXaxis()->GetXmax(), 1.0);
        	        l->SetLineColor(kRed);
                	l->Draw();
		}

		if(plot_pull)
		{
			TH1D *ratio = (TH1D*)h_data->Clone();
			for(int i = 1; i <= ratio->GetSize() - 2; i++)
			{
				ratio->SetBinError(i,0);
				//pull = a-b/d(a-b)
				float a = h_data->GetBinContent(i);
				float da = h_data->GetBinError(i) * h_data->GetBinError(i);
				float b = ((TH1D*)hs->GetStack()->Last())->GetBinContent(i);
				float db = ((TH1D*)hs->GetStack()->Last())->GetBinError(i) * ((TH1D*)hs->GetStack()->Last())->GetBinError(i);
				ratio->SetBinContent(i,(a-b)/sqrt(da+db));
			}
			ratio->Draw("hist");

			std::cout << "Pull bin " << bin_test << " " << ratio->GetBinContent(bin_test) << " +- " << ratio->GetBinError(bin_test) << "\n" << std::endl;

			ratio->SetTitle("");
			ratio->GetXaxis()->SetTitle("validation bins");
			ratio->GetXaxis()->SetTitleSize(0.1);
			ratio->GetXaxis()->SetLabelSize(0.08);
			ratio->GetXaxis()->SetNdivisions(30);
			ratio->GetYaxis()->SetTitle("pull");
			ratio->GetYaxis()->SetTitleOffset(0.4);
			ratio->GetYaxis()->SetTitleSize(0.1);
			ratio->GetYaxis()->SetLabelSize(0.08);
			ratio->GetYaxis()->SetRangeUser(-10,10);

	                TLine *l=new TLine(ratio->GetXaxis()->GetXmin(), 0.0,ratio->GetXaxis()->GetXmax(), 0.0);
        	        l->SetLineColor(kRed);
                	l->Draw();
		}
	}

	mycanvas->SaveAs("plots/validation" + low_or_high + year + people + pull + ".png");

	return 0;
}
