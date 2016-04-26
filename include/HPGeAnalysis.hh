#ifndef HPGeAnalysis_h
#define HPGeAnalysis_h 1

#include <TH1D.h>

class HPGeAnalysis
{

public:
    HPGeAnalysis();
    ~HPGeAnalysis();

public:
    void CalcEfficiencies();
    
    void SetNEvents(int NEvents) { fNEvents = NEvents; };
    
    void AddEnergy(double energy) { fenergy.push_back(energy); };
    void AddG4BR(double G4BR) { fG4BR.push_back(G4BR); };
    void AddNuDatBR(double NuDatBR) { fNuDatBR.push_back(NuDatBR); };
    void AddSigRegion(double SigRegion) { fwidth_sig.push_back(SigRegion); };
    void AddBkgRegion(double BkgRegion) { fwidth_bkg.push_back(BkgRegion); };
    
    G4int GetNLines() { return fnlines; };
    G4double GetEnergy(G4int i) { return fenergy[i]; };
    G4double GetEfficiency(G4int i) { return fefficiency[i]; };
    G4double GetEfficiency_err(G4int i) { return fefficiency_err[i]; };
    G4double GetEffBR(G4int i) { return feffBR[i]; };

    TH1D* GetHisto() { return fhTotEdep; };
	
  
private:
    std::vector<double> fenergy;
    std::vector<double> fG4BR;
    std::vector<double> fNuDatBR;
    std::vector<double> fwidth_sig;
    std::vector<double> fwidth_bkg;
    
    std::vector<double> fefficiency;
    std::vector<double> fefficiency_err;
    std::vector<double> feffBR;
    
    TH1D* fhTotEdep;
    G4int fnlines;
    G4int fNEvents;
    
};



#endif
