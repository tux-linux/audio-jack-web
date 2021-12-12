#include "mainactivity.h"
#include "loadprogressdialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(QCoreApplication::arguments().contains("load-progress")) {
        loadProgressDialog w;
        w.show();
        return a.exec();
    }
    else {
        mainActivity w;
        w.showMaximized();
        return a.exec();
    }
}
