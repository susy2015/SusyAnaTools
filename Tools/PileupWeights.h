#ifndef PILEUPWEIGHTS_H
#define PILEUPWEIGHTS_H

#include "NTupleReader.h"
#include "TH1.h"


class Pileup_Sys
{
public:
    Pileup_Sys(std::string filename="PileupHistograms_Nov17.root");
    virtual ~Pileup_Sys();
    void getPileup_Sys(NTupleReader& tr);
    void operator()(NTupleReader& tr);

protected:

private: 
    //need to define the histogram but not fill it till the c file
    TH1F * pu_central;
    TH1F * pu_up;
    TH1F * pu_down;
    std::string tru_npv_, BX_, nm1_, n0_, np1_, npv_;

    void setTreeNames(const NTupleReader& tr)
    {
        if(tr.checkBranch("met"))
        {
            tru_npv_ = "tru_npv";
            BX_ = "BX";
            nm1_ = "nm1";
            n0_ = "n0";
            np1_ = "np1";
            npv_ = "npv";            
        }
        else if(tr.checkBranch("MET"))
        {
            tru_npv_ = "";
            BX_ = "";
            nm1_ = "";
            n0_ = "";
            np1_ = "";
            npv_ = "";
        }
    }
};

#endif // Pileup_Sys_H

