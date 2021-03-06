///
/// \file AVSPrimaryGeneratorAction.cc
/// \brief Implementation of the AVSPrimaryGeneratorAction class
///

#include "AVSPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSPrimaryGeneratorAction::AVSPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0) 
  // fEnvelopeBox(0)
{
  // G4int n_particle = 1;
  // fParticleGun  = new G4ParticleGun(n_particle);

  // // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  // G4ParticleDefinition* particle
  //   = particleTable->FindParticle(particleName="alpha");
  // fParticleGun->SetParticleDefinition(particle);
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
  // fParticleGun->SetParticleEnergy(6.*MeV);


  fParticleGun = new G4GeneralParticleSource();
  // fParticleGun->SetMomentumDirection(G4ThreeVector(0., -1., 0));
  fParticleGun->SetParticlePosition(G4ThreeVector(0., +14*mm, 0.));
  fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="alpha"));
  // fParticleGun->SetKineticEnergy(6.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSPrimaryGeneratorAction::~AVSPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  // G4double envSizeX = 0;
  // G4double envSizeY = 0;
  // G4double envSizeZ = 0;

  // if (!fEnvelopeBox)
  // {
  //   G4LogicalVolume* envLV
  //     = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  //   if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  // }

  // if ( fEnvelopeBox ) {
  //   envSizeX = fEnvelopeBox->GetXHalfLength()*2.;
  //   envSizeY = fEnvelopeBox->GetYHalfLength()*2.;
  //   envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  // }  
  // else  {
  //   G4ExceptionDescription msg;
  //   msg << "Envelope volume of box shape not found.\n"; 
  //   msg << "Perhaps you have changed geometry.\n";
  //   msg << "The gun will be place at the center.";
  //   G4Exception("AVSPrimaryGeneratorAction::GeneratePrimaries()",
  //    "MyCode0002",JustWarning,msg);
  // }

  // G4double size = 0.8; 
  // G4double x0 = size * envSizeX * (G4UniformRand()-0.5);
  // G4double y0 = +0.5 * envSizeY;
  // G4double z0 = size * envSizeZ * (G4UniformRand()-0.5);

  // G4double x0 = 6 * (G4UniformRand()-0.5);
  // G4double y0 = 24.0*mm;
  // G4double z0 = 6 * (G4UniformRand()-0.5);


  // fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  // fParticleGun->GeneratePrimaryVertex(anEvent);

  fParticleGun -> GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

