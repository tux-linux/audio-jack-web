#ifndef LOADPROGRESSDIALOG_H
#define LOADPROGRESSDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>
#include <QDebug>

namespace Ui {
class loadProgressDialog;
}

class loadProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loadProgressDialog(QWidget *parent = nullptr);
    ~loadProgressDialog();
    QString readFile(QString file);
    int inputLength;

private:
    Ui::loadProgressDialog *ui;
};

#endif // LOADPROGRESSDIALOG_H
