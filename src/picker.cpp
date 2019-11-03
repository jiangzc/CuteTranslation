/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) Ziqiang Xu
 *
 * Author:     Ziqiang Xu <ziqiang_xu@yeah.net>
 * Maintainer: Ziqiang Xu <ziqiang_xu@yeah.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
