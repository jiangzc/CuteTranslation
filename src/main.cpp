#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QTime>
#include <QMessageBox>
#include <iostream>

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
ConfigTool *configTool;
const QString CUTETRANSLATION_VERSION = "0.0.1";
int checkDependency();
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    // TODO 排版
    // 重写 configTool Mode 等用 get set 函数设置
    // 调整文件位置  用户相关的全到 ~/.config/Cu..
    // 增加配置 缩放比例
    // 全局 ... 三种模式不同的图片
    // 完善 Debug 信息
    // 托盘栏 选中提示
    // 检查 可执行文件所在目录 权限
    // .config/log  日志文件


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication::setQuitOnLastWindowClosed(false); // 关闭窗口时，程序不退出
    QApplication a(argc, argv);

    qInstallMessageHandler(myMessageOutput);

    if (checkDependency() < 0)
        return -1;

    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();

    /* ConfigTool       配置工具
     * Picker           取词功能
     * ConfigWindow     配置界面
     * MainWindow       翻译界面
     * FloatButton      悬浮按钮
     * SystemTrayIcon   托盘栏
     * ShortCut         快捷键
     * SearchBar        悬浮搜索框
     */

    configTool = new ConfigTool();
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
    QObject::connect(&tray, &SystemTrayIcon::activated, &cw, &ConfigWindow::show);
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
    QObject::connect(configTool, &ConfigTool::ModeChanged, &tray, [&tray](ModeSet mode){
        tray.change_mode_all_action.setText("全局");
        tray.change_mode_custom_action.setText("自定义");
        tray.change_mode_none_action.setText("禁用");
        if (configTool->GetMode() == Mode_ALL)
            tray.change_mode_all_action.setText("✓ 全局");
        else if (configTool->GetMode() == Mode_CUSTOM)
            tray.change_mode_custom_action.setText("✓ 自定义");
        else if (configTool->GetMode() == Mode_NONE)
            tray.change_mode_none_action.setText("✓ 禁用");
    });

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
    qInfo() << "应用加载完毕";
    return a.exec();
}

int checkDependency()
{
    qInfo() << "检查依赖";
    // 防止应用多开
    int fd = open("/tmp/cute.lock", O_CREAT, S_IRUSR | S_IRGRP);
    if (fd == -1)
    {
        qCritical() << "无法打开/tmp/cute.lock";
        return -1;
    }
    int res = flock(fd, LOCK_EX | LOCK_NB); // 放置互斥锁，一直占用不释放
    if (res != 0)
    {
        qCritical() << "应用多开，自动退出。";
        return -1;
    }

    // 检查依赖文件是否存在
    QVector<QString> depends;
    depends.push_back("BaiduTranslate.py");
    depends.push_back("translate_demo.py");
    depends.push_back("BaiduOCR.py");
    depends.push_back("update_token.py");
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
            qInfo() << "file is missing: " << appDir.filePath(file);
            filesExist = false;
        }
    }
    if (QDir::home().exists(".config/CuteTranslation/config.ini") == false)
    {
        // 认为是第一次开启
        qInfo() << "复制配置文件";
        QFile::copy(appDir.filePath("config.ini"), QDir::home().filePath(".config/CuteTranslation/config.ini"));

        qInfo() << "检查python3依赖";
        int res = system("pip3 show requests &&  pip3 show PyExecJS");
        if (res != 0)
        {
            qWarning() << "缺少python3依赖";
        }
    }
    if (filesExist == false)
    {
        qCritical() << "文件缺失";
        return -1;
    }


    return 0;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    std::string time = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
    switch (type) {
    case QtDebugMsg:
        std::cout << time << " Debug: " << msg.toStdString() << std::endl;
        break;
    case QtInfoMsg:
        std::cout << time << " Info: " << msg.toStdString() << std::endl;
        break;
    case QtWarningMsg:
        std::cout << time << " Warning: " << msg.toStdString() << std::endl;
        QMessageBox::warning(nullptr, "警告", msg, QMessageBox::Ignore);
        break;
    case QtCriticalMsg:
        std::cout << time << " Critical: " << msg.toStdString() << std::endl;
        QMessageBox::warning(nullptr, "错误", msg, QMessageBox::Ok);
        abort();
    case QtFatalMsg:
        std::cout << time << " Fatal: " << msg.toStdString() << std::endl;
        QMessageBox::warning(nullptr, "错误", msg, QMessageBox::Ok);
        abort();
    }

}
