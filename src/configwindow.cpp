#include <QCloseEvent>
#include <QComboBox>
#include <QMenu>

#include "xdotool.h"
#include "ui_configwindow.h"
#include "configwindow.h"
#include "configtool.h"


ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow), rightClickMenu(this), remove_action("删除", this)
{
    ui->setupUi(this);
    this->move((xdotool.screenWidth - this->width()) / 2, (xdotool.screenHeight - this->height()) / 2);
    ui->comboBox_mode->addItem("全局");
    ui->comboBox_mode->addItem("自定义");
    ui->comboBox_mode->addItem("禁用");
    ui->comboBox_mode->setStyleSheet("combobox-popup: 0;");
    ui->comboBox_undefined->addItem("显示");
    ui->comboBox_undefined->addItem("不显示");
    ui->comboBox_undefined->setStyleSheet("combobox-popup: 0;");
    ui->listWidget->setStyleSheet(" QListWidget {padding:5px; background-color:white;} QListWidget::item { padding: 5px; }");

    rightClickMenu.addAction(&remove_action);

    connect(ui->listWidget, &QListWidget::itemPressed, ui->listWidget, [=] {
        if (QGuiApplication::mouseButtons() == Qt::RightButton)
            rightClickMenu.exec(QCursor::pos());

    });

    connect(&remove_action, &QAction::triggered, this, [=] {
        configTool->NotShow -= ui->listWidget->currentItem()->text();
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    });


    connect(ui->comboBox_mode, &QComboBox::currentTextChanged, this, [=](QString text){
        if (text == "全局")
        {
            configTool->Mode = "all";
        }
        else if (text == "自定义")
        {
            configTool->Mode = "custom";
        }
        else if (text == "禁用")
        {
            configTool->Mode = "none";
        }
    });
    connect(ui->comboBox_undefined, &QComboBox::currentTextChanged, this, [=](QString text){
        if (text == "不显示")
        {
            configTool->Undefined = "NotShow";
            ui->label_3->setText("启用：");
        }
        else if (text == "显示")
        {
            configTool->Undefined = "Show";
            ui->label_3->setText("禁用:");
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
    // 更新列表
    ui->listWidget->clear();
    ui->listWidget->addItems(configTool->NotShow.value.split(":", QString::SkipEmptyParts));
    if (configTool->Mode == "all")
        ui->comboBox_mode->setCurrentIndex(0);
    else if (configTool->Mode == "custom")
        ui->comboBox_mode->setCurrentIndex(1);
    else if (configTool->Mode == "none")
        ui->comboBox_mode->setCurrentIndex(2);

    // 更新显示设置
    if (configTool->Undefined == "Show")
    {
        ui->comboBox_undefined->setCurrentIndex(0);
        ui->label_3->setText("禁用：");

    }
    else if (configTool->Undefined == "NotShow")
    {
        ui->comboBox_undefined->setCurrentIndex(1);
        ui->label_3->setText("启用：");
    }
}

bool ConfigWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
        this->hide();
    }
    return QWidget::event(e);
}
