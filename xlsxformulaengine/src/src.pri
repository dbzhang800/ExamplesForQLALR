# config qlalr
CONFIG += qlalr
QMAKE_QLALRFLAGS += --no-lines
INCLUDEPATH += $$PWD

QLALRSOURCES += $$PWD/xlsxformula.g
LEXSOURCES += $$PWD/xlsxformula.l

HEADERS += \
    $$PWD/xlsxformulaengine.h \
    $$PWD/xlsxformulaengine_p.h \
    $$PWD/xlsxast_p.h \
    $$PWD/xlsxmemorypool_p.h \
    $$PWD/xlsxformulainterpreter_p.h \
    $$PWD/xlsxcelldata.h \
    $$PWD/xlsxworksheet.h

SOURCES += \
    $$PWD/xlsxformulaengine.cpp \
    $$PWD/xlsxast.cpp \
    $$PWD/xlsxformulainterpreter.cpp \
    $$PWD/xlsxcelldata.cpp \
    $$PWD/xlsxworksheet.cpp
