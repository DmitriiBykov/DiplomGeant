#ifndef Run_H
#define Run_H

#include "G4Run.hh"

class Run : public G4Run
{
public:
    Run();
    virtual ~Run();

    // Добавить энергию в конкретный пиксель
    void AddEdep(G4int ix, G4int iy, G4double edep);

    // Доступ к «карте» (для записи файла)
    const std::vector<std::vector<G4double>>& GetImage() const { return fEdepImage; }

    // Перегружаем метод Merge для суммирования результатов от всех рабочих потоков
    virtual void Merge(const G4Run* aRun) override;

private:
    // Используем двумерный вектор (или статический массив) размером 1000×1000
    // Здесь для гибкости — std::vector.
    std::vector<std::vector<G4double>> fEdepImage;
};

#endif
