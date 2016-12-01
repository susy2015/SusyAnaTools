#include "samples.h"
#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <sstream>
#include <getopt.h>

int main(int argc, char *argv[])
{
  int opt;
  int option_index = 0;
  static struct option long_options[] = {
    {"negw", no_argument, 0, 'w'}
  };

  bool getNegWeights = false;
  while((opt=getopt_long(argc, argv, "w", long_options, &option_index)) != -1)
  {
    switch(opt)
    {
      case 'w':
      getNegWeights = true;
      break;
    }
  }
  
  if(getNegWeights)
  std::cout << "Will compute negative weight fraction" << std::endl;
    
  AnaSamples::SampleSet        ss;
  AnaSamples::SampleCollection sc(ss);

  std::string selKeyStr;
  if(argc >= optind+1)
  {
    selKeyStr = argv[optind];
    std::cout << "selKeyStr : " << selKeyStr << std::endl;
  }

  std::stringstream ssSelKey(selKeyStr);
  std::string buf;
  std::vector<std::string> keyStrVec;
    
  while(ssSelKey >> buf) keyStrVec.push_back(buf);

  for(auto& file : ss) 
  {
    if( !keyStrVec.empty() )
    {
      bool found = false;
      for(auto& str : keyStrVec ){ if( file.first.find(str) !=std::string::npos ) found = true; }
      if( !found ) continue;
    }

    TChain *t = new TChain(file.second.treePath.c_str());
    file.second.addFilesToChain(t);
     
    std::cout << "Processing file(s): " << file.second.filePath << "\t" << t->GetEntries() << std::endl;

    if(getNegWeights)
    {
      std::set<std::string> activeBranches;
      activeBranches.insert("stored_weight");
      NTupleReader tr = NTupleReader(t, activeBranches);

      int negw = 0;
      int posw = 0;
      while(tr.getNextEvent())
      {
        double stored_weight = tr.getVar<double>("stored_weight");
        if(stored_weight > 0) ++posw;
        else ++negw;
      }
      std::cout << "Processing file(s): " << file.second.filePath << "\t" << "Neg weigths = " << negw << ", Pos weights = " << posw << std::endl;
    }
  }   
}
