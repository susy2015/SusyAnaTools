int Plot_validation_pull ()
{
	std::vector<float> bin_pull = {-0.637136, 0.727349, -0.0491864, -0.417769, -0.137546, -0.0858171, 0.185886, -0.544368, -0.508514, 1.2266, -0.156754, -0.932761, 1.48942, 1.32989, -0.617128, 0.423993, -0.607685, 0.047438, 1.11989, -0.237689, 1.28692, 0.814612, 0.60139, 1.49343, 0.596995, 0.557921, 1.36671, -0.904551, -1.50348, 0.542772, -1.6513, 0.212522, -0.391821, -0.448618, -1.71287, -0.582617, -0.290525, -0.0765329, 0.261278, 0.108228, -0.980814, -0.66172, 1.38372};

	int entry = bin_pull.size();
	float sum = 0;
	float mean = 0;
	float STD = 0;
	for (int i=0; i < entry ; i++)
	{
		sum = sum + bin_pull.at(i);
	}
	mean = sum / entry;
	for (int i=0; i < entry ; i++)
	{
		STD = STD + (bin_pull.at(i)-mean)*(bin_pull.at(i)-mean);
	}
	STD = std::sqrt(STD/(entry-1));
	std::cout << "Entries: " << entry << " mean = " << mean << " STD = " << STD << std::endl;

	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);

	TFile *f1 = new TFile("/uscms_data/d3/huiwang/CMSSW_10_2_9/src/SusyAnaTools/Tools/plots/validation_pull.root");
	TH1F *h1 = (TH1F*)f1->Get("pull_h");

	h1->Fit("gaus");
	TF1* myfunc = h1->GetFunction("gaus");
	std::cout << "Fit: " << myfunc->GetParameter(0) << ", " << myfunc->GetParameter(1) << ", " << myfunc->GetParameter(2) << std::endl; 
	h1->GetYaxis()->SetRangeUser(0,15);

	std::stringstream s1;
	s1 << "#bf{Entries: mean=" << std::setprecision(2) << mean << std::setprecision(3) << " standard deviation=" << STD << "}";
	//std::cout << ss.str() << std::endl;
	TString TS1 = s1.str();

	std::stringstream s2;
	//s2 << "Fit: mean = " << std::setprecision(2) << myfunc->GetParameter(1) << std::setprecision(3) << ",  \\sigma = " << myfunc->GetParameter(2);
	s2 << "#bf{Fit: f(x) = " << std::setprecision(3) << myfunc->GetParameter(0) << "*exp(-0.5*((x-" << std::setprecision(2) << myfunc->GetParameter(1) << ")/" << std::setprecision(3) << myfunc->GetParameter(2) << ")^2)}";
	//std::cout << s2.str() << std::endl;
	TString TS2 = s2.str();

	TLatex latex;
	latex.SetTextSize(0.04);
	latex.SetNDC();
	latex.DrawLatex(0.15,0.85,TS1);
	latex.DrawLatex(0.15,0.8,TS2);

	mycanvas->SaveAs("plots/validation_pull_fit.png");
	return 0;
}
