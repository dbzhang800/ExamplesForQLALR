QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = demo
HEADERS += calc_grammar_p.h calc_parser.h qparser.h \
    widget.h
SOURCES += calc_grammar.cpp calc_parser.cpp qparser.cpp \
    main.cpp \
    widget.cpp
LEXSOURCES += calc.l

FORMS += \
    widget.ui
