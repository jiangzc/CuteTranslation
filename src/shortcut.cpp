#include <QApplication>
#include <QDebug>
#include "shortcut.h"
#include "configtool.h"
#include "xdotool.h"
#include <algorithm>

ShortCut::ShortCut()
{
    keyCodes.push_back(0);
    keyCodes.push_back(0);
    keyCodes.push_back(0);

    auto keyMap = xdotool.getKeyMap();

    auto shortcut = configTool.FloatBarShortCutString.split("+", QString::SkipEmptyParts);
    for (auto &it : shortcut)
    {
        if (keyMap.find(it) != keyMap.end())
            FloatBarShortCut.push_back(keyMap.at(it));
        else
        {
            qDebug() << "Unknow keyName: " + it;
            qApp->quit();
        }
    }

    shortcut = configTool.ScreenShotShortCutString.split("+", QString::SkipEmptyParts);
    for (auto &it : shortcut)
    {
        if (keyMap.find(it) != keyMap.end())
            ScreenShotShortCut.push_back(keyMap.at(it));
        else
        {
            qDebug() << "Unknow keyName: " + it;
            qApp->quit();
        }
    }
}

void ShortCut::onKeyPressed(int keyCode)
{
    keyCodes.pop_front();
    keyCodes.push_back(keyCode);
    if (std::equal(FloatBarShortCut.cbegin(), FloatBarShortCut.cend(),
                   keyCodes.cbegin() + 3 - int(FloatBarShortCut.size())))
    {
        qDebug() << "FloatBarShortCut";
    }

    if (std::equal(ScreenShotShortCut.cbegin(), ScreenShotShortCut.cend(),
                   keyCodes.cbegin() + 3 - int(ScreenShotShortCut.size())))
    {
        qDebug() << "ScreenShotShortCut";
    }
}
