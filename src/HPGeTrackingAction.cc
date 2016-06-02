
#include "HPGeTrackingAction.hh"
#include "HPGeRunAction.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh" 
#include "G4PhysicalConstants.hh" 



HPGeTrackingAction::HPGeTrackingAction()
{
	
}

HPGeTrackingAction::~HPGeTrackingAction()
{

}

void HPGeTrackingAction::PreUserTrackingAction(const G4Track*)
{
	
	HPGeRunAction* runAction = (HPGeRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
	fhTotEdep = runAction->GetRunAnalysis()->GetHisto(); // get pointer to histogram
}

void HPGeTrackingAction::PostUserTrackingAction(const G4Track* theTrack){

		
	const G4ParticleDefinition* particleDefinition =  theTrack->GetDefinition();

	if(particleDefinition->GetParticleName() == "gamma") {
		//G4cout << "Gamma produced!" << G4endl;
		double Ekin = theTrack->GetVertexKineticEnergy()/keV;
		fhTotEdep->Fill(Ekin);
	}

}

	
	
	
	
	
	


  



    
    


   

