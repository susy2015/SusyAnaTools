// SusyUtility.cc
// Caleb Smith
// December 13, 2018

// utility functions

#ifndef SUSYUTILITY_H 
#define SUSYUTILITY_H 

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

namespace SusyUtility
{
    void split(const std::string &s, const char& delim, std::vector<std::string>& result);
    
    // return vector of strings given names separated by deliminator, e.g. "electron;muon" ---> {"electron", "muon"} 
    std::vector<std::string> getVecFromString(const std::string &s, const char& delim);
}

#endif
