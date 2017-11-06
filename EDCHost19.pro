# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT += core gui widgets multimedia serialport
TARGET = EDCHost19
TEMPLATE = app

CONFIG += precompile_header
PRECOMPILED_HEADER = EDCHost19/stdafx.h

HEADERS = \
    EDCHost19/Camera.h \
    EDCHost19/CoordinateConverter.h \
    EDCHost19/EDCHost19.h \
    EDCHost19/GlobalType.h \
    EDCHost19/HighResCam.h \
    EDCHost19/MainLogic.h \
    EDCHost19/MatchMain.h \
    EDCHost19/MyLogger.h \
    EDCHost19/Perspective.h \
    #EDCHost19/Serial.h \
    EDCHost19/stdafx.h \
    EDCHost19/config.h \
    EDCHost19/controller.h

SOURCES = \
    EDCHost19/Camera.cpp \
    EDCHost19/CoordinateConverter.cpp \
    EDCHost19/EDCHost19.cpp \
    EDCHost19/HighResCam.cpp \
    EDCHost19/main.cpp \
    EDCHost19/MainLogic.cpp \
    EDCHost19/MatchMain.cpp \
    EDCHost19/MyLogger.cpp \
    EDCHost19/Perspective.cpp \
    #EDCHost19/Serial.cpp \
    EDCHost19/controller.cpp
   #$$PWD/EDCHost19/stdafx.cpp

FORMS = \
    EDCHost19/EDCHost19.ui \
    EDCHost19/MyLogger.ui \
    EDCHost19/Perspective.ui \
    EDCHost19/MatchMain.ui \
    EDCHost19/EDCHost19.ui

RESOURCES = EDCHost19/EDCHost19.qrc

QMAKE_LIBDIR += /usr/lib/x86_64-linux-gnu
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \

INCLUDEPATH = \
    $$PWD/EDCHost19

#DEFINES = 

