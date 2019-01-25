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
    template<typename T>
    void split(const std::string &s, const char& delim, T result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
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
        split(s, delim, std::back_inserter(splitString));
        return splitString;
    }
}
