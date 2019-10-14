#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QColor>
#include <QDebug>
#include "xdotool.h"

// TODO:
// 四种选择方向，mainWin出现的位置相应变化
// 根据鼠标位置和屏幕大小，设置mainWin方向

const int Triangle_Height = 15;
const int Triangle_Width = 15;
const int Direction_Up = 0;
const int Direction_Down = 1;
int Direction = Direction_Up;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground);
    connect(&xdotool.eventMonitor, &EventMonitor::buttonPress , this, &MainWindow::onMouseButtonPressed ,Qt::QueuedConnection );
    xdotool.eventMonitor.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    QColor greyColor(192, 192, 192);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // painter.fillRect(QRect(0, 0, this->width() , this->height() - 30), QBrush(Qt::white));
    QPen pen;
    pen.setColor(greyColor);
    pen.setWidth(3);
    painter.setPen(pen);

    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    QPolygon polygon;
    if (Direction == Direction_Down)
    {
        polygon << QPoint(0, 0);
        polygon << QPoint(this->width(), 0);
        polygon << QPoint(this->width() ,this->height() - Triangle_Height);
        polygon << QPoint(this->width() / 2 + Triangle_Width, this->height() - Triangle_Height);
        polygon << QPoint(this->width() / 2 , this->height());
        polygon << QPoint(this->width() / 2 - Triangle_Width, this->height() - Triangle_Height);
        polygon << QPoint(0 ,this->height() - Triangle_Height);
    }
    else if (Direction == Direction_Up)
    {
        centralWidget()->move(centralWidget()->x(), Triangle_Height);
        polygon << QPoint(0, Triangle_Height);
        polygon << QPoint(this->width() / 2 - Triangle_Width, Triangle_Height);
        polygon << QPoint(this->width() / 2 , 0);
        polygon << QPoint(this->width() / 2 + Triangle_Width, Triangle_Height);
        polygon << QPoint(this->width(), Triangle_Height);
        polygon << QPoint(this->width() ,this->height());
        polygon << QPoint(0 ,this->height());
    }

    painter.drawPolygon(polygon, Qt::WindingFill);
}

void MainWindow::onMouseButtonPressed(int x, int y)
{
    qDebug() << "cc";
    if (x < this->x() || x > this->x() + width())
        hide();
    if (y < this->y() || y > this->y() + height())
        hide();
}

void MainWindow::onFloatButtonPressed(QPoint mid)
{
    mid.rx() -= this->width() / 2;
    mid.ry() += 10;
    qDebug() << "mid" << mid;
    move(mid);
    this->show();
}
