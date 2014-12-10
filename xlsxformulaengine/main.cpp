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
#include "xlsxworksheet.h"
#include "xlsxworkbook.h"
#include "xlsxcellreference.h"
#include <QtTest>

class FormulaEngineTest : public QObject
{
    Q_OBJECT

public:
    FormulaEngineTest();

private Q_SLOTS:

    //test operators which are independent of any sheet
    void testOperator_data();
    void testOperator();

    //cell reference in one sheet.
    void testSimpleCellReference_data();
    void testSimpleCellReference();

    void testImplicitIntersection();

    void testCrossSheetsCellReference();
};

FormulaEngineTest::FormulaEngineTest()
{
}

void FormulaEngineTest::testOperator_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<XlsxCellData>("result");

    //Simple
    QTest::newRow("number1") << "-1.222E+2" << XlsxCellData(-122.2);
    QTest::newRow("number2") << "1.E+2" << XlsxCellData(100);
    QTest::newRow("number2") << "100e-2" << XlsxCellData(1);

    QTest::newRow("+") << "1+2+3++2" << XlsxCellData(8);
    QTest::newRow("-") << "1-3----1" << XlsxCellData(-1);
    QTest::newRow("*") << "1+2*3" << XlsxCellData(7);
    QTest::newRow("/") << "10+2/4" << XlsxCellData(10.5);
    QTest::newRow("exp1") << "-3^2" << XlsxCellData(9);
    QTest::newRow("exp2") << "0-3^2" << XlsxCellData(-9);
    QTest::newRow("exp3") << "-3^2*2^3" << XlsxCellData(72);

    QTest::newRow("space1")<< " 1 + 2   " << XlsxCellData(3.0);
    QTest::newRow("space2")<< " + 1 + - -  2  " << XlsxCellData(3.0);
    QTest::newRow("space3")<< " + ( 1 )+ - -  2 " << XlsxCellData(3.0);

    QTest::newRow("%") << "+++++20000%%+++++1" << XlsxCellData(3);
    QTest::newRow("()") << "(1+3)*4+2*(3/2)-2" << XlsxCellData(17);

    //Error.
    QTest::newRow("100/0") << "100/0" << XlsxCellData("#DIV/0!", XlsxCellData::T_Error);

    //Convert string to number if convertable.
    QTest::newRow("str2Num") << "\"1\"+\"33\"*2" << XlsxCellData(67);
    QTest::newRow("str2Num_2") << "\"1ff\"+2" << XlsxCellData("#VALUE!", XlsxCellData::T_Error);
    QTest::newRow("str2Num_3") << "-\"1000\"%" << XlsxCellData(-10);

    //Text concatenation
    QTest::newRow("textConcat1") << "11&22" << XlsxCellData("1122");
    QTest::newRow("textConcat2") << "\"Hello \"&123&-300%" << XlsxCellData("Hello 123-3");
    QTest::newRow("textConcat3") << "\"Hello \"&(1<3)" << XlsxCellData("Hello TRUE");

    //Comparison
    QTest::newRow("numCmp1") << "4+1>2" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("strCmp1") << "\"111\"<\"2\"" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp1") << "\"111\">222" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp2") << "\"111\">(10<2)" << XlsxCellData(false, XlsxCellData::T_Boolean);
    QTest::newRow("mixCmp3") << "100>(10<2)" << XlsxCellData(false, XlsxCellData::T_Boolean);

    //Literal
    QTest::newRow("FALSE literal") << "FALSE" << XlsxCellData(false, XlsxCellData::T_Boolean);
    QTest::newRow("#NAME? literal") << "#NAME?" << XlsxCellData("#NAME?", XlsxCellData::T_Error);
    QTest::newRow("#DIV/0! literal") << "#DIV/0!" << XlsxCellData("#DIV/0!", XlsxCellData::T_Error);

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
    QCOMPARE(engine.evaluate(formula, XlsxCellReference("A1")), result);
}

void FormulaEngineTest::testSimpleCellReference_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<XlsxCellData>("result");

    QTest::newRow("A1") << "A1" << XlsxCellData(100);
    QTest::newRow("A1A2") << "A1*A1&A2" << XlsxCellData("10000Qt!");
    QTest::newRow("$A1A2") << "$A1*$A1&A2" << XlsxCellData("10000Qt!");
    QTest::newRow("A$1A2") << "A$1*A$1&A2" << XlsxCellData("10000Qt!");
    QTest::newRow("$A$1$A$2") << "$A$1*$A$1&$A$2" << XlsxCellData("10000Qt!");

    QTest::newRow("name") << "A1*TEST" << XlsxCellData(700);
    QTest::newRow("non-exist name") << "A1*TEST_2" << XlsxCellData("#NAME?", XlsxCellData::T_Error);

    QTest::newRow("empty cell 1") << "A3=0" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("empty cell 2") << "A3=FALSE" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("empty cell 3") << "A3=FALSE()" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("empty cell 4") << "A3=\"\"" << XlsxCellData(true, XlsxCellData::T_Boolean);
    QTest::newRow("empty cell 5") << "A3=\"0\"" << XlsxCellData(false, XlsxCellData::T_Boolean);
    QTest::newRow("empry cell 6") << "A3&\"Qt\"" << XlsxCellData("Qt");
    QTest::newRow("empry cell 7") << "A3" << XlsxCellData(0);
}

