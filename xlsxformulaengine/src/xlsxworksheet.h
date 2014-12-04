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
#include <QHash>

class XlsxCellData;

class XlsxWorksheet
{
public:
    XlsxWorksheet();

    void addCell(const QString &cellRef, const XlsxCellData &data);
    XlsxCellData cellAt(const QString &cellRef) const;

    void defineName(const QString &name, const QString &formula);
    bool hasDefinedName(const QString &name) const;
    QString getDefinedNameFormula(const QString &name) const;

private:
    QHash<QString, XlsxCellData> cellTable;
    QHash<QString, QString> definedNames;
};

#endif // XLSXWORKSHEET_H
