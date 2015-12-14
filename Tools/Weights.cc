#include "Weights.h"
#include "NTupleReader.h"

#include <iostream>

Pileup_Sys::Pileup_Sys()
{
    TH1::AddDirectory(false); //magic incantation that lets the root file close if this is not here segfaults 
    //Calling the Jasn file from RA2b currently
    TFile Pileup_Jasn(SearchFilePath("PileupHistograms_Nov17.root").c_str());
    //These are the historgrams currently in the Jasn file
    pu_central  = (TH1F*)Pileup_Jasn.Get("pu_weights_central");
    pu_up  = (TH1F*)Pileup_Jasn.Get("pu_weights_up");
    pu_down  = (TH1F*)Pileup_Jasn.Get("pu_weights_down");

    Pileup_Jasn.Close();
}

// ===  FUNCTION  ============================================================
//         Name:  Pileup_Sys::SearchFilePath
//  Description:  
// ===========================================================================
std::string Pileup_Sys::SearchFilePath(std::string name_) const
{
  std::vector<std::string> SearchPath = {"./", "../", "FileList"};
  TString name(name_);
  std::string repath="";

  for(auto &a : SearchPath)
  {
    const char* out = gSystem->Which(a.c_str(), name);
    if (out != 0)
    {
      repath = out;
      delete out;
      break;
    }
    delete out;
  }

  return repath;
}       // -----  end of function Pileup_Sys::SearchFilePath  -----


Pileup_Sys::~Pileup_Sys()
{
    //dtor Was going to delet histograms after use but this segfaulted the code need second magic incantation
    //delete pu_central;
    //delete pu_up;
    //delete pu_down;
}


void Pileup_Sys::getPileup_Sys(NTupleReader& tr)
{
    double _PUweightFactor = 1.;
    double _PUSysUp = 1.;
    double _PUSysDown = 1.;

    double trueint; 
    double w = 1.;


    //These are all the pileup varaibles the only one that is needed is the first
    const double &tru_npv  = tr.getVar<double>("tru_npv");
    //const int    BX       = tr.getVar<int>("BX");
    //const int    nm1      = tr.getVar<int>("nm1");
    //const int    n0       = tr.getVar<int>("n0");
    //const int    np1      = tr.getVar<int>("np1");
    //const int    npv      = tr.getVar<int>("npv");

    //This is going through and calculating the wieght 
    if (tru_npv < pu_central->GetBinLowEdge(pu_central->GetNbinsX()+1)) {
	_PUweightFactor = pu_central->GetBinContent(pu_central->GetXaxis()->FindBin(tru_npv));
    } else {
	std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
		  << " out of histogram binning." << std::endl;
	_PUweightFactor = pu_central->GetBinContent(pu_central->GetNbinsX());
    }

    if (tru_npv < pu_up->GetBinLowEdge(pu_up->GetNbinsX()+1)) {
	_PUSysUp = pu_up->GetBinContent(pu_up->GetXaxis()->FindBin(tru_npv));
    } else {
	std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
		  << " out of histogram binning." << std::endl;
	_PUSysUp = pu_up->GetBinContent(pu_up->GetNbinsX());
    }


    if (tru_npv < pu_down->GetBinLowEdge(pu_down->GetNbinsX()+1)) {
	_PUSysDown = pu_down->GetBinContent(pu_down->GetXaxis()->FindBin(tru_npv));
    } else {
	std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
		  << " out of histogram binning." << std::endl;
	_PUSysDown = pu_down->GetBinContent(pu_down->GetNbinsX());
    }


    tr.registerDerivedVar("_PUweightFactor", _PUweightFactor);
    tr.registerDerivedVar("_PUSysUp", _PUSysUp );
    tr.registerDerivedVar("_PUSysDown", _PUSysDown );


}

void Pileup_Sys::operator()(NTupleReader& tr)
{
    //
    getPileup_Sys(tr);
}

