#include "ImagingRunAcrion.h"
#include "imagingrun.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>
#include <algorithm>

XRayImagingRunAction::XRayImagingRunAction()
    : G4UserRunAction()
{}

XRayImagingRunAction::~XRayImagingRunAction()
{}

G4Run* XRayImagingRunAction::GenerateRun()
{
    // Создаём свой специальный Run для накопления массива
    return new XRayImagingRun();
}

void XRayImagingRunAction::BeginOfRunAction(const G4Run* run)
{
    // Можно вывести информацию
    if (IsMaster()) {
        G4cout << "[Master] ### Run " << run->GetRunID() << " start." << G4endl;
    } else {
        G4cout << "[Worker] Run start." << G4endl;
    }
}

void XRayImagingRunAction::EndOfRunAction(const G4Run* run)
{
    if (IsMaster()) {
        G4cout << "[Master] ### Run " << run->GetRunID() << " end." << G4endl;
        // Теперь можно взять итоговый массив и сохранить его
        const XRayImagingRun* myRun = static_cast<const XRayImagingRun*>(run);
        auto& image = myRun->GetImage();

        // Найдём max значение, чтобы нормировать
        G4double maxEdep = 0.;
        for (size_t i = 0; i < 1000; i++){
            for (size_t j = 0; j < 1000; j++){
                if (image[i][j] > maxEdep) {
                    maxEdep = image[i][j];
                }
            }
        }

        // Создадим PPM-файл (Grayscale) — простой формат
        std::ofstream outFile("output.ppm", std::ios::out | std::ios::binary);
        outFile << "P6\n" << 1000 << " " << 1000 << "\n255\n"; // заголовок PPM (RGB, но мы будем писать серый)

        for (size_t j = 0; j < 1000; j++){
            for (size_t i = 0; i < 1000; i++){
                // i, j — как индексы;
                // NB: Здесь можно поменять порядок, чтобы не переворачивать изображение.
                // Берём значение [0..1], затем умножаем на 255
                G4double normVal = 0.0;
                if (maxEdep > 0.) {
                    normVal = image[i][j] / maxEdep;
                }
                G4int pixelVal = (G4int)(normVal * 255);

                // Пишем R=G=B=pixelVal
                unsigned char r = (unsigned char)pixelVal;
                unsigned char g = (unsigned char)pixelVal;
                unsigned char b = (unsigned char)pixelVal;
                outFile.write((char*)&r, 1);
                outFile.write((char*)&g, 1);
                outFile.write((char*)&b, 1);
            }
        }
        outFile.close();

        G4cout << "Image saved to output.ppm (grayscale)." << G4endl;
        G4cout << "Max Edep = " << maxEdep/keV << " keV" << G4endl;
    }
    else {
        G4cout << "[Worker] End of run." << G4endl;
    }
}
