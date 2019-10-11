#include "mainwindow.h"
#include "floatbutton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FloatButton f;
    f.show();
    f.move(500,500);
    MainWindow w;
    w.setGeometry(200,200,400,300);
    w.show();

    w.activateWindow();


    return a.exec();
}