void FormulaEngineTest::testSimpleCellReference()
{
    XlsxWorkbook book;
    XlsxWorksheet *sheet = book.addSheet("Sheet1");
    sheet->addCell("A1", XlsxCellData(100));
    sheet->addCell("A2", XlsxCellData("Qt!"));

    book.defineName("TEST", "1+2*3");

    QFETCH(QString, formula);
    QFETCH(XlsxCellData, result);

    XlsxFormulaEngine engine(&book);
    QCOMPARE(engine.evaluate(formula, XlsxCellReference("D4")), result);
}

void FormulaEngineTest::testImplicitIntersection()
{
    XlsxWorkbook book;
    XlsxWorksheet *sheet = book.addSheet("Sheet1");
    sheet->addCell("A1", XlsxCellData("1,1"));
    sheet->addCell("A2", XlsxCellData("2,1"));
    sheet->addCell("A3", XlsxCellData("3,1"));
    sheet->addCell("A4", XlsxCellData("4,1"));
    sheet->addCell("B1", XlsxCellData("1,2"));
    sheet->addCell("C1", XlsxCellData("1,3"));
    sheet->addCell("D1", XlsxCellData("1,4"));
    sheet->addCell("E1", XlsxCellData("1,5"));

    XlsxFormulaEngine engine(&book);
    QCOMPARE(engine.evaluate("1:1", XlsxCellReference("D4")), XlsxCellData("1,4"));
    QCOMPARE(engine.evaluate("$1:1", XlsxCellReference("D4")), XlsxCellData("1,4"));
    QCOMPARE(engine.evaluate("A:A", XlsxCellReference("D4")), XlsxCellData("4,1"));
    QCOMPARE(engine.evaluate("1:2", XlsxCellReference("D4")), XlsxCellData("#VALUE!", XlsxCellData::T_Error));
    QCOMPARE(engine.evaluate("A1:A4", XlsxCellReference("D4")), XlsxCellData("4,1"));
    QCOMPARE(engine.evaluate("$A$1:A4", XlsxCellReference("D4")), XlsxCellData("4,1"));
    QCOMPARE(engine.evaluate("A1:B4", XlsxCellReference("D4")), XlsxCellData("#VALUE!", XlsxCellData::T_Error));
}

void FormulaEngineTest::testCrossSheetsCellReference()
{
    XlsxWorkbook book;
    XlsxWorksheet *sheet1 = book.addSheet("Sheet1");
    sheet1->addCell("A1", XlsxCellData("Sheet1 1,1"));
    sheet1->addCell("A2", XlsxCellData("Sheet1 2,1"));

    XlsxWorksheet *sheet2 = book.addSheet("Sheet2");
    sheet2->addCell("B1", XlsxCellData(10));
    sheet2->addCell("B2", XlsxCellData(20));

    XlsxWorksheet *sheet3 = book.addSheet("#Sheet+-!!!!");
    sheet3->addCell("C1", XlsxCellData("Valid Sheet Name"));

    XlsxFormulaEngine engine(&book);
    book.setCurrentSheet("Sheet1");
    QCOMPARE(engine.evaluate("Sheet2!B1", XlsxCellReference("D4")), XlsxCellData(10));
    QCOMPARE(engine.evaluate("Sheet2!B1+Sheet2!B2", XlsxCellReference("D4")), XlsxCellData(30));
    QCOMPARE(engine.evaluate("A1 &Sheet2!B2", XlsxCellReference("D4")), XlsxCellData("Sheet1 1,120"));
    QCOMPARE(engine.evaluate("Sheet1!A1 &Sheet2!B2", XlsxCellReference("D4")), XlsxCellData("Sheet1 1,120"));

    book.setCurrentSheet("Sheet2");
    QCOMPARE(engine.evaluate("Sheet1!A1", XlsxCellReference("D4")), XlsxCellData("Sheet1 1,1"));
    QCOMPARE(engine.evaluate("A1&Sheet1!A1", XlsxCellReference("D4")),  XlsxCellData("Sheet1 1,1"));
    QCOMPARE(engine.evaluate("A1", XlsxCellReference("Sheet1!D4")),  XlsxCellData("Sheet1 1,1"));
    QCOMPARE(engine.evaluate("\'#Sheet+-!!!!\'!C1", XlsxCellReference("D4")),  XlsxCellData("Valid Sheet Name"));
    QCOMPARE(engine.evaluate("\'#Sheet+-!!!!\'!EE11", XlsxCellReference("D4")),  XlsxCellData(0));

    QCOMPARE(engine.evaluate("NonExistingSheet!EE11", XlsxCellReference("D4")),  XlsxCellData("#REF!", XlsxCellData::T_Error));
}

QTEST_APPLESS_MAIN(FormulaEngineTest)

#include "main.moc"
