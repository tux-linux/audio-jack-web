#include "mainactivity.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainActivity w;
    w.showMaximized();
    return a.exec();
}
