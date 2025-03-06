#include "PhysicsList.h"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"

PhysicsList::PhysicsList() {
    // Добавляем стандартные электромагнитные процессы
    RegisterPhysics(new G4EmStandardPhysics());
    // Добавляем процессы распада частиц
    RegisterPhysics(new G4DecayPhysics());
}

PhysicsList::~PhysicsList() {}
