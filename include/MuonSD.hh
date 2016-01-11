
#ifndef MuonSD_h
#define MuonSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MuonHit.hh"
//#include <time.h>

#include "TTree.h"
#include <vector>
#include <string>

typedef std::vector<double> dvector;
typedef std::vector<int> ivector;

class G4Step;
class G4HCofThisEvent;

class MuonSD : public G4VSensitiveDetector
{
  public:
	MuonSD(G4String);
	~MuonSD();
	
	//! create an instance of MuonHitsCollection and add a new hits collection
	//! to the G4HCofThisEvent instance
	void Initialize(G4HCofThisEvent*);
	
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	
	void EndOfEvent(G4HCofThisEvent*);
	
	//! Return the number of Hits
	G4int getEntries();
	
  private:
	MuonHitsCollection* HitsCollection;
	TTree* MuonHitTree;

	G4int EventID;
	G4int NHits;
	G4double TotEdep;
	
    int HCID;
};

#endif
