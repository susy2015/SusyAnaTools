#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <tuple>

#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1.h"

class TopWeightCalculator
{
private:
    const std::string sampleName_;
    const std::string year_;

    TH1* h_eff_t_as_t;
    TH1* h_eff_t_as_w;
    TH1* h_eff_w_as_t;
    TH1* h_eff_w_as_w;
    TH1* h_eff_bg_as_t;
    TH1* h_eff_bg_as_w;

    class SFEntry
    {
    public:
        float binLowEdge, binHighEdge, value, error;
    };

    std::tuple<float, float> getSFAndErr(const std::vector<SFEntry>& sfVec, float pt, int nGenPart, int recoTag, bool isFastSim = false)
    {
        for(const auto& entry : sfVec)
        {
            if(pt >= entry.binLowEdge && pt < entry.binHighEdge)
            {
                if(!isFastSim && nGenPart >= 4 && recoTag == 1) return std::make_tuple(entry.value, sqrt(entry.error*entry.error + 0.2*0.2));
                else                                            return std::make_tuple(entry.value, entry.error);
            }
        }

        return std::make_tuple(1.0, 0.0);
    }

    float getEff(TH1* h, float pt)
    {
        return h->GetBinContent(h->FindBin(pt));
    }

    std::map<std::string, std::vector<SFEntry>> DeepTop_SF;
    std::map<std::string, std::vector<SFEntry>> DeepTop_Fake_SF;
    std::map<std::string, std::vector<SFEntry>> DeepTop_fastSF;
    std::map<std::string, std::vector<SFEntry>> DeepW_SF;
    std::map<std::string, std::vector<SFEntry>> DeepW_Fake_SF;
    std::map<std::string, std::vector<SFEntry>> DeepW_fastSF;

