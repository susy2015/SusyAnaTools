#include "samples.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    AnaSamples::SampleSet        ss;
    AnaSamples::SampleCollection sc(ss);

    std::string selKeyStr;
    if( argc ==2 ){
       selKeyStr = argv[1];
       std::cout<<"selKeyStr : "<<selKeyStr<<std::endl;
    }

    std::stringstream ssSelKey(selKeyStr);

    std::string buf;
    std::vector<std::string> keyStrVec;
    
    while(ssSelKey >> buf) keyStrVec.push_back(buf);

    for(auto& file : ss) 
    {
        if( !keyStrVec.empty() ){
            bool found = false;
            for(auto& str : keyStrVec ){ if( file.first.find(str) !=std::string::npos ) found = true; }
            if( !found ) continue;
        }

        TChain *t = new TChain(file.second.treePath.c_str());
        file.second.addFilesToChain(t);
     
        std::cout << "Processing file(s): " << file.second.filePath << "\t" << t->GetEntries() << std::endl;
    }
     
}
