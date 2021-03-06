///
/// \file AVSActionInitialization.cc
/// \brief Implementation of the AVSActionInitialization class
///

#include "AVSActionInitialization.hh"
#include "AVSPrimaryGeneratorAction.hh"
#include "AVSRunAction.hh"
#include "AVSEventAction.hh"
#include "AVSSteppingAction.hh"
#include "AVSTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSActionInitialization::AVSActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AVSActionInitialization::~AVSActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSActionInitialization::BuildForMaster() const
{
  G4cout<<"AVSActionInitialization::BuildForMaster()"<<G4endl;
  AVSRunAction* runAction = new AVSRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSActionInitialization::Build() const
{
  SetUserAction(new AVSPrimaryGeneratorAction);

  AVSRunAction* runAction = new AVSRunAction;
  SetUserAction(runAction);

  AVSEventAction* eventAction = new AVSEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new AVSSteppingAction(eventAction));
  SetUserAction(new AVSTrackingAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
