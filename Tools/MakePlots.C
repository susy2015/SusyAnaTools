#include "Plotter.h"

int main()
{
    using namespace std;

    vector<Plotter::FileSummary> vDY_ll, vDY_nunu;
    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150101_225307/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 194.3, 1000.0, 1.0, 4054159));
    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 52.24, 1000.0, 1.0, 4666496));
    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224040/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 6.546, 1000.0, 1.0, 4931372));
    vDY_ll.push_back( PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224252/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 2.179, 1000.0, 1.0, 4493574));

    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 372.6, 1000.0, 1.0, 4986424));
    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222913/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 100.8, 1000.0, 1.0, 4546470));
    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150109_230130/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 11.99, 1000.0, 1.0, 4433784));
    vDY_nunu.push_back(PFS("/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223010/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", 4.113, 1000.0, 1.0, 4463806));

    Plotter::DatasetSummary dsDY_ll(  "DY#rightarrow#mu#mu", vDY_ll,   "pdgIdZDec=13");
    Plotter::DatasetSummary dsDY_nunu("Z#rightarrow#nu#nu",  vDY_nunu, "");
    Plotter::DatasetSummary dsDY_test("test",  vDY_nunu, "");

    Plotter::DataCollection dcDY_ll({dsDY_ll});
    Plotter::DataCollection dcDY_nunu({dsDY_nunu});
    Plotter::DataCollection dcDY_test({dsDY_test});

    vector<Plotter::HistSummary> vh;
    vh.push_back(PHS("mht",         {{dcDY_ll, "mht"},              {dcDY_nunu, "mht"}},              "", 100, 0, 1000, true, true,  "M(H_{t}) [GeV]",     "Norm Events"));
    vh.push_back(PHS("mt2",         {{dcDY_ll, "MT2"},              {dcDY_nunu, "MT2"}},              "", 100, 0, 1000, true, true,  "MT2 [GeV]",          "Norm Events"));
    vh.push_back(PHS("met",         {{dcDY_ll, "met"},              {dcDY_nunu, "met"}},              "", 100, 0, 1000, true, true,  "MET [GeV]",          "Norm Events"));
    vh.push_back(PHS("ht",          {{dcDY_ll, "ht"},               {dcDY_nunu, "ht"}},               "", 100, 0, 1000, true, true,  "H_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("nMuons",      {{dcDY_ll, "cutMuVec(size)"},   {dcDY_nunu, "cutMuVec(size)"}},   "", 10,  0, 10,   true, false, "N(#mu)",             "Events"));
    vh.push_back(PHS("jetPt",       {{dcDY_ll, "jetsLVec(pt)"},     {dcDY_nunu, "jetsLVec(pt)"}},     "", 100, 0, 1000, true, true,  "Jet p_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("cleanJetPt",  {{dcDY_ll, "cleanJetVec(pt)"},  {dcDY_nunu, "cleanJetVec(pt)"}},  "", 100, 0, 1000, true, true,  "Cleaned Jet p_{T} [GeV]", "Norm Events"));
    vh.push_back(PHS("muMtw",       {{dcDY_ll, "muonsMtw"},         {dcDY_nunu, "muonsMtw"}},         "", 100, 0, 200,  true, false, "#mu M_{T}(W) [GeV]", "Events"));
    vh.push_back(PHS("nMuGenMatch", {{dcDY_ll, "genMatchMu(size)"}, {dcDY_nunu, "genMatchMu(size)"}}, "", 10,  0, 10,   true, false, "N(#mu) gen Matched", "Events"));
    vh.push_back(PHS("genZPt",      {{dcDY_ll, "genZPt"},           {dcDY_nunu, "genZPt"}},           "", 100, 0, 600,  true, true,  "gen Z p_{T} [GeV]",  "Norm Events"));
    vh.push_back(PHS("genZEta",     {{dcDY_ll, "genZEta"},          {dcDY_nunu, "genZEta"}},          "", 100, -3, 3,   true, true,  "gen Z #eta",         "Norm Events"));
    vh.push_back(PHS("genZmass",    {{dcDY_ll, "genZmass"},         {dcDY_nunu, "genZmass"}},         "", 100, 0, 500,  true, true,  "gen M(Z) [GeV]",     "Norm Events"));

    vh.push_back(PHS("test", {{dcDY_ll, "mht"}, {dcDY_nunu, "jetsLVec(pt)"}, {dcDY_test, "genZmass"}, {dcDY_test, "genZPt"}},  "genZmass>80", 100, 0, 1000,   true, true,  "???",         "Norm Events"));

    vector<vector<Plotter::FileSummary>> vvf = {vDY_ll, vDY_nunu};

    Plotter plotter(vh, vvf);
    plotter.plot();
}
