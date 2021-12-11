#include "mainactivity.h"
#include "ui_mainactivity.h"

#include <QDebug>

mainActivity::mainActivity(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainActivity)
{
    ui->setupUi(this);
    view = new QWebEngineView(this);
    ui->gridLayout->addWidget(view);
    view->show();
}

mainActivity::~mainActivity()
{
    delete ui;
}


void mainActivity::on_pushButton_clicked()
{
    loadNewUrl(ui->lineEdit->text());
}

void mainActivity::loadNewUrl(QString requestUrl) {
    makeRequest(requestUrl);
}

void mainActivity::makeRequest(QString requestUrl) {
    QString prog ("../multimon-ng/multimon-ng");
    QStringList args;
    args << "-a" << "POCSAG2400" << "-q";
    QProcess *proc = new QProcess();
    proc->start(prog, args);
    QString receivedWebpage = proc->readAllStandardOutput();

    QDir::setCurrent("../scripts/client");
    QProcess::execute("request.sh", QStringList() << requestUrl);

    while(true) {
        if(QFile::exists("send_done")) {
            break;
        }
    }

    QDir::setCurrent("../..");
}

QString mainActivity::readFile(QString file) {
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
