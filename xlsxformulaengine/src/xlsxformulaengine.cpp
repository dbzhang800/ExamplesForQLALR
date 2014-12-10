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
#include "xlsxworkbook.h"

#include <QRegularExpression>

#include <math.h>

XlsxFormulaEnginePrivate::XlsxFormulaEnginePrivate(XlsxFormulaEngine *q)
    :q_ptr(q)
{
}

/*!
 * \class XlsxFormulaEngine
 */

XlsxFormulaEngine::XlsxFormulaEngine(XlsxWorkbook *book, QObject *parent) :
    QObject(parent), d_ptr(new XlsxFormulaEnginePrivate(this))
{
    d_ptr->book = book;
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

XlsxCellData XlsxFormulaEngine::evaluate(const QString &formula, const XlsxCellReference &cell, XlsxWorksheet *sheet)
{
    Q_D(XlsxFormulaEngine);
    d->errorString = QString();//null string means no error.
    if (sheet) {
        d->sheet = sheet;
    } else if (d->book) {
        if (d->book->sheet(cell.sheetName()))
            d->sheet = d->book->sheet(cell.sheetName());
        else
            d->sheet = d->book->currentSheet();
    } else {
        d->sheet = 0;
    }
    d->cellRef = cell;

    XlsxFormulaParser parser(formula);
    if (!parser.parse()) {
        d->errorString = "parser formula failed.";
        return XlsxCellData();
    }

    XlsxCellData data = d->evalAst(parser.sym(1).Node);

    //Return 0 for formula when empty cell is used.
    if (data.isNull() && !d->errorString.isEmpty())
        return XlsxCellData(0);

    return data;
}

XlsxCellData XlsxFormulaEnginePrivate::evalAst(XlsxAST::Node *node)
{
    if (!errorString.isEmpty())
        return XlsxCellData();

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
    case XlsxAST::Node::Kind_LogicalConstantLiteral: {
        QString text = *(static_cast<XlsxAST::LogicalConstantLiteral *>(node)->value);
        return XlsxCellData(text.toUpper()==QStringLiteral("TRUE"), XlsxCellData::T_Boolean);
    }
    case XlsxAST::Node::Kind_IdentifierExpression: {
        return evalIdentifierExpression(static_cast<XlsxAST::IdentifierExpression *>(node));
    }
    case XlsxAST::Node::Kind_UnaryPlusExpression: {
        return evalAst(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression);
    }
    case XlsxAST::Node::Kind_UnaryMinusExpression: {
        XlsxCellData data = evalAst(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression);
        if (data.canConvertToNumeric())
            return XlsxCellData(-data.doubleValue());
        else
            return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    }
    case XlsxAST::Node::Kind_UnaryPercentExpression: {
        XlsxCellData data = evalAst(static_cast<XlsxAST::UnaryPlusExpression *>(node)->expression);
        if (data.canConvertToNumeric())
            return XlsxCellData(data.doubleValue()/100.0);
        else
            return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    }
    case XlsxAST::Node::Kind_BinaryArithmeticExpression: {
        XlsxAST::BinaryExpressionNode *binNode = static_cast<XlsxAST::BinaryExpressionNode *>(node);
        XlsxCellData left = evalAst(binNode->left);
        if (left.isError())
            return left;
        XlsxCellData right = evalAst(binNode->right);
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
        XlsxCellData left = evalAst(binNode->left);
        if (left.isError())
            return left;
        XlsxCellData right = evalAst(binNode->right);
        if (right.isError())
            return right;

        return XlsxCellData(QString("%1%2").arg(left.stringValue()).arg(right.stringValue()));
    }
    case XlsxAST::Node::Kind_BinaryComparisonExpression: {
        XlsxAST::BinaryExpressionNode *binNode = static_cast<XlsxAST::BinaryExpressionNode *>(node);
        XlsxCellData left = evalAst(binNode->left);
        if (left.isError())
            return left;
        XlsxCellData right = evalAst(binNode->right);
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
            XlsxCellData arg1 = evalAst(call->arguments->expression);
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

/*
 * Implicit intersection should be performed on except for those that allow a range.
 */
XlsxCellData XlsxFormulaEnginePrivate::getCellDataAt(const XlsxCellReference &cell)
{
    XlsxCellReference singleCell;
    if (cell.type() == XlsxCellReference::SingleCellType) {
        singleCell = cell;
    } else if (cell.type() == XlsxCellReference::RowsType && cell.rowCount() == 1) {
        singleCell = XlsxCellReference(cell.row(), this->cellRef.column());
    } else if (cell.type() == XlsxCellReference::ColumnsType && cell.columnCount() == 1) {
        singleCell = XlsxCellReference(this->cellRef.row(), cell.column());

    } else if (cell.type() == XlsxCellReference::CellRangeType) {
        if (cell.columnCount() == 1 && this->cellRef.row() >= cell.firstRow()
                && this->cellRef.row() <= cell.lastRow()) {
            singleCell = XlsxCellReference(this->cellRef.row(), cell.column());
        }
        if (cell.rowCount() == 1 && this->cellRef.column() >= cell.firstColumn()
                && this->cellRef.column() <= cell.lastColumn()) {
            singleCell = XlsxCellReference(cell.row(), this->cellRef.column());
        }
    }

    if (!singleCell.isNull() && book)
        return book->cellAt(singleCell, sheet);

    return XlsxCellData("#VALUE!", XlsxCellData::T_Error);
}

XlsxCellData XlsxFormulaEnginePrivate::evalIdentifierExpression(XlsxAST::IdentifierExpression *idExp)
{
    QString text = idExp->name->toUpper();

    //Try to find whether it's a cellReference
    XlsxCellReference cellRef(text);
    if (!cellRef.isNull())
        return getCellDataAt(cellRef);

    //Try to find whether it's a NAME
    if (book) {
        QString formula = book->getDefinedNameFormula(text);
        if (!formula.isEmpty()) {
            XlsxFormulaEngine engine(book);
            return engine.evaluate(formula, XlsxCellReference());
        }
    }

    //Invalid NAME
    return XlsxCellData("#NAME?", XlsxCellData::T_Error);
}
