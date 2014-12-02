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
    Add,
    Div,
    Equal,
    Ge,
    Gt,
    Le,
    Lt,
    Mul,
    NotEqual,
    Sub
};
class BinaryExpression;
class Node
{
public:
    enum Kind {
        Kind_Node,
        Kind_BinaryExpression,
        Kind_CallExpression,
        Kind_IdentifierExpression,
        Kind_NumericLiteral,
        Kind_StringLiteral,
        Kind_UnaryMinusExpression,
        Kind_UnaryPlusExpression
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

class IdentifierExpression: public Node
{
public:
    IdentifierExpression(const QStringRef &n):
        name (n) { kind = Kind_IdentifierExpression; }

// attributes
    QStringRef name;
};

class NumericLiteral: public Node
{
public:
    NumericLiteral(double v):
        value(v) { kind = Kind_NumericLiteral; }

// attributes:
    double value;
};

class StringLiteral: public Node
{
public:
    StringLiteral(const QString &v):
        value (v) { kind = Kind_StringLiteral; }

// attributes:
    QString value;
};

class UnaryPlusExpression: public Node
{
public:
    UnaryPlusExpression(Node *e):
        expression (e) { kind = Kind_UnaryPlusExpression; }

// attributes
    Node *expression;
};

class UnaryMinusExpression: public Node
{
public:
    UnaryMinusExpression(Node *e):
        expression (e) { kind = Kind_UnaryMinusExpression; }

// attributes
    Node *expression;
};

class BinaryExpression: public Node
{
public:
    BinaryExpression(Node *l, int o, Node *r):
        left (l), op (o), right (r)
        { kind = Kind_BinaryExpression; }

// attributes
    Node *left;
    int op;
    Node *right;
};

} // namespace XlsxAST

#endif
