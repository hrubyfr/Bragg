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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
  nistManager->FindOrBuildMaterial("G4_Galactic");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  G4double LayerThickness = 1.*um;
  G4double XYSize  = 25.*cm;

  auto BoxThickness = DetectorConstruction::LayerNumber * LayerThickness;
  auto worldSizeXY = 1.2 * XYSize;
  auto worldSizeZ  = 1.2 * BoxThickness;

  // Get materials
  auto LayerMaterial = G4Material::GetMaterial("G4_POLYETHYLENE");
  auto WorldMaterial = G4Material::GetMaterial("G4_Galactic");


  if ( ! WorldMaterial || ! LayerMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }
  //
  // World
  //
  auto worldS = new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ);
  auto worldLV = new G4LogicalVolume(worldS, WorldMaterial, "World");
  auto worldPV = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLV, "World", 0, false, 0, fCheckOverlaps);
  //
  // Box
  //
/*
  auto BoxS = new G4Box ("Box", XYSize/2, XYSize/2, BoxThickness/2);
  auto BoxLV = new G4LogicalVolume(BoxS, WorldMaterial, "Box");
  auto BoxPV = new G4PVPlacement (0, G4ThreeVector(), BoxLV, "Box", worldLV, false, 0, fCheckOverlaps);
*/
  //
  // Layer
  //

  auto layerS = new G4Box("Layer", XYSize/2, XYSize/2, LayerThickness/2);
  auto layerLV = new G4LogicalVolume(layerS, LayerMaterial, "Layer");
  for (G4int n = 0; n < DetectorConstruction::LayerNumber; n++){
    G4int copyN = 1000 + n;
    blockArray[n] = new G4PVPlacement (0, G4ThreeVector(0, 0, n * LayerThickness), layerLV, "Layer", worldLV, false, copyN, fCheckOverlaps);
    G4cout << "layer number " << n << " constructed \n";

  }

  //
  // Visualization attributes
  //

  auto simpleWorldVisAtt = new G4VisAttributes(G4Colour(10.0, 10.0, 10.0));
  simpleWorldVisAtt->SetVisibility(false);
  worldLV->SetVisAttributes (simpleWorldVisAtt);

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  //layerLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
