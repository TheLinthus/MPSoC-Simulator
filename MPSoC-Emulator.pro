# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-Emulator
TEMPLATE = app


HEADERS = \
   $$PWD/Core/appnode.h \
   $$PWD/Core/channel.h \
   $$PWD/Core/core.h \
   $$PWD/Core/mpsoc.h \
   $$PWD/View/mpsoc.h \
   $$PWD/graphicalmpsoc.h \
   $$PWD/mainwindow.h

SOURCES = \
   $$PWD/Core/appnode.cpp \
   $$PWD/Core/channel.cpp \
   $$PWD/Core/core.cpp \
   $$PWD/Core/mpsoc.cpp \
   $$PWD/View/mpsoc.cpp \
   $$PWD/graphicalmpsoc.cpp \
   $$PWD/main.cpp \
   $$PWD/mainwindow.cpp

INCLUDEPATH = \
    $$PWD/. \
    $$PWD/Core \
    $$PWD/View
	
FORMS = \
   $$PWD/graphicalmpsoc.ui \
   $$PWD/mainwindow.ui

#DEFINES = 

