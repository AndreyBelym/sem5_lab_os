#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T14:47:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab4OS-Server
TEMPLATE = app

INCLUDEPATH += "muparser/include"
LIBS += "C:/Users/Wolf/Documents/Lab4OS-Server/muparser/lib/libmuparser.a"

SOURCES += main.cpp\
        mainwindow.cpp \
    threadmanager.cpp

HEADERS  += mainwindow.h \
    threadmanager.h

FORMS    += mainwindow.ui
