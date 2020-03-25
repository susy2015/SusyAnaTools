#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cmath>

#include "SB2018.h"
#include "units.hh"

int SRbin_hui(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    if(Pass_LowDM)
    {
        return SBv4_lowdm(njets, nb, nSoftB, ISRpt, ptb, MET);
    }
    else if(Pass_HighDM)
    {
        return SBv4_highdm(mtb, njets, nb, ntop, nw, nres, HT, MET);
    }
    
    return -1;
}

int SRbin_jon(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    const bool Pass_Baseline = true;
    const bool Pass_QCDCR = false;
    const bool Pass_LepCR = false;
    const bool Pass_PhoCR = false;
    
    return SRbin(Pass_Baseline, Pass_QCDCR, Pass_LepCR, Pass_PhoCR, Pass_HighDM, Pass_LowDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
}

int SRunit(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    const bool Pass_Baseline = true;
    const bool Pass_QCDCR = true;
    const bool Pass_LepCR = true;
    const bool Pass_PhoCR = true;
    
    return SRunit(Pass_Baseline, Pass_QCDCR, Pass_LepCR, Pass_PhoCR, Pass_HighDM, Pass_LowDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
}

int CRunit_lep(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    const bool Pass_Baseline = true;
    const bool Pass_QCDCR = true;
    const bool Pass_LepCR = true;
    const bool Pass_PhoCR = true;
    
    return lepCRunit(Pass_Baseline, Pass_QCDCR, Pass_LepCR, Pass_PhoCR, Pass_HighDM, Pass_LowDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
}

int CRunit_pho(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    const bool Pass_Baseline = true;
    const bool Pass_QCDCR = true;
    const bool Pass_LepCR = true;
    const bool Pass_PhoCR = true;
    
    return phoCRunit(Pass_Baseline, Pass_QCDCR, Pass_LepCR, Pass_PhoCR, Pass_HighDM, Pass_LowDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
}

int CRunit_qcd(bool Pass_LowDM, bool Pass_HighDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw)
{
    const bool Pass_Baseline = true;
    const bool Pass_QCDCR = true;
    const bool Pass_LepCR = true;
    const bool Pass_PhoCR = true;
    
    return QCDCRunit(Pass_Baseline, Pass_QCDCR, Pass_LepCR, Pass_PhoCR, Pass_HighDM, Pass_LowDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
}

int main()
{
    std::mt19937 generator(clock());
    std::uniform_int_distribution<int>    dist_nb(0, 3);
    std::uniform_int_distribution<int>    dist_nSB(0, 1);
    std::uniform_int_distribution<int>    dist_njets(2, 10);
    std::uniform_int_distribution<int>    dist_nres(0, 3);
    std::uniform_int_distribution<int>    dist_ntop(0, 3);
    std::uniform_int_distribution<int>    dist_nw(0, 3);

    std::uniform_real_distribution<float> dist_mtb(0, 500);
    std::uniform_real_distribution<float> dist_ptb(0, 150);
    std::uniform_real_distribution<float> dist_met(250, 1000);
    std::uniform_real_distribution<float> dist_isrpt(0, 700);
    std::uniform_real_distribution<float> dist_ht(300, 2000);


    for(int i = 0; i < 100000000; ++i)
    {
        int nb      = dist_nb(generator);
        float mtb   = dist_mtb(generator);
        float ptb   = dist_ptb(generator);
        float MET   = dist_met(generator);
        int nSoftB  = dist_nSB(generator);
        int njets   = dist_njets(generator);
        float ISRpt = dist_isrpt(generator);
        float HT    = dist_ht(generator);
        int nres    = dist_nres(generator);
        int ntop    = dist_ntop(generator);
        int nw      = dist_nw(generator);

        const bool Pass_LowDM = nres == 0 && ntop == 0 && nw == 0 && ISRpt >= 200 && (nb == 0 || mtb < 175);
        const bool Pass_HighDM = njets >= 5 && nb >= 1;

        int SB_Hui = SRbin_hui(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
        int SB_Jon = SRbin_jon(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
        int SR_unit = SRunit(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
        int CR_unit_lep = CRunit_lep(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
        int CR_unit_pho = CRunit_pho(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);
        int CR_unit_qcd = CRunit_qcd(Pass_LowDM, Pass_HighDM, nb, mtb, ptb, MET, nSoftB, njets, ISRpt, HT, nres, ntop, nw);

        if(SB_Hui != SB_Jon)
        {
            std::cout << "PANIC!!! Hui: " << SB_Hui << "\tJon/Matt: " << SB_Jon  << std::endl;
        }
        
        if(SB_Jon >= 0 && SR_unit >= 0 && !unitBinMapSR[SB_Jon].count(SR_unit))
        {
            std::cout << "PANIC!!! SR unit mismatch: " << SR_unit << "\t: " << CR_unit_lep << "\t" << std::endl;
        }
        if(SB_Jon >= 0 && CR_unit_lep >= 0 && !unitBinMapCR_lepcr[SB_Jon].count(CR_unit_lep))
        {
            std::cout << "PANIC!!! CR unit mismatch: " << SR_unit << "\t: " << CR_unit_lep << "\t" << std::endl;
        }

        if(SB_Jon >= 0 && SR_unit >= 0 && !unitBinMapSR[SB_Jon].count(SR_unit))
        {
            std::cout << "PANIC!!! SR unit mismatch: " << SR_unit << "\t: " << CR_unit_pho << "\t" << std::endl;
        }
        if(SB_Jon >= 0 && CR_unit_pho >= 0 && !unitBinMapCR_phocr[SB_Jon].count(CR_unit_pho))
        {
            std::cout << "PANIC!!! CR unit mismatch: " << SR_unit << "\t: " << CR_unit_pho << "\t" << std::endl;
        }

        if(SB_Jon >= 0 && SR_unit >= 0 && !unitBinMapSR[SB_Jon].count(SR_unit))
        {
            std::cout << "PANIC!!! SR unit mismatch: " << SR_unit << "\t: " << CR_unit_qcd << "\t" << std::endl;
        }
        if(SB_Jon >= 0 && CR_unit_qcd >= 0 && !unitBinMapCR_qcdcr[SB_Jon].count(CR_unit_qcd))
        {
            std::cout << "PANIC!!! CR unit mismatch: " << SR_unit << "\t: " << CR_unit_qcd << "\t" << std::endl;
        }
    }
}
