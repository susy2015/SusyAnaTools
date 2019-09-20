#include "CrossSection.C"
#include "Plot_1D_AUX.c"
int Plot_1D_cutflow()
{
	//gROOT->ForceStyle();

	double lumi = 36;
	lumi = 137;
	TString year = "_2017";
	bool plot_log = false;
	plot_log = true;
	bool use_original_title = false;

	TString result_path = "results/SMS_";
	result_path = "results/";
	//TString result_path = "condor/hadd/v2/";
	TString full_or_fast = "_fullsim"; 
	//full_or_fast = "_fastsim"; 

	TString signal_name = "T2tt_and_T1tttt_high";

	TString data_name = "SingleMuon";


	float xmin =0, xmax = 800, ymin = 0, ymax = 35000;
	float leg_height = 0.5;
	//int hs_tot = 1, hs_tot_plot = 1;
	if(plot_log) ymin = 0.1;

	TString title = "Cutflow";
	TString var = "cutflow_h";
	TString folder = "Baseline_Only/";

	int rebin = 1;

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);
	//TCanvas* mycanvas = new TCanvas();
	gStyle->SetOptStat(kFALSE);

	TLegend* leg = new TLegend(0.4,leg_height,0.9,0.89);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);


	if (plot_log) gPad-> SetLogy();

	TString sp = "DYJetsToLL_HT_70to100";
	std::cout << sp << std::endl;
	TFile *f1 = new TFile("results/" + sp + year + ".root");
	TH1D *h1 = (TH1D*)f1->Get(folder + var);

	if(use_original_title) title = h1->GetTitle();

        //h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
        //h1->Scale(lumi * 1000);

        TH1D* DY = (TH1D*)h1->Clone("DY");
	
	sp = "DYJetsToLL_HT_100to200";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
	h1 = (TH1D*)f1->Get(folder+var);
	DY->Add(h1);
	
        sp = "DYJetsToLL_HT_200to400";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        sp = "DYJetsToLL_HT_400to600";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        sp = "DYJetsToLL_HT_600to800";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        sp = "DYJetsToLL_HT_800to1200";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        sp = "DYJetsToLL_HT_1200to2500";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        sp = "DYJetsToLL_HT_2500toInf";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        DY->Add(h1);

        DY->SetFillColor(kOrange+1);
	DY->Draw("hist");
        leg->AddEntry(DY,"DY","f");
/*
	//QCD
	sp = "QCD_HT_2000toInf";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* QCD = (TH1D*)h1->Clone("QCD");

        sp = "QCD_HT_200to300";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

        sp = "QCD_HT_300to500";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

        sp = "QCD_HT_500to700";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

        sp = "QCD_HT_700to1000";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

        sp = "QCD_HT_1000to1500";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

        sp = "QCD_HT_1500to2000";
	std::cout << sp << std::endl;
	f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        QCD->Add(h1);

	QCD->SetFillColor(kRed);
	QCD->Draw("hist same");
	leg->AddEntry(QCD,"QCD","f");
*/

	//ZJets
        sp = "ZJetsToNuNu_HT_100to200";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* ZJets = (TH1D*)h1->Clone("ZJets");

        sp = "ZJetsToNuNu_HT_200to400";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        sp = "ZJetsToNuNu_HT_400to600";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        sp = "ZJetsToNuNu_HT_600to800";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        sp = "ZJetsToNuNu_HT_800to1200";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        sp = "ZJetsToNuNu_HT_1200to2500";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        sp = "ZJetsToNuNu_HT_2500toInf";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        ZJets->Add(h1);

        ZJets->SetFillColor(kGreen);
        ZJets->Draw("hist same");
        leg->AddEntry(ZJets,"Z+jets","f");

        //WJets
        sp = "WJetsToLNu_HT_100to200";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* WJets = (TH1D*)h1->Clone("ZJets");

        sp = "WJetsToLNu_HT_200to400";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        sp = "WJetsToLNu_HT_400to600";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        sp = "WJetsToLNu_HT_600to800";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        sp = "WJetsToLNu_HT_800to1200";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        sp = "WJetsToLNu_HT_1200to2500";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        sp = "WJetsToLNu_HT_2500toInf";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        WJets->Add(h1);

        WJets->SetFillColor(kYellow);
        WJets->Draw("hist same");
        leg->AddEntry(WJets,"W+jets","f");

	//GJets
/*
	sp = "GJets_HT_100To200";
	sp = "GJets_HT_200To400";
	sp = "GJets_HT_400To600";
	sp = "GJets_HT_600ToInf";
	GJets->SetFillColor(kYellow+2);
*/
	//TTbar
        sp = "TTbarSingleLepT";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* TTbar = (TH1D*)h1->Clone("TTbar");

        sp = "TTbarSingleLepTbar";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TTbar->Add(h1);

        sp = "TTbarDiLep";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TTbar->Add(h1);

        TTbar->SetFillColor(kViolet-6);
        TTbar->Draw("hist same");
        leg->AddEntry(TTbar,"TTbar","f");

	//Single Top
        sp = "ST_s_lep";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* SingleTop = (TH1D*)h1->Clone("SingleTop");

        sp = "ST_t_antitop";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        SingleTop->Add(h1);

        sp = "ST_t_top";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        SingleTop->Add(h1);

        sp = "ST_tW_antitop_incl";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        SingleTop->Add(h1);

        sp = "ST_tW_top_incl";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        SingleTop->Add(h1);

        SingleTop->SetFillColor(kViolet+6);
        SingleTop->Draw("hist same");
        leg->AddEntry(SingleTop,"SingleTop","f");

	//TTZ
        sp = "TTZToLLNuNu";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* TTZ = (TH1D*)h1->Clone("TTZ");

        sp = "TTZToQQ";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TTZ->Add(h1);

        TTZ->SetFillColor(kViolet+6);
        TTZ->Draw("hist same");
        leg->AddEntry(TTZ,"TTZ","f");


/*
	if(plot_BG)
	{
		hs->GetYaxis()->SetTitle("events");
		hs->GetXaxis()->SetRangeUser(xmin,xmax);
		hs->GetYaxis()->SetRangeUser(ymin,ymax);
		hs->GetXaxis()->SetTitle(title);
		hs->GetYaxis()->SetTitleSize(0.045);
		hs->GetYaxis()->SetTitleOffset(1.2);

*/
	//Data
        sp = "Data_SingleMuon";
        std::cout << sp << std::endl;
        f1 = new TFile("results/" + sp + year + ".root");
        h1 = (TH1D*)f1->Get(folder+var);
        TH1D* Data = (TH1D*)h1->Clone("Data");

        Data->SetLineColor(kBlack);
        Data->Draw("P L same");
        leg->AddEntry(Data,"Data_SingleMuon","f");

	leg->Draw("same");
	
	//mycanvas->SetLogy();
	//mycanvas->SaveAs("plots/" + data_name + "_" + var + ".png");

	return 0;
}

