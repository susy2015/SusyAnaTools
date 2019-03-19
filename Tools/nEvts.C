#include "samples.h"
#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"

#include <iostream>
#include <sstream>
#include <getopt.h>

bool checkBranch(TChain* t, const char* name)
{
    TBranch* br = (TBranch*)t->GetListOfBranches()->FindObject(name);
    return bool(br);
}

int main(int argc, char *argv[])
{
    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"skipData",                no_argument, 0, 's'},
        {"legacy",                  no_argument, 0, 'l'},
        {"sampleCollectionsFile",   required_argument, 0, 'C'},
        {"sampleSetsFile",          required_argument, 0, 'S'}
    };

    bool skipData = false;
    bool legacy = false;
    std::string sampleCollectionsFile = "";
    std::string sampleSetsFile = "";
    while((opt=getopt_long(argc, argv, "slC:S:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 's':
            skipData = true;
            break;
        case 'l':
            legacy = true;
            break;
        case 'C':
            sampleCollectionsFile = optarg;
            break;
        case 'S':
            sampleSetsFile = optarg;
            break;
        }
    }
   
    // check for sample collections and sample sets files
    if ( sampleCollectionsFile.empty() )
    {
        printf("Please provide the name of the sample collections file using the -C option.\n");
        return 1;
    }
    if ( sampleSetsFile.empty() )
    {
        printf("Please provide the name of the sample sets file using the -S option.\n");
        return 1;
    }
    
    printf("sample collections file: %s\n", sampleCollectionsFile.c_str());
    printf("sample sets file: %s\n", sampleSetsFile.c_str());
  
    AnaSamples::SampleSet        ss(sampleSetsFile);
    AnaSamples::SampleCollection sc(sampleCollectionsFile, ss);

    std::string selKeyStr;
    if(argc >= optind+1)
    {
        selKeyStr = argv[optind];
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
            std::cout << "Skipping Data sample " << file.first << std::endl;
            continue;
        }

        TChain *t = new TChain(file.second.treePath.c_str());
        
        // check that tree exists
        if (!t)
        {
            printf("ERROR for %s: Unable to load tree \"%s\". Skipping %s.\n", file.first.c_str(), file.second.treePath.c_str(), file.first.c_str());
            continue;
        }
        
        file.second.addFilesToChain(t);
        
        // check that branches exist
        TObjArray* objArray = t->GetListOfBranches();
        if (! objArray)
        {
            printf("ERROR for %s: Unable to get list of branches from tree \"%s\". Skipping %s.\n", file.first.c_str(), file.second.treePath.c_str(), file.first.c_str());
            continue;
        }
    
        TH1* h3 = new TH1D("h3", "h3", 2, -100000, 100000);
        
        // --- Calculate weights --- //
        //
        // weight name for prod2016MCv2_NANO_Skim ntuples: genWeight 
        // weight name for CMSSW8028_2016 ntuples: stored_weight           
        // weight name for Stealth SUSY ntuples: Weight
        
        if(checkBranch(t, "genWeight"))
        {
            t->Draw("genWeight>>h3", "1", "goff");
        }
        else if(checkBranch(t, "stored_weight"))
        {
            t->Draw("stored_weight>>h3", "1", "goff");
        }
        else if(checkBranch(t, "Weight"))
        {
            t->Draw("Weight>>h3", "1", "goff");
        }
        else
        {
            printf("ERROR for %s: No weight variable found tree \"%s\". Skipping %s.\n", file.first.c_str(), file.second.treePath.c_str(), file.first.c_str());
            continue;
        }
        
        std::cout << "Processing file(s): " << file.second.tag << "\t" << file.second.filePath + file.second.fileName << "\t" << "Neg weigths = " << int(h3->GetBinContent(1)) << ", Pos weights = " << int(h3->GetBinContent(2)) << std::endl;
        
        // delete TH1* and TChain* to avoid memory leaks / save memory / not crash / be safe
        delete h3;
        delete t;
    }   
}

