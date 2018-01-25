#include "CrossSection.C"

int plot_SB_test()
{
const double lumi = 36;

TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 1280, 720);;
gStyle->SetOptStat(kFALSE);

TPad *padup = new TPad("padup", "padup", 0, 0.25, 1, 1.0);
padup -> SetBottomMargin(0.05);
padup -> Draw();
padup -> cd();

TString sp = "t2tt_700_400_result";
//TString sp = "t2tt_1000_1_result";

TString folder = "";
TString var = "search_bin_team_A_h";
TFile *f1 = new TFile("test/" + sp + ".root");
TH1D *h1 = (TH1D*)f1->Get(folder + var);
TH1D *h2 = (TH1D*)f1->Get("Baseline_Only/eff_h");
TH1D *h0 = (TH1D*)f1->Get("search_bin_team_A_MTb140_MT2_h");
TH1D *h00 = (TH1D*)f1->Get("search_bin_team_A_MTb175_MT2_h");

double all_events = h2->GetBinContent(1);
double left_events = h2->GetBinContent(2);

int binN = 0;
double binC = 0;
double xsec = 0.0670476;	//for mStop = 700 
//double xsec = 0.00615134;	//for mStop = 1000 

TH1D *h3 = new TH1D ("team_A_result", "team A result", 51, 53, 104);
double team_A_700_400[51] = {2.31, 3.57, 1.54, 0.58, 6.02, 8.53, 3.53, 1.29, 16.04, 9.81, 3.77, 3.85, 12.24, 7.61, 1.62, 2.36, 0.52, 0.29, 27.35, 14.62, 4.92, 0.91, 0.56, 0.09, 3.09, 1.54, 0.08, 0.57, 0.48, 0.40, 23.64, 15.54, 4.26, 1.70, 0.66, 5.83, 2.50, 1.54, 0.56, 0.27, 0.00, 1.42, 0.92, 0.18, 0.44, 0.62, 0.65, 0.27, 0.35, 4.38, 0.31};
double team_A_1000_1[51] = {0.05, 0.08, 0.04, 0.07, 0.22, 0.30, 0.21, 0.27, 0.61, 0.80, 0.89, 2.89, 0.56, 0.53, 0.71, 2.54, 1.31, 3.64, 0.80, 1.05, 1.16, 0.89, 2.67, 0.86, 0.11, 0.19, 0.24, 0.81, 1.10, 3.55, 0.77, 0.83, 0.97, 1.08, 2.26, 0.21, 0.41, 0.49, 0.45, 0.99, 0.65, 0.13, 0.14, 0.12, 0.47, 0.14, 0.16, 1.39, 1.00, 0.65, 0.14};
for (int i = 1; i < 52; i++)
{
h3->SetBinContent(i,team_A_700_400[i-1]);
//h3->SetBinContent(i,team_A_1000_1[i-1]);
}

h3->SetMarkerStyle(34);
//h3->SetMarkerStyle(2);
h3->SetMarkerSize(2);
h3->GetYaxis()->SetTitle("events");
h3->GetXaxis()->SetTitle("search bin");
h3->SetTitle("T2tt mStop 700, mLSP 400");
//h3->SetTitle("T2tt mStop 1000, mLSP 1");
h3->GetXaxis()->SetRangeUser(53,104);
h3->GetYaxis()->SetRangeUser(0,50);
//h3->GetYaxis()->SetRangeUser(0,6);
h3->GetXaxis()->SetNdivisions(26);
h3->Draw("P");


//T2tt mStop 700, mLSP 400
binN = 84;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 102;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 85;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 83;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 66;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;


/*
//T2tt mStop 1100, mLSP 1
binN = 82;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 70;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 76;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 100;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;

binN = 101;
binC = h1->GetBinContent(binN-51);
std::cout << "bin " << binN << " original events = " << binC << " after scale = " << binC*lumi*1000*xsec/all_events << std::endl;
*/

h1->Scale(lumi * xsec * 1000 / all_events);
h1->SetLineColor(kRed);
h1->SetLineWidth(3);
h1->Draw("SAME");

h0->Scale(lumi * xsec * 1000 / all_events);
h0->SetLineColor(kBlue);
h0->SetLineWidth(3);
h0->Draw("SAME");

h00->Scale(lumi * xsec * 1000 / all_events);
h00->SetLineColor(kGreen);
h00->SetLineWidth(3);
h00->Draw("SAME");

TLegend* leg = new TLegend(0.55,0.55,0.9,0.89);
leg->SetBorderSize(0);
leg->SetTextFont(42);
leg->SetFillColor(0);
leg->AddEntry(h3,"signal yield in SUS-16-049 ANA note","p");
leg->AddEntry(h1,"simulate SUS-16-049","l");
leg->AddEntry(h00,"MTb 175 with MT2 cut","l");
leg->AddEntry(h0,"MTb 140 with MT2 cut","l");
leg->AddEntry((TObject*)0,"bottom pad: ratio of simulate / ANA note","");
leg->Draw("SAME");

mycanvas -> cd();

TPad *paddown = new TPad("paddown", "paddown", 0, 0, 1, 0.25);
paddown -> SetTopMargin(0.05);
paddown -> SetBottomMargin(0.3);
paddown -> Draw();
paddown -> cd();

TH1D *h4 = new TH1D ("ratio", "ratio of simulation / team A result", 51, 53, 104);
for (int i = 1; i < 52; i++)
{
if(team_A_700_400[i-1] != 0) h4->SetBinContent(i,h1->GetBinContent(i+1) / team_A_700_400[i-1]);
//if(team_A_1000_1[i-1] != 0) h4->SetBinContent(i,h1->GetBinContent(i+1) / team_A_1000_1[i-1]);
else h4->SetBinContent(i,1);
}
h4->SetMarkerStyle(8);
//h4->SetMarkerSize(2);
h4->GetYaxis()->SetTitle("ratio");
h4->GetXaxis()->SetTitle("search bin");
h4->GetYaxis()->SetTitleSize(0.1);
h4->GetYaxis()->SetLabelSize(0.1);
h4->GetXaxis()->SetTitleSize(0.1);
h4->GetXaxis()->SetLabelSize(0.1);
h4->SetTitle("");
h4->GetXaxis()->SetNdivisions(26);
h4->GetYaxis()->SetRangeUser(0,2);
h4->Draw("P");

TLine* line = new TLine(53, 1, 104, 1);
line->SetLineColor(kRed);
line->Draw("SAME");

//paddown -> SetLogy();

mycanvas->SaveAs(sp + ".png");

return 0;
}

