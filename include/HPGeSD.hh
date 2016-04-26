
#ifndef HPGeSD_h
#define HPGeSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HPGeHit.hh"

#include <TH1D.h>


class G4Step;
class G4HCofThisEvent;

class HPGeSD : public G4VSensitiveDetector
{
  public:
	HPGeSD(G4String);
	~HPGeSD();
	
	//! create an instance of HPGeHitsCollection and add a new hits collection
	//! to the G4HCofThisEvent instance
	void Initialize(G4HCofThisEvent*);
	
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	
	void EndOfEvent(G4HCofThisEvent*);
	
	
  private:
	HPGeHitsCollection* HitsCollection;    
    int HCID;
    TH1D* fTotEdep;

	
};

#endif
