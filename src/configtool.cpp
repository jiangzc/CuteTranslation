#include <QDebug>
#include <QDir>
#include "configtool.h"
#include "xdotool.h"


ConfigTool::ConfigTool()
{
    Mode.value = settings.value("/Genenal/Mode", "all").toString();
    Undefined.value = settings.value("/Custom/Undefined").toString();

    TriangleWidth = settings.value("/Genenal/TriangleWidth", 15).toInt();
    TriangleHeight = settings.value("/Genenal/TriangleHeight", 15).toInt();
    Edge = settings.value("/Genenal/Edge", 15).toInt();
    Direction = (settings.value("/Genenal/Direction", "up").toString() == "up" ? 0 : 1);
    MainWindowWidth = settings.value("/Genenal/MainWindowWidth", 500).toInt();
    MainWindowHeight = settings.value("/Genenal/MainWindowHeight", 400).toInt();
    FloatButtonWidth = settings.value("/Genenal/FloatButtonWidth", 40).toInt();
    FloatButtonHeight = settings.value("/Genenal/FloatButtonHeight", 40).toInt();

    Show = settings.value("/Custom/Show").toString();
    NotShow = settings.value("/Custom/NotShow").toString();

    FloatBarShortCutString = settings.value("/ShortCut/FloatBar").toString();


}


QSettings ConfigTool::settings(QDir::homePath() + "/CuteTranslation/config.ini", QSettings::IniFormat);
