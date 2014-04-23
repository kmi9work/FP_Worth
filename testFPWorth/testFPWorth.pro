#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T13:51:07
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testfpworthtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_testfpworthtest.cpp \
    ../fptree.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../fptree.h
