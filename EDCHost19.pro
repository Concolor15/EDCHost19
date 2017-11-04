# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT += core gui widgets multimedia serialport
TARGET = EDCHost19
TEMPLATE = app

PRECOMPILED_HEADER = EDCHost19/stdafx.h

HEADERS = \
   $$PWD/EDCHost19/Camera.h \
   $$PWD/EDCHost19/CoordinateConverter.h \
   $$PWD/EDCHost19/EDCHost19.h \
   $$PWD/EDCHost19/GlobalType.h \
   $$PWD/EDCHost19/HighResCam.h \
   $$PWD/EDCHost19/MainLogic.h \
   $$PWD/EDCHost19/MatchMain.h \
   $$PWD/EDCHost19/MyLogger.h \
   $$PWD/EDCHost19/Perspective.h \
   $$PWD/EDCHost19/Serial.h \
   $$PWD/EDCHost19/stdafx.h \
    EDCHost19/config.h

SOURCES = \
   $$PWD/EDCHost19/Camera.cpp \
   $$PWD/EDCHost19/CoordinateConverter.cpp \
   $$PWD/EDCHost19/EDCHost19.cpp \
   $$PWD/EDCHost19/HighResCam.cpp \
   $$PWD/EDCHost19/main.cpp \
   $$PWD/EDCHost19/MainLogic.cpp \
   $$PWD/EDCHost19/MatchMain.cpp \
   $$PWD/EDCHost19/MyLogger.cpp \
   $$PWD/EDCHost19/Perspective.cpp \
   $$PWD/EDCHost19/Serial.cpp
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

