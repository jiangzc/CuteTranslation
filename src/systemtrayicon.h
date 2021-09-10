#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include "configtool.h"

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
    QAction ocr_translate_action;
    QAction change_mode_all_action;
    QAction change_mode_none_action;
    QAction change_mode_custom_action;
    QAction autostart_action;
    QAction ocr_text_action;

  private:
    QMenu menu;
    QMenu modeSubMenu;
    QMenu helpSubMenu;
    QIcon icon;
    QIcon icon_grey;

public slots:
    void OnModeChanged(ModeSet mode);

};

#endif // SYSTEMTRAYICON_H
