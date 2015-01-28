#-------------------------------------------------
#
# Project created by QtCreator 2015-01-13T22:04:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dirmanager
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    directoryscanner.cpp \
    directorychooser.cpp

HEADERS  += mainwindow.h \
    directoryscanner.h \
    directorychooser.h
