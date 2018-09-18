#ifndef PILEUPWEIGHTS_H
#define PILEUPWEIGHTS_H

#include "NTupleReader.h"
#include "TH1.h"

template<typename data_t>
class Pileup_SysTemplate
{
public:
    Pileup_SysTemplate(std::string filename="PileupHistograms_Nov17.root")
    {
        TH1::AddDirectory(false); //magic incantation that lets the root file close if this is not here segfaults 
        //Calling the Jasn file from RA2b currently
        TFile Pileup_Jasn(filename.c_str());
        //These are the historgrams currently in the Jasn file
        pu_central  = (TH1F*)Pileup_Jasn.Get("pu_weights_central");
        pu_up  = (TH1F*)Pileup_Jasn.Get("pu_weights_up");
        pu_down  = (TH1F*)Pileup_Jasn.Get("pu_weights_down");

        Pileup_Jasn.Close();
    }

    ~Pileup_SysTemplate()
    {
        //dtor Was going to delet histograms after use but this segfaulted the code need second magic incantation
        //delete pu_central;
        //delete pu_up;
        //delete pu_down;
    }

    void getPileup_Sys(NTupleReader& tr)
    {
        std::string truNpvName;
        if(tr.checkBranch("met"))
        {
            truNpvName = "tru_npv";
        }
        else if (tr.checkBranch("MET"))
        {
            truNpvName = "TrueNumInteractions";
        }

        data_t _PUweightFactor = 1.;
        data_t _PUSysUp = 1.;
        data_t _PUSysDown = 1.;

        data_t trueint; 
        data_t w = 1.;

        //These are all the pileup varaibles the only one that is needed is the first
        const auto& tru_npv  = tr.getVar<data_t>(truNpvName);
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
        tr.registerDerivedVar("_PUSysUp", _PUSysUp);
        tr.registerDerivedVar("_PUSysDown", _PUSysDown);
    }

    void operator()(NTupleReader& tr)
    {
        //
        getPileup_Sys(tr);
    }

protected:
    
private: 
    //need to define the histogram but not fill it till the c file
    TH1F * pu_central;
    TH1F * pu_up;
    TH1F * pu_down;

};

//Hack to keep name the same for people down stream
class Pileup_Sys : Pileup_SysTemplate<float>{};

#endif // Pileup_Sys_H
