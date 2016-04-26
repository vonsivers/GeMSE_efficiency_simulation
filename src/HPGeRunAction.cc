//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: HPGeRunAction.cc,v 1.10 2006/06/29 17:54:31 gunter Exp $
// GEANT4 tag $Name: geant4-09-01-patch-02 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"

#include "HPGeRunAction.hh"
#include "HPGeAnalysis.hh"
#include "HPGeRunMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "TTree.h"

//#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunAction::HPGeRunAction(TTree* tree)
{
    ftree = tree;
    timer = new G4Timer;
    
    // create run analysis
    fRunAnalysis = new HPGeAnalysis();
    
    //create a messenger for this class
    runMessenger = new HPGeRunMessenger(fRunAnalysis);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunAction::~HPGeRunAction()
{
    delete timer;
    delete fRunAnalysis;
    delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunAction::BeginOfRunAction(const G4Run* aRun)
{
    
    G4int RunID = aRun->GetRunID();
    
    G4int NEvents = aRun->GetNumberOfEventToBeProcessed();
    
    // set Nb of events
    fRunAnalysis->SetNEvents(NEvents);
    
    G4cout << "### Run " << RunID << " started with " << NEvents << " events." << G4endl;
    timer->Start();
    
    
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunAction::EndOfRunAction(const G4Run* aRun)
{
    
    // calculate efficiencies
    fRunAnalysis->CalcEfficiencies();
    
    // fill ROOT Tree
    G4double energy, efficiency, efficiency_err, eff_BR;
    ftree->Branch("energy",&energy);
    ftree->Branch("efficiency",&efficiency);
    ftree->Branch("efficiency_err",&efficiency_err);
    ftree->Branch("eff_BR",&eff_BR);
    int nlines = fRunAnalysis->GetNLines();
    for (int i=0; i<nlines; ++i) {
        energy = fRunAnalysis->GetEnergy(i);
        efficiency = fRunAnalysis->GetEfficiency(i);
        efficiency_err = fRunAnalysis->GetEfficiency_err(i);
        eff_BR = fRunAnalysis->GetEffBR(i);

        ftree->Fill();
    }
    
	timer->Stop();
	
	G4cout << "\n" << "### Finished ###" << G4endl;
	G4cout << "Runtime: " << *timer << G4endl;
	
	
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
