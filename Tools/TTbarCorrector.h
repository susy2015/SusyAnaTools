//TopPt reweighting for TTbar samples is described in https://twiki.cern.ch/twiki/bin/view/CMS/TopPtReweighting2017

#ifndef TTBARCORRECTOR_H
#define TTBARCORRECTOR_H

//custom headers
#include "NTupleReader.h"
//ROOT headers
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cmath>

class TTbarCorrector {
public:
    //constructor
    TTbarCorrector(bool isFastSim = false, std::string suffix = "") : debug(false), fastsim(false), a(0.0615), b(-0.0005) {

    }
    //destructor
    virtual ~TTbarCorrector() {}

    //accessors
    void SetDebug(bool d) { debug = d; }
    void SetFastSim(bool f) { fastsim = f; }

    //Change the scale factor parameterization: SF=exp(a+b*pT)
    void SetParameterization(double p_a, double p_b) { a = p_a;  b = p_b; }

    // To register Event weights/ Probabilities to FlatTuples
    void registerVarToNTuples(NTupleReader& tr);
    //Operator
    void operator()(NTupleReader& tr);

private:

    //member variables
    bool debug, fastsim;
    double a, b;

    //Scale Factor
    double ScaleFactor(double pt) { return exp(a+b*pt); }
    double EventWeight(double t_sf, double tbar_sf) { return sqrt(t_sf*tbar_sf); }  

};


#endif







