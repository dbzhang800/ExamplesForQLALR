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
#include "xlsxworksheet.h"
#include <QtTest>

class FormulaEngineTest : public QObject
{
    Q_OBJECT

public:
    FormulaEngineTest();

private Q_SLOTS:
    void testArithmaticOperator_data();
    void testArithmaticOperator();
};

FormulaEngineTest::FormulaEngineTest()
{
}

void FormulaEngineTest::testArithmaticOperator_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<double>("result");

    QTest::newRow("+") << "1+2+3++2" << 8.0;
    QTest::newRow("-") << "1-3--1" << -1.0;
    QTest::newRow("*") << "1+2*3" << 7.0;
    QTest::newRow("/") << "10+2/4" << 10.5;
    QTest::newRow("^") << "-3^2" << 9.0;
    QTest::newRow("^ second") << "0-3^2" << -9.0;
    QTest::newRow("^ third") << "-3^2*2^3" << 72.0;
    QTest::newRow("()") << "(1+3)*4+2*(3/2)-2" << 17.;
}

void FormulaEngineTest::testArithmaticOperator()
{
    QFETCH(QString, formula);
    QFETCH(double, result);

    XlsxFormulaEngine engine;
    QCOMPARE(engine.evaluate(formula).doubleValue(), result);
}

QTEST_APPLESS_MAIN(FormulaEngineTest)

#include "main.moc"
