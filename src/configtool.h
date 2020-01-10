#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QString>
#include <QObject>

/* ConfigTool 中使用了匿名类来实现C#-style类的属性的功能
 * 重载了匿名类的赋值运算符作为set函数
 * 设置了匿名类的类型转换 class -> type 作为get函数
 * 具体参考 https://stackoverflow.com/questions/8368512/does-c11-have-c-style-properties
 */

extern QSettings *settings;
extern const QString CUTETRANSLATION_VERSION;
enum ModeSet { Mode_ALL, Mode_CUSTOM, Mode_NONE };


class ConfigTool : public QObject
{
    Q_OBJECT

private:
    ModeSet Mode;
    bool MainWindowPin;
public:
    ConfigTool();
    int TriangleHeight;
    int TriangleWidth;
    int Edge;
    int Direction;
    int MainWindowWidth;
    int MainWindowHeight;
    int FloatButtonWidth;
    int FloatButtonHeight;
    QString SearchBarShortCutString;
    QString ScreenShotShortCutString;
    bool ChineseNotShow;
    float WebPageZoomFactor;


    // QString Mode ;
    ModeSet GetMode() const;
    ModeSet GetMode(QString modeStr) const;
    void SetMode(ModeSet mode);

    // bool MainWindowPin;
    bool GetMainWindowPin() const;
    void SetMainWindowPin(bool);


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
