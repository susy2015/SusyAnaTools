#include "samples.h"

#include <iostream>
#include <cstdio>
#include <cstring>

namespace AnaSamples
{
    void FileSummary::readFileList() const
    {
        if(filelist_.size()) filelist_.clear();
        
        FILE *f = fopen(filePath.c_str(), "r");
        char buff[512];
        if(f)
        {
            while(!feof(f) && fgets(buff, 512, f))
            {
                for(char* k = strchr(buff, '\n'); k != 0; k = strchr(buff, '\n')) *k = '\0';
                filelist_.push_back(buff);
            }
            fclose(f);
        }
        else std::cout << "Filelist file \"" << filePath << "\" not found!!!!!!!" << std::endl;
    }

    void FileSummary::addCollection(std::string colName)
    {
        collections_.insert(colName);
    }

    std::map<std::string, FileSummary>& SampleSet::getMap()
    {
        return sampleSet_;
    }
    
    SampleSet::SampleSet(std::string fDir, double lumi) : fDir_(fDir), lumi_(lumi)
    {
        // ---------------
        // - backgrounds -
        // ---------------

        // branching ratio info from PDG
        double W_Lept_BR = 0.1086*3;
        double TTbar_SingleLept_BR = 0.43930872; // 2*W_Lept_BR*(1-W_Lept_BR)
        double TTbar_DiLept_BR = 0.10614564; // W_Lept_BR^2

	std::string MCloc = "Spring16_80X_Jun_2016_Ntp_v5X/";
        std::string MCloc1 = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string MCloc2 = "Spring15_74X_Dec_2015_Ntp_v4X/";
        std::string DATAloc = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string sigMCloc = "Spring15_74X_Dec_2015_Ntp_v4X/";
        std::string addSigMCloc = "Spring15_74X_v5X_top_corridor/";

        if(fDir.compare("condor") == 0)
        {
            fDir_ = "";
            MCloc = "";
            MCloc1 = "";
            MCloc2 = "";
            DATAloc = "";
            sigMCloc = "";
            addSigMCloc = "";
        }

        //TTbar samples
        // TTbarInc has LO xsec on McM : 502.20 pb. The NNLO is 831.76 pb. The k-factor for ttbar is: kt = 831.76/502.20 ~ 1.656233
	 addSample("TTbarInc", fDir_ + MCloc1 + "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76,  lumi, 11344206, 1.0, kGreen);
        // 1.61 * kt 
        addSample("TTbar_HT-600to800", fDir_ + MCloc + "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 2.666535,    lumi, 14305795, 1.0, kGreen);
        // 0.663 * kt
        addSample("TTbar_HT-800to1200", fDir_ + MCloc + "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1.098082,    lumi, 21107332, 1.0, kGreen);
        // 0.12 * kt
        addSample("TTbar_HT-1200to2500", fDir_ + MCloc + "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.198748,    lumi, 2932983, 1.0, kGreen);
        // 0.00143 * kt
        addSample("TTbar_HT-2500toInf", fDir_ + MCloc + "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 0.002368413, lumi, 523618,  1.0, kGreen);


        // Calculated from PDG BRs'. Not from the kt * xSec in McM.
        addSample("TTbarDiLep", fDir_ + MCloc + "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 831.76*TTbar_DiLept_BR,         lumi, 30587326, 1.0, kGreen);
        addSample("TTbarSingleLepT", fDir_ + MCloc + "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 51873969, 1.0, kGreen);
        addSample("TTbarSingleLepTbar", fDir_ + MCloc + "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 59654914, 1.0, kGreen);

        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets, kw = 1.21
        addSample("WJetsToLNu_HT_100to200", fDir_ + MCloc + "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1345,  lumi, 29456028,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_200to400", fDir_ + MCloc + "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 359.7,  lumi, 19966672,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_400to600", fDir_ + MCloc + "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 48.91,  lumi, 7299788,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600to800", fDir_ + MCloc + "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 12.05,  lumi, 3723054,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_800to1200", fDir_ + MCloc + "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  5.501,   lumi, 7480017,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_1200to2500", fDir_ + MCloc + "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.329,   lumi, 7042924,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_2500toInf", fDir_ + MCloc + "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  0.03216, lumi, 252809,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600toInf", fDir_ + MCloc1 + "WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 18.77,  lumi, 1039152,  1.21, kMagenta+1);
        
        //Z -> nunu
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z, kz = 1.23
        addSample("ZJetsToNuNu_HT_100to200", fDir_ + MCloc1 + "ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 280.35, lumi, 5154824, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_400to600", fDir_ + MCloc1 + "ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 10.73,  lumi, 9591908, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_600toInf", fDir_ + MCloc1 + "ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 4.116,  lumi, 10202299, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_200to400", fDir_ + MCloc + "ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 77.67,  lumi, 19634721, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_600to800", fDir_ + MCloc + "ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.853*3,  lumi, 5650700, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_800to1200", fDir_ + MCloc + "ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.394*3,  lumi, 2156175, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_1200to2500", fDir_ + MCloc + "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.0974*3,  lumi, 365216, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_2500toInf", fDir_ + MCloc + "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.00230*3,  lumi, 405752, 1.23,  kTeal+4);
        //DY->ll
        // kz = 1.23
        addSample("DYJetsToLL_HT_100to200", fDir_ + MCloc + "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 147.4, lumi, 8415286, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_200to400", fDir_ + MCloc + "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 40.99, lumi, 8757478,  1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_400to600", fDir_ + MCloc + "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 5.678, lumi, 2893952, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_600toInf", fDir_ + MCloc1 + "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 2.198, lumi, 4864654,  1.23,  kYellow-7);
        // NNLO
        addSample("DYJetsToLL_Inc", fDir_ + MCloc + "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 6025.2, lumi, 58882716, 1.0,  kYellow-7);

        //QCD
        // Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD. But numbers are from McM.
        addSample("QCD_HT100to200", fDir_ + MCloc + "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 27540000, lumi, 71140904, 1.0,  kBlue);
        addSample("QCD_HT200to300", fDir_ + MCloc + "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 1735000, lumi, 55466657, 1.0,  kBlue);
        addSample("QCD_HT300to500", fDir_ + MCloc + "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 366800, lumi, 54666175, 1.0,  kBlue);
        addSample("QCD_HT500to700", fDir_ + MCloc + "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 29370, lumi, 62819508, 1.0,  kBlue);
        addSample("QCD_HT700to1000", fDir_ + MCloc + "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 6524, lumi, 45410383, 1.0,  kBlue);
        addSample("QCD_HT1000to1500", fDir_ + MCloc + "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1064, lumi, 15217446, 1.0,  kBlue);
        addSample("QCD_HT1500to2000", fDir_ + MCloc + "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 121.5, lumi, 11705926, 1.0,  kBlue);
        addSample("QCD_HT2000toInf", fDir_ + MCloc + "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 25.42, lumi, 6040004, 1.0,  kBlue);

        //Other Samples
        // Aprox. NNLO
        addSample("tW_top", fDir_ + MCloc + "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi,  998400,  1.0,  kYellow);
        // Aprox. NNLO
        addSample("tW_antitop", fDir_ + MCloc + "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi, 967600,  1.0,  kYellow);

        // NLO --> negative weights!
        // (sign of gen weight) * (lumi*xsec)/(effective number of events): effective number of events = N(evt) with positive weight - N(evt) with negative weight
        addSample("TTZToLLNuNu", fDir_ + MCloc + "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2529, lumi, 398600,  1.0,  kOrange+2);
        addSample("TTZToQQ", fDir_ + MCloc + "TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",          "stopTreeMaker/AUX", 0.5297, lumi, 749400,  1.0,  kOrange+2);

        // NLO --> negative weights!
        addSample("TTWJetsToLNu", fDir_ + MCloc + "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 0.2043, lumi, 252673,   1.0,  kSpring+2);
        addSample("TTWJetsToQQ", fDir_ + MCloc + "TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",  "stopTreeMaker/AUX", 0.4062, lumi, 738148,  1.0,  kSpring+2);

        // NLO --> negative weights!  
        addSample("TTGJets", fDir_ + MCloc + "TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 3.697, lumi,  4874091,  1.0,  kOrange+2);

        // ttH --> negative weights!
	addSample("ttHJetTobb", fDir_ + MCloc + "ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8.txt",             "stopTreeMaker/AUX", 0.2934,  lumi, 9509111,   1.0,  kOrange+2);
	addSample("ttHJetToNonbb", fDir_ + MCloc + "ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix.txt", "stopTreeMaker/AUX", 0.2151,  lumi, 9829539,  1.0,  kOrange+2);
        
        // Di-boson
	// Ref. https://indico.cern.ch/event/439995/session/0/contribution/6/attachments/1143460/1638648/diboson_final.pdf (NNLO is given)
        addSample("WW", fDir_ + MCloc + "WW_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 115.0,  lumi, 993214,  1.0,  kViolet+4); 
	// Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns (NLO from MCFM)
        addSample("WZ", fDir_ + MCloc1 + "WZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 47.13,  lumi, 978512,  1.0,  kViolet+4);
        addSample("ZZ", fDir_ + MCloc + "ZZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 16.523, lumi, 989312,  1.0,  kViolet+4);

        // Tri-boson: negative weights!
        addSample("WWZ", fDir_ + MCloc + "WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.1651,  lumi, 249200,  1.0,  kViolet+2);
        addSample("WZZ", fDir_ + MCloc + "WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.05565, lumi, 249800,  1.0,  kViolet+2);
        addSample("ZZZ", fDir_ + MCloc + "ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.01398, lumi, 250000,  1.0,  kViolet+2);

        // --------
        // - data -
        // --------
	//Single Muon
	addSample("Data_SingleMuon_2016", fDir_ + MCloc + "SingleMuon.txt","stopTreeMaker/AUX", 806, 1.0,  kBlack);
	//old
	addSample("Data_SingleMuon_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
	addSample("Data_SingleMuon_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
	addSample("Data_SingleMuon_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.38, 1.0,  kBlack);

	//Single Electron
//	addSample("Data_SingleElectron_2015C", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_SingleElectron-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 16.345, 1.0,  kBlack);
//	addSample("Data_SingleElectron_2015D_05Oct2015", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v 3p1_SingleElectron-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 552.67, 1.0,  kBlack);
//	addSample("Data_SingleElectron_2015D_PromptReco", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_SingleElectron-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 993.25, 1.0,  kBlack);

//	addSample("Data_DoubleMuon_2015C", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 16.345, 1.0,  kBlack);
//	addSample("Data_DoubleMuon_2015D_05Oct2015", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 552.67, 1.0,  kBlack);
//	addSample("Data_DoubleMuon_2015D_PromptReco", fDir_ + MCloc + "Spring15_74X_Nov_2015_Ntp_v3p1_DoubleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 993.72, 1.0,  kBlack);

//	addSample("Data_DoubleEG_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
//	addSample("Data_DoubleEG_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
//	addSample("Data_DoubleEG_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1669.92, 1.0,  kBlack);
	
	//Single Electron
	// new 2016
	addSample("Data_SingleElectron_2016", fDir_ + MCloc + "SingleElectron.txt","stopTreeMaker/AUX", 816.27, 1.0,  kBlack);
	
	//HT-MHT
	addSample("Data_HTMHT_2016", fDir_ + MCloc + "HTMHT.txt","stopTreeMaker/AUX", 816.27, 1.0,  kBlack);
	//old
	addSample("Data_HTMHT_2015C", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
	addSample("Data_HTMHT_2015D_05Oct2015", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 574.23, 1.0,  kBlack);
	addSample("Data_HTMHT_2015D_PromptReco", fDir_ + DATAloc + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.53, 1.0,  kBlack);

        // ----------
	// - signal -
        // ----------

	// To be updated - no T2tt, T2bb Spring15 samples yet (update later)!
        addSample("Signal_T1tttt_mGluino1200_mLSP800", fDir_ + MCloc + "SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 146849, 1.0,  kRed);
        addSample("Signal_T1tttt_mGluino1500_mLSP100", fDir_ + MCloc + "SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105415, 1.0,  kRed);
//        addSample("Signal_T5tttt_mGluino1300_mStop300_mChi280", fDir_ + sigMCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 44011,  1.0,  kRed);
//        addSample("Signal_T5tttt_mGluino1300_mStop300_mCh285", fDir_ + sigMCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 43818,  1.0,  kRed);
        addSample("Signal_T1bbbb_mGluino1000_mLSP900", fDir_ + MCloc + "SMS-T1bbbb_mGluino-1000_mLSP-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.325388,  lumi, 142671,  1.0,  kRed);
        addSample("Signal_T1bbbb_mGluino1500_mLSP100", fDir_ + MCloc + "SMS-T1bbbb_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 52465, 1.0,  kRed);
//        addSample("Signal_T2tt_mStop425_mLSP325", fDir_ + sigMCloc + "T2tt_2J_mStop-425_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030,1.0,  kRed);
        addSample("Signal_T2tt_mStop500_mLSP325", fDir_ + MCloc + "SMS-T2tt_mStop-500_mLSP-325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.51848, lumi, 388225, 1.0,  kRed);
//        addSample("Signal_T2tt_mStop650_mLSP325", fDir_ + sigMCloc + "T2tt_2J_mStop-650_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.107045,  lumi, 105672, 1.0,  kRed);
        addSample("Signal_T2tt_mStop850_mLSP100", fDir_ + MCloc + "SMS-T2tt_mStop-850_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 227907, 1.0,  kRed);
//        addSample("Signal_T2bb_mSbottom600_mLSP580", fDir_ + sigMCloc + "T2bb_2J_mStop-600_mLSP-580_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.174599,  lumi, 107316, 1.0,  kRed);
//        addSample("Signal_T2bb_mSbottom900_mLSP100", fDir_ + sigMCloc + "T2bb_2J_mStop-900_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0128895, lumi, 102661, 1.0,  kRed);    
//        addSample("Signal_TTDMDMJets_M600GeV", fDir_ + sigMCloc + "TTDMDMJets_M600GeV.txt", "stopTreeMaker/AUX", 0.1038, lumi, 126547, 1.0,  kRed);    
//        addSample("Signal_TTDMDMJets_M1000GeV", fDir_ + sigMCloc + "TTDMDMJets_M1000GeV.txt", "stopTreeMaker/AUX", 0.01585, lumi, 121817, 1.0,  kRed);    
// Fastsim signal scans
        addSample("SMS-T2tt_mStop-100-125_mLSP-1to50", fDir_ + sigMCloc + "SMS-T2tt_mStop-100-125_mLSP-1to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-150-175_mLSP-1to100", fDir_ + sigMCloc + "SMS-T2tt_mStop-150-175_mLSP-1to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-200_mLSP-1to125", fDir_ + sigMCloc + "SMS-T2tt_mStop-200_mLSP-1to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-225_mLSP-25to150", fDir_ + sigMCloc + "SMS-T2tt_mStop-225_mLSP-25to150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-250_mLSP-1to175", fDir_ + sigMCloc + "SMS-T2tt_mStop-250_mLSP-1to175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-275_mLSP-75to200", fDir_ + sigMCloc + "SMS-T2tt_mStop-275_mLSP-75to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-300to375_mLSP-1to300", fDir_ + sigMCloc + "SMS-T2tt_mStop-300to375_mLSP-1to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-400to475_mLSP-1to400", fDir_ + sigMCloc + "SMS-T2tt_mStop-400to475_mLSP-1to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475", fDir_ + sigMCloc + "SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-600-950_mLSP-1to450", fDir_ + sigMCloc + "SMS-T2tt_mStop-600-950_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T2tb
        addSample("SMS-T2tb_mStop-200to325_0to150", fDir_ + sigMCloc + "SMS-T2tb_mStop-200to325_0to150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-200to625_50to475", fDir_ + sigMCloc + "SMS-T2tb_mStop-200to625_50to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-350to400_0to225", fDir_ + sigMCloc + "SMS-T2tb_mStop-350to400_0to225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-425to600_0to425", fDir_ + sigMCloc + "SMS-T2tb_mStop-425to600_0to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-625to850_0to450", fDir_ + sigMCloc + "SMS-T2tb_mStop-625to850_0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tb_mStop-875to1125_0to475", fDir_ + sigMCloc + "SMS-T2tb_mStop-875to1125_0to475_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T1tttt
        addSample("SMS-T1tttt_mGluino-600to625_mLSP-250to375", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600to625_mLSP-250to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-600_mLSP-1to225", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600_mLSP-1to225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-600_mLSP-250to325", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-600_mLSP-250to325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-625to650_mLSP-200to400", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-625to650_mLSP-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-625_mLSP-275to375", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-625_mLSP-275to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-650to675_mLSP-250to425", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-650to675_mLSP-250to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-675_mLSP-325to450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-675_mLSP-325to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-700to750_mLSP-200to500", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-700to750_mLSP-200to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-700_mLSP-1to450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-700_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-750to775_mLSP-350to525", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-750to775_mLSP-350to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-775_mLSP-475to550", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-775_mLSP-475to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-800to825_mLSP-1to575", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-800to825_mLSP-1to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-825to850_mLSP-200to600", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-825to850_mLSP-200to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-850to875_mLSP-450to625", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-850to875_mLSP-450to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-875to900_mLSP-1to650", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-875to900_mLSP-1to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-900to950_mLSP-200to700", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-900to950_mLSP-200to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-950to975_mLSP-350to725", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-950to975_mLSP-350to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-975_mLSP-600to750", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-975_mLSP-600to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1000to1050_mLSP-1to800", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1000to1050_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1000_mLSP-1to700", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1000_mLSP-1to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1050to1075_mLSP-650to850", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1050to1075_mLSP-650to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1050_mLSP-50to775", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1050_mLSP-50to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1100to1125_mLSP-700to900", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1100to1125_mLSP-700to900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1100_mLSP-1to775", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1100_mLSP-1to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1150to1175_mLSP-750to925", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1150to1175_mLSP-750to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1150_mLSP-1to800", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1150_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1175_mLSP-950", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1175_mLSP-950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1200_mLSP-1to825", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1200_mLSP-1to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1275_mLSP-900to975", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1275_mLSP-900to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1300_mLSP-1to1075", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1300_mLSP-1to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1375_mLSP-950to1150", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1375_mLSP-950to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1400_mLSP-1to1175", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1400_mLSP-1to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1750_mLSP-50to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1750_mLSP-50to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1tttt_mGluino-1950_mLSP-700to950", fDir_ + sigMCloc + "SMS-T1tttt_mGluino-1950_mLSP-700to950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T1ttbb
        addSample("SMS-T1ttbb_mGl-600to800_mLSP-0to450", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-600to800_mLSP-0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-600to800_mLSP-275to575", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-600to800_mLSP-275to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-825to1000_mLSP-0to650", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-825to1000_mLSP-0to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-825to1000_mLSP-500to775", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-825to1000_mLSP-500to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1025to1200_mLSP-0to850", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1025to1200_mLSP-0to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1025to1200_mLSP-700to975", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1025to1200_mLSP-700to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1225to1400_mLSP-0to1050", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1225to1400_mLSP-0to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1225to1400_mLSP-900to1175", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1225to1400_mLSP-900to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1425to1600_mLSP-0to1250", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1425to1600_mLSP-0to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1425to1600_mLSP-1100to1350", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1425to1600_mLSP-1100to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1650to1750_mLSP-1350to1450", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1650to1750_mLSP-1350to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T1ttbb_mGl-1650to2000_mLSP-0to1450", fDir_ + sigMCloc + "SMS-T1ttbb_mGl-1650to2000_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T5ttcc
        addSample("SMS-T5ttcc_mGl-600to800_mLSP-0to625", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-600to800_mLSP-0to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-600to800_mLSP-450to675", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-600to800_mLSP-450to675_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-825to1000_mLSP-0to825", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-825to1000_mLSP-0to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-825to1000_mLSP-675to875", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-825to1000_mLSP-675to875_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1025to1200_mLSP-0to1025", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1025to1200_mLSP-0to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1025to1200_mLSP-875to1075", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1025to1200_mLSP-875to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1225to1400_mLSP-0to1225", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1225to1400_mLSP-0to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1225to1400_mLSP-1075to1225", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1225to1400_mLSP-1075to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1425to1525_mLSP-1275to1375", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1425to1525_mLSP-1275to1375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1425to1600_mLSP-0to1350", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1425to1600_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttcc_mGl-1650to1700_mLSP-0to1350", fDir_ + sigMCloc + "SMS-T5ttcc_mGl-1650to1700_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T5ttttDM175
        addSample("SMS-T5ttttDM175_mGl-600to800_mLSP-0to450", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-600to800_mLSP-0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-600to800_mLSP-275to525", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-600to800_mLSP-275to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-825to1000_mLSP-0to650", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-825to1000_mLSP-0to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-825to1000_mLSP-500to725", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-825to1000_mLSP-500to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1025to1200_mLSP-0to850", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1025to1200_mLSP-0to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1025to1200_mLSP-700to925", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1025to1200_mLSP-700to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1225to1400_mLSP-0to1050", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1225to1400_mLSP-0to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1225to1400_mLSP-900to1125", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1225to1400_mLSP-900to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1425to1600_mLSP-0to1250", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1425to1600_mLSP-0to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1425to1625_mLSP-1100to1300", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1425to1625_mLSP-1100to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5ttttDM175_mGl-1625to1700_mLSP-0to1300", fDir_ + sigMCloc + "SMS-T5ttttDM175_mGl-1625to1700_mLSP-0to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T5tttt_degen
        addSample("SMS-T5tttt_degen_mGl-600to800_mLSP-0to625", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-600to800_mLSP-0to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-600to800_mLSP-450to675", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-600to800_mLSP-450to675_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-825to1000_mLSP-0to825", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-825to1000_mLSP-0to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-825to1000_mLSP-675to875", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-825to1000_mLSP-675to875_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1025to1200_mLSP-0to1025", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1025to1200_mLSP-0to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1025to1200_mLSP-875to1075", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1025to1200_mLSP-875to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1225to1400_mLSP-0to1225", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1225to1400_mLSP-0to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1225to1400_mLSP-1075to1275", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1225to1400_mLSP-1075to1275_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1425to1600_mLSP-0to1350", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1425to1600_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1425to1600_mLSP-1275to1375", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1425to1600_mLSP-1275to1375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T5tttt_degen_mGl-1650to1700_mLSP-0to1350", fDir_ + sigMCloc + "SMS-T5tttt_degen_mGl-1650to1700_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T6ttWW
        addSample("SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-75to125", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-75to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-150to500", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-150to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-225to575", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-225to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-350to600_mChargino-150to400", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-350to600_mChargino-150to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-75to125", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-75to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-500to850", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-500to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-550to875", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-550to875_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T6ttWW_mLSP50_mSbottom-650to950_mChargino-150to750", fDir_ + sigMCloc + "SMS-T6ttWW_mLSP50_mSbottom-650to950_mChargino-150to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// Additional samples for top corridor studies
        addSample("TTbar_fastsim_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "TTbar_Fastsim_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 831.76,  lumi, 10970095, 1.0, kGreen);
        addSample("TTbar_fullsim_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "TTbar_Fullsim_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 831.76,  lumi, 11344206, 1.0, kGreen);

// Additional signals
        addSample("SMS-T2tt_mStop-100-125_mLSP-1to50_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-100-125_mLSP-1to50_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-150-175_mLSP-1to100_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-150-175_mLSP-1to100_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-200_mLSP-1to125_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-200_mLSP-1to125_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-225_mLSP-25to150_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-225_mLSP-25to150_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-250_mLSP-1to175_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-250_mLSP-1to175_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-275_mLSP-75to200_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-275_mLSP-75to200_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-300to375_mLSP-1to300_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-300to375_mLSP-1to300_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-400to475_mLSP-1to400_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-400to475_mLSP-1to400_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-600-950_mLSP-1to450_wt_genJets_wt_genMET", fDir_ + addSigMCloc + "condor_SMS-T2tt_mStop-600-950_mLSP-1to450_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

        addSample("Signal_fullsim_T2tt_mStop175_mLSP0", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-175_mLSP-0_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 121.416, lumi, 1360170, 1.0,  kRed);
        addSample("Signal_fullsim_T2tt_mStop200_mLSP25", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-200_mLSP-25_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 64.5085, lumi, 1479747, 1.0,  kRed);
        addSample("Signal_fullsim_T2tt_mStop225_mLSP50", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-225_mLSP-50_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 36.3818, lumi, 740397, 1.0,  kRed);
        addSample("Signal_fullsim_T2tt_mStop250_mLSP75", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-250_mLSP-75_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 21.5949, lumi, 1416350, 1.0,  kRed);
        addSample("Signal_fullsim_T2tt_mStop275_mLSP100", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-275_mLSP-100_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 13.3231, lumi, 801385, 1.0,  kRed);
        addSample("Signal_fullsim_T2tt_mStop300_mLSP125", fDir_ + addSigMCloc + "SMS-T2tt_fullsim_mStop-300_mLSP-125_wt_genJets_wt_genMET.txt", "stopTreeMaker/AUX", 8.51615, lumi, 531262, 1.0,  kRed);
    }

    SampleCollection::SampleCollection(SampleSet& samples)
    {
        //Define sets of samples for later use
        addSampleSet(samples, "TTbar", {"TTbarInc"});
        addSampleSet(samples, "TTbarSingleLep", {"TTbarSingleLepT", "TTbarSingleLepTbar"});
        addSampleSet(samples, "TTbarDiLep", {"TTbarDiLep"});
        addSampleSet(samples, "TTbarHT", {"TTbar_HT-600to800", "TTbar_HT-800to1200", "TTbar_HT-1200to2500", "TTbar_HT-2500toInf"});
        addSampleSet(samples, "TTbarNoHad", {"TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"});

        // Only all had. part of TTbarInc
        addSampleSet(samples, "TTbarAll", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"});

        // Only all had. part of TTbarInc & HT cuts on inclusive samples
        addSampleSet(samples, "TTbarExt", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep", "TTbar_HT-600to800", "TTbar_HT-800to1200", "TTbar_HT-1200to2500", "TTbar_HT-2500toInf"});

//        addSampleSet(samples, "WJetsToLNu_LESS", {"WJetsToLNu_HT_600toInf", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});
        addSampleSet(samples, "WJetsToLNu", {"WJetsToLNu_HT_2500toInf", "WJetsToLNu_HT_1200to2500", "WJetsToLNu_HT_800to1200", "WJetsToLNu_HT_600to800", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});

        addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_2500toInf", "ZJetsToNuNu_HT_1200to2500", "ZJetsToNuNu_HT_800to1200", "ZJetsToNuNu_HT_600to800", "ZJetsToNuNu_HT_200to400"});
//	 addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_600toInf", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200"});
        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
//        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
        addSampleSet(samples, "IncDY", {"DYJetsToLL_Inc"});

        addSampleSet(samples, "QCD", {"QCD_HT2000toInf", "QCD_HT1500to2000", "QCD_HT1000to1500", "QCD_HT700to1000", "QCD_HT500to700", "QCD_HT300to500", "QCD_HT200to300", "QCD_HT100to200"});

        addSampleSet(samples, "tW", {"tW_top", "tW_antitop"});

        addSampleSet(samples, "TTZ", {"TTZToLLNuNu", "TTZToQQ"});
        addSampleSet(samples, "TTW", {"TTWJetsToLNu", "TTWJetsToQQ"});

        addSampleSet(samples, "TTG", {"TTGJets"});

//	addSampleSet(samples, "ttHJetTobb", {"ttHJetTobb"});
//	addSampleSet(samples, "ttHJetToNonbb", {"ttHJetToNonbb"});
	addSampleSet(samples, "ttH", {"ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "WWZ", {"WWZ"});
        addSampleSet(samples, "WZZ", {"WZZ"});
        addSampleSet(samples, "ZZZ", {"ZZZ"});

        addSampleSet(samples, "Diboson", {"WW", "ZZ"});
//      addSampleSet(samples, "Diboson", {"WW", "WZ", "ZZ"});

        addSampleSet(samples, "Triboson", {"WWZ", "WZZ", "ZZZ"});

	addSampleSet(samples, "Rare", {"TTWJetsToLNu", "TTWJetsToQQ", "TTGJets", "WWZ", "WZZ", "ZZZ", "ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "Data_SingleMuon", {"Data_SingleMuon_2016"});

	addSampleSet(samples, "Data_SingleElectron", {"Data_SingleElectron_2016"});

//	addSampleSet(samples, "Data_SingleMuon", {"Data_SingleMuon_2015C", "Data_SingleMuon_2015D_05Oct2015", "Data_SingleMuon_2015D_PromptReco"});

        //addSampleSet(samples, "Data_SingleElectron", {"Data_SingleElectron_2015C", "Data_SingleElectron_2015D_05Oct2015", "Data_SingleElectron_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleMuon", {"Data_DoubleMuon_2015C", "Data_DoubleMuon_2015D_05Oct2015", "Data_DoubleMuon_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleEG", {"Data_DoubleEG_2015C", "Data_DoubleEG_2015D_05Oct2015", "Data_DoubleEG_2015D_PromptReco"});

        addSampleSet(samples, "Data_HTMHT", {"Data_HTMHT_2016"});
//      addSampleSet(samples, "Data_HTMHT", {"Data_HTMHT_2015C", "Data_HTMHT_2015D_05Oct2015", "Data_HTMHT_2015D_PromptReco"});

        addSampleSet(samples, "Signal_T1tttt_mGluino1200_mLSP800", {"Signal_T1tttt_mGluino1200_mLSP800"});
        addSampleSet(samples, "Signal_T1tttt_mGluino1500_mLSP100", {"Signal_T1tttt_mGluino1500_mLSP100"});
        addSampleSet(samples, "Signal_T2tt_mStop500_mLSP325", {"Signal_T2tt_mStop500_mLSP325"});
        addSampleSet(samples, "Signal_T2tt_mStop850_mLSP100", {"Signal_T2tt_mStop850_mLSP100"});

        addSampleSet(samples, "Signal_fastsim_T2tt_scan", {"SMS-T2tt_mStop-100-125_mLSP-1to50", "SMS-T2tt_mStop-150-175_mLSP-1to100", "SMS-T2tt_mStop-200_mLSP-1to125", "SMS-T2tt_mStop-225_mLSP-25to150", "SMS-T2tt_mStop-250_mLSP-1to175", "SMS-T2tt_mStop-275_mLSP-75to200", "SMS-T2tt_mStop-300to375_mLSP-1to300", "SMS-T2tt_mStop-400to475_mLSP-1to400", "SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475", "SMS-T2tt_mStop-600-950_mLSP-1to450"});

        addSampleSet(samples, "Signal_fastsim_T2tb_scan", {"SMS-T2tb_mStop-350to400_0to225", "SMS-T2tb_mStop-425to600_0to425", "SMS-T2tb_mStop-625to850_0to450", "SMS-T2tb_mStop-875to1125_0to475"});
        addSampleSet(samples, "Signal_fastsim_T1tttt_scan", {"SMS-T1tttt_mGluino-600to625_mLSP-250to375", "SMS-T1tttt_mGluino-600_mLSP-1to225", "SMS-T1tttt_mGluino-600_mLSP-250to325", "SMS-T1tttt_mGluino-625to650_mLSP-200to400", "SMS-T1tttt_mGluino-625_mLSP-275to375", "SMS-T1tttt_mGluino-650to675_mLSP-250to425", "SMS-T1tttt_mGluino-675_mLSP-325to450", "SMS-T1tttt_mGluino-700to750_mLSP-200to500", "SMS-T1tttt_mGluino-700_mLSP-1to450", "SMS-T1tttt_mGluino-750to775_mLSP-350to525", "SMS-T1tttt_mGluino-775_mLSP-475to550", "SMS-T1tttt_mGluino-800to825_mLSP-1to575", "SMS-T1tttt_mGluino-825to850_mLSP-200to600", "SMS-T1tttt_mGluino-850to875_mLSP-450to625", "SMS-T1tttt_mGluino-875to900_mLSP-1to650", "SMS-T1tttt_mGluino-900to950_mLSP-200to700", "SMS-T1tttt_mGluino-950to975_mLSP-350to725", "SMS-T1tttt_mGluino-975_mLSP-600to750", "SMS-T1tttt_mGluino-1000to1050_mLSP-1to800", "SMS-T1tttt_mGluino-1000_mLSP-1to700", "SMS-T1tttt_mGluino-1050to1075_mLSP-650to850", "SMS-T1tttt_mGluino-1050_mLSP-50to775", "SMS-T1tttt_mGluino-1100to1125_mLSP-700to900", "SMS-T1tttt_mGluino-1100_mLSP-1to775", "SMS-T1tttt_mGluino-1150to1175_mLSP-750to925", "SMS-T1tttt_mGluino-1150_mLSP-1to800", "SMS-T1tttt_mGluino-1175_mLSP-950", "SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000", "SMS-T1tttt_mGluino-1200_mLSP-1to825", "SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025", "SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050", "SMS-T1tttt_mGluino-1275_mLSP-900to975", "SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100", "SMS-T1tttt_mGluino-1300_mLSP-1to1075", "SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125", "SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025", "SMS-T1tttt_mGluino-1375_mLSP-950to1150", "SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100", "SMS-T1tttt_mGluino-1400_mLSP-1to1175", "SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200", "SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075", "SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250", "SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125", "SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300", "SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175", "SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350", "SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400", "SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450", "SMS-T1tttt_mGluino-1750_mLSP-50to1450", "SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450", "SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450", "SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450", "SMS-T1tttt_mGluino-1950_mLSP-700to950"});

        addSampleSet(samples, "Signal_fastsim_T1ttbb_scan", {"SMS-T1ttbb_mGl-600to800_mLSP-0to450", "SMS-T1ttbb_mGl-600to800_mLSP-275to575", "SMS-T1ttbb_mGl-825to1000_mLSP-0to650", "SMS-T1ttbb_mGl-825to1000_mLSP-500to775", "SMS-T1ttbb_mGl-1025to1200_mLSP-0to850", "SMS-T1ttbb_mGl-1025to1200_mLSP-700to975", "SMS-T1ttbb_mGl-1225to1400_mLSP-0to1050", "SMS-T1ttbb_mGl-1225to1400_mLSP-900to1175", "SMS-T1ttbb_mGl-1425to1600_mLSP-0to1250", "SMS-T1ttbb_mGl-1425to1600_mLSP-1100to1350", "SMS-T1ttbb_mGl-1650to1750_mLSP-1350to1450", "SMS-T1ttbb_mGl-1650to2000_mLSP-0to1450"});

        addSampleSet(samples, "Signal_fastsim_T5ttcc_scan", {"SMS-T5ttcc_mGl-600to800_mLSP-0to625", "SMS-T5ttcc_mGl-600to800_mLSP-450to675", "SMS-T5ttcc_mGl-825to1000_mLSP-0to825", "SMS-T5ttcc_mGl-825to1000_mLSP-675to875", "SMS-T5ttcc_mGl-1025to1200_mLSP-0to1025", "SMS-T5ttcc_mGl-1025to1200_mLSP-875to1075", "SMS-T5ttcc_mGl-1225to1400_mLSP-0to1225", "SMS-T5ttcc_mGl-1225to1400_mLSP-1075to1225", "SMS-T5ttcc_mGl-1425to1525_mLSP-1275to1375", "SMS-T5ttcc_mGl-1425to1600_mLSP-0to1350", "SMS-T5ttcc_mGl-1650to1700_mLSP-0to1350"});
        addSampleSet(samples, "Signal_fastsim_T5ttttDM175_scan", {"SMS-T5ttttDM175_mGl-600to800_mLSP-0to450", "SMS-T5ttttDM175_mGl-600to800_mLSP-275to525", "SMS-T5ttttDM175_mGl-825to1000_mLSP-0to650", "SMS-T5ttttDM175_mGl-825to1000_mLSP-500to725", "SMS-T5ttttDM175_mGl-1025to1200_mLSP-0to850", "SMS-T5ttttDM175_mGl-1025to1200_mLSP-700to925", "SMS-T5ttttDM175_mGl-1225to1400_mLSP-0to1050", "SMS-T5ttttDM175_mGl-1225to1400_mLSP-900to1125", "SMS-T5ttttDM175_mGl-1425to1600_mLSP-0to1250", "SMS-T5ttttDM175_mGl-1425to1625_mLSP-1100to1300", "SMS-T5ttttDM175_mGl-1625to1700_mLSP-0to1300"});
        addSampleSet(samples, "Signal_fastsim_T5tttt_degen_scan", {"SMS-T5tttt_degen_mGl-600to800_mLSP-0to625", "SMS-T5tttt_degen_mGl-600to800_mLSP-450to675", "SMS-T5tttt_degen_mGl-825to1000_mLSP-0to825", "SMS-T5tttt_degen_mGl-825to1000_mLSP-675to875", "SMS-T5tttt_degen_mGl-1025to1200_mLSP-0to1025", "SMS-T5tttt_degen_mGl-1025to1200_mLSP-875to1075", "SMS-T5tttt_degen_mGl-1225to1400_mLSP-0to1225", "SMS-T5tttt_degen_mGl-1225to1400_mLSP-1075to1275", "SMS-T5tttt_degen_mGl-1425to1600_mLSP-0to1350", "SMS-T5tttt_degen_mGl-1425to1600_mLSP-1275to1375", "SMS-T5tttt_degen_mGl-1650to1700_mLSP-0to1350"});

        addSampleSet(samples, "Signal_fastsim_T6ttWW_scan", {"SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-75to125", "SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-150to500", "SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-225to575", "SMS-T6ttWW_mLSP50_mSbottom-350to600_mChargino-150to400", "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-75to125", "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-500to850", "SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-550to875", "SMS-T6ttWW_mLSP50_mSbottom-650to950_mChargino-150to750"});

        addSampleSet(samples, "TTbar_fastsim_wt_genJets_wt_genMET", {"TTbar_fastsim_wt_genJets_wt_genMET"});
        addSampleSet(samples, "TTbar_fullsim_wt_genJets_wt_genMET", {"TTbar_fullsim_wt_genJets_wt_genMET"});

        addSampleSet(samples, "Signal_fastsim_T2tt_wt_genJets_wt_genMET_scan", {"SMS-T2tt_mStop-100-125_mLSP-1to50_wt_genJets_wt_genMET", "SMS-T2tt_mStop-150-175_mLSP-1to100_wt_genJets_wt_genMET", "SMS-T2tt_mStop-200_mLSP-1to125_wt_genJets_wt_genMET", "SMS-T2tt_mStop-225_mLSP-25to150_wt_genJets_wt_genMET", "SMS-T2tt_mStop-250_mLSP-1to175_wt_genJets_wt_genMET", "SMS-T2tt_mStop-275_mLSP-75to200_wt_genJets_wt_genMET", "SMS-T2tt_mStop-300to375_mLSP-1to300_wt_genJets_wt_genMET", "SMS-T2tt_mStop-400to475_mLSP-1to400_wt_genJets_wt_genMET", "SMS-T2tt_mStop-500-525-550_mLSP-1to425-325to450-1to475_wt_genJets_wt_genMET", "SMS-T2tt_mStop-600-950_mLSP-1to450_wt_genJets_wt_genMET"});

        addSampleSet(samples, "Signal_fullsim_T2tt_mStop175_mLSP0", {"Signal_fullsim_T2tt_mStop175_mLSP0"});
        addSampleSet(samples, "Signal_fullsim_T2tt_mStop200_mLSP25", {"Signal_fullsim_T2tt_mStop200_mLSP25"});
        addSampleSet(samples, "Signal_fullsim_T2tt_mStop225_mLSP50", {"Signal_fullsim_T2tt_mStop225_mLSP50"});
        addSampleSet(samples, "Signal_fullsim_T2tt_mStop250_mLSP75", {"Signal_fullsim_T2tt_mStop250_mLSP75"});
        addSampleSet(samples, "Signal_fullsim_T2tt_mStop275_mLSP100", {"Signal_fullsim_T2tt_mStop275_mLSP100"});
        addSampleSet(samples, "Signal_fullsim_T2tt_mStop300_mLSP125", {"Signal_fullsim_T2tt_mStop300_mLSP125"});
    }

    void SampleCollection::addSampleSet(SampleSet& samples, std::string name, std::vector<std::string> vss)
    {
        if(vss.size() > 1)
        {
            for(std::string& sn : vss)
            {
                if(sn.compare(name) == 0)
                {
                    std::cout << "You have named a sampleCollection the same as one of its member sampleSets, but it has more than one sampleSet!!!! This is bad!!!  Stop!!! Stop now!!!  This collection will be skipped until it is properly named." << std::endl;
                    return;
                }
            }
        }

        auto& map = samples.getMap();

        for(std::string& sn : vss)
        {
            map[sn].addCollection(name);
            sampleSet_[name].push_back(samples[sn]);
            nameVec_[name].push_back(sn);
            totalLumiMap_[name] += samples[sn].lumi;
        }
    }

    std::vector<std::string>& SampleCollection::getSampleLabels(std::string name)
    {
        return nameVec_[name];
    }

    bool operator< (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath < rhs.filePath || lhs.treePath < rhs.treePath;
    }

    bool operator== (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath == rhs.filePath && lhs.treePath == rhs.treePath && lhs.xsec == rhs.xsec && lhs.lumi == rhs.lumi && lhs.kfactor == rhs.kfactor && lhs.nEvts == rhs.nEvts;
    }

    bool operator!= (const FileSummary& lhs, const FileSummary& rhs)
    {
        return !(lhs == rhs);
    }
}
