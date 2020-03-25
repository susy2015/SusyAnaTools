//Tools Headers

#include "SusyAnaTools/Tools/SATException.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/samples.h"
#include "TopTagger/TopTagger/interface/TopTagger.h"
#include "TopTagger/TopTagger/interface/TopTaggerResults.h"
#include "TopTagger/TopTagger/interface/TopTaggerUtilities.h"

//ROOT headers
#include "TFile.h"
#include "TH2D.h"
#include "TString.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

//STL Headers
#include <iostream>
#include <string>
#include <vector>

const float ptBins[]  = {0,150,250,300,350,400,450,500,600,99999};
const int nPtBins = sizeof(ptBins)/sizeof(ptBins[0]) - 1;

const float ptBinsMerged[]  = {200,300,400,480,600,800,99999};
const int nPtBinsMerged = sizeof(ptBinsMerged)/sizeof(ptBinsMerged[0]) - 1;

const std::string spec = "tTagEff";

void stripRoot(std::string &path)
{
    int dot = path.rfind(".root");
    if (dot != std::string::npos)
    {
        path.resize(dot);
    }
}

// === Main Function ===================================================
int main(int argc, char* argv[])
{
    using namespace std;

    TChain *fChain = nullptr;

    const float topDiscCut = 0.92;

    if (argc < 6)
    {
    std::cerr <<"Please give 6 arguments "<<"[SubsampleName]"<< " [MaxEvent]"<< "[Startfile]"<<" [No. of Files to run]  [b discriminator cut] [sampleSet cfg file]" <<std::endl;
        std::cerr <<" Valid configurations are " << std::endl;
        std::cerr <<"./bTagEfficiencyCalc TTbar_HT_600to800_2016 1000 1 1 0.6321 sampleSets_PreProcessed_2016.cfg" << std::endl;
        return -1;
    }

    const char *subSampleName = argv[1];
    const char *maxevent = argv[2];

    // Change string arg to int
    int  maxEvent =  std::atoi(maxevent);
    int numFiles =  std::atoi(argv[3]);
    int startFile =  std::atoi(argv[4]);
    std::string taggerCfgDir(argv[5]);

    bool isCondor = std::atof(argv[6]);

    // Prepare file list and finalize it
    TString subSampleNameT = subSampleName;

    const std::string stFile = std::to_string(startFile);
    TString stFileT = stFile;
    TString specT = spec;


    TH1::AddDirectory(kFALSE);

    AnaSamples::SampleSet        ss(argv[6], argc >= 8);

    float ScaleMC = 1.;
    if(ss[subSampleName] != ss.null())
    {
        fChain = new TChain(ss[subSampleName].treePath.c_str());
        ss[subSampleName].addFilesToChain(fChain, startFile, numFiles);

        //hack for fastsim
        if(strstr(subSampleName, "fastsim") != nullptr)
        {
            std::string fname(fChain->GetFile()->GetName());
            size_t begin = fname.rfind("/") + 1;
            size_t end = fname.rfind("_Skim_");
            subSampleNameT = fname.substr(begin, end - begin);
        }
        
        ScaleMC = ss[subSampleName].getWeight();
    }
    else
    {
        cout << "SampleSet \"" << subSampleName << "\" not found" << endl;
        return 0;
    }

    TString FileName = subSampleNameT+"_"+spec+"_"+stFileT;
    TFile *f =  new TFile(FileName+".root", "RECREATE");

    if(isCondor)
    {
        std::cout << "Run on condor is true." << std::endl;
        char thistFile[128];
        //stripRoot(FileName);
        sprintf(thistFile, "%s_%s_%d.root", startFile);
        FileName = thistFile;
        std::cout << "Filename modified for use with condor: " << FileName << std::endl;
        //sampleloc = "condor";
    }
    else
    {
        std::cout << "Run on condor is false." << std::endl;
    }

    TDirectory* fdir = f->mkdir(subSampleNameT);
    fdir->cd();

    std::string taggerWD = taggerCfgDir;
    if(argc >= 8)
    {
        taggerWD = "";
    }

    TH1* n_res_sig =   new TH1D("n_res_sig_"+subSampleNameT, "resTag_Efficiency"+subSampleNameT, nPtBins, ptBins);
    TH1* n_res_bg =    new TH1D("n_res_bg_"+subSampleNameT, "notResTag_Efficiency"+subSampleNameT, nPtBins, ptBins);
    TH1* d_res_sig =   new TH1D("d_res_sig_"+subSampleNameT, "resTag_Efficiency"+subSampleNameT, nPtBins, ptBins);
    TH1* d_res_bg =    new TH1D("d_res_bg_"+subSampleNameT, "notResTag_Efficiency"+subSampleNameT, nPtBins, ptBins);

    TH1* n_merged_t_as_t =   new TH1D("n_merged_t_as_t_"+subSampleNameT,  "mergedTasT_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* n_merged_t_as_w =   new TH1D("n_merged_t_as_w_"+subSampleNameT,  "mergedTasW_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* n_merged_w_as_t =   new TH1D("n_merged_w_as_t_"+subSampleNameT,  "mergedWasT_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* n_merged_w_as_w =   new TH1D("n_merged_w_as_w_"+subSampleNameT,  "mergedWasW_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* n_merged_bg_as_t =  new TH1D("n_merged_bg_as_t_"+subSampleNameT, "mergedBgasT_Efficiency"+subSampleNameT, nPtBinsMerged, ptBinsMerged);
    TH1* n_merged_bg_as_w =  new TH1D("n_merged_bg_as_w_"+subSampleNameT, "mergedBgasW_Efficiency"+subSampleNameT, nPtBinsMerged, ptBinsMerged);
    TH1* d_merged_t =   new TH1D("d_merged_t_"+subSampleNameT,  "mergedT_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* d_merged_w =   new TH1D("d_merged_w_"+subSampleNameT,  "mergedW_Efficiency"+subSampleNameT,  nPtBinsMerged, ptBinsMerged);
    TH1* d_merged_bg =  new TH1D("d_merged_bg_"+subSampleNameT, "mergedBg_Efficiency"+subSampleNameT, nPtBinsMerged, ptBinsMerged);

    n_res_sig->GetXaxis()->SetTitle("p_{T} [GeV]");

    n_res_bg->GetXaxis()->SetTitle("p_{T} [GeV]");


    TopTagger tt("TopTagger.cfg", taggerWD);

    NTupleReader *tr = new NTupleReader(fChain, {"run"});

    // --- Analyse events --------------------------------------------

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

        // ----- AK4 Jets -----
        const auto& Jets_TLV = tr->getVec_LVFromNano<float>("Jet");
        const auto& Jets_deepCSV = tr->getVec<float>("Jet_btagDeepB");

        if(!tr->getVar<bool>("Pass_NJets30")) continue;

        // AK4 jet variables
        std::map<std::string, std::string> AK4Variables;
        //            name for top tagger                     name in ntuple
        AK4Variables["qgPtD"]                               = "Jet_qgptD";
        AK4Variables["qgAxis1"]                             = "Jet_qgAxis1";
        AK4Variables["qgAxis2"]                             = "Jet_qgAxis2";
        AK4Variables["recoJetschargedHadronEnergyFraction"] = "Jet_chHEF";
        AK4Variables["recoJetschargedEmEnergyFraction"]     = "Jet_chEmEF";
        AK4Variables["recoJetsneutralEmEnergyFraction"]     = "Jet_neEmEF";
        AK4Variables["recoJetsmuonEnergyFraction"]          = "Jet_muEF";
        AK4Variables["recoJetsHFHadronEnergyFraction"]      = "Jet_hfHadEF";
        AK4Variables["recoJetsHFEMEnergyFraction"]          = "Jet_hfEMEF";
        AK4Variables["recoJetsneutralEnergyFraction"]       = "Jet_neHEF";
        AK4Variables["PhotonEnergyFraction"]                = "Jet_phEF";
        AK4Variables["ElectronEnergyFraction"]              = "Jet_elEF";
        AK4Variables["ChargedHadronMultiplicity"]           = "Jet_chHadMult";
        AK4Variables["NeutralHadronMultiplicity"]           = "Jet_neHadMult";
        AK4Variables["PhotonMultiplicity"]                  = "Jet_phMult";
        AK4Variables["ElectronMultiplicity"]                = "Jet_elMult";
        AK4Variables["MuonMultiplicity"]                    = "Jet_muMult";
        AK4Variables["DeepCSVb"]                            = "Jet_deepCSVb";
        AK4Variables["DeepCSVc"]                            = "Jet_deepCSVc";
        AK4Variables["DeepCSVl"]                            = "Jet_deepCSVudsg";
        AK4Variables["DeepCSVbb"]                           = "Jet_deepCSVbb";

        const auto& GenPartTLV               = tr->getVec_LVFromNano<float>("GenPart"); 
        const auto& GenPart_pdgId            = tr->getVec<int>("GenPart_pdgId");
        const auto& GenPart_statusFlags      = tr->getVec<int>("GenPart_statusFlags");
        const auto& GenPart_genPartIdxMother = tr->getVec<int>("GenPart_genPartIdxMother");
        auto genMatchingInfo                 = ttUtility::GetTopdauGenLVecFromNano(GenPartTLV, GenPart_pdgId, GenPart_statusFlags, GenPart_genPartIdxMother);
        auto wGenMatchingInfo                = ttUtility::GetTopdauGenLVecFromNano(GenPartTLV, GenPart_pdgId, GenPart_statusFlags, GenPart_genPartIdxMother, 24, 2);
        std::vector<TLorentzVector> genTops(std::move(genMatchingInfo.first));
        std::vector<std::vector<const TLorentzVector*>> genTopDaughters(std::move(genMatchingInfo.second));

        //                                                                 the second Jets_deepCSV is a dummy and is unused in the algorithm 
        ttUtility::ConstAK4Inputs<float> AK4Inputs(Jets_TLV, Jets_deepCSV, Jets_deepCSV, genTops, genTopDaughters);

        std::vector<vector<float>> vvf;
        auto addInt = [&](const std::string& name, const std::string& var)
        {
            // convert qgMult to float and then add to AK4Inputs
            const std::vector<int>   &intVec = tr->getVec<int>(var);
            vvf.emplace_back(intVec.begin(), intVec.end());
            AK4Inputs.addSupplamentalVector(name, vvf.back());
        };

        addInt("qgMult", "Jet_qgMult");

        // loop over variables and add to AK4Inputs
        for (const auto& variable : AK4Variables)
        {
            //                              first: name for top tagger                          second: name in ntuple
            AK4Inputs.addSupplamentalVector(variable.first, tr->getVec<float>(variable.second));
        }

        //AK8 jets
        const auto& FatJet_LV              = tr->getVec_LVFromNano<float>("FatJet");
        const auto& FatJet_deepAK8_t       = tr->getVec<float>("FatJet_deepTag_TvsQCD");
        const auto& FatJet_deepAK8_w       = tr->getVec<float>("FatJet_deepTag_WvsQCD");
        const auto& FatJet_msoftdrop       = tr->getVec<float>("FatJet_msoftdrop");
        const auto& FatJet_subJetIdx1      = tr->getVec<int>("FatJet_subJetIdx1");
        const auto& FatJet_subJetIdx2      = tr->getVec<int>("FatJet_subJetIdx2");
        const auto& FatJet_Stop0l          = tr->getVec<int>("FatJet_Stop0l");
        std::vector<bool> FatJet_matchesPhoton;
        std::vector<bool> FatJet_matchesElectron;
        std::vector<bool> FatJet_matchesMuon;

        //AK8 subjets 
        const auto& SubJet_LV = tr->getVec_LVFromNano<float>("SubJet");

        //Correlate AK8 jets and their subjets
        unsigned int nFatJets = FatJet_LV.size();
        unsigned int nSubJets = SubJet_LV.size();
        std::vector<std::vector<TLorentzVector>> subjets(nFatJets);
        for(int i = 0; i < nFatJets; ++i)
        {
            if(FatJet_subJetIdx1[i] >= 0 && FatJet_subJetIdx1[i] < nSubJets) subjets[i].push_back(SubJet_LV[FatJet_subJetIdx1[i]]);
            if(FatJet_subJetIdx2[i] >= 0 && FatJet_subJetIdx2[i] < nSubJets) subjets[i].push_back(SubJet_LV[FatJet_subJetIdx2[i]]);
        }


        ttUtility::ConstAK8Inputs<float> ak8Inputs(FatJet_LV, FatJet_deepAK8_t, FatJet_deepAK8_w, FatJet_msoftdrop, subjets, genTops, genTopDaughters);

        std::vector<Constituent> constituents = ttUtility::packageConstituents(AK4Inputs);

        tt.runTagger(constituents);

        const auto& ttr = tt.getResults();

        const auto& tops = ttr.getTops();

        float evtWeight = tr->getVar<float>("genWeight");

        //resolved eff
        for(const auto& top : tops)
        {
            float pt = top->p().Pt();
            bool genMatched = top->getBestGenTopMatch() != nullptr;
            float disc = top->getDiscriminator();

            if(genMatched) // gen matched top quark
            {
                d_res_sig->Fill(pt, evtWeight);
                if(disc > topDiscCut) n_res_sig->Fill(pt, evtWeight);
            }
            else // not gen matched
            {
                d_res_bg->Fill(pt, evtWeight);
                if(disc > topDiscCut) n_res_bg->Fill(pt, evtWeight);
            }
        }

        //merged eff
        for(unsigned int iFJ = 0; iFJ < FatJet_LV.size(); ++iFJ)
        {
            if(FatJet_LV[iFJ].Pt() < 200 || FatJet_msoftdrop[iFJ] < 65) continue;

            int genMatch = 0;

            for(const auto genTop : genTopDaughters)
            {
                if(genTop.size() == 3)
                {
                    double dRMax = 0;
                    for(const auto* genPart : genTop)
                    {
                        double dR = ROOT::Math::VectorUtil::DeltaR(FatJet_LV[iFJ], *genPart);
                        if(dR > dRMax) dRMax = dR;
                    }
                    if(dRMax < 0.6) 
                    {
                        genMatch = 1;
                        break;
                    }
                }
            }

            if(genMatch == 0)
            {
                for(const auto genW : wGenMatchingInfo.second)
                {
                    if(genW.size() == 2)
                    {
                        double dRMax = 0;
                        for(const auto* genPart : genW)
                        {
                            double dR = ROOT::Math::VectorUtil::DeltaR(FatJet_LV[iFJ], *genPart);
                            if(dR > dRMax) dRMax = dR;
                        }
                        if(dRMax < 0.6) 
                        {
                            genMatch = 2;
                            break;
                        }
                    }
                }
            }

            if(genMatch == 1) // gen matched top quark
            {
                d_merged_t->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                if     (FatJet_Stop0l[iFJ] == 1) n_merged_t_as_t->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                else if(FatJet_Stop0l[iFJ] == 2) n_merged_t_as_w->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
            }
            else if(genMatch == 2) // gen matched W
            {
                d_merged_w->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                if     (FatJet_Stop0l[iFJ] == 1) n_merged_w_as_t->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                else if(FatJet_Stop0l[iFJ] == 2) n_merged_w_as_w->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
            }
            else // not gen matched 
            {
                d_merged_bg->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                if     (FatJet_Stop0l[iFJ] == 1) n_merged_bg_as_t->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
                else if(FatJet_Stop0l[iFJ] == 2) n_merged_bg_as_w->Fill(FatJet_LV[iFJ].Pt(), evtWeight);
            }
            
        }
    }

    /*************************************************************/
    // End of Event loop
    /*************************************************************/

    fdir->cd();
    d_res_sig->Write();
    d_res_bg->Write();

    n_res_sig->Write();
    n_res_bg->Write();

    n_merged_t_as_t->Write();
    n_merged_t_as_w->Write();
    n_merged_w_as_t->Write();
    n_merged_w_as_w->Write();
    n_merged_bg_as_t->Write();
    n_merged_bg_as_w->Write();
    d_merged_t->Write();
    d_merged_w->Write();
    d_merged_bg->Write();

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
