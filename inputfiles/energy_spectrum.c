{
	// this macro generates the energy spectrum of cosmic muons at a certain depth
	
	// depth in (hg*cm^-2)
	double depth=235.*2.65; 
	
	// spectral index
	double gamma=2.72;
	
	int nBins=1024;
	double Bin_min=0.; // energy in GeV
	double Bin_max=1000.; // energy in GeV
	double Bin_width=(Bin_max-Bin_min)/(nBins-1);
		
	TFormula* intensity = new TFormula("I","pow(x+[0]/[1]*(1-exp(-[1]*[2])),-([3]+1))");
	intensity->SetParameter(0,0.217);
	intensity->SetParameter(1,4.5e-4);
	intensity->SetParameter(2,depth);
	intensity->SetParameter(3,gamma);

	TH1D* histo = new TH1D("energy spectrum", ";energy (GeV); intensity (a.u.)", nBins, Bin_min, Bin_max);
	
	for (int i=1; i<=nBins; i++) {
		double energy=Bin_min+(i-1)*Bin_width;
		double I=intensity->Eval(energy);
		histo->SetBinContent(i,I);
	}
	
	TCanvas* c1 = new TCanvas("c1");
	c1->SetLogx();
	c1->SetLogy();
	histo->Draw();
	c1->SaveAs("energy_spectrum.root");
	c1->SaveAs("energy_spectrum.png");	
	
}
		

