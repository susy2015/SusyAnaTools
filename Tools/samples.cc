#include "samples.h"

#include <iostream>
#include <cstdio>
#include <cstring>

namespace AnaSamples
{
    void FileSummary::readFileList()
    {
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

    SampleSet::SampleSet(std::string fDir, double lumi) : fDir_(fDir), lumi_(lumi)
    {
        // ---------------
        // - backgrounds -
        // ---------------

        // branching ratio info from PDG
        double W_Lept_BR = 0.1086*3;
        double TTbar_SingleLept_BR = 0.43930872; // 2*W_Lept_BR*(1-W_Lept_BR)
        double TTbar_DiLept_BR = 0.10614564; // W_Lept_BR^2

        std::string MCloc = "Spring15_74X_Oct_2015_Ntp_v2X/";

        //TTbar samples
        // TTbarInc has LO xsec on McM : 502.20 pb. The NNLO is 831.76 pb. The k-factor for ttbar is: kt = 831.76/502.20 ~ 1.656233
        sampleSet_["TTbarInc"]  = FileSummary(fDir_ + MCloc + "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76,  lumi, 11339232, 1.0, kGreen);
        // 1.61 * kt 
        sampleSet_["TTbar_HT-600to800"]  = FileSummary(fDir_ + MCloc + "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",     "stopTreeMaker/AUX", 2.666535,  lumi, 4964914, 1.0, kGreen);
        // 0.663 * kt
        sampleSet_["TTbar_HT-800to1200"]  = FileSummary(fDir_ + MCloc + "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 1.098082,  lumi, 3445059, 1.0, kGreen);
        // 0.12 * kt
        sampleSet_["TTbar_HT-1200to2500"]  = FileSummary(fDir_ + MCloc + "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.198748,  lumi, 987054, 1.0, kGreen);
        // 0.00143 * kt
        sampleSet_["TTbar_HT-2500toInf"]  = FileSummary(fDir_ + MCloc + "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 0.002368413,  lumi, 507842, 1.0, kGreen);

        // Calculated from PDG BRs'. Not from the kt * xSec in McM.
        sampleSet_["TTbarDiLep"]  = FileSummary(fDir_ + MCloc + "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*TTbar_DiLept_BR,  lumi, 30245565, 1.0, kGreen);
        sampleSet_["TTbarSingleLepT"]  = FileSummary(fDir_ + MCloc + "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR,  lumi, 58191088, 1.0, kGreen);
        sampleSet_["TTbarSingleLepTbar"]  = FileSummary(fDir_ + MCloc + "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR,  lumi, 60166355, 1.0, kGreen);

	// WJets to be updated
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets, kw = 1.21
        sampleSet_["WJetsToLNu_HT_100to200"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1629.9,  lumi, 10142187,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_200to400"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 435.6,  lumi, 5231856,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_400to600"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 59.17,  lumi, 1901705,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_600to800"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 15.49,  lumi, 3984529,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_800to1200"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 6.3646,  lumi, 1574633,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_1200to2500"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.6093,  lumi, 180546,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_2500toInf"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0373769,  lumi, 253036,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_600toInf"]  = FileSummary(fDir_ + MCloc + "WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 24.079,  lumi, 1036108,  1.0, kMagenta+1);
        
        //Z -> nunu
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z, kz = 1.23
        sampleSet_["ZJetsToNuNu_HT_600toInf"] = FileSummary(fDir_ + MCloc + "addJetsForZinv/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 5.166,  lumi, 1015904, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_400to600"] = FileSummary(fDir_ + MCloc + "addJetsForZinv/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 13.456, lumi, 1014139, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_200to400"] = FileSummary(fDir_ + MCloc + "addJetsForZinv/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 96.38,  lumi, 4783914, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_100to200"] = FileSummary(fDir_ + MCloc + "addJetsForZinv/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 344.98, lumi, 5148193, 1.0,  kTeal+4);

        //DY->ll
        // kz = 1.23
        sampleSet_["DYJetsToLL_HT_600toInf"]  = FileSummary(fDir_ + MCloc + "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 2.72,  lumi, 987977, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_400to600"]  = FileSummary(fDir_ + MCloc + "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 6.761,  lumi, 1048047, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_200to400"]  = FileSummary(fDir_ + MCloc + "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 52.58,  lumi, 955972, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_100to200"]  = FileSummary(fDir_ + MCloc + "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 171.5,  lumi, 2171083, 1.0,  kYellow-7);

        // NNLO
//        sampleSet_["IncDYSampleSet"]          = FileSummary(fDir_ + MCloc + "DYJetsToLL_M-50_13TeV-madgraph-pythia8.txt", "stopTreeMaker/AUX", 6025.2,  lumi, 2829164, 1.0,  kYellow-4);

        //QCD
        // Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD. But numbers are from McM.
        sampleSet_["QCD_HT100to200"]    = FileSummary(fDir_ + MCloc + "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 27540000, lumi, 80093092, 1.0,  kBlue);
        sampleSet_["QCD_HT200to300"]    = FileSummary(fDir_ + MCloc + "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 1735000, lumi, 18717349, 1.0,  kBlue);
        sampleSet_["QCD_HT300to500"]    = FileSummary(fDir_ + MCloc + "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 366800, lumi, 20086103, 1.0,  kBlue);
        sampleSet_["QCD_HT500to700"]    = FileSummary(fDir_ + MCloc + "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 29370, lumi, 19542847, 1.0,  kBlue);
        sampleSet_["QCD_HT700to1000"]   = FileSummary(fDir_ + MCloc + "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 6524, lumi, 15011016, 1.0,  kBlue);
        sampleSet_["QCD_HT1000to1500"]  = FileSummary(fDir_ + MCloc + "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1064, lumi, 4963895, 1.0,  kBlue);
        sampleSet_["QCD_HT1500to2000"]  = FileSummary(fDir_ + MCloc + "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 121.5, lumi, 3848411, 1.0,  kBlue);
        sampleSet_["QCD_HT2000toInf"]   = FileSummary(fDir_ + MCloc + "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 25.42, lumi, 1856112, 1.0,  kBlue);

        sampleSet_["QCD_Pt_10to15"]       = FileSummary(fDir_ + MCloc + "QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 5887580000, lumi, 3449170, 1.0,  kBlue);
        sampleSet_["QCD_Pt_15to30"]       = FileSummary(fDir_ + MCloc + "QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 1837410000, lumi, 4942232, 1.0,  kBlue);
        sampleSet_["QCD_Pt_30to50"]       = FileSummary(fDir_ + MCloc + "QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 140932000, lumi, 4957245, 1.0,  kBlue);
        sampleSet_["QCD_Pt_50to80"]       = FileSummary(fDir_ + MCloc + "QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.txt",     "stopTreeMaker/AUX", 19204300, lumi, 4978425, 1.0,  kBlue);
        sampleSet_["QCD_Pt_80to120"]      = FileSummary(fDir_ + MCloc + "QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt",    "stopTreeMaker/AUX", 2762530, lumi, 3424782, 1.0,  kBlue);
        sampleSet_["QCD_Pt_120to170"]     = FileSummary(fDir_ + MCloc + "QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 471100, lumi, 3452896, 1.0,  kBlue);
        sampleSet_["QCD_Pt_170to300"]     = FileSummary(fDir_ + MCloc + "QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 117276, lumi, 3364368, 1.0,  kBlue);
        sampleSet_["QCD_Pt_300to470"]     = FileSummary(fDir_ + MCloc + "QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 7823, lumi, 2933611, 1.0,  kBlue);
        sampleSet_["QCD_Pt_470to600"]     = FileSummary(fDir_ + MCloc + "QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 648.2, lumi, 1936832, 1.0,  kBlue);
        sampleSet_["QCD_Pt_600to800"]     = FileSummary(fDir_ + MCloc + "QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.txt",   "stopTreeMaker/AUX", 186.9, lumi, 1964128, 1.0,  kBlue);
        sampleSet_["QCD_Pt_800to1000"]    = FileSummary(fDir_ + MCloc + "QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.txt",  "stopTreeMaker/AUX", 32.293, lumi, 1937216, 1.0,  kBlue);
        sampleSet_["QCD_Pt_1000to1400"]   = FileSummary(fDir_ + MCloc + "QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 9.4183, lumi, 1487136, 1.0,  kBlue);
        sampleSet_["QCD_Pt_1400to1800"]   = FileSummary(fDir_ + MCloc + "QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.84265, lumi, 197959, 1.0,  kBlue);
        sampleSet_["QCD_Pt_1800to2400"]   = FileSummary(fDir_ + MCloc + "QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.114943, lumi, 193608, 1.0,  kBlue);
        sampleSet_["QCD_Pt_2400to3200"]   = FileSummary(fDir_ + MCloc + "QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.txt", "stopTreeMaker/AUX", 0.00682981, lumi, 194456, 1.0,  kBlue);
        sampleSet_["QCD_Pt_3200toInf"]    = FileSummary(fDir_ + MCloc + "QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.txt",  "stopTreeMaker/AUX", 0.000165445, lumi, 192944, 1.0,  kBlue);

        //Other Samples
        // Aprox. NNLO
        sampleSet_["tW_top"]                    = FileSummary(fDir_ + MCloc + "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi, 1000000,  1.0,  kYellow);
        // Aprox. NNLO
        sampleSet_["tW_antitop"]                 = FileSummary(fDir_ + MCloc + "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi, 995600,  1.0,  kYellow);

        // NLO --> negative weights!
        // (sign of gen weight) * (lumi*xsec)/(effective number of events): effective number of events = N(evt) with positive weight - N(evt) with negative weight
        sampleSet_["TTZToLLNuNu"]                     = FileSummary(fDir_ + MCloc + "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.5297, lumi, 291495 - 106505,  1.0,  kGreen+2);
        sampleSet_["TTZToQQ"]                     = FileSummary(fDir_ + MCloc + "TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2529, lumi, 550599 - 199201,  1.0,  kGreen+2);

        // NLO --> negative weights!
        sampleSet_["TTWJetsToLNu"]                     = FileSummary(fDir_ + MCloc + "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 0.2043, lumi, 191379 - 61529,  1.0,  kGreen+2);
        sampleSet_["TTWJetsToQQ"]                     = FileSummary(fDir_ + MCloc + "TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 0.4062, lumi, 632147 - 201817,  1.0,  kGreen+2);

        // --------
        // - data -
        // --------
	
	sampleSet_["Data_SingleMuon_2015B"] = FileSummary(fDir_ + "Data_50ns_Ntp_74X_03Oct2015_v2.1/pastika/crab_SingleMuon_Run2015B-PromptReco-v1.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_SingleMuon_2015C"] = FileSummary(fDir_ + "Data_25ns_Ntp_74X_03Oct2015_v2.1/pastika/crab_SingleMuon_Run2015C-PromptReco-v1.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_SingleMuon_2015D"] = FileSummary(fDir_ + "Data_25ns_Ntp_74X_03Oct2015_v2.1/pastika/crab_SingleMuon_Run2015D-PromptReco-v3.txt","stopTreeMaker/AUX", 1.0,  kBlack);

	sampleSet_["Data_SingleElectron_2015B"] = FileSummary(fDir_ + MCloc + "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015B-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_SingleElectron_2015C"] = FileSummary(fDir_ + MCloc + "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015C-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_SingleElectron_2015D"] = FileSummary(fDir_ + MCloc + "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);

	sampleSet_["Data_DoubleMuon_2015B"] = FileSummary(fDir_ + MCloc + "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015B-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_DoubleMuon_2015C"] = FileSummary(fDir_ + MCloc + "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015C-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_DoubleMuon_2015D"] = FileSummary(fDir_ + MCloc + "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);

	sampleSet_["Data_DoubleEG_2015B"] = FileSummary(fDir_ + MCloc + "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015B-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_DoubleEG_2015C"] = FileSummary(fDir_ + MCloc + "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015C-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_DoubleEG_2015D"] = FileSummary(fDir_ + MCloc + "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);

	sampleSet_["Data_HTMHT_2015B"] = FileSummary(fDir_ + MCloc + "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015B-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_HTMHT_2015C"] = FileSummary(fDir_ + MCloc + "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015C-PromptReco.txt","stopTreeMaker/AUX", 1.0,  kBlack);
	sampleSet_["Data_HTMHT_2015D"] = FileSummary(fDir_ + MCloc + "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015D-PromptReco_FIX.txt","stopTreeMaker/AUX", 1.0,  kBlack);

        // ----------
	// - signal -
        // ----------

	// To be updated - no T2tt, T2bb Spring15 samples yet (update later)!
        sampleSet_["Signal_T1tttt_mGluino1200_mLSP800"]          = FileSummary(fDir_ + MCloc + "T1tttt_2J_mGl-1200_mLSP-800_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 100322, 1.0,  kRed);
        sampleSet_["Signal_T1tttt_mGluino1500_mLSP100"]          = FileSummary(fDir_ + MCloc + "T1tttt_2J_mGl-1500_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105679, 1.0,  kRed);
        sampleSet_["Signal_T5tttt_mGluino1300_mStop300_mChi280"] = FileSummary(fDir_ + MCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 44011,  1.0,  kRed);
        sampleSet_["Signal_T5tttt_mGluino1300_mStop300_mCh285"]  = FileSummary(fDir_ + MCloc + "", "stopTreeMaker/AUX", 0.0460525, lumi, 43818,  1.0,  kRed);
        sampleSet_["Signal_T1bbbb_mGluino1000_mLSP900"]          = FileSummary(fDir_ + MCloc + "T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.325388,  lumi, 97134,  1.0,  kRed);
        sampleSet_["Signal_T1bbbb_mGluino1500_mLSP100"]          = FileSummary(fDir_ + MCloc + "T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105149, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop425_mLSP325"]               = FileSummary(fDir_ + MCloc + "T2tt_2J_mStop-425_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030,1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop500_mLSP325"]               = FileSummary(fDir_ + MCloc + "T2tt_2J_mStop-500_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.51848,   lumi, 109591, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop650_mLSP325"]               = FileSummary(fDir_ + MCloc + "T2tt_2J_mStop-650_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.107045,  lumi, 105672, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop850_mLSP100"]               = FileSummary(fDir_ + MCloc + "T2tt_2J_mStop-850_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 102839, 1.0,  kRed);
        sampleSet_["Signal_T2bb_mSbottom600_mLSP580"]            = FileSummary(fDir_ + MCloc + "T2bb_2J_mStop-600_mLSP-580_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.174599,  lumi, 107316, 1.0,  kRed);
        sampleSet_["Signal_T2bb_mSbottom900_mLSP100"]            = FileSummary(fDir_ + MCloc + "T2bb_2J_mStop-900_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0128895, lumi, 102661, 1.0,  kRed);    
        sampleSet_["Signal_TTDMDMJets_M600GeV"]                  = FileSummary(fDir_ + MCloc + "TTDMDMJets_M600GeV.txt", "stopTreeMaker/AUX", 0.1038, lumi, 126547, 1.0,  kRed);    
        sampleSet_["Signal_TTDMDMJets_M1000GeV"]                 = FileSummary(fDir_ + MCloc + "TTDMDMJets_M1000GeV.txt", "stopTreeMaker/AUX", 0.01585, lumi, 121817, 1.0,  kRed);    
    }

    SampleCollection::SampleCollection(SampleSet samples)
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

        addSampleSet(samples, "WJetsToLNu_LESS", {"WJetsToLNu_HT_600toInf", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});
        addSampleSet(samples, "WJetsToLNu", {"WJetsToLNu_HT_2500toInf", "WJetsToLNu_HT_1200to2500", "WJetsToLNu_HT_800to1200", "WJetsToLNu_HT_600to800", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});

        addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_600toInf", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200"});
        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
        addSampleSet(samples, "IncDY", {"DYJetsToLL"});

        addSampleSet(samples, "QCD", {"QCD_HT2000toInf", "QCD_HT1500to2000", "QCD_HT1000to1500", "QCD_HT700to1000", "QCD_HT500to700", "QCD_HT300to500", "QCD_HT200to300", "QCD_HT100to200"});

        addSampleSet(samples, "tW", {"tW_top", "tW_antitop"});
        addSampleSet(samples, "TTZ", {"TTZToLLNuNu", "TTZToQQ"});
        addSampleSet(samples, "TTW", {"TTWJetsToLNu", "TTWJetsToQQ"});

        addSampleSet(samples, "Data_SingleMuon50ns", {"Data_SingleMuon_2015B"});
        addSampleSet(samples, "Data_SingleMuon25ns", {"Data_SingleMuon_2015C", "Data_SingleMuon_2015D"});

        addSampleSet(samples, "Data_SingleElectron50ns", {"Data_SingleElectron_2015B"});
        addSampleSet(samples, "Data_SingleElectron25ns", {"Data_SingleElectron_2015C", "Data_SingleElectron_2015D"});

        addSampleSet(samples, "Data_DoubleMuon50ns", {"Data_DoubleMuon_2015B"});
        addSampleSet(samples, "Data_DoubleMuon25ns", {"Data_DoubleMuon_2015C", "Data_DoubleMuon_2015D"});

        addSampleSet(samples, "Data_DoubleEG50ns", {"Data_DoubleEG_2015B"});
        addSampleSet(samples, "Data_DoubleEG25ns", {"Data_DoubleEG_2015C", "Data_DoubleEG_2015D"});

        addSampleSet(samples, "Data_HTMHT50ns", {"Data_HTMHT_2015B"});
        addSampleSet(samples, "Data_HTMHT25ns", {"Data_HTMHT_2015C", "Data_HTMHT_2015D"});

    }

    void SampleCollection::addSampleSet(SampleSet& samples, std::string name, std::vector<std::string> vss)
    {
        for(std::string& sn : vss)
        {
            sampleSet_[name].push_back(samples[sn]);
            nameVec_[name].push_back(sn);
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
