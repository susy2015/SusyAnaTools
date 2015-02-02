   double sumSM_passLeptVeto_WeightedScaledMC =0, sumSM_passLeptVeto_WeightedErrorScaledMC =0;
   double sumSM_passnJets_WeightedScaledMC =0, sumSM_passnJets_WeightedErrorScaledMC =0;
   double sumSM_passdPhis_WeightedScaledMC =0, sumSM_passdPhis_WeightedErrorScaledMC =0;
   double sumSM_passBJets_WeightedScaledMC =0, sumSM_passBJets_WeightedErrorScaledMC =0;
   double sumSM_passTagger_WeightedScaledMC =0, sumSM_passTagger_WeightedErrorScaledMC =0;
   double sumSM_passMET_WeightedScaledMC =0, sumSM_passMET_WeightedErrorScaledMC =0;
   double sumSM_passBaseline_WeightedScaledMC =0, sumSM_passBaseline_WeightedErrorScaledMC =0;

// TTJets
   std::cout<<"\nProcessing TTJets ... "<<std::endl;
   treeVec.clear(); subSampleKeysVec.clear();
   TChain *TTJets_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesTTJetsVec.size(); in++){ TTJets_AUX->Add(filesTTJetsVec.at(in).c_str()); }
   treeVec.push_back(TTJets_AUX); subSampleKeysVec.push_back("TTbar");
   anaFunc(tr, treeVec, subSampleKeysVec, "TTbar");

   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

   sumSM_passLeptVeto_WeightedScaledMC += cnt_passLeptVeto_WeightedScaledMC; sumSM_passLeptVeto_WeightedErrorScaledMC += cnt_passLeptVeto_WeightedErrorScaledMC * cnt_passLeptVeto_WeightedErrorScaledMC;
   sumSM_passnJets_WeightedScaledMC += cnt_passnJets_WeightedScaledMC; sumSM_passnJets_WeightedErrorScaledMC += cnt_passnJets_WeightedErrorScaledMC * cnt_passnJets_WeightedErrorScaledMC;
   sumSM_passdPhis_WeightedScaledMC += cnt_passdPhis_WeightedScaledMC; sumSM_passdPhis_WeightedErrorScaledMC += cnt_passdPhis_WeightedErrorScaledMC * cnt_passdPhis_WeightedErrorScaledMC;
   sumSM_passBJets_WeightedScaledMC += cnt_passBJets_WeightedScaledMC; sumSM_passBJets_WeightedErrorScaledMC += cnt_passBJets_WeightedErrorScaledMC * cnt_passBJets_WeightedErrorScaledMC;
   sumSM_passTagger_WeightedScaledMC += cnt_passTagger_WeightedScaledMC; sumSM_passTagger_WeightedErrorScaledMC += cnt_passTagger_WeightedErrorScaledMC * cnt_passTagger_WeightedErrorScaledMC;
   sumSM_passMET_WeightedScaledMC += cnt_passMET_WeightedScaledMC; sumSM_passMET_WeightedErrorScaledMC += cnt_passMET_WeightedErrorScaledMC * cnt_passMET_WeightedErrorScaledMC;
   sumSM_passBaseline_WeightedScaledMC += cnt_passBaseline_WeightedScaledMC; sumSM_passBaseline_WeightedErrorScaledMC += cnt_passBaseline_WeightedErrorScaledMC * cnt_passBaseline_WeightedErrorScaledMC;

