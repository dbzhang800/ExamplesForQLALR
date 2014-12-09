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
#include "xlsxcellreference.h"

XlsxWorksheet::XlsxWorksheet(XlsxWorkbook *book)
    :book(book)
{
}

void XlsxWorksheet::addCell(const XlsxCellReference &cellRef, const XlsxCellData &data)
{
    cellTable[cellRef.row()][cellRef.column()] = data;
}

XlsxCellData XlsxWorksheet::cellAt(const XlsxCellReference &cellRef) const
{
    if (cellTable.contains(cellRef.row()) && cellTable[cellRef.row()].contains(cellRef.column()))
        return cellTable[cellRef.row()][cellRef.column()];

    return XlsxCellData();
}
