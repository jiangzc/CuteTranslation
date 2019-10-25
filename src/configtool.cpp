#include "configtool.h"
#include <QDebug>


ConfigTool::ConfigTool() : settings("/home/jzc/CuteTranslation/config.ini", QSettings::IniFormat)
{
    qDebug() << settings.isWritable();
    Mode = settings.value("/Genenal/Mode", "all").toString();
    TriangleWidth = settings.value("/Genenal/TriangleWidth", 15).toInt();
    TriangleHeight = settings.value("/Genenal/TriangleHeight", 15).toInt();
    Edge = settings.value("/Genenal/Edge", 15).toInt();
    Direction = (settings.value("/Genenal/Direction", "up").toString() == "up" ? 0 : 1);
    MainWindowWidth = settings.value("/Genenal/MainWindowWidth", 400).toInt();
    MainWindowHeight = settings.value("/Genenal/MainWindowHeight", 300).toInt();
    FloatButtonWidth = settings.value("/Genenal/FloatButtonWidth", 40).toInt();
    FloatButtonHeight = settings.value("/Genenal/FloatButtonHeight", 40).toInt();

    Undefined = settings.value("/Custom/Undefined").toString();
    Show = settings.value("/Custom/Show").toString();
    NotShow = settings.value("/Custom/NotShow").toString();


}

