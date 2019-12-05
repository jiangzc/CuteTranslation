#include <QDebug>
#include <QDir>
#include "configtool.h"


ConfigTool::ConfigTool()
{
    Mode.value = settings.value("/Picker/Mode", "all").toString();
    Undefined.value = settings.value("/Custom/Undefined").toString();

    TriangleWidth = settings.value("/MainWindow/TriangleWidth", 15).toInt();
    TriangleHeight = settings.value("/MainWindow/TriangleHeight", 15).toInt();
    Edge = settings.value("/MainWindow/Edge", 15).toInt();
    Direction = (settings.value("/MainWindow/Direction", "up").toString() == "up" ? 0 : 1);
    MainWindowWidth = settings.value("/MainWindow/MainWindowWidth", 500).toInt();
    MainWindowHeight = settings.value("/MainWindow/MainWindowHeight", 400).toInt();
    FloatButtonWidth = settings.value("/FloatButton/FloatButtonWidth", 40).toInt();
    FloatButtonHeight = settings.value("/FloatButton/FloatButtonHeight", 40).toInt();

    Show = settings.value("/Custom/Show").toString();
    NotShow = settings.value("/Custom/NotShow").toString();

    SearchBarShortCutString = settings.value("/ShortCut/SearchBar", "ctrl+q").toString().toLower();
    ScreenShotShortCutString = settings.value("/ShortCut/ScreenShot", "alt+q").toString().toLower();

    ChineseNotShow = settings.value("/Custom/ChineseNotShow", true).toBool();
    MainWindowIsPinning = settings.value("/MainWindow/IsPinning", false).toBool();

    QString version = settings.value("/Genenal/Version", "0.0.0").toString();
    if (version != "0.1.0")
    {
        qDebug() << "警告：程序和配置文件版本不匹配，程序版本：0.1.0，配置文件版本：" << version;
    }
}


QSettings ConfigTool::settings(QDir::homePath() + "/.config/CuteTranslation/config.ini", QSettings::IniFormat);
