#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <vector>
#include <string>

class ZInvPloter
{
public:

    class HistSummary
    {
        TH1 *hist;
        std::string histname;
        int nBins;
        double low, high;
        std::string xAxisLabel, yAxisLabel;
    };

    class FileSummary
    {
        std::string name;
        double xsec, lumi, kfactor, nEvts;
    };

    void plot();

private:
    std::vector<BLAH> hists;
    std::vector<BLAH> trees;
    
    
    void createHistsFromTuple();
};
