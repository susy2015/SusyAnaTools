int cout_histogram()
{
TString file_name = "/uscms_data/d3/huiwang/CMSSW_8_0_26/src/HadronicTau/Tool/workDir/Mix_HadTauLL.root";
//TString histo_name = "hYields_LL";
//TString histo_name = "hYields_Pass_LL";
//TString histo_name = "hYields_tau";
TString histo_name = "hYields_Pass_tau";
TFile *f1 = new TFile(file_name);
TH1D *h1 = (TH1D*)f1->Get(histo_name);

std::cout << "file name " << file_name << ", histo name " << histo_name << std::endl;
for (int i = 1; i <= 84; i++)
{
std::cout << h1->GetBinContent(i) << " ";
} 
std::cout << "\n" << std::endl;

return 0;
}

