#include "ISRCorrector.h"
#include "NTupleReader.h"
#include "SATException.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TMath.h"
//STL headers
#include <string>
#include <vector>
#include<cmath>

using namespace std;

void ISRCorrector::SetWeights_Up(TH1D* weights, TH1D* all){
  if(!weights) return;
  if(!all) return;
  //normalize weights using overall NJetsISR spectrum so total number of gen events will stay the same
  h_weights_up = (TH1D*)weights->Clone();
  h_njetsisr = all;
  double A_LO = h_njetsisr->Integral(0,h_njetsisr->GetNbinsX()+1);
  TH1D* h_njetsisrW = (TH1D*)all->Clone();
  h_njetsisrW->Multiply(h_weights_up);
  double A_NLO = h_njetsisrW->Integral(0,h_njetsisrW->GetNbinsX()+1);
  h_weights_up->Scale(A_LO/A_NLO);
//  std::cout << "COrrection Up: " << A_LO/A_NLO << std::endl;
}

void ISRCorrector::SetWeights_Cent(TH1D* weights, TH1D* all){
  if(!weights) return;
  if(!all) return;
  //normalize weights using overall NJetsISR spectrum so total number of gen events will stay the same                                                  
  h_weights_central = (TH1D*)weights->Clone();
  h_njetsisr = all;
  double A_LO = h_njetsisr->Integral(0,h_njetsisr->GetNbinsX()+1);
  TH1D* h_njetsisrW = (TH1D*)all->Clone();
  h_njetsisrW->Multiply(h_weights_central);
  double A_NLO = h_njetsisrW->Integral(0,h_njetsisrW->GetNbinsX()+1);
  h_weights_central->Scale(A_LO/A_NLO);
//  std::cout << "COrrection Cent: " << A_LO/A_NLO << std::endl;
}

void ISRCorrector::SetWeights_Down(TH1D* weights, TH1D* all){
  if(!weights) return;
  if(!all) return;
  //normalize weights using overall NJetsISR spectrum so total number of gen events will stay the same                                                  
  h_weights_down = (TH1D*)weights->Clone();
  h_njetsisr = all;
  double A_LO = h_njetsisr->Integral(0,h_njetsisr->GetNbinsX()+1);
  TH1D* h_njetsisrW = (TH1D*)all->Clone();
  h_njetsisrW->Multiply(h_weights_down);
  double A_NLO = h_njetsisrW->Integral(0,h_njetsisrW->GetNbinsX()+1);
  h_weights_down->Scale(A_LO/A_NLO);
//  std::cout << "COrrection Down: " << A_LO/A_NLO << std::endl;
}

double ISRCorrector::GetCorrection_Up(int NJetsISR){
  return h_weights_up ? h_weights_up->GetBinContent(h_weights_up->GetXaxis()->FindBin(min(double(NJetsISR),h_weights_up->GetBinLowEdge(h_weights_up->GetNbinsX())))) : 1.;
}

double ISRCorrector::GetCorrection_Cent(int NJetsISR){
  return h_weights_central ? h_weights_central->GetBinContent(h_weights_central->GetXaxis()->FindBin(min(double(NJetsISR),h_weights_central->GetBinLowEdge(h_weights_central->GetNbinsX())))) :1.;
}

double ISRCorrector::GetCorrection_Down(int NJetsISR){
  return h_weights_down ? h_weights_down->GetBinContent(h_weights_down->GetXaxis()->FindBin(min(double(NJetsISR),h_weights_down->GetBinLowEdge(h_weights_down->GetNbinsX())))) :1.;
}


void ISRCorrector::registerVarToNTuples(NTupleReader& tr)
{

    double isr_Unc_Up = 1.0;
    double isr_Unc_Cent = 1.0;
    double isr_Unc_Down = 1.0;

    //Safety for data (where this variable does not exist)
    if(tr.checkBranch("NJetsISR"))
    {
        const int NJetsISR = tr.getVar<int>("NJetsISR");
    
        isr_Unc_Up = GetCorrection_Up(NJetsISR);
        isr_Unc_Cent = GetCorrection_Cent(NJetsISR);
        isr_Unc_Down = GetCorrection_Down(NJetsISR); 
    }
    else
    {
        if(isFirst)
        {
            isFirst = false;
            //Safety if it appears to be MC instead of data (not ideal ... maybe someone can improve this)
            if(tr.checkBranch("genDecayLVec"))
            {
                THROW_SATEXCEPTION("It appears that this is MC, but I don't find the \"NJetsISR\" variable in the NTuple or derived variables");
            }
        }
    }

    if( std::isnan( isr_Unc_Up) || std::isinf(isr_Unc_Up) ) isr_Unc_Up= 1.0;
    if( std::isnan( isr_Unc_Cent) || std::isinf(isr_Unc_Cent) ) isr_Unc_Cent= 1.0;
    if( std::isnan( isr_Unc_Down) || std::isinf(isr_Unc_Down) ) isr_Unc_Down= 1.0;

    tr.registerDerivedVar("isr_Unc_Up", isr_Unc_Up);
    tr.registerDerivedVar("isr_Unc_Cent", isr_Unc_Cent);
    tr.registerDerivedVar("isr_Unc_Down", isr_Unc_Down);

}



/***********************************************************************************/
void ISRCorrector::operator()(NTupleReader& tr)
{
  //
  registerVarToNTuples(tr);
}
/***********************************************************************************/
