///
/// \file AVSTrackingAction.hh
/// \brief Definition of the AVSTrackingAction class
///

#ifndef AVSTrackingAction_H
#define AVSTrackingAction_H 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AVSTrackingAction : public G4UserTrackingAction
{
  public:
    AVSTrackingAction() ;
   ~AVSTrackingAction() { };
    
    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void  PostUserTrackingAction(const G4Track*);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif