///
/// \file AVSActionInitialization.hh
/// \brief Definition of the AVSActionInitialization class
///

#ifndef AVSActionInitialization_h
#define AVSActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class AVSActionInitialization : public G4VUserActionInitialization
{
  public:
    AVSActionInitialization();
    virtual ~AVSActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
