#ifndef HPGeAnalysis_h
#define HPGeAnalysis_h 1

#include <TH1D.h>
#include <TString.h>

class HPGeAnalysis
{

public:
    HPGeAnalysis();
    ~HPGeAnalysis();

public:
    void CalcBR();
    void Clear();
    
    void SetNEvents(int NEvents) { fNEvents = NEvents; }
    
    void AddEnergy(double energy) { fenergy.push_back(energy); }
    void SetFile(TString filename) { fFileName = filename; }
  
    TH1D* GetHisto() { return fhTotEdep; }
	
  
private:
    std::vector<double> fenergy;
    std::vector<double> fBR;
    std::vector<double> fBR_err;

	TString fFileName;
    TH1D* fhTotEdep;
    int fnlines;
    int fNEvents;
    
};



#endif
