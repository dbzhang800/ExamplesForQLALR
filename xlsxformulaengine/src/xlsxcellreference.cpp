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
#include "xlsxcellreference.h"

#include <QRegularExpression>

namespace {

int intPow(int x, int p)
{
    if (p == 0) return 1;
    if (p == 1) return x;

    int tmp = intPow(x, p/2);
    if (p%2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}

QString col_to_name(int col_num)
{
    QString col_str;
    int remainder;
    while (col_num) {
        remainder = col_num % 26;
        if (remainder == 0)
            remainder = 26;
        col_str.prepend(QChar('A'+remainder-1));
        col_num = (col_num - 1) / 26;
    }
    return col_str;
}

int col_from_name(const QString &col_str)
{
    int col = 0;
    int expn = 0;
    for (int i=col_str.size()-1; i>-1; --i) {
        col += (col_str[i].unicode() - 'A' + 1) * intPow(26, expn);
        expn++;
    }

    return col;
}

QString getEscapedSheetName(const QString &name)
{
    if (name.contains(QRegularExpression("[\\+\\-%&=<>]#!")))
        return QStringLiteral("\'%1\'").arg(name);
    return name;
}
} //namespace

/*!
  \class XlsxCellReference

  "A1", "$A1", "A$1", "$A$1"
  "1:1", "$1:1", "1:$1", "$1:$1"
  "A:A", "$A:A", "A:$A", "$A:$A"
*/

/*!
    Constructs an invalid Cell Reference
*/
XlsxCellReference::XlsxCellReference()
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
}

/*!
    Constructs a single Cell Reference
*/
XlsxCellReference::XlsxCellReference(int row, int col)
    : m_row1(row), m_col1(col), m_row2(row), m_col2(col)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{

}

/*!
    Constructs a Cell range Reference
*/
XlsxCellReference::XlsxCellReference(int firstRow, int firstCol, int lastRow, int lastCol, Type flag)
    : m_row1(firstRow), m_col1(firstCol), m_row2(lastRow), m_col2(lastCol)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(flag)
{

}

/*!
    \overload
    Constructs the Reference form the given \a cell string.
*/
XlsxCellReference::XlsxCellReference(const QString &cell)
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
    initFromA1(cell);
}

/*!
    \overload
    Constructs the Reference form the given \a cell string.
*/
XlsxCellReference::XlsxCellReference(const char *cell)
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
    initFromA1(QString::fromUtf8(cell));
}

XlsxCellReference::XlsxCellReference(const QString &cell_R1C1, const XlsxCellReference &baseCell)
    : m_row1(-1), m_col1(-1), m_row2(-2), m_col2(-2)
    , m_isRow1Abs(false), m_isCol1Abs(false), m_isRow2Abs(false), m_isCol2Abs(false)
    , m_type(SingleCellType)
{
    initFromR1C1(cell_R1C1, baseCell);
}

void XlsxCellReference::initFromA1(const QString &cell_str)
{
    int idx = cell_str.lastIndexOf(QLatin1Char('!'));
    if (idx != -1) {
        m_sheetName = cell_str.left(idx);
        if (m_sheetName.startsWith(QLatin1Char('\'')))
            m_sheetName = m_sheetName.mid(1, m_sheetName.size()-2);
    }

    QRegularExpression pattern_A1(QStringLiteral("^(\\$?)([A-Z]{1,3})(\\$?)(\\d+)$"));
    QRegularExpressionMatch match = pattern_A1.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = SingleCellType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isRow1Abs = !match.captured(3).isEmpty();
        m_row1 = match.captured(4).toInt();
        return;
    }

    QRegularExpression pattern_A1A1(QStringLiteral("^(\\$?)([A-Z]{1,3})(\\$?)(\\d+):(\\$?)([A-Z]{1,3})(\\$?)(\\d+)$"));
    match = pattern_A1A1.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = CellRangeType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isRow1Abs = !match.captured(3).isEmpty();
        m_row1 = match.captured(4).toInt();
        m_isCol2Abs = !match.captured(5).isEmpty();
        m_col2 = col_from_name(match.captured(6));
        m_isRow2Abs = !match.captured(7).isEmpty();
        m_row2 = match.captured(8).toInt();
        return;
    }

    QRegularExpression pattern_AA(QStringLiteral("^(\\$?)([A-Z]{1,3}):(\\$?)([A-Z]{1,3})$"));
    match = pattern_AA.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = ColumnsType;
        m_isCol1Abs = !match.captured(1).isEmpty();
        m_col1 = col_from_name(match.captured(2));
        m_isCol2Abs = !match.captured(3).isEmpty();
        m_col2 = col_from_name(match.captured(4));
        return;
    }

    QRegularExpression pattern_11(QStringLiteral("^(\\$?)(\\d+):(\\$?)(\\d+)$"));
    match = pattern_11.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = RowsType;
        m_isRow1Abs = !match.captured(1).isEmpty();
        m_row1 = match.captured(2).toInt();
        m_isRow2Abs = !match.captured(3).isEmpty();
        m_row2 = match.captured(4).toInt();
        return;
    }

    //Invalid string pattern
}

