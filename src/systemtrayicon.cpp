#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent):QSystemTrayIcon(parent),
    quit_action("退出"), config_action("配置"), help_action("帮助"),search_action("文字翻译"), ocr_action("截图翻译"),
    change_mode_all_action("全局"), change_mode_none_action("禁用"), change_mode_custom_action("自定义"),
    modeSubMenu("模式", &menu)
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
    menu.addAction(&help_action);
    menu.addAction(&quit_action);



    setContextMenu(&menu);
}
