///
/// \file AVSSteppingAction.cc
/// \brief Implementation of the AVSSteppingAction class
///

#include "AVSSteppingAction.hh"
#include "AVSEventAction.hh"
#include "AVSDetectorConstruction.hh"
#include "AVSAnalysisManager.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSSteppingAction::AVSSteppingAction(AVSEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSSteppingAction::~AVSSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSSteppingAction::UserSteppingAction(const G4Step* step)
{

  AVSAnalysisManager* analysisManager = AVSAnalysisManager::Instance();

  // If the physical volume is not existed, saving is passed.
  if(! (step->GetPreStepPoint()->GetPhysicalVolume()) ) return;
  if(! (step->GetPostStepPoint()->GetPhysicalVolume()) ) return;

  // { // If in dictionary of ScoringVolume doesn't have the scoring volumes, input the volumes in dictionary
  //   const AVSDetectorConstruction* detectorConstruction
  //       = static_cast<const AVSDetectorConstruction*>
  //         (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //   if (!fScoringVolume) { 
  //     fScoringVolume = detectorConstruction->GetScoringVolume();
  //     // fScoringVolumes[fScoringVolume] = 1;
  //   }

  //   if (!fScoringVolume1) { 
  //     fScoringVolume1 = detectorConstruction->GetScoringVolume1();   
  //     fScoringVolumes[fScoringVolume1] = 2;
  //   }

  //   if (!fScoringVolume2) { 
  //     fScoringVolume2 = detectorConstruction->GetScoringVolume2();   
  //     fScoringVolumes[fScoringVolume2] = 3;
  //   }
  // }

  // if (!fScoringLayer) { 
  //   const AVSDetectorConstruction* detectorConstruction
  //     = static_cast<const AVSDetectorConstruction*>
  //       (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //   fScoringLayer = detectorConstruction->GetScoringLayer();   
  // }

  // get volume of the current step

      
  // check if we are in scoring volume
  // if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  // G4double edepStep = step->GetTotalEnergyDeposit();
  // fEventAction->AddEdep(edepStep);  

  // G4Track* track         = step->GetTrack();

  // if(track->GetParentID() != 0 && analysisManager->GetRecordStepPrimaryOnly()) return;

  // G4double kinEnergy     = track->GetKineticEnergy();
  // G4int pid              = track->GetDynamicParticle()->GetPDGcode();
  // G4ThreeVector direction = track->GetMomentumDirection();  
  // G4double theta = direction.theta(), phi = direction.phi();

  // G4ThreeVector preStep = step->GetPreStepPoint()->GetPosition();
  // G4double preStepX = preStep.x()/CLHEP::mm, preStepY = preStep.y()/CLHEP::mm, preStepZ = preStep.z()/CLHEP::mm;
  // G4ThreeVector postStep = step->GetPostStepPoint()->GetPosition();
  // G4double postStepX = postStep.x()/CLHEP::mm, postStepY = postStep.y()/CLHEP::mm, postStepZ = postStep.z()/CLHEP::mm;

  // G4double kinEnergy_preStep = step->GetPreStepPoint()->GetKineticEnergy();
  // G4double kinEnergy_postStep = step->GetPostStepPoint()->GetKineticEnergy();

  // G4LogicalVolume*  preVolume = step->GetPreStepPoint()->GetTouchableHandle()
  //     ->GetVolume()->GetLogicalVolume();
  // G4LogicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()
  //     ->GetVolume()->GetLogicalVolume();

  // G4int  preVolumeN;
  // G4int postVolumeN;

  // preVolumeN = fScoringVolumes[preVolume];
  // postVolumeN = fScoringVolumes[postVolume];
  // // if (preVolume == fScoringVolume1 ){
  // //   preVolumeN = 1;
  // // }else if (preVolume == fScoringVolume2 ){
  // //   preVolumeN = 2;
  // // }else if (preVolume == fScoringLayer ){
  // //   preVolumeN = 3;
  // // }else{
  // //   preVolumeN = 0;
  // // }

  // // if (postVolume == fScoringVolume1 ){
  // //   postVolumeN = 1;
  // // }
  // // else if (postVolume == fScoringVolume2 ){
  // //   postVolumeN = 2;
  // // }else if (postVolume == fScoringLayer ){
  // //   postVolumeN = 3;
  // // }else{
  // //   postVolumeN = 0;
  // // }

  // G4double TID = step->GetTotalEnergyDeposit();
  // G4double NIEL = step->GetNonIonizingEnergyDeposit();

  // StepperEntry entry;

  // entry.pid = pid;
  // entry.Ekin = kinEnergy_preStep;
  // entry.preVolume = preVolumeN;
  // entry.postVolume = postVolumeN;
  // entry.preposX = preStepX;
  // entry.preposY = preStepY;
  // entry.preposZ = preStepZ;
  // entry.postposX = postStepX;
  // entry.postposY = postStepY;
  // entry.postposZ = postStepZ;
  // entry.dirTheta = theta;
  // entry.dirPhi = phi;
  // entry.TID = TID;
  // entry.NIEL = NIEL;

  // analysisManager->FillEntryStepper(step);
  // G4cout<<"Filling Stepper"<<G4endl;
  analysisManager->FillStepper(step);
  // G4cout<<"Filling Track"<<G4endl;
  analysisManager->FillTrack(step);
  // G4cout<<"End of SteppingAction"<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

