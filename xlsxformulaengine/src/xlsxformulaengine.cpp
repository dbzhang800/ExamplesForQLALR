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

#include "xlsxformulaengine_p.h"
#include "xlsxformulaparser_p.h"
#include "xlsxast_p.h"
#include "xlsxcelldata.h"
#include "xlsxworksheet.h"

#include <QRegularExpression>

XlsxFormulaEnginePrivate::XlsxFormulaEnginePrivate(XlsxFormulaEngine *q)
    :q_ptr(q)
{
}

/*!
 * \class XlsxFormulaEngine
 */

XlsxFormulaEngine::XlsxFormulaEngine(XlsxWorksheet *sheet, QObject *parent) :
    QObject(parent), d_ptr(new XlsxFormulaEnginePrivate(this))
{
    d_ptr->sheet = sheet;
}

XlsxFormulaEngine::~XlsxFormulaEngine()
{
    delete d_ptr;
}

bool XlsxFormulaEngine::hasError() const
{
    return errorString().isNull();
}

QString XlsxFormulaEngine::errorString() const
{
    Q_D(const XlsxFormulaEngine);
    return d->errorString;
}

XlsxCellData XlsxFormulaEngine::evaluate(const QString &formula, const QString &cellRef)
{
    Q_D(XlsxFormulaEngine);
    d->errorString = QString();//null string means no error.

    XlsxFormulaParser parser(formula);
    if (!parser.parse()) {
        d->errorString = "parser formula failed.";
        return XlsxCellData();
    }

    XlsxCellData data = d->interpret(parser.sym(1).Node, cellRef);

    return data;
}

