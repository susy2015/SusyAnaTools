#ifndef ANASAMPLES_SAMPLES_H
#define ANASAMPLES_SAMPLES_H

#include <string>
#include <map>
#include <vector>

#include "TChain.h"

namespace AnaSamples
{
    class FileSummary
    {
    public:
        std::string filePath, treePath;
        double xsec, lumi, kfactor, nEvts;
        int color;
        
        FileSummary() {}
        FileSummary(std::string filePath, std::string treePath, double xsec, double lumi, double nEvts, double kfactor, int color = kBlack) : filePath(filePath), treePath(treePath), xsec(xsec), lumi(lumi), kfactor(kfactor), nEvts(nEvts), color(color)
        {
            weight_ = xsec * lumi * kfactor / nEvts;
            readFileList();
        }

        double getWeight() const {return weight_;}
        void addFilesToChain(TChain * const tc) const;
        std::vector<std::string> filelist_;

    private:
        double weight_;

        void readFileList();
    };

    bool operator< (const FileSummary& lhs, const FileSummary& rhs);
    bool operator== (const FileSummary& lhs, const FileSummary& rhs);
    bool operator!= (const FileSummary& lhs, const FileSummary& rhs);

    static const double lumi = 3000.0; // in pb-1
    //static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/";
    //static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Mar14_2014_v2/";
    //static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/PHYS14_72X_July_2015_v1.1/";
    //static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/Spring15_74X_July_2015_v1.1/";
    static const std::string fileDir = "/eos/uscms/store/user/lpcsusyhad/Spring15_74X_Oct_2015_Ntp_v2X/";

    template<class T>
    class SampleBase
    {
    public:
        SampleBase() : nullT_() {};
        const T& operator[](const std::string& key) const
        {
            auto iter = sampleSet_.find(key);
            if(iter != sampleSet_.end()) return iter->second;
            else                         return nullT_;
        }
        
        const T& null() const {return nullT_;}
    protected:
        std::map<std::string, T> sampleSet_;
        const T nullT_;

    public:
        decltype(sampleSet_.cbegin()) begin() const { return sampleSet_.cbegin(); }
        decltype(sampleSet_.cend()) end() const { return sampleSet_.cend(); }
    };

    class SampleSet : public SampleBase<FileSummary>
    {
    public:
        SampleSet(std::string fDir = fileDir);
    private:
        std::string fDir_;
    };

    class SampleCollection : public SampleBase<std::vector<FileSummary>>
    {
    public:
        SampleCollection(SampleSet samples);
    };
}

#endif
