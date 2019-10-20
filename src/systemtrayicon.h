#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QObject>
#include <QSystemTrayIcon>

class SystemTrayIcon : public QSystemTrayIcon
{
  public:
    SystemTrayIcon(QObject *parent = nullptr);
    QAction quit_action;
    QAction config_action;
    QAction help_action;
    QAction homepage_action;
    QAction donate_action;
    QAction search_action;
    QAction ocr_action;
    QAction change_mode_all_action;
    QAction change_mode_none_action;
    QAction change_mode_custom_action;


  private:
    QMenu menu;
    QMenu modeSubMenu;
    QMenu helpSubMenu;

};

#endif // SYSTEMTRAYICON_H
