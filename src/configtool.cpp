#include "configtool.h"


ConfigTool::ConfigTool() : settings("/opt/CuteTranslation/config.ini", QSettings::IniFormat)
{
    Mode = settings.value("/Genenal/Mode", "all").toString();
    TriangleWidth = settings.value("/Genenal/TriangleWidth", 15).toInt();
    TriangleHeight = settings.value("/Genenal/TriangleHeight", 15).toInt();
    Edge = settings.value("/Genenal/Edge", 15).toInt();
    Direction = (settings.value("/Genenal/Direction").toString() == "up" ? 0 : 1);
    MainWindowWidth = settings.value("/Genenal/MainWindowWidth").toInt();
    MainWindowHeight = settings.value("/Genenal/MainWindowHeight").toInt();
    FloatButtonWidth = settings.value("/Genenal/FloatButtonWidth").toInt();
    FloatButtonHeight = settings.value("/Genenal/FloatButtonHeight").toInt();

    Default = settings.value("/Custom/Default").toString();
    Show = settings.value("/Custom/Show").toString();
    NotShow = settings.value("/Custom/NotShow").toString();
}

