#include "NTupleReader.h"
#include "baselineDef.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include "samples.h" 

int main()
{
    char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Fall17_94X_v2_NanAOD_MC/PostProcessed_18Apr2019_v4_fastsimv4/SMS_T1tttt_fastsim_2017/SMS_T1tttt_mGluino1000_mLSP0_fastsim_2017.root";

    AnaSamples::SampleSet ss("/uscms_data/d3/snorberg/Fixing_SusyAnaTools/CMSSW_10_2_9/src/SusyAnaTools/Tools/SMS_T1tttt_2017.cfg");

    TChain *ch = new TChain("Events");

    ss["T1tttt_mGluino1000_mLSP0"].addFilesToChain(ch);

    try
    {
        NTupleReader tr(ch, {"nJet"});

        while(tr.getNextEvent())
        {
            if(tr.getEvtNum() == 1)
            {
                tr.printTupleMembers();
                FILE * fout = fopen("NTupleTypes.txt", "w");
                tr.printTupleMembers(fout);
                fclose(fout);
            }
      
            int njets = tr.getVar<unsigned int>("nJet");

            printf("MET_pt: %10f MET_phi: %10f nJet: %10d Jet_pt: %10d\n",
                    tr.getVar<float>("MET_pt"),
                    tr.getVar<float>("MET_phi"),
                    njets,
                    tr.getVec<float>("Jet_pt").size()
                    );
            
        }
    }
    catch(const SATException& e)
    {
        e.print();
    }

    return 0;
}

