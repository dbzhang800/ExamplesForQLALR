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

#include "xlsxformulaengine.h"
#include "xlsxformulaparser_p.h"

#include <QVariant>

XlsxFormulaEngine::XlsxFormulaEngine(QObject *parent) :
    QObject(parent)
{
}

QVariant XlsxFormulaEngine::evaluate(const QString &formula)
{
    XlsxFormulaParser p;
    p.parse(formula.toUtf8());

    return p.sym(1).dval;
}

QString XlsxFormulaEngine::errorMessage() const
{
    return QString();
}
