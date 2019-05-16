// SusyUtility.cc
// Caleb Smith
// December 13, 2018

// utility functions
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

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
            //printf("cut: %s cutLevel: %s\n", cuts[i].c_str(), cutLevel.c_str());
        }
        return cutLevels;
    }
}
