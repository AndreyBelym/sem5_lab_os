#-------------------------------------------------
#
# Project created by QtCreator 2012-10-21T22:28:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab5OS-Server
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    ../Lab5OS/mailslots.cpp

HEADERS  += mainwindow.h \
    ../Lab5OS/mailslots.h

FORMS    += mainwindow.ui
