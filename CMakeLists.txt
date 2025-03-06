cmake_minimum_required(VERSION 3.16)

project(untitled6 LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) # Включаем автоматическую обработку moc
set(CMAKE_AUTORCC ON) # (если используются ресурсы Qt)
#set(CMAKE_AUTOUIC ON) # (если используются файлы .ui)


# Находим Geant4
find_package(Geant4 REQUIRED ui_all vis_all)

# Включаем файлы Geant4
include(${Geant4_USE_FILE})

# Указываем файлы проекта
set(SOURCES
    main.cpp
    DetectorConstruction.cpp
ActionInitialization.cpp
PrimaryGeneratorAction.cpp
Run.cpp
RunAction.cpp
EventAction.cpp
SteppingAction.cpp
)

set(HEADERS
    DetectorConstruction.h
    ActionInitialization.h
    PrimaryGeneratorAction.h
    Run.h
    RunAction.h
    EventAction.h
    SteppingAction.h
)

# Подключаем директории Geant4
include_directories(${Geant4_INCLUDE_DIRS})

# Создаём исполняемый файл
add_executable(untitled6 ${SOURCES} ${HEADERS}

    DetectorConstruction.h DetectorConstruction.cpp
    ActionInitialization.h ActionInitialization.cpp
    PrimaryGeneratorAction.h PrimaryGeneratorAction.cpp
    Run.h Run.cpp
    RunAction.h RunAction.cpp
    EventAction.h EventAction.cpp
    SteppingAction.h SteppingAction.cpp


)

# Подключаем библиотеки Geant4
target_link_libraries(untitled6 ${Geant4_LIBRARIES})

# Указываем место установки
install(TARGETS untitled6
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
