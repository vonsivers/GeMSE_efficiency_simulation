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
// $Id: HPGePrimaryGeneratorAction1.cc,v 1.2 2010-07-16 07:37:48 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "HPGePrimaryGeneratorAction.hh"
//#include "HPGeHPGePrimaryGeneratorAction.hh"
//#include "HPGeRunAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh" // new for version 4.10
#include "G4PhysicalConstants.hh" // new for version 4.10
#include "G4EventManager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TString.h"

//#include <iostream> //
//#include <fstream> //


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGePrimaryGeneratorAction::HPGePrimaryGeneratorAction()
{ 
	G4int n_particle = 1;
	particleGun  = new G4ParticleGun(n_particle);
	
	TString folder = "/Users/sivers/GitHub/GeMSE_efficiency_simulation/inputfiles/";
	
	// sample zenith angle from root file
	TFile zenithFile(folder+"zenith_angle.root", "read");
	zenith_angle=(TH1D*)((TCanvas*)zenithFile.Get("c1"))->GetPrimitive("zenith angle");
	
	// sample energy from root file
	TFile energyFile(folder+"energy_spectrum.root", "read");
	energy_spectrum=(TH1D*)((TCanvas*)energyFile.Get("c1"))->GetPrimitive("energy spectrum");
	
	eventManager = G4EventManager::GetEventManager(); //get pointer to RunManager

	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGePrimaryGeneratorAction::~HPGePrimaryGeneratorAction()
{ 
	delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
		
	// muon charge ratio
	G4String particletype;
	double charge_ratio=1.27;
	double rand = G4UniformRand();
	if (rand<=charge_ratio/(1+charge_ratio)) {
		particletype="mu+";
	}
	else {
		particletype="mu-";
		
	}
	
	G4ParticleDefinition* particle
	= G4ParticleTable::GetParticleTable()->FindParticle(particletype);
	particleGun->SetParticleDefinition(particle);
	
	// sample particle position from random point above cavern
	G4double height=1.6*m;
	G4double size_x=10.*m;
	G4double size_y=10.*m;
	//G4double xPosCavern=2.925*m;
	
	G4double pos_x=size_x*(G4UniformRand()-0.5);
	G4double pos_y=size_y*(G4UniformRand()-0.5);
	G4double pos_z=height;
	
	// generate particle direction
	//
	// sample zenith angle from histogram
	G4double theta = zenith_angle->GetRandom();
	
	G4double phi=twopi*G4UniformRand();
	G4double px = sin(theta)*cos(phi);
	G4double py = sin(theta)*sin(phi);
	G4double pz = -cos(theta); 
	
		
	//-----
	//ofstream myfile; 
	//myfile.open ("muon_position.txt", std::ios::out | std::ios::app); 
	//myfile << pos_x/m << "\t" << pos_y/m << "\t" << pos_z/m << "\n";
	//-----
	
	
	
	
	// sample energy from histogram
	G4double energy = energy_spectrum->GetRandom();
	
	particleGun->SetParticlePosition(G4ThreeVector(pos_x,pos_y,pos_z));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));    
	particleGun->SetParticleEnergy(energy*GeV);
	particleGun->GeneratePrimaryVertex(anEvent);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
