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

    QString welcomeHtml = "<!DOCTYPE html><html><body><center><b>audio-jack-web</b><br/>Browse the Internet over two 3.5mm audio cables!<br/><br/><i>Type an URL into the address bar to access the Web.</center></body></html>";
    view->setHtml(welcomeHtml);
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
    ui->statusLabel->setText("Sending request to server");
    QString webpage = makeRequest(requestUrl);
    view->setHtml(webpage);
    ui->statusLabel->setText("Done loading webpage");
}

QString mainActivity::makeRequest(QString requestUrl) {
    QString multimonProg = "bash";
    QStringList multimonArgs;
    multimonArgs << "-c" << "../multimon-ng/multimon-ng -q -a POCSAG2400 > ../scripts/client/input";
    QProcess * proc = new QProcess();
    proc->start(multimonProg, multimonArgs);

    QFile::remove("../scripts/client/send_done");
    QDir::setCurrent("../scripts/client");
    QProcess::execute("request.sh", QStringList() << requestUrl);
    qDebug() << "Receiving data from server";

    while(true) {
        if(QFile::exists("send_done")) {
            qDebug() << "Done receiving data from server";
            QFile::remove("send_done");
            break;
        }
    }

    QProcess::execute("killall", QStringList() << "multimon-ng");
    QProcess::execute("bash", QStringList() << "-c" << "cat input | base64 -d | xz -d > decoded.html");
    QFile::remove("input");
    QString decodedWebpage = readFile("decoded.html");
    QFile::remove("decoded.html");
    QDir::setCurrent("../../audio-jack-web-qt");

    proc->deleteLater();

    return decodedWebpage;
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
