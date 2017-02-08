#include "mainwindow.h"
#include "tools.h"

#include <QApplication>
#include  <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString licenseFile = QDir::currentPath()+"/license.txt";
    if (getAuthorized(licenseFile)) {
        w.show();
    }
    else {
        QMessageBox::warning(&w, "Warning", "Programme should be licensed first!");
    }


    return a.exec();
}
