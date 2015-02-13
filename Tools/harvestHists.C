#include "TFile.h"
#include "TH1.h"

int main()
{
    TFile *fin = new TFile("histoutput.root");

    TH1* hEff = (TH1*)fin->Get("effMuPtgenMatchMuInAccptgenMatchMuInAcc(pt)DY#rightarrow#mu#muratio");
    TH1* hAcc_denom = (TH1*)fin->Get("genMuPtgenMuptgenMu(pt)DY#rightarrow#mu#musingle");
    TH1* hAcc_numer = (TH1*)fin->Get("genMuPtgenMuInAccptgenMuInAcc(pt)DY#rightarrow#mu#musingle");

    hEff = (TH1*)hEff->Clone("muEffPt");
    
    TH1* hAcc = (TH1*)hAcc_numer->Clone("muAccPt");
    hAcc->Divide(hAcc_denom);

    TFile *fout = new TFile("muEffHists2.root", "RECREATE");
    fout->cd();
    hEff->Write();
    hAcc->Write();
    fout->Close();
    fin->Close();
}
