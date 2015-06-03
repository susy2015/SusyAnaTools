const double nTops_SR_lo[]   = { 1,  1,  1,  2,  2,  2,  3,  3,  3 };
const double nTops_SR_hi[]   = { 2,  2,  2,  3,  3,  3, -1, -1, -1 };

const double nbJets_SR_lo[]  = { 1,  2,  3,  1,  2,  3,  1,  2,  3 };
const double nbJets_SR_hi[]  = { 2,  3, -1,  2,  3, -1,  2,  3, -1 };

const std::string keyStr_nTops_SR[]  = { "EQ1", "EQ1", "EQ1", "EQ2", "EQ2", "EQ2", "LE3", "LE3", "LE3" };
const std::string disStr_nTops_SR[]  = {  "=1",  "=1",  "=1",  "=2",  "=2",  "=2", ">=3", ">=3", ">=3" };

const std::string keyStr_nbJets_SR[] = { "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3", "EQ1", "EQ2", "LE3" };
const std::string disStr_nbJets_SR[] = {  "=1",  "=2", ">=3",  "=1",  "=2", ">=3",  "=1",  "=2", ">=3" };

const int nSR = sizeof(nTops_SR_lo)/sizeof(nTops_SR_lo[0]);

const double met_SR_lo[] = { 200,  350,  400,  450,  500};
const double met_SR_hi[] = { 350,  400,  450,  500,   -1};

const std::string keyStr_met_SR[] = {     "200to350",     "350to400",     "400to450",     "450to500",    "LE500" };
const std::string disStr_met_SR[] = { "200<=MET<350", "350<=MET<400", "400<=MET<450", "450<=MET<500", "MET>=500" };

const std::vector<std::vector<double> > vMT2_vs_met_SR_0 { // nb=1, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200, 200, /**/ 300, 300, 300, 300, 300, /**/ 400, 400, 400},
                                                           // MT2 hi 
                                                           {300, 300, 300, 300, /**/ 400, 400, 400, 400, 400, /**/  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 350, 400, /**/ 200, 250, 300, 350, 400, /**/ 200, 350, 450},
                                                           // met hi 
                                                           {300, 350, 400,  -1, /**/ 250, 300, 350, 400,  -1, /**/ 350, 450,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_1 { // nb=2, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200, /**/ 300, 300, 300, 300, 300, /**/ 400, 400, 400}, 
                                                           // MT2 hi 
                                                           {300, 300, 300, /**/ 400, 400, 400, 400, 400, /**/  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 400, /**/ 200, 250, 300, 350, 400, /**/ 200, 350, 450},
                                                           // met hi 
                                                           {300, 400,  -1, /**/ 250, 300, 350, 400,  -1, /**/ 350, 450,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_2 { // nb>=3, ntop=1
                                                           // MT2 lo 
                                                           {200, 200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 300, 375},
                                                           // met hi 
                                                           {300, 375,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_3 { // nb=1, ntop=2
                                                           // MT2 lo 
                                                           {200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400}, 
                                                           // MT2 hi 
                                                           {300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1},
                                                           // met lo 
                                                           {200, 350, /**/ 200, 250, 300, 350, /**/ 200, 350},
                                                           // met hi 
                                                           {350,  -1, /**/ 250, 300, 350,  -1, /**/ 350,  -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_4 { // nb=2, ntop=2
                                                           // MT2 lo 
                                                           {200, 200, /**/ 300, 300, 300, 300, /**/ 400, 400, 400}, 
                                                           // MT2 hi 
                                                           {300, 300, /**/ 400, 400, 400, 400, /**/  -1,  -1,  -1},
                                                           // met lo 
                                                           {200, 350, /**/ 200, 250, 300, 350, /**/ 200, 350, 450},
                                                           // met hi 
                                                           {350,  -1, /**/ 250, 300, 350,  -1, /**/ 350, 450,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_5 { // nb>=3, ntop=2
                                                           // MT2 lo 
                                                           {200, 200},
                                                           // MT2 hi 
                                                           { -1,  -1},
                                                           // met lo 
                                                           {200, 350},
                                                           // met hi 
                                                           {350,  -1}
                                                         };


const std::vector<std::vector<double> > vMT2_vs_met_SR_6 { // nb=1, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1}
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_7 { // nb=2, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1},
                                                         };

const std::vector<std::vector<double> > vMT2_vs_met_SR_8 { // nb>=3, ntop=3
                                                           // MT2 lo 
                                                           {200},
                                                           // MT2 hi 
                                                           { -1},
                                                           // met lo 
                                                           {200},
                                                           // met hi 
                                                           { -1}
                                                         };

const std::vector<std::vector<std::vector<double> > > vMT2_vs_met_all_SR {vMT2_vs_met_SR_0, vMT2_vs_met_SR_1, vMT2_vs_met_SR_2, vMT2_vs_met_SR_3, vMT2_vs_met_SR_4, vMT2_vs_met_SR_5, vMT2_vs_met_SR_6, vMT2_vs_met_SR_7, vMT2_vs_met_SR_8};

const double max_MT2_for_binEdge = 400, max_met_for_binEdge = 450;
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
