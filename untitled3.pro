#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T20:56:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectionsdialog.cpp \
    settingsmanager.cpp \
    commonutils.cpp \
    ircnetwork.cpp \
    ircchannel.cpp \
    irc.cpp \
    ircconnection.cpp \
    ircmanager.cpp \
    chatpane.cpp \
    ircmessage.cpp

HEADERS  += mainwindow.h \
    connectionsdialog.h \
    settingsmanager.h \
    commonutils.h \
    ircnetwork.h \
    ircchannel.h \
    irc.h \
    ircconnection.h \
    ircmanager.h \
    chatpane.h \
    ircmessage.h

FORMS    += mainwindow.ui \
    connectionsdialog.ui

LIBS     += -lsqlite3
