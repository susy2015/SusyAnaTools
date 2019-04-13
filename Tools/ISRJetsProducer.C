#include <iostream>
#include <string>
#include "SusyAnaTools/Tools/SATException.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/samples.h"
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include <cmath>

using namespace std;

const std::string spec = "ISRWeight";
const int nisrJetBins = 9;
const double isrJetBins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 14};

// === Main Function ===================================================
int main(int argc, char* argv[]) {

    TChain *fChain = 0;
    if (argc < 5) {
        std::cerr <<"Please give 4 arguments "<<"SubsampleName"<< " MaxEvent"<< "Startfile" <<" No. of Files to run" <<std::endl;
        return -1;
    }

    const char *subSampleName = argv[1];
    const char *maxevent = argv[2];
    
    int numFiles = -1;
    int startFile = 0;
    
    // Change string arg to int
    int  maxEvent =  std::atoi(maxevent);
    numFiles =  std::atoi(argv[3]);
    startFile =  std::atoi(argv[4]);
    
    const char *sampleSet = argv[5];
    // Prepare file list and finalize it
    TString subSampleNameT = subSampleName;

    const std::string stFile = std::to_string(startFile);
    TString stFileT = stFile;
    TString specT = spec;

    TString FileName = subSampleNameT+"_"+spec+"_"+stFileT;
    TFile *f = new TFile(FileName+".root", "RECREATE");
    TH1D *hISR = new TH1D("NJetsISR_"+subSampleNameT, "NJetsISR", nisrJetBins, isrJetBins );

    AnaSamples::SampleSet ss(sampleSet, (argc == 6), AnaSamples::luminosity);
                                   
    double ScaleMC = 1.;
    if(ss[subSampleName] != ss.null()) {
        fChain = new TChain(ss[subSampleName].treePath.c_str());
        ss[subSampleName].addFilesToChain(fChain, startFile, numFiles);
        ScaleMC = ss[subSampleName].getWeight();
    }
    else
    {
        cout << "SampleSet \"" << subSampleName << "\" not found" << endl;
        return 0;
    } 

    NTupleReader *tr = new NTupleReader(fChain, {"run"});
    // --- Analyse events --------------------------------------------
    std::cout<<"First loop begin: "<<std::endl;
      
    while(tr->getNextEvent()) {
        if(maxEvent>=0 && tr->getEvtNum() > maxEvent) break;
        if(tr->getEvtNum()-1 == 0 || tr->getEvtNum()%10000 == 0) {
            std::cout<<"\n   Processing the "<<tr->getEvtNum()-1<<"th event ..."<<std::endl;
        }
         
        float iniWeight = tr->getVar<float>("genWeight");
        float stored_weight = subSampleNameT.Contains("Data") ? 1 : tr->getVar<float>("genWeight");
        int sign_of_stored_weight = (stored_weight > 0) ? 1 : ((stored_weight < 0) ? -1 : 0);
        double evtWeight = iniWeight >=0 ? iniWeight * sign_of_stored_weight : iniWeight;
	  
        const auto& JetVec = tr->getVec_LVFromNano<Float_t>("Jet");
        const auto& mother = tr->getVec<Int_t>("GenPart_genPartIdxMother");
        const auto& pdgId = tr->getVec<Int_t>("GenPart_pdgId");
        //GenPart_genPartIdxMother : Int_t
        const auto& GenPartVec = tr->getVec_LVFromNano<Float_t>("GenPart");
        const auto& nGenPart = tr->getVar<UInt_t>("nGenPart");
        const auto& StatusFlag = tr->getVec<int>("GenPart_statusFlags");
        
        std::vector<std::vector<Int_t> > daughters(nGenPart,std::vector<Int_t>(0,-1));
        int mother_idx = -1;
        int nisr(0); 

            for(unsigned int i=0; i<nGenPart; i++) {
                mother_idx = mother.at(i);
                //std::cout<<mother_idx<<","<<std::flush;
                if(mother_idx==-1) continue;
                daughters.at(mother_idx).push_back(i);
            }

         for( int j=0; j<JetVec.size(); j++){
             bool matched=false;
             for(unsigned int i=0; i<nGenPart; i++) {
             if (StatusFlag.at(i)!=23 || abs(pdgId.at(i))>5) continue;
             Int_t mommy_pdgId = pdgId.at(mother_idx);
             if(!(mommy_pdgId==6 || mommy_pdgId==23 || mommy_pdgId==24 || mommy_pdgId==25 || mommy_pdgId>1e6)) continue;  
             for(unsigned int d=0; d<daughters.at(i).size(); d++) { 
                    float dR = JetVec[j].DeltaR(GenPartVec[d]);
                    //std::cout << dR << std::endl;
                    if(dR<0.3){
                        matched = true;
                        break;
                    } 
                }
            }
            if(!matched) {
                nisr++;
            }
        }
        //std::cout << std::endl << std::endl;
        //std::cout << "nisr "<<nisr<<std::endl;

        for(unsigned int i=0; i<nGenPart; i++) {
            //std::cout << "(" << std::flush;
            for(unsigned int j=0; j<daughters.at(i).size(); j++) {
                //std::cout << daughters.at(i).at(j) << std::flush;
                if(j!=daughters.at(i).size()-1); //std::cout << "," << std::flush;
            }
            //std::cout << ")," <<std::flush;
        }
        //std::cout << std::endl << std::endl;
        
        hISR->Fill(nisr, evtWeight);
        
    }
    f->cd();
    hISR->Write();
    f->Close();
    
    fChain->Reset();
    return 0;   
}
