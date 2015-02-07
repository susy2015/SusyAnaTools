#ifndef ANASAMPLES_SAMPLES_H
#define ANASAMPLES_SAMPLES_H

#include <string>
#include <map>
#include <vector>
#include "TColor.h"

namespace AnaSamples
{
    class FileSummary
    {
    public:
        std::string filePath, treePath;
        double xsec, lumi, kfactor, nEvts;
        int color;
        
        FileSummary() {}
        FileSummary(std::string filePath, std::string treePath, double xsec, double lumi, double kfactor, double nEvts, int color = kBlack) : filePath(filePath), treePath(treePath), xsec(xsec), lumi(lumi), kfactor(kfactor), nEvts(nEvts), color(color)
        {
            weight_ = xsec * lumi * kfactor / nEvts;
        }

        double getWeight() {return weight_;}

    private:
        double weight_;
    };

    bool operator< (const FileSummary& lhs, const FileSummary& rhs);
    bool operator== (const FileSummary& lhs, const FileSummary& rhs);

    static const double lumi = 5000.0; // in pb-1

    static std::map<std::string, FileSummary> samples = {
        //backgrounds
        {"TTbar",                   FileSummary("", "stopTreeMaker/AUX", 806.1,  lumi, 25446993, kGreen)},
        {"WJetsToLNu_HT_600toInf",  FileSummary("", "stopTreeMaker/AUX", 18.81,  lumi, 4581841,  kMagenta+1)},
        {"WJetsToLNu_HT_400to600",  FileSummary("", "stopTreeMaker/AUX", 55.61,  lumi, 4640594,  kMagenta+1)},
        {"WJetsToLNu_HT_200to400",  FileSummary("", "stopTreeMaker/AUX", 471.6,  lumi, 4686783,  kMagenta+1)},
        {"WJetsToLNu_HT_100to200",  FileSummary("", "stopTreeMaker/AUX", 1817.0, lumi, 5262265,  kMagenta+1)},
        {"ZJetsToNuNu_HT_600toInf", FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223010/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 4.113,  lumi, 4463806,  kTeal+4)},
        {"ZJetsToNuNu_HT_400to600", FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150109_230130/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 11.99,  lumi, 4433784,  kTeal+4)},
        {"ZJetsToNuNu_HT_200to400", FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222913/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 100.8,  lumi, 4546470,  kTeal+4)},
        {"ZJetsToNuNu_HT_100to200", FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 372.6,  lumi, 4986424,  kTeal+4)},
        {"DYJetsToLL_HT_600toInf",  FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224252/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 2.179,  lumi, 4493574,  kYellow-7)},
        {"DYJetsToLL_HT_400to600",  FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224040/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 6.546,  lumi, 4931372,  kYellow-7)},
        {"DYJetsToLL_HT_200to400",  FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 52.24,  lumi, 4666496,  kYellow-7)},
        {"DYJetsToLL_HT_100to200",  FileSummary("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150101_225307/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 194.3,  lumi, 4054159,  kYellow-7)},
        {"QCD_HT_1000toInf",        FileSummary("", "stopTreeMaker/AUX", 769.7,  lumi, 1464453,  kBlue)},
        {"QCD_HT_500to1000",        FileSummary("", "stopTreeMaker/AUX", 26740,  lumi, 4063345,  kBlue)},
        {"QCD_HT_250to500",         FileSummary("", "stopTreeMaker/AUX", 670500, lumi, 2668172,  kBlue)},
        {"T_tW",                    FileSummary("", "stopTreeMaker/AUX", 35.0,   lumi, 986100,   kYellow)},
        {"Tbar_tW",                 FileSummary("", "stopTreeMaker/AUX", 35.0,   lumi, 971800,   kYellow)},
        {"TTZ",                     FileSummary("", "stopTreeMaker/AUX", 2.2320, lumi, 249275,   kGreen+2)},

        //signal
        {"Signal_T1tttt_mGluino1200_mLSP800",          FileSummary("", "stopTreeMaker/AUX", 0.0856418, lumi, 100322,  kRed)},
        {"Signal_T1tttt_mGluino1500_mLSP100",          FileSummary("", "stopTreeMaker/AUX", 0.0141903, lumi, 105679,  kRed)},
        {"Signal_T5tttt_mGluino1300_mStop300_mChi280", FileSummary("", "stopTreeMaker/AUX", 0.0460525, lumi, 16360,   kRed)},
        {"Signal_T5tttt_mGluino1300_mStop300_mCh285",  FileSummary("", "stopTreeMaker/AUX", 0.0460525, lumi, 35288,   kRed)},
        {"Signal_T1bbbb_mGluino1000_mLSP900",          FileSummary("", "stopTreeMaker/AUX", 0.325388,  lumi, 97134,   kRed)},
        {"Signal_T1bbbb_mGluino1500_mLSP100",          FileSummary("", "stopTreeMaker/AUX", 0.0141903, lumi, 105149,  kRed)},
        {"Signal_T2tt_mStop425_mLSP325",               FileSummary("", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030, kRed)},
        {"Signal_T2tt_mStop500_mLSP325",               FileSummary("", "stopTreeMaker/AUX", 0.51848,   lumi, 109591,  kRed)},
        {"Signal_T2tt_mStop650_mLSP325",               FileSummary("", "stopTreeMaker/AUX", 0.107045,  lumi, 105672,  kRed)},
        {"Signal_T2tt_mStop850_mLSP100",               FileSummary("", "stopTreeMaker/AUX", 0.0189612, lumi, 102839,  kRed)},
        {"Signal_T2bb_mSbottom600_mLSP580",            FileSummary("", "stopTreeMaker/AUX", 0.174599,  lumi, 107316,  kRed)},
        {"Signal_T2bb_mSbottom900_mLSP100",            FileSummary("", "stopTreeMaker/AUX", 0.0128895, lumi, 102661,  kRed)}
    };

    class SampleHolder
    {
    public:
        SampleHolder();
        std::vector<FileSummary>& operator[](std::string key) {return sampleSet_[key];}
    private:
        std::map<std::string, std::vector<FileSummary>> sampleSet_;
    };

}

#endif
