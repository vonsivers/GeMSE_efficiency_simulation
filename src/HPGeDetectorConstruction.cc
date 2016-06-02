
#include "HPGeDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"

#include "globals.hh"


HPGeDetectorConstruction::HPGeDetectorConstruction()
{}

HPGeDetectorConstruction::~HPGeDetectorConstruction()
{}



G4VPhysicalVolume* HPGeDetectorConstruction::Construct()
{
	
	//======= define materials ==============================================

	G4double density, a, z;
	
	G4Material* vacuum_mat = new G4Material("Vacuum", z=7., a=14.007*g/mole,
										density=1.e-12*g/cm3);
		
	// +++++++ world volume ++++++++++++++++++++++++++++++++++ 

    
	G4double expHall_x=1.*m;
	G4double expHall_y=1.*m;
	G4double expHall_z=1.*m;
	
	G4Box* 			expHall_box  = new G4Box	   ("World",expHall_x,expHall_y,expHall_z);
	G4LogicalVolume* 	expHall_log  = new G4LogicalVolume (expHall_box,vacuum_mat,"World",0,0,0);
	G4VPhysicalVolume*    	expHall_phys = new G4PVPlacement   (0,G4ThreeVector(),expHall_log,"World",0,false,0);
  	
	return expHall_phys;
}