// WJetsToLNu
   std::cout<<"\nProcessing WJetsToLNu ... "<<std::endl;
   treeVec.clear(); subSampleKeysVec.clear();
 
   TChain *WJetsToLNu_HT_600toInf_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesWJetsToLNu_HT_600toInfVec.size(); in++){ WJetsToLNu_HT_600toInf_AUX->Add(filesWJetsToLNu_HT_600toInfVec.at(in).c_str()); }
   treeVec.push_back(WJetsToLNu_HT_600toInf_AUX); subSampleKeysVec.push_back("WJetsToLNu_HT_600toInf");
 
   TChain *WJetsToLNu_HT_400to600_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesWJetsToLNu_HT_400to600Vec.size(); in++){ WJetsToLNu_HT_400to600_AUX->Add(filesWJetsToLNu_HT_400to600Vec.at(in).c_str()); }
   treeVec.push_back(WJetsToLNu_HT_400to600_AUX); subSampleKeysVec.push_back("WJetsToLNu_HT_400to600");
 
   TChain *WJetsToLNu_HT_200to400_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesWJetsToLNu_HT_200to400Vec.size(); in++){ WJetsToLNu_HT_200to400_AUX->Add(filesWJetsToLNu_HT_200to400Vec.at(in).c_str()); }
   treeVec.push_back(WJetsToLNu_HT_200to400_AUX); subSampleKeysVec.push_back("WJetsToLNu_HT_200to400");
 
   TChain *WJetsToLNu_HT_100to200_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesWJetsToLNu_HT_100to200Vec.size(); in++){ WJetsToLNu_HT_100to200_AUX->Add(filesWJetsToLNu_HT_100to200Vec.at(in).c_str()); }
   treeVec.push_back(WJetsToLNu_HT_100to200_AUX); subSampleKeysVec.push_back("WJetsToLNu_HT_100to200");
   anaFunc(tr, treeVec, subSampleKeysVec, "WJetsToLNu");
 
   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

   sumSM_passLeptVeto_WeightedScaledMC += cnt_passLeptVeto_WeightedScaledMC; sumSM_passLeptVeto_WeightedErrorScaledMC += cnt_passLeptVeto_WeightedErrorScaledMC * cnt_passLeptVeto_WeightedErrorScaledMC;
   sumSM_passnJets_WeightedScaledMC += cnt_passnJets_WeightedScaledMC; sumSM_passnJets_WeightedErrorScaledMC += cnt_passnJets_WeightedErrorScaledMC * cnt_passnJets_WeightedErrorScaledMC;
   sumSM_passdPhis_WeightedScaledMC += cnt_passdPhis_WeightedScaledMC; sumSM_passdPhis_WeightedErrorScaledMC += cnt_passdPhis_WeightedErrorScaledMC * cnt_passdPhis_WeightedErrorScaledMC;
   sumSM_passBJets_WeightedScaledMC += cnt_passBJets_WeightedScaledMC; sumSM_passBJets_WeightedErrorScaledMC += cnt_passBJets_WeightedErrorScaledMC * cnt_passBJets_WeightedErrorScaledMC;
   sumSM_passTagger_WeightedScaledMC += cnt_passTagger_WeightedScaledMC; sumSM_passTagger_WeightedErrorScaledMC += cnt_passTagger_WeightedErrorScaledMC * cnt_passTagger_WeightedErrorScaledMC;
   sumSM_passMET_WeightedScaledMC += cnt_passMET_WeightedScaledMC; sumSM_passMET_WeightedErrorScaledMC += cnt_passMET_WeightedErrorScaledMC * cnt_passMET_WeightedErrorScaledMC;
   sumSM_passBaseline_WeightedScaledMC += cnt_passBaseline_WeightedScaledMC; sumSM_passBaseline_WeightedErrorScaledMC += cnt_passBaseline_WeightedErrorScaledMC * cnt_passBaseline_WeightedErrorScaledMC;

