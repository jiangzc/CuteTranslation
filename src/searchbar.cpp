#include "searchbar.h"
#include "ui_searchbar.h"

SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);


    ui->lineEdit->setStyleSheet("QLineEdit{background-color: white; border-radius: 20px;\
        padding: 2 2 2 10;border-style:solid;border-width:2px; border-color:rgb(192,192,192);\
        background-image: url(:/pic/icons-search.svg);background-repeat: no-repeat;background-position: left;\
        background-origin: content;}");
    ui->lineEdit->setTextMargins(35,0,0,0);
    ui->lineEdit->setPlaceholderText("Search");

    this->move(600, 500);

}

SearchBar::~SearchBar()
{
    delete ui;
}
