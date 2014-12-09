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

#ifndef XLSXSAST_P_H
#define XLSXSAST_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qstring.h>

namespace XlsxAST {

enum Op {
    Range, // ":"
    Union, // ","
    Intersection, // " "

    Add, //"+"
    Sub, //"-"
    Mul, //"*"
    Div, //"\"
    Exp, //"^"

    Concat, //"&"

    Equal, //"="
    Ge, //">="
    Gt, //">"
    Le, //"<="
    Lt, //"<"
    NotEqual, //"<>"
};

class Node
{
public:
    enum Kind {
        Kind_Node,
        Kind_ExpressionNode,
        Kind_BinaryExpressionNode,

        Kind_NumericLiteral,
        Kind_StringLiteral,
        Kind_ErrorConstantLiteral,
        Kind_IdentifierExpression,
        Kind_UnaryMinusExpression,
        Kind_UnaryPlusExpression,
        Kind_UnaryPercentExpression,

        Kind_BinaryReferenceExpression,
        Kind_BinaryArithmeticExpression,
        Kind_BinaryTextExpression,
        Kind_BinaryComparisonExpression,

        Kind_ArgumentList,
        Kind_CallExpression,
    };

    inline Node()
        : kind(Kind_Node) {}

    // NOTE: node destructors are never called,
    //       instead we block free the memory
    //       (see the MemoryPool class)
    virtual ~Node() {}

// attributes
    int kind;
};

class ExpressionNode: public Node
{
public:
    ExpressionNode()
    { kind = Kind_ExpressionNode; }
};

class IdentifierExpression: public ExpressionNode
{
public:
    IdentifierExpression(const QString *name, const QString *parentName=0):
        name(name), parentName(parentName)
    { kind = Kind_IdentifierExpression; }

// attributes
    const QString *name;
    const QString *parentName;//SheetName for cellRef or WorkName for name
};

class NumericLiteral: public ExpressionNode
{
public:
    NumericLiteral(double v):
        value(v) { kind = Kind_NumericLiteral; }

// attributes:
    double value;
};

class StringLiteral: public ExpressionNode
{
public:
    StringLiteral(const QString *v):
        value (v) { kind = Kind_StringLiteral; }

// attributes:
    const QString *value;
};

class ErrorConstantLiteral: public ExpressionNode
{
public:
    ErrorConstantLiteral(const QString *v):
        value (v) { kind = Kind_ErrorConstantLiteral; }

// attributes:
    const QString *value;
};

class UnaryPlusExpression: public ExpressionNode
{
public:
    UnaryPlusExpression(ExpressionNode *e):
        expression (e) { kind = Kind_UnaryPlusExpression; }

// attributes
    ExpressionNode *expression;
};

class UnaryMinusExpression: public ExpressionNode
{
public:
    UnaryMinusExpression(ExpressionNode *e):
        expression (e) { kind = Kind_UnaryMinusExpression; }

// attributes
    ExpressionNode *expression;
};

class UnaryPercentExpression: public ExpressionNode
{
public:
    UnaryPercentExpression(ExpressionNode *e):
        expression (e) { kind = Kind_UnaryPercentExpression; }

// attributes
    ExpressionNode *expression;
};

class BinaryExpressionNode: public ExpressionNode
{
public:
    BinaryExpressionNode(ExpressionNode *l, int o, ExpressionNode *r):
        left (l), op (o), right (r)
        { kind = Kind_BinaryExpressionNode; }

// attributes
    ExpressionNode *left;
    int op;
    ExpressionNode *right;
};

class BinaryReferenceExpression: public BinaryExpressionNode
{
public:
    BinaryReferenceExpression(ExpressionNode *l, int o, ExpressionNode *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryReferenceExpression; }
};

class BinaryArithmeticExpression: public BinaryExpressionNode
{
public:
    BinaryArithmeticExpression(ExpressionNode *l, int o, ExpressionNode *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryArithmeticExpression; }
};

class BinaryTextExpression: public BinaryExpressionNode
{
public:
    BinaryTextExpression(ExpressionNode *l, int o, ExpressionNode *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryTextExpression; }
};

class BinaryComparisonExpression: public BinaryExpressionNode
{
public:
    BinaryComparisonExpression(ExpressionNode *l, int o, ExpressionNode *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryComparisonExpression; }
};

class ArgumentList: public Node
{
public:
    ArgumentList(ExpressionNode *e):
        expression (e), next (this)
        { kind = Kind_ArgumentList; }

    ArgumentList(ArgumentList *previous, ExpressionNode *e):
        expression (e)
    {
        kind = Kind_ArgumentList;
        next = previous->next;
        previous->next = this;
    }

    inline ArgumentList *finish ()
    {
        ArgumentList *front = next;
        next = 0;
        return front;
    }

// attributes
    ExpressionNode *expression;
    ArgumentList *next;
};

class CallExpression: public ExpressionNode
{
public:
    CallExpression(const QString *name, ArgumentList *a):
        name (name), arguments (a)
        { kind = Kind_CallExpression; }

// attributes
    const QString *name;
    ArgumentList *arguments;
};

} // namespace XlsxAST

#endif
