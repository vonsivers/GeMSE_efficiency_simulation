
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
		
	G4double GetEdep()				{ return edep; };

private:
	G4double		edep;
	
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
