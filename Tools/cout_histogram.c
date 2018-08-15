int cout_histogram()
{
TString file_name;
//file_name = "/uscms_data/d3/huiwang/CMSSW_8_0_26/src/HadronicTau/Tool/workDir/Mix_HadTauLL.root";
file_name = "Signal_fastsim_T1tttt_mGluino2000_mLSP100.root";

TString histo_name;
//histo_name = "hYields_LL";
//histo_name = "hYields_Pass_LL";
//histo_name = "hYields_tau";
//histo_name = "hYields_Pass_tau";
histo_name = "search_bin_highdm_h";

TFile *f1 = new TFile(file_name);
TH1D *h1 = (TH1D*)f1->Get(histo_name);

std::cout << "file name " << file_name << ", histo name " << histo_name << std::endl;
for (int i = 1; i <= 124; i++)
{
//std::cout << h1->GetBinContent(i) << " ";
std::cout << "bin " << i+52 << ": " << h1->GetBinContent(i) << std::endl;
} 
std::cout << "\n" << std::endl;

return 0;
}

