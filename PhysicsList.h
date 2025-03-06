#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H
#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    virtual ~PhysicsList();
};

#endif // PHYSICSLIST_HH

#endif // PHYSICSLIST_H
