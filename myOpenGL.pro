#-------------------------------------------------
#
# Project created by QtCreator 2016-11-4T10:44:34
#
#-------------------------------------------------

TARGET = myOpenGL
TEMPLATE = app

HEADERS += \
    myglwidget.h \
    read.h \
    particle.h \
    tool.h

SOURCES += \
    main.cpp \
    myglwidget.cpp \
    read.cpp \
    particle.cpp

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += opengl
