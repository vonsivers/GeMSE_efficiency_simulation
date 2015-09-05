
#include "HPGeHit.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

G4Allocator <HPGeHit> HPGeHitAllocator;

HPGeHit::HPGeHit()
{;}


HPGeHit::~HPGeHit()
{;}


HPGeHit::HPGeHit(const HPGeHit& right)
  : G4VHit()
{
	edep			= right.edep;
	pos				= right.pos;
	particleID		= right.particleID;
	trackID			= right.trackID;
	particleEnergy	= right.particleEnergy;
	time			= right.time;
	
 }


const HPGeHit& HPGeHit::operator=(const HPGeHit& right)
{
	edep			= right.edep;
	pos				= right.pos;
	particleID		= right.particleID;
	trackID			= right.trackID;
	particleEnergy  = right.particleEnergy;
	time			= right.time;

  return *this;
}


G4int HPGeHit::operator==(const HPGeHit& right) const
{
  return (this==&right) ? 1 : 0;
}


void HPGeHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
	  /*
    G4Circle circle(pos);
    circle.SetScreenSize(5);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,.5);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
	   */
  }
}


void HPGeHit::Print()
{
	/*
  G4cout << "  energy deposit: " << G4BestUnit(edep,"Energy")
         << "  time:" <<G4BestUnit(hit_time,"Time")
	 */
}
