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

#include "xlsxworksheet.h"
#include "xlsxcelldata.h"

XlsxWorksheet::XlsxWorksheet()
{
}

void XlsxWorksheet::addCell(const QString &cellRef, const XlsxCellData &data)
{
    cellTable[cellRef.toUpper()] = data;
}

XlsxCellData XlsxWorksheet::cellAt(const QString &cellRef) const
{
    if (cellTable.contains(cellRef.toUpper()))
        return cellTable[cellRef.toUpper()];
    return XlsxCellData();
}

void XlsxWorksheet::defineName(const QString &name, const QString &formula)
{
    definedNames[name.toUpper()] = formula;
}

bool XlsxWorksheet::hasDefinedName(const QString &name) const
{
    return definedNames.contains(name.toUpper());
}

QString XlsxWorksheet::getDefinedNameFormula(const QString &name) const
{
    if (definedNames.contains(name.toUpper()))
        return definedNames[name.toUpper()];
    return QString();
}
