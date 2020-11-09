#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QPushButton>

#include "searchbar.h"
#include "ui_searchbar.h"


SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->lineEdit->setTextMargins(35,0,0,0);
    ui->lineEdit->setPlaceholderText("Search");
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, [=]{
        emit this->returnPressed(this->pos(), QPoint(this->width(), this->height()), this->ui->lineEdit->text());
    });

    QPixmap pic(":/pic/icons-search.svg");
    pic = pic.scaled(30, 30);
    searchIcon = new QLabel(this->ui->lineEdit);
    searchIcon->setObjectName("searchIcon");
    searchIcon->setFixedSize(40, 30); // 延长接触区域
    searchIcon->move(0, 10);
    searchIcon->setPixmap(pic);
    searchIcon->installEventFilter(this);
    searchIcon->show();

    hideButton = new QPushButton(this->ui->lineEdit);
    hideButton->setFixedSize(40, 30); // 延长接触区域
    hideButton->move(250, 10);
    hideButton->setFlat(true);
    hideButton->setIconSize(QSize(40, 40));
    hideButton->show();
    hideButton->installEventFilter(this);
    connect(hideButton, &QPushButton::clicked, this, &QWidget::hide);

    this->move(600, 500);

}

SearchBar::~SearchBar()
{
    delete ui;
}


bool SearchBar::eventFilter(QObject *obj, QEvent *event)
{
    // 当鼠标悬停在特定区域，显示取消按钮
    if (obj == this->hideButton && event->type() == QEvent::HoverEnter)
    {
        QPixmap pic = QPixmap(":/pic/icons-cancel.svg");
        hideButton->setIcon(pic);
        return true;
    }
    if (obj == this->hideButton && event->type() == QEvent::HoverLeave)
    {
        hideButton->setIcon(QIcon());
        return true;
    }
    // 拖动悬浮框
    if (obj == this->searchIcon && event->type() == QEvent::MouseButtonPress)
    {
        ui->lineEdit->deselect();
        QMouseEvent *e = reinterpret_cast<QMouseEvent *>(event);
        if (e->button() == Qt::LeftButton)
        {
            mDragPosition = e->pos() + this->ui->lineEdit->pos() + this->searchIcon->pos();
        }

    }
    if (obj == this->searchIcon && event->type() == QEvent::MouseMove)
    {
        ui->lineEdit->deselect();
        QMouseEvent *e = reinterpret_cast<QMouseEvent *>(event);
        this->move(e->globalPos() - mDragPosition);

    }
    return false;
}

void SearchBar::showEvent(QShowEvent *e)
{
    this->ui->lineEdit->setFocus();
    this->ui->lineEdit->clear();
    this->activateWindow();
    e->ignore();
}

void SearchBar::ClearLineEdit()
{
    this->ui->lineEdit->clear();
    this->ui->lineEdit->setFocus();
}

void SearchBar::OnSearchBarShortCutPressed(QWidget *mainWindow)
{
    if (isHidden())
    {
        move(QCursor::pos() - QPoint(150, 25));
        show();
        activateWindow();
    }
    else if (isActiveWindow() == false && mainWindow->isActiveWindow() == false)
    {
        // 这么做看全屏视频时，用悬浮搜索框有更好的体验。
        move(QCursor::pos() - QPoint(150, 25));
        ClearLineEdit();
        activateWindow();
    }
    else
        hide();
}
