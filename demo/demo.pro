QT = core
CONFIG += console
CONFIG   -= app_bundle
TARGET = demo
HEADERS += calc_grammar_p.h calc_parser.h qparser.h
SOURCES += calc_grammar.cpp calc_parser.cpp qparser.cpp \
    main.cpp
LEXSOURCES += calc.l
