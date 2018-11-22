#ifndef BTAGCORRECTOR_H
#define BTAGCORRECTOR_H

//custom headers
#include "BTagCalibrationStandalone.h"
#include "NTupleReader.h"

//ROOT headers
#include <TROOT.h>
#include "TMath.h"
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TH2.h"

//STL headers
#include <string>
#include <vector>
#include <cmath>

#include "customize.h"

template<typename data_t>
class BTagCorrectorTemplate 
{
public:
    //constructor
    BTagCorrectorTemplate(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", std::string CSVFile = "CSVv2_Moriond17_B_H.csv", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : debug(false), fastsim(false), btagSFunc(0), mistagSFunc(0), btagCFunc(0), ctagCFunc(0), mistagCFunc(0), h_eff_b(NULL), h_eff_c(NULL), h_eff_udsg(NULL), MCBranch("genDecayPdgIdVec"), JetsVec("jetsLVec"), BJetsVec("recoJetsCSVv2"), JetsFlavor("recoJetsFlavor"), myVarSuffix_("")
    {
        //Stops unwanted segfaults.
        TH1::AddDirectory(false);

        //Hard Coded here.
        // Replace "TTbarNoHad_bTagEff.root" in constuctor  with desirable efficiency file
        // Same "efficiency file and MC sample to run over must be same
        //Efficiency can be calculated using bTagEfficiencyCalc.C

        inFile = new TFile(file.c_str());
        SetEffs(inFile, suffix);
        SetCalib(CSVFile.c_str());	  
        if(CSVFilePath.size())
        {
            SetCalib((CSVFilePath + "/" + CSVFile).c_str());
        }
        else
        {
            SetCalib(CSVFile.c_str());
        }
        
        if(isFastSim)
        {
            //FastSim
            SetFastSim(true);
            SetCalibFastSim("fastsim_csvv2_ttbar_26_1_2017.csv");
        }
    }

    BTagCorrectorTemplate(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : BTagCorrectorTemplate(file,CSVFilePath,"CSVv2_Moriond17_B_H.csv",isFastSim,suffix)
    {
    }

    //destructor
    // virtual ~BTagCorrectorTemplate() {}

    //accessors
    void SetDebug(bool d) { debug = d; }
    void SetFastSim(bool f) { fastsim = f; }
    void SetEffs(TFile* file, std::string suffix = "")
    {
        if(suffix.size())
        {
            std::string suffix2 = suffix;
            h_eff_b = (TH2F*)file->Get(("n_eff_b_" + suffix2).c_str());
            if(!h_eff_b)
            {
                std::cout << "Could not find n_eff_b_" << suffix2 << " in Btag file. Will use TTbarSingleLepT as default" << std::endl;
                suffix2 = "TTbarSingleLepT"; // random default value, otherwise you get segfault
                h_eff_b = (TH2F*)file->Get(("n_eff_b_" + suffix2).c_str());
            }
            h_eff_c = (TH2F*)file->Get(("n_eff_c_" + suffix2).c_str());
            h_eff_udsg = (TH2F*)file->Get(("n_eff_udsg_" + suffix2).c_str());
            TH2F *d_eff_b = (TH2F*)file->Get(("d_eff_b_" + suffix2).c_str());
            TH2F *d_eff_c = (TH2F*)file->Get(("d_eff_c_" + suffix2).c_str());
            TH2F *d_eff_udsg = (TH2F*)file->Get(("d_eff_udsg_" + suffix2).c_str());
            
            h_eff_b->Divide(d_eff_b);
            h_eff_c->Divide(d_eff_c);
            h_eff_udsg->Divide(d_eff_udsg);
            
        }
        else
        {
            h_eff_b = (TH2F*)file->Get("n_eff_b");
            h_eff_c = (TH2F*)file->Get("n_eff_c");
            h_eff_udsg = (TH2F*)file->Get("n_eff_udsg");

	    TH2F *d_eff_b = (TH2F*)file->Get("d_eff_b");
	    TH2F *d_eff_c = (TH2F*)file->Get("d_eff_c");
            TH2F *d_eff_udsg = (TH2F*)file->Get("d_eff_udsg");
            
            if(h_eff_b)
            {
                h_eff_b->Divide(d_eff_b);
                h_eff_c->Divide(d_eff_c);
                h_eff_udsg->Divide(d_eff_udsg);
            }
        }
    }
    void resetEffs(std::string suffix)
    {
        if(h_eff_b) delete h_eff_b;
        if(h_eff_c) delete h_eff_c;
        if(h_eff_udsg) delete h_eff_udsg;
        SetEffs(inFile, suffix);
    }
    void SetCalib(std::string cfile)
    {        
        //initialize btag helper classes. Interface has been changed.
        calib = BTagCalibration("",cfile);
        reader = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central");
        reader.load(calib, BTagEntry::FLAV_B, "comb"); reader.load(calib, BTagEntry::FLAV_C, "comb");  reader.load(calib, BTagEntry::FLAV_UDSG, "incl");
        readerUp = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "up");
        readerUp.load(calib, BTagEntry::FLAV_B, "comb"); readerUp.load(calib, BTagEntry::FLAV_C, "comb");  readerUp.load(calib, BTagEntry::FLAV_UDSG, "incl");
        readerDown = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "down");
        readerDown.load(calib, BTagEntry::FLAV_B, "comb"); readerDown.load(calib, BTagEntry::FLAV_C, "comb");  readerDown.load(calib, BTagEntry::FLAV_UDSG, "incl");        
    }
    void SetCalibFastSim(std::string cfile)
    {        
        //read CFs  New 2016 Modifications
        calibFast = BTagCalibration("",cfile);
	readerFast = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central");
	readerFast.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFast.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFast.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim");
	readerFastUp = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "up");
	readerFastUp.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFastUp.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFastUp.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim");
	readerFastDown = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "down");
	readerFastDown.load(calibFast, BTagEntry::FLAV_B, "fastsim"); readerFastDown.load(calibFast, BTagEntry::FLAV_C, "fastsim");  readerFastDown.load(calibFast, BTagEntry::FLAV_UDSG, "fastsim"); 	
    }
    void SetVarNames(std::string MCBranchName, std::string JetsVecName, std::string BJetsVecName, std::string JetsFlavorName, std::string myVarSuffix = "")
    {
        MCBranch = MCBranchName;
        JetsVec = JetsVecName;
        BJetsVec = BJetsVecName;
        JetsFlavor = JetsFlavorName;
        myVarSuffix_ = myVarSuffix;
    }
    
    void SetBtagSFunc(int u) { btagSFunc = u; }
    //void SetCtagSFunc(int u) { ctagSFunc = u; }
    void SetCtagSFunc(int u) { btagSFunc = u; } //ctag and btag are correlated
    void SetMistagSFunc(int u) { mistagSFunc = u; }
    void SetBtagCFunc(int u) { btagCFunc = u; }
    void SetCtagCFunc(int u) { ctagCFunc = u; }
    void SetMistagCFunc(int u) { mistagCFunc = u; }
    
    //test Method
    std::vector<data_t> GetCorrectionsTest(std::vector<TLorentzVector>* Jets, std::vector<int>* Jets_flavor);

    /***********************************************************************************/
    // Method 1b) in twiki
    // https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods
    /***********************************************************************************/
    std::vector<data_t>* GetCorrections(const std::vector<TLorentzVector> *Jets, const std::vector<int> *Jets_flavor)
    {
        //reset probabilities
        std::vector<data_t> *prob = new std::vector<data_t>(4,0.0);
        (*prob)[0] = 1.0;
  
        //first loop over jets
        std::vector<std::vector<data_t> > sfEffLists = std::vector<std::vector<data_t> >(Jets->size(),std::vector<data_t>());
        for(unsigned ja = 0; ja < Jets->size(); ++ja){
            //HT jet cuts
            if(Jets->at(ja).Pt()< 30.0 || fabs(Jets->at(ja).Eta()) > 2.4) continue;

            //get sf and eff values (checks if already calculated)
            InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
            data_t eps_a = sfEffLists[ja][0]*sfEffLists[ja][1]*sfEffLists[ja][2];
  
            //jet index, pt, eta, flavor, eff, sf, cf
	
            if(debug) std::cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja)) 
                                << " sfEffLists[ja][0], " << sfEffLists[ja][0] << "  sfEffLists[ja][1], " << sfEffLists[ja][1] << "  sfEffLists[ja][2], " << sfEffLists[ja][2] << std::endl;
  
            //calculate prob(0 b-tags)
            (*prob)[0] *= (1-eps_a);
	 
            //sub-probabilities for following calculations
            data_t subprob1 = 1.0;
            data_t subprob2 = 0.0;
	 
            //second loop over jets
            for(unsigned jb = 0; jb < Jets->size(); ++jb){
                //skip the same jet
                if(jb==ja) continue;
	   
                //HT jet cuts
	   
                if(Jets->at(ja).Pt()<30.0 ||fabs(Jets->at(ja).Eta()) > 2.4) continue;
	   
	   
                //get sf and eff values (checks if already calculated)
                InitSFEff(Jets->at(jb).Pt(), Jets->at(jb).Eta(), Jets_flavor->at(jb), sfEffLists[jb]);
	   
                data_t eps_b = sfEffLists[jb][0]*sfEffLists[jb][1]*sfEffLists[jb][2];
	   
                //jet index, pt, eta, flavor, eff, sf, cf
	    
                if(debug) std::cout << "\tJet " << jb << ": " << Jets->at(jb).Pt() << ", " << fabs(Jets->at(jb).Eta()) << ", " << abs(Jets_flavor->at(jb)) 
                                    << ", " << sfEffLists[jb][0] << ", " << sfEffLists[jb][1] << ", " << sfEffLists[jb][2] << std::endl;
	   
                //calculate prob(1 b-tag)
                subprob1 *= (1-eps_b);
	   
                //sub-sub-probability for following calculations
                data_t subsubprob2 = 1.0;
	   
                //third loop over jets (only for jb>ja)
                if(jb<ja) continue;
                for(unsigned jc = 0; jc < Jets->size(); ++jc){
                    //skip the same jet
                    if(jc==jb || jc==ja) continue;
	     
                    //HT jet cuts
                    if(Jets->at(ja).Pt()<30.0 ||fabs(Jets->at(ja).Eta()) > 2.4) continue;
	     
                    //get sf and eff values (checks if already calculated)
                    InitSFEff(Jets->at(jc).Pt(), Jets->at(jc).Eta(), Jets_flavor->at(jc), sfEffLists[jc]);
                    data_t eps_c = sfEffLists[jc][0]*sfEffLists[jc][1]*sfEffLists[jc][2];
	     
                    //jet index, pt, eta, flavor, eff, sf, cf
		
                    if(debug) std::cout << "\t\tJet " << jc << ": " << Jets->at(jc).Pt() << ", " << fabs(Jets->at(jc).Eta()) << ", " << abs(Jets_flavor->at(jc)) 
                                        << ", " << sfEffLists[jc][0] << ", " << sfEffLists[jc][1] << ", " << sfEffLists[jc][2] << std::endl;
	     
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
        if(debug) std::cout << (*prob)[0] << ", " << (*prob)[1] << ", " << (*prob)[2] << ", " << (*prob)[3] << std::endl;
       
        return prob;
    }

    /***********************************************************************************/
    //method 1a in twiki
    // https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods  
    /***********************************************************************************/
    data_t GetSimpleCorrection(const std::vector<TLorentzVector> *Jets, const std::vector<int> *Jets_flavor, const std::vector<data_t> *Jets_bDiscriminatorCSV){

        data_t mcTag = 1.;
        data_t mcNoTag = 1.;
        data_t dataTag = 1.;
        data_t dataNoTag = 1.;
  
        //loop over jets
        std::vector<std::vector<data_t> > sfEffLists = std::vector<std::vector<data_t> >(Jets->size(),std::vector<data_t>());
        for(unsigned ja = 0; ja < Jets->size(); ++ja){
            //HT jet cuts
            if(Jets->at(ja).Pt()<30.0 || fabs(Jets->at(ja).Eta()) > 2.4) continue;
    
            //get sf and eff values (checks if already calculated)
            InitSFEff(Jets->at(ja).Pt(), Jets->at(ja).Eta(), Jets_flavor->at(ja), sfEffLists[ja]);
            data_t eff_a = sfEffLists[ja][0]; //eff
            data_t cf_a = sfEffLists[ja][2]; //CF
            data_t sf_a = sfEffLists[ja][1];
    
            if( sfEffLists[ja][0] ==0 || sfEffLists[ja][1] ==0 || sfEffLists[ja][2] ==0 ){
                if(debug) std::cout<<"sfEffLists[ja][0] : "<<sfEffLists[ja][0]<<"  sfEffLists[ja][1] : "<<sfEffLists[ja][1]<<"  sfEffLists[ja][2] : "<<sfEffLists[ja][2]<<std::endl;
            }

            //jet index, pt, eta, flavor, csv, eff, sf, cf
            // if(debug) std::cout << "Jet " << ja << ": " << Jets->at(ja).Pt() << ", " << fabs(Jets->at(ja).Eta()) << ", " << abs(Jets_flavor->at(ja))  << ", " << Jets_bDiscriminatorCSV->at(ja)
            //                       << ", " << sfEffLists[ja][0] << ", " << sfEffLists[ja][1] << ", " << sfEffLists[ja][2] << std::endl;
    
            if(Jets_bDiscriminatorCSV->at(ja) > AnaConsts::cutCSVS){
                mcTag *= eff_a*cf_a;
                dataTag *= eff_a*cf_a*sf_a;
            } else {
                mcNoTag *= (1-eff_a*cf_a);
                dataNoTag *= (1-eff_a*cf_a*sf_a);
            }
        }

        data_t result = (mcNoTag * mcTag ==0) ? 1.0 : (dataNoTag * dataTag)/(mcNoTag * mcTag);

        return result;
    }

    void InitSFEff(data_t pt, data_t eta, int flav, std::vector<data_t>& sfEffList)
    {
        //avoid rerunning this
        sfEffList.clear();
        if(sfEffList.size()>0) return;
  
        //use abs(eta) for now
        eta = fabs(eta);
        //use abs(flav) always
        flav = abs(flav);
    
        sfEffList = std::vector<data_t>(3,1.0); //eff, sf (central, up, or down), cf (central, up, or down)

        const data_t max_btagSF_pt = 670.0, max_fastsim_btagCF_pt = 800.0;
        const data_t max_ctagSF_pt = 670.0, max_fastsim_ctagCF_pt = 800.0;
        const data_t max_udsgSF_pt = 1000.0, max_fastsim_udsgCF_pt = 800.0;

        /********************************************************************/  
        if(flav==5)
        { //b-tag
            // data_t Uncertainty are now taken care automaticall with method eval_auto_bounds
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

    // To register Event weights/ Probabilities to FlatTuples
    void registerVarToNTuples(NTupleReader& tr)
    {
        //Check if this is data
        if( !tr.checkBranch(MCBranch) ) return;
        const auto& inputJets = tr.getVec<TLorentzVector>(JetsVec);
        const auto& recoJetsBtag = tr.getVec<data_t>(BJetsVec);
        const auto& recoJetsFlavor = tr.getVec<int>(JetsFlavor);

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
        data_t evtWeightSimple_Central  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
        if( std::isnan( evtWeightSimple_Central) || std::isinf(evtWeightSimple_Central) ){
            evtWeightSimple_Central = 1.0;
        } 

        // Method 1b) in different b-jet mullticipity bins.
        std::vector<data_t> *evtWeightProb_Central = GetCorrections(&inputJets, &recoJetsFlavor);
        //Register derived quantities to nTuples.
        tr.registerDerivedVar("bTagSF_EventWeightSimple_Central"+myVarSuffix_, evtWeightSimple_Central);
        //evtWeightProb[0] = probability of 0 Btags...... evtWeightProb[3] = probability of 3 Btags
        //put event in each btag bin, weighted by evtWeightprob[0], evtWeightprob[1],
        // evtWeightprob[2], evtWeightprob[3] for nb = 0, 1, 2, 3+
        tr.registerDerivedVec("bTagSF_EventWeightProb_Central"+myVarSuffix_, evtWeightProb_Central);


        /*************************************************/
        // Case 1: Up  value;                            
        /*************************************************/

        switch_Unc = 1; switch_udsg_Unc = 0;
        SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
        SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
        SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
        data_t evtWeightSimple_Up  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
        if( std::isnan( evtWeightSimple_Up) || std::isinf(evtWeightSimple_Up) ){
            evtWeightSimple_Up= 1.0;
        }
        std::vector<data_t> *evtWeightProb_Up = GetCorrections(&inputJets, &recoJetsFlavor);
        tr.registerDerivedVar("bTagSF_EventWeightSimple_Up"+myVarSuffix_, evtWeightSimple_Up);
        tr.registerDerivedVec("bTagSF_EventWeightProb_Up"+myVarSuffix_, evtWeightProb_Up);


        /*************************************************/
        // Case -1:Down  value;                            
        /*************************************************/

        switch_Unc = -1; switch_udsg_Unc = 0;
        SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
        SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
        SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
        data_t evtWeightSimple_Down  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
        if( std::isnan( evtWeightSimple_Down) || std::isinf(evtWeightSimple_Down) ){
            evtWeightSimple_Down= 1.0;
        }
        std::vector<data_t> *evtWeightProb_Down = GetCorrections(&inputJets, &recoJetsFlavor);
        tr.registerDerivedVar("bTagSF_EventWeightSimple_Down"+myVarSuffix_, evtWeightSimple_Down);
        tr.registerDerivedVec("bTagSF_EventWeightProb_Down"+myVarSuffix_, evtWeightProb_Down);

   
        /*************************************************/
        // Mistag (udsg) Case 1: Up  value;                            
        /*************************************************/

        switch_Unc = 0; switch_udsg_Unc = 1;
        SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
        SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
        SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
        data_t evtWeightSimple_mistag_Up  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
        if( std::isnan( evtWeightSimple_mistag_Up) || std::isinf(evtWeightSimple_mistag_Up) ){
            evtWeightSimple_mistag_Up= 1.0;
        }
        std::vector<data_t> *evtWeightProb_mistag_Up =  GetCorrections(&inputJets, &recoJetsFlavor);
        tr.registerDerivedVar("mistagSF_EventWeightSimple_Up"+myVarSuffix_, evtWeightSimple_mistag_Up);
        tr.registerDerivedVec("mistagSF_EventWeightProb_Up"+myVarSuffix_, evtWeightProb_mistag_Up);


        /*************************************************/
        // Case -1:Down  value;                            
        /*************************************************/

        switch_Unc = 0; switch_udsg_Unc = -1;
        SetBtagSFunc(switch_Unc); SetBtagCFunc(switch_Unc);
        SetCtagSFunc(switch_Unc); SetCtagCFunc(switch_Unc);
        SetMistagSFunc(switch_udsg_Unc); SetMistagCFunc(switch_udsg_Unc);
        data_t evtWeightSimple_mistag_Down  = GetSimpleCorrection(&inputJets ,&recoJetsFlavor,&recoJetsBtag);
        if( std::isnan( evtWeightSimple_mistag_Down) || std::isinf(evtWeightSimple_mistag_Down) ){
            evtWeightSimple_mistag_Down= 1.0;
        }
        std::vector<data_t> *evtWeightProb_mistag_Down = GetCorrections(&inputJets, &recoJetsFlavor);
        tr.registerDerivedVar("mistagSF_EventWeightSimple_Down"+myVarSuffix_, evtWeightSimple_mistag_Down);
        tr.registerDerivedVec("mistagSF_EventWeightProb_Down"+myVarSuffix_, evtWeightProb_mistag_Down);

        /*************************************************/
        // Example to use these variables are in
        // https://github.com/humkies/bTag/blob/master/bTagScaleFactor.C#L151                                                                             
        /*************************************************/
    }

    //Operator
    void operator()(NTupleReader& tr)
    {
        registerVarToNTuples(tr);
    }

    //member variables
    bool debug, fastsim;
    int btagSFunc, mistagSFunc;
    int btagCFunc, ctagCFunc, mistagCFunc;
    BTagCalibration calib, calibFast;
    BTagCalibrationReader reader, readerUp, readerDown;
    BTagCalibrationReader readerFast, readerFastUp, readerFastDown;
    TH2F *h_eff_b, *h_eff_c, *h_eff_udsg;
    TFile *inFile;
    std::string MCBranch, JetsVec, BJetsVec, JetsFlavor, myVarSuffix_;
};

//Team hack to keep name the same for people down stream
class BTagCorrector: public BTagCorrectorTemplate<float>
{
  public:
    // constructors with float as default
    BTagCorrector(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", std::string CSVFile = "CSVv2_Moriond17_B_H.csv", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : BTagCorrectorTemplate<float>(file, CSVFilePath, CSVFile, isFastSim, suffix){};
    BTagCorrector(std::string file = "allINone_bTagEff.root", std::string CSVFilePath = "", bool isFastSim = false, std::string suffix = "TTbarSingleLepT") : BTagCorrectorTemplate<float>(file,CSVFilePath,"CSVv2_Moriond17_B_H.csv",isFastSim,suffix){};
};

#endif
