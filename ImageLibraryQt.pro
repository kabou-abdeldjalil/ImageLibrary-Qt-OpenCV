QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ImageDescriptor.cpp \
    ImageLibrary.cpp \
    User.cpp \
    adminwindow.cpp \
    librarywindow.cpp \
    main.cpp \
    mainwindow.cpp \
    userwindow.cpp

HEADERS += \
    ImageDescriptor.h \
    ImageLibrary.h \
    User.h \
    adminwindow.h \
    librarywindow.h \
    mainwindow.h \
    userwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    ImageLibraryQt_fr_FR.ts

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

