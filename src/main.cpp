#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QFile>
#include <QVector>

#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"
#include "configtool.h"
#include "configwindow.h"
#include "shortcut.h"
#include "searchbar.h"
#include <unistd.h>
#include <sys/file.h>


Xdotool xdotool;
ConfigTool configTool;

int main(int argc, char *argv[])
{
    // TODO 从配置文件加载token网址  排版
    // 检查配置文件一致性
    // token 验证有效性
    // 全局 ... 三种模式不同的图片

    // BUG: 浏览器地址栏，重复出现FloatBtn  won't fix

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication::setQuitOnLastWindowClosed(false); // 关闭窗口时，程序不退出（弹框提醒）
    QApplication a(argc, argv);

    // 防止应用多开
    int fd = open("/tmp/cute.lock", O_CREAT, S_IRUSR | S_IRGRP);
    if (fd == -1)
    {
        qDebug() << "无法打开/tmp/cute.lock";
        return -1;
    }
    int res = flock(fd, LOCK_EX | LOCK_NB); // 非阻塞放置互斥锁，一直占用不释放
    if (res != 0)
    {
        qDebug() << "应用多开，自动退出。";
        return -1;
    }

    // 检查依赖文件是否存在
    QVector<QString> depends;
    depends.push_back("BaiduTranslate.py");
    depends.push_back("translate_demo.py");
    depends.push_back("BaiduOCR.py");
    depends.push_back("interpret_js_1.html");
    depends.push_back("interpret_js_2.html");
    depends.push_back("config.ini");
    depends.push_back("screenshot.sh");

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
        // 复制配置文件
        QFile::copy(appDir.filePath("config.ini"), QDir::home().filePath(".config/CuteTranslation/config.ini"));
    }
    if (filesExist == false)
        return -1;


    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();

    /* Picker           取词功能
     * ConfigWindow     配置界面
     * MainWindow       翻译界面
     * FloatButton      悬浮按钮
     * SystemTrayIcon   托盘栏
     * ShortCut         快捷键
     * SearchBar        悬浮搜索框
     */

    picker = new Picker();
    ConfigWindow cw;
    MainWindow w;
    FloatButton f;
    SystemTrayIcon tray;
    ShortCut shortcut;
    SearchBar searchBar;

    QObject::connect(picker, &Picker::wordsPicked, &f, &FloatButton::onWordPicked);
    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);
    QObject::connect(&tray.config_action, &QAction::triggered, &cw, &ConfigWindow::show);
    QObject::connect(&searchBar, &SearchBar::returnPressed, &w, &MainWindow::onSearchBarReturned);

    // 快捷键
    QObject::connect(&shortcut, &ShortCut::OCRShortCutPressed, &w, &MainWindow::onOCRShortCutPressed);
    QObject::connect(&shortcut, &ShortCut::SearchBarShortCutPressed, &searchBar, [&]{
        if (searchBar.isHidden())
        {
            searchBar.move(QCursor::pos() - QPoint(150, 25));
            searchBar.show();
            searchBar.activateWindow();
        }
        else if (searchBar.isActiveWindow() == false && w.isActiveWindow() == false)
        {
            // 这么做看全屏视频时，用悬浮搜索框有更好的体验。
            searchBar.move(QCursor::pos() - QPoint(150, 25));
            searchBar.ClearLineEdit();
            searchBar.activateWindow();
        }
        else
            searchBar.hide();
    });

    // 托盘菜单
    QObject::connect(&tray.search_action, &QAction::triggered, &shortcut, &ShortCut::SearchBarShortCutPressed);
    QObject::connect(&tray.ocr_action, &QAction::triggered, &shortcut, &ShortCut::OCRShortCutPressed);

    // 全局鼠标监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, picker, &Picker::buttonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &f, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, &f, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, picker, &Picker::buttonReleased, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &f, &FloatButton::hide, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    // 全局键盘监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &f, &FloatButton::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &shortcut, &ShortCut::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyRelease, &shortcut, &ShortCut::onKeyReleased, Qt::QueuedConnection);

    xdotool.eventMonitor.start();

    // 通知桌面环境，应用已经加载完毕
    cw.show();
    cw.hide();

    return a.exec();
}
