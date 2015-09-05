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
#include "HPGeTrackingAction.hh"
#include "HPGeRunMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TSystem.h"

//#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunAction::HPGeRunAction(G4String OutputFolder)
{
    timer = new G4Timer;
    
    selectedAction = "default";
    fOutputFolder = OutputFolder;
    
    //create a messenger for this class
    runMessenger = new HPGeRunMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunAction::~HPGeRunAction()
{
    delete timer;
    delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunAction::BeginOfRunAction(const G4Run* aRun)
{
    TString ResultFileName;

    G4int RunID = aRun->GetRunID();

    if (selectedAction=="default") {
        
        std::ostringstream convert;   // stream used for the conversion
        
        convert << RunID;      // insert the textual representation in the characters in the stream
        
        TString RunName=convert.str();
        
        ResultFileName = "results_run" + RunName + ".root";

    }
    else {

        ResultFileName = selectedAction;

    }
	
    // try to open results directory
    if (!gSystem->OpenDirectory(fOutputFolder)) {
        
        // if directory does not exist make one
        if (gSystem->MakeDirectory(fOutputFolder)==-1) {
            std::cout << "###### ERROR: could not create directory " << fOutputFolder << std::endl;
        }
    }

    
	ResultFile = new TFile(fOutputFolder+"/"+ResultFileName,"Create");
    
    if (ResultFile->IsZombie()) {
        G4cout << "##### Warning: " << ResultFileName << " already exists! Overwriting!" << G4endl;
        ResultFile = new TFile(fOutputFolder+"/"+ResultFileName,"recreate");
    }
    
        
    GeHitTree = new TTree("GeHits", "GeHits");
    PrimariesTree = new TTree("Primaries", "Primaries");
    RunTree = new TTree("RunInfo", "RunInfo");
    
    GeHitTree->Branch("EventID", &HEventID);
    GeHitTree->Branch("NHits", &NHits);
    GeHitTree->Branch("TotEdep", &TotEdep);
    
    GeHitTree->Branch("TrackID", &HTrackID);
    GeHitTree->Branch("ParticleID", &HParticleID);
    GeHitTree->Branch("Edep", &Edep);
    GeHitTree->Branch("xPos", &xPos);
    GeHitTree->Branch("yPos", &yPos);
    GeHitTree->Branch("zPos", &zPos);
    GeHitTree->Branch("Time", &Time);
    GeHitTree->Branch("Ekin", &HEkin);
    
    PrimariesTree->Branch("EventID", &PEventID);
    PrimariesTree->Branch("TrackID", &PTrackID);
    PrimariesTree->Branch("ParentID", &ParentID);
    PrimariesTree->Branch("Ekin", &PEkin);
    PrimariesTree->Branch("xDir", &xDir);
    PrimariesTree->Branch("yDir", &yDir);
    PrimariesTree->Branch("zDir", &zDir);
    PrimariesTree->Branch("ParticleID", &PParticleID);
    PrimariesTree->Branch("CreatorProcess", &Process);
    
    
    RunTree->Branch("NEvents", &NEvents);
    RunTree->Branch("NDecays", &NDecays);
    
    NEvents = aRun->GetNumberOfEventToBeProcessed();
    
    fNDecays = 0;
    
    G4cout << "### Run " << RunID << " started with " << NEvents << " events." << G4endl;
    timer->Start();
    
    
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunAction::EndOfRunAction(const G4Run* aRun)
{   
	timer->Stop();
	
	NDecays=fNDecays;
	
	G4cout << "\n" << "### Finished ###" << G4endl;
	G4cout << "Runtime: " << *timer << G4endl;
	
	//-----------write trees and close file-------------
	ResultFile->cd();
	
	RunTree->Fill();
	
	GeHitTree->Write();
	PrimariesTree->Write();
	RunTree->Write();
	
	ResultFile->Close();
	
	//delete GeHitTree;
	//delete GammaTree;
	delete ResultFile;
}

TTree* HPGeRunAction::GetGeHitTree()
{
	fGeHitTree=GeHitTree;
	return fGeHitTree;
}


TTree* HPGeRunAction::GetPrimariesTree()
{
	fPrimariesTree=PrimariesTree;
	return fPrimariesTree;
}

void HPGeRunAction::AddDecay()
{
	fNDecays++;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
