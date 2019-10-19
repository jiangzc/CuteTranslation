#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#include "floatbutton.h"
#include "xdotool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();

    MainWindow w;
    w.setGeometry(800, 200, 400, 300);
    w.show();
    w.activateWindow();

    FloatButton f;
    f.mainWindow = &w;
    f.move(800, 500);

    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);
    return a.exec();
}
