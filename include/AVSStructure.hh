///
/// \file AVSStructure.hh
/// \brief Definition of the AVSStructure class
///

#ifndef AVSGenerateStructures_h
#define AVSGenerateStructures_h 1

#include "globals.hh"

class G4Element;
class G4LogicalVolume;
class G4Material;
class G4NistManager;
class G4AssemblyVolume;
class G4UserLimits;

class AVSStructure{
    private:
    G4Element * elH;
    G4Element * elC;
    G4Element * elO;
    G4Element * elNa;
    G4Element * elMg;
    G4Element * elAl;
    G4Element * elSi;
    G4Element * elCa;
    G4Element * elTi;
    G4Element * elFe;
    G4Element * elBr;

    G4NistManager* manager;
    G4Material* fMatPCB;
    G4Material* fMatSi;
    G4Material* fMatPLA10;
    G4Material* fMatPLA03;
    G4Material* fMatPLA02;
    G4Material* fMatPLA01;

    protected:
    public:

    G4VisAttributes* white;
    G4VisAttributes* trans_white;
    G4VisAttributes* blue;
    G4VisAttributes* gray;
    G4VisAttributes* lightgray;
    G4VisAttributes* red;
    G4VisAttributes* yellow;
    G4VisAttributes* green;
    G4VisAttributes* darkGreen;
    G4VisAttributes* darkOrange3;
    G4VisAttributes* skyBlue;

    G4VisAttributes* world_color;
    G4VisAttributes* alpide_color;
    G4VisAttributes* pcb_color;
    G4VisAttributes* pla_color;

    
    AVSStructure();
    void ConstructMaterial();
    void ConstructColors();
    
    G4Material* Construct_M_PCB();
    G4Material* Construct_M_Si();
    // G4Material* Construct_M_PLA(G4Double fraction=1.0);

    G4Material* M_PCB(){return fMatPCB;}
    G4Material* M_Si(){return fMatSi;}
    G4Material* M_PLA(G4double fraction=1.0);
    G4AssemblyVolume* AV_ALPIDE();
    G4LogicalVolume* LV_ALPIDECarrierBoard();
    G4LogicalVolume* LV_ALPIDE();
    G4LogicalVolume* LV_Detector();
    G4LogicalVolume* LV_Stand();
    G4LogicalVolume* LV_SideStand();
    G4LogicalVolume* LV_SideStandStand();
    G4LogicalVolume* LV_UpperStandAlpha();
    G4LogicalVolume* LV_UpperStandGamma();
    G4LogicalVolume* LV_SubstrateU();
    G4LogicalVolume* LV_SubstrateD();

    G4LogicalVolume* Replica_ALPIDE(G4LogicalVolume * ALPIDE_Logical);

    G4UserLimits* ALPIDEStepLimit;
};

#endif