XlsxCellData XlsxFormulaEnginePrivate::interpret(XlsxAST::Node *node, const QString &cellRef)
{
    switch (node->kind) {
//    case XlsxAST::Node::Kind_Node:
//    case XlsxAST::Node::Kind_ExpressionNode:
//    case XlsxAST::Node::Kind_BinaryExpressionNode:
//        break;
    case XlsxAST::Node::Kind_NumericLiteral: {
        double val = static_cast<XlsxAST::NumericLiteral *>(node)->value;
        return XlsxCellData(val);
    }
    case XlsxAST::Node::Kind_StringLiteral: {
        QString text = *(static_cast<XlsxAST::StringLiteral *>(node)->value);
        return XlsxCellData(text, XlsxCellData::T_String);
    }
    case XlsxAST::Node::Kind_ErrorConstantLiteral: {
        QString text = *(static_cast<XlsxAST::ErrorConstantLiteral *>(node)->value);
        return XlsxCellData(text, XlsxCellData::T_Error);
    }
    case XlsxAST::Node::Kind_IdentifierExpression: {
        //TODO
        QRegularExpression re(QStringLiteral("^\\$?([A-Z]{1,3})\\$?(\\d+)$"));
        QString text = static_cast<XlsxAST::IdentifierExpression *>(node)->name->toUpper();
        if (text == "TRUE") {
            return XlsxCellData(true, XlsxCellData::T_Boolean);
        } else if (text == "FALSE") {
            return XlsxCellData(false, XlsxCellData::T_Boolean);
        } else if (re.match(text).hasMatch()) {
            return sheet->cellAt(text);
//        } else if (...) {
//            return sheet->cellAt(text);
        } else {
            if (sheet->hasDefinedName(text))
                return q_ptr->evaluate(sheet->getDefinedNameFormula(text));

            return XlsxCellData("#NAME?", XlsxCellData::T_Error);
        }
    }
    case XlsxAST::Node::Kind_UnaryPlusExpression: {
        return interpret(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression, cellRef);
    }
    case XlsxAST::Node::Kind_UnaryMinusExpression: {
        XlsxCellData data = interpret(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression, cellRef);
        if (data.canConvertToNumeric())
            return XlsxCellData(-data.doubleValue());
        else
            return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    }
    case XlsxAST::Node::Kind_UnaryPercentExpression: {
        XlsxCellData data = interpret(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression, cellRef);
        if (data.canConvertToNumeric())
            return XlsxCellData(data.doubleValue()/100.0);
        else
            return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    }
    case XlsxAST::Node::Kind_BinaryArithmeticExpression: {
        XlsxAST::BinaryExpressionNode *binNode = static_cast<XlsxAST::BinaryExpressionNode *>(node);
        XlsxCellData left = interpret(binNode->left, cellRef);
        if (left.isError())
            return left;
        XlsxCellData right = interpret(binNode->right, cellRef);
        if (right.isError())
            return right;

        if (!left.canConvertToNumeric() || !right.canConvertToNumeric())
            return XlsxCellData("#VALUE!", XlsxCellData::T_Error);

        switch (binNode->op) {
        case XlsxAST::Add:
            return XlsxCellData(left.doubleValue() + right.doubleValue());
        case XlsxAST::Sub:
            return XlsxCellData(left.doubleValue() - right.doubleValue());
        case XlsxAST::Mul:
            return XlsxCellData(left.doubleValue() * right.doubleValue());
        case XlsxAST::Div:
            if (!right.doubleValue())
                return XlsxCellData("#DIV/0!", XlsxCellData::T_Error);
            return XlsxCellData(left.doubleValue() / right.doubleValue());
        case XlsxAST::Exp:
            return XlsxCellData(pow(left.doubleValue(), right.doubleValue()));
        default:
            break;
        }
    }
    case XlsxAST::Node::Kind_BinaryTextExpression: {
        XlsxAST::BinaryExpressionNode *binNode = static_cast<XlsxAST::BinaryExpressionNode *>(node);
        XlsxCellData left = interpret(binNode->left, cellRef);
        if (left.isError())
            return left;
        XlsxCellData right = interpret(binNode->right, cellRef);
        if (right.isError())
            return right;

        return XlsxCellData(QString("%1%2").arg(left.stringValue()).arg(right.stringValue()));
    }
    case XlsxAST::Node::Kind_BinaryComparisonExpression: {
        XlsxAST::BinaryExpressionNode *binNode = static_cast<XlsxAST::BinaryExpressionNode *>(node);
        XlsxCellData left = interpret(binNode->left, cellRef);
        if (left.isError())
            return left;
        XlsxCellData right = interpret(binNode->right, cellRef);
        if (right.isError())
            return right;
        switch (binNode->op) {
        case XlsxAST::Lt:
            return XlsxCellData(left < right, XlsxCellData::T_Boolean);
        case XlsxAST::Le:
            return XlsxCellData((left < right || left == right), XlsxCellData::T_Boolean);
        case XlsxAST::Gt:
            return XlsxCellData(right < left, XlsxCellData::T_Boolean);
        case XlsxAST::Ge:
            return XlsxCellData((right < left || left == right), XlsxCellData::T_Boolean);
        case XlsxAST::Equal:
            return XlsxCellData(left == right, XlsxCellData::T_Boolean);
        case XlsxAST::NotEqual:
            return XlsxCellData(left != right, XlsxCellData::T_Boolean);
        default:
            break;
        }

        return XlsxCellData(QString("%1%2").arg(left.stringValue()).arg(right.stringValue()));
    }
    case XlsxAST::Node::Kind_CallExpression: {
        XlsxAST::CallExpression *call = static_cast<XlsxAST::CallExpression *>(node);
        QString callName = call->name->toUpper();
        if (callName == "PI") {
            return XlsxCellData(3.14159265358979); //should we use M_PI here?
        } else if (callName == "TRUE") {
            return XlsxCellData(true, XlsxCellData::T_Boolean);
        } else if (callName == "FALSE") {
            return XlsxCellData(false, XlsxCellData::T_Boolean);
        } else if (callName == "NOT") {
            XlsxCellData arg1 = interpret(call->arguments->expression, cellRef);
            if (arg1.isError())
                return arg1;
            if (arg1.isString())
                return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
            return XlsxCellData(!arg1.booleanValue(), XlsxCellData::T_Boolean);
        }
    }
    default:
        break;
    }

    return XlsxCellData();
}
