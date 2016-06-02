#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4ios.hh"

#include "HPGeDetectorConstruction.hh"
#include "HPGePhysicsList.hh"
#include "HPGePrimaryGeneratorAction.hh"
#include "HPGeRunAction.hh"
#include "HPGeTrackingAction.hh"

#include "Randomize.hh"
#include <time.h>
#include <getopt.h>


#include "G4VisExecutive.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)//
{
	// Choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
	
	//set random seed with system time
	//G4long seed = time(NULL);
    
    // set seed
	CLHEP::HepRandom::setTheSeed(24312);
	
  
  // User Verbose output class
  //
  /*G4VSteppingVerbose* verbosity = new HPGeSteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);*/
  //-----------------------------
    int c = 0;
    bool Macro = false;
    G4String MacroFilename;
    
    while((c = getopt(argc,argv,"m:")) != -1)
    {
        switch(c)
        {
            case 'm':
                Macro = true;
                MacroFilename = optarg;
                break;
        }
    }
    
  
          
    // Run manager
    //
    G4RunManager* runManager = new G4RunManager;
    
    // UserInitialization classes - mandatory
    //
    G4VUserDetectorConstruction* detector = new HPGeDetectorConstruction();
    runManager-> SetUserInitialization(detector);
    //
    G4VUserPhysicsList* physics = new HPGePhysicsList;
    runManager-> SetUserInitialization(physics);
    
    // visualization manager
    //
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // UserAction classes
    //
    HPGeRunAction* run_action = new HPGeRunAction();
    runManager->SetUserAction(run_action);
    //
    HPGePrimaryGeneratorAction* gen_action = new HPGePrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);
	//
	G4UserTrackingAction* track_action = new HPGeTrackingAction;
    runManager->SetUserAction(track_action);

    
    // Initialize G4 kernel
    //
    runManager->Initialize();
    
	

	//
	//------Shell & Visualization-------------------------------------------------
	
	if (!Macro) {
	
	// Define UI session for interactive mode
	G4UIsession* session = new G4UIterminal();
	
	session->SessionStart();
	delete session;
	}
	else {
	  
	  // Get the pointer to the User Interface manager
	  G4UImanager* UI = G4UImanager::GetUIpointer();
	
	G4String command = "/control/execute "+MacroFilename;
	UI->ApplyCommand(command);
	
	  
	}
		
	
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete visManager;
  delete runManager;
  //delete verbosity;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
