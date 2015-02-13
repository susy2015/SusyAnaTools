#include "samples.h"

#include <iostream>

namespace AnaSamples
{
    // Lumi in 1/pb
    SampleHolder::SampleHolder()
    {
        //Define sets of samples for later use 
        sampleSet_["TTbar"]              = {samples["TTbar"]};
        sampleSet_["WJetsToLNu"]         = {samples["WJetsToLNu_HT_600toInf"], samples["WJetsToLNu_HT_400to600"], samples["WJetsToLNu_HT_200to400"], samples["WJetsToLNu_HT_100to200"]};
        sampleSet_["ZJetsToNuNu"]        = {samples["ZJetsToNuNu_HT_600toInf"]/*, samples["ZJetsToNuNu_HT_400to600"], samples["ZJetsToNuNu_HT_200to400"], samples["ZJetsToNuNu_HT_100to200"]*/};
        sampleSet_["DYJetsToLL"]         = {samples["DYJetsToLL_HT_600toInf"]/*, samples["DYJetsToLL_HT_400to600"], samples["DYJetsToLL_HT_200to400"], samples["DYJetsToLL_HT_100to200"]*/};
        sampleSet_["QCD"]                =  {samples["QCD_HT_1000toInf"], samples["QCD_HT_500to1000"], samples["QCD_HT_250to500"]};
        sampleSet_["T_tW"]               =  {samples["T_tW"], samples["Tbar_tW"]};
        sampleSet_["TTZ"]                = {samples["TTZ"]};
    }

    bool operator< (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath < rhs.filePath || lhs.treePath < rhs.treePath;
    }

    bool operator== (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath == rhs.filePath && lhs.treePath == rhs.treePath && lhs.xsec == rhs.xsec && lhs.lumi == rhs.lumi && lhs.kfactor == rhs.kfactor && lhs.nEvts == rhs.nEvts;
    }
}
