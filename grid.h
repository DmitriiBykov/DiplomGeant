#ifndef Grid_H
#define Grid_H

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"

class Grid {
public:
    Grid(G4Material* mat);
    ~Grid();
    G4LogicalVolume* GetLogicalVolume() const { return fLogicGrid; }
private:
    G4LogicalVolume* fLogicGrid;
};

#endif // Grid_H
