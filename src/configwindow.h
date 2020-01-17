#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QMenu>


namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private:
    Ui::ConfigWindow *ui;
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
    bool event(QEvent *e);
    QMenu rightClickMenu;
    QAction remove_action;

signals:
    void SizeChanged(float zoom, int width);

};

#endif // CONFIGWINDOW_H
