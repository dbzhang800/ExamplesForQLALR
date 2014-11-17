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

#ifndef XLSXFORMULAENGINE_H
#define XLSXFORMULAENGINE_H

#include <QObject>

class XlsxFormulaEngine : public QObject
{
    Q_OBJECT
public:
    explicit XlsxFormulaEngine(QObject *parent = 0);

    QVariant evaluate(const QString &formula);
    QString errorMessage() const;
signals:

public slots:

};

#endif // XLSXFORMULAENGINE_H
