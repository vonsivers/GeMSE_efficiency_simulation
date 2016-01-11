
#ifndef MuonHit_h
#define MuonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class MuonHit : public G4VHit
{

public:

      MuonHit();
     ~MuonHit();
      MuonHit(const MuonHit&);
      const MuonHit& operator=(const MuonHit&);
      G4int operator==(const MuonHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

public:

	void SetEdep(G4double de)				{ edep = de; };
	void SetPos(G4ThreeVector xyz)			{ pos = xyz; };
	void SetParticle(G4int ID)				{ particleID = ID; };
	void SetTrack(G4int tID)				{ trackID = tID; };
	void SetPanelNr(G4int tNr)				{ panelNr = tNr; };
	void SetParticleEnergy(G4double e)		{ particleEnergy = e; };
	void SetTime(G4double t)				{ time = t; };
	
	G4double GetEdep()				{ return edep; };
	G4ThreeVector GetPos()			{ return pos; };
	G4int GetParticle()             { return particleID;};
	G4int GetTrack()                { return trackID;};
	G4int GetPanelNr()              { return panelNr;};
	G4double GetParticleEnergy()    { return particleEnergy;};
	G4double GetTime()              { return time; };      
	
  
private:
	G4double		edep;
	G4ThreeVector	pos;
	G4double		time;
	G4int			particleID;
	G4int			trackID;
	G4int			panelNr;
	G4double		particleEnergy;
	
};


typedef G4THitsCollection <MuonHit> MuonHitsCollection;

extern G4Allocator<MuonHit> MuonHitAllocator;


inline void* MuonHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) MuonHitAllocator.MallocSingle();
  return aHit;
}


inline void MuonHit::operator delete(void *aHit)
{
  MuonHitAllocator.FreeSingle((MuonHit*) aHit);
}


#endif
