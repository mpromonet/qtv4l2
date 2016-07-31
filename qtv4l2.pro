#-------------------------------------------------
#
# Project created by QtCreator 2016-07-30T17:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtv4l2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        v4l2devicereader.cpp \
    libv4l2cpp/src/V4l2Capture.cpp \
    libv4l2cpp/src/V4l2Device.cpp \
    libv4l2cpp/src/V4l2MmapCapture.cpp \
    libv4l2cpp/src/V4l2MmapDevice.cpp \
    libv4l2cpp/src/V4l2MmapOutput.cpp \
    libv4l2cpp/src/V4l2Output.cpp \
    libv4l2cpp/src/V4l2ReadCapture.cpp \  
    v4l2devicecontroler.cpp

HEADERS  += mainwindow.h \
            v4l2devicereader.h \
    libv4l2cpp/inc/logger.h \
    libv4l2cpp/inc/V4l2Capture.h \
    libv4l2cpp/inc/V4l2Device.h \
    libv4l2cpp/inc/V4l2MmapCapture.h \
    libv4l2cpp/inc/V4l2MmapDevice.h \
    libv4l2cpp/inc/V4l2MmapOutput.h \
    libv4l2cpp/inc/V4l2Output.h \
    libv4l2cpp/inc/V4l2ReadCapture.h \  
    v4l2devicecontroler.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/libv4l2cpp/inc
LIBS  += -llog4cpp -ljpeg
