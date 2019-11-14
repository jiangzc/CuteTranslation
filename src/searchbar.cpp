#include <QMouseEvent>
#include <QLabel>
#include <QPoint>
#include <QDebug>
#include <QIcon>

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
    PictureBox *searchIcon = new PictureBox(this->ui->lineEdit);
    searchIcon->setFixedSize(30, 30);
    searchIcon->move(10,10);
    QPixmap pic(":/pic/icons-search.svg");
    searchIcon->setPixmap(pic);
    searchIcon->setScaledContents(true);
    searchIcon->show();


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
    return false;
}
