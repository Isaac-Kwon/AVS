///
/// \file AVSRunAction.cc
/// \brief Implementation of the AVSRunAction class
///

#include "AVSRunAction.hh"
#include "AVSPrimaryGeneratorAction.hh"
#include "AVSDetectorConstruction.hh"
#include "AVSAnalysisManager.hh"

// #include "AVSRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSRunAction::AVSRunAction()
: G4UserRunAction()
{ 

  fAnalysisManager = AVSAnalysisManager::Instance();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSRunAction::~AVSRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSRunAction::BeginOfRunAction(const G4Run*)
{ 
  fAnalysisManager->Book();
  
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const AVSPrimaryGeneratorAction* generatorAction
   = static_cast<const AVSPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4GeneralParticleSource* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  fAnalysisManager->Write();
  
  fAnalysisManager->GetTreeSource()->Print();
  fAnalysisManager->GetTreeStepper()->Print();
  fAnalysisManager->GetTreeTrack()->Print();

  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << "------------------------------------------------------------"
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
