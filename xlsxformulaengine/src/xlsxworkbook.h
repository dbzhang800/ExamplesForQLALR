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

#ifndef XLSXWORKBOOK_H
#define XLSXWORKBOOK_H
#include <QString>
#include <QHash>
#include <QMap>
#include <QStringList>

class XlsxWorksheet;
class XlsxCellData;
class XlsxCellReference;

class XlsxWorkbook
{
public:
    XlsxWorkbook();
    ~XlsxWorkbook();

    XlsxWorksheet *addSheet(const QString &sheetName);
    XlsxWorksheet *sheet(const QString &sheetName) const;
    QString sheetName(XlsxWorksheet *sheet) const;
    void setCurrentSheet(const QString &sheetName);
    XlsxWorksheet *currentSheet() const;

    XlsxCellData cellAt(const XlsxCellReference &cellRef, XlsxWorksheet *defaultSheet=0) const;

    void defineName(const QString &name, const QString &formula, const QString &scope=QString());
    QString getDefinedNameFormula(const QString &name, const QString &scope=QString()) const;
    QString getDefinedNameFormula(const QString &name, XlsxWorksheet *scope) const;

private:
    struct DefineNameData
    {
        DefineNameData() {}
        DefineNameData(const QString &name, const QString &formula, const QString &scope)
            :name(name), formula(formula), scope(scope)
        {}
        QString name;
        QString formula;
        QString scope;
    };

    QList<XlsxWorksheet *> sheets;
    QStringList sheetNames;
    int currentSheetIdx;
    QHash<QString, DefineNameData> definedNames;
};

#endif // XLSXWORKBOOK_H
