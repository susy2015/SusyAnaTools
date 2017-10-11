// ===========================================================================
// 
//       Filename:  StopleAlias.h
// 
//    Description:  An interface between TeamA Ntuple and TeamAlpha Ntuple
// 
//        Version:  1.0
//        Created:  08/22/2017 11:50:09 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "NTupleReader.h"
#include "TLorentzVector.h"
#include <sstream>
#include <iostream>

// ===========================================================================
//        Class:  StopleAlias
//  Description:  
// ===========================================================================
class StopleAlias
{
  public:

    // ====================  LIFECYCLE     ===============================
    StopleAlias ();                             // constructor

    // ====================  ACCESSORS     ===============================
    void operator()(NTupleReader& tr_);
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================


    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    NTupleReader *tr;
    std::map<std::string, std::string> AliasMap;


    bool MapGlobalVar();
    bool MapGen();
    bool MapFilter();
    bool MapMuon();
    bool MapElectron();
    bool MapIsoTrack();
    bool MapMET();
    bool MapJets();

    bool addAlias(const std::string &Sfrom, const std::string &Sto);
    template <class Tfrom, class Tto>
    bool MapSingleObj(const std::string Sfrom, const std::string Sto) const;
    template <class Tfrom, class Tto>
    bool MapVectorObj(const std::string Sfrom, const std::string Sto) const;

    bool ProdLepMtw(const std::string &lep, const std::string &outname,
        const std::string s_pt = "pt" , const std::string s_phi  = "phi") const;
    bool MapVectorTLV(const std::string prefix, const std::string outname,
        const std::string s_pt = "pt", const std::string s_eta = "eta",
        const std::string s_phi = "phi", const std::string s_mass = "mass") const;

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class StopleAlias  -----

