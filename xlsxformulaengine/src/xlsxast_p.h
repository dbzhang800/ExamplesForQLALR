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
        Kind_IdentifierExpression,
        Kind_UnaryMinusExpression,
        Kind_UnaryPlusExpression,
        Kind_UnaryPercentExpression,

        Kind_BinaryReferenceExpression,
        Kind_BinaryArithmeticExpression,
        Kind_BinaryTextExpression,
        Kind_BinaryComparisonExpression,
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
    IdentifierExpression(const QStringRef &n):
        name (n) { kind = Kind_IdentifierExpression; }

// attributes
    QStringRef name;
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
    StringLiteral(const QString &v):
        value (v) { kind = Kind_StringLiteral; }

// attributes:
    QString value;
};

class UnaryPlusExpression: public ExpressionNode
{
public:
    UnaryPlusExpression(Node *e):
        expression (e) { kind = Kind_UnaryPlusExpression; }

// attributes
    Node *expression;
};

class UnaryMinusExpression: public ExpressionNode
{
public:
    UnaryMinusExpression(Node *e):
        expression (e) { kind = Kind_UnaryMinusExpression; }

// attributes
    Node *expression;
};

class UnaryPercentExpression: public ExpressionNode
{
public:
    UnaryPercentExpression(Node *e):
        expression (e) { kind = Kind_UnaryPercentExpression; }

// attributes
    Node *expression;
};

class BinaryExpressionNode: public Node
{
public:
    BinaryExpressionNode(Node *l, int o, Node *r):
        left (l), op (o), right (r)
        { kind = Kind_BinaryExpressionNode; }

// attributes
    Node *left;
    int op;
    Node *right;
};

class BinaryReferenceExpression: public BinaryExpressionNode
{
public:
    BinaryReferenceExpression(Node *l, int o, Node *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryReferenceExpression; }
};

class BinaryArithmeticExpression: public BinaryExpressionNode
{
public:
    BinaryArithmeticExpression(Node *l, int o, Node *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryArithmeticExpression; }
};

class BinaryTextExpression: public BinaryExpressionNode
{
public:
    BinaryTextExpression(Node *l, int o, Node *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryTextExpression; }
};

class BinaryComparisonExpression: public BinaryExpressionNode
{
public:
    BinaryComparisonExpression(Node *l, int o, Node *r):
        BinaryExpressionNode(l, o, r)
        { kind = Kind_BinaryComparisonExpression; }
};

} // namespace XlsxAST

#endif
