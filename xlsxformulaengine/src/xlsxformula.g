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

%token T_STRING_LITERAL "string literal"

%token T_REF_CONSTANT "#REF"
%token T_ERROR_CONSTANT "#DIV/0 etc."
-- %token T_TRUE_LITERAL "TRUE"
-- %token T_FALSE_LITERAL "FALSE"
%token T_NUMRIC_LITERAL "numric literal"
%token T_IDENTIFIER "identifier"
%token T_LPAREN "("
%token T_RPAREN ")"
%token T_LBRACKET "["
%token T_RBRACKET "]"
%token T_LBRACE "{"
%token T_RBRACE "}"
%token T_COLON ":"
%token T_SPACE "SPACE_20"
%token T_SEMICOLON ";"
%token T_COMMA ","
%token T_XOR "^"
%token T_PLUS "+"
%token T_MINUS "-"
%token T_DIVIDE "/"
%token T_STAR "*"
%token T_PERCENT "%"
%token T_AND  "&"
%token T_LT "<"
%token T_GT ">"
%token T_LE "<="
%token T_GE ">="
%token T_EQ "="
%token T_NOT_EQ "<>"

%start Goal

/:
#ifndef XLSXFORMULAPARSER_P_H
#define XLSXFORMULAPARSER_P_H

#include <QSharedDataPointer>
#include <QVarLengthArray>
#include "xlsxformulagrammar_p.h"
#include "xlsxast_p.h"

typedef void* yyscan_t;
int xlsxformulalex_init(yyscan_t* ptr_yy_globals);
int xlsxformulalex_destroy(yyscan_t yyscanner);

class XlsxMemoryPool;

class XlsxFormulaParser: protected $table
{
public:
    union Value {
        int ival;
        double dval;
        QString *sval;
        XlsxAST::Node *Node;
        XlsxAST::ArgumentList *ArgumentList;
        XlsxAST::ExpressionNode *Expression;
        XlsxAST::IdentifierExpression *Identifier;
    };
    XlsxFormulaParser();
    ~XlsxFormulaParser();

    bool parse(const QString &formula, XlsxMemoryPool *pool);
    inline Value &sym(int index);

private:
    int nextToken(yyscan_t scanner);
    void consumeRule(int ruleno, XlsxMemoryPool *pool);

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
#include "xlsxformulaengine_p.h"
#include "xlsxast_p.h"
#include "xlsxmemorypool_p.h"

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

bool XlsxFormulaParser::parse(const QString &formula, XlsxMemoryPool *pool)
{
    QByteArray bytes = formula.toUtf8();

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
            consumeRule(r, pool);
            act = d->stateStack[d->tos] = nt_action(act, lhs[r] - TERMINAL_COUNT);
        } else {
            break;
        }
    }

    xlsxformula_delete_buffer(buffer, scanner);
    xlsxformulalex_destroy(scanner);
    return false;
}

