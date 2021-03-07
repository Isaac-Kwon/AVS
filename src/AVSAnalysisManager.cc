///
/// \file AVSAnalysisManager.cc
/// \brief Implementation of the AVSAnalysisManager class
///

#include "AVSAnalysisManager.hh"

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "G4GenericMessenger.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"
#include "AVSDetectorConstruction.hh"

AVSAnalysisManager* AVSAnalysisManager::fInstance = 0;

AVSAnalysisManager::AVSAnalysisManager(): fFilename("result.root"){
  fInstance = this;
  if(!fCmdDefined) DefineCommand();
}

AVSAnalysisManager::~AVSAnalysisManager(){
  G4cout<<"AnalysisManager Deleting..."<<G4endl;
  Destroy();
}

void AVSAnalysisManager::Destroy(){
  G4cout<<"AnalysisManager Closing file."<<G4endl;
  if(fFile){
    fFile->Delete("*");
  }
  G4cout<<"AnalysisManager End of closing."<<G4endl;
  G4cout<<fTreeSource<<"\t"<<fTreeStepper <<"\t"<<fTreeTrack<<G4endl;
}


AVSAnalysisManager* AVSAnalysisManager::Instance(){
  // A new instance of AnalysisManager is created if it does not exist:
  if (fInstance == 0) 
    {
      fInstance = new AVSAnalysisManager();
    }
  
  // The instance of AnalysisManager is returned:
  return fInstance;
}

