#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include "TString.h"
/*
  This file is used to define MC/data samples related information. 
  Here currently we have MC sample names/keywords, cross-sections and number of events.
  PU related information are empty now.
*/

const TString treeStrTtype = "stopTreeMaker";

const std::string      mcStr[] = {   "TTbar", 
                                "WJetsToLNu_HT_600toInf", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_100to200", 
                                "ZJetsToNuNu_HT_600toInf", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_100to200", 
                                "DYJetsToLL_HT_600toInf", "DYJetsToLL_HT_400to600", "DYJetsToLL_HT_200to400", "DYJetsToLL_HT_100to200",  
                                "QCD_HT_1000toInf",  "QCD_HT_500to1000", "QCD_HT_250to500", 
                                "T_tW",  "Tbar_tW",  "TTZ", 
/*T1tttt 13TeV*/
                                "Signal_T1tttt_mGluino1200_mLSP800",
                                "Signal_T1tttt_mGluino1500_mLSP100", 
/*T5tttt 13TeV*/
                                "Signal_T5tttt_mGluino1300_mStop300_mChi280", "Signal_T5tttt_mGluino1300_mStop300_mCh285",
/*T1bbbb 13TeV*/                
                                "Signal_T1bbbb_mGluino1000_mLSP900",
                                "Signal_T1bbbb_mGluino1500_mLSP100",
/*Stop 13TeV*/
                                "Signal_T2tt_mStop425_mLSP325",
                                "Signal_T2tt_mStop500_mLSP325",
                                "Signal_T2tt_mStop650_mLSP325",
                                "Signal_T2tt_mStop850_mLSP100",
/*Sbottom 13TeV*/
                                "Signal_T2bb_mSbottom600_mLSP580",
                                "Signal_T2bb_mSbottom900_mLSP100"
};

const double    xSecArr[] = {     806.1,        
                                  23.14,            68.40,          580.1,           2235.0,               
                                  5.224,            15.23,          128.0,           473.2,        
                                  2.767,            8.313,          66.34,           246.8,
                                  769.7,            26740,          670500,
                                  35.0,      35.0,  2.2320,
/*T1tttt 13TeV*/
                                        0.0856418,
                                        0.0141903,
/*T5tttt 13TeV*/
                                        0.0460525,                0.0460525,
/*T1bbbb 13TeV*/
                                         0.325388,
                                        0.0141903,
/*Stop 13TeV*/
                                          1.31169,
                                          0.51848,
                                         0.107045,
                                        0.0189612,
/*Sbottom 13TeV*/
                                         0.174599,
                                        0.0128895, 
};

const int       nEvtArr[] = {   25446993,
                                 4581841,                    4640594,                4936077,               5262265,
                                 4463806,                    4433784,                4546470,               4986424,
                                 4493574,                    4931372,                4666496,               4054159,
                                 1464453,                    4063345,                2668172,
                                  986100,                     971800,                 249275, /* not produced yet */
/*T1tttt 13TeV*/
                                       100322,
                                       105679,
/*T5tttt 13TeV*/
                                        16360,                    35288,
/*T1bbbb 13TeV*/
                                        97134,
                                       210298,
/*Stop 13TeV*/
                                      1039030,
                                       109591,
                                       105672,
                                       102839,
/*Sbottom 13TeV*/
                                       107316,
                                       102661,
};

static const int colors[] = {    kGreen,
                                 kMagenta+1,            kMagenta+1,            kMagenta+1,            kMagenta+1,
                                 kTeal+4,               kTeal+4,               kTeal+4,               kTeal+4,
                                 kYellow-7,             kYellow-7,             kYellow-7,             kYellow-7,
                                 kBlue,            kBlue,          kBlue,
                                 kYellow,          kYellow,     kGreen+2,
/*T1tttt 13TeV*/
                                        kRed,
                                        kRed,
/*T5tttt 13TeV*/
                                        kRed,                     kRed,
/*T1bbbb 13TeV*/
                                        kRed,
                                        kRed,
/*Stop 13TeV*/
                                        kRed,
                                        kRed,
                                        kRed,
                                        kRed,
/*Sbottom 13TeV*/
                                        kRed,
                                        kRed
};

const int nMC = sizeof(mcStr)/sizeof(mcStr[0]);

static const double dataLumi = 5.0; // in fb-1
const double scaledToDataLumi = dataLumi; // in fb-1

double dataScale = 1.0;

std::vector<double> scalesVec;

// PU weights are currently empty...
std::vector<double> puWeights_;

double weightTruNPV(int trunpv);
int initPUinput(const std::string &puDatafileName, const std::string &puDatahistName);

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
