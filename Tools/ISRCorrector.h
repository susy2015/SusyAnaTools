#ifndef ISRCORRECTOR_H
#define ISRCORRECTOR_H

#include <TH1.h>
#include <TLorentzVector.h>
#include "TFile.h"
#include <iostream>
#include "NTupleReader.h"
#include <string>

class ISRCorrector {
 public:
  //constructor
 ISRCorrector(std::string fnISR = "TTbarNoHad_NJetsISR.root", std::string ISRJetsFilePath = "", TString massPoint = ""): massPoint_(massPoint)
  {
      
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
     


      h_njetsisr = (TH1D*)fnISR_->Get("NJetsISR");

      //get Weight histograms.
      TH1D *h_isr_up = (TH1D*)fISRWght->Get("isr_weights_up");
      TH1D *h_isr_central= (TH1D*)fISRWght->Get("isr_weights_central");
      TH1D *h_isr_down = (TH1D*)fISRWght->Get("isr_weights_down");

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
  double GetCorrection_Up(int NJetsISR);
  double GetCorrection_Cent(int NJetsISR);
  double GetCorrection_Down(int NJetsISR);

  void registerVarToNTuples(NTupleReader& tr);

  void operator()(NTupleReader& tr);





  //member variables
  TH1D *h_njetsisr, *h_weights_central, *h_weights_up, *h_weights_down;
  TFile *fnISR_,  *fISRWght;
  TString massPoint_;
};


#endif
