#include "samples.h"

#include <iostream>
#include <cstdio>

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

    void FileSummary::addFilesToChain(TChain * const tc) const
    {
        for(auto& f : filelist_) tc->Add(f.c_str());
    }

    SampleSet::SampleSet(std::string fDir) : fDir_(fDir)
    {
        // ---------------
        // - backgrounds -
        // ---------------

        sampleSet_["TTbar"]                   = FileSummary(fDir_ + "lhx/TTJets_madgraphMLM-pythia8_with_JEC_fix.txt", "stopTreeMaker/AUX", 831.76,  lumi, 11339232, 1.0, kGreen);
        
	// WJets to be updated
	sampleSet_["WJetsToLNu_HT_600toInf"]  = FileSummary(fDir_ + "WJetsToLNu_HT-600toInf_madgraph-tauola.txt", "stopTreeMaker/AUX", 23.14,  lumi, 4581841,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_400to600"]  = FileSummary(fDir_ + "WJetsToLNu_HT-400to600_madgraph-tauola.txt", "stopTreeMaker/AUX", 68.40,  lumi, 4640594,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_200to400"]  = FileSummary(fDir_ + "WJetsToLNu_HT-200to400_madgraph-tauola.txt", "stopTreeMaker/AUX", 580.1,  lumi, 4936077,  1.0, kMagenta+1);
        sampleSet_["WJetsToLNu_HT_100to200"]  = FileSummary(fDir_ + "WJetsToLNu_HT-100to200_madgraph-tauola.txt", "stopTreeMaker/AUX", 2235.0, lumi, 5262265,  1.0, kMagenta+1);

        sampleSet_["ZJetsToNuNu_HT_600toInf"] = FileSummary(fDir_ + "nstrobbe/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 4.203,  lumi, 1015904, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_400to600"] = FileSummary(fDir_ + "nstrobbe/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", "stopTreeMaker/AUX", 10.944, lumi, 1014139, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_200to400"] = FileSummary(fDir_ + "nstrobbe/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", "stopTreeMaker/AUX", 78.36,  lumi, 5032927, 1.0,  kTeal+4);
        sampleSet_["ZJetsToNuNu_HT_100to200"] = FileSummary(fDir_ + "nstrobbe/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", "stopTreeMaker/AUX", 280.47, lumi, 5148193, 1.0,  kTeal+4);


        sampleSet_["DYJetsToLL_HT_600toInf"]  = FileSummary(fDir_ + "nstrobbe/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 2.21,  lumi, 987977, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_400to600"]  = FileSummary(fDir_ + "nstrobbe/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 5.497,  lumi, 1048047, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_200to400"]  = FileSummary(fDir_ + "nstrobbe/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 42.75,  lumi, 955972, 1.0,  kYellow-7);
        sampleSet_["DYJetsToLL_HT_100to200"]  = FileSummary(fDir_ + "nstrobbe/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", "stopTreeMaker/AUX", 139.4,  lumi, 2625679, 1.0,  kYellow-7);


	// To be updated
        sampleSet_["IncDYSampleSet"]                   = FileSummary(fDir_ + "DYJetsToLL_M-50_13TeV-madgraph-pythia8.txt", "stopTreeMaker/AUX", 4746,  lumi, 2829164, 1.0,  kYellow-4);


	// To be updated
        sampleSet_["QCD_HT_1000toInf"]        = FileSummary(fDir_ + "QCD_HT_1000ToInf_13TeV-madgraph.txt", "stopTreeMaker/AUX", 769.7,  lumi, 1464453, 1.0,  kBlue);
        sampleSet_["QCD_HT_500to1000"]        = FileSummary(fDir_ + "QCD_HT_500To1000_13TeV-madgraph.txt", "stopTreeMaker/AUX", 26740,  lumi, 4063345, 1.0,  kBlue);
        sampleSet_["QCD_HT_250to500"]         = FileSummary(fDir_ + "QCD_HT_250To500_13TeV-madgraph.txt", "stopTreeMaker/AUX", 670500, lumi, 2668172, 1.0,  kBlue);
        sampleSet_["T_tW"]                    = FileSummary(fDir_ + "T_tW-channel-DR_powheg-tauola.txt", "stopTreeMaker/AUX", 35.0,   lumi, 986100,  1.0,  kYellow);
        sampleSet_["Tbar_tW"]                 = FileSummary(fDir_ + "Tbar_tW-channel-DR_powheg-tauola.txt", "stopTreeMaker/AUX", 35.0,   lumi, 971800,  1.0,  kYellow);


        sampleSet_["TTZ"]                     = FileSummary(fDir_ + "nstrobbe/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt", "stopTreeMaker/AUX", 0.2529, lumi, 398000,  1.0,  kGreen+2);


        // ----------
	// - signal -
        // ----------

	// To be updated
        sampleSet_["Signal_T1tttt_mGluino1200_mLSP800"]          = FileSummary(fDir_ + "T1tttt_2J_mGl-1200_mLSP-800_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 100322, 1.0,  kRed);
        sampleSet_["Signal_T1tttt_mGluino1500_mLSP100"]          = FileSummary(fDir_ + "T1tttt_2J_mGl-1500_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105679, 1.0,  kRed);
        sampleSet_["Signal_T5tttt_mGluino1300_mStop300_mChi280"] = FileSummary(fDir_ + "", "stopTreeMaker/AUX", 0.0460525, lumi, 44011,  1.0,  kRed);
        sampleSet_["Signal_T5tttt_mGluino1300_mStop300_mCh285"]  = FileSummary(fDir_ + "", "stopTreeMaker/AUX", 0.0460525, lumi, 43818,  1.0,  kRed);
        sampleSet_["Signal_T1bbbb_mGluino1000_mLSP900"]          = FileSummary(fDir_ + "T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.325388,  lumi, 97134,  1.0,  kRed);
        sampleSet_["Signal_T1bbbb_mGluino1500_mLSP100"]          = FileSummary(fDir_ + "T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105149, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop425_mLSP325"]               = FileSummary(fDir_ + "T2tt_2J_mStop-425_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030,1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop500_mLSP325"]               = FileSummary(fDir_ + "T2tt_2J_mStop-500_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.51848,   lumi, 109591, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop650_mLSP325"]               = FileSummary(fDir_ + "T2tt_2J_mStop-650_mLSP-325_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.107045,  lumi, 105672, 1.0,  kRed);
        sampleSet_["Signal_T2tt_mStop850_mLSP100"]               = FileSummary(fDir_ + "T2tt_2J_mStop-850_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 102839, 1.0,  kRed);
        sampleSet_["Signal_T2bb_mSbottom600_mLSP580"]            = FileSummary(fDir_ + "T2bb_2J_mStop-600_mLSP-580_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.174599,  lumi, 107316, 1.0,  kRed);
        sampleSet_["Signal_T2bb_mSbottom900_mLSP100"]            = FileSummary(fDir_ + "T2bb_2J_mStop-900_mLSP-100_madgraph-tauola.txt", "stopTreeMaker/AUX", 0.0128895, lumi, 102661, 1.0,  kRed);    
        sampleSet_["Signal_TTDMDMJets_M600GeV"]                  = FileSummary(fDir_ + "TTDMDMJets_M600GeV.txt", "stopTreeMaker/AUX", 0.1038, lumi, 126547, 1.0,  kRed);    
        sampleSet_["Signal_TTDMDMJets_M1000GeV"]                 = FileSummary(fDir_ + "TTDMDMJets_M1000GeV.txt", "stopTreeMaker/AUX", 0.01585, lumi, 121817, 1.0,  kRed);    
    }

    SampleCollection::SampleCollection(SampleSet samples)
    {
        //Define sets of samples for later use 
        sampleSet_["TTbar"]              = {samples["TTbar"]};
        sampleSet_["WJetsToLNu"]         = {samples["WJetsToLNu_HT_600toInf"], samples["WJetsToLNu_HT_400to600"], samples["WJetsToLNu_HT_200to400"], samples["WJetsToLNu_HT_100to200"]};
        sampleSet_["ZJetsToNuNu"]        = {samples["ZJetsToNuNu_HT_600toInf"], samples["ZJetsToNuNu_HT_400to600"], samples["ZJetsToNuNu_HT_200to400"], samples["ZJetsToNuNu_HT_100to200"]};
        sampleSet_["DYJetsToLL"]         = {samples["DYJetsToLL_HT_600toInf"], samples["DYJetsToLL_HT_400to600"], samples["DYJetsToLL_HT_200to400"], samples["DYJetsToLL_HT_100to200"]};
        sampleSet_["IncDY"]              = {samples["DYJetsToLL"]};
        sampleSet_["QCD"]                = {samples["QCD_HT_1000toInf"], samples["QCD_HT_500to1000"], samples["QCD_HT_250to500"]};
        sampleSet_["T_tW"]               = {samples["T_tW"], samples["Tbar_tW"]};
        sampleSet_["TTZ"]                = {samples["TTZ"]};
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
