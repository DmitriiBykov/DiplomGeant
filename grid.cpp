#include "grid.h"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

Grid::Grid(G4Material* AlMat)
{
    // Размеры решетки: активная область 50 mm x 50 mm.
    G4double gridSizeXY = 50.*mm;
    // Диаметр проволоки = 0.2 mm
    G4double wireDiameter = 0.2*mm;
    // Толщина решетки (по Z) примем равной диаметру проволоки
    G4double gridThickness = wireDiameter;

    // Создаем материнский объем решетки (GridPanel)
    G4Box* solidGrid = new G4Box("GridPanel",
                                 gridSizeXY/2.0,
                                 gridSizeXY/2.0,
                                 gridThickness/2.0);

    fLogicGrid = new G4LogicalVolume(solidGrid, AlMat, "GridPanel");

    // Зададим атрибуты визуализации для лучшего просмотра (не обязательно)
    fLogicGrid->SetVisAttributes(new G4VisAttributes(G4Colour(0.8,0.8,0.8)));

    // Параметры сетки:
    G4double pitch = 1.0*mm; // шаг по центру проводов
    // Число проводов: чтобы покрыть 50 mm, можно взять 51 провод
    G4int nWires = 51;

    // Создадим горизонтальные проволоки (идут вдоль оси X):
    // Они имеют длину по X равную 50 mm, высоту (Y) и толщину (Z) равные wireDiameter.
    G4Box* solidHorizWire = new G4Box("HorizWire",
                                      gridSizeXY/2.0,   // X: полудлина = 25 mm
                                      wireDiameter/2.0, // Y: полуширина проволоки
                                      wireDiameter/2.0);// Z: полутолщина
    G4LogicalVolume* logicHorizWire = new G4LogicalVolume(solidHorizWire, AlMat, "HorizWire");
    // Установим визуальные атрибуты (например, красный)
    logicHorizWire->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,0)));

    // Размещаем горизонтальные проволоки вдоль оси Y в материнском объеме:
    for (G4int i = 0; i < nWires; i++) {
        // Располагаем так, чтобы крайние провода совпадали с границами решетки:
        // x=0 (средняя линия), y = -gridSizeXY/2 + i*pitch
        G4double yPos = -gridSizeXY/2.0 + i*pitch;
        G4ThreeVector pos(0, yPos, 0);
        new G4PVPlacement(0, pos, logicHorizWire, "HorizWire", fLogicGrid, false, i, true);
    }

    // Создадим вертикальные проволоки (идут вдоль оси Y):
    // Они имеют длину по Y равную 50 mm, ширину (X) и толщину (Z) равные wireDiameter.
    G4Box* solidVertWire = new G4Box("VertWire",
                                     wireDiameter/2.0, // X: полуширина проволоки
                                     gridSizeXY/2.0,   // Y: полудлина = 25 mm
                                     wireDiameter/2.0);// Z: полутолщина
    G4LogicalVolume* logicVertWire = new G4LogicalVolume(solidVertWire, AlMat, "VertWire");
    // Визуальные атрибуты (например, синий)
    logicVertWire->SetVisAttributes(new G4VisAttributes(G4Colour(0,0,1)));

    // Размещаем вертикальные проволоки вдоль оси X:
    for (G4int i = 0; i < nWires; i++) {
        G4double xPos = -gridSizeXY/2.0 + i*pitch;
        G4ThreeVector pos(xPos, 0, 0);
        new G4PVPlacement(0, pos, logicVertWire, "VertWire", fLogicGrid, false, i, true);
    }
}

Grid::~Grid()
{
}
