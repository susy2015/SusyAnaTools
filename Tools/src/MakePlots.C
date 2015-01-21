#include "Plotter.h"

int main()
{
    std::vector<Plotter::HistSummary> vh;
    vh.push_back(Plotter::HistSummary("mht", "mht", "", 100, 0, 1000, true, "M(H_{t}) [GeV]", "Stuff"));
    vh.push_back(Plotter::HistSummary("mt2", "MT2", "", 100, 0, 1000, true, "MT2 [GeV]", "Stuff"));

    std::vector<Plotter::FileSummary> vf, vf2, vf3;
    vf.push_back( Plotter::FileSummary("DY#rightarrow ll", "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 1.0, 1.0, 1.0, 1.0));
    vf2.push_back(Plotter::FileSummary("DY#rightarrow ll", "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "MT2>120", 1.0, 1.0, 1.0, 1.0));
    vf3.push_back(Plotter::FileSummary("DY#rightarrow #nu #nu", "/eos/uscms/store/user/pastika/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_222641/0000/stopFlatNtuples_1.root", "stopTreeMaker/AUX", "", 1.0, 1.0, 1.0, 1.0));

    std::vector<std::vector<Plotter::FileSummary>> vvf;
    vvf.push_back(vf);
    vvf.push_back(vf2);
    vvf.push_back(vf3);

    Plotter plotter(vh, vvf);
    plotter.plot();
}
