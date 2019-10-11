#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <QWidget>

namespace Ui {
class FloatButton;
}

class FloatButton : public QWidget
{
    Q_OBJECT

public:
    explicit FloatButton(QWidget *parent = nullptr);

    ~FloatButton();

private:
    Ui::FloatButton *ui;
};

#endif // FLOATBUTTON_H
