int save_NNLO_xSec()
{
	//TString csv_file = "glu_xSec.csv", hist_name = "gluino_xsection";
	//NNLO gluino xSec twiki: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu 
	TString csv_file = "stop_xSec.csv", hist_name = "stop_xsection";
	//NNLO stop xSec twiki: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVstopsbottom

	std::ifstream temp_file (csv_file);
	std::string value;
	std::vector <double> mass_vec;
	std::vector <double> xSec_vec;
	std::vector <double> xSec_err_vec;
	int cell = 0;

	while ( temp_file.good() )
	{
		getline ( temp_file, value, ',' );
		if(value != "\n")
		{	
			if(cell % 3 == 0) mass_vec.push_back(std::stod(value));
			if(cell % 3 == 1) xSec_vec.push_back(std::stod(value));
			if(cell % 3 == 2) xSec_err_vec.push_back(std::stod(value));
			cell++;
			//std::cout << std::stod(value) << std::endl;
			//std::cout << value << std::endl;
		}
	}
	std::cout << mass_vec.at(9) << ": " << xSec_vec.at(9) << " +- " << xSec_err_vec.at(9) << std::endl;

	double mass_min = mass_vec.front();
	double mass_max = mass_vec.back() + 5;
	int mass_bins = mass_vec.size();
	auto xSec_h=new TH1D(hist_name,hist_name,mass_bins,mass_min,mass_max);
	for(int i = 0; i < mass_bins; i++)
	{
		xSec_h->SetBinContent(i+1, xSec_vec.at(i));
		xSec_h->SetBinError(i+1, xSec_vec.at(i) * xSec_err_vec.at(i) / 100);
	}

	std::cout << mass_vec.at(9) << ": " << xSec_h->GetBinContent(xSec_h->FindBin(mass_vec.at(9))) << " +- " << xSec_h->GetBinError(xSec_h->FindBin(mass_vec.at(9))) << std::endl;

	TFile out_file("xSec.root","UPDATE");
	xSec_h->Write();
	out_file.Close();

	return 0;
}
