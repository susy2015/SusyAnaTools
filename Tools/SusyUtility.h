// SusyUtility.cc
// Caleb Smith
// December 13, 2018

// utility functions

#ifndef SUSYUTILITY_H 
#define SUSYUTILITY_H 

#include "TLorentzVector.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

namespace SusyUtility
{
    void split(const std::string &s, const char& delim, std::vector<std::string>& result);
    
    template<typename T1, typename T2> 
    bool greaterThan(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2)
    {
        return p1.first > p2.first;
    }

    template<> 
    bool greaterThan(const std::pair<TLorentzVector,unsigned int>& p1, const std::pair<TLorentzVector,unsigned int>& p2);

    template<typename T1, typename T2> 
    bool lessThan(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2)
    {
        return p1.first < p2.first;
    }

    template<> 
    bool lessThan(const std::pair<TLorentzVector,unsigned int>& p1, const std::pair<TLorentzVector,unsigned int>& p2);
    
    // return vector of strings given names separated by deliminator, e.g. "electron;muon" ---> {"electron", "muon"} 
    std::vector<std::string> getVecFromString(const std::string &s, const char& delim);
    // get cut levels, e.g. {"", "cut1", "cut2", "cut3"} ---> {"", "cut1", "cut1;cut2", "cut1;cut2;cut3"}
    std::vector<std::string> getCutLevels(const std::vector<std::string> cuts);
    // functions to parse cuts, e.g. "NBeq0_NSVeq0" ---> ";nBottoms_drLeptonCleaned=0;nSoftBottoms_drLeptonCleaned=0"
    // https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
    std::string parseCuts(std::string& input, std::map<std::string, std::string> var_map);

}

#endif
