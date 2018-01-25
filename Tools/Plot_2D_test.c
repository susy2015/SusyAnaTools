#include "CrossSection.C"

int Plot_2D_test()
{
const double lumi = 36;

//TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 1280, 720);
TCanvas* mycanvas = new TCanvas();

//TString sp = "Signal_T2tt_mStop850_mLSP100";
//TString sp = "Signal_T1tttt_mGluino2000_mLSP100";
//TString sp = "Signal_T1tttt_mGluino1500_mLSP1200";
//TString sp = "Signal_T1tttt_mGluino1500_mLSP100";
TString sp = "Signal_T2bw_mStop800_mLSP100";

//TString folder = "Baseline_Only/";
TString folder = "";
//TString var = "mtb_mt2_h";
TString var = "ntop_nw_h";
gStyle->SetOptStat(kFALSE);
gStyle->SetPaintTextFormat("4.2f");
//TFile *f1 = new TFile("results/" + sp + ".root");
TFile *f1 = new TFile(sp + ".root");
TH2D *h1 = (TH2D*)f1->Get(folder + var);
TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");

double all_events = h2->GetBinContent(1);
double left_events = h2->GetBinContent(2);

h1->Scale(lumi * CrossSection.at(sp) * 1000 / all_events );
//h1->Rebin2D(5,5);
h1->SetTitle(sp);
h1->GetXaxis()->SetRangeUser(0,4);
h1->GetYaxis()->SetRangeUser(0,4);
h1->GetYaxis()->SetTitle("nw");
h1->GetXaxis()->SetTitle("ntop");
//h1->DrawClone("Colztexte");
h1->SetMarkerSize(2);
h1->Draw("COLZTEXT");

TLegend* leg = new TLegend(0.7,0.9,0.89,0.95);
leg->SetBorderSize(0);
//leg->SetTextFont(42);
//leg->SetFillColor(0);
leg->AddEntry((TObject*)0,"36 fb^{-1} (13TeV)","");
leg->Draw("SAME");

mycanvas->Print(sp + ".png");

return 0;
}

