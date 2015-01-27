#include "Plotter.h"

int main()
{
    std::vector<Plotter::HistSummary> vh;
    vh.push_back(PHS("mht",         "mht",              "", 100, 0, 1000, true, "M(H_{t}) [GeV]",     "Events"));
    vh.push_back(PHS("mt2",         "MT2",              "", 100, 0, 1000, true, "MT2 [GeV]",          "Events"));
    vh.push_back(PHS("met",         "met",              "", 100, 0, 1000, true, "MET [GeV]",          "Events"));
    vh.push_back(PHS("ht",          "ht",               "", 100, 0, 1000, true, "H_{T} [GeV]",        "Events"));
    vh.push_back(PHS("nMuons",      "nMuons",           "", 10,  0, 10,   true, "N(#mu)",             "Events"));
    vh.push_back(PHS("jetPt",       "jetsLVec(pt)",     "", 100, 0, 1000, true, "Jet p_{T} [GeV]",    "Events"));
    vh.push_back(PHS("muMtw",       "muonsMtw",         "", 100, 0, 200,  true, "#mu M_{T}(W) [GeV]", "Events"));
    vh.push_back(PHS("nMuGenMatch", "genMatchMu(size)", "", 10,  0, 10,   true, "N(#mu) gen Matched", "Events"));

    std::vector<Plotter::FileSummary> vf, vf2, vf3;
    vf.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150101_225307/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 194.3, 1000.0, 1.0, 4054159));
    vf.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 52.24, 1000.0, 1.0, 4666496));
    vf.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 6.546, 1000.0, 1.0, 4931372));
    vf.push_back( PFS("DY#rightarrowll", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223846/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 2.179, 1000.0, 1.0, 4493574));

    vf2.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 372.6, 1000.0, 1.0, 4986424));
    vf2.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222913/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 100.8, 1000.0, 1.0, 4546470));
    vf2.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/lhx/PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150109_230130/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 11.99, 1000.0, 1.0, 4433784));
    vf2.push_back(PFS("DY#rightarrow#nu#nu", "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223010/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 4.113, 1000.0, 1.0, 4463806));


    std::vector<std::vector<Plotter::FileSummary>> vvf;
    vvf.push_back(vf);
    vvf.push_back(vf2);
    //vvf.push_back(vf3);

    Plotter plotter(vh, vvf);
    plotter.plot();
}
