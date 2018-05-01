#include "CrossSection.C"

int Plot_2D_test()
{
const double lumi = 36;

//TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 1280, 720);
TCanvas* mycanvas = new TCanvas();

//TString sp = "T2bwC_mStop500_mLSP490";
//TString sp = "T2bwC_mStop600_mLSP520";
//TString sp = "T2fbd_mStop600_mLSP520";
TString sp = "T2fbd_mStop500_mLSP490";

//TString folder = "Baseline_Only/";
TString folder = "";
//TString var = "mtb_mt2_h";
TString var = "ISRpt_MET_lowdm_h";
gStyle->SetOptStat(kFALSE);
gStyle->SetPaintTextFormat("4.2f");
//TFile *f1 = new TFile("results/" + sp + ".root");
TFile *f1 = new TFile("results/Signal_" + sp + ".root");
TH2D *h1 = (TH2D*)f1->Get(folder + var);
TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

double all_events = h2->GetBinContent(1);
double left_events = h2->GetBinContent(2);

h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
h1->Rebin2D(4,4);
h1->SetTitle(sp);
h1->GetXaxis()->SetRangeUser(240,800);
h1->GetYaxis()->SetRangeUser(240,800);
h1->GetYaxis()->SetTitle("ISR pt");
h1->GetXaxis()->SetTitle("MET");
h1->SetMarkerSize(2);
h1->Draw("COLZTEXT");

TLegend* leg = new TLegend(0.7,0.9,0.89,0.95);
leg->SetBorderSize(0);
//leg->SetTextFont(42);
//leg->SetFillColor(0);
leg->AddEntry((TObject*)0,"36 fb^{-1} (13TeV)","");
leg->Draw("SAME");

mycanvas->Print(sp + "_" + var + ".png");

return 0;
}

