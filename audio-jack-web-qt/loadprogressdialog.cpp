#include "loadprogressdialog.h"
#include "ui_loadprogressdialog.h"

loadProgressDialog::loadProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loadProgressDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Loading progress");

    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    ui->progressLabel->setText("Initializing");
    QString header;
    while(true) {
        if(QFile::exists("iterations_total")) {
            header = readFile("iterations_total");
            break;
        }
    }
    QStringList splitHeader = header.split("-");
    int iterationsNumber = splitHeader[0].toInt();
    ui->progressLabel->setText("Loading " + splitHeader[0] + " chunks");

    QTimer *t = new QTimer(this);
    t->setInterval(10);
    connect(t, &QTimer::timeout, [&]() {
       if(QFile::exists("send_done")) {
           QFile::remove("send_done");
           qApp->quit();
       }
    } );
    t->start();
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
