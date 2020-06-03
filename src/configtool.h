#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QString>
#include <QObject>
#include <QDir>

/* ConfigTool 中使用了匿名类来实现C#-style类的属性的功能
 * 重载了匿名类的赋值运算符作为set函数
 * 设置了匿名类的类型转换 class -> type 作为get函数
 * 具体参考 https://stackoverflow.com/questions/8368512/does-c11-have-c-style-properties
 */

extern QSettings *settings;
enum ModeSet { Mode_ALL, Mode_CUSTOM, Mode_NONE };
extern QDir dataDir;
extern QDir appDir;

class ConfigTool : public QObject
{
    Q_OBJECT

private:
    ModeSet Mode;
    bool MainWindowPin;
    int MainWindowWidth;
    float WebPageZoomFactor;
public:
    ConfigTool();
    int TriangleHeight;
    int TriangleWidth;
    int Edge;
    int Direction;
    int MainWindowHeight; // 过时
    int FloatButtonWidth;
    int FloatButtonHeight;
    QString SearchBarShortCutString;
    QString OCRTranslateShortCutString;
    QString OCRTextShortCutString;
    bool ChineseNotShow;
    float TextTimeout;
    float OCRTimeout;
    QString TokenURL;


    // QString Mode ;
    ModeSet GetMode() const;
    ModeSet GetMode(QString modeStr) const;
    void SetMode(ModeSet mode);

    // bool MainWindowPin;
    bool GetMainWindowPin() const;
    void SetMainWindowPin(bool);

    int GetMainWindowWidth() const;
    void SetMainWindowWidth(int width);

    float GetWebPageZoomFactor() const;
    void SetWebPageZoomFactor(float zoom);


    // QString Undefined;
    class
    {

    public:
        QString value;
        QString &operator=(const QString &i)
        {
            settings->setValue("/Custom/Undefined", i);
            value = i;
            return value;
        }
        bool operator==(const QString str)
        {
            return (str == value);
        }
        operator QString() const
        {
            return value;
        }

    } Undefined;

    // QString NotShow;
    class
    {

    public:
        QString value;
        QString &operator=(const QString &i)
        {
            settings->setValue("/Custom/NotShow", i);
            value = i;
            return value;
        }
        bool operator==(const QString &str)
        {
            return (str == value);
        }
        QString& operator+=(const QString &str)
        {
            value += str;
            settings->setValue("/Custom/NotShow", value);
            return value;
        }
        QString& operator-=(const QString &str)
        {
            value.remove(":" + str);
            settings->setValue("/Custom/NotShow", value);
            return value;
        }
        operator QString() const
        {
            return value;
        }

    } NotShow;

    // QString Show;
    class
    {

    public:
        QString value;
        QString &operator=(const QString &i)
        {
            settings->setValue("/Custom/Show", i);
            value = i;
            return value;
        }
        bool operator==(const QString str)
        {
            return (str == value);
        }
        QString& operator+=(const QString &str)
        {
            value += str;
            settings->setValue("/Custom/Show", value);
            return value;
        }
        operator QString() const
        {
            return value;
        }

    } Show;


signals:
    void ModeChanged(ModeSet mode);

};


extern ConfigTool *configTool;

#endif // CONFIG_H