// ZJetsToNuNu
   std::cout<<"\nProcessing ZJetsToNuNu ... "<<std::endl;                                                                                                                                      treeVec.clear(); subSampleKeysVec.clear();
                                                                                                                                                                                               TChain *ZJetsToNuNu_HT_600toInf_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesZJetsToNuNu_HT_600toInfVec.size(); in++){ ZJetsToNuNu_HT_600toInf_AUX->Add(filesZJetsToNuNu_HT_600toInfVec.at(in).c_str()); }
   treeVec.push_back(ZJetsToNuNu_HT_600toInf_AUX); subSampleKeysVec.push_back("ZJetsToNuNu_HT_600toInf");
                                                                                                                                                                                               TChain *ZJetsToNuNu_HT_400to600_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesZJetsToNuNu_HT_400to600Vec.size(); in++){ ZJetsToNuNu_HT_400to600_AUX->Add(filesZJetsToNuNu_HT_400to600Vec.at(in).c_str()); }
   treeVec.push_back(ZJetsToNuNu_HT_400to600_AUX); subSampleKeysVec.push_back("ZJetsToNuNu_HT_400to600");
                                                                                                                                                                                               TChain *ZJetsToNuNu_HT_200to400_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesZJetsToNuNu_HT_200to400Vec.size(); in++){ ZJetsToNuNu_HT_200to400_AUX->Add(filesZJetsToNuNu_HT_200to400Vec.at(in).c_str()); }
   treeVec.push_back(ZJetsToNuNu_HT_200to400_AUX); subSampleKeysVec.push_back("ZJetsToNuNu_HT_200to400");
                                                                                                                                                                                               TChain *ZJetsToNuNu_HT_100to200_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesZJetsToNuNu_HT_100to200Vec.size(); in++){ ZJetsToNuNu_HT_100to200_AUX->Add(filesZJetsToNuNu_HT_100to200Vec.at(in).c_str()); }
   treeVec.push_back(ZJetsToNuNu_HT_100to200_AUX); subSampleKeysVec.push_back("ZJetsToNuNu_HT_100to200");
   anaFunc(tr, treeVec, subSampleKeysVec, "ZJetsToNuNu");

   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();                     

   sumSM_passLeptVeto_WeightedScaledMC += cnt_passLeptVeto_WeightedScaledMC; sumSM_passLeptVeto_WeightedErrorScaledMC += cnt_passLeptVeto_WeightedErrorScaledMC * cnt_passLeptVeto_WeightedErrorScaledMC;
   sumSM_passnJets_WeightedScaledMC += cnt_passnJets_WeightedScaledMC; sumSM_passnJets_WeightedErrorScaledMC += cnt_passnJets_WeightedErrorScaledMC * cnt_passnJets_WeightedErrorScaledMC;
   sumSM_passdPhis_WeightedScaledMC += cnt_passdPhis_WeightedScaledMC; sumSM_passdPhis_WeightedErrorScaledMC += cnt_passdPhis_WeightedErrorScaledMC * cnt_passdPhis_WeightedErrorScaledMC;
   sumSM_passBJets_WeightedScaledMC += cnt_passBJets_WeightedScaledMC; sumSM_passBJets_WeightedErrorScaledMC += cnt_passBJets_WeightedErrorScaledMC * cnt_passBJets_WeightedErrorScaledMC;
   sumSM_passTagger_WeightedScaledMC += cnt_passTagger_WeightedScaledMC; sumSM_passTagger_WeightedErrorScaledMC += cnt_passTagger_WeightedErrorScaledMC * cnt_passTagger_WeightedErrorScaledMC;
   sumSM_passMET_WeightedScaledMC += cnt_passMET_WeightedScaledMC; sumSM_passMET_WeightedErrorScaledMC += cnt_passMET_WeightedErrorScaledMC * cnt_passMET_WeightedErrorScaledMC;
   sumSM_passBaseline_WeightedScaledMC += cnt_passBaseline_WeightedScaledMC; sumSM_passBaseline_WeightedErrorScaledMC += cnt_passBaseline_WeightedErrorScaledMC * cnt_passBaseline_WeightedErrorScaledMC;

