#include <QCloseEvent>
#include <QComboBox>
#include "configwindow.h"
#include "ui_configwindow.h"
#include "xdotool.h"
#include "configtool.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    this->move((xdotool.screenWidth - this->width()) / 2, (xdotool.screenHeight - this->height()) / 2);
    ui->comboBox_mode->addItem("全局");
    ui->comboBox_mode->addItem("自定义");
    ui->comboBox_mode->addItem("禁用");
    ui->comboBox_mode->setStyleSheet("combobox-popup: 0;");
    ui->listView->setModel(&model);
    connect(ui->comboBox_mode, &QComboBox::currentTextChanged, this, [=](QString text){
        if (text == "全局")
        {
            configTool.Mode = "all";
        }
        else if (text == "自定义")
        {
            configTool.Mode = "custom";
        }
        else if (text == "禁用")
        {
            configTool.Mode = "none";
        }
    });

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

void ConfigWindow::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    model.setStringList(configTool.NotShow.split(":", QString::SkipEmptyParts));
    if (configTool.Mode == "all")
        ui->comboBox_mode->setCurrentIndex(0);
    else if (configTool.Mode == "custom")
        ui->comboBox_mode->setCurrentIndex(1);
    else if (configTool.Mode == "none")
        ui->comboBox_mode->setCurrentIndex(2);
}
