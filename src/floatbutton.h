#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <QWidget>
#include <QMenu>
#include <QString>
#include "mainwindow.h"


namespace Ui
{
class FloatButton;
}

class FloatButton : public QWidget
{
    Q_OBJECT

  public:
    explicit FloatButton(QWidget *parent = nullptr);
    MainWindow *mainWindow;
    ~FloatButton();

  protected:
    void mousePressEvent(QMouseEvent *event);

  signals:
    void floatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);

  private:
    Ui::FloatButton *ui;
    QMenu floatButtonMenu;
    QAction notShow;
    QPoint mousePressPosition;
    QPoint mouseReleasedPosition;
    void onMouseButtonPressed(int x, int y);  // global listen
    void onMouseButtonReleased(int x, int y); // global listen

private slots:
    void onWordPicked(QString text);
};

#endif // FLOATBUTTON_H
