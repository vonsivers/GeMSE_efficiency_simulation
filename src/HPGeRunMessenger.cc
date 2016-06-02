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
// $Id: PrimaryGeneratorMessenger.cc,v 1.1 2010-07-16 07:37:48 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HPGeRunMessenger.hh"
#include "HPGeRunAction.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "G4Run.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunMessenger::HPGeRunMessenger(HPGeAnalysis* RunAnalysis)
:fRunAnalysis(RunAnalysis)
{
    fileCmd = new G4UIcmdWithAString("file",this);
	fileCmd->SetDefaultValue("results/isotope_BR.txt");

    energyCmd = new G4UIcmdWithADouble("energy",this);
    energyCmd->SetDefaultValue(0.);
    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunMessenger::~HPGeRunMessenger()
{
	delete fileCmd;
    delete energyCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValues)
{ 
    if (command == energyCmd) {
        fRunAnalysis->AddEnergy(energyCmd->GetNewDoubleValue(newValues));
    }
	else if (command == fileCmd) {
        fRunAnalysis->SetFile(newValues);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

