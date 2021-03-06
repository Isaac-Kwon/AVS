///
/// \file AVSRunAction.hh
/// \brief Definition of the AVSRunAction class
///

#ifndef AVSRunAction_h
#define AVSRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class AVSAnalysisManager;

/// Run action class
///
///

class AVSRunAction : public G4UserRunAction
{
  public:
    AVSRunAction();
    virtual ~AVSRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    AVSAnalysisManager* fAnalysisManager;

  private:

};

#endif

