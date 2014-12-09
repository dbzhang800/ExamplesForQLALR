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

#include "xlsxworkbook.h"
#include "xlsxworksheet.h"
#include "xlsxcelldata.h"
#include "xlsxcellreference.h"

XlsxWorkbook::XlsxWorkbook()
    :currentSheetIdx(-1)
{
}

XlsxWorkbook::~XlsxWorkbook()
{
    qDeleteAll(sheets);
    sheets.clear();
}

XlsxWorksheet *XlsxWorkbook::addSheet(const QString &sheetName)
{
    XlsxWorksheet *s = new XlsxWorksheet;
    sheets.append(s);
    sheetNames.append(sheetName);
    currentSheetIdx = sheetNames.length() - 1;

    return s;
}

XlsxWorksheet *XlsxWorkbook::sheet(const QString &sheetName) const
{
    int idx = sheetNames.indexOf(sheetName);
    if (idx == -1)
        return 0;
    return sheets[idx];
}

QString XlsxWorkbook::sheetName(XlsxWorksheet *sheet) const
{
    int idx = sheets.indexOf(sheet);
    if (idx == -1)
        return QString();
    return sheetNames[idx];
}

void XlsxWorkbook::setCurrentSheet(const QString &sheetName)
{
    int idx = sheetNames.indexOf(sheetName);
    if (idx != -1)
        currentSheetIdx = idx;
}

XlsxWorksheet *XlsxWorkbook::currentSheet() const
{
    if (currentSheetIdx == -1)
        return 0;
    return sheets[currentSheetIdx];
}

XlsxCellData XlsxWorkbook::cellAt(const XlsxCellReference &cellRef, XlsxWorksheet *defaultSheet) const
{
    if (!defaultSheet)
        defaultSheet = currentSheet();

    XlsxWorksheet *st = cellRef.sheetName().isEmpty() ? defaultSheet : sheet(cellRef.sheetName());
    if (st)
        return st->cellAt(cellRef);
    return XlsxCellData("#NAME!", XlsxCellData::T_Error);
}

void XlsxWorkbook::defineName(const QString &name, const QString &formula, const QString &scope)
{
    definedNames[name.toUpper()] = DefineNameData(name, formula, scope);
}

QString XlsxWorkbook::getDefinedNameFormula(const QString &name, const QString &scope) const
{
    if (!definedNames.contains(name.toUpper()))
        return QString();
    DefineNameData d = definedNames[name.toUpper()];
    if (d.scope == scope || d.scope.isEmpty())
        return d.formula;
    return QString();
}

QString XlsxWorkbook::getDefinedNameFormula(const QString &name, XlsxWorksheet *scope) const
{
    return getDefinedNameFormula(name, sheetName(scope));
}
