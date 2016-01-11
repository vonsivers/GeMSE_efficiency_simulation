#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4ios.hh"

#include "HPGeDetectorConstruction.hh"
//#include "LBE.hh"
#include "Shielding.hh"
#include "HPGePrimaryGeneratorAction.hh"
#include "HPGeRunAction.hh"

#include "Randomize.hh"
#include <time.h>

#include "G4VisExecutive.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)//
{
    // Choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    
    //set random seed with system time
    G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);
    
    
    // User Verbose output class
    //
    /*G4VSteppingVerbose* verbosity = new HPGeSteppingVerbose;
     G4VSteppingVerbose::SetInstance(verbosity);*/
    //-----------------------------
    
    
    
    
    // Run manager
    //
    G4RunManager* runManager = new G4RunManager;
    
    // UserInitialization classes - mandatory
    //
    G4VUserDetectorConstruction* detector = new HPGeDetectorConstruction;
    runManager-> SetUserInitialization(detector);
    //
    G4VUserPhysicsList* physics = new Shielding;
    runManager-> SetUserInitialization(physics);
    
    // visualization manager
    //
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // UserAction classes
    //
    HPGeRunAction* run_action = new HPGeRunAction;
    runManager->SetUserAction(run_action);
    //
    HPGePrimaryGeneratorAction* gen_action = new HPGePrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);    
    
    
    // Initialize G4 kernel
    //
    runManager->Initialize();
    
    
    
    
    
    //
    //------Shell & Visualization-------------------------------------------------
    
    if (argc==1) {
        
        // Define UI session for interactive mode
        G4UIsession* session = new G4UIterminal();
        
        session->SessionStart();
        delete session;
    }
    else {
        
        // Get the pointer to the User Interface manager
        G4UImanager* UI = G4UImanager::GetUIpointer();
        
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UI->ApplyCommand(command+fileName);
        
        
    }
    
    
    //-------------------------------------
    
    
    
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
