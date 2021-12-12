#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class progressDialog;
}

class progressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit progressDialog(QWidget *parent = nullptr);
    ~progressDialog();
    QString readFile(QString file);

private:
    Ui::progressDialog *ui;
};

#endif // PROGRESSDIALOG_H
