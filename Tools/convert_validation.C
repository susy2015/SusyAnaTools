int convert_validation()
{
	bool QCD_highdm = false;
	bool LL_highdm = false;
	bool LL_lowdm = false;
	bool zinv_lowdm = false;
	bool zinv_highdm = true;

	TString f1_name = "QCD_validation_lowdm_2016";
	TString hist1 = "QCD"; 
	TString f2_name = "std_pred_trad_QCD_2016"; 
	TString hist2 = "qcd_pred";
	int first_bin = 0;

	if(QCD_highdm)
	{
		f1_name = "QCD_validation_highdm_2016";
		hist1 = "QCD"; 
		f2_name = "std_pred_trad_QCD_2016"; 
		hist2 = "qcd_pred";
		first_bin = 19;
	}

	if(LL_highdm)
	{
		f1_name = "LL_validation_highdm_2016";
		hist1 = "LL_SR"; 
		f2_name = "std_pred_trad_HM_2016"; 
		hist2 = "ttbarplusw_pred";
	}

	if(LL_lowdm)
	{
		f1_name = "LL_validation_lowdm_2016";
		hist1 = "LL_SR"; 
		f2_name = "std_pred_trad_LM_2016"; 
		hist2 = "ttbarplusw_pred";
	}

	if(zinv_lowdm)
	{
		f1_name = "Zinv_validation_lowdm_2016";
		hist1 = "zjets"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "validation_pred_lowdm";
	}

	if(zinv_highdm)
	{
		f1_name = "Zinv_validation_highdm_2016";
		hist1 = "zjets"; 
		f2_name = "validationBinsZinv_2016"; 
		hist2 = "validation_pred_highdm";
	}

	TFile *f1 = new TFile(f1_name + ".root");
	TFile *f2 = new TFile(f2_name + ".root");

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

	TFile out_file(f1_name + "_people.root","RECREATE");
	h2_clone->Write();
	out_file.Close();

	return 0;
}
