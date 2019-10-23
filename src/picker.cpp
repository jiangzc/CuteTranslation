#include <QApplication>
#include <QDebug>
#include "picker.h"
#include "xdotool.h"


Picker::Picker(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::selectionChanged, this, [=] {
        // do not emit the signal right now, for browser
        text = clipboard->text(QClipboard::Selection);
        CurrentWindowsPath = xdotool.getProcessPathByPID(xdotool.getActiveWindowPID());
        // qDebug() << CurrentWindowsPath;
        if (!isPressed && text != "")
            emit wordsPicked(text);
    });
}

void Picker::buttonPressed()
{
    this->text = "";
    this->isPressed = true;
}

void Picker::buttonReleased()
{
    this->isPressed = false;
    // to get the selected text
    if (text != "")
    {
        // qDebug() << "Text from picker" << text;
        emit wordsPicked(text);
    }
}

QString Picker::getSelectedText()
{
    return clipboard->text(QClipboard::Selection);
    //    return "Picker";
}

Picker *picker;
