#ifndef HPGeAnalysis_h
#define HPGeAnalysis_h 1

#include <TH1D.h>

class HPGeAnalysis : public HPGeAnalysis
{

public:
    HPGeAnalysis();
    ~HPGeAnaylsis();

public:
    void CalcEfficiencies();
    
    void AddEnergy(double energy) { fenergy.push_back(energy); };
    void AddG4BR(double G4BR) { fG4BR.push_back(G4BR); };
    void AddNuDatBR(double NuDatBR) { fNuDatBR.push_back(NuDatBR); };
    void AddSigRegion(double SigRegion) { fwidth_sig.push_back(SigRegion); };
    void AddBkgRegion(double BkgRegion) { fwidth_bkg.push_back(BkgRegion); };
    
    int GetNLines() { return fnlines; };
    double GetEfficiency(int i) { return fefficiency[i]; };
    double GetEfficiency_err(int i) { return fefficiency_err[i]; };
    TH1D* GetHisto() { return fhTotEdep; };
	
  
private:
    std::vector<double> fenergy;
    std::vector<double> fG4BR;
    std::vector<double> fNuDatBR;
    std::vector<double> fwidth_sig;
    std::vector<double> fwidth_bkg;
    
    std::vector<double> fefficiency;
    std::vector<double> fefficiency_err;
    
    TH1D* fhTotEdep;
    int fnlines;
    
};



#endif
