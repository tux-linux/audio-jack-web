#include "loadprogressdialog.h"
#include "ui_loadprogressdialog.h"

loadProgressDialog::loadProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loadProgressDialog)
{
    ui->setupUi(this);

    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    ui->progressLabel->setText("Initializing");
    QString iterations;
    while(true) {
        if(QFile::exists("iterations_total")) {
            iterations = readFile("iterations_total");
            break;
        }
    }
    int iterationsNumber = iterations.toInt();
    ui->progressLabel->setText("Loading " + iterations + " chunks");
}

loadProgressDialog::~loadProgressDialog()
{
    delete ui;
}

QString loadProgressDialog::readFile(QString file) {
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
