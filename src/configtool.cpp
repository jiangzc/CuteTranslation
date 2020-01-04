#include <QDebug>
#include <QDir>
#include "configtool.h"

QSettings *settings;

ConfigTool::ConfigTool()
{
    settings = new QSettings(QDir::homePath() + "/.config/CuteTranslation/config.ini", QSettings::IniFormat);

    QString mode = settings->value("/Picker/Mode", "all").toString().toLower();
    if (mode == "all")
        Mode2 = Mode_ALL;
    else if (mode == "custom")
        Mode2 = Mode_CUSTOM;
    else if (mode == "none")
        Mode2 = Mode_NONE;


    Undefined.value = settings->value("/Custom/Undefined").toString();

    TriangleWidth = settings->value("/MainWindow/TriangleWidth", 15).toInt();
    TriangleHeight = settings->value("/MainWindow/TriangleHeight", 15).toInt();
    Edge = settings->value("/MainWindow/Edge", 15).toInt();
    Direction = (settings->value("/MainWindow/Direction", "up").toString() == "up" ? 0 : 1);
    MainWindowWidth = settings->value("/MainWindow/MainWindowWidth", 500).toInt();
    MainWindowHeight = settings->value("/MainWindow/MainWindowHeight", 400).toInt();
    FloatButtonWidth = settings->value("/FloatButton/FloatButtonWidth", 40).toInt();
    FloatButtonHeight = settings->value("/FloatButton/FloatButtonHeight", 40).toInt();

    Show = settings->value("/Custom/Show").toString();
    NotShow = settings->value("/Custom/NotShow").toString();

    SearchBarShortCutString = settings->value("/ShortCut/SearchBar", "ctrl+q").toString().toLower();
    ScreenShotShortCutString = settings->value("/ShortCut/ScreenShot", "alt+q").toString().toLower();

    ChineseNotShow = settings->value("/Custom/ChineseNotShow", true).toBool();
    MainWindowIsPinning = settings->value("/MainWindow/IsPinning", false).toBool();

    // top-level key -> /General/Version
    QString version = settings->value("Version", "0.0.0").toString();
    if (version != CUTETRANSLATION_VERSION)
    {
        qWarning() << "程序和配置文件版本不匹配，程序版本：" << CUTETRANSLATION_VERSION << "，配置文件版本：" << version;
    }
}
