#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QNetworkAccessManager *manager;
    QByteArray cache;

private slots:
    void on_getButton_clicked();
    void on_postButton_clicked();
    void replyFinished( QNetworkReply *reply );

private:
    Ui::MainWindow *ui;

    void setNetwork();
};

#endif // MAINWINDOW_H
