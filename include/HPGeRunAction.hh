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
// $Id: HPGeRunAction.hh,v 1.9 2006/06/29 17:54:10 gunter Exp $
// GEANT4 tag $Name: geant4-09-01-patch-02 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HPGeRunAction_h
#define HPGeRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"

#include "TTree.h"
#include "TFile.h"

#include <vector>
#include <string>

using std::string;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;
class HPGeRunMessenger;

class HPGeRunAction : public G4UserRunAction
{
  public:
    HPGeRunAction(G4String Outputfolder);
   ~HPGeRunAction();

  public:
    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);

	TTree* GetGeHitTree();
	TTree* GetPrimariesTree();
	
	void AddDecay();
    
    void SelectAction(G4String string)	{ selectedAction = string; };
    G4String GetSelectedAction()			{ return selectedAction; };


  private:
    G4Timer* timer;
	
	G4int NDecays;
	G4int fNDecays;

	G4int NEvents;
	G4int HEventID;
	G4int NHits;
	G4double TotEdep;
	
	std::vector<double> Edep;
	std::vector<double> HEkin;
	std::vector<double> Time;
	std::vector<double> xPos;
	std::vector<double> yPos;
	std::vector<double> zPos;
	std::vector<int> HParticleID;
	std::vector<int> HTrackID;

	G4int PEventID;
	G4int PTrackID;
    G4int ParentID;
    string* PParticleID;
    string* Process;
    G4double PEkin;
	G4double xDir;
	G4double yDir;
	G4double zDir;
	
	TFile* ResultFile;
	
	TTree* GeHitTree;
	TTree* PrimariesTree;
	TTree* RunTree;
	
	TTree* fGeHitTree;
	TTree* fPrimariesTree;
    
    G4String selectedAction;
    G4String fOutputFile;

    HPGeRunMessenger* runMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*HPGeRunAction_h*/
