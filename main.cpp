#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.h"
#include "DetectorConstruction.h"
#include <G4RunManagerFactory.hh>
#include <QBBC.hh>
#include <qapplication.h>
#include "G4AnalysisManager.hh"
#include <G4SystemOfUnits.hh>
int main(int argc, char** argv)
{

    // Создаём интерфейс пользователя
    G4UIExecutive* ui = new G4UIExecutive(argc, argv, "Qt");

    // Создаём менеджер запуска
    auto* runManager = G4RunManagerFactory::CreateRunManager();



    // Устанавливаем геометрию
    auto* detectorConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detectorConstruction);

    // Физический список
    auto* physicsList = new QBBC();
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    // Инициализация действий
    runManager->SetUserInitialization(new ActionInitialization());

    // Инициализация визуализации
    auto* visManager = new G4VisExecutive();
    visManager->SetVerboseLevel(4);
    visManager->Initialize();
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    // Настройка интерфейса
    auto* UImanager = G4UImanager::GetUIpointer();

    // Визуализация
    UImanager->ApplyCommand("/run/initialize");
    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/drawVolume");

    // UImanager->ApplyCommand("/vis/scene/add/trajectories");
    // UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 100");
    // UImanager->ApplyCommand("/vis/trajectory/store true");


    // Запускаем интерфейс
    // ui->SessionStart();
    for(int i=0;i<6;i++){
        runManager->BeamOn(13);}



    // Завершаем приложение Qt

    delete visManager;
    delete runManager;
    return 0;
}
