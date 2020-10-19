
set(kvkbd_SRCS vkeyboard.cpp
    x11keyboard.cpp
    dragwidget.cpp
    mainwidget.cpp
    vbutton.cpp
    main.cpp
    resizabledragwidget.cpp
    keysymconvert.cpp
    kbddock.cpp
    kvkbdapp.cpp
    kbdtray.cpp
    themeloader.cpp
)

SET(kvkbd_RESOURCES resources.qrc)

qt5_add_resources(kvkbd_RESOURCES_RCC ${kvkbd_RESOURCES})

qt5_add_dbus_adaptor(kvkbd_SRCS org.kde.kvkbd.Kvkbd.xml
                       mainwidget.h MainWidget)

qt5_add_dbus_adaptor(kvkbd_SRCS org.kde.kvkbd.Dock.xml
                       kbddock.h KbdDock)

add_executable(kvkbd ${kvkbd_SRCS} ${kvkbd_RC_SRCS} ${kvkbd_RESOURCES_RCC})

target_link_libraries(kvkbd Qt5::Core Qt5::Gui Qt5::Xml Qt5::Widgets Qt5::DBus Qt5::X11Extras
                      KF5::ConfigCore KF5::CoreAddons KF5::I18n KF5::IconThemes KF5::XmlGui KF5::WindowSystem X11 Xtst)

install(TARGETS kvkbd ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES kvkbd.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR})
