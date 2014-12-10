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
        T_Numeric, //numeric < string < boolean ??
        T_String,
        T_Boolean,
        T_Error
    };

    XlsxCellData();
    XlsxCellData(const QString &text, Type type=T_String);
    XlsxCellData(double val, Type type = T_Numeric);
    XlsxCellData(const XlsxCellData &other);
    XlsxCellData &operator=(const XlsxCellData &rhs);

    Type cellType() const { return type;}
    bool isNumeric() const { return type == T_Numeric; }
    bool isBoolean() const { return type == T_Boolean; }
    bool isString() const { return type == T_String; }
    bool isError() const { return type == T_Error; }
    bool isNull() const;
    bool canConvertToNumeric() const;

    QVariant cellValue() const { return val; }
    double doubleValue() const;
    bool booleanValue() const;
    QString stringValue() const;

    operator QVariant() const;

    bool operator == (const XlsxCellData &other) const;
    bool operator != (const XlsxCellData &other) const;
    bool operator < (const XlsxCellData &other) const;

    friend QDataStream &operator>>(QDataStream &, XlsxCellData &);
private:
    Type type;
    QVariant val;
};

#if !defined(QT_NO_DATASTREAM)
QDataStream &operator<<(QDataStream &, const XlsxCellData &);
QDataStream &operator>>(QDataStream &, XlsxCellData &);
#endif

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const XlsxCellData &c);
#endif

Q_DECLARE_METATYPE(XlsxCellData)
#endif // XLSXCELLDATA_H