    std::tuple<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>  calculateWeights(const std::vector<TLorentzVector>& FatJet_LV, const std::vector<int>& FatJet_genMatch, const std::vector<int>& FatJet_Stop0l, const std::vector<int>& FatJet_nGenPart)
    {
        float numerator = 1.0;
        float numerator_up = 1.0;
        float numerator_dn = 1.0;

        float numerator_t_up = 1.0;
        float numerator_t_dn = 1.0;

        float numerator_w_up = 1.0;
        float numerator_w_dn = 1.0;

        float numerator_v_up = 1.0;
        float numerator_v_dn = 1.0;

        float denominator = 1.0;

        float numerator_fast = 1.0;
        float numerator_fast_up = 1.0;
        float numerator_fast_dn = 1.0;

        float numerator_fast_t_up = 1.0;
        float numerator_fast_t_dn = 1.0;

        float numerator_fast_w_up = 1.0;
        float numerator_fast_w_dn = 1.0;

        float numerator_fast_v_up = 1.0;
        float numerator_fast_v_dn = 1.0;

        const auto& DeepTop_SF_era      = DeepTop_SF[year_];
        const auto& DeepTop_Fake_SF_era = DeepTop_Fake_SF[year_];
        const auto& DeepTop_fastSF_era  = DeepTop_fastSF[year_];
        const auto& DeepW_SF_era        = DeepW_SF[year_];
        const auto& DeepW_Fake_SF_era   = DeepW_Fake_SF[year_];
        const auto& DeepW_fastSF_era    = DeepW_fastSF[year_];

        for(int iFJ = 0; iFJ < FatJet_LV.size(); ++iFJ)
        {
            const auto& fjPt = FatJet_LV[iFJ].Pt();
            const int& genMatch = FatJet_genMatch[iFJ];
            const int& recoTag = FatJet_Stop0l[iFJ];
            const int& nGenPart = FatJet_nGenPart[iFJ];

            if(fjPt < 200.0) continue;

            if(recoTag == 1)
            {
                float SF = 1.0;
                float SFerr = 0.0;

                float SFfast = 1.0;
                float SFfasterr = 0.0;

                if(genMatch == 1) std::tie(SF, SFerr) = getSFAndErr(DeepTop_SF_era,      fjPt, nGenPart, recoTag);
                else              std::tie(SF, SFerr) = getSFAndErr(DeepTop_Fake_SF_era, fjPt, nGenPart, recoTag);

                std::tie(SFfast, SFfasterr) = getSFAndErr(DeepTop_fastSF_era, fjPt, nGenPart, recoTag, true);

                numerator *= SF;
                numerator_up *= SF+SFerr;
                numerator_dn *= SF-SFerr;

                numerator_t_up *= SF+SFerr;
                numerator_t_dn *= SF-SFerr;
                numerator_w_up *= SF;
                numerator_w_dn *= SF;
                numerator_v_up *= SF;
                numerator_v_dn *= SF;

                numerator_fast *= SF*SFfast;
                numerator_fast_up *= (SFfast+SFfasterr)*SF;
                numerator_fast_dn *= (SFfast-SFfasterr)*SF;

                numerator_fast_t_up *= (SFfast+SFfasterr)*SF;
                numerator_fast_t_dn *= (SFfast-SFfasterr)*SF;
                numerator_fast_w_up *= SF*SFfast;
                numerator_fast_w_dn *= SF*SFfast;
                numerator_fast_v_up *= SF*SFfast;
                numerator_fast_v_dn *= SF*SFfast;
            }
            else if(recoTag == 2)
            {
                float SF = 1.0;
                float SFerr = 0.0;

                float SFfast = 1.0;
                float SFfasterr = 0.0;

                if(genMatch == 2) std::tie(SF, SFerr) = getSFAndErr(DeepW_SF_era,      fjPt, nGenPart, recoTag);
                else              std::tie(SF, SFerr) = getSFAndErr(DeepW_Fake_SF_era, fjPt, nGenPart, recoTag);

                std::tie(SFfast, SFfasterr) = getSFAndErr(DeepW_fastSF_era, fjPt, nGenPart, recoTag, true);

                numerator *= SF;
                numerator_up *= SF+SFerr;
                numerator_dn *= SF-SFerr;

                numerator_t_up *= SF;
                numerator_t_dn *= SF;
                numerator_w_up *= SF+SFerr;
                numerator_w_dn *= SF-SFerr;
                numerator_v_up *= SF;
                numerator_v_dn *= SF;

                numerator_fast *= SF*SFfast;
                numerator_fast_up *= (SFfast+SFfasterr)*SF;
                numerator_fast_dn *= (SFfast-SFfasterr)*SF;

                numerator_fast_t_up *= SF*SFfast;
                numerator_fast_t_dn *= SF*SFfast;
                numerator_fast_w_up *= (SFfast+SFfasterr)*SF;
                numerator_fast_w_dn *= (SFfast-SFfasterr)*SF;
                numerator_fast_v_up *= SF*SFfast;
                numerator_fast_v_dn *= SF*SFfast;
            }
            else
            {
                float SF_t = 1.0;
                float SFerr_t = 0.0;
                float SF_w = 1.0;
                float SFerr_w = 0.0;

                float SFfast_t = 1.0;
                float SFfasterr_t = 0.0;
                float SFfast_w = 1.0;
                float SFfasterr_w = 0.0;

                float Eff_t = 0.0;
                float Eff_w = 0.0;

                if(genMatch == 1) std::tie(SF_t, SFerr_t) = getSFAndErr(DeepTop_SF_era,      fjPt, nGenPart, recoTag);
                else              std::tie(SF_t, SFerr_t) = getSFAndErr(DeepTop_Fake_SF_era, fjPt, nGenPart, recoTag);

                if(genMatch == 2) std::tie(SF_w, SFerr_w) = getSFAndErr(DeepW_SF_era,      fjPt, nGenPart, recoTag);
                else              std::tie(SF_w, SFerr_w) = getSFAndErr(DeepW_Fake_SF_era, fjPt, nGenPart, recoTag);

                std::tie(SFfast_t, SFfasterr_t) = getSFAndErr(DeepTop_fastSF_era, fjPt, nGenPart, recoTag, true);
                std::tie(SFfast_w, SFfasterr_w) = getSFAndErr(DeepW_fastSF_era, fjPt, nGenPart, recoTag, true);

                if(genMatch == 1)
                {
                    Eff_t = getEff(h_eff_t_as_t, fjPt);
                    Eff_w = getEff(h_eff_t_as_w, fjPt);
                }
                else if(genMatch == 2)
                {
                    Eff_t = getEff(h_eff_w_as_t, fjPt);
                    Eff_w = getEff(h_eff_w_as_w, fjPt);
                }
                else
                {
                    Eff_t = getEff(h_eff_bg_as_t, fjPt);
                    Eff_w = getEff(h_eff_bg_as_w, fjPt);
                }

                numerator *= 1 - SF_t*Eff_t - SF_w*Eff_w;
                numerator_up *= 1 - (SF_t+SFerr_t)*Eff_t - (SF_w+SFerr_w)*Eff_w;
                numerator_dn *= 1 - (SF_t-SFerr_t)*Eff_t - (SF_w-SFerr_w)*Eff_w;

                numerator_t_up *= 1 - SF_t*Eff_t - SF_w*Eff_w;
                numerator_t_dn *= 1 - SF_t*Eff_t - SF_w*Eff_w;
                numerator_w_up *= 1 - SF_t*Eff_t - SF_w*Eff_w;
                numerator_w_dn *= 1 - SF_t*Eff_t - SF_w*Eff_w;
                numerator_v_up *= 1 - (SF_t+SFerr_t)*Eff_t - (SF_w+SFerr_w)*Eff_w;
                numerator_v_dn *= 1 - (SF_t-SFerr_t)*Eff_t - (SF_w-SFerr_w)*Eff_w;

                numerator_fast *= 1 - SF_t*SFfast_t*Eff_t - SF_w*SFfast_w*Eff_w;
                numerator_fast_up *= 1 - (SFfast_t+SFfasterr_t)*SF_t*Eff_t - (SFfast_w+SFfasterr_w)*SF_w*Eff_w;
                numerator_fast_dn *= 1 - (SFfast_t-SFfasterr_t)*SF_t*Eff_t - (SFfast_w-SFfasterr_w)*SF_w*Eff_w;

                numerator_fast_t_up *= 1 - SF_t*SFfast_t*Eff_t - SF_w*SFfast_w*Eff_w;
                numerator_fast_t_dn *= 1 - SF_t*SFfast_t*Eff_t - SF_w*SFfast_w*Eff_w;
                numerator_fast_w_up *= 1 - SF_t*SFfast_t*Eff_t - SF_w*SFfast_w*Eff_w;
                numerator_fast_w_dn *= 1 - SF_t*SFfast_t*Eff_t - SF_w*SFfast_w*Eff_w;
                numerator_fast_v_up *= 1 - (SFfast_t+SFfasterr_t)*SF_t*Eff_t - (SFfast_w+SFfasterr_w)*SF_w*Eff_w;
                numerator_fast_v_dn *= 1 - (SFfast_t-SFfasterr_t)*SF_t*Eff_t - (SFfast_w-SFfasterr_w)*SF_w*Eff_w;

                denominator *= 1 - Eff_t - Eff_w;
            }

        }

        //safety just in case!
        if(denominator < 0.0001) denominator = 0.0001;

        return std::make_tuple(numerator/denominator, 
                               numerator_up/denominator,
                               numerator_dn/denominator, 
                               numerator_t_up/denominator, 
                               numerator_t_dn/denominator, 
                               numerator_w_up/denominator, 
                               numerator_w_dn/denominator, 
                               numerator_v_up/denominator, 
                               numerator_v_dn/denominator,
                               numerator_fast/denominator, 
                               numerator_fast_up/denominator,
                               numerator_fast_dn/denominator, 
                               numerator_fast_t_up/denominator, 
                               numerator_fast_t_dn/denominator, 
                               numerator_fast_w_up/denominator, 
                               numerator_fast_w_dn/denominator, 
                               numerator_fast_v_up/denominator, 
                               numerator_fast_v_dn/denominator);
    }

public:
    TopWeightCalculator(const std::string& effFileName, const std::string& sampleName, const std::string& year) : sampleName_(sampleName), year_(year)
    {
        TH1::AddDirectory(false);

        TFile *f = TFile::Open(effFileName.c_str());

        if(f)
        {

            h_eff_t_as_t = (TH1*)f->Get((sampleName + "/n_merged_t_as_t_" + sampleName).c_str());
            h_eff_t_as_w = (TH1*)f->Get((sampleName + "/n_merged_t_as_w_" + sampleName).c_str());
            h_eff_w_as_t = (TH1*)f->Get((sampleName + "/n_merged_w_as_t_" + sampleName).c_str());
            h_eff_w_as_w = (TH1*)f->Get((sampleName + "/n_merged_w_as_w_" + sampleName).c_str());
            h_eff_bg_as_t = (TH1*)f->Get((sampleName + "/n_merged_bg_as_t_" + sampleName).c_str());
            h_eff_bg_as_w = (TH1*)f->Get((sampleName + "/n_merged_bg_as_w_" + sampleName).c_str());
            TH1* h_d_t = (TH1*)f->Get((sampleName + "/d_merged_t_" + sampleName).c_str());
            TH1* h_d_w = (TH1*)f->Get((sampleName + "/d_merged_w_" + sampleName).c_str());
            TH1* h_d_bg = (TH1*)f->Get((sampleName + "/d_merged_bg_" + sampleName).c_str());

            f->Close();

            h_eff_t_as_t->Divide(h_d_t);
            h_eff_t_as_w->Divide(h_d_t);
            h_eff_w_as_t->Divide(h_d_w);
            h_eff_w_as_w->Divide(h_d_w);
            h_eff_bg_as_t->Divide(h_d_bg);
            h_eff_bg_as_w->Divide(h_d_bg);
        }
        else
        {
            std::cout << "Failed to load file " << effFileName << std::endl;
        }

        DeepTop_SF = {
            {"2016",
             {
                 {400, 480,  1.01, 0.11},
                 {480, 600,  1.05, 0.08},
                 {600, 9999, 1.06, 0.05},
             }
            },
            {"2017",
             {
                 {400, 480, 1.08, 0.10},
                 {480, 600, 0.97, 0.07},
                 {600, 9999, 1.02, 0.08},
             }
            },
            {"2018",
             {
                 {400, 480, 0.95, 0.07},
                 {480, 600, 1.06, 0.05},
                 {600, 9999, 0.94, 0.05},
             }
            }
        };
        //// from Stop1lep AN2019_003_v11, Table 39
        DeepTop_fastSF = {
            {"2016",
             {
                 {400,  480, 0.99, 0.06},
                 {480,  600, 0.96, 0.04},
                 {600,  9999, 0.92, 0.02},
             }
            },
            {"2017",
             {
                 {400,  480, 0.95, 0.04},
                 {480,  600, 1.00, 0.04},
                 {600,  9999, 0.96, 0.02},
             }
            },
            {"2018",
             {
                 {400, 480, 1.03, 0.06},
                 {480, 600, 1.03, 0.04},
                 {600, 9999, 1.03, 0.04},
             }
            }
        };

        DeepTop_Fake_SF = {
            {"2016",
             {
                 {400, 9999, 0.921, 0.072},
             }
            },
            {"2017",
             {
                 {400, 9999, 1.213, 0.079},
             }
            },
            {"2018",
             {
                 {400, 9999, 1.086, 0.069},
             }
            },
        };

        // Get it from https://indico.cern.ch/event/840827/contributions/3527925/attachments/1895214/3126510/DeepAK8_Top_W_SFs_2017_JMAR_PK.pdf
        // No direct value, get it from the png using http://www.graphreader.com/
        // Take it with a grain of salt
        // Use 1% mistag rate plots, taking the large side of asymmetric systematic as symmetric uncertainty
        // Using 2017 SF and uncertainty for 2018
        DeepW_SF = {
            {"2016" ,
             {
                 {200, 300, 0.946, 0.132},
                 {300, 400, 0.88, 0.107},
                 {400, 9999, 0.901, 0.103},
             }
            },
            {"2017" ,
             {
                 {200, 300, 0.941, 0.037},
                 {300, 400, 0.937, 0.078},
                 {400, 9999, 0.908, 0.115},
             }
            },
            {"2018" ,
             {
                 {200, 300, 0.941, 0.037},
                 {300, 400, 0.937, 0.078},
                 {400, 9999, 0.908, 0.115},
             }
            }
        };

        //// Fullsim/fastsim SF measured from
        // https://indico.cern.ch/event/877611/contributions/3762997/attachments/1992874/3326860/Zhenbin_WtaggerSF_v3.pdf
        DeepW_fastSF = {
            {"2016" ,
             {
                 {200, 300, 0.94, 0.05},
                 {300, 400, 0.99, 0.19},
                 {400, 9999, 1.08, 0.12},
             }
            },
            {"2017" ,
             {
                 {200, 300, 1.05, 0.02},
                 {300, 400, 1.12, 0.23},
                 {400, 9999, 1.05, 0.12},
             }
            },
            {"2018" ,
             {
                 {200, 300, 1.11, 0.09},
                 {300, 400, 1.04, 0.01},
                 {400, 9999, 1.16, 0.02},
             }
            }
        };

        DeepW_Fake_SF = {
            {"2016" ,
             {
                 {200, 300, 1.161, 0.059},
                 {300, 400, 1.178, 0.060},
                 {400, 9999, 1.314, 0.064},
             }
            },
            {"2017" ,
             {
                 {200, 300, 1.072, 0.073},
                 {300, 400, 1.132, 0.065},
                 {400, 9999, 1.159, 0.065},
             }
            },
            {"2018" ,
             {
                 {200, 300, 1.087, 0.065},
                 {300, 400, 1.058, 0.067},
                 {400, 9999, 1.139, 0.071},
             }
            }
        };
    }

