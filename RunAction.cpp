#include "RunAction.h"
#include "Run.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <QImage>
#include <QColor>
#include <QRgb>
#include <fstream>
#include <algorithm>

// Статические переменные для накопления изображений
QImage RunAction::accumulatedImage(1000, 1000, QImage::Format_RGB888);
int RunAction::runCounter = 0;

RunAction::RunAction() : G4UserRunAction()
{
    // При первом запуске очищаем аккумулятор
    if (runCounter == 0) {
        accumulatedImage.fill(qRgb(255,255,255)); // начальное изображение – белое (нет энергии)
    }
}

RunAction::~RunAction()
{
}

G4Run* RunAction::GenerateRun()
{
    // Создаем специальный Run для накопления энергодепозиции
    return new Run();
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    if (IsMaster()) {
        G4cout << "[Master] ### Run " << run->GetRunID() << " start." << G4endl;
    } else {
        G4cout << "[Worker] Run start." << G4endl;
    }
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    if (IsMaster()) {
        G4cout << "[Master] ### Run " << run->GetRunID() << " end." << G4endl;
        // Получаем массив энергодепозиции из текущего Run
        const Run* myRun = static_cast<const Run*>(run);
        auto& image = myRun->GetImage();

        // Для нормализации используем фиксированное значение, например, энергию насыщения пикселя
        G4double maxEdep = 5.85E-14; // [J]

        // Создаем QImage для текущего запуска
        QImage qimg(1000, 1000, QImage::Format_RGB888);
        for (size_t j = 0; j < 1000; j++){
            for (size_t i = 0; i < 1000; i++){
                // Нормализуем энергодепозицию
                G4double normVal = 0.0;
                if (maxEdep > 0.) {
                    normVal = image[i][j] / maxEdep;
                    if(normVal > 1.0) normVal = 1.0;
                }
                // Чем больше энергия, тем темнее:  normVal==1 => 255 - 255 = 0 (чёрный), normVal==0 => 255 (белый)
                G4int pixelVal = static_cast<G4int>(255 - normVal * 255);
                qimg.setPixel(i, j, qRgb(pixelVal, pixelVal, pixelVal));
            }
        }
        // Сохраняем изображение текущего запуска для отладки (например, output_<runID>.png)
        QString currentName = QString("output_%1.png").arg(run->GetRunID());
        qimg.save(currentName, "PNG");
        G4cout << "Image saved to " << currentName.toStdString() << G4endl;

        // Аккумулируем изображение текущего запуска в глобальном аккумуляторе:
        // Для каждого пикселя складываем значение интенсивности (с ограничением до 255)
        for (int j = 0; j < qimg.height(); j++){
            for (int i = 0; i < qimg.width(); i++){
                QColor curCol = qimg.pixelColor(i, j);
                int curIntensity = curCol.red(); // значение от 0 до 255 (0=чёрный, 255=белый)
                QColor accCol = accumulatedImage.pixelColor(i, j);
                int accIntensity = accCol.red();
                // Для суммирования инвертируем: если пиксель темный (низкое значение) значит много энергии,
                // поэтому берем минимум из значений, например, можно выбирать максимум накопленной энергии,
                // но в данном случае мы суммируем так, чтобы если в одном запуске пиксель темный, то он остается темным.
                // Если суммировать, то чем больше энергии – значение становится меньше.
                // Вычислим "новое" значение как минимум из текущего и уже накопленного:
                int newIntensity = (accIntensity + curIntensity)/2;
                accumulatedImage.setPixel(i, j, qRgb(newIntensity, newIntensity, newIntensity));
            }
        }

        runCounter++;
        G4cout << "Run counter = " << runCounter << G4endl;
        // Если выполнено 3 запуска, сохраняем итоговое изображение
        if (runCounter == 6)
        {
            if (accumulatedImage.save("final_output.png", "PNG"))
                G4cout << "Final accumulated image saved to final_output.png" << G4endl;
            else
                G4cout << "Failed to save final accumulated image." << G4endl;
            // При необходимости можно сбросить аккумулятор и счетчик
        }
    }
    else {
        G4cout << "[Worker] End of run." << G4endl;
    }
}

void RunAction::ClearAccumulatedImage()
{
    accumulatedImage.fill(qRgb(255,255,255));
    runCounter = 0;
}
