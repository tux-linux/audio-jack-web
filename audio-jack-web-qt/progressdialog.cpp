#include "progressdialog.h"
#include "ui_progressdialog.h"

progressDialog::progressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressDialog)
{
    ui->setupUi(this);
    ui->progressLabel->setText("Initializing");
    QString iterations;
    while(true) {
        if(QFile::exists("../scripts/client/iterations_total")) {
            iterations = readFile("../scripts/client/iterations_total");
        }
    }
    int iterationsNumber = iterations.toInt();
    ui->progressLabel->setText("Loading " + iterations + " chunks");
}

progressDialog::~progressDialog()
{
    delete ui;
}

QString progressDialog::readFile(QString file) {
    if(QFile::exists(file)) {
        QString content;
        QFile fileToRead(file);
        fileToRead.open(QIODevice::ReadOnly);
        QTextStream in (&fileToRead);
        content = in.readAll();
        fileToRead.close();
        return content;
    }
    else {
        return NULL;
    }
}
