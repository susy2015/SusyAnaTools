#ifndef BTAGCORRECTOR_H
#define BTAGCORRECTOR_H

//custom headers
#include "BTagCalibrationStandalone.h"
#include "NTupleReader.h"
//ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TH2.h"

//STL headers
#include <string>
#include <vector>

class BTagCorrector {
public:
    //constructor
BTagCorrector(std::string file = "batchSignalPlots.root", std::string CSVFilePath = "", bool isFastSim = false) : debug(false), fastsim(false), btagSFunc(0), ctagSFunc(0), mistagSFunc(0), btagCFunc(0), ctagCFunc(0), mistagCFunc(0), h_eff_b(NULL), h_eff_c(NULL), h_eff_udsg(NULL) {
        //Stops unwanted segfaults.
        TH1::AddDirectory(false);

        //Hard Coded here.
        // Replace "bTagEfficiency_test.root" with desirable efficiency file
        // Same "efficiency file and MC sample to run over must be same
        //Efficiency can be calculated using bTagEfficiencyCalc.C

        inFile = new TFile(file.c_str());
        SetEffs(inFile);
        if(CSVFilePath.size())
        {
            SetCalib((CSVFilePath + "/CSVv2_mod.csv").c_str());
        }
        else
        {
            SetCalib("CSVv2_mod.csv");
        }

        if(isFastSim)
        {
            //FastSim
            SetFastSim(true);
            SetCalibFastSim("CSVFiles/CSV_13TEV_Combined_20_11_2015.csv");
        }

    }
    //destructor
    virtual ~BTagCorrector() {}

    //accessors
    void SetDebug(bool d) { debug = d; }
    void SetFastSim(bool f) { fastsim = f; }
    void SetEffs(TFile* file, std::string suffix = ""){
        if(suffix.size())
        {
            h_eff_b = (TH2F*)file->Get(("eff_b_" + suffix).c_str());
            h_eff_c = (TH2F*)file->Get(("eff_c_" + suffix).c_str());
            h_eff_udsg = (TH2F*)file->Get(("eff_udsg_" + suffix).c_str());
        }
        else
        {
            h_eff_b = (TH2F*)file->Get("eff_b");
            h_eff_c = (TH2F*)file->Get("eff_c");
            h_eff_udsg = (TH2F*)file->Get("eff_udsg");
        }
    }
    void resetEffs(std::string suffix)
    {
        if(h_eff_b) delete h_eff_b;
        if(h_eff_c) delete h_eff_c;
        if(h_eff_udsg) delete h_eff_udsg;
        SetEffs(inFile, suffix);
    }
    void SetCalib(std::string cfile){
        //initialize btag helper classes
        calib = BTagCalibration("",cfile);
        reader = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "central");
        readerUp = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "up");
        readerDown = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "down");
    }
    void SetCalibFastSim(std::string cfile){
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
    std::vector<double> GetCorrectionsTest(std::vector<TLorentzVector>* Jets, std::vector<int>* Jets_flavor);


    //method 1b
    std::vector<double> GetCorrections(std::vector<TLorentzVector> *Jets, std::vector<int> *Jets_flavor);
    //method 1a
    double GetSimpleCorrection(std::vector<TLorentzVector> *Jets, std::vector<int> *Jets_flavor,  std::vector<double> *Jets_bDiscriminatorCSV);

    void InitSFEff(double pt, double eta, int flav, std::vector<double>& sfEffList);

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







