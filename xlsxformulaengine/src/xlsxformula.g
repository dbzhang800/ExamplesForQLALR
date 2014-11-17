----------------------------------------------------------------------------
--
-- Copyright (C) 2014 Debao Zhang
-- Contact: hello@debao.me
--
-- This file may be used under the terms of the GNU Lesser
-- General Public License version 2.1 or version 3 as published by the Free
-- Software Foundation and appearing in the file LICENSE.LGPLv21 and
-- LICENSE.LGPLv3 included in the packaging of this file. Please review the
-- following information to ensure the GNU Lesser General Public License
-- requirements will be met: https://www.gnu.org/licenses/lgpl.html and
-- http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
--
----------------------------------------------------------------------------

%parser XlsxFormulaGrammar
%decl xlsxformulaparser_p.h
%impl xlsxformulaparser.cpp

%token T_NUMRIC_LITERAL "numric literal"
%token T_STRING_LITERAL "string literal"
%token T_IDENTIFIER "identifier"
%token T_LPAREN "("
%token T_RPAREN ")"
%token T_PLUS "+"
%token T_MINUS "-"
%token T_DIVIDE "/"
%token T_STAR "*"

%start Goal

/:
#ifndef XLSXFORMULAPARSER_P_H
#define XLSXFORMULAPARSER_P_H

#include <QSharedDataPointer>
#include <QVarLengthArray>
#include "xlsxformulagrammar_p.h"

typedef void* yyscan_t;
int xlsxformulalex_init(yyscan_t* ptr_yy_globals);
int xlsxformulalex_destroy(yyscan_t yyscanner);

class XlsxFormulaNameIdImpl;

class XlsxFormulaParser: protected $table
{
public:
    union Value {
        int ival;
        double dval;
        XlsxFormulaNameIdImpl *sval;
    };
    XlsxFormulaParser();
    ~XlsxFormulaParser();

    bool parse(const QByteArray &bytes);
    inline Value &sym(int index);

private:
    int nextToken(yyscan_t scanner);
    void consumeRule(int ruleno);

    enum { DefaultStackSize = 128 };

    struct Data: public QSharedData
    {
        Data(): stackSize (DefaultStackSize), tos (0) {}

        QVarLengthArray<int, DefaultStackSize> stateStack;
        QVarLengthArray<Value, DefaultStackSize> parseStack;
        int stackSize;
        int tos;

        void reallocateStack() {
            stackSize <<= 1;
            stateStack.resize(stackSize);
            parseStack.resize(stackSize);
        }
    };

    QSharedDataPointer<Data> d;
};

inline XlsxFormulaParser::Value &XlsxFormulaParser::sym(int n)
{
    return d->parseStack [d->tos + n - 1];
}

#endif // XLSXFORMULAPARSER_P_H
:/





/.
#include "xlsxformulaparser_p.h"

#include <QDebug>
#include <QByteArray>
#include <stdlib.h>

struct yy_buffer_state;
yy_buffer_state * xlsxformula_scan_bytes(const char *bytes,int len, yyscan_t yyscanner);
void xlsxformula_delete_buffer(yy_buffer_state *buffer, yyscan_t yyscanner);

XlsxFormulaParser::XlsxFormulaParser():
    d(new Data())
{
}

XlsxFormulaParser::~XlsxFormulaParser()
{
}

bool XlsxFormulaParser::parse(const QByteArray &bytes)
{
    const int INITIAL_STATE = 0;
    yyscan_t scanner;
    xlsxformulalex_init(&scanner);
    yy_buffer_state * buffer = xlsxformula_scan_bytes(bytes.data(), bytes.size(), scanner);

    d->tos = 0;
    d->reallocateStack();

    int act = d->stateStack[++d->tos] = INITIAL_STATE;
    int token = -1;

    forever {
        if (token == -1 && - TERMINAL_COUNT != action_index[act])
            token = nextToken(scanner);

        act = t_action(act, token);

        if (d->stateStack[d->tos] == ACCEPT_STATE) {
            xlsxformula_delete_buffer(buffer, scanner);
            xlsxformulalex_destroy(scanner);
            return true;
        }

        if (act > 0) {
            if (++d->tos == d->stackSize)
                d->reallocateStack();

            d->parseStack[d->tos] = d->parseStack[d->tos - 1];
            d->stateStack[d->tos] = act;
            token = -1;
        } else if (act < 0) {
            int r = - act - 1;
            d->tos -= rhs[r];
            act = d->stateStack[d->tos++];
            consumeRule(r);
            act = d->stateStack[d->tos] = nt_action(act, lhs[r] - TERMINAL_COUNT);
        } else {
            break;
        }
    }

    xlsxformula_delete_buffer(buffer, scanner);
    xlsxformulalex_destroy(scanner);
    return false;
}

void XlsxFormulaParser::consumeRule(int ruleno)
  {
    switch (ruleno) {
./

Goal: Expression ;
/.
case $rule_number:
  break;
./

PrimaryExpression: T_NUMRIC_LITERAL ;
PrimaryExpression: T_LPAREN Expression T_RPAREN ;
/.
case $rule_number:
  sym(1) = sym (2);
  break;
./

UnaryExpression: PrimaryExpression;
UnaryExpression: T_PLUS PrimaryExpression;
UnaryExpression: T_MINUS PrimaryExpression;
/.
case $rule_number:
  sym(1).dval = -sym(1).dval;
  break;
./

MultiplicativeExpression : UnaryExpression;
MultiplicativeExpression : MultiplicativeExpression T_STAR UnaryExpression;
/.
case $rule_number:
  sym(1).dval *= sym (3).dval;
  break;
./

MultiplicativeExpression : MultiplicativeExpression T_DIVIDE UnaryExpression;
/.
case $rule_number:
  sym(1).dval /= sym (3).dval;
  break;
./

AdditiveExpression: MultiplicativeExpression;
AdditiveExpression: AdditiveExpression T_PLUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1).dval += sym (3).dval;
  break;
./

AdditiveExpression: AdditiveExpression T_MINUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1).dval -= sym (3).dval;
  break;
./

Expression: AdditiveExpression ;

/.
    } // switch
}
./
