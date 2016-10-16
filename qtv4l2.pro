#-------------------------------------------------
#
# Project created by QtCreator 2016-07-30T17:36:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$basename(_PRO_FILE_PWD_)
OBJECTS_DIR = $$PWD/obj
MOC_DIR =     $$PWD/moc
RCC_DIR =     $$PWD/rcc
UI_DIR =      $$PWD/gui

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        v4l2devicereader.cpp \
    libv4l2cpp/src/V4l2Capture.cpp \
    libv4l2cpp/src/V4l2Output.cpp \
    libv4l2cpp/src/V4l2Device.cpp \
    libv4l2cpp/src/V4l2MmapDevice.cpp \
    v4l2devicecontroler.cpp

HEADERS  += mainwindow.h \
            v4l2devicereader.h \
    libv4l2cpp/inc/logger.h \
    libv4l2cpp/inc/V4l2Capture.h \
    libv4l2cpp/inc/V4l2Output.h \
    libv4l2cpp/inc/V4l2Device.h \
    libv4l2cpp/inc/V4l2MmapDevice.h \
    v4l2devicecontroler.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/libv4l2cpp/inc
LIBS  += -llog4cpp -ljpeg