    std::map<std::string, float> getTopWWeight(const std::vector<TLorentzVector>& FatJet_LV, const std::vector<int>& FatJet_genMatch, const std::vector<int>& FatJet_Stop0l, const std::vector<int>& FatJet_nGenPart)
    {
        auto weights = calculateWeights(FatJet_LV, FatJet_genMatch, FatJet_Stop0l, FatJet_nGenPart);
        return {{"weight", std::get<0>(weights)}, 
                {"weight_up", std::get<1>(weights)}, 
                {"weight_dn", std::get<2>(weights)},
                {"weight_t_up", std::get<3>(weights)},
                {"weight_t_dn", std::get<4>(weights)},
                {"weight_w_up", std::get<5>(weights)},
                {"weight_w_dn", std::get<6>(weights)},
                {"weight_v_up", std::get<7>(weights)},
                {"weight_v_dn", std::get<8>(weights)},
                {"weight_fast", std::get<9>(weights)}, 
                {"weight_fast_up", std::get<10>(weights)}, 
                {"weight_fast_dn", std::get<11>(weights)},
                {"weight_fast_t_up", std::get<12>(weights)},
                {"weight_fast_t_dn", std::get<13>(weights)},
                {"weight_fast_w_up", std::get<14>(weights)},
                {"weight_fast_w_dn", std::get<15>(weights)},
                {"weight_fast_v_up", std::get<16>(weights)},
                {"weight_fast_v_dn", std::get<17>(weights)}, };
    }

