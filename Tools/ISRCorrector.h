#ifndef ISRCORRECTOR_H
#define ISRCORRECTOR_H

#include <TH1.h>
#include <TLorentzVector.h>
#include "TFile.h"
#include <iostream>
#include "NTupleReader.h"
#include <string>

class ISRCorrector {
private:
    bool isFirst;

 public:
  //constructor
ISRCorrector(std::string fnISR = "Signal_fastsim_T1tttt_scan_ISR.root", std::string ISRJetsFilePath = "ISR_Root_Files", TString massPoint = "1200_800", std::string suffix = "TTbarSingleLepT"): massPoint_(massPoint)
  {
      isFirst = true;
      
      if(ISRJetsFilePath.size())
        {
	  fnISR_ = new TFile((ISRJetsFilePath +"/"+fnISR).c_str());
	  fISRWght = new TFile((ISRJetsFilePath+"/"+"ISRWeights.root").c_str());
        }
      else
        {
	  fnISR_ = new TFile(fnISR.c_str());
	  fISRWght = new TFile("ISRWeights.root");
	}

      if( massPoint.IsNull() ){
          if(suffix !="")
              h_njetsisr = (TH1D*)fnISR_->Get(("NJetsISR_"+suffix).c_str());
          else {
              h_njetsisr = (TH1D*)fnISR_->Get("NJetsISR");
          }
      } else{
         h_njetsisr = (TH1D*)fnISR_->Get("NJetsISR_"+ massPoint_);
      }


      //get Weight histograms.
      h_isr_up = (TH1D*)fISRWght->Get("isr_weights_up");
      h_isr_central= (TH1D*)fISRWght->Get("isr_weights_central");
      h_isr_down = (TH1D*)fISRWght->Get("isr_weights_down");

      //Set Weight
      SetWeights_Up(h_isr_up,  h_njetsisr);
      SetWeights_Cent(h_isr_central,  h_njetsisr);
      SetWeights_Down(h_isr_down,  h_njetsisr);

    }
  //destructor
  virtual ~ISRCorrector() {}
  
  //accessors
  void SetWeights_Up(TH1D* weights, TH1D* all);
  void SetWeights_Cent(TH1D* weights, TH1D* all);
  void SetWeights_Down(TH1D* weights, TH1D* all);  
  
  //functions
  float GetCorrection_Up(int NJetsISR);
  float GetCorrection_Cent(int NJetsISR);
  float GetCorrection_Down(int NJetsISR);

  void registerVarToNTuples(NTupleReader& tr);

  void operator()(NTupleReader& tr);

  void resetSample(std::string newsample) 
  {
      suffix = newsample;
      
      TH1D* h_njetsisr2 = (TH1D*)fnISR_->Get(("NJetsISR_"+newsample).c_str());
      if(h_njetsisr2)
      {
          h_njetsisr = h_njetsisr2;
          SetWeights_Up(h_isr_up,  h_njetsisr);
          SetWeights_Cent(h_isr_central,  h_njetsisr);
          SetWeights_Down(h_isr_down,  h_njetsisr);
      }
      else 
      {
          h_njetsisr = (TH1D*)fnISR_->Get("NJetsISR_TTbarSingleLepT");
          SetWeights_Up(h_isr_up,  h_njetsisr);
          SetWeights_Cent(h_isr_central,  h_njetsisr);
          SetWeights_Down(h_isr_down,  h_njetsisr);
      }
  }

  //member variables
  TH1D *h_njetsisr, *h_weights_central, *h_weights_up, *h_weights_down;
  TH1D *h_isr_up, *h_isr_central, *h_isr_down;
  TFile *fnISR_,  *fISRWght;
  TString massPoint_;
  std::string suffix;
};


#endif
