#ifndef MAINACTIVITY_H
#define MAINACTIVITY_H

#include <QMainWindow>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class mainActivity; }
QT_END_NAMESPACE

class mainActivity : public QMainWindow
{
    Q_OBJECT

public:
    mainActivity(QWidget *parent = nullptr);
    ~mainActivity();
    QString readFile(QString file);
    bool connectUpdateTitleDone;

private slots:
    void on_pushButton_clicked();
    void on_searchBtn_clicked();

protected slots:
    void loadNewUrl(QString requestUrl);
    void loadSearchResults(QString searchTerm);
    QString makeRequest(QString requestUrl);
    void interceptUrl(QUrl url);
    void updateTitle(QString title);

private:
    Ui::mainActivity *ui;
    QWebEngineView * view;
};
#endif // MAINACTIVITY_H
