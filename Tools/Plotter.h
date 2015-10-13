#ifndef STOP_PLOTTER_H
#define STOP_PLOTTER_H

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TPad.h"

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
    
    class VarName
    {
    public:
        std::string name;
        std::string var;
        int index;

        VarName() {index = -1;}
    };

    class Cut
    {
    public:
        std::string rawName;
        VarName name;
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
        void extractCuts(std::set<std::string>& ab) const;
        const std::string& getCuts() const {return cuts_;}
        
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
        double extractWeightNames(std::set<std::string>& ab) const;

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

    Plotter(std::vector<HistSummary>& h, std::set<AnaSamples::FileSummary>& t, const bool readFromTuple = true, std::string ofname = "", const int nFile = -1, const int startFile = 0, const int nEvts = -1);
    ~Plotter();

    static void parseSingleVar(const std::string& name, VarName& var);

    void setPlotDir(const std::string plotDir);

    void plot();
    void saveHists();

private:
    std::vector<HistSummary> hists_;
    std::set<AnaSamples::FileSummary> trees_;
    std::string plotDir_;
    TFile *fout_;
    bool readFromTuple_;
    const int nFile_;
    const int startFile_;
    const int maxEvts_;

    class HistCutSummary
    {
    public:
        std::string label, name;
        VarName variable;
        TH1 *h;
        const HistSummary *hs;
        DatasetSummary dss;

        HistCutSummary(const std::string& lab, const std::string& name, const VarName v, const HistSummary* hsum, const DatasetSummary& ds) : label(lab), name(name), h(nullptr), variable(v), hs(hsum), dss(ds) {}
        ~HistCutSummary();
    };
    
    void createHistsFromTuple();
    void createHistsFromFile();
    void fillHist(TH1 * const h, const VarName& name, const NTupleReader& tr, const double weight);
    void smartMax(const TH1* const h, const TLegend* const l, const TPad* const p, double& gmin, double& gmax, double& gpThreshMax) const;

    template<typename T> static const double& tlvGetValue(const std::string& name, const T& v)
    {
        fflush(stdin);
        
        if     (name.find("pt")  != std::string::npos) 
        {
            const auto& retval = v.Pt();
            return retval;
        }
        else if(name.find("eta") != std::string::npos) 
        {
            const auto& retval = v.Eta();
            return retval;
        }
        else if(name.find("phi") != std::string::npos) 
        {
            const auto& retval = v.Phi();
            return retval;
        }
        else if(name.find("E")   != std::string::npos) 
        {
            const auto& retval = v.E();
            return retval;
        }
        else if(name.find("M")   != std::string::npos) 
        {
            const auto& retval = v.M();
            return retval;
        }
        else if(name.find("Mt")  != std::string::npos) 
        {
            const auto& retval = v.Mt();
            return retval;
        }
        else
        {
            printf("Invalid lorentz variable: %s returning nullptr segfault incoming!!!\n", name.c_str());
            fflush(stdin);
            return *static_cast<double*>(nullptr);
        }
    }

    template<typename T> inline const T pointerDeref(T obj) const
    {
        return obj;
    }

    template<typename T> inline const T& pointerDeref(T* const obj) const
    {
        return *obj;
    }
    
    template<typename T> void fillHistFromVec(TH1* const h, const VarName& name, const NTupleReader& tr, const double weight)
    {
        if(name.var.compare("size") == 0) 
        {
            const auto& vec = tr.getVec<T>(name.name);
            if(&vec != nullptr) h->Fill(vec.size(), weight);
        }
        else
        {
            if(name.index >= 0)
            {
                auto& var = getVarFromVec<T>(name, tr);
                if(&var != nullptr) vectorFill(h, name, pointerDeref(var), weight);
            }
            else
            {
                const auto& vec = tr.getVec<T>(name.name);
                for(auto& var : vec) vectorFill(h, name, pointerDeref(var), weight);
            }
        }
    }

    template<typename T, typename R = T> static const R& getVarFromVec(const VarName& name, const NTupleReader& tr)
    {
        const auto& vec = tr.getVec<T>(name.name);
        
        if(&vec != nullptr)
        {
            const int& i = name.index;
            if(i < vec.size()) return vec.at(i);
            else return *static_cast<R*>(nullptr);
        }
        return *static_cast<R*>(nullptr);
    }

    template<typename T> inline void vectorFill(TH1 * const h, const VarName& name, const T& obj, const double weight)
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
