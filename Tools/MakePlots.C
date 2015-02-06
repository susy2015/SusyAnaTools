#include "Plotter.h"

int main()
{
    using namespace std;

    vector<Plotter::FileSummary> vDY_ll, vDY_nunu;
    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150101_225307/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 194.3, 1000.0, 1.0, 4054159));
//    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 52.24, 1000.0, 1.0, 4666496));
//    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224040/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 6.546, 1000.0, 1.0, 4931372));
//    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224252/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 2.179, 1000.0, 1.0, 4493574));

    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 372.6, 1000.0, 1.0, 4986424));
//    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222913/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 100.8, 1000.0, 1.0, 4546470));
//    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150109_230130/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 11.99, 1000.0, 1.0, 4433784));
//    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223010/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", 4.113, 1000.0, 1.0, 4463806));

    Plotter::DatasetSummary dsDY_ll(       "DY#rightarrow#mu#mu",           vDY_ll,   "pdgIdZDec=13");
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

    Plotter::DataCollection dcDY_test(   "single", {{"mht", dsDY_ll}, {"jetsLVec(pt)", dsDY_nunu}, {"genZmass", dsDY_test}, {"genZPt", dsDY_test}});
    Plotter::DataCollection dcDY_ratio(  "ratio",  {{"cleanJetVec(pt)", dsDY_ll}, {"jetsLVec(pt)", dsDY_ll}});
    Plotter::DataCollection dcDY_stack(  "stack",  "mht", {dsDY_ll, dsDY_nunu});

    vector<Plotter::HistSummary> vh;
    vh.push_back(PHS("mht",         {mht},          "", 100, 0, 400,  true,  true,  "M(H_{t}) [GeV]",     "Norm Events"));
    vh.push_back(PHS("mt2",         {mt2},          "", 100, 0, 1000, true,  true,  "MT2 [GeV]",          "Norm Events"));
    vh.push_back(PHS("met",         {met},          "", 100, 0, 400,  true,  true,  "MET [GeV]",          "Norm Events"));
    vh.push_back(PHS("ht",          {ht},           "", 100, 0, 700,  true,  true,  "H_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("nMuons",      {nMuon},        "", 10,  0, 10,   true,  false, "N(#mu)",             "Events"));
    vh.push_back(PHS("jetPt",       {jetPt},        "", 100, 0, 400,  true,  true,  "Jet p_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("cleanJetPt",  {cleanJetPt},   "", 100, 0, 400,  true,  true,  "Cleaned Jet p_{T} [GeV]", "Norm Events"));
    vh.push_back(PHS("muMtw",       {muMtw},        "", 100, 0, 200,  true,  false, "#mu M_{T}(W) [GeV]", "Events"));
    vh.push_back(PHS("nMuGenMatch", {nMuGenMatch},  "", 10,  0, 10,   true,  false, "N(#mu) gen Matched", "Events"));
    vh.push_back(PHS("genZPt",      {genZPt},       "", 100, 0, 300,  true,  true,  "gen Z p_{T} [GeV]",  "Norm Events"));
    vh.push_back(PHS("genZEta",     {genZEta},      "", 100, -3, 3,   false, true,  "gen Z #eta",         "Norm Events"));
    vh.push_back(PHS("genZmass",    {genZmass},     "", 100, 0, 500,  true,  true,  "gen M(Z) [GeV]",     "Norm Events"));

    vh.push_back(PHS("cleanht",          {cleanht},     "",             100, 0,     700,  true, true,  "H_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("cleanht_baseline", {cleanht},     "passBaseline", 100, 0,     700,  true, true,  "H_{T} [GeV]",         "Norm Events"));
    vh.push_back(PHS("cleanmht",         {cleanmht},    "",             100, 0,     400, true, true,  "MH_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("cleanmhtphi",      {cleanmhtphi}, "",             100, -3.14, 3.14, true, true,  "#phi(MH_{T})",        "Norm Events"));

    vh.push_back(PHS("test",  {dcDY_stack, dcDY_test},  "genZmass>80", 100, 0, 1000,   true, true,  "???",         "Norm Events"));
    vh.push_back(PHS("test2", {dcDY_ratio},  "", 100, 0, 500, true, false, "jpt???", "Ratio"));

    vector<vector<Plotter::FileSummary>> vvf = {vDY_ll, vDY_nunu};

    Plotter plotter(vh, vvf);
    plotter.plot();
}
