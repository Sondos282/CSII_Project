cmake_minimum_required(VERSION 3.5)

project(CSII-Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CSII-Project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        main.h main.cpp
        citizen.h citizen.cpp
        fence.h fence.cpp
        menu.h menu.cpp menu.ui
        level.h level.cpp
        bullet.h bullet.cpp
        defense.h defense.cpp
        winning.h winning.cpp winning.ui
        settings.h settings.cpp settings.ui
        enemy.h enemy.cpp
        castle.h castle.cpp
        gameover.h gameover.cpp gameover.ui
        gem.h gem.cpp
        powerup.h powerup.cpp
        userguide.h userguide.cpp userguide.ui
        graph.h graph.cpp
        node.h node.cpp
        healthmarkers.h healthmarkers.cpp
        pausewindow.h pausewindow.cpp pausewindow.ui
        largerenemy.h largerenemy.cpp










    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CSII-Project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CSII-Project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CSII-Project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CSII-Project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CSII-Project)
endif()
set_target_properties(CSII-Project PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CSII-Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CSII-Project)
endif()