void XlsxFormulaParser::consumeRule(int ruleno, XlsxMemoryPool *pool)
  {
    switch (ruleno) {
./

Goal: Expression ;
/.
case $rule_number:
  break;
./

PrimaryExpression: T_STRING_LITERAL ;
/.
case $rule_number:
{
  XlsxAST::Node *node = makeAstNode<XlsxAST::StringLiteral> (pool, *sym(1).sval);
  delete sym(1).sval;
  sym(1).Node = node;
}
  break;
./

PrimaryExpression: T_NUMRIC_LITERAL ;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::NumericLiteral> (pool, sym(1).dval);
  break;
./
PrimaryExpression: NameExpression;
NameExpression: T_IDENTIFIER;
/.
  case $rule_number:
  {
    XlsxAST::Node *node = makeAstNode<XlsxAST::IdentifierExpression> (pool, *(sym(1).sval));
    delete sym(1).sval;
    sym(1).Node = node;
  }
    break;
./

PrimaryExpression: T_LPAREN Expression T_RPAREN;
/.
case $rule_number:
  sym(1).Node = sym(2).Node;
  break;
./

PrimaryExpression: CallExpression;
CallExpression: NameExpression Arguments;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::CallExpression> (pool, sym(1).Identifier->name, sym(2).ArgumentList);
    break;
./

Arguments: T_LPAREN T_RPAREN;
/.
  case $rule_number:
    sym(1).Node = 0;
    break;
./

Arguments: T_LPAREN ArgumentList T_RPAREN;
/.
  case $rule_number:
   sym(1).Node = sym(2).ArgumentList->finish ();
   break;
./

ArgumentList: Expression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::ArgumentList> (pool, sym(1).Expression);
    break;
./

ArgumentList: ArgumentList T_COMMA Expression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::ArgumentList> (pool, sym(1).ArgumentList, sym(3).Expression);
    break;
./

UnaryExpression: PrimaryExpression;
UnaryExpression: T_PLUS UnaryExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::UnaryPlusExpression> (pool, sym(2).Expression);
  break;
./

UnaryExpression: T_MINUS UnaryExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::UnaryMinusExpression> (pool, sym(2).Expression);
  break;
./

UnaryExpression: UnaryExpression T_PERCENT;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::UnaryPercentExpression> (pool, sym(2).Expression);
    break;
./

ExponentialExpression : UnaryExpression;
ExponentialExpression : ExponentialExpression T_XOR UnaryExpression;
/.
  case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::BinaryArithmeticExpression> (pool, sym(1).Expression, XlsxAST::Exp, sym(3).Expression);
  break;
./

MultiplicativeExpression : ExponentialExpression;
MultiplicativeExpression : MultiplicativeExpression T_STAR ExponentialExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::BinaryArithmeticExpression> (pool, sym(1).Expression, XlsxAST::Mul, sym(3).Expression);
  break;
./

MultiplicativeExpression : MultiplicativeExpression T_DIVIDE ExponentialExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::BinaryArithmeticExpression> (pool, sym(1).Expression, XlsxAST::Div, sym(3).Expression);
  break;
./

AdditiveExpression: MultiplicativeExpression;
AdditiveExpression: AdditiveExpression T_PLUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::BinaryArithmeticExpression> (pool, sym(1).Expression, XlsxAST::Add, sym(3).Expression);
  break;
./

AdditiveExpression: AdditiveExpression T_MINUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1).Node = makeAstNode<XlsxAST::BinaryArithmeticExpression> (pool, sym(1).Expression, XlsxAST::Sub, sym(3).Expression);
  break;
./

ConcatenationExpression: AdditiveExpression;
ConcatenationExpression: ConcatenationExpression T_AND AdditiveExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryTextExpression> (pool, sym(1).Expression, XlsxAST::Concat, sym(3).Expression);
    break;
./

ComparisonExpression: ConcatenationExpression;
ComparisonExpression: ComparisonExpression T_LT ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::Lt, sym(3).Expression);
    break;
./

ComparisonExpression: ComparisonExpression T_LE ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::Le, sym(3).Expression);
    break;
./

ComparisonExpression: ComparisonExpression T_GT ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::Gt, sym(3).Expression);
    break;
./

ComparisonExpression: ComparisonExpression T_GE ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::Ge, sym(3).Expression);
    break;
./

ComparisonExpression: ComparisonExpression T_EQ ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::Equal, sym(3).Expression);
    break;
./

ComparisonExpression: ComparisonExpression T_NOT_EQ ConcatenationExpression;
/.
  case $rule_number:
    sym(1).Node = makeAstNode<XlsxAST::BinaryComparisonExpression> (pool, sym(1).Expression, XlsxAST::NotEqual, sym(3).Expression);
    break;
./

Expression: ComparisonExpression;
Expression: Expression T_SPACE;
Expression: T_SPACE Expression;
/.
  case $rule_number:
    sym(1).Node = sym(2).Node;
    break;
./

/.
    } // switch
}
./
