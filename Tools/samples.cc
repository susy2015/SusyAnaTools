#include "samples.h"

#include <iostream>
#include <cstdio>

namespace AnaSamples
{
    void FileSummary::readFileList() const
    {
        if(filelist_.size()) filelist_.clear();
        
        FILE *f = fopen((filePath+fileName).c_str(), "r");
        char buff[512];
        if(f)
        {
            while(!feof(f) && fgets(buff, 512, f))
            {
                for(char* k = strchr(buff, '\n'); k != 0; k = strchr(buff, '\n')) *k = '\0';
                filelist_.push_back(buff);
            }
            fclose(f);
        }
        else std::cout << "Filelist file \"" << filePath << "\" not found!!!!!!!" << std::endl;
    }

    void FileSummary::addCollection(const std::string& colName)
    {
        collections_.insert(colName);
    }

    std::map<std::string, FileSummary>& SampleSet::getMap()
    {
        return sampleSet_;
    }

    bool SampleSet::parseCfgLine(const char* buf)
    {
        char cDSname[256], cFPath[256], cfName[256], cTPath[256];
        float f1, f2, f3, f4;
        int nMatches = sscanf(buf, "%s %s %s %s %f %f %f %f", cDSname, cFPath, cfName, cTPath, &f1, &f2, &f3, &f4);
        if(nMatches == 8) //this is MC 
        {
            //                                         xsec        NEvts+ NEvts-  kfactor
            addSample(cDSname, cFPath, cfName, cTPath, f1,   lumi_, f2 -  f3,     f4,     kGreen);
            return true;
        }
        else if(nMatches == 6) //this is Data
        {
            //                                         lumi  kfactor
            addSample(cDSname, cFPath, cfName, cTPath, f1,   f2,     kBlack);
            return true;
        }

        return false;            
    }
    
    SampleSet::SampleSet(std::string file, double lumi) : lumi_(lumi)
    {
        readCfg(file);
    }

    bool SampleCollection::parseCfgLine(const char* buf)
    {
        char rbuf[BUF_LEN_];
        size_t offset = 0;
        std::vector<std::string> sampleSetNames;

        memcpy(rbuf, buf, strlen(buf) + 1);

        //Get sampleCollection name 
        char * pch;
        pch = strtok(rbuf," ");

        if(pch == NULL) return false;

        std::string collectionName(pch);
        while(pch != NULL)
        {
            pch = strtok(NULL, " \t");
            if(pch != NULL) 
            {
                sampleSetNames.push_back(pch);
            }
        }

        if(sampleSetNames.size())
        {
            addSampleSet(ss_, collectionName, sampleSetNames);
            return true;
        }
        
        return false;
    }

    SampleCollection::SampleCollection(const std::string& file, SampleSet& samples) : ss_(samples)
    {
        readCfg(file);
    }

    void SampleCollection::addSampleSet(SampleSet& samples, const std::string& name, const std::vector<std::string>& vss)
    {
        if(vss.size() > 1)
        {
            for(const std::string& sn : vss)
            {
                if(sn.compare(name) == 0)
                {
                    std::cout << "You have named a sampleCollection the same as one of its member sampleSets, but it has more than one sampleSet!!!! This is bad!!!  Stop!!! Stop now!!!  This collection will be skipped until it is properly named." << std::endl;
                    return;
                }
            }
        }

        auto& map = samples.getMap();

        for(const std::string& sn : vss)
        {
            map[sn].addCollection(name);
            sampleSet_[name].push_back(samples[sn]);
            nameVec_[name].push_back(sn);
            totalLumiMap_[name] += samples[sn].lumi;
        }
    }

    std::vector<std::string>& SampleCollection::getSampleLabels(std::string name)
    {
        return nameVec_[name];
    }

    bool operator< (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath < rhs.filePath || lhs.treePath < rhs.treePath;
    }

    bool operator== (const FileSummary& lhs, const FileSummary& rhs)
    {
        return lhs.filePath == rhs.filePath && lhs.treePath == rhs.treePath && lhs.xsec == rhs.xsec && lhs.lumi == rhs.lumi && lhs.kfactor == rhs.kfactor && lhs.nEvts == rhs.nEvts;
    }

    bool operator!= (const FileSummary& lhs, const FileSummary& rhs)
    {
        return !(lhs == rhs);
    }
}