    template<typename NTupleReader>
    void operator()(NTupleReader& tr)
    {
        const std::vector<TLorentzVector>& FJ_tlv = tr.template getVec_LVFromNano<float>("FatJet");
        const std::vector<int>& FJ_genMatch       = tr.template getVec<int>("FatJet_GenMatch");
        const std::vector<int>& FJ_Stop0l         = tr.template getVec<int>("FatJet_Stop0l");
        const std::vector<int>& FJ_nGenPart       = tr.template getVec<int>("FatJet_nGenPart");
        
        float& weight           = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc");
        float& weight_up        = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_up");
        float& weight_dn        = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_total_dn");
        float& weight_t_up      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_up");
        float& weight_t_dn      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_top_dn");
        float& weight_w_up      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_up");
        float& weight_w_dn      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_w_dn");
        float& weight_v_up      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_up");
        float& weight_v_dn      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_recalc_veto_dn");
        float& weight_fast      = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc");
        float& weight_fast_up   = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_total_up");
        float& weight_fast_dn   = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_total_dn");
        float& weight_fast_t_up = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_top_up");
        float& weight_fast_t_dn = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_top_dn");
        float& weight_fast_w_up = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_w_up");
        float& weight_fast_w_dn = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_w_dn");
        float& weight_fast_v_up = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_veto_up");
        float& weight_fast_v_dn = tr.template createDerivedVar<float>("Stop0l_DeepAK8_SFWeight_fast_recalc_veto_dn");
        std::tie(weight, weight_up, weight_dn, weight_t_up, weight_t_dn, weight_w_up, weight_w_dn, weight_v_up, weight_v_dn, weight_fast, weight_fast_up, weight_fast_dn, weight_fast_t_up, weight_fast_t_dn, weight_fast_w_up, weight_fast_w_dn, weight_fast_v_up, weight_fast_v_dn) = calculateWeights(FJ_tlv, FJ_genMatch, FJ_Stop0l, FJ_nGenPart);
    }

};
