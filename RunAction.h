#ifndef RUNACTION_H
#define RUNACTION_H

#include "G4UserRunAction.hh"
#include <QImage>

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

    // Метод для очистки аккумулятора (если потребуется)
    static void ClearAccumulatedImage();

private:
    // Статические переменные для аккумуляции изображений
    static QImage accumulatedImage; // итоговое изображение
    static int runCounter;          // счетчик выполненных запусков
};

#endif // RUNACTION_H
