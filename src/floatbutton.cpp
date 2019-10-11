#include "floatbutton.h"
#include "ui_floatbutton.h"
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

FloatButton::FloatButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatButton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: white; border-radius: 5px;");

    QPixmap *pic = new QPixmap(":/google-translate.png");
    QLabel *label = new QLabel(this);
    label->setScaledContents(true);
    label->setGeometry(0, 0, this->width(), this->height());
    label->setPixmap(*pic);


}

FloatButton::~FloatButton()
{
    delete ui;
}



