#include "searchBins.h"

#include <iostream>
#include "TLine.h"
#include "TLatex.h"

SearchBins::searchBinDef::searchBinDef() : idx_SR_(-1), bJet_lo_(-1), bJet_hi_(-1), top_lo_(-1), top_hi_(-1), met_lo_(-1), met_hi_(-1), MT2_lo_(-1), MT2_hi_(-1)
{
    
}

SearchBins::searchBinDef::searchBinDef(int idx_SR, int idx_vMT2_vs_met, int bJet_lo, int bJet_hi, int top_lo, int top_hi, double met_lo, double met_hi, double MT2_lo, double MT2_hi) : idx_SR_(idx_SR), idx_vMT2_vs_met_(idx_vMT2_vs_met), bJet_lo_(bJet_lo), bJet_hi_(bJet_hi), top_lo_(top_lo), top_hi_(top_hi), met_lo_(met_lo), met_hi_(met_hi), MT2_lo_(MT2_lo), MT2_hi_(MT2_hi)
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

SearchBins::SearchBins(std::string binEra)
{
    NSearchRegions_ = 0;

    if(binEra.compare("SB_37_2015") == 0)
    {
        SearchBins_37_2015();
    }
    else if(binEra.compare("SB_45_2015") == 0)
    {
        SearchBins_45_2015();
    }
    else if(binEra.compare("SB_69_2016") == 0)
    {
        SearchBins_69_2016();
    }
    else if(binEra.compare("SB_59_2016") == 0)
    {
        SearchBins_59_2016();
    }
    else
    {
        std::cout << "!!!!!!!!!!!!!!!!!!!!BINS NOT ADDED!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cout << "Bin era \"" << binEra << "\" does not exist" << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!BINS NOT ADDED!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    }
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
        searchBins_.emplace_back(searchBinDef(NSearchRegions_, iBin, bJet_lo, bJet_hi, top_lo, top_hi, met_lo[iBin], met_hi[iBin], mt2_lo[iBin], mt2_hi[iBin]));
    }
    ++NSearchRegions_;
}

void SearchBins::build_MT2_met_Binning_forTH2Poly(std::vector<std::vector<std::vector<double> > > & outBinning) const
{
    const double max_MT2_for_binEdge = 450, max_met_for_binEdge = 800;
    //const double max_MT2_for_binCent = max_MT2_for_binEdge + 50, max_met_for_binCent = max_met_for_binEdge + 25;
    const double pseudoMax_MT2_for_hist = max_MT2_for_binEdge + 100, pseudoMax_met_for_hist = max_met_for_binEdge + 50;
    
    int nTotBins = 0;
    int currentSR = -1;
    outBinning.clear();
    for(auto& sb : searchBins_)
    {
        double MT2_lo = sb.MT2_lo_;
        double MT2_hi = sb.MT2_hi_;
        double met_lo = sb.met_lo_;
        double met_hi = sb.met_hi_;
        if( MT2_hi == -1 ) MT2_hi = pseudoMax_MT2_for_hist;
        if( met_hi == -1 ) met_hi = pseudoMax_met_for_hist;
        ++nTotBins;

        if(sb.idx_SR_ != currentSR)
        {
            currentSR = sb.idx_SR_;
            outBinning.emplace_back(std::vector<std::vector<double> >({{met_lo, MT2_lo, met_hi, MT2_hi}}));
        }
        else
        {
            outBinning.back().push_back({met_lo, MT2_lo, met_hi, MT2_hi});
        }
    }
    //   std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
}

