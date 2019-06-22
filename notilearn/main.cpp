#include "mainwindow.h"
#include "addnewdialog.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MainWindow timer;
    QApplication::quit();

    return a.exec();
}
