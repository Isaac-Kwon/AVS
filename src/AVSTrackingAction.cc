///
/// \file AVSTrackingAction.cc
/// \brief Implementation of the AVSTrackingAction class
///

#include "AVSTrackingAction.hh"
#include "AVSAnalysisManager.hh"

#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AVSTrackingAction::AVSTrackingAction()
:G4UserTrackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AVSTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  AVSAnalysisManager* analysisManager = AVSAnalysisManager::Instance();
  analysisManager->FillSource(track);
}

void AVSTrackingAction::PostUserTrackingAction(const G4Track* track){
  AVSAnalysisManager* analysisManager = AVSAnalysisManager::Instance();
  analysisManager->EndOfTrack(track);
  analysisManager->AutoSave();
}