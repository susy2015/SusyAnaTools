#ifndef BTAGCORRECTOR_H
#define BTAGCORRECTOR_H

//custom headers
#include "BTagCalibrationStandalone.h"
#include "NTupleReader.h"
//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TH2.h"

//STL headers
#include <string>
#include <vector>

using namespace std;


class BTagCorrector {
 public:
  //constructor
 BTagCorrector() : debug(false), fastsim(false), btagSFunc(0), ctagSFunc(0), mistagSFunc(0), btagCFunc(0), ctagCFunc(0), mistagCFunc(0), h_eff_b(NULL), h_eff_c(NULL), h_eff_udsg(NULL) {

    //Hard Coded here.
    // Replace "bTagEfficiency_test.root" with desirable efficiency file
    // Same "efficiency file and MC sample to run over must be same
    //Efficiency can be calculated using bTagEfficiencyCalc.C

    inFile = new TFile("batchSignalPlots.root");
    SetEffs(inFile);
    SetCalib("CSVv2_mod.csv");

    //FastSim
    //Uncomment following two line for fastSim
    //SetFastSim(true);
    //SetCalibFastSim("CSVFiles/CSV_13TEV_Combined_20_11_2015.csv");

  }
  //destructor
  virtual ~BTagCorrector() {}
  
  //accessors
  void SetDebug(bool d) { debug = d; }
  void SetFastSim(bool f) { fastsim = f; }
  void SetEffs(TFile* file){
    h_eff_b = (TH2F*)file->Get("eff_b");
    h_eff_c = (TH2F*)file->Get("eff_c");
    h_eff_udsg = (TH2F*)file->Get("eff_udsg");
  }
  void SetCalib(string cfile){
    //initialize btag helper classes
    calib = BTagCalibration("",cfile);
    reader = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "central");
    readerUp = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "up");
    readerDown = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "down");
  }
  void SetCalibFastSim(string cfile){
    //read CFs
    calibFast = BTagCalibration("",cfile);
    readerFast = BTagCalibrationReader(&calibFast, BTagEntry::OP_MEDIUM, "fastsim", "central");
    readerFastUp = BTagCalibrationReader(&calibFast, BTagEntry::OP_MEDIUM, "fastsim", "up");
    readerFastDown = BTagCalibrationReader(&calibFast, BTagEntry::OP_MEDIUM, "fastsim", "down");
  }
  void SetBtagSFunc(int u) { btagSFunc = u; }
  void SetCtagSFunc(int u) { ctagSFunc = u; }
  void SetMistagSFunc(int u) { mistagSFunc = u; }
  void SetBtagCFunc(int u) { btagCFunc = u; }
  void SetCtagCFunc(int u) { ctagCFunc = u; }
  void SetMistagCFunc(int u) { mistagCFunc = u; }


  //test Method
  vector<double> GetCorrectionsTest(vector<TLorentzVector>* Jets, vector<int>* Jets_flavor);


  //method 1b
  vector<double> GetCorrections(vector<TLorentzVector> *Jets, vector<int> *Jets_flavor);
  //method 1a
  double GetSimpleCorrection(vector<TLorentzVector> *Jets, vector<int> *Jets_flavor,  vector<double> *Jets_bDiscriminatorCSV);

  void InitSFEff(double pt, double eta, int flav, vector<double>& sfEffList);

  // To register Event weights/ Probabilities to FlatTuples
  void registerVarToNTuples(NTupleReader& tr);
  //Operator
  void operator()(NTupleReader& tr);
  
  //member variables
  bool debug, fastsim;
  int btagSFunc, ctagSFunc, mistagSFunc;
  int btagCFunc, ctagCFunc, mistagCFunc;
  BTagCalibration calib, calibFast;
  BTagCalibrationReader reader, readerUp, readerDown;
  BTagCalibrationReader readerFast, readerFastUp, readerFastDown;
  TH2F *h_eff_b, *h_eff_c, *h_eff_udsg;
  TFile *inFile;
};


#endif







