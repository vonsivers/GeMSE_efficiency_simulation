
#include "HPGeDetectorConstruction.hh"
#include "HPGeSD.hh" 
#include "HPGeRunAction.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4EllipticalTube.hh"
#include "G4SDManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"

//#include "G4tgrMessenger.hh"
#include "G4tgbVolumeMgr.hh"


#include "globals.hh"


HPGeDetectorConstruction::HPGeDetectorConstruction(G4String GeometryFile)
{
    fGeometryFile = GeometryFile;
}

HPGeDetectorConstruction::~HPGeDetectorConstruction()
{}



G4VPhysicalVolume* HPGeDetectorConstruction::Construct()
{
	
	//======= define materials ==============================================

	G4double density, fractionmass, temperature, pressure, a, z;
    G4int ncomponents;

	// define materials from NIST manager
	G4NistManager* nist = G4NistManager::Instance();
	
	G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* copper_mat   = nist->FindOrBuildMaterial("G4_Cu");
	G4Material* lead_mat   = nist->FindOrBuildMaterial("G4_Pb");
	G4Material* germanium_mat   = nist->FindOrBuildMaterial("G4_Ge");
	
	G4Material* pvt_mat   = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Material* teflon_mat   = nist->FindOrBuildMaterial("G4_TEFLON");
    
    G4Material* pmma_mat   = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* ps_mat   = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    
    // define elements
    G4Element* C  = nist->FindOrBuildElement("C");
    G4Element* H  = nist->FindOrBuildElement("H");
    G4Element* O  = nist->FindOrBuildElement("O");
    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* Mg = nist->FindOrBuildElement("Mg");
    G4Element* Fe = nist->FindOrBuildElement("Fe");
    G4Element* S  = nist->FindOrBuildElement("S");
    G4Element* Al = nist->FindOrBuildElement("Al");
    G4Element* Ca = nist->FindOrBuildElement("Ca");
    G4Element* Na = nist->FindOrBuildElement("Na");
    G4Element* Ni = nist->FindOrBuildElement("Ni");

    
    G4Material* matrix_mat = new G4Material("matrix", density=0.98*g/cm3, ncomponents=4);
    matrix_mat->AddElement (C , fractionmass= 32.4*perCent);
    matrix_mat->AddElement (H , fractionmass= 8.1*perCent);
    matrix_mat->AddElement (O , fractionmass= 21.6*perCent);
    matrix_mat->AddElement (Si , fractionmass= 37.9*perCent);
    
	
	G4Material* standardrock_mat = new G4Material("Standard_Rock", z=11., a=22.*g/mole,
								   density=2.65*g/cm3);
	
	G4Material* vacuum_mat = new G4Material("Vacuum", z=7., a=14.007*g/mole,
										density=1.e-12*g/cm3);
    
	
	//======= define colors ==============================================
	
	G4VisAttributes* red		= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	G4VisAttributes* violet		= new G4VisAttributes(G4Colour(0.5,0.0,1.0));
	G4VisAttributes* pink		= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	G4VisAttributes* black		= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	G4VisAttributes* lightblue  = new G4VisAttributes(G4Colour(0.0,0.5,1.0));
	G4VisAttributes* yellow     = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	G4VisAttributes* orange		= new G4VisAttributes(G4Colour(1.0,0.4,0.2));
	G4VisAttributes* purple     = new G4VisAttributes(G4Colour(0.7,0.0,0.7));
	G4VisAttributes* green		= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	G4VisAttributes* blue		= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	G4VisAttributes* cyan		= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	G4VisAttributes* grey		= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
	
	//======= define dimensions ============================================
	
	// overlap for boolean solids
	double overlap = 0.0001 *cm;
		
	// +++++++ world volume ++++++++++++++++++++++++++++++++++ 

    /*
	G4double expHall_x=10.*m;
	G4double expHall_y=10.*m;
	G4double expHall_z=10.*m;
	
	G4Box* 			expHall_box  = new G4Box	   ("World",expHall_x,expHall_y,expHall_z);
	G4LogicalVolume* 	expHall_log  = new G4LogicalVolume (expHall_box,air_mat,"World",0,0,0);
	G4VPhysicalVolume*    	expHall_phys = new G4PVPlacement   (0,G4ThreeVector(),expHall_log,"World",0,false,0);
    */
    
    // +++++++ sample geometry + world volume ++++++++++++++++++++++++++++++++++
    
    //------------------------------------------------
    // Define one or several text files containing the geometry description
    //------------------------------------------------
    G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
    volmgr->AddTextFile(fGeometryFile);
    
    //------------------------------------------------
    // Read the text files and construct the GEANT4 geometry
    //------------------------------------------------
    G4VPhysicalVolume* physiWorld = volmgr->ReadAndConstructDetector();

    G4LogicalVolume* expHall_log = G4tgbVolumeMgr::GetInstance()->FindG4LogVol("World",1);
    
	
	// +++++++ parts of Ge detector ++++++++++++++++++++++++++++++++++
    
    G4RotationMatrix rm1;
    rm1.rotateX(180.*deg);
	
	// germanium cylinder 
	G4double outerRadiusGe		= 4.25 *cm;
	G4double heightGe			= 6.5 *cm;
	G4double startAngle			= 0. *deg;
	G4double spanningAngle		= 360. *deg;
    
    G4double edgeRadius         = 0.1 *cm;
	
	// cylindrical hole 	
	G4double outerRadiusHole	= 0.5 *cm;
	G4double heightHole			= 4.25 *cm;
	
	// outer contact
    G4double d_LiContact		= 0.067 *cm; //#####################################
    
    G4double edgeInRadius       = edgeRadius-d_LiContact;
    
	
	// +++++++ parts of Ge detector holder ++++++++++++++++++++++++++++++++++
    
    // Teflon like holder
    G4double d_TefHolder_top        = 0.1 *cm;
    G4double d_TefHolder_side       = 0.07 *cm;
    G4double heightTeflonHolder     = 8.4 *cm;
    
    G4double zPosTeflonHolder       = heightGe/2.+d_TefHolder_top/2.+0.005*cm;
	
	// Cu holder top part
	G4double innerRadiusDetHolder	= 4.39 *cm;
	G4double outerRadiusDetHolder	= 4.47 *cm;
	G4double heightDetHolder		= 8.5 *cm;
    
    G4double heightSupportHolder1       = 0.13 *cm;
    G4double heightSupportHolder2       = 0.86 *cm;
    G4double outerRadiusSupportHolder   = 4.69 *cm;
    
    G4double d_Support1 = 1.4 *cm;
    G4double d_Support2 = 5.14 *cm;
	
	G4double zPosDetHolder			= heightGe/2.+d_TefHolder_top+0.005*cm-heightDetHolder/2.;
	
	// Cu holder bottom part
	G4double heightDetHolder_bottom1		= 2.5 *cm;
	G4double innerRadiusDetHolder_bottom1	= 0.8 *cm;
	
	G4double heightDetHolder_bottom2		= 2.5 *cm;
    G4double outerRadiusDetHolder_bottom2   = 1.27 *cm; // from GATOR drawing
	
	G4double heightDetHolder_bottom3		= 1.0 *cm;
	G4double innerRadiusDetHolder_bottom3	= 0.3 *cm;
	
		
	// +++++++ parts of Ge detector housing ++++++++++++++++++++++++++++++++++ 
	
	// distance from Ge detector to inside of endcap
	G4double d_Det_Endcap		= 0.6 *cm;
	
	// endcap of copper housing 
	G4double outerRadiusEndcap	= 5.08 *cm;
    G4double heightEndcap		= 0.1 *cm;
	
	G4double zPosEndcap			= heightGe/2.+d_Det_Endcap+heightEndcap/2.;
	
	// outer copper housing top part
    G4double innerRadiusCuHsg	= 4.93 *cm;
	G4double heightCuHsg		= 15.88 *cm;
	
	//G4double zPosCuHsg			= -(heightCuHsg/2.-heightEndcap/2.-heightGe/2.)+d_Det_Endcap;
	
	// outer copper housing bottom part
	G4double heightCuHsg_bottom         = 6.29 *cm;
    G4double dCuHsg_bottom              = 0.3 *cm; // from GATOR drawings
    G4double innerRadiusCuHsg_bottom    = outerRadiusEndcap-dCuHsg_bottom;
	
	//G4double zPosCuHsg_bottom	= zPosCuHsg-(heightCuHsg/2.-heightEndcap/2.)-heightCuHsg_bottom/2.;
	
	// copper arm + cold finger connection to cryostat
	G4double outerRadiusColdfinger	= 0.95 *cm; // from GATOR drawings
	G4double innerRadiusArm			= 1.4 *cm; // from GATOR drawings
	G4double outerRadiusArm			= 1.58 *cm;
	G4double heightArm				= 50. *cm;
	
	G4double yPosArm				= -outerRadiusEndcap-heightArm/2.+2.5*cm;
    G4double zPosArm				= zPosEndcap+heightEndcap/2.-heightCuHsg-heightCuHsg_bottom/2.;
    
	G4RotationMatrix rm2;
	rm2.rotateX(90.*deg);
	
	
    // Vacuum inside detector housing
    G4double zPosVacuumDet      = zPosEndcap-heightEndcap/2.-(heightCuHsg-heightEndcap)/2.;
    
    // +++++++ parts of the shielding ++++++++++++++++++++++++++++++++++
    
    // bottom part
    //
    // inner Cu shielding
    G4double xSizeSampleChamber		= 24. *cm;
    G4double ySizeSampleChamber		= 24. *cm;
    G4double zSizeSampleChamber		= 35. *cm;
    
    G4double d_CuShielding_sides	= 8. *cm; // thickness of Cu shielding
    G4double d_CuShielding_bottom   = 16. *cm; // additional Cu plate at bottom
    G4double h_CuShielding_hole     = 8.66 *cm; // height of hole in Cu shielding bottom for detector
    G4double r_CuShielding_hole     = 5.58 *cm; // radius of hole in Cu shielding bottom for detector
    
    G4double r_CuShieldingCFHole    = 1.8*cm; // radius of hole for coldfinger
    
    G4double xSizeCuShielding       = xSizeSampleChamber+2.*d_CuShielding_sides;
    G4double ySizeCuShielding       = ySizeSampleChamber+2.*d_CuShielding_sides;
    G4double zSizeCuShielding       = zSizeSampleChamber+d_CuShielding_bottom;
    
    G4double zPosCuShielding		= zPosEndcap+heightEndcap/2.-(heightCuHsg+heightCuHsg_bottom)+(zSizeCuShielding/2.-d_CuShielding_bottom)+h_CuShielding_hole;
    
    // inner Pb shielding (low activity)
    G4double d_innerPbShielding		= 5. *cm; // thickness of inner Pb shielding
    
    G4double xSizeInnerPbShielding  = xSizeCuShielding+2.*d_innerPbShielding;
    G4double ySizeInnerPbShielding  = ySizeCuShielding+2.*d_innerPbShielding;
    G4double zSizeInnerPbShielding  = zSizeCuShielding+d_innerPbShielding;
    
    G4double zPosInnerPbShielding   = zPosCuShielding-d_innerPbShielding/2.;
    
    // outer Pb shielding
    G4double d_outerPbShielding		= 15. *cm; // thickness of outer Pb shielding
    
    G4double xSizeOuterPbShielding  = xSizeInnerPbShielding+2.*d_outerPbShielding;
    G4double ySizeOuterPbShielding  = ySizeInnerPbShielding+2.*d_outerPbShielding;
    G4double zSizeOuterPbShielding  = zSizeInnerPbShielding+d_outerPbShielding;
    
    G4double zPosOuterPbShielding   = zPosInnerPbShielding-d_outerPbShielding/2.;
    
    
    // fixed top part
    //
    // Cu plate
    G4double xSizeCuPlate_fixed     = 80.*cm;
    G4double ySizeCuPlate_fixed     = 20.*cm;
    G4double zSizeCuPlate_fixed     = 2.*cm;
    
    G4double zPosCuPlate_fixed      = zPosOuterPbShielding+zSizeOuterPbShielding/2.+zSizeCuPlate_fixed/2.+0.001*cm;
    G4double yPosCuPlate_fixed      = -(ySizeCuShielding/2.+ySizeCuPlate_fixed/2.);
    
    // inner Pb shielding (low activity)
    G4double xSizeInnerPbShielding_fixed    = 50.*cm;
    G4double ySizeInnerPbShielding_fixed    = 5.*cm;
    G4double zSizeInnerPbShielding_fixed    = 20.*cm;
    
    G4double yPosInnerPbShielding_fixed     = -(ySizeCuShielding/2.+ySizeInnerPbShielding_fixed/2.);
    G4double zPosInnerPbShielding_fixed     = zPosCuPlate_fixed+zSizeCuPlate_fixed/2.+zSizeInnerPbShielding_fixed/2.+0.001*cm;
    
    // outer Pb shielding
    G4double xSizeOuterPbShielding_fixed    = 80.*cm;
    G4double ySizeOuterPbShielding_fixed    = 20.*cm;
    G4double zSizeOuterPbShielding_fixed    = 25.*cm;
    
    G4double yPosOuterPbShielding_fixed     = -(ySizeCuShielding/2.+ySizeOuterPbShielding_fixed/2.);
    G4double zPosOuterPbShielding_fixed     = zPosCuPlate_fixed+zSizeCuPlate_fixed/2.+zSizeOuterPbShielding_fixed/2.+0.001*cm;
    
    
    // movable top part
    //
    // horizontal gap between shielding and lid
    G4double d_lid = 0.2*cm;
    
    // Cu plate thin
    G4double xSizeCuPlateThin_mov       = 100.*cm;
    G4double ySizeCuPlateThin_mov       = 60.*cm;
    G4double zSizeCuPlateThin_mov       = 2.*cm;
    
    G4double yPosCuPlateThin_mov       = -ySizeCuShielding/2.+ySizeCuPlateThin_mov/2.+0.001*cm;
    G4double zPosCuPlateThin_mov        = zPosOuterPbShielding+zSizeOuterPbShielding/2.+zSizeCuPlateThin_mov/2.+d_lid;
    
    // Cu plate thick
    G4double xSizeCuPlateThick_mov      = 40.*cm;
    G4double ySizeCuPlateThick_mov      = 40.*cm;
    G4double zSizeCuPlateThick_mov      = 8.*cm;
    
    G4double yPosCuPlateThick_mov       = -ySizeCuShielding/2.+ySizeCuPlateThick_mov/2.+0.001*cm;
    G4double zPosCuPlateThick_mov       = zPosOuterPbShielding+zSizeOuterPbShielding/2.+zSizeCuPlateThick_mov/2.+d_lid;
    
    // inner Pb shielding (low activity)
    G4double xSizeInnerPbShieldingSide_mov  = 5.*cm;
    G4double ySizeInnerPbShieldingSide_mov  = 45.*cm;
    G4double zSizeInnerPbShieldingSide_mov  = 20.*cm;
    
    G4double xSizeInnerPbShieldingBack_mov  = 40.*cm;
    G4double ySizeInnerPbShieldingBack_mov  = 5.*cm;
    G4double zSizeInnerPbShieldingBack_mov  = 20.*cm;
    
    G4double xSizeInnerPbShieldingTop_mov   = 40.*cm;
    G4double ySizeInnerPbShieldingTop_mov   = 40.*cm;
    G4double zSizeInnerPbShieldingTop_mov   = 5.*cm;
    
    G4double yPosInnerPbShielding_mov       = -ySizeCuShielding/2.+ySizeInnerPbShieldingTop_mov/2.;
    G4double zPosInnerPbShielding_mov       = zPosCuPlateThick_mov+zSizeCuPlateThick_mov/2.+zSizeInnerPbShieldingTop_mov/2.;
    
    // outer Pb shielding
    G4double xSizeOuterPbShielding_mov      = 80.*cm;
    G4double ySizeOuterPbShielding_mov      = 60.*cm;
    G4double zSizeOuterPbShielding_mov      = 25.*cm;
    
    G4double yPosOuterPbShielding_mov       = -ySizeCuShielding/2.+ySizeOuterPbShielding_mov/2.;
    G4double zPosOuterPbShielding_mov       = zPosCuPlateThin_mov+zSizeCuPlateThin_mov/2.+zSizeOuterPbShielding_mov/2.;


    // Cu shielding closing gap
    G4double xSizeCuShieldingGap    = 2.*cm;
    G4double ySizeCuShieldingGap    = 60.*cm;
    G4double zSizeCuShieldingGap    = 2.*cm;
    
    G4double xPosCuShieldingGap     = xSizeOuterPbShielding/2.+xSizeCuShieldingGap/2.+0.5*cm;
    G4double yPosCuShieldingGap     = -ySizeCuShielding/2.+ySizeCuShieldingGap/2.;
    G4double zPosCuShieldingGap     = zPosCuPlateThin_mov-zSizeCuPlateThin_mov/2.-zSizeCuShieldingGap/2.;
    
    
    // +++++++ CBSS2 calibration source ++++++++++++++++++++++++++++++++++
    /*
    // PS container (Semadeni)
    G4double outerRadiusPS = 3.83*cm;
    G4double innerRadiusPS = 3.68*cm;
    G4double heightPS = 3.9*cm;
    G4double heightPS_top = 0.12*cm;
    G4double heightPS_bottom = 0.15*cm;
    G4double dPS_bottom = 0.1*cm;
    
    G4double zPosPS = zPosEndcap+heightEndcap/2.+heightPS/2.+overlap;
    
    // source
    G4double heightCBSS2 = 2.82*cm;
    
    G4double zPosCBSS2 = zPosEndcap+heightEndcap/2.+heightCBSS2/2.+dPS_bottom+heightPS_bottom+2.*overlap;
    */
    
    // +++++++ important dimensions ++++++++++++++++++++++++++++++++++
    G4cout << "############################" << G4endl;
    G4cout << "z-position of endcap: " << (zPosEndcap+heightEndcap/2.)/cm << " cm" << G4endl;
    G4cout << "z-position of sample cavity center: " << (zPosCuShielding+zSizeCuShielding/2.-zSizeSampleChamber/2.)/cm << " cm" << G4endl;
    //G4cout << "z-position of gap center: " << (zPosOuterPbShielding+zSizeOuterPbShielding/2.+d_lid/2.)/cm << " cm" << G4endl;
    //G4cout << "z-position of source: " << zPosCBSS2/cm << " cm" << G4endl;
    G4cout << "############################" << G4endl;

	
	//======= construct volumes ======================================
    
    // +++++++ calibration source ++++++++++++++++++++++++++++++++++
    
    /*
    G4Box* source_box = new G4Box("source_box", 1.675*cm, 0.55*cm, 0.1*cm);
    G4Sphere* hole_source   	= new G4Sphere      ("hole_source", 0, 0.05*cm, 0, 360.*deg, 0., 180.*deg);
    G4SubtractionSolid* source_sub	= new G4SubtractionSolid("source_sub",source_box,hole_source,0,G4ThreeVector(0,0,0));
    
    G4LogicalVolume* source_log = new G4LogicalVolume(source_sub,pmma_mat,"source_log",0,0,0);
    new G4PVPlacement(0,G4ThreeVector(0,0,4.75*cm),source_log,"source",expHall_log,false,0);
    */
    
    // +++++++ CBSS2 calibration source ++++++++++++++++++++++++++++++++++
    // PS container (Semadeni)
    /*
    G4Tubs*	PSwall_tube			= new G4Tubs	("PSwall_tube", innerRadiusPS, outerRadiusPS, heightPS/2., startAngle, spanningAngle);
    G4Tubs*	PSbottom_tube		= new G4Tubs	("PSbottom_tube", 0.*cm, innerRadiusPS+0.001*cm, heightPS_bottom/2., startAngle, spanningAngle);
    G4Tubs*	PStop_tube          = new G4Tubs	("PStop_tube", 0.*cm, innerRadiusPS+0.001*cm, heightPS_top/2., startAngle, spanningAngle);
    
    G4UnionSolid* PS_uni1       = new G4UnionSolid("PS_uni1",PSwall_tube,PStop_tube,0,G4ThreeVector(0.,0.,heightPS/2.-heightPS_top/2.-0.001*cm));
    G4UnionSolid* PS_uni2       = new G4UnionSolid("PS_uni2",PS_uni1,PSbottom_tube,0,G4ThreeVector(0.,0.,-heightPS/2.+heightPS_bottom/2.+dPS_bottom));

    // source
    G4Tubs*	CBSS2_tube			= new G4Tubs	("CBSS2_tube", 0.*cm, innerRadiusPS-overlap, heightCBSS2/2., startAngle, spanningAngle);
    
    // define logical volumes
    G4LogicalVolume* 	PS_log			= new G4LogicalVolume  (PS_uni2,ps_mat,"PS_log",0,0,0);
    G4LogicalVolume* 	CBSS2_log		= new G4LogicalVolume  (CBSS2_tube,matrix_mat,"CBSS2_log",0,0,0);
    
    // place volumes
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosPS),PS_log,"PS",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosCBSS2),CBSS2_log,"CBSS2",expHall_log,false,0);
    
    // set colors
    PS_log->SetVisAttributes(lightblue);
    CBSS2_log->SetVisAttributes(violet);
*/
    
    
    // +++++++ parts of the shielding ++++++++++++++++++++++++++++++++++
    
    // bottom part
    //
    // Sample cavity
    G4Box* SampleCavity_box = new G4Box("SampleCavity_box",xSizeSampleChamber/2., ySizeSampleChamber/2., zSizeSampleChamber/2.+0.001*cm);
    
    // hole for HPGe detector
    G4Tubs* CuShieldingHole_tubs = new G4Tubs("CuShieldingHole_tubs",0.*cm,r_CuShielding_hole,h_CuShielding_hole/2.+0.001*cm, 0.*deg, 360.*deg);

    // hole for Coldfinger
    G4Tubs* CuShieldingCFHole_tubs = new G4Tubs("CuShieldingCFHole_tubs",0.*cm,r_CuShieldingCFHole,ySizeOuterPbShielding/2.+0.001*cm, 0.*deg, 360.*deg);
    
    // Cu shielding
    G4Box* CuShielding_box = new G4Box("CuShielding_box", xSizeCuShielding/2., ySizeCuShielding/2., zSizeCuShielding/2.);
    
    // inner Pb shielding
    G4Box* innerPbShielding_box = new G4Box("innerPbShielding_box", xSizeInnerPbShielding/2., ySizeInnerPbShielding/2., zSizeInnerPbShielding/2.);
    G4Box* innerPbShieldingHole_box = new G4Box("innerPbShieldingHole_box", xSizeCuShielding/2.+0.001*cm, ySizeCuShielding/2.+0.001*cm, zSizeCuShielding/2+0.001*cm);
    
    // outer Pb shielding
    G4Box* outerPbShielding_box = new G4Box("outerPbShielding_box", xSizeOuterPbShielding/2., ySizeOuterPbShielding/2., zSizeOuterPbShielding/2.);
    G4Box* outerPbShieldingHole_box = new G4Box("outerPbShieldingHole_box", xSizeInnerPbShielding/2.+0.001*cm, ySizeInnerPbShielding/2.+0.001*cm, zSizeInnerPbShielding/2+0.001*cm);
    
    
    // top part fixed
    //
    // Cu plate
    G4Box* CuPlateFixed_box = new G4Box("CuPlateFixed_box",xSizeCuPlate_fixed/2.,ySizeCuPlate_fixed/2.,zSizeCuPlate_fixed/2.);
    
    // inner Pb layer
    G4Box* innerPbShieldingFixed_box = new G4Box("innerPbShieldingFixed_box",xSizeInnerPbShielding_fixed/2.,ySizeInnerPbShielding_fixed/2.,zSizeInnerPbShielding_fixed/2.);

    // outer Pb layer
    G4Box* outerPbShieldingFixed_box = new G4Box("outerPbShieldingFixed_box",xSizeOuterPbShielding_fixed/2.,ySizeOuterPbShielding_fixed/2.,zSizeOuterPbShielding_fixed/2.);
    G4Box* outerPbShieldingFixedHole_box = new G4Box("outerPbShieldingFixedHole_box",xSizeInnerPbShielding_fixed/2.+0.001*cm,ySizeInnerPbShielding_fixed/2.+0.001*cm,zSizeInnerPbShielding_fixed/2.+0.001*cm);

    // movable top part
    //
    // Cu plate
    G4Box* CuPlateThinMov_box = new G4Box("CuPlateThinMov_box",xSizeCuPlateThin_mov/2.,ySizeCuPlateThin_mov/2.,zSizeCuPlateThin_mov/2.);
    G4Box* CuPlateThickMov_box = new G4Box("CuPlateThickMov",xSizeCuPlateThick_mov/2.,ySizeCuPlateThick_mov/2.,zSizeCuPlateThick_mov/2.);
    
    // inner Pb layer
    G4Box* innerPbShieldingSideMov_box = new G4Box("innerPbShieldingSideMov_box",xSizeInnerPbShieldingSide_mov/2.,ySizeInnerPbShieldingSide_mov/2.,zSizeInnerPbShieldingSide_mov/2.);
    G4Box* innerPbShieldingBackMov_box = new G4Box("innerPbShieldingBackMov_box",xSizeInnerPbShieldingBack_mov/2.+0.001*cm,ySizeInnerPbShieldingBack_mov/2.,zSizeInnerPbShieldingBack_mov/2.);
    G4Box* innerPbShieldingTopMov_box = new G4Box("innerPbShieldingTopMov_box",xSizeInnerPbShieldingTop_mov/2.+0.001*cm,ySizeInnerPbShieldingTop_mov/2.+0.001*cm,zSizeInnerPbShieldingTop_mov/2.);
    
    // outer Pb layer
    G4Box* outerPbShieldingMov_box = new G4Box("outerPbShieldingMov_box",xSizeOuterPbShielding_mov/2.,ySizeOuterPbShielding_mov/2.,zSizeOuterPbShielding_mov/2.);
    G4Box* outerPbShieldingHoleSideMov_box = new G4Box("outerPbShieldingHoleSideMov",xSizeInnerPbShieldingSide_mov/2.+0.005*cm,ySizeInnerPbShieldingSide_mov/2.+0.005*cm,zSizeInnerPbShieldingSide_mov/2.+0.005*cm);
    G4Box* outerPbShieldingHoleBackMov_box = new G4Box("outerPbShieldingHoleSideMov_box",xSizeInnerPbShieldingBack_mov/2.+0.005*cm,ySizeInnerPbShieldingBack_mov/2.+0.005*cm,zSizeInnerPbShieldingBack_mov/2.+0.005*cm);
    G4Box* outerPbShieldingHoleTopMov_box = new G4Box("outerPbShieldingHoleTopMov_box",xSizeInnerPbShieldingTop_mov/2.+0.005*cm,ySizeInnerPbShieldingTop_mov/2.+0.005*cm,(zSizeInnerPbShieldingTop_mov+zSizeCuPlateThick_mov-zSizeCuPlateThin_mov)/2.+0.005*cm);
    
    // Cu shielding closing gap
    G4Box* CuShieldingGap_box = new G4Box("CuShieldingGap_box",xSizeCuShieldingGap/2.,ySizeCuShieldingGap/2.,zSizeCuShieldingGap/2.);

    
    // combine volumes
    
    // bottom part
    //
    // Cu shielding
    G4SubtractionSolid* CuShielding_sub1 = new G4SubtractionSolid("CuShielding_sub1",CuShielding_box,SampleCavity_box,0,G4ThreeVector(0,0,zSizeCuShielding/2.-zSizeSampleChamber/2.+0.001*cm));
    G4SubtractionSolid* CuShielding_sub2 = new G4SubtractionSolid("CuShielding_sub2",CuShielding_sub1,CuShieldingHole_tubs,0,G4ThreeVector(0,0,-zSizeCuShielding/2.+d_CuShielding_bottom-h_CuShielding_hole/2.+0.001*cm));
    G4SubtractionSolid* CuShielding_sub3 = new G4SubtractionSolid("CuShielding_sub3",CuShielding_sub2,CuShieldingCFHole_tubs,G4Transform3D(rm2,G4ThreeVector(0.,-ySizeOuterPbShielding/2.,-zPosCuShielding+zPosArm)));
    
    // inner Pb shielding
    G4SubtractionSolid* innerPbShielding_sub1 = new G4SubtractionSolid("innerPbShielding_sub1",innerPbShielding_box,innerPbShieldingHole_box,0,G4ThreeVector(0,0,zSizeInnerPbShielding/2.-zSizeCuShielding/2.));
    G4SubtractionSolid* innerPbShielding_sub2 = new G4SubtractionSolid("innerPbShielding_sub2",innerPbShielding_sub1,CuShieldingCFHole_tubs,G4Transform3D(rm2,G4ThreeVector(0.,-ySizeOuterPbShielding/2.,-zPosInnerPbShielding+zPosArm)));
    
    // outer Pb shielding
    G4SubtractionSolid* outerPbShielding_sub1 = new G4SubtractionSolid("outerPbShielding_sub1",outerPbShielding_box,outerPbShieldingHole_box,0,G4ThreeVector(0,0,zSizeOuterPbShielding/2.-zSizeInnerPbShielding/2.));
    G4SubtractionSolid* outerPbShielding_sub2 = new G4SubtractionSolid("outerPbShielding_sub2",outerPbShielding_sub1,CuShieldingCFHole_tubs,G4Transform3D(rm2,G4ThreeVector(0.,-ySizeOuterPbShielding/2.,-zPosOuterPbShielding+zPosArm)));
    
    // top part fixed
    //
    // outer Pb layer
    G4SubtractionSolid* outerPbShieldingFixed_sub = new G4SubtractionSolid("outerPbShieldingFixed_sub",outerPbShieldingFixed_box,outerPbShieldingFixedHole_box,0,G4ThreeVector(0,ySizeOuterPbShielding_fixed/2.-ySizeInnerPbShielding_fixed/2.,-zSizeOuterPbShielding_fixed/2.+zSizeInnerPbShielding_fixed/2.));
    
    // top part movable
    //
    // Cu plate
    G4UnionSolid* CuPlateMov_uni = new G4UnionSolid("CuPlateMov_uni",CuPlateThinMov_box,CuPlateThickMov_box,0,G4ThreeVector(0,yPosCuPlateThick_mov-yPosCuPlateThin_mov,zPosCuPlateThick_mov-zPosCuPlateThin_mov));
    
    // inner Pb layer
    G4UnionSolid* innerPbShieldingMov_uni1 = new G4UnionSolid("innerPbShieldingMov_uni1",innerPbShieldingTopMov_box,innerPbShieldingSideMov_box,0,G4ThreeVector(xSizeInnerPbShieldingTop_mov/2.+xSizeInnerPbShieldingSide_mov/2.,ySizeInnerPbShieldingSide_mov/2.-ySizeInnerPbShieldingTop_mov/2.,-zSizeInnerPbShieldingTop_mov/2.+zSizeInnerPbShieldingSide_mov/2.-(zSizeCuPlateThick_mov-zSizeCuPlateThin_mov)));
    G4UnionSolid* innerPbShieldingMov_uni2 = new G4UnionSolid("innerPbShieldingMov_uni2",innerPbShieldingMov_uni1,innerPbShieldingSideMov_box,0,G4ThreeVector(-(xSizeInnerPbShieldingTop_mov/2.+xSizeInnerPbShieldingSide_mov/2.),ySizeInnerPbShieldingSide_mov/2.-ySizeInnerPbShieldingTop_mov/2.,-zSizeInnerPbShieldingTop_mov/2.+zSizeInnerPbShieldingSide_mov/2.-(zSizeCuPlateThick_mov-zSizeCuPlateThin_mov)));
    G4UnionSolid* innerPbShieldingMov_uni3 = new G4UnionSolid("innerPbShieldingMov_uni3",innerPbShieldingMov_uni2,innerPbShieldingBackMov_box,0,G4ThreeVector(0,ySizeInnerPbShieldingTop_mov/2.+ySizeInnerPbShieldingBack_mov/2.,-zSizeInnerPbShieldingTop_mov/2.+zSizeInnerPbShieldingSide_mov/2.-(zSizeCuPlateThick_mov-zSizeCuPlateThin_mov)));
    
    // outer Pb layer
    G4SubtractionSolid* outerPbShieldingMov_sub1 = new G4SubtractionSolid("outerPbShieldingMov_sub1",outerPbShieldingMov_box,outerPbShieldingHoleSideMov_box,0,G4ThreeVector(xSizeCuPlateThick_mov/2.+xSizeInnerPbShieldingSide_mov/2.,ySizeInnerPbShieldingSide_mov/2.-ySizeOuterPbShielding_mov/2.,-zSizeOuterPbShielding_mov/2.+zSizeInnerPbShieldingSide_mov/2.));
    G4SubtractionSolid* outerPbShieldingMov_sub2 = new G4SubtractionSolid("outerPbShieldingMov_sub2",outerPbShieldingMov_sub1,outerPbShieldingHoleSideMov_box,0,G4ThreeVector(-(xSizeCuPlateThick_mov/2.+xSizeInnerPbShieldingSide_mov/2.),ySizeInnerPbShieldingSide_mov/2.-ySizeOuterPbShielding_mov/2.,-zSizeOuterPbShielding_mov/2.+zSizeInnerPbShieldingSide_mov/2.));
    G4SubtractionSolid* outerPbShieldingMov_sub3 = new G4SubtractionSolid("outerPbShieldingMov_sub3",outerPbShieldingMov_sub2,outerPbShieldingHoleBackMov_box,0,G4ThreeVector(0,ySizeInnerPbShieldingTop_mov+ySizeInnerPbShieldingBack_mov/2.-ySizeOuterPbShielding_mov/2.,-zSizeOuterPbShielding_mov/2.+zSizeInnerPbShieldingBack_mov/2.));
    G4SubtractionSolid* outerPbShieldingMov_sub4 = new G4SubtractionSolid("outerPbShieldingMov_sub4",outerPbShieldingMov_sub3,outerPbShieldingHoleTopMov_box,0,G4ThreeVector(0,ySizeInnerPbShieldingTop_mov/2.-ySizeOuterPbShielding_mov/2.,-zSizeOuterPbShielding_mov/2.+(zSizeInnerPbShieldingTop_mov+zSizeCuPlateThick_mov-zSizeCuPlateThin_mov)/2.));


    
    // define logical volumes
    // bottom part
    //
    G4LogicalVolume* 	CuShielding_log			= new G4LogicalVolume  (CuShielding_sub3,copper_mat,"CuShielding_log",0,0,0);
    G4LogicalVolume* 	innerPbShielding_log	= new G4LogicalVolume  (innerPbShielding_sub2,lead_mat,"innerPbShielding_log",0,0,0);
    G4LogicalVolume* 	outerPbShielding_log	= new G4LogicalVolume  (outerPbShielding_sub2,lead_mat,"outerPbShielding_log",0,0,0);
    
    // top part fixed
    //
    G4LogicalVolume* 	CuPlateFixed_log            = new G4LogicalVolume  (CuPlateFixed_box,copper_mat,"CuPlateFixed_log",0,0,0);
    G4LogicalVolume* 	innerPbShieldingFixed_log	= new G4LogicalVolume  (innerPbShieldingFixed_box,lead_mat,"innerPbShieldingFixed_log",0,0,0);
    G4LogicalVolume* 	outerPbShieldingFixed_log	= new G4LogicalVolume  (outerPbShieldingFixed_sub,lead_mat,"outerPbShieldingFixed_log",0,0,0);

    // top part movable
    //
    G4LogicalVolume*    CuPlateMov_log              = new G4LogicalVolume  (CuPlateMov_uni,copper_mat,"CuPlateMov_log",0,0,0);
    G4LogicalVolume* 	innerPbShieldingMov_log     = new G4LogicalVolume  (innerPbShieldingMov_uni3,lead_mat,"innerPbShieldingMov_log",0,0,0);
    G4LogicalVolume* 	outerPbShieldingMov_log     = new G4LogicalVolume  (outerPbShieldingMov_sub4,lead_mat,"outerPbShieldingMov_log",0,0,0);
    
    // Cu shielding closing gap
    //
    G4LogicalVolume*    CuShieldingGap_log          = new G4LogicalVolume  (CuShieldingGap_box,copper_mat,"CuShieldingGap_log",0,0,0);


    
    // place volumes
    // bottom part
    //
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosCuShielding),CuShielding_log,"CuShielding",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosInnerPbShielding),innerPbShielding_log,"innerPbShielding",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosOuterPbShielding),outerPbShielding_log,"outerPbShielding",expHall_log,false,0);
    
    // top part fixed
    //
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosCuPlate_fixed,zPosCuPlate_fixed),CuPlateFixed_log,"CuPlateFixed",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosInnerPbShielding_fixed,zPosInnerPbShielding_fixed),innerPbShieldingFixed_log,"innerPbShieldingFixed",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosOuterPbShielding_fixed,zPosOuterPbShielding_fixed),outerPbShieldingFixed_log,"outerPbShieldingFixed",expHall_log,false,0);

    // top part movable
    //
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosCuPlateThin_mov,zPosCuPlateThin_mov),CuPlateMov_log,"CuPlateMovable",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosInnerPbShielding_mov,zPosInnerPbShielding_mov),innerPbShieldingMov_log,"innerPbShieldingMovable",expHall_log,false,0);
    new G4PVPlacement    (0,G4ThreeVector(0.,yPosOuterPbShielding_mov,zPosOuterPbShielding_mov),outerPbShieldingMov_log,"outerPbShieldingMovable",expHall_log,false,0);

    // Cu shielding closing gap
    //
    //new G4PVPlacement    (0,G4ThreeVector(xPosCuShieldingGap,yPosCuShieldingGap,zPosCuShieldingGap),CuShieldingGap_log,"CuShieldingGap",expHall_log,false,0);

    
    // set colors
    CuShielding_log->SetVisAttributes(orange);
    CuPlateFixed_log->SetVisAttributes(orange);
    CuPlateMov_log->SetVisAttributes(orange);
    innerPbShielding_log->SetVisAttributes(cyan);
    innerPbShieldingFixed_log->SetVisAttributes(cyan);
    innerPbShieldingMov_log->SetVisAttributes(cyan);
    outerPbShielding_log->SetVisAttributes(blue);
    outerPbShieldingFixed_log->SetVisAttributes(blue);
    outerPbShieldingMov_log->SetVisAttributes(blue);
    
	// +++++++ parts of Ge detector housing ++++++++++++++++++++++++++++++++++
	
	G4Tubs*	Endcap_tube			= new G4Tubs	("Endcap_tube", 0.*cm, outerRadiusEndcap-0.005*cm, heightEndcap/2., startAngle, spanningAngle);
	G4Tubs*	CuHsg_tube			= new G4Tubs	("CuHsg_tube", innerRadiusCuHsg, outerRadiusEndcap, heightCuHsg/2., startAngle, spanningAngle);
	G4Tubs*	CuHsg_bottom_tube1	= new G4Tubs	("CuHsg_bottom_tube1", innerRadiusCuHsg_bottom, outerRadiusEndcap+0.005*cm, heightCuHsg_bottom/2., startAngle, spanningAngle);
    G4Tubs*	CuHsg_bottom_tube2	= new G4Tubs	("CuHsg_bottom_tube2", 0.*cm, outerRadiusEndcap-0.005*cm, dCuHsg_bottom/2., startAngle, spanningAngle);
    
    G4Tubs*	CuHsg_bottom_solid	= new G4Tubs	("CuHsg_bottom_solid", 0.*cm, outerRadiusEndcap, heightCuHsg_bottom/2., startAngle, spanningAngle);
    
	G4Tubs*	Arm_tube			= new G4Tubs	("Arm_tube", innerRadiusArm, outerRadiusArm, heightArm/2., startAngle, spanningAngle);
    G4Tubs*	ArmInside_tube		= new G4Tubs	("Arm_tube", 0.*cm, innerRadiusArm-0.005*cm, heightArm/2., startAngle, spanningAngle);
	G4Tubs*	Coldfinger_tube		= new G4Tubs	("Coldfinger_tube", 0.*cm, outerRadiusColdfinger, heightArm/2., startAngle, spanningAngle);
	
    // combine volumes
    G4UnionSolid* CuHsg_bottom_uni	= new G4UnionSolid("CuHsg_bottom_uni",CuHsg_bottom_tube1,CuHsg_bottom_tube2,0,G4ThreeVector(0.,0.,-(heightCuHsg_bottom-dCuHsg_bottom)/2.));
    G4SubtractionSolid* CuHsg_bottom_sub	= new G4SubtractionSolid("CuHsg_bottom_sub",CuHsg_bottom_uni,ArmInside_tube,G4Transform3D(rm2,G4ThreeVector(0.,yPosArm,0)));
    
    G4SubtractionSolid* Arm_sub             = new G4SubtractionSolid("Arm_sub",Arm_tube,CuHsg_bottom_solid,G4Transform3D(rm2,G4ThreeVector(0.,0,yPosArm)));

    G4UnionSolid* CuHsg_bottom_uni2          = new G4UnionSolid("CuHsg_bottom_uni2",CuHsg_bottom_sub,Arm_sub,G4Transform3D(rm2,G4ThreeVector(0.,yPosArm+0.1*cm,0)));

    G4UnionSolid* CuHsg_uni1                = new G4UnionSolid("CuHsg_uni1",Endcap_tube,CuHsg_tube,0,G4ThreeVector(0.,0.,-heightCuHsg/2.+heightEndcap/2.-0.005*cm));
    G4UnionSolid* CuHsg_uni2                = new G4UnionSolid("CuHsg_uni2",CuHsg_uni1,CuHsg_bottom_uni2,0,G4ThreeVector(0.,0.,-heightCuHsg+heightEndcap/2.-heightCuHsg_bottom/2.+0.005*cm));

	// define logical volumes
	G4LogicalVolume* 	CuHsg_log			= new G4LogicalVolume  (CuHsg_uni2,copper_mat,"CuHsg_log",0,0,0);
	G4LogicalVolume* 	Coldfinger_log		= new G4LogicalVolume  (Coldfinger_tube,copper_mat,"Coldfinger_log",0,0,0);
	
	// place volumes
	new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosEndcap),CuHsg_log,"CuHsg",expHall_log,false,0);
	new G4PVPlacement    (G4Transform3D(rm2,G4ThreeVector(0.,yPosArm,zPosArm)),Coldfinger_log,"Coldfinger",expHall_log,false,0);
	
	// set colors
	CuHsg_log->SetVisAttributes(orange);
	Coldfinger_log->SetVisAttributes(orange);

    
	
	// +++++++ parts of Ge detector holder ++++++++++++++++++++++++++++++++++ 
	
    // ++++++++++++++ copper holder
	G4Tubs*			DetHolder_tube			= new G4Tubs        ("DetHolder_tube", innerRadiusDetHolder, outerRadiusDetHolder, heightDetHolder/2.+overlap, startAngle, spanningAngle);
	
	G4Tubs*			DetHolder_bottom_tube1 	= new G4Tubs        ("DetHolder_bottom_tube1", innerRadiusDetHolder_bottom1, outerRadiusDetHolder, heightDetHolder_bottom1/2.+overlap, startAngle, spanningAngle);
	
	G4Tubs*			DetHolder_bottom_tube2 	= new G4Tubs        ("DetHolder_bottom_tube2", innerRadiusDetHolder_bottom1, outerRadiusDetHolder_bottom2, heightDetHolder_bottom2/2.+overlap, startAngle, spanningAngle);
	
	G4Tubs*			DetHolder_bottom_tube3 	= new G4Tubs        ("DetHolder_bottom_tube3", innerRadiusDetHolder_bottom3, outerRadiusDetHolder_bottom2, heightDetHolder_bottom3/2., startAngle, spanningAngle);
    
    G4Tubs*			DetHolderSupport_tube1			= new G4Tubs        ("DetHolder_tube", outerRadiusDetHolder-0.005*cm, outerRadiusSupportHolder, heightSupportHolder1/2., startAngle, spanningAngle);
    
    G4Tubs*			DetHolderSupport_tube2			= new G4Tubs        ("DetHolder_tube", outerRadiusDetHolder-0.005*cm, outerRadiusSupportHolder, heightSupportHolder2/2., startAngle, spanningAngle);
	
	// join parts of detector holder
	G4UnionSolid*  		DetHolder_uni1	= new G4UnionSolid("DetHolder_uni1",DetHolder_tube,DetHolder_bottom_tube1,0,G4ThreeVector(0.,0.,-(heightDetHolder+heightDetHolder_bottom1)/2.));
	G4UnionSolid*  		DetHolder_uni2	= new G4UnionSolid("DetHolder_uni2",DetHolder_uni1,DetHolder_bottom_tube2,0,G4ThreeVector(0.,0.,-(heightDetHolder+2.*heightDetHolder_bottom1+heightDetHolder_bottom2)/2.));
	G4UnionSolid*  		DetHolder_uni3	= new G4UnionSolid("DetHolder_uni3",DetHolder_uni2,DetHolder_bottom_tube3,0,G4ThreeVector(0.,0.,-(heightDetHolder+2.*heightDetHolder_bottom1+2.*heightDetHolder_bottom2+heightDetHolder_bottom3)/2.));

    G4UnionSolid*  		DetHolder_uni4	= new G4UnionSolid("DetHolder_uni4",DetHolder_uni3,DetHolderSupport_tube1,0,G4ThreeVector(0.,0.,(heightDetHolder/2.-heightSupportHolder1/2.)));
    G4UnionSolid*  		DetHolder_uni5	= new G4UnionSolid("DetHolder_uni5",DetHolder_uni4,DetHolderSupport_tube2,0,G4ThreeVector(0.,0.,(heightDetHolder/2.-heightSupportHolder2/2.-d_Support1)));
    G4UnionSolid*  		DetHolder_uni6	= new G4UnionSolid("DetHolder_uni6",DetHolder_uni5,DetHolderSupport_tube2,0,G4ThreeVector(0.,0.,(heightDetHolder/2.-heightSupportHolder2/2.-d_Support2)));
    
    // ++++++++++++++ teflon holder
	G4Tubs*			TeflonHolder_top_tube			= new G4Tubs        ("TeflonHolder_top_tube", 0.*cm, outerRadiusGe+d_TefHolder_side+0.001*cm, d_TefHolder_top/2.+0.001*cm, startAngle, spanningAngle);
	G4Tubs*			TeflonHolder_side_tube			= new G4Tubs        ("TeflonHolder_side_tube", outerRadiusGe+0.005*cm, outerRadiusGe+d_TefHolder_side, (heightTeflonHolder-d_TefHolder_top)/2., startAngle, spanningAngle);
    
    // combine volumes
    G4UnionSolid*  		TeflonHolder_uni	= new G4UnionSolid("TeflonHolder_uni",TeflonHolder_top_tube,TeflonHolder_side_tube,0,G4ThreeVector(0.,0.,-(heightTeflonHolder-d_TefHolder_top)/2.));
    
	// define logical volumes
	G4LogicalVolume* 	DetHolder_log  = new G4LogicalVolume  (DetHolder_uni6,copper_mat,"DetHolder_log",0,0,0);
    
    G4LogicalVolume* 	TeflonHolder_log  = new G4LogicalVolume  (TeflonHolder_uni,teflon_mat,"TeflonHolder_log",0,0,0);
	
	// place volumes
	new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosDetHolder),DetHolder_log,"DetHolder",expHall_log,false,0);
    
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosTeflonHolder),TeflonHolder_log,"TeflonHolder",expHall_log,false,0);
	
	DetHolder_log->SetVisAttributes(orange);
	
	
	// +++++++ parts of Ge detector ++++++++++++++++++++++++++++++++++
    
    
    // inner part (HPGe)
    G4Tubs*			GeIn_tube     = new G4Tubs        ("GeIn_tube", 0.*cm, outerRadiusGe-d_LiContact, (heightGe-d_LiContact)/2., startAngle, spanningAngle);
    
    // substract hole from cylinder
    G4Tubs*			Hole_tube 	= new G4Tubs        ("Hole_tube", 0.*cm, outerRadiusHole, heightHole/2.+0.005*cm, startAngle, spanningAngle);
    G4SubtractionSolid*  	GeIn_sub		= new G4SubtractionSolid("GeIn_sub",GeIn_tube,Hole_tube,0,G4ThreeVector(0.,0.,-(heightGe/2.-heightHole/2.+0.005*cm)));
    
    /*
    G4Tubs*			GeCenterIn_tube     = new G4Tubs        ("GeCenterIn_tube", 0.*cm, outerRadiusGe-d_LiContact, heightGe/2.-edgeRadius, startAngle, spanningAngle);
    G4Tubs*			GeEndsIn_tube       = new G4Tubs        ("GeEndsIn_tube", 0.*cm, outerRadiusGe-edgeRadius, edgeInRadius/2.+0.0001*cm, startAngle, spanningAngle);
    G4Tubs*			GeBottomIn_tube     = new G4Tubs        ("GeBottomIn_tube", 0.*cm, outerRadiusGe-edgeRadius, d_LiContact/2.+0.0001*cm, startAngle, spanningAngle);
    G4Torus*        GeEdgeIn_torus      = new G4Torus       ("GeEdgeIn_torus", 0.*cm, edgeInRadius, outerRadiusGe-edgeRadius, 0.*deg, 360.*deg);
    
	
    G4UnionSolid*  		GeIn_uni1	= new G4UnionSolid("GeIn_uni1",GeCenterIn_tube,GeEndsIn_tube,0,G4ThreeVector(0.,0.,heightGe/2.-edgeRadius+edgeInRadius/2.-0.0001*cm));
    G4UnionSolid*  		GeIn_uni2	= new G4UnionSolid("GeIn_uni2",GeIn_uni1,GeEndsIn_tube,0,G4ThreeVector(0.,0.,-(heightGe/2.-edgeRadius+edgeInRadius/2.-0.0001*cm)));
    G4UnionSolid*  		GeIn_uni3	= new G4UnionSolid("GeIn_uni3",GeIn_uni2,GeEdgeIn_torus,0,G4ThreeVector(0.,0.,heightGe/2.-edgeRadius));
    G4UnionSolid*  		GeIn_uni4	= new G4UnionSolid("GeIn_uni4",GeIn_uni3,GeEdgeIn_torus,0,G4ThreeVector(0.,0.,-(heightGe/2.-edgeRadius)));
    G4UnionSolid*  		GeIn_uni5	= new G4UnionSolid("GeIn_uni5",GeIn_uni4,GeBottomIn_tube,0,G4ThreeVector(0.,0.,-(heightGe/2.-d_LiContact/2.)));
    */
    
    // outer electrode (Li contact)
    G4Tubs*			LiWindow_tube           = new G4Tubs        ("LiWindow_tube", 0.*cm, outerRadiusGe, d_LiContact/2., startAngle, spanningAngle);
    G4Tubs*			LiOuterContact_tube     = new G4Tubs        ("LiOuterContact_tube", outerRadiusGe-d_LiContact, outerRadiusGe, (heightGe-d_LiContact)/2.+0.0001*cm, startAngle, spanningAngle);

    G4UnionSolid*  		LiContact_uni	= new G4UnionSolid("LiContact_uni",LiWindow_tube,LiOuterContact_tube,0,G4ThreeVector(0.,0.,-d_LiContact/2.-(heightGe-d_LiContact)/2.+0.0001*cm));

    /*
    G4Tubs*			GeCenter_tube 	= new G4Tubs        ("GeCenter_tube", 0.*cm, outerRadiusGe, heightGe/2.-edgeRadius, startAngle, spanningAngle);
    G4Tubs*			GeEnds_tube     = new G4Tubs        ("GeEnds_tube", 0.*cm, outerRadiusGe-edgeRadius, edgeRadius/2.+0.0001*cm, startAngle, spanningAngle);
    G4Torus*        GeEdge_torus    = new G4Torus       ("GeEdge_torus", 0.*cm, edgeRadius, outerRadiusGe-edgeRadius, 0.*deg, 360.*deg);
    
    G4UnionSolid*  		Ge_uni1	= new G4UnionSolid("Ge_uni1",GeCenter_tube,GeEnds_tube,0,G4ThreeVector(0.,0.,heightGe/2.-edgeRadius/2.-0.0001*cm));
    G4UnionSolid*  		Ge_uni2	= new G4UnionSolid("Ge_uni2",Ge_uni1,GeEnds_tube,0,G4ThreeVector(0.,0.,-(heightGe/2.-edgeRadius/2.-0.0001*cm)));
    G4UnionSolid*  		Ge_uni3	= new G4UnionSolid("Ge_uni3",Ge_uni2,GeEdge_torus,0,G4ThreeVector(0.,0.,heightGe/2.-edgeRadius));
    G4UnionSolid*  		Ge_uni4	= new G4UnionSolid("Ge_uni4",Ge_uni3,GeEdge_torus,0,G4ThreeVector(0.,0.,-(heightGe/2.-edgeRadius)));
    
    // subtract inner part to get outer electrode
    G4SubtractionSolid*  	Ge_sub1		= new G4SubtractionSolid("Ge_sub1",Ge_uni4,GeIn_uni5,0,G4ThreeVector(0.,0.,0.));
     */

	
	
    //G4Sphere*		Hole_cap   	= new G4Sphere      ("Hole_cap", innerRadius, outerRadiusHole, startPhi, deltaPhi, startTheta, deltaTheta);
    
    //G4Tubs*			Groove_tube = new G4Tubs        ("Groove_tube", innerRadiusGroove, outerRadiusGroove, heightGroove/2.+overlap, startAngle, spanningAngle);
    
    
    //G4Tubs*			Wcontact_tube 	= new G4Tubs        ("Window_contact", 0.*cm, outerRadiusGe, d_LiContact/2., startAngle, spanningAngle);
    
    //G4Tubs*			Ocontact_tube 	= new G4Tubs        ("Outer_contact", outerRadiusGe-d_LiContact, outerRadiusGe, (heightGe-heightWContact)/2., startAngle, spanningAngle);
    
    // combine hole + cap
    //G4UnionSolid*  		Ge_hole	= new G4UnionSolid("Ge_hole",Hole_tube,Hole_cap,G4Transform3D(rm1,G4ThreeVector(0.,0.,heightHole/2.)));
    
	// define logical volumes
	G4LogicalVolume* 	Ge_log  = new G4LogicalVolume  (GeIn_sub,germanium_mat,"Ge_log",0,0,0);
		
	G4LogicalVolume* 	LiContact_log  = new G4LogicalVolume  (LiContact_uni,germanium_mat,"LiContact_log",0,0,0);
	
	// place volumes
	new G4PVPlacement    (0,G4ThreeVector(0.,0.,-d_LiContact/2.),Ge_log,"Ge_detector",expHall_log,false,0);
	new G4PVPlacement    (0,G4ThreeVector(0.,0.,(heightGe-d_LiContact)/2.),LiContact_log,"Li_contact",expHall_log,false,0);
	
	// set colors
	Ge_log->SetVisAttributes(grey);
	LiContact_log->SetVisAttributes(blue);
	
    
    // +++++++ vacuum inside Ge detector housing ++++++++++++++++++++++++++++++++++

    G4Tubs*	VacuumDet_tube              = new G4Tubs	("VacuumDet_tube", 0.*cm, innerRadiusCuHsg, (heightCuHsg-heightEndcap)/2., startAngle, spanningAngle);
    G4Tubs*	VacuumDetBottom_tube		= new G4Tubs	("VacuumDetBottom_tube", 0.*cm, innerRadiusCuHsg_bottom, (heightCuHsg_bottom-dCuHsg_bottom)/2., startAngle, spanningAngle);
    G4UnionSolid* VacuumDet_uni         = new G4UnionSolid("VacuumDet_uni",VacuumDet_tube,VacuumDetBottom_tube,0,G4ThreeVector(0.,0.,-((heightCuHsg-heightEndcap)/2.+(heightCuHsg_bottom-dCuHsg_bottom)/2.)));

    G4SubtractionSolid* VacuumDet_sub1		= new G4SubtractionSolid("VacuumDet_sub1",VacuumDet_uni,DetHolder_uni6,0,G4ThreeVector(0.,0.,zPosDetHolder-zPosVacuumDet));
    G4SubtractionSolid* VacuumDet_sub2		= new G4SubtractionSolid("VacuumDet_sub2",VacuumDet_sub1,TeflonHolder_uni,0,G4ThreeVector(0.,0.,zPosTeflonHolder-zPosVacuumDet));
    G4SubtractionSolid* VacuumDet_sub3		= new G4SubtractionSolid("VacuumDet_sub3",VacuumDet_sub2,LiContact_uni,0,G4ThreeVector(0.,0.,(heightGe-d_LiContact)/2.-zPosVacuumDet));
    G4SubtractionSolid* VacuumDet_sub4		= new G4SubtractionSolid("VacuumDet_sub4",VacuumDet_sub3,GeIn_sub,0,G4ThreeVector(0.,0.,-d_LiContact/2.-zPosVacuumDet));
    G4SubtractionSolid* VacuumDet_sub5		= new G4SubtractionSolid("VacuumDet_sub5",VacuumDet_sub4,Coldfinger_tube,G4Transform3D(rm2,G4ThreeVector(0.,yPosArm,zPosArm-zPosVacuumDet)));
    
    // define logical volumes
    G4LogicalVolume* 	VacuumDet_log  = new G4LogicalVolume  (VacuumDet_sub5,vacuum_mat,"VacuumDet_log",0,0,0);
    
    // place volumes
    new G4PVPlacement    (0,G4ThreeVector(0.,0.,zPosVacuumDet),VacuumDet_log,"VacuumDet",expHall_log,false,0);
    
    

	
	//======= define sensitive detector ======================================
	
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
    
	// Ge detector
    G4String SDname = "Ge_detector";
    HPGeSD* aSD = new HPGeSD(SDname);
    
    SDman->AddNewDetector(aSD);
    
	Ge_log->SetSensitiveDetector(aSD);
    
	
	return physiWorld;
}

