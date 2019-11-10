#include <QApplication>
#include <QDebug>
#include "picker.h"
#include "xdotool.h"


Picker::Picker(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();

    connect(clipboard, &QClipboard::selectionChanged, this, [=] {

        text = clipboard->text(QClipboard::Selection);

        // 在浏览器上选中文字会收到大量selectionChanged信号，并且收到的text==""。
        // 如果text==""，可以认为当前用户操作的是浏览器
        // 为了避免处理大量信号导致floatBtn.show() 延时，此处屏蔽clipboard所有信号，在picker.buttonReleased()中恢复
        if (text == "")
        {
            clipboard->blockSignals(true);
        }
        else
        {
            CurrentWindowsPath = xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
            CurrentWindowsPath = CurrentWindowsPath.mid(1 + CurrentWindowsPath.lastIndexOf("/"));
            emit wordsPicked(text);
        }

    });
}

void Picker::buttonPressed()
{

}

void Picker::buttonReleased()
{

    if (clipboard->signalsBlocked())
    {
        text = clipboard->text(QClipboard::Selection);
        clipboard->blockSignals(false);

        CurrentWindowsPath = xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
        CurrentWindowsPath = CurrentWindowsPath.mid(1 + CurrentWindowsPath.lastIndexOf("/"));
        emit wordsPicked(text);

    }
}

QString Picker::getSelectedText()
{
    return clipboard->text(QClipboard::Selection);
    //    return "Picker";
}

Picker *picker;
