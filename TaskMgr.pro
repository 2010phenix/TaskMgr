#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T18:01:40
#
#-------------------------------------------------

QT       += core gui
QT += widgets

TARGET = TskMgr
TEMPLATE = app

CONFIG += c++11
#CONFIG += console


SOURCES += main.cpp\
	ProcessView.cpp \
	SystemUtils.cpp \
    GraphView.cpp \
    TaskManager.cpp

HEADERS  += \
	ProcessView.h \
	SystemUtils.h \
    GraphView.h \
    TaskManager.h


LIBS += -lpsapi -lpdh

FORMS +=
