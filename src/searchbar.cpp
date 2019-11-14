#include <QMouseEvent>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QDebug>
#include "searchbar.h"
#include "ui_searchbar.h"
class PictureBox;
class PictureBox : public QLabel
{

private:
    QPoint mDragPosition;
protected:
    virtual void mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            mDragPosition = event->pos();
            event->accept();
        }
    }
    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        reinterpret_cast<QWidget*>(this->parent()->parent())->move(event->globalPos() - mDragPosition
                                   - reinterpret_cast<QWidget*>(this->parent())->pos() - this->pos());

    }
public:
    explicit PictureBox(QWidget *parent = nullptr);
    ~PictureBox();
};

PictureBox::PictureBox(QWidget *parent) : QLabel(parent)
{

}

PictureBox::~PictureBox()
{

}

SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    qDebug() << ui->lineEdit->parent();
    ui->lineEdit->setStyleSheet("QLineEdit{background-color: white; border-radius: 20px;\
        padding: 2 2 2 10;border-style:solid;border-width:2px; border-color:rgb(192,192,192);\
        }");
    ui->lineEdit->setTextMargins(35,0,0,0);
    //ui->lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lineEdit->setPlaceholderText("Search");
    PictureBox *searchIcon = new PictureBox(this->ui->lineEdit);
    searchIcon->setFixedSize(30, 30);
    searchIcon->move(10,10);
    QPixmap *pic = new QPixmap(":/pic/icons-search.svg");
    searchIcon->setPixmap(*pic);
    searchIcon->setScaledContents(true);
    searchIcon->show();

    this->move(600, 500);

}

SearchBar::~SearchBar()
{
    delete ui;
}


void SearchBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDragPosition = event->pos();
        event->accept();
    }
}

void SearchBar::mouseMoveEvent(QMouseEvent *event)
{

        move(event->globalPos() - mDragPosition);

}
