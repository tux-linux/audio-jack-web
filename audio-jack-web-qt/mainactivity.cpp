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
    QDir::setCurrent("../scripts/client");
    QProcess::execute("request.sh", QStringList() << requestUrl);
}
