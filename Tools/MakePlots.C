#include "Plotter.h"
#include "samples.h"

#include <getopt.h>
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace std;

    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"plot",            no_argument, 0, 'p'},
        {"savehist",        no_argument, 0, 's'},
        {"fromFile",        no_argument, 0, 'f'},
        {"condor",          no_argument, 0, 'c'},
        {"histFile",  required_argument, 0, 'H'},
        {"dataSets",  required_argument, 0, 'D'},
    };

    bool doPlots = true, doSave = true, fromTuple = true, runOnCondor = false;
    string histFile = "", dataSets = "", sampleloc = AnaSamples::fileDir;

    while((opt = getopt_long(argc, argv, "psfcH:D:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 'p':
            if(doPlots) doSave  = false;
            else        doPlots = true;
            break;

        case 's':
            if(doSave) doPlots = false;
            else       doSave  = true;
            break;

        case 'f':
            fromTuple = false;
            break;

        case 'c':
            runOnCondor = true;
            break;

        case 'H':
            histFile = optarg;
            break;

        case 'D':
            dataSets = optarg;
            break;
        }
    }

    //if running on condor override all optional settings
    if(runOnCondor)
    {
        histFile = "histoutput_" + dataSets + ".root";
        doSave = true;
        doPlots = false;
        fromTuple = true;
        sampleloc = "";
    }

    AnaSamples::SampleSet        ss(sampleloc);
    AnaSamples::SampleCollection sc(ss);

    const double zAcc = 1.0;
//    const double zAcc = 0.5954;
//    const double zAcc = 0.855;
    const double znunu_mumu_ratio = 5.942;

    map<string, vector<AnaSamples::FileSummary>> fileMap;

    //Select approperiate datasets here
    if(dataSets.size() == 0)
    {
        fileMap["DYJetsToLL"]  = sc["DYJetsToLL"];
        fileMap["ZJetsToNuNu"] = sc["ZJetsToNuNu"];
    }
    else if(sc[dataSets] != sc.null())
    {
        fileMap[dataSets] = sc[dataSets];
    }
    else if(ss[dataSets] != ss.null())
    {
        for(const auto& fsnp : sc)
        {
            if(dataSets.find(fsnp.first) !=std::string::npos)
            {
                fileMap[fsnp.first] = {ss[dataSets]};
                break;
            }
        }
    }

    Plotter::DatasetSummary dsDY_ll(        "DY#rightarrow#mu#mu",                   fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "");
    Plotter::DatasetSummary dsDY_ll_zWeight("DY#rightarrow#mu#mu no #mu, Z eff",     fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "zEffWgt");
    Plotter::DatasetSummary dsDY_ll_zAcc(   "DY#rightarrow#mu#mu no #mu, Z eff+acc", fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "zEffWgt;zAccWgt");
    Plotter::DatasetSummary dsDY_ll_gen(    "DY#rightarrow#mu#mu Gen",               fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "");
    Plotter::DatasetSummary dsDY_llclean(   "DY#rightarrow#mu#mu no #mu",            fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "");

    Plotter::DatasetSummary dsDY_ll_scaled(        "DY#rightarrow#mu#mu",                   fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "",                znunu_mumu_ratio / zAcc);
    Plotter::DatasetSummary dsDY_ll_zWeight_scaled("DY#rightarrow#mu#mu no #mu, Z eff",     fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "zEffWgt",         znunu_mumu_ratio / zAcc);
    Plotter::DatasetSummary dsDY_ll_zAcc_scaled(   "DY#rightarrow#mu#mu no #mu, Z eff+acc", fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "zEffWgt;zAccWgt", znunu_mumu_ratio / zAcc);
    Plotter::DatasetSummary dsDY_llclean_scaled(   "DY#rightarrow#mu#mu no #mu",            fileMap["DYJetsToLL"],   "pdgIdZDec=13;passMuZinvSel", "",                znunu_mumu_ratio / zAcc);

    Plotter::DatasetSummary dsDY_ll_forEff( "DY#rightarrow#mu#mu no Sel",               fileMap["DYJetsToLL"],  "pdgIdZDec=13", "");
    Plotter::DatasetSummary dsDY_nunu(      "Z#rightarrow#nu#nu",                fileMap["ZJetsToNuNu"], "passLeptVeto", "");
    Plotter::DatasetSummary dsDY_test(      "test",                              fileMap["ZJetsToNuNu"], "passLeptVeto", "");

    vector<Plotter::HistSummary> vh;

    //Basic Raw compairson plots  
    vh.push_back(PHS("mht",         {PDC("single", "mht",                   {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1000,  true,  true,  "M(H_{t}) [GeV]",          "Norm Events"));
    vh.push_back(PHS("mt2",         {PDC("single", "mt2",                   {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1000,  true,  true,  "MT2 [GeV]",               "Norm Events"));
    vh.push_back(PHS("met",         {PDC("single", "met",                   {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1500,  true,  true,  "MET [GeV]",               "Norm Events"));
    vh.push_back(PHS("ht",          {PDC("single", "ht",                    {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 2000,  true,  true,  "H_{T} [GeV]",             "Norm Events"));
    vh.push_back(PHS("nMuons",      {PDC("single", "cutMuVec(size)",        {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 10,  0, 10,    true,  false, "N(#mu)",                  "Events"));
    vh.push_back(PHS("jetPt",       {PDC("single", "jetsLVec(pt)",          {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1500,  true,  true,  "Jet p_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("nJets",       {PDC("single", "cntNJetsPt30Eta24",     {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 10,  0, 20,    true,  false, "N(jets)",                 "Events"));
    vh.push_back(PHS("muPt",        {PDC("single", "cutMuVec(pt)",          {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1500,  true,  true,  "#mu p_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("cleanJetPt",  {PDC("single", "cleanJetVec(pt)",       {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1500,  true,  true,  "Cleaned Jet p_{T} [GeV]", "Norm Events"));
    vh.push_back(PHS("muMtw",       {PDC("single", "muonsMtw",              {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 600,   true,  false, "#mu M_{T}(W) [GeV]",      "Events"));
    vh.push_back(PHS("nMuGenMatch", {PDC("single", "genMatchMuInAcc(size)", {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 10,  0, 10,    true,  false, "N(#mu) gen Matched",      "Events"));
    vh.push_back(PHS("genZPt",      {PDC("single", "genZPt",                {dsDY_ll, dsDY_nunu, dsDY_ll_forEff})}, {3, 2}, "", 100, 0, 1500,  true,  true,  "gen Z p_{T} [GeV]",       "Norm Events"));
    //vh.push_back(PHS("genMuPt",     {PDC("single", "genMu(pt)",             {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1500,  true,  true,  "gen #mu p_{T} [GeV]",     "Norm Events"));
    vh.push_back(PHS("genMuEta",    {PDC("single", "genMu(eta)",            {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, -3, 3,    true,  true,  "gen #mu #eta",            "Norm Events"));
    vh.push_back(PHS("genZEta",     {PDC("single", "genZEta",               {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, -3, 3,    false, true,  "gen Z #eta",              "Norm Events"));
    vh.push_back(PHS("genZmass",    {PDC("single", "genZmass",              {dsDY_ll, dsDY_nunu})}, {1, 2}, "", 100, 0, 1000,  true,  true,  "gen M(Z) [GeV]",          "Norm Events"));

    // Simple Reco vs. Gen plots
    vh.push_back(PHS("bestRecoZPt"  , {PDC("single",  {{"bestRecoZPt", dsDY_ll}, {"genZPt", dsDY_ll_gen}})},    {1, 2}, "", 100, 0, 1500,  true,  true,  "Z p_{T} [GeV]",           "Norm Events"));    
    vh.push_back(PHS("bestRecoZMass", {PDC("single",  {{"bestRecoZM", dsDY_ll},  {"genZmass", dsDY_ll_gen}})},  {1, 2}, "", 100, 0, 1000,  true,  true,  "M(Z) [GeV]",              "Norm Events"));    


    // Here are the interesting plots for the closure test 

    // DataCollections which are reused multiple times are defined "upfront" to save time 
    Plotter::DataCollection cleanht(     "single", {{"ht", dsDY_ll},                       {"cleanHt", dsDY_nunu},            {"cleanHt", dsDY_llclean},                {"cleanHt", dsDY_ll_zWeight},               {"cleanHt", dsDY_ll_zAcc}              });
    Plotter::DataCollection cleanmht(    "single", {{"mht", dsDY_ll},                      {"cleanMHt", dsDY_nunu},           {"cleanMHt", dsDY_llclean},               {"cleanMHt", dsDY_ll_zWeight},              {"cleanMHt", dsDY_ll_zAcc}             });
    Plotter::DataCollection cleanmhtphi( "single", {{"mhtphi", dsDY_ll},                   {"cleanMHtPhi", dsDY_nunu},        {"cleanMHtPhi", dsDY_llclean},            {"cleanMHtPhi", dsDY_ll_zWeight},           {"cleanMHtPhi", dsDY_ll_zAcc}          });
    Plotter::DataCollection cleanMet(    "single", {{"met", dsDY_ll},                      {"cleanMetPt", dsDY_nunu},         {"cleanMetPt", dsDY_llclean},             {"cleanMetPt", dsDY_ll_zWeight},            {"cleanMetPt", dsDY_ll_zAcc}           });
    Plotter::DataCollection nCleanJet(   "single", {{"cntNJetsPt30Eta24Zinv", dsDY_ll},    {"cntNJetsPt30Eta24", dsDY_nunu},  {"cntNJetsPt30Eta24Zinv", dsDY_llclean},  {"cntNJetsPt30Eta24Zinv", dsDY_ll_zWeight}, {"cntNJetsPt30Eta24Zinv", dsDY_ll_zAcc}});
    Plotter::DataCollection nTop(        "single", {{"nTopCandSortedCntZinv", dsDY_ll},    {"nTopCandSortedCnt", dsDY_nunu},  {"nTopCandSortedCntZinv", dsDY_llclean},  {"nTopCandSortedCntZinv", dsDY_ll_zWeight}, {"nTopCandSortedCntZinv", dsDY_ll_zAcc}});
    Plotter::DataCollection nBottom(     "single", {{"cntCSVSZinv", dsDY_ll},              {"cntCSVS", dsDY_nunu},            {"cntCSVSZinv", dsDY_llclean},            {"cntCSVSZinv", dsDY_ll_zWeight},           {"cntCSVSZinv", dsDY_ll_zAcc}          });

    Plotter::DataCollection scaled_cleanht(     "single", {{"ht", dsDY_ll_scaled},                       {"cleanHt", dsDY_nunu},            {"cleanHt", dsDY_llclean_scaled},                {"cleanHt", dsDY_ll_zWeight_scaled},              {"cleanHt", dsDY_ll_zAcc_scaled}                });
    Plotter::DataCollection scaled_cleanmht(    "single", {{"mht", dsDY_ll_scaled},                      {"cleanMHt", dsDY_nunu},           {"cleanMHt", dsDY_llclean_scaled},               {"cleanMHt", dsDY_ll_zWeight_scaled},             {"cleanMHt", dsDY_ll_zAcc_scaled}               });
    Plotter::DataCollection scaled_cleanmhtphi( "single", {{"mhtphi", dsDY_ll_scaled},                   {"cleanMHtPhi", dsDY_nunu},        {"cleanMHtPhi", dsDY_llclean_scaled},            {"cleanMHtPhi", dsDY_ll_zWeight_scaled},          {"cleanMHtPhi", dsDY_ll_zAcc_scaled}            });
    Plotter::DataCollection scaled_cleanMet(    "single", {{"met", dsDY_ll_scaled},                      {"cleanMetPt", dsDY_nunu},         {"cleanMetPt", dsDY_llclean_scaled},             {"cleanMetPt", dsDY_ll_zWeight_scaled},           {"cleanMetPt", dsDY_ll_zAcc_scaled}             });
    Plotter::DataCollection scaled_nCleanJet(   "single", {{"cntNJetsPt30Eta24Zinv", dsDY_ll_scaled},    {"cntNJetsPt30Eta24", dsDY_nunu},  {"cntNJetsPt30Eta24Zinv", dsDY_llclean_scaled},  {"cntNJetsPt30Eta24Zinv", dsDY_ll_zWeight_scaled}, {"cntNJetsPt30Eta24Zinv", dsDY_ll_zAcc_scaled} });
    Plotter::DataCollection scaled_nTop(        "single", {{"nTopCandSortedCntZinv", dsDY_ll_scaled},    {"nTopCandSortedCnt", dsDY_nunu},  {"nTopCandSortedCntZinv", dsDY_llclean_scaled},  {"nTopCandSortedCntZinv", dsDY_ll_zWeight_scaled}, {"nTopCandSortedCntZinv", dsDY_ll_zAcc_scaled} });
    Plotter::DataCollection scaled_nBottom(     "single", {{"cntCSVSZinv", dsDY_ll_scaled},              {"cntCSVS", dsDY_nunu},            {"cntCSVSZinv", dsDY_llclean_scaled},            {"cntCSVSZinv", dsDY_ll_zWeight_scaled},          {"cntCSVSZinv", dsDY_ll_zAcc_scaled}            });

    vh.push_back(PHS("norm_cleanht",            {cleanht},      {5, 2}, "",                                    100, 0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_baseline_cleanht",   {cleanht},      {5, 2}, "passZinvBaseline",                    50,  0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_nb_0_cleanht",       {cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_nb_1_cleanht",       {cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_nb_2_cleanht",       {cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_nb_3_cleanht",       {cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     2000, true, true,  "H_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("norm_cleanmht",           {cleanmht},     {5, 2}, "",                                    100, 0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_baseline_cleanmht",  {cleanmht},     {5, 2}, "passZinvBaseline",                    50,  0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_nb_0_cleanmht",      {cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_nb_1_cleanmht",      {cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_nb_2_cleanmht",      {cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_nb_3_cleanmht",      {cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     1500, true, true,  "MH_{T} [GeV]",   "Norm Events"));
    vh.push_back(PHS("norm_cleanmet",           {cleanMet},     {5, 2}, "",                                    100, 0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_baseline_cleanmet",  {cleanMet},     {5, 2}, "passZinvBaseline",                    50,  0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_nb_0_cleanmet",      {cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_nb_1_cleanmet",      {cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_nb_2_cleanmet",      {cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_nb_3_cleanmet",      {cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     1500, true, true,  "MET [GeV]",      "Norm Events"));
    vh.push_back(PHS("norm_nTop",               {nTop},         {5, 2}, "",                                    10,  0,       10, true, true,  "N(t)",           "Norm Events"));
    vh.push_back(PHS("norm_baseline_nTop",      {nTop},         {5, 2}, "passZinvBaseline",                    10,  0,       10, true, true,  "N(t)",           "Norm Events"));
    vh.push_back(PHS("norm_cleannJet",          {nCleanJet},    {5, 2}, "",                                    20,  0,       20, true, true,  "N(jet)",         "Norm Events"));
    vh.push_back(PHS("norm_baseline_cleannJet", {nCleanJet},    {5, 2}, "passZinvBaseline",                    20,  0,       20, true, true,  "N(jet)",         "Norm Events"));
    vh.push_back(PHS("norm_nBottom",            {nBottom},      {5, 2}, "",                                    10,  0,       10, true, true,  "N(b)",           "Norm Events"));
    vh.push_back(PHS("norm_baseline_nBottom,",  {nBottom},      {5, 2}, "passZinvBaseline",                    10,  0,       10, true, true,  "N(b)",           "Norm Events"));
    vh.push_back(PHS("norm_cleanmhtphi",        {cleanmhtphi},  {5, 2}, "",                                    100, -3.14, 3.14, true, true,  "#phi(MH_{T})",   "Norm Events"));

    vh.push_back(PHS("cleanht",            {scaled_cleanht},      {5, 2}, "",                                    100, 0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("baseline_cleanht",   {scaled_cleanht},      {5, 2}, "passZinvBaseline",                    50,  0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("nb_0_cleanht",       {scaled_cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("nb_1_cleanht",       {scaled_cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("nb_2_cleanht",       {scaled_cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("nb_3_cleanht",       {scaled_cleanht},      {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     2000, true, false,  "H_{T} [GeV]",    "Events"));
    vh.push_back(PHS("cleanmht",           {scaled_cleanmht},     {5, 2}, "",                                    100, 0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("baseline_cleanmht",  {scaled_cleanmht},     {5, 2}, "passZinvBaseline",                    50,  0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("nb_0_cleanmht",      {scaled_cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("nb_1_cleanmht",      {scaled_cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("nb_2_cleanmht",      {scaled_cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("nb_3_cleanmht",      {scaled_cleanmht},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     1500, true, false,  "MH_{T} [GeV]",   "Events"));
    vh.push_back(PHS("cleanmet",           {scaled_cleanMet},     {5, 2}, "",                                    100, 0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("baseline_cleanmet",  {scaled_cleanMet},     {5, 2}, "passZinvBaseline",                    50,  0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("nb_0_cleanmet",      {scaled_cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=0", 100, 0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("nb_1_cleanmet",      {scaled_cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=1", 50,  0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("nb_2_cleanmet",      {scaled_cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv=2", 25,  0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("nb_3_cleanmet",      {scaled_cleanMet},     {5, 2}, "passZinvBaselineNoTag;cntCSVSZinv>2", 15,  0,     1500, true, false,  "MET [GeV]",      "Events"));
    vh.push_back(PHS("nTop",               {scaled_nTop},         {5, 2}, "",                                    10,  0,       10, true, false,  "N(t)",           "Events"));
    vh.push_back(PHS("baseline_nTop",      {scaled_nTop},         {5, 2}, "passZinvBaseline",                    10,  0,       10, true, false,  "N(t)",           "Events"));
    vh.push_back(PHS("cleannJet",          {scaled_nCleanJet},    {5, 2}, "",                                    20,  0,       20, true, false,  "N(jet)",         "Events"));
    vh.push_back(PHS("baseline_cleannJet", {scaled_nCleanJet},    {5, 2}, "passZinvBaseline",                    20,  0,       20, true, false,  "N(jet)",         "Events"));
    vh.push_back(PHS("nBottom",            {scaled_nBottom},      {5, 2}, "",                                    10,  0,       10, true, false,  "N(b)",           "Events"));
    vh.push_back(PHS("baseline_nBottom,",  {scaled_nBottom},      {5, 2}, "passZinvBaseline",                    10,  0,       10, true, false,  "N(b)",           "Events"));
    vh.push_back(PHS("cleanmhtphi",        {scaled_cleanmhtphi},  {5, 2}, "",                                    100, -3.14, 3.14, true, false,  "#phi(MH_{T})",   "Events"));

    // Plots the information needed to calculate acceptance corrections, plot itself is meaningless 
    vh.push_back(PHS("accInfo",  {PDC("single", {{"genMuInAcc(size)", dsDY_ll_forEff}, {"genMu(size)", dsDY_ll_forEff}})},    {1, 2}, "",  20,  0,       20, true, false,  "gen N(#mu)",     "Events"));

    vh.push_back(PHS("genMuPt",  {PDC("single", {{"genMuInAcc(pt)", dsDY_ll_forEff}, {"genMu(pt)", dsDY_ll_forEff}})},    {1, 2}, "",  150,  0,       1500, true, false,  "gen #mu p_{T} [GeV]",     "Events"));

    // Plots for calculation of muon efficiency 
    vh.push_back(PHS("effMuPt",  {PDC("ratio",  {{"genMatchMuInAcc(pt)", dsDY_ll_forEff},  {"genMuInAcc(pt)", dsDY_ll_forEff}})},   {1, 1}, "", 200, 0,     2000, false, false,  "#mu p_{T} [GeV]",  "Efficiency"));
    vh.push_back(PHS("effMuEta", {PDC("ratio",  {{"genMatchMuInAcc(eta)", dsDY_ll_forEff}, {"genMuInAcc(eta)", dsDY_ll_forEff}})},  {1, 1}, "", 100, -3.0,  3.0,  false, false,  "#mu #eta",         "Efficiency"));

    // Plots of weight and efficiencies used as sanity checks
    vh.push_back(PHS("zEffWgt", {PDC("single", "zEffWgt",  {dsDY_ll})},  {1, 1}, "", 100, 0.0,  20.0,  false, false,  "Z Eff Weight",         ""));
    vh.push_back(PHS("zEff",    {PDC("single", "zEff",     {dsDY_ll})},  {1, 1}, "", 100, 0.0,  1.0,   false, false,  "Z Eff Weight",         ""));

    // Test plots, these are simply meaningless demonstrations of features
    Plotter::DataCollection dcDY_test(   "single", {{"mht", dsDY_ll}, {"jetsLVec(pt)", dsDY_nunu}, {"genZmass", dsDY_test}, {"genZPt", dsDY_test}});
    Plotter::DataCollection dcDY_ratio(  "ratio",  {{"cleanJetVec(pt)", dsDY_ll}, {"jetsLVec(pt)", dsDY_ll}});
    Plotter::DataCollection dcDY_stack(  "stack",  "mht", {dsDY_ll, dsDY_nunu});

    vh.push_back(PHS("test",  {dcDY_stack, dcDY_test},  {1, 2}, "genZmass>80", 100, 0, 1000,   true, true,  "???",         "Norm Events"));
    vh.push_back(PHS("test2", {dcDY_ratio},  {1, 1}, "", 100, 0, 500, true, false, "jpt???", "Ratio"));

    vector<vector<AnaSamples::FileSummary>> vvf;
    for(auto& fsVec : fileMap) vvf.push_back(fsVec.second);

    Plotter plotter(vh, vvf, fromTuple, histFile);
    if(doSave)  plotter.saveHists();
    if(doPlots) plotter.plot();
}
