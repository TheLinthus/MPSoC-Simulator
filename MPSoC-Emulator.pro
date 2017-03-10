# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPSoC-Emulator
TEMPLATE = app

HEADERS = Core/appnode.h \
    Core/channel.h \
    Core/core.h \
    Core/mpsoc.h \
    graphicalmpsoc.h \
    mainwindow.h \
    Core/application.h \
    Core/appload.h \
    View/mpsocbox.h \
    View/viewchannel.h \
    View/viewprocessor.h

SOURCES = Core/appnode.cpp \
    Core/channel.cpp \
    Core/core.cpp \
    Core/mpsoc.cpp \
    graphicalmpsoc.cpp \
    main.cpp \
    mainwindow.cpp \
    Core/application.cpp \
    Core/appload.cpp \
    View/mpsocbox.cpp \
    View/viewchannel.cpp \
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
message("Will instal those applications in: "$$applications.path)

#linux-g++{
#    for(FILE,heuristics.files){
#        QMAKE_POST_LINK += $$quote(cp $${FILE} $${heuristics.path}$$escape_expand(\n\t))
#    }
#    for(FILE,applications.files){
#        QMAKE_POST_LINK += $$quote(cp $${FILE} $${applications.path}$$escape_expand(\n\t))
#    }
#}

#win32 {
#    EXTRA_BINFILES_WIN = $${heuristics.files}
#    EXTRA_BINFILES_WIN ~= s,/,\\,g
#    DESTDIR_WIN = $${heuristics.path}
#    DESTDIR_WIN ~= s,/,\\,g
#    for(FILE,EXTRA_BINFILES_WIN){
#        QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#    }
#    EXTRA_BINFILES_WIN = $${applications.files}
#    EXTRA_BINFILES_WIN ~= s,/,\\,g
#    DESTDIR_WIN = $${applications.path}
#    DESTDIR_WIN ~= s,/,\\,g
#    for(FILE,EXTRA_BINFILES_WIN){
#        QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#    }
#}

INSTALLS += \ heuristics \ applications
