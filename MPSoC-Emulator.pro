# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += script core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-Emulator
TEMPLATE = app


HEADERS = Core/appnode.h \
   Core/channel.h \
   Core/mpsoc.h \
   graphicalmpsoc.h \
   mainwindow.h \
    Core/application.h \
    Core/appload.h \
    View/mpsocbox.h \
    View/viewchannel.h
    Core/processor.h \
    View/viewprocessor.h

SOURCES = Core/appnode.cpp \
   Core/channel.cpp \
   Core/mpsoc.cpp \
   graphicalmpsoc.cpp \
   main.cpp \
   mainwindow.cpp \
    View/viewprocessor.h
    Core/application.cpp \
    Core/appload.cpp \
    View/mpsocbox.cpp \
    View/viewchannel.cpp
    Core/processor.cpp \
    View/viewprocessor.cpp

INCLUDEPATH = . \
    Core \
    View
	
FORMS = graphicalmpsoc.ui \
   mainwindow.ui \
    View/mpsocbox.ui

#DEFINES = 

RESOURCES += \
    resources.qrc

DISTFILES += \
    Heuristics/FirstFree.js \
    Applications/DefaultApplications.json


heuristics.path = $$OUT_PWD/Heuristics
heuristics.files += $$files(Heuristics/*)

message("Found heuristics for installation: "$$heuristics.files)
message("Will install those heuristics in: "$$heuristics.path)

applications.path = $$OUT_PWD/Applications
applications.files += $$files(Applications/*)

message("Found applications for installation: "$$applications.files)
message("Will install those applications in: "$$applications.path)

INSTALLS += \ heuristics \ applications
