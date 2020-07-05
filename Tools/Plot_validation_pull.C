int Plot_validation_pull ()
{
	TCanvas* mycanvas = new TCanvas("mycanvas", "mycanvas", 600, 600);

	TFile *f1 = new TFile("plots/validation_pull.root");
	TH1F *h1 = (TH1F*)f1->Get("pull_h");
	int entry = h1->GetEntries();
	float mean = h1->GetMean();
	float STD = h1->GetStdDev() * std::sqrt((float)entry / (entry -1));
	std::cout << "Entries: " << entry << " mean = " << mean << " STD = " << STD << std::endl;

	h1->Draw();
	//h1->Fit("gaus");
	//TF1* myfunc = h1->GetFunction("gaus");
	//std::cout << "Fit: " << myfunc->GetParameter(0) << ", " << myfunc->GetParameter(1) << ", " << myfunc->GetParameter(2) << std::endl; 
	h1->GetYaxis()->SetRangeUser(0,10);

	std::stringstream s1;
	s1 << "#bf{Entries: mean=" << std::setprecision(2) << mean << std::setprecision(3) << " standard deviation=" << STD << "}";
	TString TS1 = s1.str();

	//std::stringstream s2;
	//s2 << "#bf{Fit: f(x) = " << std::setprecision(3) << myfunc->GetParameter(0) << "*exp(-0.5*((x-" << std::setprecision(2) << myfunc->GetParameter(1) << ")/" << std::setprecision(3) << myfunc->GetParameter(2) << ")^2)}";
	//TString TS2 = s2.str();

	TLatex latex;
	latex.SetTextSize(0.04);
	latex.SetNDC();
	latex.DrawLatex(0.15,0.85,TS1);
	//latex.DrawLatex(0.15,0.8,TS2);

	mycanvas->SaveAs("plots/validation_pull.png");
	return 0;
}
