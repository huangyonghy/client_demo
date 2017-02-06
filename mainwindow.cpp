#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

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

void MainWindow::on_getButton_clicked()
{
    QString ip = ui->ipLineEdit->text();
    QString pn = ui->pnLineEdit->text();
    QString uri = "http://" + ip + ":" + pn + "?user=huangyong&number=7";
    QNetworkRequest request;
    request.setUrl(QUrl( uri ));
    manager->get( request );
}

void MainWindow::on_postButton_clicked()
{
    QString ip = ui->ipLineEdit->text();
    QString pn = ui->pnLineEdit->text();
    QString uri = "http://" + ip + ":" + pn;
//    QByteArray post_data;
    QJsonObject json;
    json.insert( "user", QString( "huangyong" ) );
    json.insert( "password", QString( "1234" ) );
    json.insert( "version", 5 );
    json.insert( "windows", true );
    QJsonDocument document;
    document.setObject( json );
    QByteArray byte_array = document.toJson( QJsonDocument::Compact );
//    QString json_str(byte_array);

//    post_data.append("user=wangliang&");
//    post_data.append("password=111111&");
//    post_data.append("domain=sugon&");
//    post_data.append("client=windowstool&");
//    post_data.append("version=1.1.1.1");
    QNetworkRequest request = QNetworkRequest( QUrl( uri ) );
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json " );
//    request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
//    manager->post(request, post_data);
    request.setHeader( QNetworkRequest::ContentLengthHeader, byte_array.length() );
    manager->post( request, byte_array );
}

void MainWindow::replyFinished( QNetworkReply *reply )
{
    cache = reply->readAll();
    if( reply->error() == QNetworkReply::NoError )
    {
        if( cache.size() > 0 )
        {
            qDebug() << cache;
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson( cache, &json_error );
            if( json_error.error == QJsonParseError::NoError )
            {
                if( parse_doucment.isObject() )
                {
                    QJsonObject obj = parse_doucment.object();
                    foreach(QString key, obj.keys())
                    {
                        QJsonValue value = obj.take( key );
                        qDebug()<<key<<value;
                    }
                }
            }

            QString s;
            for( int i = 0; i < cache.size(); ++i )
            {
                if( cache[i] != ' ' )
                {
                    s += cache[i];
                }
            }
            ui->textEdit->append( s );
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
