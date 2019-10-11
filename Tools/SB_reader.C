#include "NTupleReader.h"
#include "baselineDef.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include "TH1F.h"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include "customize.h"
#include "ZInvisible/Tools/include/BasicLepton.h"
//#include "ZInvisible/Tools/include/LepInfo.h"
#include "GetVectors.h"
//#include "CleanedJets.h"
#include "RunTopTagger.h"
#include "ZInvisible/Tools/include/Gamma.h"
#include "SB2018.h"
#include <getopt.h>

int main(int argc, char* argv[])
{
    //defaults
    std::string era = "2018";
    //float SF = 1.17; //for TTZToLLNuNu
    float SF = 1.0; //for everything else
    bool verbose = false;
    int max_events = -1;
    bool isData = false;
    bool isTTZ = false;
    bool PeriodF = false;

    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *outputfile    = argv[2];

    int c;
    while (1)
    {
        //int option_index = 3;
        //required_argument: option takes an argument. Still optional to include.
        //no_argument: option does not take an argument
        static struct option long_options[] = {
            {"era",         required_argument,  0,  'e'},
            {"max_events",  required_argument,  0,  'm'},
            {"isData",      no_argument,        0,  'd'},
            {"isTTZ",       no_argument,        0,  'T'},
            {"PeriodF",     no_argument,        0,  'F'},
        };
        c = getopt_long(argc,argv,"e:m:dTF",long_options,NULL);
        if(c==-1) break;
        switch (c)
        {
            case 'e':
                era = optarg;
                break;
            case 'm':
                max_events = std::stoi(optarg);
                break;
            case 'd':
                isData = true;
                break;
            case 'T':
                isTTZ = true;
                break;
            case 'F':
                PeriodF = true;
                break;
        }
    }

    if(isTTZ) SF = 1.17;

    std::cout << "Era: " << era << "\tisTTZ: " << isTTZ << "\tSF: " << SF << "\tisData: " << isData << "\tPeriodF: " << PeriodF << "\tMax events: " << max_events << std::endl;

    TChain *ch = new TChain("Events");

    std::string filename;
    std::ifstream bigfile;

    bigfile.open (inputfilelist);

    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            ch->Add(filename.c_str());
        }
        bigfile.close();
    }

    if(verbose) std::cout << "Setting up" << std::endl;

    NTupleReader tr(ch);
    GetVectors getVectors;
    plotterFunctions::BasicLepton basicLepton;
    plotterFunctions::Gamma gamma(era);
    //CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    BaselineVessel blv(tr,era,"");
    
    if(verbose) std::cout << "Registering getVectors" << std::endl;

    tr.registerFunction(getVectors);
    tr.registerFunction(gamma);
    tr.registerFunction(basicLepton);
    //tr.registerFunction(cleanedJets);
    if(verbose) std::cout << "Registering runTopTagger" << std::endl;
    tr.registerFunction(runTopTagger);
    if(verbose) std::cout << "Registering blv" << std::endl;
    tr.registerFunction(blv);

    auto *h_vb_low = new TH1F("h_vb_low","Low dm Validation Bins",19,0,19);
    auto *h_vb_low_bsf_up = new TH1F("h_vb_low_bsf_up","VB Low B SF Up",19,0,19);
    auto *h_vb_low_bsf_down = new TH1F("h_vb_low_bsf_down","VB Low B SF Down",19,0,19);
    auto *h_vb_low_trig_eff_up = new TH1F("h_vb_low_trig_eff_up","VB Low Trigger Efficiency Up",19,0,19);
    auto *h_vb_low_trig_eff_down = new TH1F("h_vb_low_trig_eff_down","VB Low Trigger Efficiency Down",19,0,19);
    auto *h_vb_low_puWeight_up = new TH1F("h_vb_low_puWeight_up","VB Low PU Weight Up",19,0,19);
    auto *h_vb_low_puWeight_down = new TH1F("h_vb_low_puWeight_down","VB Low PU Weight Down",19,0,19);
    auto *h_vb_low_PFWeight_up = new TH1F("h_vb_low_PFWeight_up","VB Low PF Weight Up",19,0,19);
    auto *h_vb_low_PFWeight_down = new TH1F("h_vb_low_PFWeight_down","VB Low PF Weight Down",19,0,19);
    auto *h_vb_low_pdfWeight_up = new TH1F("h_vb_low_pdfWeight_up","VB Low PDF Weight Up",19,0,19);
    auto *h_vb_low_pdfWeight_down = new TH1F("h_vb_low_pdfWeight_down","VB Low PDF Weight Down",19,0,19);

    auto *h_vb_high = new TH1F("h_vb_high","High dm Validation Bins",24,19,43);
    auto *h_vb_high_bsf_up = new TH1F("h_vb_high_bsf_up","VB High B SF Up",24,19,43);
    auto *h_vb_high_bsf_down = new TH1F("h_vb_high_bsf_down","VB High B SF Down",24,19,43);
    auto *h_vb_high_trig_eff_up = new TH1F("h_vb_high_trig_eff_up","VB High Trigger Efficiency Up",24,19,43);
    auto *h_vb_high_trig_eff_down = new TH1F("h_vb_high_trig_eff_down","VB High Trigger Efficiency Down",24,19,43);
    auto *h_vb_high_puWeight_up = new TH1F("h_vb_high_puWeight_up","VB High PU Weight Up",24,19,43);
    auto *h_vb_high_puWeight_down = new TH1F("h_vb_high_puWeight_down","VB High PU Weight Down",24,19,43);
    auto *h_vb_high_PFWeight_up = new TH1F("h_vb_high_PFWeight_up","VB High PF Weight Up",24,19,43);
    auto *h_vb_high_PFWeight_down = new TH1F("h_vb_high_PFWeight_down","VB High PF Weight Down",24,19,43);
    auto *h_vb_high_pdfWeight_up = new TH1F("h_vb_high_pdfWeight_up","VB High PDF Weight Up",24,19,43);
    auto *h_vb_high_pdfWeight_down = new TH1F("h_vb_high_pdfWeight_down","VB High PDF Weight Down",24,19,43);

    auto *h_sb_low = new TH1F("h_sb_low","Low dm Search Bins",53,0,53);
    auto *h_sb_low_bsf_up = new TH1F("h_sb_low_bsf_up","SB Low B SF Up",53,0,53);
    auto *h_sb_low_bsf_down = new TH1F("h_sb_low_bsf_down","SB Low B SF Down",53,0,53);
    auto *h_sb_low_trig_eff_up = new TH1F("h_sb_low_trig_eff_up","SB Low Trigger Efficiency Up",53,0,53);
    auto *h_sb_low_trig_eff_down = new TH1F("h_sb_low_trig_eff_down","SB Low Trigger Efficiency Down",53,0,53);
    auto *h_sb_low_puWeight_up = new TH1F("h_sb_low_puWeight_up","SB Low PU Weight Up",53,0,53);
    auto *h_sb_low_puWeight_down = new TH1F("h_sb_low_puWeight_down","SB Low PU Weight Down",53,0,53);
    auto *h_sb_low_PFWeight_up = new TH1F("h_sb_low_PFWeight_up","SB Low PF Weight Up",53,0,53);
    auto *h_sb_low_PFWeight_down = new TH1F("h_sb_low_PFWeight_down","SB Low PF Weight Down",53,0,53);
    auto *h_sb_low_pdfWeight_up = new TH1F("h_sb_low_pdfWeight_up","SB Low PDF Weight Up",53,0,53);
    auto *h_sb_low_pdfWeight_down = new TH1F("h_sb_low_pdfWeight_down","SB Low PDF Weight Down",53,0,53);

    auto *h_sb_high = new TH1F("h_sb_high","High dm Search Bins",130,53,183);
    auto *h_sb_high_bsf_up = new TH1F("h_sb_high_bsf_up","SB High B SF Up",130,53,183);
    auto *h_sb_high_bsf_down = new TH1F("h_sb_high_bsf_down","SB High B SF Down",130,53,183);
    auto *h_sb_high_trig_eff_up = new TH1F("h_sb_high_trig_eff_up","SB High Trigger Efficiency Up",130,53,183);
    auto *h_sb_high_trig_eff_down = new TH1F("h_sb_high_trig_eff_down","SB High Trigger Efficiency Down",130,53,183);
    auto *h_sb_high_puWeight_up = new TH1F("h_sb_high_puWeight_up","SB High PU Weight Up",130,53,183);
    auto *h_sb_high_puWeight_down = new TH1F("h_sb_high_puWeight_down","SB High PU Weight Down",130,53,183);
    auto *h_sb_high_PFWeight_up = new TH1F("h_sb_high_PFWeight_up","SB High PF Weight Up",130,53,183);
    auto *h_sb_high_PFWeight_down = new TH1F("h_sb_high_PFWeight_down","SB High PF Weight Down",130,53,183);
    auto *h_sb_high_pdfWeight_up = new TH1F("h_sb_high_pdfWeight_up","SB High PDF Weight Up",130,53,183);
    auto *h_sb_high_pdfWeight_down = new TH1F("h_sb_high_pdfWeight_down","SB High PDF Weight Down",130,53,183);

    auto *h_njets_low_middphi = new TH1F("h_njets_low_middphi","NJets for Low DM mid-dphi region",25,0,25);
    auto *h_njets_high_middphi = new TH1F("h_njets_high_middphi","NJets for High DM mid-dphi region",25,0,25);

    auto *eff_h = new TH1F("eff_h","eff_h",4,0,4); //Counting events. First bin counts all, second event filter, third baselines, fourth counts total to VB.

    if(verbose) std::cout << "Starting while loop." << std::endl;

    while(tr.getNextEvent())
    {

        if(tr.getEvtNum() % 1000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

        if(max_events != -1 && tr.getEvtNum() > max_events) break;

        //Need sign for all events
        float sign = 1.0;

        float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
        if(Stop0l_evtWeight < 0) sign = -1.0; //Just taking the sign from Stop0l_evtWeight

        //Count all events:
        eff_h->Fill(0.,sign);

        bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
        //bool SAT_Pass_EventFilter = tr.getVar<bool>("SAT_Pass_EventFilter");
        //Since every validation bin requires the event filter:
        bool Pass_JetID = tr.getVar<bool>("Pass_JetID");
        bool Pass_CaloMETRatio = tr.getVar<bool>("Pass_CaloMETRatio");
        bool Pass_trigger_MET = true;
        if(isData) Pass_trigger_MET = tr.getVar<bool>("Pass_trigger_MET");
        Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio && Pass_trigger_MET;

        if(!Pass_EventFilter) continue;
        //Count events passing filter:
        eff_h->Fill(1.,sign);

        //Gen Lepton Veto
        if(!isData)
        {
            bool lepton = false;
            auto pdgidvec = tr.getVec<int>("GenPart_pdgId");
            for(int i = 0; i < pdgidvec.size(); i++)
            {
                //e +/- 11, mu +/- 13, tau +/- 15
                if(std::abs(pdgidvec[i]) == 11 || std::abs(pdgidvec[i]) == 13 || std::abs(pdgidvec[i]) == 15)
                {
                    lepton = true;
                    break;
                }
            }
            if(lepton) continue;
        }

        //mtb
        float mtb = tr.getVar<float>("Stop0l_Mtb");

        //njets
        int njetspt20 = tr.getVar<int>("Stop0l_nJets");

        //ntop
        int ntop_merge = tr.getVar<int>("Stop0l_nTop");

        //nw
        int nw = tr.getVar<int>("Stop0l_nW");

        //nres
        int ntop_res = tr.getVar<int>("Stop0l_nResolved");

        //nb
        int nbottompt20 = tr.getVar<int>("Stop0l_nbtags");

        //nSV
        int nSV = tr.getVar<int>("Stop0l_nSoftb");

        //ISRpt
        float ISRpt = tr.getVar<float>("Stop0l_ISRJetPt");

        //bottompt_scalar_sum
        float bottompt_scalar_sum = tr.getVar<float>("Stop0l_Ptb");

        //HT
        float HT = tr.getVar<float>("Stop0l_HT");

        //met
        float met = tr.getVar<float>("MET_pt");

        //S_met
        float S_met = tr.getVar<float>("Stop0l_METSig");

        bool Pass_lowDM_nodPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets20") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
        bool Val_bin_0_14 = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Val_bin_15_18 = Pass_lowDM_nodPhi && tr.getVar<bool>("Pass_dPhiMETMedDM");
        bool Pass_highDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njetspt20 >= 5 && nbottompt20 >= 1);
        bool Val_bin_19_42 = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Search_highdm = Pass_EventFilter && tr.getVar<bool>("Pass_highDM");

        float evtWeight = 1.0;
        float B_SF = 1.0;
        float B_SF_Down = 1.0;
        float B_SF_Up = 1.0;
        float trigger_eff = 1.0;
        float trigger_eff_down = 1.0;
        float trigger_eff_up = 1.0;
        float puWeight = 1.0;
        float puWeightDown = 1.0;
        float puWeightUp = 1.0;
        float PrefireWeight = 1.0;
        float PrefireWeightDown = 1.0;
        float PrefireWeightUp = 1.0;
        float pdfWeight_Up = 1.0;
        float pdfWeight_Down = 1.0;
        
        if(!isData)
        {
            B_SF = tr.getVar<float>("BTagWeight");
            B_SF_Down = tr.getVar<float>("BTagWeight_Down");
            B_SF_Up = tr.getVar<float>("BTagWeight_Up");
            trigger_eff = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline");
            trigger_eff_down = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_down");
            trigger_eff_up = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_up");
            pdfWeight_Up = tr.getVar<float>("pdfWeight_Up");
            pdfWeight_Down = tr.getVar<float>("pdfWeight_Down");
            if(era != "2017")
            {
                puWeight = tr.getVar<float>("puWeight");
                puWeightDown = tr.getVar<float>("puWeight_Down");
                puWeightUp = tr.getVar<float>("puWeight_Up");
            }
            else
            {
                if(!PeriodF)
                {
                    puWeight = tr.getVar<float>("17BtoEpuWeight");
                    puWeightDown = tr.getVar<float>("17BtoEpuWeight_Down");
                    puWeightUp = tr.getVar<float>("17BtoEpuWeight_Up");
                }
                else
                {
                    puWeight = tr.getVar<float>("17FpuWeight");
                    puWeightDown = tr.getVar<float>("17FpuWeight_Down");
                    puWeightUp = tr.getVar<float>("17FpuWeight_Up");
                }
            }
            if(era == "2016" || era == "2017")
            {
                PrefireWeight = tr.getVar<float>("PrefireWeight");
                PrefireWeightDown = tr.getVar<float>("PrefireWeight_Down");
                PrefireWeightUp = tr.getVar<float>("PrefireWeight_Up");
            }

            evtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight;

        }

        if(!Val_bin_0_14 && !Val_bin_15_18 && !Val_bin_19_42 && !Search_lowdm && !Search_highdm) continue;
        eff_h->Fill(2.,sign);

        int bin_num = -1;
        int reset = 0;

        //Bins 0-14: int SB_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Val_bin_0_14)
        {
            bin_num = SB_lowdm_validation(njetspt20,nbottompt20,nSV,ISRpt,bottompt_scalar_sum,met);
            if(bin_num != -1)
            {
                h_vb_low->Fill(bin_num,SF*evtWeight);
                h_vb_low_bsf_up->Fill(bin_num, SF * sign * B_SF_Up * trigger_eff * puWeight * PrefireWeight);
                h_vb_low_bsf_down->Fill(bin_num, SF * sign * B_SF_Down * trigger_eff * puWeight * PrefireWeight);
                h_vb_low_trig_eff_up->Fill(bin_num, SF * sign * B_SF * trigger_eff_up * puWeight * PrefireWeight);
                h_vb_low_trig_eff_down->Fill(bin_num, SF * sign * B_SF * trigger_eff_down * puWeight * PrefireWeight);
                h_vb_low_puWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightUp * PrefireWeight);
                h_vb_low_puWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightDown * PrefireWeight);
                h_vb_low_PFWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightUp);
                h_vb_low_PFWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightDown);
                h_vb_low_pdfWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Up);
                h_vb_low_pdfWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Down);
                eff_h->Fill(3.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_0_14 has bin number -1." << std::endl;
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //Bins 15-18: SB_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met)
        if(Val_bin_15_18)
        {
            bin_num = SB_lowdm_validation_high_MET(nbottompt20,nSV,ISRpt,met);
            if(bin_num != -1)
            {
                h_vb_low->Fill(bin_num,SF*evtWeight);
                h_njets_low_middphi->Fill(njetspt20,SF*evtWeight);
                h_vb_low_bsf_up->Fill(bin_num, SF * sign * B_SF_Up * trigger_eff * puWeight * PrefireWeight);
                h_vb_low_bsf_down->Fill(bin_num, SF * sign * B_SF_Down * trigger_eff * puWeight * PrefireWeight);
                h_vb_low_trig_eff_up->Fill(bin_num, SF * sign * B_SF * trigger_eff_up * puWeight * PrefireWeight);
                h_vb_low_trig_eff_down->Fill(bin_num, SF * sign * B_SF * trigger_eff_down * puWeight * PrefireWeight);
                h_vb_low_puWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightUp * PrefireWeight);
                h_vb_low_puWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightDown * PrefireWeight);
                h_vb_low_PFWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightUp);
                h_vb_low_PFWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightDown);
                h_vb_low_pdfWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Up);
                h_vb_low_pdfWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Down);
                eff_h->Fill(3.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_15_18 has bin number -1." << std::endl;
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //Bins 19-42: SBv3_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met)
        if(Val_bin_19_42)
        {
            bin_num = SBv3_highdm_validation(mtb,njetspt20,ntop_merge,nw,ntop_res,nbottompt20,met);
            if(bin_num != -1)
            {
                h_vb_high->Fill(bin_num,SF*evtWeight);
                h_njets_high_middphi->Fill(njetspt20,SF*evtWeight);
                h_vb_high_bsf_up->Fill(bin_num, SF * sign * B_SF_Up * trigger_eff * puWeight * PrefireWeight);
                h_vb_high_bsf_down->Fill(bin_num, SF * sign * B_SF_Down * trigger_eff * puWeight * PrefireWeight);
                h_vb_high_trig_eff_up->Fill(bin_num, SF * sign * B_SF * trigger_eff_up * puWeight * PrefireWeight);
                h_vb_high_trig_eff_down->Fill(bin_num, SF * sign * B_SF * trigger_eff_down * puWeight * PrefireWeight);
                h_vb_high_puWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightUp * PrefireWeight);
                h_vb_high_puWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightDown * PrefireWeight);
                h_vb_high_PFWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightUp);
                h_vb_high_PFWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightDown);
                h_vb_high_pdfWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Up);
                h_vb_high_pdfWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Down);
                eff_h->Fill(3.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_19_42 has bin number -1." << std::endl;
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //int SB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Search_lowdm)
        {
            bin_num = SB_lowdm(njetspt20,nbottompt20,nSV,ISRpt,bottompt_scalar_sum,met);
            if(bin_num != -1)
            {
                h_sb_low->Fill(bin_num,SF*evtWeight);
                h_sb_low_bsf_up->Fill(bin_num, SF * sign * B_SF_Up * trigger_eff * puWeight * PrefireWeight);
                h_sb_low_bsf_down->Fill(bin_num, SF * sign * B_SF_Down * trigger_eff * puWeight * PrefireWeight);
                h_sb_low_trig_eff_up->Fill(bin_num, SF * sign * B_SF * trigger_eff_up * puWeight * PrefireWeight);
                h_sb_low_trig_eff_down->Fill(bin_num, SF * sign * B_SF * trigger_eff_down * puWeight * PrefireWeight);
                h_sb_low_puWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightUp * PrefireWeight);
                h_sb_low_puWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightDown * PrefireWeight);
                h_sb_low_PFWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightUp);
                h_sb_low_PFWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightDown);
                h_sb_low_pdfWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Up);
                h_sb_low_pdfWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Down);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        bin_num = -1;

        //int SBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Search_highdm)
        {
            bin_num = SBv4_highdm(mtb,njetspt20,nbottompt20,ntop_merge,nw,ntop_res,HT,met);
            if(bin_num != -1)
            {
                h_sb_high->Fill(bin_num,SF*evtWeight);
                h_sb_high_bsf_up->Fill(bin_num, SF * sign * B_SF_Up * trigger_eff * puWeight * PrefireWeight);
                h_sb_high_bsf_down->Fill(bin_num, SF * sign * B_SF_Down * trigger_eff * puWeight * PrefireWeight);
                h_sb_high_trig_eff_up->Fill(bin_num, SF * sign * B_SF * trigger_eff_up * puWeight * PrefireWeight);
                h_sb_high_trig_eff_down->Fill(bin_num, SF * sign * B_SF * trigger_eff_down * puWeight * PrefireWeight);
                h_sb_high_puWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightUp * PrefireWeight);
                h_sb_high_puWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeightDown * PrefireWeight);
                h_sb_high_PFWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightUp);
                h_sb_high_PFWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeightDown);
                h_sb_high_pdfWeight_up->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Up);
                h_sb_high_pdfWeight_down->Fill(bin_num, SF * sign * B_SF * trigger_eff * puWeight * PrefireWeight * pdfWeight_Down);
                eff_h->Fill(3.,sign);
            }
        }

        if(bin_num != -1) reset++;
        if(reset > 1) std::cout << "Error: Bin number reset multiple times in event " << tr.getEvtNum() << "." << std::endl;
    }

    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    eff_h->Write();
    out_file.cd("../");
    h_njets_low_middphi->Write();
    h_njets_high_middphi->Write();
    h_vb_low->Write();
    h_vb_low_bsf_up->Write();
    h_vb_low_bsf_down->Write();
    h_vb_low_trig_eff_up->Write();
    h_vb_low_trig_eff_down->Write();
    h_vb_low_puWeight_up->Write();
    h_vb_low_puWeight_down->Write();
    h_vb_low_PFWeight_up->Write();
    h_vb_low_PFWeight_down->Write();
    h_vb_low_pdfWeight_up->Write();
    h_vb_low_pdfWeight_down->Write();
    h_vb_high->Write();
    h_vb_high_bsf_up->Write();
    h_vb_high_bsf_down->Write();
    h_vb_high_trig_eff_up->Write();
    h_vb_high_trig_eff_down->Write();
    h_vb_high_puWeight_up->Write();
    h_vb_high_puWeight_down->Write();
    h_vb_high_PFWeight_up->Write();
    h_vb_high_PFWeight_down->Write();
    h_vb_high_pdfWeight_up->Write();
    h_vb_high_pdfWeight_down->Write();
    h_sb_low->Write();
    h_sb_low_bsf_up->Write();
    h_sb_low_bsf_down->Write();
    h_sb_low_trig_eff_up->Write();
    h_sb_low_trig_eff_down->Write();
    h_sb_low_puWeight_up->Write();
    h_sb_low_puWeight_down->Write();
    h_sb_low_PFWeight_up->Write();
    h_sb_low_PFWeight_down->Write();
    h_sb_low_pdfWeight_up->Write();
    h_sb_low_pdfWeight_down->Write();
    h_sb_high->Write();
    h_sb_high_bsf_up->Write();
    h_sb_high_bsf_down->Write();
    h_sb_high_trig_eff_up->Write();
    h_sb_high_trig_eff_down->Write();
    h_sb_high_puWeight_up->Write();
    h_sb_high_puWeight_down->Write();
    h_sb_high_PFWeight_up->Write();
    h_sb_high_PFWeight_down->Write();
    h_sb_high_pdfWeight_up->Write();
    h_sb_high_pdfWeight_down->Write();
    out_file.Close();

    return 0;

}

