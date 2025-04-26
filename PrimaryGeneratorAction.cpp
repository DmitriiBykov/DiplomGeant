#include "PrimaryGeneratorAction.h"

#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    // Создаём Gun с 1 частицей "за раз" (но мы вручную будем вызывать GeneratePrimaryVertex 1000 раз на событие)
    fParticleGun = new G4ParticleGun(1E6);

    // Устанавливаем тип частицы: гамма
    fParticleGun->SetParticleDefinition(G4Gamma::GammaDefinition());

    // Энергия фотона  кэВ
    fParticleGun->SetParticleEnergy(150.*keV);

    // Исходная позиция (например, z = -400 мм)
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.*mm));

    // По умолчанию пусть летит вдоль +Z
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // За 1 event выпускаем n частиц
    G4long nParticles = 3*1E6;
    G4double diametr = 0.2*mm; //диаметр фокусного пятна
    G4double diametrX = 0.1*mm;
    G4double diametrY = 0.5*mm;
    for (G4int i = 0; i < nParticles; i++)
    {
        G4double x = diametrX*G4UniformRand()-diametrX/2; //Для простого фокусного пятна
        G4double y = diametrY*G4UniformRand()-diametrY/2;
        // стандартное отклонение sigma = diametr / 2.355
        // G4double sigma = (diametr*mm) / 2.355;
        // // Генерируем координаты x и y из нормального распределения с mean = 0 и sigma
        // G4double x = G4RandGauss::shoot((diametr*mm)/2, sigma)-(diametr*mm)/2;
        // G4double y = G4RandGauss::shoot((diametr*mm)/2, sigma)-(diametr*mm)/2;
        // Генерируем случайное направление внутри конуса 30 градусов вокруг +Z
        // Конус: theta в [0, 30°], phi в [0, 360°]
        G4double thetaMax = 30.*deg;
        // cos(theta) в [cos(30°), 1]
        G4double cosThetaMin = std::cos(thetaMax);

        // Выбираем равномерно cosTheta в [cosThetaMin, 1]
        G4double randCosTheta = G4UniformRand()*(1. - cosThetaMin) + cosThetaMin;
        G4double sinTheta     = std::sqrt(1. - randCosTheta*randCosTheta);

        // phi в [0, 2π)
        G4double phi = 2.*CLHEP::pi*G4UniformRand();

        // Направление
        G4double xDir = sinTheta*std::cos(phi);
        G4double yDir = sinTheta*std::sin(phi);
        G4double zDir = randCosTheta;
        fParticleGun->SetParticlePosition(G4ThreeVector(x,y,0));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xDir, yDir, zDir));

        // Генерируем эту одну частицу
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
