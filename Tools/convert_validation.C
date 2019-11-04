int convert_validation()
{
	TString year = "";
	year = "_2016";
	year = "_2017";
	year = "_2018";

	TString f1_name = "QCD_validation_lowdm";
	TString hist1 = "QCD"; 
	TString f2_name = "std_pred_trad_QCD"; 
	TString hist2 = "qcd_pred";
	int first_bin = 0;

	std::vector<TString> hist_list = {"data_lowdm", "data_highdm", "QCD_lowdm", "QCD_highdm", "LL_lowdm", "LL_highdm", "zinv_lowdm", "zinv_highdm", "TTZ_lowdm", "TTZ_highdm", "rare_lowdm", "rare_highdm"};
	//std::vector<TString> hist_list = {"data_lowdm", "QCD_lowdm", "LL_lowdm", "zinv_lowdm", "TTZ_lowdm", "rare_lowdm"};
	//std::vector<TString> hist_list = {"data_highdm", "QCD_highdm", "LL_highdm", "zinv_highdm", "TTZ_highdm", "rare_highdm"};
	for(int i=0; i<hist_list.size(); i++ )
	{
		if(hist_list.at(i) == "QCD_lowdm")
		{
			f1_name = "QCD_validation_lowdm";
			hist1 = "QCD"; 
			f2_name = "std_pred_trad_LM_QCD"; 
			hist2 = "qcd_pred";
			//first_bin = 19;
		}

		if(hist_list.at(i) == "QCD_highdm")
		{
			f1_name = "QCD_validation_highdm";
			hist1 = "QCD"; 
			f2_name = "std_pred_trad_HM_QCD"; 
			hist2 = "qcd_pred";
			//first_bin = 19;
		}

		if(hist_list.at(i) == "data_lowdm")
		{
			f1_name = "Data_validation_lowdm";
			hist1 = "data"; 
			f2_name = "std_pred_trad_LM"; 
			hist2 = "data";
		}

		if(hist_list.at(i) == "data_highdm")
		{
			f1_name = "Data_validation_highdm";
			hist1 = "data"; 
			f2_name = "std_pred_trad_HM"; 
			hist2 = "data";
		}

		if(hist_list.at(i) == "LL_lowdm")
		{
			f1_name = "LL_validation_lowdm";
			hist1 = "LL_SR"; 
			f2_name = "std_pred_trad_LM"; 
			hist2 = "ttbarplusw_pred";
		}

		if(hist_list.at(i) == "LL_highdm")
		{
			f1_name = "LL_validation_highdm";
			hist1 = "LL_SR"; 
			f2_name = "std_pred_trad_HM"; 
			hist2 = "ttbarplusw_pred";
		}

		if(hist_list.at(i) == "zinv_lowdm")
		{
			f1_name = "Zinv_validation_lowdm";
			hist1 = "zjets"; 
			f2_name = "validationBinsZinv"; 
			hist2 = "pred_lowdm";
		}

		if(hist_list.at(i) == "zinv_highdm")
		{
			f1_name = "Zinv_validation_highdm";
			hist1 = "zjets"; 
			f2_name = "validationBinsZinv"; 
			hist2 = "pred_highdm";
		}

		if(hist_list.at(i) == "TTZ_lowdm")
		{
			f1_name = "TTZ_validation_lowdm";
			hist1 = "TTZ"; 
			f2_name = "TTZ_final_vb"; 
			hist2 = "TTZ_vb_low";
		}

		if(hist_list.at(i) == "TTZ_highdm")
		{
			f1_name = "TTZ_validation_highdm";
			hist1 = "TTZ"; 
			f2_name = "TTZ_final_vb"; 
			hist2 = "TTZ_vb_high";
		}

		if(hist_list.at(i) == "rare_lowdm")
		{
			f1_name = "Rare_validation_lowdm";
			hist1 = "rare"; 
			f2_name = "Rare_final_vb"; 
			hist2 = "Rare_vb_low";
		}

		if(hist_list.at(i) == "rare_highdm")
		{
			f1_name = "Rare_validation_highdm";
			hist1 = "rare"; 
			f2_name = "Rare_final_vb"; 
			hist2 = "Rare_vb_high";
		}

		TFile *f1 = new TFile(f1_name + year + ".root");
		TFile *f2 = new TFile(f2_name + year + ".root");

		std::cout << "converting " << hist_list.at(i) << std::endl;
		TH1F *h1 = (TH1F*)f1->Get(hist1);
		std::cout << "hist 1 total bins " << h1->GetSize() - 2 << " (" << h1->GetXaxis()->GetXmin() << "-" << h1->GetXaxis()->GetXmax() << ")" << std::endl;

		TH1F *h2 = (TH1F*)f2->Get(hist2);
		std::cout << "hist 2 total bins " << h2->GetSize() - 2 << " (" << h2->GetXaxis()->GetXmin() << "-" << h2->GetXaxis()->GetXmax() << ")" << std::endl;
		TH1F *h2_clone = (TH1F*)h1->Clone();
		for(int i = 1; i <= h1->GetSize() - 2; i++)
		{
			h2_clone->SetBinContent(i, h2->GetBinContent(i + first_bin));
			h2_clone->SetBinError(i, h2->GetBinError(i + first_bin));
		}

		TFile out_file(f1_name + year + "_people.root","RECREATE");
		h2_clone->Write();
		out_file.Close();
	}
	return 0;
}
