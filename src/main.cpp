#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(200,200,400,300);
    w.show();

    w.activateWindow();

    return a.exec();
}
