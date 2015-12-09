#ifndef WEIGHTS_H
#define WEIGHTS_H

#include "NTupleReader.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <vector>

class Pileup_Sys
{
 public:
  Pileup_Sys();
  virtual ~Pileup_Sys();
  void getPileup_Sys(NTupleReader& tr);
  void operator()(NTupleReader& tr);

 protected:
 private:

};

#endif // Pileup_Sys_H

