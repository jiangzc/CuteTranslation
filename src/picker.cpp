#include <QApplication>
#include <QDebug>
#include "picker.h"
#include "xdotool.h"


Picker::Picker(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::selectionChanged, this, [=] {
        //
        text = clipboard->text(QClipboard::Selection);

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
        qDebug() << "signalsBlocked";
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
