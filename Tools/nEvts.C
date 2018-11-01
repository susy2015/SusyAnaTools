#include "samples.h"
#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"

#include <iostream>
#include <sstream>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"negw",     no_argument, 0, 'w'},
        {"skipData", no_argument, 0, 's'}
    };

    bool getNegWeights = false, skipData = false;
    while((opt=getopt_long(argc, argv, "ws", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 'w':
            getNegWeights = true;
            break;

        case 's':
            skipData = true;
            break;
        }
    }
  
    if(getNegWeights)
        std::cout << "Will compute negative weight fraction" << std::endl;
    
    AnaSamples::SampleSet        ss("sampleSets.cfg");
    AnaSamples::SampleCollection sc("sampleCollections.cfg", ss);

    std::string selKeyStr;
    if(argc >= optind+1)
    {
        selKeyStr = argv[optind];
        std::cout << "selKeyStr : " << selKeyStr << std::endl;
    }

    std::stringstream ssSelKey(selKeyStr);
    std::string buf;
    std::vector<std::string> keyStrVec;

    // don't forget to fill keyStrVec
    while(ssSelKey >> buf) keyStrVec.push_back(buf);
    
    for(auto& file : ss) 
    {
        if( !keyStrVec.empty() )
        {
            bool found = false;
            for(auto& str : keyStrVec )
            { 
                if( file.first.find(str) !=std::string::npos ) { 
                    found = true; 
                    break; 
                }
            }
            if( !found ) continue;
        }
        if(skipData && file.first.find("Data") != std::string::npos)
        {
            std::cout << "Skipping " << file.first << std::endl;
            continue;
        }

        TChain *t = new TChain(file.second.treePath.c_str());
        file.second.addFilesToChain(t);
    
        //int nEntries = t->GetEntries();
        //if(nEntries != file.second.nEvts)
        //    std::cout << "DIFFERENT:\t";
        //std::cout << "Processing file(s): " << file.second.tag << "\t" << file.second.filePath + file.second.fileName << "\t" << nEntries << "\twas: " << file.second.nEvts  << std::endl;

        TH1* h3 = new TH1D("h3", "h3", 2, -100000, 100000);
            
        t->Draw("stored_weight>>h3", "1", "goff");
        std::cout << "Processing file(s): " << file.second.tag << "\t" << file.second.filePath + file.second.fileName << "\t" << "Neg weigths = " << int(h3->GetBinContent(1)) << ", Pos weights = " << int(h3->GetBinContent(2)) << std::endl;
        // delete TH1* to avoid memory leaks / save memory / not crash / be safe
        delete h3;
        // delete TChain* to avoid memory leaks / save memory / not crash / be safe
        delete t;
    }   
}

