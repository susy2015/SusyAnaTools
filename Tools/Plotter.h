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
#include <memory>

#include "NTupleReader.h"
#include "samples.h"

class Plotter
{
private:
    class HistCutSummary;

    class Cut
    {
    public:
        std::string name, vecVar;
        char type;
        double val, val2;
        bool inverted;

        Cut(std::string s, char t, bool inv, double v, double v2 = 0);
        bool passCut(const NTupleReader& tr) const;
    private:
        void parseName();
        double translateVar(const NTupleReader& tr) const;
        bool boolReturn(const NTupleReader& tr) const;
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

    class HistVecAndType
    {
    public:
        TNamed* h;
        std::vector<std::shared_ptr<HistCutSummary>> hcsVec;
        std::string type;

        HistVecAndType(std::vector<std::shared_ptr<HistCutSummary>> hcsVec, std::string type) : hcsVec(hcsVec), type(type) {h = nullptr;}
        ~HistVecAndType() { if(h) delete h;}

        const std::string& flabel() const {return hcsVec.front()->label;}
    };

public:

    class DatasetSummary : public Cuttable
    {
    public:
        std::string label, weightStr;
        std::vector<AnaSamples::FileSummary> files;
        double kfactor;

        DatasetSummary() {}
//        DatasetSummary(std::string lab, std::string nam, std::string tree, std::string cuts, double xs, double l, double k, double n);
        DatasetSummary(std::string lab, std::vector<AnaSamples::FileSummary>& f, std::string cuts = "", std::string weights = "", double k = 1.0);

        double getWeight(const NTupleReader& tr) const;

    private:
        std::vector<std::string> weightVec_;
        void parseWeights();
    };

    class DataCollection
    {
    public:
        std::string type;
        std::vector<std::pair<std::string, DatasetSummary>> datasets;

        DataCollection(std::string type, std::vector<std::pair<std::string, DatasetSummary>> ds) : type(type), datasets(ds) {}
        DataCollection(std::string type, std::string var, std::vector<DatasetSummary> ds);
    };

    class HistSummary : public Cuttable
    {
    public:
        std::vector<HistVecAndType> hists;
        std::string name;
        int nBins;
        double low, high;
        bool isLog, isNorm;
        std::string xAxisLabel, yAxisLabel;
        std::pair<int, int> ratio;
        
        HistSummary() {}
        HistSummary(std::string l, std::vector<Plotter::DataCollection> ns, std::pair<int, int> ratio, std::string cuts, int nb, double ll, double ul, bool log, bool norm, std::string xal, std::string yal);
        ~HistSummary();

        TH1* fhist(){if(hists.size()) return hists.front().hcsVec.front()->h;}
    private:
        void parseName(std::vector<Plotter::DataCollection>& ns);
    };

    Plotter(std::vector<HistSummary>& h, std::vector<std::vector<AnaSamples::FileSummary>>& t);
    ~Plotter();

    void plot();

private:
    std::vector<HistSummary> hists_;
    std::vector<std::vector<AnaSamples::FileSummary>> trees_;
    std::set<std::string> activeBranches_;
    TFile *fout_;

    class HistCutSummary
    {
    public:
        std::string label;
        std::pair<std::string, std::string> variable;
        TH1 *h;
        const HistSummary *hs;
        DatasetSummary dss;

        HistCutSummary(const std::string lab, TH1* hist, const std::pair<std::string, std::string> v, const HistSummary* hsum, const DatasetSummary& ds) : label(lab), h(hist), variable(v), hs(hsum), dss(ds) {}
        ~HistCutSummary();
    };
    
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

    template<typename T> inline const T pointerDeref(T obj) const
    {
        return obj;
    }

    template<typename T> inline const T& pointerDeref(T* const obj) const
    {
        return *obj;
    }
    
    template<typename T> void fillHistFromVec(TH1* const h, const std::pair<std::string, std::string>& name, const NTupleReader& tr, const double weight)
    {
        const auto& vec = tr.getVec<T>(name.first);
        
        if(name.second.compare("size") == 0) h->Fill(vec.size(), weight);
        else
        {
            for(auto& obj : vec)
            {
                vectorFill(h, name, pointerDeref(obj), weight);
            }
        }
    }

    template<typename T> inline void vectorFill(TH1 * const h, const std::pair<std::string, std::string>& name, const T& obj, const double weight)
    {
        h->Fill(obj, weight);
    }
};

typedef Plotter::HistSummary PHS;
typedef AnaSamples::FileSummary AFS;
typedef Plotter::DatasetSummary PDS;
typedef Plotter::DataCollection PDC;

inline bool operator< (const Plotter::DataCollection& lhs, const Plotter::DataCollection& rhs)
{
    return lhs.datasets < rhs.datasets;
}

inline bool operator< (const Plotter::DatasetSummary& lhs, const Plotter::DatasetSummary& rhs)
{
    return lhs.label < rhs.label || lhs.files < rhs.files;
}

#endif
