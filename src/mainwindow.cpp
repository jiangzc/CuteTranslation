#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <algorithm>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xdotool.h"
#include "picker.h"

// Why does “extern const int n;” not work as expected?
// https://stackoverflow.com/questions/14894698/why-does-extern-const-int-n-not-work-as-expected

extern const int Direction_Up;
extern const int Direction_Down;
const int Direction_Up = 0;
const int Direction_Down = 1;
extern QString TranslateWord(QString word);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->view = new QWebEngineView(this->centralWidget());

    view->setZoomFactor(1.2);
    view->setGeometry(5,10,490,350);
    connect(view, &QWebEngineView::loadFinished, this, [=]{
        view->page()->runJavaScript("document.body.clientHeight;",[=](QVariant result){
            int newHeight=result.toInt() * 1.2 + 10;
            view->setFixedSize(view->width(),newHeight);
            this->setFixedHeight(newHeight + 30);
            emit gotHeight();
        });
    });

    setFixedSize(configTool.MainWindowWidth, configTool.MainWindowHeight);

    QFile file(QCoreApplication::applicationDirPath() + "/interpret_js_2.html");
    if (!file.open(QFile::ReadOnly | QFile::Text))
        qDebug() << "fail to open";
    QTextStream in(&file);
    this->html = in.readAll();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QColor greyColor(192, 192, 192);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

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

}

void MainWindow::onMouseButtonPressed(int x, int y)
{
    if (!this->isHidden() && (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height()))
        hide();

}

void MainWindow::onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition)
{
    QEventLoop qel;
    connect(this, &MainWindow::gotHeight, &qel, &QEventLoop::quit);
    // 获取翻译
    QString json = TranslateWord(picker->getSelectedText());
    qDebug() << json;
    if (json.startsWith("{"))
    {
        QString html = this->html;
        this->view->setHtml(html.replace("\"{0}\"", json));
        qel.exec();
    }
    else
    {
        this->view->setHtml(json);
        qel.exec();
        this->view->setFixedHeight(300);
        this->setFixedSize(configTool.MainWindowWidth, configTool.MainWindowHeight);

    }

    // 获取默认方向向 重置三角形偏移量
    int direction = configTool.Direction;
    TriangleOffset = 0;

    QPoint mid(0, 0);
    mid.rx() = (mousePressPosition.x() + mouseReleasedPosition.x() - width()) / 2;

    if (direction == Direction_Up)
        mid.ry() = std::max(mousePressPosition.y(), mouseReleasedPosition.y()) + 15;
    else
        mid.ry() = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - this->height() - 15;
    // 判断是否超出屏幕上边界
    if (direction == Direction_Down && mid.y() < 0)
    {
        direction = Direction_Up;
        mid.ry() = std::max(mousePressPosition.y(), mouseReleasedPosition.y()) + 15;
    }
    // 判断是否超出屏幕下边界
    if (direction == Direction_Up && mid.y() + this->height() > xdotool.screenHeight)
    {
        direction = Direction_Down;
        mid.ry() = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - this->height() - 15;
    }
    Direction = direction;
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
    this->show();

}
