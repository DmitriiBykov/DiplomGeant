#ifndef SENSITIVEDETECTOR_H
#define SENSITIVEDETECTOR_H

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String& name, int id);
    virtual ~SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

    G4double GetTotalEnergy() const { return fTotalEnergy; }
    void Reset() { fTotalEnergy = 0; }

private:
    G4double fTotalEnergy; // Поглощённая энергия
    int fID;               // Идентификатор пикселя
};

#endif
