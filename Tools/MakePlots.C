#include "Plotter.h"
#include "samples.h"

#include <iostream>

int main()
{
    using namespace std;

    AnaSamples::SampleHolder sample;

    vector<AnaSamples::FileSummary> vDY_ll = sample["DYJetsToLL"], vDY_nunu = sample["ZJetsToNuNu"];

    Plotter::DatasetSummary dsDY_ll(       "DY#rightarrow#mu#mu",           vDY_ll,   "pdgIdZDec=13");
    Plotter::DatasetSummary dsDY_ll_gen(   "DY#rightarrow#mu#mu Gen",       vDY_ll,   "pdgIdZDec=13");
    Plotter::DatasetSummary dsDY_llclean(  "DY#rightarrow#mu#mu muRemoved", vDY_ll,   "pdgIdZDec=13");
    Plotter::DatasetSummary dsDY_nunu(     "Z#rightarrow#nu#nu",            vDY_nunu, "");
    Plotter::DatasetSummary dsDY_test(     "test",                          vDY_nunu, "");

    Plotter::DataCollection mht(        "single", "mht",              {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection mt2(        "single", "mt2",              {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection met(        "single", "met",              {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection ht(         "single", "ht",               {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection nMuon(      "single", "cutMuVec(size)",   {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection jetPt(      "single", "jetsLVec(pt)",     {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection muMtw(      "single", "muonsMtw",         {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection nMuGenMatch("single", "genMatchMu(size)", {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection genZPt(     "single", "genZPt",           {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection genZEta(    "single", "genZEta",          {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection genZmass(   "single", "genZmass",         {dsDY_ll, dsDY_nunu});
    Plotter::DataCollection cleanJetPt( "single", "cleanJetVec(pt)",  {dsDY_ll, dsDY_nunu});

    Plotter::DataCollection cleanht(     "single", {{"ht", dsDY_ll},     {"cleanHt", dsDY_nunu},     {"cleanHt", dsDY_llclean}});
    Plotter::DataCollection cleanmht(    "single", {{"mht", dsDY_ll},    {"cleanMHt", dsDY_nunu},    {"cleanMHt", dsDY_llclean}});
    Plotter::DataCollection cleanmhtphi( "single", {{"mhtphi", dsDY_ll}, {"cleanMHtPhi", dsDY_nunu}, {"cleanMHtPhi", dsDY_llclean}});
    Plotter::DataCollection cleanMet(    "single", {{"met", dsDY_ll},    {"cleanMetPt", dsDY_nunu},  {"cleanMetPt", dsDY_llclean}});

    Plotter::DataCollection dcDY_MuEffPt(  "ratio",  {{"genMatchMu(pt)", dsDY_ll},  {"genDecayLVec(pt)", dsDY_ll}});
    Plotter::DataCollection dcDY_MuEffEta( "ratio",  {{"genMatchMu(eta)", dsDY_ll}, {"genDecayLVec(eta)", dsDY_ll}});

    Plotter::DataCollection bestZPt("single",  {{"bestRecoZPt", dsDY_ll}, {"genZPt", dsDY_ll_gen}});
    Plotter::DataCollection bestZM( "single",  {{"bestRecoZM", dsDY_ll},  {"genZmass", dsDY_ll_gen}});


    Plotter::DataCollection dcDY_test(   "single", {{"mht", dsDY_ll}, {"jetsLVec(pt)", dsDY_nunu}, {"genZmass", dsDY_test}, {"genZPt", dsDY_test}});
    Plotter::DataCollection dcDY_ratio(  "ratio",  {{"cleanJetVec(pt)", dsDY_ll}, {"jetsLVec(pt)", dsDY_ll}});
    Plotter::DataCollection dcDY_stack(  "stack",  "mht", {dsDY_ll, dsDY_nunu});

    vector<Plotter::HistSummary> vh;
    vh.push_back(PHS("mht",           {mht},          {1, 2}, "", 100, 0, 400,  true,  true,  "M(H_{t}) [GeV]",     "Norm Events"));
    vh.push_back(PHS("mt2",           {mt2},          {1, 2}, "", 100, 0, 1000, true,  true,  "MT2 [GeV]",          "Norm Events"));
    vh.push_back(PHS("met",           {met},          {1, 2}, "", 100, 0, 400,  true,  true,  "MET [GeV]",          "Norm Events"));
    vh.push_back(PHS("ht",            {ht},           {1, 2}, "", 100, 0, 700,  true,  true,  "H_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("nMuons",        {nMuon},        {1, 2}, "", 10,  0, 10,   true,  false, "N(#mu)",             "Events"));
    vh.push_back(PHS("jetPt",         {jetPt},        {1, 2}, "", 100, 0, 400,  true,  true,  "Jet p_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("cleanJetPt",    {cleanJetPt},   {1, 2}, "", 100, 0, 400,  true,  true,  "Cleaned Jet p_{T} [GeV]", "Norm Events"));
    vh.push_back(PHS("muMtw",         {muMtw},        {1, 2}, "", 100, 0, 200,  true,  false, "#mu M_{T}(W) [GeV]", "Events"));
    vh.push_back(PHS("nMuGenMatch",   {nMuGenMatch},  {1, 2}, "", 10,  0, 10,   true,  false, "N(#mu) gen Matched", "Events"));
    vh.push_back(PHS("genZPt",        {genZPt},       {1, 2}, "", 100, 0, 300,  true,  true,  "gen Z p_{T} [GeV]",  "Norm Events"));
    vh.push_back(PHS("genZEta",       {genZEta},      {1, 2}, "", 100, -3, 3,   false, true,  "gen Z #eta",         "Norm Events"));
    vh.push_back(PHS("genZmass",      {genZmass},     {1, 2}, "", 100, 0, 500,  true,  true,  "gen M(Z) [GeV]",     "Norm Events"));
    vh.push_back(PHS("bestRecoZPi"  , {bestZPt},      {1, 2}, "", 100, 0, 300,  true,  true,  "Z p_{T} [GeV]",      "Norm Events"));    
    vh.push_back(PHS("bestRecoZMass", {bestZM},       {1, 2}, "", 100, 0, 500,  true,  true,  "M(Z) [GeV]",         "Norm Events"));    

    vh.push_back(PHS("cleanht",          {cleanht},     {2, 3}, "",             100, 0,     700,  true, true,  "H_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("cleanht_baseline", {cleanht},     {2, 3}, "passBaseline", 100, 0,     700,  true, true,  "H_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("cleanmht",         {cleanmht},    {2, 3}, "",             100, 0,     400,  true, true,  "MH_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("cleanmhtphi",      {cleanmhtphi}, {2, 3}, "",             100, -3.14, 3.14, true, true,  "#phi(MH_{T})",        "Norm Events"));
    vh.push_back(PHS("cleanmet",         {cleanMet},    {2, 3}, "",             100, 0,     400,  true, true,  "MET [GeV]",           "Norm Events"));

    vh.push_back(PHS("effMuPt",       {dcDY_MuEffPt},   {1, 1}, "",             100, 0,     400, true, false,  "#mu p_{T} [GeV]",  "Efficiency"));
    vh.push_back(PHS("effMuEta",      {dcDY_MuEffEta},  {1, 1}, "",             100, -3.0,  3.0, true, false,  "#mu #eta",         "Efficiency"));

    vh.push_back(PHS("test",  {dcDY_stack, dcDY_test},  {1, 2}, "genZmass>80", 100, 0, 1000,   true, true,  "???",         "Norm Events"));
    vh.push_back(PHS("test2", {dcDY_ratio},  {1, 1}, "", 100, 0, 500, true, false, "jpt???", "Ratio"));

    vector<vector<AnaSamples::FileSummary>> vvf = {vDY_ll, vDY_nunu};

    Plotter plotter(vh, vvf);
    plotter.plot();
}
