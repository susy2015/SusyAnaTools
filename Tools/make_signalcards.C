#include "TH1.h"
#include "TFile.h"
#include "TString.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 8)
	{
		std::cerr << "please give 6 arguments" << std::endl;
		std::cerr << "mother mass, daughter mass, signal name, SR, SingleMuCR, SingleElCR, mid bin" << std::endl;
		return -1;
	}

	const int  mother = atoi(argv[1]);
	const int  daughter = atoi(argv[2]);
	TString sp = argv[3];
	TString SR = argv[4];
	TString SingleMuCR = argv[5];
	TString SingleElCR = argv[6];
	int mid_bin = atoi(argv[7]);
	const double lumi = 137;

	bool is_compressed_signal = false;
	TFile *f3 = new TFile("xSec.root");
	TH1F *h3 = NULL;

	TString result_path;

	if (sp.Contains("T2tt"))
	{
		std::cout << "=============== find T2tt =================" << std::endl;
		result_path = "results/T2tt_signal_scan/";
		h3 = (TH1F*)f3->Get("stop_xsection");
	}

	else if (sp.Contains("T1tttt"))
	{
		std::cout << "=============== find T1tttt =================" << std::endl;
		result_path = "results/T1tttt_signal_scan/";
		h3 = (TH1F*)f3->Get("gluino_xsection");
	}

	else if (sp.Contains("T2fbd"))
	{
		std::cout << "=============== find T2fbd =================" << std::endl;
		result_path = "results/T2fbd_signal_scan/";
		h3 = (TH1F*)f3->Get("stop_xsection");
		is_compressed_signal = true;
	}

	else
	{
		std::cout << "only support T2tt and T1tttt, exit now" << std::endl;
		return -1;
	}

	TFile *f1 = new TFile(result_path + sp + ".root");
	TH1F *h1 = (TH1F*)f1->Get(SR);
	TH1F *h1_singleMuCR = (TH1F*)f1->Get(SingleMuCR);
	TH1F *h1_singleElCR = (TH1F*)f1->Get(SingleElCR);
	TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
	float gen_filter_eff = 1;
	if(is_compressed_signal)
	{
		TH1F *h4 = (TH1F*)f1->Get("Baseline_Only/gen_filter_eff_h");
		gen_filter_eff = h4->GetBinContent(2)/ h4->GetBinContent(1);
		std::cout << "this is a compressed signal with gen_filter_eff = " << gen_filter_eff << std::endl;
	}	

	double CrossSection = h3->GetBinContent(h3->FindBin(mother));
	int NSB = h1->GetSize() - 2;
	int first_bin = int(h1->GetXaxis()->GetBinCenter(1));

	double all_events = h2->GetBinContent(1);

	h1->Sumw2();
	h1_singleMuCR->Sumw2();
	h1_singleElCR->Sumw2();
	h1->SetBinErrorOption(TH1::kPoisson);

	std::cout << "\n" << sp << " xsec = " << CrossSection << std::endl;
	//std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
	//std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
	//std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
	//std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

	float sig_scale = lumi * CrossSection * 1000 / all_events;
	sig_scale = sig_scale * gen_filter_eff;
	h1->Scale(sig_scale);
	h1_singleMuCR->Scale(sig_scale);
	h1_singleElCR->Scale(sig_scale);

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

		signalfile << "cs_event = ";
		std::vector<int> cs_event(NSB,0);
		for(int i=0;i<NSB;i++)
		{
			cs_event[i] = ceil(h1->GetBinContent(i+1)/sig_scale);
			signalfile << cs_event[i] << " ";
		} signalfile << "\n";
		signalfile << "contam = "; for(int i=0;i<NSB;i++){ signalfile << "0.000" << " "; } signalfile << "\n";
		signalfile << "contam_muCS = "; for(int i=0;i<NSB;i++){ signalfile << h1_singleMuCR->GetBinContent(i+1) << " "; } signalfile << "\n";
		signalfile << "contam_eleCS = "; for(int i=0;i<NSB;i++){ signalfile << h1_singleElCR->GetBinContent(i+1) << " "; } signalfile << "\n";

		signalfile << "avg_weight = ";
		for(int i=0;i<NSB;i++)
		{
			if(cs_event[i] > 0) signalfile << h1->GetBinContent(i+1)/cs_event[i] << " ";
			else signalfile << sig_scale << " ";
		} signalfile << "\n";

		/*signalfile << "stat_unc_up = ";
		  for(int i=0;i<NSB;i++)
		  {
		  if (raw_sig->GetBinContent(i+1) > 0) signalfile << raw_sig->GetBinErrorUp(i+1) / raw_sig->GetBinContent(i+1) << " ";
		  else signalfile << "0.00" << " ";
		  } signalfile << "\n";
		  signalfile << "stat_unc_dn = ";
		  for(int i=0;i<NSB;i++)
		  {
		  if (raw_sig->GetBinContent(i+1) > 0) signalfile << raw_sig->GetBinErrorLow(i+1) / raw_sig->GetBinContent(i+1) << " ";
		  else signalfile << "0.00" << " ";
		  } signalfile << "\n";*/

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
