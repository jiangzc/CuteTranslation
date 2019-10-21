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
    resize(configTool.MainWindowWidth, configTool.MainWindowHeight);
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
        polygon << QPoint(this->width(), this->height() - TriangleHeight);
        polygon << QPoint(this->width() / 2 + TriangleWidth + TriangleOffset, this->height() - TriangleHeight);
        polygon << QPoint(this->width() / 2 + TriangleOffset, this->height());
        polygon << QPoint(this->width() / 2 - TriangleWidth + TriangleOffset, this->height() - TriangleHeight);
        polygon << QPoint(0, this->height() - TriangleHeight);
    }
    else if (Direction == Direction_Up)
    {
        centralWidget()->move(centralWidget()->x(), TriangleHeight);
        polygon << QPoint(0, TriangleHeight);
        polygon << QPoint(this->width() / 2 - TriangleWidth + TriangleOffset, TriangleHeight);
        polygon << QPoint(this->width() / 2 + TriangleOffset, 0);
        polygon << QPoint(this->width() / 2 + TriangleWidth + TriangleOffset, TriangleHeight);
        polygon << QPoint(this->width(), TriangleHeight);
        polygon << QPoint(this->width(), this->height());
        polygon << QPoint(0, this->height());
    }

    painter.drawPolygon(polygon, Qt::WindingFill);
    QMainWindow::paintEvent(event);
}

void MainWindow::onMouseButtonPressed(int x, int y)
{
    if (!this->isHidden() && (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height()))
        hide();

}

void MainWindow::onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition)
{
    // 默认方向向上 重置三角形偏移量
    Direction = configTool.Direction;
    TriangleOffset = 0;

    QPoint mid;
    mid.rx() = (mousePressPosition.x() + mouseReleasedPosition.x() - width()) / 2;

    if (Direction == Direction_Up)
        mid.ry() = std::max(mousePressPosition.y(), mouseReleasedPosition.y()) + 15;
    else
        mid.ry() = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - this->height() - 15;

    // 判断是否超出屏幕上边界
    if (Direction == Direction_Down && mid.y() < 0)
    {
        Direction = Direction_Up;
        mid.ry() = std::max(mousePressPosition.y(), mouseReleasedPosition.y()) + 15;
    }
    // 判断是否超出屏幕下边界
    if (Direction == Direction_Up && mid.y() + this->height() > xdotool.screenHeight)
    {
        Direction = Direction_Down;
        mid.ry() = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - this->height() - 15;
    }
    // 判断是否超出屏幕左边界
    if (mid.x() < configTool.Edge)
    {
        TriangleOffset = configTool.Edge - mid.x();
        if (TriangleOffset > this->width() / 2 - TriangleWidth * 2)
            TriangleOffset = this->width() / 2 - TriangleWidth * 2;
        mid.rx() = configTool.Edge;
        TriangleOffset = -TriangleOffset;
    }
    // 判断是否超出屏幕右边界
    if (mid.x() + this->width() > xdotool.screenWidth - configTool.Edge)
    {
        TriangleOffset = mid.x() + this->width() - (xdotool.screenWidth - configTool.Edge);
        if (TriangleOffset > this->width() / 2 - TriangleWidth * 2)
            TriangleOffset = this->width() / 2 - TriangleWidth * 2;
        mid.rx() = xdotool.screenWidth - configTool.Edge - this->width();
    }
    move(mid);
    ui->label->setText(picker->getSelectedText());
    ui->textBrowser->setText(picker->getSelectedText());
    this->show();
}
