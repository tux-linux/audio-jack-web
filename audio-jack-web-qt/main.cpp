#include "mainactivity.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(QCoreApplication::arguments().contains("load-progress")) {
        exit(0);
    }
    else {
        mainActivity w;
        w.showMaximized();
        return a.exec();
    }
}
