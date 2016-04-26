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
#include "G4UIdirectory.hh"

#include "G4Run.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunMessenger::HPGeRunMessenger(HPGeAnalysis* RunAnalysis)
:fRunAnalysis(RunAnalysis)
{
    runDirectory = new G4UIdirectory("/gammaline/");

    energyCmd = new G4UIcmdWithADouble("/gammaline/energy",this);
    G4BRCmd = new G4UIcmdWithADouble("/gammaline/G4BR",this);
    NuDatBRCmd = new G4UIcmdWithADouble("/gammaline/NuDatBR",this);
    SigRegionCmd = new G4UIcmdWithADouble("/gammaline/SigRegion",this);
    BkgRegionCmd = new G4UIcmdWithADouble("/gammaline/BkgRegion",this);
    energyCmd->SetDefaultValue(0.);
    G4BRCmd->SetDefaultValue(1.);
    NuDatBRCmd->SetDefaultValue(1.);
    SigRegionCmd->SetDefaultValue(0.4);
    BkgRegionCmd->SetDefaultValue(10.);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeRunMessenger::~HPGeRunMessenger()
{
    delete energyCmd;
    delete G4BRCmd;
    delete NuDatBRCmd;
    delete SigRegionCmd;
    delete BkgRegionCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeRunMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValues)
{ 
    if (command == energyCmd) {
        fRunAnalysis->AddEnergy(energyCmd->GetNewDoubleValue(newValues));
    }
    else if (command == G4BRCmd) {
        fRunAnalysis->AddG4BR(G4BRCmd->GetNewDoubleValue(newValues));
    }
    else if (command == NuDatBRCmd) {
        fRunAnalysis->AddNuDatBR(NuDatBRCmd->GetNewDoubleValue(newValues));
    }
    else if (command == SigRegionCmd) {
        fRunAnalysis->AddSigRegion(SigRegionCmd->GetNewDoubleValue(newValues));
    }
    else if (command == BkgRegionCmd) {
        fRunAnalysis->AddBkgRegion(BkgRegionCmd->GetNewDoubleValue(newValues));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

