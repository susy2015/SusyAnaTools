#include "searchBins.h"

#include <iostream>
#include "TLine.h"
#include "TLatex.h"

SearchBins::searchBinDef::searchBinDef() : idx_SR_(-1), bJet_lo_(-1), bJet_hi_(-1), top_lo_(-1), top_hi_(-1), met_lo_(-1), met_hi_(-1), MT2_lo_(-1), MT2_hi_(-1)
{
    
}

SearchBins::searchBinDef::searchBinDef(int idx_SR, int bJet_lo, int bJet_hi, int top_lo, int top_hi, double met_lo, double met_hi, double MT2_lo, double MT2_hi) : idx_SR_(idx_SR), bJet_lo_(bJet_lo), bJet_hi_(bJet_hi), top_lo_(top_lo), top_hi_(top_hi), met_lo_(met_lo), met_hi_(met_hi), MT2_lo_(MT2_lo), MT2_hi_(MT2_hi)
{
    
}

bool SearchBins::searchBinDef::compare(const int ibJet, const int iTop, const double MT2, const double met) const
{
    return (bJet_lo_ < 0  || ibJet >= bJet_lo_)
        && (bJet_hi_ < 0  || ibJet  < bJet_hi_)
        && (top_lo_ < 0   || iTop >= top_lo_)
        && (top_hi_ < 0   || iTop  < top_hi_)
        && (MT2_lo_ < 0   || MT2 >= MT2_lo_ )
        && (MT2_hi_ < 0   || MT2  < MT2_hi_ )
        && (met_lo_ < 0   || met >= met_lo_ )
        && (met_hi_ < 0   || met  < met_hi_ );
}

