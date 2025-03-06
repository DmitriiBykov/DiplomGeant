#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    // master-тред использует RunAction для накопления глобальных результатов
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    // Для рабочих потоков:
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new RunAction());
    SetUserAction(new EventAction());
    SetUserAction(new SteppingAction());
}
