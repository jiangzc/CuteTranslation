#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configtool.h"
#include "picker.h"
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
    const int TriangleHeight = configTool.TriangleHeight;
    const int TriangleWidth = configTool.TriangleWidth;
    int TriangleOffset = 0;
    const int Direction_Up = 0;
    const int Direction_Down = 1;
    int Direction;

  public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
};

#endif // MAINWINDOW_H
