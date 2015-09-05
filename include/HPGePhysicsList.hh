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
// $Id: HPGePhysicsList.hh,v 1.7 2006/06/29 17:53:59 gunter Exp $
// GEANT4 tag $Name: geant4-09-01-patch-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HPGePhysicsList_h
#define HPGePhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HPGePhysicsList: public G4VModularPhysicsList
{
public:
	HPGePhysicsList();
	virtual ~HPGePhysicsList();
	
	void ConstructParticle();
	
	void SetCuts();
	
	void ConstructProcess();
	
private:
	
	
	// hide assignment operator
	//exrdmPhysicsList & operator=(const exrdmPhysicsList &right);
	//exrdmPhysicsList(const exrdmPhysicsList&);
	
	
	G4VPhysicsConstructor*  emPhysicsList;
	G4VPhysicsConstructor*  raddecayList;
	G4VPhysicsConstructor*  particleList;
    

    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 

