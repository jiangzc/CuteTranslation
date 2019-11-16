#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QVector>

#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"
#include "configtool.h"
#include "configwindow.h"
#include "shortcut.h"
#include "searchbar.h"

Xdotool xdotool;
ConfigTool configTool;

int main(int argc, char *argv[])
{
    // TODO 截图翻译，针对单词优化。添加开机启动功能


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication a(argc, argv);

    // 检查依赖文件是否存在
    QVector<QString> depends;
    depends.push_back("BaiduTranslate.py");
    depends.push_back("translate_demo.py");
    depends.push_back("BaiduOCR.py");
    depends.push_back("interpret_js_2.html");

    bool filesExist = true;
    QDir appDir(QCoreApplication::applicationDirPath());
    for (auto file : depends)
    {
        if (!appDir.exists(file))
        {
            qDebug() << "file is missing: " << appDir.filePath(file);
            filesExist = false;
        }
    }

    if (!QDir::home().exists(".config/CuteTranslation/config.ini"))
    {
        qDebug() << "file is missing: " << QDir::homePath() + "/.config/CuteTranslation/config.ini";
        filesExist = false;
    }
    if (filesExist == false)
        return -1;


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
    SearchBar sb;
    // sb.show();

    QObject::connect(picker, &Picker::wordsPicked, &f, &FloatButton::onWordPicked);

    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);

    QObject::connect(&tray.config_action, &QAction::triggered, &cw, &ConfigWindow::show );

    QObject::connect(&sb, &SearchBar::returnPressed, &w, &MainWindow::onSearchBarReturned);

    // 快捷键
    QObject::connect(&shortcut, &ShortCut::OCRShortCutPressed, &w, &MainWindow::onOCRShortCutPressed);
    QObject::connect(&shortcut, &ShortCut::SearchBarShortCutPressed, &sb, [&]{
        if (sb.isHidden())
        {
            sb.move(QCursor::pos() - QPoint(150, 25));
            sb.show();
        }
        else
            sb.hide();
    });
    // 托盘菜单
    QObject::connect(&tray.search_action, &QAction::triggered, &shortcut, &ShortCut::SearchBarShortCutPressed);
    QObject::connect(&tray.ocr_action, &QAction::triggered, &shortcut, &ShortCut::OCRShortCutPressed);

    // 全局鼠标监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &f, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, picker, &Picker::buttonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, &f, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, picker, &Picker::buttonReleased, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &f, &FloatButton::hide, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    // 全局键盘监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &f, &FloatButton::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &shortcut, &ShortCut::onKeyPressed, Qt::QueuedConnection);

    xdotool.eventMonitor.start();

    return a.exec();
}
