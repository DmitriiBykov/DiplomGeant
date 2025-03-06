#include "SensitiveDetector.h"
#include "G4Step.hh"
#include <G4SystemOfUnits.hh>

SensitiveDetector::SensitiveDetector(const G4String& name, int id)
    : G4VSensitiveDetector(name), fTotalEnergy(0), fID(id) {}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Получаем энергию, поглощённую в детекторе
    G4double edep = step->GetTotalEnergyDeposit();
    G4cout << edep << G4endl;
    if (edep > 0) {
        fTotalEnergy += edep;
        G4cout << "Energy deposited: " << edep / joule << " J" << G4endl;
    } else {
        G4cout << "No energy deposition in this step." << G4endl;
    }

    return true;
}
