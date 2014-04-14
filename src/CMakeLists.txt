find_package(KDE4 REQUIRED)
find_package(Qt4 COMPONENTS QtGui QtCore QtXml REQUIRED)
find_package(LibXslt REQUIRED)

include(${QT_USE_FILE})

include_directories( ${KDE4_INCLUDES}  ${QT_INCLUDES} )

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


QT4_ADD_RESOURCES(kvkbd_RESOURCES_RCC ${kvkbd_RESOURCES})

QT4_ADD_DBUS_ADAPTOR(kvkbd_SRCS org.kde.kvkbd.Kvkbd.xml
                       mainwidget.h MainWidget)

QT4_ADD_DBUS_ADAPTOR(kvkbd_SRCS org.kde.kvkbd.Dock.xml
                       kbddock.h KbdDock)

kde4_add_executable(kvkbd ${kvkbd_SRCS} ${kvkbd_RC_SRCS} ${kvkbd_RESOURCES_RCC})

target_link_libraries(kvkbd ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_LIBRARIES} X11 Xtst)


install(TARGETS kvkbd ${INSTALL_TARGETS_DEFAULT_ARGS})

install( FILES kvkbd.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
