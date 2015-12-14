#ifndef WEIGHTS_H
#define WEIGHTS_H

#include "NTupleReader.h"
#include "TH1.h"
#include "TSystem.h"
#include "TFile.h"


class Pileup_Sys
{
public:
    Pileup_Sys();
    virtual ~Pileup_Sys();
    void getPileup_Sys(NTupleReader& tr);
    void operator()(NTupleReader& tr);

protected:

private: 
    std::string SearchFilePath(std::string name_) const;
    //need to define the histogram but not fill it till the c file
    TH1F * pu_central;
    TH1F * pu_up;
    TH1F * pu_down;

};

#endif // Pileup_Sys_H

