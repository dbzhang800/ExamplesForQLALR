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
#ifndef XLSXFORMULAINTERPRETER_P_H
#define XLSXFORMULAINTERPRETER_P_H

#include "xlsxast_p.h"
#include "xlsxworksheet.h"

class XlsxFormulaEnginePrivate;

class XlsxFormulaInterpreter
{
public:
    XlsxFormulaInterpreter(XlsxFormulaEnginePrivate *engine);
    XlsxCellData interpret(XlsxAST::Node *node);

private:
    XlsxFormulaEnginePrivate *m_engine;
};

#endif // XLSXFORMULAINTERPRETER_P_H
