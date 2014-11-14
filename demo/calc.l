
%option noyywrap nounistd never-interactive

%{
/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QLALR tool of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "calc_parser.h"
#include <cstdlib>
#ifdef _MSC_VER
  #pragma warning(disable: 4003)
  #pragma warning(disable: 4018)
#endif
#define YY_DECL int CalcParser::nextToken()
%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
%%

[ \t\n]     { /* eat me */ }

{L}({L}|{D})*       { return T_IDENTIFIER; }
{D}+{E}?            { sym(1) = atof(yytext); return T_NUMRIC_LITERAL; }
{D}*"."{D}+({E})?   { sym(1) = atof(yytext); return T_NUMRIC_LITERAL; }
{D}+"."{D}*({E})?   { sym(1) = atof(yytext); return T_NUMRIC_LITERAL; }
\"(\\.|[^\\"])*\"   { return T_STRING_LITERAL; }

"("         { return T_LPAREN; }
")"         { return T_RPAREN; }
"+"         { return T_PLUS; }
"-"         { return T_MINUS; }
"/"         { return T_DIVIDE; }
"*"         { return T_STAR; }

%%
