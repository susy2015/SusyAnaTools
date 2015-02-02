// Signal T1tttt_mGluino1200_mLSP800
  std::cout<<"\n\nProcessing T1tttt_mGluino1200_mLSP800 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T1tttt_mGluino1200_mLSP800_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T1tttt_mGluino1200_mLSP800Vec.size(); in++){ Signal_T1tttt_mGluino1200_mLSP800_AUX->Add(filesSignal_T1tttt_mGluino1200_mLSP800Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T1tttt_mGluino1200_mLSP800_AUX); subSampleKeysVec.push_back("Signal_T1tttt_mGluino1200_mLSP800");
  anaFunc(tr, treeVec, subSampleKeysVec, "T1tttt_mGluino1200_mLSP800");

// Signal T1tttt_mGluino1500_mLSP100
  std::cout<<"\n\nProcessing T1tttt_mGluino1500_mLSP100 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T1tttt_mGluino1500_mLSP100_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T1tttt_mGluino1500_mLSP100Vec.size(); in++){ Signal_T1tttt_mGluino1500_mLSP100_AUX->Add(filesSignal_T1tttt_mGluino1500_mLSP100Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T1tttt_mGluino1500_mLSP100_AUX); subSampleKeysVec.push_back("Signal_T1tttt_mGluino1500_mLSP100");
  anaFunc(tr, treeVec, subSampleKeysVec, "T1tttt_mGluino1500_mLSP100");

// Signal T5tttt_mGluino1300_mStop300_mChi280
  std::cout<<"\n\nProcessing T5tttt_mGluino1300_mStop300_mChi280 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T5tttt_mGluino1300_mStop300_mChi280_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T5tttt_mGluino1300_mStop300_mChi280Vec.size(); in++){ Signal_T5tttt_mGluino1300_mStop300_mChi280_AUX->Add(filesSignal_T5tttt_mGluino1300_mStop300_mChi280Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T5tttt_mGluino1300_mStop300_mChi280_AUX); subSampleKeysVec.push_back("Signal_T5tttt_mGluino1300_mStop300_mChi280");
  anaFunc(tr, treeVec, subSampleKeysVec, "T5tttt_mGluino1300_mStop300_mChi280");

// Signal T5tttt_mGluino1300_mStop300_mCh285
  std::cout<<"\n\nProcessing T5tttt_mGluino1300_mStop300_mCh285 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T5tttt_mGluino1300_mStop300_mCh285_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T5tttt_mGluino1300_mStop300_mCh285Vec.size(); in++){ Signal_T5tttt_mGluino1300_mStop300_mCh285_AUX->Add(filesSignal_T5tttt_mGluino1300_mStop300_mCh285Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T5tttt_mGluino1300_mStop300_mCh285_AUX); subSampleKeysVec.push_back("Signal_T5tttt_mGluino1300_mStop300_mCh285");
  anaFunc(tr, treeVec, subSampleKeysVec, "T5tttt_mGluino1300_mStop300_mCh285");

// Signal T2tt_mStop650_mLSP325
  std::cout<<"\n\nProcessing T2tt_mStop650_mLSP325 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T2tt_mStop650_mLSP325_AUX= new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T2tt_mStop650_mLSP325Vec.size(); in++){ Signal_T2tt_mStop650_mLSP325_AUX->Add(filesSignal_T2tt_mStop650_mLSP325Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T2tt_mStop650_mLSP325_AUX); subSampleKeysVec.push_back("Signal_T2tt_mStop650_mLSP325");
  anaFunc(tr, treeVec, subSampleKeysVec, "T2tt_mStop650_mLSP325");

// Signal T2tt_mStop850_mLSP100
  std::cout<<"\n\nProcessing T2tt_mStop850_mLSP100 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T2tt_mStop850_mLSP100_AUX= new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T2tt_mStop850_mLSP100Vec.size(); in++){ Signal_T2tt_mStop850_mLSP100_AUX->Add(filesSignal_T2tt_mStop850_mLSP100Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T2tt_mStop850_mLSP100_AUX); subSampleKeysVec.push_back("Signal_T2tt_mStop850_mLSP100");
  anaFunc(tr, treeVec, subSampleKeysVec, "T2tt_mStop850_mLSP100");

// Signal T1bbbb_mGluino1000_mLSPr90
  std::cout<<"\n\nProcessing T1bbbb_mGluino1000_mLSP900 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T1bbbb_mGluino1000_mLSP900_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T1bbbb_mGluino1000_mLSP900Vec.size(); in++){ Signal_T1bbbb_mGluino1000_mLSP900_AUX->Add(filesSignal_T1bbbb_mGluino1000_mLSP900Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T1bbbb_mGluino1000_mLSP900_AUX); subSampleKeysVec.push_back("Signal_T1bbbb_mGluino1000_mLSP900");
  anaFunc(tr, treeVec, subSampleKeysVec, "T1bbbb_mGluino1000_mLSP900");

// Signal T1bbbb_mGluino1500_mLSP100
  std::cout<<"\n\nProcessing T1bbbb_mGluino1500_mLSP100 ... "<<std::endl;
  treeVec.clear(); subSampleKeysVec.clear();
  TChain *Signal_T1bbbb_mGluino1500_mLSP100_AUX = new TChain(treeStrTtype+"/AUX");
  for(unsigned int in=0; in<filesSignal_T1bbbb_mGluino1500_mLSP100Vec.size(); in++){ Signal_T1bbbb_mGluino1500_mLSP100_AUX->Add(filesSignal_T1bbbb_mGluino1500_mLSP100Vec.at(in).c_str()); }
  treeVec.push_back(Signal_T1bbbb_mGluino1500_mLSP100_AUX); subSampleKeysVec.push_back("Signal_T1bbbb_mGluino1500_mLSP100");
  anaFunc(tr, treeVec, subSampleKeysVec, "T1bbbb_mGluino1500_mLSP100");
