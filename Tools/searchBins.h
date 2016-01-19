#ifndef SEARCH_BINS_H
#define SEARCH_BINS_H


#include <vector>
#include <iostream>

/*
const double nTops_SR_lo[]   = { 0,  0,  0,  1,  1,  1,  2,  2,  2,  3,  3,  3 };
const double nTops_SR_hi[]   = { 1,  1,  1,  2,  2,  2,  3,  3,  3, -1, -1, -1 };

const double nbJets_SR_lo[]  = { 1,  2,  3,  1,  2,  3,  1,  2,  3,  1,  2,  3 };
const double nbJets_SR_hi[]  = { 2,  3, -1,  2,  3, -1,  2,  3, -1,  2,  3, -1 };
*/
extern double nTops_SR_lo[];
extern double nTops_SR_hi[];

extern double nbJets_SR_lo[];
extern double nbJets_SR_hi[];
/*
const std::string keyStr_nTops_SR[]  = { "EQ0", "EQ0", "EQ0", "EQ1", "EQ1", "EQ1", "EQ2", "EQ2", "EQ2", "LE3", "LE3", "LE3" };
const std::string disStr_nTops_SR[]  = {  "=0",  "=0",  "=0",  "=1",  "=1",  "=1",  "=2",  "=2",  "=2", ">=3", ">=3", ">=3" };

const std::string keyStr_nbJets_SR[] = { "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3" };
const std::string disStr_nbJets_SR[] = {  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3" };
*/
extern std::string keyStr_nTops_SR[];
extern std::string disStr_nTops_SR[];

extern std::string keyStr_nbJets_SR[];
extern std::string disStr_nbJets_SR[];

extern int nSR;

extern double met_SR_lo[];
extern double met_SR_hi[];

extern std::string keyStr_met_SR[];
extern std::string disStr_met_SR[];
/* VERY crude binning on MET for the ntop=0 scenario. Not used for this time!
const std::vector<std::vector<double> > vMT2_vs_met_SR_0 { // nb=1, ntop=0
                                                           // MT2 lo 
                                                           {200, 200, 200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400, 500, 600},
                                                           // met hi 
                                                           {300, 400, 500, 600,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_1 { // nb=2, ntop=0
                                                           // MT2 lo 
                                                           {200, 200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400, 500},
                                                           // met hi 
                                                           {300, 400, 500,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_2 { // nb>=3, ntop=0
                                                           // MT2 lo 
                                                           {200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1},
                                                           // met lo 
                                                           {200, 300},
                                                           // met hi 
                                                           {300,  -1},
                                                         };
*/

extern std::vector<std::vector<double> > vMT2_vs_met_SR_3;

extern std::vector<std::vector<double> > vMT2_vs_met_SR_4;

extern std::vector<std::vector<double> > vMT2_vs_met_SR_5;
extern std::vector<std::vector<double> > vMT2_vs_met_SR_6;
extern std::vector<std::vector<double> > vMT2_vs_met_SR_7;

extern std::vector<std::vector<double> > vMT2_vs_met_SR_8;
extern std::vector<std::vector<double> > vMT2_vs_met_SR_9;
extern std::vector<std::vector<double> > vMT2_vs_met_SR_10;
extern std::vector<std::vector<double> > vMT2_vs_met_SR_11;
extern std::vector<std::vector<std::vector<double> > > vMT2_vs_met_all_SR;

extern double max_MT2_for_binEdge    ;
extern double max_met_for_binEdge    ;
extern double max_MT2_for_binCent    ;
extern double max_met_for_binCent    ;
extern double pseudoMax_MT2_for_hist ;
extern double pseudoMax_met_for_hist ;

extern double MT2_SR_Arr[] ;
extern double met_SR_Arr[] ;

extern int nSR_MT2 ;
extern int nSR_met ;
extern int nSR_met_alt ;

extern std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

extern int nTotBins;

void build_MT2_met_Binning_forTH2Poly(std::vector<std::vector<std::vector<double> > > & outBinning);

void build_MT2_met_Binning(std::vector<std::vector<std::vector<double> > > & outBinning);

int find_Binning_Index(int ibJet, int iTop, double MT2, double met);
struct searchBinDef{
   double bJet_lo, top_lo, met_lo, MT2_lo;
   double bJet_hi, top_hi, met_hi, MT2_hi;
   int idx_SR, idx_vMT2_vs_met;
};

void find_BinBoundaries(int inputIdx, searchBinDef & outBinDef);

void print_searchBins();

void print_searchBins_headerstr(const std::string addstr="\\\\");

std::string get_searchBins_defstr(const int binIdx, const std::string addstr="\\\\");
void print_searchBins_latex();
#endif
