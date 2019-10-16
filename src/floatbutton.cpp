#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "floatbutton.h"
#include "ui_floatbutton.h"
#include "xdotool.h"

FloatButton::FloatButton(QWidget *parent) : QWidget(parent),
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
    connect(picker, &Picker::wordsPicked, this, [=](QString text) {
        qDebug() << "Text from picker" << text;
        int x, y;
        xdotool.getMousePosition(x, y);
        this->move(x - 10, y + 15);
        this->show();
        // this->activateWindow();
    });
    connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, this, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, this, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
}

FloatButton::~FloatButton()
{
    delete ui;
}

void FloatButton::onMouseButtonPressed(int x, int y)
{
    // qDebug() << "FloatButton::onMouseButtonPressed, press at :" << mousePressPosition;
    picker->buttonPressed();
    mainWindow->onMouseButtonPressed(x, y);
    if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
    {
        mousePressPosition.setX(x);
        mousePressPosition.setY(y);
    }
    if (this->isHidden())
    {

    }
    else
    {
        if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
        {
            hide();
            move(0, 0);
        }
        else
        {
            // go to mousePressEvent
        }
    }
}

void FloatButton::onMouseButtonReleased(int x, int y)
{
    // qDebug() << "FloatButton::onMouseButtonReleased, release at " << mouseReleasedPosition;
    picker->buttonReleased();
    if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
    {
        mouseReleasedPosition.setX(x);
        mouseReleasedPosition.setY(y);
    }
}

void FloatButton::mousePressEvent(QMouseEvent *event)
{
    // 重写窗口鼠标按下事件
    if (event->button() == Qt::LeftButton)
    {
        this->hide();
        qDebug() << "floatButtonPressed";
        emit floatButtonPressed(mousePressPosition, mouseReleasedPosition);
    }
}
