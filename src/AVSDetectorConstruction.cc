///
/// \file AVSDetectorConstruction.cc
/// \brief Implementation of the AVSDetectorConstruction class
///

#include "AVSDetectorConstruction.hh"

#include "string"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4AssemblyVolume.hh"

#include "AVSStructure.hh"

#include "G4UImanager.hh"
#include "G4LogicalVolumeStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSDetectorConstruction::AVSDetectorConstruction()
: G4VUserDetectorConstruction()
{
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AVSDetectorConstruction::~AVSDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* AVSDetectorConstruction::Construct()
{  
  G4bool checkOverlaps = true;

  //     
  // World
  //

  G4NistManager * nist = G4NistManager::Instance();
  G4double world_sizeXY = 300.0 * mm;
  G4double world_sizeZ  = 150.0 * mm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  
  fStructure = new AVSStructure();

  //BulkALPIDE
  // ALPIDELogical = fStructure->LV_ALPIDE();
  // new G4PVPlacement(0, G4ThreeVector(), ALPIDELogical, "ALPIDE", logicWorld, false, 0, checkOverlaps);
  // ALPIDESliceLogical = fStructure->Replica_ALPIDE(ALPIDELogical);

  //11um SliceALPIDE
  ALPIDEAssembly = fStructure->AV_ALPIDE();
  {
    // G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 0.0*deg, 0.0*deg);
    // G4ThreeVector Ta = G4ThreeVector(0,0,0);
    G4Transform3D t3d = G4Transform3D();
    ALPIDEAssembly->MakeImprint(logicWorld, t3d);
  }


  CarrierBoardLogical = fStructure->LV_ALPIDECarrierBoard();
  UpperStandAlphaLogical = fStructure->LV_UpperStandAlpha();
  UpperStandGammaLogical = fStructure->LV_UpperStandGamma();

  new G4PVPlacement(0, G4ThreeVector(0, -1.0*mm*0.5 + -100*um*0.5, -2.8*mm), CarrierBoardLogical, "CarrierBoard", logicWorld, false, 0, checkOverlaps);

  fStandDistance = 14*mm;
  fStandState = 0;
  fUpperStandAlpha = new G4PVPlacement(0, G4ThreeVector(0, fStandDistance-1*mm, 0), UpperStandAlphaLogical, "UpperStandAlpha", logicWorld, false, 0, checkOverlaps);

  fUpperStandGamma = new G4PVPlacement(0, G4ThreeVector(0, fStandDistance-1*mm, 40*mm), UpperStandGammaLogical, "UpperStandGamma", logicWorld, false, 0, checkOverlaps);

  fScoringALPIDEUpper = FindLogicalVolume("ALPIDEUpper_Logical");
  fScoringALPIDEUnder = FindLogicalVolume("ALPIDEUnder_Logical");

  return physWorld;
}

G4LogicalVolume* AVSDetectorConstruction::FindLogicalVolume(const G4String name){
  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  for (size_t iLV = 0; iLV < pLVStore->size(); iLV++ ) {
    G4LogicalVolume* pLV = (*pLVStore)[iLV];
    const G4String& logVolName = pLV->GetName();
    if (logVolName == name) return pLV;
  }
  return nullptr;
}

void AVSDetectorConstruction::SetDistanceUpperStand(G4double val)
{
  if (!fUpperStandAlpha || ! fUpperStandGamma) {
      G4cerr << "Upper stand has not yet been constructed." << G4endl;
      return;
  }
  
  fStandDistance = val;

  fUpperStandAlpha->SetTranslation(G4ThreeVector(0, fStandDistance*mm-1*mm, (fStandState * 40*mm)));
  fUpperStandGamma->SetTranslation(G4ThreeVector(0, fStandDistance*mm-1*mm, ((fStandState-1) * 40*mm)));

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  std::ostringstream stringStream;
  stringStream << "/gps/pos/centre 0. " << fStandDistance/mm << " 0. "<< G4String("mm");

  UImanager->ApplyCommand(G4String(stringStream.str()));

  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void AVSDetectorConstruction::SetStand(G4int StandNo){
  if (!fUpperStandAlpha || ! fUpperStandGamma) {
      G4cerr << "Upper stand has not yet been constructed." << G4endl;
      return;
  }

  if(StandNo < 0 || StandNo > 1){
    G4cerr << "Stand Number if out of range" << G4endl;
  }

  fStandState = StandNo;
  SetDistanceUpperStand(fStandDistance);
}

void AVSDetectorConstruction::SetParticlePreset(G4int preset){
  // 0: alpha 5.486MeV Mono
  // 1: gamma 0.0595MeV Mono
  if(preset == 0){
    SetAlpha(5.486*MeV);
  }
  if(preset == 1){
    SetGamma(0.0595*MeV);
  }
}

void AVSDetectorConstruction::SetAlpha(G4double Energy){
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  std::ostringstream stringStream;

  stringStream << "/gps/ene/mono " << Energy/MeV << G4String(" MeV");
  std::vector<G4String> commands = {
    "/gps/particle alpha",
    "/gps/pos/type Plane",
    "/gps/pos/shape Circle",
    "/gps/pos/radius 2.5 mm",
    "/gps/pos/rot1 0. 0. 1.",
    "/gps/pos/rot2 1. 0. 0.",
    "/gps/ang/type iso",
    "/gps/ene/type Mono",
    G4String(stringStream.str())
  };
  for(G4String command : commands){
    UImanager->ApplyCommand(command);
  }

  SetStand(0);
  
}

void AVSDetectorConstruction::SetGamma(G4double Energy){
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  std::ostringstream stringStream;

  stringStream << "/gps/ene/mono " << Energy/MeV << G4String(" MeV");

  std::vector<G4String> commands = {
    "/gps/particle gamma",
    "/gps/pos/type Plane",
    "/gps/pos/shape Circle",
    "/gps/pos/radius 5 mm",
    "/gps/pos/rot1 0. 0. 1.",
    "/gps/pos/rot2 1. 0. 0.",
    "/gps/ang/type iso",
    "/gps/ene/type Mono",
    G4String(stringStream.str())
  };
  for(G4String command : commands){
    UImanager->ApplyCommand(command);
  }

  SetStand(1);
  
}



// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AVSDetectorConstruction::DefineCommands()
{
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/AVS/Source/", 
                                      "Detector control");

  // StandDistance CMD
  auto& StandDistanceCmd
    = fMessenger->DeclareMethodWithUnit("StandDistance","mm",
                                &AVSDetectorConstruction::SetDistanceUpperStand, 
                                "Set distance between upperstand and ALPIDE");
  StandDistanceCmd.SetParameterName("distance", true);
  StandDistanceCmd.SetRange("distance>=0. && distance<100.");
  StandDistanceCmd.SetDefaultValue("14.");

  // SetAlpha CMD
  auto& SetAlphaCmd
    = fMessenger->DeclareMethodWithUnit("SetAlpha","MeV",
                                &AVSDetectorConstruction::SetAlpha, 
                                "Set Particle *alpha* and energy");
  SetAlphaCmd.SetParameterName("KE", true);
  SetAlphaCmd.SetRange("KE>=0. && KE<100.");
  SetAlphaCmd.SetDefaultValue("5.486");

  // SetGammaCMD
  auto& SetGammaCmd
    = fMessenger->DeclareMethodWithUnit("SetGamma","MeV",
                                &AVSDetectorConstruction::SetGamma, 
                                "Set Particle *gamma* and energy");
  SetGammaCmd.SetParameterName("E", true);
  SetGammaCmd.SetRange("E>=0. && E<100.");
  SetGammaCmd.SetDefaultValue("0.0595");

  // SetStandCMD
  auto& SetStandCmd
    = fMessenger->DeclareMethod("SetStand",
                                &AVSDetectorConstruction::SetStand, 
                                "Set Stand");
  SetStandCmd.SetParameterName("n", true);
  // SetStandCmd.SetRange("n>-1 && n<2.");
  SetStandCmd.SetDefaultValue("0");

  // SetParticlePresetCMD
  auto& SetParticlePresetCmd
    = fMessenger->DeclareMethod("SetParticlePreset",
                                &AVSDetectorConstruction::SetParticlePreset, 
                                "Set Particle with preset");
  SetParticlePresetCmd.SetParameterName("n", true);
  // SetParticlePresetCmd.SetRange("n>-1. && n<2.");
  SetParticlePresetCmd.SetDefaultValue("0");
  


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
