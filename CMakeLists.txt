cmake_minimum_required(VERSION 2.8.12)

project(kvkbd)

set(REQUIRED_QT_VERSION 5.9.0)
set(KF5_MIN_VERSION 5.40.0)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Gui
    Core
    Widgets
    Xml
    X11Extras
    DBus)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    I18n
    Config
    CoreAddons
    IconThemes
    XmlGui
    WindowSystem
    DocTools)

find_package(LibXslt REQUIRED)

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( colors )
add_subdirectory( themes )
