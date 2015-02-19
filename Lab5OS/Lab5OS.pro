#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T08:44:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab5OS
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsscene.cpp \
    mailslots.cpp

HEADERS  += mainwindow.h \
    graphicsscene.h \
    mailslots.h

FORMS    += mainwindow.ui
