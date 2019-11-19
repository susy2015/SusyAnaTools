int TTree_read_test()
{
	TFile *f1 = new TFile("/eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_25Apr2019_v4_fastsimv4/SMS_T2fbd_fastsim_2016/SMS_T2fbd_mStop600_mLSP520_fastsim_2016.root");
	TTree *MyTree;
	f1->GetObject("Events",MyTree);
	//MyTree->Draw("SigWTab_SigWeightGenCutPass");
	MyTree->Draw("SigWTab_SigWeightGenCut");
	TH1F *h1 = (TH1F*)gPad->GetPrimitive("htemp");
	std::cout << "h1->Integral() " << h1->Integral() << std::endl;

	int nBins = h1->GetSize()-2;
	float integral = 0;
	float mean = 0;
	float area = 0;
	for (int i=1; i <= nBins; i++)
	{
		float binc = h1->GetBinContent(i);
		//float binx = h1->GetXaxis()->GetBinLowEdge(i);
		float binx = h1->GetXaxis()->GetBinCenter(i);
		integral = integral + binc;
		mean = mean + binx;
		area = area + binc * binx;
		std::cout << "bin " << i << "(x=" << binx << ") " << binc <<std::endl;
	}
	std::cout << "integral = " << integral << std::endl;
	std::cout << "mean = " << mean/nBins << std::endl;
	std::cout << "area = " << area << std::endl;

	return 0;
}
