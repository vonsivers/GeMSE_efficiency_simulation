{
	// this macro generates the zenith angle distribution of cosmic muons at a certain depth
	
	// depth in (m.w.e.)
	double depth=235.*2.65; 
	
	double pi=3.14159265;

	int nBins=1024;
	double Bin_min=0.;
	double Bin_max=pi/2.;
	double Bin_width=(Bin_max-Bin_min)/(nBins-1);
	
	double n = 1.53+8e-4*depth;
	
	TFormula* intensity = new TFormula("I","pow(cos(x),[0])");
	intensity->SetParameter(0,n);
		
	TH1D* histo = new TH1D("zenith angle", ";theta (rad); intensity (a.u.)", nBins, Bin_min, Bin_max);
	
	for (int i=1; i<=nBins; i++) {
		double theta=Bin_min+(i-1)*Bin_width;
		double I=intensity->Eval(theta);
		histo->SetBinContent(i,I);
	}
	
	TCanvas* c1 = new TCanvas("c1");
	histo->Draw();
	c1->SaveAs("zenith_angle.root");
	c1->SaveAs("zenith_angle.png");
	
	
}
		

