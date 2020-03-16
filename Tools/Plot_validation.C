#include <iostream>
#include <iomanip>

int Plot_validation()
{
	//gROOT->ForceStyle();

	int bin_test = 5;
	//bin_test = 9;
	bin_test = 10;
	//bin_test = 24;
	//bin_test = 19;

	double lumi = 137;
	//lumi = 36;
	//lumi = 42;
	//lumi = 59;

	TString year = "";
	//year = "_2016";
	//year = "_2017";
	//year = "_2018";
	TString folder = "";
	//folder = "validation_2016/";
	//folder = "validation_2017/";
	//folder = "validation_2018/";

	bool plot_MC_only = false;
	bool plot_log = true;
	bool plot_sig_pad = true;
	bool plot_BG = true;
	bool plot_pull = false;
	TString pull = "";
	if(plot_pull) pull = "_pull";

	TString low_or_high = "_lowdm";
	//low_or_high = "_highdm";

	float ymin = 0.1, ymax = 100000;

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.6,0.74,0.9,0.89);
	leg->SetNColumns(3);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);

	TLegend* leg_MC = new TLegend(0.6,0.64,0.9,0.74);
	leg_MC->SetNColumns(2);
	leg_MC->SetBorderSize(0);
	leg_MC->SetTextSize(0.04);

	TPad *padup = new TPad("padup", "padup", 0, 0.3, 1, 1.0);
	padup -> SetBottomMargin(0);
	padup -> Draw();
	padup -> cd();

	if (plot_log) gPad-> SetLogy();

	THStack *hs = new THStack();
	THStack *hs_up = new THStack();
	THStack *hs_down = new THStack();
	THStack *hs_stat = new THStack();
	THStack *hs_MC = new THStack();

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "Rare_validation" + low_or_high + year + "_people.root");
		TH1D *pro = (TH1D*)f1->Get("rare");
		TH1D *pro_up = (TH1D*)f1->Get("rare_up");
		TH1D *pro_down = (TH1D*)f1->Get("rare_down");
		TH1D *pro_stat = (TH1D*)f1->Get("rare_stat");

		TFile *f2 = new TFile("validation/" + folder + "Rare_validation" + low_or_high + year + ".root");
		TH1D *pro_MC = (TH1D*)f2->Get("rare");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro_stat->Sumw2();
		pro->SetFillColor(kYellow);
		pro_MC->SetFillColor(kYellow);
		leg->AddEntry(pro,"Rare","f");
		std::cout << "Rare total " << pro->Integral() << std::endl;
		std::cout << "Tot unc bin " << bin_test << ": " << pro->GetBinContent(bin_test) << " + " << pro_up->GetBinError(bin_test) << " - " << pro_down->GetBinError(bin_test) << std::endl;
		std::cout << "Stat unc bin " << bin_test << ": " << pro_stat->GetBinContent(bin_test) << " +- " << pro_stat->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
		hs_up->Add(pro_up);
		hs_down->Add(pro_down);
		hs_stat->Add(pro_stat);
		hs_MC->Add(pro_MC);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "TTZ_validation" + low_or_high + year + "_people.root");
		TH1D *pro = (TH1D*)f1->Get("TTZ");
		TH1D *pro_up = (TH1D*)f1->Get("TTZ_up");
		TH1D *pro_down = (TH1D*)f1->Get("TTZ_down");
		TH1D *pro_stat = (TH1D*)f1->Get("TTZ_stat");

		TFile *f2 = new TFile("validation/" + folder + "TTZ_validation" + low_or_high + year + ".root");
		TH1D *pro_MC = (TH1D*)f2->Get("TTZ");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro_stat->Sumw2();
		pro->SetFillColor(kViolet);
		pro_MC->SetFillColor(kViolet);
		leg->AddEntry(pro,"TTZ","f");
		std::cout << "TTZ total " << pro->Integral() << std::endl;
		std::cout << "Tot unc bin " << bin_test << ": " << pro->GetBinContent(bin_test) << " + " << pro_up->GetBinError(bin_test) << " - " << pro_down->GetBinError(bin_test) << std::endl;
		std::cout << "Stat unc bin " << bin_test << ": " << pro_stat->GetBinContent(bin_test) << " +- " << pro_stat->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
		hs_up->Add(pro_up);
		hs_down->Add(pro_down);
		hs_stat->Add(pro_stat);
		hs_MC->Add(pro_MC);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "QCD_validation" + low_or_high + year + "_people.root");
		TH1D *pro = (TH1D*)f1->Get("QCD");
		TH1D *pro_up = (TH1D*)f1->Get("QCD_up");
		TH1D *pro_down = (TH1D*)f1->Get("QCD_down");
		TH1D *pro_stat = (TH1D*)f1->Get("QCD_stat");

		TFile *f2 = new TFile("validation/" + folder + "QCD_validation" + low_or_high + year + "_people.root");
		TH1D *pro_MC = (TH1D*)f2->Get("QCD");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro_stat->Sumw2();
		pro->SetFillColor(kGreen);
		pro_MC->SetFillColor(kGreen);
		leg->AddEntry(pro,"QCD","f");
		std::cout << "QCD total " << pro->Integral() << std::endl;
		std::cout << "Tot unc bin " << bin_test << ": " << pro->GetBinContent(bin_test) << " + " << pro_up->GetBinError(bin_test) << " - " << pro_down->GetBinError(bin_test) << std::endl;
		std::cout << "Stat unc bin " << bin_test << ": " << pro_stat->GetBinContent(bin_test) << " +- " << pro_stat->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
		hs_up->Add(pro_up);
		hs_down->Add(pro_down);
		hs_stat->Add(pro_stat);
		hs_MC->Add(pro_MC);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "Zinv_validation" + low_or_high + year + "_people.root");
		TH1D *pro = (TH1D*)f1->Get("zjets");
		TH1D *pro_up = (TH1D*)f1->Get("zjets_up");
		TH1D *pro_down = (TH1D*)f1->Get("zjets_down");
		TH1D *pro_stat = (TH1D*)f1->Get("zjets_stat");

		TFile *f2 = new TFile("validation/" + folder + "Zinv_validation" + low_or_high + year + ".root");
		TH1D *pro_MC = (TH1D*)f2->Get("zjets");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro_stat->Sumw2();
		pro->SetFillColor(kRed);
		pro_MC->SetFillColor(kRed);
		leg->AddEntry(pro,"Zinv","f");
		std::cout << "Zinv total " << pro->Integral() << std::endl;
		std::cout << "Tot unc bin " << bin_test << ": " << pro->GetBinContent(bin_test) << " + " << pro_up->GetBinError(bin_test) << " - " << pro_down->GetBinError(bin_test) << std::endl;
		std::cout << "Stat unc bin " << bin_test << ": " << pro_stat->GetBinContent(bin_test) << " +- " << pro_stat->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
		hs_up->Add(pro_up);
		hs_down->Add(pro_down);
		hs_stat->Add(pro_stat);
		hs_MC->Add(pro_MC);
	}

	if (plot_BG)
	{
		TFile *f1 = new TFile("validation/" + folder + "LL_validation" + low_or_high + year + "_people.root");
		TH1D *pro = (TH1D*)f1->Get("LL_SR");
		TH1D *pro_up = (TH1D*)f1->Get("LL_SR_up");
		TH1D *pro_down = (TH1D*)f1->Get("LL_SR_down");
		TH1D *pro_stat = (TH1D*)f1->Get("LL_SR_stat");

		TFile *f2 = new TFile("validation/" + folder + "LL_validation" + low_or_high + year + ".root");
		TH1D *pro_MC = (TH1D*)f2->Get("LL_SR");

		pro->SetLineColor(kBlack);
		//pro->SetLineWidth(3);
		pro->Sumw2();
		pro_stat->Sumw2();
		pro->SetFillColor(kBlue);
		pro_MC->SetFillColor(kBlue);
		leg->AddEntry(pro,"LL","f");
		std::cout << "LL total " << pro->Integral() << std::endl;
		std::cout << "Tot unc bin " << bin_test << ": " << pro->GetBinContent(bin_test) << " + " << pro_up->GetBinError(bin_test) << " - " << pro_down->GetBinError(bin_test) << std::endl;
		std::cout << "Stat unc bin " << bin_test << ": " << pro_stat->GetBinContent(bin_test) << " +- " << pro_stat->GetBinError(bin_test) << "\n" << std::endl;

		hs->Add(pro);
		hs_up->Add(pro_up);
		hs_down->Add(pro_down);
		hs_stat->Add(pro_stat);
		hs_MC->Add(pro_MC);
	}

	if(!plot_MC_only)
	{
		hs->Draw("hist");

		hs->GetYaxis()->SetTitle("events");
		hs->GetYaxis()->SetTitleSize(0.045);
		hs->GetYaxis()->SetTitleOffset(0.8);
		//hs->GetYaxis()->SetRangeUser(ymin,ymax);
		hs->SetMinimum(ymin);
		hs->SetMaximum(ymax);
	}

	if(plot_MC_only)
	{
		hs_MC->Draw("hist");

		hs_MC->GetYaxis()->SetTitle("events");
		hs_MC->GetYaxis()->SetTitleSize(0.045);
		hs_MC->GetYaxis()->SetTitleOffset(0.8);
		//hs->GetYaxis()->SetRangeUser(ymin,ymax);
		hs_MC->SetMinimum(ymin);
		hs_MC->SetMaximum(ymax);
	}

	TFile *f_data = new TFile("validation/" + folder + "Data_validation" + low_or_high + year + ".root");
	TH1D *h_data = (TH1D*)f_data->Get("data");

	h_data->Sumw2();
	h_data->SetBinErrorOption(TH1::kPoisson);
	h_data->SetLineColor(kBlack);
	h_data->SetMarkerStyle(20);
	h_data->SetMarkerColor(kBlack);
	//h_data->SetLineWidth(2);
	h_data->Draw("E0same");
	leg->AddEntry(h_data,"Data","lp");

	std::cout << "BG total " << ((TH1D*)(hs->GetStack()->Last()))->Integral() << std::endl;
	std::cout << "Tot unc BG bin " << bin_test << ": " << ((TH1D*)(hs->GetStack()->Last()))->GetBinContent(bin_test) << " + " << ((TH1D*)(hs_up->GetStack()->Last()))->GetBinError(bin_test) << " - " << ((TH1D*)(hs_down->GetStack()->Last()))->GetBinError(bin_test) << std::endl;
	std::cout << "Stat unc BG bin " << bin_test << ": " << ((TH1D*)(hs_stat->GetStack()->Last()))->GetBinContent(bin_test) << " +- " << ((TH1D*)(hs_stat->GetStack()->Last()))->GetBinError(bin_test) << "\n" << std::endl;
	std::cout << "Data total " << h_data->Integral() << std::endl;
	std::cout << "Data bin " << bin_test << ": " << h_data->GetBinContent(bin_test) << " +- " << h_data->GetBinError(bin_test) << std::endl;
	std::cout << "Data bin " << bin_test << ": " << h_data->GetBinContent(bin_test) << " + " << h_data->GetBinErrorUp(bin_test) << " - " << h_data->GetBinErrorLow(bin_test) << "\n" << std::endl;

	TLatex latex;
	latex.SetTextSize(0.04);
	latex.SetNDC();
	//latex.SetTextAlign(13);  //align at top
	//latex.DrawLatex(0.5,ymax+0.4,"#bf{CMS} Preliminary, 2017 data");
	latex.DrawLatex(0.1,0.91,"CMS #bf{Preliminary}");
	TString lumi_and_energy = "#bf{" + std::to_string((int)lumi) + " fb^{-1} (13TeV)}";
	if(plot_BG) latex.DrawLatex(0.73,0.91,lumi_and_energy);
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
			TH1D *ratio_MC = (TH1D*)h_data->Clone();
			ratio->Sumw2();
			ratio_MC->Sumw2();
			ratio_MC->SetLineColor(kRed);
			//ratio_MC->SetLineWidth(2);
			//ratio_MC->SetLineColor(kGreen);
			TH1D *BG_MC = (TH1D*)(hs_MC->GetStack()->Last())->Clone();
			TH1D *BG_central = (TH1D*)(hs->GetStack()->Last())->Clone();
			TH1D *BG_err = (TH1D*)(hs->GetStack()->Last())->Clone();
			TH1D *BG_err_stat = (TH1D*)(hs_stat->GetStack()->Last())->Clone();
			for(int i = 1; i <= ratio->GetSize() - 2; i++)
			{
				BG_central->SetBinError(i,0);
				BG_err->SetBinError(i,BG_err->GetBinError(i)/BG_err->GetBinContent(i));
				BG_err->SetBinContent(i,1);
				BG_err_stat->SetBinError(i,BG_err_stat->GetBinError(i)/BG_err_stat->GetBinContent(i));
				BG_err_stat->SetBinContent(i,1);
				if(ratio->GetBinContent(i)==0) ratio->SetBinError(i,1.8);
			}

			ratio->Divide(BG_central);
			ratio_MC->Divide(BG_MC);
			//============test======================
			/*TH1D *BG_err_stat = (TH1D*)(BG_err)->Clone();
			  for(int i = 1; i <= ratio->GetSize() - 2; i++)
			  {
			  BG_err->SetBinError(i,BG_err->GetBinError(i)*2);
			  }*/
			//===========end test==================
			BG_err->Draw("E2");
			BG_err_stat->Draw("E2same");
			BG_err_stat->SetFillColor(kBlue-9);
			ratio->Draw("E0same");
			ratio_MC->Draw("histsame");

			std::cout << "Ratio bin " << bin_test << " " << ratio->GetBinContent(bin_test) << " +- " << ratio->GetBinError(bin_test) << "\n" << std::endl;

			//BG_err->SetFillColor(kBlack);
			BG_err->SetFillStyle(3004);
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
			l->SetLineColor(kBlack);
			l->Draw();

			leg_MC->AddEntry(BG_err,"tot. unc.","f");
			leg_MC->AddEntry(BG_err_stat,"stat. unc.","f");
			leg_MC->AddEntry(ratio_MC,"simulation","l");
			padup -> cd();
			leg_MC->Draw("same");
		}

		if(plot_pull)
		{
			auto pull_h=new TH1F("pull_h","distribution of pull",24,-3,3);
			TH1D *ratio = (TH1D*)h_data->Clone();
			for(int i = 1; i <= ratio->GetSize() - 2; i++)
			{
				ratio->SetBinError(i,0);
				float a = h_data->GetBinContent(i);
				float b = ((TH1D*)hs->GetStack()->Last())->GetBinContent(i);
				//if data >= BG
				float da = h_data->GetBinErrorLow(i) * h_data->GetBinErrorLow(i);
				float db = ((TH1D*)hs_up->GetStack()->Last())->GetBinError(i) * ((TH1D*)hs_up->GetStack()->Last())->GetBinError(i);
				if (a < b)//if data < BG
				{
					da = h_data->GetBinErrorUp(i) * h_data->GetBinErrorUp(i);
					db = ((TH1D*)hs_down->GetStack()->Last())->GetBinError(i) * ((TH1D*)hs_down->GetStack()->Last())->GetBinError(i);
				}
				float pull = (a-b)/sqrt(da+db);	//pull = data-pred/sqrt(d_data^2 + d_pred^2)
				pull = (a-b)/sqrt(b+db);	//ken's formular: pull = data-pred/sqrt(pred + d_pred^2)
				ratio->SetBinContent(i,pull);
				pull_h->Fill(pull);
				std::cout << pull << ", ";
			}
			ratio->Draw("hist");

			std::cout << std::fixed;
			std::cout << std::setprecision(2);
			std::cout << "\nBin " << bin_test << " pull: " << ratio->GetBinContent(bin_test) << ". Data " << h_data->GetBinContent(bin_test) << " + " << h_data->GetBinErrorUp(bin_test) << " - " << h_data->GetBinErrorLow(bin_test) << ". BG " << ((TH1D*)(hs->GetStack()->Last()))->GetBinContent(bin_test) << " + " << ((TH1D*)(hs_up->GetStack()->Last()))->GetBinError(bin_test) << " - " << ((TH1D*)(hs_down->GetStack()->Last()))->GetBinError(bin_test) << std::endl;

			ratio->SetTitle("");
			ratio->GetXaxis()->SetTitle("validation bins ");
			ratio->GetXaxis()->SetTitleSize(0.1);
			ratio->GetXaxis()->SetLabelSize(0.08);
			ratio->GetXaxis()->SetNdivisions(30);
			ratio->GetYaxis()->SetTitle("pull ");
			ratio->GetYaxis()->SetTitleOffset(0.4);
			ratio->GetYaxis()->SetTitleSize(0.1);
			ratio->GetYaxis()->SetLabelSize(0.08);
			ratio->GetYaxis()->SetRangeUser(-5,5);

			TLine *l=new TLine(ratio->GetXaxis()->GetXmin(), 0.0,ratio->GetXaxis()->GetXmax(), 0.0);
			l->SetLineColor(kRed);
			l->Draw();

			//save histogram for pull
			TFile pull_file("plots/validation" + low_or_high + year + pull + ".root","RECREATE");
			pull_h->Write();
			pull_file.Close();
		}
	}

	TString MC_only = "_MC_only";
	if(!plot_MC_only) MC_only = "";
	mycanvas->SaveAs("plots/validation" + low_or_high + year + pull + MC_only + ".png");
	mycanvas->SaveAs("plots/validation" + low_or_high + year + pull + MC_only + ".pdf");

	return 0;
}
