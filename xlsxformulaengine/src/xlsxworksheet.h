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

#ifndef XLSXWORKSHEET_H
#define XLSXWORKSHEET_H

#include <QString>
#include <QVariant>
#include <QHash>

class XlsxCellData
{
public:
    enum Type {
        T_String,
        T_Numric,
        T_Boolean,
        T_Error
    };

    XlsxCellData(const QString &text=QString(), Type type=T_String)
        :type(type), val(text)
    {}

    XlsxCellData(double val)
        :type(T_Numric), val(val)
    {}
    XlsxCellData(bool val)
        :type(T_Boolean), val(val)
    {}

    Type cellType() const { return type;}
    bool isNumeric() const { return type == T_Numric; }
    bool isBoolean() const { return type == T_Boolean; }
    bool isString() const { return type == T_String; }
    bool isError() const { return type == T_Error; }

    QVariant cellValue() const { return val; }
    double doubleValue() const { return val.toDouble(); }
    bool booleanValue() const { return val.toBool(); }
    QString stringValue() const {return val.toString(); }

private:
    Type type;
    QVariant val;
};

class XlsxWorksheet
{
public:
    XlsxWorksheet();

    void addCell(const QString &cellRef, const XlsxCellData &data);
    XlsxCellData cellAt(const QString &cellRef);

    void defineName(const QString &name, const QString &formula);

private:
    QHash<QString, XlsxCellData> cellTable;
    QHash<QString, QString> definedNames;
};

#endif // XLSXWORKSHEET_H
