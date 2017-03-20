#include "BTagCorrector.h"
#include "BTagCalibrationStandalone.h"
#include "NTupleReader.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TMath.h"
//STL headers
#include <string>
#include <vector>
#include<cmath>

#include "customize.h"

using namespace std;


/***********************************************************************************/
// Method 1b) in twiki
// https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods
/***********************************************************************************/

vector<double>* BTagCorrector::GetCorrections(const vector<TLorentzVector> *Jets, const vector<int> *Jets_flavor)
{

    //reset probabilities
    vector<double> *prob = new vector<double>(4,0.0);
    (*prob)[0] = 1.0;
  
    //first loop over jets
    vector<vector<double> > sfEffLists = vector<vector<double> >(Jets->size(),vector<double>());
    for(unsigned ja = 0; ja < Jets->size(); ++ja){
        //HT jet cuts
        if(Jets->at(ja).Pt()< 30.0 || fabs(Jets->at(ja).Eta()) > 2.4) continue;

  
        //get sf and eff values (checks if already calculated)
        InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
        double eps_a = sfEffLists[ja][0]*sfEffLists[ja][1]*sfEffLists[ja][2];
  
        //jet index, pt, eta, flavor, eff, sf, cf
	
        if(debug) cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja)) 
                       << " sfEffLists[ja][0], " << sfEffLists[ja][0] << "  sfEffLists[ja][1], " << sfEffLists[ja][1] << "  sfEffLists[ja][2], " << sfEffLists[ja][2] << endl;
  
        //calculate prob(0 b-tags)
        (*prob)[0] *= (1-eps_a);
	 
        //sub-probabilities for following calculations
        double subprob1 = 1.0;
        double subprob2 = 0.0;
	 
        //second loop over jets
        for(unsigned jb = 0; jb < Jets->size(); ++jb){
            //skip the same jet
            if(jb==ja) continue;
	   
            //HT jet cuts
	   
            if(Jets->at(ja).Pt()<30.0 ||fabs(Jets->at(ja).Eta()) > 2.4) continue;
	   
	   
            //get sf and eff values (checks if already calculated)
            InitSFEff(Jets->at(jb).Pt(), Jets->at(jb).Eta(), Jets_flavor->at(jb), sfEffLists[jb]);
	   
            double eps_b = sfEffLists[jb][0]*sfEffLists[jb][1]*sfEffLists[jb][2];
	   
            //jet index, pt, eta, flavor, eff, sf, cf
	    
            if(debug) cout << "\tJet " << jb << ": " << Jets->at(jb).Pt() << ", " << fabs(Jets->at(jb).Eta()) << ", " << abs(Jets_flavor->at(jb)) 
                           << ", " << sfEffLists[jb][0] << ", " << sfEffLists[jb][1] << ", " << sfEffLists[jb][2] << endl;
	   
            //calculate prob(1 b-tag)
            subprob1 *= (1-eps_b);
	   
            //sub-sub-probability for following calculations
            double subsubprob2 = 1.0;
	   
            //third loop over jets (only for jb>ja)
            if(jb<ja) continue;
            for(unsigned jc = 0; jc < Jets->size(); ++jc){
                //skip the same jet
                if(jc==jb || jc==ja) continue;
	     
                //HT jet cuts
                if(Jets->at(ja).Pt()<30.0 ||fabs(Jets->at(ja).Eta()) > 2.4) continue;
	     
                //get sf and eff values (checks if already calculated)
                InitSFEff(Jets->at(jc).Pt(), Jets->at(jc).Eta(), Jets_flavor->at(jc), sfEffLists[jc]);
                double eps_c = sfEffLists[jc][0]*sfEffLists[jc][1]*sfEffLists[jc][2];
	     
                //jet index, pt, eta, flavor, eff, sf, cf
		
                if(debug) cout << "\t\tJet " << jc << ": " << Jets->at(jc).Pt() << ", " << fabs(Jets->at(jc).Eta()) << ", " << abs(Jets_flavor->at(jc)) 
                               << ", " << sfEffLists[jc][0] << ", " << sfEffLists[jc][1] << ", " << sfEffLists[jc][2] << endl;
	     
                //calculate prob(2 b-tags)
                subsubprob2 *= (1-eps_c);
            }
	   
            //add up sub-sub-prob
            subprob2 += eps_b*subsubprob2;
        }
	 
        //add up sub-probs
        (*prob)[1] += eps_a*subprob1;
        (*prob)[2] += eps_a*subprob2;
    }
       
    //conserve probability
       
    (*prob)[3] = 1 - (*prob)[0] -(*prob)[1] - (*prob)[2];
    if(debug) cout << (*prob)[0] << ", " << (*prob)[1] << ", " << (*prob)[2] << ", " << (*prob)[3] << endl;
       
    return prob;
}

