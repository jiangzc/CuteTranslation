#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDir>
#include "systemtrayicon.h"
#include "shortcut.h"
#include "xdotool.h"
#include "configtool.h"
#include "errno.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent):QSystemTrayIcon(parent),
    quit_action("退出"), config_action("配置"), help_action("用户手册"), search_action("文字翻译"), ocr_action("截图翻译"),
    change_mode_all_action("全局"), change_mode_none_action("禁用"), change_mode_custom_action("自定义"),
    homepage_action("项目主页"), donate_action("捐赠"),
    modeSubMenu("取词模式", &menu), helpSubMenu("帮助", &menu),
    autostart_action("开机启动")
{
    QIcon icon(":/pic/icon.png");
    setIcon(icon);

    modeSubMenu.addAction(&change_mode_all_action);
    modeSubMenu.addAction(&change_mode_custom_action);
    modeSubMenu.addAction(&change_mode_none_action);
    menu.addMenu(&modeSubMenu);


    menu.addAction(&search_action);
    menu.addAction(&ocr_action);
    menu.addAction(&config_action);
    menu.addAction(&autostart_action);

    helpSubMenu.addAction(&help_action);
    helpSubMenu.addAction(&homepage_action);
    helpSubMenu.addAction(&donate_action);
    menu.addMenu(&helpSubMenu);

    menu.addAction(&quit_action);

    setContextMenu(&menu);

    connect(&quit_action, &QAction::triggered, this, []{
        xdotool.eventMonitor.terminate();
        xdotool.eventMonitor.wait();
        qApp->quit();
    });



    connect(&donate_action, &QAction::triggered, this, []{
        QDesktopServices::openUrl(QUrl("https://github.com/jiangzc/CuteTranslation#%E6%8D%90%E8%B5%A0"));
        QMessageBox::information(nullptr, "提示", "已在浏览器中打开网页");
    });

    connect(&homepage_action, &QAction::triggered, this, []{
        QDesktopServices::openUrl(QUrl("https://github.com/jiangzc/CuteTranslation"));
        QMessageBox::information(nullptr, "提示", "已在浏览器中打开网页");
    });

    connect(&autostart_action, &QAction::triggered, this, []{
        int res = system("cp /opt/CuteTranslation/CuteTranslation.desktop ~/.config/autostart/CuteTranslation.desktop");
        if (res == 0)
            QMessageBox::information(nullptr, "提示", "添加成功");
        else
        {
            QMessageBox::warning(nullptr, "提示", "错误：return " + QString::number(res));
        }
    });

    connect(&change_mode_all_action, &QAction::triggered, this, []{
        configTool.Mode = "all";
    });

    connect(&change_mode_custom_action, &QAction::triggered, this, []{
        configTool.Mode = "custom";
    });

    connect(&change_mode_none_action, &QAction::triggered, this, []{
        configTool.Mode = "none";
    });
    this->show();
}


