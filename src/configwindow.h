#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QStringListModel>


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
    QStringListModel notShowModel;
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);

};

#endif // CONFIGWINDOW_H
