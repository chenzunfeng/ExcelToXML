#-------------------------------------------------
#
# Project created by QtCreator 2016-05-05T10:18:18
#
#-------------------------------------------------

QT       += core gui xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExcelToXML
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    finder.cpp \
    partinfo.cpp

HEADERS  += mainwindow.h \
    finder.h \
    partinfo.h

FORMS    += mainwindow.ui
