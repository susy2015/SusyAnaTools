#include "CrossSection.C"
int make_datacards()
{
	gROOT->ForceStyle();

	double lumi = 36;
	//lumi = 137;
	TString year = "_2016";
	//year = "_2017";

	bool do_validation = true;
	bool do_photon_unc = false;
	bool compare_TF = false;
	bool round_data = false;
	//bool only_lowdm = true;

	bool make_QCD_datacard = false;
	bool make_smear_QCD_datacard = true;
	bool make_Zinv_datacard = true;
	bool make_LL_datacard = true;
	bool make_TTZ_datacard = true;
	bool make_Rare_datacard = true;
	bool make_data_datacard = true;
	bool make_signal_datacard = false;

	bool team_A = false;
	bool team_A_high_dm_MT2 = false;
	bool team_A_high_dm_merge = false;
	bool SBv2 = false;
	bool SBv3 = false;
	bool SBv2_jetpt30 = false;
	bool SBv2_highdm = false;
	bool SBv2_highdm_validation = false;
	bool SBv3_highdm_validation = false;
	bool SBv2_lowdm = false;
	bool SBv2_lowdm_validation = true;
	bool SBv2_lowdm_more_MET = false;
	bool SBv2_lowdm_high_ISRpt = false;
	bool more_HT_bins = false;

	TString full_or_fast = "_fullsim";
	full_or_fast = "_fastsim";

	//TString signal_name = "T2tt_mStop500_mLSP413";
	//TString signal_name = "T2tt_mStop550_mLSP400";
	TString signal_name = "T2tt_mStop600_mLSP514";
	//TString signal_name = "T2tt_mStop850_mLSP100";
	//TString signal_name = "T2tt_mStop1000_mLSP1";
	//TString signal_name = "T2tt_mStop1000_mLSP50";
	//TString signal_name = "T2tt_mStop1000_mLSP500";
	//TString signal_name = "T2tt_mStop1200_mLSP50";
	//TString signal_name = "T2tt_mStop1200_mLSP650";
	//TString signal_name = "T1tttt_mGluino950_mLSP650";
	//TString signal_name = "T1tttt_mGluino1200_mLSP800";
	//TString signal_name = "T1tttt_mGluino2000_mLSP100";
	//TString signal_name = "T1tttt_mGluino2000_mLSP1000";
	//TString signal_name = "T1tttt_mGluino2200_mLSP100";
	//TString signal_name = "T1tttt_mGluino2200_mLSP1300";
	//TString signal_name = "T1ttbb_mGluino2000_mLSP1000";
	//TString signal_name = "T1ttbb_mGluino2200_mLSP1300";
	//TString signal_name = "T2bw_mStop800_mLSP100";
	//TString signal_name = "T2bw_mStop500_mLSP325";
	//TString signal_name = "T2bw_mStop700_mLSP526";
	//TString signal_name = "T2bt_mStop800_mLSP100";
	//TString signal_name = "T2bt_mStop500_mLSP300";
	//TString signal_name = "T2fbd_mStop500_mLSP490";
	//TString signal_name = "T2fbd_mStop500_mLSP450";
	//TString signal_name = "T2fbd_mStop600_mLSP520";
	//TString signal_name = "T2fbd_mStop600_mLSP590";
	//TString signal_name = "T2cc_mStop550_mLSP540";
	//TString signal_name = "T2cc_mStop600_mLSP520";
	//TString signal_name = "T2cc_mStop650_mLSP570";

	TString folder = "";
	TString SR = "", SR_loose_bin = "", SR_weight = "";
	TString SingleMuCR = "", SingleMuCR_loose_bin = "";
	TString SingleElCR = "", SingleElCR_loose_bin = "";
	TString low_or_high = "_lowdm";

 	int first_bin;
	int NSB = 999;
	int mid_bin = 53;	//53 for SBv2
	double zinv_sf = 666.8 / 851.6;		//ratio of 050 data card / MC with B_SF
	//zinv_sf = 1;				//Caleb's 2016 Rz scale factor: low dm 1.038, high dm 0.955
	double ll_sf = 2957.6 / 3378.4;		//ratio of 050 data card / MC with ISR and B_SF

	if (team_A)
	{
		SR = "search_bin_team_A_h";
		SingleMuCR = "search_bin_team_A_singleMuCR_h";
		SingleElCR = "search_bin_team_A_singleElCR_h";
	}

	if (team_A_high_dm_MT2)
	{
		SR = "search_bin_team_A_highdm_MTb175_MT2_h";
		SingleMuCR = "search_bin_team_A_highdm_MTb175_MT2_singleMuCR_h";
		SingleElCR = "search_bin_team_A_highdm_MTb175_MT2_singleElCR_h";
	}

	if (team_A_high_dm_merge)
	{
		SR = "search_bin_team_A_highdm_merge_h";
		SingleMuCR = "search_bin_team_A_highdm_singleMuCR_merge_h";
		SingleElCR = "search_bin_team_A_highdm_singleElCR_merge_h";
	}

	if (SBv2)
	{
		SR = "search_bin_v2_h";
		SingleMuCR = "search_bin_v2_singleMuCR_h";
		SingleElCR = "search_bin_v2_singleElCR_h";
		SR_weight = "search_bin_v2_Stop0l_evtWeight_h";
	}

	if (SBv3)
	{
		SR = "search_bin_v3_h";
		SingleMuCR = "search_bin_v3_singleMuCR_h";
		SingleElCR = "search_bin_v3_singleElCR_h";
		SR_weight = "search_bin_v3_Stop0l_evtWeight_h";
	}

	if (SBv2_jetpt30)
	{
		SR = "search_bin_v2_jetpt30_h";
		SingleMuCR = "search_bin_v2_singleMuCR_jetpt30_h";
		SingleElCR = "search_bin_v2_singleElCR_jetpt30_h";
		SR_weight = "search_bin_v2_Stop0l_evtWeight_jetpt30_h";
	}

	if (SBv2_highdm)
	{
		SR = "search_bin_v2_highdm_h";
		SR_loose_bin = "search_bin_v2_highdm_loose_bin_h";
		SingleMuCR = "search_bin_v2_singleMuCR_highdm_h";
		SingleMuCR_loose_bin = "search_bin_v2_singleMuCR_highdm_loose_bin_h";
		SingleElCR = "search_bin_v2_singleElCR_highdm_h";
		SingleElCR_loose_bin = "search_bin_v2_singleElCR_highdm_loose_bin_h";
		mid_bin = -1;
	}

	if (SBv2_highdm_validation)
	{
		SR = "search_bin_v2_highdm_validation_h";
		SingleMuCR = "search_bin_v2_singleMuCR_highdm_h";
		SingleElCR = "search_bin_v2_singleElCR_highdm_h";
		SR_weight = "search_bin_v2_highdm_validation_Stop0l_evtWeight_h";
		low_or_high = "_highdm";
		mid_bin = -1;
	}

	if (SBv3_highdm_validation)
	{
		SR = "search_bin_v3_highdm_validation_h";
		SingleMuCR = "search_bin_v2_singleMuCR_highdm_h";
		SingleElCR = "search_bin_v2_singleElCR_highdm_h";
		SR_weight = "search_bin_v3_highdm_validation_Stop0l_evtWeight_h";
		low_or_high = "_highdm";
		mid_bin = -1;
	}

	if (SBv2_lowdm)
	{
		SR = "search_bin_v2_lowdm_h";
		SingleMuCR = "search_bin_v2_singleMuCR_lowdm_h";
		SingleElCR = "search_bin_v2_singleElCR_lowdm_h";
		mid_bin = 999;
	}

	if (SBv2_lowdm_validation)
	{
		SR = "search_bin_v2_lowdm_validation_h";
		SingleMuCR = "search_bin_v2_singleMuCR_highdm_h";
		SingleElCR = "search_bin_v2_singleElCR_highdm_h";
		SR_weight = "search_bin_v2_lowdm_validation_Stop0l_evtWeight_h";
		low_or_high = "_lowdm";
		mid_bin = 999;
	}

	if (SBv2_lowdm_more_MET)
	{
		SR = "search_bin_v2_lowdm_more_MET_h";
		SingleMuCR = "search_bin_v2_singleMuCR_lowdm_more_MET_h";
		SingleElCR = "search_bin_v2_singleElCR_lowdm_more_MET_h";
		mid_bin = 999;
	}

	if (SBv2_lowdm_high_ISRpt)
	{
		SR = "search_bin_v2_lowdm_high_ISRpt_h";
		SingleMuCR = "search_bin_v2_singleMuCR_lowdm_high_ISRpt_h";
		SingleElCR = "search_bin_v2_singleElCR_lowdm_high_ISRpt_h";
		mid_bin = 999;
	}

	if (more_HT_bins)
	{
		SR = "search_bin_more_HT_bins_h";
		SingleMuCR = "search_bin_more_HT_bins_singleMuCR_h";
		SingleElCR = "search_bin_more_HT_bins_singleElCR_h";
	}

	THStack *hs = new THStack();

	if (make_QCD_datacard)
	{
		TH1F * pro = NULL;
		TH1F * avg_weight_sq = new TH1F("avg_weight_sq","avg_weight_sq",NSB,0,NSB);
		TH1F * avg_weight = new TH1F("avg_weight","avg_weight",NSB,0,NSB);

		if (true)
		{
			TString sp = "QCD_HT_300to500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			NSB = h1->GetSize() - 2;
			//if(only_lowdm) NSB = mid_bin;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("QCD");
		}

		if (false)
		{
			TString sp = "QCD_HT_200to300";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_HT_500to700";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_HT_700to1000";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_HT_1000to1500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_HT_1500to2000";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_HT_2000toInf";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);

		if(do_validation)
		{
			TFile out_file("validation/QCD_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}

		std::ofstream QCDfile (("datacards/temp/qcd.txt"));
		if (QCDfile.is_open())
		{
			QCDfile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			QCDfile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			QCDfile << "sample = qcd     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			QCDfile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ QCDfile << "bin" << i+1 << " "; } QCDfile << "\n";

			QCDfile << "\n# Predicted central numbers (need from all backgrounds)\n";
			QCDfile << "rate = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro->GetBinContent(i+1) > 0) QCDfile << pro->GetBinContent(i+1) << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			QCDfile << "cs_event = ";
			std::vector<int> cs_event(NSB,0);
			for(int i=0;i<NSB;i++)
			{ 
				if(avg_weight_sq->GetBinContent(i+1) > 0) cs_event[i] = round(pro->GetBinContent(i+1) / avg_weight_sq->GetBinContent(i+1) * avg_weight->GetBinContent(i+1));
				if(cs_event[i] > 0) QCDfile << cs_event[i] << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			QCDfile << "avg_weight = ";
			for(int i=0;i<NSB;i++)
			{
				//if(avg_weight->GetBinContent(i+1) > 0) QCDfile << avg_weight_sq->GetBinContent(i+1) / avg_weight->GetBinContent(i+1) << " ";
				if(cs_event[i] > 0) QCDfile << pro->GetBinContent(i+1) / cs_event[i] << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			//QCDfile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ QCDfile << pro->GetBinErrorUp(i+1) / pro->GetBinContent(i+1) << " "; } QCDfile << "\n";
			//QCDfile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ QCDfile << pro->GetBinErrorLow(i+1) / pro->GetBinContent(i+1) << " "; } QCDfile << "\n";

			QCDfile << "syst_unc_all_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) QCDfile << 0.84 << " ";
				else QCDfile << 1.5 << " ";
			}
			QCDfile << "\n";

			QCDfile << "syst_unc_all_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) QCDfile << 0.84 << " ";
				else QCDfile << 0.998 << " ";
			}
			QCDfile << "\n";

			QCDfile.close();
		}
		else std::cout << "Unable to open QCDfile";
	}

	if (make_smear_QCD_datacard)
	{
		TH1F * pro = NULL;
		TH1F * avg_weight_sq = new TH1F("avg_weight_sq","avg_weight_sq",NSB,0,NSB);
		TH1F * avg_weight = new TH1F("avg_weight","avg_weight",NSB,0,NSB);

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_300to500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				//avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				//avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
				avg_weight_sq->SetBinContent(i+1,scale*scale);
				avg_weight->SetBinContent(i+1,scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("QCD");
		}

		if (false)
		{
			TString sp = "QCD_SMEAR_HT_200to300";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_500to700";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_700to1000";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_1000to1500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_1500to2000";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "QCD_SMEAR_HT_2000toInf";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR_weight);
			//TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");
			TH1F *h2 = (TH1F*)f1->Get(folder + SR);

			NSB = h1->GetSize() - 2;
			first_bin = int(h1->GetXaxis()->GetBinCenter(1));

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * 1000);

			for(int i = 0; i < NSB; i++)
			{
				double scale = 0.0;
				if(h2->GetBinContent(i+1) > 0) scale = h1->GetBinContent(i+1) / h2->GetBinContent(i+1);
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+scale);
			}

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);

		if(do_validation)
		{
			TFile out_file("validation/QCD_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}

		std::ofstream QCDfile (("datacards/temp/qcd.txt"));
		if (QCDfile.is_open())
		{
			QCDfile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			QCDfile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			QCDfile << "sample = qcd     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			QCDfile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ QCDfile << "bin" << i+1 << " "; } QCDfile << "\n";

			QCDfile << "\n# Predicted central numbers (need from all backgrounds)\n";
			QCDfile << "rate = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro->GetBinContent(i+1) > 0) QCDfile << pro->GetBinContent(i+1) << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			QCDfile << "cs_event = ";
			std::vector<int> cs_event(NSB,0);
			for(int i=0;i<NSB;i++)
			{
				//std::cout << "avg_weight->GetBinContent(i+1) " << avg_weight->GetBinContent(i+1) << std::endl; 
				if(avg_weight_sq->GetBinContent(i+1) > 0) cs_event[i] = round(pro->GetBinContent(i+1) / avg_weight_sq->GetBinContent(i+1) * avg_weight->GetBinContent(i+1));
				if(cs_event[i] > 0) QCDfile << cs_event[i] << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			QCDfile << "avg_weight = ";
			for(int i=0;i<NSB;i++)
			{
				//if(avg_weight->GetBinContent(i+1) > 0) QCDfile << avg_weight_sq->GetBinContent(i+1) / avg_weight->GetBinContent(i+1) << " ";
				if(cs_event[i] > 0) QCDfile << pro->GetBinContent(i+1) / cs_event[i] << " ";
				else QCDfile << "0.0000" << " ";
			}
			QCDfile << "\n";

			//QCDfile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ QCDfile << pro->GetBinErrorUp(i+1) / pro->GetBinContent(i+1) << " "; } QCDfile << "\n";
			//QCDfile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ QCDfile << pro->GetBinErrorLow(i+1) / pro->GetBinContent(i+1) << " "; } QCDfile << "\n";

			QCDfile << "syst_unc_all_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) QCDfile << 0.84 << " ";
				else QCDfile << 1.5 << " ";
			}
			QCDfile << "\n";

			QCDfile << "syst_unc_all_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) QCDfile << 0.84 << " ";
				else QCDfile << 0.998 << " ";
			}
			QCDfile << "\n";

			QCDfile.close();
		}
		else std::cout << "Unable to open QCDfile";
	}

	if (make_Zinv_datacard)
	{
		TH1F * pro = NULL;
		TH1F * avg_weight_sq = new TH1F("avg_weight_sq","avg_weight_sq",NSB,0,NSB);
		TH1F * avg_weight = new TH1F("avg_weight","avg_weight",NSB,0,NSB);

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_100to200";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("zjets");
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_200to400";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_400to600";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_600to800";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_800to1200";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_1200to2500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "ZJetsToNuNu_HT_2500toInf";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
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

			pro->Add(h1);
		}

		if(!do_validation && !compare_TF)pro->Scale(zinv_sf);
		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);

		if(do_validation)
		{
			TFile out_file("validation/Zinv_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}

		std::ofstream Zinvfile (("datacards/temp/zinv.txt"));
		if (Zinvfile.is_open())
		{
			Zinvfile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			Zinvfile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			Zinvfile << "sample = zinv     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			Zinvfile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ Zinvfile << "bin" << i+1 << " "; } Zinvfile << "\n";

			Zinvfile << "\n# Predicted central numbers (need from all backgrounds)\n";
			Zinvfile << "rate = "; for(int i=0;i<NSB;i++){ Zinvfile << pro->GetBinContent(i+1) << " "; } Zinvfile << "\n";

			Zinvfile << "cs_event = ";
			std::vector<int> cs_event(NSB,0);
			for(int i=0;i<NSB;i++)
			{ 
				if(avg_weight_sq->GetBinContent(i+1) > 0) cs_event[i] = round(pro->GetBinContent(i+1) / avg_weight_sq->GetBinContent(i+1) * avg_weight->GetBinContent(i+1));
				Zinvfile << cs_event[i] << " ";
			}
			Zinvfile << "\n";

			Zinvfile << "avg_weight = ";
			for(int i=0;i<NSB;i++)
			{
				//if(avg_weight->GetBinContent(i+1) > 0) Zinvfile << avg_weight_sq->GetBinContent(i+1) / avg_weight->GetBinContent(i+1) << " ";
				if(cs_event[i] > 0) Zinvfile << pro->GetBinContent(i+1) / cs_event[i] << " ";
				else Zinvfile << "0.0000" << " ";
			}
			Zinvfile << "\n";

			//Zinvfile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ Zinvfile << pro->GetBinErrorUp(i+1) / pro->GetBinContent(i+1) << " "; } Zinvfile << "\n";
			//Zinvfile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ Zinvfile << pro->GetBinErrorLow(i+1) / pro->GetBinContent(i+1) << " "; } Zinvfile << "\n";

			Zinvfile << "syst_unc_shape_central_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.03 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_shape_central_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.03 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";

			Zinvfile << "syst_unc_shape_stat_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.0 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_shape_stat_dn = ";
			for(int i=0;i<NSB;i++){
				if(i < mid_bin) Zinvfile << 0.0 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";

			if(do_photon_unc)
			{
			Zinvfile << "syst_unc_photon_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.5 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_photon_dn = ";
			for(int i=0;i<NSB;i++){
				if(i < mid_bin) Zinvfile << 0.5 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			}

			Zinvfile << "syst_unc_norm_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.15 << " ";
				else Zinvfile << 0.076 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_norm_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.15 << " ";
				else Zinvfile << 0.076 << " ";
			}
			Zinvfile << "\n";

			Zinvfile << "syst_unc_jec_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.13 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_jec_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.13 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";

			Zinvfile << "syst_unc_btag_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.07 << " ";
				else Zinvfile << 0.1 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_btag_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.07 << " ";
				else Zinvfile << 0.1 << " ";
			}
			Zinvfile << "\n";

			Zinvfile << "syst_unc_pdf_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.00 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_pdf_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.00 << " ";
				else Zinvfile << 0.2 << " ";
			}
			Zinvfile << "\n";
			//Zinvfile << "syst_unc_mc_stat_up = "      ; for(int i=0;i<NSB;i++){ Zinvfile << 0.4 << " "; } Zinvfile << "\n";
			//Zinvfile << "syst_unc_mc_stat_dn = "      ; for(int i=0;i<NSB;i++){ Zinvfile << 0.4 << " "; } Zinvfile << "\n";
			Zinvfile << "syst_unc_trig_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.00 << " ";
				else Zinvfile << 0.05 << " ";
			}
			Zinvfile << "\n";
			Zinvfile << "syst_unc_trig_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) Zinvfile << 0.00 << " ";
				else Zinvfile << 0.05 << " ";
			}
			Zinvfile << "\n";
			Zinvfile.close();
		}
		else std::cout << "Unable to open Zinvfile";
	}

	if (make_LL_datacard)
	{
		TH1F * pro = NULL;
		TH1F * pro_loose_bin = NULL;
		TH1F * pro_singleMuCR = NULL;
		TH1F * pro_singleMuCR_loose_bin = NULL;
		TH1F * pro_singleElCR = NULL;
		TH1F * pro_singleElCR_loose_bin = NULL;

		if (true)
		{
			TString sp = "WJetsToLNu_HT_100to200";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("LL_SR");
			pro_singleMuCR = (TH1F*)h1_singleMuCR->Clone("LL_singleMuCR");
			pro_singleElCR = (TH1F*)h1_singleElCR->Clone("LL_singleElCR");
			if(compare_TF)
			{
				pro_loose_bin = (TH1F*)h1_loose_bin->Clone("LL_SR_loose_bin");
				pro_singleMuCR_loose_bin = (TH1F*)h1_singleMuCR_loose_bin->Clone("LL_singleMuCR_loose_bin");
				pro_singleElCR_loose_bin = (TH1F*)h1_singleElCR_loose_bin->Clone("LL_singleElCR_loose_bin");
			}
		}

		if (false)
		{
			TString sp = "WJetsToLNu_HT_70to100";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_200to400";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_400to600";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_600to800";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_800to1200";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_1200to2500";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "WJetsToLNu_HT_2500toInf";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}


		if (true)
		{
			TString sp = "TTbarSingleLepT";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;
			std::cout << "unscale mu bin 1 content " << h1_singleMuCR->GetBinContent(1) << std::endl;
			std::cout << "unscale mu bin 2 content " << h1_singleMuCR->GetBinContent(2) << std::endl;
			std::cout << "unscale mu bin 1 error " << h1_singleMuCR->GetBinError(1) << std::endl;
			std::cout << "unscale mu bin 2 error " << h1_singleMuCR->GetBinError(2) << std::endl;
			std::cout << "unscale el bin 1 content " << h1_singleElCR->GetBinContent(1) << std::endl;
			std::cout << "unscale el bin 2 content " << h1_singleElCR->GetBinContent(2) << std::endl;
			std::cout << "unscale el bin 1 error " << h1_singleElCR->GetBinError(1) << std::endl;
			std::cout << "unscale el bin 2 error " << h1_singleElCR->GetBinError(2) << std::endl;

			std::cout << "=========================================================================" << std::endl;
			std::cout << "lumi = " << lumi << " CrossSection.at(sp) " << CrossSection.at(sp) << " all_events " << all_events << std::endl;
			std::cout << "=========================================================================" << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			std::cout << "scaled mu bin 1 content " << h1_singleMuCR->GetBinContent(1) << std::endl;
			std::cout << "scaled mu bin 2 content " << h1_singleMuCR->GetBinContent(2) << std::endl;
			std::cout << "scaled mu bin 1 error " << h1_singleMuCR->GetBinError(1) << std::endl;
			std::cout << "scaled mu bin 2 error " << h1_singleMuCR->GetBinError(2) << std::endl;
			std::cout << "scaled el bin 1 content " << h1_singleElCR->GetBinContent(1) << std::endl;
			std::cout << "scaled el bin 2 content " << h1_singleElCR->GetBinContent(2) << std::endl;
			std::cout << "scaled el bin 1 error " << h1_singleElCR->GetBinError(1) << std::endl;
			std::cout << "scaled el bin 2 error " << h1_singleElCR->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "TTbarSingleLepTbar";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "TTbarDiLep";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "ST_s_lep";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "ST_t_antitop";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "ST_t_top";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "ST_tW_antitop_incl";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if (true)
		{
			TString sp = "ST_tW_top_incl";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			TH1F *h1_loose_bin = (TH1F*)f1->Get(folder + SR_loose_bin);
			TH1F *h1_singleMuCR = (TH1F*)f1->Get(folder + SingleMuCR);
			TH1F *h1_singleMuCR_loose_bin = (TH1F*)f1->Get(folder + SingleMuCR_loose_bin);
			TH1F *h1_singleElCR = (TH1F*)f1->Get(folder + SingleElCR);
			TH1F *h1_singleElCR_loose_bin = (TH1F*)f1->Get(folder + SingleElCR_loose_bin);
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();
			h1_singleMuCR->Sumw2();
			h1_singleElCR->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleMuCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			h1_singleElCR->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);
			//h1_singleMuCR->Scale(lumi * 1000);
			//h1_singleElCR->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;
			pro->Add(h1);
			pro_singleMuCR->Add(h1_singleMuCR);
			pro_singleElCR->Add(h1_singleElCR);
			if(compare_TF)
			{
				pro_loose_bin->Add(h1_loose_bin);
				pro_singleMuCR_loose_bin->Add(h1_singleMuCR_loose_bin);
				pro_singleElCR_loose_bin->Add(h1_singleElCR_loose_bin);
			}
		}

		if(!do_validation && !compare_TF)pro->Scale(ll_sf);
		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);
		pro_singleMuCR->SetBinErrorOption(TH1::kPoisson);
		pro_singleElCR->SetBinErrorOption(TH1::kPoisson);
		if(do_validation)
		{
			TFile out_file("validation/LL_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}
		if(compare_TF)
		{
			pro_loose_bin->SetBinErrorOption(TH1::kPoisson);
			pro_singleMuCR_loose_bin->SetBinErrorOption(TH1::kPoisson);
			pro_singleElCR_loose_bin->SetBinErrorOption(TH1::kPoisson);
		}

		//========= Direct TF ===========
		TH1F * pro_combCR = (TH1F*)pro_singleMuCR->Clone("pro_combCR");
		pro_combCR->Add(pro_singleElCR);
		pro_combCR->Scale(0.5);
		TH1F * TF_singleMuCR = (TH1F*)pro->Clone("TF_singleMuCR");
		TF_singleMuCR->Divide(pro_singleMuCR);
		TH1F * TF_singleElCR = (TH1F*)pro->Clone("TF_singleElCR");
		TF_singleElCR->Divide(pro_singleElCR);
		TH1F * TF_combCR = (TH1F*)pro->Clone("TF_combCR");
		TF_combCR->Divide(pro_combCR);
		TH1F * Pred_direct_singleMu = (TH1F*)TF_singleMuCR->Clone("Pred_direct_singleMu");
		Pred_direct_singleMu->Multiply(pro_singleMuCR);
		TH1F * Pred_direct_singleEl = (TH1F*)TF_singleElCR->Clone("Pred_direct_singleEl");
		Pred_direct_singleEl->Multiply(pro_singleElCR);

		//========= TF CR to SR, loose bin ==============
		TH1F * TF_singleMuCR_loose_bin = NULL;
		TH1F * TF_singleElCR_loose_bin = NULL;
		if(compare_TF)
		{
			TF_singleMuCR_loose_bin = (TH1F*)pro_loose_bin->Clone("TF_singleMuCR_loose_bin");
			TF_singleMuCR_loose_bin->Divide(pro_singleMuCR_loose_bin);
			TF_singleElCR_loose_bin = (TH1F*)pro_loose_bin->Clone("TF_singleElCR_loose_bin");
			TF_singleElCR_loose_bin->Divide(pro_singleElCR_loose_bin);
		}

		//========= TF SR extrapolation ==============
		TH1F * TF_SR_extrap = NULL;
		if(compare_TF)
		{
			TF_SR_extrap = (TH1F*)pro->Clone("TF_SR_extrap");
			TF_SR_extrap->Divide(pro_loose_bin);
		}

		//========= TF (CRtoSR) * (SR extrap)==============
		TH1F * TF_singleMu = NULL;
		TH1F * TF_singleEl = NULL;
		TH1F * Pred_extrap_singleMu = NULL;
		TH1F * Pred_extrap_singleEl = NULL;
		if(compare_TF)
		{
			TF_singleMu = (TH1F*)TF_singleMuCR_loose_bin->Clone("TF_singleMu");
			TF_singleMu->Multiply(TF_SR_extrap);
			TF_singleEl = (TH1F*)TF_singleElCR_loose_bin->Clone("TF_singleEl");
			TF_singleEl->Multiply(TF_SR_extrap);
			Pred_extrap_singleMu = (TH1F*)TF_singleMu->Clone("Pred_extrap_singleMu");
			Pred_extrap_singleMu->Multiply(pro_singleMuCR_loose_bin);
			Pred_extrap_singleEl = (TH1F*)TF_singleEl->Clone("Pred_extrap_singleEl");
			Pred_extrap_singleEl->Multiply(pro_singleElCR_loose_bin);

			TFile out_file("TF_compare.root","RECREATE");
			TF_singleMuCR->Write();
			TF_singleElCR->Write();
			TF_singleMuCR_loose_bin->Write();
			TF_singleElCR_loose_bin->Write();
			TF_SR_extrap->Write();
			TF_singleMu->Write();
			TF_singleEl->Write();
			Pred_direct_singleMu->Write();
			Pred_direct_singleEl->Write();
			Pred_extrap_singleMu->Write();
			Pred_extrap_singleEl->Write();
			out_file.Close();
		}

		std::ofstream LL_mu_file (("datacards/temp/comb_mu.txt"));
		if (LL_mu_file.is_open())
		{
			LL_mu_file << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			LL_mu_file << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			LL_mu_file << "sample = comb     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			LL_mu_file << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ LL_mu_file << "bin" << i+1 << " "; } LL_mu_file << "\n";

			LL_mu_file << "\n# Predicted central numbers (need from all backgrounds)\n";
			LL_mu_file << "rate = "; for(int i=0;i<NSB;i++){ LL_mu_file << pro->GetBinContent(i+1) << " "; } LL_mu_file << "\n";

			//LL_mu_file << "cs_event = "; for(int i=0;i<NSB;i++){ LL_mu_file << pro_singleMuCR->GetBinContent(i+1) << " "; } LL_mu_file << "\n";

			LL_mu_file << "# fin_TF_to_mu = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleMuCR->GetBinContent(i+1) > 0) LL_mu_file << TF_singleMuCR->GetBinContent(i+1) << " ";
				else LL_mu_file << "1.0000" << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "# fin_TF_to_ele = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleElCR->GetBinContent(i+1) > 0) LL_mu_file << TF_singleElCR->GetBinContent(i+1) << " ";
				else LL_mu_file << "1.0000" << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "# stat_unc_abs_up = "; for(int i=0;i<NSB;i++){ LL_mu_file << pro_singleMuCR->GetBinErrorUp(i+1) << " "; } LL_mu_file << "\n";
			LL_mu_file << "# stat_unc_abs_dn = "; for(int i=0;i<NSB;i++){ LL_mu_file << pro_singleMuCR->GetBinErrorLow(i+1) << " "; } LL_mu_file << "\n";
			LL_mu_file << "stat_unc_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_singleMuCR->GetBinContent(i+1) > 0) LL_mu_file << pro_singleMuCR->GetBinErrorUp(i+1) / pro_singleMuCR->GetBinContent(i+1) << " ";
				else LL_mu_file << 0.0000 << " ";
			} LL_mu_file << "\n";
			LL_mu_file << "stat_unc_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_singleMuCR->GetBinContent(i+1) > 0) LL_mu_file << pro_singleMuCR->GetBinErrorLow(i+1) / pro_singleMuCR->GetBinContent(i+1) << " ";
				else LL_mu_file << 0.0000 << " ";
			} LL_mu_file << "\n";

			LL_mu_file << "syst_unc_TF_stat_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleMuCR->GetBinContent(i+1) > 0) LL_mu_file << TF_singleMuCR->GetBinErrorUp(i+1) / TF_singleMuCR->GetBinContent(i+1) << " ";
				else LL_mu_file << 0.0000 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_TF_stat_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleMuCR->GetBinContent(i+1) > 0) LL_mu_file << TF_singleMuCR->GetBinErrorLow(i+1) / TF_singleMuCR->GetBinContent(i+1) << " ";
				else LL_mu_file << 0.0000 << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "syst_unc_SF_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.08 << " ";
				else LL_mu_file << 0.2 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_SF_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.08 << " ";
				else LL_mu_file << 0.2 << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "syst_unc_JEC_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.09 << " ";
				else LL_mu_file << 0.2 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_JEC_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.09 << " ";
				else LL_mu_file << 0.2 << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "syst_unc_ISR_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.05 << " ";
				else LL_mu_file << 0.05 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_ISR_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.05 << " ";
				else LL_mu_file << 0.05 << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "syst_unc_pdf_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.05 << " ";
				else LL_mu_file << 0.1 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_pdf_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.05 << " ";
				else LL_mu_file << 0.1 << " ";
			}
			LL_mu_file << "\n";

			LL_mu_file << "syst_unc_btag_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.04 << " ";
				else LL_mu_file << 0.01 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file << "syst_unc_btag_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_mu_file << 0.04 << " ";
				else LL_mu_file << 0.01 << " ";
			}
			LL_mu_file << "\n";
			LL_mu_file.close();
		}
		else std::cout << "Unable to open LL_mu_file";

		std::ofstream LL_ele_file (("datacards/temp/comb_ele.txt"));
		if (LL_ele_file.is_open())
		{
			LL_ele_file << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			LL_ele_file << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			LL_ele_file << "sample = comb     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			LL_ele_file << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ LL_ele_file << "bin" << i+1 << " "; } LL_ele_file << "\n";

			LL_ele_file << "\n# Predicted central numbers (need from all backgrounds)\n";
			LL_ele_file << "rate = "; for(int i=0;i<NSB;i++){ LL_ele_file << pro->GetBinContent(i+1) << " "; } LL_ele_file << "\n";

			//LL_ele_file << "cs_event = "; for(int i=0;i<NSB;i++){ LL_ele_file << pro_singleMuCR->GetBinContent(i+1) << " "; } LL_ele_file << "\n";

			LL_ele_file << "# fin_TF_to_mu = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleMuCR->GetBinContent(i+1) > 0) LL_ele_file << TF_singleMuCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "1.0000" << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "# fin_TF_to_ele = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleElCR->GetBinContent(i+1) > 0) LL_ele_file << TF_singleElCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "1.0000" << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "# stat_unc_abs_up = "; for(int i=0;i<NSB;i++){ LL_ele_file << pro_singleElCR->GetBinErrorUp(i+1) << " "; } LL_ele_file << "\n";
			LL_ele_file << "# stat_unc_abs_dn = "; for(int i=0;i<NSB;i++){ LL_ele_file << pro_singleElCR->GetBinErrorLow(i+1) << " "; } LL_ele_file << "\n";
			LL_ele_file << "stat_unc_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_singleElCR->GetBinContent(i+1) > 0) LL_ele_file << pro_singleElCR->GetBinErrorUp(i+1) / pro_singleElCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "0.0000" << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "stat_unc_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_singleElCR->GetBinContent(i+1) > 0) LL_ele_file << pro_singleElCR->GetBinErrorLow(i+1) / pro_singleElCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "0.0000" << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_TF_stat_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleElCR->GetBinContent(i+1)) LL_ele_file << TF_singleElCR->GetBinErrorUp(i+1) / TF_singleElCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "0.0000" << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_TF_stat_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleElCR->GetBinContent(i+1)) LL_ele_file << TF_singleElCR->GetBinErrorLow(i+1) / TF_singleElCR->GetBinContent(i+1) << " ";
				else LL_ele_file << "0.0000" << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_SF_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.08 << " ";
				else LL_ele_file << 0.2 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_SF_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.08 << " ";
				else LL_ele_file << 0.2 << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_JEC_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.09 << " ";
				else LL_ele_file << 0.2 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_JEC_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.09 << " ";
				else LL_ele_file << 0.2 << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_ISR_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.05 << " ";
				else LL_ele_file << 0.05 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_ISR_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.05 << " ";
				else LL_ele_file << 0.05 << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_pdf_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.05 << " ";
				else LL_ele_file << 0.1 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_pdf_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.05 << " ";
				else LL_ele_file << 0.1 << " ";
			}
			LL_ele_file << "\n";

			LL_ele_file << "syst_unc_btag_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.04 << " ";
				else LL_ele_file << 0.01 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file << "syst_unc_btag_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_ele_file << 0.04 << " ";
				else LL_ele_file << 0.01 << " ";
			}
			LL_ele_file << "\n";
			LL_ele_file.close();
		}
		else std::cout << "Unable to open LL_ele_file";

		std::ofstream LL_comb_file (("datacards/temp/comb_comb.txt"));
		if (LL_comb_file.is_open())
		{
			LL_comb_file << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			LL_comb_file << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			LL_comb_file << "sample = comb     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			LL_comb_file << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ LL_comb_file << "bin" << i+1 << " "; } LL_comb_file << "\n";

			LL_comb_file << "\n# Predicted central numbers (need from all backgrounds)\n";
			LL_comb_file << "rate = "; for(int i=0;i<NSB;i++){ LL_comb_file << pro->GetBinContent(i+1) << " "; } LL_comb_file << "\n";

			//LL_comb_file << "cs_event = "; for(int i=0;i<NSB;i++){ LL_comb_file << pro_singleMuCR->GetBinContent(i+1) << " "; } LL_comb_file << "\n";

			LL_comb_file << "# fin_TF_to_mu = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleMuCR->GetBinContent(i+1) > 0) LL_comb_file << TF_singleMuCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "1.0000" << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "# fin_TF_to_ele = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_singleElCR->GetBinContent(i+1) > 0) LL_comb_file << TF_singleElCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "1.0000" << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "# stat_unc_abs_up = "; for(int i=0;i<NSB;i++){ LL_comb_file << pro_combCR->GetBinErrorUp(i+1) / 1.414 << " "; } LL_comb_file << "\n";
			LL_comb_file << "# stat_unc_abs_dn = "; for(int i=0;i<NSB;i++){ LL_comb_file << pro_combCR->GetBinErrorLow(i+1) / 1.414 << " "; } LL_comb_file << "\n";
			LL_comb_file << "stat_unc_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_combCR->GetBinContent(i+1) > 0) LL_comb_file << pro_combCR->GetBinErrorUp(i+1) / pro_combCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "0.0000" << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "stat_unc_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(pro_combCR->GetBinContent(i+1) > 0) LL_comb_file << pro_combCR->GetBinErrorLow(i+1) / pro_combCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "0.0000" << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_TF_stat_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_combCR->GetBinContent(i+1) > 0) LL_comb_file << TF_combCR->GetBinErrorUp(i+1) / TF_combCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "0.0000" << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_TF_stat_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(TF_combCR->GetBinContent(i+1) > 0) LL_comb_file << TF_combCR->GetBinErrorLow(i+1) / TF_combCR->GetBinContent(i+1) << " ";
				else LL_comb_file << "0.0000" << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_SF_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.08 << " ";
				else LL_comb_file << 0.2 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_SF_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.08 << " ";
				else LL_comb_file << 0.2 << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_JEC_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.09 << " ";
				else LL_comb_file << 0.2 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_JEC_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.09 << " ";
				else LL_comb_file << 0.2 << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_ISR_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.05 << " ";
				else LL_comb_file << 0.05 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_ISR_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.05 << " ";
				else LL_comb_file << 0.05 << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_pdf_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.05 << " ";
				else LL_comb_file << 0.1 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_pdf_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.05 << " ";
				else LL_comb_file << 0.1 << " ";
			}
			LL_comb_file << "\n";

			LL_comb_file << "syst_unc_btag_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.04 << " ";
				else LL_comb_file << 0.01 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file << "syst_unc_btag_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) LL_comb_file << 0.04 << " ";
				else LL_comb_file << 0.01 << " ";
			}
			LL_comb_file << "\n";
			LL_comb_file.close();
		}
		else std::cout << "Unable to open LL_comb_file";
	}

	if (make_TTZ_datacard)
	{
		TH1F * pro = NULL;
		TH1F * avg_weight_sq = new TH1F("avg_weight_sq","avg_weight_sq",NSB,0,NSB);
		TH1F * avg_weight = new TH1F("avg_weight","avg_weight",NSB,0,NSB);

		if (true)
		{
			TString sp = "TTZToLLNuNu";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("TTZ");
		}

		if (true)
		{
			TString sp = "TTZToQQ";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);

		if(do_validation)
		{
			TFile out_file("validation/TTZ_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}

		std::ofstream TTZfile (("datacards/temp/ttz.txt"));
		if (TTZfile.is_open())
		{
			TTZfile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			TTZfile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			TTZfile << "sample = ttz     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			TTZfile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ TTZfile << "bin" << i+1 << " "; } TTZfile << "\n";

			TTZfile << "\n# Predicted central numbers (need from all backgrounds)\n";
			TTZfile << "rate = "; for(int i=0;i<NSB;i++){ TTZfile << pro->GetBinContent(i+1) << " "; } TTZfile << "\n";

			TTZfile << "cs_event = ";
			std::vector<int> cs_event(NSB,0);
			for(int i=0;i<NSB;i++)
			{ 
				if(avg_weight_sq->GetBinContent(i+1) > 0) cs_event[i] = round(pro->GetBinContent(i+1) / avg_weight_sq->GetBinContent(i+1) * avg_weight->GetBinContent(i+1));
				TTZfile << cs_event[i] << " ";
			}
			TTZfile << "\n";

			TTZfile << "avg_weight = ";
			for(int i=0;i<NSB;i++)
			{
				//if(avg_weight->GetBinContent(i+1) > 0) TTZfile << avg_weight_sq->GetBinContent(i+1) / avg_weight->GetBinContent(i+1) << " ";
				if(cs_event[i] > 0) TTZfile << pro->GetBinContent(i+1) / cs_event[i] << " ";
				else TTZfile << "0.0000" << " ";
			}
			TTZfile << "\n";

			//TTZfile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ TTZfile << pro->GetBinErrorUp(i+1) / pro->GetBinContent(i+1) << " "; } TTZfile << "\n";
			//TTZfile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ TTZfile << pro->GetBinErrorLow(i+1) / pro->GetBinContent(i+1) << " "; } TTZfile << "\n";

			TTZfile << "syst_unc_up = "         ; for(int i=0;i<NSB;i++){ TTZfile << 0.3 * pro->GetBinContent(i+1) << " "; } TTZfile << "\n";
			TTZfile << "syst_unc_dn = "         ; for(int i=0;i<NSB;i++){ TTZfile << 0.3 * pro->GetBinContent(i+1) << " "; } TTZfile << "\n";
			TTZfile.close();
		}
		else std::cout << "Unable to open TTZfile";
	}

	if (make_Rare_datacard)
	{
		TH1F * pro = NULL;
		TH1F * avg_weight_sq = new TH1F("avg_weight_sq","avg_weight_sq",NSB,0,NSB);
		TH1F * avg_weight = new TH1F("avg_weight","avg_weight",NSB,0,NSB);

		if (true)
		{
			TString sp = "ZZTo2L2Nu";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro = (TH1F*)h1->Clone("rare");
		}

		if (true)
		{
			TString sp = "ZZTo2Q2Nu";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "WZ";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "WWTo2L2Nu";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		if (true)
		{
			TString sp = "WWToLNuQQ";

			TFile *f1 = new TFile("results/" + sp + year + ".root");
			TH1F *h1 = (TH1F*)f1->Get(folder + SR);
			//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
			TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

			double all_events = h2->GetBinContent(1);

			h1->Sumw2();

			std::cout << "\n" << sp << std::endl;
			std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

			double scale = lumi * CrossSection.at(sp) * 1000 / all_events;
			for(int i = 0; i < NSB; i++)
			{
				if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);

				avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
				avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
			}

			h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
			//h1->Scale(lumi * 1000);

			std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
			std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
			std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
			std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

			pro->Add(h1);
		}

		hs->Add(pro);
		pro->SetBinErrorOption(TH1::kPoisson);

		if(do_validation)
		{
			TFile out_file("validation/Rare_validation" + low_or_high + year + ".root","RECREATE");
			pro->Write();
			out_file.Close();
		}

		std::ofstream Rarefile (("datacards/temp/rare.txt"));
		if (Rarefile.is_open())
		{
			Rarefile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			Rarefile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			Rarefile << "sample = rare     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			Rarefile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ Rarefile << "bin" << i+1 << " "; } Rarefile << "\n";

			Rarefile << "\n# Predicted central numbers (need from all backgrounds)\n";
			Rarefile << "rate = "; for(int i=0;i<NSB;i++){ Rarefile << pro->GetBinContent(i+1) << " "; } Rarefile << "\n";

			Rarefile << "cs_event = ";
			std::vector<int> cs_event(NSB,0);
			for(int i=0;i<NSB;i++)
			{ 
				if(avg_weight_sq->GetBinContent(i+1) > 0) cs_event[i] = round(pro->GetBinContent(i+1) / avg_weight_sq->GetBinContent(i+1) * avg_weight->GetBinContent(i+1));
				Rarefile << cs_event[i] << " ";
			}
			Rarefile << "\n";

			Rarefile << "avg_weight = ";
			for(int i=0;i<NSB;i++)
			{
				//if(avg_weight->GetBinContent(i+1) > 0) Rarefile << avg_weight_sq->GetBinContent(i+1) / avg_weight->GetBinContent(i+1) << " ";
				if(cs_event[i] > 0) Rarefile << pro->GetBinContent(i+1) / cs_event[i] << " ";
				else Rarefile << "0.0000" << " ";
			}
			Rarefile << "\n";

			//Rarefile << "stat_unc_up = "; for(int i=0;i<NSB;i++){ Rarefile << "0.00" << " "; } Rarefile << "\n";
			//Rarefile << "stat_unc_dn = "; for(int i=0;i<NSB;i++){ Rarefile << "0.00" << " "; } Rarefile << "\n";

			Rarefile << "syst_unc_up = "         ; for(int i=0;i<NSB;i++){ Rarefile << 0.1 * pro->GetBinContent(i+1) << " "; } Rarefile << "\n";
			Rarefile << "syst_unc_dn = "         ; for(int i=0;i<NSB;i++){ Rarefile << 0.1 * pro->GetBinContent(i+1) << " "; } Rarefile << "\n";
			Rarefile.close();
		}
		else std::cout << "Unable to open Rarefile";
	}

	if (make_data_datacard)
	{

		std::ofstream Datafile (("datacards/temp/data.txt"));
		if (Datafile.is_open())
		{
			Datafile << "luminosity = " << lumi << "     # in pb-1 (FIXED)\n";
			Datafile << "channels = " << NSB << "     # total number of channels -> following our search bin definition (FIXED)\n";
			Datafile << "sample = data     # name of the background: hadtau, lostle, zinv, qcd, ttz\n";
			Datafile << "channel = "; for(int i=first_bin;i<first_bin + NSB;i++){ Datafile << "bin" << i+1 << " "; } Datafile << "\n";

			Datafile << "\n# Predicted central numbers (need from all backgrounds)\n";
			Datafile << "rate = ";
			if(round_data) {
				for(int i=0;i<NSB;i++){ Datafile << round( ((TH1F*)(hs -> GetStack() -> Last())) -> GetBinContent(i+1) ) << " "; } Datafile << "\n";
			}
			else {
				for(int i=0;i<NSB;i++){ Datafile << ((TH1F*)(hs -> GetStack() -> Last())) -> GetBinContent(i+1) << " "; } Datafile << "\n";
			}
			Datafile.close();
		}
		else std::cout << "Unable to open Datafile";
	}

	if (make_signal_datacard)
	{
		TString sp = signal_name;

		TFile *f1 = new TFile("results/SMS_" + sp + full_or_fast + year + ".root");
		TH1F *h1 = (TH1F*)f1->Get(folder + SR);
		//TH1F *h2 = (TH1F*)f1->Get(folder + "/eff_h");
		TH1F *h2 = (TH1F*)f1->Get("Baseline_Only/eff_h");

		double all_events = h2->GetBinContent(1);

		h1->Sumw2();
		h1->SetBinErrorOption(TH1::kPoisson);

		std::cout << "\n" << sp << std::endl;
		std::cout << "unscale bin 1 content " << h1->GetBinContent(1) << std::endl;
		std::cout << "unscale bin 2 content " << h1->GetBinContent(2) << std::endl;
		std::cout << "unscale bin 1 error " << h1->GetBinError(1) << std::endl;
		std::cout << "unscale bin 2 error " << h1->GetBinError(2) << std::endl;

		TH1F * raw_sig = (TH1F*)h1->Clone("raw_sig");

		h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
		//h1->Scale(lumi * 1000);

		TH1F * TF_sig = (TH1F*)h1->Clone("TF_sig");
		TF_sig->Divide(raw_sig);

		std::cout << "scaled bin 1 content " << h1->GetBinContent(1) << std::endl;
		std::cout << "scaled bin 2 content " << h1->GetBinContent(2) << std::endl;
		std::cout << "scaled bin 1 error " << h1->GetBinError(1) << std::endl;
		std::cout << "scaled bin 2 error " << h1->GetBinError(2) << std::endl;

		std::ofstream signalfile (("datacards/temp/signal.txt"));
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

			signalfile << "stat_unc_up = ";
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
			} signalfile << "\n";

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
				if(i < mid_bin) signalfile << 0.0 << " ";
				else signalfile << 0.20 << " ";
			}
			signalfile << "\n";
			signalfile << "syst_isrUnc_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) signalfile << 0.0 << " ";
				else signalfile << 0.20 << " ";
			}
			signalfile << "\n";

			if(do_photon_unc)
			{
			signalfile << "syst_photon_up = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) signalfile << 0.15 << " ";
				else signalfile << 0.10 << " ";
			}
			signalfile << "\n";
			signalfile << "syst_photon_dn = ";
			for(int i=0;i<NSB;i++)
			{
				if(i < mid_bin) signalfile << 0.15 << " ";
				else signalfile << 0.10 << " ";
			}
			signalfile << "\n";
			}

			signalfile << "syst_jetJEC_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.15 << " "; } signalfile << "\n";
			signalfile << "syst_jetJEC_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.15 << " "; } signalfile << "\n";
			signalfile << "syst_TopReco_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.05 << " "; } signalfile << "\n";
			signalfile << "syst_TopReco_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.05 << " "; } signalfile << "\n";
			signalfile << "syst_TopSF_up = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
			signalfile << "syst_TopSF_dn = "         ; for(int i=0;i<NSB;i++){ signalfile << 0.10 << " "; } signalfile << "\n";
			signalfile.close();
		}
		else std::cout << "Unable to open signalfile";
	}

	if (do_validation)
	{
		TFile *f1 = new TFile("results/Data_MET" + year + ".root");
		TH1F *h1 = (TH1F*)f1->Get(folder + SR);
		TFile out_file("validation/Data_validation" + low_or_high + year + ".root","RECREATE");
		h1->SetName("data");
		h1->Write();
		out_file.Close();
	}


	return 0;
}
