#ifndef SENSETIVEDETECTOR_H
#define SENSETIVEDETECTOR_H

#include <G4String.hh>
#include <G4VSensitiveDetector.hh>
class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override;

    // Обработка попаданий
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
};


#endif // SENSETIVEDETECTOR_H
