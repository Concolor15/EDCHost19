# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT += core gui quickcontrols2 qml quick  multimedia serialport
TARGET = EDCHost19
TEMPLATE = app

CONFIG += c++17
CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h

CONFIG += qtquickcompiler

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS = \
    controller.h \
    CoordinateConverter.h \
    stdafx.h \
    globalconfig.h \
    imgproc.h \
    logic.h \
    locate.h \
    camera.h \
    util.h

SOURCES = \
    controller.cpp \
    CoordinateConverter.cpp \
    main.cpp \
    globalconfig.cpp \
    imgproc.cpp \
    logic.cpp \
    locate.cpp \
    camera.cpp

FORMS =

linux {
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
}


#DEFINES = 

RESOURCES += \
    EDCHost19.qrc

win32 {
    INCLUDEPATH += E:\Library\opencv-3.3.0-official\build\include
    INCLUDEPATH += E:\Library\Eigen
    QMAKE_LIBDIR += E:\Library\opencv-3.3.0-official\build\x64\vc14\lib
    release: LIBS += opencv_world330.lib
    debug: LIBS += opencv_world330d.lib
}


