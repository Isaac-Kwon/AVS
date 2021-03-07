///
/// \file main.cc
/// \brief Main program
///

#include "AVSDetectorConstruction.hh"
#include "AVSActionInitialization.hh"
#include "AVSPrimaryGeneratorAction.hh"
#include "AVSAnalysisManager.hh"

#include "G4GeneralParticleSource.hh"

// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
#include "G4RunManager.hh"
// #endif

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "unistd.h"
#include "string"
#include "TString.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
struct Arguments{
  // G4bool batch;
  G4long   seed = 1; // -s
  G4String macroname = "./macro/test/gps_1.mac"; // -i
  TString  outfilename = "test_result.root"; // -o

  G4bool b_seed = FALSE;
  G4bool b_macroname = FALSE;
  G4bool b_outfilename = FALSE;
};

int Simulation(G4UIExecutive* ui, Arguments arg){
  CLHEP::RanecuEngine * RandomEngine = new CLHEP::RanecuEngine;
  // Choose the Random engine
  RandomEngine->setSeed(arg.seed);
  G4Random::setTheEngine(RandomEngine);
  
  // Construct the default run manager
  //
  // #ifdef G4MULTITHREADED
  // G4MTRunManager* runManager = new G4MTRunManager;
  // #else
  G4RunManager* runManager = new G4RunManager;
  // #endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new AVSDetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new AVSActionInitialization());
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  AVSAnalysisManager * analysisManager = AVSAnalysisManager::Instance();
  analysisManager->SetFilename(arg.outfilename);
  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    // G4String fileName = argv[1];
    UImanager->ApplyCommand(command+arg.macroname);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // AVSAnalysisManager * analysisManager = AVSAnalysisManager::Instance();
  analysisManager->Write();

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete analysisManager;
  delete visManager;
  delete runManager;
}


// struct Arguments{
//   // G4bool batch;
//   G4long seed; // -s
//   G4String macroname; // -i
//   G4String outfilename; // -o
// };

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  char c; // option
  Arguments args;
  while( (c = getopt(argc, argv, "s:i:o:h")) != -1){
    switch(c){
      case 's':
        args.seed = std::stol(optarg);
        args.b_seed = TRUE;
      break;
      case 'i':
        args.macroname   = G4String(optarg);
        args.b_macroname = TRUE;
      break;
      case 'o':
        args.outfilename   = TString(optarg);
        args.b_outfilename = TRUE;
      break;
      case 'h':
      G4cout<<"AVS Excutable (Argumented)"<<G4endl;
      G4cout<<"If there's less argv than parameters, it will use default value for absents"<<G4endl<<G4endl;
      G4cout<<"-s \tRandom Seed" <<G4endl;
      G4cout<<"-i \tExcutable input macro" <<G4endl;
      G4cout<<"-o \tOutput Filename (path, root-file)" <<G4endl;
      G4cout<<"\t \tin macro, if there's /AVS/analysis/SetFilename, that will override this -o option." << G4endl;
      G4cout<<"-h \tHelp (this message)" <<G4endl;
      return 0;
      case '?':
        printf("Unknown flag : %c", optopt);
      return 1;
    }
  }

  G4cout<< "AVS Excutable (Argumented)"<< G4endl;
  
  G4cout<< "Seed: \t" << args.seed << G4endl;
  if( argc != 1 ) G4cout<< "Input Macro: \t" << args.macroname << G4endl;
  G4cout<< "Output Filename (in argument): \t" << args.outfilename << G4endl;
  G4cout<< "Output Filename can be different due to saving command input macro" <<G4endl;


  
  Simulation(ui, args);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
