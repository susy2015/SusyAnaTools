#include <iostream>
#include <string>
#include "SusyAnaTools/Tools/SATException.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/samples.h"
#include "SusyAnaTools/Tools/baselineDef.h"
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"

using namespace std;

const std::string spec = "ISRJets";
const int nisrJetBins = 9;
const double isrJetBins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 14};

// === Main Function ===================================================
int main(int argc, char* argv[]) 
{


  
TChain *fChain = 0;
      if (argc < 5)
	{
	  std::cerr <<"Please give 4 arguments "<<"SubsampleName"<< " MaxEvent"<< "Startfile" <<" No. of Files to run" <<std::endl;
	  std::cerr <<" Valid configurations are " << std::endl;
	  std::cerr <<" ./remnantSys TTbarInc 1000 1 0" << std::endl;
	  return -1;
	}

      const char *subSampleName = argv[1];
      const char *maxevent = argv[2];
    
      int numFiles = -1;
      int startFile = 0;
      // Change string arg to int
      int  maxEvent =  std::atoi(maxevent);
      numFiles =  std::atoi(argv[3]);
      startFile =  std::atoi(argv[4]);

      // Prepare file list and finalize it
      //TChain *fChain = 0;
      TString subSampleNameT = subSampleName;

      const std::string stFile = std::to_string(startFile);
      TString stFileT = stFile;
      TString specT = spec;

      TString FileName = subSampleNameT+"_"+spec+"_"+stFileT;
      TFile *f = new TFile(FileName+".root", "RECREATE");
      TH1D *hISR = new TH1D("NJetsISR", "NJetsISR", nisrJetBins, isrJetBins );

      AnaSamples::SampleSet        ss("sampleSets.txt", (argc == 6), AnaSamples::luminosity);
      AnaSamples::SampleCollection sc("sampleCollections.txt", ss);
                                   
      double ScaleMC = 1.;                                                                              
      if(ss[subSampleName] != ss.null())                                                                             
	{                                                                                                               
	  fChain = new TChain(ss[subSampleName].treePath.c_str());                                                           
	  ss[subSampleName].addFilesToChain(fChain, startFile, numFiles);

	  ScaleMC = ss[subSampleName].getWeight();
	} 

      AnaFunctions::prepareForNtupleReader();
      NTupleReader *tr =0;
      tr = new NTupleReader(fChain);

/*
      BaselineVessel *CSBaseline = 0;
      CSBaseline = new BaselineVessel(*tr, spec);
      tr->registerFunction((*CSBaseline));
      CSBaseline->SetupTopTagger(true,"TopTagger.cfg");
*/

      // --- Analyse events --------------------------------------------
      std::cout<<"First loop begin: "<<std::endl;
      int entries = tr->getNEntries();
      std::cout<<"\nentries : "<<entries<<"\t MC Scale: "<<ScaleMC<<std::endl; 
      cout<<"maxevent: "<<maxEvent<<endl;
      while(tr->getNextEvent())
	{
	  if(maxEvent>=0 && tr->getEvtNum() > maxEvent ) break;
	  // Add print out of the progress of looping
	  if( tr->getEvtNum()-1 == 0 || tr->getEvtNum() == entries || (tr->getEvtNum()-1)%(entries/10) == 0 ) 
	    {
	      std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;
	    }

           double iniWeight = tr->getVar<double>("evtWeight");

           double stored_weight = subSampleNameT.Contains("Data") ? 1 : tr->getVar<double>("stored_weight");
           int sign_of_stored_weight = (stored_weight > 0) ? 1 : ((stored_weight < 0) ? -1 : 0);

           double evtWeight = iniWeight >=0 ? iniWeight * sign_of_stored_weight : iniWeight;

	  /*
	  const bool passBaseline = tr->getVar<bool>("passBaseline" + spec);
	  if(!passBaseline) continue;
	  */
	  
	  const int nJetsISR = tr->getVar<int>("NJetsISR");
	  hISR->Fill(nJetsISR, evtWeight);



	}
      f->cd();
      hISR->Write();
      f->Close();

  fChain->Reset();
  return 0;
    
}
