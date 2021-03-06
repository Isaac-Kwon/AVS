///
/// \file AVSEventAction.hh
/// \brief Definition of the AVSEventAction class
///

#ifndef AVSEventAction_h
#define AVSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class AVSRunAction;

/// Event action class
///

class AVSEventAction : public G4UserEventAction
{
  public:
    AVSEventAction(AVSRunAction* runAction);
    virtual ~AVSEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
    // AVSRunAction* fRunAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
