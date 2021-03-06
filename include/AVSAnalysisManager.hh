///
/// \file AVSActionInitialization.hh
/// \brief Definition of the AVSActionInitialization class
///

#ifndef AVSAnalysis_h
#define AVSAnalysis_h 1

#include "TROOT.h"
#include "map"
#include "TTree.h"

// class TTree;
class TFile;
class G4Track;
class G4GenericMessenger;
class G4Step;
class G4LogicalVolume;

// using G4AnalysisManager = AVSAnalysisManager;


//============================================//
// Tree 1 : Initial Source Distribution (Profile) : (pre-Event analysis)
// 0. pid         {I} Particle ID
// 1. EKin        {D} Kinetic Energy of Generated Particle
// 2. posX        {D} Position {X,Y,Z} of Primary Particle at first position
// 3. posY        {D}
// 4. posZ        {D}
// 5. dirTheta    {D} Direction {Theta, Phi} of Primary Particle at first position
// 6. dirPhi      {D} 
// 7. weight (??) {D}

struct SourceEntry{
    Int_t pid;
    Double_t EKin;
    Double_t posX;
    Double_t posY;
    Double_t posZ;
    Double_t dirTheta;
    Double_t dirPhi;
    Double_t weight;
};
//============================================//


//============================================//
// Tree 2 : Detecting Distribution (StepAction Analysis, for scoring volumes)
// 0.   PID           {I} 
// 1.   Ekin          {D}
// 2.   preVolume     {I} (0: Otherwise, 1: () , 2: Stand), (-1, -2, -3 ... : Level inside of ALPIDE)
// 3.   postVolume    {I} (0: Otherwise, 1: () , 2: Stand), (-1, -2, -3 ... : Level inside of ALPIDE)
// 4.   preposX       {D}
// 5.   preposY       {D}
// 6.   preposZ       {D}
// 7.   postposX      {D}
// 8.   postposY      {D}
// 9.   postposZ      {D}
// 10.  Theta         {D}
// 11.  Phi           {D}
// 12.  TID           {D}
// 13.  NIEL          {D}

struct StepperEntry{
    Int_t pid;
    Double_t Ekin;
    Char_t preVolume;
    Char_t postVolume;
    Double_t preposX;
    Double_t preposY;
    Double_t preposZ;
    Double_t postposX;
    Double_t postposY;
    Double_t postposZ;
    Double_t dirTheta;
    Double_t dirPhi;
    Double_t TID;
    Double_t NIEL;
};

struct TrackEntry{
    Int_t    pid;
    Double_t  posX;
    Double_t  posY;
    Double_t  posZ;
    Int_t    nVol=6;
    Float_t  Eloss[10];
    Bool_t   Passed[10];
    Double_t dirTheta;
    Double_t dirPhi;

};

//============================================//

class AVSAnalysisManager{
    private:
    SourceEntry     fSource;
    // DetectorEntry   fDetector;
    StepperEntry    fStepper;
    TrackEntry      fTrack;

    TFile * fFile;
    TTree * fTreeSource;
    TTree * fTreeStepper;
    TTree * fTreeTrack;

    // Bool_t bFile = kTRUE;
    Bool_t bTreeSource = kTRUE;
    Bool_t bTreeStepper = kTRUE;
    Bool_t bTreeTrack = kTRUE;

    TString fFilename;

    static AVSAnalysisManager* fInstance;
    G4GenericMessenger* fMessenger;

    Bool_t fBooked = kFALSE;
    Bool_t fCmdDefined = kFALSE;
    
    Bool_t fRecordPrimaryOnly = kFALSE;

    std::map<Int_t, Float_t> fTIDs; // Ioninzing Dose in specific Volume

    G4LogicalVolume * fScoringStandAlpha;
    G4LogicalVolume * fScoringStandGamma;
    G4LogicalVolume * fScoringALPIDEUpper;
    G4LogicalVolume * fScoringALPIDEUnder;
    G4LogicalVolume * fScoringPCBBoard;
    std::map<G4LogicalVolume*, Int_t> fScoringVolumes;

    //Track Recording part
    std::map<Int_t, Float_t> WDeltaEnergy;
    std::map<Int_t, Bool_t>  WPassed;
    
    protected:
    AVSAnalysisManager();
    void Destroy();
    void DefineCommand();

    public:
    ~AVSAnalysisManager();

    void Book();
    static AVSAnalysisManager* Instance();
    
    void SetFilename(TString filename);
    TString GetFilename(){return fFilename;}
    
    // void FillEntrySource(const SourceEntry& entry);
    void FillSource(const G4Track* track);
    void FillStepper(const G4Step* step);
    void FillTrack(const G4Step* step);
    void EndOfTrack(const G4Track* track);

    // void SetRecordStepPrimaryOnly(Int_t primaryOnly);
    // Bool_t GetRecordStepPrimaryOnly();

    void SetRecordOption(Int_t number);
    void SetRecordSet(Int_t number);

    void   SetRecordPrimaryOnly(Bool_t primaryOnly=kTRUE){fRecordPrimaryOnly=primaryOnly;}
    Bool_t GetRecordPrimaryOnly(){return fRecordPrimaryOnly;}
    
    TTree* GetTreeSource();
    TTree* GetTreeStepper();
    TTree* GetTreeTrack();
    
    
    void Write();

};


#endif