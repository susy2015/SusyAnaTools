#include "NTupleReader.h"
#include "baselineDef.h"
#include "TopWeightCalculator.h"
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
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include "customize.h"
#include "ZInvisible/Tools/include/BasicLepton.h"
//#include "ZInvisible/Tools/include/LepInfo.h"
//#include "ZInvisible/Tools/include/GetSearchBin.h"
#include "GetVectors.h"
//#include "CleanedJets.h"
#include "RunTopTagger.h"
#include "ZInvisible/Tools/include/Gamma.h"
#include "SB2018.h"
#include <getopt.h>
#include "units.hh"

class GetScaleWeights
{
private:
    NTupleReader trSupp_;

    bool getScaleWeights(NTupleReader& tr)
    {
        const auto& event = tr.getVar<unsigned long long>("event");
        const auto& genmetPt = tr.getVar<float>("GenMET_pt");

        if(trSupp_.checkBranch("event"))
        {
            if(trSupp_.getNextEvent() &&
               event    == trSupp_.getVar<unsigned long long>("event") &&
               genmetPt == trSupp_.getVar<float>("GenMET_pt") )
            {
                tr.registerDerivedVec("LHEScaleWeight", new std::vector(trSupp_.getVec<float>("LHEScaleWeight")));
                return true;
            }
            else
            {
                int startEvt = trSupp_.getEvtNum();
                while(trSupp_.getNextEvent())
                {
                    if(event == trSupp_.getVar<unsigned long long>("event") && genmetPt == trSupp_.getVar<float>("GenMET_pt"))
                    {
                        tr.registerDerivedVec("LHEScaleWeight", new std::vector(trSupp_.getVec<float>("LHEScaleWeight")));
                        return true;
                    }
                }
                trSupp_.goToEvent(0);
                do
                {
                    if(event == trSupp_.getVar<unsigned long long>("event") && genmetPt == trSupp_.getVar<float>("GenMET_pt"))
                    {
                        tr.registerDerivedVec("LHEScaleWeight", new std::vector(trSupp_.getVec<float>("LHEScaleWeight")));
                        return true;
                    }
                } while((trSupp_.getNextEvent()) && (trSupp_.getEvtNum() < (startEvt + 1)));
            }
            std::cout << "ERROR: No matching event found in supplemental event file!!!! " << event << std::endl;
            return false;
        }
        else
        {
            //root will already print an error statement about not being able to open the file
            //std::cout << "ERROR: File missing" << std::endl;
            return false;
        }
    }

public:
    GetScaleWeights(TChain *ch) : trSupp_(ch, {"event"}) {}
    GetScaleWeights(GetScaleWeights&) = delete;
    GetScaleWeights(GetScaleWeights&& gsw) : trSupp_(std::move(gsw.trSupp_)) {}

    bool operator()(NTupleReader& tr) { return getScaleWeights(tr); }
};

