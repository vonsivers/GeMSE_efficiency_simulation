#include "HPGeAnalysis.hh"
#include <fstream>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeAnalysis::HPGeAnalysis()
{
    fhTotEdep = new TH1D("hTotEdep",";Energy (keV);Counts",30000,0.,3000.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeAnalysis::~HPGeAnalysis()
{
    delete fhTotEdep;
	fenergy.clear();
	fBR.clear();
	fBR_err.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// --------------------------------
// get BR for all lines
// --------------------------------

void HPGeAnalysis::CalcBR() {
    
    fnlines = fenergy.size();

	// open results file
    ofstream results_file;
    results_file.open(fFileName);

	// write header
	results_file << "Energy (keV)" << "\t" << "Branching Ratio" << "\t" << "Branching Ratio Error" << "\n";
    
    // loop over all gamma lines
    for (int i=0; i<fnlines; ++i) {
        
        double energy = fenergy[i];
    
        // counts in peak
        double bin_min_peak = fhTotEdep->FindBin(energy-0.1);
        double bin_max_peak = fhTotEdep->FindBin(energy+0.1);
        double sum_peak=fhTotEdep->Integral(bin_min_peak,bin_max_peak);
        double sum_peak_err=sqrt(sum_peak);
     
        double BR = sum_peak/fNEvents;
        double BR_err = sum_peak_err/fNEvents;
    
		// save to file
        results_file << energy << "\t" << BR << "\t" << BR_err << "\n"; 
        
    }

    results_file.close();
}


// --------------------------------
// clear all data
// --------------------------------

void HPGeAnalysis::Clear() {

	fenergy.clear();
	fBR.clear();
	fBR_err.clear();

}
    




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

