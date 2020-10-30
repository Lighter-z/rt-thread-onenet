#-------------------------------------------------
#
# Project created by QtCreator 2019-06-19T23:52:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygauge2.cpp \
    onenet.cpp

HEADERS  += mainwindow.h \
    mygauge2.h \
    onenet.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc
