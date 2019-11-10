#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"
#include "configtool.h"
#include "configwindow.h"
#include "shortcut.h"

Xdotool xdotool;
ConfigTool configTool;

int main(int argc, char *argv[])
{
    // TODO check files exist
    // TODO analyse picker's text language type
    // BUG First select invaild
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication a(argc, argv);

    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();
    picker = new Picker();
    ConfigWindow cw;
    MainWindow w;
    w.setGeometry(800, 200, 400, 300);

    FloatButton f;

    SystemTrayIcon tray;
    tray.show();

    ShortCut shortcut;

    // cw.show();

    QObject::connect(picker, &Picker::wordsPicked, &f, &FloatButton::onWordPicked);

    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);

    QObject::connect(&tray.config_action, &QAction::triggered, &cw, &ConfigWindow::show );

    // 全局鼠标监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &f, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, picker, &Picker::buttonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, &f, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, picker, &Picker::buttonReleased, Qt::QueuedConnection);

    // 全局键盘监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &f, &FloatButton::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &shortcut, &ShortCut::onKeyPressed, Qt::QueuedConnection);

    xdotool.eventMonitor.start();
    return a.exec();
}
