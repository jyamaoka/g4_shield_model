//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file ShieldModelEventAction.cc
/// \brief Implementation of the ShieldModelEventAction class

#include "ShieldModelEventAction.hh"
#include "ShieldModelRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "ShieldModelAnalysis.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShieldModelEventAction::ShieldModelEventAction(ShieldModelRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fLtvl(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ShieldModelEventAction::~ShieldModelEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShieldModelEventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fLtvl = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ShieldModelEventAction::EndOfEventAction(const G4Event* event)
{ 
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  
  // fill ntuple
  if (fEdep>0.0){
    analysisManager->FillNtupleDColumn(0, fEdep);
    analysisManager->FillNtupleDColumn(1, fLtvl);
    analysisManager->AddNtupleRow();  
  }
  
  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
  //if ( 1 ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    G4cout
       << "   Absorber: total energy: " << std::setw(7)
                                        << G4BestUnit(fEdep,"Energy")
       << "       total track length: " << std::setw(7)
                                        << G4BestUnit(fLtvl,"Length")
       << G4endl;
  }

  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......