/***********************************************************************************/
//method 1a in twiki
// https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods  
/***********************************************************************************/



double BTagCorrector::GetSimpleCorrection(const vector<TLorentzVector> *Jets, const vector<int> *Jets_flavor, const vector<double> *Jets_bDiscriminatorCSV){

    double mcTag = 1.;
    double mcNoTag = 1.;
    double dataTag = 1.;
    double dataNoTag = 1.;
  
    //loop over jets
    vector<vector<double> > sfEffLists = vector<vector<double> >(Jets->size(),vector<double>());
    for(unsigned ja = 0; ja < Jets->size(); ++ja){
        //HT jet cuts
        if(Jets->at(ja).Pt()<30.0 || fabs(Jets->at(ja).Eta()) > 2.4) continue;
    
        //get sf and eff values (checks if already calculated)
        InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
        double eff_a = sfEffLists[ja][0]; //eff
        double cf_a = sfEffLists[ja][2]; //CF
        double sf_a = sfEffLists[ja][1];
    
        if( sfEffLists[ja][0] ==0 || sfEffLists[ja][1] ==0 || sfEffLists[ja][2] ==0 ){
           if(debug) std::cout<<"sfEffLists[ja][0] : "<<sfEffLists[ja][0]<<"  sfEffLists[ja][1] : "<<sfEffLists[ja][1]<<"  sfEffLists[ja][2] : "<<sfEffLists[ja][2]<<std::endl;
        }

        //jet index, pt, eta, flavor, csv, eff, sf, cf
	// if(debug) cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja))  << ", " << Jets_bDiscriminatorCSV->at(ja)
	//                       << ", " << sfEffLists[ja][0] << ", " << sfEffLists[ja][1] << ", " << sfEffLists[ja][2] << endl;
    
        if(Jets_bDiscriminatorCSV->at(ja) > AnaConsts::cutCSVS){
            mcTag *= eff_a*cf_a;
            dataTag *= eff_a*cf_a*sf_a;
        } else {
            mcNoTag *= (1-eff_a*cf_a);
            dataNoTag *= (1-eff_a*cf_a*sf_a);
        }
    }

    double result = (mcNoTag * mcTag ==0) ? 1.0 : (dataNoTag * dataTag)/(mcNoTag * mcTag);

    return result;
}


