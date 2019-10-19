#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "picker.h"
#include "configtool.h"
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);
    Picker *picker;
    const int TriangleHeight = configTool.TriangleHeight;
    const int TriangleWidth = configTool.TriangleWidth;
    int TriangleOffset = 0;
    const int Direction_Up = 0;
    const int Direction_Down = 1;
    int Direction = Direction_Up;

  public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
};

#endif // MAINWINDOW_H
