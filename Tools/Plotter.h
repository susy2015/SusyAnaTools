#ifndef STOP_PLOTTER_H
#define STOP_PLOTTER_H

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TProfile.h"

#include <vector>
#include <utility>
#include <string>

#include "NTupleReader.h"

class Plotter
{
private:
    class Cut
    {
    public:
        std::string name;
        char type;
        double val, val2;
        bool inverted;

        Cut(std::string s, char t, double v, double v2 = 0);
        bool passCut(const NTupleReader& tr) const;
    };
    
    class Cuttable
    {
    public:
        Cuttable() {}
        Cuttable(const std::string& c);
        bool passCuts(const NTupleReader& tr) const;
        void setCuts(const std::string& c);
        std::string getCuts() {return cuts_;}
        
    private:
        std::string cuts_;
        std::vector<Cut> cutVec_;
        void parseCutString();
    };


public:

    class HistSummary : public Cuttable
    {
    public:
        std::vector<std::pair<std::string, TH1*>> histVec;
        std::string name, label, vecVar;
        int nBins;
        double low, high;
        bool isLog;
        std::string xAxisLabel, yAxisLabel;
        
        HistSummary() {}
        HistSummary(std::string l, std::string n, std::string cuts, int nb, double ll, double ul, bool log, std::string xal = "", std::string yal = "");
        TH1* hist() {return histVec.back().second;}
    private:
        void parseName();
    };

    class FileSummary : public Cuttable
    {
    public:
        std::string name, treePath, label;
        double xsec, lumi, kfactor, nEvts;

        FileSummary() {}
        FileSummary(std::string lab, std::string nam, std::string tree, std::string cuts, double xs, double l, double k, double n);
    };

    Plotter(std::vector<HistSummary> h, std::vector<std::vector<FileSummary>> t);
    void plot();

private:
    std::vector<HistSummary> hists_;
    std::vector<std::vector<FileSummary>> trees_;
        
    void createHistsFromTuple();
    void fillHist(TH1 * const h, const HistSummary& name, const NTupleReader& tr, const double weight);
    
    template<typename T> void fillHistFromVec(TH1 * const h, const HistSummary& hs, const NTupleReader& tr, const double weight)
    {
        const auto& vec = tr.getVec<T>(hs.name);
        
        if(hs.vecVar.compare("size") == 0) h->Fill(vec.size(), weight);
        else
        {
            for(auto& obj : vec)
            {
                vectorFill(h, hs, obj, weight);
            }
        }
    }

    template<typename T> inline void vectorFill(TH1 * const h, const HistSummary& hs, const T& obj, const double weight)
    {
        h->Fill(obj, weight);
    }
};

typedef Plotter::HistSummary PHS;
typedef Plotter::FileSummary PFS;

#endif
