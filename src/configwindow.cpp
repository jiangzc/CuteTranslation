#include <QCloseEvent>
#include "configwindow.h"
#include "ui_configwindow.h"
#include "xdotool.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    this->move((xdotool.screenWidth - this->width()) / 2, (xdotool.screenHeight - this->height()) / 2);
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}
