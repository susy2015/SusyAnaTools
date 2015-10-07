/*
const double nTops_SR_lo[]   = { 0,  0,  0,  1,  1,  1,  2,  2,  2,  3,  3,  3 };
const double nTops_SR_hi[]   = { 1,  1,  1,  2,  2,  2,  3,  3,  3, -1, -1, -1 };

const double nbJets_SR_lo[]  = { 1,  2,  3,  1,  2,  3,  1,  2,  3,  1,  2,  3 };
const double nbJets_SR_hi[]  = { 2,  3, -1,  2,  3, -1,  2,  3, -1,  2,  3, -1 };
*/
const double nTops_SR_lo[]   = { 1,  1,  1,  2,  2,  2,  3,  3,  3 };
const double nTops_SR_hi[]   = { 2,  2,  2,  3,  3,  3, -1, -1, -1 };

const double nbJets_SR_lo[]  = { 1,  2,  3,  1,  2,  3,  1,  2,  3 };
const double nbJets_SR_hi[]  = { 2,  3, -1,  2,  3, -1,  2,  3, -1 };
/*
const std::string keyStr_nTops_SR[]  = { "EQ0", "EQ0", "EQ0", "EQ1", "EQ1", "EQ1", "EQ2", "EQ2", "EQ2", "LE3", "LE3", "LE3" };
const std::string disStr_nTops_SR[]  = {  "=0",  "=0",  "=0",  "=1",  "=1",  "=1",  "=2",  "=2",  "=2", ">=3", ">=3", ">=3" };

const std::string keyStr_nbJets_SR[] = { "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3" };
const std::string disStr_nbJets_SR[] = {  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3" };
*/
const std::string keyStr_nTops_SR[]  = { "EQ1", "EQ1", "EQ1", "EQ2", "EQ2", "EQ2", "LE3", "LE3", "LE3" };
const std::string disStr_nTops_SR[]  = {  "=1",  "=1",  "=1",  "=2",  "=2",  "=2", ">=3", ">=3", ">=3" };

