#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QColor>
#include <QDebug>
#include "xdotool.h"



const int Triangle_Height = 15;
const int Triangle_Width = 15;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
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
            int x,y;
            getMousePosition(x, y);
            qDebug() << x << y;
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
    polygon << QPoint(0, 0);
    polygon << QPoint(this->width(), 0);
    polygon << QPoint(this->width() ,this->height() - Triangle_Height);
    polygon << QPoint(this->width() / 2 + Triangle_Width, this->height() - Triangle_Height);
    polygon << QPoint(this->width() / 2 , this->height());
    polygon << QPoint(this->width() / 2 - Triangle_Width, this->height() - Triangle_Height);
    polygon << QPoint(0 ,this->height() - Triangle_Height);
    painter.drawPolygon(polygon, Qt::WindingFill);



}
