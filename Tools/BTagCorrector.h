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

class BTagCorrector 
{
public:
    //constructor
    BTagCorrector(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", std::string CSVFile = "CSVv2_Moriond17_B_H.csv", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : debug(false), fastsim(false), btagSFunc(0), mistagSFunc(0), btagCFunc(0), ctagCFunc(0), mistagCFunc(0), h_eff_b(NULL), h_eff_c(NULL), h_eff_udsg(NULL) 
    {
        //Stops unwanted segfaults.
        TH1::AddDirectory(false);
        
        //Hard Coded here.
        // Replace "TTbarNoHad_bTagEff.root" in constuctor  with desirable efficiency file
        // Same "efficiency file and MC sample to run over must be same
        //Efficiency can be calculated using bTagEfficiencyCalc.C

        inFile = new TFile(file.c_str());
        SetEffs(inFile, suffix);
        SetCalib(CSVFile.c_str());	  
        if(CSVFilePath.size())
        {
            SetCalib((CSVFilePath + "/" + CSVFile).c_str());
        }
        else
        {
            SetCalib(CSVFile.c_str());
        }
        
        if(isFastSim)
        {
            //FastSim
            SetFastSim(true);
            SetCalibFastSim("fastsim_csvv2_ttbar_26_1_2017.csv");
        }
    }

    BTagCorrector(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : BTagCorrector(file,CSVFilePath,"CSVv2_Moriond17_B_H.csv",isFastSim,suffix)
    {
    }

    //destructor
    virtual ~BTagCorrector() {}

    //accessors
    void SetDebug(bool d) { debug = d; }
    void SetFastSim(bool f) { fastsim = f; }
    void SetEffs(TFile* file, std::string suffix = "")
    {
        if(suffix.size())
        {
            std::string suffix2 = suffix;
            h_eff_b = (TH2F*)file->Get(("n_eff_b_" + suffix2).c_str());
            if(!h_eff_b)
            {
                std::cout << "Could not find n_eff_b_" << suffix2 << " in Btag file. Will use TTbarSingleLepT as default" << std::endl;
                suffix2 = "TTbarSingleLepT"; // random default value, otherwise you get segfault
                h_eff_b = (TH2F*)file->Get(("n_eff_b_" + suffix2).c_str());
            }
            h_eff_c = (TH2F*)file->Get(("n_eff_c_" + suffix2).c_str());
            h_eff_udsg = (TH2F*)file->Get(("n_eff_udsg_" + suffix2).c_str());
            TH2F *d_eff_b = (TH2F*)file->Get(("d_eff_b_" + suffix2).c_str());
            TH2F *d_eff_c = (TH2F*)file->Get(("d_eff_c_" + suffix2).c_str());
            TH2F *d_eff_udsg = (TH2F*)file->Get(("d_eff_udsg_" + suffix2).c_str());
            
            h_eff_b->Divide(d_eff_b);
            h_eff_c->Divide(d_eff_c);
            h_eff_udsg->Divide(d_eff_udsg);
            
        }
        else
        {
            h_eff_b = (TH2F*)file->Get("n_eff_b");
            h_eff_c = (TH2F*)file->Get("n_eff_c");
            h_eff_udsg = (TH2F*)file->Get("n_eff_udsg");

	    TH2F *d_eff_b = (TH2F*)file->Get("d_eff_b");
	    TH2F *d_eff_c = (TH2F*)file->Get("d_eff_c");
            TH2F *d_eff_udsg = (TH2F*)file->Get("d_eff_udsg");
            
            if(h_eff_b)
            {
                h_eff_b->Divide(d_eff_b);
                h_eff_c->Divide(d_eff_c);
                h_eff_udsg->Divide(d_eff_udsg);
            }
        }
    }
    void resetEffs(std::string suffix)
    {
        if(h_eff_b) delete h_eff_b;
        if(h_eff_c) delete h_eff_c;
        if(h_eff_udsg) delete h_eff_udsg;
        SetEffs(inFile, suffix);
    }
    void SetCalib(std::string cfile)
    {
        
        //initialize btag helper classes. Interface has been changed.
        calib = BTagCalibration("",cfile);
        reader = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central");
        reader.load(calib, BTagEntry::FLAV_B, "comb"); reader.load(calib, BTagEntry::FLAV_C, "comb");  reader.load(calib, BTagEntry::FLAV_UDSG, "incl");
        readerUp = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "up");
        readerUp.load(calib, BTagEntry::FLAV_B, "comb"); readerUp.load(calib, BTagEntry::FLAV_C, "comb");  readerUp.load(calib, BTagEntry::FLAV_UDSG, "incl");
        readerDown = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "down");
        readerDown.load(calib, BTagEntry::FLAV_B, "comb"); readerDown.load(calib, BTagEntry::FLAV_C, "comb");  readerDown.load(calib, BTagEntry::FLAV_UDSG, "incl");        
    }
    void SetCalibFastSim(std::string cfile)
    {        
        //read CFs  New 2016 Modifications
        calibFast = BTagCalibration("",cfile);
	readerFast = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central");
	readerFast.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFast.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFast.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim");
	readerFastUp = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "up");
	readerFastUp.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFastUp.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFastUp.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim");
	readerFastDown = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "down");
	readerFastDown.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFastDown.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFastDown.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim"); 	
    }
    void SetTreeNames(const NTupleReader& tr)
    {
        //Switch based on which nTuples are used

        //For stop group's nTuples
        if(tr.checkBranch("met"))
        {
            isData = (tr.checkBranch("genDecayPdgIdVec")) ? false : true;
            JetsVec = "jetsLVec";
            BJetsVec = "recoJetsBtag_0";
            JetsFlavor = "recoJetsFlavor";
        }
        //For stealth group's nTuples
        else if(tr.checkBranch("MET"))
        {
            isData = (tr.checkBranch("GenParticles_PdgId")) ? false : true;
            JetsVec = "Jets";
            BJetsVec = "Jets_bDiscriminatorCSV";
            JetsFlavor = "Jets_partonFlavor";
        }
    }
    
    void SetBtagSFunc(int u) { btagSFunc = u; }
    //void SetCtagSFunc(int u) { ctagSFunc = u; }
    void SetCtagSFunc(int u) { btagSFunc = u; } //ctag and btag are correlated
    void SetMistagSFunc(int u) { mistagSFunc = u; }
    void SetBtagCFunc(int u) { btagCFunc = u; }
    void SetCtagCFunc(int u) { ctagCFunc = u; }
    void SetMistagCFunc(int u) { mistagCFunc = u; }

    
    //test Method
    std::vector<float> GetCorrectionsTest(std::vector<TLorentzVector>* Jets, std::vector<int>* Jets_flavor);
    //method 1b
    std::vector<float>* GetCorrections(const std::vector<TLorentzVector> *Jets, const std::vector<int> *Jets_flavor);
    //method 1a
    float GetSimpleCorrection(const std::vector<TLorentzVector> *Jets, const std::vector<int> *Jets_flavor, const std::vector<float> *Jets_bDiscriminatorCSV);

    void InitSFEff(float pt, float eta, int flav, std::vector<float>& sfEffList);

    // To register Event weights/ Probabilities to FlatTuples
    void registerVarToNTuples(NTupleReader& tr);
    //Operator
    void operator()(NTupleReader& tr);

    //member variables
    bool debug, fastsim, isData;
    int btagSFunc, mistagSFunc;
    int btagCFunc, ctagCFunc, mistagCFunc;
    BTagCalibration calib, calibFast;
    BTagCalibrationReader reader, readerUp, readerDown;
    BTagCalibrationReader readerFast, readerFastUp, readerFastDown;
    TH2F *h_eff_b, *h_eff_c, *h_eff_udsg;
    TFile *inFile;
    std::string JetsVec, BJetsVec, JetsFlavor;
};


#endif







