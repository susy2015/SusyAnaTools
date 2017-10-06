#include <getopt.h>
#include <vector>
#include <cstdio>

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TLorentzVector.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/samples.h"

#include "DeepTrim.h"

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace std;

    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"condor",           no_argument, 0, 'c'},
        {"dataSets",   required_argument, 0, 'D'},
        {"numFiles",   required_argument, 0, 'N'},
        {"startFile",  required_argument, 0, 'M'},
        {"numEvts",    required_argument, 0, 'E'},
    };

    bool runOnCondor = false;
    string outputFile = "skimmedStopNTuple.root", dataSets = "", sampleloc = AnaSamples::fileDir, plotDir = "top_plots";
    int nFiles = -1, startFile = 0, nEvts = -1;

    char optionCharacters[128] = "";

    for(const auto& option : long_options)
    {
        if(option.has_arg == no_argument) sprintf(optionCharacters, "%s%c", optionCharacters, static_cast<char>(option.val));
        else                              sprintf(optionCharacters, "%s%c:", optionCharacters, static_cast<char>(option.val));
    }

    while((opt = getopt_long(argc, argv, optionCharacters, long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 'c':
            runOnCondor = true;
            break;

        case 'D':
            dataSets = optarg;
            break;

        case 'N':
            nFiles = int(atoi(optarg));
            break;

        case 'M':
            startFile = int(atoi(optarg));
            break;

        case 'E':
            nEvts = int(atoi(optarg));
            break;
        }
    }

    //if running on condor override certain optional settings
    if(runOnCondor)
    {
        char toutputFile[128];
        sprintf(toutputFile, "skimmedStopNTuple_%s_%d.root", dataSets.c_str(), startFile);
        outputFile = toutputFile;
        sampleloc = "condor";
    }

    AnaSamples::SampleSet        ss(sampleloc);
    //AnaSamples::SampleCollection sc(ss);

    auto& sample = ss[dataSets];

    if(sample == ss.null())
    {
        printf("Dataset %s not found, terminating!!!!\n", dataSets.c_str());
        return 0;
    }

    TChain *originalTree = new TChain("stopTreeMaker/AUX");
    sample.addFilesToChain(originalTree, startFile, nFiles);
    originalTree->SetBranchStatus("*", 1);
   
    TFile* output = new TFile(outputFile.c_str(), "RECREATE");
    TDirectory *mydict = output->mkdir("stopTreeMaker");
    mydict->cd();
    TTree* selectedTree = originalTree->CloneTree(0);

    NTupleReader tr(originalTree);

    const std::string spec = "";
    BaselineVessel myBaselineVessel(tr, spec);
    //The passBaseline is registered here
    tr.registerFunction(myBaselineVessel);

    while(tr.getNextEvent())
    {
        if(nEvts > 0 && tr.getEvtNum() > nEvts) break;

        double met = tr.getVar<double>("met");

        //bool passLeptVeto = tr.getVar<bool>("passLeptVeto"+spec);
        bool passnJets = tr.getVar<bool>("passnJets"+spec);
        //bool passMET = tr.getVar<bool>("passMET"+spec);
        bool passHT = tr.getVar<bool>("passHT"+spec);
        bool passMT2 = tr.getVar<bool>("passMT2"+spec);
        bool passTagger = tr.getVar<bool>("passTagger"+spec);
        bool passBJets = tr.getVar<bool>("passBJets"+spec);
        bool passNoiseEventFilter = tr.getVar<bool>("passNoiseEventFilter"+spec);
        bool passQCDHighMETFilter = tr.getVar<bool>("passQCDHighMETFilter"+spec);
        bool passdPhis = tr.getVar<bool>("passdPhis"+spec);

        bool passDeepTrim = false;
        passDeepTrim = ( met > 200 )
            && passnJets
            && passHT
            && passMT2
            && passTagger
            && (!passBJets)
            && passNoiseEventFilter;

        if(passDeepTrim) selectedTree->Fill();
        else continue;
    }

    selectedTree->Write();
    output->Write(); 
    output->Close();

    if(originalTree) delete originalTree;
    if(output) delete output;
    if(selectedTree) delete selectedTree;

    //std::string d = "root://cmseos.fnal.gov//store/group/lpcsusyhad/hua/Skimmed_2015Nov15";
    //std::system(("xrdcp " + output_str + " " + d).c_str());
    //std::system(("rm " + output_str).c_str());

    return 0;
}
