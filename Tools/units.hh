#ifndef SEARCHUNITDEF_HH
#define SEARCHUNITDEF_HH
int SRbin(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
int SRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
int QCDCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
int lepCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
int phoCRunit(bool Pass_Baseline, bool Pass_QCDCR, bool Pass_LepCR, bool Pass_PhoCR, bool HighDM, bool LowDM, int nb, float mtb, float ptb, float MET, int nSoftB, int njets, float ISRpt, float HT, int nres, int ntop, int nw);
#endif
