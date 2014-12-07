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
#ifndef XLSXFORMULAENGINE_P_H
#define XLSXFORMULAENGINE_P_H

#include "xlsxformulaengine.h"
#include "xlsxmemorypool_p.h"
#include "xlsxast_p.h"
#include "xlsxcellreference.h"

class XlsxWorksheet;
class XlsxFormulaEnginePrivate
{
    Q_DECLARE_PUBLIC(XlsxFormulaEngine)
public:
    XlsxFormulaEnginePrivate(XlsxFormulaEngine *q);

    XlsxCellData evalAst(XlsxAST::Node *node);

    XlsxWorksheet *sheet;
    XlsxCellReference cellRef;
    QString errorString;

private:
    XlsxCellData getCellDataAt(const XlsxCellReference &cell);
    XlsxCellData evalIdentifierExpression(XlsxAST::IdentifierExpression *idExp);
    XlsxCellData evalCellReferenceExpression(XlsxAST::CellReferenceExpression *cellExp);
    XlsxFormulaEngine * q_ptr;
};

#endif // XLSXFORMULAENGINE_P_H
