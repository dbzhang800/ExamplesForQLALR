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
#include "xlsxcelldata.h"
#include <QtTest>

class FormulaEngineTest : public QObject
{
    Q_OBJECT

public:
    FormulaEngineTest();

private Q_SLOTS:
    void testOperator_data();
    void testOperator();
};

FormulaEngineTest::FormulaEngineTest()
{
}

void FormulaEngineTest::testOperator_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<XlsxCellData>("result");

    //Simple
    QTest::newRow("+")     << "1+2+3++2"    << XlsxCellData(8);
    QTest::newRow("-")     << "1-3----1"    << XlsxCellData(-1);
    QTest::newRow("*")     << "1+2*3"       << XlsxCellData(7);
    QTest::newRow("/")     << "10+2/4"      << XlsxCellData(10.5);
    QTest::newRow("exp1")  << "-3^2"        << XlsxCellData(9);
    QTest::newRow("exp2")  << "0-3^2"       << XlsxCellData(-9);
    QTest::newRow("exp3")  << "-3^2*2^3"    << XlsxCellData(72);

    QTest::newRow("space1")<< " 1 + 2   "          << XlsxCellData(3.0);
    QTest::newRow("space2")<< " + 1 + - -  2  "    << XlsxCellData(3.0);
    QTest::newRow("space3")<< " + ( 1 )+ - -  2 "  << XlsxCellData(3.0);

    QTest::newRow("%")     << "+++++20000%%+++++1" << XlsxCellData(3);
    QTest::newRow("()")    << "(1+3)*4+2*(3/2)-2"  << XlsxCellData(17);

    //Convert string to number if convertable.
    QTest::newRow("str2Num")     << "\"1\"+\"33\"*2" << XlsxCellData(67);
    QTest::newRow("str2Num_2")   << "\"1ff\"+2"      << XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    QTest::newRow("str2Num_3")   << "-\"1000\"%"     << XlsxCellData(-10);

    //Text concatenation
    QTest::newRow("textConcat1") << "11&22"                << XlsxCellData("1122");
    QTest::newRow("textConcat2") << "\"Hello \"&123&-300%" << XlsxCellData("Hello 123-3");
    QTest::newRow("textConcat3") << "\"Hello \"&(1<3)"     << XlsxCellData("Hello TRUE");

    //Comparison
    QTest::newRow("numCmp1") << "4+1>2"          << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("strCmp1") << "\"111\"<\"2\""  << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp1") << "\"111\">222"    << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp2") << "\"111\">(10<2)" << XlsxCellData(false, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp3") << "100>(10<2)" << XlsxCellData(false, XlsxCellData::T_Boolean);

    //Funcation call
    QTest::newRow("NOT") << "NOT(0)" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("NOT 2") << "\"result \"&NOT(0)" << XlsxCellData("result TRUE");
    QTest::newRow("TRUE") << "TRUE()" << XlsxCellData(true, XlsxCellData::T_Boolean);
}

void FormulaEngineTest::testOperator()
{
    QFETCH(QString, formula);
    QFETCH(XlsxCellData, result);

    XlsxFormulaEngine engine(0);
    QCOMPARE(engine.evaluate(formula), result);
}

QTEST_APPLESS_MAIN(FormulaEngineTest)

#include "main.moc"
