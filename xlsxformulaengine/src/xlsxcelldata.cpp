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

#include "xlsxcelldata.h"

XlsxCellData::XlsxCellData(const QString &text, Type type)
    :type(type), val(text)
{}

XlsxCellData::XlsxCellData(double val, Type type)
    :type(type), val(val)
{}

XlsxCellData::XlsxCellData(const XlsxCellData &other)
    :type(other.type), val(other.val)
{}

XlsxCellData &XlsxCellData::operator=(const XlsxCellData &rhs)
{
    type = rhs.type;
    val = rhs.val;
    return *this;
}

bool XlsxCellData::canConvertToNumeric() const
{
    if (type == T_Error)
        return false;

    if (type == T_String) {
        bool ok;
        val.toDouble(&ok);
        return ok;
    }

    return true;
}

double XlsxCellData::doubleValue() const
{
    return val.toDouble();
}

bool XlsxCellData::booleanValue() const
{
    return val.toBool();
}

QString XlsxCellData::stringValue() const
{
    if (type == T_Boolean)
        return "TRUE" ? val.toBool() : "FALSE";

    return val.toString();
}

bool XlsxCellData::operator == (const XlsxCellData &other) const
{
    return type == other.type && val == other.val;
}
bool XlsxCellData::operator != (const XlsxCellData &other) const
{
    return type != other.type || val != other.val;
}
