TEMPLATE = app
TARGET = proslideshower

greaterThan(QT_VERSION, 5.0.0) {
    QT += widgets
}

SOURCES += \
    main.cpp \
    projdisplay.cpp \
    presmodel.cpp \
    projcontroller.cpp \
    projmanager.cpp \
    presstyle.cpp \
    tests/desktopsimulatorwidget.cpp \
    subdisplayhandler.cpp \
    projcontrollerpane.cpp

HEADERS += \
    projdisplay.h \
    presmodel.h \
    projcontroller.h \
    projmanager.h \
    presstyle.h \
    tests/desktopsimulatorwidget.h \
    subdisplayhandler.h \
    projcontrollerpane.h

SIMULATING_DESKTOPS=yes
H_DESKTOPS=2
V_DESKTOPS=1

!isEmpty(SIMULATING_DESKTOPS) {
    DEFINES+="SIMULATING_DESKTOPS"
    DEFINES+="SIMULATING_H_DESKTOPS=$$H_DESKTOPS"
    DEFINES+="SIMULATING_V_DESKTOPS=$$V_DESKTOPS"
}

# Link against the right version of libPoppler
win32 {
    INCLUDEPATH += C:/Dev/Poppler/include
    LIBS += -LC:/Dev/Poppler/lib
}
greaterThan(QT_VERSION, 5.0.0) {
    LIBS += -lpoppler-qt5
} else {
    greaterThan(QT_VERSION, 4.0.0) {
        LIBS += -lpoppler-qt4
    } else {
        warning("Qt version should be at least 4.0.0")
    }
}

# The directory where to put MOC-generated files :
MOC_DIR = ./.moc

# Changing output directories :
win32 {
    OBJECTS_DIR = ./.obj_win
    CONFIG(debug, debug|release):DESTDIR = ./debug
    else:DESTDIR = ./release
}
unix {
    OBJECTS_DIR = ./.obj_unix
    CONFIG(debug, debug|release):DESTDIR = ./debug
    else:DESTDIR = ./release
}

RESOURCES += \
    proslideshower.qrc

