///
/// \file AVSStructure.cc
/// \brief Implementation of the AVSStructure class
///

#ifndef AVSStructure_h
#define AVSStructure_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4LogicalVolume.hh"
#include "AVSStructure.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4AssemblyVolume.hh"
#include "G4UserLimits.hh"

#include "G4PVReplica.hh"


AVSStructure::AVSStructure(){
    manager = G4NistManager::Instance();
    ConstructMaterial();
    ConstructColors();
}

void AVSStructure::ConstructMaterial(){
    elH     = new G4Element("Hydrogen" , "H" , 1 , 1.008  * g/mole);
    elC     = new G4Element("Carbon"   , "C" , 6 , 12.011 * g/mole);
    elO     = new G4Element("Oxygen"   , "O" , 8 , 15.999 * g/mole);
    elNa    = new G4Element("Natrium"  , "Na", 11, 22.990 * g/mole);
    elMg    = new G4Element("Magnesium", "Mg", 12, 24.305 * g/mole);
    elAl    = new G4Element("Aluminium", "Al", 13, 26.982 * g/mole);
    elSi    = new G4Element("Silicon"  , "Si", 14, 28.085 * g/mole);
    elCa    = new G4Element("Calcium"  , "Ca", 20, 40.078 * g/mole);
    elTi    = new G4Element("Titanium" , "Ti", 22, 47.867 * g/mole);
    elFe    = new G4Element("Iron"     , "Fe", 26, 55.845 * g/mole);
    elBr    = new G4Element("Bromine"  , "Br", 35, 79.904 * g/mole);
    fMatPCB   = Construct_M_PCB();
    fMatSi    = Construct_M_Si();
    fMatPLA10 = M_PLA(1.0);
    fMatPLA03 = M_PLA(0.3);
    fMatPLA02 = M_PLA(0.2);
    fMatPLA01 = M_PLA(0.1);
}

G4Material * AVSStructure::Construct_M_PCB(){

    G4Material* FR4          = new G4Material("FR4", 1.98281*g/cm3, 2);

    G4Material* EpoxyResin   = new G4Material("EpoxyResin", 1.1250*g/cm3,    4);
    G4Material* FibrousGlass = new G4Material("FibrousGlass", 2.74351*g/cm3, 7);

    G4Material* SiO2         = new G4Material("SiO2"  , 2.20*g/cm3 , 2);
    SiO2->AddElement(elSi, 1);
    SiO2->AddElement(elO , 2);
    G4Material* Al2O3        = new G4Material("Al2O3" , 3.97*g/cm3 , 2);
    Al2O3->AddElement(elAl, 2);
    Al2O3->AddElement(elO,  3);
    G4Material* Fe2O3        = new G4Material("Fe2O3" , 5.24*g/cm3 , 2);
    Fe2O3->AddElement(elFe, 2);
    Fe2O3->AddElement(elO,  3);
    G4Material* CaO          = new G4Material("CaO"   , 3.35*g/cm3 , 2);
    CaO->AddElement(elCa, 1);
    CaO->AddElement(elO,  1);
    G4Material* MgO          = new G4Material("MgO"   , 3.58*g/cm3 , 2);
    MgO->AddElement(elMg, 1);
    MgO->AddElement(elO,  1);
    G4Material* Na2O         = new G4Material("Na2O"  , 2.27*g/cm3 , 2);
    Na2O->AddElement(elNa, 2);
    Na2O->AddElement(elO,  1);
    G4Material* TiO2         = new G4Material("TiO2"  , 4.23*g/cm3 , 2);
    TiO2->AddElement(elTi, 1);
    TiO2->AddElement(elO,  2);

    EpoxyResin->AddElement(elC, 38);
    EpoxyResin->AddElement(elH, 40);
    EpoxyResin->AddElement(elO, 6);
    EpoxyResin->AddElement(elBr, 4);

    FibrousGlass->AddMaterial(SiO2  , 60.0*perCent);
    FibrousGlass->AddMaterial(Al2O3 , 11.8*perCent);
    FibrousGlass->AddMaterial(Fe2O3 ,  0.1*perCent);
    FibrousGlass->AddMaterial(CaO   , 22.4*perCent);
    FibrousGlass->AddMaterial(MgO   ,  3.4*perCent);
    FibrousGlass->AddMaterial(Na2O  ,  1.0*perCent);
    FibrousGlass->AddMaterial(TiO2  ,  1.3*perCent);

    FR4->AddMaterial(EpoxyResin  , 47*perCent);
    FR4->AddMaterial(FibrousGlass, 53*perCent);

    return FR4;
}

