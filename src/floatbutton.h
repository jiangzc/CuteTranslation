#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <QWidget>
#include "picker.h"
#include "mainwindow.h"
namespace Ui {
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
    void _mousePressEvent();

signals:
    void floatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
private:
    Ui::FloatButton *ui;
    Picker *picker;
    QPoint mousePressPosition;
    QPoint mouseReleasedPosition;
    void onMouseButtonPressed(int x, int y); // global listen
    void onMouseButtonReleased(int x, int y); // global listen


};

#endif // FLOATBUTTON_H
