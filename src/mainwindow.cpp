#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <algorithm>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xdotool.h"

// TODO:

// 右键菜单，配置文件

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
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
        polygon << QPoint(this->width(), this->height() - Triangle_Height);
        polygon << QPoint(this->width() / 2 + Triangle_Width + Triangle_Offset, this->height() - Triangle_Height);
        polygon << QPoint(this->width() / 2 + Triangle_Offset, this->height());
        polygon << QPoint(this->width() / 2 - Triangle_Width + Triangle_Offset, this->height() - Triangle_Height);
        polygon << QPoint(0, this->height() - Triangle_Height);
    }
    else if (Direction == Direction_Up)
    {
        centralWidget()->move(centralWidget()->x(), Triangle_Height);
        polygon << QPoint(0, Triangle_Height);
        polygon << QPoint(this->width() / 2 - Triangle_Width + Triangle_Offset, Triangle_Height);
        polygon << QPoint(this->width() / 2 + Triangle_Offset, 0);
        polygon << QPoint(this->width() / 2 + Triangle_Width + Triangle_Offset, Triangle_Height);
        polygon << QPoint(this->width(), Triangle_Height);
        polygon << QPoint(this->width(), this->height());
        polygon << QPoint(0, this->height());
    }

    painter.drawPolygon(polygon, Qt::WindingFill);
}

void MainWindow::onMouseButtonPressed(int x, int y)
{
    if (!this->isHidden() && (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height()))
        hide();

}

void MainWindow::onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition)
{
    // 默认方向向上 重置三角形偏移量
    Direction = Direction_Up;
    Triangle_Offset = 0;
    QPoint mid = QPoint((mousePressPosition.x() + mouseReleasedPosition.x()) / 2,
                        std::max(mousePressPosition.y(), mouseReleasedPosition.y()));
    mid.rx() -= this->width() / 2;
    mid.ry() += 15;
    // 判断是否超出屏幕下边界
    if (mid.y() + this->height() > xdotool.screenHeight)
    {
        Direction = Direction_Down;
        mid.ry() = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - this->height() - 15;
    }
    // 判断是否超出屏幕左边界
    if (mid.x() < 15)
    {
        Triangle_Offset = 15 - mid.x();
        if (Triangle_Offset > this->width() / 2 - Triangle_Width - 20)
            Triangle_Offset = this->width() / 2 - Triangle_Width - 20;
        mid.rx() = 15;
        Triangle_Offset = -Triangle_Offset;
    }
    // 判断是否超出屏幕右边界
    if (mid.x() + this->width() > xdotool.screenWidth - 15)
    {
        Triangle_Offset = mid.x() + this->width() - (xdotool.screenWidth - 15);
        if (Triangle_Offset > this->width() / 2 - Triangle_Width - 20)
            Triangle_Offset = this->width() / 2 - Triangle_Width - 20;
        mid.rx() = xdotool.screenWidth - 15 - this->width();
        // Triangle_Offset = -Triangle_Offset;
    }
    move(mid);
    ui->label->setText(picker->getSelectedText());
    ui->textBrowser->setText(picker->getSelectedText());
    this->show();
}
