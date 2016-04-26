#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4ios.hh"

#include "HPGeDetectorConstruction.hh"
#include "HPGePhysicsList.hh"
#include "HPGePrimaryGeneratorAction.hh"
#include "HPGeRunAction.hh"

#include "Randomize.hh"
#include <time.h>
#include <getopt.h>

#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TString.h>

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
    G4String GeometryFilename = "worldVolume.txt";
    G4String OutputFolder = "results";
    
    while((c = getopt(argc,argv,"m:o:g:")) != -1)
    {
        switch(c)
        {
            case 'm':
                Macro = true;
                MacroFilename = optarg;
                break;
                
            case 'g':
                GeometryFilename = optarg;
                break;
                
            case 'o':
                OutputFolder = optarg;
                break;
                
        }
    }
    
    // try to open results directory
    if (!gSystem->OpenDirectory(OutputFolder)) {
        
        // if directory does not exist make one
        if (gSystem->MakeDirectory(OutputFolder)==-1) {
            std::cout << "###### ERROR: could not create directory " << OutputFolder << std::endl;
            return 0;
        }
    }
    
    // create output file
    TString outputfile = OutputFolder+"/simulated_efficiencies.root";
    TFile* file = new TFile(outputfile,"Create");
    
    if (file->IsZombie()) {
        G4cout << "###### ERROR: could not create file 'simulated_efficiencies.root'" << G4endl;
        return 0;
    }

    TTree* tree = new TTree("tree","tree");
    
    // Run manager
    //
    G4RunManager* runManager = new G4RunManager;
    
    // UserInitialization classes - mandatory
    //
    G4VUserDetectorConstruction* detector = new HPGeDetectorConstruction(GeometryFilename);
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
    HPGeRunAction* run_action = new HPGeRunAction(tree);
    runManager->SetUserAction(run_action);
    //
    HPGePrimaryGeneratorAction* gen_action = new HPGePrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);
    
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
		
	
    
    //------------- plot efficiency curve -------------------
    TCanvas* c1 = new TCanvas("c1");
    tree->Draw("efficiency:energy","","*");
    TString graphfile = OutputFolder+"/simulated_efficiencies.pdf";
    c1->SaveAs(graphfile);
	
    
    //-------------------------------------------------------
    
    file->cd();
    tree->Write();
    file->Close();

	
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