const std::string keyStr_nbJets_SR[] = { "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3" };
const std::string disStr_nbJets_SR[] = {  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3" };

const int nSR = sizeof(nTops_SR_lo)/sizeof(nTops_SR_lo[0]);

const double met_SR_lo[] = { 200,  350,  400,  450,  500};
const double met_SR_hi[] = { 350,  400,  450,  500,   -1};

const std::string keyStr_met_SR[] = {     "200to350",     "350to400",     "400to450",     "450to500",    "LE500" };
const std::string disStr_met_SR[] = { "200<=MET<350", "350<=MET<400", "400<=MET<450", "450<=MET<500", "MET>=500" };
/* VERY crude binning on MET for the ntop=0 scenario. Not used for this time!
const std::vector<std::vector<double> > vMT2_vs_met_SR_0 { // nb=1, ntop=0
                                                           // MT2 lo 
                                                           {200, 200, 200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400, 500, 600},
                                                           // met hi 
                                                           {300, 400, 500, 600,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_1 { // nb=2, ntop=0
                                                           // MT2 lo 
                                                           {200, 200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400, 500},
                                                           // met hi 
                                                           {300, 400, 500,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_2 { // nb>=3, ntop=0
                                                           // MT2 lo 
                                                           {200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1},
                                                           // met lo 
                                                           {200, 300},
                                                           // met hi 
                                                           {300,  -1},
                                                         };
*/

const std::vector<std::vector<double> > vMT2_vs_met_SR_3 { // nb=1, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400, 400},
                                                           // MT2 hi 
                                                           {300, 300, 300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 275, 350, 450, /**/ 200, 275, 350, 450, /**/ 200, 350, 450},
                                                           // met hi 
                                                           {275, 350, 450,  -1, /**/ 275, 350, 450,  -1, /**/ 350, 450,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_4 { // nb=2, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400},
                                                           // MT2 hi 
                                                           {300, 300, 300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1},
                                                           // met lo 
                                                           {200, 275, 350, 450, /**/ 200, 275, 350, 450, /**/ 200, 450},
                                                           // met hi 
                                                           {275, 350, 450,  -1, /**/ 275, 350, 450,  -1, /**/ 450,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_5 { // nb>=3, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400},
                                                           // met hi 
                                                           {300, 400,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_6 { // nb=1, ntop=2
                                                           // MT2 lo 
                                                           {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
                                                           // MT2 hi 
                                                           {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
                                                           // met lo 
                                                           {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
                                                           // met hi 
                                                           {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_7 { // nb=2, ntop=2
                                                           // MT2 lo 
                                                           {200, 200, 200, /**/ 300, 300, 300, /**/ 400, 400},
                                                           // MT2 hi 
                                                           {300, 300, 300, /**/ 400, 400, 400, /**/  -1,  -1},
                                                           // met lo 
                                                           {200, 275, 350, /**/ 200, 275, 350, /**/ 200, 350},
                                                           // met hi 
                                                           {275, 350,  -1, /**/ 275, 350,  -1, /**/ 350,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_8 { // nb>=3, ntop=2
                                                           // MT2 lo 
                                                           {200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1},
                                                           // met lo 
                                                           {200, 300},
                                                           // met hi 
                                                           {300,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_9 { // nb=1, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_10 { // nb=2, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_11 { // nb>=3, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1}
                                                         };

//const std::vector<std::vector<std::vector<double> > > vMT2_vs_met_all_SR {vMT2_vs_met_SR_0, vMT2_vs_met_SR_1, vMT2_vs_met_SR_2, vMT2_vs_met_SR_3, vMT2_vs_met_SR_4, vMT2_vs_met_SR_5, vMT2_vs_met_SR_6, vMT2_vs_met_SR_7, vMT2_vs_met_SR_8, vMT2_vs_met_SR_9, vMT2_vs_met_SR_10, vMT2_vs_met_SR_11};
const std::vector<std::vector<std::vector<double> > > vMT2_vs_met_all_SR {vMT2_vs_met_SR_3, vMT2_vs_met_SR_4, vMT2_vs_met_SR_5, vMT2_vs_met_SR_6, vMT2_vs_met_SR_7, vMT2_vs_met_SR_8, vMT2_vs_met_SR_9, vMT2_vs_met_SR_10, vMT2_vs_met_SR_11};

const double max_MT2_for_binEdge = 400, max_met_for_binEdge = 600;
const double max_MT2_for_binCent = max_MT2_for_binEdge + 50, max_met_for_binCent = max_met_for_binEdge + 25;
const double pseudoMax_MT2_for_hist = max_MT2_for_binEdge + 100, pseudoMax_met_for_hist = max_met_for_binEdge + 50;

const double MT2_SR_Arr[] = {0, 200, 300, 400, 500};
const double met_SR_Arr[] = {200, 250, 300, 350, 400, 450, 500, 550, 600};

const int nSR_MT2 = sizeof(MT2_SR_Arr)/sizeof(MT2_SR_Arr[0])-1;
const int nSR_met = sizeof(met_SR_Arr)/sizeof(met_SR_Arr[0])-1;
const int nSR_met_alt = sizeof(met_SR_lo)/sizeof(met_SR_lo[0]);

std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning_forTH2Poly;

int nTotBins =0;

void build_MT2_met_Binning_forTH2Poly(std::vector<std::vector<std::vector<double> > > & outBinning){
   nTotBins = 0;
   outBinning.clear();
   for(unsigned int iSR =0; iSR<nSR; iSR++){
      std::vector<std::vector<double> > perBinning;
      std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
      for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++){
         double MT2_lo = vMT2_vs_met_per_SR[0].at(ib);
         double MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
         double met_lo = vMT2_vs_met_per_SR[2].at(ib);
         double met_hi = vMT2_vs_met_per_SR[3].at(ib);
         if( MT2_hi == -1 ) MT2_hi = pseudoMax_MT2_for_hist;
         if( met_hi == -1 ) met_hi = pseudoMax_met_for_hist;
         perBinning.push_back({met_lo, MT2_lo, met_hi, MT2_hi});
         nTotBins++;
      }
      outBinning.push_back(perBinning);
   }
   std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
}

void build_MT2_met_Binning(std::vector<std::vector<std::vector<double> > > & outBinning){
   nTotBins = 0;
   outBinning.clear();
   for(unsigned int iSR =0; iSR<nSR; iSR++){
      std::vector<std::vector<double> > perBinning;
      std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
      for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++){
         double MT2_lo = vMT2_vs_met_per_SR[0].at(ib);
         double MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
         double met_lo = vMT2_vs_met_per_SR[2].at(ib);
         double met_hi = vMT2_vs_met_per_SR[3].at(ib);
         perBinning.push_back({met_lo, MT2_lo, met_hi, MT2_hi});
         nTotBins++;
      }
      outBinning.push_back(perBinning);
   }
   std::cout<<"\n\nTotal search bins : "<<nTotBins<<std::endl<<std::endl;
}

int find_Binning_Index(int ibJet, int iTop, double MT2, double met){
   int idx = -1;
   int pickedIdx = -1;
   for(unsigned int iSR=0; iSR<nSR; iSR++){
      std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
      for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++){
         double MT2_lo = vMT2_vs_met_per_SR[0].at(ib);
         double MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
         double met_lo = vMT2_vs_met_per_SR[2].at(ib);
         double met_hi = vMT2_vs_met_per_SR[3].at(ib);
         idx++;
         if(   (nbJets_SR_lo[iSR] == -1 || ibJet >= nbJets_SR_lo[iSR])
            && (nbJets_SR_hi[iSR] == -1 || ibJet  < nbJets_SR_hi[iSR])
            && (nTops_SR_lo[iSR] == -1 || iTop >= nTops_SR_lo[iSR])
            && (nTops_SR_hi[iSR] == -1 || iTop  < nTops_SR_hi[iSR])
            && (MT2_lo == -1 || MT2 >= MT2_lo )
            && (MT2_hi == -1 || MT2  < MT2_hi )
            && (met_lo == -1 || met >= met_lo )
            && (met_hi == -1 || met  < met_hi ) ){
            pickedIdx = idx;
            break;
         }
      }
   }
   return pickedIdx;
}

struct searchBinDef{
   double bJet_lo, top_lo, met_lo, MT2_lo;
   double bJet_hi, top_hi, met_hi, MT2_hi;
   int idx_SR, idx_vMT2_vs_met;
};

void find_BinBoundaries(int inputIdx, searchBinDef & outBinDef){
   outBinDef.bJet_lo = -1; outBinDef.bJet_hi = -1;
   outBinDef.top_lo = -1; outBinDef.top_hi = -1;
   outBinDef.met_lo = -1; outBinDef.met_hi = -1;
   outBinDef.MT2_lo = -1; outBinDef.MT2_hi = -1;
   outBinDef.idx_SR = -1; outBinDef.idx_vMT2_vs_met = -1;
   int idx = -1;
   for(unsigned int iSR=0; iSR<nSR; iSR++){
      std::vector<std::vector<double> > vMT2_vs_met_per_SR = vMT2_vs_met_all_SR.at(iSR);
      for(unsigned int ib=0; ib<vMT2_vs_met_per_SR[0].size(); ib++){
         idx++;
         if( inputIdx == idx ){
            outBinDef.bJet_lo = nbJets_SR_lo[iSR]; outBinDef.bJet_hi = nbJets_SR_hi[iSR];
            outBinDef.top_lo = nTops_SR_lo[iSR]; outBinDef.top_hi = nTops_SR_hi[iSR];
            outBinDef.MT2_lo = vMT2_vs_met_per_SR[0].at(ib); outBinDef.MT2_hi = vMT2_vs_met_per_SR[1].at(ib);
            outBinDef.met_lo = vMT2_vs_met_per_SR[2].at(ib); outBinDef.met_hi = vMT2_vs_met_per_SR[3].at(ib);
            outBinDef.idx_SR = iSR; outBinDef.idx_vMT2_vs_met = ib;
            return;
         }
      }
   }
}

void print_searchBins(){
   std::vector<std::vector<std::vector<double> > > out_MT2_met_Binning;
   build_MT2_met_Binning(out_MT2_met_Binning);
   printf("%6s        %12s        %12s        %12s        %12s\n", "binIdx", "nTops", "nbJets", "MT2", "met");
   int preiSR = 0;
   for(int ib=0; ib<nTotBins; ib++){
      struct searchBinDef sbDef;
      find_BinBoundaries(ib, sbDef);
      if( preiSR != sbDef.idx_SR ){
         std::cout<<std::endl<<std::endl;
         preiSR = sbDef.idx_SR;
      }
      char tmpnbJetStr[20], tmpnTopStr[20], tmpMT2Str[20], tmpmetStr[20];
      if( sbDef.top_hi != -1 ) sprintf(tmpnTopStr, "=%1.0f", sbDef.top_lo); else sprintf(tmpnTopStr, ">=%1.0f", sbDef.top_lo);
      if( sbDef.bJet_hi != -1 ) sprintf(tmpnbJetStr, "=%1.0f", sbDef.bJet_lo); else sprintf(tmpnbJetStr, ">=%1.0f", sbDef.bJet_lo);
      if( sbDef.MT2_hi != -1 ) sprintf(tmpMT2Str, "[%3.0f, %3.0f)", sbDef.MT2_lo, sbDef.MT2_hi); else sprintf(tmpMT2Str, "[%3.0f, inf)", sbDef.MT2_lo);
      if( sbDef.met_hi != -1 ) sprintf(tmpmetStr, "[%3.0f, %3.0f)", sbDef.met_lo, sbDef.met_hi); else sprintf(tmpmetStr, "[%3.0f, inf)", sbDef.met_lo);
      printf("%6d        %12s        %12s        %12s        %12s\n", ib, tmpnTopStr, tmpnbJetStr, tmpMT2Str, tmpmetStr);
      if( sbDef.met_hi == -1 ) std::cout<<std::endl;
   }
}
