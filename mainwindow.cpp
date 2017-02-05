#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setNetwork();
}

void MainWindow::setNetwork()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getButton_clicked()
{
    QString ip = ui->ipLineEdit->text();
    QString pn = ui->pnLineEdit->text();
    QString uri = "http://" + ip + ":" + pn;
    manager->get(QNetworkRequest(QUrl(uri)));
}

void MainWindow::on_postButton_clicked()
{
    QString ip = ui->ipLineEdit->text();
    QString pn = ui->pnLineEdit->text();
    QString uri = "http://" + ip + ":" + pn;
    QByteArray post_data;
    post_data.append("user=wangliang&");
    post_data.append("password=111111&");
    post_data.append("domain=sugon&");
    post_data.append("client=windowstool&");
    post_data.append("version=1.1.1.1");
    QNetworkRequest request = QNetworkRequest(QUrl(uri));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
    manager->post(request, post_data);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    cache = reply->readAll();
    if (reply->error() == QNetworkReply::NoError)
    {
        if (cache.size() > 0)
        {
            qDebug() << cache;
            QString s;
            for (int i = 0; i < cache.size(); ++i) {
                if (cache[i] != ' ') {
                    s += cache[i];
                }
            }
            ui->textEdit->append(s);
        }
        else
        {
            qDebug() << "data is empty";
            qDebug() << reply->request().url().toString();
        }
    }
    else
    {
        if (reply->error() == QNetworkReply::AuthenticationRequiredError)
        {
            QMessageBox::warning(QApplication::activeWindow(), tr("Warning"), tr("Session expires, please re-login!"));
        }
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << "\n" << reply->error();
        qDebug() << reply->request().url().toString();
        QString error_message = cache;
        if (error_message.contains("error_message"))
            qDebug() << error_message;
    }
    reply->deleteLater();
}
