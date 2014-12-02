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

#include "xlsxformulaengine_p.h"
#include "xlsxformulaparser_p.h"
#include "xlsxformulainterpreter_p.h"
#include "xlsxast_p.h"
#include "xlsxworksheet.h"

XlsxFormulaEnginePrivate::XlsxFormulaEnginePrivate(XlsxFormulaEngine *q)
    :q_ptr(q), pool(0)
{

}

/*!
 * \class XlsxFormulaEngine
 */

XlsxFormulaEngine::XlsxFormulaEngine(QObject *parent) :
    QObject(parent), d_ptr(new XlsxFormulaEnginePrivate(this))
{
}

XlsxFormulaEngine::~XlsxFormulaEngine()
{
    delete d_ptr;
}

XlsxCellData XlsxFormulaEngine::evaluate(const QString &formula)
{
    Q_D(XlsxFormulaEngine);
    d->formulaString = formula;
    d->pool = new XlsxMemoryPool;

    XlsxFormulaParser parser;
    parser.parse(d);

    XlsxAST::Node *ast = parser.sym(1).Node;
    XlsxFormulaInterpreter interpreter(d);
    XlsxCellData data = interpreter.interpret(ast);

    delete d->pool;
    d->pool = 0;

    return data;
}
