#-------------------------------------------------
#
# Project created by QtCreator 2017-10-24T20:15:47
#
#-------------------------------------------------

QT       += core gui websockets xml network multimedia xmlpatterns multimediawidgets network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = replicator-client
TEMPLATE = app
#Release:DEFINES += QT_NO_DEBUG_OUTPUT

VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = din-a-testware
QMAKE_TARGET_PRODUCT = replicator
QMAKE_TARGET_DESCRIPTION = replication machine
QMAKE_TARGET_COPYRIGHT = din-a-testware

#target.path = /usr/local/bin/
target.path = /home/pi/bin/
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES -= QT_DEPRECATED_WARNINGS
DEFINES -= -WUnused-parameter

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    extra_functions.cpp \
    process_data.cpp \
    lockscreen.cpp \
    settings.cpp \
    animations.cpp \
    videoplayer_control.cpp \
    musicplayer_control.cpp \
    listdelegate.cpp \
    loadingscreen.cpp \
    listdelegaterecipe.cpp \
    gradientprogressbar.cpp \
    lcars_roundededgeconnector.cpp\
    lcars_button.cpp \
    lcars_line.cpp \
    lcars_sliders.cpp \
    ExifTool.cpp \
    ExifToolPipe.cpp \
    musicdb_control.cpp \
    TagInfo.cpp \
    listdelegate_mediadatabase.cpp \
    listdelegateedge.cpp \
    listdelegatenotes.cpp \
    videofullscreen.cpp \
    listdelegatelanguages.cpp

HEADERS += \
        mainwindow.h \
    extra_functions.h \
    globalvars.h \
    process_data.h \
    lockscreen.h \
    settings.h \
    animations.h \
    videoplayer_control.h \
    musicplayer_control.h \
    listdelegate.h \
    loadingscreen.h \
    listdelegaterecipe.h \
    gradientprogressbar.h \
    lcars_roundededgeconnector.h\
    lcars_button.h \
    lcars_line.h \
    lcars_sliders.h \
    ExifTool.h \
    ExifToolPipe.h \
    musicdb_control.h \
    TagInfo.h \
    listdelegate_mediadatabase.h \
    listdelegateedge.h \
    listdelegatenotes.h \
    videofullscreen.h \
    listdelegatelanguages.h

FORMS += \
    mainwindow.ui \
    lockscreen.ui \
    loadingscreen.ui \
    videofullscreen.ui

RESOURCES += \
    resources.qrc

CONFIG += c++11

DISTFILES += \
    cleanup.sh \
    roundededgeconnector.json \
    lcars_line.json \
    lcars_sliders.json \
    lcars_slider.json

