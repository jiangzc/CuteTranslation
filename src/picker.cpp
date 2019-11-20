#include <QApplication>
#include <QDebug>
#include "picker.h"
#include "xdotool.h"


Picker::Picker(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    this->isPressed = false;
    lastTime = QTime::currentTime();
    connect(clipboard, &QClipboard::selectionChanged, this, [=] {

        // 在浏览器和Calibre上选中文字会收到大量selectionChanged信号。
        // 如果selectionChanged信号之间的时间间隔很短，可以认为当前用户操作的是浏览器
        // 为了避免处理大量信号导致floatBtn.show() 延时，此处屏蔽clipboard所有信号，在picker.buttonReleased()中恢复
        int elapsed = lastTime.msecsTo(QTime::currentTime());
        lastTime = QTime::currentTime();
        if (elapsed < 200) // less than 200ms
        {
            clipboard->blockSignals(true);
            return;
        }

        text = clipboard->text(QClipboard::Selection);
        CurrentWindowsPath = xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
        CurrentWindowsPath = CurrentWindowsPath.mid(1 + CurrentWindowsPath.lastIndexOf("/"));
        // 关闭窗口时 text == ""
        if (!isPressed && text != "")
        {
            emit wordsPicked(text);
        }
    });
}

void Picker::buttonPressed()
{
    text = "";
     this->isPressed = true;
}

void Picker::buttonReleased()
{
    this->isPressed = false;
    if (clipboard->signalsBlocked())
    {
        text = clipboard->text(QClipboard::Selection);
        clipboard->blockSignals(false); // 恢复clipboard信号

        CurrentWindowsPath = xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
        CurrentWindowsPath = CurrentWindowsPath.mid(1 + CurrentWindowsPath.lastIndexOf("/"));

    }
    if (text != "")
        emit wordsPicked(text);
}

QString Picker::getSelectedText()
{
    return clipboard->text(QClipboard::Selection);
    //    return "Picker";
}

Picker *picker;
