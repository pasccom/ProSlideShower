TEMPLATE = app
TARGET = proslideshower

greaterThan(QT_VERSION, 5.0.0) {
    QT += widgets
}

SOURCES +=                  \
    main.cpp                \
    projdisplay.cpp         \
    presmodel.cpp           \
    projcontroller.cpp      \
    projmanager.cpp         \
    presstyle.cpp           \
    subdisplayhandler.cpp   \
    projcontrollerpane.cpp  \
    galleryview.cpp         \

HEADERS +=                  \
    projdisplay.h           \
    presmodel.h             \
    projcontroller.h        \
    projmanager.h           \
    presstyle.h             \
    subdisplayhandler.h     \
    projcontrollerpane.h    \
    galleryview.h           \
    previewmodel.h          \

RESOURCES +=                \
    proslideshower.qrc      \

TRANSLATIONS +=             \
    proslideshower_en.ts    \
    proslideshower_fr.ts    \

exists(ProSlideShower.local.pri) {
    include(ProSlideShower.local.pri)
}

equals(SIMULATING_DESKTOPS, "yes") {
    message("Building with simulated desktops ($${V_DESKTOPS}x$${H_DESKTOPS})")
    SOURCES += tests/desktopsimulatorwidget.cpp
    HEADERS += tests/desktopsimulatorwidget.h
    DEFINES+="SIMULATING_DESKTOPS"
    DEFINES+="SIMULATING_H_DESKTOPS=$$H_DESKTOPS"
    DEFINES+="SIMULATING_V_DESKTOPS=$$V_DESKTOPS"
}

# Link against the right version of libPoppler
!isEmpty(LIB_POPPLER_INCLUDE_PATH): INCLUDEPATH += "$$LIB_POPPLER_INCLUDE_PATH"
!isEmpty(LIB_POPPLER_LIBRARY_PATH): LIBS += "-L$$LIB_POPPLER_LIBRARY_PATH"

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

# lupdate target :
isEmpty(QMAKE_LUPDATE) {
    win32:QMAKE_LUPDATE = $$[QT_INSTALL_BINS]\lupdate.exe
    else:QMAKE_LUPDATE = $$[QT_INSTALL_BINS]/lupdate
}

lupdate.depends += $$SOURCES
lupdate.depends += $$TRANSLATIONS
lupdate.commands = $$QMAKE_LUPDATE ProSlideShower.pro
QMAKE_EXTRA_TARGETS += lupdate

# lrelease target :
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

lrelease.input = TRANSLATIONS
lrelease.output = ${QMAKE_FILE_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += lrelease
POST_TARGETDEPS += compiler_lrelease_make_all
