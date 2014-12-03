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

#ifndef XLSXCELLDATA_H
#define XLSXCELLDATA_H

#include <QString>
#include <QVariant>
#include <QHash>

class XlsxCellData
{
public:
    enum Type {
        T_String,
        T_Numeric,
        T_Boolean,
        T_Error
    };

    XlsxCellData(const QString &text=QString(), Type type=T_String);
    XlsxCellData(double val, Type type = T_Numeric);
    XlsxCellData(const XlsxCellData &other);
    XlsxCellData &operator=(const XlsxCellData &rhs);

    Type cellType() const { return type;}
    bool isNumeric() const { return type == T_Numeric; }
    bool isBoolean() const { return type == T_Boolean; }
    bool isString() const { return type == T_String; }
    bool isError() const { return type == T_Error; }
    bool canConvertToNumeric() const;

    QVariant cellValue() const { return val; }
    double doubleValue() const;
    bool booleanValue() const;
    QString stringValue() const;

    bool operator == (const XlsxCellData &other) const;
    bool operator != (const XlsxCellData &other) const;

private:
    Type type;
    QVariant val;
};

Q_DECLARE_METATYPE(XlsxCellData)
#endif // XLSXCELLDATA_H
