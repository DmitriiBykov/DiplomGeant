#include "DetectorConstruction.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // 1) Создаём материалы
    // Воздух (или вакуум), алюминий и т.д.
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* airMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* AlMat  = nist->FindOrBuildMaterial("G4_Al");
    G4Material* SiMat  = nist->FindOrBuildMaterial("G4_Si");
    G4Material* CuMat  = nist->FindOrBuildMaterial("G4_Cu");
    G4Material* FeMat = nist->FindOrBuildMaterial("G4_Fe");

    // 2) Размеры
    //    Мировой объём 1 м^3 => полуразмер 0.5 м = 500 мм
    G4double worldHalfSize = 500.*mm;

    // 3) Создаём shape для мира
    G4Box* solidWorld =
        new G4Box("World",        // имя
                  worldHalfSize,  // полуразмер x
                  worldHalfSize,  // полуразмер y
                  worldHalfSize); // полуразмер z

    // 4) Логический объём мира
    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,    // solid
                            airMat,        // материал
                            "World");      // имя

    // 5) Физический объём мира (укореняем в 0)
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(nullptr,           // без трансформа
                          G4ThreeVector(),   // в (0,0,0)
                          logicWorld,        // лог.объём
                          "World",           // имя
                          nullptr,           // нет родителя
                          false,
                          0,
                          true);

    // 6) Алюминиевая пластина толщиной 2 см (20 мм)
    //    Пусть она будет по центру (Z=0) и занимать всю площадь по X-Y  (немного меньше мира)
    // G4double plateThickness = 10.*mm;
    // G4Box* solidPlate =
    //     new G4Box("AlPlate",
    //               25.*mm,     // полуразмер X (условно 20 см)
    //               25.*mm,     // полуразмер Y (условно 20 см)
    //               plateThickness/2.); // полуразмер Z = 10 мм => итого 20 мм

    // G4LogicalVolume* logicPlate =
    //     new G4LogicalVolume(solidPlate,
    //                         AlMat,
    //                         "AlPlate");
    // G4RotationMatrix* rot = new G4RotationMatrix();
    // rot->rotateY(0.*deg);
    // new G4PVPlacement(rot,
    //                   G4ThreeVector(0,0,250.*mm), // по центру
    //                   logicPlate,
    //                   "AlPlate",
    //                   logicWorld,
    //                   false,
    //                   0,
    //                   true);


    // // Размеры решётки и проволоки
    // // Размеры решётки и проволоки:
    // G4double gridSizeXY   = 50.*mm;      // Размер решётки по X и Y (50 мм)
    // G4double wireDiameter = 0.2*mm;       // Диаметр проволоки (0.2 мм)
    // G4double gridThickness = wireDiameter; // Толщина решётки (можно взять равной диаметру)

    // // Создаем материнский объём для решётки (GridPanel) как сплошной параллелепипед
    // G4Box* solidGrid = new G4Box("GridPanel",
    //                              gridSizeXY/2., gridSizeXY/2., gridThickness/2.);
    // G4LogicalVolume* logicGrid = new G4LogicalVolume(solidGrid, airMat, "GridPanel");

    // // Определяем шаг размещения проводов
    // G4double pitch = 0.5*mm;
    // // Число проводов (от -gridSizeXY/2 до +gridSizeXY/2 с шагом pitch)
    // G4int nWires = static_cast<G4int>(gridSizeXY/pitch) + 1;  // 50/1 + 1 = 51

    // // --- Горизонтальные проволоки ---
    // // Проволока должна быть длинной по оси X (50 мм) и иметь круглое сечение с диаметром wireDiameter.
    // // По умолчанию G4Tubs создаёт цилиндр с осью вдоль Z, поэтому для ориентации вдоль X применим поворот.
    // G4Tubs* solidHorizWire = new G4Tubs("HorizWire",
    //                                     0,                    // внутренний радиус (отверстие)
    //                                     wireDiameter/2.,      // внешний радиус
    //                                     gridSizeXY/2.,        // полудлина цилиндра (50 мм/2 = 25 мм)
    //                                     0.*deg, 360.*deg);    // полный угол
    // G4LogicalVolume* logicHorizWire = new G4LogicalVolume(solidHorizWire, AlMat, "HorizWire");

    // // Создаем поворот, чтобы цилиндр с осью Z повернуть так, чтобы ось стала вдоль X:
    // G4RotationMatrix* rotHoriz = new G4RotationMatrix();
    // rotHoriz->rotateY(-90.*deg); // поворот на -90° вокруг оси Y

    // // Размещаем горизонтальные проволоки вдоль оси Y:
    // for (G4int i = 0; i < nWires; i++) {
    //     // y изменяется от -gridSizeXY/2 до +gridSizeXY/2 с шагом pitch
    //     G4double yPos = -gridSizeXY/2. + i*pitch;
    //     // По X и Z размещаем в центре решётки
    //     G4ThreeVector pos(0, yPos, 0);
    //     new G4PVPlacement(rotHoriz, pos, logicHorizWire, "HorizWire", logicGrid, false, i, true);
    // }

    // // --- Вертикальные проволоки ---
    // // Создаем цилиндр с помощью G4Tubs, затем поворачиваем так, чтобы ось стала вдоль Y.
    // G4Tubs* solidVertWire = new G4Tubs("VertWire",
    //                                    0,
    //                                    wireDiameter/2.,
    //                                    gridSizeXY/2.,
    //                                    0.*deg, 360.*deg);
    // G4LogicalVolume* logicVertWire = new G4LogicalVolume(solidVertWire, AlMat, "VertWire");

    // // Для ориентации вдоль Y повернем цилиндр (по умолчанию ось Z) на 90° вокруг оси X:
    // G4RotationMatrix* rotVert = new G4RotationMatrix();
    // rotVert->rotateX(90.*deg);

    // for (G4int i = 0; i < nWires; i++) {
    //     G4double xPos = -gridSizeXY/2. + i*pitch;
    //     G4ThreeVector pos(xPos, 0, 0);
    //     new G4PVPlacement(rotVert, pos, logicVertWire, "VertWire", logicGrid, false, i, true);
    // }

    // // Размещаем решётку в мире:
    // G4RotationMatrix* rotGrid = new G4RotationMatrix();
    // new G4PVPlacement(rotGrid,
    //                   G4ThreeVector(0,0,50.*mm),
    //                   logicGrid,
    //                   "GridPanel",
    //                   logicWorld,
    //                   false,
    //                   0,
    //                   true);


    G4double stepWidthX = 1.0*cm;   // полная ширина по X
    G4double stepHeightY = 10.0*cm;  // полная высота по Y
    // Для удобного "разноса" вдоль X — сделаем небольшой зазор между брусками
    G4double gapX = 0.0*cm; // 0 мм зазор
    // Начальная координата X (слева направо); можно выбрать так, чтобы бруски
    // оказались примерно по центру (X=0). Ниже делаем "сдвиг" относительно нуля.
    G4double startX = -4.5*cm;  // пример: сдвинем всё левее (можно подбирать)

    for(G4int i = 1; i <= 10; i++)
    {
        // Толщина бруска вдоль Z = i см
        G4double fullThicknessZ = i*mm;
        G4double halfThicknessZ = 0.5*fullThicknessZ;

        // Полуразмеры по X и Y
        G4double halfX = 0.5*stepWidthX;
        G4double halfY = 0.5*stepHeightY;

        // Создаём твёрдое тело (G4Box) — имя можно просто "StepBox"
        G4Box* solidStep = new G4Box("StepBox",
                                     halfX,  // полширина (X)
                                     halfY,  // полвысота (Y)
                                     halfThicknessZ); // полтолщина (Z)

        // Логический объём из меди
        G4LogicalVolume* logicStep =
            new G4LogicalVolume(solidStep,
                                CuMat,
                                "Step");

        // Вычисляем позицию по оси X, чтобы каждый брусок не перекрывался
        // Пусть каждую новую «ступень» двигаем на (ширина + зазор)
        // i-я ступень: xPos = startX + (i-1)*( stepWidthX + gapX )
        G4double xPos = startX + (i-1)*(stepWidthX + gapX);

        // По Z хотим, чтобы "передняя" грань бруска была в плоскости z=0,
        // значит центр бруска сместим на половину толщины вдоль Z:
        G4double zPos = 10-halfThicknessZ+27.5*cm;

        // По Y (вертикали) пусть все будут на уровне 0 (центр в y=0).
        G4double yPos = 0.0;

        // Размещаем
        new G4PVPlacement(nullptr,
                          G4ThreeVector(xPos, yPos, zPos),
                          logicStep,
                          "Step",
                          logicWorld,
                          false,
                          i,
                          true);

        // Зададим визуальные атрибуты (цвет, стиль)
        G4VisAttributes* copperColor =
            new G4VisAttributes(G4Colour(0.9, 0.45, 0.0)); // медный оттенок
        copperColor->SetForceSolid(true);
        logicStep->SetVisAttributes(copperColor);
    }



    // Проволка цилиндр длиной 10 см, диаметром 1 см (радиус=0.5 см),
    // ось вдоль X. Полудлина = 5 см.
    G4double wireRadius       = 1.*mm;
    G4double wireHalfLengthZ  = 5.0*cm;
    G4Tubs* solidWire = new G4Tubs("Wire",
                                   0.0,           // внутренний радиус
                                   wireRadius,    // внешний радиус
                                   wireHalfLengthZ,
                                   0.*deg, 360.*deg);

    // Логический объём из железа
    G4LogicalVolume* logicWire =
        new G4LogicalVolume(solidWire, FeMat, "Wire");

    // Разместим проволоку за ступеньками, например, при z=20 см (центр проволоки).

    G4double wirePosX = 0.0;   // по оси X — в центре
    G4double wirePosY = 0.0;   // по оси Y — в центре
    G4double wirePosZ = 29.*cm;  // за ступеньками (те заканчиваются ~ z=24 см)
    G4RotationMatrix* rotateWire = new G4RotationMatrix();
    rotateWire->rotateY(90.*deg);
    new G4PVPlacement(rotateWire,
                      G4ThreeVector(wirePosX, wirePosY, wirePosZ),
                      logicWire,
                      "Wire",
                      logicWorld,
                      false,
                      0,
                      true);

    // Визуальные атрибуты для проволоки
    G4VisAttributes* ironColor =
        new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // Серый цвет (железо)
    ironColor->SetForceSolid(true);
    logicWire->SetVisAttributes(ironColor);


    // 7) Детекторная «панель» — один логический объём.
    //    Будем считать, что это прямоугольник 20 см × 20 см, толщина ~0.5 мм
    //    Расположим на z= +300 мм внутри мира.
    G4double detSizeXY = 200.*mm;
    G4double detThickness = 20.*mm;

    G4Box* solidDetector =
        new G4Box("Detector",
                  detSizeXY/2.,
                  detSizeXY/2.,
                  detThickness/2.);

    G4LogicalVolume* logicDetector =
        new G4LogicalVolume(solidDetector,
                            SiMat,  // или какой-то другой материал
                            "Detector");

    // Разместим детектор на z= +300 мм
    G4double detPosZ = 300.*mm;
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0,0,detPosZ),
                      logicDetector,
                      "Detector",
                      logicWorld,
                      false,
                      0,
                      true);

    // Настраиваем визуальные атрибуты (не обязательно)
    G4VisAttributes* plateColor = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // серый
    logicWorld->SetVisAttributes(plateColor);

    // logicVertWire ->SetVisAttributes(plateColor);
    // logicHorizWire ->SetVisAttributes(plateColor);




    G4VisAttributes* detColor = new G4VisAttributes(G4Colour(0.0,1.0,0.0)); // зелёный
    detColor->SetForceWireframe(true);
    logicDetector->SetVisAttributes(detColor);

    return physWorld;
}
