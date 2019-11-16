#include <QApplication>
#include <QDebug>
#include "shortcut.h"
#include "configtool.h"
#include "xdotool.h"
#include <algorithm>

extern QString OCRTranslate();

ShortCut::ShortCut()
{
    // keyCodes是双向队列，记录最近按下的3个键
    keyCodes.push_back(0);
    keyCodes.push_back(0);
    keyCodes.push_back(0);
    // 获取 keyName -> keyCode 映射
    auto keyMap = xdotool.getKeyMap();
    // 加载 FloatBarShortCut
    auto shortcut = configTool.SearchBarShortCutString.split("+", QString::SkipEmptyParts);
    for (auto &it : shortcut)
    {
        if (keyMap.find(it) != keyMap.end())
            SearchBarShortCut.push_back(keyMap.at(it));
        else
        {
            qDebug() << "Unknow keyName: " + it;
            qApp->quit();
        }
    }
    // 加载 ScreenShotShortCut
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
    if (std::equal(SearchBarShortCut.cbegin(), SearchBarShortCut.cend(),
                   keyCodes.cbegin() + 3 - int(SearchBarShortCut.size())))
    {
        qDebug() << "SearchBarShortCut";
        emit SearchBarShortCutPressed();
    }

    if (std::equal(ScreenShotShortCut.cbegin(), ScreenShotShortCut.cend(),
                   keyCodes.cbegin() + 3 - int(ScreenShotShortCut.size())))
    {
        qDebug() << "ScreenShotShortCut";
        emit OCRShortCutPressed();

    }
}