void SearchBins::build_MT2_met_Binning(std::vector<std::vector<std::vector<double> > > & outBinning) const
{
    int nTotBins = 0;
    int currentSR = -1;
    outBinning.clear();
    for(auto& sb : searchBins_)
    {
        double MT2_lo = sb.MT2_lo_;
        double MT2_hi = sb.MT2_hi_;
        double met_lo = sb.met_lo_;
        double met_hi = sb.met_hi_;
        ++nTotBins;

        if(sb.idx_SR_ != currentSR)
        {
            currentSR = sb.idx_SR_;
            outBinning.emplace_back(std::vector<std::vector<double> >({{met_lo, MT2_lo, met_hi, MT2_hi}}));
        }
        else
        {
            outBinning.back().push_back({met_lo, MT2_lo, met_hi, MT2_hi});
        }
    }
    std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
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
        if( sbDef.top_hi_ != -1 ) sprintf(tmpnTopStr, "=%d", sbDef.top_lo_); else sprintf(tmpnTopStr, ">=%d", sbDef.top_lo_);
        if( sbDef.bJet_hi_ != -1 ) sprintf(tmpnbJetStr, "=%d", sbDef.bJet_lo_); else sprintf(tmpnbJetStr, ">=%d", sbDef.bJet_lo_);
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
    char tmpnbJetStr[50], tmpnTopStr[50], tmpMT2Str[50], tmpmetStr[50];
    if( sbDef.top_hi_ != -1 ) sprintf(tmpnTopStr, "%d", sbDef.top_lo_); else sprintf(tmpnTopStr, "%d+", sbDef.top_lo_);
    if( sbDef.bJet_hi_ != -1 ) sprintf(tmpnbJetStr, "%d", sbDef.bJet_lo_); else sprintf(tmpnbJetStr, "%d+", sbDef.bJet_lo_);
    if( sbDef.MT2_hi_ != -1 ) sprintf(tmpMT2Str, "%3.0f-%3.0f", sbDef.MT2_lo_, sbDef.MT2_hi_); else sprintf(tmpMT2Str, "%3.0f+", sbDef.MT2_lo_);
    if( sbDef.met_hi_ != -1 ) sprintf(tmpmetStr, "%3.0f-%3.0f", sbDef.met_lo_, sbDef.met_hi_); else sprintf(tmpmetStr, "%3.0f+", sbDef.met_lo_);

    char totStr[500];
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
void SearchBins::drawSBregionDef(const double ymin_Yields, const double ymax_Yields, const bool logscale){
    int NSB = 59;

    const double adjHalfBin = 0.5;
    const double deltaY = ymax_Yields - ymin_Yields;
    //-----------------------------------------------------------
    // Putting lines and labels explaining search region definitions
    //-----------------------------------------------------------
    // Ntop separation lines
    TLine *tl_ntop = new TLine();
    tl_ntop->SetLineStyle(2);
    if(NSB == 59)
      {
	tl_ntop->DrawLine(27.5 + adjHalfBin,ymin_Yields,27.5 + adjHalfBin,ymax_Yields);
        tl_ntop->DrawLine(52.5 + adjHalfBin,ymin_Yields,52.5 + adjHalfBin,ymax_Yields);

      }
    else if(NSB == 45)
      {
        tl_ntop->DrawLine(23.5 + adjHalfBin,ymin_Yields,23.5 + adjHalfBin,ymax_Yields);
        tl_ntop->DrawLine(41.5 + adjHalfBin,ymin_Yields,41.5 + adjHalfBin,ymax_Yields);
      } 
    else if(NSB == 37)
      {
        tl_ntop->DrawLine(20.5 + adjHalfBin,ymin_Yields,20.5 + adjHalfBin,ymax_Yields);
      }
    // Ntop labels
    TLatex * ttext_ntop = new TLatex();
    ttext_ntop->SetTextFont(42);
    ttext_ntop->SetTextSize(0.045);
    ttext_ntop->SetTextAlign(22);
    if(logscale)
      {
      
	if(NSB == 59)
	{
	  ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields/2 ,"N_{t} = 1");
	  ttext_ntop->DrawLatex(39.5 + adjHalfBin, ymax_Yields/200. ,"N_{t} = 2");
	  ttext_ntop->SetTextAngle(90.);
	  ttext_ntop->DrawLatex(54.5 + adjHalfBin, ymax_Yields/20. ,"N_{t} #geq 3");

	}

        else if(NSB == 45)
        {
            ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields/3 ,"N_{t} = 1");
            ttext_ntop->DrawLatex(32.5 + adjHalfBin, ymax_Yields/300. ,"N_{t} = 2");
            ttext_ntop->SetTextAngle(90.);
            ttext_ntop->DrawLatex(42.5 + adjHalfBin, ymax_Yields/30. ,"N_{t} #geq 3"); 
        } 
        else if (NSB == 37)
        {
            ttext_ntop->DrawLatex(10 + adjHalfBin, ymax_Yields/1.8 ,"N_{t} = 1");
            ttext_ntop->DrawLatex(29.5 + adjHalfBin, ymax_Yields/70. ,"N_{t} #geq 2");
        }
    }
    else
      {
	if(NSB == 59)
	  {
	    ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} = 1");
            ttext_ntop->DrawLatex(38.5 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} = 2");
            ttext_ntop->SetTextAngle(90.);
	    ttext_ntop->DrawLatex(55.5 + adjHalfBin, ymax_Yields*0.85 ,"N_{t} #geq 3");
	  }
        else if(NSB == 45)
	  {
            ttext_ntop->DrawLatex(11.5 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} = 1");
            ttext_ntop->DrawLatex(35.5 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} = 2");
            ttext_ntop->SetTextAngle(90.);
            ttext_ntop->DrawLatex(43. + adjHalfBin, ymax_Yields*0.85 ,"N_{t} #geq 3");
	  }
        else if(NSB == 37)
	  {
            ttext_ntop->DrawLatex(10 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} = 1");
            ttext_ntop->DrawLatex(29.5 + adjHalfBin, ymax_Yields*0.93 ,"N_{t} #geq 2");
	  }
    }

    // Nb separation lines
    TLine *tl_nb = new TLine();
    tl_nb->SetLineStyle(3);
    tl_nb->SetLineColor(32);
    if(logscale)
      {
	if(NSB == 59)
	  {
	    tl_nb->DrawLine(11.5 + adjHalfBin,ymin_Yields,11.5 + adjHalfBin,ymax_Yields/6);
            tl_nb->DrawLine(22.5 + adjHalfBin,ymin_Yields,22.5 + adjHalfBin,ymax_Yields/6.);
            tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields/160.);
            tl_nb->DrawLine(48.5 + adjHalfBin,ymin_Yields,48.5 + adjHalfBin,ymax_Yields/160.);
            tl_nb->DrawLine(54.5 + adjHalfBin,ymin_Yields,54.5 + adjHalfBin,ymax_Yields/160.);
            tl_nb->DrawLine(56.5 + adjHalfBin,ymin_Yields,56.5 + adjHalfBin,ymax_Yields/160.);  
	    
	  }
      
        else if(NSB == 45)
	  {
            tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields/40.);
            tl_nb->DrawLine(20.5 + adjHalfBin,ymin_Yields,20.5 + adjHalfBin,ymax_Yields/40.);
            tl_nb->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields/400.);
            tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields/400.);
            tl_nb->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields/400.);
            tl_nb->DrawLine(43.5 + adjHalfBin,ymin_Yields,43.5 + adjHalfBin,ymax_Yields/400.);
	  }
        else if (NSB == 37)
	  {
            tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields/4.);
            tl_nb->DrawLine(28.5 + adjHalfBin,ymin_Yields,28.5 + adjHalfBin,ymax_Yields/160.);
	  }
    }
    else
      {
	if(NSB == 59)
	  {
	    tl_nb->DrawLine(11.5 + adjHalfBin,ymin_Yields,11.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(22.5 + adjHalfBin,ymin_Yields,22.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(48.5 + adjHalfBin,ymin_Yields,48.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(54.5 + adjHalfBin,ymin_Yields,54.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(56.5 + adjHalfBin,ymin_Yields,56.5 + adjHalfBin,ymax_Yields*0.8);
	  }

        else if(NSB == 45)
	  {
            tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(20.5 + adjHalfBin,ymin_Yields,20.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(39.5 + adjHalfBin,ymin_Yields,39.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(43.5 + adjHalfBin,ymin_Yields,43.5 + adjHalfBin,ymax_Yields*0.8);
	  } 
        else if (NSB == 37)
	  {
	    tl_nb->DrawLine(10.5 + adjHalfBin,ymin_Yields,10.5 + adjHalfBin,ymax_Yields*0.8);
            tl_nb->DrawLine(28.5 + adjHalfBin,ymin_Yields,28.5 + adjHalfBin,ymax_Yields*0.8);
	  }
    }
    // Nb labels
    TLatex * ttext2 = new TLatex();
    ttext2->SetTextFont(42);
    //ttext2->SetTextColor(32);
    ttext2->SetTextColor(kBlack);
    ttext2->SetTextSize(0.045);
    ttext2->SetTextAlign(22);
    ttext2->Draw();

    if(logscale)
      {
        ttext2->DrawLatex( 4.5 + adjHalfBin, ymax_Yields/4., "N_{b} = 1");
        if(NSB == 37) ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields/3.5, "N_{b} #geq 2");
        else if(NSB == 45)
	  {
            ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields/7, "N_{b} = 2");
            ttext2->SetTextAngle(90.);
            ttext2->DrawLatex(22.5  + adjHalfBin, ymax_Yields/20 , "N_{b} #geq 3");
	  }
	else if(NSB == 59)
	  {
	    ttext2->DrawLatex(17.5 + adjHalfBin, ymax_Yields/4., "N_{b} = 2");
            ttext2->SetTextAngle(90.);
            ttext2->DrawLatex(23.5  + adjHalfBin, ymax_Yields/30 , "N_{b} #geq 3");
	  }
      } 
    else
      {
        ttext2->DrawLatex( 4.5 + adjHalfBin, ymax_Yields*0.87, "N_{b} = 1");
        if(NSB == 37) ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields*0.87, "N_{b} #geq 2");
        else if(NSB == 45)
	  {
            ttext2->DrawLatex(14.5 + adjHalfBin, ymax_Yields*0.87, "N_{b} = 2");
            ttext2->SetTextAngle(90.);
            ttext2->DrawLatex(21.5 + adjHalfBin + 0.5, ymax_Yields*0.87, "N_{b} #geq 3");
	  }
	else if(NSB == 59)
          {
            ttext2->DrawLatex(17.5 + adjHalfBin, ymax_Yields*0.87, "N_{b} = 2");
            ttext2->SetTextAngle(90.);
            ttext2->DrawLatex(25.5 + adjHalfBin + 0.5, ymax_Yields*0.87, "N_{b} #geq 3");
          }

      }
    // MT2 separation lines
    TLine *tl_mt2 = new TLine();
    tl_mt2->SetLineStyle(4);
    tl_mt2->SetLineColor(49);
    if(logscale)
    {
        tl_mt2->DrawLine(3.5 + adjHalfBin,ymin_Yields,3.5 + adjHalfBin,ymax_Yields/40.);
        tl_mt2->DrawLine(7.5 + adjHalfBin,ymin_Yields,7.5 + adjHalfBin,ymax_Yields/40.);
        

        if(NSB == 45)
	  {
	    tl_mt2->DrawLine(14.5 + adjHalfBin,ymin_Yields,14.5 + adjHalfBin,ymax_Yields/300.);
	    tl_mt2->DrawLine(18.5 + adjHalfBin,ymin_Yields,18.5 + adjHalfBin,ymax_Yields/300.);
            tl_mt2->DrawLine(26.5 + adjHalfBin,ymin_Yields,26.5 + adjHalfBin,ymax_Yields/300.);
            tl_mt2->DrawLine(29.5 + adjHalfBin,ymin_Yields,29.5 + adjHalfBin,ymax_Yields/300.);
            tl_mt2->DrawLine(34.5 + adjHalfBin,ymin_Yields,34.5 + adjHalfBin,ymax_Yields/300.);
            tl_mt2->DrawLine(37.5 + adjHalfBin,ymin_Yields,37.5 + adjHalfBin,ymax_Yields/300.);
        } 
        else if(NSB == 37)
	  {
	    tl_mt2->DrawLine(14.5 + adjHalfBin,ymin_Yields,14.5 + adjHalfBin,ymax_Yields/500.);
	    tl_mt2->DrawLine(18.5 + adjHalfBin,ymin_Yields,18.5 + adjHalfBin,ymax_Yields/500.);
	    tl_mt2->DrawLine(23.5 + adjHalfBin,ymin_Yields,23.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(26.5 + adjHalfBin,ymin_Yields,26.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(34.5 + adjHalfBin,ymin_Yields,34.5 + adjHalfBin,ymax_Yields/320.);
        }

	else if(NSB == 59)
	  {
	    tl_mt2->DrawLine(15.5 + adjHalfBin,ymin_Yields,15.5 + adjHalfBin,ymax_Yields/40.);
	    tl_mt2->DrawLine(19.5 + adjHalfBin,ymin_Yields,19.5 + adjHalfBin,ymax_Yields/40.);
	    tl_mt2->DrawLine(25.5 + adjHalfBin,ymin_Yields,25.5 + adjHalfBin,ymax_Yields/100.);
	    tl_mt2->DrawLine(31.5 + adjHalfBin,ymin_Yields,31.5 + adjHalfBin,ymax_Yields/100.);
            tl_mt2->DrawLine(35.5 + adjHalfBin,ymin_Yields,35.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(42.5 + adjHalfBin,ymin_Yields,42.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(45.5 + adjHalfBin,ymin_Yields,45.5 + adjHalfBin,ymax_Yields/320.);
            tl_mt2->DrawLine(50.5 + adjHalfBin,ymin_Yields,50.5 + adjHalfBin,ymax_Yields/320.);
	  }

    } 
    else
    {
        tl_mt2->DrawLine( 3.5 + adjHalfBin, ymin_Yields,  3.5 + adjHalfBin, ymax_Yields*0.6);
        tl_mt2->DrawLine( 7.5 + adjHalfBin, ymin_Yields,  7.5 + adjHalfBin, ymax_Yields*0.6);

        if(NSB == 45)
	  {
	    tl_mt2->DrawLine(14.5 + adjHalfBin, ymin_Yields, 14.5 + adjHalfBin, ymax_Yields*0.6);
	    tl_mt2->DrawLine(18.5 + adjHalfBin, ymin_Yields, 18.5 + adjHalfBin, ymax_Yields*0.6);  
            tl_mt2->DrawLine(26.5 + adjHalfBin, ymin_Yields, 26.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(29.5 + adjHalfBin, ymin_Yields, 29.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(34.5 + adjHalfBin, ymin_Yields, 34.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(37.5 + adjHalfBin, ymin_Yields, 37.5 + adjHalfBin, ymax_Yields*0.6);
	  } 
        else if(NSB == 37)
	  {
	    tl_mt2->DrawLine(14.5 + adjHalfBin, ymin_Yields, 14.5 + adjHalfBin, ymax_Yields*0.6);
	    tl_mt2->DrawLine(18.5 + adjHalfBin, ymin_Yields, 18.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(23.5 + adjHalfBin, ymin_Yields, 23.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(26.5 + adjHalfBin, ymin_Yields, 26.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(31.5 + adjHalfBin, ymin_Yields, 31.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(34.5 + adjHalfBin, ymin_Yields, 34.5 + adjHalfBin, ymax_Yields*0.6);
	  }
	else if(NSB == 59)
          {
            tl_mt2->DrawLine(15.5 + adjHalfBin, ymin_Yields, 15.5 + adjHalfBin, ymax_Yields*0.6);
	    tl_mt2->DrawLine(19.5 + adjHalfBin, ymin_Yields, 19.5 + adjHalfBin, ymax_Yields*0.6);
	    tl_mt2->DrawLine(25.5 + adjHalfBin, ymin_Yields, 25.5 + adjHalfBin, ymax_Yields*0.6);
	    tl_mt2->DrawLine(31.5 + adjHalfBin, ymin_Yields, 31.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(35.5 + adjHalfBin, ymin_Yields, 35.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(42.5 + adjHalfBin, ymin_Yields, 42.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(45.5 + adjHalfBin, ymin_Yields, 45.5 + adjHalfBin, ymax_Yields*0.6);
            tl_mt2->DrawLine(50.5 + adjHalfBin, ymin_Yields, 50.5 + adjHalfBin, ymax_Yields*0.6);
          }
    }
    // MT2 labels
    TLatex * ttextmt2 = new TLatex();
    ttextmt2->SetTextFont(42);
    //ttextmt2->SetTextColor(49);
    ttextmt2->SetTextColor(kBlack);
    ttextmt2->SetTextSize(0.040);
    ttextmt2->SetTextAlign(12);
    ttextmt2->SetTextAngle(90);
    if(logscale)
      {
	//Only done upto here
	if(NSB == 59)
	  {
	    ttextmt2->DrawLatex( 2.0, ymax_Yields/40. , "M_{T2}=[200,350]");
	    ttextmt2->DrawLatex( 6.0, ymax_Yields/100. , "M_{T2}=[350,450]");
	    ttextmt2->DrawLatex( 10.5, ymax_Yields/100. , "M_{T2}#geq450 GeV");
	    ttextmt2->DrawLatex( 27.5, ymax_Yields/100. , "M_{T2}#geq350 GeV");
	  }
	else if(NSB == 37 || NSB == 45)
	  {
	    ttextmt2->DrawLatex( 2.0, ymax_Yields/2000. , "M_{T2}=[200,300]");
	    ttextmt2->DrawLatex( 6.0, ymax_Yields/2000. , "M_{T2}=[300,400]");
	    ttextmt2->DrawLatex( 9.5, ymax_Yields/2000. , "M_{T2}#geq400 GeV");
	  } 
      }
    else
      {
	if(NSB == 59)
	  {
	    ttextmt2->DrawLatex( 2.0, deltaY*0.52 + ymin_Yields , "M_{T2}=[200,350]");
	    ttextmt2->DrawLatex( 6.0, deltaY*0.52 + ymin_Yields , "M_{T2}=[350,450]");
	    ttextmt2->DrawLatex( 10.5, deltaY*0.52 + ymin_Yields , "M_{T2}#geq450 GeV");
	    ttextmt2->DrawLatex( 27.5, deltaY*0.52 + ymin_Yields , "M_{T2}#geq350 GeV");
	  }
	
	else if(NSB == 37 || NSB == 45)
	  {
	    ttextmt2->DrawLatex( 2.0, deltaY*0.52 + ymin_Yields , "M_{T2}=[200,300]");
	    ttextmt2->DrawLatex( 6.0, deltaY*0.52 + ymin_Yields , "M_{T2}=[300,400]");
	    ttextmt2->DrawLatex( 9.5, deltaY*0.52 + ymin_Yields , "M_{T2}#geq400 GeV");
	  }
      }


}

void SearchBins::SearchBins_59_2016()
{
    //         nb=1, ntop=1
    addNbNtBin(1, 2, 1, 2,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb=2, ntop=1
    addNbNtBin(2, 3, 1, 2,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 500, 650,  -1});

    //         nb>=3, ntop=1
    addNbNtBin(3, -1, 1, 2,
               // MT2 lo
               {200, 200, 200, /**/ 350, 350},
               // MT2 hi
               {350, 350, 350, /**/  -1,  -1},
               // met lo
               {200, 350, 500, /**/ 200, 350},
               // met hi
               {350, 500,  -1, /**/ 350,  -1});

    //         nb=1, ntop=2
    addNbNtBin(1, 2, 2, 3,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb=2, ntop=2
    addNbNtBin(2, 3, 2, 3,
               // MT2 lo
               {200, 200, 200, /**/ 350, 350, 350, /**/ 450, 450, 450},
               // MT2 hi
               {350, 350, 350, /**/ 450, 450, 450, /**/  -1,  -1,  -1},
               // met lo
               {200, 350, 500, /**/ 200, 350, 500, /**/ 200, 350, 500},
               // met hi
               {350, 500,  -1, /**/ 350, 500,  -1, /**/ 350, 500,  -1});

    //         nb>=3, ntop=2
    addNbNtBin(3, -1, 2, 3,
               // MT2 lo
               {200, 200, /**/ 350, 350},
               // MT2 hi
               {350, 350, /**/  -1,  -1},
               // met lo
               {200, 350, /**/ 200, 350},
               // met hi
               {350,  -1, /**/ 350,  -1});

    //         nb=1, ntop>=3
    addNbNtBin(1, 2, 3, -1,
               // MT2 lo
               {200, 200},
               // MT2 hi
               { -1,  -1},
               // met lo
               {200, 350},
               // met hi
               {350,  -1});

    //         nb=2, ntop>=3
    addNbNtBin(2, 3, 3, -1,
               // MT2 lo
               {200, 200},
               // MT2 hi
               { -1,  -1},
               // met lo
               {200, 350},
               // met hi
               {350,  -1});

    //         nb>=3, ntop>=3
    addNbNtBin(3, -1, 3, -1,
               // MT2 lo
               {200, 200},
               // MT2 hi
               { -1,  -1},
               // met lo
               {200, 350},
               // met hi
               {350,  -1});
}


void SearchBins::SearchBins_69_2016()
{
    //         nb=1, ntop=1
    addNbNtBin(1, 2, 1, 2,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb=2, ntop=1
    addNbNtBin(2, 3, 1, 2,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb>=3, ntop=1
    addNbNtBin(3, -1, 1, 2,
               // MT2 lo
               {200, 200, 200, /**/ 350, 350, 350},
               // MT2 hi
               {350, 350, 350, /**/  -1,  -1,  -1},
               // met lo
               {200, 350, 500, /**/ 200, 350, 500},
               // met hi
               {350, 500,  -1, /**/ 350, 500,  -1});

    //         nb=1, ntop=2
    addNbNtBin(1, 2, 2, 3,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb=2, ntop=2
    addNbNtBin(2, 3, 2, 3,
               // MT2 lo
               {200, 200, 200, 200, /**/ 350, 350, 350, 350, /**/ 450, 450, 450, 450},
               // MT2 hi
               {350, 350, 350, 350, /**/ 450, 450, 450, 450, /**/  -1,  -1,  -1,  -1},
               // met lo
               {200, 350, 500, 650, /**/ 200, 350, 500, 650, /**/ 200, 350, 500, 650},
               // met hi
               {350, 500, 650,  -1, /**/ 350, 500, 650,  -1, /**/ 350, 500, 650,  -1});

    //         nb>=3, ntop=2
    addNbNtBin(3, -1, 2, 3,
               // MT2 lo
               {200, 200, 200, /**/ 350, 350, 350},
               // MT2 hi
               {350, 350, 350, /**/  -1,  -1,  -1},
               // met lo
               {200, 350, 500, /**/ 200, 350, 500},
               // met hi
               {350, 500,  -1, /**/ 350, 500,  -1});

    //         nb=1, ntop>=3
    addNbNtBin(1, 2, 3, -1,
               // MT2 lo
               {200, 200, 200},
               // MT2 hi
               { -1,  -1,  -1},
               // met lo
               {200, 350, 500},
               // met hi
               {350, 500,  -1});

    //         nb=2, ntop>=3
    addNbNtBin(2, 3, 3, -1,
               // MT2 lo
               {200, 200, 200},
               // MT2 hi
               { -1,  -1,  -1},
               // met lo
               {200, 350, 500},
               // met hi
               {350, 500,  -1});

    //         nb>=3, ntop>=3
    addNbNtBin(3, -1, 3, -1,
               // MT2 lo
               {200, 200, 200},
               // MT2 hi
               { -1,  -1,  -1},
               // met lo
               {200, 350, 500},
               // met hi
               {350, 500,  -1});
}

void SearchBins::SearchBins_45_2015()
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

void SearchBins::SearchBins_37_2015()
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

    //         nb>=2, ntop=1
    addNbNtBin(2, -1, 1, 2,
               // MT2 lo 
               {200, 200, 200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, 450, /**/ 200, 275, 350, 450, /**/ 200, 450},
               // met hi 
               {275, 350, 450,  -1, /**/ 275, 350, 450,  -1, /**/ 450,  -1});
    //         nb=1, ntop>=2
    addNbNtBin(1, 2, 2, -1,
               // MT2 lo 
               {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
               // met hi 
               {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1});
    //         nb>=2, ntop>=2
    addNbNtBin(2, -1, 2, -1,
               // MT2 lo 
               {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
               // MT2 hi 
               {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
               // met lo 
               {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
               // met hi 
               {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1});
}
