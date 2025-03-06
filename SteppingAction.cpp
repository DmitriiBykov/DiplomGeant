#include "SteppingAction.h"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "Run.h"

SteppingAction::SteppingAction()
    : G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Смотрим объем
    G4VPhysicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    if (!vol) return;

    // Если это не детектор, выходим
    G4String volName = vol->GetName();
    if (volName != "Detector") return;

    // Энергия, поглощённая за шаг
    G4double edep = step->GetTotalEnergyDeposit();


    // Позиция шага (можно взять mid-point)
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

    // Размер детектора в конструкторе: 200×200 мм, центр в (0,0,300 мм)
    // но нам важно только x и y в диапазоне [-100..+100].
    // Индексы: i по x, j по y
    G4double halfSize = 100.*mm; // половина 200 мм

    G4double x = pos.x();
    G4double y = pos.y();

    // Превратим в [0..999]
    // x= -100=> i=0, x= +100 => i=999 (примерно)
    // i = floor((x + halfSize)*(1000/ (2*halfSize))) = floor((x+100)/0.5)
    // но аккуратнее:
    G4int ix = (G4int) ( ( (x + halfSize) / (2.*halfSize) ) * 1000. );
    G4int iy = (G4int) ( ( (y + halfSize) / (2.*halfSize) ) * 1000. );

    // Получаем текущий Run, приводим к Run
    Run* run = static_cast<Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );

    // Добавляем энергию
    //G4cout <<"AddEdep X:"<< ix<<"Y:"<<iy<<"Edep: "<< edep << G4endl;
    run->AddEdep(ix, iy, edep/joule);
}