// QCD
   std::cout<<"\nProcessing QCD ... "<<std::endl;
   treeVec.clear(); subSampleKeysVec.clear();
 
   TChain *QCD_HT_1000toInf_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesQCD_HT_1000toInfVec.size(); in++){ QCD_HT_1000toInf_AUX->Add(filesQCD_HT_1000toInfVec.at(in).c_str()); }
   treeVec.push_back(QCD_HT_1000toInf_AUX); subSampleKeysVec.push_back("QCD_HT_1000toInf");
 
   TChain *QCD_HT_500to1000_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesQCD_HT_500to1000Vec.size(); in++){ QCD_HT_500to1000_AUX->Add(filesQCD_HT_500to1000Vec.at(in).c_str()); }
   treeVec.push_back(QCD_HT_500to1000_AUX); subSampleKeysVec.push_back("QCD_HT_500to1000");
 
   TChain *QCD_HT_250to500_AUX = new TChain(treeStrTtype+"/AUX");
   for(unsigned int in=0; in<filesQCD_HT_250to500Vec.size(); in++){ QCD_HT_250to500_AUX->Add(filesQCD_HT_250to500Vec.at(in).c_str()); }
   treeVec.push_back(QCD_HT_250to500_AUX); subSampleKeysVec.push_back("QCD_HT_250to500");
   anaFunc(tr, treeVec, subSampleKeysVec, "QCD");

   std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();

   sumSM_passLeptVeto_WeightedScaledMC += cnt_passLeptVeto_WeightedScaledMC; sumSM_passLeptVeto_WeightedErrorScaledMC += cnt_passLeptVeto_WeightedErrorScaledMC * cnt_passLeptVeto_WeightedErrorScaledMC;
   sumSM_passnJets_WeightedScaledMC += cnt_passnJets_WeightedScaledMC; sumSM_passnJets_WeightedErrorScaledMC += cnt_passnJets_WeightedErrorScaledMC * cnt_passnJets_WeightedErrorScaledMC;
   sumSM_passdPhis_WeightedScaledMC += cnt_passdPhis_WeightedScaledMC; sumSM_passdPhis_WeightedErrorScaledMC += cnt_passdPhis_WeightedErrorScaledMC * cnt_passdPhis_WeightedErrorScaledMC;
   sumSM_passBJets_WeightedScaledMC += cnt_passBJets_WeightedScaledMC; sumSM_passBJets_WeightedErrorScaledMC += cnt_passBJets_WeightedErrorScaledMC * cnt_passBJets_WeightedErrorScaledMC;
   sumSM_passTagger_WeightedScaledMC += cnt_passTagger_WeightedScaledMC; sumSM_passTagger_WeightedErrorScaledMC += cnt_passTagger_WeightedErrorScaledMC * cnt_passTagger_WeightedErrorScaledMC;
   sumSM_passMET_WeightedScaledMC += cnt_passMET_WeightedScaledMC; sumSM_passMET_WeightedErrorScaledMC += cnt_passMET_WeightedErrorScaledMC * cnt_passMET_WeightedErrorScaledMC;
   sumSM_passBaseline_WeightedScaledMC += cnt_passBaseline_WeightedScaledMC; sumSM_passBaseline_WeightedErrorScaledMC += cnt_passBaseline_WeightedErrorScaledMC * cnt_passBaseline_WeightedErrorScaledMC;

   sumSM_passLeptVeto_WeightedErrorScaledMC = sqrt(sumSM_passLeptVeto_WeightedErrorScaledMC);
   sumSM_passnJets_WeightedErrorScaledMC = sqrt(sumSM_passnJets_WeightedErrorScaledMC);
   sumSM_passdPhis_WeightedErrorScaledMC = sqrt(sumSM_passdPhis_WeightedErrorScaledMC);
   sumSM_passBJets_WeightedErrorScaledMC = sqrt(sumSM_passBJets_WeightedErrorScaledMC);
   sumSM_passTagger_WeightedErrorScaledMC = sqrt(sumSM_passTagger_WeightedErrorScaledMC);
   sumSM_passMET_WeightedErrorScaledMC = sqrt(sumSM_passMET_WeightedErrorScaledMC);
   sumSM_passBaseline_WeightedErrorScaledMC = sqrt(sumSM_passBaseline_WeightedErrorScaledMC);

   std::cout<<"\nFor cutFlow ..."<<std::endl;
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passLeptVeto", sumSM_passLeptVeto_WeightedScaledMC, sumSM_passLeptVeto_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passnJets", sumSM_passnJets_WeightedScaledMC, sumSM_passnJets_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passdPhis", sumSM_passdPhis_WeightedScaledMC, sumSM_passdPhis_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passBJets", sumSM_passBJets_WeightedScaledMC, sumSM_passBJets_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passMET", sumSM_passMET_WeightedScaledMC, sumSM_passMET_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passTagger", sumSM_passTagger_WeightedScaledMC, sumSM_passTagger_WeightedErrorScaledMC);
   printf("%30s   sumSM : %10.5f +- %9.5f\n", "passBaseline", sumSM_passBaseline_WeightedScaledMC, sumSM_passBaseline_WeightedErrorScaledMC);
