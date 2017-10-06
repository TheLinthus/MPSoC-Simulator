# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += script core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-Emulator
TEMPLATE = app


HEADERS = Core/appnode.h \
    Core/channel.h \
    Core/mpsoc.h \
    mainwindow.h \
    Core/application.h \
    Core/appload.h \
    View/mpsocbox.h \
    View/viewchannel.h \
    Core/processor.h \
    View/viewprocessor.h

SOURCES = Core/appnode.cpp \
    Core/channel.cpp \
    Core/mpsoc.cpp \
    main.cpp \
    mainwindow.cpp \
    Core/application.cpp \
    Core/appload.cpp \
    View/mpsocbox.cpp \
    View/viewchannel.cpp \
    Core/processor.cpp \
    View/viewprocessor.cpp

INCLUDEPATH = . \
    Core \
    View
	
FORMS = \
    mainwindow.ui \
    View/mpsocbox.ui

#DEFINES = 

RESOURCES += \
    resources.qrc

DISTFILES += \
    Heuristics/FirstFree.js \
    Applications/APP_GENE/Aplicacao_0.txt \
    Applications/APP_GENE/Aplicacao_1.txt \
    Applications/APP_GENE/Aplicacao_10.txt \
    Applications/APP_GENE/Aplicacao_11.txt \
    Applications/APP_GENE/Aplicacao_12.txt \
    Applications/APP_GENE/Aplicacao_13.txt \
    Applications/APP_GENE/Aplicacao_14.txt \
    Applications/APP_GENE/Aplicacao_15.txt \
    Applications/APP_GENE/Aplicacao_16.txt \
    Applications/APP_GENE/Aplicacao_17.txt \
    Applications/APP_GENE/Aplicacao_18.txt \
    Applications/APP_GENE/Aplicacao_19.txt \
    Applications/APP_GENE/Aplicacao_2.txt \
    Applications/APP_GENE/Aplicacao_20.txt \
    Applications/APP_GENE/Aplicacao_3.txt \
    Applications/APP_GENE/Aplicacao_4.txt \
    Applications/APP_GENE/Aplicacao_5.txt \
    Applications/APP_GENE/Aplicacao_6.txt \
    Applications/APP_GENE/Aplicacao_7.txt \
    Applications/APP_GENE/Aplicacao_8.txt \
    Applications/APP_GENE/Aplicacao_9.txt \
    Applications/APP_GENE.json

heuristics.path = $$OUT_PWD/Heuristics
heuristics.files += $$files(Heuristics/*)

#message("Found heuristics for installation: "$$heuristics.files)
#message("Will install those heuristics in: "$$heuristics.path)

applications.path = $$OUT_PWD/Applications
applications.files += $$files(Applications/*)

#message("Found applications for installation: "$$applications.files)
#message("Will install those applications in: "$$applications.path)

INSTALLS += \
    heuristics \
    applications
