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

add_library(qt-framless-window STATIC
    ${LIBRARY_SOURCES}
    qt-framless-window.qrc
)

target_link_libraries(qt-framless-window PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(SAMPLE_SOURCES
    main.cpp
    samplewindow.h samplewindow.cpp
    samplewindow2.h samplewindow2.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-framless-window-sample
        MANUAL_FINALIZATION
        ${SAMPLE_SOURCES}
        qt-frameless-window-sample.qrc
    )
else()
    add_executable(qt-framless-window-sample
        ${SAMPLE_SOURCES}
    )
endif()

# Release 模式下也输出文件、函数
add_compile_definitions(QT_MESSAGELOGCONTEXT)
add_subdirectory(submodules/qt-easy-logger)
target_include_directories(qt-framless-window-sample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/submodules")
target_link_libraries(qt-framless-window-sample PRIVATE qt-easy-logger)
target_link_libraries(qt-framless-window-sample PRIVATE qt-framless-window)
target_link_libraries(qt-framless-window-sample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-framless-window-sample)
endif()