/***********************************************************************************/
//helper function
void BTagCorrector::InitSFEff(double pt, double eta, int flav, vector<double>& sfEffList){
    //avoid rerunning this
    sfEffList.clear();
    if(sfEffList.size()>0) return;
  
    //use abs(eta) for now
    eta = fabs(eta);
    //use abs(flav) always
    flav = abs(flav);
    
    sfEffList = vector<double>(3,1.0); //eff, sf (central, up, or down), cf (central, up, or down)

    const double max_btagSF_pt = 670.0, max_fastsim_btagCF_pt = 800.0;
    const double max_ctagSF_pt = 670.0, max_fastsim_ctagCF_pt = 800.0;
    const double max_udsgSF_pt = 1000.0, max_fastsim_udsgCF_pt = 800.0;

    /********************************************************************/  
    if(flav==5)
    { //b-tag
      // Double Uncertainty are now taken care automaticall with method eval_auto_bounds
      //in new interface.
        int pt_bin = h_eff_b->GetXaxis()->FindBin(pt); 
        if( pt_bin > h_eff_b->GetXaxis()->GetNbins() ) pt_bin = h_eff_b->GetXaxis()->GetNbins(); 
        int eta_bin = h_eff_b->GetYaxis()->FindBin(eta); 
        if ( eta_bin > h_eff_b->GetYaxis()->GetNbins() ) eta_bin = h_eff_b->GetYaxis()->GetNbins();

        sfEffList[0] = h_eff_b->GetBinContent(pt_bin, eta_bin);

        sfEffList[1] = (btagSFunc==0 ? reader.eval_auto_bounds("central",BTagEntry::FLAV_B,eta,pt) :
                        (btagSFunc==1 ? readerUp.eval_auto_bounds("up",BTagEntry::FLAV_B,eta,pt) :
                         readerDown.eval_auto_bounds("down",BTagEntry::FLAV_B,eta,pt) ) );
       
        if(fastsim)
	{
	  sfEffList[2] = (btagCFunc==0 ? readerFast.eval_auto_bounds("central",BTagEntry::FLAV_B,eta,pt) :
			  (btagCFunc==1 ? readerFastUp.eval_auto_bounds("up",BTagEntry::FLAV_B,eta,pt) :
			   readerFastDown.eval_auto_bounds("down", BTagEntry::FLAV_B,eta,pt) ) );
           
	}
    }
    /********************************************************************/
    else if(flav==4)
    { //charm mistag
        int pt_bin = h_eff_c->GetXaxis()->FindBin(pt); 
        if( pt_bin > h_eff_c->GetXaxis()->GetNbins() ) pt_bin = h_eff_c->GetXaxis()->GetNbins();
        int eta_bin = h_eff_c->GetYaxis()->FindBin(eta); 
        if ( eta_bin > h_eff_c->GetYaxis()->GetNbins() ) eta_bin = h_eff_c->GetYaxis()->GetNbins();
        sfEffList[0] =h_eff_c->GetBinContent(pt_bin, eta_bin);

        sfEffList[1] = (btagSFunc==0 ? reader.eval_auto_bounds("central",BTagEntry::FLAV_C,eta,pt) :
                        (btagSFunc==1 ? readerUp.eval_auto_bounds("up",BTagEntry::FLAV_C,eta,pt) :
                         readerDown.eval_auto_bounds("down", BTagEntry::FLAV_C,eta,pt) ) );
         if(fastsim)
	{
	  sfEffList[2] = (ctagCFunc==0 ? readerFast.eval_auto_bounds("central",BTagEntry::FLAV_C,eta,pt) :
			  (ctagCFunc==1 ? readerFastUp.eval_auto_bounds("up",BTagEntry::FLAV_C,eta,pt) :
			   readerFastDown.eval_auto_bounds("down", BTagEntry::FLAV_C,eta,pt) ) );

       	}
    }
    /********************************************************************/
    else if(flav<4 || flav==21)
    { //udsg mistag
      int pt_bin = h_eff_udsg->GetXaxis()->FindBin(pt); 
      if( pt_bin > h_eff_udsg->GetXaxis()->GetNbins() ) pt_bin = h_eff_udsg->GetXaxis()->GetNbins(); 
      int eta_bin = h_eff_udsg->GetYaxis()->FindBin(eta); 
      if ( eta_bin > h_eff_udsg->GetYaxis()->GetNbins() ) eta_bin = h_eff_udsg->GetYaxis()->GetNbins();

      sfEffList[0] = h_eff_udsg->GetBinContent( pt_bin, eta_bin);

      sfEffList[1] = (mistagSFunc==0 ? reader.eval_auto_bounds("central",BTagEntry::FLAV_UDSG,eta,pt) :
		      (mistagSFunc==1 ? readerUp.eval_auto_bounds("up",BTagEntry::FLAV_UDSG,eta,pt) :
		       readerDown.eval_auto_bounds("down", BTagEntry::FLAV_UDSG,eta,pt) ) );
       
    
        if(fastsim)
	{
	  sfEffList[2] = (mistagCFunc==0 ? readerFast.eval_auto_bounds("central", BTagEntry::FLAV_UDSG,eta,pt) :
			  (mistagCFunc==1 ? readerFastUp.eval_auto_bounds("up",BTagEntry::FLAV_UDSG,eta,pt) :
			   readerFastDown.eval_auto_bounds("down", BTagEntry::FLAV_UDSG,eta,pt) ) );
    
	}
    }
  
}



/***********************************************************************************/

