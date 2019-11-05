#include <QDebug>
#include "shortcut.h"
#include "configtool.h"
#include <algorithm>

ShortCut::ShortCut()
{
    keyCodes.push_back(0);
    keyCodes.push_back(0);
    keyCodes.push_back(0);
}

void ShortCut::onKeyPressed(int keyCode)
{
    keyCodes.pop_front();
    keyCodes.push_back(keyCode);
    if (std::equal(configTool.FloatBarShortCut.cbegin(), configTool.FloatBarShortCut.cend(),
                   keyCodes.cbegin() + 3 - int(configTool.FloatBarShortCut.size())))
    {
        qDebug() << "sho";
    }
}
