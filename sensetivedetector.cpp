#include "SensitiveDetector.h"
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <vector>
#include <mutex>

constexpr int kMatrixSizeX = 1000;
constexpr int kMatrixSizeY = 1000;
constexpr double kPixelSizeX = 0.02 * cm; // Размер пикселя по X
constexpr double kPixelSizeY = 0.02 * cm; // Размер пикселя по Y

// Глобальная матрица энергии (в многопоточной симуляции потребуется защита)
std::vector<std::vector<double>> energyMatrix(kMatrixSizeX, std::vector<double>(kMatrixSizeY, 0.0));
std::mutex matrixMutex; // Для защиты данных в многопоточной симуляции

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    if (volume->GetName() != "Detector") return false;

    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    double energy = step->GetTotalEnergyDeposit();

    // Преобразование позиции в индексы матрицы
    int xIndex = static_cast<int>((position.x() + 10 * cm) / kPixelSizeX);    int yIndex = static_cast<int>((position.y() + 10 * cm) / kPixelSizeY);

    if (xIndex >= 0 && xIndex < kMatrixSizeX && yIndex >= 0 && yIndex < kMatrixSizeY) {
        std::lock_guard<std::mutex> lock(matrixMutex); // Защита данных
        energyMatrix[xIndex][yIndex] += energy; // Накопление энергии в пикселе
    }

    return true;
}
