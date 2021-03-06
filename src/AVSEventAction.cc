///
/// \file AVSEventAction.cc
/// \brief Implementation of the AVSEventAction class
///

#include "AVSEventAction.hh"
#include "AVSRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSEventAction::AVSEventAction(AVSRunAction* runAction)
: G4UserEventAction()
  // fRunAction(runAction)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSEventAction::~AVSEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSEventAction::BeginOfEventAction(const G4Event*)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSEventAction::EndOfEventAction(const G4Event*)
{
  //endofevent   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
