 #-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T13:47:02
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FP_worth
TEMPLATE = app


SOURCES += main.cpp\
        fpworth.cpp \
    fptree.cpp \
    candidatetree.cpp \
    plot.cpp \
    alglib/alglibinternal.cpp \
    alglib/alglibmisc.cpp \
    alglib/ap.cpp \
    alglib/dataanalysis.cpp \
    alglib/diffequations.cpp \
    alglib/fasttransforms.cpp \
    alglib/integration.cpp \
    alglib/interpolation.cpp \
    alglib/linalg.cpp \
    alglib/optimization.cpp \
    alglib/solvers.cpp \
    alglib/specialfunctions.cpp \
    alglib/statistics.cpp \
    neuronnetwork.cpp

HEADERS  += fpworth.h \
    fptree.h \
    candidatetree.h \
    plot.h \
    alglib/alglibinternal.h \
    alglib/alglibmisc.h \
    alglib/ap.h \
    alglib/dataanalysis.h \
    alglib/diffequations.h \
    alglib/fasttransforms.h \
    alglib/integration.h \
    alglib/interpolation.h \
    alglib/linalg.h \
    alglib/optimization.h \
    alglib/solvers.h \
    alglib/specialfunctions.h \
    alglib/statistics.h \
    alglib/stdafx.h \
    neuronnetwork.h \
    structs.h

FORMS    += fpworth.ui

OTHER_FILES += \
    logo.png \
    images/loading.gif \
    TODO.txt

CONFIG += qwt

RESOURCES += \
    resources.qrc


