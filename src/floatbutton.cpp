#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMenu>
#include <QAction>


#include "floatbutton.h"
#include "ui_floatbutton.h"
#include "xdotool.h"

FloatButton::FloatButton(QWidget *parent) : QWidget(parent),
                                            ui(new Ui::FloatButton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: white; border-radius: 5px;border-style:solid;border-width:1px; border-color:rgb(192,192,192);");

    QPixmap *pic = new QPixmap(":/google-translate.png");
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setGeometry(0, 0, this->width(), this->height());
    label->setPixmap(*pic);
    // label->setStyleSheet("border-width:5");

    floatButtonMenu.addAction(&notShow);
    connect(&notShow, &QAction::triggered, this, [=]{
        configTool.NotShow += ":" + picker->CurrentWindowsPath;

    });

    picker->buttonReleased();
    connect(picker, &Picker::wordsPicked, this, &FloatButton::onWordPicked);

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
    else if (event->button() == Qt::RightButton)
    {
        int x, y;
        notShow.setText("不要在" + picker->CurrentWindowsPath.mid(1 + picker->CurrentWindowsPath.lastIndexOf("/")) + "显示");
        xdotool.getMousePosition(x, y);
        floatButtonMenu.move(x, y);
        floatButtonMenu.show();
    }
}

void FloatButton::onWordPicked(QString text)
{
    if (configTool.Mode == "none")
        return;
    else if (configTool.Mode == "custom" && configTool.Default == "on" && configTool.NotShow.contains(picker->CurrentWindowsPath))
    {
        return;
    }
    else if (configTool.Mode == "custom" && configTool.Default == "off" && configTool.Show.contains(picker->CurrentWindowsPath) == false)
    {
        return;
    }
    // qDebug() << xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
    qDebug() << "Text from picker" << text;
    int x, y;
    xdotool.getMousePosition(x, y);
    this->move(x - 10, y + 15);
    this->show();
    // this->activateWindow();
}
