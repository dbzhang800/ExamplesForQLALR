# config qlalr
CONFIG += qlalr
QMAKE_QLALRFLAGS += --no-lines
INCLUDEPATH += $$PWD

QLALRSOURCES += $$PWD/xlsxformula.g
LEXSOURCES += $$PWD/xlsxformula.l

HEADERS += \
    $$PWD/xlsxformulaengine.h

SOURCES += \
    $$PWD/xlsxformulaengine.cpp
