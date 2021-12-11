#ifndef MAINACTIVITY_H
#define MAINACTIVITY_H

#include <QMainWindow>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets/QWebEngineView>

QT_BEGIN_NAMESPACE
namespace Ui { class mainActivity; }
QT_END_NAMESPACE

class mainActivity : public QMainWindow
{
    Q_OBJECT

public:
    mainActivity(QWidget *parent = nullptr);
    ~mainActivity();

private slots:
    void on_pushButton_clicked();

protected slots:
    void loadNewUrl(QString requestUrl);
    void makeRequest(QString requestUrl);

private:
    Ui::mainActivity *ui;
    QWebEngineView * view;
};
#endif // MAINACTIVITY_H
