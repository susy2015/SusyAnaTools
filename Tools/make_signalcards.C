#include "TH1.h"
#include "TFile.h"
#include "TString.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cerr << "please give 4 arguments" << std::endl;
		std::cerr << "mother mass, daughter mass, signal name and histo name" << std::endl;
		return -1;
	}

	const int  mother = atoi(argv[1]);
	const int  daughter = atoi(argv[2]);
	TString sp = argv[3];
	TString var = argv[4];
	const double lumi = 36;
	//const double lumi = 120;

	TFile *f3 = new TFile("xSec.root");
	TH1D *h3 = NULL;

	TString result_path;
	TString folder = "";

	if (sp.Contains("T2tt"))
	{
		std::cout << "=============== find T2tt =================" << std::endl;
		result_path = "results/T2tt_signal_scan/";
		h3 = (TH1D*)f3->Get("stop_xsection");
	}

	else if (sp.Contains("T1tttt"))
	{
		std::cout << "=============== find T1tttt =================" << std::endl;
		result_path = "results/T1tttt_signal_scan/";
		h3 = (TH1D*)f3->Get("gluino_xsection");
	}

	else
	{
		std::cout << "only support T2tt and T1tttt, exit now" << std::endl;
		return -1;
	}

	TFile *f1 = new TFile(result_path + sp + ".root");
	TH1D *h1 = (TH1D*)f1->Get(folder + var);
	//TH1D *h2 = (TH1D*)f1->Get(folder + "/eff_h");
	TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

	double CrossSection = h3->GetBinContent(h3->FindBin(mother));
	int NSB = h1->GetSize() - 2;
	int first_bin = int(h1->GetXaxis()->GetBinCenter(1));
	int mid_bin = -1;

	double all_events = h2->GetBinContent(1);
	double left_events = h2->GetBinContent(2);

	h1->Sumw2();
	h1->SetBinErrorOption(TH1::kPoisson);

	std::cout << "\n" << sp << " xsec = " << CrossSection << std::endl;
	//std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
	//std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
	//std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
	//std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

	TH1D * raw_sig = (TH1D*)h1->Clone("raw_sig");

	h1->Scale(lumi * CrossSection * 1000 / all_events );

	TH1D * TF_sig = (TH1D*)h1->Clone("TF_sig");
	TF_sig->Divide(raw_sig);

	//std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
	//std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
	//std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
	//std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

	std::ofstream signalfile (("datacards/signal_temp/signal_" + std::to_string(mother) + "_" + std::to_string(daughter) + ".txt"));
	if (signalfile.is_open())
	{
		signalfile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
		signalfile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
		signalfile << "sample = signal     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
		signalfile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ signalfile << "bin" << i+1 << " "; } signalfile << "\n";

		signalfile << "\n# Predicted central numbers (need from all backgrounds)\n";
		signalfile << "rate = "; for(int i=0;i<NSB;i++){ signalfile << h1->GetBinContent(i+1) << " "; } signalfile << "\n";

		signalfile << "cs_event = "; for(int i=0;i<NSB;i++){ signalfile << raw_sig->GetBinContent(i+1) << " "; } signalfile << "\n";
		signalfile << "contam = "; for(int i=0;i<NSB;i++){ signalfile << "0.0000" << " "; } signalfile << "\n";

		signalfile << "avg_weight = "; for(int i=0;i<NSB;i++){ signalfile << TF_sig->GetBinContent(i+1) << " "; } signalfile << "\n";

		//signalfile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ signalfile << raw_sig->GetBinErrorUp(i+1) / raw_sig->GetBinContent(i+1) << " "; } signalfile << "\n";
		//signalfile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ signalfile << raw_sig->GetBinErrorLow(i+1) / raw_sig->GetBinContent(i+1) << " "; } signalfile << "\n";

		signalfile << "syst_lumi_unc_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.025 << " "; } signalfile << "\n";
		signalfile << "syst_lumi_unc_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.025 << " "; } signalfile << "\n";
		signalfile << "syst_lepVetoUnc_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.03 << " "; } signalfile << "\n";
		signalfile << "syst_lepVetoUnc_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.03 << " "; } signalfile << "\n";
		signalfile << "syst_bTagSF_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
		signalfile << "syst_bTagSF_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
		signalfile << "syst_trigUnc_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.01 << " "; } signalfile << "\n";
		signalfile << "syst_trigUnc_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.01 << " "; } signalfile << "\n";
		signalfile << "syst_scaleUnc_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.01 << " "; } signalfile << "\n";
		signalfile << "syst_scaleUnc_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.01 << " "; } signalfile << "\n";
		signalfile << "syst_isrUnc_up = ";
		for(int i=0;i<NSB;i++)
		{
			if(i < mid_bin) signalfile << 0.00 << " ";
			else signalfile << 0.20 << " ";
		}
		signalfile << "\n";
		signalfile << "syst_isrUnc_dn = ";
		for(int i=0;i<NSB;i++)
		{
			if(i < mid_bin) signalfile << 0.00 << " ";
			else signalfile << 0.20 << " ";
		}
		signalfile << "\n";
		signalfile << "syst_jetJEC_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.15 << " "; } signalfile << "\n";
		signalfile << "syst_jetJEC_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.15 << " "; } signalfile << "\n";
		signalfile << "syst_TopReco_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.05 << " "; } signalfile << "\n";
		signalfile << "syst_TopReco_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.05 << " "; } signalfile << "\n";
		signalfile << "syst_TopSF_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
		signalfile << "syst_TopSF_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
		signalfile.close();
	}
	else std::cout << "Unable to open signalfile";
	return 0;
}