void AVSAnalysisManager::Book(){
  if(fBooked) return;

  // Construction of file
  fFile = new TFile(TString::Format("%s", fFilename.Data()),"RECREATE");

  //-------------------------------------------//
  // Construction of Trees

  fTreeSource = new TTree("tSource", "Source");
  fTreeSource->Branch("pid"       ,& fSource.pid, "pid/I"           );
  fTreeSource->Branch("EKin"      ,& fSource.EKin, "EKin/I"         );
  fTreeSource->Branch("posX"      ,& fSource.posX, "posX/D"         );
  fTreeSource->Branch("posY"      ,& fSource.posY, "posY/D"         );
  fTreeSource->Branch("posZ"      ,& fSource.posZ, "posZ/D"         );
  fTreeSource->Branch("dirTheta"  ,& fSource.dirTheta, "dirTheta/D" );
  fTreeSource->Branch("dirPhi"    ,& fSource.dirPhi, "dirPhi/D"     );
  fTreeSource->Branch("weight"    ,& fSource.weight, "weight/D"     );
  
  fTreeStepper = new TTree("tStepper", "Stepper");
  fTreeStepper->Branch("pid",       & fStepper.pid, "pid/I");
  fTreeStepper->Branch("Ekin",      & fStepper.Ekin, "Ekin/D");
  fTreeStepper->Branch("preVolume", & fStepper.preVolume, "preVolume/B");
  fTreeStepper->Branch("postVolume",& fStepper.postVolume, "postVolume/B");
  fTreeStepper->Branch("preposX",   & fStepper.preposX, "preposX/D");
  fTreeStepper->Branch("preposY",   & fStepper.preposY, "preposY/D");
  fTreeStepper->Branch("preposZ",   & fStepper.preposZ, "preposZ/D");
  fTreeStepper->Branch("postposX",  & fStepper.postposX, "postposX/D");
  fTreeStepper->Branch("postposY",  & fStepper.postposY, "postposY/D");
  fTreeStepper->Branch("postposZ",  & fStepper.postposZ, "postposZ/D");
  fTreeStepper->Branch("dirTheta",  & fStepper.dirTheta, "dirTheta/D");
  fTreeStepper->Branch("dirPhi",    & fStepper.dirPhi, "dirPhi/D");
  fTreeStepper->Branch("TID",       & fStepper.TID, "TID/D");
  fTreeStepper->Branch("NIEL",      & fStepper.NIEL, "NIEL/D");
  

  fTreeTrack = new TTree("tTrack", "TrackCollector");
  fTreeTrack->Branch("pid",       & fTrack.pid,      "pid/I");
  fTreeTrack->Branch("posX",      & fTrack.posX,     "posX/D");
  fTreeTrack->Branch("posY",      & fTrack.posY,     "posY/D");
  fTreeTrack->Branch("posZ",      & fTrack.posZ,     "posZ/D");
  fTreeTrack->Branch("nVol",      & fTrack.nVol,     "nVol/I");
  fTreeTrack->Branch("Eloss",       fTrack.Eloss,    "Eloss[nVol]/F");
  fTreeTrack->Branch("Passed",      fTrack.Passed,   "Passed[nVol]/O");
  fTreeTrack->Branch("dirTheta",  & fTrack.dirTheta, "dirTheta/D");
  fTreeTrack->Branch("dirPhi",    & fTrack.dirPhi,   "dirPhi/D");


  //-------------------------------------------//
  // Construction of Volume mapping

  { // If in dictionary of ScoringVolume doesn't have the scoring volumes, input the volumes in dictionary
    const AVSDetectorConstruction* detectorConstruction
        = static_cast<const AVSDetectorConstruction*>
          (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    if (!fScoringStandAlpha) { 
      fScoringStandAlpha = detectorConstruction->GetScoringStandAlpha();
      fScoringVolumes[fScoringStandAlpha] = 1;
    }

    if (!fScoringStandGamma) { 
      fScoringStandGamma = detectorConstruction->GetScoringStandGamma();
      fScoringVolumes[fScoringStandGamma] = 2;
    }

    if (!fScoringALPIDEUpper) { 
      fScoringALPIDEUpper = detectorConstruction->GetScoringALPIDE_Upper();   
      fScoringVolumes[fScoringALPIDEUpper] = 3;
    }

    if (!fScoringALPIDEUnder) { 
      fScoringALPIDEUnder = detectorConstruction->GetScoringALPIDE_Under();   
      fScoringVolumes[fScoringALPIDEUnder] = 4;
    }

    if (!fScoringPCBBoard) {
      fScoringPCBBoard = detectorConstruction->GetScoringPCBBoard();   
      fScoringVolumes[fScoringPCBBoard] = 5;

    }
  }

  G4cout<<"Scoring Mapping"<<G4endl;
  for(auto it=fScoringVolumes.begin(); it!=fScoringVolumes.end(); it++){
    G4cout<<it->first << " :: " << it->second << G4endl;
  }

  fBooked = true;

}

void AVSAnalysisManager::FillSource(const G4Track* track){
  if(!bTreeSource) return;
  if(track->GetParentID() != 0 && GetRecordPrimaryOnly()) return;
  
  fSource.pid      = track->GetDynamicParticle()->GetPDGcode();
  fSource.EKin     = track->GetKineticEnergy();

  G4ThreeVector vertex    = track->GetPosition();
  fSource.posX     = vertex.x();
  fSource.posY     = vertex.y();
  fSource.posZ     = vertex.z();

  G4ThreeVector direction = track->GetMomentumDirection();
  fSource.dirTheta = direction.theta();
  fSource.dirPhi   = direction.phi();

  if (fSource.dirPhi < 0.) fSource.dirPhi += twopi;
  
  fSource.weight   = track->GetWeight();

  fTreeSource->Fill();

}

void AVSAnalysisManager::FillStepper(const G4Step* step){
  if(!bTreeStepper) return;
  if(step->GetTrack()->GetParentID() != 0 && GetRecordPrimaryOnly()) return;

  G4Track* track      = step->GetTrack();
  fStepper.pid        = track->GetDynamicParticle()->GetPDGcode();
  fStepper.Ekin       = step->GetPreStepPoint()->GetKineticEnergy();

  G4ThreeVector preStep = step->GetPreStepPoint()->GetPosition();
  fStepper.preposX    = preStep.x()/CLHEP::mm;
  fStepper.preposY    = preStep.y()/CLHEP::mm;
  fStepper.preposZ    = preStep.z()/CLHEP::mm;

  G4ThreeVector postStep = step->GetPostStepPoint()->GetPosition();
  fStepper.postposX   = postStep.x()/CLHEP::mm;
  fStepper.postposY   = postStep.y()/CLHEP::mm;
  fStepper.postposZ   = postStep.z()/CLHEP::mm;

  G4ThreeVector direction = track->GetMomentumDirection();
  fStepper.dirTheta   = direction.theta();
  fStepper.dirPhi     = direction.phi();

  fStepper.TID        = step->GetTotalEnergyDeposit();
  fStepper.NIEL       = step->GetNonIonizingEnergyDeposit();

  G4LogicalVolume*  preVolume = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  G4LogicalVolume* postVolume = step->GetPostStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  fStepper.preVolume  = fScoringVolumes[preVolume ];
  fStepper.postVolume = fScoringVolumes[postVolume];

  fTreeStepper->Fill();
  
}


// Traking 
// 1st impact point
// last impacting point
// total kinetic energy below 11um ()

void AVSAnalysisManager::FillTrack(const G4Step* step){
  if(!bTreeTrack) return;
  if(step->GetTrack()->GetParentID() != 0 && GetRecordPrimaryOnly()) return;
  
  // fTrack.pid = step->GetTrack()->GetTrackID(); // Track Number
  fTrack.pid = step->GetTrack()->GetDynamicParticle()->GetPDGcode(); // Track Number

  G4LogicalVolume* lv1 = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4LogicalVolume* lv2 = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  Int_t lv1n = fScoringVolumes[lv1]; //preStep Volume number
  // Int_t lv2n = fScoringVolumes[lv2]; //postStep Volume number

  G4double edep = (step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy())/MeV;
  WDeltaEnergy[lv1n] += edep;
  
  if(!WPassed[lv1n]) WPassed[lv1n]=kTRUE;

  G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
  fTrack.posX = pos.getX();
  fTrack.posY = pos.getY();
  fTrack.posZ = pos.getZ();

  G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();
  fSource.dirTheta = direction.theta();
  fSource.dirPhi   = direction.phi();

}

void AVSAnalysisManager::EndOfTrack(const G4Track* track){
  if(track->GetParentID() != 0 && GetRecordPrimaryOnly()) return;

  G4cout<<"TrackPID = "<<fTrack.pid<<G4endl;

  G4cout<<"Track Eloss\t";
  // Int_t maxvn=0;
  for (auto it = WDeltaEnergy.begin(); it != WDeltaEnergy.end(); ++it) {
    // maxvn = maxvn>it->first ? maxvn : it->first;
    fTrack.Eloss[it->first] = it->second;
    G4cout<<"LOSSAT"<<it->first<< "::" << it->second << "\t";
  }
  G4cout<<G4endl;

  for (auto it = WPassed.begin(); it != WPassed.end(); ++it) {
    // maxvn = maxvn>it->first ? maxvn : it->first;
    fTrack.Passed[it->first] = it->second;
    if(it->second) G4cout<<"PASSED"<<it->first<<"\t";
  }
  G4cout<<G4endl;

  // fTrack.nVol = maxvn;
  if(bTreeTrack){
    fTreeTrack->Fill();
  }
  WDeltaEnergy.clear();
  WPassed.clear();                                       
  for (Int_t i=0; i<fTrack.nVol; i++){
    fTrack.Eloss[i] = 0;
    fTrack.Passed[i] = kFALSE;
  }
}

void AVSAnalysisManager::Write(){
    /*if(bTreeSource)*/ fFile->Add(fTreeSource);
    /*if(bTreeStepper)*/ fFile->Add(fTreeStepper);
    /*if(bTreeTrack)*/ fFile->Add(fTreeTrack);
    G4cout<<"AVSAnalysisManager::Write - Writing File to " << fFile->GetName() <<G4endl;
    TString fst = fFile->IsOpen() ? "True" : "False";
    G4cout<<"File is opened? : "<< fst << G4endl;
    G4cout<<"In Option : "<< fFile->GetOption() << G4endl;
    fFile->Write(0, TObject::kOverwrite, 0);
    G4cout<<"AVSAnalysisManager::Write - File wrtied in " << fFile->GetName() <<G4endl;
}

void AVSAnalysisManager::SetFilename(TString filename){
  if(fBooked){
    G4cout<<"AVSAnalysisManager::SetFilename - Already the output rootfile is written before setting filename. (The filename is changed to " << filename << " from " << fFilename <<G4endl;
  }
  fFilename = filename;
}


void AVSAnalysisManager::SetRecordOption(const Int_t number){
  //(3)(2)(1)
  // Components
  // 1: OnlyRecordPrimaryParticle

  Int_t tempnum = number;
  Bool_t * refs[1] = {&fRecordPrimaryOnly};

  for(Int_t i=0; i<1; i++){
    if(tempnum % 10){
      *refs[i] = true;
    }else{
      *refs[i] = false;
    }
  }

}

void AVSAnalysisManager::SetRecordSet(const Int_t number){
  //(3)(2)(1)
  // Components
  // 1: TreeSource
  // 2: TreeStepper
  // 3: TreeTrack

  Int_t tempnum = number;
  Bool_t * refs[3] = {&bTreeSource, &bTreeStepper, &bTreeTrack};

  for(Int_t i=0; i<3; i++){
    if(tempnum % 10 == 1){
      *refs[i] = true;
    }else{
      *refs[i] = false;
    }
    tempnum = tempnum/10;
  }
}
// void AVSAnalysisManager::SetRecordStepPrimaryOnly(Int_t primaryOnly){
//   if(primaryOnly == 0){
//     fRecordPrimaryOnly=false;
//   }else{
//     fRecordPrimaryOnly=true;
//   }

//   return;
// }

// Bool_t AVSAnalysisManager::GetRecordStepPrimaryOnly(){
//   return fRecordPrimaryOnly;
// }

void AVSAnalysisManager::DefineCommand(){
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, 
                                      "/AVS/Analysis/", 
                                      "AnalysisControl");
  auto& SetFileNameCmd
    = fMessenger->DeclareMethod("SetFilename",
                                &AVSAnalysisManager::SetFilename, 
                                "SetFilename");
  SetFileNameCmd.SetParameterName("fname", true);
  // SetParticlePresetCmd.SetRange("n>-1. && n<2.");
  SetFileNameCmd.SetDefaultValue("result.root");

  auto& SetRecordPrimaryCmd
    = fMessenger->DeclareMethod("RecordPrimaryOnly",
                                &AVSAnalysisManager::SetRecordPrimaryOnly, 
                                "RecordPrimaryOnly");
  SetRecordPrimaryCmd.SetParameterName("PrimaryOnly", true);
  SetRecordPrimaryCmd.SetDefaultValue("true");

  auto& SetRecordSetCmd
    = fMessenger->DeclareMethod("SetRecordSet",
                                &AVSAnalysisManager::SetRecordSet, 
                                "Option for RecordingSet, [Track, Stepper, Source]");
  SetRecordSetCmd.SetParameterName("Set[111]", true);
  SetRecordSetCmd.SetDefaultValue("111");
}


TTree* AVSAnalysisManager::GetTreeSource(){return fTreeSource;}
TTree* AVSAnalysisManager::GetTreeStepper(){return fTreeStepper;}
TTree* AVSAnalysisManager::GetTreeTrack(){return fTreeTrack;}

void AVSAnalysisManager::AutoSave(){
  fTreeSource->AutoSave();
  fTreeStepper->AutoSave();
  fTreeTrack->AutoSave();
}