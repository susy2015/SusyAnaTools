#ifndef SEARCH_BINS_H
#define SEARCH_BINS_H

#include <vector>
#include <string>

//a generic base class for accessing analysis search bin information
class SearchBins
{
public:
    SearchBins(std::string binEra = "SB_v1_2017");

    int find_Binning_Index(const int ibJet, const int iTop, const float MT2, const float met) const;
    int find_Binning_Index(const int ibJet, const int iTop, const float MT2, const float met, const float ht) const;
    std::vector<int> find_Binning_Indices(const int ibJet, const int iTop, const float MT2, const float met) const;
    std::vector<int> find_Binning_Indices(const int ibJet, const int iTop, const float MT2, const float met, const float ht) const;

    struct searchBinDef
    {
    public:
        int bJet_lo_, top_lo_;
        float met_lo_, MT2_lo_, HT_lo_;
        int bJet_hi_, top_hi_;
        float met_hi_, MT2_hi_, HT_hi_;
        int idx_SR_, idx_vMT2_vs_met_;

        searchBinDef();
        searchBinDef(int idx_SR, int idx_vMT2_vs_met, int bJet_lo, int bJet_hi, int top_lo, int top_hi, float met_lo, float met_hi, float MT2_lo, float MT2_hi, float HT_lo, float HT_hi);

        bool compare(const int ibJet, const int iTop, const float MT2, const float HT, const float met) const;
    };

    int nSearchBins() {return searchBins_.size();}

    void build_MT2_met_Binning_forTH2Poly(std::vector<std::vector<std::vector<float> > > & outBinning) const;
    void build_MT2_met_Binning(std::vector<std::vector<std::vector<float> > > & outBinning) const;
    
    void find_BinBoundaries(const int inputIdx, searchBinDef & outBinDef) const;
    void print_searchBins() const;
    void print_searchBins_headerstr(const std::string addstr="\\\\") const;
    std::string get_searchBins_defstr(const int binIdx, const std::string addstr="\\\\") const;
    void print_searchBins_latex() const;
    void print_searchBins_latex(const std::vector<float>& prediction, const std::vector<float>& uncertainty, const std::string label) const;
    void print_searchBinsPred_latex(const std::vector<float>& prediction, const std::vector<float>& StatUp,  const std::vector<float>& StatDown, const std::vector<float>& SysUp, const std::vector<float>& SysDown, std::string label) const;
    static void drawSBregionDef(const float ymin_Yields = 0.05, const float ymax_Yields = 500., const bool logscale=true, const bool drawOnlyLines = false);

    static constexpr float max_MT2_for_binEdge = 900, max_met_for_binEdge = 1000, max_HT_for_binEdge = 2000;
    static constexpr float pseudoMax_MT2_for_hist = max_MT2_for_binEdge + 100, pseudoMax_met_for_hist = max_met_for_binEdge + 50, pseudoMax_HT_for_hist = max_HT_for_binEdge + 100;

private:
    //note, this is NOT the number of search bins, but instead th enumber of sepeate bins in the Nb - Nt plane
    //NEVER adjust this variable by hand, only in addNbNtBin(...)
    int NSearchRegions_;

    //The vector of search bins
    std::vector<searchBinDef> searchBins_;
    
    //Use this to add bins to searchBins_, it adds all met and mt2 bins for a particular bin in Nb-Nt
    void addNbNtBin_MT2_MET(int bJet_lo, int bJet_hi, int top_lo, int top_hi, const std::vector<float> mt2_lo, const std::vector<float> mt2_hi, const std::vector<float> met_lo, const std::vector<float> met_hi);
    void addNbNtBin_HT_MET(int bJet_lo, int bJet_hi, int top_lo, int top_hi, const std::vector<float> ht_lo, const std::vector<float> ht_hi, const std::vector<float> met_lo, const std::vector<float> met_hi);

    void SearchBins_37_2015();
    void SearchBins_45_2015();
    void SearchBins_69_2016();
    void SearchBins_59_2016();
    void SearchBins_Aggregate_ICHEP_2016();
    void SearchBins_v1_2017();
    void SearchBins_Aggregate_2017();

    std::string binEra_;
};

#endif
