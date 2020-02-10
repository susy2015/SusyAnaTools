#include <iostream>
#include <iomanip>

int convert_validation()
{
	TString year = "";
	//year = "_2016";
	//year = "_2017";
	//year = "_2018";

	int bin_test = 5;
	bin_test = 9;
	bin_test = 10;
	//bin_test = 24;
	//bin_test = 1;

	TString my_file = "QCD_validation_lowdm";
	TString my_hist = "QCD"; 
	TString people_file_stat = "std_pred_trad_QCD"; 
	TString people_hist_stat = "qcd_pred";
	TString people_file_sys = "qcd"; 
	TString people_hist_sys_up = "Up_total";
	TString people_hist_sys_down = "Down_total";
	int first_bin = 0;
	int first_bin_sys = 0;

	//std::vector<TString> hist_list = {"zinv_highdm", "QCD_highdm", "LL_highdm", "TTZ_highdm", "rare_highdm", "zinv_lowdm", "QCD_lowdm", "LL_lowdm", "TTZ_lowdm", "rare_lowdm"};
	//std::vector<TString> hist_list = {"data_lowdm", "data_highdm", "QCD_lowdm", "QCD_highdm", "LL_lowdm", "LL_highdm", "zinv_lowdm", "zinv_highdm", "TTZ_lowdm", "TTZ_highdm", "rare_lowdm", "rare_highdm"};
	//std::vector<TString> hist_list = {"zinv_lowdm", "QCD_lowdm", "LL_lowdm", "TTZ_lowdm", "rare_lowdm"};
	std::vector<TString> hist_list = {"zinv_highdm", "QCD_highdm", "LL_highdm", "TTZ_highdm", "rare_highdm"};
	for(int i=0; i<hist_list.size(); i++ )
	{
		if(hist_list.at(i) == "data_lowdm")
		{
			my_file = "Data_validation_lowdm";
			my_hist = "data"; 
			people_file_stat = "std_pred_trad_LM"; 
			people_hist_stat = "data";
		}

		if(hist_list.at(i) == "data_highdm")
		{
			my_file = "Data_validation_highdm";
			my_hist = "data"; 
			people_file_stat = "std_pred_trad_HM"; 
			people_hist_stat = "data";
		}

		if(hist_list.at(i) == "QCD_lowdm")
		{
			my_file = "QCD_validation_lowdm";
			my_hist = "QCD"; 
			people_file_stat = "Validation_QCD"; 
			people_hist_stat = "QCD";
			people_file_sys = "Validation_QCD"; 
			people_hist_sys_up = "Tot_Up";
			people_hist_sys_down = "Tot_Down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "QCD_highdm")
		{
			my_file = "QCD_validation_highdm";
			my_hist = "QCD"; 
			people_file_stat = "Validation_QCD"; 
			people_hist_stat = "QCD";
			people_file_sys = "Validation_QCD"; 
			people_hist_sys_up = "Tot_Up";
			people_hist_sys_down = "Tot_Down";
			first_bin = 19;
			first_bin_sys = 19;
		}

		if(hist_list.at(i) == "LL_lowdm")
		{
			my_file = "LL_validation_lowdm";
			my_hist = "LL_SR"; 
			people_file_stat = "std_pred_trad_LM"; 
			people_hist_stat = "ttbarplusw_pred";
			people_file_sys = "ll"; 
			people_hist_sys_up = "Up_total";
			people_hist_sys_up = "Up";
			people_hist_sys_down = "Down_total";
			people_hist_sys_down = "Down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "LL_highdm")
		{
			my_file = "LL_validation_highdm";
			my_hist = "LL_SR"; 
			people_file_stat = "std_pred_trad_HM"; 
			people_hist_stat = "ttbarplusw_pred";
			people_file_sys = "ll"; 
			people_hist_sys_up = "Up_total";
			people_hist_sys_up = "Up";
			people_hist_sys_down = "Down_total";
			people_hist_sys_down = "Down";
			first_bin = 0;
			first_bin_sys = 19;
		}

		if(hist_list.at(i) == "zinv_lowdm")
		{
			my_file = "Zinv_validation_lowdm";
			my_hist = "zjets"; 
			people_file_stat = "validationBinsZinv"; 
			people_hist_stat = "pred_lowdm";
			people_file_sys = "validationBinsZinv_syst"; 
			people_hist_sys_up = "syst_up_lowdm";
			people_hist_sys_down = "syst_down_lowdm";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "zinv_highdm")
		{
			my_file = "Zinv_validation_highdm";
			my_hist = "zjets"; 
			people_file_stat = "validationBinsZinv"; 
			people_hist_stat = "pred_highdm";
			people_file_sys = "validationBinsZinv_syst"; 
			people_hist_sys_up = "syst_up_highdm";
			people_hist_sys_down = "syst_down_highdm";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "TTZ_lowdm")
		{
			my_file = "TTZ_validation_lowdm";
			my_hist = "TTZ"; 
			people_file_stat = "TTZ_final_vb"; 
			people_hist_stat = "TTZ_vb_low";
			people_file_sys = "TTZ_final_vb"; 
			people_hist_sys_up = "TTZ_vb_low_up";
			people_hist_sys_down = "TTZ_vb_low_down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "TTZ_highdm")
		{
			my_file = "TTZ_validation_highdm";
			my_hist = "TTZ"; 
			people_file_stat = "TTZ_final_vb"; 
			people_hist_stat = "TTZ_vb_high";
			people_file_sys = "TTZ_final_vb"; 
			people_hist_sys_up = "TTZ_vb_high_up";
			people_hist_sys_down = "TTZ_vb_high_down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "rare_lowdm")
		{
			my_file = "Rare_validation_lowdm";
			my_hist = "rare"; 
			people_file_stat = "Rare_final_vb"; 
			people_hist_stat = "Rare_vb_low";
			people_file_sys = "Rare_final_vb"; 
			people_hist_sys_up = "Rare_vb_low_up";
			people_hist_sys_down = "Rare_vb_low_down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		if(hist_list.at(i) == "rare_highdm")
		{
			my_file = "Rare_validation_highdm";
			my_hist = "rare"; 
			people_file_stat = "Rare_final_vb"; 
			people_hist_stat = "Rare_vb_high";
			people_file_sys = "Rare_final_vb"; 
			people_hist_sys_up = "Rare_vb_high_up";
			people_hist_sys_down = "Rare_vb_high_down";
			first_bin = 0;
			first_bin_sys = 0;
		}

		TFile *f1 = new TFile(my_file + year + ".root");
		TFile *f2 = new TFile(people_file_stat + year + ".root");
		TFile *f2_sys = new TFile(people_file_sys + year + ".root");

		std::cout << "\n converting " << hist_list.at(i) << std::endl;
		TH1F *h1 = (TH1F*)f1->Get(my_hist);
		std::cout << "hist 1 total bins " << h1->GetSize() - 2 << " (" << h1->GetXaxis()->GetXmin() << "-" << h1->GetXaxis()->GetXmax() << ")" << std::endl;

		TH1F *h2 = (TH1F*)f2->Get(people_hist_stat);
		TH1F *h2_up = (TH1F*)f2_sys->Get(people_hist_sys_up);
		TH1F *h2_down = (TH1F*)f2_sys->Get(people_hist_sys_down);
		std::cout << "hist 2 total bins " << h2->GetSize() - 2 << " (" << h2->GetXaxis()->GetXmin() << "-" << h2->GetXaxis()->GetXmax() << ")" << std::endl;
		TH1F *h2_clone = (TH1F*)h1->Clone();
		for(int i = 1; i <= h1->GetSize() - 2; i++)
		{
			h2_clone->SetBinContent(i, h2->GetBinContent(i + first_bin));
			h2_clone->SetBinError(i, h2->GetBinError(i + first_bin));
		}
		TH1F *h2_clone_sys = (TH1F*)h2_clone->Clone();
		for(int i = 1; i <= h1->GetSize() - 2; i++)
		{
			float mean_sys = (fabs(h2_up->GetBinContent(i + first_bin_sys)-1) + fabs(1-h2_down->GetBinContent(i + first_bin_sys)))/2 * h2_clone->GetBinContent(i);
			//if(i==1)std::cout << "bin 1 central = " << h2_clone->GetBinContent(i) << std::endl;
			//if(i==1)std::cout << "bin 1 sys unc = " << mean_sys << std::endl;
			float stat = h2_clone->GetBinError(i);
			//if(i==1)std::cout << "bin 1 stat unc = " << stat << std::endl;
			float total_unc = sqrt(mean_sys* mean_sys + stat*stat);
			//if(i==1)std::cout << "bin 1 total unc = " << total_unc << std::endl;
			h2_clone_sys->SetBinError(i, total_unc);
			//if(i==1)std::cout << "bin test: simulation, prediction, stat unc, sys down/up, sys unc, tot unc" << std::endl;
			if(i==1)std::cout << "bin test: central, stat unc, sys down/up, sys unc, tot unc" << std::endl;
			std::cout << std::fixed;
			std::cout << std::setprecision(2);
			//if(i==bin_test)std::cout << h1->GetBinContent(i) << " " << h2_clone->GetBinContent(i) << " " << stat << " " << h2_down->GetBinContent(i + first_bin_sys) << "/" << h2_up->GetBinContent(i + first_bin_sys) << " " << mean_sys << " " << total_unc << std::endl;
			if(i==bin_test)std::cout << h2_clone->GetBinContent(i) << " " << stat << " " << h2_down->GetBinContent(i + first_bin_sys) << "/" << h2_up->GetBinContent(i + first_bin_sys) << " " << mean_sys << " " << total_unc << std::endl;
		}
		TString stat_unc_name = h2_clone->GetName();
		stat_unc_name = stat_unc_name + "_stat";
		h2_clone->SetName(stat_unc_name);
		
		TFile out_file(my_file + year + "_people.root","RECREATE");
		h2_clone->Write();
		std::cout << h2_clone->GetName() << " bin 1 stat unc = " << h2_clone->GetBinError(1) << std::endl;
		h2_clone_sys->Write();
		std::cout << h2_clone_sys->GetName() << " bin 1 total unc = " << h2_clone_sys->GetBinError(1) << std::endl;
		out_file.Close();
	}
	return 0;
}
