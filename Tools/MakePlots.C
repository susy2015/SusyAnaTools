#include "Plotter.h"

int main()
{
    using namespace std;

    vector<Plotter::FileSummary> vDY_ll, vDY_nunu;
    vDY_ll.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150101_225307/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "pdgIdZDec=13", 194.3, 1000.0, 1.0, 4054159));
    vDY_ll.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "pdgIdZDec=13", 52.24, 1000.0, 1.0, 4666496));
    vDY_ll.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224040/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "pdgIdZDec=13", 6.546, 1000.0, 1.0, 4931372));
    vDY_ll.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_224252/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "pdgIdZDec=13", 2.179, 1000.0, 1.0, 4493574));

    vDY_nunu.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "", 372.6, 1000.0, 1.0, 4986424));
    vDY_nunu.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222913/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "", 100.8, 1000.0, 1.0, 4546470));
    vDY_nunu.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150109_230130/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "", 11.99, 1000.0, 1.0, 4433784));
    vDY_nunu.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223010/0000/stopFlatNtuples_*.root", "stopTreeMaker/AUX", "", 4.113, 1000.0, 1.0, 4463806));

    vector<vector<Plotter::FileSummary>> vvf;
    vvf.push_back(vDY_ll);
    vvf.push_back(vDY_nunu);

    vector<Plotter::HistSummary> vh;
    vh.push_back(PHS("mht",         "mht",              "", 100, 0, 1000, true, true,  "M(H_{t}) [GeV]",     "Norm Events"));
    vh.push_back(PHS("mt2",         "MT2",              "", 100, 0, 1000, true, true,  "MT2 [GeV]",          "Norm Events"));
    vh.push_back(PHS("met",         "met",              "", 100, 0, 1000, true, true,  "MET [GeV]",          "Norm Events"));
    vh.push_back(PHS("ht",          "ht",               "", 100, 0, 1000, true, true,  "H_{T} [GeV]",        "Norm Events"));
    vh.push_back(PHS("nMuons",      "cutMuVec(size)",   "", 10,  0, 10,   true, false, "N(#mu)",             "Events"));
    vh.push_back(PHS("jetPt",       "jetsLVec(pt)",     "", 100, 0, 1000, true, true,  "Jet p_{T} [GeV]",    "Norm Events"));
    vh.push_back(PHS("muMtw",       "muonsMtw",         "", 100, 0, 200,  true, false, "#mu M_{T}(W) [GeV]", "Events"));
    vh.push_back(PHS("nMuGenMatch", "genMatchMu(size)", "", 10,  0, 10,   true, false, "N(#mu) gen Matched", "Events"));
    vh.push_back(PHS("genZPt",      "genZPt",           "", 100, 0, 500,  true, true,  "gen Z p_{T} [GeV]",  "Norm Events"));
    vh.push_back(PHS("genZEta",     "genZEta",          "", 100, -3, 3,   true, true,  "gen Z #eta",         "Norm Events"));
    vh.push_back(PHS("genZmass",    "genZmass",         "", 100, 0, 500,  true, true,  "gen M(Z) [GeV]",     "Norm Events"));

    vh.push_back(PHS("test", {{vDY_ll, "mht"}, {vDY_nunu, "jetsLVec(pt)"}},  "", 100, 0, 1000,   true, true,  "???",         "Norm Events"));

    Plotter plotter(vh, vvf);
    plotter.plot();
}
