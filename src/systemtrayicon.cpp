#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent):QSystemTrayIcon(parent)
{
    QIcon icon(":/icon.png");
    setIcon(icon);
    quit_action.setText("退出");
    menu.addAction(&quit_action);
    setContextMenu(&menu);
}
