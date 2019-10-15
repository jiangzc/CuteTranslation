#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#include "floatbutton.h"
#include "xdotool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QGuiApplication::screens().
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();
    FloatButton f;
    // f.show();
    f.move(800, 500);

    MainWindow w;
    f.mainWindow = &w;
    w.setGeometry(800, 200, 400, 300);
    w.show();

    w.activateWindow();
    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);

    return a.exec();
}
