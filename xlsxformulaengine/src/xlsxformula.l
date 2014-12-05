
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
#include <stdio.h>
#include <QString>
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

%x str

%%

%{
char string_buf[1024];
char *string_buf_ptr;
#define SAVESTRING(bytes) \
    sym(1).sval = new QString(QString::fromUtf8(bytes)); \
    stringList.append(sym(1).sval)

%}

\"             { /* start double-quote */
                   string_buf_ptr = string_buf; BEGIN(str);
               }

<str>\"        { /* closing double-quote */
                   BEGIN(INITIAL);
                   *string_buf_ptr = '\0';
                   SAVESTRING(string_buf);
                   return T_STRING_LITERAL;
               }
<str>\"\"      { /* 2double-quote means escaped double-quote */
                   *string_buf_ptr++ = '\"';
               }
<str>[^\"]+    {
                   char *yptr = yytext;
                   while (*yptr)
                       *string_buf_ptr++ = *yptr++;
               }

[a-zA-Z_]([a-zA-Z0-9_])*  {
                              SAVESTRING(yytext);
                              return T_IDENTIFIER;
                          }

"#DIV/0!"|"#N/A"|"#NAME?"|"#NULL!"|"#NUM!"|"#REF!"|"#VALUE!"|"#GETTING_DATA" {
                              SAVESTRING(yytext);
                              return T_ERROR_CONSTANT;
                          }

{D}+{E}?            { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }
{D}*"."{D}+({E})?   { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }
{D}+"."{D}*({E})?   { sym(1).dval = atof(yytext); return T_NUMRIC_LITERAL; }

[ ]+        { return T_SPACE; }

"("         { return T_LPAREN; }
")"         { return T_RPAREN; }
"["         { return T_LBRACKET; }
"]"         { return T_RBRACKET; }
"{"         { return T_LBRACE; }
"}"         { return T_RBRACE; }

[ ]*":"[ ]*         { return T_COLON; }
[ ]*";"[ ]*         { return T_SEMICOLON; }
[ ]*"+"[ ]*         { return T_PLUS; }
[ ]*"-"[ ]*         { return T_MINUS; }
[ ]*"/"[ ]*         { return T_DIVIDE; }
[ ]*"*"[ ]*         { return T_STAR; }
[ ]*"^"[ ]*         { return T_XOR; }
[ ]*","[ ]*         { return T_COMMA; }
[ ]*"%"[ ]*         { return T_PERCENT; }
[ ]*"&"[ ]*         { return T_AND; }
[ ]*"="[ ]*         { return T_EQ; }
[ ]*"<"[ ]*         { return T_LT; }
[ ]*">"[ ]*         { return T_GT; }
[ ]*"<>"[ ]*        { return T_NOT_EQ; }
[ ]*"<="[ ]*        { return T_LE; }
[ ]*">="[ ]*        { return T_GE; }

[\t\v\f\n]                   { /* eat me */ }

%%
