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
#include <QMap>

class XlsxCellData;
class XlsxCellReference;
class XlsxWorkbook;

class XlsxWorksheet
{
public:
    XlsxWorksheet(const QString &name, XlsxWorkbook *book=0);
    QString sheetName() const {return name;}

    void addCell(const XlsxCellReference &cellRef, const XlsxCellData &data);
    XlsxCellData cellAt(const XlsxCellReference &cellRef) const;

private:
    QMap<int, QMap<int, XlsxCellData> > cellTable;
    XlsxWorkbook *book;
    QString name;
};

#endif // XLSXWORKSHEET_H
