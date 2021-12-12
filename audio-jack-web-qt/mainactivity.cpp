#include "mainactivity.h"
#include "ui_mainactivity.h"

#include <QDebug>

mainActivity::mainActivity(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainActivity)
{
    ui->setupUi(this);

    // Cleanup
    QFile::remove("../scripts/client/iterations_total");
    QFile::remove("../scripts/client/input");
    QFile::remove("../scripts/client/decoded");

    connectUpdateTitleDone = false;
    ui->searchLineEdit->setFocus();

    view = new QWebEngineView(this);
    connect(view, SIGNAL(urlChanged(QUrl)), SLOT(interceptUrl(QUrl)));
    ui->gridLayout->addWidget(view);
    view->show();

    QString welcomeHtml = "<!DOCTYPE html><html><body><center><b>audio-jack-web</b><br/>Browse the Internet over two 3.5mm audio cables!<br/><br/><i>Type an URL into the address bar to access the Web.</center></body></html>";
    view->setHtml(welcomeHtml);
    this->setWindowTitle("audio-jack-web browser");
}

mainActivity::~mainActivity()
{
    delete ui;
}


void mainActivity::on_pushButton_clicked()
{
    if(!connectUpdateTitleDone) {
        connect(view, SIGNAL(titleChanged(QString)), SLOT(updateTitle(QString)));
        connectUpdateTitleDone = true;
    }
    loadNewUrl(ui->lineEdit->text());
}

void mainActivity::loadNewUrl(QString requestUrl) {
    if(!requestUrl.isEmpty()) {
        ui->statusLabel->setText("Sending request to server");
        QString webpage = makeRequest(requestUrl);
        view->setHtml(webpage);
        ui->statusLabel->setText("Done loading webpage " + requestUrl);
    }
    else {
        QMessageBox::critical(this, tr("No URL to request"), tr("Please type in an URL into the address bar."));
    }
}

QString mainActivity::makeRequest(QString requestUrl) {
    qDebug() << "Making request for" << requestUrl;

    QString multimonProg = "bash";
    QStringList multimonArgs;
    multimonArgs << "-c" << "../multimon-ng/multimon-ng -q -a POCSAG2400 > ../scripts/client/input";
    QProcess * proc = new QProcess();
    proc->start(multimonProg, multimonArgs);

    QFile::remove("../scripts/client/send_done");
    QDir::setCurrent("../scripts/client");
    QProcess::execute("request.sh", QStringList() << requestUrl);
    qDebug() << "Receiving data from server";
    QProcess::startDetached("../../audio-jack-web-qt/audio-jack-web-qt", QStringList() << "load-progress");

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

void mainActivity::interceptUrl(QUrl url) {
    QString urlString = url.toString();
    if(urlString.contains("data:text/html")) {
        // This is a local page received from server, do nothing
        ;
    }
    else {
        // Intercept web request
        loadNewUrl(urlString);
        ui->lineEdit->setText(urlString);
    }
}

void mainActivity::on_searchBtn_clicked()
{
    if(!connectUpdateTitleDone) {
        connect(view, SIGNAL(titleChanged(QString)), SLOT(updateTitle(QString)));
        connectUpdateTitleDone = true;
    }
    loadSearchResults(ui->searchLineEdit->text());
}

void mainActivity::loadSearchResults(QString searchTerm) {
    if(!searchTerm.isEmpty()) {
        loadNewUrl("http://frogfind.com/?q=" + searchTerm);
    }
    else {
        QMessageBox::critical(this, tr("Search is empty"), tr("Please specify a search term in the search bar."));
    }
}

void mainActivity::updateTitle(QString title) {
    this->setWindowTitle(title);
}
