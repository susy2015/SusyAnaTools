int print_datacard()
{
    TString SampleGroup = "TTZ";
    //TString SampleGroup = "Rare";

    TString year = "2016";

    TFile *f1 = new TFile("TTZ_VB_2016.root");
    
    TString extensions[8] = {"_bsf_down","_bsf_up","_trig_eff_down","_trig_eff_up","_puWeight_down","_puWeight_up","_PFWeight_down","_PFWeight_up"};
    TString bintype[2] = {"_vb","_sb"};
    TString regions[2] = {"_low","_high"};

    int bi = 0;

    TH1D* h1 = NULL;
    TH1D* h2 = NULL;

    h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0]);
    h2 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1]);

    int num_bins1 = h1->GetNbinsX();
    int num_bins2 = h2->GetNbinsX();

    double lumi = 36; //2016

    int channels = num_bins1 + num_bins2; //VB

    std::cout << "Outputting to datacard_" << SampleGroup << bintype[bi] << "_" << year << ".txt" << std::endl;
    ofstream outputfile;
    outputfile.open("datacard_" + SampleGroup + bintype[bi] + "_" + year + ".txt");

    outputfile << "luminosity = " << lumi << "\t# in pb-1 (FIXED)\n";
    outputfile << "channels = " << channels << "\t# total number of channels -> following our search bin definition (FIXED)\n";
    outputfile << "sample = " << SampleGroup << "\t# name of the background: hadtau, lostle, zinv, qcd, ttz\n";
    outputfile << "channel =";

    for(int i = 0; i < channels; i++)
    {
        outputfile << " bin" << i+1;
    }
    outputfile << "\n\n# Predicted central numbers (need from all backgrounds)\n";

    outputfile << "rate =";
    for(int i = 0; i < num_bins1; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0]); 
        outputfile << " " << h1->GetBinContent(i+1);
    }
    for(int i = 0; i < num_bins2; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1]);
        outputfile << " " << h1->GetBinContent(i+1);
    }

    outputfile << "\ncs_event =";
    std::vector<int> cs_event(channels,0);
    for(int i = 0; i < num_bins1; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0]);
        h2 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0] + "_avg_weight_sq");
        if(h2->GetBinContent(i+1) > 0) cs_event[i] = round(h1->GetBinContent(i+1) / h2->GetBinContent(i+1));
        outputfile << " " << cs_event[i];
    }
    for(int i = 0; i < num_bins2; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1]);
        h2 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1] + "_avg_weight_sq");
        if(h2->GetBinContent(i+1) > 0) cs_event[i] = round(h1->GetBinContent(i+1) / h2->GetBinContent(i+1));
        outputfile << " " << cs_event[i];
    }

    outputfile << "\navg_weight =";
    for(int i = 0; i < num_bins1; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0]);
        if(cs_event[i] > 0) outputfile << " " << h1->GetBinContent(i+1) / cs_event[i];
        else outputfile << " 0.0000";
    }
    for(int i = 0; i < num_bins2; i++)
    {
        h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1]);
        if(cs_event[i] > 0) outputfile << " " << h1->GetBinContent(i+1) / cs_event[i];
        else outputfile << " 0.0000";
    }

    for(int j = 0; j < 8; j++)
    {
        outputfile << "\nsyst_unc" << extensions[j] << " =";
        for(int i = 0; i < num_bins1; i++)
        {
            h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[0] + extensions[j]);
            outputfile << " " << h1->GetBinContent(i+1);
        }
        for(int i = 0; i < num_bins2; i++)
        {
            h1 = (TH1D*)f1->Get(SampleGroup + bintype[bi] + regions[1] + extensions[j]);
            outputfile << " " << h1->GetBinContent(i+1);
        }
    }

    outputfile << std::endl;
    outputfile.close();

    return 0;
}
