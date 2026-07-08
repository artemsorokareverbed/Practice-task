cmake_minimum_required(VERSION 3.16)

project(Practice-Task-v1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Practice-Task-v1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        bus_logic.h
        res.qrc
    )
else()
    if(ANDROID)
        add_library(Practice-Task-v1 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Practice-Task-v1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Practice-Task-v1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Practice-Task-v1)
endif()
set_target_properties(Practice-Task-v1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Practice-Task-v1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Practice-Task-v1)
endif()

add_executable(bus_test_run
    bus_test.cpp
)

target_link_libraries(bus_test_run PRIVATE Qt${QT_VERSION_MAJOR}::Test)