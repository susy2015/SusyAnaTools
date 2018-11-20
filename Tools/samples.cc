#include "samples.h"

#include <iostream>
#include <cstdio>

namespace AnaSamples
{
    void FileSummary::readFileList() const
    {
        if(filelist_.size()) filelist_.clear();
        
        std::string filePathAndName;
        if(filePath.size() > 0)
        {
            filePathAndName = filePath + "/" + fileName;
        }
        else
        {
            filePathAndName = fileName;
        }

        std::cout << "In FileSummary::readFileList(): filePathAndName = " << filePathAndName << std::endl;
        FILE *f = fopen(filePathAndName.c_str(), "r");
        char buff[1024];
        if(f)
        {
            while(!feof(f) && fgets(buff, 1023, f))
            {
                for(char* k = strchr(buff, '\n'); k != 0; k = strchr(buff, '\n')) *k = '\0';
                filelist_.push_back(buff);
            }
            fclose(f);
        }
        else std::cout << "In FileSummary::readFileList(): Filelist file \"" << filePath << "\" not found!!!!!!!" << std::endl;
    }

    void FileSummary::addCollection(const std::string& colName)
    {
        collections_.insert(colName);
    }

    std::map<std::string, FileSummary>& SampleSet::getMap()
    {
        return sampleSet_;
    }

    // modify weights to compare two MC samples
    void SampleSet::modifyWeights(const std::vector<std::string>& sampleTags1, const std::vector<std::string>& sampleTags2, std::vector<bool>& matchingTags1)
    {
        if (sampleTags1.size() != matchingTags1.size())
        {
            std::cout << "ERROR: sampleTags1 and matchingTags1 need to be the same size" << std::endl;
            return;
        }
        int nSamples = sampleTags1.size();
        int nMatching = 0;
        double sum_xsec_ratios = 0.0;
        for (int i = 0; i < nSamples; i++)
        {
            // if the samples have a matching range, add the cross section ratios to calculate average factor
            if (matchingTags1[i])
            {
                FileSummary fs1 = sampleSet_[sampleTags1[i]];
                FileSummary fs2 = sampleSet_[sampleTags2[i]];
                
                double xsec_ratio = (fs2.xsec * fs2.kfactor) / (fs1.xsec * fs1.kfactor);
                sum_xsec_ratios += xsec_ratio;
                nMatching += 1;

                printf("%s / %s xsec_ratio = %f\n", fs2.tag.c_str(), fs1.tag.c_str(), xsec_ratio);
            }
        }
        if (nMatching == 0)
        {
            std::cout << "ERROR: No matches found in matchingTags1. Please include at least one match (true)." << std::endl;
            return;
        }
        // calculate average cross section ratio
        double ave_xsec_ratio = sum_xsec_ratios / double(nMatching);
        printf("ave_xsec_ratio = %f\n", ave_xsec_ratio);
        for (int i = 0; i < nSamples; i++)
        {
            FileSummary fs1 = sampleSet_[sampleTags1[i]];
            fs1.setWeight(ave_xsec_ratio * fs1.getWeight());
        }
    }

    bool SampleSet::parseCfgLine(const char* buf)
    {
        char cDSname[256], cFPath[256], cfName[256], cTPath[256];
        float f1, f2, f3, f4;
        int nMatches = sscanf(buf, "%s %s %s %s %f %f %f %f", cDSname, cFPath, cfName, cTPath, &f1, &f2, &f3, &f4);
        if(nMatches == 8) //this is MC 
        {
            //                                                        xsec        NEvts+ NEvts-  kfactor
            if(!isCondor_) addSample(cDSname, cFPath, cfName, cTPath, f1,   lumi_, f2 -  f3,     f4,     kGreen);
            else           addSample(cDSname, "",     cfName, cTPath, f1,   lumi_, f2 -  f3,     f4,     kGreen);
            return true;
        }
        else if(nMatches == 6) //this is Data
        {
            //                                                        lumi  kfactor
            if(!isCondor_) addSample(cDSname, cFPath, cfName, cTPath, f1,   f2,     kBlack);
            else           addSample(cDSname, "",     cfName, cTPath, f1,   f2,     kBlack);
            return true;
        }

        return false;            
    }
    
    SampleSet::SampleSet(std::string file, bool isCondor, double lumi) : isCondor_(isCondor), lumi_(lumi)
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
