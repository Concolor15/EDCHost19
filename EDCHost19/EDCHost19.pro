# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT += core gui widgets qml quick multimedia serialport
TARGET = EDCHost19
TEMPLATE = app

CONFIG += c++14
CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS = \
    Camera.h \
    config.h \
    controller.h \
    CoordinateConverter.h \
    EDCHost19.h \
    GlobalType.h \
    HighResCam.h \
    MainLogic.h \
    MatchMain.h \
    MyLogger.h \
    Perspective.h \
    stdafx.h \
    globalconfig.h \
    MyCamera.h

SOURCES = \
    Camera.cpp \
    controller.cpp \
    CoordinateConverter.cpp \
    EDCHost19.cpp \
    HighResCam.cpp \
    main.cpp \
    MainLogic.cpp \
    MatchMain.cpp \
    MyLogger.cpp \
    Perspective.cpp \
    stdafx.cpp \
    globalconfig.cpp \
    MyCamera.cpp

FORMS = \
    EDCHost19.ui \
    MatchMain.ui \
    MyLogger.ui \
    Perspective.ui


QMAKE_LIBDIR += /usr/lib/x86_64-linux-gnu
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \



#DEFINES = 

RESOURCES += \
    EDCHost19.qrc


