#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include "systemtrayicon.h"
#include "xdotool.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent):QSystemTrayIcon(parent),
    quit_action("退出"), config_action("配置"), help_action("帮助手册"), search_action("文字翻译"), ocr_action("截图翻译"),
    change_mode_all_action("全局"), change_mode_none_action("禁用"), change_mode_custom_action("自定义"),
    homepage_action("项目主页"), donate_action("捐赠"),
    modeSubMenu("取词模式", &menu), helpSubMenu("帮助", &menu)
{
    QIcon icon(":/icon.png");
    setIcon(icon);

    modeSubMenu.addAction(&change_mode_all_action);
    modeSubMenu.addAction(&change_mode_custom_action);
    modeSubMenu.addAction(&change_mode_none_action);
    menu.addMenu(&modeSubMenu);


    menu.addAction(&search_action);
    menu.addAction(&ocr_action);
    menu.addAction(&config_action);

    helpSubMenu.addAction(&help_action);
    helpSubMenu.addAction(&homepage_action);
    helpSubMenu.addAction(&donate_action);
    menu.addMenu(&helpSubMenu);

    menu.addAction(&quit_action);

    setContextMenu(&menu);

    connect(&quit_action, &QAction::triggered, this, [=]{
        xdotool.eventMonitor.terminate();
        xdotool.eventMonitor.wait();
        qApp->quit();
    });

    connect(&homepage_action, &QAction::triggered, this, [=]{
        QDesktopServices::openUrl(QUrl("https://github.com/jiangzc/CuteTranslation"));
    });


}
