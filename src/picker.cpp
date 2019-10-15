#include "picker.h"
#include <QApplication>
#include <QDebug>

Picker::Picker(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::selectionChanged, this, [=]
    {
        // do not emit the signal right now, for browser
        text = clipboard->text(QClipboard::Selection);
        if (!isPressed)
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
    qDebug() << "Text from picker" << text;
    if (text != "") emit wordsPicked(text);
}

QString Picker::getSelectedText()
{
    return clipboard->text(QClipboard::Selection);
//    return "Picker";
}
