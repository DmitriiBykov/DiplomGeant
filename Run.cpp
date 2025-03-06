#include "Run.h"
#include "G4SystemOfUnits.hh"
#include <vector>

Run::Run()
    : G4Run()
{
    // Инициализируем массив 1000×1000 нулями
    fEdepImage.resize(1000, std::vector<G4double>(1000, 0.));
}

Run::~Run()
{}

void Run::AddEdep(G4int ix, G4int iy, G4double edep)
{
    // Простая проверка границ
    if(ix<0 || ix>=1000) return;
    if(iy<0 || iy>=1000) return;

    fEdepImage[ix][iy] += edep;
}

void Run::Merge(const G4Run* aRun)
{
    // Сначала вызываем базовый метод Merge
    G4Run::Merge(aRun);

    // Теперь суммируем  данные
    const Run* localRun = static_cast<const Run*>(aRun);
    for (size_t i = 0; i < 1000; i++)
    {
        for (size_t j = 0; j < 1000; j++)
        {
            fEdepImage[i][j] += localRun->fEdepImage[i][j];
        }
    }
}
