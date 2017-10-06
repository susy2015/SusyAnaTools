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
    bool MapMET() const;
    bool MapJets() const;

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class StopleAlias  -----

