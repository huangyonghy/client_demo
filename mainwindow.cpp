#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    setNetwork();
}

void MainWindow::setNetwork()
{
    manager = new QNetworkAccessManager( this );
    connect( manager, SIGNAL( finished( QNetworkReply* ) ),
             this, SLOT( replyFinished( QNetworkReply* ) ) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString ip =ui->ipLineEdit->text();
    QString pn = ui->pnLineEdit->text();
    QString uri = "http://" + ip + ":" + pn;
    manager->get( QNetworkRequest( QUrl( uri ) ) );
}

void MainWindow::replyFinished( QNetworkReply *reply )
{
    cache = reply->readAll();
    if( reply->error() == QNetworkReply::NoError )
    {
        if( cache.size() > 0 )
        {
            ui->textEdit->append(cache.data());
        }
        else
        {
            qDebug() << "data is empty";
            qDebug() << reply->request().url().toString();
        }
    }
    else
    {
        if( reply->error() == QNetworkReply::AuthenticationRequiredError )
        {
            QMessageBox::warning( QApplication::activeWindow(), tr( "Warning" ), tr( "Session expires, please re-login!" ) );
        }
        qDebug() << reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ) << "\n" << reply->error();
        qDebug() << reply->request().url().toString();
        QString error_message = cache;
        if( error_message.contains( "error_message" ) )
            qDebug() << error_message;
    }
    reply->deleteLater();
}
