#include "mainactivity.h"
#include "loadprogressdialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "audio-jack-web-qt | Copyright (C) 2021 Nicolas Mailloux";
    qDebug() << "Licensed under the GNU General Public License version 3.0";
    qDebug() << "See the LICENSE file for details.";

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
