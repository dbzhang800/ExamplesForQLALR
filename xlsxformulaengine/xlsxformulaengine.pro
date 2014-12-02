QT       += testlib
QT       -= gui

TEMPLATE = app

TARGET = tst_xlsxformulaengine
CONFIG   += console
CONFIG   -= app_bundle

include ($$PWD/src/src.pri)

SOURCES += main.cpp
