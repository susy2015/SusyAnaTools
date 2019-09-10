// SusyUtility.cc
// Caleb Smith
// December 13, 2018

// utility functions
#include "SusyAnaTools/Tools/SusyUtility.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>

namespace SusyUtility
{
    void split(const std::string &s, const char& delim, std::vector<std::string>& result) {
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
        if (p1.first.Pt() == p2.first.Pt())
        {
            return abs(p1.first.Eta()) < abs(p2.first.Eta());
        }
        else
        {
            return p1.first.Pt() > p2.first.Pt();
        }
    }
    
    template<> 
    bool lessThan(const std::pair<TLorentzVector,unsigned int>& p1, const std::pair<TLorentzVector,unsigned int>& p2)
    {
        if (p1.first.Pt() == p2.first.Pt())
        {
            return abs(p1.first.Eta()) > abs(p2.first.Eta());
        }
        else
        {
            return p1.first.Pt() < p2.first.Pt();
        }
    }
    
    
    // return vector of strings given names separated by deliminator, e.g. "electron;muon" ---> {"electron", "muon"} 
    std::vector<std::string> getVecFromString(const std::string &s, const char& delim) {
        std::vector<std::string> splitString;
        if (!delim)
        {
            std::cout << "ERROR in getVecFromString(): please provide string (s) and char (delim)" << std::endl;
            return splitString;
        }
        split(s, delim, splitString);
        return splitString;
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
}
