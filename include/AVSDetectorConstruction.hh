///
/// \file AVSDetectorConstruction.hh
/// \brief Definition of the AVSDetectorConstruction class
///

#ifndef AVSDetectorConstruction_h
#define AVSDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "map"
#include "string"
#include "vector"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class AVSStructure;
class G4GenericMessenger;
class G4AssemblyVolume;

/// Detector construction class to define materials and geometry.

class AVSDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    AVSDetectorConstruction();
    virtual ~AVSDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructMaterials();
    
    // G4LogicalVolume* GetScoringVolume() const { return fScoringVolume1; }
    G4LogicalVolume* GetScoringStandAlpha()   const { return UpperStandAlphaLogical; }
    G4LogicalVolume* GetScoringStandGamma()   const { return UpperStandGammaLogical; }
    G4LogicalVolume* GetScoringALPIDE_Upper() const { return fScoringALPIDEUpper; }
    G4LogicalVolume* GetScoringALPIDE_Under() const { return fScoringALPIDEUnder; }
    G4LogicalVolume* GetScoringPCBBoard() const { return CarrierBoardLogical; }
    
    void SetDistanceUpperStand(G4double val);
    void SetParticlePreset(G4int preset);
    void SetStand(G4int StandNo);
    void SetAlpha(G4double Energy);
    void SetGamma(G4double Energy);

    static G4LogicalVolume* FindLogicalVolume(const G4String name);
    
  protected:
    AVSStructure * fStructure;
    // G4LogicalVolume* fScoringVolume1;
    // G4LogicalVolume* fScoringVolume2;
    // G4LogicalVolume* fScoringLayer;

    G4LogicalVolume* fScoringALPIDEUpper;
    G4LogicalVolume* fScoringALPIDEUnder;

    G4LogicalVolume* CarrierBoardLogical;
    G4LogicalVolume* ALPIDELogical;
    G4LogicalVolume* ALPIDESliceLogical;
    G4LogicalVolume* UpperStandAlphaLogical;
    G4LogicalVolume* UpperStandGammaLogical;
    G4LogicalVolume* Detector;
    G4LogicalVolume* Stand;
    G4LogicalVolume* SideStand;
    G4LogicalVolume* SideStandStand;
    G4LogicalVolume* SubstrateU;
    G4LogicalVolume* SubstrateD;

    G4AssemblyVolume* ALPIDEAssembly;

    G4VPhysicalVolume* fUpperStandAlpha;
    G4VPhysicalVolume* fUpperStandGamma;
    G4double fStandDistance;

    G4int fStandState = 0; // 0: alpha, 1: gamma

    void DefineCommands();

    private:
    G4GenericMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