SearchBins::SearchBins()
{
    //         nb=1, ntop=1
    addNbNtBin(1, 2, 1, 2,
               // MT2 lo 
               {200, 200, 200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400, 400},
               // MT2 hi 
               {300, 300, 300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1,  -1},
               // met lo 
               {200, 275, 350, 450, /**/ 200, 275, 350, 450, /**/ 200, 350, 450},
               // met hi 
               {275, 350, 450,  -1, /**/ 275, 350, 450,  -1, /**/ 350, 450,  -1});

    //         nb=2, ntop=1
    addNbNtBin(2, 3, 1, 2,
               // MT2 lo 
               {200, 200, 200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, 450, /**/ 200, 275, 350, 450, /**/ 200, 450},
               // met hi 
               {275, 350, 450,  -1, /**/ 275, 350, 450,  -1, /**/ 450,  -1});
    //         nb>=3, ntop=1
    addNbNtBin(3, -1, 1, 2,
               // MT2 lo 
               {200, 200, 200},
               // MT2 hi 
               { -1,  -1,  -1},
               // met lo 
               {200, 300, 400},
               // met hi 
               {300, 400,  -1});
    //         nb=1, ntop=2
    addNbNtBin(1, 2, 2, 3,
               // MT2 lo 
               {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
               // met hi 
               {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1});
    //         nb=2, ntop=2
    addNbNtBin(2, 3, 2, 3,
               // MT2 lo 
               {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
               // met hi 
               {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1});
    //         nb>=3, ntop=2
    addNbNtBin(3, -1, 2, 3,
               // MT2 lo 
               {200, 200},
               // MT2 hi 
               { -1,  -1},
               // met lo 
               {200, 300},
               // met hi 
               {300,  -1});
    //         nb=1, ntop=3
    addNbNtBin(1, 2, 3, -1,
               // MT2 lo 
               {200},
               // MT2 hi 
               { -1},
               // met lo 
               {200},
               // met hi 
               { -1});
    //         nb=2, ntop=3
    addNbNtBin(2, 3, 3, -1,
               // MT2 lo 
               {200},
               // MT2 hi 
               { -1},
               // met lo 
               {200},
               // met hi 
               { -1});
    //         nb>=3, ntop=3
    addNbNtBin(3, -1, 3, -1,
               // MT2 lo 
               {200},
               // MT2 hi 
               { -1},
               // met lo 
               {200},
               // met hi 
               { -1});
}

void SearchBins::addNbNtBin(int bJet_lo, int bJet_hi, int top_lo, int top_hi, const std::vector<double> mt2_lo, const std::vector<double> mt2_hi, const std::vector<double> met_lo, const std::vector<double> met_hi)
{
    if((met_lo.size() != met_hi.size()) || (met_lo.size() != mt2_lo.size()) || (met_lo.size() != mt2_hi.size()))
    {
        std::cout << "!!!!!!!!!!!!!!!!!!!!BIN NOT ADDED!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cout << "Mismatch in number of met and/or mt2 bin edges for bin Nb: [" << bJet_lo << "," << bJet_hi << "), Nt: [" << top_lo << "," << top_hi << ")!!!!!!" << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!BIN NOT ADDED!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return;
    }
    
    const int Nbins = met_lo.size();
    for(int iBin = 0; iBin < Nbins; ++iBin)
    {
        searchBins_.emplace_back(searchBinDef(NSearchRegions_, bJet_lo, bJet_hi, top_lo, top_hi, met_lo[iBin], met_hi[iBin], mt2_lo[iBin], mt2_hi[iBin]));
    }
    ++NSearchRegions_;
}

void SearchBins::build_MT2_met_Binning_forTH2Poly(std::vector<std::vector<std::vector<double> > > & outBinning) const
{
//    const double max_MT2_for_binEdge = 400, max_met_for_binEdge = 600;
//    //const double max_MT2_for_binCent = max_MT2_for_binEdge + 50, max_met_for_binCent = max_met_for_binEdge + 25;
//    const double pseudoMax_MT2_for_hist = max_MT2_for_binEdge + 100, pseudoMax_met_for_hist = max_met_for_binEdge + 50;
//
//    nTotBins = 0;
//    outBinning.clear();
//    for(unsigned int iSR =0; iSR<nSR; iSR++)
//    {
//        std::vector<std::vector<double> > perBinning;
//        std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
//        for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++)
//        {
//            double MT2_lo = vMT2_vs_met_per_SR[0].at(ib);
//            double MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
//            double met_lo = vMT2_vs_met_per_SR[2].at(ib);
//            double met_hi = vMT2_vs_met_per_SR[3].at(ib);
//            if( MT2_hi == -1 ) MT2_hi = pseudoMax_MT2_for_hist;
//            if( met_hi == -1 ) met_hi = pseudoMax_met_for_hist;
//            perBinning.push_back({met_lo, MT2_lo, met_hi, MT2_hi});
//            nTotBins++;
//        }
//        outBinning.push_back(perBinning);
//    }
////   std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
}

void SearchBins::build_MT2_met_Binning(std::vector<std::vector<std::vector<double> > > & outBinning) const
{
//    nTotBins = 0;
//    outBinning.clear();
//    for(unsigned int iSR =0; iSR<nSR; iSR++)
//    {
//        std::vector<std::vector<double> > perBinning;
//        std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
//        for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++)
//        {
//            double MT2_lo = vMT2_vs_met_per_SR[0].at(ib);
//            double MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
//            double met_lo = vMT2_vs_met_per_SR[2].at(ib);
//            double met_hi = vMT2_vs_met_per_SR[3].at(ib);
//            perBinning.push_back({met_lo, MT2_lo, met_hi, MT2_hi});
//            nTotBins++;
//        }
//        outBinning.push_back(perBinning);
//    }
//    std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
}

int SearchBins::find_Binning_Index(int ibJet, int iTop, double MT2, double met) const
{
    for(int iBin = 0; iBin < searchBins_.size(); ++iBin)
    {
        if(searchBins_[iBin].compare(ibJet, iTop, MT2, met))
        {
            return iBin;
        }
    }
    return -1;
}

void SearchBins::find_BinBoundaries(int inputIdx, searchBinDef & outBinDef) const
{
    if(inputIdx >= 0 && inputIdx < searchBins_.size())
    {
        outBinDef = searchBins_[inputIdx];
    }
    else
    {
        std::cout << "Bin with index " << inputIdx << " does not exist!!!!!!" << std::endl;
    }
}

void SearchBins::print_searchBins() const
{
    printf("%6s        %12s        %12s        %12s        %12s\n", "binIdx", "nTops", "nbJets", "MT2", "met");
    int preiSR = 0;
    for(int ib=0; ib<searchBins_.size(); ib++)
    {
        SearchBins::searchBinDef sbDef;
        find_BinBoundaries(ib, sbDef);
        if( preiSR != sbDef.idx_SR_ )
        {
            std::cout<<std::endl<<std::endl;
            preiSR = sbDef.idx_SR_;
        }
        char tmpnbJetStr[20], tmpnTopStr[20], tmpMT2Str[20], tmpmetStr[20];
        if( sbDef.top_hi_ != -1 ) sprintf(tmpnTopStr, "=%1.0f", sbDef.top_lo_); else sprintf(tmpnTopStr, ">=%1.0f", sbDef.top_lo_);
        if( sbDef.bJet_hi_ != -1 ) sprintf(tmpnbJetStr, "=%1.0f", sbDef.bJet_lo_); else sprintf(tmpnbJetStr, ">=%1.0f", sbDef.bJet_lo_);
        if( sbDef.MT2_hi_ != -1 ) sprintf(tmpMT2Str, "[%3.0f, %3.0f)", sbDef.MT2_lo_, sbDef.MT2_hi_); else sprintf(tmpMT2Str, "[%3.0f, inf)", sbDef.MT2_lo_);
        if( sbDef.met_hi_ != -1 ) sprintf(tmpmetStr, "[%3.0f, %3.0f)", sbDef.met_lo_, sbDef.met_hi_); else sprintf(tmpmetStr, "[%3.0f, inf)", sbDef.met_lo_);
        printf("%6d        %12s        %12s        %12s        %12s\n", ib, tmpnTopStr, tmpnbJetStr, tmpMT2Str, tmpmetStr);
        if( sbDef.met_hi_ == -1 ) std::cout<<std::endl;
    }
}

void SearchBins::print_searchBins_headerstr(const std::string addstr) const
{

    printf("\n%15s & %15s & %15s & %15s & %15s %s\n \\hline\n", "Search Bin", "\\ntops", "\\nbjets", "\\MTTwo [\\GeV]", "\\MET [\\GeV]", addstr.c_str());

}

std::string SearchBins::get_searchBins_defstr(const int binIdx, const std::string addstr) const
{

    SearchBins::searchBinDef sbDef;
    find_BinBoundaries(binIdx, sbDef);
    char tmpnbJetStr[20], tmpnTopStr[20], tmpMT2Str[20], tmpmetStr[20];
    if( sbDef.top_hi_ != -1 ) sprintf(tmpnTopStr, "%1.0f", sbDef.top_lo_); else sprintf(tmpnTopStr, "%1.0f+", sbDef.top_lo_);
    if( sbDef.bJet_hi_ != -1 ) sprintf(tmpnbJetStr, "%1.0f", sbDef.bJet_lo_); else sprintf(tmpnbJetStr, "%1.0f+", sbDef.bJet_lo_);
    if( sbDef.MT2_hi_ != -1 ) sprintf(tmpMT2Str, "%3.0f-%3.0f", sbDef.MT2_lo_, sbDef.MT2_hi_); else sprintf(tmpMT2Str, "%3.0f+", sbDef.MT2_lo_);
    if( sbDef.met_hi_ != -1 ) sprintf(tmpmetStr, "%3.0f-%3.0f", sbDef.met_lo_, sbDef.met_hi_); else sprintf(tmpmetStr, "%3.0f+", sbDef.met_lo_);

    char totStr[200];
    sprintf(totStr, "%15d & %15s & %15s & %15s & %15s %s\n \\hline\n", binIdx, tmpnTopStr, tmpnbJetStr, tmpMT2Str, tmpmetStr, addstr.c_str());
 
    return std::string(totStr);
}

void SearchBins::print_searchBins_latex() const
{
    print_searchBins_headerstr();
    for(int ib=0; ib<searchBins_.size(); ib++)
    {
        std::string outstr = get_searchBins_defstr(ib);
        printf("%s", outstr.c_str());
    }
    std::cout<<std::endl<<std::endl;
}

void SearchBins::print_searchBins_latex(const std::vector<double>& prediction, const std::vector<double>& uncertainty, std::string label) const
{
    print_searchBins_headerstr(label);
    for(int ib=0; ib<searchBins_.size(); ib++)
    {
	char addon[20];
	if( prediction[ib] > 0.01) sprintf(addon, "& $%.2f \\pm %.2f$ \\\\", prediction[ib], uncertainty[ib]);
        else                       sprintf(addon, "& $%.3f \\pm %.3f$ \\\\", prediction[ib], uncertainty[ib]);
	std::string outstr = get_searchBins_defstr(ib, std::string(addon));
	printf("%s", outstr.c_str());
    }
    std::cout<<std::endl<<std::endl;
}

// Function to draw the signal bin definition
void SearchBins::drawSBregionDef(const double ymin_Yields, const double ymax_Yields, const bool logscale)
{

    const double adjHalfBin = 0.5;
    //-----------------------------------------------------------
    // Putting lines and labels explaining search region definitions
    //-----------------------------------------------------------
    // Ntop separation lines
    TLine *tl_ntop = new TLine();
    tl_ntop->SetLineStyle(2);
    tl_ntop->DrawLine(23.5 + adjHalfBin,ymin_Yields,23.5 + adjHalfBin,ymax_Yields);
    tl_ntop->DrawLine(41.5 + adjHalfBin,ymin_Yields,41.5 + adjHalfBin,ymax_Yields);

    // Ntop labels
    TLatex * ttext_ntop = new TLatex();
    ttext_ntop->SetTextFont(42);
    ttext_ntop->SetTextSize(0.06);
    ttext_ntop->SetTextAlign(22);
    if(logscale)
    
    {
        ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields/1.8 ,"N_{top} = 1");
        ttext_ntop->DrawLatex(35.5 + adjHalfBin, ymax_Yields/35. ,"N_{top} = 2");
        ttext_ntop->SetTextAngle(90.);
        ttext_ntop->DrawLatex(43. + adjHalfBin, ymax_Yields/15. ,"N_{top} #geq 3");
    } 
    else
    {
        ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields*0.92 ,"N_{top} = 1");
        ttext_ntop->DrawLatex(35.5 + adjHalfBin, ymax_Yields*0.92 ,"N_{top} = 2");
        ttext_ntop->SetTextAngle(90.);
        ttext_ntop->DrawLatex(43. + adjHalfBin, ymax_Yields*0.85 ,"N_{top} #geq 3");
    }

    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);
    tl_nb->SetLineColor(32);
    if(logscale)
    {
        tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields/4.);
        tl_nb->DrawLine(20.5 + adjHalfBin,ymin_Yields,20.5 + adjHalfBin,ymax_Yields/4.);
        tl_nb->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields/40.);
        tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields/40.);
        tl_nb->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields/40.);
        tl_nb->DrawLine(43.5 + adjHalfBin,ymin_Yields,43.5 + adjHalfBin,ymax_Yields/40.);
    } 
    else
    {
        tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields*0.8);
        tl_nb->DrawLine(20.5 + adjHalfBin,ymin_Yields,20.5 + adjHalfBin,ymax_Yields*0.8);
        tl_nb->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields*0.8);
        tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields*0.8);
        tl_nb->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields*0.8);
        tl_nb->DrawLine(43.5 + adjHalfBin,ymin_Yields,43.5 + adjHalfBin,ymax_Yields*0.8);
    }
    // Nb labels
    TLatex * ttext2 = new TLatex();
    ttext2->SetTextFont(42);
    ttext2->SetTextColor(32);
    ttext2->SetTextSize(0.05);
    ttext2->SetTextAlign(22);
    ttext2->Draw();

    if(logscale)
    {
        ttext2->DrawLatex( 4.5 + adjHalfBin, ymax_Yields/3., "N_{b} = 1");
        ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields/3., "N_{b} = 2");
        ttext2->SetTextAngle(90.);
        ttext2->DrawLatex(21.5 + adjHalfBin, ymax_Yields/3. , "N_{b} #geq 3");
    } 
    else
    {
        ttext2->DrawLatex( 4.5 + adjHalfBin, ymax_Yields*0.8, "N_{b} = 1");
        ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields*0.8, "N_{b} = 2");
        ttext2->SetTextAngle(90.);
        ttext2->DrawLatex(21.5 + adjHalfBin + 0.5, ymax_Yields*0.8, "N_{b} #geq 3");
    }
    // MT2 separation lines
    TLine *tl_mt2 = new TLine();
    tl_mt2->SetLineStyle(4);
    tl_mt2->SetLineColor(49);
    if(logscale)
    {
        tl_mt2->DrawLine(3.5 + adjHalfBin,ymin_Yields,3.5 + adjHalfBin,ymax_Yields/20.);
        tl_mt2->DrawLine(7.5 + adjHalfBin,ymin_Yields,7.5 + adjHalfBin,ymax_Yields/20.);
        tl_mt2->DrawLine(14.5 + adjHalfBin,ymin_Yields,14.5 + adjHalfBin,ymax_Yields/20.);
        tl_mt2->DrawLine(18.5 + adjHalfBin,ymin_Yields,18.5 + adjHalfBin,ymax_Yields/20.);
       
        tl_mt2->DrawLine(26.5 + adjHalfBin,ymin_Yields,26.5 + adjHalfBin,ymax_Yields/80.);
        tl_mt2->DrawLine(29.5 + adjHalfBin,ymin_Yields,29.5 + adjHalfBin,ymax_Yields/80.);
        tl_mt2->DrawLine(34.5 + adjHalfBin,ymin_Yields,34.5 + adjHalfBin,ymax_Yields/80.);
        tl_mt2->DrawLine(37.5 + adjHalfBin,ymin_Yields,37.5 + adjHalfBin,ymax_Yields/80.);
    }
    else
    {
        tl_mt2->DrawLine(3.5 + adjHalfBin,ymin_Yields,3.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(7.5 + adjHalfBin,ymin_Yields,7.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(14.5 + adjHalfBin,ymin_Yields,14.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(18.5 + adjHalfBin,ymin_Yields,18.5 + adjHalfBin,ymax_Yields*0.6);
       
        tl_mt2->DrawLine(26.5 + adjHalfBin,ymin_Yields,26.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(29.5 + adjHalfBin,ymin_Yields,29.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(34.5 + adjHalfBin,ymin_Yields,34.5 + adjHalfBin,ymax_Yields*0.6);
        tl_mt2->DrawLine(37.5 + adjHalfBin,ymin_Yields,37.5 + adjHalfBin,ymax_Yields*0.6);
    }
    // MT2 labels
    TLatex * ttextmt2 = new TLatex();
    ttextmt2->SetTextFont(42);
    ttextmt2->SetTextColor(49);
    ttextmt2->SetTextSize(0.02);
    ttextmt2->SetTextAlign(22);
    ttextmt2->SetTextAngle(90);
    if(logscale)
    {
        ttextmt2->DrawLatex( 1.5 + adjHalfBin, ymax_Yields/10. , "M_{T2}=[200,300]");
        ttextmt2->DrawLatex( 5.0 + adjHalfBin, ymax_Yields/10. , "M_{T2}=[300,400]");
        ttextmt2->DrawLatex(8.5 + adjHalfBin, ymax_Yields/10. , "M_{T2}#geq400");
    }
    else
    {
        ttextmt2->DrawLatex( 1.5 + adjHalfBin, ymax_Yields*0.55 , "M_{T2}=[200,300]");
        ttextmt2->DrawLatex( 5.0 + adjHalfBin, ymax_Yields*0.55 , "M_{T2}=[300,400]");
        ttextmt2->DrawLatex(8.5 + adjHalfBin, ymax_Yields*0.55 , "M_{T2}#geq400");
    }
    //-----------------------------------------------------------

}

