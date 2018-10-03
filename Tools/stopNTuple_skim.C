#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


#include "TFile.h"
#include "TChain.h"
//#include "TH1.h"
//#include "TH2.h"
//#include "TProfile.h"
//#include "TRandom.h"
#include "TDirectory.h"
#include "TTree.h"

int main(int argc, char* argv[])
{
  //----------------------------------------------------------------------------
  //  Getting input information
  //----------------------------------------------------------------------------
  if (argc < 2)
  {
    std::cerr <<"Please give 2 arguments " << "runList " << " " << "outputFileName" << std::endl;
    std::cerr <<" Valid configurations are " << std::endl;
    std::cerr <<" ./LostLepton_MuCS_TTbar runlist_ttjets.txt isoplots.root" << std::endl;
    return -1;
  }

  //----------------------------------------------------------------------------
  //  Input and output files
  //----------------------------------------------------------------------------
  const int  stopmass = atoi(argv[1]);
  const int  LSPmass = atoi(argv[2]);
  const char *inputFileList = argv[3];
  const char *outFileName   = argv[4];


  TChain *fChain = new TChain("stopTreeMaker/AUX");
  // :WARNING:06/16/2015 11:41:12:benwu: Can't work with passing TChain
  // through a function. Why?
  std::fstream input(inputFileList, std::fstream::in );
  if(!input.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  for(std::string line; getline(input, line);)
  {
    if (line[0] == '#') continue;
    //boost::algorithm::trim_right(line);
    std::cout << "Add File: " << line << std::endl;
    fChain->Add(line.c_str());
  }

  std::stringstream ss;
  ss <<"SusyMotherMass == "<< stopmass <<" && SusyLSPMass == " << LSPmass;

  TFile* output = new TFile(outFileName, "RECREATE");
  TDirectory *mydict = output->mkdir("stopTreeMaker");
  mydict->cd();
  TTree* selectedTree = fChain->CopyTree(ss.str().c_str());

  selectedTree->Write();
  output->Close();

  return 0;
}
