#include "TTbarCorrector.h"
#include "NTupleReader.h"

//ROOT headers
#include <TROOT.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TMath.h"
//STL headers
#include <string>
#include <vector>
#include <cmath>

#include "customize.h"

using namespace std;


/***********************************************************************************/
// TTbar rewighting described in twiki @
// https://twiki.cern.ch/twiki/bin/view/CMS/TopPtReweighting2017
/***********************************************************************************/


/***********************************************************************************/

void TTbarCorrector::registerVarToNTuples(NTupleReader& tr)
{
    //This will only work on a ttbar sample with exactly 2 tops (ttbar), please use this accordingly
    //Check if this is data by looking for "genDecayPdgIdVec" collection (yes, this is dumb)
    if(!tr.checkBranch("genDecayPdgIdVec")) return;

    const vector<TLorentzVector>& genDecayLVec = tr.getVec<TLorentzVector>("genDecayLVec");
    const vector<int>& genDecayPdgIdVec        = tr.getVec<int>("genDecayPdgIdVec");

    const TLorentzVector *t = NULL, *tbar = NULL;

    //Sanity Check
    if(genDecayLVec.size() != genDecayPdgIdVec.size()) return;

    for(int i = 0; i < genDecayLVec.size(); i++){

        if(genDecayPdgIdVec[i] == 6){
            if(!t){
                t = &genDecayLVec[i];
            }else{
                return; //This event has more than one top, it fails our assumptions.
            }
            continue;
        }

        if(genDecayPdgIdVec[i] == -6){
            if(!tbar){
                tbar = &genDecayLVec[i];
            }else{
                return; //This event has more than one top, it fails our assumptions.
            }
            continue;
        }
    }

    if(!t || !tbar) return; //This event does not have both a t and tbar, it fails our assumptions

    tr.registerDerivedVar("TTbarWF", EventWeight( ScaleFactor(t->Perp()), ScaleFactor(tbar->Perp()) ));

}



/***********************************************************************************/
void TTbarCorrector::operator()(NTupleReader& tr)
{
    //
    registerVarToNTuples(tr);
}
/***********************************************************************************/


