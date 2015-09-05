
#ifndef HPGeHit_h
#define HPGeHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class HPGeHit : public G4VHit
{

public:

      HPGeHit();
     ~HPGeHit();
      HPGeHit(const HPGeHit&);
      const HPGeHit& operator=(const HPGeHit&);
      G4int operator==(const HPGeHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

public:

	void SetEdep(G4double de)				{ edep = de; };
	void SetPos(G4ThreeVector xyz)			{ pos = xyz; };
	void SetParticle(G4int ID)				{ particleID = ID; };
	void SetTrack(G4int tID)				{ trackID = tID; };
	void SetParticleEnergy(G4double e)		{ particleEnergy = e; };
	void SetTime(G4double t)				{ time = t; };
	
	G4double GetEdep()				{ return edep; };
	G4ThreeVector GetPos()			{ return pos; };
	G4int GetParticle()             { return particleID;};
	G4int GetTrack()                { return trackID;};
	G4double GetParticleEnergy()    { return particleEnergy;};
	G4double GetTime()              { return time; };      
	
  
private:
	G4double		edep;
	G4ThreeVector	pos;
	G4double		time;
	G4int			particleID;
	G4int			trackID;
	G4double		particleEnergy;
	
};


typedef G4THitsCollection <HPGeHit> HPGeHitsCollection;

extern G4Allocator<HPGeHit> HPGeHitAllocator;


inline void* HPGeHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HPGeHitAllocator.MallocSingle();
  return aHit;
}


inline void HPGeHit::operator delete(void *aHit)
{
  HPGeHitAllocator.FreeSingle((HPGeHit*) aHit);
}


#endif
