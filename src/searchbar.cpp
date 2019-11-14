#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QIcon>

#include "searchbar.h"
#include "ui_searchbar.h"


SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    qDebug() << ui->lineEdit->parent();
    ui->lineEdit->setStyleSheet("QLineEdit{background-color: white; border-radius: 20px;\
        padding: 2 2 2 10;border-style:solid;border-width:2px; border-color:rgb(192,192,192);\
        }");
    ui->lineEdit->setTextMargins(35,0,0,0);
    //ui->lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lineEdit->setPlaceholderText("Search");

    QPixmap pic(":/pic/icons-search.svg");

    searchIcon = new QLabel(this->ui->lineEdit);
    searchIcon->setFixedSize(30, 30);
    searchIcon->move(10,10);
    searchIcon->setPixmap(pic);
    searchIcon->setScaledContents(true);
    searchIcon->show();
    searchIcon->installEventFilter(this);


    hideButton = new QPushButton(this->ui->lineEdit);
    hideButton->setFixedSize(30, 30);
    hideButton->move(250, 10);
    hideButton->setStyleSheet("border:none;background-color:white; ");
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
    if (obj == this->searchIcon && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = reinterpret_cast<QMouseEvent *>(event);
        if (e->button() == Qt::LeftButton)
        {
            mDragPosition = e->pos() + this->ui->lineEdit->pos() + this->searchIcon->pos();
        }

    }
    if (obj == this->searchIcon && event->type() == QEvent::MouseMove)
    {
        QMouseEvent *e = reinterpret_cast<QMouseEvent *>(event);
        this->move(e->globalPos() - mDragPosition);

    }
    return false;
}
