#include <QCloseEvent>
#include <QComboBox>
#include <QMenu>
#include "configtool.h"
#include "configwindow.h"
#include "ui_configwindow.h"
#include "xdotool.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow), rightClickMenu(this), remove_action("删除", this)
{
    ui->setupUi(this);
    this->move((xdotool->screenWidth - this->width()) / 2, (xdotool->screenHeight - this->height()) / 2);
    ui->comboBox_mode->addItem("全局", Mode_ALL);
    ui->comboBox_mode->addItem("自定义", Mode_CUSTOM);
    ui->comboBox_mode->addItem("禁用", Mode_NONE);
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

    void (QComboBox::*fp)(int) = &QComboBox::currentIndexChanged ;
    connect(ui->comboBox_mode, fp, this, [=](int index){
        ModeSet mode = ModeSet(ui->comboBox_mode->itemData(index).toInt());
        configTool->SetMode(mode);
    });
    connect(ui->comboBox_undefined, &QComboBox::currentTextChanged, this, [=](QString text){
        if (text == "NotShow")
        {
            configTool->Undefined = "NotShow";
        }
        else if (text == "Show")
        {
            configTool->Undefined = "Show";
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
    // 更新 列表
    ui->listWidget->clear();
    ui->listWidget->addItems(configTool->NotShow.value.split(":", QString::SkipEmptyParts));
    // 更新 取词模式
    if (configTool->GetMode() == Mode_ALL)
        ui->comboBox_mode->setCurrentIndex(0);
    else if (configTool->GetMode() == Mode_CUSTOM)
        ui->comboBox_mode->setCurrentIndex(1);
    else if (configTool->GetMode() == Mode_NONE)
        ui->comboBox_mode->setCurrentIndex(2);
    // 更新 未定义
    if (configTool->Undefined == "Show")
        ui->comboBox_undefined->setCurrentIndex(0);
    else if (configTool->Undefined == "NotShow")
        ui->comboBox_undefined->setCurrentIndex(1);
}

bool ConfigWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
        this->hide();
    }
    return QWidget::event(e);
}
