#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include "TString.h"

nanespace AnaSamples
{
    class FileSummary
    {
    public:
        std::string name, filePath, treePath;
        double xsec, lumi, kfactor, nEvts;
        int color;
        
        FileSummary() {}
        FileSummary(std::string name, std::string filePath, std::string treePath, double xsec, double lumi, double kfactor, double nEvts, int color = kBlack) : name(name), filePath(filePath), treePath(treePath), xsec(xsec), lumi(lumi), kfactor(kfactor), nEvts(nEvts), color(color)
        {
            weight_ = xsec * lumi * kfactor / nEvts;
        }

        getWeight() {return weight_;}

    private:
        weight_;
    };

    std::map<std::String, Sample> samples;

    void initSamples()
    {
        //backgrounds
        samples["TTbar"] =                   Samples("TTbar",                   "", "stopTreeMaker/AUX", 806.1,  25446993, kGreen));
        samples["WJetsToLNu_HT_600toInf"] =  Samples("WJetsToLNu_HT_600toInf",  "", "stopTreeMaker/AUX", 18.81,  4581841,  kMagenta+1);
        samples["WJetsToLNu_HT_400to600"] =  Samples("WJetsToLNu_HT_400to600",  "", "stopTreeMaker/AUX", 55.61,  4640594,  kMagenta+1);
        samples["WJetsToLNu_HT_200to400"] =  Samples("WJetsToLNu_HT_200to400",  "", "stopTreeMaker/AUX", 471.6,  4686783,  kMagenta+1);
        samples["WJetsToLNu_HT_100to200"] =  Samples("WJetsToLNu_HT_100to200",  "", "stopTreeMaker/AUX", 1817.0, 5262265,  kMagenta+1);
        samples["ZJetsToNuNu_HT_600toInf"] = Samples("ZJetsToNuNu_HT_600toInf", "", "stopTreeMaker/AUX", 4.113,  4463806,  kTeal+4);
        samples["ZJetsToNuNu_HT_400to600"] = Samples("ZJetsToNuNu_HT_400to600", "", "stopTreeMaker/AUX", 11.99,  4433784,  kTeal+4);
        samples["ZJetsToNuNu_HT_200to400"] = Samples("ZJetsToNuNu_HT_200to400", "", "stopTreeMaker/AUX", 100.8,  4546470,  kTeal+4);
        samples["ZJetsToNuNu_HT_100to200"] = Samples("ZJetsToNuNu_HT_100to200", "", "stopTreeMaker/AUX", 372.6,  4986424,  kTeal+4);
        samples["DYJetsToLL_HT_600toInf"] =  Samples("DYJetsToLL_HT_600toInf",  "", "stopTreeMaker/AUX", 2.179,  4493574,  kYellow-7);
        samples["DYJetsToLL_HT_400to600"] =  Samples("DYJetsToLL_HT_400to600",  "", "stopTreeMaker/AUX", 6.546,  4931372,  kYellow-7);
        samples["DYJetsToLL_HT_200to400"] =  Samples("DYJetsToLL_HT_200to400",  "", "stopTreeMaker/AUX", 52.24,  4666496,  kYellow-7);
        samples["DYJetsToLL_HT_100to200"] =  Samples("DYJetsToLL_HT_100to200",  "", "stopTreeMaker/AUX", 194.3,  4054159,  kYellow-7);
        samples["QCD_HT_1000toInf"] =        Samples("QCD_HT_1000toInf",        "", "stopTreeMaker/AUX", 769.7,  1464453,  kBlue);
        samples["QCD_HT_500to1000"] =        Samples("QCD_HT_500to1000",        "", "stopTreeMaker/AUX", 26740,  4063345,  kBlue);
        samples["QCD_HT_250to500"] =         Samples("QCD_HT_250to500",         "", "stopTreeMaker/AUX", 670500, 2668172,  kBlue);
        samples["T_tW"] =                    Samples("T_tW",                    "", "stopTreeMaker/AUX", 35.0,   986100,   kYellow);
        samples["Tbar_tW"] =                 Samples("Tbar_tW",                 "", "stopTreeMaker/AUX", 35.0,   971800,   kYellow);
        samples["TTZ"] =                     Samples("TTZ",                     "", "stopTreeMaker/AUX", 2.2320, 249275,   kGreen+2);

        //signal
        samples["Signal_T1tttt_mGluino1200_mLSP800"] =          Samples("Signal_T1tttt_mGluino1200_mLSP800",          "", "stopTreeMaker/AUX", 0.0856418, 100322,  kRed);
        samples["Signal_T1tttt_mGluino1500_mLSP100"] =          Samples("Signal_T1tttt_mGluino1500_mLSP100",          "", "stopTreeMaker/AUX", 0.0141903, 105679,  kRed);
        samples["Signal_T5tttt_mGluino1300_mStop300_mChi280"] = Samples("Signal_T5tttt_mGluino1300_mStop300_mChi280", "", "stopTreeMaker/AUX", 0.0460525, 16360,   kRed);
        samples["Signal_T5tttt_mGluino1300_mStop300_mCh285"] =  Samples("Signal_T5tttt_mGluino1300_mStop300_mCh285",  "", "stopTreeMaker/AUX", 0.0460525, 35288,   kRed);
        samples["Signal_T1bbbb_mGluino1000_mLSP900"] =          Samples("Signal_T1bbbb_mGluino1000_mLSP900",          "", "stopTreeMaker/AUX", 0.325388,  97134,   kRed);
        samples["Signal_T1bbbb_mGluino1500_mLSP100"] =          Samples("Signal_T1bbbb_mGluino1500_mLSP100",          "", "stopTreeMaker/AUX", 0.0141903, 105149,  kRed);
        samples["Signal_T2tt_mStop425_mLSP325"] =               Samples("Signal_T2tt_mStop425_mLSP325",               "", "stopTreeMaker/AUX", 1.31169,   1039030, kRed);
        samples["Signal_T2tt_mStop500_mLSP325"] =               Samples("Signal_T2tt_mStop500_mLSP325",               "", "stopTreeMaker/AUX", 0.51848,   109591,  kRed);
        samples["Signal_T2tt_mStop650_mLSP325"] =               Samples("Signal_T2tt_mStop650_mLSP325",               "", "stopTreeMaker/AUX", 0.107045,  105672,  kRed);
        samples["Signal_T2tt_mStop850_mLSP100"] =               Samples("Signal_T2tt_mStop850_mLSP100",               "", "stopTreeMaker/AUX", 0.0189612, 102839,  kRed);
        samples["Signal_T2bb_mSbottom600_mLSP580"] =            Samples("Signal_T2bb_mSbottom600_mLSP580",            "", "stopTreeMaker/AUX", 0.174599,  107316,  kRed);
        samples["Signal_T2bb_mSbottom900_mLSP100"] =            Samples("Signal_T2bb_mSbottom900_mLSP100",            "", "stopTreeMaker/AUX", 0.0128895, 102661,  kRed);
    }

    static const double dataLumi = 5.0; // in fb-1
    const double scaledToDataLumi = dataLumi; // in fb-1
    
    double dataScale = 1.0;
    
    std::vector<double> scalesVec;
    
// PU weights are currently empty...
    std::vector<double> puWeights_;
    
    double weightTruNPV(int trunpv);
    int initPUinput(const std::string &puDatafileName));

}

void initMCscales(){
    
    dataScale = scaledToDataLumi/dataLumi;
    std::cout<<"\nscaledToDataLumi : "<<scaledToDataLumi<<"  dataLumi : "<<dataLumi<<"  dataScale : "<<dataScale<<std::endl;
    for(int ib=0; ib<nMC; ib++){
        double perScale = scaledToDataLumi/(nEvtArr[ib]/xSecArr[ib]/1000.);
        scalesVec.push_back(perScale);
        printf("%30s    xSec : %8.3e  nEvt : %10d  scale : %10.8f\n", mcStr[ib].c_str(), xSecArr[ib], nEvtArr[ib], perScale);
    }
    std::cout<<std::endl;
}