G4Material* AVSStructure::Construct_M_Si(){
    G4Material * Si = new G4Material("Silicon", 14, 28.085 * g/mole, 2330*kg/m3);
    return Si;
}

G4Material* AVSStructure::M_PLA(G4double fraction){
    G4Material * PLA = new G4Material(G4String("PLA")+G4String(G4int(fraction*100)), 1.210 * fraction * g/cm3, 3);
    PLA->AddElement(elC, 3);
    PLA->AddElement(elH, 4);
    PLA->AddElement(elO, 2);
    return PLA;
}

G4LogicalVolume* AVSStructure::LV_ALPIDECarrierBoard(){
    G4Box* ALPIDEPartSolid  = new G4Box("ALPIDECarrierBoardUp_Solid"   , 70.*mm*0.5, 1.0*mm*0.5, 70.*mm*0.5 );
    G4Box* ALPIDEInnerSolid = new G4Box("ALPIDECarrierBoardInner_Solid", 31.*mm*0.5, 3*mm,      12.8*mm*0.5 );
    G4SubtractionSolid * ALPIDEBoardSolid = new G4SubtractionSolid("ALPIDECarrierBoard_Solid", ALPIDEPartSolid, ALPIDEInnerSolid, G4Translate3D(0.,0.,2.7*mm));
    G4LogicalVolume * ALPIDECarrierBoardLogical = new G4LogicalVolume(ALPIDEBoardSolid, M_PCB(), "ALPIDECarrierBoard_Logical");
    ALPIDECarrierBoardLogical->SetVisAttributes(pcb_color);
    return ALPIDECarrierBoardLogical;
}

G4LogicalVolume* AVSStructure::LV_ALPIDE(){
    G4Box * ALPIDESolid = new G4Box("ALPIDE_Solid", 30.0*mm*0.5, 100.0*um*0.5, 15.0*mm*0.5);
    G4LogicalVolume * ALPIDELogical = new G4LogicalVolume(ALPIDESolid, M_Si(), "ALPIDE_Logical");
    ALPIDELogical->SetVisAttributes(alpide_color);

    return ALPIDELogical;
}

G4AssemblyVolume* AVSStructure::AV_ALPIDE(){
    G4Box * ALPIDEUpperSolid = new G4Box("ALPIDEUpper_Solid", 30.0*mm*0.5, (11.0)*um*0.5, 15.0*mm*0.5);
    G4Box * ALPIDEUnderSolid = new G4Box("ALPIDEUnder_Solid", 30.0*mm*0.5, (100.0 - 11.0)*um*0.5, 15.0*mm*0.5);
    G4LogicalVolume * ALPIDEUpperLogical = new G4LogicalVolume(ALPIDEUpperSolid, M_Si(), "ALPIDEUpper_Logical");
    G4LogicalVolume * ALPIDEUnderLogical = new G4LogicalVolume(ALPIDEUnderSolid, M_Si(), "ALPIDEUnder_Logical");
    ALPIDEUpperLogical->SetVisAttributes(alpide_color);
    ALPIDEUnderLogical->SetVisAttributes(alpide_color);

    ALPIDEStepLimit = new G4UserLimits(1*um, 1*um);

    ALPIDEUpperLogical->SetUserLimits(ALPIDEStepLimit);
    ALPIDEUnderLogical->SetUserLimits(ALPIDEStepLimit);

    G4AssemblyVolume* ALPIDEAssembly = new G4AssemblyVolume();

    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 0.0*deg, 0.0*deg);
    G4ThreeVector uppervector = G4ThreeVector(0,  (50.0 - 11.0*0.5)*um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDEUpperLogical,uppervector, Ra);

    G4ThreeVector undervector = G4ThreeVector(0, -(11.0)*0.5*um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDEUnderLogical,undervector, Ra);

    return ALPIDEAssembly;
}

