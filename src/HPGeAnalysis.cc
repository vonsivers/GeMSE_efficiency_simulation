#include "HPGeAnalysis.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeAnalysis::HPGeAnalysis()
{
    fhTotEdep = new TH1D("hTotEdep",";Energy (keV);Counts",30000,0.,3000.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeAnalysis::~HPGeAnalysis()
{
    delete fhTotEdep;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// --------------------------------
// get efficiency for single line
// --------------------------------

void HPGeAnalysis::CalcEfficiencies() {
    
    fnlines = fenergy.size();
    
    // loop over all gamma lines
    for (int i=0; i<fnlines; ++i) {
        
        G4double energy = fenergy[i];
        G4double width_sig = fwidth_sig[i];
        G4double width_bck = fwidth_bkg[i];
        G4double BR_NuDat = fNuDatBR[i];
        G4double BR_G4 = fG4BR[i];
    
        // counts in peak
        G4double bin_min_peak = fhTotEdep->FindBin(energy-width_sig/2.);
        G4double bin_max_peak = fhTotEdep->FindBin(energy+width_sig/2.);
        G4double sum_peak=fhTotEdep->Integral(bin_min_peak,bin_max_peak);
        G4double sum_peak_err=sqrt(sum_peak);
        G4int nBins_peak=bin_max_peak-bin_min_peak+1;
        
        
        // calculate counts in left bck region
        G4double bin_min_bckleft = fhTotEdep->FindBin(energy-width_bck/2.-width_sig/2.);
        G4double bin_max_bckleft = fhTotEdep->FindBin(energy-width_bck/2.)-1;
        G4double sum_bckleft=fhTotEdep->Integral(bin_min_bckleft,bin_max_bckleft);
        G4double sum_bckleft_err=sqrt(sum_bckleft);
        G4int nBins_bckleft=bin_max_bckleft-bin_min_bckleft+1;
        
        
        // calculate counts in right bck region
        G4double bin_min_bckright = fhTotEdep->FindBin(energy+width_sig/2.)+1;
        G4double bin_max_bckright = fhTotEdep->FindBin(energy+width_sig/2.+width_bck/2.);
        G4double sum_bckright=fhTotEdep->Integral(bin_min_bckright,bin_max_bckright);
        G4double sum_bckright_err=sqrt(sum_bckright);
        G4int nBins_bckright=bin_max_bckright-bin_min_bckright+1;
        
        // substract bck counts from peak
        G4double f_bckleft = nBins_peak/2. / nBins_bckleft;
        G4double f_bckright = nBins_peak/2. / nBins_bckright;
        
        G4double counts = sum_peak - f_bckleft*sum_bckleft - f_bckright*sum_bckright;
        G4double counts_err = sqrt(sum_peak_err*sum_peak_err + f_bckleft*sum_bckleft_err*sum_bckleft_err + f_bckright*sum_bckright_err*sum_bckright_err);
        
        G4double efficiency = counts/fNEvents/BR_G4;
        G4double efficiency_err = counts_err/fNEvents/BR_G4;
    
        fefficiency.push_back(efficiency);
        fefficiency_err.push_back(efficiency_err);
        
        feffBR.push_back(efficiency*BR_NuDat);

        
    }
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

