#include "XRayImagingSteppingAction.h"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "ImagingRun.h"

XRayImagingSteppingAction::XRayImagingSteppingAction()
    : G4UserSteppingAction()
{}

XRayImagingSteppingAction::~XRayImagingSteppingAction()
{}

void XRayImagingSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Смотрим объем
    G4VPhysicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    if (!vol) return;

    // Если это не детектор, выходим
    G4String volName = vol->GetName();
    if (volName != "Detector") return;

    // Энергия, поглощённая за шаг
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return;

    // Позиция шага (можно взять mid-point)
    G4ThreeVector pos = 0.5*( step->GetPreStepPoint()->GetPosition() + step->GetPostStepPoint()->GetPosition() );

    // Размер детектора в конструкторе: 500×500 мм, центр в (0,0,300 мм)
    // но нам важно только x и y в диапазоне [-250..+250].
    // Индексы: i по x, j по y
    G4double halfSize = 250.*mm; // половина 500 мм

    G4double x = pos.x();
    G4double y = pos.y();

    // Превратим в [0..999]
    // x= -250 => i=0, x= +250 => i=999 (примерно)
    // i = floor((x + halfSize)*(1000/ (2*halfSize))) = floor((x+250)/0.5)
    // но аккуратнее:
    G4int ix = (G4int) ( ( (x + halfSize) / (2.*halfSize) ) * 1000. );
    G4int iy = (G4int) ( ( (y + halfSize) / (2.*halfSize) ) * 1000. );

    // Получаем текущий Run, приводим к XRayImagingRun
    XRayImagingRun* run = static_cast<XRayImagingRun*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );

    // Добавляем энергию
    run->AddEdep(ix, iy, edep);
}
