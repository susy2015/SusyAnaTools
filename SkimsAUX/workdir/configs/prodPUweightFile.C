void prodPUweightFile(const std::string fileName="puDistSpring11.root", const std::string inputTxtFile="flat10.txt", const int inputnBins = -1){

   ifstream fin(inputTxtFile.c_str(), ifstream::in);

   TFile *pufile = new TFile(fileName.c_str(), "RECREATE");

   std::vector<double> probVec;
   double perProb = 0, sumTot =0;
   cout<<"\nReading inputs from file : "<<inputTxtFile.c_str()<<endl;
   int cntBins =0;
   while(fin>>perProb){
      probVec.push_back(perProb);
      sumTot += perProb;
      cntBins ++;
      printf("%2dth  prob : %11.9f  sumTot : %11.9f\n", cntBins, perProb, sumTot);
   }
   Int_t nBins = probVec.size();
   cout<<"probVec.size : "<<probVec.size()<<endl;

   if( inputnBins != -1 ) nBins = inputnBins;
 
   cout<<"nBins : "<<nBins<<endl<<endl;
   
   TH1F *puhist = new TH1F("pileup", "pile up distribution", nBins, -0.5, float(nBins)-0.5); 

   for(int ibin = 1; ibin<nBins+1; ++ibin ) {
      if( ibin <= probVec.size() ){
         puhist->SetBinContent(ibin, probVec[ibin-1]);
      }
   }

   float deltaH = puhist->Integral();
   if( fabs(1.0 - deltaH) > 0.02 ){ //*OOPS*...
      puhist->Scale( 1.0/puhist->Integral() );
   }
   
   pufile->Write(); pufile->Close();  

}
