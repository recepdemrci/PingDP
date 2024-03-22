#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "ping.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void output(QString data);
    void draw(double x, double y = -0.5);
    void reset();
    void on_txtIP_textChanged(const QString &arg1);
    void on_interval_valueChanged(double arg1);

private:
    Ui::Dialog *ui;
    Ping ping;
};
#endif // DIALOG_H
