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

extern const int Direction_Up;
extern const int Direction_Down;

FloatButton::FloatButton(QWidget *parent) : QWidget(parent),
                                            ui(new Ui::FloatButton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(configTool->FloatButtonWidth, configTool->FloatButtonHeight);

    QPixmap *pic = new QPixmap(":/pic/google-translate.png");
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setGeometry(0, 0, this->width(), this->height());
    label->setPixmap(*pic);
    label->setStyleSheet("background-color: white; border-radius: 5px;border-style:solid;border-width:1px; border-color:rgb(192,192,192);");


    floatButtonMenu.addAction(&notShow);
    connect(&notShow, &QAction::triggered, this, [=]{
        if (configTool->GetMode() == Mode_ALL)
            configTool->SetMode(Mode_CUSTOM);
        if (configTool->NotShow.value.contains(picker->CurrentWindowsPath) == false)
            configTool->NotShow += ":" + picker->CurrentWindowsPath;
        this->hide();
    });

    picker->buttonReleased();


}

FloatButton::~FloatButton()
{
    delete ui;
}

void FloatButton::onMouseButtonPressed(int x, int y)
{
    // qInfo() << "FloatButton::onMouseButtonPressed, press at :" << mousePressPosition;
    if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
    {
        mousePressPosition.setX(x);
        mousePressPosition.setY(y);
    }
    if (this->isHidden() == false)
    {
        if (x < this->x() || x > this->x() + width() || y < this->y() || y > this->y() + height())
        {
            // 如果点击菜单，此处 hide()与triggered()有可能形成竞争，所以加一个判断。
            if (floatButtonMenu.isHidden())
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
        qInfo() << "floatButtonPressed";
        emit floatButtonPressed(mousePressPosition, mouseReleasedPosition);
    }
    else if (event->button() == Qt::RightButton)
    {
        this->activateWindow();
        notShow.setText("不要在" + picker->CurrentWindowsPath + "显示");
        floatButtonMenu.exec(QCursor::pos());
    }
    QWidget::mousePressEvent(event);
}

void FloatButton::onWordPicked(QString text)
{
    if (configTool->GetMode() == Mode_NONE)
    {
        return;
    }
    else if (configTool->GetMode() == Mode_CUSTOM && configTool->Undefined == "Show" && configTool->NotShow.value.contains(picker->CurrentWindowsPath))
    {
        return;
    }
    else if (configTool->GetMode() == Mode_CUSTOM && configTool->Undefined == "NotShow" && configTool->Show.value.contains(picker->CurrentWindowsPath) == false)
    {
        return;
    }
    else if (configTool->GetMode() == Mode_CUSTOM && configTool->ChineseNotShow)
    {
        // 分析text中文比例是否超过70%
        int chineseChars = 0;
        for (auto &c : text)
        {
            if(c.unicode() >= 0x4E00 && c.unicode() <= 0x9FA5)
            {
                // 这个字符是中文
                chineseChars++;
            }
        }
        if (1.0 * chineseChars / text.size() > 0.7)
            return;
    }

    qInfo() << "Text from picker" << text;

    int y;
    int direction = configTool->Direction;
    int mainWindowHeight = configTool->MainWindowHeight;

    if (direction == Direction_Up)
        y = std::max(mousePressPosition.y(), mouseReleasedPosition.y()) + 15;
    else
        y = std::min(mousePressPosition.y(), mouseReleasedPosition.y()) - mainWindowHeight - 15;

    // 判断是否超出屏幕上边界
    if (direction == Direction_Down && y < 0)
    {
        direction = Direction_Up;
    }
    // 判断是否超出屏幕下边界
    if (direction == Direction_Up && y + mainWindowHeight > xdotool->screenHeight)
    {
        direction = Direction_Down;
    }

    if (direction == 1)
        this->move(QCursor::pos() + QPoint(-10, 15));
    else
        this->move(QCursor::pos() + QPoint(-10, -(15 + this->height())));

    this->show();
    // this->activateWindow();
}

void FloatButton::onKeyPressed(int keyCode)
{
    if (this->isHidden() == false)
        this->hide();
}
