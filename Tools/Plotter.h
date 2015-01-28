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
#include <set>

#include "NTupleReader.h"

class Plotter
{
private:
    class Cut
    {
    public:
        std::string name, vecVar;
        char type;
        double val, val2;
        bool inverted;

        Cut(std::string s, char t, double v, double v2 = 0);
        bool passCut(const NTupleReader& tr) const;
    private:
        void parseName();
        double translateVar(const NTupleReader& tr) const;
    };
    
    class Cuttable
    {
    public:
        Cuttable() {}
        Cuttable(const std::string& c);
        bool passCuts(const NTupleReader& tr) const;
        void setCuts(const std::string& c);
        void extractCuts(std::set<std::string>& ab);
        std::string getCuts() {return cuts_;}
        
    private:
        std::string cuts_;
        std::vector<Cut> cutVec_;
        void parseCutString();
    };


public:

    class FileSummary : public Cuttable
    {
    public:
        std::string name, treePath, label;
        double xsec, lumi, kfactor, nEvts;

        FileSummary() {}
        FileSummary(std::string lab, std::string nam, std::string tree, std::string cuts, double xs, double l, double k, double n);
    };

    class HistSummary : public Cuttable
    {
    public:
        std::vector<std::pair<std::string, TH1*>> histVec;
        std::map<std::vector<FileSummary>, std::pair<std::string, std::string>> names;
        std::string name, label, vecVar;
        int nBins;
        double low, high;
        bool isLog, isNorm;
        std::string xAxisLabel, yAxisLabel;
        
        HistSummary() {}
        HistSummary(std::string l, std::map<std::vector<Plotter::FileSummary>, std::string> ns, std::string cuts, int nb, double ll, double ul, bool log, bool norm, std::string xal, std::string yal);
        HistSummary(std::string l, std::string name, std::string cuts, int nb, double ll, double ul, bool log, bool norm, std::string xal, std::string yal);
        TH1* hist() {return histVec.back().second;}
    private:
        void parseName();
    };

    Plotter(std::vector<HistSummary> h, std::vector<std::vector<FileSummary>> t);
    void plot();

private:
    std::vector<HistSummary> hists_;
    std::vector<std::vector<FileSummary>> trees_;
    std::set<std::string> activeBranches_;
    
    void createHistsFromTuple();
    void fillHist(TH1 * const h, const std::pair<std::string, std::string>& name, const NTupleReader& tr, const double weight);

    template<typename T> double tlvGetValue(std::string name, T v)
    {
        if     (name.find("pt")  != std::string::npos) return v.Pt();
        else if(name.find("eta") != std::string::npos) return v.Eta();
        else if(name.find("phi") != std::string::npos) return v.Phi();
        else if(name.find("E")   != std::string::npos) return v.E();
        else if(name.find("M")   != std::string::npos) return v.M();
        else if(name.find("Mt")  != std::string::npos) return v.Mt();
    }
    
    /*template<typename T> void fillHist(TH1* const h, const std::pair<std::string, std::string>& name, const NTupleReader& tr, const double weight)
    {
        h->Fill(tr.getVar<T>(name.first), weight);
        }*/

    template<typename T> void fillHistFromVec(TH1* const h, const std::pair<std::string, std::string>& name, const NTupleReader& tr, const double weight)
    {
        const auto& vec = tr.getVec<T>(name.first);
        
        if(name.second.compare("size") == 0) h->Fill(vec.size(), weight);
        else
        {
            for(auto& obj : vec)
            {
                vectorFill(h, name, obj, weight);
            }
        }
    }

    template<typename T> inline void vectorFill(TH1 * const h, const std::pair<std::string, std::string>& name, const T& obj, const double weight)
    {
        h->Fill(obj, weight);
    }
};

typedef Plotter::HistSummary PHS;
typedef Plotter::FileSummary PFS;

inline bool operator< (const Plotter::FileSummary& lhs, const Plotter::FileSummary& rhs)
{
    return lhs.name < rhs.name || lhs.label < rhs.label || lhs.treePath < rhs.treePath;
}

#endif
