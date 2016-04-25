
#include "HPGeSD.hh"
#include "HPGeHit.hh"
#include "HPGeDetectorConstruction.hh"
#include "HPGeRunAction.hh"
#include "HPGePrimaryGeneratorAction.hh"
//#include "HPGeUserEventInformation.hh"

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
#include "G4SystemOfUnits.hh" // new for version 4.10
#include "G4PhysicalConstants.hh" // new for version 4.10

//#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

//#include <time.h>
#include <vector>



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
	
	GeHitTree = runAction->GetGeHitTree(); //get name of results tree
 
	
}

G4bool HPGeSD::ProcessHits(G4Step* aStep){

	G4double edep = aStep->GetTotalEnergyDeposit();
	G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();

	if(edep==0.) return false;

	HPGeHit* newHit = new HPGeHit();
	newHit->SetTrack(aStep->GetTrack()->GetTrackID());
	newHit->SetEdep(edep);
	newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
	newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());
	newHit->SetParticle(particleType->GetPDGEncoding());
	newHit->SetParticleEnergy(aStep->GetPreStepPoint()->GetKineticEnergy() );

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
	G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
	G4int n_hits = HitsCollection->entries();
	
	dvector* Edep = new dvector;
	dvector* Ekin = new dvector;
	dvector* Time = new dvector;
	dvector* xPos = new dvector;
	dvector* yPos = new dvector;
	dvector* zPos = new dvector;
	ivector* ParticleID = new ivector;
	ivector* TrackID = new ivector;
	
		
		// gather info on hits 
		for(int i=0;i<n_hits;i++){ 
			G4double edep=(*HitsCollection)[i]->GetEdep()/keV;
			G4ThreeVector Pos=(*HitsCollection)[i]->GetPos()/mm;
			Edep->push_back(edep);
			xPos->push_back(Pos[0]);
			yPos->push_back(Pos[1]);
			zPos->push_back(Pos[2]);
			Time->push_back((*HitsCollection)[i]->GetTime()/ns);
			ParticleID->push_back((*HitsCollection)[i]->GetParticle());
			TrackID->push_back((*HitsCollection)[i]->GetTrack());
			Ekin->push_back((*HitsCollection)[i]->GetParticleEnergy()/keV);
			
			totalEdep+=edep; //sum up the edep
		}	
	
	
	if (totalEdep>0) {
		
		GeHitTree->SetBranchAddress("EventID", &EventID);
		GeHitTree->SetBranchAddress("NHits", &n_hits);
		GeHitTree->SetBranchAddress("TotEdep", &totalEdep);
		
		GeHitTree->SetBranchAddress("Edep", &Edep);
		GeHitTree->SetBranchAddress("xPos", &xPos);
		GeHitTree->SetBranchAddress("yPos", &yPos);
		GeHitTree->SetBranchAddress("zPos", &zPos);
		GeHitTree->SetBranchAddress("Time", &Time);
		GeHitTree->SetBranchAddress("ParticleID", &ParticleID);
		GeHitTree->SetBranchAddress("TrackID", &TrackID);
		GeHitTree->SetBranchAddress("Ekin", &Ekin);
		
		GeHitTree->Fill();
	}
	 
	if (EventID%10000==0) {
		G4cout << "\r" << "Simulating Event Nr.: " << EventID << std::flush;
	}

    delete Edep;
    delete Ekin;
    delete Time;
    delete xPos;
    delete yPos;
    delete zPos;
    delete ParticleID;
    delete TrackID;
}

  



    
    


   

