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


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/v4l2devicereader.cpp \
	src/v4l2devicecontroler.cpp \
    libv4l2cpp/src/V4l2Capture.cpp \
    libv4l2cpp/src/V4l2Output.cpp \
    libv4l2cpp/src/V4l2Device.cpp \
    libv4l2cpp/src/V4l2MmapDevice.cpp 

HEADERS  += inc/mainwindow.h \
        inc/v4l2devicereader.h \
	inc/v4l2devicecontroler.h \
    libv4l2cpp/inc/logger.h \
    libv4l2cpp/inc/V4l2Capture.h \
    libv4l2cpp/inc/V4l2Output.h \
    libv4l2cpp/inc/V4l2Device.h \
    libv4l2cpp/inc/V4l2MmapDevice.h \

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/libv4l2cpp/inc $$PWD/inc
LIBS  += -llog4cpp -ljpeg
