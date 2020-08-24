#include <QDebug>
#include <QCoreApplication>
#include "configtool.h"

QSettings *settings;
QDir dataDir;
QDir appDir;
ConfigTool *configTool;

ConfigTool::ConfigTool()
{

    settings = new QSettings(dataDir.filePath("config.ini"), QSettings::IniFormat);
    Mode = GetMode(settings->value("/Picker/Mode", "all").toString().toLower());
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

    OCRTranslateShortCutString = settings->value("/ShortCut/OCRTranslate", "alt+q").toString().toLower();
    SearchBarShortCutString = settings->value("/ShortCut/SearchBar", "alt+w").toString().toLower();
    OCRTextShortCutString = settings->value("/ShortCut/OCRText", "alt+e").toString().toLower();

    ChineseNotShow = settings->value("/Custom/ChineseNotShow", true).toBool();
    MainWindowPin = settings->value("/MainWindow/IsPinning", false).toBool();

    WebPageZoomFactor = settings->value("/MainWindow/WebPageZoomFactor", 1.0).toFloat();
    TextTimeout = settings->value("/Translate/TextTimeout", 2.0).toFloat();
    OCRTimeout = settings->value("/Translate/OCRTimeout", 3.0).toFloat();

    // top-level key -> /General/Version
    QString version = settings->value("Version", "0.0.0").toString();
    TokenURL = settings->value("TokenURL", "http://67.216.199.87:5000/token").toString();
    if (version != qApp->applicationVersion())
    {
        if (version.startsWith("0.2") || version.startsWith("0.3") || version.startsWith("0.4"))
            qInfo() << "程序和配置文件版本不匹配，\n程序版本：" << qApp->applicationVersion() << "，配置文件版本：" << version
                    << "\n  配置文件： ~/.local/share/CuteTranslation";
        else
            qWarning() << "程序和配置文件版本不匹配，\n程序版本：" << qApp->applicationVersion() << "，配置文件版本：" << version
                    << "\n  配置文件： ~/.local/share/CuteTranslation";
    }

}

ModeSet ConfigTool::GetMode() const
{
    return this->Mode;
}

ModeSet ConfigTool::GetMode(QString modeStr) const
{
    if (modeStr == "all")
        return Mode_ALL;
    else if (modeStr == "custom")
        return Mode_CUSTOM;
    else if (modeStr == "none")
        return Mode_NONE;
    return Mode_CUSTOM;
}

void ConfigTool::SetMode(ModeSet mode)
{
    this->Mode = mode;
    if (mode == Mode_ALL)
    {
        settings->setValue("/Picker/Mode", "all");
    }
    else if(mode == Mode_CUSTOM)
    {
        settings->setValue("/Picker/Mode", "custom");
    }
    else if(mode == Mode_NONE)
    {
        settings->setValue("/Picker/Mode", "none");
    }
    emit ModeChanged(mode);
}

bool ConfigTool::GetMainWindowPin() const
{
    return this->MainWindowPin;
}

void ConfigTool::SetMainWindowPin(bool pin)
{
    this->MainWindowPin = pin;
    settings->setValue("/MainWindow/IsPinning", pin);
}

int ConfigTool::GetMainWindowWidth() const
{
    return this->MainWindowWidth;
}

void ConfigTool::SetMainWindowWidth(int width)
{
    this->MainWindowWidth = width;
    settings->setValue("/MainWindow/MainWindowWidth", width);
}

float ConfigTool::GetWebPageZoomFactor() const
{
    return this->WebPageZoomFactor;
}

void ConfigTool::SetWebPageZoomFactor(float zoom)
{
    this->WebPageZoomFactor = zoom;
    settings->setValue("/MainWindow/WebPageZoomFactor", QString::number(double(zoom)));
}
