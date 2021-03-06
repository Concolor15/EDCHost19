# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT += core gui quickcontrols2 qml quick  multimedia serialport
TARGET = EDCHost19
TEMPLATE = app

CONFIG += c++14
CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h

CONFIG += qtquickcompiler

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS = \
    config.h \
    controller.h \
    CoordinateConverter.h \
    GlobalType.h \
    stdafx.h \
    globalconfig.h \
    MyCamera.h \
    imgproc.h \
    logic.h

SOURCES = \
    controller.cpp \
    CoordinateConverter.cpp \
    main.cpp \
    globalconfig.cpp \
    MyCamera.cpp \
    imgproc.cpp \
    logic.cpp

FORMS =


#QMAKE_LIBDIR += /usr/lib/x86_64-linux-gnu
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \



#DEFINES = 

RESOURCES += \
    EDCHost19.qrc


