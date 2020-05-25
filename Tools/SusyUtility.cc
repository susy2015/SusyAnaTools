// SusyUtility.cc
// Caleb Smith
// December 13, 2018

// utility functions
#include "SusyAnaTools/Tools/SusyUtility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>
#include <sys/stat.h>

// Repo for json.hpp: https://github.com/nlohmann/json/tree/master
#include "../../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

namespace SusyUtility
{
    void splitString(const std::string &s, const char& delim, std::vector<std::string>& result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            result.push_back(item);
        }
    }
    
    template<> 
    bool greaterThan(const std::pair<TLorentzVector,unsigned int>& p1, const std::pair<TLorentzVector,unsigned int>& p2)
    {
        // WARNING: round pt to float to match post-processing sorting
        float p1_pt = p1.first.Pt();
        float p2_pt = p2.first.Pt();
        if (p1_pt == p2_pt)
        {
            //printf("INFO: pt are equal; sorting by abs(eta). pt_1 = %f, pt_2 = %f, eta_1 = %f, eta_2 = %f\n", p1.first.Pt(), p2.first.Pt(), p1.first.Eta(), p2.first.Eta());
            return abs(p1.first.Eta()) < abs(p2.first.Eta());
        }
        else
        {
            //printf("pt_1 = %f, pt_2 = %f, pt_1 - pt_2 = %f, diff_equal_zero = %d\n", p1.first.Pt(), p2.first.Pt(), p1.first.Pt() - p2.first.Pt(), p1.first.Pt() - p2.first.Pt() == 0.0);
            return p1_pt > p2_pt;
        }
    }
    
    template<> 
    bool lessThan(const std::pair<TLorentzVector,unsigned int>& p1, const std::pair<TLorentzVector,unsigned int>& p2)
    {
        // WARNING: round pt to float to match post-processing sorting
        float p1_pt = p1.first.Pt();
        float p2_pt = p2.first.Pt();
        if (p1_pt == p2_pt)
        {
            return abs(p1.first.Eta()) > abs(p2.first.Eta());
        }
        else
        {
            return p1_pt < p2_pt;
        }
    }
    
    
    // return vector of strings given names separated by deliminator, e.g. "electron;muon" ---> {"electron", "muon"} 
    std::vector<std::string> getVecFromString(const std::string &s, const char& delim) {
        std::vector<std::string> mySplitString;
        if (!delim)
        {
            std::cout << "ERROR in getVecFromString(): please provide string (s) and char (delim)" << std::endl;
            return mySplitString;
        }
        splitString(s, delim, mySplitString);
        return mySplitString;
    }
    
    // get cut levels
    // example: {"", "cut1", "cut2", "cut3"} ---> {"", "cut1", "cut1;cut2", "cut1;cut2;cut3"}
    // example: {"cut1", "cut2", "", "cut3"} ---> {"cut1", "cut1;cut2", "cut1;cut2", "cut1;cut2;cut3"}
    std::vector<std::string> getCutLevels(const std::vector<std::string> cuts)
    {
        bool verbose = false;
        std::vector<std::string> cutLevels;
        std::string cutLevel;
        for (int i = 0; i < cuts.size(); ++i)
        {
            if (i > 0 && ! cutLevel.empty() && ! cuts[i].empty())
            {
                cutLevel += ";";
            }
            cutLevel += cuts[i];
            cutLevels.push_back(cutLevel);
            // debugging
            if (verbose)
            {
                printf("cut: %s cutLevel: %s\n", cuts[i].c_str(), cutLevel.c_str());
            }
        }
        return cutLevels;
    }

    // functions to parse cuts, e.g. "NBeq0_NSVeq0" ---> ";nBottoms_drLeptonCleaned=0;nSoftBottoms_drLeptonCleaned=0"
    // https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
    std::string parseCuts(std::string& input, std::map<std::string, std::string> var_map)
    {
        bool verbose = false;
        std::string output = input;
        std::map<std::string, std::string> cut_map = {
            {"eq", "="},
            {"ge", ">="},
            {"le", "<="},
            {"gt", ">"},
            {"lt", "<"},
        };
        // be careful with order of search and replace
        // it matter if replace patterns (output) appear in search patterns (intput)
        std::replace(output.begin(), output.end(), '_', ';');
        for (const auto& cut : cut_map)
        {
            output = std::regex_replace(output, std::regex(cut.first), cut.second);
        }
        for (const auto& var : var_map)
        {
            output = std::regex_replace(output, std::regex(var.first), var.second);
        }
        if (verbose)
        {
            printf("%s: %s\n", input.c_str(), output.c_str());
        }

        return output;
    }
    
    // print json file
    void printJson(const std::string& fileName, const std::string& key, const std::string& title)
    {
        // read json file
        std::ifstream i(fileName);
        json j;
        i >> j;

        // invert map
        std::map<int, std::string> invMap;

        for (const auto& element : j[key].items())
        {
            std::string k = element.key();
            int v = std::stoi(std::string(element.value())); 
            invMap[v] = k;
        }
        
        // print json file
        int nBins = invMap.size();
        std::cout << title << " : " << nBins << " bins" << std::endl;
        for (int i = 0; i < nBins; ++i)
        {
            std::cout << i << " : " << invMap[i] << std::endl;
        }
    }
    
    // check if file exists
    bool fileExists(const std::string& fileName)
    {
        // check if file exists
        struct stat buffer;  
        bool file_exists = bool(stat(fileName.c_str(), &buffer) == 0);
        return file_exists;
    }
}
