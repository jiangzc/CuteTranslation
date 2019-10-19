#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

class SystemTrayIcon : public QSystemTrayIcon
{
  public:
    SystemTrayIcon(QObject *parent = nullptr);
    QAction quit_action;

  private:
    QMenu menu;

};

#endif // SYSTEMTRAYICON_H