G4LogicalVolume* AVSStructure::Replica_ALPIDE(G4LogicalVolume * ALPIDE_Logical){
    G4Box * ALPIDESliceSolid = new G4Box("ALPIDESlice_Solid", 30.0*mm*0.5, 100.0*um*0.5/(100.), 15.0*mm*0.5);
    G4LogicalVolume * ALPIDESlice_Logical = new G4LogicalVolume(ALPIDESliceSolid, M_Si(), "ALPIDESlice_Logical");
    G4VPhysicalVolume* ALPIDESlice_Physical = new G4PVReplica("ALPIDESlice", ALPIDESlice_Logical, ALPIDE_Logical, kYAxis, 100, 1.0*um, 0);

    return ALPIDESlice_Logical;
}

// G4AssemblyVolume* AVSStructure::AV_Source(){
//     return nullptr;
// }

G4LogicalVolume* AVSStructure::LV_Detector(){
    return nullptr;

}

G4LogicalVolume* AVSStructure::LV_Stand(){
    return nullptr;

}

G4LogicalVolume* AVSStructure::LV_SideStand(){
    return nullptr;


}

G4LogicalVolume* AVSStructure::LV_SideStandStand(){
    return nullptr;
    
}

G4LogicalVolume* AVSStructure::LV_UpperStandAlpha(){
    G4Box * UpperStandBoxSolid = new G4Box("UpperStandBoxAlpha_Solid",
                        180.0*mm*0.5, 8.00*mm*0.5, 28.50*mm*0.5);
    G4Tubs * UpperStandHoleFirstSolid = new G4Tubs("UpperStandHoleAlphaFirst_Solid",0., 6.5*mm, 3.0*mm, 0., 360.0*deg);
    G4Tubs * UpperStandHoleSecondSolid = new G4Tubs("UpperStandHoleAlphaSecond_Solid",0., 3.0*mm, 8.0*mm, 0., 360.0*deg);

    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 90.0*deg, 90.*deg);
    G4ThreeVector Ta1 = G4ThreeVector(0.0 *mm, 4.0 *mm, 0.0 *m);
    G4SubtractionSolid * UpperStandHoleSubFirstSolid = new G4SubtractionSolid("UpperStandHoleAlphaSubFirst_Solid", UpperStandBoxSolid, UpperStandHoleFirstSolid, Ra, Ta1);
    G4SubtractionSolid * UpperStandHoleSubSecondSolid = new G4SubtractionSolid("UpperStandHoleAlphaSubSecond_Solid", UpperStandHoleSubFirstSolid, UpperStandHoleSecondSolid, Ra, Ta1);

    G4LogicalVolume * UpperStandLogical = new G4LogicalVolume(UpperStandHoleSubSecondSolid, fMatPLA03, "UpperStandAlpha_Logical");
    UpperStandLogical->SetVisAttributes(pla_color);
    return UpperStandLogical;
    // return new G4LogicalVolume(UpperStandHoleSubFirstSolid, fMatPLA03, "UpperStand_Logical");
}

