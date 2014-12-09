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
#ifndef XLSXCELLREFERENCE_H
#define XLSXCELLREFERENCE_H

#include <QString>

class XlsxCellReference
{
public:
    enum Type {
        SingleCellType,
        CellRangeType,
        RowsType,
        ColumnsType,
    };

    XlsxCellReference();
    XlsxCellReference(int row, int col);
    XlsxCellReference(int firstRow, int firstCol, int lastRow, int lastCol, Type type=CellRangeType);
    XlsxCellReference(const QString &cell_A1);
    XlsxCellReference(const char *cell_A1);
    XlsxCellReference(const QString &cell_R1C1, const XlsxCellReference &baseCell);
    ~XlsxCellReference();

    QString toString() const;
    QString toString(bool isRowAbs, bool isColAbs) const;
    QString toString(bool isRow1Abs, bool isCol1Abs, bool isRow2Abs, bool isCol2Abs) const;
    QString toR1C1String(const XlsxCellReference &baseCell) const;
    static XlsxCellReference fromR1C1String(const QString &cell, const XlsxCellReference &baseCell);
    static XlsxCellReference fromRows(int firstRow, int lastRow);
    static XlsxCellReference fromColumns(int firstColumn, int lastColumn);
    bool isNull() const;

    inline int row() const { return m_row1; }
    inline int column() const { return m_col1; }
    inline int firstRow() const { return m_row1; }
    inline int firstColumn() const { return m_col1; }
    inline int lastRow() const { return m_row2; }
    inline int lastColumn() const { return m_col2; }
    inline int rowCount() const { return m_row2 - m_row1 + 1; }
    inline int columnCount() const { return m_col2 - m_col1 + 1; }
    inline Type type() const {return m_type; }
    inline QString sheetName() const {return m_sheetName; }

    bool operator ==(const XlsxCellReference &other) const;
    bool operator !=(const XlsxCellReference &other) const;

private:
    void initFromA1(const QString &cell);
    void initFromR1C1(const QString &cell, const XlsxCellReference &baseCell);
    int m_row1;
    int m_col1;
    int m_row2;
    int m_col2;
    bool m_isRow1Abs;
    bool m_isCol1Abs;
    bool m_isRow2Abs;
    bool m_isCol2Abs;
    QString m_sheetName;
    Type m_type;
};

#endif // XLSXCELLREFERENCE_H
