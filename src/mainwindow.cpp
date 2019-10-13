#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QColor>
#include <QDebug>
#include "xdotool.h"



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
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    picker = new Picker();
    picker->buttonPressed();
    connect(picker, &Picker::wordsPicked,
            this, [=] {qDebug() << "dd" ;this->show(); this->activateWindow();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
            qDebug() << "aaa";
            //int x,y;
            //xdotool.getMousePosition(x, y);
            qDebug() << xdotool.getActiveWindowName();
            //qDebug() << x << y;
            this->hide();
    }
    return QMainWindow::event(e);

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
