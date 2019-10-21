#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();

    MainWindow w;
    w.setGeometry(800, 200, 400, 300);

    FloatButton f;
    f.mainWindow = &w;
    w.picker = f.picker;

    SystemTrayIcon tray;
    tray.show();

    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);
    QObject::connect(&tray.quit_action, &QAction::triggered, &a, [=]{ xdotool.eventMonitor.terminate(); xdotool.eventMonitor.wait(); qApp->quit(); });

    xdotool.eventMonitor.start();

    return a.exec();
}
