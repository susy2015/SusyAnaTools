#include "CrossSection.C"
TH1D* AddToHist(TH1D* hist, TString year, TString dir, TString suffix, TString name, TString var, double lumi, TString type)
{
    std::cout << name << " " << var << " " << type << std::endl;

    //TFile *f1 = new TFile("results/TTZ_VBSB/" + dir + "/" + name + year + suffix + ".root");
    TFile *f1 = new TFile("results/TTZ_VBSB/" + dir + "/" + name + year + suffix + ".root");
    TH1D *h1 = (TH1D*)f1->Get(var);
    TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

    double all_events = h2->GetBinContent(1);
    //h1->Sumw2();

    int num_bins = h1->GetNbinsX();
    double scale = lumi * CrossSection.at(name) * 1000 / all_events;

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
        //std::cout << "bin 1 content " << hist->GetBinContent(1) << std::endl;
        //std::cout << "bin 2 content " << hist->GetBinContent(2) << std::endl;
        //std::cout << "bin 1 error " << hist->GetBinError(1) << std::endl;
        //std::cout << "bin 2 error " << hist->GetBinError(2) << std::endl;
    }
    else 
    {
        hist->Add(h1);
        //std::cout << "hist is not null, adding" << std::endl;
        //std::cout << "bin 1 content " << hist->GetBinContent(1) << std::endl;
        //std::cout << "bin 2 content " << hist->GetBinContent(2) << std::endl;
        //std::cout << "bin 1 error " << hist->GetBinError(1) << std::endl;
        //std::cout << "bin 2 error " << hist->GetBinError(2) << std::endl;
    }
    f1->Close();
    return hist;
}
TH1D* AddToHist(TH1D* hist, TString year, TString dir, TString suffix, TString name, TString var, double lumi, TString type, TH1D* avg_weight, TH1D* avg_weight_sq)
{
    //version used for nominal values
    std::cout << name << " " << var << " " << type << std::endl;

    //TFile *f1 = new TFile("results/TTZ_VBSB/" + dir + "/" + name + year + suffix + ".root");
    TFile *f1 = new TFile("results/TTZ_VBSB/" + dir + "/" + name + year + suffix + ".root");
    TH1D *h1 = (TH1D*)f1->Get(var);
    TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

    double all_events = h2->GetBinContent(1);
    //h1->Sumw2();

    int num_bins = h1->GetNbinsX();
    double scale = lumi * CrossSection.at(name) * 1000 / all_events;

    for(int i = 0; i < num_bins; i++)
    {
        if(h1->GetBinContent(i+1) < 0) h1->SetBinContent(i+1, 0);
        if(h1->GetBinContent(i+1) != h1->GetBinContent(i+1)) std::cout << name << " " << var << " Bin number " << i << " is NaN: " << h1->GetBinContent(i+1) << std::endl;

        avg_weight_sq->SetBinContent(i+1,avg_weight_sq->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale*scale);
        avg_weight->SetBinContent(i+1,avg_weight->GetBinContent(i+1)+h1->GetBinContent(i+1)*scale);
    }

    h1->Scale(scale);

    /*
    if(year == "_2017" || year == "_2018")
    {
        double statscale = 0;
        if(year == "_2017" && suffix == "")  statscale = 41.208034/lumi;
        if(year == "_2017" && suffix == "2") statscale = 41.208034/lumi;
        if(year == "_2018" && suffix == "")  statscale = 59.699489/lumi;
        if(year == "_2018" && suffix == "2") statscale = 59.699489/lumi;
        for(int i = 0; i < num_bins; i++)
        {
            h1->SetBinError(i+1,h1->GetBinError(i+1)*statscale);
        }
    }
    */
    /*
    for(int i = 0; i < num_bins; i++)
    {
        if(h1->GetBinContent(i+1) != h1->GetBinContent(i+1)) std::cout << name << " " << var << " Bin number " << i << " is NaN: " << h1->GetBinContent(i+1) << std::endl;
    }*/
    if(h1->GetBinContent(1) != h1->GetBinContent(1)) std::cout << "NaN in " << name << " " << var << ". Scale is: " << scale << ". all_events: " << all_events << std::endl;

    if(hist == NULL)
    {
        //std::cout << "hist is NULL, cloning new" << std::endl;
        hist = (TH1D*)h1->Clone(type);
        hist->SetDirectory(0);
        //std::cout << "bin 1 content " << hist->GetBinContent(1) << std::endl;
        //std::cout << "bin 2 content " << hist->GetBinContent(2) << std::endl;
        //std::cout << "bin 1 error " << hist->GetBinError(1) << std::endl;
        //std::cout << "bin 2 error " << hist->GetBinError(2) << std::endl;
    }
    else 
    {
        hist->Add(h1);
        //std::cout << "hist is not null, adding" << std::endl;
        //std::cout << "bin 1 content " << hist->GetBinContent(1) << std::endl;
        //std::cout << "bin 2 content " << hist->GetBinContent(2) << std::endl;
        //std::cout << "bin 1 error " << hist->GetBinError(1) << std::endl;
        //std::cout << "bin 2 error " << hist->GetBinError(2) << std::endl;
    }
    f1->Close();
    return hist;
}
int TTZ_VB()
{
    TString year = "_2016";
    //TString year = "_2017";
    bool PeriodF = false;
    //TString year = "_2018";
    bool PostHEM = false;
    //TString outputfile = "TTZ_VB" + year + ".root";
    TString outputfile = "results/TTZ_VBSB/TTZ_VB" + year + ".root";
    TString suffix = "";
    bool createoutput = true;
    /*
    if(year == "_2017")
    {
        if(PeriodF)
        {
            outputfile = "TTZ_VB" + year + "_PeriodF.root";
            suffix = "2";
        }
        else outputfile = "TTZ_VB" + year + "_BtoE.root";
    }
    if(year == "_2018")
    {
        if(PostHEM)
        {
            outputfile = "TTZ_VB" + year + "_PostHEM.root";
            suffix = "2";
        }
        else outputfile = "TTZ_VB" + year + "_PreHEM.root";
    }
    */
    bool forgraphs = false;

    TString dir = "";
    if(year == "_2016") dir = "2016";
    if(year == "_2017")
    {
        if(PeriodF) dir = "2017_PeriodF";
        else dir = "2017";
    }
    if(year == "_2018")
    {
        if(PostHEM) dir = "2018_PostHEM";
        else dir = "2018";
    }

    //double SF = 1.17;
    //double SF_up = 1.33;
    //double SF_down = 1.01;

    /*
    double lumi = 35.815165; //36; //2016
    //if(year == "_2017") lumi = 42; //2017
    if(year == "_2017")
    {
        if(PeriodF) lumi = 13.220313; //13.5;
        else lumi = 27.987721; //28.1;
    }
	//if(year == "_2018") lumi = 59; //2018
    if(year == "_2018")
    {
        if(PostHEM) lumi = 38.630913; //38.8; //38.8, 38.8, 38.2, 38.4
        else lumi = 21.068576; //21; //21, 20.8, 20.7, 21
    }
    */
    double lumi = 1.0;

    TString TTZ_name = "TTZToLLNuNu";
    TString Rare_names[18] = {"ttHToNonbb","ttHTobb","TTZToQQ","WWTo2L2Nu","WWTo4Q","WWToLNuQQ","VHToNonbb","WZ","ZZTo4L","ZZTo2L2Nu","ZZTo2L2Q","ZZTo2Q2Nu","WWZ","WZZ","ZZZ","WZG","WWG","WWW"};
    TString Comb_names[19] = {"ttHToNonbb","ttHTobb","TTZToQQ","WWTo2L2Nu","WWTo4Q","WWToLNuQQ","VHToNonbb","WZ","ZZTo4L","ZZTo2L2Nu","ZZTo2L2Q","ZZTo2Q2Nu","WWZ","WZZ","ZZZ","WZG","WWG","WWW","TTZToLLNuNu"};
    //TString Rare_names[17] = {"ttHToNonbb","ttHTobb","TTZToQQ","WWTo2L2Nu","WWTo4Q","WWToLNuQQ","VHToNonbb","ZZTo4L","ZZTo2L2Nu","ZZTo2L2Q","ZZTo2Q2Nu","WWZ","WZZ","ZZZ","WZG","WWG","WWW"};
    //TString Rare_names[21] = {"ttHToNonbb","ttHTobb","TTZToQQ","WWTo2L2Nu","WWTo4Q","WWToLNuQQ","VHToNonbb","WZTo1L1Nu2Q","WZTo1L3Nu","WZTo3LNu","WZTo2L2Q","ZZTo4L","ZZTo2L2Nu","ZZTo2L2Q","ZZTo2Q2Nu","WWZ","WZZ","ZZZ","WZG","WWG","WWW"};
    //TString Rare_names[19] = {"ttHToNonbb","ttHTobb","TTZToQQ","WWTo2L2Nu","WWTo4Q","WWToLNuQQ","VHToNonbb","WZTo1L3Nu","WZTo3LNu","WZTo2L2Q","ZZTo4L","ZZTo2L2Nu","ZZTo2L2Q","WWZ","WZZ","ZZZ","WZG","WWG","WWW"};
    int rare_num = 18;
    int comb_num = 19;
    TString vars[4] = {"h_vb_low","h_vb_high","h_sb_low","h_sb_high"};
    TString extensions[38] = {"_bsf_down","_bsf_up","_bsf_light_down","_bsf_light_up","_bsf_heavy_down","_bsf_heavy_up","_trig_eff_down","_trig_eff_up","_puWeight_down","_puWeight_up","_PFWeight_down","_PFWeight_up","_pdfWeight_down","_pdfWeight_up","_JES_down","_JES_up","_METUnClust_down","_METUnClust_up","_SF_down","_SF_up","_ivfunc_down","_ivfunc_up","_eff_e_down","_eff_e_up","_err_mu_down","_err_mu_up","_eff_tau_down","_eff_tau_up","_eff_restoptag_down","_eff_restoptag_up","_eff_toptag_down","_eff_toptag_up","_eff_wtag_down","_eff_wtag_up","_ak8veto_down","_ak8veto_up","_eff_densetoptag_down","_eff_densetoptag_up"};
    int exts = 38;

    //low dm VB: [0,19)
    //high dm VB: [19,43)
    //low dm SB: [0,53)
    //high dm SB: [53,183)

    TH1D *TTZ_vb_low = NULL;
    TH1D *TTZ_vb_low_updown[exts];
    TH1D *TTZ_vb_low_LHEScaleWeight[9];
    TH1D *TTZ_vb_low_avgwt = new TH1D("TTZ_vb_low_avg_weight","avg_weight",19,0,19);
    TH1D *TTZ_vb_low_avgwtsq = new TH1D("TTZ_vb_low_avg_weight_sq","avg_weight_sq",19,0,19);
    TH1D *TTZ_vb_high = NULL;
    TH1D *TTZ_vb_high_updown[exts];
    TH1D *TTZ_vb_high_LHEScaleWeight[9];
    TH1D *TTZ_vb_high_avgwt = new TH1D("TTZ_vb_high_avg_weight","avg_weight",24,19,43);
    TH1D *TTZ_vb_high_avgwtsq = new TH1D("TTZ_vb_high_avg_weight_sq","avg_weight_sq",24,19,43);
    TH1D *Rare_vb_low = NULL;
    TH1D *Rare_vb_low_updown[exts];
    TH1D *Rare_vb_low_LHEScaleWeight[9];
    TH1D *Rare_vb_low_avgwt = new TH1D("Rare_vb_low_avg_weight","avg_weight",19,0,19);
    TH1D *Rare_vb_low_avgwtsq = new TH1D("Rare_vb_low_avg_weight_sq","avg_weight_sq",19,0,19);
    TH1D *Rare_vb_high = NULL;
    TH1D *Rare_vb_high_updown[exts];
    TH1D *Rare_vb_high_LHEScaleWeight[9];
    TH1D *Rare_vb_high_avgwt = new TH1D("Rare_vb_high_avg_weight","avg_weight",24,19,43);
    TH1D *Rare_vb_high_avgwtsq = new TH1D("Rare_vb_high_avg_weight_sq","avg_weight_sq",24,19,43);
    TH1D *Comb_vb_low = NULL;
    TH1D *Comb_vb_low_updown[exts];
    TH1D *Comb_vb_low_LHEScaleWeight[9];
    TH1D *Comb_vb_low_avgwt = new TH1D("Comb_vb_low_avg_weight","avg_weight",19,0,19);
    TH1D *Comb_vb_low_avgwtsq = new TH1D("Comb_vb_low_avg_weight_sq","avg_weight_sq",19,0,19);
    TH1D *Comb_vb_high = NULL;
    TH1D *Comb_vb_high_updown[exts];
    TH1D *Comb_vb_high_LHEScaleWeight[9];
    TH1D *Comb_vb_high_avgwt = new TH1D("Comb_vb_high_avg_weight","avg_weight",24,19,43);
    TH1D *Comb_vb_high_avgwtsq = new TH1D("Comb_vb_high_avg_weight_sq","avg_weight_sq",24,19,43);

    TH1D *TTZ_sb_low = NULL;
    TH1D *TTZ_sb_low_updown[exts];
    TH1D *TTZ_sb_low_LHEScaleWeight[9];
    TH1D *TTZ_sb_low_avgwt = new TH1D("TTZ_sb_low_avg_weight","avg_weight",53,0,53);
    TH1D *TTZ_sb_low_avgwtsq = new TH1D("TTZ_sb_low_avg_weight_sq","avg_weight_sq",53,0,53);
    TH1D *TTZ_sb_high = NULL;
    TH1D *TTZ_sb_high_updown[exts];
    TH1D *TTZ_sb_high_LHEScaleWeight[9];
    TH1D *TTZ_sb_high_avgwt = new TH1D("TTZ_sb_high_avg_weight","avg_weight",130,53,183);
    TH1D *TTZ_sb_high_avgwtsq = new TH1D("TTZ_sb_high_avg_weight_sq","avg_weight_sq",130,53,183);
    TH1D *Rare_sb_low = NULL;
    TH1D *Rare_sb_low_updown[exts];
    TH1D *Rare_sb_low_LHEScaleWeight[9];
    TH1D *Rare_sb_low_avgwt = new TH1D("Rare_sb_low_avg_weight","avg_weight",53,0,53);
    TH1D *Rare_sb_low_avgwtsq = new TH1D("Rare_sb_low_avg_weight_sq","avg_weight_sq",53,0,53);
    TH1D *Rare_sb_high = NULL;
    TH1D *Rare_sb_high_updown[exts];
    TH1D *Rare_sb_high_LHEScaleWeight[9];
    TH1D *Rare_sb_high_avgwt = new TH1D("Rare_sb_high_avg_weight","avg_weight",130,53,183);
    TH1D *Rare_sb_high_avgwtsq = new TH1D("Rare_sb_high_avg_weight_sq","avg_weight_sq",130,53,183);
    TH1D *Comb_sb_low = NULL;
    TH1D *Comb_sb_low_updown[exts];
    TH1D *Comb_sb_low_LHEScaleWeight[9];
    TH1D *Comb_sb_low_avgwt = new TH1D("Comb_sb_low_avg_weight","avg_weight",53,0,53);
    TH1D *Comb_sb_low_avgwtsq = new TH1D("Comb_sb_low_avg_weight_sq","avg_weight_sq",53,0,53);
    TH1D *Comb_sb_high = NULL;
    TH1D *Comb_sb_high_updown[exts];
    TH1D *Comb_sb_high_LHEScaleWeight[9];
    TH1D *Comb_sb_high_avgwt = new TH1D("Comb_sb_high_avg_weight","avg_weight",130,53,183);
    TH1D *Comb_sb_high_avgwtsq = new TH1D("Comb_sb_high_avg_weight_sq","avg_weight_sq",130,53,183);

    for(int j = 0; j < exts; j++)
    {
        TTZ_vb_low_updown[j]   = NULL;
        TTZ_vb_high_updown[j]  = NULL;
        Rare_vb_low_updown[j]  = NULL;
        Rare_vb_high_updown[j] = NULL;
        Comb_vb_low_updown[j]  = NULL;
        Comb_vb_high_updown[j] = NULL;
        TTZ_sb_low_updown[j]   = NULL;
        TTZ_sb_high_updown[j]  = NULL;
        Rare_sb_low_updown[j]  = NULL;
        Rare_sb_high_updown[j] = NULL;
        Comb_sb_low_updown[j]  = NULL;
        Comb_sb_high_updown[j] = NULL;
    }
    for(int j = 0; j < 9; j++)
    {
        TTZ_vb_low_LHEScaleWeight[j]   = NULL;
        TTZ_vb_high_LHEScaleWeight[j]  = NULL;
        Rare_vb_low_LHEScaleWeight[j]  = NULL;
        Rare_vb_high_LHEScaleWeight[j] = NULL;
        Comb_vb_low_LHEScaleWeight[j]  = NULL;
        Comb_vb_high_LHEScaleWeight[j] = NULL;
        TTZ_sb_low_LHEScaleWeight[j]   = NULL;
        TTZ_sb_high_LHEScaleWeight[j]  = NULL;
        Rare_sb_low_LHEScaleWeight[j]  = NULL;
        Rare_sb_high_LHEScaleWeight[j] = NULL;
        Comb_sb_low_LHEScaleWeight[j]  = NULL;
        Comb_sb_high_LHEScaleWeight[j] = NULL;
    }

    TTZ_vb_low = AddToHist(TTZ_vb_low, year, dir, suffix, TTZ_name, vars[0], lumi, "TTZ_vb_low", TTZ_vb_low_avgwt, TTZ_vb_low_avgwtsq);
    TTZ_vb_high = AddToHist(TTZ_vb_high, year, dir, suffix, TTZ_name, vars[1], lumi, "TTZ_vb_high", TTZ_vb_high_avgwt, TTZ_vb_high_avgwtsq);
    TTZ_sb_low = AddToHist(TTZ_sb_low, year, dir, suffix, TTZ_name, vars[2], lumi, "TTZ_sb_low", TTZ_sb_low_avgwt, TTZ_sb_low_avgwtsq);
    TTZ_sb_high = AddToHist(TTZ_sb_high, year, dir, suffix, TTZ_name, vars[3], lumi, "TTZ_sb_high", TTZ_sb_high_avgwt, TTZ_sb_high_avgwtsq);
/*
    TTZ_vb_low_SF_up = TTZ_vb_low->Clone("TTZ_vb_low_SF_up");
    TTZ_vb_low_SF_up->Scale(SF_up/SF);
    TTZ_vb_low_SF_down = TTZ_vb_low->Clone("TTZ_vb_low_SF_down");
    TTZ_vb_low_SF_down->Scale(SF_down/SF);
    TTZ_vb_high_SF_up = TTZ_vb_high->Clone("TTZ_vb_high_SF_up");
    TTZ_vb_high_SF_up->Scale(SF_up/SF);
    TTZ_vb_high_SF_down = TTZ_vb_high->Clone("TTZ_vb_high_SF_down");
    TTZ_vb_high_SF_down->Scale(SF_down/SF);
    TTZ_sb_low_SF_up = TTZ_sb_low->Clone("TTZ_sb_low_SF_up");
    TTZ_sb_low_SF_up->Scale(SF_up/SF);
    TTZ_sb_low_SF_down = TTZ_sb_low->Clone("TTZ_sb_low_SF_down");
    TTZ_sb_low_SF_down->Scale(SF_down/SF);
    TTZ_sb_high_SF_up = TTZ_sb_high->Clone("TTZ_sb_high_SF_up");
    TTZ_sb_high_SF_up->Scale(SF_up/SF);
    TTZ_sb_high_SF_down = TTZ_sb_high->Clone("TTZ_sb_high_SF_down");
    TTZ_sb_high_SF_down->Scale(SF_down/SF);
*/
    for(int j = 0; j < exts; j++)
    {
        TTZ_vb_low_updown[j] = AddToHist(TTZ_vb_low_updown[j], year, dir, suffix, TTZ_name, vars[0]+extensions[j], lumi, "TTZ_vb_low" + extensions[j]);
        TTZ_vb_high_updown[j] = AddToHist(TTZ_vb_high_updown[j], year, dir, suffix, TTZ_name, vars[1]+extensions[j], lumi, "TTZ_vb_high" + extensions[j]);
        TTZ_sb_low_updown[j] = AddToHist(TTZ_sb_low_updown[j], year, dir, suffix, TTZ_name, vars[2]+extensions[j], lumi, "TTZ_sb_low" + extensions[j]);
        TTZ_sb_high_updown[j] = AddToHist(TTZ_sb_high_updown[j], year, dir, suffix, TTZ_name, vars[3]+extensions[j], lumi, "TTZ_sb_high" + extensions[j]);
    }

    for(int j = 0; j < 9; j++)
    {
        if(j == 2 || j == 6) continue;
        TTZ_vb_low_LHEScaleWeight[j] = AddToHist(TTZ_vb_low_LHEScaleWeight[j], year, dir, suffix, TTZ_name, vars[0]+"_LHEScaleWeight_"+std::to_string(j), lumi, "TTZ_vb_low_LHEScaleWeight_"+std::to_string(j));
        TTZ_vb_high_LHEScaleWeight[j] = AddToHist(TTZ_vb_high_LHEScaleWeight[j], year, dir, suffix, TTZ_name, vars[1]+"_LHEScaleWeight_"+std::to_string(j), lumi, "TTZ_vb_high_LHEScaleWeight_"+std::to_string(j));
        TTZ_sb_low_LHEScaleWeight[j] = AddToHist(TTZ_sb_low_LHEScaleWeight[j], year, dir, suffix, TTZ_name, vars[2]+"_LHEScaleWeight_"+std::to_string(j), lumi, "TTZ_sb_low_LHEScaleWeight_"+std::to_string(j));
        TTZ_sb_high_LHEScaleWeight[j] = AddToHist(TTZ_sb_high_LHEScaleWeight[j], year, dir, suffix, TTZ_name, vars[3]+"_LHEScaleWeight_"+std::to_string(j), lumi, "TTZ_sb_high_LHEScaleWeight_"+std::to_string(j));
    }

    for(int i = 0; i < rare_num; i++)
    {
        Rare_vb_low = AddToHist(Rare_vb_low, year, dir, suffix, Rare_names[i], vars[0], lumi, "Rare_vb_low", Rare_vb_low_avgwt, Rare_vb_low_avgwtsq);
        Rare_vb_high = AddToHist(Rare_vb_high, year, dir, suffix, Rare_names[i], vars[1], lumi, "Rare_vb_high", Rare_vb_low_avgwt, Rare_vb_low_avgwtsq);
        Rare_sb_low = AddToHist(Rare_sb_low, year, dir, suffix, Rare_names[i], vars[2], lumi, "Rare_sb_low", Rare_sb_low_avgwt, Rare_sb_low_avgwtsq);
        Rare_sb_high = AddToHist(Rare_sb_high, year, dir, suffix, Rare_names[i], vars[3], lumi, "Rare_sb_high", Rare_sb_low_avgwt, Rare_sb_low_avgwtsq);
        
        for(int j = 0; j < exts; j++)
        {
            Rare_vb_low_updown[j] = AddToHist(Rare_vb_low_updown[j], year, dir, suffix, Rare_names[i], vars[0]+extensions[j], lumi, "Rare_vb_low" + extensions[j]);
            Rare_vb_high_updown[j] = AddToHist(Rare_vb_high_updown[j], year, dir, suffix, Rare_names[i], vars[1]+extensions[j], lumi, "Rare_vb_high" + extensions[j]);
            Rare_sb_low_updown[j] = AddToHist(Rare_sb_low_updown[j], year, dir, suffix, Rare_names[i], vars[2]+extensions[j], lumi, "Rare_sb_low" + extensions[j]);
            Rare_sb_high_updown[j] = AddToHist(Rare_sb_high_updown[j], year, dir, suffix, Rare_names[i], vars[3]+extensions[j], lumi, "Rare_sb_high" + extensions[j]);
        }
        for(int j = 0; j < 9; j++)
        {
            if(j == 2 || j == 6) continue;
            Rare_vb_low_LHEScaleWeight[j] = AddToHist(Rare_vb_low_LHEScaleWeight[j], year, dir, suffix, Rare_names[i], vars[0]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Rare_vb_low_LHEScaleWeight_"+std::to_string(j));
            Rare_vb_high_LHEScaleWeight[j] = AddToHist(Rare_vb_high_LHEScaleWeight[j], year, dir, suffix, Rare_names[i], vars[1]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Rare_vb_high_LHEScaleWeight_"+std::to_string(j));
            Rare_sb_low_LHEScaleWeight[j] = AddToHist(Rare_sb_low_LHEScaleWeight[j], year, dir, suffix, Rare_names[i], vars[2]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Rare_sb_low_LHEScaleWeight_"+std::to_string(j));
            Rare_sb_high_LHEScaleWeight[j] = AddToHist(Rare_sb_high_LHEScaleWeight[j], year, dir, suffix, Rare_names[i], vars[3]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Rare_sb_high_LHEScaleWeight_"+std::to_string(j));
        }
    }
    for(int i = 0; i < comb_num; i++)
    {
        Comb_vb_low = AddToHist(Comb_vb_low, year, dir, suffix, Comb_names[i], vars[0], lumi, "Comb_vb_low", Comb_vb_low_avgwt, Comb_vb_low_avgwtsq);
        Comb_vb_high = AddToHist(Comb_vb_high, year, dir, suffix, Comb_names[i], vars[1], lumi, "Comb_vb_high", Comb_vb_low_avgwt, Comb_vb_low_avgwtsq);
        Comb_sb_low = AddToHist(Comb_sb_low, year, dir, suffix, Comb_names[i], vars[2], lumi, "Comb_sb_low", Comb_sb_low_avgwt, Comb_sb_low_avgwtsq);
        Comb_sb_high = AddToHist(Comb_sb_high, year, dir, suffix, Comb_names[i], vars[3], lumi, "Comb_sb_high", Comb_sb_low_avgwt, Comb_sb_low_avgwtsq);
        
        for(int j = 0; j < exts; j++)
        {
            Comb_vb_low_updown[j] = AddToHist(Comb_vb_low_updown[j], year, dir, suffix, Comb_names[i], vars[0]+extensions[j], lumi, "Comb_vb_low" + extensions[j]);
            Comb_vb_high_updown[j] = AddToHist(Comb_vb_high_updown[j], year, dir, suffix, Comb_names[i], vars[1]+extensions[j], lumi, "Comb_vb_high" + extensions[j]);
            Comb_sb_low_updown[j] = AddToHist(Comb_sb_low_updown[j], year, dir, suffix, Comb_names[i], vars[2]+extensions[j], lumi, "Comb_sb_low" + extensions[j]);
            Comb_sb_high_updown[j] = AddToHist(Comb_sb_high_updown[j], year, dir, suffix, Comb_names[i], vars[3]+extensions[j], lumi, "Comb_sb_high" + extensions[j]);
        }
        for(int j = 0; j < 9; j++)
        {
            if(j == 2 || j == 6) continue;
            Comb_vb_low_LHEScaleWeight[j] = AddToHist(Comb_vb_low_LHEScaleWeight[j], year, dir, suffix, Comb_names[i], vars[0]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Comb_vb_low_LHEScaleWeight_"+std::to_string(j));
            Comb_vb_high_LHEScaleWeight[j] = AddToHist(Comb_vb_high_LHEScaleWeight[j], year, dir, suffix, Comb_names[i], vars[1]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Comb_vb_high_LHEScaleWeight_"+std::to_string(j));
            Comb_sb_low_LHEScaleWeight[j] = AddToHist(Comb_sb_low_LHEScaleWeight[j], year, dir, suffix, Comb_names[i], vars[2]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Comb_sb_low_LHEScaleWeight_"+std::to_string(j));
            Comb_sb_high_LHEScaleWeight[j] = AddToHist(Comb_sb_high_LHEScaleWeight[j], year, dir, suffix, Comb_names[i], vars[3]+"_LHEScaleWeight_"+std::to_string(j), lumi, "Comb_sb_high_LHEScaleWeight_"+std::to_string(j));
        }
    }

    TTZ_vb_low_avgwtsq->Divide(TTZ_vb_low_avgwt);
    TTZ_vb_high_avgwtsq->Divide(TTZ_vb_high_avgwt);
    Rare_vb_low_avgwtsq->Divide(Rare_vb_low_avgwt);
    Rare_vb_high_avgwtsq->Divide(Rare_vb_high_avgwt);
    Comb_vb_low_avgwtsq->Divide(Comb_vb_low_avgwt);
    Comb_vb_high_avgwtsq->Divide(Comb_vb_high_avgwt);
    TTZ_sb_low_avgwtsq->Divide(TTZ_sb_low_avgwt);
    TTZ_sb_high_avgwtsq->Divide(TTZ_sb_high_avgwt);
    Rare_sb_low_avgwtsq->Divide(Rare_sb_low_avgwt);
    Rare_sb_high_avgwtsq->Divide(Rare_sb_high_avgwt);
    Comb_sb_low_avgwtsq->Divide(Comb_sb_low_avgwt);
    Comb_sb_high_avgwtsq->Divide(Comb_sb_high_avgwt);

    //Set nonpositive values to 0:
    for(int i = 0; i < 19; i++)
    {
        if(TTZ_vb_low->GetBinContent(i+1) < 0) TTZ_vb_low->SetBinContent(i+1,0);
        if(Rare_vb_low->GetBinContent(i+1) < 0) Rare_vb_low->SetBinContent(i+1,0);
        if(Comb_vb_low->GetBinContent(i+1) < 0) Comb_vb_low->SetBinContent(i+1,0);
    }
    for(int i = 0; i < 24; i++)
    {
        if(TTZ_vb_high->GetBinContent(i+1) < 0) TTZ_vb_high->SetBinContent(i+1,0);
        if(Rare_vb_high->GetBinContent(i+1) < 0) Rare_vb_high->SetBinContent(i+1,0);
        if(Comb_vb_high->GetBinContent(i+1) < 0) Comb_vb_high->SetBinContent(i+1,0);
    }
    for(int i = 0; i < 53; i++)
    {
        if(TTZ_sb_low->GetBinContent(i+1) < 0) TTZ_sb_low->SetBinContent(i+1,0);
        if(Rare_sb_low->GetBinContent(i+1) < 0) Rare_sb_low->SetBinContent(i+1,0);
        if(Comb_sb_low->GetBinContent(i+1) < 0) Comb_sb_low->SetBinContent(i+1,0);
    }
    for(int i = 0; i < 130; i++)
    {
        if(TTZ_sb_high->GetBinContent(i+1) < 0) TTZ_sb_high->SetBinContent(i+1,0);
        if(Rare_sb_high->GetBinContent(i+1) < 0) Rare_sb_high->SetBinContent(i+1,0);
        if(Comb_sb_high->GetBinContent(i+1) < 0) Comb_sb_high->SetBinContent(i+1,0);
    }

    //if(!forgraphs)
    if(false)
    {
        for(int j = 0; j < exts; j++)
        {
            //TTZ_vb_low_updown[j] = (TTZ_vb_low_updown[j]-TTZ_vb_low)/TTZ_vb_low
            TTZ_vb_low_updown[j]->Add(TTZ_vb_low,-1);
            TTZ_vb_low_updown[j]->Divide(TTZ_vb_low);
            TTZ_vb_high_updown[j]->Add(TTZ_vb_high,-1);
            TTZ_vb_high_updown[j]->Divide(TTZ_vb_high);
            Rare_vb_low_updown[j]->Add(Rare_vb_low,-1);
            Rare_vb_low_updown[j]->Divide(Rare_vb_low);
            Rare_vb_high_updown[j]->Add(Rare_vb_high,-1);
            Rare_vb_high_updown[j]->Divide(Rare_vb_high);
            TTZ_sb_low_updown[j]->Add(TTZ_sb_low,-1);
            TTZ_sb_low_updown[j]->Divide(TTZ_sb_low);
            TTZ_sb_high_updown[j]->Add(TTZ_sb_high,-1);
            TTZ_sb_high_updown[j]->Divide(TTZ_sb_high);
            Rare_sb_low_updown[j]->Add(Rare_sb_low,-1);
            Rare_sb_low_updown[j]->Divide(Rare_sb_low);
            Rare_sb_high_updown[j]->Add(Rare_sb_high,-1);
            Rare_sb_high_updown[j]->Divide(Rare_sb_high);
        }
    }

    if(createoutput)
    {
        std::cout << "Outputting to file " + outputfile << std::endl;
        TFile out_file(outputfile,"RECREATE");
        TTZ_vb_low->Write();
        TTZ_vb_high->Write();
        Rare_vb_low->Write();
        Rare_vb_high->Write();
        Comb_vb_low->Write();
        Comb_vb_high->Write();
        TTZ_sb_low->Write();
        TTZ_sb_high->Write();
        Rare_sb_low->Write();
        Rare_sb_high->Write();
        Comb_sb_low->Write();
        Comb_sb_high->Write();
        for(int j = 0; j < exts; j++)
        {
            TTZ_vb_low_updown[j]->Write();
            TTZ_vb_high_updown[j]->Write();
            Rare_vb_low_updown[j]->Write();
            Rare_vb_high_updown[j]->Write();
            Comb_vb_low_updown[j]->Write();
            Comb_vb_high_updown[j]->Write();
            TTZ_sb_low_updown[j]->Write();
            TTZ_sb_high_updown[j]->Write();
            Rare_sb_low_updown[j]->Write();
            Rare_sb_high_updown[j]->Write();
            Comb_sb_low_updown[j]->Write();
            Comb_sb_high_updown[j]->Write();
        }
        for(int j = 0; j < 9; j++)
        {
            if(j == 2 || j == 6) continue;
            TTZ_vb_low_LHEScaleWeight[j]->Write();
            TTZ_vb_high_LHEScaleWeight[j]->Write();
            Rare_vb_low_LHEScaleWeight[j]->Write();
            Rare_vb_high_LHEScaleWeight[j]->Write();
            Comb_vb_low_LHEScaleWeight[j]->Write();
            Comb_vb_high_LHEScaleWeight[j]->Write();
            TTZ_sb_low_LHEScaleWeight[j]->Write();
            TTZ_sb_high_LHEScaleWeight[j]->Write();
            Rare_sb_low_LHEScaleWeight[j]->Write();
            Rare_sb_high_LHEScaleWeight[j]->Write();
            Comb_sb_low_LHEScaleWeight[j]->Write();
            Comb_sb_high_LHEScaleWeight[j]->Write();
        }
        TTZ_vb_low_avgwtsq->Write();
        TTZ_vb_high_avgwtsq->Write();
        Rare_vb_low_avgwtsq->Write();
        Rare_vb_high_avgwtsq->Write();
        Comb_vb_low_avgwtsq->Write();
        Comb_vb_high_avgwtsq->Write();
        TTZ_sb_low_avgwtsq->Write();
        TTZ_sb_high_avgwtsq->Write();
        Rare_sb_low_avgwtsq->Write();
        Rare_sb_high_avgwtsq->Write();
        Comb_sb_low_avgwtsq->Write();
        Comb_sb_high_avgwtsq->Write();
        out_file.Close();
    }

    delete TTZ_vb_low;
    delete TTZ_vb_high;
    delete Rare_vb_low;
    delete Rare_vb_high;
    delete Comb_vb_low;
    delete Comb_vb_high;
    delete TTZ_sb_low;
    delete TTZ_sb_high;
    delete Rare_sb_low;
    delete Rare_sb_high;
    delete Comb_sb_low;
    delete Comb_sb_high;
    for(int j = 0; j < exts; j++)
    {
        delete TTZ_vb_low_updown[j];
        delete TTZ_vb_high_updown[j];
        delete Rare_vb_low_updown[j];
        delete Rare_vb_high_updown[j];
        delete Comb_vb_low_updown[j];
        delete Comb_vb_high_updown[j];
        delete TTZ_sb_low_updown[j];
        delete TTZ_sb_high_updown[j];
        delete Rare_sb_low_updown[j];
        delete Rare_sb_high_updown[j];
        delete Comb_sb_low_updown[j];
        delete Comb_sb_high_updown[j];
    }
    for(int j = 0; j < 9; j++)
    {
        if(j == 2 || j == 6) continue;
        delete TTZ_vb_low_LHEScaleWeight[j];
        delete TTZ_vb_high_LHEScaleWeight[j];
        delete Rare_vb_low_LHEScaleWeight[j];
        delete Rare_vb_high_LHEScaleWeight[j];
        delete Comb_vb_low_LHEScaleWeight[j];
        delete Comb_vb_high_LHEScaleWeight[j];
        delete TTZ_sb_low_LHEScaleWeight[j];
        delete TTZ_sb_high_LHEScaleWeight[j];
        delete Rare_sb_low_LHEScaleWeight[j];
        delete Rare_sb_high_LHEScaleWeight[j];
        delete Comb_sb_low_LHEScaleWeight[j];
        delete Comb_sb_high_LHEScaleWeight[j];
    }
/*
    delete TTZ_vb_low_SF_up;
    delete TTZ_vb_low_SF_down;
    delete TTZ_vb_high_SF_up;
    delete TTZ_vb_high_SF_down;
    delete TTZ_sb_low_SF_up;
    delete TTZ_sb_low_SF_down;
    delete TTZ_sb_high_SF_up;
    delete TTZ_sb_high_SF_down;
*/
    delete TTZ_vb_low_avgwtsq;
    delete TTZ_vb_high_avgwtsq;
    delete Rare_vb_low_avgwtsq;
    delete Rare_vb_high_avgwtsq;
    delete Comb_vb_low_avgwtsq;
    delete Comb_vb_high_avgwtsq;
    delete TTZ_sb_low_avgwtsq;
    delete TTZ_sb_high_avgwtsq;
    delete Rare_sb_low_avgwtsq;
    delete Rare_sb_high_avgwtsq;
    delete Comb_sb_low_avgwtsq;
    delete Comb_sb_high_avgwtsq;

    return 0;
}
