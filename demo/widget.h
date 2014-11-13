#ifndef WIDGET_H
#define WIDGET_H

#include <QDialog>

namespace Ui {
class Widget;
}

class Widget : public QDialog
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void onCalculateButtonClicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
