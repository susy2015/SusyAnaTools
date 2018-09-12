
//Tools Headers

#include "SusyAnaTools/Tools/SATException.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/samples.h"
#include "SusyAnaTools/Tools/customize.h"

//ROOT headers
#include "TFile.h"
#include "TH2D.h"
#include "TString.h"
#include "TChain.h"

//STL Headers
#include <iostream>
#include <string>
#include <vector>

const float ptBins[]  = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800,99999};
const int nPtBins = sizeof(ptBins)/sizeof(ptBins[0]) - 1;

const float etaBins[] = {0.0,0.8,1.6,2.4};
const int nEtaBins = sizeof(etaBins)/sizeof(etaBins[0]) - 1;

const float csv_btag = AnaConsts::cutCSVS;

const std::string spec = "bTagEff";

// === Main Function ===================================================
int main(int argc, char* argv[]) 
{
    using namespace std;

    TChain *fChain = nullptr;
    
    if (argc < 5)
    {
	std::cerr <<"Please give 4 arguments "<<"SubsampleName"<< " MaxEvent"<< "Startfile"<<" No. of Files to run" <<std::endl;
        std::cerr <<" Valid configurations are " << std::endl;
        std::cerr <<" ./bTagEfficiencyCalc TTbarSingleLepT 1000 1 0" << std::endl;
        return -1;
    }

    const char *subSampleName = argv[1];
    const char *maxevent = argv[2];
    
    // Change string arg to int
    int  maxEvent =  std::atoi(maxevent);
    int numFiles =  std::atoi(argv[3]);
    int startFile =  std::atoi(argv[4]);

    // Prepare file list and finalize it
    TString subSampleNameT = subSampleName;

    const std::string stFile = std::to_string(startFile);
    TString stFileT = stFile;
    TString specT = spec;

    TString FileName = subSampleNameT+"_"+spec+"_"+stFileT;
    TFile *f =  new TFile(FileName+".root", "RECREATE");

    /*************************************************************/      
    //Declare efficiency histograms. n-> numerator d-> denominator
    // b-> bquark, c-> charm, usdg-> other light quarks.
    /*************************************************************/

    TH1::AddDirectory(kFALSE);

    TH2* n_eff_b =    new TH2D("n_eff_b_"+subSampleNameT, "bTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);
    TH2* n_eff_c =    new TH2D("n_eff_c_"+subSampleNameT, "cTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);
    TH2* n_eff_udsg = new TH2D("n_eff_udsg_"+subSampleNameT, "udsgTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);
    TH2* d_eff_b =    new TH2D("d_eff_b_"+subSampleNameT, "bTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);
    TH2* d_eff_c =    new TH2D("d_eff_c_"+subSampleNameT, "cTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);
    TH2* d_eff_udsg = new TH2D("d_eff_udsg_"+subSampleNameT, "udsgTag_Efficiency"+subSampleNameT, nPtBins, ptBins, nEtaBins, etaBins);

    n_eff_b->GetXaxis()->SetTitle("p_{T} [GeV]");
    n_eff_b->GetYaxis()->SetTitle("#eta");

    n_eff_c->GetXaxis()->SetTitle("p_{T} [GeV]");
    n_eff_c->GetYaxis()->SetTitle("#eta");
      
    n_eff_udsg->GetXaxis()->SetTitle("p_{T} [GeV]");
    n_eff_udsg->GetYaxis()->SetTitle("#eta");
 

    AnaSamples::SampleSet        ss("sampleSets.txt", (argc == 6), AnaSamples::luminosity);
                                   
    float ScaleMC = 1.;                                                                              
    if(ss[subSampleName] != ss.null())                                                                             
    {                                                                                                               
        fChain = new TChain(ss[subSampleName].treePath.c_str());                                                           
        ss[subSampleName].addFilesToChain(fChain, startFile, numFiles);

        ScaleMC = ss[subSampleName].getWeight();
    }
    else
    {
        cout << "SampleSet \"" << subSampleName << "\" not found" << endl;
        return 0;
    }

    AnaFunctions::prepareForNtupleReader();
    NTupleReader *tr = new NTupleReader(fChain);

    // --- Analyse events --------------------------------------------
    std::cout<<"First loop begin: "<<std::endl;

    /*************************************************************/
    // Event loop begins                                                                                                            
    /*************************************************************/
    while(tr->getNextEvent())
    {

	if(maxEvent>=0 && tr->getEvtNum() > maxEvent ) break;
	// Add print out of the progress of looping
	if( tr->getEvtNum()-1 == 0 || tr->getEvtNum()%10000 == 0 ) 
        {
	    std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;
        }
	  
	  
        const  vector<TLorentzVector> inputJets = tr->getVec<TLorentzVector>("jetsLVec");
        const vector<float> recoJetsBtag = tr->getVec<float>("recoJetsCSVv2");
        const vector<int> recoJetsFlavor = tr->getVec<int>("recoJetsFlavor");
         
        float iniWeight = tr->getVar<float>("evtWeight");

        float stored_weight = subSampleNameT.Contains("Data") ? 1 : tr->getVar<float>("stored_weight");
        int sign_of_stored_weight = (stored_weight > 0) ? 1 : ((stored_weight < 0) ? -1 : 0);

        float evtWeight = iniWeight >=0 ? iniWeight * sign_of_stored_weight : iniWeight;
     
        for(unsigned int ij=0; ij<inputJets.size(); ij++)
        {
            float pt = inputJets[ij].Pt();
            float eta = fabs(inputJets[ij].Eta());
            if( ! AnaFunctions::jetPassCuts(inputJets[ij], AnaConsts::bTagArr) ) continue;
            float csv = recoJetsBtag.at(ij);
            int flav =  abs(recoJetsFlavor.at(ij));
	      
            if(flav==5) //b Jets
            {
                d_eff_b->Fill(pt,eta, evtWeight);
                if(csv > csv_btag) n_eff_b->Fill(pt,eta, evtWeight);
            }
            else if(flav==4) // c jets
            {
                d_eff_c->Fill(pt,eta, evtWeight);
                if(csv > csv_btag) n_eff_c->Fill(pt,eta, evtWeight);
            }
            else if(flav<4 || flav==21) // other flavours
            {
                d_eff_udsg->Fill(pt,eta, evtWeight);
                if(csv > csv_btag) n_eff_udsg->Fill(pt,eta, evtWeight);
            }
        }
    }

    /*************************************************************/
    // End of Event loop
    /*************************************************************/
  
    f->cd();
    d_eff_b->Write();
    d_eff_c->Write();
    d_eff_udsg->Write();

    n_eff_b->Write();
    n_eff_c->Write();
    n_eff_udsg->Write();

    f->Close();

    fChain->Reset();

    delete tr;

    return 0;
}

/**************************************************************************/
// End of Main function
// MakeFile example to compile this is at 
// https://github.com/humkies/bTag/blob/master/Makefile#L67
/**************************************************************************/
