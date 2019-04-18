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
    
    template<typename T1, typename T2> bool greaterThan(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2)
    {
        return p1.first > p2.first;
    }
    
    template<typename T1, typename T2> bool lessThan(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2)
    {
        return p1.first < p2.first;
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
}
