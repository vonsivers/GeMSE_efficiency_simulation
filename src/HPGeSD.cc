
#include "HPGeSD.hh"
#include "HPGeHit.hh"
#include "HPGeDetectorConstruction.hh"
#include "HPGeRunAction.hh"

#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4THitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


HPGeSD::HPGeSD(G4String name)
:G4VSensitiveDetector(name),
HCID(-1)
{
	collectionName.insert("GeHitsCollection");
}


HPGeSD::~HPGeSD(){}



void HPGeSD::Initialize(G4HCofThisEvent*)
{

	HitsCollection = new HPGeHitsCollection
					 (SensitiveDetectorName,collectionName[0]);
	  
	
	HPGeRunAction* runAction = (HPGeRunAction*) G4RunManager::GetRunManager()->GetUserRunAction(); //get pointer to RunAction
	
	fhTotEdep = runAction->GetRunAnalysis()->GetHisto(); // get pointer to histogram
 
	
}

G4bool HPGeSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist){

	G4double edep = aStep->GetTotalEnergyDeposit();

	if(edep==0.) return false;

	HPGeHit* newHit = new HPGeHit();
	newHit->SetEdep(edep);
    
	HitsCollection->insert( newHit );

	return true;
}



void HPGeSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    if (HCID<0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    
    HCE->AddHitsCollection(HCID, HitsCollection);
    
    
	G4double totalEdep=0.;
	G4int n_hits = HitsCollection->entries();
	
    
		// gather info on hits 
		for(int i=0;i<n_hits;i++){ 
			G4double edep=(*HitsCollection)[i]->GetEdep()/keV;
			totalEdep+=edep; //sum up the edep
		}	
	
	// fill histogram
	if (totalEdep>0) {
		
		fhTotEdep->Fill(totalEdep);
	}
    
    G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
	 
	if (EventID%10000==0) {
		G4cout << "\r" << "Simulating Event Nr.: " << EventID << std::flush;
	}


}

  



    
    


   

