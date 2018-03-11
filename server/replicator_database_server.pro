QT       += core websockets xml xmlpatterns
QT       -= gui
QT += sql
QT += network
TARGET = replicator-server
target.path = /home/pi/bin/
INSTALLS += target
#Release:TARGET = replicator-server

#Debug:TARGET = replicator-server



CONFIG   += console
CONFIG   -= app_bundle

#target.path = /usr/local/bin/
#INSTALLS += target

TEMPLATE = app

SOURCES += \
    main.cpp \
    replicator_database_server.cpp \
    db_search.cpp \
    console.cpp

HEADERS += \
    replicator_database_server.h \
    globalvars.h \
    db_search.h \
    console.h

#RESOURCES += replicator_database_server.qrc
