///
/// \file AVSSteppingAction.hh
/// \brief Definition of the AVSSteppingAction class
///

#ifndef AVSSteppingAction_h
#define AVSSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class AVSEventAction;
class G4LogicalVolume;

/// Stepping action class
/// 

class AVSSteppingAction : public G4UserSteppingAction
{
  public:
    AVSSteppingAction(AVSEventAction* eventAction);
    virtual ~AVSSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);
    void MakeDictionaryScoringVolume();

  private:
    AVSEventAction*  fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
