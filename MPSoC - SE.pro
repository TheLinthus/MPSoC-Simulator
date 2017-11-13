# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

CONFIG += qt

QT       += script core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-SE
TEMPLATE = app

HEADERS = Core/appnode.h \
    Core/channel.h \
    Core/mpsoc.h \
    Core/application.h \
    Core/appload.h \
    View/mpsocbox.h \
    View/viewchannel.h \
    Core/processor.h \
    View/viewprocessor.h \
    Core/applicationcontroller.h \
    Core/mpsoccontroller.h \
    singleton.h \
    call_once.h \
    View/newmpsocdialog.h \
    Core/heuristic.h \
    Core/heuristiccontroller.h \
    mpsoc_se.h \
    View/simulationtab.h \
    View/applicationstab.h \
    View/heuristicstab.h \
    View/viewappnode.h \
    Core/simulationcontroller.h \
    Debug/logger.h

SOURCES = Core/appnode.cpp \
    Core/channel.cpp \
    Core/mpsoc.cpp \
    main.cpp \
    Core/application.cpp \
    Core/appload.cpp \
    View/mpsocbox.cpp \
    View/viewchannel.cpp \
    Core/processor.cpp \
    View/viewprocessor.cpp \
    Core/applicationcontroller.cpp \
    Core/mpsoccontroller.cpp \
    View/newmpsocdialog.cpp \
    Core/heuristic.cpp \
    Core/heuristiccontroller.cpp \
    mpsoc_se.cpp \
    View/simulationtab.cpp \
    View/applicationstab.cpp \
    View/heuristicstab.cpp \
    View/viewappnode.cpp \
    Core/simulationcontroller.cpp \
    Debug/logger.cpp

INCLUDEPATH = . \
    Core \
    View
	
FORMS = \
    View/mpsocbox.ui \
    View/newmpsocdialog.ui \
    mpsoc_se.ui \
    View/simulationtab.ui \
    View/applicationstab.ui \
    View/heuristicstab.ui

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
    Applications/APP_GENE.json \
    mpsoc.ico \
    mpsoc.rc \
    Heuristics/NearestNeighbor.js \
    mpsoc.icns \
    .gitignore \
    LICENSE \
    README.md

macx {
    ICON = mpsoc.icns

    heuristics.path = Contents/Resources/Heuristics
    heuristics.files += $$files(Heuristics/*)

    applications.path = Contents/Resources/Applications
    applications.files += $$files(Applications/*)

    QMAKE_BUNDLE_DATA += \
        heuristics \
        applications
}
!macx {
    RC_FILE = mpsoc.rc

    heuristics.path = $$OUT_PWD/Heuristics
    heuristics.files += $$files(Heuristics/*)

    applications.path = $$OUT_PWD/Applications
    applications.files += $$files(Applications/*)

    INSTALLS += \
        heuristics \
        applications
}
