#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QTime>
#include <QPalette>

#include "picker.h"
#include "mainwindow.h"
#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"
#include "configtool.h"
#include "configwindow.h"
#include "shortcut.h"
#include "searchbar.h"
#include <unistd.h>
#include <sys/file.h>
#include "baidutranslate.h"
#include "messagebox.h"

/* appDir   可执行文件所在目录, /opt/CuteTranslation
 * dataDir  数据文件目录，~/.local/share/CuteTranslation
 * logFile  日志文件，~/.local/share/CuteTranslation/log.txt
 */


static QFile *logFile;
int checkDependency();
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


// TODO 多屏幕支持   清空配置目录
// TODO  可执行文件指定兼容哪些版本的配置文件

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication::setQuitOnLastWindowClosed(false); // 关闭窗口时，程序不退出
    QApplication a(argc, argv);
    a.setApplicationName("CuteTranslation");
    a.setApplicationVersion("0.4.0");

    QPalette palette(Qt::white);
    a.setPalette(palette);

    // 必须文件夹
    appDir.setPath(QCoreApplication::applicationDirPath());
    dataDir.setPath(QDir::homePath() + "/.local/share/CuteTranslation");
    QDir::home().mkpath(dataDir.absolutePath());
    QDir::home().mkpath(QDir::homePath() + "/.config/autostart");

    xdotool = new Xdotool();
    logFile = new QFile(dataDir.filePath("log.txt"));

    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) == false)
    {
        qCritical() << "无法记录日志";
    }
    qInstallMessageHandler(myMessageOutput);
    qInfo() << "---------- Start --------------";
    if (checkDependency() < 0)
        return -1;

    configTool = new ConfigTool();

    /* ConfigTool       配置工具
     * Picker           取词功能
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
    QObject::connect(&tray.config_action, &QAction::triggered, &cw, [&cw]{ cw.show(); cw.activateWindow(); });
    QObject::connect(&tray, &SystemTrayIcon::activated, &cw, [&cw]{ cw.show(); cw.activateWindow(); });
    QObject::connect(&searchBar, &SearchBar::returnPressed, &w, &MainWindow::onSearchBarReturned);
    QObject::connect(&cw, &ConfigWindow::SizeChanged, &w, &MainWindow::onAdjustSize);

    // 快捷键
    QObject::connect(&shortcut, &ShortCut::OCRTranslateShortCutPressed, &w, &MainWindow::onOCRTranslateShortCutPressed);
    QObject::connect(&shortcut, &ShortCut::OCRTextShortCutPressed, &w, &MainWindow::onOCRTextShortCutPressed);
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
    QObject::connect(&tray.ocr_text_action, &QAction::triggered, &shortcut,  &ShortCut::OCRTextShortCutPressed);
    QObject::connect(&tray.ocr_translate_action, &QAction::triggered, &shortcut, &ShortCut::OCRTranslateShortCutPressed);
    QObject::connect(configTool, &ConfigTool::ModeChanged, &tray, &SystemTrayIcon::OnModeChanged);

    QObject::connect(&tray.quit_action, &QAction::triggered, &tray, [=]{
        xdotool->eventMonitor.terminate();
        xdotool->eventMonitor.wait();
        qInfo() << "---------- Exit --------------";
        logFile->close();
        qApp->quit();
    });

    // 全局鼠标监听
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::buttonPress, picker, &Picker::buttonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool->eventMonitor, &EventMonitor::buttonPress, &f, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::buttonPress, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool->eventMonitor, &EventMonitor::buttonRelease, &f, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::buttonRelease, picker, &Picker::buttonReleased, Qt::QueuedConnection);

    QObject::connect(&xdotool->eventMonitor, &EventMonitor::mouseWheel, &f, &FloatButton::hide, Qt::QueuedConnection);
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::mouseWheel, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    // 全局键盘监听
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::keyPress, &f, &FloatButton::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::keyPress, &shortcut, &ShortCut::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool->eventMonitor, &EventMonitor::keyRelease, &shortcut, &ShortCut::onKeyReleased, Qt::QueuedConnection);

    xdotool->eventMonitor.start();
    configTool->SetMode(configTool->GetMode()); // 触发ModeChanged，修改托盘文字

    // 通知桌面环境，应用已经加载完毕
    cw.show();
    cw.hide();
    qInfo() << "应用加载完毕";

    BaiduTranslate::instance();

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
    depends.push_back("config.ini");
    depends.push_back("screenshot.sh");
    depends.push_back("baidu.js");
    depends.push_back("guide.txt");

    bool filesExist = true;
    for (auto file : depends)
    {
        if (!appDir.exists(file))
        {
            qInfo() << "file is missing: " << appDir.filePath(file);
            filesExist = false;
        }
    }
    if (QFile::exists(dataDir.filePath("config.ini")) == false)
    {
        // 认为是第一次开启
        qInfo() << "复制配置文件";
        QFile::copy(appDir.filePath("config.ini"), dataDir.filePath("config.ini"));
        // 打开指南文本文件
        system(("xdg-open " + appDir.filePath("guide.txt")).toStdString().c_str());
    }
    if (filesExist == false)
    {
        qCritical() << "文件缺失";
        return -1;
    }
    res = system("which nodejs > /dev/null && which gnome-screenshot > /dev/null  && which tidy > /dev/null");
    if (res != 0)
    {
        qCritical() << "缺少依赖 nodejs gnome-screenshot tidy";
    }
    return 0;
}

QTextStream& qStdOut()
{
    static QTextStream ts(stdout);
    return ts;
}

QTextStream& logOutput()
{
    static QTextStream ts(logFile);
    return ts;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    static MessageBox msgBox;
    if (!logFile->isOpen())
        return;
    QString time =  QDateTime::currentDateTime().toString(Qt::ISODate);
    switch (type) {
    case QtDebugMsg:
        qStdOut() << time << " Debug: " << msg << endl;
        logOutput() << time<< " Debug: " << msg << endl;
        break;
    case QtInfoMsg:
        qStdOut() << time << " Info: " << msg << endl;
        logOutput() << time<< " Info: " << msg << endl;
        break;
    case QtWarningMsg:
        qStdOut() << time << " Warning: " << msg << endl;
        logOutput() << time<< " Warning: " << msg << endl;
        msgBox.setText(msg);
        msgBox.show();
        break;
    case QtCriticalMsg:
        qStdOut() << time << " Critical: " << msg << endl;
        logOutput() << time<< " Critical: " << msg << endl;
        msgBox.setText(msg);
        msgBox.show();
        qApp->exit(1);
        break;
    case QtFatalMsg:
        qStdOut() << time << " Fatal: " << msg << endl;
        logOutput() << time<< " Fatal: " << msg << endl;
        msgBox.setText(msg);
        msgBox.show();
        qApp->exit(2);
        break;
    }

}
