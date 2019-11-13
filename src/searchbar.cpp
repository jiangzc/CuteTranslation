#include "searchbar.h"
#include "ui_searchbar.h"

SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //ui->frame->setGeometry(5, 5, 390, 50);

    ui->lineEdit->setStyleSheet("QLineEdit{background-color: white; border-radius: 10px;padding: 2 2 2 20;border-style:solid;border-width:1px; border-color:rgb(192,192,192);}");
    this->move(300, 300);
    //ui->lineEdit->setCursorPosition(3);
}

SearchBar::~SearchBar()
{
    delete ui;
}
