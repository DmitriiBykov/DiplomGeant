#ifndef MYPHYSICSLIST_H
#define MYPHYSICSLIST_H

#include "G4VModularPhysicsList.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public:
    MyPhysicsList();
    virtual ~MyPhysicsList();
};

#endif
