# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-Emulator
TEMPLATE = app


HEADERS = Core/appnode.h \
   Core/channel.h \
   Core/core.h \
   Core/mpsoc.h \
   graphicalmpsoc.h \
   mainwindow.h \
    View/viewmpsoc.h \
    Core/application.h \
    Core/appload.h

SOURCES = Core/appnode.cpp \
   Core/channel.cpp \
   Core/core.cpp \
   Core/mpsoc.cpp \
   graphicalmpsoc.cpp \
   main.cpp \
   mainwindow.cpp \
    View/viewmpsoc.cpp \
    Core/application.cpp \
    Core/appload.cpp

INCLUDEPATH = . \
    Core \
    View
	
FORMS = graphicalmpsoc.ui \
   mainwindow.ui

#DEFINES = 

