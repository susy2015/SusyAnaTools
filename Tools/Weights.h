#ifndef WEIGHTS_H
#define WEIGHTS_H

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

};

#endif // Pileup_Sys_H

