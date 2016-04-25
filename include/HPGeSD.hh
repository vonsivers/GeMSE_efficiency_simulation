
#ifndef HPGeSD_h
#define HPGeSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HPGeHit.hh"
//#include <time.h>

#include "TTree.h"
#include <vector>
#include <string>

typedef std::vector<double> dvector;
typedef std::vector<int> ivector;

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
	
	G4bool ProcessHits(G4Step*);
	
	void EndOfEvent(G4HCofThisEvent*);
	
	//! Return the number of Hits
	G4int getEntries();
	
  private:
	HPGeHitsCollection* HitsCollection;
	TTree* GeHitTree;

	G4int EventID;
	G4int NHits;
	G4double TotEdep;
    
    int HCID;

	
};

#endif
