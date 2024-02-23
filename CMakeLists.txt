cmake_minimum_required(VERSION 3.5)

project(qt-framless-window VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(LIBRARY_SOURCES
    framelesswindow.cpp
    framelesswindow.h
)

add_library(qt-frameless-window STATIC
    ${LIBRARY_SOURCES}
    qt-frameless-window.qrc
)

target_link_libraries(qt-frameless-window PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
