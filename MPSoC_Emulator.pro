#-------------------------------------------------
#
# Project created by Caio Alexandre 2016-11-25T16:15:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC_Emulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mpsoc.cpp \
    graphicalmpsoc.cpp \
    core.cpp \
    channel.cpp \
    appnode.cpp

HEADERS  += mainwindow.h \
    mpsoc.h \
    graphicalmpsoc.h \
    core.h \
    channel.h \
    appnode.h

FORMS    += mainwindow.ui \
    graphicalmpsoc.ui
