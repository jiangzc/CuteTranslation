#include "floatbutton.h"
#include "ui_floatbutton.h"
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
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
    connect(picker, &Picker::wordsPicked, this, [=]
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
    mainWindow->onMouseButtonPressed(x, y);
    if (this->isHidden())
    {
        mousePressPosition.setX(x);
        mousePressPosition.setY(y);
        qDebug() << "press:" << mousePressPosition;
    }
    else
    {
        if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
        {
            hide();
        }
        else
        {
             // go to mousePressEvent
        }
    }




}

void FloatButton::onMouseButtonReleased(int x, int y)
{
    picker->buttonReleased();
    if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
    {
        mouseReleasedPosition.setX(x);
        mouseReleasedPosition.setY(y);
        qDebug() << "release:" << mouseReleasedPosition;
    }
    else
    {

    }
}

void FloatButton::mousePressEvent(QMouseEvent *event)
{
    // 重写窗口鼠标按下事件
    if (event->button() == Qt::LeftButton)
    {
        this->hide();
        emit floatButtonPressed(mousePressPosition, mouseReleasedPosition);

    }
}