int analyze(std::string filename, std::string era, int max_events, bool isData, bool isSignal, bool PeriodF, bool PostHEM, float SF, bool nosplit, bool verbose, bool doLHEScaleWeight)
{
    TChain *ch = new TChain("Events");
    ch->Add(filename.c_str());
    
    
    std::string samplename;
    samplename = filename.substr(filename.find("TTbarDMJets_"),filename.find(".root")-filename.find("TTbarDMJets_"));
    if(samplename.find("/") != std::string::npos) samplename = samplename.substr(samplename.find("/") + 1);
    if(samplename.find("Skim") != std::string::npos) samplename = samplename.substr(0,samplename.find("Skim") - 1);
    std::string longsamplename = samplename;
    if(samplename.find("Mchi_1_Mphi_50") != std::string::npos || samplename.find("Mchi_1_Mphi_100") != std::string::npos) samplename = samplename.substr(0,samplename.length()-6) + era;
    std::cout << samplename << std::endl;

    if(verbose) std::cout << "Setting up" << std::endl;

    NTupleReader tr(ch);
    //LHEScaleWeight supplemental file for fastsim; these samples should have it included
    if(false){
        std::string supplementalFile = filename;
        std::size_t pos = supplementalFile.find("PostProcessed");
        if(pos != std::string::npos) supplementalFile.replace(pos,4,"Scale");
        else std::cout << "Error: Can't find \"PostProcessed\" to replace for the supplemental file" << std::endl;
        std::cout << filename << std::endl;
        std::cout << supplementalFile << std::endl;
        TChain *chSupp = new TChain("Events");
        chSupp->Add(supplementalFile.c_str());
        tr.emplaceModule<GetScaleWeights>(chSupp);
    }
    GetVectors getVectors;
    plotterFunctions::BasicLepton basicLepton;
    plotterFunctions::Gamma gamma(era);
    //CleanedJets cleanedJets;
    RunTopTagger runTopTagger;
    BaselineVessel blv(tr,era,"");
    //BaselineVessel blv_jetpt30(tr,era,"_jetpt30");
    //plotterFunctions::GetSearchBin getsearchbin;
    //TopWeightCalculator twc("TopTaggerCfg-DeepResolved_DeepCSV_GR_nanoAOD_2016_v1.0.6/tTagEff_2016.root", "SMS_T2tt_mStop1000_mLSP0_fastsim_2016", "2016");
    std::string tageffFile;
    if(era == "2016") tageffFile = "TopTaggerCfg-TTbarDMJets/TTagEff_TTbarDMJets_2016.root";
    else if(era == "2017") tageffFile = "TopTaggerCfg-TTbarDMJets/TTagEff_TTbarDMJets_2017.root";
    else if(era == "2018") tageffFile = "TopTaggerCfg-TTbarDMJets/TTagEff_TTbarDMJets_2018.root";
    TopWeightCalculator twc(tageffFile, samplename, era);
    
    if(verbose) std::cout << "Registering getVectors" << std::endl;

    tr.registerFunction(getVectors);
    tr.registerFunction(gamma);
    tr.registerFunction(basicLepton);
    //tr.registerFunction(cleanedJets);
    if(verbose) std::cout << "Registering runTopTagger" << std::endl;
    tr.registerFunction(runTopTagger);
    if(verbose) std::cout << "Registering blv" << std::endl;
    tr.registerFunction(blv);
    //if(verbose) std::cout << "Registering blv_jetpt30" << std::endl;
    //tr.registerFunction(blv_jetpt30);
    //if(verbose) std::cout << "Registering getsearchbin" << std::endl;
    //tr.registerFunction(getsearchbin);
    tr.registerFunction(twc);
    if(verbose) std::cout << "Done registering" << std::endl;

    auto *h_vb = new TH1F("h_vb","Validation Bins",43,0,43);
    h_vb->Sumw2();
    h_vb->SetBinErrorOption(TH1::kPoisson);
    auto *h_vb_bsf_up = new TH1F("h_vb_bsf_up","VB B SF Up",43,0,43);
    auto *h_vb_bsf_down = new TH1F("h_vb_bsf_down","VB B SF Down",43,0,43);
    auto *h_vb_bsf_light_up = new TH1F("h_vb_bsf_light_up","VB B SF Light Up",43,0,43);
    auto *h_vb_bsf_light_down = new TH1F("h_vb_bsf_light_down","VB B SF Light Down",43,0,43);
    auto *h_vb_bsf_heavy_up = new TH1F("h_vb_bsf_heavy_up","VB B SF Heavy Up",43,0,43);
    auto *h_vb_bsf_heavy_down = new TH1F("h_vb_bsf_heavy_down","VB B SF Heavy Down",43,0,43);
    auto *h_vb_trig_eff_up = new TH1F("h_vb_trig_eff_up","VB Trigger Efficiency Up",43,0,43);
    auto *h_vb_trig_eff_down = new TH1F("h_vb_trig_eff_down","VB Trigger Efficiency Down",43,0,43);
    auto *h_vb_puWeight_up = new TH1F("h_vb_puWeight_up","VB PU Weight Up",43,0,43);
    auto *h_vb_puWeight_down = new TH1F("h_vb_puWeight_down","VB PU Weight Down",43,0,43);
    auto *h_vb_PFWeight_up = new TH1F("h_vb_PFWeight_up","VB PF Weight Up",43,0,43);
    auto *h_vb_PFWeight_down = new TH1F("h_vb_PFWeight_down","VB PF Weight Down",43,0,43);
    auto *h_vb_pdfWeight_up = new TH1F("h_vb_pdfWeight_up","VB PDF Weight Up",43,0,43);
    auto *h_vb_pdfWeight_down = new TH1F("h_vb_pdfWeight_down","VB PDF Weight Down",43,0,43);
    auto *h_vb_JES_up = new TH1F("h_vb_JES_up","VB JES Up",43,0,43);
    auto *h_vb_JES_down = new TH1F("h_vb_JES_down","VB JES Down",43,0,43);
    auto *h_vb_METUnClust_up = new TH1F("h_vb_METUnClust_up","VB METUnClust Up",43,0,43);
    auto *h_vb_METUnClust_down = new TH1F("h_vb_METUnClust_down","VB METUnClust Down",43,0,43);
    auto *h_vb_ivfunc_up = new TH1F("h_vb_ivfunc_up","VB Soft B Tag SF Up",43,0,43);
    auto *h_vb_ivfunc_down = new TH1F("h_vb_ivfunc_down","VB Soft B Tag SF Down",43,0,43);
    auto *h_vb_eff_e_up = new TH1F("h_vb_eff_e_up","VB Electron Veto SF Up",43,0,43);
    auto *h_vb_eff_e_down = new TH1F("h_vb_eff_e_down","VB Electron Veto SF Down",43,0,43);
    auto *h_vb_err_mu_up = new TH1F("h_vb_err_mu_up","VB Muon Loose SF Up",43,0,43);
    auto *h_vb_err_mu_down = new TH1F("h_vb_err_mu_down","VB Muon Loose SF Down",43,0,43);
    auto *h_vb_eff_tau_up = new TH1F("h_vb_eff_tau_up","VB Tau POG SF Up",43,0,43);
    auto *h_vb_eff_tau_down = new TH1F("h_vb_eff_tau_down","VB Tau POG SF Down",43,0,43);
    auto *h_vb_eff_restoptag_up = new TH1F("h_vb_eff_restoptag_up","VB Resolved Top Tag SF Up",43,0,43);
    auto *h_vb_eff_restoptag_down = new TH1F("h_vb_eff_restoptag_down","VB Resolved Top Tag SF Down",43,0,43);
    auto *h_vb_eff_toptag_up = new TH1F("h_vb_eff_toptag_up","VB Merged Top Tag SF Up",43,0,43);
    auto *h_vb_eff_toptag_down = new TH1F("h_vb_eff_toptag_down","VB Merged Top Tag SF Down",43,0,43);
    auto *h_vb_eff_densetoptag_up = new TH1F("h_vb_eff_densetoptag_up","VB Dense Top Tag SF Up",43,0,43);
    auto *h_vb_eff_densetoptag_down = new TH1F("h_vb_eff_densetoptag_down","VB Dense Top Tag SF Down",43,0,43);
    auto *h_vb_eff_wtag_up = new TH1F("h_vb_eff_wtag_up","VB W Tag SF Up",43,0,43);
    auto *h_vb_eff_wtag_down = new TH1F("h_vb_eff_wtag_down","VB W TagSF Down",43,0,43);
    auto *h_vb_ak8veto_up = new TH1F("h_vb_ak8veto_up","VB AK8Jet Veto Up",43,0,43);
    auto *h_vb_ak8veto_down = new TH1F("h_vb_ak8veto_down","VB AK8Jet Veto Down",43,0,43);
    auto *h_vb_LHEScaleWeight_0 = new TH1F("h_vb_LHEScaleWeight_0", "VB LHE Scale Weight 0",43,0,43);
    auto *h_vb_LHEScaleWeight_1 = new TH1F("h_vb_LHEScaleWeight_1", "VB LHE Scale Weight 1",43,0,43);
    auto *h_vb_LHEScaleWeight_3 = new TH1F("h_vb_LHEScaleWeight_3", "VB LHE Scale Weight 3",43,0,43);
    auto *h_vb_LHEScaleWeight_4 = new TH1F("h_vb_LHEScaleWeight_4", "VB LHE Scale Weight 4",43,0,43);
    auto *h_vb_LHEScaleWeight_5 = new TH1F("h_vb_LHEScaleWeight_5", "VB LHE Scale Weight 5",43,0,43);
    auto *h_vb_LHEScaleWeight_7 = new TH1F("h_vb_LHEScaleWeight_7", "VB LHE Scale Weight 7",43,0,43);
    auto *h_vb_LHEScaleWeight_8 = new TH1F("h_vb_LHEScaleWeight_8", "VB LHE Scale Weight 8",43,0,43);
    auto *h_vb_LHENoOtherWeights_0 = new TH1F("h_vb_LHENoOtherWeights_0", "VB LHE Scale Weight 0",43,0,43);
    auto *h_vb_LHENoOtherWeights_1 = new TH1F("h_vb_LHENoOtherWeights_1", "VB LHE Scale Weight 1",43,0,43);
    auto *h_vb_LHENoOtherWeights_3 = new TH1F("h_vb_LHENoOtherWeights_3", "VB LHE Scale Weight 3",43,0,43);
    auto *h_vb_LHENoOtherWeights_4 = new TH1F("h_vb_LHENoOtherWeights_4", "VB LHE Scale Weight 4",43,0,43);
    auto *h_vb_LHENoOtherWeights_5 = new TH1F("h_vb_LHENoOtherWeights_5", "VB LHE Scale Weight 5",43,0,43);
    auto *h_vb_LHENoOtherWeights_7 = new TH1F("h_vb_LHENoOtherWeights_7", "VB LHE Scale Weight 7",43,0,43);
    auto *h_vb_LHENoOtherWeights_8 = new TH1F("h_vb_LHENoOtherWeights_8", "VB LHE Scale Weight 8",43,0,43);
    auto *h_vb_elecyield = new TH1F("h_vb_elecyield","VB Yield with >=1 Electron",43,0,43);
    auto *h_vb_muonyield = new TH1F("h_vb_muonyield","VB Yield with >=1 Muon",43,0,43);
    auto *h_vb_tauyield = new TH1F("h_vb_tauyield","VB Yield with >=1 Tau",43,0,43);
    auto *h_vb_elecyield_up = new TH1F("h_vb_elecyield_up","VB Yield Up with >=1 Electron",43,0,43);
    auto *h_vb_muonyield_up = new TH1F("h_vb_muonyield_up","VB Yield Up with >=1 Muon",43,0,43);
    auto *h_vb_tauyield_up = new TH1F("h_vb_tauyield_up","VB Yield Up with >=1 Tau",43,0,43);
    auto *h_vb_elecyield_down = new TH1F("h_vb_elecyield_down","VB Yield Down with >=1 Electron",43,0,43);
    auto *h_vb_muonyield_down = new TH1F("h_vb_muonyield_down","VB Yield Down with >=1 Muon",43,0,43);
    auto *h_vb_tauyield_down = new TH1F("h_vb_tauyield_down","VB Yield Down with >=1 Tau",43,0,43);

    auto *h_sb = new TH1F("h_sb","Search Bins",183,0,183);
    h_sb->Sumw2();
    h_sb->SetBinErrorOption(TH1::kPoisson);
    auto *h_sb_raw = new TH1F("h_sb_raw","Search Bins raw val",183,0,183);
    auto *h_sb_bsf_up = new TH1F("h_sb_bsf_up","SB B SF Up",183,0,183);
    auto *h_sb_bsf_down = new TH1F("h_sb_bsf_down","SB B SF Down",183,0,183);
    auto *h_sb_bsf_light_up = new TH1F("h_sb_bsf_light_up","SB B SF Light Up",183,0,183);
    auto *h_sb_bsf_light_down = new TH1F("h_sb_bsf_light_down","SB B SF Light Down",183,0,183);
    auto *h_sb_bsf_heavy_up = new TH1F("h_sb_bsf_heavy_up","SB B SF Heavy Up",183,0,183);
    auto *h_sb_bsf_heavy_down = new TH1F("h_sb_bsf_heavy_down","SB B SF Heavy Down",183,0,183);
    auto *h_sb_trig_eff_up = new TH1F("h_sb_trig_eff_up","SB Trigger Efficiency Up",183,0,183);
    auto *h_sb_trig_eff_down = new TH1F("h_sb_trig_eff_down","SB Trigger Efficiency Down",183,0,183);
    auto *h_sb_puWeight_up = new TH1F("h_sb_puWeight_up","SB PU Weight Up",183,0,183);
    auto *h_sb_puWeight_down = new TH1F("h_sb_puWeight_down","SB PU Weight Down",183,0,183);
    auto *h_sb_PFWeight_up = new TH1F("h_sb_PFWeight_up","SB PF Weight Up",183,0,183);
    auto *h_sb_PFWeight_down = new TH1F("h_sb_PFWeight_down","SB PF Weight Down",183,0,183);
    auto *h_sb_pdfWeight_up = new TH1F("h_sb_pdfWeight_up","SB PDF Weight Up",183,0,183);
    auto *h_sb_pdfWeight_down = new TH1F("h_sb_pdfWeight_down","SB PDF Weight Down",183,0,183);
    auto *h_sb_JES_up = new TH1F("h_sb_JES_up","SB JES Up",183,0,183);
    auto *h_sb_JES_down = new TH1F("h_sb_JES_down","SB JES Down",183,0,183);
    auto *h_sb_METUnClust_up = new TH1F("h_sb_METUnClust_up","SB METUnClust Up",183,0,183);
    auto *h_sb_METUnClust_down = new TH1F("h_sb_METUnClust_down","SB METUnClust Down",183,0,183);
    auto *h_sb_ivfunc_up = new TH1F("h_sb_ivfunc_up","SB Soft B Tag SF Up",183,0,183);
    auto *h_sb_ivfunc_down = new TH1F("h_sb_ivfunc_down","SB Soft B Tag SF Down",183,0,183);
    auto *h_sb_eff_e_up = new TH1F("h_sb_eff_e_up","SB Electron Veto SF Up",183,0,183);
    auto *h_sb_eff_e_down = new TH1F("h_sb_eff_e_down","SB Electron Veto SF Down",183,0,183);
    auto *h_sb_err_mu_up = new TH1F("h_sb_err_mu_up","SB Muon Loose SF Up",183,0,183);
    auto *h_sb_err_mu_down = new TH1F("h_sb_err_mu_down","SB Muon Loose SF Down",183,0,183);
    auto *h_sb_eff_tau_up = new TH1F("h_sb_eff_tau_up","SB Tau POG SF Up",183,0,183);
    auto *h_sb_eff_tau_down = new TH1F("h_sb_eff_tau_down","SB Tau POG SF Down",183,0,183);
    auto *h_sb_eff_restoptag_up = new TH1F("h_sb_eff_restoptag_up","SB Resolved Top Tag SF Up",183,0,183);
    auto *h_sb_eff_restoptag_down = new TH1F("h_sb_eff_restoptag_down","SB Resolved Top Tag SF Down",183,0,183);
    auto *h_sb_eff_toptag_up = new TH1F("h_sb_eff_toptag_up","SB Merged Top Tag SF Up",183,0,183);
    auto *h_sb_eff_toptag_down = new TH1F("h_sb_eff_toptag_down","SB Merged Top Tag SF Down",183,0,183);
    auto *h_sb_eff_densetoptag_up = new TH1F("h_sb_eff_densetoptag_up","SB Dense Top Tag SF Up",183,0,183);
    auto *h_sb_eff_densetoptag_down = new TH1F("h_sb_eff_densetoptag_down","SB Dense Top Tag SF Down",183,0,183);
    auto *h_sb_eff_wtag_up = new TH1F("h_sb_eff_wtag_up","SB W Tag SF Up",183,0,183);
    auto *h_sb_eff_wtag_down = new TH1F("h_sb_eff_wtag_down","SB W Tag SF Down",183,0,183);
    auto *h_sb_ak8veto_up = new TH1F("h_sb_ak8veto_up","SB AK8Jet Veto Up",183,0,183);
    auto *h_sb_ak8veto_down = new TH1F("h_sb_ak8veto_down","SB AK8Jet Veto Down",183,0,183);
    auto *h_sb_LHEScaleWeight_0 = new TH1F("h_sb_LHEScaleWeight_0", "SB LHE Scale Weight 0",183,0,183);
    auto *h_sb_LHEScaleWeight_1 = new TH1F("h_sb_LHEScaleWeight_1", "SB LHE Scale Weight 1",183,0,183);
    auto *h_sb_LHEScaleWeight_3 = new TH1F("h_sb_LHEScaleWeight_3", "SB LHE Scale Weight 3",183,0,183);
    auto *h_sb_LHEScaleWeight_4 = new TH1F("h_sb_LHEScaleWeight_4", "SB LHE Scale Weight 4",183,0,183);
    auto *h_sb_LHEScaleWeight_5 = new TH1F("h_sb_LHEScaleWeight_5", "SB LHE Scale Weight 5",183,0,183);
    auto *h_sb_LHEScaleWeight_7 = new TH1F("h_sb_LHEScaleWeight_7", "SB LHE Scale Weight 7",183,0,183);
    auto *h_sb_LHEScaleWeight_8 = new TH1F("h_sb_LHEScaleWeight_8", "SB LHE Scale Weight 8",183,0,183);
    auto *h_sb_LHENoOtherWeights_0 = new TH1F("h_sb_LHENoOtherWeights_0", "SB LHE Scale Weight 0",183,0,183);
    auto *h_sb_LHENoOtherWeights_1 = new TH1F("h_sb_LHENoOtherWeights_1", "SB LHE Scale Weight 1",183,0,183);
    auto *h_sb_LHENoOtherWeights_3 = new TH1F("h_sb_LHENoOtherWeights_3", "SB LHE Scale Weight 3",183,0,183);
    auto *h_sb_LHENoOtherWeights_4 = new TH1F("h_sb_LHENoOtherWeights_4", "SB LHE Scale Weight 4",183,0,183);
    auto *h_sb_LHENoOtherWeights_5 = new TH1F("h_sb_LHENoOtherWeights_5", "SB LHE Scale Weight 5",183,0,183);
    auto *h_sb_LHENoOtherWeights_7 = new TH1F("h_sb_LHENoOtherWeights_7", "SB LHE Scale Weight 7",183,0,183);
    auto *h_sb_LHENoOtherWeights_8 = new TH1F("h_sb_LHENoOtherWeights_8", "SB LHE Scale Weight 8",183,0,183);
    auto *h_sb_elecyield = new TH1F("h_sb_elecyield","SB Yield with >=1 Electron",183,0,183);
    auto *h_sb_muonyield = new TH1F("h_sb_muonyield","SB Yield with >=1 Muon",183,0,183);
    auto *h_sb_tauyield = new TH1F("h_sb_tauyield","SB Yield with >=1 Tau",183,0,183);
    auto *h_sb_elecyield_up = new TH1F("h_sb_elecyield_up","SB Yield Up with >=1 Electron",183,0,183);
    auto *h_sb_muonyield_up = new TH1F("h_sb_muonyield_up","SB Yield Up with >=1 Muon",183,0,183);
    auto *h_sb_tauyield_up = new TH1F("h_sb_tauyield_up","SB Yield Up with >=1 Tau",183,0,183);
    auto *h_sb_elecyield_down = new TH1F("h_sb_elecyield_down","SB Yield Down with >=1 Electron",183,0,183);
    auto *h_sb_muonyield_down = new TH1F("h_sb_muonyield_down","SB Yield Down with >=1 Muon",183,0,183);
    auto *h_sb_tauyield_down = new TH1F("h_sb_tauyield_down","SB Yield Down with >=1 Tau",183,0,183);

    auto *h_ub = new TH1F("h_ub","LL CR Unit Bins",112,0,112);
    h_ub->Sumw2();
    h_ub->SetBinErrorOption(TH1::kPoisson);
    auto *h_ub_bsf_up = new TH1F("h_ub_bsf_up","UB B SF Up",112,0,112);
    auto *h_ub_bsf_down = new TH1F("h_ub_bsf_down","UB B SF Down",112,0,112);
    auto *h_ub_bsf_light_up = new TH1F("h_ub_bsf_light_up","UB B SF Light Up",112,0,112);
    auto *h_ub_bsf_light_down = new TH1F("h_ub_bsf_light_down","UB B SF Light Down",112,0,112);
    auto *h_ub_bsf_heavy_up = new TH1F("h_ub_bsf_heavy_up","UB B SF Heavy Up",112,0,112);
    auto *h_ub_bsf_heavy_down = new TH1F("h_ub_bsf_heavy_down","UB B SF Heavy Down",112,0,112);
    auto *h_ub_trig_eff_up = new TH1F("h_ub_trig_eff_up","UB Trigger Efficiency Up",112,0,112);
    auto *h_ub_trig_eff_down = new TH1F("h_ub_trig_eff_down","UB Trigger Efficiency Down",112,0,112);
    auto *h_ub_puWeight_up = new TH1F("h_ub_puWeight_up","UB PU Weight Up",112,0,112);
    auto *h_ub_puWeight_down = new TH1F("h_ub_puWeight_down","UB PU Weight Down",112,0,112);
    auto *h_ub_PFWeight_up = new TH1F("h_ub_PFWeight_up","UB PF Weight Up",112,0,112);
    auto *h_ub_PFWeight_down = new TH1F("h_ub_PFWeight_down","UB PF Weight Down",112,0,112);
    auto *h_ub_pdfWeight_up = new TH1F("h_ub_pdfWeight_up","UB PDF Weight Up",112,0,112);
    auto *h_ub_pdfWeight_down = new TH1F("h_ub_pdfWeight_down","UB PDF Weight Down",112,0,112);
    auto *h_ub_JES_up = new TH1F("h_ub_JES_up","UB JES Up",112,0,112);
    auto *h_ub_JES_down = new TH1F("h_ub_JES_down","UB JES Down",112,0,112);
    auto *h_ub_METUnClust_up = new TH1F("h_ub_METUnClust_up","UB METUnClust Up",112,0,112);
    auto *h_ub_METUnClust_down = new TH1F("h_ub_METUnClust_down","UB METUnClust Down",112,0,112);
    auto *h_ub_ivfunc_up = new TH1F("h_ub_ivfunc_up","UB Soft B Tag SF Up",112,0,112);
    auto *h_ub_ivfunc_down = new TH1F("h_ub_ivfunc_down","UB Soft B Tag SF Down",112,0,112);
    auto *h_ub_eff_e_up = new TH1F("h_ub_eff_e_up","UB Electron Veto SF Up",112,0,112);
    auto *h_ub_eff_e_down = new TH1F("h_ub_eff_e_down","UB Electron Veto SF Down",112,0,112);
    auto *h_ub_err_mu_up = new TH1F("h_ub_err_mu_up","UB Muon Loose SF Up",112,0,112);
    auto *h_ub_err_mu_down = new TH1F("h_ub_err_mu_down","UB Muon Loose SF Down",112,0,112);
    auto *h_ub_eff_tau_up = new TH1F("h_ub_eff_tau_up","UB Tau POG SF Up",112,0,112);
    auto *h_ub_eff_tau_down = new TH1F("h_ub_eff_tau_down","UB Tau POG SF Down",112,0,112);
    auto *h_ub_eff_restoptag_up = new TH1F("h_ub_eff_restoptag_up","UB Resolved Top Tag SF Up",112,0,112);
    auto *h_ub_eff_restoptag_down = new TH1F("h_ub_eff_restoptag_down","UB Resolved Top Tag SF Down",112,0,112);
    auto *h_ub_eff_toptag_up = new TH1F("h_ub_eff_toptag_up","UB Merged Top Tag SF Up",112,0,112);
    auto *h_ub_eff_toptag_down = new TH1F("h_ub_eff_toptag_down","UB Merged Top Tag SF Down",112,0,112);
    auto *h_ub_eff_densetoptag_up = new TH1F("h_ub_eff_densetoptag_up","UB Dense Top Tag SF Up",112,0,112);
    auto *h_ub_eff_densetoptag_down = new TH1F("h_ub_eff_densetoptag_down","UB Dense Top Tag SF Down",112,0,112);
    auto *h_ub_eff_wtag_up = new TH1F("h_ub_eff_wtag_up","UB W Tag SF Up",112,0,112);
    auto *h_ub_eff_wtag_down = new TH1F("h_ub_eff_wtag_down","UB W TagSF Down",112,0,112);
    auto *h_ub_ak8veto_up = new TH1F("h_ub_ak8veto_up","UB AK8Jet Veto Up",112,0,112);
    auto *h_ub_ak8veto_down = new TH1F("h_ub_ak8veto_down","UB AK8Jet Veto Down",112,0,112);
    auto *h_ub_LHEScaleWeight_0 = new TH1F("h_ub_LHEScaleWeight_0", "UB LHE Scale Weight 0",112,0,112);
    auto *h_ub_LHEScaleWeight_1 = new TH1F("h_ub_LHEScaleWeight_1", "UB LHE Scale Weight 1",112,0,112);
    auto *h_ub_LHEScaleWeight_3 = new TH1F("h_ub_LHEScaleWeight_3", "UB LHE Scale Weight 3",112,0,112);
    auto *h_ub_LHEScaleWeight_4 = new TH1F("h_ub_LHEScaleWeight_4", "UB LHE Scale Weight 4",112,0,112);
    auto *h_ub_LHEScaleWeight_5 = new TH1F("h_ub_LHEScaleWeight_5", "UB LHE Scale Weight 5",112,0,112);
    auto *h_ub_LHEScaleWeight_7 = new TH1F("h_ub_LHEScaleWeight_7", "UB LHE Scale Weight 7",112,0,112);
    auto *h_ub_LHEScaleWeight_8 = new TH1F("h_ub_LHEScaleWeight_8", "UB LHE Scale Weight 8",112,0,112);
    auto *h_ub_LHENoOtherWeights_0 = new TH1F("h_ub_LHENoOtherWeights_0", "UB LHE Scale Weight 0",112,0,112);
    auto *h_ub_LHENoOtherWeights_1 = new TH1F("h_ub_LHENoOtherWeights_1", "UB LHE Scale Weight 1",112,0,112);
    auto *h_ub_LHENoOtherWeights_3 = new TH1F("h_ub_LHENoOtherWeights_3", "UB LHE Scale Weight 3",112,0,112);
    auto *h_ub_LHENoOtherWeights_4 = new TH1F("h_ub_LHENoOtherWeights_4", "UB LHE Scale Weight 4",112,0,112);
    auto *h_ub_LHENoOtherWeights_5 = new TH1F("h_ub_LHENoOtherWeights_5", "UB LHE Scale Weight 5",112,0,112);
    auto *h_ub_LHENoOtherWeights_7 = new TH1F("h_ub_LHENoOtherWeights_7", "UB LHE Scale Weight 7",112,0,112);
    auto *h_ub_LHENoOtherWeights_8 = new TH1F("h_ub_LHENoOtherWeights_8", "UB LHE Scale Weight 8",112,0,112);
    auto *h_ub_elecyield = new TH1F("h_ub_elecyield","UB Yield with >=1 Electron",112,0,112);
    auto *h_ub_muonyield = new TH1F("h_ub_muonyield","UB Yield with >=1 Muon",112,0,112);
    auto *h_ub_tauyield = new TH1F("h_ub_tauyield","UB Yield with >=1 Tau",112,0,112);
    auto *h_ub_elecyield_up = new TH1F("h_ub_elecyield_up","UB Yield Up with >=1 Electron",112,0,112);
    auto *h_ub_muonyield_up = new TH1F("h_ub_muonyield_up","UB Yield Up with >=1 Muon",112,0,112);
    auto *h_ub_tauyield_up = new TH1F("h_ub_tauyield_up","UB Yield Up with >=1 Tau",112,0,112);
    auto *h_ub_elecyield_down = new TH1F("h_ub_elecyield_down","UB Yield Down with >=1 Electron",112,0,112);
    auto *h_ub_muonyield_down = new TH1F("h_ub_muonyield_down","UB Yield Down with >=1 Muon",112,0,112);
    auto *h_ub_tauyield_down = new TH1F("h_ub_tauyield_down","UB Yield Down with >=1 Tau",112,0,112);

    auto *h_qb = new TH1F("h_qb","QCD CR Unit Bins",92,0,92);
    h_qb->Sumw2();
    h_qb->SetBinErrorOption(TH1::kPoisson);
    auto *h_qb_bsf_up = new TH1F("h_qb_bsf_up","QCD CR B SF Up",92,0,92);
    auto *h_qb_bsf_down = new TH1F("h_qb_bsf_down","QCD CR B SF Down",92,0,92);
    auto *h_qb_bsf_light_up = new TH1F("h_qb_bsf_light_up","QCD CR B SF Light Up",92,0,92);
    auto *h_qb_bsf_light_down = new TH1F("h_qb_bsf_light_down","QCD CR B SF Light Down",92,0,92);
    auto *h_qb_bsf_heavy_up = new TH1F("h_qb_bsf_heavy_up","QCD CR B SF Heavy Up",92,0,92);
    auto *h_qb_bsf_heavy_down = new TH1F("h_qb_bsf_heavy_down","QCD CR B SF Heavy Down",92,0,92);
    auto *h_qb_trig_eff_up = new TH1F("h_qb_trig_eff_up","QCD CR Trigger Efficiency Up",92,0,92);
    auto *h_qb_trig_eff_down = new TH1F("h_qb_trig_eff_down","QCD CR Trigger Efficiency Down",92,0,92);
    auto *h_qb_puWeight_up = new TH1F("h_qb_puWeight_up","QCD CR PU Weight Up",92,0,92);
    auto *h_qb_puWeight_down = new TH1F("h_qb_puWeight_down","QCD CR PU Weight Down",92,0,92);
    auto *h_qb_PFWeight_up = new TH1F("h_qb_PFWeight_up","QCD CR PF Weight Up",92,0,92);
    auto *h_qb_PFWeight_down = new TH1F("h_qb_PFWeight_down","QCD CR PF Weight Down",92,0,92);
    auto *h_qb_pdfWeight_up = new TH1F("h_qb_pdfWeight_up","QCD CR PDF Weight Up",92,0,92);
    auto *h_qb_pdfWeight_down = new TH1F("h_qb_pdfWeight_down","QCD CR PDF Weight Down",92,0,92);
    auto *h_qb_JES_up = new TH1F("h_qb_JES_up","QCD CR JES Up",92,0,92);
    auto *h_qb_JES_down = new TH1F("h_qb_JES_down","QCD CR JES Down",92,0,92);
    auto *h_qb_METUnClust_up = new TH1F("h_qb_METUnClust_up","QCD CR METUnClust Up",92,0,92);
    auto *h_qb_METUnClust_down = new TH1F("h_qb_METUnClust_down","QCD CR METUnClust Down",92,0,92);
    auto *h_qb_ivfunc_up = new TH1F("h_qb_ivfunc_up","QCD CR Soft B Tag SF Up",92,0,92);
    auto *h_qb_ivfunc_down = new TH1F("h_qb_ivfunc_down","QCD CR Soft B Tag SF Down",92,0,92);
    auto *h_qb_eff_e_up = new TH1F("h_qb_eff_e_up","QCD CR Electron Veto SF Up",92,0,92);
    auto *h_qb_eff_e_down = new TH1F("h_qb_eff_e_down","QCD CR Electron Veto SF Down",92,0,92);
    auto *h_qb_err_mu_up = new TH1F("h_qb_err_mu_up","QCD CR Muon Loose SF Up",92,0,92);
    auto *h_qb_err_mu_down = new TH1F("h_qb_err_mu_down","QCD CR Muon Loose SF Down",92,0,92);
    auto *h_qb_eff_tau_up = new TH1F("h_qb_eff_tau_up","QCD CR Tau POG SF Up",92,0,92);
    auto *h_qb_eff_tau_down = new TH1F("h_qb_eff_tau_down","QCD CR Tau POG SF Down",92,0,92);
    auto *h_qb_eff_restoptag_up = new TH1F("h_qb_eff_restoptag_up","QCD CR Resolved Top Tag SF Up",92,0,92);
    auto *h_qb_eff_restoptag_down = new TH1F("h_qb_eff_restoptag_down","QCD CR Resolved Top Tag SF Down",92,0,92);
    auto *h_qb_eff_toptag_up = new TH1F("h_qb_eff_toptag_up","QCD CR Merged Top Tag SF Up",92,0,92);
    auto *h_qb_eff_toptag_down = new TH1F("h_qb_eff_toptag_down","QCD CR Merged Top Tag SF Down",92,0,92);
    auto *h_qb_eff_densetoptag_up = new TH1F("h_qb_eff_densetoptag_up","QCD CR Dense Top Tag SF Up",92,0,92);
    auto *h_qb_eff_densetoptag_down = new TH1F("h_qb_eff_densetoptag_down","QCD CR Dense Top Tag SF Down",92,0,92);
    auto *h_qb_eff_wtag_up = new TH1F("h_qb_eff_wtag_up","QCD CR W Tag SF Up",92,0,92);
    auto *h_qb_eff_wtag_down = new TH1F("h_qb_eff_wtag_down","QCD CR W TagSF Down",92,0,92);
    auto *h_qb_ak8veto_up = new TH1F("h_qb_ak8veto_up","QCD CR AK8Jet Veto Up",92,0,92);
    auto *h_qb_ak8veto_down = new TH1F("h_qb_ak8veto_down","QCD CR AK8Jet Veto Down",92,0,92);
    auto *h_qb_LHEScaleWeight_0 = new TH1F("h_qb_LHEScaleWeight_0", "QB LHE Scale Weight 0",92,0,92);
    auto *h_qb_LHEScaleWeight_1 = new TH1F("h_qb_LHEScaleWeight_1", "QB LHE Scale Weight 1",92,0,92);
    auto *h_qb_LHEScaleWeight_3 = new TH1F("h_qb_LHEScaleWeight_3", "QB LHE Scale Weight 3",92,0,92);
    auto *h_qb_LHEScaleWeight_4 = new TH1F("h_qb_LHEScaleWeight_4", "QB LHE Scale Weight 4",92,0,92);
    auto *h_qb_LHEScaleWeight_5 = new TH1F("h_qb_LHEScaleWeight_5", "QB LHE Scale Weight 5",92,0,92);
    auto *h_qb_LHEScaleWeight_7 = new TH1F("h_qb_LHEScaleWeight_7", "QB LHE Scale Weight 7",92,0,92);
    auto *h_qb_LHEScaleWeight_8 = new TH1F("h_qb_LHEScaleWeight_8", "QB LHE Scale Weight 8",92,0,92);
    auto *h_qb_LHENoOtherWeights_0 = new TH1F("h_qb_LHENoOtherWeights_0", "QB LHE Scale Weight 0",92,0,92);
    auto *h_qb_LHENoOtherWeights_1 = new TH1F("h_qb_LHENoOtherWeights_1", "QB LHE Scale Weight 1",92,0,92);
    auto *h_qb_LHENoOtherWeights_3 = new TH1F("h_qb_LHENoOtherWeights_3", "QB LHE Scale Weight 3",92,0,92);
    auto *h_qb_LHENoOtherWeights_4 = new TH1F("h_qb_LHENoOtherWeights_4", "QB LHE Scale Weight 4",92,0,92);
    auto *h_qb_LHENoOtherWeights_5 = new TH1F("h_qb_LHENoOtherWeights_5", "QB LHE Scale Weight 5",92,0,92);
    auto *h_qb_LHENoOtherWeights_7 = new TH1F("h_qb_LHENoOtherWeights_7", "QB LHE Scale Weight 7",92,0,92);
    auto *h_qb_LHENoOtherWeights_8 = new TH1F("h_qb_LHENoOtherWeights_8", "QB LHE Scale Weight 8",92,0,92);
    auto *h_qb_elecyield = new TH1F("h_qb_elecyield","QB Yield with >=1 Electron",92,0,92);
    auto *h_qb_muonyield = new TH1F("h_qb_muonyield","QB Yield with >=1 Muon",92,0,92);
    auto *h_qb_tauyield = new TH1F("h_qb_tauyield","QB Yield with >=1 Tau",92,0,92);
    auto *h_qb_elecyield_up = new TH1F("h_qb_elecyield_up","QB Yield Up with >=1 Electron",92,0,92);
    auto *h_qb_muonyield_up = new TH1F("h_qb_muonyield_up","QB Yield Up with >=1 Muon",92,0,92);
    auto *h_qb_tauyield_up = new TH1F("h_qb_tauyield_up","QB Yield Up with >=1 Tau",92,0,92);
    auto *h_qb_elecyield_down = new TH1F("h_qb_elecyield_down","QB Yield Down with >=1 Electron",92,0,92);
    auto *h_qb_muonyield_down = new TH1F("h_qb_muonyield_down","QB Yield Down with >=1 Muon",92,0,92);
    auto *h_qb_tauyield_down = new TH1F("h_qb_tauyield_down","QB Yield Down with >=1 Tau",92,0,92);

    if(verbose) std::cout << "bin histograms declared" << std::endl;

    //Vars will NOT be nSRUnitLowDM and nSRUnitHighDM
    //Low DM range: 0, 52 -> 53,0,53
    //High DM range: 53, 528 -> 476,53,529
    //Vars will be nCRUnitLowDM and nCRUnitHighDM
    //Range: 0,111 -> 112,0,112

    auto *h_njets_low_middphi = new TH1F("h_njets_low_middphi","NJets for Low DM mid-dphi region",25,0,25);
    auto *h_njets_high_middphi = new TH1F("h_njets_high_middphi","NJets for High DM mid-dphi region",25,0,25);
    auto *MET = new TH1F("MET","MET",40,0,1000);
    auto *BTagWeight = new TH1F("BTagWeight","BTagWeight",50,0,5);
    auto *PUWeight = new TH1F("PUWeight","PU Weight",100,0,2);

    auto *eff_h = new TH1F("eff_h","eff_h",4,0,4); //Counting events. First bin counts all, second event filter, third baselines, fourth counts total to VB.
    auto *h_weights = new TH1F("h_weights","h_weights",9,0,9); //raw weights (no cuts, all events, sum of weights to see which has largest impact)
    float numerator_genfiltereff = 0;
    float denominator_genfiltereff = 0;
    float SigWTab_SigWeightGenCutPass = 0;
    float SigWTab_SigWeightGenCut = 0;

    float nosplit_lumi = 1.0;
    if(nosplit)
    {
        if(era == "2016") nosplit_lumi = 35.815165;
        if(era == "2017") nosplit_lumi = 41.208034;
        if(era == "2018") nosplit_lumi = 21.068576; //PreHEM. Use full if pass HEM veto. (59.699489)
    }
    float nosplit_lumi_JESUp = nosplit_lumi; //split out as PostHEM veto has up/down versions
    float nosplit_lumi_JESDown = nosplit_lumi;
    float nosplit_lumi_METUnClustUp = nosplit_lumi;
    float nosplit_lumi_METUnClustDown = nosplit_lumi;


    if(verbose) std::cout << "Starting while loop." << std::endl;

    while(tr.getNextEvent())
    {
        if(verbose) std::cout << "Inside while loop" << std::endl;
        if(tr.getEvtNum() == 1)
        {
            std::cout << "Outputting tuplemembers to SBR_NTupleTypes.txt" << std::endl;
            if(verbose) tr.printTupleMembers();
            FILE *fout = fopen("SBR_NTupleTypes.txt","w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }

        if(tr.getEvtNum() % 1000 == 0) std::cout << "Event Number " << tr.getEvtNum() << std::endl;

        if(max_events != -1 && tr.getEvtNum() > max_events) break;

        //Need sign for all events
        float sign = 1.0;

        float Stop0l_evtWeight = tr.getVar<float>("Stop0l_evtWeight");
        if(Stop0l_evtWeight < 0) sign = -1.0; //Just taking the sign from Stop0l_evtWeight

        //Count all events:
        eff_h->Fill(0.,sign);

        //gen filter efficiency
        if(tr.checkBranch("SigWTab_SigWeightGenCut") && tr.checkBranch("SigWTab_SigWeightGenCutPass"))
        {
            SigWTab_SigWeightGenCutPass = tr.getVar<float>("SigWTab_SigWeightGenCutPass");
            SigWTab_SigWeightGenCut = tr.getVar<float>("SigWTab_SigWeightGenCut");
        }
        numerator_genfiltereff += SigWTab_SigWeightGenCutPass;
        denominator_genfiltereff += SigWTab_SigWeightGenCut;

        bool Pass_EventFilter = tr.getVar<bool>("Pass_EventFilter");
        //bool SAT_Pass_EventFilter = tr.getVar<bool>("SAT_Pass_EventFilter");
        //Since every validation bin requires the event filter:
        bool Pass_JetID = tr.getVar<bool>("Pass_JetID");
        bool Pass_CaloMETRatio = tr.getVar<bool>("Pass_CaloMETRatio");
        bool Pass_trigger_MET = true;
        if(isData) Pass_trigger_MET = tr.getVar<bool>("Pass_trigger_MET");
        bool Pass_HEMVeto30 = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30 = tr.getVar<bool>("Pass_exHEMVeto30");
        Pass_EventFilter = Pass_EventFilter && Pass_JetID && Pass_CaloMETRatio && Pass_trigger_MET;
        if (!nosplit) Pass_EventFilter = Pass_EventFilter && Pass_HEMVeto30;

        bool Pass_EventFilter_JESUp = tr.getVar<bool>("Pass_EventFilter_JESUp");
        bool Pass_JetID_JESUp = tr.getVar<bool>("Pass_JetID_JESUp");
        bool Pass_CaloMETRatio_JESUp = tr.getVar<bool>("Pass_CaloMETRatio_JESUp");
        bool Pass_HEMVeto30_JESUp = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_JESUp = tr.getVar<bool>("Pass_exHEMVeto30_JESUp");
        Pass_EventFilter_JESUp = Pass_EventFilter_JESUp && Pass_JetID_JESUp && Pass_CaloMETRatio_JESUp && Pass_trigger_MET;
        if (!nosplit) Pass_EventFilter_JESUp = Pass_EventFilter_JESUp && Pass_HEMVeto30_JESUp;
        
        bool Pass_EventFilter_JESDown = tr.getVar<bool>("Pass_EventFilter_JESDown");
        bool Pass_JetID_JESDown = tr.getVar<bool>("Pass_JetID_JESDown");
        bool Pass_CaloMETRatio_JESDown = tr.getVar<bool>("Pass_CaloMETRatio_JESDown");
        bool Pass_HEMVeto30_JESDown = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_JESDown = tr.getVar<bool>("Pass_exHEMVeto30_JESDown");
        Pass_EventFilter_JESDown = Pass_EventFilter_JESDown && Pass_JetID_JESDown && Pass_CaloMETRatio_JESDown && Pass_trigger_MET;
        if (!nosplit) Pass_EventFilter_JESDown = Pass_EventFilter_JESDown && Pass_HEMVeto30_JESDown;

        bool Pass_EventFilter_METUnClustUp = tr.getVar<bool>("Pass_EventFilter_METUnClustUp");
        bool Pass_JetID_METUnClustUp = tr.getVar<bool>("Pass_JetID_METUnClustUp");
        bool Pass_CaloMETRatio_METUnClustUp = tr.getVar<bool>("Pass_CaloMETRatio_METUnClustUp");
        bool Pass_HEMVeto30_METUnClustUp = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_METUnClustUp = tr.getVar<bool>("Pass_exHEMVeto30_METUnClustUp");
        Pass_EventFilter_METUnClustUp = Pass_EventFilter_METUnClustUp && Pass_JetID_METUnClustUp && Pass_CaloMETRatio_METUnClustUp && Pass_trigger_MET;
        if(!nosplit) Pass_EventFilter_METUnClustUp = Pass_EventFilter_METUnClustUp && Pass_HEMVeto30_METUnClustUp;
        
        bool Pass_EventFilter_METUnClustDown = tr.getVar<bool>("Pass_EventFilter_METUnClustDown");
        bool Pass_JetID_METUnClustDown = tr.getVar<bool>("Pass_JetID_METUnClustDown");
        bool Pass_CaloMETRatio_METUnClustDown = tr.getVar<bool>("Pass_CaloMETRatio_METUnClustDown");
        bool Pass_HEMVeto30_METUnClustDown = true;
        if(PostHEM && era == "2018") Pass_HEMVeto30_METUnClustDown = tr.getVar<bool>("Pass_exHEMVeto30_METUnClustDown");
        Pass_EventFilter_METUnClustDown = Pass_EventFilter_METUnClustDown && Pass_JetID_METUnClustDown && Pass_CaloMETRatio_METUnClustDown && Pass_trigger_MET;
        if(!nosplit) Pass_EventFilter_METUnClustDown = Pass_EventFilter_METUnClustDown && Pass_HEMVeto30_METUnClustDown;

        //met
        float met = tr.getVar<float>("MET_pt");
        float met_JESUp = tr.getVar<float>("MET_pt_jesTotalUp");
        float met_JESDown = tr.getVar<float>("MET_pt_jesTotalDown");
        float met_METUnClustUp = tr.getVar<float>("MET_pt_unclustEnUp");
        float met_METUnClustDown = tr.getVar<float>("MET_pt_unclustEnDown");

        bool Pass_Baseline = tr.getVar<bool>("Pass_Baseline");
        bool Pass_Baseline_JESUp = tr.getVar<bool>("Pass_Baseline_JESUp");
        bool Pass_Baseline_JESDown = tr.getVar<bool>("Pass_Baseline_JESDown");
        bool Pass_Baseline_METUnClustUp = tr.getVar<bool>("Pass_Baseline_METUnClustUp");
        bool Pass_Baseline_METUnClustDown = tr.getVar<bool>("Pass_Baseline_METUnClustDown");
        //PassBaseline    = PassEventFilter and PassJetID and PassLeptonVeto and PassNjets and PassMET and PassHT and PassdPhiLowDM
        bool Pass_Baseline_nolep = Pass_EventFilter && tr.getVar<bool>("Pass_NJets30") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && tr.getVar<bool>("Pass_dPhiMETLowDM");

        //LLCR METUnc, QCDCR METUnc are defined near filling
        bool Pass_LLCR = tr.getVar<bool>("Pass_LLCR");
        bool Pass_LLCR_JESUp = tr.getVar<bool>("Pass_LLCR_JESUp");
        bool Pass_LLCR_JESDown = tr.getVar<bool>("Pass_LLCR_JESDown");
        bool Pass_LLCR_METUnClustUp = tr.getVar<bool>("Pass_LLCR_METUnClustUp");
        bool Pass_LLCR_METUnClustDown = tr.getVar<bool>("Pass_LLCR_METUnClustDown");
        bool Pass_QCDCR = tr.getVar<bool>("Pass_QCDCR");
        bool Pass_QCDCR_JESUp = tr.getVar<bool>("Pass_QCDCR_JESUp");
        bool Pass_QCDCR_JESDown = tr.getVar<bool>("Pass_QCDCR_JESDown");
        bool Pass_QCDCR_METUnClustUp = tr.getVar<bool>("Pass_QCDCR_METUnClustUp");
        bool Pass_QCDCR_METUnClustDown = tr.getVar<bool>("Pass_QCDCR_METUnClustDown");
        bool Pass_dPhiMETHighDM = tr.getVar<bool>("Pass_dPhiMETHighDM"); //used in Pass_LLCR_highdm
        bool Pass_dPhiMETHighDM_JESUp = tr.getVar<bool>("Pass_dPhiMETHighDM_JESUp");
        bool Pass_dPhiMETHighDM_JESDown = tr.getVar<bool>("Pass_dPhiMETHighDM_JESDown");
        bool Pass_dPhiMETHighDM_METUnClustUp = tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustUp");
        bool Pass_dPhiMETHighDM_METUnClustDown = tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustDown");

        //Define LowDM and HighDM later (after 'fixed' restop count)

        //if(!Pass_EventFilter) continue;
        //Count events passing filter:
        //eff_h->Fill(1.,sign);

        //Gen Lepton Veto; only TTZ/Rare
        if(false)
        {
            if(verbose && (tr.getEvtNum() == 1)) std::cout << "Gen Lepton Veto" << std::endl;
            bool lepton = false;
            auto pdgidvec = tr.getVec<int>("GenPart_pdgId");
            auto statusvec = tr.getVec<int>("GenPart_statusFlags");
            for(int i = 0; i < pdgidvec.size(); i++)
            {
                //e +/- 11, mu +/- 13, tau +/- 15
                if(std::abs(pdgidvec[i]) == 11 || std::abs(pdgidvec[i]) == 13 || std::abs(pdgidvec[i]) == 15)
                {
                    if(((statusvec[i]&0x2100)==0x2100) || ((statusvec[i]&0x2080)==0x2080))
                    {
                        lepton = true;
                        break;
                    }
                }
            }
            if(lepton) continue;
        }

        //Soft B SF
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Soft B SF" << std::endl;
        if(verbose) std::cout << "Soft B SF" << std::endl;
        auto vec_SB_SF = tr.getVec<float>("SB_SF");
        auto vec_SB_SFerr = tr.getVec<float>("SB_SFerr");
        float SB_SF = 1.0;
        float SB_SFerr = 0.0;
        auto SB_Stop0l = tr.getVec<unsigned char>("SB_Stop0l");
        //if(verbose && tr.getEvtNum() < 1000) std::cout << "SB_Stop0l size: " << SB_Stop0l.size() <<"\tSB_SF size: " << vec_SB_SF.size() << std::endl;
        for(int v = 0; v < vec_SB_SF.size(); v++)
        {
            if(SB_Stop0l[v])
            {
                SB_SF *= vec_SB_SF[v];
                SB_SFerr += vec_SB_SFerr[v]*vec_SB_SFerr[v];
            }
        }
        float SB_SF_up = SB_SF + std::sqrt(SB_SFerr);
        float SB_SF_down = SB_SF - std::sqrt(SB_SFerr);

        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "SB_SF: " << SB_SF << "\tSB_SFerr: " << SB_SFerr << std::endl;


        //Let's try again. Electron Veto SF.
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Electron Veto SF" << std::endl;
        if(verbose) std::cout << "Electron Veto SF" << std::endl;
        auto vec_Electron_VetoSF = tr.getVec<float>("Electron_VetoSF");
        auto vec_Electron_VetoSFErr = tr.getVec<float>("Electron_VetoSFErr");
        float Electron_VetoSF = 1.0;
        float Electron_VetoSFErr = 0.0;
        int pickedEleIdx = -1;
        auto e_pt = tr.getVec<float>("Electron_pt");
        auto e_eta = tr.getVec<float>("Electron_eta");
        auto isElectron = tr.getVec<unsigned char>("Electron_Stop0l");
        if((vec_Electron_VetoSF.size() != e_pt.size()) || (e_pt.size() != isElectron.size())) std::cout << "Size mismatch: " << vec_Electron_VetoSF.size() << e_pt.size() << isElectron.size() << std::endl;
        for(int ie = 0; ie < e_pt.size(); ie++)
        {
            if(std::abs(e_eta[ie]) < 2.5 && e_pt[ie] > 5 && isElectron[ie])
            {
                if(pickedEleIdx == -1) pickedEleIdx = ie; //Separated to avoid asking for e_pt[-1]
                else if(e_pt[pickedEleIdx] > e_pt[ie]) pickedEleIdx = ie;
            }
        }
        if(pickedEleIdx != -1)
        {
            Electron_VetoSF = vec_Electron_VetoSF[pickedEleIdx];
            Electron_VetoSFErr = vec_Electron_VetoSFErr[pickedEleIdx];
        }
        float Electron_VetoSF_up = Electron_VetoSF + Electron_VetoSFErr;
        float Electron_VetoSF_down = Electron_VetoSF - Electron_VetoSFErr;
        //Count of events passing baseline except for >=1 electron:

        int pickedMuIdx = -1;
        auto vec_Muon_MediumSF = tr.getVec<float>("Muon_MediumSF");
        auto vec_Muon_MediumSFErr = tr.getVec<float>("Muon_MediumSFErr");
        float Muon_VetoSF = 1.0;
        float Muon_VetoSFErr = 0.0;
        auto mu_pt = tr.getVec<float>("Muon_pt");
        auto mu_eta = tr.getVec<float>("Muon_eta");
        auto isMuon = tr.getVec<unsigned char>("Muon_Stop0l");
        for(int im = 0; im < vec_Muon_MediumSF.size(); im++)
        {
            if(std::abs(mu_eta[im]) < 2.4 && mu_pt[im] > 5 && isMuon[im])
            {
                if(pickedMuIdx == -1) pickedMuIdx = im; //Separated to avoid asking for e_pt[-1]
                else if(mu_pt[pickedMuIdx] > mu_pt[im]) pickedMuIdx = im;
            }
        }
        if(pickedMuIdx != -1)
        {
            Muon_VetoSF = vec_Muon_MediumSF[pickedMuIdx];
            Muon_VetoSFErr = vec_Muon_MediumSFErr[pickedMuIdx];
        }
        float Muon_VetoSF_up = Muon_VetoSF + Muon_VetoSFErr;
        float Muon_VetoSF_down = Muon_VetoSF - Muon_VetoSFErr;

        //Pass_LLLep
        int totlep = 0; //just electrons and muons
        float totMtW = 0;
        auto vec_Electron_MtW = tr.getVec<float>("Electron_MtW");
        auto vec_Muon_MtW = tr.getVec<float>("Muon_MtW");
        for(int ie = 0; ie < e_pt.size(); ie++)
        {
            if(std::abs(e_eta[ie]) < 2.5 && e_pt[ie] > 5 && isElectron[ie])
            {
                totlep++;
                totMtW += vec_Electron_MtW[ie];
            }
        }
        for(int im = 0; im < vec_Muon_MediumSF.size(); im++)
        {
            if(std::abs(mu_eta[im]) < 2.4 && mu_pt[im] > 5 && isMuon[im])
            {
                totlep++;
                totMtW += vec_Muon_MtW[im];
            }
        }

        int pickedTauIdx = -1;
        auto vec_Tau_MediumSF = tr.getVec<float>("Tau_MediumSF");
        auto vec_Tau_MediumSF_Up = tr.getVec<float>("Tau_MediumSF_Up");
        auto vec_Tau_MediumSF_Down = tr.getVec<float>("Tau_MediumSF_Down");
        float Tau_VetoSF = 1.0;
        float Tau_VetoSF_up = 1.0;
        float Tau_VetoSF_down = 1.0;
        auto tau_pt = tr.getVec<float>("Tau_pt");
        auto tau_eta = tr.getVec<float>("Tau_eta");
        auto isTau = tr.getVec<unsigned char>("Tau_Stop0l");
        for(int it = 0; it < vec_Tau_MediumSF.size(); it++)
        {
            if(std::abs(tau_eta[it]) < 2.4 && tau_pt[it] > 20 && isTau[it])
            {
                if(pickedTauIdx == -1) pickedTauIdx = it;
                else if(tau_pt[pickedTauIdx] > tau_pt[it]) pickedTauIdx = it;
            }
        }
        if(pickedTauIdx != -1)
        {
            Tau_VetoSF = vec_Tau_MediumSF[pickedTauIdx];
            Tau_VetoSF_up = vec_Tau_MediumSF_Up[pickedTauIdx];
            Tau_VetoSF_down = vec_Tau_MediumSF_Down[pickedTauIdx];
        }

        //Electron SF
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Electron SF" << std::endl;
        if(verbose) std::cout << "Electron SF" << std::endl;
        //auto vec_Electron_VetoSF = tr.getVec<float>("Electron_VetoSF");
        //auto vec_Electron_VetoSFErr = tr.getVec<float>("Electron_VetoSFErr");
        float Electron_SF = 1.0;
        float Electron_SFErr = 0.0;
        //auto e_pt = tr.getVec<float>("Electron_pt");
        //auto e_eta = tr.getVec<float>("Electron_eta");
        for(int v = 0; v < vec_Electron_VetoSF.size(); v++)
        {
            if(std::abs(e_eta[v]) < 2.5 && e_pt[v] > 5 && isElectron[v])
            {
                Electron_SF *= vec_Electron_VetoSF[v];
                Electron_SFErr += vec_Electron_VetoSFErr[v]*vec_Electron_VetoSFErr[v];
            }
        }
        float Electron_SF_up = Electron_SF + std::sqrt(Electron_SFErr);
        float Electron_SF_down = Electron_SF - std::sqrt(Electron_SFErr);
        
        
        //Muon SF
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Muon SF" << std::endl;
        if(verbose) std::cout << "Muon SF" << std::endl;
        //auto vec_Muon_MediumSF = tr.getVec<float>("Muon_MediumSF");
        //auto vec_Muon_MediumSFErr = tr.getVec<float>("Muon_MediumSFErr");
        float Muon_MediumSF = 1.0;
        float Muon_MediumSFErr = 0.0;
        //auto mu_pt = tr.getVec<float>("Muon_pt");
        //auto mu_eta = tr.getVec<float>("Muon_eta");
        for(int v = 0; v < vec_Muon_MediumSF.size(); v++)
        {
            if(std::abs(mu_eta[v]) < 2.4 && mu_pt[v] > 5 && isMuon[v])
            {
                Muon_MediumSF *= vec_Muon_MediumSF[v];
                Muon_MediumSFErr += vec_Muon_MediumSFErr[v]*vec_Muon_MediumSFErr[v];
            }
        }
        float Muon_MediumSF_up = Muon_MediumSF + std::sqrt(Muon_MediumSFErr);
        float Muon_MediumSF_down = Muon_MediumSF - std::sqrt(Muon_MediumSFErr);

        //Tau SF
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Tau SF" << std::endl;
        if(verbose) std::cout << "Tau SF" << std::endl;
        //auto vec_Tau_MediumSF = tr.getVec<float>("Tau_MediumSF");
        //auto vec_Tau_MediumSF_Up = tr.getVec<float>("Tau_MediumSF_Up");
        //auto vec_Tau_MediumSF_Down = tr.getVec<float>("Tau_MediumSF_Down");
        float Tau_MediumSF = 1.0;
        float Tau_MediumSF_Up = 1.0;
        float Tau_MediumSF_Down = 1.0;
        //auto tau_pt = tr.getVec<float>("Tau_pt");
        //auto tau_eta = tr.getVec<float>("Tau_eta");
        for(int v = 0; v < vec_Tau_MediumSF.size(); v++)
        {
            if(std::abs(tau_eta[v]) < 2.4 && tau_pt[v] > 20 && isTau[v])
            {
                Tau_MediumSF *= vec_Tau_MediumSF[v];
                Tau_MediumSF_Up *= vec_Tau_MediumSF_Up[v];
                Tau_MediumSF_Down *= vec_Tau_MediumSF_Down[v];
            }
        }
        

        //Res top tag SF
        //Up and down values are already set to be summed in quadrature. Need to make total SF.
        auto ResolvedTopCandidate_sf = tr.getVec<float>("ResolvedTopCandidate_sf");
        auto ResTopCand_CSPur_Up = tr.getVec<float>("ResolvedTopCandidate_syst_CSPur_Up");
        auto ResTopCand_CSPur_Down = tr.getVec<float>("ResolvedTopCandidate_syst_CSPur_Down");
        auto ResTopCand_Stat_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Stat_Up");
        auto ResTopCand_Stat_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Stat_Down");
        auto ResTopCand_Btag_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Btag_Up");
        auto ResTopCand_Btag_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Btag_Down");
        auto ResTopCand_Closure_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Closure_Up");
        auto ResTopCand_Closure_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Closure_Down");
        auto ResTopCand_Pileup_Up = tr.getVec<float>("ResolvedTopCandidate_syst_Pileup_Up");
        auto ResTopCand_Pileup_Down = tr.getVec<float>("ResolvedTopCandidate_syst_Pileup_Down");
        auto ResolvedTop_Stop0l = tr.getVec<unsigned char>("ResolvedTop_Stop0l");
        auto ResolvedTopCandidate_genMatch = tr.getVec<unsigned char>("ResolvedTopCandidate_genMatch");
        auto ResolvedTopCandidate_pt = tr.getVec<float>("ResolvedTopCandidate_pt");
        auto ResolvedTopCandidate_eta = tr.getVec<float>("ResolvedTopCandidate_eta");
        auto ResolvedTopCandidate_phi = tr.getVec<float>("ResolvedTopCandidate_phi");
        auto ResolvedTopCandidate_mass = tr.getVec<float>("ResolvedTopCandidate_mass");

        
        float ResTop_SF = 1.0;
        float ResTop_SF_up = 0.0;
        float ResTop_SF_down = 0.0;

        float test_sf = 1.0;
        float test_sf_cspur_up = 1.0;
        float test_sf_stat_up = 1.0;
        float test_sf_cspur_down = 1.0;
        float test_sf_stat_down = 1.0;

        int n_falseres = 0;
        for(int v = 0; v < ResolvedTop_Stop0l.size(); v++)
        {
            if(ResolvedTop_Stop0l[v])
            {
                if(ResolvedTopCandidate_pt[v] >= 1000 && ResolvedTopCandidate_sf[v] == 0)
                {
                    n_falseres++;
                }
                else
                {
                    ResTop_SF *= ResolvedTopCandidate_sf[v];
                    ResTop_SF_up = ResTop_SF_up + std::pow(ResTopCand_CSPur_Up[v],2) + std::pow(ResTopCand_Stat_Up[v],2) + std::pow(ResTopCand_Btag_Up[v],2) + std::pow(ResTopCand_Pileup_Up[v],2);
                    ResTop_SF_down = ResTop_SF_down + std::pow(ResTopCand_CSPur_Down[v],2) + std::pow(ResTopCand_Stat_Down[v],2) + std::pow(ResTopCand_Btag_Down[v],2) + std::pow(ResTopCand_Pileup_Down[v],2);
                    if(!ResolvedTopCandidate_genMatch[v])
                    {
                        ResTop_SF_up += std::pow(ResTopCand_Closure_Up[v],2);
                        ResTop_SF_down += std::pow(ResTopCand_Closure_Down[v],2);
                    }
                }
                test_sf_cspur_up = ResTopCand_CSPur_Up[v];
                test_sf_stat_up = ResTopCand_Stat_Up[v];
                test_sf_cspur_down = ResTopCand_CSPur_Down[v];
                test_sf_stat_down = ResTopCand_Stat_Down[v];
            }
        }
        ResTop_SF_up = std::sqrt(ResTop_SF_up);
        ResTop_SF_down = std::sqrt(ResTop_SF_down);
        //ResTop_SF_up = ResTop_SF * (1 + ResTop_SF_up);
        ResTop_SF_up = ResTop_SF + ResTop_SF_up;
        //ResTop_SF_down = ResTop_SF * (1 - ResTop_SF_down);
        ResTop_SF_down = ResTop_SF - ResTop_SF_down;

        if(verbose)
        {
            std::cout << "ResTop SF: " << ResTop_SF << "\tResTop SF Down: " << ResTop_SF_down << "\tlast CSPur down: " << test_sf_cspur_down << "\tlast stat down: " << test_sf_stat_down << std::endl;
        }
        

        //Merged top tag SF

        //W tag SF

        
        //both are part of the following:
        //FatJetSF
        auto vec_FatJet_SF = tr.getVec<float>("FatJet_SF");
        auto vec_FatJet_SFerr = tr.getVec<float>("FatJet_SFerr");
        auto FatJet_Stop0l = tr.getVec<int>("FatJet_Stop0l");
        float W_SF = 1.0;
        float W_SFerr = 0.0;
        float MergedTop_SF = 1.0;
        float MergedTop_SFerr = 0.0;
        for(int v = 0; v < vec_FatJet_SF.size(); v++)
        {
            if(FatJet_Stop0l[v] == 1)
            {
                MergedTop_SF *= vec_FatJet_SF[v];
                MergedTop_SFerr += vec_FatJet_SFerr[v]*vec_FatJet_SFerr[v];
            }
            else if(FatJet_Stop0l[v] == 2)
            {
                W_SF *= vec_FatJet_SF[v];
                W_SFerr += vec_FatJet_SFerr[v]*vec_FatJet_SFerr[v];
            }
        }
        float W_SF_up = W_SF + std::sqrt(W_SFerr);
        float W_SF_down = W_SF - std::sqrt(W_SFerr);
        float MergedTop_SF_up = MergedTop_SF + std::sqrt(MergedTop_SFerr);
        float MergedTop_SF_down = MergedTop_SF - std::sqrt(MergedTop_SFerr);
        
        //TODO LHEScaleWeights
        std::vector<float> LHEScaleWeight{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
        if(doLHEScaleWeight)
        {
            if(tr.checkBranch("LHEScaleWeight")) LHEScaleWeight = tr.getVec<float>("LHEScaleWeight");
            //else std::cout << "Can't find LHEScaleWeight. Keeping at 1.0." << std::endl;
        }
        //New vars for v6:
        auto Stop0l_ResTopWeight = tr.getVar<float>("Stop0l_ResTopWeight");
        auto Stop0l_ResTopWeight_Up = tr.getVar<float>("Stop0l_ResTopWeight_Up");
        auto Stop0l_ResTopWeight_Dn = tr.getVar<float>("Stop0l_ResTopWeight_Dn");
        auto Stop0l_ResTopWeight_JESUp = tr.getVar<float>("Stop0l_ResTopWeight_JESUp");
        auto Stop0l_ResTopWeight_JESDown = tr.getVar<float>("Stop0l_ResTopWeight_JESDown");
        //orig
        /*
        auto Stop0l_DeepAK8_SFWeight = tr.getVar<float>("Stop0l_DeepAK8_SFWeight");
        auto Stop0l_DeepAK8_SFWeight_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_total_up");
        auto Stop0l_DeepAK8_SFWeight_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_total_dn");
        auto Stop0l_DeepAK8_SFWeight_veto_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_veto_up");
        auto Stop0l_DeepAK8_SFWeight_veto_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_veto_dn");
        auto Stop0l_DeepAK8_SFWeight_w_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_w_up");
        auto Stop0l_DeepAK8_SFWeight_w_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_w_dn");
        auto Stop0l_DeepAK8_SFWeight_top_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_top_up");
        auto Stop0l_DeepAK8_SFWeight_top_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_top_dn");
        float Stop0l_DeepAK8_SFWeight_densetop_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_densetop_up");
        float Stop0l_DeepAK8_SFWeight_densetop_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_densetop_dn");
        */
        //recalc
        auto Stop0l_DeepAK8_SFWeight_densetop_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_densetop_up");
        auto Stop0l_DeepAK8_SFWeight_densetop_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_densetop_dn");
        auto Stop0l_DeepAK8_SFWeight = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc");
        auto Stop0l_DeepAK8_SFWeight_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_up");
        auto Stop0l_DeepAK8_SFWeight_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_dn");
        auto Stop0l_DeepAK8_SFWeight_veto_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_up");
        auto Stop0l_DeepAK8_SFWeight_veto_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_dn");
        auto Stop0l_DeepAK8_SFWeight_w_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_up");
        auto Stop0l_DeepAK8_SFWeight_w_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_dn");
        auto Stop0l_DeepAK8_SFWeight_top_up = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_up");
        auto Stop0l_DeepAK8_SFWeight_top_dn = tr.getVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_dn");

        //std::cout << (Stop0l_DeepAK8_SFWeight_top_up - Stop0l_DeepAK8_SFWeight_recalc_top_up)/Stop0l_DeepAK8_SFWeight_recalc_top_up << std::endl;
       
        //MET uncertainty
        //doesn't seem to be a SF, but rather error. Listed in MET var section.

        //mtb
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "mtb" << std::endl;
        if(verbose) std::cout << "mtb" << std::endl;
        float mtb = tr.getVar<float>("Stop0l_Mtb");
        float mtb_JESUp = tr.getVar<float>("Stop0l_Mtb_JESUp");
        float mtb_JESDown = tr.getVar<float>("Stop0l_Mtb_JESDown");
        float mtb_METUnClustUp = tr.getVar<float>("Stop0l_Mtb_METUnClustUp");
        float mtb_METUnClustDown = tr.getVar<float>("Stop0l_Mtb_METUnClustDown");

        //njets
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "njets" << std::endl;
        if(verbose) std::cout << "njets" << std::endl;
        int njetspt30 = tr.getVar<int>("Stop0l_nJets");
        int njets = njetspt30;
        int njets_JESUp = tr.getVar<int>("Stop0l_nJets_JESUp");
        int njets_JESDown = tr.getVar<int>("Stop0l_nJets_JESDown");
        //int njets_METUnClustUp = tr.getVar<int>("Stop0l_nJets_METUnClustUp");
        //int njets_METUnClustDown = tr.getVar<int>("Stop0l_nJets_METUnClustDown");

        //ntop
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ntop" << std::endl;
        if(verbose) std::cout << "ntop" << std::endl;
        int ntop_merge = tr.getVar<int>("Stop0l_nTop");
        int ntop_merge_JESUp = tr.getVar<int>("Stop0l_nTop_JESUp");
        int ntop_merge_JESDown = tr.getVar<int>("Stop0l_nTop_JESDown");

        //nw
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nw" << std::endl;
        if(verbose) std::cout << "nw" << std::endl;
        int nw = tr.getVar<int>("Stop0l_nW");
        int nw_JESUp = tr.getVar<int>("Stop0l_nW_JESUp");
        int nw_JESDown = tr.getVar<int>("Stop0l_nW_JESDown");

        //nres
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nres" << std::endl;
        if(verbose) std::cout << "nres" << std::endl;
        int ntop_res = tr.getVar<int>("Stop0l_nResolved");
        int ntop_res_JESUp = tr.getVar<int>("Stop0l_nResolved_JESUp");
        int ntop_res_JESDown = tr.getVar<int>("Stop0l_nResolved_JESDown");
        ntop_res -= n_falseres;
        ntop_res_JESUp -= n_falseres;
        ntop_res_JESDown -= n_falseres;

        if(ResTop_SF == 0)
        {
            std::cout << "Event Number: " << tr.getEvtNum() << "\tResTop SF: " << ResTop_SF << "\tResTop SF Up: " << ResTop_SF_up << "\tResTop SF Down: " << ResTop_SF_down << std::endl;
            std::cout << "ntop_res: " << ntop_res << "\tntop_res_JESUp: " << ntop_res_JESUp << "\tntop_res_JESDown: " << ntop_res_JESDown << std::endl;
            std::cout << "ResolvedTopCandidate pt size: " << ResolvedTopCandidate_pt.size() << "\tsf size: " << ResolvedTopCandidate_sf.size() << std::endl;
            for(int v = 0; v < ResolvedTopCandidate_pt.size(); v++)
            {
                if(ResolvedTop_Stop0l[v]) std::cout << "true ";
                else std::cout << "false ";
                std::cout << "pt: " << ResolvedTopCandidate_pt[v] << "\teta: " << ResolvedTopCandidate_eta[v] << "\tphi: " << ResolvedTopCandidate_phi[v] << "\tmass: " << ResolvedTopCandidate_mass[v] << std::endl;
            }
            for(int v = 0; v < ResolvedTopCandidate_sf.size(); v++)
            {
                std::cout << ResolvedTopCandidate_sf[v] << " ";
            }
            std::cout << std::endl;
        }

        //nb
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nb" << std::endl;
        if(verbose) std::cout << "nb" << std::endl;
        int nbottompt20 = tr.getVar<int>("Stop0l_nbtags");
        int nb = nbottompt20;
        int nb_JESUp = tr.getVar<int>("Stop0l_nbtags_JESUp");
        int nb_JESDown = tr.getVar<int>("Stop0l_nbtags_JESDown");

        //nSV //TODO: JESUp, JESDown ?
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "nSV" << std::endl;
        if(verbose) std::cout << "nSV" << std::endl;
        int nSV = tr.getVar<int>("Stop0l_nSoftb");
        int nSV_JESUp = tr.getVar<int>("Stop0l_nSoftb");
        int nSV_JESDown = tr.getVar<int>("Stop0l_nSoftb");

        //ISRpt
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ISRpt" << std::endl;
        if(verbose) std::cout << "ISRpt" << std::endl;
        float ISRpt = tr.getVar<float>("Stop0l_ISRJetPt");
        float ISRpt_JESUp = tr.getVar<float>("Stop0l_ISRJetPt_JESUp");
        float ISRpt_JESDown = tr.getVar<float>("Stop0l_ISRJetPt_JESDown");

        //bottompt_scalar_sum
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "ptb" << std::endl;
        if(verbose) std::cout << "ptb" << std::endl;
        float ptb = tr.getVar<float>("Stop0l_Ptb");
        float ptb_JESUp = tr.getVar<float>("Stop0l_Ptb_JESUp");
        float ptb_JESDown = tr.getVar<float>("Stop0l_Ptb_JESDown");

        //HT
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "HT" << std::endl;
        if(verbose) std::cout << "HT" << std::endl;
        float HT = tr.getVar<float>("Stop0l_HT");
        float HT_JESUp = tr.getVar<float>("Stop0l_HT_JESUp");
        float HT_JESDown = tr.getVar<float>("Stop0l_HT_JESDown");

        //met //defined by baselines

        //S_met
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "S_met" << std::endl;
        if(verbose) std::cout << "S_met" << std::endl;
        float S_met = tr.getVar<float>("Stop0l_METSig");
        float S_met_JESUp = tr.getVar<float>("Stop0l_METSig_JESUp");
        float S_met_JESDown = tr.getVar<float>("Stop0l_METSig_JESDown");
        float S_met_METUnClustUp = tr.getVar<float>("Stop0l_METSig_METUnClustUp");
        float S_met_METUnClustDown = tr.getVar<float>("Stop0l_METSig_METUnClustDown");

        //Event Filters
        //if(verbose && (tr.getEvtNum() < 1000)) std::cout << "Event Filters" << std::endl;
        if(verbose) std::cout << "Event Filters" << std::endl;
        bool Pass_lowDM_nodPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_NJets30") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200);
        bool Val_bin_0_14 = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Val_bin_15_18 = Pass_lowDM_nodPhi && tr.getVar<bool>("Pass_dPhiMETMedDM");
        bool Pass_highDM_no_dPhi = (Pass_EventFilter && tr.getVar<bool>("Pass_LeptonVeto") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42 = (Pass_highDM_no_dPhi && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm = Pass_EventFilter && tr.getVar<bool>("Pass_lowDM");
        bool Search_highdm = Pass_EventFilter && tr.getVar<bool>("Pass_highDM");

        bool Val_bin_0_14_nolep = (Pass_EventFilter && tr.getVar<bool>("Pass_NJets30") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200 && tr.getVar<bool>("Pass_dPhiMETLowDM"));
        bool Val_bin_15_18_nolep = (Pass_EventFilter && tr.getVar<bool>("Pass_NJets30") && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb < 175 && S_met > 10 && ISRpt >= 200 && tr.getVar<bool>("Pass_dPhiMETMedDM"));
        bool Val_bin_19_42_nolep = (Pass_EventFilter && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1 && tr.getVar<bool>("Pass_dPhiMET") && (!tr.getVar<bool>("Pass_dPhiMETHighDM")));
        bool Search_lowdm_nolep = Val_bin_0_14_nolep;
        bool Search_highdm_nolep = (Pass_EventFilter && tr.getVar<bool>("Pass_MET") && tr.getVar<bool>("Pass_HT") && njets >= 5 && nb >= 1 && tr.getVar<bool>("Pass_dPhiMETHighDM"));

        bool Pass_lowDM_nodPhi_JESUp = (Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_LeptonVeto_JESUp") && tr.getVar<bool>("Pass_NJets30_JESUp") && tr.getVar<bool>("Pass_MET_JESUp") && tr.getVar<bool>("Pass_HT_JESUp") && ntop_merge_JESUp == 0 && ntop_res_JESUp == 0 && nw_JESUp == 0 && mtb_JESUp < 175 && S_met_JESUp > 10 && ISRpt_JESUp >= 200);
        bool Val_bin_0_14_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_lowDM_JESUp");
        bool Val_bin_15_18_JESUp = Pass_lowDM_nodPhi_JESUp && tr.getVar<bool>("Pass_dPhiMETMedDM_JESUp");
        bool Pass_highDM_no_dPhi_JESUp = (Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_LeptonVeto_JESUp") && tr.getVar<bool>("Pass_MET_JESUp") && tr.getVar<bool>("Pass_HT_JESUp") && njets_JESUp >= 5 && nb_JESUp >= 1);
        bool Val_bin_19_42_JESUp = (Pass_highDM_no_dPhi_JESUp && tr.getVar<bool>("Pass_dPhiMET_JESUp") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_JESUp")));
        bool Search_lowdm_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_lowDM_JESUp");
        bool Search_highdm_JESUp = Pass_EventFilter_JESUp && tr.getVar<bool>("Pass_highDM_JESUp");

        bool Pass_lowDM_nodPhi_JESDown = (Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_LeptonVeto_JESDown") && tr.getVar<bool>("Pass_NJets30_JESDown") && tr.getVar<bool>("Pass_MET_JESDown") && tr.getVar<bool>("Pass_HT_JESDown") && ntop_merge_JESDown == 0 && ntop_res_JESDown == 0 && nw_JESDown == 0 && mtb_JESDown < 175 && S_met_JESDown > 10 && ISRpt_JESDown >= 200);
        bool Val_bin_0_14_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_lowDM_JESDown");
        bool Val_bin_15_18_JESDown = Pass_lowDM_nodPhi_JESDown && tr.getVar<bool>("Pass_dPhiMETMedDM_JESDown");
        bool Pass_highDM_no_dPhi_JESDown = (Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_LeptonVeto_JESDown") && tr.getVar<bool>("Pass_MET_JESDown") && tr.getVar<bool>("Pass_HT_JESDown") && njets_JESDown >= 5 && nb_JESDown >= 1);
        bool Val_bin_19_42_JESDown = (Pass_highDM_no_dPhi_JESDown && tr.getVar<bool>("Pass_dPhiMET_JESDown") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_JESDown")));
        bool Search_lowdm_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_lowDM_JESDown");
        bool Search_highdm_JESDown = Pass_EventFilter_JESDown && tr.getVar<bool>("Pass_highDM_JESDown");

        bool Pass_lowDM_nodPhi_METUnClustUp = (Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_LeptonVeto_METUnClustUp") && tr.getVar<bool>("Pass_NJets30_METUnClustUp") && tr.getVar<bool>("Pass_MET_METUnClustUp") && tr.getVar<bool>("Pass_HT_METUnClustUp") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb_METUnClustUp < 175 && S_met_METUnClustUp > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_lowDM_METUnClustUp");
        bool Val_bin_15_18_METUnClustUp = Pass_lowDM_nodPhi_METUnClustUp && tr.getVar<bool>("Pass_dPhiMETMedDM_METUnClustUp");
        bool Pass_highDM_no_dPhi_METUnClustUp = (Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_LeptonVeto_METUnClustUp") && tr.getVar<bool>("Pass_MET_METUnClustUp") && tr.getVar<bool>("Pass_HT_METUnClustUp") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METUnClustUp = (Pass_highDM_no_dPhi_METUnClustUp && tr.getVar<bool>("Pass_dPhiMET_METUnClustUp") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustUp")));
        bool Search_lowdm_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_lowDM_METUnClustUp");
        bool Search_highdm_METUnClustUp = Pass_EventFilter_METUnClustUp && tr.getVar<bool>("Pass_highDM_METUnClustUp");

        bool Pass_lowDM_nodPhi_METUnClustDown = (Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_LeptonVeto_METUnClustDown") && tr.getVar<bool>("Pass_NJets30_METUnClustDown") && tr.getVar<bool>("Pass_MET_METUnClustDown") && tr.getVar<bool>("Pass_HT_METUnClustDown") && ntop_merge == 0 && ntop_res == 0 && nw == 0 && mtb_METUnClustDown < 175 && S_met_METUnClustDown > 10 && ISRpt >= 200);
        bool Val_bin_0_14_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_lowDM_METUnClustDown");
        bool Val_bin_15_18_METUnClustDown = Pass_lowDM_nodPhi_METUnClustDown && tr.getVar<bool>("Pass_dPhiMETMedDM_METUnClustDown");
        bool Pass_highDM_no_dPhi_METUnClustDown = (Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_LeptonVeto_METUnClustDown") && tr.getVar<bool>("Pass_MET_METUnClustDown") && tr.getVar<bool>("Pass_HT_METUnClustDown") && njets >= 5 && nb >= 1);
        bool Val_bin_19_42_METUnClustDown = (Pass_highDM_no_dPhi_METUnClustDown && tr.getVar<bool>("Pass_dPhiMET_METUnClustDown") && (!tr.getVar<bool>("Pass_dPhiMETHighDM_METUnClustDown")));
        bool Search_lowdm_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_lowDM_METUnClustDown");
        bool Search_highdm_METUnClustDown = Pass_EventFilter_METUnClustDown && tr.getVar<bool>("Pass_highDM_METUnClustDown");

        //pass met: met >= 250

        float evtWeight = 1.0;
        float lowDMevtWeight = 1.0;
        float highDMevtWeight = 1.0;
        float lowDMevtWeight_JESUp = 1.0;
        float lowDMevtWeight_JESDown = 1.0;
        float highDMevtWeight_JESUp = 1.0;
        float highDMevtWeight_JESDown = 1.0;
        float lowDMevtWeight_METUnClustUp = 1.0;
        float lowDMevtWeight_METUnClustDown = 1.0;
        float highDMevtWeight_METUnClustUp = 1.0;
        float highDMevtWeight_METUnClustDown = 1.0;
        float B_SF = 1.0;
        float B_SF_Down = 1.0;
        float B_SF_Up = 1.0;
        float B_SF_Light = 1.0;
        float B_SF_Light_Down = 1.0;
        float B_SF_Light_Up = 1.0;
        float B_SF_Heavy = 1.0;
        float B_SF_Heavy_Down = 1.0;
        float B_SF_Heavy_Up = 1.0;
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
        
        if(verbose) std::cout << "Weights" << std::endl;

        if(!isData)
        {
            B_SF = tr.getVar<float>("BTagWeight");
            B_SF_Down = tr.getVar<float>("BTagWeight_Down");
            B_SF_Up = tr.getVar<float>("BTagWeight_Up");
            B_SF_Light = tr.getVar<float>("BTagWeightLight");
            B_SF_Light_Down = tr.getVar<float>("BTagWeightLight_Down");
            B_SF_Light_Up = tr.getVar<float>("BTagWeightLight_Up");
            B_SF_Heavy = tr.getVar<float>("BTagWeightHeavy");
            B_SF_Heavy_Down = tr.getVar<float>("BTagWeightHeavy_Down");
            B_SF_Heavy_Up = tr.getVar<float>("BTagWeightHeavy_Up");
            //v6
            trigger_eff = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline");
            trigger_eff_down = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_down");
            trigger_eff_up = tr.getVar<float>("Stop0l_trigger_eff_MET_loose_baseline_up");
            pdfWeight_Up = tr.getVar<float>("pdfWeight_Up");
            pdfWeight_Down = tr.getVar<float>("pdfWeight_Down");
            if((era != "2017") || nosplit)
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
            //sanity check for large or infinite weights
            //float* weightList[] = {B_SF, SB_SF, trigger_eff, puWeight, PrefireWeight, Stop0l_DeepAK8_SFWeight, Stop0l_ResTopWeight};
            float* weightList[] = {&B_SF, &B_SF_Up, &B_SF_Down, &SB_SF, &SB_SF_up, &SB_SF_down, &trigger_eff, &trigger_eff_up, &trigger_eff_down, &puWeight, &puWeightUp, &puWeightDown, &PrefireWeight, &PrefireWeightUp, &PrefireWeightDown, &Stop0l_DeepAK8_SFWeight, &Stop0l_DeepAK8_SFWeight_w_up, &Stop0l_DeepAK8_SFWeight_w_dn, &Stop0l_DeepAK8_SFWeight_top_up, &Stop0l_DeepAK8_SFWeight_top_dn, &Stop0l_DeepAK8_SFWeight_densetop_up, &Stop0l_DeepAK8_SFWeight_densetop_dn, &Stop0l_DeepAK8_SFWeight_veto_up, &Stop0l_DeepAK8_SFWeight_veto_dn, &Stop0l_ResTopWeight, &Stop0l_ResTopWeight_Up, &Stop0l_ResTopWeight_Dn, &Electron_SF, &Electron_SF_up, &Electron_SF_down, &Muon_MediumSF, &Muon_MediumSF_up, &Muon_MediumSF_down, &Tau_MediumSF, &Tau_MediumSF_Up, &Tau_MediumSF_Down, &Electron_VetoSF, &Electron_VetoSF_up, &Electron_VetoSF_down, &Stop0l_ResTopWeight_JESUp, &Stop0l_ResTopWeight_JESDown};
            for(int i = 0; i < std::size(weightList); i++)
            {
                if(!isfinite(*weightList[i]))
                {
                    std::cout << "Infinite weight indexed at " << i << " in event " << tr.getVar<unsigned long long>("event") << ". Setting to 1.0." << std::endl;
                    *weightList[i] = 1.0;
                }
                if(*weightList[i] > 5.0)
                {
                    std::cout << "Large weight indexed at " << i << " in event " << tr.getVar<unsigned long long>("event") << ". Capping to 5.0." << std::endl;
                    *weightList[i] = 5.0;
                }
                if(*weightList[i] < 0.0)
                {
                    std::cout << "Negative weight indexed at " << i << " in event " << tr.getVar<unsigned long long>("event") << ". Setting to 1.0." << std::endl;
                    *weightList[i] = 1.0;
                }
            }
            for(int i = 0; i < 9; i++)
            {
                if(!isfinite(LHEScaleWeight[i]))
                {
                    std::cout << "Infinite LHEScaleWeight[" << i << "] in event " << tr.getVar<unsigned long long>("event") << ". Setting to 1.0." << std::endl;
                    LHEScaleWeight[i] = 1.0;
                }
                if(LHEScaleWeight[i] > 5.0)
                {
                    std::cout << "Large LHEScaleWeight[" << i << "] in event " << tr.getVar<unsigned long long>("event") << ". Capping to 5.0." << std::endl;
                    LHEScaleWeight[i] = 5.0;
                }
                if(LHEScaleWeight[i] < 0.0)
                {
                    std::cout << "Negative LHEScaleWeight[" << i << "] in event " << tr.getVar<unsigned long long>("event") << ". Setting to 1.0." << std::endl;
                    LHEScaleWeight[i] = 1.0;
                }
            }

            //lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * Electron_VetoSF * Muon_MediumSF * Tau_MediumSF * W_SF * MergedTop_SF * ResTop_SF;
            //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * Electron_VetoSF * Muon_MediumSF * Tau_MediumSF * W_SF * MergedTop_SF * ResTop_SF;
            //Turning off veto scale factors for full status (lepton veto as well as low dm top/w veto)
            //lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF;
            //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight;
            if(nosplit && (era == "2018") && Pass_HEMVeto30) nosplit_lumi = 59.699489;
            if(nosplit && (era == "2018") && Pass_HEMVeto30_JESUp) nosplit_lumi_JESUp = 59.699489;
            if(nosplit && (era == "2018") && Pass_HEMVeto30_JESDown) nosplit_lumi_JESDown = 59.699489;
            if(nosplit && (era == "2018") && Pass_HEMVeto30_METUnClustUp) nosplit_lumi_METUnClustUp = 59.699489;
            if(nosplit && (era == "2018") && Pass_HEMVeto30_METUnClustDown) nosplit_lumi_METUnClustDown = 59.699489;
            //add in recalc if running over it here
            lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            //v5
            //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi;
            //v6
            highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            W_SF = 1.0;
            MergedTop_SF = 1.0;
            ResTop_SF = 1.0;

            lowDMevtWeight_JESUp = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi_JESUp * Stop0l_ResTopWeight_JESUp * Stop0l_DeepAK8_SFWeight;
            highDMevtWeight_JESUp = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi_JESUp * Stop0l_ResTopWeight_JESUp * Stop0l_DeepAK8_SFWeight;
            lowDMevtWeight_JESDown = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi_JESDown * Stop0l_ResTopWeight_JESDown * Stop0l_DeepAK8_SFWeight;
            highDMevtWeight_JESDown = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi_JESDown * Stop0l_ResTopWeight_JESDown * Stop0l_DeepAK8_SFWeight;
            lowDMevtWeight_METUnClustUp = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi_METUnClustUp * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            highDMevtWeight_METUnClustUp = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi_METUnClustUp * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            lowDMevtWeight_METUnClustDown = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi_METUnClustDown * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            highDMevtWeight_METUnClustDown = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi_METUnClustDown * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            //Multiply in W_SF, MergedTop_SF, and ResTop_SF as needed
            //Hui: evtWeight = evtWeight * B_SF * trigger_eff * puWeight * PrefireWeight * Top_SF;
            //diff's: SB_SF, mergedtop/w sf?
            //lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight;
            //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight;
            //lowDMevtWeight = sign;
            //highDMevtWeight = sign;
        }
        h_weights->GetXaxis()->SetBinLabel(1,"sign");
        h_weights->Fill(0.,sign);
        h_weights->GetXaxis()->SetBinLabel(2,"evtWeight");
        h_weights->Fill(1.,evtWeight);
        h_weights->GetXaxis()->SetBinLabel(3,"B");
        h_weights->Fill(2.,B_SF);
        h_weights->GetXaxis()->SetBinLabel(4,"trigger_eff");
        h_weights->Fill(3.,trigger_eff);
        h_weights->GetXaxis()->SetBinLabel(5,"Pileup");
        h_weights->Fill(4.,puWeight);
        h_weights->GetXaxis()->SetBinLabel(6,"Prefire");
        h_weights->Fill(5.,PrefireWeight);
        h_weights->GetXaxis()->SetBinLabel(7,"Soft B");
        h_weights->Fill(6.,SB_SF);
        h_weights->GetXaxis()->SetBinLabel(8,"ResTop");
        h_weights->Fill(7.,Stop0l_ResTopWeight);
        h_weights->GetXaxis()->SetBinLabel(9,"DeepAK8");
        h_weights->Fill(8.,Stop0l_DeepAK8_SFWeight);

        MET->Fill(met,lowDMevtWeight);
        BTagWeight->Fill(B_SF,lowDMevtWeight);
        PUWeight->Fill(puWeight);

        /*
        if((lowDMevtWeight/nosplit_lumi) > 3.0)
        {
            std::cout << "Large event weight. Event number " << tr.getVar<unsigned long long>("event") << "\tWeight before lumi * xs * veto eff * 1000/nevents: " << lowDMevtWeight/nosplit_lumi << std::endl;
            std::cout << "B_SF: " << B_SF << " SB_SF: " << SB_SF << " trigger_eff: " << trigger_eff << " puWeight: " << puWeight << " PrefireWeight: " << PrefireWeight << " DeepAK8: " << Stop0l_DeepAK8_SFWeight << " ResTop: " << Stop0l_ResTopWeight << std::endl;
            //lowDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * SB_SF * nosplit_lumi * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
            //std::cout << "ak8veto_up: " << Stop0l_DeepAK8_SFWeight_veto_up << "\tak8veto_dn: " << Stop0l_DeepAK8_SFWeight_veto_dn << std::endl;
            std::cout << "nb " << nb << "\tmtb " << mtb << "\tptb " << ptb << "\tmet " << met << "\tnSV " << nSV << "\tnjets " << njets << std::endl;
            std::cout << "ISRpt " << ISRpt << "\tHT" << HT << "\t nres" << ntop_res << "\tnt " << ntop_merge << "\tnw" << nw << std::endl;
            
        }
        */

        //if(!Val_bin_0_14 && !Val_bin_15_18 && !Val_bin_19_42 && !Search_lowdm && !Search_highdm) continue;
        //eff_h->Fill(2.,sign);

        if(verbose) std::cout << "Validation Bins" << std::endl;

        int bin_num = -1;
        int bin_num_JESUp = -1;
        int bin_num_JESDown = -1;
        int bin_num_METUnClustUp = -1;
        int bin_num_METUnClustDown = -1;
        int bin_num_nolep = -1;
        int reset = 0;

        /*
        if(tr.getVar<unsigned long long>("event") == 177709245)
        {
            std::cout << "Event 177709245. ntop_merge = " << ntop_merge << ". ntop_res = " << ntop_res << std::endl;
        }
        */
        int bin_num_val_low = -1;
        if(Val_bin_0_14) bin_num_val_low = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met);
        int bin_num_val_low_highMET = -1;
        if(Val_bin_15_18) bin_num_val_low_highMET = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met);
        int bin_num_val_high = -1;
        if(Val_bin_19_42) bin_num_val_high = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,met);
        if((bin_num_val_low == -1) && (bin_num_val_low_highMET == -1) && (bin_num_val_high == -1))
        {
                h_vb->Fill(bin_num,SF*highDMevtWeight);
                h_vb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_vb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_vb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_vb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_vb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_vb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_vb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
        }

        //Bins 0-14: int SB_lowdm_validation(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Val_bin_0_14)
        {
            bin_num = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met);
            if(bin_num != -1)
            {
                h_vb->Fill(bin_num,SF*lowDMevtWeight);
                h_vb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_vb_bsf_light_up->Fill(bin_num, lowDMevtWeight * B_SF_Light_Up / B_SF_Light);
                h_vb_bsf_light_down->Fill(bin_num, lowDMevtWeight * B_SF_Light_Down / B_SF_Light);
                h_vb_bsf_heavy_up->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Up / B_SF_Heavy);
                h_vb_bsf_heavy_down->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Down / B_SF_Heavy);
                h_vb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_vb_LHEScaleWeight_0->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[0]);
                h_vb_LHEScaleWeight_1->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[1]);
                h_vb_LHEScaleWeight_3->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[3]);
                h_vb_LHEScaleWeight_4->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[4]);
                h_vb_LHEScaleWeight_5->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[5]);
                h_vb_LHEScaleWeight_7->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[7]);
                h_vb_LHEScaleWeight_8->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[8]);
                h_vb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_vb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_vb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_vb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_vb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_vb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_vb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_0_14 has bin number -1." << std::endl;
        }
        if(Val_bin_0_14_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight_JESUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight_JESDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight_METUnClustUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_0_14_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight_METUnClustDown);
                eff_h->Fill(3.,sign);
            }
        }
        
        if(Val_bin_0_14_nolep)
        {
            bin_num_nolep = SB_lowdm_validation(njets,nb,nSV,ISRpt,ptb,met);
            if(bin_num_nolep != -1)
            {
                if(pickedEleIdx != -1)
                {
                    h_vb_elecyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF));
                    h_vb_elecyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_up));
                    h_vb_elecyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_down));
                }
                if(pickedMuIdx  != -1)
                {
                    h_vb_muonyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF));
                    h_vb_muonyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_up));
                    h_vb_muonyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_down));
                }
                if(pickedTauIdx != -1)
                {
                    h_vb_tauyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF));
                    h_vb_tauyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_up));
                    h_vb_tauyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_down));
                }
            }
        }
        

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        //Bins 15-18: SB_lowdm_validation_high_MET(int nb, int nSV, float ISRpt, float met)
        if(Val_bin_15_18)
        {
            bin_num = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met);
            if(bin_num != -1)
            {
                h_vb->Fill(bin_num,SF*lowDMevtWeight);
                h_njets_low_middphi->Fill(njets,SF*lowDMevtWeight);
                h_vb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_vb_bsf_light_up->Fill(bin_num, lowDMevtWeight * B_SF_Light_Up / B_SF_Light);
                h_vb_bsf_light_down->Fill(bin_num, lowDMevtWeight * B_SF_Light_Down / B_SF_Light);
                h_vb_bsf_heavy_up->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Up / B_SF_Heavy);
                h_vb_bsf_heavy_down->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Down / B_SF_Heavy);
                h_vb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight);
                h_vb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_vb_LHEScaleWeight_0->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[0]);
                h_vb_LHEScaleWeight_1->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[1]);
                h_vb_LHEScaleWeight_3->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[3]);
                h_vb_LHEScaleWeight_4->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[4]);
                h_vb_LHEScaleWeight_5->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[5]);
                h_vb_LHEScaleWeight_7->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[7]);
                h_vb_LHEScaleWeight_8->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[8]);
                h_vb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_vb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_vb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_vb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_vb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_vb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_vb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_15_18 has bin number -1." << std::endl;
        }
        if(Val_bin_15_18_JESUp)
        {
            bin_num_JESUp = SB_lowdm_validation_high_MET(nb_JESUp,nSV_JESUp,ISRpt_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_vb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight_JESUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_JESDown)
        {
            bin_num_JESDown = SB_lowdm_validation_high_MET(nb_JESDown,nSV_JESDown,ISRpt_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_vb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight_JESDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustUp)
        {
            bin_num_METUnClustUp = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight_METUnClustUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_15_18_METUnClustDown)
        {
            bin_num_METUnClustDown = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight_METUnClustDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_15_18_nolep)
        {
            bin_num_nolep = SB_lowdm_validation_high_MET(nb,nSV,ISRpt,met);
            if(bin_num_nolep != -1)
            {
                if(pickedEleIdx != -1)
                {
                    h_vb_elecyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF));
                    h_vb_elecyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_up));
                    h_vb_elecyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_down));
                }
                if(pickedMuIdx  != -1)
                {
                    h_vb_muonyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF));
                    h_vb_muonyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_up));
                    h_vb_muonyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_down));
                }
                if(pickedTauIdx != -1)
                {
                    h_vb_tauyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF));
                    h_vb_tauyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_up));
                    h_vb_tauyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_down));
                }
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        float topw_sf = 1;
        //Bins 19-42: SBv3_highdm_validation(float mtb, int njets, int ntop, int nw, int nres, int nb, float met)
        if(Val_bin_19_42)
        {
            bin_num = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,met);
            if(bin_num != -1)
            {
                topw_sf = 1;
                //if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                //if(ntop_res != 0) topw_sf *= ResTop_SF;
                //if(nw != 0) topw_sf *= W_SF;
                h_vb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_njets_high_middphi->Fill(njets,SF*highDMevtWeight * topw_sf);
                h_vb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_vb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_vb_bsf_light_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Up / B_SF_Light);
                h_vb_bsf_light_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Down / B_SF_Light);
                h_vb_bsf_heavy_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Up / B_SF_Heavy);
                h_vb_bsf_heavy_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Down / B_SF_Heavy);
                h_vb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_vb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_vb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_vb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_vb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_vb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_vb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_vb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_vb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_vb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_vb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_vb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_densetoptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_vb_ak8veto_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_vb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_vb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_vb_LHEScaleWeight_0->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[0]);
                h_vb_LHEScaleWeight_1->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[1]);
                h_vb_LHEScaleWeight_3->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[3]);
                h_vb_LHEScaleWeight_4->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[4]);
                h_vb_LHEScaleWeight_5->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[5]);
                h_vb_LHEScaleWeight_7->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[7]);
                h_vb_LHEScaleWeight_8->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[8]);
                h_vb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_vb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_vb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_vb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_vb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_vb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_vb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
            else if(verbose) std::cout << "Event number " << tr.getEvtNum() << " passing Val_bin_19_42 has bin number -1." << std::endl;
        }
        if(Val_bin_19_42_JESUp)
        {
            bin_num_JESUp = SBv3_highdm_validation(mtb_JESUp,njets_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,nb_JESUp,met_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_vb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight_JESUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_JESDown)
        {
            bin_num_JESDown = SBv3_highdm_validation(mtb_JESDown,njets_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,nb_JESDown,met_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_vb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight_JESDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustUp)
        {
            bin_num_METUnClustUp = SBv3_highdm_validation(mtb_METUnClustUp,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustUp);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight_METUnClustUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Val_bin_19_42_METUnClustDown)
        {
            bin_num_METUnClustDown = SBv3_highdm_validation(mtb_METUnClustDown,njets,ntop_merge,nw,ntop_res,nb,met_METUnClustDown);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_vb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight_METUnClustDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Val_bin_19_42_nolep)
        {
            bin_num_nolep = SBv3_highdm_validation(mtb,njets,ntop_merge,nw,ntop_res,nb,met);
            if(bin_num_nolep != -1)
            {
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                if(pickedEleIdx != -1)
                {
                    h_vb_elecyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF));
                    h_vb_elecyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF_up));
                    h_vb_elecyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF_down));
                }
                if(pickedMuIdx  != -1)
                {
                    h_vb_muonyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF));
                    h_vb_muonyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF_up));
                    h_vb_muonyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF_down));
                }
                if(pickedTauIdx != -1)
                {
                    h_vb_tauyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF));
                    h_vb_tauyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF_up));
                    h_vb_tauyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF_down));
                }
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        if(verbose) std::cout << "Search Bins" << std::endl;

        //bool Pass_LowDM = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb < 175);
        //bool Pass_LowDM = (ntop_res == 0) && (ntop_merge == 0) && (nw == 0) && (ISRpt >= 200) && (mtb < 175);
        bool Pass_LowDM = (ntop_res == 0) && (ntop_merge == 0) && (nw == 0) && (ISRpt >= 200) && (mtb < 175) && (S_met > 10);
        bool Pass_HighDM = (njets >= 5) && (nb >= 1);
        //bool Pass_LowDM_JESUp = ntop_res_JESUp == 0 && ntop_merge_JESUp == 0 && nw_JESUp == 0 && ISRpt_JESUp >= 200 && (nb_JESUp == 0 || mtb_JESUp < 175);
        bool Pass_LowDM_JESUp = ntop_res_JESUp == 0 && ntop_merge_JESUp == 0 && nw_JESUp == 0 && ISRpt_JESUp >= 200 && mtb_JESUp < 175;
        //bool Pass_LowDM_JESDown = ntop_res_JESDown == 0 && ntop_merge_JESDown == 0 && nw_JESDown == 0 && ISRpt_JESDown >= 200 && (nb_JESDown == 0 || mtb_JESDown < 175);
        bool Pass_LowDM_JESDown = ntop_res_JESDown == 0 && ntop_merge_JESDown == 0 && nw_JESDown == 0 && ISRpt_JESDown >= 200 && mtb_JESDown < 175;
        bool Pass_HighDM_JESUp = njets_JESUp >= 5 && nb_JESUp >= 1;
        bool Pass_HighDM_JESDown = njets_JESDown >= 5 && nb_JESDown >= 1;
        //bool Pass_LowDM_METUnClustUp = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb_METUnClustUp < 175);
        bool Pass_LowDM_METUnClustUp = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && mtb_METUnClustUp < 175;
        //bool Pass_LowDM_METUnClustDown = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb_METUnClustDown < 175);
        bool Pass_LowDM_METUnClustDown = ntop_res == 0 && ntop_merge == 0 && nw == 0 && ISRpt >= 200 && mtb_METUnClustDown < 175;
        bool Pass_HighDM_METUnClustUp = njets >= 5 && nb >= 1;
        bool Pass_HighDM_METUnClustDown = njets >= 5 && nb >= 1;

        bin_num = SRbin(Pass_Baseline,false,false,false,Pass_HighDM,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
        if(bin_num == -1)
        {
                if(!isfinite(highDMevtWeight))
                {
                    std::cout << "Infinite highDMevtWeight detected in event " << tr.getEvtNum() << " (or " << tr.getVar<unsigned long long>("event") << ")" << std::endl;
                    std::cout << "B_SF: " << B_SF << " SB_SF: " << SB_SF << " trigger_eff: " << trigger_eff << " puWeight: " << puWeight << " PrefireWeight: " << PrefireWeight << " DeepAK8: " << Stop0l_DeepAK8_SFWeight << " ResTop: " << Stop0l_ResTopWeight << std::endl;
                    std::cout << "nb " << nb << "\tmtb " << mtb << "\tptb " << ptb << "\tmet " << met << "\tnSV " << nSV << "\tnjets " << njets << std::endl;
                    std::cout << "ISRpt " << ISRpt << "\tHT" << HT << "\t nres" << ntop_res << "\tnt " << ntop_merge << "\tnw" << nw << std::endl;
                    auto Jet_pt = tr.getVec<float>("Jet_pt");
                    auto Jet_eta = tr.getVec<float>("Jet_eta");
                    auto Jet_btagDeepB = tr.getVec<float>("Jet_btagDeepB");
                    auto Jet_hadronFlavour = tr.getVec<int>("Jet_hadronFlavour");
                    std::cout << "Jet pt, eta, btagDeepB, hadronFlavour:" << std::endl;
                    for(int i = 0; i < Jet_pt.size(); i++)
                    {
                        std::cout << Jet_pt[i] << "\t" << Jet_eta[i] << "\t" << Jet_btagDeepB[i] << "\t" << Jet_hadronFlavour[i] << std::endl;
                    }
                    if(!isfinite(Stop0l_ResTopWeight)) Stop0l_ResTopWeight = 1.0;
                    highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi * Stop0l_ResTopWeight * Stop0l_DeepAK8_SFWeight;
                }
                h_sb->Fill(bin_num,SF*highDMevtWeight);
                h_sb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_sb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_sb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_sb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_sb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_sb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_sb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
        }

        //int SB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Search_lowdm)
        //if(Pass_Baseline && Pass_LowDM)
        {
            //bin_num = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //int SRbin(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = SRbin(Pass_Baseline,false,false,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                h_sb->Fill(bin_num,SF*lowDMevtWeight);
                h_sb_raw->Fill(bin_num,sign);
                h_sb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_sb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_sb_bsf_light_up->Fill(bin_num, lowDMevtWeight * B_SF_Light_Up / B_SF_Light);
                h_sb_bsf_light_down->Fill(bin_num, lowDMevtWeight * B_SF_Light_Down / B_SF_Light);
                h_sb_bsf_heavy_up->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Up / B_SF_Heavy);
                h_sb_bsf_heavy_down->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Down / B_SF_Heavy);
                h_sb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_sb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_sb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_sb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_sb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_sb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_sb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_sb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_sb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_sb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_sb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight);
                h_sb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight);
                h_sb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight);
                h_sb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight);
                h_sb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight);
                h_sb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight);
                //v6
                
                h_sb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_densetoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_densetoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_sb_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                /*
                h_sb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_w_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_w_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_top_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_top_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_veto_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_recalc_veto_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                */
                h_sb_LHEScaleWeight_0->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[0]);
                h_sb_LHEScaleWeight_1->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[1]);
                h_sb_LHEScaleWeight_3->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[3]);
                h_sb_LHEScaleWeight_4->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[4]);
                h_sb_LHEScaleWeight_5->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[5]);
                h_sb_LHEScaleWeight_7->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[7]);
                h_sb_LHEScaleWeight_8->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[8]);
                h_sb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_sb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_sb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_sb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_sb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_sb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_sb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
                /*
                if(bin_num == 6) 
                {
                    std::cout << "bin_num == 6. Event number " << tr.getVar<unsigned long long>("event") << "\tWeight before lumi * xs * veto eff * 1000/nevents: " << SF*lowDMevtWeight*topw_sf/nosplit_lumi << std::endl;
                    std::cout << "B_SF: " << B_SF << " SB_SF: " << SB_SF << " trigger_eff: " << trigger_eff << " puWeight: " << puWeight << " PrefireWeight: " << PrefireWeight << " topw_sf: " << topw_sf << " MergedTop_SF: " << MergedTop_SF << " ResTop_SF: " << ResTop_SF << " W_SF: " << W_SF << " lumi: " << nosplit_lumi << std::endl;
                    //lowDMevtWeight = evtWeight * sign * B_SF * SB_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi;
                    //std::cout << "ak8veto_up: " << Stop0l_DeepAK8_SFWeight_veto_up << "\tak8veto_dn: " << Stop0l_DeepAK8_SFWeight_veto_dn << std::endl;
                    std::cout << "nb " << nb << "\tmtb " << mtb << "\tptb " << ptb << "\tmet " << met << "\tnSV " << nSV << "\tnjets " << njets << std::endl;
                    std::cout << "ISRpt " << ISRpt << "\tHT" << HT << "\t nres" << ntop_res << "\tnt " << ntop_merge << "\tnw" << nw << std::endl;
                    auto Jet_pt = tr.getVec<float>("Jet_pt");
                    auto Jet_eta = tr.getVec<float>("Jet_eta");
                    auto Jet_btagDeepB = tr.getVec<float>("Jet_btagDeepB");
                    auto Jet_hadronFlavour = tr.getVec<int>("Jet_hadronFlavour");
                    std::cout << "Jet pt, eta:" << std::endl;
                    for(int i = 0; i < Jet_pt.size(); i++)
                    {
                        std::cout << Jet_pt[i] << "\t" << Jet_eta[i] << "\t" << Jet_btagDeepB[i] << "\t" << Jet_hadronFlavour[i] << std::endl;
                    }
                }
                */
            }
        }
        if(Search_lowdm_JESUp)
        {
            //bin_num_JESUp = SB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = SRbin(Pass_Baseline_JESUp,false,false,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_sb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight_JESUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_JESDown)
        {
            //bin_num_JESDown = SB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = SRbin(Pass_Baseline_JESDown,false,false,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_sb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight_JESDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = SRbin(Pass_Baseline_METUnClustUp,false,false,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_sb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight_METUnClustUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = SB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = SRbin(Pass_Baseline_METUnClustDown,false,false,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_sb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight_METUnClustDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_lowdm_nolep)
        {
            bin_num_nolep = SRbin(Pass_Baseline_nolep,false,false,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_nolep != -1)
            {
                if(pickedEleIdx != -1)
                {
                    h_sb_elecyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF));
                    h_sb_elecyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_up));
                    h_sb_elecyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Electron_VetoSF_down));
                }
                if(pickedMuIdx  != -1)
                {
                    h_sb_muonyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF));
                    h_sb_muonyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_up));
                    h_sb_muonyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Muon_VetoSF_down));
                }
                if(pickedTauIdx != -1)
                {
                    h_sb_tauyield->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF));
                    h_sb_tauyield_up->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_up));
                    h_sb_tauyield_down->Fill(bin_num_nolep,SF*lowDMevtWeight*(1-Tau_VetoSF_down));
                }
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        bin_num = -1;

        //int SBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Search_highdm)
        //if(Pass_Baseline && Pass_HighDM)
        {
            //bin_num = SBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            bin_num = SRbin(Pass_Baseline,false,false,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                topw_sf = 1;
                //if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                //if(ntop_res != 0) topw_sf *= ResTop_SF;
                //if(nw != 0) topw_sf *= W_SF;
                h_sb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_sb_raw->Fill(bin_num,sign);
                h_sb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_sb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_sb_bsf_light_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Up / B_SF_Light);
                h_sb_bsf_light_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Down / B_SF_Light);
                h_sb_bsf_heavy_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Up / B_SF_Heavy);
                h_sb_bsf_heavy_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Down / B_SF_Heavy);
                h_sb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_sb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_sb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_sb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_sb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_sb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_sb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_sb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_sb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_sb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_sb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_sb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_sb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_sb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_sb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                h_sb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf);
                //v6
                
                h_sb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_densetoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_densetoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_ak8veto_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_sb_ak8veto_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                /*
                h_sb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_w_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_w_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_top_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_top_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_ak8veto_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_veto_up / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_ak8veto_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_DeepAK8_SFWeight_recalc_veto_dn / Stop0l_DeepAK8_SFWeight_recalc);
                h_sb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_sb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                */
                h_sb_LHEScaleWeight_0->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[0]);
                h_sb_LHEScaleWeight_1->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[1]);
                h_sb_LHEScaleWeight_3->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[3]);
                h_sb_LHEScaleWeight_4->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[4]);
                h_sb_LHEScaleWeight_5->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[5]);
                h_sb_LHEScaleWeight_7->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[7]);
                h_sb_LHEScaleWeight_8->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[8]);
                h_sb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_sb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_sb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_sb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_sb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_sb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_sb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
                
                /*
                int binslist[] = {61,62,63,64,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182};
                std::list<int> binlist (binslist, binslist + sizeof(binslist) / sizeof(int));
                int opt[] = {53,54,55,56};
                bool found = (std::find(binlist.begin(), binlist.end(), bin_num) != binlist.end());
                if((SF*highDMevtWeight/nosplit_lumi < 0.7) && found)
                {
                    std::cout << "bin_num == " << bin_num << ". Event number " << tr.getVar<unsigned long long>("event") << "\tWeight before lumi * xs * veto eff * 1000/nevents: " << SF*highDMevtWeight*topw_sf/nosplit_lumi << std::endl;
                    std::cout << "B_SF: " << B_SF << " SB_SF: " << SB_SF << " trigger_eff: " << trigger_eff << " puWeight: " << puWeight << " PrefireWeight: " << PrefireWeight << " DeepAK8: " << Stop0l_DeepAK8_SFWeight << " ResTop: " << Stop0l_ResTopWeight << std::endl;
                    //highDMevtWeight = evtWeight * sign * B_SF * trigger_eff * puWeight * PrefireWeight * nosplit_lumi;
                }
                */
            }
        }
        if(Search_highdm_JESUp)
        {
            //bin_num_JESUp = SBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = SRbin(Pass_Baseline_JESUp,false,false,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_sb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight_JESUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_JESDown)
        {
            //bin_num_JESDown = SBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = SRbin(Pass_Baseline_JESDown,false,false,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_sb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight_JESDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = SBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = SRbin(Pass_Baseline_METUnClustUp,false,false,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight_METUnClustUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(Search_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = SBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = SRbin(Pass_Baseline_METUnClustDown,false,false,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_sb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight_METUnClustDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(Search_highdm_nolep)
        {
            bin_num_nolep = SRbin(Pass_Baseline_nolep,false,false,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_nolep != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                if(pickedEleIdx != -1)
                {
                    h_sb_elecyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF));
                    h_sb_elecyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF_up));
                    h_sb_elecyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Electron_VetoSF_down));
                }
                if(pickedMuIdx  != -1)
                {
                    h_sb_muonyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF));
                    h_sb_muonyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF_up));
                    h_sb_muonyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Muon_VetoSF_down));
                }
                if(pickedTauIdx != -1)
                {
                    h_sb_tauyield->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF));
                    h_sb_tauyield_up->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF_up));
                    h_sb_tauyield_down->Fill(bin_num_nolep,SF*topw_sf*highDMevtWeight*(1-Tau_VetoSF_down));
                }
            }
        }

        if(bin_num != -1)
        {
            reset++;
        }
        //if(reset > 1) std::cout << "Error: Bin number reset multiple times in event " << tr.getEvtNum() << "." << std::endl;

        if(verbose) std::cout << "Unit Bins" << std::endl;

        //int nCRUnitLowDM  = tr.getVar<int>("nCRUnitLowDM_jetpt30");
        //int nCRUnitHighDM = tr.getVar<int>("nCRUnitHighDM_jetpt30");

        /*
        bool Unit_lowdm = Pass_LLCR && Pass_LowDM && (S_met > 10);
        bool Unit_highdm = Pass_LLCR && Pass_HighDM && Pass_dPhiMETHighDM;
        bool Unit_lowdm_JESUp = Pass_LLCR_JESUp && Pass_LowDM_JESUp && (S_met_JESUp > 10);
        bool Unit_lowdm_JESDown = Pass_LLCR_JESDown && Pass_LowDM_JESDown && (S_met_JESDown > 10);
        bool Unit_highdm_JESUp = Pass_LLCR_JESUp && Pass_HighDM_JESUp && Pass_dPhiMETHighDM_JESUp;
        bool Unit_highdm_JESDown = Pass_LLCR_JESDown && Pass_HighDM_JESDown && Pass_dPhiMETHighDM_JESDown;
        bool Unit_lowdm_METUnClustUp = Pass_LLCR_METUnClustUp && Pass_LowDM_METUnClustUp && (S_met_METUnClustUp > 10);
        bool Unit_lowdm_METUnClustDown = Pass_LLCR_METUnClustDown && Pass_LowDM_METUnClustDown && (S_met_METUnClustDown > 10);
        bool Unit_highdm_METUnClustUp = Pass_LLCR_METUnClustUp && Pass_HighDM_METUnClustUp && Pass_dPhiMETHighDM_METUnClustUp;
        bool Unit_highdm_METUnClustDown = Pass_LLCR_METUnClustDown && Pass_HighDM_METUnClustDown && Pass_dPhiMETHighDM_METUnClustDown;
        */
        bool Unit_lowdm = tr.getVar<bool>("Pass_LLCR_lowDM");
        bool Unit_highdm = tr.getVar<bool>("Pass_LLCR_highDM");
        bool Unit_lowdm_JESUp = tr.getVar<bool>("Pass_LLCR_lowDM_JESUp");
        bool Unit_highdm_JESUp = tr.getVar<bool>("Pass_LLCR_highDM_JESUp");
        bool Unit_lowdm_JESDown = tr.getVar<bool>("Pass_LLCR_lowDM_JESDown");
        bool Unit_highdm_JESDown = tr.getVar<bool>("Pass_LLCR_highDM_JESDown");
        bool Unit_lowdm_METUnClustUp = tr.getVar<bool>("Pass_LLCR_lowDM_METUnClustUp");
        bool Unit_highdm_METUnClustUp = tr.getVar<bool>("Pass_LLCR_highDM_METUnClustUp");
        bool Unit_lowdm_METUnClustDown = tr.getVar<bool>("Pass_LLCR_lowDM_METUnClustDown");
        bool Unit_highdm_METUnClustDown = tr.getVar<bool>("Pass_LLCR_highDM_METUnClustDown");
        bool Pass_LLLep = (totlep == 1) && (totMtW < 100);

        bin_num = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,Pass_HighDM,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
        if(bin_num == -1)
        {
                h_ub->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF);
                h_ub_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_ub_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_ub_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_ub_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_ub_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_ub_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_ub_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
        }

        //int UB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(Unit_lowdm)
        {
            //bin_num = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //bin_num = nCRUnitLowDM;
            //int lepCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "Low DM UB num: " << bin_num << std::endl;
                h_ub->Fill(bin_num,SF*lowDMevtWeight * Electron_SF * Muon_MediumSF);
                h_ub_bsf_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Up / B_SF);
                h_ub_bsf_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Down / B_SF);
                h_ub_bsf_light_up->Fill(bin_num, lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Light_Up / B_SF_Light);
                h_ub_bsf_light_down->Fill(bin_num, lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Light_Down / B_SF_Light);
                h_ub_bsf_heavy_up->Fill(bin_num, lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Heavy_Up / B_SF_Heavy);
                h_ub_bsf_heavy_down->Fill(bin_num, lowDMevtWeight * Electron_SF * Muon_MediumSF * B_SF_Heavy_Down / B_SF_Heavy);
                h_ub_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * trigger_eff_up / trigger_eff);
                h_ub_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * trigger_eff_down / trigger_eff);
                h_ub_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * puWeightUp / puWeight);
                h_ub_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * puWeightDown / puWeight);
                h_ub_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF *PrefireWeightUp / PrefireWeight);
                h_ub_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * PrefireWeightDown / PrefireWeight);
                h_ub_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * pdfWeight_Up);
                h_ub_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * pdfWeight_Down);
                h_ub_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * SB_SF_up / SB_SF);
                h_ub_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * SB_SF_down / SB_SF);
                h_ub_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Electron_SF_up / Electron_SF);
                h_ub_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Electron_SF_down / Electron_SF);
                h_ub_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Muon_MediumSF_up / Muon_MediumSF);
                h_ub_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Muon_MediumSF_down / Muon_MediumSF);
                h_ub_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF);
                h_ub_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF);
                h_ub_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_densetoptag_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_densetoptag_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_ub_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_ub_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_ub_LHEScaleWeight_0->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[0]);
                h_ub_LHEScaleWeight_1->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[1]);
                h_ub_LHEScaleWeight_3->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[3]);
                h_ub_LHEScaleWeight_4->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[4]);
                h_ub_LHEScaleWeight_5->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[5]);
                h_ub_LHEScaleWeight_7->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[7]);
                h_ub_LHEScaleWeight_8->Fill(bin_num, SF * lowDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[8]);
                h_ub_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_ub_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_ub_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_ub_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_ub_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_ub_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_ub_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
        }
        if(Unit_lowdm_JESUp)
        {
            //bin_num_JESUp = UB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            //bin_num_JESUp = nCRUnitLowDM;
            //bin_num_JESUp = getsearchbin.getUnitNumLowDM("unitCRNum",njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = lepCRunit(Pass_Baseline_JESUp,false,Pass_LLCR_JESUp,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_ub_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight_JESUp * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_lowdm_JESDown)
        {
            //bin_num_JESDown = UB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            //bin_num_JESDown = nCRUnitLowDM;
            //bin_num_JESDown = getsearchbin.getUnitNumLowDM("unitCRNum",njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = lepCRunit(Pass_Baseline_JESDown,false,Pass_LLCR_JESDown,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_ub_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight_JESDown * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRUnitLowDM;
            //bin_num_METUnClustUp = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = lepCRunit(Pass_Baseline_METUnClustUp,false,Pass_LLCR_METUnClustUp,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_ub_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight_METUnClustUp * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRUnitLowDM;
            //bin_num_METUnClustDown = getsearchbin.getUnitNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = lepCRunit(Pass_Baseline_METUnClustDown,false,Pass_LLCR_METUnClustDown,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_ub_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight_METUnClustDown * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(3.,sign);
            }
        }

        //int UBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(Unit_highdm)
        {
            //bin_num = UBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            //bin_num = nCRUnitHighDM;
            bin_num = lepCRunit(Pass_Baseline,false,Pass_LLCR,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "High DM UB num: " << bin_num << std::endl;
                topw_sf = 1;
                //if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                //if(ntop_res != 0) topw_sf *= ResTop_SF;
                //if(nw != 0) topw_sf *= W_SF;
                h_ub->Fill(bin_num,SF*highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf);
                h_ub_bsf_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Up / B_SF);
                h_ub_bsf_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Down / B_SF);
                h_ub_bsf_light_up->Fill(bin_num, highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Light_Up / B_SF_Light);
                h_ub_bsf_light_down->Fill(bin_num, highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Light_Down / B_SF_Light);
                h_ub_bsf_heavy_up->Fill(bin_num, highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Heavy_Up / B_SF_Heavy);
                h_ub_bsf_heavy_down->Fill(bin_num, highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * B_SF_Heavy_Down / B_SF_Heavy);
                h_ub_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * trigger_eff_up / trigger_eff);
                h_ub_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * trigger_eff_down / trigger_eff);
                h_ub_puWeight_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * puWeightUp / puWeight);
                h_ub_puWeight_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * puWeightDown / puWeight);
                h_ub_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * PrefireWeightUp / PrefireWeight);
                h_ub_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * PrefireWeightDown / PrefireWeight);
                h_ub_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * pdfWeight_Up);
                h_ub_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * pdfWeight_Down);
                h_ub_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * SB_SF_up / SB_SF);
                h_ub_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * SB_SF_down / SB_SF);
                h_ub_eff_e_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Electron_SF_up / Electron_SF);
                h_ub_eff_e_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Electron_SF_down / Electron_SF);
                h_ub_err_mu_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_ub_err_mu_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_ub_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_ub_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_ub_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_densetoptag_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_densetoptag_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_ak8veto_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_ub_ak8veto_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_ub_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_ub_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_ub_LHEScaleWeight_0->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[0]);
                h_ub_LHEScaleWeight_1->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[1]);
                h_ub_LHEScaleWeight_3->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[3]);
                h_ub_LHEScaleWeight_4->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[4]);
                h_ub_LHEScaleWeight_5->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[5]);
                h_ub_LHEScaleWeight_7->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[7]);
                h_ub_LHEScaleWeight_8->Fill(bin_num, SF * highDMevtWeight * Electron_SF * Muon_MediumSF * LHEScaleWeight[8]);
                h_ub_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_ub_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_ub_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_ub_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_ub_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_ub_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_ub_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
        }
        if(Unit_highdm_JESUp)
        {
            //bin_num_JESUp = UBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            //bin_num_JESUp = nCRUnitHighDM;
            //bin_num_JESUp = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = lepCRunit(Pass_Baseline_JESUp,false,Pass_LLCR_JESUp,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_ub_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight_JESUp * topw_sf * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_highdm_JESDown)
        {
            //bin_num_JESDown = UBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            //bin_num_JESDown = nCRUnitHighDM;
            //bin_num_JESDown = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = lepCRunit(Pass_Baseline_JESDown,false,Pass_LLCR_JESDown,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_ub_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight_JESDown * topw_sf * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(3.,sign);
            }
        }
        if(Unit_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRUnitHighDM;
            //bin_num_METUnClustUp = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = lepCRunit(Pass_Baseline_METUnClustUp,false,Pass_LLCR_METUnClustUp,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight_METUnClustUp * topw_sf * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(2.,sign);
            }
        }
        if(Unit_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRUnitHighDM;
            //bin_num_METUnClustDown = getsearchbin.getUnitNumHighDM("unitCRNum",mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = lepCRunit(Pass_Baseline_METUnClustDown,false,Pass_LLCR_METUnClustDown,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_ub_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight_METUnClustDown * topw_sf * Electron_SF * Muon_MediumSF * Tau_MediumSF);
                eff_h->Fill(3.,sign);
            }
        }

        //QCD CR
        if(verbose) std::cout << "QCD CR Bins" << std::endl;
        //int QCDCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
        if(verbose) std::cout << "Pass QCD CR" << std::endl;
        bool QCD_lowdm = tr.getVar<bool>("Pass_QCDCR_lowDM");
        bool QCD_highdm = tr.getVar<bool>("Pass_QCDCR_highDM");
        bool QCD_lowdm_JESUp = tr.getVar<bool>("Pass_QCDCR_lowDM_JESUp");
        bool QCD_highdm_JESUp = tr.getVar<bool>("Pass_QCDCR_highDM_JESUp");
        bool QCD_lowdm_JESDown = tr.getVar<bool>("Pass_QCDCR_lowDM_JESDown");
        bool QCD_highdm_JESDown = tr.getVar<bool>("Pass_QCDCR_highDM_JESDown");
        bool QCD_lowdm_METUnClustUp = tr.getVar<bool>("Pass_QCDCR_lowDM_METUnClustUp");
        bool QCD_highdm_METUnClustUp = tr.getVar<bool>("Pass_QCDCR_highDM_METUnClustUp");
        bool QCD_lowdm_METUnClustDown = tr.getVar<bool>("Pass_QCDCR_lowDM_METUnClustDown");
        bool QCD_highdm_METUnClustDown = tr.getVar<bool>("Pass_QCDCR_highDM_METUnClustDown");
        //need dphi of jets 1,2,3 < 0.1,0.1,0.1
        if(verbose) std::cout << "Build QCD METunc" << std::endl;
        bool Pass_dphiQCD = true;
        auto Jet_phi = tr.getVec<float>("Jet_phi");
        if(Jet_phi.size() == 1 && Jet_phi[0] > 0.1) Pass_dphiQCD = false;
        else if(Jet_phi.size() == 2 && (Jet_phi[0] > 0.1 || Jet_phi[1] > 0.1)) Pass_dphiQCD = false;
        else if(Jet_phi.size() > 2 && (Jet_phi[0] > 0.1 || Jet_phi[1] > 0.1 || Jet_phi[2] > 0.1)) Pass_dphiQCD = false;

        bin_num = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,Pass_HighDM,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
        if(bin_num == -1)
        {
                h_qb->Fill(bin_num, SF * highDMevtWeight);
                h_qb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_qb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_qb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_qb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_qb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_qb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_qb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
        }

        if(verbose) std::cout << "QCD CR Low DM" << std::endl;
        //int UB_lowdm(int njets, int nb, int nSV, float ISRpt, float bottompt_scalar_sum, float met)
        if(QCD_lowdm)
        {
            //bin_num = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met);
            //bin_num = nCRQCDLowDM;
            //int QCDCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
            bin_num = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,false,Pass_LowDM,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "Low DM UB num: " << bin_num << std::endl;
                h_qb->Fill(bin_num,SF*lowDMevtWeight);
                h_qb_bsf_up->Fill(bin_num, SF * lowDMevtWeight * B_SF_Up / B_SF);
                h_qb_bsf_down->Fill(bin_num, SF * lowDMevtWeight * B_SF_Down / B_SF);
                h_qb_bsf_light_up->Fill(bin_num, lowDMevtWeight * B_SF_Light_Up / B_SF_Light);
                h_qb_bsf_light_down->Fill(bin_num, lowDMevtWeight * B_SF_Light_Down / B_SF_Light);
                h_qb_bsf_heavy_up->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Up / B_SF_Heavy);
                h_qb_bsf_heavy_down->Fill(bin_num, lowDMevtWeight * B_SF_Heavy_Down / B_SF_Heavy);
                h_qb_trig_eff_up->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_up / trigger_eff);
                h_qb_trig_eff_down->Fill(bin_num, SF * lowDMevtWeight * trigger_eff_down / trigger_eff);
                h_qb_puWeight_up->Fill(bin_num, SF * lowDMevtWeight * puWeightUp / puWeight);
                h_qb_puWeight_down->Fill(bin_num, SF * lowDMevtWeight * puWeightDown / puWeight);
                h_qb_PFWeight_up->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightUp / PrefireWeight);
                h_qb_PFWeight_down->Fill(bin_num, SF * lowDMevtWeight * PrefireWeightDown / PrefireWeight);
                h_qb_pdfWeight_up->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Up);
                h_qb_pdfWeight_down->Fill(bin_num, SF * lowDMevtWeight * pdfWeight_Down);
                h_qb_ivfunc_up->Fill(bin_num, SF * lowDMevtWeight * SB_SF_up / SB_SF);
                h_qb_ivfunc_down->Fill(bin_num, SF * lowDMevtWeight * SB_SF_down / SB_SF);
                h_qb_eff_e_up->Fill(bin_num, SF * lowDMevtWeight * Electron_SF_up / Electron_SF);
                h_qb_eff_e_down->Fill(bin_num, SF * lowDMevtWeight * Electron_SF_down / Electron_SF);
                h_qb_err_mu_up->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_up / Muon_MediumSF);
                h_qb_err_mu_down->Fill(bin_num, SF * lowDMevtWeight * Muon_MediumSF_down / Muon_MediumSF);
                h_qb_eff_tau_up->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Up / Tau_MediumSF);
                h_qb_eff_tau_down->Fill(bin_num, SF * lowDMevtWeight * Tau_MediumSF_Down / Tau_MediumSF);
                h_qb_eff_wtag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_wtag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_toptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_toptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_densetoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_densetoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_ak8veto_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_qb_ak8veto_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_restoptag_up->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_qb_eff_restoptag_down->Fill(bin_num, SF * lowDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_qb_LHEScaleWeight_0->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[0]);
                h_qb_LHEScaleWeight_1->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[1]);
                h_qb_LHEScaleWeight_3->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[3]);
                h_qb_LHEScaleWeight_4->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[4]);
                h_qb_LHEScaleWeight_5->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[5]);
                h_qb_LHEScaleWeight_7->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[7]);
                h_qb_LHEScaleWeight_8->Fill(bin_num, SF * lowDMevtWeight * LHEScaleWeight[8]);
                h_qb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_qb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_qb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_qb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_qb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_qb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_qb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
        }
        if(QCD_lowdm_JESUp)
        {
            //bin_num_JESUp = UB_lowdm(njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            //bin_num_JESUp = nCRQCDLowDM;
            //bin_num_JESUp = getsearchbin.getQCDNumLowDM("unitCRNum",njets_JESUp,nb_JESUp,nSV_JESUp,ISRpt_JESUp,ptb_JESUp,met_JESUp);
            bin_num_JESUp = QCDCRunit(Pass_Baseline_JESUp,Pass_QCDCR_JESUp,false,false,false,Pass_LowDM_JESUp,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                h_qb_JES_up->Fill(bin_num_JESUp,SF*lowDMevtWeight_JESUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_lowdm_JESDown)
        {
            //bin_num_JESDown = UB_lowdm(njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            //bin_num_JESDown = nCRQCDLowDM;
            //bin_num_JESDown = getsearchbin.getQCDNumLowDM("unitCRNum",njets_JESDown,nb_JESDown,nSV_JESDown,ISRpt_JESDown,ptb_JESDown,met_JESDown);
            bin_num_JESDown = QCDCRunit(Pass_Baseline_JESDown,Pass_QCDCR_JESDown,false,false,false,Pass_LowDM_JESDown,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                h_qb_JES_down->Fill(bin_num_JESDown,SF*lowDMevtWeight_JESDown);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_lowdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRQCDLowDM;
            //bin_num_METUnClustUp = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustUp);
            bin_num_METUnClustUp = QCDCRunit(Pass_Baseline_METUnClustUp,Pass_QCDCR_METUnClustUp,false,false,false,Pass_LowDM_METUnClustUp,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                h_qb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*lowDMevtWeight_METUnClustUp);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_lowdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UB_lowdm(njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRQCDLowDM;
            //bin_num_METUnClustDown = getsearchbin.getQCDNumLowDM("unitCRNum",njets,nb,nSV,ISRpt,ptb,met_METUnClustDown);
            bin_num_METUnClustDown = QCDCRunit(Pass_Baseline_METUnClustDown,Pass_QCDCR_METUnClustDown,false,false,false,Pass_LowDM_METUnClustDown,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                h_qb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*lowDMevtWeight_METUnClustDown);
                eff_h->Fill(3.,sign);
            }
        }

        //int UBv4_highdm(float mtb, int njets, int nb, int ntop, int nw, int nres, float ht, float met)
        if(verbose) std::cout << "QCD CR High DM" << std::endl;
        if(QCD_highdm)
        {
            //bin_num = UBv4_highdm(mtb,njets,nb,ntop_merge,nw,ntop_res,HT,met);
            //bin_num = nCRQCDHighDM;
            bin_num = QCDCRunit(Pass_Baseline,Pass_QCDCR,false,false,Pass_HighDM,false,nb,mtb,ptb,met,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num != -1)
            {
                //std::cout << "High DM UB num: " << bin_num << std::endl;
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb->Fill(bin_num,SF*highDMevtWeight * topw_sf);
                h_qb_bsf_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Up / B_SF);
                h_qb_bsf_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * B_SF_Down / B_SF);
                h_qb_bsf_light_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Up / B_SF_Light);
                h_qb_bsf_light_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Light_Down / B_SF_Light);
                h_qb_bsf_heavy_up->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Up / B_SF_Heavy);
                h_qb_bsf_heavy_down->Fill(bin_num, highDMevtWeight * topw_sf * B_SF_Heavy_Down / B_SF_Heavy);
                h_qb_trig_eff_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_up / trigger_eff);
                h_qb_trig_eff_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * trigger_eff_down / trigger_eff);
                h_qb_puWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightUp / puWeight);
                h_qb_puWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * puWeightDown / puWeight);
                h_qb_PFWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightUp / PrefireWeight);
                h_qb_PFWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * PrefireWeightDown / PrefireWeight);
                h_qb_pdfWeight_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Up);
                h_qb_pdfWeight_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * pdfWeight_Down);
                h_qb_ivfunc_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_up / SB_SF);
                h_qb_ivfunc_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * SB_SF_down / SB_SF);
                h_qb_eff_e_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_SF_up / Electron_SF);
                h_qb_eff_e_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Electron_SF_down / Electron_SF);
                h_qb_err_mu_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_up / Muon_MediumSF);
                h_qb_err_mu_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Muon_MediumSF_down / Muon_MediumSF);
                h_qb_eff_tau_up->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Up / Tau_MediumSF);
                h_qb_eff_tau_down->Fill(bin_num, SF * highDMevtWeight * topw_sf * Tau_MediumSF_Down / Tau_MediumSF);
                h_qb_eff_wtag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_w_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_wtag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_w_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_toptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_top_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_toptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_top_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_densetoptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_up / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_densetoptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_densetop_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_ak8veto_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_up / Stop0l_DeepAK8_SFWeight);
                h_qb_ak8veto_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_DeepAK8_SFWeight_veto_dn / Stop0l_DeepAK8_SFWeight);
                h_qb_eff_restoptag_up->Fill(bin_num, SF * highDMevtWeight * Stop0l_ResTopWeight_Up / Stop0l_ResTopWeight);
                h_qb_eff_restoptag_down->Fill(bin_num, SF * highDMevtWeight * Stop0l_ResTopWeight_Dn / Stop0l_ResTopWeight);
                h_qb_LHEScaleWeight_0->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[0]);
                h_qb_LHEScaleWeight_1->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[1]);
                h_qb_LHEScaleWeight_3->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[3]);
                h_qb_LHEScaleWeight_4->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[4]);
                h_qb_LHEScaleWeight_5->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[5]);
                h_qb_LHEScaleWeight_7->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[7]);
                h_qb_LHEScaleWeight_8->Fill(bin_num, SF * highDMevtWeight * LHEScaleWeight[8]);
                h_qb_LHENoOtherWeights_0->Fill(bin_num, nosplit_lumi * LHEScaleWeight[0]);
                h_qb_LHENoOtherWeights_1->Fill(bin_num, nosplit_lumi * LHEScaleWeight[1]);
                h_qb_LHENoOtherWeights_3->Fill(bin_num, nosplit_lumi * LHEScaleWeight[3]);
                h_qb_LHENoOtherWeights_4->Fill(bin_num, nosplit_lumi * LHEScaleWeight[4]);
                h_qb_LHENoOtherWeights_5->Fill(bin_num, nosplit_lumi * LHEScaleWeight[5]);
                h_qb_LHENoOtherWeights_7->Fill(bin_num, nosplit_lumi * LHEScaleWeight[7]);
                h_qb_LHENoOtherWeights_8->Fill(bin_num, nosplit_lumi * LHEScaleWeight[8]);
                eff_h->Fill(1.,sign);
            }
        }
        if(QCD_highdm_JESUp)
        {
            //bin_num_JESUp = UBv4_highdm(mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            //bin_num_JESUp = nCRQCDHighDM;
            //bin_num_JESUp = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_JESUp,njets_JESUp,nb_JESUp,ntop_merge_JESUp,nw_JESUp,ntop_res_JESUp,HT_JESUp,met_JESUp);
            bin_num_JESUp = QCDCRunit(Pass_Baseline_JESUp,Pass_QCDCR_JESUp,false,false,Pass_HighDM_JESUp,false,nb_JESUp,mtb_JESUp,ptb_JESUp,met_JESUp,nSV_JESUp,njets_JESUp,ISRpt_JESUp,HT_JESUp,ntop_res_JESUp,ntop_merge_JESUp,nw_JESUp);
            if(bin_num_JESUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESUp != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESUp != 0) topw_sf *= ResTop_SF;
                if(nw_JESUp != 0) topw_sf *= W_SF;
                h_qb_JES_up->Fill(bin_num_JESUp,SF*highDMevtWeight_JESUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_highdm_JESDown)
        {
            //bin_num_JESDown = UBv4_highdm(mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            //bin_num_JESDown = nCRQCDHighDM;
            //bin_num_JESDown = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_JESDown,njets_JESDown,nb_JESDown,ntop_merge_JESDown,nw_JESDown,ntop_res_JESDown,HT_JESDown,met_JESDown);
            bin_num_JESDown = QCDCRunit(Pass_Baseline_JESDown,Pass_QCDCR_JESDown,false,false,Pass_HighDM_JESDown,false,nb_JESDown,mtb_JESDown,ptb_JESDown,met_JESDown,nSV_JESDown,njets_JESDown,ISRpt_JESDown,HT_JESDown,ntop_res_JESDown,ntop_merge_JESDown,nw_JESDown);
            if(bin_num_JESDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge_JESDown != 0) topw_sf *= MergedTop_SF;
                if(ntop_res_JESDown != 0) topw_sf *= ResTop_SF;
                if(nw_JESDown != 0) topw_sf *= W_SF;
                h_qb_JES_down->Fill(bin_num_JESDown,SF*highDMevtWeight_JESDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(QCD_highdm_METUnClustUp)
        {
            //bin_num_METUnClustUp = UBv4_highdm(mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            //bin_num_METUnClustUp = nCRQCDHighDM;
            //bin_num_METUnClustUp = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_METUnClustUp,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustUp);
            bin_num_METUnClustUp = QCDCRunit(Pass_Baseline_METUnClustUp,Pass_QCDCR_METUnClustUp,false,false,Pass_HighDM_METUnClustUp,false,nb,mtb_METUnClustUp,ptb,met_METUnClustUp,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustUp != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METUnClust_up->Fill(bin_num_METUnClustUp,SF*highDMevtWeight_METUnClustUp * topw_sf);
                eff_h->Fill(2.,sign);
            }
        }
        if(QCD_highdm_METUnClustDown)
        {
            //bin_num_METUnClustDown = UBv4_highdm(mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            //bin_num_METUnClustDown = nCRQCDHighDM;
            //bin_num_METUnClustDown = getsearchbin.getQCDNumHighDM("unitCRNum",mtb_METUnClustDown,njets,nb,ntop_merge,nw,ntop_res,HT,met_METUnClustDown);
            bin_num_METUnClustDown = QCDCRunit(Pass_Baseline_METUnClustDown,Pass_QCDCR_METUnClustDown,false,false,Pass_HighDM_METUnClustDown,false,nb,mtb_METUnClustDown,ptb,met_METUnClustDown,nSV,njets,ISRpt,HT,ntop_res,ntop_merge,nw);
            if(bin_num_METUnClustDown != -1)
            {
                topw_sf = 1;
                if(ntop_merge != 0) topw_sf *= MergedTop_SF;
                if(ntop_res != 0) topw_sf *= ResTop_SF;
                if(nw != 0) topw_sf *= W_SF;
                h_qb_METUnClust_down->Fill(bin_num_METUnClustDown,SF*highDMevtWeight_METUnClustDown * topw_sf);
                eff_h->Fill(3.,sign);
            }
        }
        if(verbose) std::cout << "End of loop from event " << tr.getEvtNum() << std::endl;
    }

    if(verbose) std::cout << "Rescaling LHE histograms" << std::endl;
    h_vb_LHEScaleWeight_0->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_0->Integral(0, h_vb_LHENoOtherWeights_0->GetNbinsX() + 1));
    h_vb_LHEScaleWeight_1->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_1->Integral(0, h_vb_LHENoOtherWeights_1->GetNbinsX() + 1));
    h_vb_LHEScaleWeight_3->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_3->Integral(0, h_vb_LHENoOtherWeights_3->GetNbinsX() + 1));
    h_vb_LHEScaleWeight_5->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_5->Integral(0, h_vb_LHENoOtherWeights_5->GetNbinsX() + 1));
    h_vb_LHEScaleWeight_7->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_7->Integral(0, h_vb_LHENoOtherWeights_7->GetNbinsX() + 1));
    h_vb_LHEScaleWeight_8->Scale(h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_vb_LHENoOtherWeights_8->Integral(0, h_vb_LHENoOtherWeights_8->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_0->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_0->Integral(0, h_sb_LHENoOtherWeights_0->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_1->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_1->Integral(0, h_sb_LHENoOtherWeights_1->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_3->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_3->Integral(0, h_sb_LHENoOtherWeights_3->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_5->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_5->Integral(0, h_sb_LHENoOtherWeights_5->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_7->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_7->Integral(0, h_sb_LHENoOtherWeights_7->GetNbinsX() + 1));
    h_sb_LHEScaleWeight_8->Scale(h_sb_LHENoOtherWeights_4->Integral(0, h_sb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_sb_LHENoOtherWeights_8->Integral(0, h_sb_LHENoOtherWeights_8->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_0->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_0->Integral(0, h_ub_LHENoOtherWeights_0->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_1->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_1->Integral(0, h_ub_LHENoOtherWeights_1->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_3->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_3->Integral(0, h_ub_LHENoOtherWeights_3->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_5->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_5->Integral(0, h_ub_LHENoOtherWeights_5->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_7->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_7->Integral(0, h_ub_LHENoOtherWeights_7->GetNbinsX() + 1));
    h_ub_LHEScaleWeight_8->Scale(h_ub_LHENoOtherWeights_4->Integral(0, h_ub_LHENoOtherWeights_4->GetNbinsX() + 1) / h_ub_LHENoOtherWeights_8->Integral(0, h_ub_LHENoOtherWeights_8->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_0->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_0->Integral(0, h_qb_LHENoOtherWeights_0->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_1->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_1->Integral(0, h_qb_LHENoOtherWeights_1->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_3->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_3->Integral(0, h_qb_LHENoOtherWeights_3->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_5->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_5->Integral(0, h_qb_LHENoOtherWeights_5->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_7->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_7->Integral(0, h_qb_LHENoOtherWeights_7->GetNbinsX() + 1));
    h_qb_LHEScaleWeight_8->Scale(h_qb_LHENoOtherWeights_4->Integral(0, h_qb_LHENoOtherWeights_4->GetNbinsX() + 1) / h_qb_LHENoOtherWeights_8->Integral(0, h_qb_LHENoOtherWeights_8->GetNbinsX() + 1));

    //calculating lepton veto SFs: 1 + (elec_yield - scaled_elec_yield)/SB_yield. Hist is already the difference.
    if(verbose) std::cout << "Lepton Yields" << std::endl;
    h_vb_elecyield->Divide(h_vb);
    h_vb_elecyield_up->Divide(h_vb);
    h_vb_elecyield_down->Divide(h_vb);
    h_vb_muonyield->Divide(h_vb);
    h_vb_muonyield_up->Divide(h_vb);
    h_vb_muonyield_down->Divide(h_vb);
    h_vb_tauyield->Divide(h_vb);
    h_vb_tauyield_up->Divide(h_vb);
    h_vb_tauyield_down->Divide(h_vb);
    for(int i = 1; i < 44; i++)
    {
        if(!isfinite(h_vb_elecyield->GetBinContent(i))) h_vb_elecyield->SetBinContent(i,0);
        h_vb_elecyield->AddBinContent(i);
        if(!isfinite(h_vb_elecyield_up->GetBinContent(i))) h_vb_elecyield_up->SetBinContent(i,0);
        h_vb_elecyield_up->AddBinContent(i);
        if(!isfinite(h_vb_elecyield_down->GetBinContent(i))) h_vb_elecyield_down->SetBinContent(i,0);
        h_vb_elecyield_down->AddBinContent(i);
        if(!isfinite(h_vb_muonyield->GetBinContent(i))) h_vb_muonyield->SetBinContent(i,0);
        h_vb_muonyield->AddBinContent(i);
        if(!isfinite(h_vb_muonyield_up->GetBinContent(i))) h_vb_muonyield_up->SetBinContent(i,0);
        h_vb_muonyield_up->AddBinContent(i);
        if(!isfinite(h_vb_muonyield_down->GetBinContent(i))) h_vb_muonyield_down->SetBinContent(i,0);
        h_vb_muonyield_down->AddBinContent(i);
        if(!isfinite(h_vb_tauyield->GetBinContent(i))) h_vb_tauyield->SetBinContent(i,0);
        h_vb_tauyield->AddBinContent(i);
        if(!isfinite(h_vb_tauyield_up->GetBinContent(i))) h_vb_tauyield_up->SetBinContent(i,0);
        h_vb_tauyield_up->AddBinContent(i);
        if(!isfinite(h_vb_tauyield_down->GetBinContent(i))) h_vb_tauyield_down->SetBinContent(i,0);
        h_vb_tauyield_down->AddBinContent(i);
    }
    h_sb_elecyield->Divide(h_sb);
    h_sb_elecyield_up->Divide(h_sb);
    h_sb_elecyield_down->Divide(h_sb);
    h_sb_muonyield->Divide(h_sb);
    h_sb_muonyield_up->Divide(h_sb);
    h_sb_muonyield_down->Divide(h_sb);
    h_sb_tauyield->Divide(h_sb);
    h_sb_tauyield_up->Divide(h_sb);
    h_sb_tauyield_down->Divide(h_sb);
    for(int i = 1; i < 184; i++)
    {
        if(!isfinite(h_sb_elecyield->GetBinContent(i)))
        {
            h_sb_elecyield->SetBinContent(i,0);
            h_sb_elecyield->SetBinError(i,0.01);
        }
        h_sb_elecyield->AddBinContent(i);
        if(!isfinite(h_sb_elecyield_up->GetBinContent(i)))
        {
            h_sb_elecyield_up->SetBinContent(i,0);
            h_sb_elecyield_up->SetBinError(i,0.01);
        }
        h_sb_elecyield_up->AddBinContent(i);
        if(!isfinite(h_sb_elecyield_down->GetBinContent(i)))
        {
            h_sb_elecyield_down->SetBinContent(i,0);
            h_sb_elecyield_down->SetBinError(i,0.01);
        }
        h_sb_elecyield_down->AddBinContent(i);
        if(!isfinite(h_sb_muonyield->GetBinContent(i)))
        {
            h_sb_muonyield->SetBinContent(i,0);
            h_sb_muonyield->SetBinError(i,0.01);
        }
        h_sb_muonyield->AddBinContent(i);
        if(!isfinite(h_sb_muonyield_up->GetBinContent(i)))
        {
            h_sb_muonyield_up->SetBinContent(i,0);
            h_sb_muonyield_up->SetBinError(i,0.01);
        }
        h_sb_muonyield_up->AddBinContent(i);
        if(!isfinite(h_sb_muonyield_down->GetBinContent(i)))
        {
            h_sb_muonyield_down->SetBinContent(i,0);
            h_sb_muonyield_down->SetBinError(i,0.01);
        }
        h_sb_muonyield_down->AddBinContent(i);
        if(!isfinite(h_sb_tauyield->GetBinContent(i)))
        {
            h_sb_tauyield->SetBinContent(i,0);
            h_sb_tauyield->SetBinError(i,0.01);
        }
        h_sb_tauyield->AddBinContent(i);
        if(!isfinite(h_sb_tauyield_up->GetBinContent(i)))
        {
            h_sb_tauyield_up->SetBinContent(i,0);
            h_sb_tauyield_up->SetBinError(i,0.01);
        }
        h_sb_tauyield_up->AddBinContent(i);
        if(!isfinite(h_sb_tauyield_down->GetBinContent(i)))
        {
            h_sb_tauyield_down->SetBinContent(i,0);
            h_sb_tauyield_down->SetBinError(i,0.01);
        }
        h_sb_tauyield_down->AddBinContent(i);
    }
    
    h_vb_eff_e_up->Multiply(h_vb_elecyield_up);
    h_vb_eff_e_up->Multiply(h_vb_muonyield);
    h_vb_eff_e_up->Multiply(h_vb_tauyield);
    h_vb_eff_e_down->Multiply(h_vb_elecyield_down);
    h_vb_eff_e_down->Multiply(h_vb_muonyield);
    h_vb_eff_e_down->Multiply(h_vb_tauyield);
    h_vb_err_mu_up->Multiply(h_vb_muonyield_up);
    h_vb_err_mu_up->Multiply(h_vb_elecyield);
    h_vb_err_mu_up->Multiply(h_vb_tauyield);
    h_vb_err_mu_down->Multiply(h_vb_muonyield_down);
    h_vb_err_mu_down->Multiply(h_vb_elecyield);
    h_vb_err_mu_down->Multiply(h_vb_tauyield);
    h_vb_eff_tau_up->Multiply(h_vb_tauyield_up);
    h_vb_eff_tau_up->Multiply(h_vb_elecyield);
    h_vb_eff_tau_up->Multiply(h_vb_muonyield);
    h_vb_eff_tau_down->Multiply(h_vb_tauyield_down);
    h_vb_eff_tau_down->Multiply(h_vb_elecyield);
    h_vb_eff_tau_down->Multiply(h_vb_muonyield);
    auto *h_vb_lepyield = (TH1F*)h_vb_elecyield->Clone("h_vb_lepyield");
    h_vb_lepyield->Multiply(h_vb_muonyield);
    h_vb_lepyield->Multiply(h_vb_tauyield);
    h_vb_lepyield->SetBinContent(0,1.0);
    ///*
    h_vb->Multiply(h_vb_lepyield);
    h_vb_bsf_up->Multiply(h_vb_lepyield);
    h_vb_bsf_down->Multiply(h_vb_lepyield);
    h_vb_bsf_light_up->Multiply(h_vb_lepyield);
    h_vb_bsf_light_down->Multiply(h_vb_lepyield);
    h_vb_bsf_heavy_up->Multiply(h_vb_lepyield);
    h_vb_bsf_heavy_down->Multiply(h_vb_lepyield);
    h_vb_trig_eff_up->Multiply(h_vb_lepyield);
    h_vb_trig_eff_down->Multiply(h_vb_lepyield);
    h_vb_puWeight_up->Multiply(h_vb_lepyield);
    h_vb_puWeight_down->Multiply(h_vb_lepyield);
    h_vb_PFWeight_up->Multiply(h_vb_lepyield);
    h_vb_PFWeight_down->Multiply(h_vb_lepyield);
    h_vb_pdfWeight_up->Multiply(h_vb_lepyield);
    h_vb_pdfWeight_down->Multiply(h_vb_lepyield);
    h_vb_JES_up->Multiply(h_vb_lepyield);
    h_vb_JES_down->Multiply(h_vb_lepyield);
    h_vb_METUnClust_up->Multiply(h_vb_lepyield);
    h_vb_METUnClust_down->Multiply(h_vb_lepyield);
    h_vb_ivfunc_up->Multiply(h_vb_lepyield);
    h_vb_ivfunc_down->Multiply(h_vb_lepyield);
    h_vb_eff_e_up->Multiply(h_vb_lepyield);
    h_vb_eff_e_down->Multiply(h_vb_lepyield);
    h_vb_err_mu_up->Multiply(h_vb_lepyield);
    h_vb_err_mu_down->Multiply(h_vb_lepyield);
    h_vb_eff_tau_up->Multiply(h_vb_lepyield);
    h_vb_eff_tau_down->Multiply(h_vb_lepyield);
    h_vb_eff_wtag_up->Multiply(h_vb_lepyield);
    h_vb_eff_wtag_down->Multiply(h_vb_lepyield);
    h_vb_eff_toptag_up->Multiply(h_vb_lepyield);
    h_vb_eff_toptag_down->Multiply(h_vb_lepyield);
    h_vb_eff_densetoptag_up->Multiply(h_vb_lepyield);
    h_vb_eff_densetoptag_down->Multiply(h_vb_lepyield);
    h_vb_eff_restoptag_up->Multiply(h_vb_lepyield);
    h_vb_eff_restoptag_down->Multiply(h_vb_lepyield);
    h_vb_ak8veto_up->Multiply(h_vb_lepyield);
    h_vb_ak8veto_down->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_0->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_1->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_3->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_4->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_5->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_7->Multiply(h_vb_lepyield);
    h_vb_LHEScaleWeight_8->Multiply(h_vb_lepyield);
    //*/
    auto *h_sb_lepyield = (TH1F*)h_sb_elecyield->Clone("h_sb_lepyield");
    h_sb_lepyield->Multiply(h_sb_muonyield);
    h_sb_lepyield->Multiply(h_sb_tauyield);
    h_sb_lepyield->SetBinContent(0,1.0);
    h_sb_eff_e_up->Multiply(h_sb_elecyield_up);
    h_sb_eff_e_up->Multiply(h_sb_muonyield);
    h_sb_eff_e_up->Multiply(h_sb_tauyield);
    h_sb_eff_e_down->Multiply(h_sb_elecyield_down);
    h_sb_eff_e_down->Multiply(h_sb_muonyield);
    h_sb_eff_e_down->Multiply(h_sb_tauyield);
    h_sb_err_mu_up->Multiply(h_sb_muonyield_up);
    h_sb_err_mu_up->Multiply(h_sb_elecyield);
    h_sb_err_mu_up->Multiply(h_sb_tauyield);
    h_sb_err_mu_down->Multiply(h_sb_muonyield_down);
    h_sb_err_mu_down->Multiply(h_sb_elecyield);
    h_sb_err_mu_down->Multiply(h_sb_tauyield);
    h_sb_eff_tau_up->Multiply(h_sb_tauyield_up);
    h_sb_eff_tau_up->Multiply(h_sb_elecyield);
    h_sb_eff_tau_up->Multiply(h_sb_muonyield);
    h_sb_eff_tau_down->Multiply(h_sb_tauyield_down);
    h_sb_eff_tau_down->Multiply(h_sb_elecyield);
    h_sb_eff_tau_down->Multiply(h_sb_muonyield);
    ///*
    //std::cout << "SB yield bin 183 no lumi: " << h_sb->GetBinContent(183)/nosplit_lumi << std::endl;
    //std::cout << "SB yield bin 183: " << h_sb->GetBinContent(183) << std::endl;
    h_sb->Multiply(h_sb_lepyield);
    //std::cout << "SB yield bin 183: " << h_sb->GetBinContent(183) << std::endl;
    h_sb_bsf_up->Multiply(h_sb_lepyield);
    h_sb_bsf_down->Multiply(h_sb_lepyield);
    h_sb_bsf_light_up->Multiply(h_sb_lepyield);
    h_sb_bsf_light_down->Multiply(h_sb_lepyield);
    h_sb_bsf_heavy_up->Multiply(h_sb_lepyield);
    h_sb_bsf_heavy_down->Multiply(h_sb_lepyield);
    h_sb_trig_eff_up->Multiply(h_sb_lepyield);
    h_sb_trig_eff_down->Multiply(h_sb_lepyield);
    h_sb_puWeight_up->Multiply(h_sb_lepyield);
    h_sb_puWeight_down->Multiply(h_sb_lepyield);
    h_sb_PFWeight_up->Multiply(h_sb_lepyield);
    h_sb_PFWeight_down->Multiply(h_sb_lepyield);
    h_sb_pdfWeight_up->Multiply(h_sb_lepyield);
    h_sb_pdfWeight_down->Multiply(h_sb_lepyield);
    h_sb_JES_up->Multiply(h_sb_lepyield);
    h_sb_JES_down->Multiply(h_sb_lepyield);
    h_sb_METUnClust_up->Multiply(h_sb_lepyield);
    h_sb_METUnClust_down->Multiply(h_sb_lepyield);
    h_sb_ivfunc_up->Multiply(h_sb_lepyield);
    h_sb_ivfunc_down->Multiply(h_sb_lepyield);
    h_sb_eff_e_up->Multiply(h_sb_lepyield);
    h_sb_eff_e_down->Multiply(h_sb_lepyield);
    h_sb_err_mu_up->Multiply(h_sb_lepyield);
    h_sb_err_mu_down->Multiply(h_sb_lepyield);
    h_sb_eff_tau_up->Multiply(h_sb_lepyield);
    h_sb_eff_tau_down->Multiply(h_sb_lepyield);
    h_sb_eff_wtag_up->Multiply(h_sb_lepyield);
    h_sb_eff_wtag_down->Multiply(h_sb_lepyield);
    h_sb_eff_toptag_up->Multiply(h_sb_lepyield);
    h_sb_eff_toptag_down->Multiply(h_sb_lepyield);
    h_sb_eff_densetoptag_up->Multiply(h_sb_lepyield);
    h_sb_eff_densetoptag_down->Multiply(h_sb_lepyield);
    h_sb_eff_restoptag_up->Multiply(h_sb_lepyield);
    h_sb_eff_restoptag_down->Multiply(h_sb_lepyield);
    h_sb_ak8veto_up->Multiply(h_sb_lepyield);
    h_sb_ak8veto_down->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_0->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_1->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_3->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_4->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_5->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_7->Multiply(h_sb_lepyield);
    h_sb_LHEScaleWeight_8->Multiply(h_sb_lepyield);
    //*/

    if(verbose) std::cout << "Lumi" << std::endl;

    float all_events = eff_h->GetBinContent(1);
    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_50_2016") all_events = 2992924 - 2042;
    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_100_2016") all_events = 2638153 - 1924;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_50_2016") all_events = 2666897 - 1453;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_100_2016") all_events = 2638153 - 1924;

    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_50_2017") all_events = 2053015 - 1347;
    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_100_2017") all_events = 2026375 - 1370;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_50_2017") all_events = 1513435 - 807;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_100_2017") all_events = 1541563 - 1057;

    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_50_2018") all_events = 1590281 - 1067;
    if(samplename == "TTbarDMJets_Inclusive_pseudoscalar_LO_Mchi_1_Mphi_100_2018") all_events = 1606863 - 1129;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_50_2018") all_events = 1794634 - 976;
    if(samplename == "TTbarDMJets_Inclusive_scalar_LO_Mchi_1_Mphi_100_2018") all_events = 1930098 - 1365;

    float lumi = 1.0;
    if(era == "2016") lumi = 35.815165; //MET. SM 35.917149
    if(era == "2017")
    {
        if(PeriodF) lumi = 13.220313; //MET. SM 13.538559
        else lumi = 27.987721; //MET. SM 27.9865
    }
    if(era == "2018")
    {
        if(PostHEM) lumi = 38.630913; //MET. SM 38.65435
        else lumi = 21.068576; //MET. SM 21.071447
    }
    if(nosplit) lumi = 1.0;

    if(verbose) std::cout << "XS" << std::endl;

    /*
    std::string str_mass = "";
    TString histname = "";
    if(samplename.find("mGluino") != std::string::npos)
    {
        str_mass = samplename.substr(samplename.find("mGluino") + 7,samplename.find("_mLSP") - samplename.find("mGluino") - 7);
        histname = "gluino_xsection";
    }
    if(samplename.find("mStop") != std::string::npos)
    {
        str_mass = samplename.substr(samplename.find("mStop") + 5,samplename.find("_mLSP") - samplename.find("mStop") - 5);
        histname = "stop_xsection";
    }
    if(samplename.find("mSbot") != std::string::npos) str_mass = samplename.substr(samplename.find("mSbot") + 5,samplename.find("_mLSP") - samplename.find("mSbot") - 5);
    */
    std::string chimass = samplename.substr(samplename.find("Mchi_") + 5, samplename.find("_Mphi") - samplename.find("Mchi_") - 5);
    std::string phimass = samplename.substr(samplename.find("Mphi_") + 5, samplename.find("_" + era) - samplename.find("Mphi_") - 5);
    bool Dilepton = (samplename.find("Dilepton") != std::string::npos);
    bool pseudoscalar = (samplename.find("pseudoscalar") != std::string::npos);
    std::cout << samplename << " mChi: " << chimass << " mPhi: " << phimass;
    if(Dilepton) std::cout << " Dilepton";
    else std::cout << " Inclusive";
    if(pseudoscalar) std::cout << " pseudoscalar";
    else std::cout << " scalar";
    //endl after printing xs
    std::map<std::string,float> xs_dl_ps;
    xs_dl_ps["1_100"] = 0.0239975136;
    xs_dl_ps["1_150"] = 0.0155679408;
    xs_dl_ps["1_200"] = 0.01044406224;
    xs_dl_ps["1_250"] = 0.00720975168;
    xs_dl_ps["1_300"] = 0.00510603264;
    xs_dl_ps["1_350"] = 0.00294667632;
    xs_dl_ps["1_400"] = 0.0015221520000000001;
    xs_dl_ps["1_450"] = 0.001011653712;
    xs_dl_ps["1_500"] = 0.000713521872;
    xs_dl_ps["1_50"] = 0.03857868;
    xs_dl_ps["20_100"] = 0.0240814944;
    xs_dl_ps["30_100"] = 0.024039504;
    xs_dl_ps["40_100"] = 0.0241024896;
    xs_dl_ps["45_100"] = 0.0241024896;
    xs_dl_ps["49_100"] = 0.0241759728;
    xs_dl_ps["51_100"] = 0.00220764528;
    xs_dl_ps["55_100"] = 0.0008535598559999999;

    std::map<std::string,float> xs_dl_s;
    xs_dl_s["1_100"] = 0.0829940256;
    xs_dl_s["1_150"] = 0.0268843536;
    xs_dl_s["1_200"] = 0.0112744224;
    xs_dl_s["1_250"] = 0.00591329808;
    xs_dl_s["1_300"] = 0.0036374184;
    xs_dl_s["1_350"] = 0.00247533408;
    xs_dl_s["1_400"] = 0.00151900272;
    xs_dl_s["1_450"] = 0.0009804758399999999;
    xs_dl_s["1_500"] = 0.000672476256;
    xs_dl_s["1_50"] = 0.368255808;
    xs_dl_s["20_100"] = 0.0831829824;
    xs_dl_s["30_100"] = 0.0832669632;
    xs_dl_s["40_100"] = 0.0843692112;
    xs_dl_s["45_100"] = 0.08536648320000001;
    xs_dl_s["49_100"] = 0.0862692768;
    xs_dl_s["51_100"] = 0.000990343584;
    xs_dl_s["55_100"] = 0.00048708864;

    float xs = 1.0;
    if(pseudoscalar) xs = xs_dl_ps[chimass + "_" + phimass];
    else xs = xs_dl_s[chimass + "_" + phimass];
    if(!Dilepton) xs = xs / (3 * 0.108 * 3 * 0.108);
    std::cout << "; xs = " << xs << std::endl;

    /*
    //std::cout << str_mass << std::endl;
    //std::cout << "Era: " << era << "\tLumi: " << lumi << "\tstr_mass: " << str_mass << std::endl;
    //std::string str_mass = samplename.substr(samplename.find("m"),samplename.find("mLSP") - samplename.find("m"));
    int mass = std::stoi(str_mass);
    //std::cout << mass << std::endl;

    if(verbose) std::cout << "GenFilterEff" << std::endl;
    float genfiltereff = 1;
    if(denominator_genfiltereff != 0) genfiltereff = numerator_genfiltereff / denominator_genfiltereff;
    else std::cout << "Warning: SigWTab_SigWeightGenCut branch not found. denominator_genfiltereff = 0. Setting genfiltereff = 1." << std::endl;

    TFile *infile = new TFile("xSec.root");
    TH1F *xsection = (TH1F*) infile->Get(histname);
    float xs = xsection->GetBinContent(xsection->GetXaxis()->FindBin(mass));

    std::cout << "Era: " << era << "\tLumi: " << lumi << "\tMass: " << mass << "\txs: " << xs << "\tall_events: " << all_events << std::endl;
    //std::cout << "First few raw SB vals: " << h_sb_raw->GetBinContent(1) << " " << h_sb_raw->GetBinContent(2) << " " << h_sb_raw->GetBinContent(3) << std::endl;
    */
    float genfiltereff = 1;

    if(verbose) std::cout << "Scaling" << std::endl;

    h_njets_low_middphi->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_njets_high_middphi->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    MET->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    BTagWeight->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_light_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_light_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_heavy_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_bsf_heavy_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_densetoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_densetoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ak8veto_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_ak8veto_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_0->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_1->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_3->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_4->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_5->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_7->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_vb_LHEScaleWeight_8->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    //std::cout << "SB yield bin 183: " << h_sb->GetBinContent(183) << std::endl;
    h_sb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_light_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_light_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_heavy_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_bsf_heavy_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_densetoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_densetoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ak8veto_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_ak8veto_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_0->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_1->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_3->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_4->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_5->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_7->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_sb_LHEScaleWeight_8->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_light_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_light_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_heavy_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_bsf_heavy_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_densetoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_densetoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ak8veto_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_ak8veto_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_0->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_1->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_3->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_4->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_5->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_7->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_ub_LHEScaleWeight_8->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_light_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_light_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_heavy_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_bsf_heavy_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_trig_eff_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_trig_eff_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_puWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_puWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_PFWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_PFWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_pdfWeight_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_pdfWeight_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_JES_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_JES_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METUnClust_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_METUnClust_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ivfunc_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ivfunc_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_e_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_e_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_err_mu_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_err_mu_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_tau_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_tau_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_wtag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_wtag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_toptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_toptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_densetoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_densetoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_restoptag_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_eff_restoptag_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ak8veto_up->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_ak8veto_down->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_0->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_1->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_3->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_4->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_5->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_7->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    h_qb_LHEScaleWeight_8->Scale(lumi * xs * genfiltereff * 1000 / all_events);

    h_vb_LHENoOtherWeights_4->Scale(lumi * xs * genfiltereff * 1000 / all_events);
    std::cout << "Integral: " << h_vb_LHENoOtherWeights_4->Integral(0, h_vb_LHENoOtherWeights_4->GetNbinsX() + 1) << std::endl;

    TString outputfile = longsamplename + ".root";
    std::cout << "Outputting to file " << outputfile << std::endl;
    TFile out_file(outputfile,"RECREATE");
    out_file.mkdir("Baseline_Only");
    out_file.cd("Baseline_Only");
    if(verbose) std::cout << "Writing eff_h" << std::endl;
    eff_h->Write();
    h_weights->Write();
    out_file.cd("../");
    if(verbose) std::cout << "Writing the rest" << std::endl;
    h_njets_low_middphi->Write();
    h_njets_high_middphi->Write();
    MET->Write();
    BTagWeight->Write();
    PUWeight->Write();
    h_vb->Write();
    h_vb_bsf_up->Write();
    h_vb_bsf_down->Write();
    h_vb_bsf_light_up->Write();
    h_vb_bsf_light_down->Write();
    h_vb_bsf_heavy_up->Write();
    h_vb_bsf_heavy_down->Write();
    h_vb_trig_eff_up->Write();
    h_vb_trig_eff_down->Write();
    h_vb_puWeight_up->Write();
    h_vb_puWeight_down->Write();
    h_vb_PFWeight_up->Write();
    h_vb_PFWeight_down->Write();
    h_vb_pdfWeight_up->Write();
    h_vb_pdfWeight_down->Write();
    h_vb_JES_up->Write();
    h_vb_JES_down->Write();
    h_vb_METUnClust_up->Write();
    h_vb_METUnClust_down->Write();
    h_vb_ivfunc_up->Write();
    h_vb_ivfunc_down->Write();
    h_vb_eff_e_up->Write();
    h_vb_eff_e_down->Write();
    h_vb_err_mu_up->Write();
    h_vb_err_mu_down->Write();
    h_vb_eff_tau_up->Write();
    h_vb_eff_tau_down->Write();
    h_vb_eff_wtag_up->Write();
    h_vb_eff_wtag_down->Write();
    h_vb_eff_toptag_up->Write();
    h_vb_eff_toptag_down->Write();
    h_vb_eff_densetoptag_up->Write();
    h_vb_eff_densetoptag_down->Write();
    h_vb_eff_restoptag_up->Write();
    h_vb_eff_restoptag_down->Write();
    h_vb_ak8veto_up->Write();
    h_vb_ak8veto_down->Write();
    h_vb_lepyield->Write();
    h_vb_LHEScaleWeight_0->Write();
    h_vb_LHEScaleWeight_1->Write();
    h_vb_LHEScaleWeight_3->Write();
    h_vb_LHEScaleWeight_4->Write();
    h_vb_LHEScaleWeight_5->Write();
    h_vb_LHEScaleWeight_7->Write();
    h_vb_LHEScaleWeight_8->Write();
    h_sb->Write();
    h_sb_raw->Write();
    h_sb_bsf_up->Write();
    h_sb_bsf_down->Write();
    h_sb_bsf_light_up->Write();
    h_sb_bsf_light_down->Write();
    h_sb_bsf_heavy_up->Write();
    h_sb_bsf_heavy_down->Write();
    h_sb_trig_eff_up->Write();
    h_sb_trig_eff_down->Write();
    h_sb_puWeight_up->Write();
    h_sb_puWeight_down->Write();
    h_sb_PFWeight_up->Write();
    h_sb_PFWeight_down->Write();
    h_sb_pdfWeight_up->Write();
    h_sb_pdfWeight_down->Write();
    h_sb_JES_up->Write();
    h_sb_JES_down->Write();
    h_sb_METUnClust_up->Write();
    h_sb_METUnClust_down->Write();
    h_sb_ivfunc_up->Write();
    h_sb_ivfunc_down->Write();
    h_sb_eff_e_up->Write();
    h_sb_eff_e_down->Write();
    h_sb_err_mu_up->Write();
    h_sb_err_mu_down->Write();
    h_sb_eff_tau_up->Write();
    h_sb_eff_tau_down->Write();
    h_sb_eff_wtag_up->Write();
    h_sb_eff_wtag_down->Write();
    h_sb_eff_toptag_up->Write();
    h_sb_eff_toptag_down->Write();
    h_sb_eff_densetoptag_up->Write();
    h_sb_eff_densetoptag_down->Write();
    h_sb_eff_restoptag_up->Write();
    h_sb_eff_restoptag_down->Write();
    h_sb_ak8veto_up->Write();
    h_sb_ak8veto_down->Write();
    h_sb_lepyield->Write();
    h_sb_LHEScaleWeight_0->Write();
    h_sb_LHEScaleWeight_1->Write();
    h_sb_LHEScaleWeight_3->Write();
    h_sb_LHEScaleWeight_4->Write();
    h_sb_LHEScaleWeight_5->Write();
    h_sb_LHEScaleWeight_7->Write();
    h_sb_LHEScaleWeight_8->Write();
    h_ub->Write();
    h_ub_bsf_up->Write();
    h_ub_bsf_down->Write();
    h_ub_bsf_light_up->Write();
    h_ub_bsf_light_down->Write();
    h_ub_bsf_heavy_up->Write();
    h_ub_bsf_heavy_down->Write();
    h_ub_trig_eff_up->Write();
    h_ub_trig_eff_down->Write();
    h_ub_puWeight_up->Write();
    h_ub_puWeight_down->Write();
    h_ub_PFWeight_up->Write();
    h_ub_PFWeight_down->Write();
    h_ub_pdfWeight_up->Write();
    h_ub_pdfWeight_down->Write();
    h_ub_JES_up->Write();
    h_ub_JES_down->Write();
    h_ub_METUnClust_up->Write();
    h_ub_METUnClust_down->Write();
    h_ub_ivfunc_up->Write();
    h_ub_ivfunc_down->Write();
    h_ub_eff_e_up->Write();
    h_ub_eff_e_down->Write();
    h_ub_err_mu_up->Write();
    h_ub_err_mu_down->Write();
    h_ub_eff_tau_up->Write();
    h_ub_eff_tau_down->Write();
    h_ub_eff_wtag_up->Write();
    h_ub_eff_wtag_down->Write();
    h_ub_eff_toptag_up->Write();
    h_ub_eff_toptag_down->Write();
    h_ub_eff_densetoptag_up->Write();
    h_ub_eff_densetoptag_down->Write();
    h_ub_eff_restoptag_up->Write();
    h_ub_eff_restoptag_down->Write();
    h_ub_ak8veto_up->Write();
    h_ub_ak8veto_down->Write();
    h_ub_LHEScaleWeight_0->Write();
    h_ub_LHEScaleWeight_1->Write();
    h_ub_LHEScaleWeight_3->Write();
    h_ub_LHEScaleWeight_4->Write();
    h_ub_LHEScaleWeight_5->Write();
    h_ub_LHEScaleWeight_7->Write();
    h_ub_LHEScaleWeight_8->Write();
    h_qb->Write();
    h_qb_bsf_up->Write();
    h_qb_bsf_down->Write();
    h_qb_bsf_light_up->Write();
    h_qb_bsf_light_down->Write();
    h_qb_bsf_heavy_up->Write();
    h_qb_bsf_heavy_down->Write();
    h_qb_trig_eff_up->Write();
    h_qb_trig_eff_down->Write();
    h_qb_puWeight_up->Write();
    h_qb_puWeight_down->Write();
    h_qb_PFWeight_up->Write();
    h_qb_PFWeight_down->Write();
    h_qb_pdfWeight_up->Write();
    h_qb_pdfWeight_down->Write();
    h_qb_JES_up->Write();
    h_qb_JES_down->Write();
    h_qb_METUnClust_up->Write();
    h_qb_METUnClust_down->Write();
    h_qb_ivfunc_up->Write();
    h_qb_ivfunc_down->Write();
    h_qb_eff_e_up->Write();
    h_qb_eff_e_down->Write();
    h_qb_err_mu_up->Write();
    h_qb_err_mu_down->Write();
    h_qb_eff_tau_up->Write();
    h_qb_eff_tau_down->Write();
    h_qb_eff_wtag_up->Write();
    h_qb_eff_wtag_down->Write();
    h_qb_eff_toptag_up->Write();
    h_qb_eff_toptag_down->Write();
    h_qb_eff_densetoptag_up->Write();
    h_qb_eff_densetoptag_down->Write();
    h_qb_eff_restoptag_up->Write();
    h_qb_eff_restoptag_down->Write();
    h_qb_ak8veto_up->Write();
    h_qb_ak8veto_down->Write();
    h_qb_LHEScaleWeight_0->Write();
    h_qb_LHEScaleWeight_1->Write();
    h_qb_LHEScaleWeight_3->Write();
    h_qb_LHEScaleWeight_4->Write();
    h_qb_LHEScaleWeight_5->Write();
    h_qb_LHEScaleWeight_7->Write();
    h_qb_LHEScaleWeight_8->Write();
    if(verbose) std::cout << "Closing file" << std::endl;
    out_file.Close();

    if(verbose) std::cout << "Deleting histograms" << std::endl;
    delete eff_h;
    delete h_weights;
    delete h_njets_low_middphi;
    delete h_njets_high_middphi;
    delete MET;
    delete BTagWeight;
    delete PUWeight;
    delete h_vb;
    delete h_vb_bsf_up;
    delete h_vb_bsf_down;
    delete h_vb_bsf_light_up;
    delete h_vb_bsf_light_down;
    delete h_vb_bsf_heavy_up;
    delete h_vb_bsf_heavy_down;
    delete h_vb_trig_eff_up;
    delete h_vb_trig_eff_down;
    delete h_vb_puWeight_up;
    delete h_vb_puWeight_down;
    delete h_vb_PFWeight_up;
    delete h_vb_PFWeight_down;
    delete h_vb_pdfWeight_up;
    delete h_vb_pdfWeight_down;
    delete h_vb_JES_up;
    delete h_vb_JES_down;
    delete h_vb_METUnClust_up;
    delete h_vb_METUnClust_down;
    delete h_vb_ivfunc_up;
    delete h_vb_ivfunc_down;
    delete h_vb_eff_e_up;
    delete h_vb_eff_e_down;
    delete h_vb_err_mu_up;
    delete h_vb_err_mu_down;
    delete h_vb_eff_tau_up;
    delete h_vb_eff_tau_down;
    delete h_vb_eff_wtag_up;
    delete h_vb_eff_wtag_down;
    delete h_vb_eff_toptag_up;
    delete h_vb_eff_toptag_down;
    delete h_vb_eff_densetoptag_up;
    delete h_vb_eff_densetoptag_down;
    delete h_vb_eff_restoptag_up;
    delete h_vb_eff_restoptag_down;
    delete h_vb_ak8veto_up;
    delete h_vb_ak8veto_down;
    delete h_vb_LHEScaleWeight_0;
    delete h_vb_LHEScaleWeight_1;
    delete h_vb_LHEScaleWeight_3;
    delete h_vb_LHEScaleWeight_4;
    delete h_vb_LHEScaleWeight_5;
    delete h_vb_LHEScaleWeight_7;
    delete h_vb_LHEScaleWeight_8;
    delete h_vb_LHENoOtherWeights_0;
    delete h_vb_LHENoOtherWeights_1;
    delete h_vb_LHENoOtherWeights_3;
    delete h_vb_LHENoOtherWeights_4;
    delete h_vb_LHENoOtherWeights_5;
    delete h_vb_LHENoOtherWeights_7;
    delete h_vb_LHENoOtherWeights_8;
    delete h_vb_lepyield;
    delete h_vb_elecyield;
    delete h_vb_muonyield;
    delete h_vb_tauyield;
    delete h_sb;
    delete h_sb_bsf_up;
    delete h_sb_bsf_down;
    delete h_sb_bsf_light_up;
    delete h_sb_bsf_light_down;
    delete h_sb_bsf_heavy_up;
    delete h_sb_bsf_heavy_down;
    delete h_sb_trig_eff_up;
    delete h_sb_trig_eff_down;
    delete h_sb_puWeight_up;
    delete h_sb_puWeight_down;
    delete h_sb_PFWeight_up;
    delete h_sb_PFWeight_down;
    delete h_sb_pdfWeight_up;
    delete h_sb_pdfWeight_down;
    delete h_sb_JES_up;
    delete h_sb_JES_down;
    delete h_sb_METUnClust_up;
    delete h_sb_METUnClust_down;
    delete h_sb_ivfunc_up;
    delete h_sb_ivfunc_down;
    delete h_sb_eff_e_up;
    delete h_sb_eff_e_down;
    delete h_sb_err_mu_up;
    delete h_sb_err_mu_down;
    delete h_sb_eff_tau_up;
    delete h_sb_eff_tau_down;
    delete h_sb_eff_wtag_up;
    delete h_sb_eff_wtag_down;
    delete h_sb_eff_toptag_up;
    delete h_sb_eff_toptag_down;
    delete h_sb_eff_densetoptag_up;
    delete h_sb_eff_densetoptag_down;
    delete h_sb_eff_restoptag_up;
    delete h_sb_eff_restoptag_down;
    delete h_sb_ak8veto_up;
    delete h_sb_ak8veto_down;
    delete h_sb_LHEScaleWeight_0;
    delete h_sb_LHEScaleWeight_1;
    delete h_sb_LHEScaleWeight_3;
    delete h_sb_LHEScaleWeight_4;
    delete h_sb_LHEScaleWeight_5;
    delete h_sb_LHEScaleWeight_7;
    delete h_sb_LHEScaleWeight_8;
    delete h_sb_LHENoOtherWeights_0;
    delete h_sb_LHENoOtherWeights_1;
    delete h_sb_LHENoOtherWeights_3;
    delete h_sb_LHENoOtherWeights_4;
    delete h_sb_LHENoOtherWeights_5;
    delete h_sb_LHENoOtherWeights_7;
    delete h_sb_LHENoOtherWeights_8;
    delete h_sb_lepyield;
    delete h_sb_elecyield;
    delete h_sb_muonyield;
    delete h_sb_tauyield;
    delete h_ub;
    delete h_ub_bsf_up;
    delete h_ub_bsf_down;
    delete h_ub_bsf_light_up;
    delete h_ub_bsf_light_down;
    delete h_ub_bsf_heavy_up;
    delete h_ub_bsf_heavy_down;
    delete h_ub_trig_eff_up;
    delete h_ub_trig_eff_down;
    delete h_ub_puWeight_up;
    delete h_ub_puWeight_down;
    delete h_ub_PFWeight_up;
    delete h_ub_PFWeight_down;
    delete h_ub_pdfWeight_up;
    delete h_ub_pdfWeight_down;
    delete h_ub_JES_up;
    delete h_ub_JES_down;
    delete h_ub_METUnClust_up;
    delete h_ub_METUnClust_down;
    delete h_ub_ivfunc_up;
    delete h_ub_ivfunc_down;
    delete h_ub_eff_e_up;
    delete h_ub_eff_e_down;
    delete h_ub_err_mu_up;
    delete h_ub_err_mu_down;
    delete h_ub_eff_tau_up;
    delete h_ub_eff_tau_down;
    delete h_ub_eff_wtag_up;
    delete h_ub_eff_wtag_down;
    delete h_ub_eff_toptag_up;
    delete h_ub_eff_toptag_down;
    delete h_ub_eff_densetoptag_up;
    delete h_ub_eff_densetoptag_down;
    delete h_ub_eff_restoptag_up;
    delete h_ub_eff_restoptag_down;
    delete h_ub_ak8veto_up;
    delete h_ub_ak8veto_down;
    delete h_ub_LHEScaleWeight_0;
    delete h_ub_LHEScaleWeight_1;
    delete h_ub_LHEScaleWeight_3;
    delete h_ub_LHEScaleWeight_4;
    delete h_ub_LHEScaleWeight_5;
    delete h_ub_LHEScaleWeight_7;
    delete h_ub_LHEScaleWeight_8;
    delete h_ub_LHENoOtherWeights_0;
    delete h_ub_LHENoOtherWeights_1;
    delete h_ub_LHENoOtherWeights_3;
    delete h_ub_LHENoOtherWeights_4;
    delete h_ub_LHENoOtherWeights_5;
    delete h_ub_LHENoOtherWeights_7;
    delete h_ub_LHENoOtherWeights_8;
    delete h_qb;
    delete h_qb_bsf_up;
    delete h_qb_bsf_down;
    delete h_qb_bsf_light_up;
    delete h_qb_bsf_light_down;
    delete h_qb_bsf_heavy_up;
    delete h_qb_bsf_heavy_down;
    delete h_qb_trig_eff_up;
    delete h_qb_trig_eff_down;
    delete h_qb_puWeight_up;
    delete h_qb_puWeight_down;
    delete h_qb_PFWeight_up;
    delete h_qb_PFWeight_down;
    delete h_qb_pdfWeight_up;
    delete h_qb_pdfWeight_down;
    delete h_qb_JES_up;
    delete h_qb_JES_down;
    delete h_qb_METUnClust_up;
    delete h_qb_METUnClust_down;
    delete h_qb_ivfunc_up;
    delete h_qb_ivfunc_down;
    delete h_qb_eff_e_up;
    delete h_qb_eff_e_down;
    delete h_qb_err_mu_up;
    delete h_qb_err_mu_down;
    delete h_qb_eff_tau_up;
    delete h_qb_eff_tau_down;
    delete h_qb_eff_wtag_up;
    delete h_qb_eff_wtag_down;
    delete h_qb_eff_toptag_up;
    delete h_qb_eff_toptag_down;
    delete h_qb_eff_densetoptag_up;
    delete h_qb_eff_densetoptag_down;
    delete h_qb_eff_restoptag_up;
    delete h_qb_eff_restoptag_down;
    delete h_qb_LHEScaleWeight_0;
    delete h_qb_LHEScaleWeight_1;
    delete h_qb_LHEScaleWeight_3;
    delete h_qb_LHEScaleWeight_4;
    delete h_qb_LHEScaleWeight_5;
    delete h_qb_LHEScaleWeight_7;
    delete h_qb_LHEScaleWeight_8;
    delete h_qb_LHENoOtherWeights_0;
    delete h_qb_LHENoOtherWeights_1;
    delete h_qb_LHENoOtherWeights_3;
    delete h_qb_LHENoOtherWeights_4;
    delete h_qb_LHENoOtherWeights_5;
    delete h_qb_LHENoOtherWeights_7;
    delete h_qb_LHENoOtherWeights_8;
    delete h_qb_ak8veto_up;
    delete h_qb_ak8veto_down;

    if(verbose) std::cout << "End." << std::endl;
    return 0;

}

int main(int argc, char* argv[])
{
    //defaults
    std::string era = "2018";
    //float SF = 1.17; //for TTZToLLNuNu
    float SF = 1.0; //for everything else
    float SF_up = 1.0;
    float SF_down = 1.0;
    bool verbose = false;
    int max_events = -1;
    bool isData = false;
    bool PeriodF = false;
    bool PostHEM = false;
    bool isSignal = true;
    bool nosplit = true;
    bool doLHEScaleWeight = false;

    if (argc < 3)
    {
        std::cerr <<"Please give at least 2 arguments " << "File List " << "Name of output file" << std::endl;
        return -1;
    }
    const char *inputfilelist = argv[1];
    const char *unused        = argv[2];

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
            {"isSignal",    no_argument,        0,  'S'},
            {"doLHEScaleWeight",no_argument,    0,  'L'},
            {"PeriodF",     no_argument,        0,  'F'},
            {"PostHEM",     no_argument,        0,  'H'},
            {"verbose",     no_argument,        0,  'v'},
        };
        c = getopt_long(argc,argv,"e:m:dTSLFHv",long_options,NULL);
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
            case 'S':
                isSignal = true;
                break;
            case 'L':
                doLHEScaleWeight = true;
                break;
            case 'F':
                PeriodF = true;
                break;
            case 'H':
                PostHEM = true;
                break;
            case 'v':
                verbose = true;
                break;
        }
    }

    std::cout << "Era: " << era << "\tSF: " << SF << "\tisSignal: " << isSignal << "\tisData: " << isData << "\tdoLHEScaleWeight: " << doLHEScaleWeight << "\tPeriodF: " << PeriodF << "\tPostHEM: " << PostHEM << "\tMax events: " << max_events << std::endl;

    /*
    TChain *ch = new TChain("Events");

    std::string filename;
    std::ifstream bigfile;
    std::string samplename;

    bigfile.open (inputfilelist);

    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            ch->Add(filename.c_str());
            samplename = filename.substr(filename.find("SMS_"),filename.find(".root")-filename.find("SMS_"));
            if(samplename.find("/") != std::string::npos) samplename = samplename.substr(samplename.find("/") + 1);
            std::cout << samplename << std::endl;
        }
        bigfile.close();
    }
    */
    std::string filename;
    std::ifstream bigfile;
    bigfile.open (inputfilelist);
    if (bigfile.is_open())
    {
        while (getline(bigfile,filename))
        {
            std::cout << filename << std::endl;
            analyze(filename,era,max_events,isData,isSignal,PeriodF,PostHEM,SF,nosplit,verbose,doLHEScaleWeight);
        }
        bigfile.close();
    }
    return 0;
}

