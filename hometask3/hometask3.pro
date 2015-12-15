TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c
QMAKE_LFLAGS += -fopenmp

include(deployment.pri)
qtcAddDeployment()

