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

    std::string MCloc = "Spring16_80X_Jul_2016_Ntp_v8X/";
        std::string MClocv6X = "Spring16_80X_Jun_2016_Ntp_v6X/";
        std::string MCloc1 ="Spring16_80X_Jun_2016_Ntp_v5X/"; 
        std::string MCloc2 = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string DATAloc = "Spring16_80X_Jul_2016_Ntp_v9X/";
        std::string DATAloc74X = "Spring15_74X_Feb_2016_Ntp_v6X_forMoriond/";
        std::string sigMCloc = "Spring16_80X_Jul_2016_Ntp_v8X/";
        std::string addSigMCloc = "Spring15_74X_v5X_top_corridor/";
        std::string QGPloc = "Spring16_80X_Jul_2016_Ntp_v9X_QGD/";
        std::string JetChrgloc = "hua/addJetsCharge/";

        if(fDir.compare("condor") == 0)
        {
            fDir_ = "";
            MCloc = "";
            MClocv6X = "";
            MCloc1 = "";
            MCloc2 = "";
            DATAloc = "";
            DATAloc74X = "";
            sigMCloc = "";
            addSigMCloc = "";
            QGPloc = "";
	    JetChrgloc = "";
        }

        //TTbar samples
        // TTbarInc has LO xsec on McM : 502.20 pb. The NNLO is 831.76 pb. The k-factor for ttbar is: kt = 831.76/502.20 ~ 1.656233
        // 1.61 * kt 
        addSample("TTbar_HT-600to800", fDir_ + MCloc1 + "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 2.666535,    lumi, 14305795, 1.0, kGreen);
        // 0.663 * kt
        addSample("TTbar_HT-800to1200", fDir_ + MCloc1 + "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1.098082,    lumi, 21107332, 1.0, kGreen);
        // 0.12 * kt
        addSample("TTbar_HT-1200to2500", fDir_ + MCloc1 + "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.198748,    lumi, 2932983, 1.0, kGreen);
        // 0.00143 * kt
        addSample("TTbar_HT-2500toInf", fDir_ + MCloc1 + "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 0.002368413, lumi, 523618,  1.0, kGreen);
    
        // Calculated from PDG BRs'. Not from the kt * xSec in McM
        //addSample("TTbarDiLep", fDir_ + QGPloc + "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 831.76*TTbar_DiLept_BR,         lumi, 30682233, 1.0, kGreen);
        addSample("TTbarDiLep", fDir_ + JetChrgloc + "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 831.76*TTbar_DiLept_BR,         lumi, 30682233, 1.0, kGreen);
        //addSample("TTbarSingleLepT", fDir_ + QGPloc + "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 49576803, 1.0, kGreen);
    addSample("TTbarSingleLepT", fDir_ + JetChrgloc + "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 49576803, 1.0, kGreen);
        //addSample("TTbarSingleLepTbar", fDir_ + QGPloc + "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 60494823, 1.0, kGreen);
 addSample("TTbarSingleLepTbar", fDir_ + JetChrgloc + "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 831.76*0.5*TTbar_SingleLept_BR, lumi, 60494823, 1.0, kGreen);
	
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets, kw = 1.21
        addSample("WJetsToLNu_Inc", fDir_ + MCloc + "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 61526.7,  lumi, 28062407,  1.21, kMagenta+1);

    addSample("WJetsToLNu_HT_100to200", fDir_ + MCloc + "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1345,  lumi, 27546978,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_200to400", fDir_ + MCloc + "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 359.7,  lumi, 19851624,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_400to600", fDir_ + MCloc + "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 48.91,  lumi, 7432746,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600to800", fDir_ + MCloc + "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 12.05,  lumi, 3722395,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_800to1200", fDir_ + MCloc + "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  5.501,   lumi, 7854734,  1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_1200to2500", fDir_ + MCloc + "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.329,   lumi, 7063909,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_2500toInf", fDir_ + MCloc + "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX",  0.03216, lumi, 253561,   1.21, kMagenta+1);
        addSample("WJetsToLNu_HT_600toInf", fDir_ + MCloc2 + "WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 18.77,  lumi, 1039152,  1.21, kMagenta+1);
        
        //Z -> nunu
        // From https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z, kz = 1.23
    // addSample("ZJetsToNuNu_HT_100to200",   fDir_ + QGPloc + "ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 280.35, lumi, 18139590, 1.23,  kTeal+4);
    addSample("ZJetsToNuNu_HT_100to200",   fDir_ + JetChrgloc + "ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 280.35, lumi, 18181042, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_200to400",   fDir_ + QGPloc + "ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 77.67,  lumi, 19878267, 1.23,  kTeal+4);
    addSample("ZJetsToNuNu_HT_200to400",   fDir_ + JetChrgloc + "ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 77.67,  lumi, 19914665, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_400to600",   fDir_ + QGPloc + "ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 10.73,  lumi, 1020309, 1.23,  kTeal+4);
    addSample("ZJetsToNuNu_HT_400to600",   fDir_ + JetChrgloc + "ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 10.73,  lumi, 9290017, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_600to800",   fDir_ + QGPloc + "ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.853*3,  lumi, 5712221, 1.23,  kTeal+4);
    addSample("ZJetsToNuNu_HT_600to800",   fDir_ + JetChrgloc + "ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.853*3,  lumi, 5712221, 1.23,  kTeal+4);
    //addSample("ZJetsToNuNu_HT_800to1200",  fDir_ + QGPloc + "ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.394*3,  lumi, 1944423, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_800to1200",  fDir_ + JetChrgloc + "ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.394*3,  lumi, 1944423, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_1200to2500", fDir_ + QGPloc + "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.0974*3,  lumi, 513471, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_1200to2500", fDir_ + JetChrgloc + "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.0974*3,  lumi, 513471, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_2500toInf",  fDir_ + QGPloc + "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.00230*3,  lumi, 405752, 1.23,  kTeal+4);
        addSample("ZJetsToNuNu_HT_2500toInf",  fDir_ + JetChrgloc + "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 0.00230*3,  lumi, 405752, 1.23,  kTeal+4);
        //addSample("ZJetsToNuNu_HT_600toInf",   fDir_ + QGPloc + "ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 4.116,  lumi, 10202299, 1.23,  kTeal+4);
       
        //DY->ll
        // kz = 1.23
        addSample("DYJetsToLL_HT_100to200", fDir_ + MCloc + "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 147.4, lumi, 8434125, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_200to400", fDir_ + MCloc + "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 40.99, lumi, 8683719,  1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_400to600", fDir_ + MCloc + "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 5.678, lumi, 7974802, 1.23,  kYellow-7);
        addSample("DYJetsToLL_HT_600toInf", fDir_ + MCloc + "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 2.198, lumi, 5150414,  1.23,  kYellow-7);
        // NNLO
        addSample("DYJetsToLL_Inc", fDir_ + MCloc + "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",             "stopTreeMaker/AUX", 6025.2, lumi, 49877138, 1.0,  kYellow-7);

        //QCD
        // Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD. But numbers are from McM.
        addSample("QCD_HT100to200", fDir_ + MCloc + "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 27540000, lumi, 71140904, 1.0,  kBlue);
        addSample("QCD_HT200to300", fDir_ + MCloc + "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 1735000, lumi, 55433477, 1.0,  kBlue);
        addSample("QCD_HT300to500", fDir_ + MCloc + "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 366800, lumi, 54706298, 1.0,  kBlue);
        addSample("QCD_HT500to700", fDir_ + MCloc + "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",    "stopTreeMaker/AUX", 29370, lumi, 63337753, 1.0,  kBlue);
        addSample("QCD_HT700to1000", fDir_ + MCloc + "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 6524, lumi, 45453945, 1.0,  kBlue);
        addSample("QCD_HT1000to1500", fDir_ + MCloc + "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 1064, lumi, 15316362, 1.0,  kBlue);
        addSample("QCD_HT1500to2000", fDir_ + MCloc + "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",  "stopTreeMaker/AUX", 121.5, lumi, 11650581, 1.0,  kBlue);
        addSample("QCD_HT2000toInf", fDir_ + MCloc + "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",   "stopTreeMaker/AUX", 25.42, lumi, 6007777, 1.0,  kBlue);

        //Other Samples
        // Aprox. NNLO
        addSample("tW_top", fDir_ + MCloc + "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi,  998400,  1.0,  kYellow);
        // Aprox. NNLO
        addSample("tW_antitop", fDir_ + MCloc + "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt", "stopTreeMaker/AUX", 35.6, lumi, 985000,  1.0,  kYellow);

        // NLO --> negative weights!
        // (sign of gen weight) * (lumi*xsec)/(effective number of events): effective number of events = N(evt) with positive weight - N(evt) with negative weight
        addSample("TTZToLLNuNu", fDir_ + MCloc + "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2529, lumi, 291916 - 106684,  1.0,  kOrange+2);
        addSample("TTZToQQ", fDir_ + MCloc + "TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",          "stopTreeMaker/AUX", 0.5297, lumi, 550282 - 199118,  1.0,  kOrange+2);

        // NLO --> negative weights!
        addSample("TTWJetsToLNu", fDir_ + MCloc + "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 0.2043, lumi, 191474 - 61199,   1.0,  kSpring+2);
        addSample("TTWJetsToQQ", fDir_ + MCloc + "TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",  "stopTreeMaker/AUX", 0.4062, lumi, 631804 - 201494,  1.0,  kSpring+2);

        // NLO --> negative weights!  
        addSample("TTGJets", fDir_ + MCloc + "TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt", "stopTreeMaker/AUX", 3.697, lumi, 3226484 - 1647607,  1.0,  kOrange+2);

        // ttH --> negative weights!
    addSample("ttHJetTobb", fDir_ + MCloc + "ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8.txt",             "stopTreeMaker/AUX", 0.2934,  lumi, 6371846 - 3452319,   1.0,  kOrange+2);
    addSample("ttHJetToNonbb", fDir_ + MCloc + "ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix.txt", "stopTreeMaker/AUX", 0.2151,  lumi, 6443112 - 3493972,  1.0,  kOrange+2);
        
        // Di-boson
    // Ref. https://indico.cern.ch/event/439995/session/0/contribution/6/attachments/1143460/1638648/diboson_final.pdf (NNLO is given)
        addSample("WW", fDir_ + MCloc + "WW_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 115.0,  lumi, 993214,  1.0,  kViolet+4); 
    // Ref. https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns (NLO from MCFM)
        addSample("WZ", fDir_ + MCloc + "WZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 47.13,  lumi, 1000000,  1.0,  kViolet+4);
        addSample("ZZ", fDir_ + MCloc + "ZZ_TuneCUETP8M1_13TeV-pythia8.txt", "stopTreeMaker/AUX", 16.523, lumi, 989312,  1.0,  kViolet+4);

        // Tri-boson: negative weights!
        addSample("WWW", fDir_ + MCloc + "WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2086,  lumi, 225269 - 14731,  1.0,  kViolet+2);
        addSample("WWZ", fDir_ + MCloc + "WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.1651,  lumi, 234991 - 14209,  1.0,  kViolet+2);
        addSample("WZZ", fDir_ + MCloc + "WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.05565, lumi, 234393 - 15407,  1.0,  kViolet+2);
        addSample("ZZZ", fDir_ + MCloc + "ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.01398, lumi, 231925 - 18075,  1.0,  kViolet+2);

        // --------
        // - data -
        // --------
    //Single Muon
    addSample("Data_SingleMuon_2016", fDir_ + DATAloc + "SingleMuon.txt","stopTreeMaker/AUX", 5929.6264 + 2646.1215 + 4329.5899, 1.0,  kBlack);
    //old
    addSample("Data_SingleMuon_2015C", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
    addSample("Data_SingleMuon_2015D_05Oct2015", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
    addSample("Data_SingleMuon_2015D_PromptReco", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_SingleMuon-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.38, 1.0,  kBlack);

//    addSample("Data_DoubleEG_2015C", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
//    addSample("Data_DoubleEG_2015D_05Oct2015", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 575.34, 1.0,  kBlack);
//    addSample("Data_DoubleEG_2015D_PromptReco", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_DoubleEG-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1669.92, 1.0,  kBlack);
    
    //Single Electron
    // new 2016
    addSample("Data_SingleElectron_2016", fDir_ + MCloc + "SingleElectron.txt","stopTreeMaker/AUX", 4006.593, 1.0,  kBlack);
    
    //HT-MHT
    addSample("Data_HTMHT_2016", fDir_ + DATAloc + "HTMHT.txt","stopTreeMaker/AUX",  5926.7518 + 2646.1215 + 4329.5899, 1.0,  kBlack);
    //old
    addSample("Data_HTMHT_2015C", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015C-25ns-05Oct2015.txt","stopTreeMaker/AUX", 17.226, 1.0,  kBlack);
    addSample("Data_HTMHT_2015D_05Oct2015", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-05Oct2015.txt","stopTreeMaker/AUX", 574.23, 1.0,  kBlack);
    addSample("Data_HTMHT_2015D_PromptReco", fDir_ + DATAloc74X + "Spring15_74X_Feb_2016_Ntp_v6p0_forMoriond_HTMHT-Run2015D-PromptReco.txt","stopTreeMaker/AUX", 1670.53, 1.0,  kBlack);

        // ----------
    // - signal -
        // ----------

	// To be updated - no T2tt, T2bb Spring15 samples yet (update later)!
        addSample("Signal_T1tttt_mGluino1200_mLSP800", fDir_ + QGPloc + "SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 146849, 1.0,  kRed);
        addSample("Signal_T1tttt_mGluino1500_mLSP100", fDir_ + QGPloc + "SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105415, 1.0,  kRed);
        addSample("Signal_T2tt_mStop500_mLSP325",      fDir_ + QGPloc + "SMS-T2tt_mStop-500_mLSP-325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.51848, lumi, 396079, 1.0,  kRed);
        addSample("Signal_T2tt_mStop850_mLSP100",      fDir_ + QGPloc + "SMS-T2tt_mStop-850_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 235029, 1.0,  kRed);

// Fastsim signal scans
        addSample("SMS-T2tt_mStop-150to250", fDir_ + sigMCloc + "SMS-T2tt_mStop-150to250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-250to350", fDir_ + sigMCloc + "SMS-T2tt_mStop-250to350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-350to400", fDir_ + sigMCloc + "SMS-T2tt_mStop-350to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);
        addSample("SMS-T2tt_mStop-400to1200", fDir_ + sigMCloc + "SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T2tb

// T1tttt
        addSample("SMS-T1tttt_2016", fDir_ + sigMCloc + "SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T1ttbb

// T5ttcc
        addSample("SMS-T5ttcc_2016", fDir_ + sigMCloc + "SMS-T5ttcc_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 1.0, 1.0, 1.0, 1.0, kRed);

// T5ttttDM175

// T5tttt_degen

// T6ttWW

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

        //MC samples for Zinv
        addSampleSet(samples, "ZinvAllMC", {"TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep", 
                    "ZJetsToNuNu_HT_2500toInf", "ZJetsToNuNu_HT_1200to2500", "ZJetsToNuNu_HT_800to1200", "ZJetsToNuNu_HT_600to800", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200", 
                    "WW", "WZ", "ZZ", 
                    "TTWJetsToLNu", "TTWJetsToQQ", "TTGJets", "WWZ", "WZZ", "ZZZ", "ttHJetTobb", "ttHJetToNonbb",
                    "TTZToLLNuNu", "TTZToQQ",
                    "DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200", "DYJetsToLL_Inc",
                    "tW_top", "tW_antitop"});

        // Only all had. part of TTbarInc
        addSampleSet(samples, "TTbarAll", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"});

        // Only all had. part of TTbarInc & HT cuts on inclusive samples
        addSampleSet(samples, "TTbarExt", {"TTbarInc", "TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep", "TTbar_HT-600to800", "TTbar_HT-800to1200", "TTbar_HT-1200to2500", "TTbar_HT-2500toInf"});

    addSampleSet(samples, "WJetsToLNuInc", {"WJetsToLNu_Inc"});
        addSampleSet(samples, "WJetsToLNu", {"WJetsToLNu_HT_2500toInf", "WJetsToLNu_HT_1200to2500", "WJetsToLNu_HT_800to1200", "WJetsToLNu_HT_600to800", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200"});

        addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_2500toInf", "ZJetsToNuNu_HT_1200to2500", "ZJetsToNuNu_HT_800to1200", "ZJetsToNuNu_HT_600to800", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200"});
//     addSampleSet(samples, "ZJetsToNuNu", {"ZJetsToNuNu_HT_600toInf", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200"});
        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
//        addSampleSet(samples, "DYJetsToLL", {"DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200"});
        addSampleSet(samples, "IncDY", {"DYJetsToLL_Inc"});

        addSampleSet(samples, "QCD", {"QCD_HT2000toInf", "QCD_HT1500to2000", "QCD_HT1000to1500", "QCD_HT700to1000", "QCD_HT500to700", "QCD_HT300to500", "QCD_HT200to300", "QCD_HT100to200"});

        addSampleSet(samples, "tW", {"tW_top", "tW_antitop"});

        addSampleSet(samples, "TTZ", {"TTZToLLNuNu", "TTZToQQ"});
        addSampleSet(samples, "TTW", {"TTWJetsToLNu", "TTWJetsToQQ"});

        addSampleSet(samples, "TTG", {"TTGJets"});

//    addSampleSet(samples, "ttHJetTobb", {"ttHJetTobb"});
//    addSampleSet(samples, "ttHJetToNonbb", {"ttHJetToNonbb"});
    addSampleSet(samples, "ttH", {"ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "WWZ", {"WWZ"});
        addSampleSet(samples, "WZZ", {"WZZ"});
        addSampleSet(samples, "ZZZ", {"ZZZ"});


    addSampleSet(samples, "Diboson", {"WW", "WZ", "ZZ"});

        addSampleSet(samples, "Triboson", {"WWW", "WWZ", "WZZ", "ZZZ"});

    addSampleSet(samples, "Rare", {"TTWJetsToLNu", "TTWJetsToQQ", "TTGJets", "WWZ", "WZZ", "ZZZ", "ttHJetTobb", "ttHJetToNonbb"});

        addSampleSet(samples, "Data_SingleMuon", {"Data_SingleMuon_2016"});


    addSampleSet(samples, "Data_SingleElectron", {"Data_SingleElectron_2016"});


//    addSampleSet(samples, "Data_SingleMuon", {"Data_SingleMuon_2015C", "Data_SingleMuon_2015D_05Oct2015", "Data_SingleMuon_2015D_PromptReco"});

        //addSampleSet(samples, "Data_SingleElectron", {"Data_SingleElectron_2015C", "Data_SingleElectron_2015D_05Oct2015", "Data_SingleElectron_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleMuon", {"Data_DoubleMuon_2015C", "Data_DoubleMuon_2015D_05Oct2015", "Data_DoubleMuon_2015D_PromptReco"});

        addSampleSet(samples, "Data_DoubleEG", {"Data_DoubleEG_2015C", "Data_DoubleEG_2015D_05Oct2015", "Data_DoubleEG_2015D_PromptReco"});

        addSampleSet(samples, "Data_HTMHT", {"Data_HTMHT_2016"});
//      addSampleSet(samples, "Data_HTMHT", {"Data_HTMHT_2015C", "Data_HTMHT_2015D_05Oct2015", "Data_HTMHT_2015D_PromptReco"});

        addSampleSet(samples, "Signal_T1tttt_mGluino1200_mLSP800", {"Signal_T1tttt_mGluino1200_mLSP800"});
        addSampleSet(samples, "Signal_T1tttt_mGluino1500_mLSP100", {"Signal_T1tttt_mGluino1500_mLSP100"});
        addSampleSet(samples, "Signal_T2tt_mStop500_mLSP325", {"Signal_T2tt_mStop500_mLSP325"});
        addSampleSet(samples, "Signal_T2tt_mStop850_mLSP100", {"Signal_T2tt_mStop850_mLSP100"});

        addSampleSet(samples, "Signal_fastsim_T2tt_scan", {"SMS-T2tt_mStop-150to250", "SMS-T2tt_mStop-250to350", "SMS-T2tt_mStop-350to400", "SMS-T2tt_mStop-400to1200"});

        addSampleSet(samples, "Signal_fastsim_T1tttt_scan", {"SMS-T1tttt_2016"});

        addSampleSet(samples, "Signal_fastsim_T5ttcc_scan", {"SMS-T5ttcc_2016"});

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