void XlsxCellReference::initFromR1C1(const QString &cell_str, const XlsxCellReference &baseCell)
{
    int idx = cell_str.lastIndexOf(QLatin1Char('!'));
    if (idx != -1) {
        m_sheetName = cell_str.left(idx);
        if (m_sheetName.startsWith(QLatin1Char('\'')))
            m_sheetName = m_sheetName.mid(1, m_sheetName.size()-2);
    }

    QRegularExpression pattern_R1C1(QStringLiteral("^R(\\[?)(\\d*)(\\]?)C(\\[?)(\\d*)(\\]?)$"));
    QRegularExpressionMatch match = pattern_R1C1.match(cell_str.mid(idx+1));
    if (match.hasMatch()) {
        m_type = SingleCellType;
        m_isCol1Abs = match.captured(1).isEmpty() && !match.captured(2).isEmpty();
        m_col1 = match.captured(2).toInt();
        if (!m_isCol1Abs)
            m_col1 += baseCell.m_col1;
        m_isRow1Abs = match.captured(4).isEmpty() && !match.captured(5).isEmpty();
        m_row1 = match.captured(5).toInt();
        if (!m_isRow1Abs)
            m_row1 += baseCell.m_row1;
        return;
    }

    //TODO
    //Invalid string pattern
}

/*!
    Destroys the Reference.
*/
XlsxCellReference::~XlsxCellReference()
{
}

QString XlsxCellReference::toString() const
{
     return toString(m_isRow1Abs, m_isCol1Abs, m_isRow2Abs, m_isCol2Abs);
}

QString XlsxCellReference::toString(bool isRowAbs, bool isColAbs) const
{
    return toString(isRowAbs, isColAbs, isRowAbs, isColAbs);
}

/*!
     Convert the Reference to string notation, such as "A1" or "$A$1".
     If current object is invalid, an empty string will be returned.
*/
QString XlsxCellReference::toString(bool isRow1Abs, bool isCol1Abs, bool isRow2Abs, bool isCol2Abs) const
{
    if (!isNull())
        return QString();
    QStringList items;
    //sheetName
    if (!m_sheetName.isEmpty()) {
        items.append(getEscapedSheetName(m_sheetName));
        items.append(QStringLiteral("!"));
    }
    //col1
    if (m_type != RowsType) {
        if (isCol1Abs)
            items.append(QStringLiteral("$"));
        items.append(col_to_name(m_col1));
    }
    //row1
    if (m_type != ColumnsType) {
        if (isRow1Abs)
            items.append(QStringLiteral("$"));
        items.append(QString::number(m_row1));
    }
    //single cell or not?
    if (m_type == SingleCellType)
        return items.join(QString());
    items.append(QStringLiteral(":"));
    //col2
    if (m_type != RowsType) {
        if (isCol2Abs)
            items.append(QStringLiteral("$"));
        items.append(col_to_name(m_col2));
    }
    //row2
    if (m_type != ColumnsType) {
        if (isRow2Abs)
            items.append(QStringLiteral("$"));
        items.append(QString::number(m_row2));
    }

    return items.join(QString());
}

QString XlsxCellReference::toR1C1String(const XlsxCellReference &baseCell) const
{
    if (!isNull())
        return QString();

    QStringList items;
    //sheetName
    if (!m_sheetName.isEmpty()) {
        items.append(getEscapedSheetName(m_sheetName));
        items.append(QStringLiteral("!"));
    }
    //row1
    if (m_type != ColumnsType) {
        if (m_isRow1Abs)
            items.append(QStringLiteral("R%1").arg(m_row1));
        else
            items.append(QStringLiteral("R[%1]").arg(m_row1-baseCell.m_row1));
    }
    //col1
    if (m_type != RowsType) {
        if (m_isCol1Abs)
            items.append(QStringLiteral("C%1").arg(m_col1));
        else
            items.append(QStringLiteral("C[%1]").arg(m_col1-baseCell.m_col1));
    }

    //single cell/row/column or not?
    if ((m_type == SingleCellType)
            ||(m_type == RowsType && m_row1==m_row2 && m_isRow1Abs==m_isRow2Abs)
            ||(m_type == ColumnsType && m_col1==m_col2 && m_isCol1Abs==m_isCol2Abs)) {
        return items.join(QString());
    }

    items.append(QStringLiteral(":"));
    //row2
    if (m_type != ColumnsType) {
        if (m_isRow2Abs)
            items.append(QStringLiteral("R%1").arg(m_row2));
        else
            items.append(QStringLiteral("R[%1]").arg(m_row2-baseCell.m_row1));
    }
    //col2
    if (m_type != RowsType) {
        if (m_isCol2Abs)
            items.append(QStringLiteral("C%1").arg(m_col2));
        else
            items.append(QStringLiteral("C[%1]").arg(m_col2-baseCell.m_col1));
    }
    return items.join(QString());
}

/*!
 * Returns true if the Reference is invalid.
 */
bool XlsxCellReference::isNull() const
{
    return m_row1 < 0 && m_col1 < 0;
}

XlsxCellReference XlsxCellReference::fromR1C1String(const QString &cell, const XlsxCellReference &baseCell)
{
    return XlsxCellReference(cell, baseCell);
}

XlsxCellReference XlsxCellReference::fromRows(int firstRow, int lastRow)
{
    return XlsxCellReference(firstRow, -1, lastRow, -2, RowsType);
}

XlsxCellReference XlsxCellReference::fromColumns(int firstColumn, int lastColumn)
{
    return XlsxCellReference(-1, firstColumn, -2, lastColumn, ColumnsType);
}

bool XlsxCellReference::operator ==(const XlsxCellReference &other) const
{
    if (m_type != other.m_type)
        return false;

    if (m_type == SingleCellType)
        return m_row1==other.m_row1 && m_col1==other.m_col1;
    if (m_type == RowsType)
        return m_row1==other.m_row1 && m_row2==other.m_row2;
    if (m_type == ColumnsType)
        return m_col1==other.m_col1 && m_col2==other.m_col2;

    return m_row1==other.m_row1 && m_row2==other.m_row2 && m_col1==other.m_col1 && m_col2==other.m_col2;
}

bool XlsxCellReference::operator !=(const XlsxCellReference &other) const
{
    return !(*this == other);
}
