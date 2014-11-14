----------------------------------------------------------------------------
--
-- Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
-- Contact: http://www.qt-project.org/legal
--
-- This file is part of the QtCore module of the Qt Toolkit.
--
-- $QT_BEGIN_LICENSE:LGPL$
-- Commercial License Usage
-- Licensees holding valid commercial Qt licenses may use this file in
-- accordance with the commercial license agreement provided with the
-- Software or, alternatively, in accordance with the terms contained in
-- a written agreement between you and Digia.  For licensing terms and
-- conditions see http://qt.digia.com/licensing.  For further information
-- use the contact form at http://qt.digia.com/contact-us.
--
-- GNU Lesser General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU Lesser
-- General Public License version 2.1 as published by the Free Software
-- Foundation and appearing in the file LICENSE.LGPL included in the
-- packaging of this file.  Please review the following information to
-- ensure the GNU Lesser General Public License version 2.1 requirements
-- will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
--
-- In addition, as a special exception, Digia gives you certain additional
-- rights.  These rights are described in the Digia Qt LGPL Exception
-- version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
--
-- GNU General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU
-- General Public License version 3.0 as published by the Free Software
-- Foundation and appearing in the file LICENSE.GPL included in the
-- packaging of this file.  Please review the following information to
-- ensure the GNU General Public License version 3.0 requirements will be
-- met: http://www.gnu.org/copyleft/gpl.html.
--
--
-- $QT_END_LICENSE$
--
----------------------------------------------------------------------------

%parser calc_grammar
%decl calc_parser.h
%impl calc_parser.cpp

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
#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "qparser.h"
#include "calc_grammar_p.h"

class CalcParser: public QParser<CalcParser, $table, double>
{
public:
  int nextToken();
  void consumeRule(int ruleno);
};

#endif // CALC_PARSER_H
:/





/.
#include "calc_parser.h"

#include <QtDebug>
#include <cstdlib>

void CalcParser::consumeRule(int ruleno)
  {
    switch (ruleno) {
./

Goal: Expression ;
/.
case $rule_number:
  qDebug() << "value:" << sym(1);
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
  sym(1) = -sym(1);
  break;
./

MultiplicativeExpression : UnaryExpression;
MultiplicativeExpression : MultiplicativeExpression T_STAR UnaryExpression;
/.
case $rule_number:
  sym(1) *= sym (3);
  break;
./

MultiplicativeExpression : MultiplicativeExpression T_DIVIDE UnaryExpression;
/.
case $rule_number:
  sym(1) /= sym (3);
  break;
./

AdditiveExpression: MultiplicativeExpression;
AdditiveExpression: AdditiveExpression T_PLUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1) += sym (3);
  break;
./

AdditiveExpression: AdditiveExpression T_MINUS MultiplicativeExpression;
/.
case $rule_number:
  sym(1) -= sym (3);
  break;
./

Expression: AdditiveExpression ;

/.
    } // switch
}
./
