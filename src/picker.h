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

#ifndef PICKER_H
#define PICKER_H

#include <QClipboard>
#include <QObject>
#include <QString>



class Picker : public QObject
{
    Q_OBJECT
  public:
    explicit Picker(QObject *parent = nullptr);
    QString CurrentWindowsPath;
    void buttonPressed();
    void buttonReleased();
    QString getSelectedText();

  private:
    QClipboard *clipboard;
    QString text;

  signals:
    void wordsPicked(QString);

  public slots:
};
extern Picker *picker;
#endif // PICKER_H
