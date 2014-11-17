
%option reentrant noyywrap nounistd never-interactive

%{
/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang
** Contact: hello@debao.me
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "xlsxformulaparser_p.h"
#include <stdlib.h>
#ifdef _MSC_VER
  #pragma warning(disable: 4003)
  #pragma warning(disable: 4018)
#endif
#define YY_DECL int XlsxFormulaParser::nextToken(yyscan_t yyscanner)
%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
%%

[ \t\n]     { /* eat me */ }

{L}({L}|{D})*       { sym(1).dval = atof(yytext); return T_IDENTIFIER; }
{D}+{E}?            { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }
{D}*"."{D}+({E})?   { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }
{D}+"."{D}*({E})?   { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }
\"(\\.|[^\\"])*\"   { return T_STRING_LITERAL; }

"("         { return T_LPAREN; }
")"         { return T_RPAREN; }
"+"         { return T_PLUS; }
"-"         { return T_MINUS; }
"/"         { return T_DIVIDE; }
"*"         { return T_STAR; }

%%
