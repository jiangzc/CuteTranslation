#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "picker.h"

namespace Ui {
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
    void paintEvent(QPaintEvent * event);
    Picker *picker;
    const int Triangle_Height = 15;
    const int Triangle_Width = 15;
    int Triangle_Offset = 0;
    const int Direction_Up = 0;
    const int Direction_Down = 1;
    int Direction = Direction_Up;

public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
};

#endif // MAINWINDOW_H
