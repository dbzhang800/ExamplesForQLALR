TARGET = demo

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += qlalr
    QMAKE_QLALRFLAGS += --no-lines
    INCLUDEPATH += $$PWD
    QLALRSOURCES += calc.g
} else {
    #Run "qlalr calc.g" by hand for Qt4
    HEADERS += calc_grammar_p.h calc_parser.h
    SOURCES += calc_grammar.cpp calc_parser.cpp
}

HEADERS += qparser.h widget.h
SOURCES += qparser.cpp main.cpp widget.cpp
FORMS += widget.ui

LEXSOURCES += calc.l
