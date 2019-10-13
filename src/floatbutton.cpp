#include "floatbutton.h"
#include "ui_floatbutton.h"
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include "xdotool.h"


FloatButton::FloatButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatButton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: white; border-radius: 5px;");

    QPixmap *pic = new QPixmap(":/google-translate.png");
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setGeometry(0, 0, this->width(), this->height());
    label->setPixmap(*pic);

    picker = new Picker();
    picker->buttonReleased();
    connect(picker, &Picker::wordsPicked,
            this, [=]
    {
        int x, y;
        xdotool.getMousePosition(x, y);
        this->move(x - 10, y + 15);
        this->show(); this->activateWindow();
    });
    connect(&xdotool.eventMonitor, &EventMonitor::buttonPress , this, &FloatButton::onMouseButtonPressed ,Qt::QueuedConnection );
    connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease , this, &FloatButton::onMouseButtonReleased ,Qt::QueuedConnection );
}

FloatButton::~FloatButton()
{
    delete ui;
}

void FloatButton::onMouseButtonPressed(int x, int y)
{
    picker->buttonPressed();

    if (x < this->x() || x > this->x() + width())
        hide();
    if (y < this->y() || y > this->y() + height())
        hide();
}

void FloatButton::onMouseButtonReleased(int x, int y)
{
    picker->buttonReleased();
}
