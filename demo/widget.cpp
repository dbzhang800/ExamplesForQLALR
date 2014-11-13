#include "widget.h"
#include "ui_widget.h"
#include "calc_parser.h"

Widget::Widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->calculateButton, SIGNAL(clicked()), SLOT(onCalculateButtonClicked()));
    connect(ui->inputEdit, SIGNAL(textChanged(QString)), SLOT(onInputTextChanged()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onCalculateButtonClicked()
{
    CalcParser p;

    if (p.parse(ui->inputEdit->text().toUtf8())) {
        ui->resultEdit->appendPlainText(QString("%1 = %2")
                                        .arg(ui->inputEdit->text())
                                        .arg(p.sym(1)));
        ui->inputEdit->clear();
    } else {
        ui->resultEdit->appendPlainText(QString("Failed to parse: %1")
                                        .arg(ui->inputEdit->text()));
    }
}

void Widget::onInputTextChanged()
{
    ui->calculateButton->setEnabled(!ui->inputEdit->text().isEmpty());
}
