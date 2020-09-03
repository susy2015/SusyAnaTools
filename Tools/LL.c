#include "CrossSection.C"
TH1D* AddToHist(TH1D* hist, TString year, TString dir, TString suffix, TString name, TString var, double lumi, TString type)
{
    //std::cout << name << " " << var << " " << type << std::endl;

    //TFile *f1 = new TFile("results/TTZ_VBSB/" + dir + "/" + name + year + suffix + ".root");
    TFile *f1 = new TFile("results/LL_crosscheck/" + dir + "/" + name + year + suffix + ".root");
    TH1D *h1 = (TH1D*)f1->Get(var);
    TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

    double all_events = h2->GetBinContent(1);
    //h1->Sumw2();

    int num_bins = h1->GetNbinsX();
    //double scale = lumi * CrossSection.at(name) * 1000 / all_events;
    double scale = 1.0; //already taken care of

    for(int i = 0; i < num_bins; i++)
    {
        if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);
        //if(h1->GetBinContent(i+1) > 10000) std::cout << name << " " << var << " Bin number " << i << " is large: " << h1->GetBinContent(i+1) << std::endl;
        if(h1->GetBinContent(i+1) != h1->GetBinContent(i+1)) std::cout << name << " " << var << " Bin number " << i << " is NaN: " << h1->GetBinContent(i+1) << std::endl;
    }

    h1->Scale(scale);
    for(int i = 0; i < num_bins; i++)
    {
        //if(h1->GetBinContent(i+1) != h1->GetBinContent(i+1)) std::cout << name << " " << var << " Bin number " << i << " is NaN: " << h1->GetBinContent(i+1) << std::endl;
    }

    if(hist == NULL)
    {
        //std::cout << "hist is NULL, cloning new" << std::endl;
        hist = (TH1D*)h1->Clone(type);
        hist->SetDirectory(0);
    }
    else 
    {
        hist->Add(h1);
        //std::cout << "hist is not null, adding" << std::endl;
    }
    f1->Close();
    return hist;
}
int LL()
{
    //TString year = "_2016";
    //TString year = "_2017";
    TString year = "_2018";
    TString outputfile = "results/LL_crosscheck/LL" + year + ".root";
    TString suffix = "";
    bool createoutput = true;

    TString dir = "";
    if(year == "_2016") dir = "2016";
    if(year == "_2017") dir = "2017";
    if(year == "_2018") dir = "2018";

    double lumi = 1.0;

    TString LL_names[27] = {"TTbar_HT_600to800","TTbar_HT_800to1200","TTbar_HT_1200to2500","TTbar_HT_2500toInf","TTbarDiLep","TTbarSingleLepT","TTbarSingleLepTbar","WJetsToLNu_HT_70to100","WJetsToLNu_HT_100to200","WJetsToLNu_HT_200to400","WJetsToLNu_HT_400to600","WJetsToLNu_HT_600to800","WJetsToLNu_HT_800to1200","WJetsToLNu_HT_1200to2500","WJetsToLNu_HT_2500toInf","ST_tW_top_NoHad","ST_tW_antitop_NoHad","ST_s_lep","ST_t_top","ST_t_antitop","ST_tWll","ST_tWnunu","TTZToLLNuNu","TTZToQQ","TTWJetsToLNu","TTWJetsToQQ","tZq_ll"};
    TString ttbar_names[7] = {"TTbar_HT_600to800","TTbar_HT_800to1200","TTbar_HT_1200to2500","TTbar_HT_2500toInf","TTbarDiLep","TTbarSingleLepT","TTbarSingleLepTbar"};
    TString wjets_names[8] = {"WJetsToLNu_HT_70to100","WJetsToLNu_HT_100to200","WJetsToLNu_HT_200to400","WJetsToLNu_HT_400to600","WJetsToLNu_HT_600to800","WJetsToLNu_HT_800to1200","WJetsToLNu_HT_1200to2500","WJetsToLNu_HT_2500toInf"};
    TString tW_names[8] = {"ST_tW_top_NoHad","ST_tW_antitop_NoHad","ST_s_lep","ST_t_top","ST_t_antitop","ST_tWll","ST_tWnunu","tZq_ll"};
    TString ttW_names[2] = {"TTWJetsToLNu","TTWJetsToQQ"};
    TString vars[1] = {"h_ub"};
    TString extensions[33] = {"_raw","_bsf_down","_bsf_up","_trig_eff_down","_trig_eff_up","_puWeight_down","_puWeight_up","_PFWeight_down","_PFWeight_up","_pdfWeight_down","_pdfWeight_up","_JES_down","_JES_up","_METUnClust_down","_METUnClust_up","_ivfunc_down","_ivfunc_up","_eff_e_down","_eff_e_up","_err_mu_down","_err_mu_up","_eff_tau_down","_eff_tau_up","_eff_restoptag_down","_eff_restoptag_up","_eff_toptag_down","_eff_toptag_up","_eff_wtag_down","_eff_wtag_up","_ak8veto_down","_ak8veto_up","_ISRWeight_down","_ISRWeight_up"};
    int exts = 33;

    //low dm VB: [0,19)
    //high dm VB: [19,43)
    //low dm SB: [0,53)
    //high dm SB: [53,183)

    TH1D *LL_ub = NULL;
    TH1D *LL_ub_updown[exts];
    TH1D *ttbar_ub = NULL;
    TH1D *ttbar_ub_updown[exts];
    TH1D *wjets_ub = NULL;
    TH1D *wjets_ub_updown[exts];
    TH1D *tW_ub = NULL;
    TH1D *tW_ub_updown[exts];
    TH1D *ttW_ub = NULL;
    TH1D *ttW_ub_updown[exts];

    for(int j = 0; j < exts; j++)
    {
        LL_ub_updown[j] = NULL;
        ttbar_ub_updown[j] = NULL;
        wjets_ub_updown[j] = NULL;
        tW_ub_updown[j] = NULL;
        ttW_ub_updown[j] = NULL;
    }

    std::cout << "Filling LL hists" << std::endl;
    for(int i = 0; i < 27; i++)
    {
        LL_ub = AddToHist(LL_ub, year, dir, suffix, LL_names[i], vars[0], lumi, "LL_ub");
        for(int j = 0; j < exts; j++)
        {
            LL_ub_updown[j] = AddToHist(LL_ub_updown[j], year, dir, suffix, LL_names[i], vars[0]+extensions[j], lumi, "LL_ub" + extensions[j]);
        }
    }
    std::cout << "Filling ttbar hists" << std::endl;
    for(int i = 0; i < 7; i++)
    {
        ttbar_ub = AddToHist(ttbar_ub, year, dir, suffix, ttbar_names[i], vars[0], lumi, "ttbar_ub");
        for(int j = 0; j < exts; j++)
        {
            ttbar_ub_updown[j] = AddToHist(ttbar_ub_updown[j], year, dir, suffix, ttbar_names[i], vars[0]+extensions[j], lumi, "ttbar_ub" + extensions[j]);
        }
    }
    std::cout << "Filling wjets hists" << std::endl;
    for(int i = 0; i < 8; i++)
    {
        wjets_ub = AddToHist(wjets_ub, year, dir, suffix, wjets_names[i], vars[0], lumi, "wjets_ub");
        for(int j = 0; j < exts; j++)
        {
            wjets_ub_updown[j] = AddToHist(wjets_ub_updown[j], year, dir, suffix, wjets_names[i], vars[0]+extensions[j], lumi, "wjets_ub" + extensions[j]);
        }
    }
    std::cout << "Filling tW hists" << std::endl;
    for(int i = 0; i < 8; i++)
    {
        tW_ub = AddToHist(tW_ub, year, dir, suffix, tW_names[i], vars[0], lumi, "tW_ub");
        for(int j = 0; j < exts; j++)
        {
            tW_ub_updown[j] = AddToHist(tW_ub_updown[j], year, dir, suffix, tW_names[i], vars[0]+extensions[j], lumi, "tW_ub" + extensions[j]);
        }
    }
    std::cout << "Filling ttW hists" << std::endl;
    for(int i = 0; i < 2; i++)
    {
        ttW_ub = AddToHist(ttW_ub, year, dir, suffix, ttW_names[i], vars[0], lumi, "ttW_ub");
        for(int j = 0; j < exts; j++)
        {
            ttW_ub_updown[j] = AddToHist(ttW_ub_updown[j], year, dir, suffix, ttW_names[i], vars[0]+extensions[j], lumi, "ttW_ub" + extensions[j]);
        }
    }

    std::cout << "Checking for nonpositive values" << std::endl;

    //Set nonpositive values to 0:
    for(int i = 0; i < 112; i++)
    {
        if(LL_ub->GetBinContent(i+1) < 0) LL_ub->SetBinContent(i+1,0);
        if(ttbar_ub->GetBinContent(i+1) < 0) ttbar_ub->SetBinContent(i+1,0);
        if(wjets_ub->GetBinContent(i+1) < 0) wjets_ub->SetBinContent(i+1,0);
        if(tW_ub->GetBinContent(i+1) < 0) tW_ub->SetBinContent(i+1,0);
        if(ttW_ub->GetBinContent(i+1) < 0) ttW_ub->SetBinContent(i+1,0);
    }

    if(createoutput)
    {
        std::cout << "Outputting to file " + outputfile << std::endl;
        TFile out_file(outputfile,"RECREATE");
        LL_ub->Write();
        ttbar_ub->Write();
        wjets_ub->Write();
        tW_ub->Write();
        ttW_ub->Write();
        for(int j = 0; j < exts; j++)
        {
            LL_ub_updown[j]->Write();
            ttbar_ub_updown[j]->Write();
            wjets_ub_updown[j]->Write();
            tW_ub_updown[j]->Write();
            ttW_ub_updown[j]->Write();
        }
        out_file.Close();
    }

    delete LL_ub;
    delete ttbar_ub;
    delete wjets_ub;
    delete tW_ub;
    delete ttW_ub;
    for(int j = 0; j < exts; j++)
    {
        delete LL_ub_updown[j];
        delete ttbar_ub_updown[j];
        delete wjets_ub_updown[j];
        delete tW_ub_updown[j];
        delete ttW_ub_updown[j];
    }

    return 0;
}