void BTagCorrector::registerVarToNTuples(NTupleReader& tr)
{
    //Check if this is data by looking for "genDecayPdgIdVec" collection (yes, this is dumb)
    if(!tr.checkBranch("genDecayPdgIdVec")) return;

    const vector<TLorentzVector>& inputJets = tr.getVec<TLorentzVector>("jetsLVec");
    const vector<double>& recoJetsBtag = tr.getVec<double>("recoJetsBtag_0");
    const vector<int>& recoJetsFlavor = tr.getVec<int>("recoJetsFlavor");

    /*************************************************/
    // Here we define which(up, down or central
    // to be stored central = 0, up = 1 down = else
    /*************************************************/
    
    /*************************************************/
    // Case 0: Central value;
    /*************************************************/

    int switch_Unc = 0, switch_udsg_Unc = 0;
    SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
    SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
    SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
    //Method 1a) ignoring b-tag status 
    double evtWeightSimple_Central  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
    if( std::isnan( evtWeightSimple_Central) || std::isinf(evtWeightSimple_Central) ){
      evtWeightSimple_Central = 1.0;
    } 

    // Method 1b) in different b-jet mullticipity bins.
    vector<double> *evtWeightProb_Central = GetCorrections(&inputJets, &recoJetsFlavor);
    //Register derived quantities to nTuples.
    tr.registerDerivedVar("bTagSF_EventWeightSimple_Central", evtWeightSimple_Central);
    //evtWeightProb[0] = probability of 0 Btags...... evtWeightProb[3] = probability of 3 Btags
    //put event in each btag bin, weighted by evtWeightprob[0], evtWeightprob[1],
    // evtWeightprob[2], evtWeightprob[3] for nb = 0, 1, 2, 3+
    tr.registerDerivedVec("bTagSF_EventWeightProb_Central", evtWeightProb_Central);


    /*************************************************/
    // Case 1: Up  value;                            
    /*************************************************/

    switch_Unc = 1; switch_udsg_Unc = 0;
    SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
    SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
    SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
    double evtWeightSimple_Up  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
    if( std::isnan( evtWeightSimple_Up) || std::isinf(evtWeightSimple_Up) ){
      evtWeightSimple_Up= 1.0;
    }
    vector<double> *evtWeightProb_Up = GetCorrections(&inputJets, &recoJetsFlavor);
    tr.registerDerivedVar("bTagSF_EventWeightSimple_Up", evtWeightSimple_Up);
    tr.registerDerivedVec("bTagSF_EventWeightProb_Up", evtWeightProb_Up);


    /*************************************************/
    // Case -1:Down  value;                            
    /*************************************************/

    switch_Unc = -1; switch_udsg_Unc = 0;
    SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
    SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
    SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
    double evtWeightSimple_Down  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
    if( std::isnan( evtWeightSimple_Down) || std::isinf(evtWeightSimple_Down) ){
      evtWeightSimple_Down= 1.0;
    }
    vector<double> *evtWeightProb_Down = GetCorrections(&inputJets, &recoJetsFlavor);
    tr.registerDerivedVar("bTagSF_EventWeightSimple_Down", evtWeightSimple_Down);
    tr.registerDerivedVec("bTagSF_EventWeightProb_Down", evtWeightProb_Down);

   
    /*************************************************/
    // Mistag (udsg) Case 1: Up  value;                            
    /*************************************************/

    switch_Unc = 0; switch_udsg_Unc = 1;
    SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
    SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
    SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
    double evtWeightSimple_mistag_Up  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
    if( std::isnan( evtWeightSimple_mistag_Up) || std::isinf(evtWeightSimple_mistag_Up) ){
      evtWeightSimple_mistag_Up= 1.0;
    }
    vector<double> *evtWeightProb_mistag_Up =  GetCorrections(&inputJets, &recoJetsFlavor);
    tr.registerDerivedVar("mistagSF_EventWeightSimple_Up", evtWeightSimple_mistag_Up);
    tr.registerDerivedVec("mistagSF_EventWeightProb_Up", evtWeightProb_mistag_Up);


    /*************************************************/
    // Case -1:Down  value;                            
    /*************************************************/

    switch_Unc = 0; switch_udsg_Unc = -1;
    SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
    SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
    SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
    double evtWeightSimple_mistag_Down  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
    if( std::isnan( evtWeightSimple_mistag_Down) || std::isinf(evtWeightSimple_mistag_Down) ){
      evtWeightSimple_mistag_Down= 1.0;
    }
    vector<double> *evtWeightProb_mistag_Down = GetCorrections(&inputJets, &recoJetsFlavor);
    tr.registerDerivedVar("mistagSF_EventWeightSimple_Down", evtWeightSimple_mistag_Down);
    tr.registerDerivedVec("mistagSF_EventWeightProb_Down", evtWeightProb_mistag_Down);

    /*************************************************/
    // Example to use these variables are in
    // https://github.com/humkies/bTag/blob/master/bTagScaleFactor.C#L151                                                                             
    /*************************************************/

}



/***********************************************************************************/
void BTagCorrector::operator()(NTupleReader& tr)
{
    //
    registerVarToNTuples(tr);
}
/***********************************************************************************/


