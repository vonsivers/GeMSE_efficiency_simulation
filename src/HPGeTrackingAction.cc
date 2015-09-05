

//======================================================== LENA STEPPING ACTION
// $Author: randy $
// $Date: 2012-12-21 14:28:50 +0100 (Fri, 21 Dec 2012) $
// $Rev: 579 $


#include "HPGeTrackingAction.hh"
#include "HPGeTrackingMessenger.hh"
#include "HPGeRunAction.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
//#include "G4Step.hh"
//#include "G4Event.hh"
//#include "HPGeDetectorConstruction.hh"
//#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh" // new for version 4.10
#include "G4PhysicalConstants.hh" // new for version 4.10

//#include <math.h>
//#include <stdlib.h>

//#include <iostream>


HPGeTrackingAction::HPGeTrackingAction()
{
	//create a messenger for this class
	trackingMessenger = new HPGeTrackingMessenger(this); 
}

HPGeTrackingAction::~HPGeTrackingAction()
{

	delete trackingMessenger;      

}

void HPGeTrackingAction::PreUserTrackingAction(const G4Track* theTrack)
{
	
	//G4cout << "Tracking active!!!!!" << G4endl;
	// Get pointer to the current LenaRunAction instance
	run_action = (HPGeRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	PrimariesTree = run_action->GetPrimariesTree();
}

void HPGeTrackingAction::PostUserTrackingAction(const G4Track* theTrack){
	const G4ParticleDefinition* particleDefinition =  theTrack->GetDefinition();
		
	if (theTrack->GetTrackID()==2 && theTrack->GetCreatorProcess()->GetProcessName()=="RadioactiveDecay") {
		run_action->AddDecay();
	}
	
	//if(particleDefinition->GetParticleName() == "gamma")
	//{
		//G4cout << "Gamma produced!!!!!" << G4endl;
		
	if(selectedAction==true)
	{
		//save the particle only after it has been tracked
		if(theTrack->GetTrackStatus()==fStopAndKill)
		{
			EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
			TrackID = theTrack->GetTrackID();
            ParentID = theTrack->GetParentID();
			Ekin = theTrack->GetVertexKineticEnergy()/keV;
			G4ThreeVector VDir = theTrack->GetVertexMomentumDirection();
			xDir=VDir[0];
			yDir=VDir[1];
			zDir=VDir[2];
            ParticleID = new G4String(particleDefinition->GetParticleName());
            
            if(theTrack->GetCreatorProcess()!=0){
                CreatorProcess = new G4String(theTrack->GetCreatorProcess()->GetProcessName());
            }
            else {
                CreatorProcess = new G4String;
            }
            
			PrimariesTree->SetBranchAddress("EventID", &EventID);
			PrimariesTree->SetBranchAddress("TrackID", &TrackID);
            PrimariesTree->SetBranchAddress("ParentID", &ParentID);
			PrimariesTree->SetBranchAddress("Ekin", &Ekin);
			PrimariesTree->SetBranchAddress("xDir", &xDir);
			PrimariesTree->SetBranchAddress("yDir", &yDir);
			PrimariesTree->SetBranchAddress("zDir", &zDir);
			PrimariesTree->SetBranchAddress("ParticleID", &ParticleID);
            PrimariesTree->SetBranchAddress("CreatorProcess", &CreatorProcess);
			
			PrimariesTree->Fill();
		}
	}
		
	

}

	
	
	
	
	
	
