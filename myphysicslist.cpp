#include "MyPhysicsList.h"
#include "G4VModularPhysicsList.hh"
#include "G4EmLivermorePhysics.hh"

MyPhysicsList::MyPhysicsList() : G4VModularPhysicsList()
{
    // Устанавливаем уровень подробности
    SetVerboseLevel(1);

    // Регистрируем низкоэнергетическую электромагнитную физику
    RegisterPhysics(new G4EmLivermorePhysics());

}

MyPhysicsList::~MyPhysicsList()
{
}
