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
        FileSummary(std::string filePath, std::string treePath, double xsec, double lumi, double nEvts, double kfactor, int color = kBlack) : filePath(filePath), treePath(treePath), xsec(xsec), lumi(lumi), kfactor(kfactor), nEvts(nEvts), color(color)
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
    static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/";

    static std::map<std::string, FileSummary> samples = {
        //backgrounds
        {"TTbar",                   FileSummary(fileDir + "rootlist_TTJets.txt", "stopTreeMaker/AUX", 806.1,  lumi, 25446993, 1.0, kGreen)},
        {"WJetsToLNu_HT_600toInf",  FileSummary(fileDir + "rootlist_WJetsToLNu_HT_600toInf.txt", "stopTreeMaker/AUX", 18.81,  lumi, 4581841,  1.0, kMagenta+1)},
        {"WJetsToLNu_HT_400to600",  FileSummary(fileDir + "rootlist_WJetsToLNu_HT_400to600.txt", "stopTreeMaker/AUX", 55.61,  lumi, 4640594,  1.0, kMagenta+1)},
        {"WJetsToLNu_HT_200to400",  FileSummary(fileDir + "rootlist_WJetsToLNu_HT_200to400.txt", "stopTreeMaker/AUX", 471.6,  lumi, 4686783,  1.0, kMagenta+1)},
        {"WJetsToLNu_HT_100to200",  FileSummary(fileDir + "rootlist_WJetsToLNu_HT_100to200.txt", "stopTreeMaker/AUX", 1817.0, lumi, 5262265,  1.0, kMagenta+1)},
        {"ZJetsToNuNu_HT_600toInf", FileSummary(fileDir + "rootlist_ZJetsToNuNu_HT_600toInf.txt", "stopTreeMaker/AUX", 4.113,  lumi, 4463806, 1.0,  kTeal+4)},
        {"ZJetsToNuNu_HT_400to600", FileSummary(fileDir + "rootlist_ZJetsToNuNu_HT_400to600.txt", "stopTreeMaker/AUX", 11.99,  lumi, 4433784, 1.0,  kTeal+4)},
        {"ZJetsToNuNu_HT_200to400", FileSummary(fileDir + "rootlist_ZJetsToNuNu_HT_200to400.txt", "stopTreeMaker/AUX", 100.8,  lumi, 4546470, 1.0,  kTeal+4)},
        {"ZJetsToNuNu_HT_100to200", FileSummary(fileDir + "rootlist_ZJetsToNuNu_HT_100to200.txt", "stopTreeMaker/AUX", 372.6,  lumi, 4986424, 1.0,  kTeal+4)},
        {"DYJetsToLL_HT_600toInf",  FileSummary(fileDir + "rootlist_DYJetsToLL_HT_600toInf.txt", "stopTreeMaker/AUX", 2.179,  lumi, 4493574, 1.0,  kYellow-7)},
        {"DYJetsToLL_HT_400to600",  FileSummary(fileDir + "rootlist_DYJetsToLL_HT_400to600.txt", "stopTreeMaker/AUX", 6.546,  lumi, 4931372, 1.0,  kYellow-7)},
        {"DYJetsToLL_HT_200to400",  FileSummary(fileDir + "rootlist_DYJetsToLL_HT_200to400.txt", "stopTreeMaker/AUX", 52.24,  lumi, 4666496, 1.0,  kYellow-7)},
        {"DYJetsToLL_HT_100to200",  FileSummary(fileDir + "rootlist_DYJetsToLL_HT_100to200.txt", "stopTreeMaker/AUX", 194.3,  lumi, 4054159, 1.0,  kYellow-7)},
        {"QCD_HT_1000toInf",        FileSummary(fileDir + "rootlist_QCD_HT_1000toInf.txt", "stopTreeMaker/AUX", 769.7,  lumi, 1464453, 1.0,  kBlue)},
        {"QCD_HT_500to1000",        FileSummary(fileDir + "rootlist_QCD_HT_500to1000.txt", "stopTreeMaker/AUX", 26740,  lumi, 4063345, 1.0,  kBlue)},
        {"QCD_HT_250to500",         FileSummary(fileDir + "rootlist_QCD_HT_250to500.txt", "stopTreeMaker/AUX", 670500, lumi, 2668172, 1.0,  kBlue)},
        {"T_tW",                    FileSummary(fileDir + "", "stopTreeMaker/AUX", 35.0,   lumi, 986100,  1.0,  kYellow)},
        {"Tbar_tW",                 FileSummary(fileDir + "", "stopTreeMaker/AUX", 35.0,   lumi, 971800,  1.0,  kYellow)},
        {"TTZ",                     FileSummary(fileDir + "", "stopTreeMaker/AUX", 2.2320, lumi, 249275,  1.0,  kGreen+2)},

        //signal
        {"Signal_T1tttt_mGluino1200_mLSP800",          FileSummary(fileDir + "rootlist_T1tttt_mGluino1200_mLSP800.txt", "stopTreeMaker/AUX", 0.0856418, lumi, 100322, 1.0,  kRed)},
        {"Signal_T1tttt_mGluino1500_mLSP100",          FileSummary(fileDir + "rootlist_T1tttt_mGluino1500_mLSP100.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105679, 1.0,  kRed)},
        {"Signal_T5tttt_mGluino1300_mStop300_mChi280", FileSummary(fileDir + "rootlist_T5tttt_mGluino1300_mStop300_mChi280.txt", "stopTreeMaker/AUX", 0.0460525, lumi, 16360,  1.0,  kRed)},
        {"Signal_T5tttt_mGluino1300_mStop300_mCh285",  FileSummary(fileDir + "rootlist_T5tttt_mGluino1300_mStop300_mCh285.txt", "stopTreeMaker/AUX", 0.0460525, lumi, 35288,  1.0,  kRed)},
        {"Signal_T1bbbb_mGluino1000_mLSP900",          FileSummary(fileDir + "rootlist_T1bbbb_mGluino1000_mLSP900.txt", "stopTreeMaker/AUX", 0.325388,  lumi, 97134,  1.0,  kRed)},
        {"Signal_T1bbbb_mGluino1500_mLSP100",          FileSummary(fileDir + "rootlist_T1bbbb_mGluino1500_mLSP100.txt", "stopTreeMaker/AUX", 0.0141903, lumi, 105149, 1.0,  kRed)},
        {"Signal_T2tt_mStop425_mLSP325",               FileSummary(fileDir + "rootlist_T2tt_mStop425_mLSP325.txt", "stopTreeMaker/AUX", 1.31169,   lumi, 1039030,1.0,  kRed)},
        {"Signal_T2tt_mStop500_mLSP325",               FileSummary(fileDir + "rootlist_T2tt_mStop500_mLSP325.txt", "stopTreeMaker/AUX", 0.51848,   lumi, 109591, 1.0,  kRed)},
        {"Signal_T2tt_mStop650_mLSP325",               FileSummary(fileDir + "rootlist_T2tt_mStop650_mLSP325.txt", "stopTreeMaker/AUX", 0.107045,  lumi, 105672, 1.0,  kRed)},
        {"Signal_T2tt_mStop850_mLSP100",               FileSummary(fileDir + "rootlist_T2tt_mStop850_mLSP100.txt", "stopTreeMaker/AUX", 0.0189612, lumi, 102839, 1.0,  kRed)},
        {"Signal_T2bb_mSbottom600_mLSP580",            FileSummary(fileDir + "rootlist_T2bb_mSbottom600_mLSP580.txt", "stopTreeMaker/AUX", 0.174599,  lumi, 107316, 1.0,  kRed)},
        {"Signal_T2bb_mSbottom900_mLSP100",            FileSummary(fileDir + "rootlist_T2bb_mSbottom900_mLSP100.txt", "stopTreeMaker/AUX", 0.0128895, lumi, 102661, 1.0,  kRed)}
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
