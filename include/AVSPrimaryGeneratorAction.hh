///
/// \file AVSPrimaryGeneratorAction.hh
/// \brief Definition of the AVSPrimaryGeneratorAction class
///

#ifndef AVSPrimaryGeneratorAction_h
#define AVSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class AVSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    AVSPrimaryGeneratorAction();    
    virtual ~AVSPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4GeneralParticleSource* GetParticleGun() const { return fParticleGun; }
  
  private:
    // G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4GeneralParticleSource*  fParticleGun; // pointer a to G4 gun class
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