G4LogicalVolume* AVSStructure::LV_UpperStandGamma(){
    G4Box * UpperStandBoxSolid = new G4Box("UpperStandBoxGamma_Solid",
                        180.0*mm*0.5, 8.00*mm*0.5, 28.50*mm*0.5);
    G4Tubs * UpperStandHoleFirstSolid = new G4Tubs("UpperStandHoleGammaFirst_Solid",0., 7.75*mm, 3.0*mm, 0., 360.0*deg);
    G4Tubs * UpperStandHoleSecondSolid = new G4Tubs("UpperStandHoleGammaSecond_Solid",0., 6.5*mm, 8.0*mm, 0., 360.0*deg);

    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 90.0*deg, 90.*deg);
    G4ThreeVector Ta1 = G4ThreeVector(0.0 *mm, 4.0 *mm, 0.0 *m);
    G4SubtractionSolid * UpperStandHoleSubFirstSolid = new G4SubtractionSolid("UpperStandHoleGammaSubFirst_Solid", UpperStandBoxSolid, UpperStandHoleFirstSolid, Ra, Ta1);
    G4SubtractionSolid * UpperStandHoleSubSecondSolid = new G4SubtractionSolid("UpperStandHoleGammaSubSecond_Solid", UpperStandHoleSubFirstSolid, UpperStandHoleSecondSolid, Ra, Ta1);

    G4LogicalVolume * UpperStandLogical = new G4LogicalVolume(UpperStandHoleSubSecondSolid, fMatPLA03, "UpperStandGamma_Logical");
    UpperStandLogical->SetVisAttributes(pla_color);
    return UpperStandLogical;
    // return new G4LogicalVolume(UpperStandHoleSubFirstSolid, fMatPLA03, "UpperStand_Logical");
}

G4LogicalVolume* AVSStructure::LV_SubstrateU(){
    return nullptr;


}

G4LogicalVolume* AVSStructure::LV_SubstrateD(){
    return nullptr;


}

void AVSStructure::ConstructColors(){
  white = new G4VisAttributes( G4Colour());
  white -> SetVisibility(true);
  white -> SetForceSolid(true);

  trans_white = new G4VisAttributes( G4Colour());
  trans_white -> SetVisibility(false);
  trans_white -> SetForceSolid(false);

  blue = new G4VisAttributes(G4Colour(0. ,0. ,1.));
  blue -> SetVisibility(true);
  blue -> SetForceSolid(true);

  gray = new G4VisAttributes( G4Colour(0.5, 0.5, 0.5 ));
  gray-> SetVisibility(true);
  gray-> SetForceSolid(true);

  lightgray = new G4VisAttributes( G4Colour(0.75, 0.75, 0.75 ));
  lightgray-> SetVisibility(true);
  lightgray-> SetForceSolid(true);

  red = new G4VisAttributes(G4Colour(1. ,0. ,0.));
  red-> SetVisibility(true);
  red-> SetForceSolid(true);

  yellow = new G4VisAttributes(G4Colour(1., 1., 0. ));
  yellow-> SetVisibility(true);
  yellow-> SetForceSolid(true);

  green = new G4VisAttributes( G4Colour(25/255. , 255/255. ,  25/255. ));
  green -> SetVisibility(true);
  green -> SetForceSolid(true);

  darkGreen = new G4VisAttributes( G4Colour(0/255. , 100/255. ,  0/255. ));
  darkGreen -> SetVisibility(true);
  darkGreen -> SetForceSolid(true);

  darkOrange3 = new G4VisAttributes( G4Colour(205/255. , 102/255. ,  000/255. ));
  darkOrange3 -> SetVisibility(true);
  darkOrange3 -> SetForceSolid(true);

  skyBlue = new G4VisAttributes( G4Colour(135/255. , 206/255. ,  235/255. ));
  skyBlue -> SetVisibility(true);
  skyBlue -> SetForceSolid(true);

  world_color = trans_white;    // For world volume
  alpide_color = yellow;    // For world volume
  pcb_color = darkGreen;
  pla_color = white;

}



#endif